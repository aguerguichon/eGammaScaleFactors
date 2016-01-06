#include "ZeeAnalysis/Analysis.h"
#include "TFile.h"
#include "ZeeAnalysis/SideFunctions.h"
#include "TString.h"
#include "TCanvas.h"
#include "TTree.h"
#include "GoodRunsLists/GoodRunsListSelectionTool.h"
#include <boost/program_options.hpp>
#include "TChain.h"
#include "xAODEgamma/EgammaDefs.h"
#include "CaloGeoHelpers/CaloSampling.h"
#include "../LineShapeTool/LineShapeTool/LineShapeTool.h"
#include "xAODTruth/TruthEventContainer.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::min;
namespace po = boost::program_options;


Analysis::Analysis() : m_tevent( xAOD::TEvent::kClassAccess), 
		       m_debug( false), m_name( "Analysis" ), m_numEvent(0), m_goodEvent(0),
		       m_doSmearing(false), m_doScaleFactor(false), m_electronID(1), m_esModel( "es2015PRE" ), m_ptCut( 27000 )
{
  if ( m_debug ) cout << "Analysis::Analysis()" << endl;
  //  cout << "m_debug : " << m_debug << endl;

  m_fileName.clear();

  m_logFile = 0;

  //Initialize depp copy container
  m_eContainer = 0;
  m_EgammaCalibrationAndSmearingTool = 0;
  m_grl = 0;
  m_selectionTree = 0;
  m_LHToolMedium2012 = 0;
  m_CutToolMedium2012 = 0;
  m_ZVertex = 0;
  m_eventInfo = 0;
  m_tfile = 0;
  m_pileup = 0;
  m_vtxTool = 0;
  m_electronSFReco = 0;
  m_electronSFID = 0;

  //Initialize histograms
  m_mapHist["ZMass"] = new TH1D ("ZMass", "ZMass", 40, 80, 100); //Masses in GeV
  m_mapHist["ZMass"]->GetXaxis()->SetTitle("M_{ee} [GeV]");
  m_mapHist["ZMass"]->GetYaxis()->SetTitle("Event/0.5 GeV");
  m_mapHist["ZMass"]->Sumw2();

  m_mapHist["elEta"] = new TH1D( "elEta", "elEta", 100, -5, 5 );
  m_mapHist["elEta"]->GetXaxis()->SetTitle( "#eta_{el}" );
  m_mapHist["elEta"]->GetYaxis()->SetTitle( "Event / 0.1" );
  m_mapHist["elEta"]->Sumw2();

  m_mapHist["elPt"] = new TH1D( "elPt", "elPt", 100, 0, 100 );
  m_mapHist["elPt"]->GetXaxis()->SetTitle( "pt_{el}" );
  m_mapHist["elPt"]->GetYaxis()->SetTitle( "Event / 1 GeV" );
  m_mapHist["elPt"]->Sumw2();

  m_mapHist["eventZVertex"] = new TH1D( "eventZVertex", "eventZVertex", 100, -250, 250 );
  m_mapHist["eventZVertex"]->GetXaxis()->SetTitle( "Z Primary Vertex" );
  m_mapHist["eventZVertex"]->GetYaxis()->SetTitle( "Event / 5 mm" );
  m_mapHist["eventZVertex"]->Sumw2();

  vector<TString> cutFlowNames = { "init", "GRL", "initEl", "mediumID", "eta", "pt",  "OQ", "2el", "charge", "ZVertex" };
  m_mapHist["cutFlow"] = new TH1D( "cutFlow", "cutFlow", cutFlowNames.size(), 0.5, cutFlowNames.size()+0.5);
  m_mapHist["cutFlow"]->GetXaxis()->SetTitle( "Cuts" );
  m_mapHist["cutFlow"]->GetYaxis()->SetTitle( "# Events" );
  for ( int iBin = 1; iBin <= m_mapHist["cutFlow"]->GetNbinsX(); iBin++ ) {
    m_mapHist["cutFlow"]->GetXaxis()->SetBinLabel( iBin, cutFlowNames[iBin-1] );
  }

  m_mapHist["puWeight"] = new TH1D( "puWeight", "puWeight", 100, 0, 2 );
  m_mapHist["puWeight"]->GetXaxis()->SetTitle( "puWeights" );

  m_mapHist["lineshapeWeight"] = new TH1D( "lineshapeWeight", "lineshapeWeight", 100, 0.9, 1.1 );
  m_mapHist["lineshapeWeight"]->GetXaxis()->SetTitle( "lineshapeWeights" );

  m_mapHist["vertexWeight"] = new TH1D ("vertexWeight", "vertexWeight", 100, 0, 2 ); 
  m_mapHist["vertexWeight"]->GetXaxis()->SetTitle("vertexWeight");
  m_mapHist["vertexWeight"]->Sumw2();

  m_mapHist["SFReco"] = new TH1D ("SFReco", "SFReco", 100, 0.98, 1.01 );
  m_mapHist["SFReco"]->GetXaxis()->SetTitle("SFReco");
  m_mapHist["SFReco"]->Sumw2();

  m_mapHist["SFID"] = new TH1D ("SFID", "SFID", 100, 0.8, 1.2 );
  m_mapHist["SFID"]->GetXaxis()->SetTitle("SFID");
  m_mapHist["SFID"]->Sumw2();


  m_mapVar["energy_1"]=0;
  m_mapVar["pt_1"]=0;
  m_mapVar["eta_1"]=0;
  m_mapVar["phi_1"]=0;
  m_mapVar["eta_cl_1"]=0;
  m_mapVar["eta_calo_1"]=0;
  m_mapVar["e1_1"]=0;
  m_mapVar["e2_1"]=0;
  m_mapVar["energy_2"]=0;
  m_mapVar["pt_2"]=0;
  m_mapVar["eta_2"]=0;
  m_mapVar["phi_2"]=0;
  m_mapVar["eta_cl_2"]=0;
  m_mapVar["eta_calo_2"]=0;
  m_mapVar["e1_2"]=0;
  m_mapVar["e2_2"]=0;
  m_mapVar["m12"]=0;
  m_mapVar["SFReco"]=1;
  m_mapVar["SFID"]=1;
  m_mapVar["weight"]=1;
  m_mapVar["puWeight"]=1;
  m_mapVar[ "vertexWeight" ] = 1;
  m_mapVar["lineshapeWeight"]=1;
  m_mapLongVar["runNumber"]=0;
  m_mapLongVar["eventNumber"]=0;
  m_mapVar["m12_True"]=0;

  if ( m_debug ) cout << "Analysis::Analysis() Done" << endl;  
}

