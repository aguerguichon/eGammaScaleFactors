#include "ZeeAnalysis/Analysis.h"
#include "ZeeAnalysis/SideFunctions.h"
#include "../LineShapeTool/LineShapeTool/LineShapeTool.h"

#include "TFile.h"
#include "TString.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TChain.h"

#include "GoodRunsLists/GoodRunsListSelectionTool.h"
#include "CaloGeoHelpers/CaloSampling.h"
#include "PATInterfaces/SystematicRegistry.h"
#include "PATInterfaces/SystematicVariation.h"
#include "PATInterfaces/SystematicsUtil.h"
#include "IsolationSelection/IsolationSelectionTool.h"
#include "xAODTracking/TrackParticlexAODHelpers.h"
#include "xAODTracking/Vertex.h"
#include "xAODTruth/TruthEventContainer.h"
#include "xAODEgamma/EgammaDefs.h"
#include "xAODRootAccess/tools/ReturnCheck.h"

#include <boost/program_options.hpp>

using std::ifstream;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::min;
namespace po = boost::program_options;


Analysis::Analysis() : m_tevent( xAOD::TEvent::kClassAccess), 
		       m_debug( false ),  m_numEvent(0), m_goodEvent(0),
		       m_doScaleFactor(0), m_electronID(1), m_ptCut( 27000 ), m_fBremCut( 1 ), 
		       m_scaleSyst(0), m_doIso(1)
{
  if ( m_debug ) cout << "Analysis::Analysis()" << endl;
  //  cout << "m_debug : " << m_debug << endl;
  m_mapString["name"] = "Analysis";
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
  m_electronSFIso = 0;
  m_electronSFTrig = 0;
  m_trigDecisionTool = 0;
  m_trigConfigTool = 0;
  m_Z=0;
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

  vector<TString> cutFlowNames = { "init", "GRL", "Trigger", "initEl", "fBrem", "Isolation", "d0","z0","mediumID",  "eta", "pt",  "OQ", "2el", "charge", "ZVertex" };
  m_mapHist["cutFlow"] = new TH1D( "cutFlow", "cutFlow", cutFlowNames.size(), 0.5, cutFlowNames.size()+0.5);
  m_mapHist["cutFlow"]->GetXaxis()->SetTitle( "Cuts" );
  m_mapHist["cutFlow"]->GetYaxis()->SetTitle( "# Events" );
  for ( int iBin = 1; iBin <= m_mapHist["cutFlow"]->GetNbinsX(); iBin++ ) {
    m_mapHist["cutFlow"]->GetXaxis()->SetBinLabel( iBin, cutFlowNames[iBin-1] );
  }

  m_mapHist["puWeight"] = new TH1D( "puWeight", "puWeight", 100, 0, 2 );
  m_mapHist["puWeight"]->GetXaxis()->SetTitle( "puWeights" );

  // m_mapHist["lineshapeWeight"] = new TH1D( "lineshapeWeight", "lineshapeWeight", 100, 0.9, 1.1 );
  // m_mapHist["lineshapeWeight"]->GetXaxis()->SetTitle( "lineshapeWeights" );

  m_mapHist["vertexWeight"] = new TH1D ("vertexWeight", "vertexWeight", 100, 0, 2 ); 
  m_mapHist["vertexWeight"]->GetXaxis()->SetTitle("vertexWeight");
  m_mapHist["vertexWeight"]->Sumw2();

  m_mapHist["SFReco"] = new TH1D ("SFReco", "SFReco", 100, 0.98, 1.01 );
  m_mapHist["SFReco"]->GetXaxis()->SetTitle("SFReco");
  m_mapHist["SFReco"]->Sumw2();

  m_mapHist["SFID"] = new TH1D ("SFID", "SFID", 100, 0.8, 1.2 );
  m_mapHist["SFID"]->GetXaxis()->SetTitle("SFID");
  m_mapHist["SFID"]->Sumw2();

  m_mapHist["fBrem"] = new TH1D ("fBrem", "fBrem", 100, 0, 1 );
  m_mapHist["fBrem"]->GetXaxis()->SetTitle("fBrem");
  m_mapHist["fBrem"]->Sumw2();


  if ( m_debug ) cout << "Analysis::Analysis() Done" << endl;  
}

