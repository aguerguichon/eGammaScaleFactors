#include <iostream>
#include "ZeeAnalysis/Selection.h"
#include <math.h>
#include "ZeeAnalysis/SideFunctions.h"

using std::cout;
using std::endl;

bool PassSelection( xAOD::ElectronContainer &eContainer){

  MakeKinCut(eContainer);

  //Request exactly two electrons
  if (eContainer.size()!=2) return false;
  if (ComputeZMass(eContainer) > 100 || ComputeZMass(eContainer) < 80) return false;  

   return true;
}

//###############################################
void MakeKinCut( xAOD::ElectronContainer &eContainer ) {



  //loop over the electrons in the container
  // xAOD::ElectronContainer::const_iterator electron_itr = eContainer.begin();
//   xAOD::ElectronContainer::const_iterator electron_end = eContainer.end();
//   for( ; electron_itr != electron_end; ++electron_itr ) {
//   } // end for loop over electrons

  for (unsigned int i=0; i<eContainer.size(); i++) {
    if (fabs((*eContainer[i]).eta()) > 2.47 || (*eContainer[i]).pt() < 27e3) {
      //Have to erase this electron from container
      eContainer.erase(eContainer.begin()+i);
      i--;
    }

  }

}//MakeKincut