Analysis::Analysis( string name , string outputFile ) : Analysis() {
  if ( m_debug ) cout << "Analysis::Analysis( string name , string outputFile )" << endl;
  m_name = name;
  for ( auto it = m_mapHist.begin(); it != m_mapHist.end(); it++) 
    if ( it->second ) it->second->SetName( TString( m_name + "_" + it->second->GetName() ) );

  m_logFile = new TFile( outputFile.c_str(), "RECREATE" );  
  if ( m_debug ) cout << "Analysis::Analysis( string name , string outputFile ) Done" << endl;
}

//=================================================================
Analysis::Analysis ( string name, string infileName, string outputFile ) : Analysis(name, outputFile) {
  if ( m_debug ) cout << "Analysis::Analysis ( string name, string infileName, string outputFile )" << endl;
  cout << "Adding file" << endl;
  try {
    AddFile( infileName );
  }//try
  catch (int code) {
  }//catch
  if ( m_debug ) cout << "Analysis::Analysis ( string name, string infileName, string outputFile ) Done" << endl;
}//Analysis

//==========================================================
Analysis::Analysis( string name, vector< string > v_infileName, string outputFile ) : Analysis(name, outputFile) {
  if ( m_debug ) cout << "Analysis::Analysis( string name, vector< string > v_infileName, string outputFile )" << endl;
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
    exit(code);
  }//catch
  
  ResetTEvent();
  if ( m_debug ) cout << "Analysis::Analysis( string name, vector< string > v_infileName, string outputFile ) Done" << endl;
}
//===============================================
Analysis::~Analysis() {

  for ( auto it = m_mapHist.begin(); it != m_mapHist.end(); it++)  {
    if ( it->second ) delete it->second;
    it->second = 0;
  }
  
  if ( m_EgammaCalibrationAndSmearingTool )  delete  m_EgammaCalibrationAndSmearingTool;
  if ( m_grl )  delete m_grl;
  if ( m_LHToolMedium2012 ) delete m_LHToolMedium2012;
  if ( m_selectionTree ) delete m_selectionTree;
  //  if ( m_pileup ) delete m_pileup;
  if ( m_electronSFReco ) delete m_electronSFReco;
  if ( m_electronSFID ) delete m_electronSFID;
  if ( m_vtxTool ) delete m_vtxTool;

  if ( m_tfile ) {
    m_tfile->Close();
    delete m_tfile;
    m_tfile = 0;
  }

  if ( m_logFile ) {
    m_logFile->Close();
    delete m_logFile;
    m_logFile = 0;
  }

}//~Analysis

