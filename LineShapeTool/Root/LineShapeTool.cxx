#include "LineShapeTool/Constants.h"
#include "LineShapeTool/LineShapeTool.h"

#include <cmath>
#include <cstdlib>

using namespace std;

// Conventions:
// - At construction, the mass and width arguments are assumed to be RUNNING mass and width
// - by default alphaQED is running, with alphaQED(Mz)~1/128.8

using namespace GeneratorSettings;

LineShapeTool::LineShapeTool( double massIni, double widthIni, bool isNC ) {

  m_isNC = isNC;

  m_runMass = massIni; 
  m_runWidth = widthIni;

  m_gamma = 1./sqrt( 1. + m_runWidth*m_runWidth/m_runMass/m_runMass );

  m_cstMass = m_runMass*m_gamma;
  m_cstWidth = m_runWidth*m_gamma;

  if( m_isNC ) { // g*/Z lineshape with running couplings
    
    m_bg = new GaugeBoson();
    m_bg->setPhoton();
    
    m_bz = new GaugeBoson();
    m_bz->setZ0();
    m_bz->setMass( m_runMass );
    m_bz->setWidth( m_runWidth );
    
    m_lineshape = new LineShape();
    m_lineshape->addBoson( m_bz );
    m_lineshape->addBoson( m_bg );

    m_iOut1 = m_iOut2 = 13;
    
  } else { // W lineshape
    
    m_bw = new GaugeBoson();
    m_bw->setWplus();
    m_bw->setMass( m_runMass );
    m_bw->setWidth( m_runWidth );
    
    m_lineshape = new LineShape();
    m_lineshape->addBoson( m_bw );

    m_iOut1 = 13;
    m_iOut2 = 14;
    
  }

}


LineShapeTool::~LineShapeTool() {

    if( m_isNC ) { // g*/Z lineshape with running couplings
        delete m_bg;
        delete m_bz;
        delete m_lineshape;
    } else { // W lineshape
        delete m_bw;
        delete m_lineshape;
    }

}


// basic Breit-Wigner reweighting


double LineShapeTool::reweightWidthTo( int i1, int i2, double m, double widthNew ) {

  double widthIni = m_lineshape->getBoson( 0 )->getWidth();

  m_lineshape->setWidth( 0, widthNew );
  double num = m_lineshape->dSdM( m, i1, i2, m_iOut1, m_iOut2 );

  m_lineshape->setWidth( 0, widthIni );
  double den = m_lineshape->dSdM( m, i1, i2, m_iOut1, m_iOut2 );

  return num/den;

}


double LineShapeTool::reweightMassTo( int i1, int i2, double m, double massNew ) {

  double massIni = m_lineshape->getBoson( 0 )->getMass();

  m_lineshape->setMass( 0, massNew );
  double num = m_lineshape->dSdM( m, i1, i2, m_iOut1, m_iOut2 );

  m_lineshape->setMass( 0, massIni );
  double den = m_lineshape->dSdM( m, i1, i2, m_iOut1, m_iOut2 );

  return num/den;

}


double LineShapeTool::reweightMassWidthTo( int i1, int i2, double m, double massNew, double widthNew ) {

  double massIni  = m_lineshape->getBoson( 0 )->getMass();
  double widthIni = m_lineshape->getBoson( 0 )->getWidth();

  m_lineshape->setMass( 0, massNew );
  m_lineshape->setWidth( 0, widthNew );
  double num = m_lineshape->dSdM( m, i1, i2, m_iOut1, m_iOut2 );

  m_lineshape->setMass( 0, massIni );
  m_lineshape->setWidth( 0, widthIni );
  double den = m_lineshape->dSdM( m, i1, i2, m_iOut1, m_iOut2 );

  return num/den;

}


double LineShapeTool::reweightWidthFrom( int i1, int i2, double m, double widthOld ) {

  return 1./reweightWidthTo( i1, i2, m, widthOld );

}


double LineShapeTool::reweightMassFrom( int i1, int i2, double m, double massOld ) {

  return 1./reweightMassTo( i1, i2, m, massOld );

}


double LineShapeTool::reweightMassWidthFrom( int i1, int i2, double m, double massOld, double widthOld ) {

  return 1./reweightMassWidthTo( i1, i2, m, massOld, widthOld );

}


// coupling reweighting


double LineShapeTool::reweightNoRunningToImprovedBorn( int i1, int i2, double m, double coupling ) {
  
  if( !m_isNC )
    return 1.;

  double oldCoupling = m_lineshape->getCoupling(1);

  m_lineshape->setCoupling( 1, coupling );
  m_lineshape->useRunningCoupling( false );
  double den = m_lineshape->dSdM( m, i1, i2, m_iOut1, m_iOut2 );

  m_lineshape->setCoupling( 1, oldCoupling );
  m_lineshape->useRunningCoupling( true );
  double num = m_lineshape->dSdM( m, i1, i2, m_iOut1, m_iOut2 );

  return num/den;

}


