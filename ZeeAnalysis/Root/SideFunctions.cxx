#include <iostream>
using std::cout; using std::endl;
#include "ZeeAnalysis/SideFunctions.h"


double ComputeZMass(const xAOD::ElectronContainer &eContainer){
  //Check if the container has exactly two electrons
  //We do not consider combinations for the calibration
  if (eContainer.size()!=2) {cout << "Error : Too many electrons in container" << endl; exit(1);}

  TLorentzVector Zmomentum(0,0,0,0); //Momentum of the initial Z particle

  //Loop other electrons to get their 4 momentum
  xAOD::ElectronContainer::const_iterator electron_itr = eContainer.begin();
  xAOD::ElectronContainer::const_iterator electron_end = eContainer.end();
  for( ; electron_itr != electron_end; ++electron_itr ) {
    Zmomentum+= (*electron_itr)->p4();
  } // end for loop over electrons  


  return Zmomentum.M()*0.001; // Gives result in GeV
}
