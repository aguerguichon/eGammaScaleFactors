#include "ZeeAnalysis/Analysis.h"
#include "TFile.h"
#include "ZeeAnalysis/SideFunctions.h"
#include "TString.h"
#include "TCanvas.h"
#include "TTree.h"
#include "GoodRunsLists/GoodRunsListSelectionTool.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;

Analysis::Analysis() : m_tevent( xAOD::TEvent::kClassAccess), 
		       m_numEvent(0), m_goodEvent(0), debug(0)
{
  m_fileName.clear();

  //Initialize depp copy container
  m_eContainer = 0;

  m_EgammaCalibrationAndSmearingTool  = new CP::EgammaCalibrationAndSmearingTool("EgammaCalibrationAndSmearingTool"); 
  m_EgammaCalibrationAndSmearingTool->setProperty("ESModel", "es2012c"); 
  m_EgammaCalibrationAndSmearingTool->setProperty("ResolutionType", "SigmaEff90"); 
  m_EgammaCalibrationAndSmearingTool->initialize();

  // GRL
  m_grl = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");
  std::vector<std::string> vecStringGRL;
  vecStringGRL.push_back("/afs/cern.ch/user/a/atlasdqm/grlgen/All_Good/data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml");
  m_grl->setProperty( "GoodRunsListVec", vecStringGRL);
  m_grl->setProperty("PassThrough", false); // if true (default) will ignore result of GRL and will just pass all events
  if (!m_grl->initialize().isSuccess()) { // check this isSuccess
    cout << "Erreur : GRL not initialized" << endl;
    exit(1);
  }

  m_eventInfo = 0;
  m_tfile = 0;

  //Initialize histograms
  m_ZMass = new TH1F ("ZMass", "ZMass", 40, 80, 100); //Masses in GeV
  m_ZMass->GetXaxis()->SetTitle("M_{ee} [GeV]");
  m_ZMass->GetYaxis()->SetTitle("Event/0.5 GeV");
  m_ZMass->Sumw2();
  v_hist.push_back( m_ZMass );

}

Analysis::Analysis( string name , int debug ) : Analysis()  {
  m_name = name;
  m_debug = debug;
  for (unsigned int ihist = 0; ihist < v_hist.size() ; ihist++) {
    v_hist[ihist]->SetName( TString( m_name + "_" + v_hist[ihist]->GetName() ) );
  }
}

//=================================================================
Analysis::Analysis ( string name, string infileName, int debug) : Analysis(name, debug) {

  cout << "Adding file" << endl;
  try {
      AddFile( infileName );
  }//try
  catch (int code) {
  }//catch

}//Analysis

  //==========================================================
