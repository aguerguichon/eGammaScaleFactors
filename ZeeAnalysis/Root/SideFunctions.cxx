#include <iostream>
using std::cout; using std::endl;
#include "ZeeAnalysis/SideFunctions.h"


double ComputeZMass( xAOD::ElectronContainer *eCont ){
  //Check if the container has exactly two electrons
  //We do not consider combinations for the calibration
  if ( eCont->size()!=2 ) return 0;

  return ( (*eCont)[0]->p4() + (*eCont)[1]->p4() ).M() * 0.001; //Give the result in Gev
}


