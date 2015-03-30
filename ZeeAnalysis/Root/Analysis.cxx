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

using std::cout;
using std::endl;
using std::vector;
using std::string;
namespace po = boost::program_options;


Analysis::Analysis() : m_tevent( xAOD::TEvent::kClassAccess), 
		       m_debug( false), m_numEvent(0), m_goodEvent(0), 
		       m_doSmearing(false), m_doScaleFactor(false), m_weight(1)
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
  m_ZVertex = 0;
  m_eventInfo = 0;
  m_tfile = 0;
  m_electronSF = 0;

  //Initialize histograms
  m_ZMass = new TH1F ("ZMass", "ZMass", 40, 80, 100); //Masses in GeV
  m_ZMass->GetXaxis()->SetTitle("M_{ee} [GeV]");
  m_ZMass->GetYaxis()->SetTitle("Event/0.5 GeV");
  m_ZMass->Sumw2();
  v_hist.push_back( m_ZMass );

  m_elEta = new TH1F( "elEta", "elEta", 100, -5, 5 );
  m_elEta->GetXaxis()->SetTitle( "#eta_{el}" );
  m_elEta->GetYaxis()->SetTitle( "Event / 0.1" );
  m_elEta->Sumw2();
  v_hist.push_back( m_elEta );

  m_elPt = new TH1F( "elPt", "elPt", 100, 0, 100 );
  m_elPt->GetXaxis()->SetTitle( "pt_{el}" );
  m_elPt->GetYaxis()->SetTitle( "Event / 1 GeV" );
  m_elPt->Sumw2();
  v_hist.push_back( m_elPt );

  m_eventZVertex = new TH1F( "eventZVertex", "eventZVertex", 100, -250, 250 );
  m_eventZVertex->GetXaxis()->SetTitle( "Z Primary Vertex" );
  m_eventZVertex->GetYaxis()->SetTitle( "Event / 5 mm" );
  m_eventZVertex->Sumw2();
  v_hist.push_back( m_eventZVertex );

  m_cutFlow = new TH1F( "cutFlow", "cutFlow", 9, 0.5, 9+0.5);
  m_cutFlow->GetXaxis()->SetTitle( "Cuts" );
  m_cutFlow->GetYaxis()->SetTitle( "# Events" );
  m_cutFlow->GetXaxis()->SetBinLabel( 1, "init" );
  m_cutFlow->GetXaxis()->SetBinLabel( 2, "GRL" );
  m_cutFlow->GetXaxis()->SetBinLabel( 3, "eta" );
  m_cutFlow->GetXaxis()->SetBinLabel( 4, "pt" );
  m_cutFlow->GetXaxis()->SetBinLabel( 5, "mediumID" );
  m_cutFlow->GetXaxis()->SetBinLabel( 6, "OQ" );
  m_cutFlow->GetXaxis()->SetBinLabel( 7, "2el" );
  m_cutFlow->GetXaxis()->SetBinLabel( 8, "charge" );
  m_cutFlow->GetXaxis()->SetBinLabel( 9, "ZVertex" );
  v_hist.push_back( m_cutFlow );

  if ( m_debug ) cout << "Analysis::Analysis() Done" << endl;  
}

