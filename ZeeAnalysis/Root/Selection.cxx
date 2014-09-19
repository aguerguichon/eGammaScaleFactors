#include <iostream>
#include "ZeeAnalysis/Selection.h"
#include <math.h>
#include "ZeeAnalysis/SideFunctions.h"

bool PassSelection(const xAOD::ElectronContainer *eContainer){
  //Request exactly two electrons
  if (eContainer->size()!=2) return false;
  if (ComputeZMass(eContainer) > 100 || ComputeZMass(eContainer) < 80) return false;  

  // loop over the electrons in the container
  // xAOD::ElectronContainer::const_iterator electron_itr = eContainer->begin();
  // xAOD::ElectronContainer::const_iterator electron_end = eContainer->end();
  // for( ; electron_itr != electron_end; ++electron_itr ) {
  //   if (fabs((*electron_itr)->eta())>2.47) return false;
  // } // end for loop over electrons
   return true;
}
