#include <iostream>
//#include <ofstream>
#include <string>

#include "TTree.h"
#include "TH1.h"
#include "TFile.h"
#include "TCanvas.h"

using namespace std;


int MergeCutFlows()
{
  string path = "/sps/atlas/a/aguerguichon/Calibration/DataxAOD/MC15c_13TeV_Zee_Lkh1/";

  double nEntries=0;
  // float nZ=0;

  TH1D *finalHist=0;
  TH1D *finalHistData=0;
  TH1D *CFhist=0;
  TH1::AddDirectory(kFALSE);
  
  for (int iFile=0; iFile<10; iFile++)
    {
      string name= path+"MC15c_13TeV_Zee_Lkh1_"+to_string(iFile)+".root";
      TFile *inFile= TFile::Open(name.c_str());
      if (!inFile) break;
      name ="MC15c_13TeV_Zee_Lkh1_"+to_string(iFile)+"_cutFlow";
      
      cout<<"Getting cutFlow histogram from "<<path<<"MC15c_13TeV_Zee_Lkh1_"<<to_string(iFile)<<".root\n";
      CFhist= (TH1D*) inFile->Get(name.c_str());
      
      // name="MC15c_13TeV_Zee_Lkh1_"+to_string(iFile)+"_ZMass";
      // TH1D *Z= (TH1D*) inFile->Get(name.c_str());
      // nZ+= Z->GetEntries();
      // cout<<"Z#: "<<nZ<<endl;
      
      
      if (iFile==0) finalHist= new TH1D ("CFMC", "", CFhist->GetXaxis()->GetNbins(), 0.5, CFhist->GetXaxis()->GetNbins()+0.5);
 
      for (int iBin=1; iBin<=CFhist->GetXaxis()->GetNbins(); iBin++)
	{
	  finalHist->SetBinContent( iBin, finalHist->GetBinContent(iBin)+CFhist->GetBinContent(iBin)  );	  
	  finalHist->GetXaxis()->SetBinLabel( iBin,  CFhist->GetXaxis()->GetBinLabel(iBin)  );
	  
	} 
      nEntries+=CFhist->GetBinContent( CFhist->GetXaxis()->GetNbins() );

      inFile->Close();
      
    }

  cout<<"nEntries MC: "<<nEntries<<endl;

  nEntries=0;
  TFile *inFileData= TFile::Open("/sps/atlas/a/aguerguichon/Calibration/DataxAOD/Data15_13TeV_Zee_Lkh1/Data15_13TeV_Zee_Lkh1_0.root");
  TH1D *CFhistData= (TH1D*) inFileData->Get("Data15_13TeV_Zee_Lkh1_0_cutFlow");
    


  path = "/sps/atlas/a/aguerguichon/Calibration/DataxAOD/Data16_13TeV_Zee_Lkh1/";

  for (int iFile=0; iFile<10; iFile++)
    {
      string name= path+"Data16_13TeV_Zee_Lkh1_"+to_string(iFile)+".root";
      TFile *inFile= TFile::Open(name.c_str());
      if (!inFile) break;
      name ="Data16_13TeV_Zee_Lkh1_"+to_string(iFile)+"_cutFlow";
      
      cout<<"Getting cutFlow histogram from "<<path<<"Data16_13TeV_Zee_Lkh1_"<<to_string(iFile)<<".root\n";
      CFhist= (TH1D*) inFile->Get(name.c_str());
          
      
      if (iFile==0) finalHistData= new TH1D ("CFData", "", CFhist->GetXaxis()->GetNbins(), 0.5, CFhist->GetXaxis()->GetNbins()+0.5);
 
      for (int iBin=1; iBin<=CFhist->GetXaxis()->GetNbins(); iBin++)
	{

	  if (iFile==0) finalHistData->SetBinContent( iBin, CFhistData->GetBinContent(iBin) );
	  finalHistData->SetBinContent( iBin, finalHistData->GetBinContent(iBin)+CFhist->GetBinContent(iBin)  );	  
	  finalHistData->GetXaxis()->SetBinLabel( iBin,  CFhist->GetXaxis()->GetBinLabel(iBin)  );

	}    
      nEntries+=CFhist->GetBinContent( CFhist->GetXaxis()->GetNbins() );
      inFile->Close();
      
    }

  cout<<"nEntries data: "<<nEntries<<endl;

  inFileData->Close();

  TFile outFile("/afs/in2p3.fr/home/a/aguergui/public/eGammaScaleFactors/TotalCutFlow.root","recreate");

  finalHist->Write();

  finalHistData->Write();
  outFile.Close();

  // delete finalHist;
  // delete finalHistData;
  // delete CFhist;
  // delete CFhistData;
  // delete c1;
  // delete c2;
  // delete inFileData;

  return 0;
}
