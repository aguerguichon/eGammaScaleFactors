#ifndef LINESHAPE
#define LINESHAPE

#include <iostream>
#include <string>
#include <vector>

#include "LineShapeTool/GaugeBoson.h"

class LineShape 
{
 public:

  // default constructor
  LineShape();
  ~LineShape();

  // add boson to tower
  // ... cf GaugeBoson.h
  void addBoson(GaugeBoson* b);
  GaugeBoson* getBoson(int i);

  // interference terms
  // ... 0 : no interference
  // ... 1 : interference included
  // ... 2 : interference term only
  void useInterference(int t=1);
  
  // use running or fixed width
  // ... 0 : fixed width
  // ... 1 : running width (default)
  void useRunningWidth(bool t=true);

  // use running coupling
  // ... 0 : constant
  // ... 1 : running
  void useRunningCoupling(bool t=true);

  // set beam and final state particles
  // ... i1, i2 : PDG codes
  void setIn(int i1, int i2);
  void setOut(int o1, int o2);

  // get properties
  double getCharge();

  // (re)set some properties
  void setMass(int iBoson, double mass);
  void setWidth(int iBoson, double width);
  void setCoupling(int iBoson, double coupling);
  double getCoupling(int iBoson);

  // return differential cross-sections
  // ... symmetric part ~ (1+costh)^2
  double dSdMdCosthSym(double m, double costh,
		       int i1, int i2, int o1, int o2);
  // ... asymmetric part ~ costh
  double dSdMdCosthAsym(double m, double costh,
			int i1, int i2, int o1, int o2);
  // ... integrated over theta
  double dSdM(double m, int i1, int i2, int o1, int o2);

  // print
  void print();

 private:

  int m_nBoson;                // nb of bosons in tower
  std::vector<GaugeBoson*> m_boson;
  
  // charge
  double m_charge;

  // mass and width
  std::vector<double> m_M;
  std::vector<double> m_M2;
  std::vector<double> m_G;
  std::vector<double> m_G2;

  // coupling strength
  std::vector<double> m_cpl;
  typedef double (*fct_t)(double);
  std::vector<fct_t> m_run;

  // CKM for charged current; analog for neutral current
  double m_ckmIn;
  double m_ckmOut;

  // std::vector and axial couplings
  std::vector<double> m_vIn;
  std::vector<double> m_vOut;
  std::vector<double> m_aIn;
  std::vector<double> m_aOut;

  // colour factors : 1,1 for leptons; 1/3,3 for quarks
  double m_ncIn;
  double m_ncOut;

  int  m_useInterference;
  bool m_useRunningWidth;
  bool m_useRunningCoupling;

};
#endif
