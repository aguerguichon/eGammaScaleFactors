using namespace std;

#include <iostream>

#include "LineShapeTool/LineShape.h"
#include "LineShapeTool/GaugeBoson.h"
#include "LineShapeTool/LineShapeTool.h"
#include "LineShapeTool/GeneratorSettings.h"

#include "TGraph.h"
#include "TFile.h"

using namespace std;

int main()
{

  // Default initialization (Z PDG parameters, isNC=true)
  LineShapeTool rwtz;                              

  // the true (Born) mass for this event
  double m = 66.000;

  // incoming flavour (1:down, 2:up, ...)
  int iq1 = 2; 
  int iq2 = 21; 

  cout.precision(12);
  int i=0;

  // graphs
  TGraph wpow1, wpow2, wpyt1, wpyt2;
  TGraph pytpow;
  TGraph pytTot, powTot, alpTot, sheTot, mcaTot;
  
  while ( m<=116. ) {


    // OVERALL REWEIGHTING FOR VARIOUS GENERATORS

    // these are the main user functions, hiding the details
    // arguments : 
    // - iq : type of incoming quarks
    // - m : true mass for this event
    
    double wPythiaTot = rwtz.reweightPythiaToImprovedBorn( iq1, iq2, m );
    double wPowhegTot = rwtz.reweightPowhegToImprovedBorn( iq1, iq2, m );
    double wAlpgenTot = rwtz.reweightAlpgenToImprovedBorn( iq1, iq2, m );
    double wSherpaTot = rwtz.reweightSherpaToImprovedBorn( iq1, iq2, m );
    double wMcanloTot = rwtz.reweightMCANLOToImprovedBorn( iq1, iq2, m );
    

    // BREIT-WIGNER REWEIGHTING

    // this is to reweight Powheg (parameters are given, fixed -> running width parametrization):
    // arguments : 
    // - iq : type of incoming quarks
    // - m : true mass for this event
    
    double wPowheg1 = rwtz.reweightFixedToRunningWidth( iq1, iq2, m );
         
    // this is to reweight Pythia (gz = 2.48118 -> 2.4952 GeV)
    // arguments : 
    // - iq : type of incoming quarks
    // - m : true mass for this event
    // - width : original width
    
    double wPythia1 = rwtz.reweightWidthFrom( iq1, iq2, m, GeneratorSettings::zWidth[GeneratorSettings::PythiaMC11] );


    // REWEIGHTING FOR RUNNING COUPLINGS

    // Fixed couplings -> improved Born (relevant for Powheg)
    // arguments : 
    // - iq : type of incoming quarks
    // - m : true mass for this event
    
    double wPowheg2 = rwtz.reweightNoRunningToImprovedBorn( iq1, iq2, m, GeneratorSettings::emCouplings[GeneratorSettings::PowhegMC11] );
    
    // qed running for all couplings -> improved Born (relevant for Pythia)
    // arguments : 
    // - iq : type of incoming quarks
    // - m : true mass for this event
    
    double wPythia2 = rwtz.reweightQEDRunningToImprovedBorn( iq1, iq2, m, GeneratorSettings::emCouplings[GeneratorSettings::PythiaMC11] );


    // Plotting

    wpow1.SetPoint(i, m, wPowheg1);
    wpow2.SetPoint(i, m, wPowheg2);
    wpyt1.SetPoint(i, m, wPythia1);
    wpyt2.SetPoint(i, m, wPythia2);
    pytpow.SetPoint(i, m, wPowheg2/wPythia2);
 
    pytTot.SetPoint(i, m, wPythiaTot);
    powTot.SetPoint(i, m, wPowhegTot);
    alpTot.SetPoint(i, m, wAlpgenTot);
    sheTot.SetPoint(i, m, wSherpaTot);
    mcaTot.SetPoint(i, m, wMcanloTot);

    m += .01;
    i++;

  }

  TFile f("zOutput.root","recreate");
  wpow1.Write("wpow1");
  wpow2.Write("wpow2");
  wpyt1.Write("wpyt1");
  wpyt2.Write("wpyt2");
  pytpow.Write("pytpow");
  pytTot.Write("pytTot");
  powTot.Write("powTot");
  alpTot.Write("alpTot");
  sheTot.Write("sheTot");
  mcaTot.Write("mcaTot");
  f.Close();

}