Analysis::Analysis( string name, vector< string > v_infileName ) : Analysis(name, debug) {

  cout << "Input file : " << v_infileName.size() << endl;

  try {
    // Loop on file names to create TFiles
    for (unsigned int i = 0; i < v_infileName.size(); i++) {
      //Add one file to the list of TFile
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

  delete  m_EgammaCalibrationAndSmearingTool;
  delete m_tfile;

  if( m_grl ) {
    delete m_grl; m_grl = 0; }

  }//~Analysis

//================================================
void Analysis::AddFile( string infileName ) {

  try {
    if ( !m_fileName.size() )  if ( !xAOD::Init( infileName.c_str() ) ) throw 0;

    // Open the input file:                                                        
    m_fileName.push_back( infileName );
    if ( m_tfile ) {
      m_tfile->Close();
      delete m_tfile;
      m_tfile = 0;}
    m_tfile = TFile::Open( m_fileName.back().c_str() );
    
    if ( !  m_tevent.readFrom( m_tfile ) ) throw 2;
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
  delete m_tfile;
  m_tfile = new TFile( m_fileName.front().c_str() );
  m_tevent.readFrom( m_tfile ).ignore();
}

//=======================================================
void Analysis::TreatEvents(int nevent) {
  int currentEvent=0;


  //Loop on all TFile stored in the class
  for (unsigned int i_file = 0 ; i_file < m_fileName.size() ; i_file++) {

    //Set the new file in the current file pointer
    delete m_tfile; m_tfile = 0;
    m_tfile = new TFile( m_fileName[i_file].c_str() );
    // Set the TEvent on the current TFile
    m_tevent.readFrom( m_tfile ).ignore();
    int nentries = m_tevent.getEntries();

    //Loop on all events of the TFile
    for (int i_event = 0 ; i_event < nentries ; i_event++) {
      currentEvent++;
      if ( currentEvent == nevent ) return;
      if (currentEvent % 1000 == 0 ) cout << "Event : " << currentEvent << endl;
 
      // Read event 
      m_tevent.getEntry( i_event );

      //Retrieve the electron container                  
      if ( ! m_tevent.retrieve( m_eContainer, "ElectronCollection" ).isSuccess() ){ cout << "Can not retrieve ElectronContainer : ElectronCollection" << endl; exit(1); }// if retrieve                                                                 
      if ( ! m_tevent.retrieve( m_eventInfo, "EventInfo" ).isSuccess() ){ cout << "Can Not retriev EventInfo" << endl; exit(1); }

      //Create a shallow copy
      // Allow to modify electrons properties for calibration
      m_eShallowContainer = xAOD::shallowCopyContainer( *m_eContainer );

      //Initialize calibration Tool
      m_EgammaCalibrationAndSmearingTool->setDefaultConfiguration( m_eventInfo );
      m_EgammaCalibrationAndSmearingTool->forceSmearing( false );
      m_EgammaCalibrationAndSmearingTool->forceScaleCorrection( false );
      
      //GRL
      if ( ! m_eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) //test if is data
      	   && !m_grl->passRunLB(*m_eventInfo) )  //passes the GRL
      	continue;
      
      //      Make the electron selection and fill m_eGoodContainer
      if ( PassSelection() )  {
      	m_goodEvent++;
      	//Should not contain events in bin 0
      	m_ZMass->Fill( ComputeZMass( m_veGood ) );
      }

      // Free the memory from copy
      if ( m_eShallowContainer.first )  delete m_eShallowContainer.first;
      if ( m_eShallowContainer.second ) delete m_eShallowContainer.second;

      //Reset elecron vector to size 0
      m_veGood.clear();

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

  delete canvas;
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
  delete treeout;
  outfile->Close();
  delete outfile; 
}//void 


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
    delete infile;
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
//Make selection on event level
bool Analysis::PassSelection() {


  //Reduce number of electron in container by appling cuts on electrons
  MakeElectronCut();

  //Request exactly two electrons
  if ( m_veGood.size() != 2 ) return false;

  //  Check the sign of the two electrons
  if ( m_veGood[0]->charge() *  m_veGood[1]->charge() > 0 ) return false;

  if (ComputeZMass( m_veGood ) > 100 || ComputeZMass( m_veGood ) < 80) return false;

  return true;
}

//===================================================================
//Make selection on electron level
void Analysis::MakeElectronCut() {

  for ( xAOD::ElectronContainer::iterator eContItr = (m_eShallowContainer.first)->begin(); eContItr != (m_eShallowContainer.first)->end(); eContItr++ ) {

    // //Calibrate this new electron
    m_EgammaCalibrationAndSmearingTool->applyCorrection( **eContItr, m_eventInfo);

    // //Make the selection of electron 
    if ( !isGoodElectron( **eContItr ) ) continue;

    //Fill the vector of good electrons
    m_veGood.push_back( 0 );
    m_veGood.back() = *eContItr;
  }

}//MakeKincut

//==================================================================

bool Analysis::isGoodElectron( xAOD::Electron const & el ) {

  //kinematical cuts on electrons
  if ( fabs( el.eta() ) > 2.47 ) return false;
  if ( el.pt() < 27e3 ) return false;
  
  // //  Author cut
  if ( el.author() != 1 && el.author() != 3 ) return false;
  
  //  Cut on the quality of the electron
   bool selected = false;
  if ( ! el.passSelection(selected, "Medium") ) return false;
  if ( !selected ) return false;

  // //  OQ cut
  // // if ( !isGoodOQ()) return false;
  
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