//================================================
void Analysis::AddFile( string infileName ) {

  try {
    if ( !m_fileName.size() )  if ( !xAOD::Init( infileName.c_str() ) ) throw 0;

    // Open the input file:                                                        
    m_fileName.push_back( infileName );
    if ( m_tfile ) {
      //      m_tfile->Close();
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

//=====================================================
void Analysis::ResetTEvent() {
  if ( m_debug ) cout << "Analysis::ResetTevent" << endl;
  delete m_tfile;
  m_tfile = TFile::Open( m_fileName.front().c_str() );
  m_tevent.readFrom( m_tfile ).ignore();
  if ( m_debug ) cout << "Analysis::ResetTevent" << endl;
}

//=======================================================
void Analysis::SetName( string name ) { 
  if ( m_debug ) cout << "Analysis::SetName" << endl;
  m_name = name; 
  if ( m_debug )  cout << "Changed name : " << m_name << endl;

  for ( auto it = m_mapHist.begin(); it != m_mapHist.end(); it++)  {
    if ( it->second ) it->second->SetName( TString( m_name + "_" + it->second->GetTitle() ) );
    if ( m_debug && it->second)   cout << it->second->GetName() << endl;
  }

  if ( m_selectionTree ) m_selectionTree->SetName( TString (m_name + "_" + m_selectionTree->GetTitle() ) ); 
}

//=======================================================
void Analysis::PlotResult(string fileName) {

  //check that the last character of directory is '/'
  if ( fileName == "" ) fileName = m_name;


  TCanvas *canvas = new TCanvas();

  for ( auto it = m_mapHist.begin(); it != m_mapHist.end(); it++)  {
    it->second->Draw("E");
    canvas->SaveAs( TString( fileName + "_" + it->second->GetName() + ".pdf" ) );
  }

  delete canvas;
}//plotresutl
 

//=====================================================================
void Analysis::Save( ) {
  if ( m_debug ) cout << "Analysis::Save()" << endl;
  m_logFile->cd();

  for ( auto it = m_mapHist.begin(); it != m_mapHist.end(); it++)  {
    it->second->Write( "", TObject::kOverwrite );
  }

  TString bufferName = TString::Format( "%s", m_name.c_str() );
  TTree * treeout = new TTree( "InfoTree" , "InfoTree" );
  treeout->Branch( "m_name", &bufferName, "m_name/C" );
  treeout->Branch( "m_numEvent", &m_numEvent, "m_numEvent/l" );
  treeout->Branch( "m_goodEvent", &m_goodEvent, "m_goodEvent/l" );
  treeout->Branch( "m_doScaleFactor", &m_doScaleFactor);
  treeout->Branch( "m_doSmearing", &m_doSmearing);

  treeout->Fill();
  treeout->Write( "", TObject::kOverwrite );
  delete treeout;
  treeout = 0;

  //Save Ntuple
  m_selectionTree->Write( "", TObject::kOverwrite );
  m_logFile->SaveSelf();
  cout << "Saved in : " << m_logFile->GetName() << endl;
}//void 


//========================================================================
int Analysis::Load( string fileName ) {

  //Open the storing root file
  TFile *infile = TFile::Open( fileName.c_str() );
  if ( !infile ) return 1;
  if ( m_debug ) cout << "file " << fileName << " opened" << endl;

  // Read Analysis info from Info Tree
  if ( infile->Get( "InfoTree" ) ){
    if ( m_debug ) cout << "Read Info Tree" << endl;
    char buffer_name[100];
    TTree *treeout = (TTree*) infile->Get( "InfoTree" );
    treeout->SetBranchStatus( "*", 1);
    treeout->SetBranchAddress( "m_name", &buffer_name);
    treeout->SetBranchAddress( "m_numEvent", &m_numEvent);
    treeout->SetBranchAddress( "m_goodEvent", &m_goodEvent);
    treeout->SetBranchAddress( "m_doScaleFactor", &m_doScaleFactor );      
    treeout->SetBranchAddress( "m_doSmearing", &m_doSmearing );      
    treeout->GetEntry(0);
    //    m_name = string( buffer_name );
    delete treeout;
    treeout = 0;
  }
  else return 2;
  
  for ( auto it = m_mapHist.begin(); it != m_mapHist.end(); it++)  {
    if ( !it->second ) continue;
    string histName = m_name + "_" + it->second->GetTitle();


    if ( !infile->Get( histName.c_str() ) ) continue;
    delete it->second;
    it->second = (TH1D*) infile->Get( histName.c_str() )->Clone();  
    it->second->SetDirectory( 0 );
    if ( m_debug ) cout << "Loaded histogram : " << it->second->GetName() << endl;
      
  }//for ihist


  if ( m_selectionTree ) delete m_selectionTree;
  m_selectionTree = (TTree* ) infile->Get( TString( m_name + "_selectionTree" ) );
  if ( !m_selectionTree ) return 4;
  m_selectionTree->SetDirectory( 0 );

  infile->Close("R");
  delete infile;
  infile = 0;

  if ( m_debug ) cout << "Loaded : " << fileName << endl;
  return 0;
}//Load


//========================================================
void Analysis::Add( Analysis &analysis ) {

  // Add input files names
  for ( unsigned int i = 0; i < analysis.m_fileName.size(); i++) {
    m_fileName.push_back( m_fileName[i] );
  }

  // reset the tevent on the first file if exists
  if ( m_tfile ) delete m_tfile;
  if ( m_fileName.size() )  m_tfile = TFile::Open( m_fileName[0].c_str() );
  else m_tfile = 0;
  if ( m_tfile ) m_tevent.readFrom( m_tfile );

  //Add events counters
  m_numEvent += analysis.m_numEvent;
  m_goodEvent += analysis.m_goodEvent;

  //Add histograms
  for ( auto it = m_mapHist.begin(); it != m_mapHist.end(); it++)  {
       it->second->Add( analysis.m_mapHist[it->first] );
  }

  //Add selectionTree's
  int dumSelectionTreeEntries = m_selectionTree->GetEntries();
  //  m_selectionTree->CopyEntries( analysis.m_selectionTree );
  TList *list = new TList();
  list->Add( m_selectionTree );
  list->Add( analysis.m_selectionTree );
  m_selectionTree->Merge( list );
  if ( dumSelectionTreeEntries + analysis.m_selectionTree->GetEntries() != m_selectionTree->GetEntries() ) exit( 1 );

  if ( m_debug ) cout << analysis.m_name << " Added" << endl;
}//Analysis

//=======================================================
void Analysis::TreatEvents(int nevent) {
  if ( m_debug ) cout << "Analysis::TreatEvents" << endl;
  int currentEvent=0;
  
  InitializeTools();
  cout << "treat event initialize" << endl;
  //Setup m_SelectionTree
  m_selectionTree = new TTree( TString( m_name.c_str() ) + "_selectionTree", "selectionTree" ); 
  m_selectionTree->SetDirectory( 0 );
  for ( map<string, double>::iterator it = m_mapVar.begin(); it != m_mapVar.end(); it++) {
    m_selectionTree->Branch( it->first.c_str(), &it->second );
  }
  for ( map<string, long long int>::iterator it = m_mapLongVar.begin(); it != m_mapLongVar.end(); it++) {
    m_selectionTree->Branch( it->first.c_str(), &it->second );
  }

  //Loop on all TFile stored in the class
  for (unsigned int i_file = 0 ; i_file < m_fileName.size() ; i_file++) {
    cout << "file : " << i_file << " " << m_tfile->GetName() << endl;
    //Set the new file in the current file pointer
    if ( m_tfile ) { 
      delete m_tfile; 
      m_tfile = 0;
    }
    m_tfile = TFile::Open( m_fileName[i_file].c_str() );
    // Set the TEvent on the current TFile
    m_tevent.readFrom( m_tfile ).ignore();
    int nentries = m_tevent.getEntries();

    //Loop on all events of the TFile
    for (int i_event = 0 ; i_event < nentries ; i_event++) {
      currentEvent++;
      //      cout << "event : " << i_event << endl;
      if ( currentEvent == nevent ) return;
      if (currentEvent % 1000 == 0 ) {
	cout << "Event : " << currentEvent << endl;
	//cout << m_eventInfo->runNumber() << " " << m_eventInfo->eventNumber()  << endl;
      }
      // Read event 
      m_tevent.getEntry( i_event );


      //Retrieve the electron container                  
      //RELEASE      
      if ( ! m_tevent.retrieve( m_eContainer, "Electrons" ).isSuccess() ){ cout << "Can not retrieve ElectronContainer : ElectronCollection" << endl; exit(1); }// if retrieve                                                                 
      if ( ! m_tevent.retrieve( m_eventInfo, "EventInfo" ).isSuccess() ){ cout << "Can Not retrieve EventInfo" << endl; exit(1); }
      if ( ! m_tevent.retrieve( m_ZVertex, "PrimaryVertices" ).isSuccess() ){ cout << "Can Not retrieve Vertex Info" << endl; exit(1); }


      //Create a shallow copy
      // Allow to modify electrons properties for calibration
      m_eShallowContainer = xAOD::shallowCopyContainer( *m_eContainer );

      //GRL

      m_mapHist["cutFlow"]->Fill( "init", 1 );
      if ( ! m_eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ) {//test if is data
	if ( !m_grl->passRunLB(*m_eventInfo) ) continue;  //passes the GRL
	if ( ( m_eventInfo->errorState(xAOD::EventInfo::LAr)==xAOD::EventInfo::Error ) 
	     || (m_eventInfo->errorState(xAOD::EventInfo::Tile)==xAOD::EventInfo::Error ) 
	     || (m_eventInfo->isEventFlagBitSet(xAOD::EventInfo::Core, 18) )  )
	  continue;
      }
	
      m_mapHist["cutFlow"]->Fill( "GRL", 1 );
      //      Make the electron selection and fill m_eGoodContainer
      int err = (int) PassSelection();
      if ( m_debug ) cout << "PassSelection : " << err << endl;
      if ( err )  { //Pass selection return a boolean 

      m_goodEvent++;
      //Should not contain events in bin 0

      m_mapHist["ZMass"]->Fill( ComputeZMass( m_veGood ) );
      if ( FillSelectionTree() ) {
	cout << "Error Filling selectionTree" << endl;
	exit(2);
      }
      }      
      
      // Free the memory from copy
      if ( m_eShallowContainer.first )  delete m_eShallowContainer.first;
      if ( m_eShallowContainer.second ) delete m_eShallowContainer.second;

      //Reset elecron vector to size 0
      while ( m_veGood.size() ) {
	//delete m_veGood.back();
	m_veGood.pop_back();
      }
      m_veGoodWeight.clear();
      //      map<string, int>::iterator it;
      for ( auto it = m_mapVar.begin(); it != m_mapVar.end(); it++ ) it->second = 1;
    }//for i_event    
  }//for i_file

  cout << "Total Events Treated : " << m_numEvent << endl;
  cout << "Total Good Events    : " << m_goodEvent << endl;

  if ( m_debug ) cout << "Analysis::TreatEvents Done" << endl;
}//TreatEvents


//==========================================================
//Make selection on event level
bool Analysis::PassSelection() {
  if ( m_debug ) cout << "Analysis::PassSelection()" << endl;
  m_mapHist["eventZVertex"]->Fill( (*m_ZVertex)[0]->auxdecor< float >("beamPosZ") );
  //m_mapHist["eventZVertex"]->Fill( (*m_ZVertex)[0]->z() );
  //Reduce number of electron in container by appling cuts on electrons
  MakeElectronCut();

  //Request exactly two electrons
  if ( m_veGood.size() != 2 ) return false;
  m_mapHist["cutFlow"]->Fill( "2el", 1 );
  //  Check the sign of the two electrons
  if ( m_veGood[0]->charge() *  m_veGood[1]->charge() > 0 ) return false;
  m_mapHist["cutFlow"]->Fill( "charge", 1 );
  //Cut on the position of the primary Vertex
  if ( m_esModel.find( "2015" ) == string::npos && fabs( (*m_ZVertex)[0]->z() ) > 150 ) return false;
    m_mapHist["cutFlow"]->Fill( "ZVertex", 1 );

  //if (ComputeZMass( m_veGood ) > 100 || ComputeZMass( m_veGood ) < 80) return false;
  if ( m_debug ) cout << "Analysis::PassSelection done" << endl;
  return true;
}

//===================================================================
//Make selection on electron level
void Analysis::MakeElectronCut() {
  if ( m_debug ) cout << "Analysis::MakeElectronCut" << endl;
  for ( xAOD::ElectronContainer::iterator eContItr = (m_eShallowContainer.first)->begin(); eContItr != (m_eShallowContainer.first)->end(); eContItr++ ) {

    m_mapHist["cutFlow"]->Fill( "initEl", 1 );
    //  Cut on the quality of the **eContItrectron
    if ( !(m_electronID/3)  && !m_LHToolMedium2012->accept( **eContItr ) ) continue;
    if ( (m_electronID/3) && !m_CutToolMedium2012->accept( **eContItr ) ) continue;
    
    m_mapHist["cutFlow"]->Fill( "mediumID", 1 );

    //Calibrate this new electron
    //    cout << (*eContItr)->pt() << " ";
    m_EgammaCalibrationAndSmearingTool->applyCorrection( **eContItr );
    //    cout << (*eContItr)->pt() << endl;
    m_mapHist["elEta"]->Fill( (*eContItr)->eta() );
    m_mapHist["elPt"]->Fill( (*eContItr)->pt() /1000 );
    
    //kinematical cuts on electrons
    double eta_calo = 0;
    (*eContItr)->caloCluster()->retrieveMoment(xAOD::CaloCluster::ETACALOFRAME,eta_calo); 
    if ( fabs( eta_calo ) > 2.47 ) continue;
    m_mapHist["cutFlow"]->Fill( "eta", 1 );

    if ( (*eContItr)->pt() < 27e3 ) continue;
    m_mapHist["cutFlow"]->Fill( "pt", 1 );
    
    //  OQ cut
    if ( !(*eContItr)->isGoodOQ( xAOD::EgammaParameters::BADCLUSELECTRON ) ) continue;
    m_mapHist["cutFlow"]->Fill( "OQ", 1 );
    
    m_veGood.push_back( 0 );
    m_veGood.back() = *eContItr;
    m_veGoodWeight.push_back( 1 );
  }

  if ( m_debug ) cout << "Analysis::MakeElectronCut done" << endl;
}//MakeKincut


//========================================================================
void Analysis::Divide( Analysis  &analysis ) {

  for ( auto it = m_mapHist.begin(); it != m_mapHist.end(); it++ ) { 

    analysis.m_mapHist[it->first]->Scale( 1 / analysis.m_mapHist[it->first]->Integral() );    
    it->second->Scale( 1 / it->second->Integral() );
    
    it->second->Divide( analysis.m_mapHist[it->first] );

    it->second->SetTitle( TString( "Ratio") + TString(it->second->GetTitle()) );

    it->second->GetYaxis()->SetTitle( TString( "Data/MC" ) );
  }
}

//=======================================================================
int Analysis::FillSelectionTree() {
  if ( m_debug ) cout << "Analysis::FillSelectionTree" << endl;
  //RELEASE
  if ( m_veGood.size() != 2 ) return 1;
  m_mapVar["pt_1"] = m_veGood[0]->pt();
  m_mapVar["eta_1"] = m_veGood[0]->eta();
  m_mapVar["phi_1"] = m_veGood[0]->phi();
  m_mapVar["eta_cl_1"] = m_veGood[0]->caloCluster()->eta();
  m_veGood[0]->caloCluster()->retrieveMoment(xAOD::CaloCluster::ETACALOFRAME,m_mapVar["eta_calo_1"]); 
  m_mapVar["e1_1"] = m_veGood[0]->caloCluster()->energyBE(1);
  m_mapVar["e2_1"] = m_veGood[0]->caloCluster()->energyBE(2);
  m_mapVar["energy_1"] = m_veGood[0]->e();

  m_mapVar["pt_2"] = m_veGood[1]->pt();
  m_mapVar["eta_2"] = m_veGood[1]->eta();
  m_mapVar["phi_2"] = m_veGood[1]->phi();
  m_mapVar["eta_cl_2"] = m_veGood[1]->caloCluster()->eta();
  m_veGood[1]->caloCluster()->retrieveMoment(xAOD::CaloCluster::ETACALOFRAME,m_mapVar["eta_calo_2"]); 
  m_mapVar["e1_2"] = m_veGood[1]->caloCluster()->energyBE(1);
  m_mapVar["e2_2"] = m_veGood[1]->caloCluster()->energyBE(2);
  m_mapVar["energy_2"] = m_veGood[1]->e();

  m_mapLongVar["runNumber"] = m_eventInfo->runNumber();
  m_mapLongVar["eventNumber"] = m_eventInfo->eventNumber();
  m_mapVar["m12"] = ComputeZMass( m_veGood );


  if ( m_eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ) {
    if ( m_pileup ) {
      m_pileup->apply( *m_eventInfo );
      m_mapVar["puWeight"] = m_eventInfo->auxdecor< float >( "PileupWeight" );
      m_mapHist["puWeight"]->Fill( m_mapVar["puWeight"] );
    }

    if ( m_vtxTool ) {
      m_vtxTool->getWeight( m_mapVar[ "vertexWeight" ] );
      m_mapHist["vertexWeight"]->Fill( m_mapVar[ "vertexWeight" ] );
    }
    
    if ( m_electronSFReco ) {
      double sf1, sf2;
      m_electronSFReco->getEfficiencyScaleFactor(*m_veGood[0],sf1);
      m_electronSFReco->getEfficiencyScaleFactor(*m_veGood[1],sf2);
      m_mapHist["SFReco"]->Fill( sf1 );
      m_mapHist["SFReco"]->Fill( sf2 );
      m_mapVar["SFReco"] = sf1*sf2;
    }

    if ( m_electronSFID ) {
      double sf1, sf2;
      m_electronSFID->getEfficiencyScaleFactor(*m_veGood[0],sf1);
      m_electronSFID->getEfficiencyScaleFactor(*m_veGood[1],sf2);
      m_mapHist["SFID"]->Fill( sf1 );
      m_mapHist["SFID"]->Fill( sf2 );
      m_mapVar["SFID"] = sf1*sf2;
    }

  }

  m_mapVar["weight"] = m_mapVar["lineshapeWeight"]*m_mapVar["puWeight"]*m_mapVar["vertexWeight"]*m_mapVar["SFReco"]*m_mapVar["SFID"];
  m_selectionTree->Fill();
  if ( m_debug ) cout << "Analysis::FillSelectionTree done" << endl;
  return 0;
}

//=====================================================
int Analysis::InitializeTools () {

  //Setup the calibration tool
  m_EgammaCalibrationAndSmearingTool  = new CP::EgammaCalibrationAndSmearingTool("EgammaCalibrationAndSmearingTool"); 
  m_EgammaCalibrationAndSmearingTool->setProperty("ESModel", m_esModel.c_str()); 
  m_EgammaCalibrationAndSmearingTool->setProperty("ResolutionType", "SigmaEff90"); 
  m_EgammaCalibrationAndSmearingTool->setProperty( "doSmearing", m_doSmearing );
  m_EgammaCalibrationAndSmearingTool->setProperty( "doScaleCorrection", m_doScaleFactor );
  m_EgammaCalibrationAndSmearingTool->initialize();

  //Setup the electron ID tool  
  string IDselection;
  switch ( m_electronID %3 ) {
  case 0 :
    IDselection = "Loose";
    break;
  case 1 :
    IDselection = "Medium";
    break;
  case 2 :
    IDselection = "Tight";
    break;
  }
  m_LHToolMedium2012 = new AsgElectronLikelihoodTool ("m_LHToolMedium2012"); 
  // initialize the primary vertex container for the tool to have access to the number of vertices used to adapt cuts based on the pileup
  m_LHToolMedium2012->setProperty("primaryVertexContainer","PrimaryVertices");
  string confDir = "ElectronPhotonSelectorTools/offline/mc15_20150712/";
  m_LHToolMedium2012->setProperty("ConfigFile",confDir+"ElectronLikelihood" + IDselection + "OfflineConfig2015.conf");
  m_LHToolMedium2012->initialize();

  m_CutToolMedium2012 = new  AsgElectronIsEMSelector("m_CutToolMedium2012"); // create the selector
  // set the config file that contains the cuts on the shower shapes 
  m_CutToolMedium2012->setProperty("ConfigFile",confDir+"ElectronIsEM" + IDselection + "SelectorCutDefs.conf"); 
  m_CutToolMedium2012->initialize();

  //Setup the GRL 
  m_grl = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");
  std::vector<std::string> vecStringGRL;
  string grlLocalFile = "/afs/in2p3.fr/home/c/cgoudet/private/eGammaScaleFactors/";
  vector< string > grlFile = {
    "data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml",
    "data15_13TeV.periodAllYear_DetStatus-v73-pro19-08_DQDefects-00-01-02_PHYS_StandardGRL_All_Good_25ns.xml"
  };
  bool isLocal = system( ("ls " + grlFile[0]).c_str() );    
  
  for ( unsigned int i = 0; i< grlFile.size(); i++ ) 
    vecStringGRL.push_back( string( isLocal ? grlLocalFile : "" ) + grlFile[i] );
  
  m_grl->setProperty( "GoodRunsListVec", vecStringGRL);
  m_grl->setProperty("PassThrough", false); // if true (default) will ignore result of GRL and will just pass all events
  if (!m_grl->initialize().isSuccess()) { // check this isSuccess
    cout << "Erreur : GRL not initialized" << endl;
    exit(1);
  }

  Info("","Declaring pileup reweighting tool");
  m_pileup = new CP::PileupReweightingTool("prw");
  std::vector<std::string> confFiles;
  std::vector<std::string> lcalcFiles;
  if ( m_esModel.find( "2015" ) == string::npos ) {
    m_pileup->setProperty("DataScaleFactor",1./1.09);    //    m_pileup.SetDataScaleFactors(1/1.09); // For 2012
    confFiles.push_back("PileupReweighting/mc14v1_defaults.prw.root");
    lcalcFiles.push_back("ilumicalc_histograms_None_200842-215643.root");
  }
  else {
    m_pileup->setProperty("DataScaleFactor",1./1.16);
    // confFiles.push_back( ( isLocal ? grlLocalFile : "" ) + "PileUpReweighting_50ns_prw.root");
    // confFiles.push_back( ( isLocal ? grlLocalFile : "" ) + "PileUpReweighting_25nsa_prw.root");
    confFiles.push_back( ( isLocal ? grlLocalFile : "" ) + "PileUpReweighting_25nsb_prw.root");
    lcalcFiles.push_back( ( isLocal ? grlLocalFile : "" ) + "ilumicalc_histograms_None_13TeV_25ns.root");
    //    lcalcFiles.push_back( ( isLocal ? grlLocalFile : "" ) + "ilumicalc_histograms_None_13TeV_50ns.root");
  }
  dynamic_cast<CP::PileupReweightingTool&>(*m_pileup).setProperty( "ConfigFiles", confFiles);
  dynamic_cast<CP::PileupReweightingTool&>(*m_pileup).setProperty( "LumiCalcFiles", lcalcFiles);
  
  m_pileup->initialize();


  m_vtxTool = new CP::VertexPositionReweightingTool( "VertexPosition" );  
  m_vtxTool->setProperty("DataMean", -25.7903);
  m_vtxTool->setProperty("DataSigma", 43.7201);


  m_electronSFReco = new AsgElectronEfficiencyCorrectionTool( "AsgElectronEfficiencyCorrectionTool" ) ;
  m_electronSFID = new AsgElectronEfficiencyCorrectionTool( "AsgElectronEfficiencyCorrectionTool" ) ;
  //define input file
  vector<string> filePerTool;
  filePerTool.push_back( "ElectronEfficiencyCorrection/efficiencySF.offline.RecoTrk.2015.13TeV.rel20p0.25ns.v02.root" );
  filePerTool.push_back( "ElectronEfficiencyCorrection/efficiencySF.offline.MediumLLH_d0z0.2015.13TeV.rel20p0.25ns.v02.root" );

  vector<AsgElectronEfficiencyCorrectionTool*> dumVectorTool = { m_electronSFReco, m_electronSFID };
  for ( unsigned int iTool = 0; iTool < dumVectorTool.size(); iTool++ ) {
    vector<string> inputFile = { filePerTool[iTool] };
    dumVectorTool[iTool]->setProperty("CorrectionFileNameList",inputFile);
    //set datatype, 0-Data(or dont use the tool - faster), 1-FULLSIM, 3-AF2
    dumVectorTool[iTool]->setProperty("ForceDataType",1);
    //if you want to use the apply function and put a prefix/name (default: "") to the new branch (ends with "SF"), not needed otherwise
    dumVectorTool[iTool]->setProperty("ResultPrefix", "my");
    dumVectorTool[iTool]->setProperty("ResultName", "name" );
    //init the tool
    dumVectorTool[iTool]->initialize();
  }
  return 0;
}

//====================================================
double Analysis::GetLineShapeWeight() {
  //Code adapted from JB devivie

  class TrueInfo {
  public:
    TrueInfo() { pdg1 = 0; pdg2 = 0; }
    ~TrueInfo() { };
    
    int pdg1, pdg2;
    TLorentzVector e11,e12,z1,e31,e32,z3;
  };
  TrueInfo *GetHPInfo();
  
  
  double weight =1;
  TrueInfo *ti = new TrueInfo();


  const xAOD::TruthEventContainer *teC(0);
  if ( m_tevent.retrieve(teC,"TruthEvents").isFailure()) {
    Info("","Cannot retrieve TruthEvents");
    exit( 1 );
  }

  const xAOD::TruthEvent* te = teC->front();
  const xAOD::TruthVertex* tvtx = te->signalProcessVertex();
  if (tvtx) {
    TString iN[2] = { "In ", "Out" };
    long unsigned int n[2] = { tvtx->nIncomingParticles(), tvtx->nOutgoingParticles() };
    for (int it = 0; it < 2; it++) {
      for (long unsigned int i = 0; i < n[it]; i++) {
	const xAOD::TruthParticle *tp = i == 0 ? tvtx->incomingParticle(i) : tvtx->outgoingParticle(i);
	Info("","%s pdg = %i, e = %3.2f, m = %3.2f",iN[i].Data(),tp->pdgId(),tp->e()*1e-3,tp->m()*1e-3);
      }
    }
  } else {
    //Info("","No signal vertex");
    //xAOD::TruthEvent::PdfInfo pdfinfo = te->pdfInfo();
    //Info("","pdg1 = %i, pdg2 = %i",pdfinfo.pdgId1,pdfinfo.pdgId2);
    const xAOD::TruthParticleContainer *tpC(0);
    if ( m_tevent.retrieve(tpC,"TruthParticles").isFailure()) {
      Info("","Cannot retrieve TruthParticles");
      exit( 1 );
    }
    for (const auto *const tp : *tpC) {
      if (tp->pdgId() != 23) continue;
      if (tp->status() < 30 && tp->status() > 20) {
	ti->z3 = tp->p4();
	const xAOD::TruthVertex* proVtx = tp->prodVtx();
	if (proVtx) {
	  int nP = proVtx->nIncomingParticles();
	  if (nP > 2)
	    Info("","more than two incoming particles in the Z production vertex ! %i",nP);
	  int pdg[2] = { 0, 0 };
	  for (int ii = 0; ii < min(2,nP); ii++) 
	    pdg[ii] = proVtx->incomingParticle(ii)->pdgId();
	  ti->pdg1 = pdg[0];
	  ti->pdg2 = pdg[1];
	}
      }
      if (tp->status() == 62) 
	ti->z1 = tp->p4();

      if ( tp->status() == 62 || ( tp->status() < 30  && tp->status() > 20 ) ) {
	const xAOD::TruthVertex* decVtx = tp->decayVtx();
	if (decVtx) {
	  int nP = decVtx->nIncomingParticles();
	  int ne = 0;
	  TLorentzVector el[2];
	  for (int ii = 0; ii < nP; ii++) {
	    if (abs(decVtx->outgoingParticle(ii)->pdgId()) == 11) {
	      if (ne < 2)
		el[ne] = decVtx->outgoingParticle(ii)->p4();
	      ne++;
	    }
	  }
	  if (ne > 2)
	    Info("","more than two electrons in Z decays");
	  if (tp->status() == 62) {
	    ti->e11 = el[0];
	    ti->e12 = el[1];
	  } else {
	    ti->e31 = el[0];
	    ti->e32 = el[1];
	  }
	}
      }
    }
  }    

  LineShapeTool *lineShapeTool = new LineShapeTool();
  m_mapVar["m12_True"] = ti->z3.M()*1e-3;
  weight = lineShapeTool->reweightPowhegToImprovedBorn(ti->pdg1,ti->pdg2,ti->z3.M()*1e-3);
  m_mapHist["lineshapeWeight"]->Fill( weight );
  //  cout << "lineshapeWeight : " << weight << endl;
  return weight;
}
