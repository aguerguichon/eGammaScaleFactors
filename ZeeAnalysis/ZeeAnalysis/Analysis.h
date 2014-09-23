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

class Analysis 
{

 public : 

  Analysis();   
  Analysis( string name );
  Analysis( string name, string infileName );
  Analysis( string name, vector<string> v_infileName ); 
  ~Analysis();

  void SetName(string name);
  string GetName();

  // Function that makes ratio plot
  // Need to have access to attributes
  friend  void CompareAnalysis(Analysis &anaMC, Analysis &anaData, string filename="");

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

  void Add( Analysis analysis );

 private :
  //I assume that the TEvent can only read one fiel at the time
  //and it can not concatenate multiple files
  //Hence I store in the class the vector of TFile
  //When doing event per event method i will make the TEvent run over all files

  vector< TFile* > m_tfile; // Contains all TFiles which will be read by 
  xAOD::TEvent m_tevent;

  string m_name;

  int  m_numEvent;
  int m_goodEvent;

  TH1F *m_ZMass;
  TH1F *m_EPerEventBFSel;
  TH1F *m_EPerEventAFSel;

};
#endif
