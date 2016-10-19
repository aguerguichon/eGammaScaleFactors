#include <iostream>
//#include <ofstream>
#include <string>

#include "TTree.h"
#include "TH1.h"
#include "TFile.h"
#include "TCanvas.h"

using namespace std;


int GetNumberEventsCutFlow()
{

  TH1::AddDirectory(kFALSE);

  TFile *CFfile= TFile::Open("TestAnalysis.root");
  TH1D *CFhist= (TH1D*) CFfile->Get("Analysis_cutFlow");


  for (int iBin=1; iBin<=CFhist->GetXaxis()->GetNbins(); iBin++)
    {

      cout<<CFhist->GetXaxis()->GetBinLabel(iBin)<<" "<<CFhist->GetBinContent(iBin)<<"\n";

    }

  TCanvas *c1= new TCanvas("","CF");
  CFhist->Draw();
  c1->Print("/afs/in2p3.fr/home/a/aguergui/public/eGammaScaleFactors/CutFlow.pdf");

  return 0;
}
