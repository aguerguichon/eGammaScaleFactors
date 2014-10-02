#include <iostream>
using std::cout; using std::endl;
#include "ZeeAnalysis/SideFunctions.h"


double ComputeZMass( vector< xAOD::Electron *> v_electron ){
  //Check if the container has exactly two electrons
  //We do not consider combinations for the calibration
  if ( v_electron.size()!=2 ) return 0;

  TLorentzVector Zmomentum = v_electron[0]->p4() + v_electron[1]->p4();

  return Zmomentum.M()*0.001; // Gives result in GeV
}


