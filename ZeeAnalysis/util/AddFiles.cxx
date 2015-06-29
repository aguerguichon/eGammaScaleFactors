#include <iostream>
#include <vector>
using std::vector;
using std::cout; 
using std::endl;


#include "ZeeAnalysis/Analysis.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main( int argc, char* argv[] ) {

  //################# BOOST ####################
  //Check validity of Arguments
  po::options_description desc("LikelihoodProfiel Usage");
  //define all options in the program
  vector<string> infile;
  string outName;
  desc.add_options()
    ("help", "Display this help message")
    ("outName", po::value<string >(&outName)->default_value("Analysis.root") , "Name of the output root file")
    ("infile", po::value< vector< string > >(&infile), "The name of result rootfiles")
    ("divide", "Divide the two input files")
    ("debug", "Debug mode" )
    ;
  //Define options gathered by position
  po::positional_options_description p;
  p.add("infile", -1);
  // create a map vm that contains options and all arguments of options
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).positional(p).style(po::command_line_style::unix_style ^ po::command_line_style::allow_short).run(), vm);
  po::notify(vm);
  if(vm.count("help")) {cout << desc; return 0;}
  if ( ! infile.size() ) { cout << "No input file" << endl; return 1;}
  //########## END BOOST ##############################

  //  int debug = ( vm.count( "debug" ) ) ? 1 : 0;


  Analysis *final_analysis = 0;
  unsigned int counterAnalysis = 0;
  string name;
  //Load input files into an analysis
  for (unsigned int i = 0; i < infile.size(); i++ ) {
    cout << infile[i] << endl;
    
    if ( !final_analysis ) {
      char buffer[100];
      string dumOutName=outName;
      sprintf( buffer, "_%d", counterAnalysis );
      final_analysis = new Analysis( "Final", dumOutName.insert( dumOutName.find_last_of( "."), buffer ));
      final_analysis->SetName( "Analysis" );
      final_analysis->Load( infile[i] );
      name = dumOutName;  
      if ( name.find_last_of( "/" ) != string::npos ) name = name.substr( name.find_last_of( "/" ) +1 );
      name = name.substr( 0, name.find_last_of( "." ) );
      final_analysis->SetName( name );}
    else {
      Analysis dummy_analysis;
      dummy_analysis.SetName( "Analysis" );
      dummy_analysis.Load( infile[i] );
      final_analysis->Add( dummy_analysis );
    }
    if ( final_analysis->GetGoodEvents()>5000000 ) {
      cout << "saving : " << name << endl;
      cout << "GoodEvents : " << final_analysis->GetGoodEvents() << endl;
      final_analysis->Save();
      delete final_analysis;
      final_analysis = 0;
      counterAnalysis++;
    }
  }
  cout << "saving : " << name << endl;
  cout << "GoodEvents : " << final_analysis->GetGoodEvents() << endl;
  final_analysis->Save();
  delete final_analysis;
  final_analysis = 0;

  
  cout << "Saved " << counterAnalysis+1 << " files"<< endl;
  return 0;
}