Analysis::Analysis( string name , string outputFile ) : Analysis() {
  if ( m_debug ) cout << "Analysis::Analysis( string name , string outputFile )" << endl;
  m_mapString["name"] = name;
  for ( auto it = m_mapHist.begin(); it != m_mapHist.end(); it++) 
    if ( it->second ) it->second->SetName( TString( m_mapString["name"] + "_" + it->second->GetName() ) );

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
  if ( m_electronSFIso ) delete m_electronSFIso;
  if ( m_vtxTool ) delete m_vtxTool;
  if( m_trigConfigTool ) delete m_trigConfigTool;
  if( m_trigDecisionTool ) delete m_trigDecisionTool;

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
  m_mapString["name"] = name; 
  if ( m_debug )  cout << "Changed name : " << m_mapString["name"] << endl;

  for ( auto it = m_mapHist.begin(); it != m_mapHist.end(); it++)  {
    if ( it->second ) it->second->SetName( TString( m_mapString["name"] + "_" + it->second->GetTitle() ) );
    if ( m_debug && it->second)   cout << it->second->GetName() << endl;
  }

  if ( m_selectionTree ) m_selectionTree->SetName( TString (m_mapString["name"] + "_" + m_selectionTree->GetTitle() ) ); 
}

//=======================================================
void Analysis::PlotResult(string fileName) {

  //check that the last character of directory is '/'
  if ( fileName == "" ) fileName = m_mapString["name"];

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

  for ( auto it = m_mapHist.begin(); it != m_mapHist.end(); it++) it->second->Write( "", TObject::kOverwrite );

  TString bufferName = TString::Format( "%s", m_mapString["name"].c_str() );
  TTree * treeout = new TTree( "InfoTree" , "InfoTree" );
  treeout->Branch( "name", &bufferName, "name/C" );
  treeout->Branch( "m_numEvent", &m_numEvent, "m_numEvent/l" );
  treeout->Branch( "m_goodEvent", &m_goodEvent, "m_goodEvent/l" );
  treeout->Branch( "m_doScaleFactor", &m_doScaleFactor);

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
    treeout->SetBranchAddress( "name", &buffer_name);
    treeout->SetBranchAddress( "m_numEvent", &m_numEvent);
    treeout->SetBranchAddress( "m_goodEvent", &m_goodEvent);
    treeout->SetBranchAddress( "m_doScaleFactor", &m_doScaleFactor );      
    treeout->GetEntry(0);
    //    m_mapString["name"] = string( buffer_name );
    delete treeout;
    treeout = 0;
  }
  else return 2;
  
  for ( auto it = m_mapHist.begin(); it != m_mapHist.end(); it++)  {
    if ( !it->second ) continue;
    string histName = m_mapString["name"] + "_" + it->second->GetTitle();


    if ( !infile->Get( histName.c_str() ) ) continue;
    delete it->second;
    it->second = (TH1D*) infile->Get( histName.c_str() )->Clone();  
    it->second->SetDirectory( 0 );
    if ( m_debug ) cout << "Loaded histogram : " << it->second->GetName() << endl;
      
  }//for ihist


  if ( m_selectionTree ) delete m_selectionTree;
  m_selectionTree = (TTree* ) infile->Get( TString( m_mapString["name"] + "_selectionTree" ) );
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

  if ( m_debug ) cout << analysis.m_mapString["name"] << " Added" << endl;
}//Analysis

