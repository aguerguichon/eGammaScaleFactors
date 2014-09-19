//C++ libraries
#include <iostream>
#include <string>

//Misc libraries
#include "ZeeAnalysis/Analysis.h"

using std::string;
using std::cout;
using std::endl;

void Style_Christophe();

int main( int argc, char* argv[] ) {
  Style_Christophe(); //Setup style for histograms

  //temporary useless line to avoid warning messages
  if (argc >1) cout << argv[0] << endl; 

  system( "mkdir submitDir" );

  string infileNameMC = "/sps/atlas/f/fayard/christophe/mc14_8TeV.129680.PowhegPythia8_AU2CT10_Zee_DiLeptonFilter.merge.AOD.e1861_s1933_s1911_r5591_r5625_tid01512382_00/AOD.01512382._000239.pool.root.1";
  string infileNameData = "/sps/atlas/f/fayard/christophe1/data12_8TeV.00204796.physics_Egamma.merge.AOD.r5723_p1751_tid01534473_00/AOD.01534473._000007.pool.root.1";

  Analysis anaMC( "anaMC", infileNameMC );
  anaMC.TreatEvents();
  anaMC.PlotResult( "submitDir/anaMC.root" );
  
  Analysis anaData( "anaData", infileNameData );
  anaData.TreatEvents();
  anaData.PlotResult( "submitDir/anaData.root" );

  CompareAnalysis( anaMC, anaData, "submitDir/ratio.root" );

return 0;
}
