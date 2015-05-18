#ifndef GENERATORSETTINGS
#define GENERATORSETTINGS


namespace GeneratorSettings {
  
  enum Generators { PythiaMC11, PowhegMC11, AlpgenMC11, SherpaMC11, McanloMC11 };
  
  const double emCouplings[6] = { 1./128.8, 1./127.9, 1./132.3, 1./128.8, 1./137.0 }; 
  const double zMass[6]       = {  91.1876,  91.1876,  91.1880,  91.1876,  91.1876 }; 
  const double zWidth[6]      = {   2.4812,   2.4952,   2.4409,   2.4952,   2.4952 }; 
  const double wMass[6]       = {   80.399,   80.399,   80.419,   80.399,   80.399 }; 
  const double wWidth[6]      = {    2.072,    2.085,    2.047,    2.085,    2.085 }; 
  
}

#endif
