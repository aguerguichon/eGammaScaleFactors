#include "LineShapeTool/Constants.h"
#include "LineShapeTool/GaugeBoson.h"

using namespace constants;
using namespace std;

GaugeBoson::GaugeBoson() {

  m_name = "none";
  m_mass = 0.;
  m_width = 0.;
  m_refCoupling = 0.;
  m_runCoupling = NULL;

  for(int i=0; i<NMAX; ++i)
    for(int j=0; j<NMAX; ++j) {

	m_gV[i][j] = 0.;
	m_gA[i][j] = 0.;

    }

}

GaugeBoson::~GaugeBoson()
{}

// charge

void GaugeBoson::setNeutral() { 

  m_charge = 0; 

}


void GaugeBoson::setChargedPlus() { 

  m_charge = 1; 

}


void GaugeBoson::setChargedMinus() { 

  m_charge = -1; 

}


int GaugeBoson::getCharge() { 

  return m_charge; 

}


bool GaugeBoson::isNeutral() { 

  if(m_charge == 0) 
    return true; 

  return false;

}


bool GaugeBoson::isCharged() { 

  if(m_charge != 0) 
    return true; 

  return false;

}


bool GaugeBoson::isChargedPlus() { 

  if(m_charge == 1) 
    return true; 

  return false;

}


bool GaugeBoson::isChargedMinus() { 

  if(m_charge == -1) 
    return true; 

  return false;

}

// couplings

void GaugeBoson::setGV(int i1, int i2, double gv) {

  if(i1<NMAX && i2<NMAX)
    m_gV[i1][i2] = gv;

}
 

void GaugeBoson::setGA(int i1, int i2, double ga) {

  if(i1<NMAX && i2<NMAX)
    m_gA[i1][i2] = ga;

}
 

double GaugeBoson::getGV(int i1, int i2) {

  if(i1<NMAX && i2<NMAX)
    return m_gV[i1][i2];

  return 0.;

}


double GaugeBoson::getGA(int i1, int i2) {

  if(i1<NMAX && i2<NMAX)
    return m_gA[i1][i2];

  return 0.;

}


// specific gauge bosons

void GaugeBoson::setPhoton()
{ 

  m_name = "Photon";

  m_refCoupling = Aqed0()/2.;
  m_runCoupling = &Rqed;

  m_mass = Mg();
  m_width = Gg();
  m_charge = 0;

  for(int i=0; i<NMAX; ++i)
    m_gV[i][i] = Q(i);

} 

void GaugeBoson::setWplus()
{ 

  m_name = "W+";

  m_refCoupling = Gf()*Mw()*Mw()/(2*sqrt(2)*Pi());
  m_runCoupling = &Rweak;

  m_mass = Mw();
  m_width = Gw();
  m_charge = 1;
  
  for(int i=0; i<NMAX; ++i)
    for(int j=0; j<NMAX; ++j)
      if( (isLepton(i) && isLepton(j)) || 
	  (isQuark(i) && isQuark(j))) {
	
	m_gV[i][j] = gVcc(i,j);
	m_gA[i][j] = gAcc(i,j);
	
      }

}


void GaugeBoson::setWminus() { 

  m_name = "W-";

  m_refCoupling = Gf()*Mw()*Mw()/(2*sqrt(2)*Pi());
  m_runCoupling = &Rweak;

  m_mass = Mw();
  m_width = Gw();
  m_charge = -1;
  
  for(int i=0; i<NMAX; ++i)
    for(int j=0; j<NMAX; ++j)
      if( (isLepton(i) && isLepton(j)) || (isQuark(i) && isQuark(j))) {

	m_gV[i][j] = gVcc(i,j);
	m_gA[i][j] = gAcc(i,j);

      }
  
}

void GaugeBoson::setZ0() { 

  m_name = "Z0";

  m_refCoupling = Gf()*Mz()*Mz()/(4*sqrt(2)*Pi());
  m_runCoupling = &Rweak;

  m_mass = Mz();
  m_width = Gz();
  m_charge = 0;

  for(int i=0; i<NMAX; ++i) {

    m_gV[i][i] = gVnc(i,i);
    m_gA[i][i] = gAnc(i,i);

  }

}

