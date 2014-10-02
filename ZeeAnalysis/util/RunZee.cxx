//C++ libraries
#include <iostream>
#include <string>
#include <boost/program_options.hpp>

//Misc libraries
#include "ZeeAnalysis/Analysis.h"

using std::string;
using std::cout;
using std::endl;

namespace po = boost::program_options;

void Style_Christophe();

int main( int argc, char* argv[] ) {
  Style_Christophe(); //Setup style for histograms

  //Check validity of Arguments                                                      
  po::options_description desc("LikelihoodProfiel Usage");
  //define all options in the program                                                
  vector<string> infile;
  string outName,anaName;
  desc.add_options()
    ("help", "Display this help message")
    ("outName", po::value<string >(&outName)->default_value("Analysis.root") , "Name of the output file")
    ("infile", po::value< vector< string > >(&infile), "The name of xAOD rootfiles")
    ("anaName", po::value< string >( &anaName )->default_value( "Analysis" ), "Name of the object")
    ;

  //Define options gathered by position                                              
  po::positional_options_description p;
  p.add("infile", -1);

  // create a map vm that contains options and all arguments of options              
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).positional(p).style(po::command_line_style::unix_style ^ po::command_line_style::allow_short).run(), vm);
  po::notify(vm);
  if(vm.count("help")) {cout << desc; return 0;}

  //########## END BOOST ##############################"
  if ( ! infile.size() ) { 
  string infileNameMC = "/sps/atlas/f/fayard/christophe/mc14_8TeV.129680.PowhegPythia8_AU2CT10_Zee_DiLeptonFilter.merge.AOD.e1861_s1933_s1911_r5591_r5625_tid01512382_00/AOD.01512382._000239.pool.root.1";
  string infileNameMC2 = "/sps/atlas/f/fayard/christophe/mc14_8TeV.129680.PowhegPythia8_AU2CT10_Zee_DiLeptonFilter.merge.AOD.e1861_s1933_s1911_r5591_r5625_tid01512382_00/AOD.01512382._000001.pool.root.1";

  vector< string > inputMC;
  inputMC.push_back( infileNameMC );
  //  inputMC.push_back( infileNameMC2 );

  Analysis analysisMC( "MC",  inputMC );
  analysisMC.TreatEvents();
  analysisMC.Save("results/test.root");

  }
  else {
    cout << "infile size : " << infile.size() << endl;

  Analysis analysisData( anaName,  infile );
  analysisData.TreatEvents();
  analysisData.Save( outName );
  }

return 0;
}
