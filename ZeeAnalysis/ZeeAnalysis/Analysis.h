#ifndef ANALYSIS_H
#define ANALYSIS_H
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <map>
using std::map;
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TStore.h"
#include "TH1D.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODTruth/TruthEventContainer.h"
#include "xAODEgamma/Electron.h"
#include "ElectronPhotonFourMomentumCorrection/EgammaCalibrationAndSmearingTool.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODCore/ShallowCopy.h"
#include "TTree.h"
#include "xAODTracking/VertexContainer.h"
#include "ElectronPhotonSelectorTools/AsgElectronLikelihoodTool.h"
#include "PileupReweighting/PileupReweightingTool.h"
#include "ElectronPhotonSelectorTools/AsgElectronIsEMSelector.h"
#include "VertexPositionReweighting/VertexPositionReweightingTool.h"
#include "ElectronEfficiencyCorrection/AsgElectronEfficiencyCorrectionTool.h"
#include "TrigConfxAOD/xAODConfigTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"
#include "AsgTools/ToolHandle.h"

using namespace Trig;
using namespace TrigConf;
using namespace xAOD;

class GoodRunsListSelectionTool;
class LineShapeTool;
/**\class < Analysis > [<Analysis.h>]
  \brief Main function for the selection
  
*/

class Analysis 
{

 public : 

  Analysis();   
  Analysis( string name , string outputFile  );
  Analysis( string name, string infileName, string outputFile );
  Analysis( string name, vector<string> v_infileName , string outputFile ); 
  ~Analysis();

  /**\brief Add Content of another Analysis
     \param analysis Analysis which content should be added

     Adding two analyses consist of : \n
     - Summing all histograms
     - Merging lists of input files
     - Keeping the name of the first analysis
     - Summing the counters of events
   */
  void Add( Analysis & analysis  );
  

  /**\brief Add a ROOT file to the list of input files.
     \param infileName Name of the input ROOT file

     The TEvent object points to the new file.

     If the file do not exist, the objects remains unchanged and return an exception

   */
  void AddFile(string infileName);

  /** \brief Use configuration file to set some variables values
      \param configFile Input configuration file 

      Input file must be in Boost language \n
      [plugins] \n
      name = somePlugin \n
      name = otherPlugin \n
   */
  //  void Configure( string configFile );


  /**\brief Make a ratio Analysis
     \param analysis Denominator analysis

     The ratio Analysis consist of : \n
     - m_histograms / histograms_input
     - Change histograms properties
   */
  void Divide ( Analysis & analysis );

  string GetName() const { return m_name; }
  int GetGoodEvents() const {return m_goodEvent; }
  int GetNEvents() const { return m_numEvent; }
  double GetPtCut() const { return m_ptCut; }


  void SetEsModel( string esModel ) { m_esModel = esModel;} 
  void SetDebug( bool debug ) { m_debug = debug; }
  void SetDoScaleFactor( int doScale ) { m_doScaleFactor = doScale;}
  void SetElectronID( int electronID ) { m_electronID = electronID; }  
  void SetPtCut( double ptCut ) { m_ptCut = ptCut; }
  void SetFBremCut( double fBremCut ) { m_fBremCut = fBremCut; }
  TH1* GetHist( string histName )  { return m_mapHist[histName]; }
  double GetFBrem( xAOD::Electron *el );
  /**\brief Create an Analysis object from a ROOT file saving
     \param fileName ROOT file created by Analysis::Save 

     Loading an Analysis consist of : \n
     - Changing the name of the Analysis
     - Loading the histograms
     - Loading event counters
  */
  int Load( string fileName = "");

  //plot and save histograms results
  void PlotResult(string fileName="");

  //Set the TEvent to read from first file
  void ResetTEvent();

  /**\brief Write the content of the Analysis in a ROOT file
     Properties of the analysis that are saved : \n
     - name
     - event counters values
     - histograms
  */
  void Save();

  /**\brief Change the name of the Analysis
     \param name new name
     
     The change of name will be propagated to the titles and names of attributes.
  */  
  void SetName(string name);


  /**\brief Event per event analysis
     \param nevent Number of events to run over

     This is the main function of this class. 
     It must be called in order to perform the the analysis.
     - It runs over events in all input files of the class
     - Retrieve the containers
     - Call Analysis::PassSelection to perform the cuts

     If nevent = 0, the program will run over all events
  */
  void TreatEvents(int nevent=0);
  //  bool IsMedium(const xAOD::Electron* el ) const;
 private :
  int InitializeTools();
  double GetLineShapeWeight();

  //I assume that the TEvent can only read one fiel at the time
  //and it can not concatenate multiple files
  //Hence I store in the class the vector of TFile
  //When doing event per event method i will make the TEvent run over all files

  vector< string > m_fileName; // Contains all Files Names which will be read by 
  xAOD::TEvent m_tevent;

  //debug mode
  bool m_debug;

  //Name of the object
  //This name will be displayed in the namme and title of all output histograms
  string m_name;



  //Counters of events
  unsigned long long int m_numEvent;
  unsigned long long int m_goodEvent;
  /* int m_numEvent; */
  /* int m_goodEvent; */

  //Configuration attributes
  int m_doScaleFactor;

  /**\brief TTree containing minimal information of selected events
   */
  TTree *m_selectionTree;

  //Store pointers of histograms to lighten the saving and reading code
  map< string, TH1* > m_mapHist;

  //Store electron container and its shallow copy 
  const xAOD::ElectronContainer* m_eContainer;
  std::pair< xAOD::ElectronContainer*, xAOD::ShallowAuxContainer* > m_eShallowContainer;
  const xAOD::VertexContainer *m_ZVertex;

  //GRL
  GoodRunsListSelectionTool *m_grl; 
    
  //Electron calibration tool
  CP::EgammaCalibrationAndSmearingTool *m_EgammaCalibrationAndSmearingTool;
  //  ToolHandle<CP::IPileupReweightingTool*> m_pileup;
  CP::PileupReweightingTool *m_pileup;
  CP::VertexPositionReweightingTool *m_vtxTool;
  AsgElectronEfficiencyCorrectionTool *m_electronSFReco;
  AsgElectronEfficiencyCorrectionTool *m_electronSFID;
  
  const xAOD::EventInfo* m_eventInfo;
  AsgElectronLikelihoodTool* m_LHToolMedium2012;
  AsgElectronIsEMSelector* m_CutToolMedium2012;
  LineShapeTool *m_LineShapeTool;
  const xAOD::TruthEventContainer *m_teC;
  Trig::TrigDecisionTool *m_trigDecisionTool;
  TrigConf::xAODConfigTool *m_trigConfigTool;

  vector< xAOD::Electron* > m_veGood;
  vector< double > m_veGoodWeight;

  // Current read file 
  // Opening one file at a time and deleting others allow to have more files in the job
  TFile* m_tfile;

  /**\brief Make the selection at Event Level
   */
  bool PassSelection( );
  void MakeElectronCut( );

  /**\brief Fill the TTree with events passing the selection
     \return 0 Success return
     \return 1 m_veGood has wrong size
   */
  int FillSelectionTree();

  /**\brief TFile to save the content of the analysis
   */
  TFile *m_logFile;

  map<string, double> m_mapVar;
  map<string, long long int> m_mapLongVar;

  /*
    O Likellihood Loose
    1 Likelihood Medium
    2 Likeihood Tight
    3 CutBase Loose
    4 CutBased Medium
    5 CutBased Tight
   */
  int m_electronID;
  string m_esModel;
  /*
    2015 : es2015PRE
    2012 : es2012c
  */

  double m_ptCut;
  double m_fBremCut;
};
#endif
