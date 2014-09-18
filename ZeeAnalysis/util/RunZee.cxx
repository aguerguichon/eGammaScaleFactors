//C++ libraries
#include <iostream>
#include <string>

//Misc libraries
#include "ZeeAnalysis/Analysis.h"

using std::string;
using std::cout;
using std::endl;


int main( int argc, char* argv[] ) {

  //temporary useless line to avoid warning messages
  if (argc >1) cout << argv[0] << endl; 

  string infileName = "/sps/atlas/f/fayard/christophe/mc14_8TeV.129680.PowhegPythia8_AU2CT10_Zee_DiLeptonFilter.merge.AOD.e1861_s1933_s1911_r5591_r5625_tid01512382_00/AOD.01512382._000239.pool.root.1";

  Analysis anaMC(infileName);
   return 0;
}