void GaugeBoson::setZpSSM(double m) { 

  m_name = "Z' (SSM)";

  m_refCoupling = Gf()*Mz()*Mz()/(4*sqrt(2)*Pi());
  m_runCoupling = &Rweak;

  m_mass = m;
  m_width = m*Gz()/Mz();
  m_charge = 0;

  for(int i=0; i<NMAX; ++i) {

    m_gV[i][i] = gVnc(i,i);
    m_gA[i][i] = gAnc(i,i);

  }

}


void GaugeBoson::setWplusSSM(double m) { 

  m_name = "W'+ (SSM)";

  m_refCoupling = Gf()*Mw()*Mw()/(2*sqrt(2)*Pi());
  m_runCoupling = &Rweak;

  m_mass = m;
  m_width = m*Gw()/Mw();
  m_charge = +1;

  for(int i=0; i<NMAX; ++i)
    for(int j=0; j<NMAX; ++j)
      if( (isLepton(i) && isLepton(j)) || (isQuark(i) && isQuark(j))) {

	m_gV[i][j] = gVcc(i,j);
	m_gA[i][j] = gAcc(i,j);
	
      }

}


void GaugeBoson::setWplusRight(double m) { 
  
  m_name = "W'+ (Right)";

  m_refCoupling = Gf()*Mw()*Mw()/(2*sqrt(2)*Pi());
  m_runCoupling = &Rweak;

  m_mass = m;
  m_width = m*Gw()/Mw();
  m_charge = +1;

  for(int i=0; i<NMAX; ++i)
    for(int j=0; j<NMAX; ++j)
      if( (isLepton(i) && isLepton(j)) || (isQuark(i) && isQuark(j))) {

	m_gV[i][j] = gVcc(i,j);
	m_gA[i][j] = -gAcc(i,j);
	
      }

}

void GaugeBoson::setWminusSSM(double m)
{ 
  m_name = "W'- (SSM)";

  m_refCoupling = Gf()*Mw()*Mw()/(2*sqrt(2)*Pi());
  m_runCoupling = &Rweak;

  m_mass = m;
  m_width = m*Gw()/Mw();
  m_charge = -1;

  for(int i=0; i<NMAX; ++i)
    for(int j=0; j<NMAX; ++j) 
      if( (isLepton(i) && isLepton(j)) || (isQuark(i) && isQuark(j))) {

	m_gV[i][j] = gVcc(i,j);
	m_gA[i][j] = gAcc(i,j);
	
      }
}

void GaugeBoson::setWminusRight(double m)
{ 
  m_name = "W'- (Right)";

  m_refCoupling = Gf()*Mw()*Mw()/(2*sqrt(2)*Pi());
  m_runCoupling = &Rweak;

  m_mass = m;
  m_width = m*Gw()/Mw();
  m_charge = -1;

  for(int i=0; i<NMAX; ++i)
    for(int j=0; j<NMAX; ++j)
      if( (isLepton(i) && isLepton(j)) || (isQuark(i) && isQuark(j))) {

	m_gV[i][j] = gVcc(i,j);
	m_gA[i][j] = -gAcc(i,j);
	
      }

}

void GaugeBoson::print()
{
  cout << endl;
  cout << "Boson : " << m_name << endl;
  cout << "Mass / width : " << m_mass << " / " << m_width << endl;
  cout << "Coupling strength (reference) : " << m_refCoupling << endl;
  cout << "Couplings to fermions : " << endl;
  for(int i=0; i<NMAX; ++i)
    for(int j=0; j<NMAX; ++j) {
      if(m_gV[i][j] != 0. || m_gA[i][j] != 0.)
	cout << "   Current (PDG code) : " << i << " "  << j
	     << " ; gV = " << m_gV[i][j] << " ; gA = " << m_gA[i][j] << endl;
    }
}