Analysis::Analysis( string name , string outputFile ) : Analysis() {
  if ( m_debug ) cout << "Analysis::Analysis( string name , string outputFile )" << endl;
  m_name = name;
  for (unsigned int ihist = 0; ihist < v_hist.size() ; ihist++) {
    v_hist[ihist]->SetName( TString( m_name + "_" + v_hist[ihist]->GetName() ) );
  }

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

  while ( v_hist.size() ) {
    delete v_hist.back();
    v_hist.back() = 0;
    v_hist.pop_back();
  }
  
  if ( m_EgammaCalibrationAndSmearingTool )  delete  m_EgammaCalibrationAndSmearingTool;
  if ( m_grl )  delete m_grl;
  if ( m_LHToolMedium2012 ) delete m_LHToolMedium2012;
  if ( m_selectionTree ) delete m_selectionTree;
  if ( m_eContainer ) delete m_eContainer;
  if ( m_ZVertex ) delete m_ZVertex;
  if ( m_eventInfo ) delete m_eventInfo;

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
// void Analysis::Configure( string configFile ) {
  
  //Define the properties we will load from configFile
  //po::options_description desc;
  // desc.add_options()
  //   ("EGammaCalibTool.doSmearing", po::value< bool >( &m_doSmearing ), "Choose MC smearing" )
  //   ("EGammaCalibTool.doScaleFactor", po::value< bool >( &m_doScaleFactor ), "Choose data scaling" )
  //   ;
  
  // // Load setting file.
  // po::variables_map vm;
  // std::ifstream settings_file( configFile.c_str() , std::ifstream::in );
  // po::store( po::parse_config_file( settings_file , desc ), vm );
  // po::notify( vm );
  
//}


//======================================================
void Analysis::ResetTEvent() {
  if ( m_debug ) cout << "Analysis::ResetTevent" << endl;
  delete m_tfile;
  m_tfile = TFile::Open( m_fileName.front().c_str() );
  m_tevent.readFrom( m_tfile ).ignore();
  if ( m_debug ) cout << "Analysis::ResetTevent" << endl;
}

//=======================================================
void Analysis::SetName( string name ) { 
  m_name = name; 
  if ( m_debug )  cout << "Changed name : " << m_name << endl;

  for (unsigned int ihist = 0; ihist < v_hist.size(); ihist++ ) {
    v_hist[ihist]->SetName( TString( m_name + "_" + v_hist[ihist]->GetTitle() ) );
    if ( m_debug )   cout << v_hist[ihist]->GetName() << endl;
  }

  m_selectionTree->SetName( TString (m_name + "_" + m_selectionTree->GetTitle() ) ); 
}

string Analysis::GetName() const { return m_name; }
int Analysis::GetGoodEvents() const {return m_goodEvent; }
int Analysis::GetNEvents() const { return m_numEvent; }

void Analysis::SetDebug( bool debug ) { m_debug = debug; }
void Analysis::SetDoScaleFactor( bool doScale ) { m_doScaleFactor = doScale;}
void Analysis::SetDoSmearing( bool doSmearing ) { m_doSmearing = doSmearing; }
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
void Analysis::Save( ) {
  if ( m_debug ) cout << "Analysis::Save()" << endl;
  m_logFile->cd();

  for (unsigned int ihist = 0; ihist < v_hist.size() ; ihist++) {
    v_hist[ihist]->Write( "", TObject::kOverwrite );
  }

  TString bufferName = TString::Format( "%s", m_name.c_str() );
  TTree * treeout = new TTree( "InfoTree" , "InfoTree" );
  treeout->Branch( "m_name", &bufferName, "m_name/C" );
  treeout->Branch( "m_numEvent", &m_numEvent, "m_numEvent/I" );
  treeout->Branch( "m_goodEvent", &m_goodEvent, "m_goodEvent/I" );
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
    m_name = string( buffer_name );
    delete treeout;
    treeout = 0;
  }
  else return 2;
  
  for (unsigned int ihist = 0; ihist < v_hist.size() ; ihist++) {
    string histName = m_name + "_" + v_hist[ihist]->GetTitle();
    delete v_hist[ihist];

    if ( !infile->Get( histName.c_str() ) ) {
      cout << histName << " was not found" << endl;
      return 3;
    }
    
    v_hist[ihist] = (TH1F*) infile->Get( histName.c_str() )->Clone();  
    v_hist[ihist]->SetDirectory( 0 );
    if ( m_debug ) cout << "Loaded histogram : " << v_hist[ihist]->GetName() << endl;
      
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
void Analysis::Add( Analysis const &analysis ) {

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
  for (unsigned int ihist = 0; ihist < v_hist.size(); ihist++ ) {
       v_hist[ihist]->Add( analysis.v_hist[ihist] );
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
  
  //Setup the calibration tool
  m_EgammaCalibrationAndSmearingTool  = new CP::EgammaCalibrationAndSmearingTool("EgammaCalibrationAndSmearingTool"); 
  m_EgammaCalibrationAndSmearingTool->setProperty("ESModel", "es2012c"); 
  m_EgammaCalibrationAndSmearingTool->setProperty("ResolutionType", "SigmaEff90"); 
  m_EgammaCalibrationAndSmearingTool->initialize();

  //Setup the electron ID tool  
  m_LHToolMedium2012   = new AsgElectronLikelihoodTool ("m_LHToolMedium2012"); 
  m_LHToolMedium2012  ->setProperty("primaryVertexContainer","PrimaryVertices");
  string confDir = "ElectronPhotonSelectorTools/offline/mc15_20150224/";
  m_LHToolMedium2012->setProperty("ConfigFile",confDir+"ElectronLikelihoodMediumOfflineConfig2012.conf");
  m_LHToolMedium2012   ->initialize();

  m_electronSF = new AsgElectronEfficiencyCorrectionTool("AsgElectronEfficiencyCorrectionTool");
  std::vector<std::string> inputFiles{"efficiencySF.offline.RecoTrk.2015.13TeV.rel19.GEO21.v01.root",
      "efficiencySF.offline.XXX.2015.13TeV.rel19.GEO21.v01.root "} ;
  m_electronSF->setProperty("CorrectionFileNameList",inputFiles);
  //set datatype, 0-Data(or dont use the tool - faster), 1-FULLSIM, 3-AF2
  m_electronSF->setProperty("ForceDataType",2);
  m_electronSF->initialize();

  //Setup the GRL 
  m_grl = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");
  std::vector<std::string> vecStringGRL;
  vecStringGRL.push_back("data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml");
  //  vecStringGRL.push_back("/afs/in2p3.fr/home/c/cgoudet/private/eGammaScaleFactors/data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml");
  m_grl->setProperty( "GoodRunsListVec", vecStringGRL);
  m_grl->setProperty("PassThrough", false); // if true (default) will ignore result of GRL and will just pass all events
  if (!m_grl->initialize().isSuccess()) { // check this isSuccess
    cout << "Erreur : GRL not initialized" << endl;
    exit(1);
  }

  //Setup m_SelectionTree
  double dummyVar = -99;
  unsigned long long dummyLong = 0;
  m_selectionTree = new TTree( TString( m_name.c_str() ) + "_selectionTree", "selectionTree" ); 
  m_selectionTree->SetDirectory( 0 );

  m_selectionTree->Branch( "runNumber", &dummyLong );
  m_selectionTree->Branch( "eventNumber", &dummyLong );
  m_selectionTree->Branch( "energy_1", &dummyVar );
  m_selectionTree->Branch( "e_raw_sampl1_1", &dummyVar );
  m_selectionTree->Branch( "e_raw_sampl2_1", &dummyVar );
  m_selectionTree->Branch( "pt_1" , &dummyVar );
  m_selectionTree->Branch( "eta_1", &dummyVar );
  m_selectionTree->Branch( "phi_1", &dummyVar );
  m_selectionTree->Branch( "eta_cl_1", &dummyVar );
  m_selectionTree->Branch( "eta_calo_1", &dummyVar );
  m_selectionTree->Branch( "e1_1", &dummyVar );
  m_selectionTree->Branch( "e2_1", &dummyVar );
  
  m_selectionTree->Branch( "energy_2", &dummyVar );
  m_selectionTree->Branch( "e_raw_sampl1_2", &dummyVar );
  m_selectionTree->Branch( "e_raw_sampl2_2", &dummyVar );
  m_selectionTree->Branch( "pt_2" , &dummyVar );
  m_selectionTree->Branch( "eta_2", &dummyVar );
  m_selectionTree->Branch( "phi_2", &dummyVar );
  m_selectionTree->Branch( "eta_cl_2", &dummyVar );
  m_selectionTree->Branch( "eta_calo_2", &dummyVar ); 
  m_selectionTree->Branch( "e1_2", &dummyVar );
  m_selectionTree->Branch( "e2_2", &dummyVar );

  m_selectionTree->Branch( "m12",  &dummyVar );
  m_selectionTree->Branch( "weight", &dummyVar );


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
      //     if ( ! m_tevent.retrieve( m_eContainer, "Electrons" ).isSuccess() ){ cout << "Can not retrieve ElectronContainer : ElectronCollection" << endl; exit(1); }// if retrieve                                                                 
      if ( ! m_tevent.retrieve( m_eContainer, "Electrons" ).isSuccess() ){ cout << "Can not retrieve ElectronContainer : ElectronCollection" << endl; exit(1); }// if retrieve                                                                 
      if ( ! m_tevent.retrieve( m_eventInfo, "EventInfo" ).isSuccess() ){ cout << "Can Not retrieve EventInfo" << endl; exit(1); }
      if ( ! m_tevent.retrieve( m_ZVertex, "PrimaryVertices" ).isSuccess() ){ cout << "Can Not retrieve Vertex Info" << endl; exit(1); }

      // if ( ! i_event || i_event == nentries-1 ) {
      // 	cout << m_eventInfo->eventNumber() << "  " << m_tfile->GetName() << endl;
      // }

      //Create a shallow copy
      // Allow to modify electrons properties for calibration
      m_eShallowContainer = xAOD::shallowCopyContainer( *m_eContainer );

      //Initialize calibration Tool
      //      m_EgammaCalibrationAndSmearingTool->setDefaultConfiguration( m_eventInfo );
      m_EgammaCalibrationAndSmearingTool->forceSmearing( m_doSmearing );
      m_EgammaCalibrationAndSmearingTool->forceScaleCorrection( m_doScaleFactor );
      
      //GRL
      m_cutFlow->Fill( "init", 1 );
      if ( ! m_eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ) {//test if is data
	if ( !m_grl->passRunLB(*m_eventInfo) ) continue;  //passes the GRL
	if ( ( m_eventInfo->errorState(xAOD::EventInfo::LAr)==xAOD::EventInfo::Error ) 
	     || (m_eventInfo->errorState(xAOD::EventInfo::Tile)==xAOD::EventInfo::Error ) 
	     || (m_eventInfo->isEventFlagBitSet(xAOD::EventInfo::Core, 18) )  )
	  continue;
      }
      m_cutFlow->Fill( "GRL", 1 );

      //      Make the electron selection and fill m_eGoodContainer
      int err = (int) PassSelection();
      if ( m_debug ) cout << "PassSelection : " << err << endl;
      if ( err )  {
	m_goodEvent++;
	//Should not contain events in bin 0

	m_ZMass->Fill( ComputeZMass( m_veGood ) );

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
	delete m_veGood.back();
	m_veGood.pop_back();
      }
      m_veGoodWeight.clear();
      m_weight=1;
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

  m_eventZVertex->Fill( (*m_ZVertex)[0]->z() );

  //Reduce number of electron in container by appling cuts on electrons
  MakeElectronCut();

  //Request exactly two electrons
  if ( m_veGood.size() != 2 ) return false;
  m_cutFlow->Fill( "2el", 1 );
  //  Check the sign of the two electrons
  if ( m_veGood[0]->charge() *  m_veGood[1]->charge() > 0 ) return false;
  m_cutFlow->Fill( "charge", 1 );
  //Cut on the position of the primary Vertex
  if ( fabs( (*m_ZVertex)[0]->z() ) > 150 ) return false;
  m_cutFlow->Fill( "ZVertex", 1 );

  //if (ComputeZMass( m_veGood ) > 100 || ComputeZMass( m_veGood ) < 80) return false;
  if ( m_debug ) cout << "Analysis::PassSelection done" << endl;
  return true;
}

//===================================================================
//Make selection on electron level
void Analysis::MakeElectronCut() {
  if ( m_debug ) cout << "Analysis::MakeElectronCut" << endl;

  for ( xAOD::ElectronContainer::iterator eContItr = (m_eShallowContainer.first)->begin(); eContItr != (m_eShallowContainer.first)->end(); eContItr++ ) {
    double efficiencyScaleFactor = 1;

    //  Cut on the quality of the **eContItrectron
    if ( !m_LHToolMedium2012->accept( **eContItr ) ) continue;
    m_cutFlow->Fill( "mediumID", 1 );

    //Calibrate this new electron
    m_EgammaCalibrationAndSmearingTool->applyCorrection( **eContItr );

//    m_electronSF->getEfficiencyScaleFactor(**eContItr,efficiencyScaleFactor);
    
    m_elEta->Fill( (*eContItr)->eta() );
    m_elPt->Fill( (*eContItr)->pt() /1000 );
    
    //kinematical cuts on electrons
    if ( fabs( (*eContItr)->eta() ) > 2.47 ) continue;
    m_cutFlow->Fill( "eta", 1 );
    
    if ( (*eContItr)->pt() < 27e3 ) continue;
    m_cutFlow->Fill( "pt", 1 );
    
    //  OQ cut
    if ( !(*eContItr)->isGoodOQ( xAOD::EgammaParameters::BADCLUSELECTRON ) ) continue;
    m_cutFlow->Fill( "OQ", 1 );
    
    m_veGood.push_back( 0 );
    m_veGood.back() = *eContItr;
    m_veGoodWeight.push_back( efficiencyScaleFactor );
  }

  if ( m_debug ) cout << "Analysis::MakeElectronCut done" << endl;
}//MakeKincut


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

//=======================================================================
int Analysis::FillSelectionTree() {
  if ( m_debug ) cout << "Analysis::FillSelectionTree" << endl;
  //RELEASE
  if ( m_veGood.size() != 2 ) return 1;
  double pt_1 = m_veGood[0]->pt();
  double eta_1 = m_veGood[0]->eta();
  double phi_1 = m_veGood[0]->phi();
  double eta_cl_1 = m_veGood[0]->caloCluster()->eta();
  //Do not put = between eta_calo_1 and m_veGood, the function retrieveMoment fill the value
  double eta_calo_1; m_veGood[0]->caloCluster()->retrieveMoment(xAOD::CaloCluster::ETACALOFRAME,eta_calo_1); 
  double e1_1 = m_veGood[0]->caloCluster()->energyBE(1);
  double e2_1 = m_veGood[0]->caloCluster()->energyBE(2);
  double energy_1 = m_veGood[0]->e();
  double e_raw_sampl1_1 = m_veGood[0]->caloCluster()->eSample( CaloSampling::CaloSample::EMB1  );
  double e_raw_sampl2_1 = m_veGood[0]->caloCluster()->eSample( CaloSampling::CaloSample::EMB2  );

  double pt_2 = m_veGood[1]->pt();
  double eta_2 = m_veGood[1]->eta();
  double phi_2 = m_veGood[1]->phi();
  double eta_cl_2 = m_veGood[1]->caloCluster()->eta();
  double eta_calo_2; m_veGood[1]->caloCluster()->retrieveMoment(xAOD::CaloCluster::ETACALOFRAME,eta_calo_2); 
  double e1_2 = m_veGood[1]->caloCluster()->energyBE(1);
  double e2_2 = m_veGood[1]->caloCluster()->energyBE(2);
  double energy_2 = m_veGood[1]->e();
  double e_raw_sampl1_2 = m_veGood[1]->caloCluster()->eSample( CaloSampling::CaloSample::EMB1  );
  double e_raw_sampl2_2 = m_veGood[1]->caloCluster()->eSample( CaloSampling::CaloSample::EMB2  );

  unsigned long long runNumber = m_eventInfo->runNumber();
  unsigned long long eventNumber = m_eventInfo->eventNumber();
  double mass = ComputeZMass( m_veGood );
  double weight = m_veGoodWeight[0]*m_veGoodWeight[1]*m_weight;

  m_selectionTree->SetBranchStatus( "*", 1);
  m_selectionTree->SetBranchAddress( "runNumber", &runNumber );
  m_selectionTree->SetBranchAddress( "eventNumber", &eventNumber );
  m_selectionTree->SetBranchAddress( "energy_1" , &energy_1 );
  m_selectionTree->SetBranchAddress( "e_raw_sampl1_1" , &e_raw_sampl1_1 );
  m_selectionTree->SetBranchAddress( "e_raw_sampl2_1" , &e_raw_sampl2_1 );
  m_selectionTree->SetBranchAddress( "pt_1" , &pt_1 );
  m_selectionTree->SetBranchAddress( "eta_1" , &eta_1 );
  m_selectionTree->SetBranchAddress( "phi_1" , &phi_1 );
  m_selectionTree->SetBranchAddress( "eta_cl_1" , &eta_cl_1 );
  m_selectionTree->SetBranchAddress( "eta_calo_1" , &eta_calo_1 );
  m_selectionTree->SetBranchAddress( "e1_1", &e1_1);
  m_selectionTree->SetBranchAddress( "e2_1", &e2_1);

  m_selectionTree->SetBranchAddress( "energy_2" , &energy_2 );
  m_selectionTree->SetBranchAddress( "e_raw_sampl1_2" , &e_raw_sampl1_2 );
  m_selectionTree->SetBranchAddress( "e_raw_sampl2_2" , &e_raw_sampl2_2 );
  m_selectionTree->SetBranchAddress( "pt_2" , &pt_2 );
  m_selectionTree->SetBranchAddress( "eta_2" , &eta_2 );
  m_selectionTree->SetBranchAddress( "phi_2" , &phi_2 );
  m_selectionTree->SetBranchAddress( "eta_cl_2" , &eta_cl_2 );
  m_selectionTree->SetBranchAddress( "eta_calo_2" , &eta_calo_2 );
  m_selectionTree->SetBranchAddress( "e1_2", &e1_2);
  m_selectionTree->SetBranchAddress( "e2_2", &e2_2);

  m_selectionTree->SetBranchAddress( "m12" , &mass );  
  m_selectionTree->SetBranchAddress( "weight" , &weight );
 
  m_selectionTree->Fill();
  if ( m_debug ) cout << "Analysis::FillSelectionTree done" << endl;
  return 0;
}

//=====================================================
TH1F* Analysis::GetZMass() const { 
  return  m_ZMass; }
