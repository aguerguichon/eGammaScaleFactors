#include "ZeeAnalysis/Analysis.h"
#include "TFile.h"
#include "ZeeAnalysis/SideFunctions.h"
#include "TString.h"
#include "TCanvas.h"
#include "TTree.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;


Analysis::Analysis() : m_tevent( xAOD::TEvent::kClassAccess), m_numEvent(0),
		       m_goodEvent(0)
{
  m_tfile.clear();

  //Initialize histograms
  m_ZMass = new TH1F ("ZMass", "ZMass", 40, 80, 100); //Masses in GeV
  m_ZMass->GetXaxis()->SetTitle("M_{ee} [GeV]");
  m_ZMass->GetYaxis()->SetTitle("Event/0.5 GeV");
  m_ZMass->Sumw2();

  m_EPerEventBFSel = new TH1F ("EPerEventBFSel", "EPerEventBFSel", 10, -0.5, 9.5); //Masses in GeV
  m_EPerEventBFSel->GetXaxis()->SetTitle("electrons");
  m_EPerEventBFSel->GetYaxis()->SetTitle("Events");
  m_EPerEventBFSel->Sumw2();

  m_EPerEventAFSel = new TH1F ("EPerEventAFSel", "EPerEventAFSel", 10, -0.5, 9.5); //Masses in GeV
  m_EPerEventAFSel->GetXaxis()->SetTitle("electrons");
  m_EPerEventAFSel->GetYaxis()->SetTitle("Events");
  m_EPerEventAFSel->Sumw2();

}

Analysis::Analysis( string name ) : Analysis()  {
  m_name = name;

  m_ZMass->SetName( TString( m_name + "_ZMass" ) );
  m_EPerEventBFSel->SetName( TString( m_name + "_EPerEventBFSel" ) );
  m_EPerEventAFSel->SetName( TString( m_name + "_EPerEventAFSel" ) );
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

  delete m_ZMass;
  delete m_EPerEventBFSel;
  delete m_EPerEventAFSel;

}//~Analysis

//================================================
void Analysis::AddFile( string infileName ) {

  try {
    if ( !m_tfile.size() )  if ( !xAOD::Init( infileName.c_str() ) ) throw 0;

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
void Analysis::TreatEvents(int nevent) {
  int currentEvent=0;
  //Loop on all TFile stored in the class
  for (unsigned int i_file = 0 ; i_file < m_tfile.size() ; i_file++) {

    // Set the TEvent on the current TFile
    m_tevent.readFrom( m_tfile[i_file] ).ignore();
    int nentries = ( nevent ) ? nevent : m_tevent.getEntries();

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

      m_EPerEventBFSel->Fill( tmp_econt.size() );

      if ( ! PassSelection( tmp_econt ) ) {
	m_EPerEventAFSel->Fill( tmp_econt.size() );
	continue;}
      else {
	m_EPerEventAFSel->Fill( tmp_econt.size() );
	m_goodEvent++;
      }

      m_ZMass->Fill( ComputeZMass( tmp_econt ) );

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
  if ( fileName == "" ) fileName = m_name;


  TCanvas *canvas = new TCanvas();

  m_ZMass->Draw("E");
  canvas->SaveAs( TString( fileName + "_ZMass.pdf" ) );

  m_EPerEventBFSel->Draw("E");
  canvas->SaveAs( TString( fileName + "_EPerEventBFSel.pdf" ) );

  m_EPerEventAFSel->Draw("E");
  canvas->SaveAs( TString( fileName + "_EPerEventAFSel.pdf" ) );



}//plotresutl
 

//=====================================================================
void Analysis::Save( string fileName ) {
  if ( fileName == "" ) fileName = m_name + ".root";

  TFile *outfile = new TFile( fileName.c_str() , "RECREATE"); 

  m_ZMass->Write( "", TObject::kOverwrite );
  m_EPerEventBFSel->Write( "", TObject::kOverwrite );
  m_EPerEventAFSel->Write( "", TObject::kOverwrite );

  char buffer_name[100];
  sprintf( buffer_name, "%s", m_name.c_str() );
  TTree * treeout = new TTree( "_InfoTree" , "InfoTree" );
  treeout->Branch( "m_name", &buffer_name, "m_name/C" );
  treeout->Branch( "m_numEvent", &m_numEvent, "m_numEvent/I" );
  treeout->Branch( "m_goodEvent", &m_goodEvent, "m_goodEvent/I" );

  treeout->Fill();
  treeout->Write( "", TObject::kOverwrite );

  outfile->Close();
}


//void 


//========================================================================
void Analysis::Load( string fileName ) {

  try {
    TFile *infile = TFile::Open( fileName.c_str() );
    if ( !infile ) throw 1;

    if ( infile->Get( "InfoTree" ) ){
      char buffer_name[100];
      TTree *treeout = (TTree*) infile->Get( "InfoTree" );
      treeout->SetBranchStatus( "*", 1);
      treeout->SetBranchAddress( "m_name", &buffer_name);
      treeout->SetBranchAddress( "m_numEvent", &m_numEvent);
      treeout->SetBranchAddress( "m_goodEvent", &m_goodEvent);

      treeout->GetEntry(0);
      m_name = string( buffer_name );
      delete treeout;
    }
    else throw 3;
    
    if ( infile->Get( TString(m_name + "_ZMass" ) ) ) {
      delete m_ZMass;
      m_ZMass = (TH1F*) infile->Get( TString( m_name + "_ZMass" ) );
    }
    else throw 2;


    if ( infile->Get( TString( m_name + "_EPerEventBFSel" ) ) ) {
      delete m_EPerEventBFSel;
      m_EPerEventBFSel = (TH1F*) infile->Get( TString( m_name + "_EPerEventBFSel" ) );
    }
    else throw 4;

    if ( infile->Get( TString( m_name + "_EPerEventAFSel" ) ) ) {
      delete m_EPerEventAFSel;
      m_EPerEventAFSel = (TH1F*) infile->Get( TString( m_name + "_EPerEventAFSel" ) );
    }
    else throw 5;

  }//try
  catch (int code) {
    switch (code) {
    case 1 : 
      cout << "TFile Not found" << endl;
      break;
    case 2 :
      cout << "ZMass hist do not exist" << endl;
      break;
    case 3 :
      cout << "InfoTree do not exist" << endl;
      break;
    case 4 :
      cout << "EPerEventBFSel do not exist" << endl;
      break;
    case 5 :
      cout << "EPerEventAFSel do not exist" << endl;
      break;
    default : break;
    }
  }

  cout << "Loaded : " << fileName << endl;
}//Load


//========================================================
void Analysis::Add( Analysis const &analysis ) {

  m_numEvent += analysis.m_numEvent;
  m_goodEvent += analysis.m_goodEvent;


  m_ZMass->Add( analysis.m_ZMass );
  m_EPerEventBFSel->Add( analysis.m_EPerEventBFSel );
  m_EPerEventAFSel->Add( analysis.m_EPerEventAFSel );


}//Analysis

//==========================================================
bool Analysis::PassSelection( xAOD::ElectronContainer &eContainer ) {

  //Reduce number of electron in container by appling kinematical cuts
  MakeKinCut(eContainer);

  //Request exactly two electrons
  if (eContainer.size()!=2) return false;
  if (ComputeZMass(eContainer) > 100 || ComputeZMass(eContainer) < 80) return false;

  return true;
}
