#include "LineShapeTool/Constants.h"
#include "LineShapeTool/LineShape.h"
using namespace std;
using namespace constants;
 
LineShape::LineShape() {

  m_nBoson=0;
  m_boson.clear();
  m_charge = 0.;
  m_M.clear();
  m_M2.clear();
  m_G.clear();
  m_G2.clear();
  m_cpl.clear();
  m_run.clear();
  m_ckmIn = 0.;
  m_ckmOut = 0.;
  m_vIn.clear();
  m_vOut.clear();
  m_aIn.clear();
  m_aOut.clear();

  m_useInterference = 1;
  m_useRunningWidth = true;
  m_useRunningCoupling = true;

}

LineShape::~LineShape()
{}


// Add a boson to the "tower"

void LineShape::addBoson(GaugeBoson* b) {

  if(m_nBoson == 0)

    m_charge = b->getCharge();

  else if(b->getCharge() != m_charge) {

    cout << " Charge of boson " << m_nBoson
	 << " does not match charge of initial boson" << endl;
    cout << " Initial : " << m_boson[0]->getName()
	 << " , charge = " << m_charge << endl; 
    cout << " Current : " << b->getName() 
	 << " , charge = " << b->getCharge() << endl; 
    return;

  }

  m_nBoson++;
  m_boson.push_back( b );

  // ... the scaling below is needed because the cross section formulas in this code are written in terms of constant width:
  
  double wfac = 1;
  if( m_useRunningWidth && b->getMass()>0. )
    wfac = 1./sqrt( 1. + b->getWidth()*b->getWidth()/b->getMass()/b->getMass() );
  
  m_M.push_back( b->getMass()*wfac );
  m_M2.push_back( b->getMass()*b->getMass()*wfac*wfac );
  m_G.push_back( b->getWidth()*wfac );
  m_G2.push_back( b->getWidth()*b->getWidth()*wfac*wfac );

  // ... couplings

  m_cpl.push_back( b->getReferenceCoupling() );
  m_run.push_back( b->getRunningCoupling() );

  // ... CKM

  m_vIn.resize(m_nBoson);
  m_vOut.resize(m_nBoson);
  m_aIn.resize(m_nBoson);
  m_aOut.resize(m_nBoson);

}


GaugeBoson* LineShape::getBoson(int i) {

  if( (unsigned int)i>=m_boson.size() )
    return 0;

  return m_boson[i];

}


void LineShape::useInterference(int t) {

  m_useInterference = t;

}


void LineShape::useRunningWidth(bool t) {

  m_useRunningWidth = t;

}


void LineShape::useRunningCoupling(bool t) {

  m_useRunningCoupling = t;

}


// set couplings for incoming particles

void LineShape::setIn(int i1, int i2)
{
  int ai1 = i1; if(ai1<0) ai1 = -ai1;
  int ai2 = i2; if(ai2<0) ai2 = -ai2;

  if(m_charge == 0)

    m_ckmIn = Vnc(ai1,ai2)*Vnc(ai1,ai2);

  else if(fabs(m_charge) == 1)

    m_ckmIn = Vcc(ai1,ai2)*Vcc(ai1,ai2);
  
  if(Nc(ai1)==Nc(ai2))

    m_ncIn = 1./Nc(ai1);

  for(int i = 0; i<m_nBoson; i++) {

    m_vIn[i] = m_boson[i]->getGV(ai1,ai2);
    m_aIn[i] = m_boson[i]->getGA(ai1,ai2); 

  }

}


// set couplings for outgoing particles

