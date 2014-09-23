#ifndef SELECTION_H
#define SELECTION_H
#include <xAODEgamma/ElectronContainer.h>

bool PassSelection( xAOD::ElectronContainer &eContainer);
void MakeKinCut( xAOD::ElectronContainer &eContainer);

#endif
