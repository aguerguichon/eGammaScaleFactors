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

class Analysis 
{

 public : 

  Analysis();   
  Analysis( string name , int debug = 0);
  Analysis( string name, string infileName , int debug = 0);
  Analysis( string name, vector<string> v_infileName, int debug = 0); 
  ~Analysis();

  void SetName(string name);
  string GetName();

  //*******************************************
  // Add a Tfile to m_tfile
  // Return an exception if fiel is not found
  // Clear pointers if needed
  // The TEvent read the new file
  void AddFile(string infileName);

  //Set the TEvent to read from first file
  void ResetTEvent();

  //Loop on all available events to perform the analysis
  void TreatEvents(int nevent=0);

  //plot and save histograms results
  void PlotResult(string fileName="");

  //Save All content of analysis in a root file
  //fileName = output rootfile
  void Save(string fileName="");

  void Load( string fileName = "" );

  void Add( const Analysis & analysis  );

  //Make ratio of current histograms over input histograms
  void Divide ( Analysis & analysis );

 private :
  //I assume that the TEvent can only read one fiel at the time
  //and it can not concatenate multiple files
  //Hence I store in the class the vector of TFile
  //When doing event per event method i will make the TEvent run over all files

  vector< string > m_fileName; // Contains all Files Names which will be read by 
  xAOD::TEvent m_tevent;
  xAOD::TStore *m_tstore;

  string m_name;

  int m_debug;
  int  m_numEvent;
  int m_goodEvent;

  //output histograms
  TH1F *m_ZMass;


  //========================================
  // Define variables internal to the analysis

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