double LineShapeTool::reweightQEDRunningToImprovedBorn( int i1, int i2, double m, double coupling ) {

  if( !m_isNC )
    return 1.;

  double mz = constants::Mz();
  double qedfac = pow(constants::Rqed( mz*mz ) / constants::Rqed( m*m ), 2);

  double oldCoupling = m_lineshape->getCoupling(1);

  m_lineshape->setCoupling( 1, coupling );
  m_lineshape->useRunningCoupling( false );
  double den = m_lineshape->dSdM( m, i1, i2, m_iOut1, m_iOut2 );

  m_lineshape->setCoupling( 1, oldCoupling );
  m_lineshape->useRunningCoupling( true );
  double num = m_lineshape->dSdM( m, i1, i2, m_iOut1, m_iOut2 );

  return qedfac*num/den;

}


// derived reweighting functions


double LineShapeTool::reweightFixedToRunningWidth( int i1, int i2, double m ) {

  return reweightMassWidthFrom( i1, i2, m, m_runMass/m_gamma, m_runWidth/m_gamma );
  
}


double LineShapeTool::reweightPythiaToImprovedBorn( int i1, int i2, double m ) {

  checkIncoming(i1, i2);

  // divides off the overall running aEM, and re-includes it in the photon term only
  double w1 = m_isNC ? reweightQEDRunningToImprovedBorn( i1, i2, m, emCouplings[PythiaMC11] ) : 1.;

  // corrects the width back to PDG value
  double width = m_isNC ? zWidth[PythiaMC11] : wWidth[PythiaMC11];
  double w2 = reweightWidthFrom( i1, i2, m, width );

  return w1*w2;

}


double LineShapeTool::reweightPowhegToImprovedBorn( int i1, int i2, double m ) {

  checkIncoming(i1, i2);

  // divides off fixed aEM and includes running value in the photon term
  double w1 = m_isNC ? reweightNoRunningToImprovedBorn( i1, i2, m, emCouplings[PowhegMC11] ) : 1.;

  // corrects propagator to running width
  double w2 = reweightFixedToRunningWidth( i1, i2, m );

  return w1*w2;

}


double LineShapeTool::reweightAlpgenToImprovedBorn( int i1, int i2, double m ) {

  checkIncoming(i1, i2);
  
  // replaces fixed by running aEM in the photon term
  double w1 = m_isNC ? reweightNoRunningToImprovedBorn( i1, i2, m, emCouplings[AlpgenMC11] ) : 1.;

  // corrects mass and width back to PDG value
  double  mass = m_isNC ? zMass[AlpgenMC11] : wMass[AlpgenMC11];
  double width = m_isNC ? zWidth[AlpgenMC11] : wWidth[AlpgenMC11];
  double w2 = reweightMassWidthFrom( i1, i2, m, mass, width );

  // corrects propagator to running width
  double w3 = reweightFixedToRunningWidth( i1, i2, m );

  return w1*w2*w3;

}


double LineShapeTool::reweightSherpaToImprovedBorn( int i1, int i2, double m ) {

  checkIncoming(i1, i2);

  // replaces fixed by running aEM in the photon term
  double w1 = m_isNC ? reweightNoRunningToImprovedBorn( i1, i2, m, emCouplings[SherpaMC11] ) : 1.;

  // corrects propagator to running width
  double w2 = reweightFixedToRunningWidth( i1, i2, m );

  return w1*w2;

}


double LineShapeTool::reweightMCANLOToImprovedBorn( int i1, int i2, double m ) {

  checkIncoming(i1, i2);

  // replaces fixed by running aEM in the photon term
  double w1 = m_isNC ? reweightNoRunningToImprovedBorn( i1, i2, m, emCouplings[PowhegMC11] ) : 1.;

  // corrects propagator to running width
  double w2 = reweightFixedToRunningWidth( i1, i2, m );

  return w1*w2;

}



void LineShapeTool::checkIncoming(int& i1, int& i2) {

  // adapted from J.Kretzschmar

  if ( i1==21 && i2==21 ) { // gluon + gluon
    
    if ( m_isNC ) {         // ... Z/gamma*

      i1 = 2;
      i2 = 2;

    } else {                // ... W

      i1 = 2;
      i2 = 1;

    }

  } else if ( i1==21 ) {    // gluon + quark

    if ( m_isNC ) {         // ... Z/gamma*

      i1 = i2;

    } else {                // ... W

      if( i2%2==0 ) 
	i1 = abs(i2)-1;
      else
	i1 = abs(i2)+1;

    }

  } else if ( i2==21 ) {    // quark + gluon

    if ( m_isNC ) {         // ... Z/gamma*

      i2 = i1;

    } else {                // ... W

      if( i1%2==0 )
        i2 = abs(i1)-1;
      else
        i2 = abs(i1)+1;

    }

  }

}