void LineShape::setOut(int i1, int i2) {

  int ai1 = i1; if(ai1<0) ai1 = -ai1;
  int ai2 = i2; if(ai2<0) ai2 = -ai2;

  if(m_charge == 0)

    m_ckmOut = Vnc(ai1,ai2)*Vnc(ai1,ai2);

  else if(fabs(m_charge) == 1)

    m_ckmOut = Vcc(ai1,ai2)*Vcc(ai1,ai2);

  if(Nc(ai1)==Nc(ai2))

    m_ncOut = Nc(ai1);

  for(int i = 0; i<m_nBoson; i++) {

    m_vOut[i] = m_boson[i]->getGV(ai1,ai2);
    m_aOut[i] = m_boson[i]->getGA(ai1,ai2); 

  }

}


double LineShape::getCharge() {

  return m_charge;

}


// modify properties of given boson in the tower

void LineShape::setMass(int iBoson, double mass) {

  if( (unsigned int)iBoson>=m_boson.size() )
    return;

  m_boson[iBoson]->setMass( mass );
  double width = m_boson[iBoson]->getWidth();

  double wfac = 1;
  if( m_useRunningWidth && mass>0. )
    wfac = 1./sqrt( 1. + width*width/mass/mass );

  m_M[iBoson] = mass*wfac;
  m_M2[iBoson] = mass*mass*wfac*wfac;

}

void LineShape::setWidth(int iBoson, double width) {

  if( (unsigned int)iBoson>=m_boson.size() )
    return;

  m_boson[iBoson]->setWidth( width );
  double mass = m_boson[iBoson]->getMass();

  double wfac = 1;
  if( m_useRunningWidth && mass>0. )
    wfac = 1./sqrt( 1. + width*width/mass/mass );

  m_G[iBoson] = width*wfac;
  m_G2[iBoson] = width*width*wfac*wfac;

}

void LineShape::setCoupling(int iBoson, double coupling) {

  if( (unsigned int)iBoson>=m_boson.size() )
    return;

  m_cpl[iBoson] = coupling/2.; // beware the factor 1/2, consistent with normalization used in GaugeBoson class

}

double LineShape::getCoupling(int iBoson) {

  if( (unsigned int)iBoson>=m_boson.size() )
    return 0.;

  return m_cpl[iBoson]*2.; // beware the factor 1/2, consistent with normalization used in GaugeBoson class

}



// Various differential cross sections

double LineShape::dSdMdCosthSym(double m, double costh, 
			    int i1, int i2, int o1, int o2) {

  // ... set in/out particles

  setIn(i1,i2);
  setOut(o1,o2);

  // ... couplings and propagators

  double shat = m*m;
  double sumS = 0;

  for(int i = 0; i<m_nBoson; i++)
    for(int j = 0; j<m_nBoson; j++) {

	if( m_useInterference==0 && j!=i ) continue;
	if( m_useInterference==2 && j==i ) continue;
	
	// ... propagator

	double Pij = shat * 
	  ( (shat-m_M2[i])*(shat-m_M2[j]) + m_M[i]*m_M[j]*m_G[i]*m_G[j] ) /
	  ( (shat-m_M2[i])*(shat-m_M2[i]) + m_M2[i]*m_G2[i] ) /
	  ( (shat-m_M2[j])*(shat-m_M2[j]) + m_M2[j]*m_G2[j] );
 
	// ... couplings to leptons and quarks

	double Bij = 
	  (m_vIn[i]*m_vIn[j] + m_aIn[i]*m_aIn[j]) * 
	  (m_vOut[i]*m_vOut[j] + m_aOut[i]*m_aOut[j]);

	// ... product
	
	double xsec = Pij * Bij * m_cpl[i] * m_cpl[j] * (1+costh*costh);
	if( m_useRunningCoupling )
	  xsec *= m_run[i](shat) * m_run[j](shat);

	sumS += xsec;


    }
  
  // ... overall
  
  double norm = 2*Pi() * m_ncIn * m_ncOut * m_ckmIn * m_ckmOut * GeV2nb() ;
  return norm*sumS;

}

