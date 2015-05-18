#include "LineShapeTool/Constants.h"

double constants::Q(int ipdg)
{
  if(ipdg==1 || ipdg==3 || ipdg==5)
    return -1./3.;
  if(ipdg==-1 || ipdg==-3 || ipdg==-5)
    return +1./3.;
  else if(ipdg==2 || ipdg==4 || ipdg==6)
    return +2./3.;
  else if(ipdg==-2 || ipdg==-4 || ipdg==-6)
    return -2./3.;
  else if(ipdg==11 || ipdg==13 || ipdg==15)
    return -1.;
  else if(ipdg==-11 || ipdg==-13 || ipdg==-15)
    return +1.;
  else
    return 0.;
}

double constants::Nc(int ipdg)
{
  if(ipdg==2 || ipdg==4 || ipdg==6 || ipdg==1 || ipdg==3 || ipdg==5)
    return 3.;
  else if(ipdg==12 || ipdg==14 || ipdg==16 || ipdg==11 || ipdg==13 || ipdg==15)
    return 1.;
  else
    return 0.;
}

double constants::T3(int ipdg)
{
  if(ipdg==1 || ipdg==3 || ipdg==5 || ipdg==11 || ipdg==13 || ipdg==15 )
    return -1./2.;
  else if(ipdg==2 || ipdg==4 || ipdg==6 || ipdg==12 || ipdg==14 || ipdg==16 )
    return +1./2.;
  else
    return 0.;
}

double constants::M(int ipdg)
{
  int aipdg;
  if(ipdg>0) 
    aipdg = ipdg;
  else
    aipdg = -ipdg;

  if     (aipdg ==  1) return 0.002;
  else if(aipdg ==  2) return 0.005;
  else if(aipdg ==  3) return 0.104;
  else if(aipdg ==  4) return 1.27;
  else if(aipdg ==  5) return 4.20;
  else if(aipdg ==  6) return 171.2;
  else if(aipdg == 11) return 0.000510998910;
  else if(aipdg == 13) return 0.105658367;
  else if(aipdg == 15) return 1.77684;
  else return 0.;
}

// neutral current "CKM" : diagonal (no FCNC)  
double constants::Vnc(int i1, int i2)
{
  if(i1 != i2)
    return 0.;
  return 1.;
}

// CKM matrix
double constants::Vcc(int i1, int i2)
{
  int imin, imax;
  if(i1<i2) {
    imin = i1;
    imax = i2;
  }
  else {
    imin = i2;
    imax = i1;
  }
  if     (imin==1 && imax==2) return 0.97418;  // Vud
  else if(imin==1 && imax==4) return 0.230;    // Vcd
  else if(imin==1 && imax==6) return 0.0081;   // Vtd
  else if(imin==2 && imax==3) return 0.2255;   // Vus
  else if(imin==3 && imax==4) return 1.;       // Vcs
  else if(imin==3 && imax==6) return 0.00387;  // Vts
  else if(imin==2 && imax==5) return 0.00393;  // Vub
  else if(imin==4 && imax==5) return 0.00412;  // Vcb
  else if(imin==5 && imax==6) return 1.;       // Vtb
  else if(imin==11 && imax==12) return 1.;       // Venu
  else if(imin==13 && imax==14) return 1.;       // Vmunu
  else if(imin==15 && imax==16) return 1.;       // Vtaunu
  else return 0.;
}

// SM neutral current couplings

double constants::gVnc(unsigned int i1, unsigned int i2)
{
  if( (isUp(i1) && isUp(i2)) || 
      (isDown(i1) && isDown(i2)) )
    return T3(i1) - 2*Q(i1)*S2w();
  else
    return 0.;
}
  
double constants::gAnc(unsigned int i1, unsigned int i2)
{
  if( (isUp(i1) && isUp(i2)) || 
      (isDown(i1) && isDown(i2)) )
    return T3(i1);
  else
    return 0.;
}

// SM charged current couplings

double constants::gVcc(unsigned int i1, unsigned int i2)
{
  if( (isUp(i1) && isDown(i2)) ||
      (isDown(i1) && isUp(i2)) )
    return fabs( T3(i1) ) ;
  else
    return 0.;
}
  
double constants::gAcc(unsigned int i1, unsigned int i2)
{
  if( (isUp(i1) && isDown(i2)) ||
      (isDown(i1) && isUp(i2)) )
    return fabs( T3(i1) );
  else
    return 0.;
}

// QED, QCD running couplings (stolen from Pythia PYALEM, PYALPS)
// only the running factors are included; the reference coupling is factored out

double constants::Rqed(double s) {

  double aempi = Aqed0()/3./Pi();
  double rpigg;

  if( s <0.09 )
    rpigg = aempi*(13.4916+1.*log(s)) + 0.00835*log(1.+s);
  else if( s < 9. )
    rpigg = aempi*(16.3200+2.*log(s)) + 0.00238*log(1.+3.927*s);
  else if(s < 10000)
    rpigg = aempi*(13.4955+3.*log(s)) + 0.00165 + 0.00299*log(1.+s);
  else
    rpigg = aempi*(13.4955+3.*log(s)) + 0.00221 + 0.00293*log(1.+s);

  return 1./(1. - rpigg);

}

double constants::Rqcd(double s)
{
  return 1.;
}

double constants::Rweak(double s)
{
  return 1.;
}

double constants::Rconst(double s)
{
  return 1.;
}
