#ifndef ANALYSIS_H
#define ANALYSIS_H
#include <string>
using std::string;
#include <vector>
using std::vector;
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TStore.h"
#include "TH1F.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/Electron.h"
#include "ElectronPhotonFourMomentumCorrection/EgammaCalibrationAndSmearingTool.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODCore/ShallowCopy.h"

class GoodRunsListSelectionTool;

/**\class < Analysis > [<Analysis.h>]
 * \brief{Main function for the selection.}
 *
 * 
 */

class Analysis 
{

 public : 

  Analysis();   
  Analysis( string name , int debug = 0);
  Analysis( string name, string infileName , int debug = 0);
  Analysis( string name, vector<string> v_infileName, int debug = 0); 
  ~Analysis();


  void Add( const Analysis & analysis  );
  
  // Add a Tfile to m_tfile
  // Return an exception if fiel is not found
  // Clear pointers if needed
  // The TEvent read the new file
  void AddFile(string infileName);

  /** \brief Use configuration file to set some variables values
      \param configFile Input configuration file 
      
      Input file must be in Boost language \n
      [plugins] \n
      name = somePlugin \n
      name = otherPlugin \n
    
   */
  void Configure( string configFile );

  //Make ratio of current histograms over input histograms
  void Divide ( Analysis & analysis );

  string GetName();

  void Load( string fileName = "" , int debug = 0);

  //plot and save histograms results
  void PlotResult(string fileName="");

  //Set the TEvent to read from first file
  void ResetTEvent();

  //Save All content of analysis in a root file
  //fileName = output rootfile
  void Save(string fileName="");

  void SetName(string name);

  //Loop on all available events to perform the analysis
  void TreatEvents(int nevent=0);


 private :
  //I assume that the TEvent can only read one fiel at the time
  //and it can not concatenate multiple files
  //Hence I store in the class the vector of TFile
  //When doing event per event method i will make the TEvent run over all files

  vector< string > m_fileName; // Contains all Files Names which will be read by 
  xAOD::TEvent m_tevent;
  xAOD::TStore *m_tstore;

  //Name of the object
  //This name will be displayed in the namme and title of all output histograms
  string m_name;

  //debug mode
  int m_debug;

  //Counters of events
  int  m_numEvent;
  int m_goodEvent;

  //Configuration attributes
  bool m_doSmearing;
  bool m_doScaleFactor;

  //output histograms
  TH1F *m_ZMass;

  //Store pointers of histograms to lighten the saving and reading code
  vector< TH1F* > v_hist;

  //Store electron container and its shallow copy 
  const xAOD::ElectronContainer* m_eContainer;
  std::pair< xAOD::ElectronContainer*, xAOD::ShallowAuxContainer* > m_eShallowContainer;

  //GRL
#ifndef __CINT__
  GoodRunsListSelectionTool *m_grl; 
#endif // not __CINT__
    
  //Electron calibration tool
  CP::EgammaCalibrationAndSmearingTool *m_EgammaCalibrationAndSmearingTool;
  const xAOD::EventInfo* m_eventInfo;

  vector< xAOD::Electron* > m_veGood;

  // Current read file 
  // Opening one file at a time and deleting others allow to have more files in the job
  TFile* m_tfile;

  //==========================================

  //Internal functions
  bool PassSelection( );
  void MakeElectronCut( );
  bool isGoodElectron( xAOD::Electron const & el);
};
#endif
