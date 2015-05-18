#ifndef GAUGEBOSON
#define GAUGEBOSON

#include <iostream>

#define NMAX 25

// this class holds mass, width and couplings of gauge bosons

class GaugeBoson {

 public:

  // constructors

  GaugeBoson();
  ~GaugeBoson();

  // properties

  void setName(std::string name)          { m_name = name; }
  std::string getName()                   { return m_name; }

  double getReferenceCoupling()           { return m_refCoupling; }
  double (*getRunningCoupling())(double)  { return m_runCoupling; }
  void   setCoupling(double a)            { m_refCoupling = a; }

  void   setMass(double m)                { m_mass = m; }
  void   setWidth(double w)               { m_width = w; }  
  double getMass()                        { return m_mass; }
  double getWidth()                       { return m_width; }

  void setNeutral();
  void setChargedPlus();
  void setChargedMinus();
  int  getCharge();
  bool isNeutral();
  bool isCharged();
  bool isChargedPlus();
  bool isChargedMinus();

  void   setGV(int i1, int i2, double gv);
  void   setGA(int i1, int i2, double ga);
  double getGV(int i1, int i2);
  double getGA(int i1, int i2);

  // some default settings
  void setPhoton();
  void setWminus();
  void setWplus();
  void setZ0();
  void setZpSSM(double m);
  void setWplusSSM(double m);
  void setWminusSSM(double m);
  void setWplusRight(double m);
  void setWminusRight(double m);

  // print
  void print();

 private:

  std::string m_name;
  double m_refCoupling;
  double (*m_runCoupling)(double);
  double m_mass;
  double m_width;
  int    m_charge;

  double m_gV[NMAX][NMAX]; // vector coupling per particle
  double m_gA[NMAX][NMAX]; // axial    "           "

};
#endif
