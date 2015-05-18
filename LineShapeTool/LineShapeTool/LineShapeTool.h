#ifndef LINESHAPETOOL_H
#define LINESHAPETOOL_H

#include <iostream>

#include "LineShapeTool/GaugeBoson.h"
#include "LineShapeTool/LineShape.h"
#include "LineShapeTool/GeneratorSettings.h"

// Summary of generator assumptions
//
//                alphaQED         Mz       Gz        Mw       Gw         BW form
//
// Pythia          Running    91.1876   2.4812    80.399    2.072   Running-width
//
// Powheg          1/127.9    91.1876   2.4952    80.399    2.085     Fixed-width
//
// Alpgen          1/132.3    91.1880   2.4409    80.419    2.047     Fixed-width
//
// Sherpa          1/128.8    91.1876   2.4952    80.399    2.085     Fixed-width
//
// MC@NLO          1/137.0?   91.1876   2.4952    80.399    2.085     Fixed-width         


class LineShapeTool
{
  
 public:

  // initialize with initial values for mass and width
  // alphaQED is taken running, with alphaQED(Mz)~1/128.8

  LineShapeTool( double massIni=91.1876, double widthIni=2.4952, bool isNC=true );
  ~LineShapeTool();

  // reweight couplings

  double reweightFixedToRunningWidth( int i1, int i2, double m );
  double reweightNoRunningToImprovedBorn( int i1, int i2, double m, double coupling );
  double reweightQEDRunningToImprovedBorn( int i1, int i2, double m, double coupling );

  // reweight mass/width: massIni -> massNew, widthIni -> widthNew.

  double reweightMassTo( int i1, int i2, double m, double massNew );
  double reweightWidthTo( int i1, int i2, double m, double widthNew );
  double reweightMassWidthTo( int i1, int i2, double m, double massNew, double widthNew );

  // reweight mass/width: massOld -> massIni, widthOld -> widthIni.

  double reweightMassFrom( int i1, int i2, double m, double massOld );
  double reweightWidthFrom( int i1, int i2, double m, double widthOld );
  double reweightMassWidthFrom( int i1, int i2, double m, double massOld, double widthOld );

  // main user reweighting functions, wrapping the above

  double reweightPythiaToImprovedBorn( int i1, int i2, double m );
  double reweightPowhegToImprovedBorn( int i1, int i2, double m );
  double reweightAlpgenToImprovedBorn( int i1, int i2, double m );
  double reweightSherpaToImprovedBorn( int i1, int i2, double m );
  double reweightMCANLOToImprovedBorn( int i1, int i2, double m );

  //  // cross sections - TODO
  //
  //  double xsecPythia( int i1, int i2, double m );
  //  double xsecPowheg( int i1, int i2, double m );
  //  double xsecAlpgen( int i1, int i2, double m );
  //  double xsecSherpa( int i1, int i2, double m );
  //  double xsecMCANLO( int i1, int i2, double m );

  // handle on the LineShape, in case

  inline LineShape* getLineShape() { return m_lineshape; }

 private:

  void checkIncoming(int& i1, int& i2);

  bool m_isNC;

  double m_gamma;
  double m_runMass, m_runWidth;
  double m_cstMass, m_cstWidth;

  GaugeBoson *m_bg, *m_bz, *m_bw;
  LineShape *m_lineshape;

  int m_iOut1, m_iOut2; 

};

#endif
