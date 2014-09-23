#include <iostream>
using std::cout; using std::endl;
#include "ZeeAnalysis/Analysis.h"
#include "TCanvas.h"
#include "TFile.h"

void CompareAnalysis(Analysis &anaMC, Analysis &anaData, string fileName ){


  TH1F* ZMassRawMC = ( TH1F* ) anaMC.m_ZMassRaw->Clone();
  TH1F* ZMassRawData = ( TH1F* ) anaData.m_ZMassRaw->Clone();

  //Normalize hist before comparing
  ZMassRawMC->Scale( 1 / ZMassRawMC->Integral() );
  ZMassRawData->Scale( 1 / ZMassRawData->Integral() );

  //Make ratio of histo
  ZMassRawData->Divide( ZMassRawMC );

  //Setup Ratio plot
  ZMassRawData->SetTitle( TString( "ZMassRaw_" + anaData.m_name + "_" + anaMC.m_name )  );
  ZMassRawData->SetName(  TString( "ZMassRaw_" + anaData.m_name + "_" + anaMC.m_name ) );

  //Deal with name of saving
  if ( fileName == "" ) fileName = anaData.m_name + anaMC.m_name + ".root";

  //Save plots
  TCanvas *canvas = new TCanvas();
  ZMassRawData->Draw("E");
  canvas->SaveAs( TString( fileName.substr( 0, fileName.find_last_of(".")) + "_ZMassRawDataMC.pdf") );

  TFile *outfile = new TFile( fileName.c_str() , "RECREATE" );
  ZMassRawData->Write( "", TObject::kOverwrite );
  outfile->Close();
}
