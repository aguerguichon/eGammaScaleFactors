#ifndef HEPCONST
#define HEPCONST

#include <math.h>

namespace constants {

  // constants, parameters

  inline double Pi() { return 3.14159265358979323846; }

  inline double GeV2nb() { return 389379.; }

  inline double Gf() { return 0.0000116637; }

  inline double Mg() { return 0.; }
  inline double Mw() { return 80.399; }
  inline double Mz() { return 91.188; }
  
  inline double Gg() { return 0.; }
  inline double Gw() { return 2.085; }
  inline double Gz() { return 2.4952; }
  
  inline double Aqed0()  { return 0.00729735; }
  inline double AqedMz() { return 0.00776357; }
  inline double AqcdMz() { return 0.1176; }
  double Rqed(double s);
  double Rqcd(double s);
  double Rweak(double s);
  double Rconst(double s);

  inline double S2w() { return 0.23119; }
  inline double Sw()  { return sqrt(S2w()); }
  inline double C2w() { return 1-S2w(); }
  inline double Cw()  { return sqrt(C2w()); }

  double Q(int ipdg);
  inline double Qn() { return  0.; }
  inline double Qe() { return -1.; }
  inline double Qu() { return 2./3.; }
  inline double Qd() { return -1./3.; }

  double Nc(int ipdg);
  inline double NCn() { return 1.; }
  inline double NCe() { return 1.; }
  inline double NCl() { return 1.; }
  inline double NCu() { return 3.; }
  inline double NCd() { return 3.; }
  inline double NCq() { return 3.; }

  double T3(int ipdg);
  inline double T3n() { return   1./2.; }
  inline double T3e() { return  -1./2.; }
  inline double T3u() { return   1./2.; }
  inline double T3d() { return  -1./2.; }

  double M(int ipdg);
  inline double Me()   { return 0.000510998910; }
  inline double Mmu()  { return 0.105658367; }
  inline double Mtau() { return 1.77684; }
  inline double Md()   { return 0.002; }
  inline double Mu()   { return 0.005; }
  inline double Ms()   { return 0.104; }
  inline double Mc()   { return 1.27; }
  inline double Mb()   { return 4.20; }
  inline double Mt()   { return 171.2; }

  double Vnc(int i1, int i2);
  double Vcc(int i1, int i2);
  inline double Vud() { return 0.97418; }
  inline double Vus() { return 0.2255; }
  inline double Vub() { return 0.00393; }
  inline double Vcd() { return 0.230; }
  inline double Vcs() { return 1.; }
  inline double Vcb() { return 0.00412; }
  inline double Vtd() { return 0.0081; }
  inline double Vts() { return 0.00387; }
  inline double Vtb() { return 1.; }

  // standard model relations

  inline double gVn() { return T3n() - 2*Qn()*S2w(); }
  inline double gVe() { return T3e() - 2*Qe()*S2w(); }
  inline double gVu() { return T3u() - 2*Qu()*S2w(); }
  inline double gVd() { return T3d() - 2*Qd()*S2w(); }

  inline double gAn() { return T3n(); }
  inline double gAe() { return T3e(); }
  inline double gAu() { return T3u(); }
  inline double gAd() { return T3d(); }

  double gVnc(unsigned int i1, unsigned int i2);
  double gAnc(unsigned int i1, unsigned int i2);
  double gVcc(unsigned int i1, unsigned int i2);
  double gAcc(unsigned int i1, unsigned int i2);

  inline bool isUp(unsigned int i) 
    { return (i==2 || i==4 || i==6 || i==12 || i==14 || i==16); }

  inline bool isUpLepton(unsigned int i) 
    { return (i==12 || i==14 || i==16); }

  inline bool isUpQuark(unsigned int i)
    { return (i==2 || i==4 || i==6); }

  inline bool isDown(unsigned int i) 
    { return (i==1 || i==3 || i==5 || i==11 || i==13 || i==15); }

  inline bool isDownLepton(unsigned int i)
    { return (i==11 || i==13 || i==15); }

  inline bool isDownQuark(unsigned int i)
    { return (i==1 || i==3 || i==5); }

  inline bool isQuark(unsigned int i)
    { return (i>=1 && i <= 6); }

  inline bool isLepton(unsigned int i)
    { return (i>=11 && i <= 16); }

  inline double GfSM()  { return Aqed0()*Pi()/(sqrt(2)*Mw()*Mw()*S2w()); }

  inline double Gznn()  { return NCn()*Gf()*Mz()*Mz()*Mz()/(6*sqrt(2)*Pi()) 
			    * (gVn()*gVn()+gAn()*gAn()); }

  inline double Gznn1()  { return NCn()*Aqed0()*Mz()/(12*S2w()*(1-S2w()))
			    * (gVn()*gVn()+gAn()*gAn()); }

  inline double Gzee()  { return NCe()*Gf()*Mz()*Mz()*Mz()/(6*sqrt(2)*Pi()) 
			    * (gVe()*gVe()+gAe()*gAe()); }

  inline double Gzee1()  { return NCe()*Aqed0()*Mz()/(12*S2w()*(1-S2w()))
			    * (gVe()*gVe()+gAe()*gAe()); }

  inline double Gzuu()  { return NCu()*Gf()*Mz()*Mz()*Mz()/(6*sqrt(2)*Pi()) 
			    * (gVu()*gVu()+gAu()*gAu()); }

  inline double Gzuu1()  { return NCu()*Aqed0()*Mz()/(12*S2w()*(1-S2w()))
			    * (gVu()*gVu()+gAu()*gAu()); }

  inline double Gzdd()  { return NCd()*Gf()*Mz()*Mz()*Mz()/(6*sqrt(2)*Pi()) 
			    * (gVd()*gVd()+gAd()*gAd()); }

  inline double Gzdd1()  { return NCd()*Aqed0()*Mz()/(12*S2w()*(1-S2w()))
			    * (gVd()*gVd()+gAd()*gAd()); }

  inline double Gzall() { return 3*Gznn()+3*Gzee()+2*Gzuu()+3*Gzdd(); }
  inline double Gzall1() { return 3*Gznn1()+3*Gzee1()+2*Gzuu1()+3*Gzdd1(); }
}

enum pdg {

  d=1, 
  u=2, 
  s=3, 
  c=4, 
  b=5, 
  t=6, 
  e=11, 
  nue=12, 
  mu=13, 
  numu=14, 
  tau=15, 
  nutau=16,
  p=2212,
  pbar=-2212

};

#endif
