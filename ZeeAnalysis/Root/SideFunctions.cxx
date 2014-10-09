#include <iostream>
using std::cout; using std::endl;
#include "ZeeAnalysis/SideFunctions.h"


double ComputeZMass( vector< xAOD::Electron * > velectrons ){
  //Check if the container has exactly two electrons
  //We do not consider combinations for the calibration
  if ( velectrons.size() != 2 ) return 0;

  return ( velectrons[0]->p4() + velectrons[1]->p4() ).M() * 0.001; //Give the result in Gev
}


