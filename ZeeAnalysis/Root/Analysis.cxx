#include "ZeeAnalysis/Analysis.h"
#include "TFile.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;

Analysis::Analysis() : m_tevent( xAOD::TEvent::kClassAccess), m_numEvent(0) {
  m_tfile.clear();
}

//=================================================================
Analysis::Analysis ( string infileName) : Analysis() {

  try {
    AddFile( infileName );
  }//try
  catch (int code) {
  }//catch
}//Analysis

  //==========================================================
Analysis::Analysis( vector< string > v_infileName ) : Analysis() {

  try {
    // Loop on file names to create TFiles
    for (unsigned int i = 0; i < v_infileName.size(); i++) {
      //Add one file to the list of TFile
      AddFile(v_infileName[i]);
	}// for ifile
  }//try 

  catch (int code) { //Crash as soon as one file is not correct
    //Clear the TFile pointers created so far
    while (m_tfile.size()) {
      delete m_tfile.back();
      m_tfile.pop_back(); 
    }//while
  }//catch
  
  //Reset the  TEvent to look at the first file
  m_tevent.readFrom( m_tfile.front() );

}
//===============================================
Analysis::~Analysis() {
  while ( m_tfile.size() ) {
    delete m_tfile.back();
    m_tfile.pop_back();
  }//while
}//~Analysis

//================================================
void Analysis::AddFile( string infileName ) {

  try {
    // Initialise the application:                                                 
    if ( ! xAOD::Init( infileName.c_str() ) ) throw 0;

    // Open the input file:                                                        
    m_tfile.push_back(0);
    m_tfile.back() = TFile::Open( infileName.c_str() );
    if ( ! m_tfile.back() ) throw 1;

    // TEvent reads the file
    if ( !  m_tevent.readFrom( m_tfile.back() ) ) throw 2;
  }//try

  catch (int code) {
    if (! code ) cout << "Initialization Failed" << endl;
    else {
      cout << "Root file not found" << endl;
      delete m_tfile.back();
      m_tfile.pop_back();
    }
  }//catch
  m_numEvent += m_tevent.getEntries();

}//Addfile


