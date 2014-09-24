#ifndef SIDEFUNCTIONS_H
#define SIDEFUNCTIONS_H

#include "xAODEgamma/ElectronContainer.h"

double ComputeZMass( const xAOD::ElectronContainer &eContainer);
void MakeElectronCut( xAOD::ElectronContainer &eContainer );


#endif
