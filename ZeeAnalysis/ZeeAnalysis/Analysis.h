#ifndef ANALYSIS_H
#define ANALYSIS_H
#include <string>
using std::string;
#include <vector>
using std::vector;
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TStore.h"


class Analysis 
{

 public : 

  Analysis();   // Will initialize histograms
  Analysis( string infileName);
  Analysis( vector<string> v_infileName ); //will initialize TFiles and TEvents

  //*******************************************
  // Add a Tfile to m_tfile
  // Return an exception if fiel is not found
  // Clear pointers if needed
  // The TEvent read the new file
  void AddFile(string infileName);

 private :
  //I assume that the TEvent can only read one fiel at the time
  //and it can not concatenate multiple files
  //Hence I store in the class the vector of TFile
  //When doing event per event method i will make the TEvent run over all files

  vector< TFile* > m_tfile; // Contains all TFiles which will be read by 
  xAOD::TEvent m_tevent;

 int  m_numEvent;
};
#endif