double LineShape::dSdMdCosthAsym(double m, double costh,
			     int i1, int i2, int o1, int o2) {

  // ... set in/out particles

  setIn(i1,i2);
  setOut(o1,o2);

  // ... couplings and propagators

  double shat = m*m;
  double sumA = 0;

  for(int i = 0; i<m_nBoson; i++)
    for(int j = 0; j<m_nBoson; j++) {

	if( m_useInterference==0 && j!=i ) continue;
	if( m_useInterference==2 && j==i ) continue;
	
	// ... propagator

	double Pij = shat * 
	  ( (shat-m_M2[i])*(shat-m_M2[j]) + m_M[i]*m_M[j]*m_G[i]*m_G[j] ) /
	  ( (shat-m_M2[i])*(shat-m_M2[i]) + m_M2[i]*m_G2[i]) /
	  ( (shat-m_M2[j])*(shat-m_M2[j]) + m_M2[j]*m_G2[j]);
 
	// ... couplings to leptons and quarks

	double Cij = 2 *
	  (m_vIn[i]*m_aIn[j] + m_aIn[i]*m_vIn[j]) * 
	  (m_vOut[i]*m_aOut[j] + m_aOut[i]*m_vOut[j]);

	// ... product

	double xsec = Pij * Cij * m_cpl[i] * m_cpl[j] * costh;
	if( m_useRunningCoupling )
	  xsec *= m_run[i](shat) * m_run[j](shat);

	sumA += xsec;

    }

  // ... overall sum

  double norm = 2*Pi() * m_ncIn * m_ncOut * m_ckmIn * m_ckmOut * GeV2nb() ;
  return sumA * norm;

}


double LineShape::dSdM(double m, int i1, int i2, int o1, int o2) {

  // ... set in/out particles

  setIn(i1,i2);
  setOut(o1,o2);

  // ... couplings and propagators

  double shat = m*m;
  double sumS = 0;
  double Pij, Bij;

  for(int i = 0; i<m_nBoson; i++)
    for(int j = 0; j<m_nBoson; j++) {

	if( m_useInterference==0 && j!=i ) continue;
	if( m_useInterference==2 && j==i ) continue;

	// ... propagator

	Pij = shat * 
	  ( (shat-m_M2[i])*(shat-m_M2[j]) + m_M[i]*m_M[j]*m_G[i]*m_G[j] ) /
	  ( (shat-m_M2[i])*(shat-m_M2[i]) + m_M2[i]*m_G2[i]) /
	  ( (shat-m_M2[j])*(shat-m_M2[j]) + m_M2[j]*m_G2[j]);

	// ... couplings to leptons and quarks

	Bij =
	  (m_vIn[i]*m_vIn[j] + m_aIn[i]*m_aIn[j]) * 
	  (m_vOut[i]*m_vOut[j] + m_aOut[i]*m_aOut[j]);

	// ... product
	
	double xsec = Pij * Bij * m_cpl[i] * m_cpl[j];
	if( m_useRunningCoupling )
	  xsec *= m_run[i](shat) * m_run[j](shat);

	sumS += xsec;

      }


  double norm = 2*Pi() * 8./3. * m_ncIn * m_ncOut * m_ckmIn * m_ckmOut * GeV2nb() ;
  return sumS * norm;
}


void LineShape::print() {

  cout << endl;
  cout << "LineShape properties : " << endl;
  cout << "Nb of bosons : " << m_nBoson << endl;

  for(int i = 0; i<m_nBoson; i++) {

    cout << "  " << i << "  - mass / width : " << m_M[i] << " / " << m_G[i] << endl;
    //     cout << "       gV(n) / gA(n) : " << m_Vn[i] << " / " << m_An[i] << endl;
    //     cout << "       gV(e) / gA(e) : " << m_Ve[i] << " / " << m_Ae[i] << endl;
    //     cout << "       gV(u) / gA(u) : " << m_Vu[i] << " / " << m_Au[i] << endl;
    //     cout << "       gV(d) / gA(d) : " << m_Vd[i] << " / " << m_Ad[i] << endl;

  }

}
