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

  int debug = ( vm.count( "debug" ) ) ? 1 : 0;

  string name = outName;
  if ( name.find_last_of( "/" ) != string::npos ) name = name.substr( name.find_last_of( "/" ) +1 );
  name = name.substr( 0, name.find_last_of( "." ) );

  Analysis final_analysis( "Final", debug );;

  if ( vm.count( "divide" ) ) {
    if ( infile.size() != 2 ) { cout << "Not only 2 inputs" << endl; return 1;}

    final_analysis.Load( infile[0] );

    Analysis tmp_ana;
    tmp_ana.Load( infile[1] );

    cout << "Divide" << endl;
    final_analysis.Divide( tmp_ana );
    cout << "Divided" << endl;
  }
  else {
    //Load input files into an analysis

    for (unsigned int i = 0; i < infile.size(); i++ ) {
      if (!i) {
	final_analysis.Load( infile[0] );
	final_analysis.SetName( "Final" );}
      else {
	Analysis dummy_analysis;
	dummy_analysis.Load( infile[i] );
	final_analysis.Add( dummy_analysis );
      }
    }
  }
  final_analysis.SetName( name );
  final_analysis.Save( outName );

  return 0;
}