//=======================================================
void Analysis::TreatEvents(int nevent) {
  if ( m_debug ) cout << "Analysis::TreatEvents" << endl;
  int currentEvent=0;
  InitializeTools();
  cout << "treat event initialize" << endl;
  //Setup m_SelectionTree
  m_selectionTree = new TTree( TString( m_mapString["name"].c_str() ) + "_selectionTree", "selectionTree" ); 
  m_selectionTree->SetDirectory( 0 );

  //Loop on all TFile stored in the class
  for (unsigned int i_file = 0 ; i_file < m_fileName.size() ; i_file++) {
    cout << "file : " << i_file << " " << m_tfile->GetName() << endl;
    //Set the new file in the current file pointer
    if ( m_tfile ) { 
      delete m_tfile; 
      m_tfile = 0;
    }
    m_tfile = TFile::Open( m_fileName[i_file].c_str() );

    m_mapVar["datasetWeight"] = m_datasetWeight.size() ? ( i_file >= m_datasetWeight.size() ? m_datasetWeight.back() : m_datasetWeight[i_file] ) : 1;
   
    // Set the TEvent on the current TFile
    m_tevent.readFrom( m_tfile ).ignore();
    int nentries = m_tevent.getEntries();

    //Loop on all events of the TFile
    for (int i_event = 0 ; i_event < nentries ; i_event++) {
      currentEvent++;
      if ( currentEvent == nevent ) return;
      if (currentEvent % 1000 == 0 ) {
	cout << "Event : " << currentEvent << endl;
      }
      // Read event 
      m_tevent.getEntry( i_event );

      //Retrieve the electron container                  
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

      //Trigger
      bool passTrig = false;
      auto chainGroup = m_trigDecisionTool->getChainGroup( m_mapString["trigName"].c_str() );

      std::map<std::string,int> triggerCounts;
      for(auto &trig : chainGroup->getListOfTriggers()) {
	auto cg = m_trigDecisionTool->getChainGroup(trig);
	std::string thisTrig = trig;
	m_mapVar["TrigPS"] = cg->getPrescale();
	passTrig = passTrig || cg->isPassed();
	//	Info( "execute()", "%30s chain passed(1)/failed(0): %d total chain prescale (L1*HLT): %.1f", thisTrig.c_str(), cg->isPassed(), cg->getPrescale() );
      }
      if ( !passTrig ) continue;
      m_mapHist["cutFlow"]->Fill("Trigger", 1);


      //      Make the electron selection and fill m_eGoodContainer
      m_mapHist["cutFlow"]->Fill( "initEl", m_eContainer->size() );

      int err = (int) PassSelection();
      if ( m_debug ) cout << "PassSelection : " << err << endl;
      if ( err )  { //Pass selection return a boolean 

      m_goodEvent++;
      //Should not contain events in bin 0

      m_Z = new TLorentzVector();
      *m_Z = m_veGood.front()->p4() + m_veGood.back()->p4();
      m_mapHist["ZMass"]->Fill( m_Z->M() /1000. );
      if ( FillSelectionTree() ) {
	cout << "Error Filling selectionTree" << endl;
	exit(2);
      }
      }

      // Free the memory from copy
      if ( m_eShallowContainer.first )  delete m_eShallowContainer.first;
      if ( m_eShallowContainer.second ) delete m_eShallowContainer.second;
      delete m_Z; m_Z=0;
      //Reset elecron vector to size 0
      while ( m_veGood.size() ) {
	//delete m_veGood.back();
	m_veGood.pop_back();
      }
      m_veGoodWeight.clear();
      //      map<string, int>::iterator it;
      //      for ( auto it = m_mapVar.begin(); it != m_mapVar.end(); it++ ) it->second = 1;
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
  //Reduce number of electron in container by appling cuts on electrons
  MakeElectronCut();

  //Request exactly two electrons
  if ( m_veGood.size() != 2 ) return false;
  m_mapHist["cutFlow"]->Fill( "2el", 1 );
  //  Check the sign of the two electrons
  if ( m_veGood[0]->charge() *  m_veGood[1]->charge() > 0 ) return false;
  m_mapHist["cutFlow"]->Fill( "charge", 1 );
  //Cut on the position of the primary Vertex
  if ( fabs( (*m_ZVertex)[0]->z() ) > 150 ) return false;

  m_mapHist["cutFlow"]->Fill( "ZVertex", 1 );

  if ( m_debug ) cout << "Analysis::PassSelection done" << endl;
  return true;
}

//===================================================================
//Make selection on electron level
void Analysis::MakeElectronCut() {
  if ( m_debug ) cout << "Analysis::MakeElectronCut" << endl;
  for ( xAOD::ElectronContainer::iterator eContItr = (m_eShallowContainer.first)->begin(); eContItr != (m_eShallowContainer.first)->end(); eContItr++ ) {

    double fBrem = GetFBrem( *eContItr );
    m_mapHist["fBrem"]->Fill( fBrem );
    if ( fBrem > m_fBremCut  ) continue;
    m_mapHist["cutFlow"]->Fill( "fBrem", 1 );

  
    bool isIso =  m_IsoSelTool->accept(**eContItr) ? true : false;
    if ( !isIso && m_doIso ) continue;
    m_mapHist["cutFlow"]->Fill( "Isolation", 1 );

    double d0sign = Getd0significance ( *eContItr );
    if ( fabs(d0sign) > 5 ) continue;
    m_mapHist["cutFlow"]->Fill( "d0", 1 );

    double z0sin = Getz0sintheta ( *eContItr );
    if ( fabs(z0sin) > 0.5 ) continue;
    m_mapHist["cutFlow"]->Fill( "z0", 1 );

    //  Cut on the quality of the **eContItrectron
    //    cout << (m_electronID/3) << endl;
    if ( !(m_electronID/3)  && !m_LHToolMedium2012->accept( **eContItr ) ) continue;
    if ( (m_electronID/3) && !m_CutToolMedium2012->accept( **eContItr ) ) continue;
    
    m_mapHist["cutFlow"]->Fill( "mediumID", 1 );


    //Calibrate this new electron
    m_EgammaCalibrationAndSmearingTool->applyCorrection( **eContItr );
    m_mapHist["elEta"]->Fill( (*eContItr)->eta() );
    m_mapHist["elPt"]->Fill( (*eContItr)->pt() /1000 );
    
    //kinematical cuts on electrons
    double eta_calo = 0;
    (*eContItr)->caloCluster()->retrieveMoment(xAOD::CaloCluster::ETACALOFRAME,eta_calo); 
    if ( fabs( eta_calo ) > 2.47 ) continue;
    m_mapHist["cutFlow"]->Fill( "eta", 1 );

    if ( (*eContItr)->pt() < m_ptCut ) continue;
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

  for ( unsigned int iEl = 0; iEl < 2; iEl++ ) {
    m_mapVar[string(TString::Format( "pt_%d", iEl+1 ))] = m_veGood[iEl]->pt();
    m_mapVar[string(TString::Format( "eta_%d", iEl+1 ))] = m_veGood[iEl]->eta();
    m_mapVar[string(TString::Format( "phi_%d", iEl+1 ))] = m_veGood[iEl]->phi();
    m_mapVar[string(TString::Format( "eta_cl_%d", iEl+1 ))] = m_veGood[iEl]->caloCluster()->eta();
    m_veGood[iEl]->caloCluster()->retrieveMoment(xAOD::CaloCluster::ETACALOFRAME,m_mapVar[string(TString::Format( "eta_calo_%d", iEl+1 ))]); 
    m_mapVar[string(TString::Format( "e0_%d", iEl+1 ))] = m_veGood[iEl]->caloCluster()->energyBE(0);
    m_mapVar[string(TString::Format( "e1_%d", iEl+1 ))] = m_veGood[iEl]->caloCluster()->energyBE(1);
    m_mapVar[string(TString::Format( "e2_%d", iEl+1 ))] = m_veGood[iEl]->caloCluster()->energyBE(2);
    m_mapVar[string(TString::Format( "energy_%d", iEl+1 ))] = m_veGood[iEl]->e();

    //Get the fbrm variable as computed in the likelihood code
    m_mapVar[string(TString::Format("fBrem_%d", iEl+1))] = GetFBrem( m_veGood[iEl] );
   
    //Get information for the gain
    for (unsigned int iLr = 0; iLr < 4; iLr++){ 
      m_mapVar[string(TString::Format("E_Lr%d_HiG_%d", iLr,iEl+1))] = m_veGood[iEl]->auxdata<float>( string(TString::Format("E_Lr%d_HiG", iLr))  );
      m_mapVar[string(TString::Format("E_Lr%d_MedG_%d", iLr, iEl+1))] = m_veGood[iEl]->auxdata<float>( string(TString::Format("E_Lr%d_MedG", iLr))  );
      m_mapVar[string(TString::Format("E_Lr%d_LowG_%d", iLr, iEl+1))] = m_veGood[iEl]->auxdata<float>( string(TString::Format("E_Lr%d_LowG", iLr))  ); 
      m_mapVar[string(TString::Format("nCells_Lr%d_HiG_%d", iLr,iEl+1))] = m_veGood[iEl]->auxdata<unsigned char>( string(TString::Format("nCells_Lr%d_HiG", iLr))  );
      m_mapVar[string(TString::Format("nCells_Lr%d_MedG_%d", iLr, iEl+1))] = m_veGood[iEl]->auxdata<unsigned char>( string(TString::Format("nCells_Lr%d_MedG", iLr)) );
      m_mapVar[string(TString::Format("nCells_Lr%d_LowG_%d", iLr, iEl+1))] = m_veGood[iEl]->auxdata<unsigned char>( string(TString::Format("nCells_Lr%d_LowG", iLr)) ); 

    }
  }//end for iEl


  m_mapLongVar["runNumber"] = m_eventInfo->runNumber();
  m_mapLongVar["eventNumber"] = m_eventInfo->eventNumber();
  m_mapLongVar["timeStamp"] = m_eventInfo->timeStamp();

  m_mapVar["m12"] = m_Z->M() / 1000.;
  m_mapVar["ptZ"] = m_Z->Pt() / 1000.;

  m_mapVar["NVertex"] = m_ZVertex->size();
  m_mapVar["muPU"] = m_eventInfo->averageInteractionsPerCrossing();

  vector<string> weightNames = {  "SFIso", "SFReco", "SFID", "vertexWeight", "puWeight" };
  for ( auto name : weightNames ) m_mapVar[name]=1;  
  if ( m_eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ) {
    if ( m_pileup ) {
      m_pileup->apply( *m_eventInfo, 1 );
      m_mapVar["puWeight"] = m_eventInfo->auxdecor< float >( "PileupWeight" );
    }
    m_mapHist["puWeight"]->Fill( m_mapVar["puWeight"] );

    if ( m_vtxTool ) {
      m_vtxTool->getWeight( m_mapVar[ "vertexWeight" ] );
      m_mapHist["vertexWeight"]->Fill( m_mapVar[ "vertexWeight" ] );
    }

    vector<AsgElectronEfficiencyCorrectionTool*> dumVectorTool = { m_electronSFIso, m_electronSFReco, m_electronSFID };
 
    for ( unsigned int  iTool = 0; iTool < dumVectorTool.size(); iTool++ ) {
      double sf1=2, sf2=2;
      if ( !dumVectorTool[iTool] ) continue;
      dumVectorTool[iTool]->getEfficiencyScaleFactor(*m_veGood[0],sf1);
      dumVectorTool[iTool]->getEfficiencyScaleFactor(*m_veGood[1],sf2);
      m_mapVar[weightNames[iTool]] = sf1*sf2;

    }//end for
  }
  //End isSimulation
  else {

  }

  //  m_mapVar["weight"] = m_mapVar["lineshapeWeight"]*m_mapVar["puWeight"]*m_mapVar["SFReco"]*m_mapVar["SFID"]*m_mapVar["datasetWeight"];
  m_mapVar["weight"] = m_mapVar["puWeight"]*m_mapVar["SFReco"]*m_mapVar["SFID"]*m_mapVar["datasetWeight"]*m_mapVar["SFIso"]*m_mapVar["vertexWeight"];
  
  if ( !m_selectionTree->GetEntries() ) {
    for ( auto &it : m_mapVar ) {
      m_selectionTree->Branch( it.first.c_str(), &it.second );
    }
    for ( auto &it : m_mapLongVar ) {
      m_selectionTree->Branch( it.first.c_str(), &it.second );
    }
  }

  m_selectionTree->Fill();
  if ( m_debug ) cout << "Analysis::FillSelectionTree done" << endl;
  return 0;}


//=====================================================
int Analysis::InitializeTools () {

  //Setup the calibration tool
  m_EgammaCalibrationAndSmearingTool  = new CP::EgammaCalibrationAndSmearingTool("EgammaCalibrationAndSmearingTool"); 
  m_EgammaCalibrationAndSmearingTool->setProperty("ESModel", m_mapString["esModel"].c_str()); 
  cout << "esModel : " <<  m_mapString["esModel"] << endl;
  cout << "doScaleFactor : " << m_doScaleFactor << endl;
  m_EgammaCalibrationAndSmearingTool->setProperty("ResolutionType", "SigmaEff90"); 
  m_EgammaCalibrationAndSmearingTool->setProperty( "doSmearing", m_doScaleFactor );
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
  string confDir = "ElectronPhotonSelectorTools/offline/mc15_20160512/";
  m_LHToolMedium2012->setProperty("ConfigFile",confDir+"ElectronLikelihood" + IDselection + "OfflineConfig2016_Smooth.conf");
  cout << "configLkh : " << confDir+"ElectronLikelihood" + IDselection + "OfflineConfig2016_Smooth.conf" << endl;   
  
  m_LHToolMedium2012->initialize();


  m_CutToolMedium2012 = new  AsgElectronIsEMSelector("m_CutToolMedium2012"); // create the selector
  // set the config file that contains the cuts on the shower shapes 
  m_CutToolMedium2012->setProperty("ConfigFile","ElectronPhotonSelectorTools/offline/mc15_20151012/ElectronIsEM" + IDselection + "SelectorCutDefs.conf"); 
  m_CutToolMedium2012->initialize();


  //Setup the GRL 
  m_grl = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");
    
  m_grl->setProperty( "GoodRunsListVec", m_mapVectString["grl"]);
  m_grl->setProperty("PassThrough", false); // if true (default) will ignore result of GRL and will just pass all events
  if (!m_grl->initialize().isSuccess()) { // check this isSuccess
    cout << "Erreur : GRL not initialized" << endl;
    exit(1);
  }


  Info("","Declaring pileup reweighting tool");
  m_pileup = new CP::PileupReweightingTool("prw");
  m_pileup->setProperty( "DataScaleFactor", 1./m_dataPUSF );

  dynamic_cast<CP::PileupReweightingTool&>(*m_pileup).setProperty( "ConfigFiles", m_mapVectString["pileupFile"]);
  dynamic_cast<CP::PileupReweightingTool&>(*m_pileup).setProperty( "LumiCalcFiles", m_mapVectString["ilumCalc"]);
  m_pileup->initialize();


  // m_vtxTool = new CP::VertexPositionReweightingTool( "VertexPosition" );  
  // m_vtxTool->setProperty("DataMean", 0.);
  // m_vtxTool->setProperty("DataSigma", 0.);


  m_electronSFReco = new AsgElectronEfficiencyCorrectionTool( "SFReco" ) ;
  m_electronSFID = new AsgElectronEfficiencyCorrectionTool( "SFID" ) ;
  if ( m_doIso )  m_electronSFIso = new AsgElectronEfficiencyCorrectionTool( "SFIso" );
  m_electronSFTrig = new AsgElectronEfficiencyCorrectionTool( "SFTrig" ) ;

  //define input file
  vector<string> filePerTool;
  filePerTool.push_back( "ElectronEfficiencyCorrection/2015_2016/rel20.7/ICHEP_June2016_v3/isolation/efficiencySF.Isolation.LooseAndBLayerLLH_d0z0_v11_isolLoose.root");
  filePerTool.push_back( "ElectronEfficiencyCorrection/2015_2016/rel20.7/ICHEP_June2016_v3/offline/efficiencySF.offline.RecoTrk.root" );
  switch ( m_electronID ) {
  case 1 : 
    filePerTool.push_back( "ElectronEfficiencyCorrection/2015_2016/rel20.7/ICHEP_June2016_v3/offline/efficiencySF.offline.MediumLLH_d0z0_v11.root" );
    break;
  case 2 :
    filePerTool.push_back( "ElectronEfficiencyCorrection/2015_2016/rel20.7/ICHEP_June2016_v3/offline/efficiencySF.offline.TightLLH_d0z0_v11.root" );
    break;
  default :
    cout << "SFReco file not defined for m_electronID=" << m_electronID << endl;
    exit(0);
  }

  vector<AsgElectronEfficiencyCorrectionTool*> dumVectorTool = { m_electronSFIso, m_electronSFReco, m_electronSFID };

  vector<string> SFSyst = { "EL_EFF_Iso_CorrUncertainty__1up", "EL_EFF_Reco_TOTAL_UncorrUncertainty__1up", "EL_EFF_ID_TOTAL_UncorrUncertainty__1up" };
  for ( unsigned int iTool = 0; iTool < dumVectorTool.size(); iTool++ ) {
    if ( !dumVectorTool[iTool] ) continue;
    vector<string> inputFile = { filePerTool[iTool] };
    dumVectorTool[iTool]->setProperty("CorrectionFileNameList",inputFile);
    //set datatype, 0-Data(or dont use the tool - faster), 1-FULLSIM, 3-AF2
    dumVectorTool[iTool]->setProperty("ForceDataType",1);
    //if you want to use the apply function and put a prefix/name (default: "") to the new branch (ends with "SF"), not needed otherwise
    dumVectorTool[iTool]->setProperty("ResultPrefix", "my");
    dumVectorTool[iTool]->setProperty("ResultName", "name" );
    //init the tool
    dumVectorTool[iTool]->initialize();

    if ( m_scaleSyst == (int) iTool+1 ) {
      CP::SystematicVariation systVar( SFSyst[iTool] );
      dumVectorTool[iTool]->applySystematicVariation(CP::SystematicSet({systVar}));
    }
  }


  m_trigConfigTool = new TrigConf::xAODConfigTool("xAODConfigTool"); // gives us access to the meta-data
  ToolHandle< TrigConf::ITrigConfigTool > trigConfigHandle( m_trigConfigTool );
  m_trigDecisionTool = new Trig::TrigDecisionTool("TrigDecisionTool");
  // connect the TrigDecisionTool to the ConfigTool
  m_trigDecisionTool->setProperty( "ConfigTool", trigConfigHandle );
  m_trigDecisionTool->setProperty( "TrigDecisionKey", "xTrigDecision" );
  m_trigDecisionTool->initialize();


  m_IsoSelTool = new CP::IsolationSelectionTool("IsoSelTool");
  m_IsoSelTool->setProperty("ElectronWP","Loose").ignore();
  //OA  m_IsoSelTool->setProperty("doCutInterpolationElec",true).ignore(); // not the default, but prettier...
  m_IsoSelTool->initialize().ignore();

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
  //m_mapHist["lineshapeWeight"]->Fill( weight );
  //  cout << "lineshapeWeight : " << weight << endl;
  return weight;
}



//============================================================
double Analysis::GetFBrem( xAOD::Electron *el ) {
    //Get the fbrm variable as computed in the likelihood code
    const xAOD::TrackParticle* t = el->trackParticle();
    if (t) {
      float trackqoverp = t->qOverP();
      unsigned int index;
      if( t->indexOfParameterAtPosition(index, xAOD::LastMeasurement) ) {
	double refittedTrack_LMqoverp  = 
	  t->charge() / sqrt(std::pow(t->parameterPX(index), 2) +
			     std::pow(t->parameterPY(index), 2) +
			     std::pow(t->parameterPZ(index), 2));
	return  1 - trackqoverp/(refittedTrack_LMqoverp);
      }
    }
    return 2;

}

//==============================================================
double Analysis::Getd0significance ( xAOD::Electron *el ) 
{
      const xAOD::TrackParticle* t = el->trackParticle();
      if (t) 
	{
	  return xAOD::TrackingHelpers::d0significance( t, m_eventInfo->beamPosSigmaX(), m_eventInfo->beamPosSigmaY(), m_eventInfo->beamPosSigmaXY() );
	}
      cout<<"electron has no track"<<endl;
      return 2;
}

//==============================================================
double Analysis::Getz0sintheta ( xAOD::Electron *el )
{
  //https://twiki.cern.ch/twiki/bin/view/AtlasProtected/EGammaIdentificationRun2#Electron_d0_and_z0_cut_definitio
  //loop over vertices and look for good primary vertex
  for (xAOD::VertexContainer::const_iterator vxIter = m_ZVertex->begin(); vxIter != m_ZVertex->end(); ++vxIter) {
    // Select good primary vertex
    if ((*vxIter)->vertexType() == xAOD::VxType::PriVtx) {
      //This is the primary vertex
      const xAOD::TrackParticle* t = el->trackParticle();
      return (t->z0() + t->vz() - (*vxIter)->z())*sin(t->theta());
    }
  }
  cout <<"Getz0sintheta failed"<<endl;
  return 2;
}



//==============================================================
void Analysis::Configure( string configFile ) {
  cout << "configFile : " << configFile << endl;
  po::options_description configOptions("configOptions");
  configOptions.add_options()
    ("trigger", po::value<string>( &m_mapString["trigName"] ), "" )
    ("esModel", po::value< string >( &m_mapString["esModel"] ), "" ) 
    ("outName", po::value< string >( &m_mapString["name"] )->default_value( "Analysis" ), "Name of the object")
    ("doScale", po::value<int >( &m_doScaleFactor )->default_value( 0 )->implicit_value(1), "Switch on the scale")
    ("electronID", po::value< int >( &m_electronID )->default_value( 1 ), "" )
    ("ptCut", po::value<double>( &m_ptCut ), "" )
    ("fBremCut", po::value<double>( &m_fBremCut ), "" )
    ("datasetWeight", po::value<vector<double>>( &m_datasetWeight )->multitoken(), "" )
    ("pileupFile", po::value<vector<string> >( &m_mapVectString["pileupFile"] )->multitoken(), "" )
    ("scaleSyst", po::value<int>( &m_scaleSyst )->default_value(0), "" )
    ("doIso", po::value<int>( &m_doIso )->default_value(1), "" )
    ("dataPUSF", po::value<double>( &m_dataPUSF )->default_value(1), "" )
    ("ilumCalc", po::value<vector<string >>( &m_mapVectString["ilumCalc"] )->multitoken(), "" )
    ("grl", po::value<vector< string> >( &m_mapVectString["grl"] )->multitoken(), "" )
    ;

  po::variables_map vm;
  ifstream ifs( configFile, ifstream::in );
  po::store(po::parse_config_file(ifs, configOptions), vm);
  po::notify( vm );

}
