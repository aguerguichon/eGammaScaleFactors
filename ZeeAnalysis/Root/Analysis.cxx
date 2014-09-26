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
  m_fileName.clear();

  //Initialize histograms
  m_ZMass = new TH1F ("ZMass", "ZMass", 40, 80, 100); //Masses in GeV
  m_ZMass->GetXaxis()->SetTitle("M_{ee} [GeV]");
  m_ZMass->GetYaxis()->SetTitle("Event/0.5 GeV");
  m_ZMass->Sumw2();
  v_hist.push_back( m_ZMass );

  m_ZMassRaw = new TH1F ("ZMassRaw", "ZMassRaw", 40, 80, 100); //Masses in GeV
  m_ZMassRaw->GetXaxis()->SetTitle("M_{ee} [GeV]");
  m_ZMassRaw->GetYaxis()->SetTitle("Event/0.5 GeV");
  m_ZMassRaw->Sumw2();
  v_hist.push_back( m_ZMassRaw );

  m_EPerEventBFSel = new TH1F ("EPerEventBFSel", "EPerEventBFSel", 10, -0.5, 9.5); //Masses in GeV
  m_EPerEventBFSel->GetXaxis()->SetTitle("electrons");
  m_EPerEventBFSel->GetYaxis()->SetTitle("Events");
  m_EPerEventBFSel->Sumw2();
  v_hist.push_back( m_EPerEventBFSel );

  m_EPerEventAFSel = new TH1F ("EPerEventAFSel", "EPerEventAFSel", 10, -0.5, 9.5); //Masses in GeV
  m_EPerEventAFSel->GetXaxis()->SetTitle("electrons");
  m_EPerEventAFSel->GetYaxis()->SetTitle("Events");
  m_EPerEventAFSel->Sumw2();
  v_hist.push_back( m_EPerEventAFSel );
}

Analysis::Analysis( string name ) : Analysis()  {
  m_name = name;

  for (unsigned int ihist = 0; ihist < v_hist.size() ; ihist++) {
    v_hist[ihist]->SetName( TString( m_name + "_" + v_hist[ihist]->GetName() ) );
  }
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
    cout << "Crash" << endl;
    //Clear the TFile pointers created so far
    while (m_fileName.size()) {
      m_fileName.pop_back(); 
    }//while
  }//catch
  
  ResetTEvent();
}
//===============================================
Analysis::~Analysis() {
  for ( unsigned int ihist = 0; ihist < v_hist.size(); ihist++ ) {
    delete v_hist[ihist];
  }

}//~Analysis

//================================================
void Analysis::AddFile( string infileName ) {

  try {
    if ( !m_fileName.size() )  if ( !xAOD::Init( infileName.c_str() ) ) throw 0;

    // Open the input file:                                                        
    m_fileName.push_back( infileName );

    if ( !  m_tevent.readFrom( TFile::Open( m_fileName.back().c_str() ) ) ) throw 2;
  }//try

  catch (int code) {
    if (! code ) cout << "Initialization Failed" << endl;
    else {
      cout << "Root file not found" << endl;
      m_fileName.pop_back();
    }
  }//catch
  m_numEvent += m_tevent.getEntries();

}//Addfile

//======================================================
void Analysis::ResetTEvent() {
  m_tevent.readFrom( TFile::Open( m_fileName.front().c_str() ) ).ignore();
}

//=======================================================
void Analysis::TreatEvents(int nevent) {
  int currentEvent=0;
  //Loop on all TFile stored in the class
  for (unsigned int i_file = 0 ; i_file < m_fileName.size() ; i_file++) {

    // Set the TEvent on the current TFile
    m_tevent.readFrom( TFile::Open( m_fileName[i_file].c_str() ) ).ignore();
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
      m_ZMassRaw->Fill( ComputeZMass( tmp_econt ) );

      if ( ! PassSelection( tmp_econt ) ) continue;
      else m_goodEvent++;


      m_ZMass->Fill( ComputeZMass( tmp_econt ) );

    }//for i_event    
  }//for i_file

  cout << "Total Events Treated : " << m_numEvent << endl;
  cout << "Total Good Events    : " << m_goodEvent << endl;

}//TreatEvents

//=======================================================
void Analysis::SetName( string name ) { 
m_name = name; 

 for (unsigned int ihist = 0; ihist < v_hist.size(); ihist++ ) {
   v_hist[ihist]->SetName( TString( m_name + "_" + v_hist[ihist]->GetTitle() ) );
 }
}
string Analysis::GetName() { return m_name; }


//=======================================================
void Analysis::PlotResult(string fileName) {

  //check that the last character of directory is '/'
  if ( fileName == "" ) fileName = m_name;


  TCanvas *canvas = new TCanvas();

  for (unsigned int ihist = 0; ihist < v_hist.size() ; ihist++) {
    v_hist[ihist]->Draw("E");
    canvas->SaveAs( TString( fileName + "_" + v_hist[ihist]->GetName() + ".pdf" ) );
  }

}//plotresutl
 

