#include "ZeeAnalysis/Analysis.h"
#include "TFile.h"
#include "ZeeAnalysis/Selection.h"
#include "ZeeAnalysis/SideFunctions.h"
#include "TString.h"
#include "TCanvas.h"


using std::cout;
using std::endl;
using std::vector;
using std::string;


Analysis::Analysis() : m_tevent( xAOD::TEvent::kClassAccess), m_numEvent(0),
		       m_goodEvent(0)
{
  m_tfile.clear();

  //Initialize histograms
  m_ZMassRaw = new TH1F ("ZMassRaw", "ZMassRaw", 40, 80, 100); //Masses in GeV
  m_ZMassRaw->GetXaxis()->SetTitle("M_{ee} [GeV]");
  m_ZMassRaw->GetYaxis()->SetTitle("Event/0.5 GeV");
  m_ZMassRaw->Sumw2();
}

Analysis::Analysis( string name ) : Analysis()  {
  m_name = name;
  m_ZMassRaw->SetName( TString( "ZMassRaw_" + m_name ) );
  m_ZMassRaw->SetTitle( TString( "ZMassRaw_" + m_name ) );
}

//=================================================================
Analysis::Analysis ( string name, string infileName) : Analysis(name) {

  cout << "Adding file" << endl;
  try {
    AddFile( infileName );
  }//try
  catch (int code) {
  }//catch

}//Analysis

  //==========================================================
Analysis::Analysis( string name, vector< string > v_infileName ) : Analysis(name) {

  cout << "Input file : " << v_infileName.size() << endl;

  try {
    // Loop on file names to create TFiles
    for (unsigned int i = 0; i < v_infileName.size(); i++) {
      //Add one file to the list of TFile
      cout << v_infileName[i] << endl;
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
  
  ResetTEvent();
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

//======================================================
void Analysis::ResetTEvent() {
  m_tevent.readFrom( m_tfile.front() ).ignore();
}

//=======================================================
void Analysis::TreatEvents() {
  int currentEvent=0;
  //Loop on all TFile stored in the class
  for (unsigned int i_file = 0 ; i_file < m_tfile.size() ; i_file++) {

    // Set the TEvent on the current TFile
    m_tevent.readFrom( m_tfile[i_file] ).ignore();
    int nentries = m_tevent.getEntries();

    //Loop on all events of the TFile
    for (int i_event = 0 ; i_event < nentries ; i_event++) {
      currentEvent++;
      if (currentEvent % 1000 == 0 ) cout << "Event : " << currentEvent << endl;
 
      // Read event 
      m_tevent.getEntry( i_event );
      //Retrieve the electron container                                               
      const xAOD::ElectronContainer* eContainer = 0;
      if ( !m_tevent.retrieve( eContainer, "ElectronCollection" ).isSuccess() ){
	cout << "Can not retrieve ElectronContainer : ElectronCollection" << endl;
	exit(1);
      }// if retrieve                                                                 

      //copy the retrieved container in order to modify it.
      xAOD::ElectronContainer tmp_econt( *eContainer );
      
      if ( ! PassSelection( tmp_econt ) ) continue;
      else m_goodEvent++;

      m_ZMassRaw->Fill( ComputeZMass( tmp_econt ) );

    }//for i_event    
  }//for i_file

  cout << "Total Events Treated : " << m_numEvent << endl;
  cout << "Total Good Events    : " << m_goodEvent << endl;

}//TreatEvents

//=======================================================
void Analysis::SetName( string name ) { m_name = name; }
string Analysis::GetName() { return m_name; }


//=======================================================
void Analysis::PlotResult(string fileName) {

  //check that the last character of directory is '/'
  if ( fileName == "" ) fileName = m_name + ".root";

  TFile *outfile = new TFile( fileName.c_str() , "RECREATE"); 
  TCanvas *canvas = new TCanvas();

  m_ZMassRaw->Draw("E");
  canvas->SaveAs( TString( fileName.substr( 0, fileName.find_last_of ( "." )  ) + "_ZMassRaw.pdf" ) );

  m_ZMassRaw->Write( "", TObject::kOverwrite );
  outfile->Close();


}//plotresutl