//=====================================================================
void Analysis::Save( string fileName ) {
  if ( fileName == "" ) fileName = m_name + ".root";

  TFile *outfile = new TFile( fileName.c_str() , "RECREATE"); 

  for (unsigned int ihist = 0; ihist < v_hist.size() ; ihist++) {
    v_hist[ihist]->Write( "", TObject::kOverwrite );
  }

  char buffer_name[100];
  sprintf( buffer_name, "%s", m_name.c_str() );
  TTree * treeout = new TTree( "InfoTree" , "InfoTree" );
  treeout->Branch( "m_name", &buffer_name, "m_name/C" );
  treeout->Branch( "m_numEvent", &m_numEvent, "m_numEvent/I" );
  treeout->Branch( "m_goodEvent", &m_goodEvent, "m_goodEvent/I" );

  treeout->Fill();
  treeout->Write( "", TObject::kOverwrite );

  cout << "Written on : " << outfile->GetName() << endl;
  outfile->Close();
 
}


//void 


//========================================================================
void Analysis::Load( string fileName ) {

  try {
    TFile *infile = TFile::Open( fileName.c_str() );
    if ( !infile ) throw -1;

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
    else throw -2;

  for (unsigned int ihist = 0; ihist < v_hist.size() ; ihist++) {
    string buffer = v_hist[ihist]->GetTitle();
    if ( infile->Get( TString(m_name + "_" + v_hist[ihist]->GetTitle() ) ) ) {
      delete v_hist[ihist];
      v_hist[ihist] = (TH1F*) infile->Get( TString(m_name + "_" + buffer ) );  
    }
    else throw (int) ihist;
  }
  }//try
  catch (int code) {
    switch (code) {
    case -1 : 
      cout << "TFile Not found" << endl;
      break;
    case -2 :
      cout << "InfoTree do not exist" << endl;
      break;
    default : 
      cout << "Histogram not found : " << endl;
      break;
    }
  }

  cout << "Loaded : " << fileName << endl;
}//Load


//========================================================
void Analysis::Add( Analysis const &analysis ) {

  m_numEvent += analysis.m_numEvent;
  m_goodEvent += analysis.m_goodEvent;

  for (unsigned int ihist = 0; ihist < v_hist.size(); ihist++ ) {
    v_hist[ihist]->Add( analysis.v_hist[ihist] );
  }

}//Analysis

//==========================================================
bool Analysis::PassSelection( xAOD::ElectronContainer &eContainer ) {

  //Reduce number of electron in container by appling cuts on electrons
  MakeElectronCut(eContainer);

  m_EPerEventAFSel->Fill( eContainer.size() );
  //Request exactly two electrons
  if (eContainer.size()!=2) return false;

  //Check the sign of the two electrons
  if ( ( (*eContainer[0]).charge() > 0 && (*eContainer[1]).charge() > 0 )
       || ( (*eContainer[1]).charge() < 0 && (*eContainer[0]).charge() < 0 ) )
    return false;
  
  if (ComputeZMass(eContainer) > 100 || ComputeZMass(eContainer) < 80) return false;

  return true;
}

//===================================================================
void Analysis::MakeElectronCut( xAOD::ElectronContainer &eContainer ) {
  for (unsigned int i=0; i<eContainer.size(); i++) {
    if ( !isGoodElectron( *eContainer[i] ) ) {  
      eContainer.erase(eContainer.begin()+i);
      i--;
    }
  }
}//MakeKincut

//==================================================================

bool Analysis::isGoodElectron( xAOD::Electron &el ) {

  //kinematical cuts on electrons
  if ( el.eta() > 2.47 ) return false;
  if ( el.pt() < 27e3 ) return false;
  
  //Author cut
  if ( el.author() != 1 && el.author() != 3 ) return false;
  
  //Cut on the quality of the electron
  bool selected = false;
  if ( ! el.passSelection(selected, "Medium") ) return false;
  if ( !selected ) return false;
  
  //OQ cut
  //    if ( !isGoodOQ()) return false;
  
  return true;
}

//========================================================================
void Analysis::Divide( Analysis  &analysis ) {

  for ( unsigned int ihist = 0; ihist < v_hist.size(); ihist++ ) {

    analysis.v_hist[ihist]->Scale( 1 / analysis.v_hist[ihist]->Integral() );    

    v_hist[ihist]->Scale( 1 / v_hist[ihist]->Integral() );
    
    v_hist[ihist]->Divide( analysis.v_hist[ihist] );

    v_hist[ihist]->SetTitle( TString( "Ratio") + TString(v_hist[ihist]->GetTitle()) );

    v_hist[ihist]->GetYaxis()->SetTitle( TString( "Data/MC" ) );
  }
}
