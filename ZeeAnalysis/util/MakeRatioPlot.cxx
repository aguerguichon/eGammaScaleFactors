#include <iostream>
#include "ZeeAnalysis/Analysis.h"
#include <boost/program_options.hpp>
#include "TPad.h"
#include "TCanvas.h"
#include "TLegend.h"

using std::cout;
using std::endl;

namespace po = boost::program_options;

void Style_Christophe();

int main( int argc, char* argv[] ) {
  Style_Christophe();

  po::options_description desc("LikelihoodProfiel Usage");
  //define all options in the program                                                
  vector<string> infile;
  string outName, inMCName, inMCScaleName, inMCScaleSmearName, inDataName, inDataScaleName, inDataScaleSmearName, savingDirectory;

  desc.add_options()
    ("help", "Display this help message")
    ("outName", po::value<string >(&outName)->default_value("results/TestAnalysis.root") , "Name of the output file")
    ("inMCName", po::value< string >(&inMCName)->default_value("/sps/atlas/c/cgoudet/Calibration/MC_8TeV_Zee_ZeeNtuple.root" ), "Name of MC root file without correction")
    ("inMCScaleName", po::value< string >(&inMCScaleName)->default_value("/sps/atlas/c/cgoudet/Calibration/MC_8TeV_Zee_scale_ZeeNtuple.root" ), "Name of MC root file with scale only")
    ("inMCScaleSmearName", po::value< string >(&inMCScaleSmearName)->default_value("/sps/atlas/c/cgoudet/Calibration/MC_8TeV_Zee_scale_smear_ZeeNtuple.root" ), "Name of MC root file with scale a,d smearing")
    ("inDataName", po::value< string >(&inDataName)->default_value("/sps/atlas/c/cgoudet/Calibration/Data_8TeV_Zee_ZeeNtuple.root"), "Name of Data root file without correction")
    ("inDataScaleName", po::value< string >(&inDataScaleName)->default_value("/sps/atlas/c/cgoudet/Calibration/Data_8TeV_Zee_scale_ZeeNtuple.root"), "Name of Data root file with scale only")
    ("inDataScaleSmearName", po::value< string >(&inDataScaleSmearName)->default_value("/sps/atlas/c/cgoudet/Calibration/Data_8TeV_Zee_scale_smear_ZeeNtuple.root"), "Name of Data root file with scale a,d smearing")
    ("savingDirectory", po::value< string >( &savingDirectory )->default_value( "/sps/atlas/c/cgoudet/Calibration/plot/" ), "Directory where to put plots" )
    ;

  //Define options gathered by position                                              
  po::positional_options_description p;

  // create a map vm that contains options and all arguments of options              
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).positional(p).style(po::command_line_style::unix_style ^ po::command_line_style::allow_short).run(), vm);
  po::notify(vm);
  if(vm.count("help")) {cout << desc; return 0;}

  if ( savingDirectory.back() != '/' ) savingDirectory += "/";
  cout << outName << endl;
  TFile *outFile = new TFile( outName.c_str(), "RECREATE");


  //Load the input files into analyses
  Analysis aMCNoCorr,  aMCScale, aMCScaleSmear, aDataNoCorr, aDataScale, aDataScaleSmear;

  aMCNoCorr.Load( inMCName );
  TH1F *hMCNoCorr = (TH1F* ) aMCNoCorr.GetHist( "ZMass" )->Clone();
  hMCNoCorr->SetLineColor( 1 );
  outFile->cd();  
  hMCNoCorr->Write("", TObject::kOverwrite );
  
  aMCScale.Load( inMCScaleName );
  TH1F *hMCScale = (TH1F*) aMCScale.GetHist( "ZMass" )->Clone();
  hMCScale->SetLineColor( 2 );
  outFile->cd();
  hMCScale->Write("", TObject::kOverwrite );

  aMCScaleSmear.Load( inMCScaleSmearName );
  TH1F *hMCScaleSmear = (TH1F*) aMCScaleSmear.GetHist( "ZMass" )->Clone();
  hMCScaleSmear->SetLineColor( 4 );
  outFile->cd();
  hMCScaleSmear->Write("", TObject::kOverwrite );

  aDataNoCorr.Load( inDataName );
  TH1F *hDataNoCorr = (TH1F*)  aDataNoCorr.GetHist( "ZMass" )->Clone();
  hDataNoCorr->SetLineColor( 1 );
  outFile->cd();
  hDataNoCorr->Write("", TObject::kOverwrite );
  

  aDataScale.Load( inDataScaleName );
  TH1F *hDataScale = (TH1F*) aDataScale.GetHist( "ZMass" )->Clone();
  hDataScale->SetLineColor( 2 );
  outFile->cd();
  hDataScale->Write("", TObject::kOverwrite );

  aDataScaleSmear.Load( inDataScaleSmearName );
  TH1F *hDataScaleSmear = (TH1F*) aDataScaleSmear.GetHist( "ZMass" )->Clone();
  hDataScaleSmear->SetLineColor( 4 );
  outFile->cd();
  hDataScaleSmear->Write("", TObject::kOverwrite );
  
  cout << "done" << endl;
  
  //Create the checks plot/
  //MC distribution without correction and with scale only should be identical
  //Data distributions with just alpha or both correction must be identical
  TH1F *hMCCompareScale = (TH1F*) hMCNoCorr->Clone();
  hMCCompareScale->Divide( hMCScale );
  hMCCompareScale->SetLabelSize(0.1, "Y");
  hMCCompareScale->SetLabelSize(0.1, "X");
  hMCCompareScale->SetTitleSize(0.1,"Y");
  hMCCompareScale->SetTitleSize(0.1,"X");
  hMCCompareScale->SetTitleOffset(0.9,"X");
  hMCCompareScale->SetTitleOffset(0.5,"Y");
  hMCCompareScale->SetYTitle("NoCorr/NoSmearing");
  hMCCompareScale->SetTitle("");

  TH1F *hMCCompareSmear = (TH1F*) hDataScale->Clone();
  hMCCompareSmear->Divide( hMCScale );
  hMCCompareSmear->SetLabelSize(0.1, "Y");
  hMCCompareSmear->SetLabelSize(0.1, "X");
  hMCCompareSmear->SetTitleSize(0.1,"Y");
  hMCCompareSmear->SetTitleSize(0.1,"X");
  hMCCompareSmear->SetTitleOffset(0.9,"X");
  hMCCompareSmear->SetTitleOffset(0.5,"Y");
  hMCCompareSmear->SetYTitle("FullCorr/NoSmearing");
  hMCCompareSmear->SetTitle("");

  TH1F *hDataCompareScale = (TH1F*) hDataNoCorr->Clone();
  hDataCompareScale->Divide( hDataScale );
  hDataCompareScale->SetLabelSize(0.1, "Y");
  hDataCompareScale->SetLabelSize(0.1, "X");
  hDataCompareScale->SetTitleSize(0.1,"Y");
  hDataCompareScale->SetTitleSize(0.1,"X");
  hDataCompareScale->SetTitleOffset(0.9,"X");
  hDataCompareScale->SetTitleOffset(0.5,"Y");
  hDataCompareScale->SetYTitle("NoCorr/NoSmearing");
  hDataCompareScale->SetTitle("");

  TH1F *hDataCompareSmear = (TH1F*) hDataScale->Clone();
  hDataCompareSmear->Divide( hDataScale );
  hDataCompareSmear->SetLabelSize(0.1, "Y");
  hDataCompareSmear->SetLabelSize(0.1, "X");
  hDataCompareSmear->SetTitleSize(0.1,"Y");
  hDataCompareSmear->SetTitleSize(0.1,"X");
  hDataCompareSmear->SetTitleOffset(0.9,"X");
  hDataCompareSmear->SetTitleOffset(0.5,"Y");
  hDataCompareSmear->SetYTitle("FullCorr/NoSmearing");
  hDataCompareSmear->SetTitle("");

  TH1F *hDum = 0;
  TH1F *hRatioNoCorr = (TH1F*)  hDataNoCorr->Clone();
  hRatioNoCorr->Scale( 1/ hRatioNoCorr->Integral() );
  hDum= (TH1F*) hMCNoCorr->Clone();
  hDum->Scale( 1/ hDum->Integral() );
  hRatioNoCorr->Divide( hDum );
  hRatioNoCorr->SetYTitle("Data/MC");
  hRatioNoCorr->SetTitle("Data/MC");
  hRatioNoCorr->SetLineColor( 1 );
  delete hDum; hDum = 0;

  TH1F *hRatioScale = (TH1F*)  hDataScale->Clone();
  hRatioScale->Scale( 1/ hRatioScale->Integral() );
  hDum= (TH1F*) hMCScale->Clone();
  hDum->Scale( 1/ hDum->Integral() );
  hRatioScale->Divide( hDum );
  hRatioScale->SetYTitle("Data/MC");
  hRatioScale->SetTitle("Data/MC");
  hRatioScale->SetLineColor( kRed );
  delete hDum; hDum = 0;

  TH1F *hRatioScaleSmear = (TH1F*)  hDataScaleSmear->Clone();
  hRatioScaleSmear->Scale( 1/ hRatioScaleSmear->Integral() );
  hDum= (TH1F*) hMCScaleSmear->Clone();
  hDum->Scale( 1/ hDum->Integral() );
  hRatioScaleSmear->Divide( hDum );
  hRatioScaleSmear->SetYTitle("Data/MC");
  hRatioScaleSmear->SetTitle("Data/MC");
  hRatioScaleSmear->SetLineColor( 4 );
  delete hDum; hDum = 0;

  //Create canvas and pads
  TCanvas *canvas = new TCanvas();
  TLegend *legend = new TLegend( 0.7, 0.8, 1, 1 );
  legend->SetFillColor( 0 );
  TPad *padUp = new TPad( "padUp", "padUp", 0, 0.3, 1, 1 );
  padUp->SetBottomMargin(0);
  padUp->Draw();
  canvas->cd();
  TPad *padDown = new TPad( "padDown", "padDown", 0, 0, 1, 0.3 );
  padDown->SetTopMargin(0);
  padDown->SetBottomMargin(0.2);
  padDown->Draw();

  //plots histogams
  padUp->cd();
  legend->AddEntry( hMCNoCorr, "MC No_Corr", "lpe" );
  legend->AddEntry( hMCScale, "MC No_resolution", "lpe" );
  hMCNoCorr->Draw();
  hMCScale->Draw("SAME");
  legend->Draw();
  padDown->cd();
  hMCCompareScale->Draw();
  canvas->SaveAs( TString( savingDirectory + "MCCompareScale.pdf" ) );
  delete legend;

  padUp->cd();
  legend->Clear();
  legend->AddEntry( hMCScaleSmear, "MC Full_correction", "lpe" );
  legend->AddEntry( hMCScale, "MC No_resolution", "lpe" );
  hMCScaleSmear->Draw();
  hMCScale->Draw("SAME");
  legend->Draw();
  padDown->cd();
  hMCCompareSmear->Draw();
  canvas->SaveAs( TString( savingDirectory + "MCCompareSmear.pdf" ) );
  delete legend;

  padUp->cd();
  legend->Clear();
  hDataNoCorr->Draw();
  hDataScale->Draw("SAME");
  legend->AddEntry( hDataNoCorr, "Data No_correction", "lpe" );
  legend->AddEntry( hDataScale, "Data No_resolution", "lpe" );
  legend->Draw();
  padDown->cd();
  hDataCompareScale->Draw();
  canvas->SaveAs( TString( savingDirectory + "DataCompareScale.pdf" ) );
  delete legend;

  padUp->cd();
  legend->Clear();
  legend->AddEntry( hDataScaleSmear, "Data Full_correction", "lpe" );
  legend->AddEntry( hDataScale, "Data No_resolution", "lpe" );
  hDataScaleSmear->Draw();
  hDataScale->Draw("SAME");
  legend->Draw();
  padDown->cd();
  hDataCompareSmear->Draw();
  canvas->SaveAs( TString( savingDirectory + "DataCompareSmear.pdf" ) );
  delete legend;



  //RatioPlot
  legend->Clear();
  canvas->cd();
  hRatioNoCorr->Draw("e");
  hRatioScale->Draw("SAME e");
  hRatioScaleSmear->Draw("SAME E");
  legend->AddEntry( hRatioNoCorr, "No_correction", "lpe");
  legend->AddEntry( hRatioScale, "No_resolution", "lpe");
  legend->AddEntry( hRatioScaleSmear, "Full_correction", "lpe");
  legend->Draw();
  canvas->SaveAs( TString( savingDirectory + "Ratio.pdf" ) );

  legend->Clear();
  hMCNoCorr->Draw( "e");
  legend->AddEntry( hMCNoCorr, "MC MVA Calib", "lpe");
  legend->Draw();
  canvas->SaveAs( TString( savingDirectory + "MCNoCorr.pdf" ) );

  hMCScale->Draw( "e");
  canvas->SaveAs( TString( savingDirectory + "MCScale.pdf" ) );
  hMCScaleSmear->Draw( "e");
  canvas->SaveAs( TString( savingDirectory + "MCScaleSmear.pdf" ) );

  legend->Clear();
  hDataNoCorr->Draw( "e");
  legend->AddEntry( hDataNoCorr, "Data MVA Calib", "lpe");
  legend->Draw();

  canvas->SaveAs( TString( savingDirectory + "DataNoCorr.pdf" ) );
  hDataScale->Draw( "e");
  canvas->SaveAs( TString( savingDirectory + "DataScale.pdf" ) );
  hDataScaleSmear->Draw( "e");
  canvas->SaveAs( TString( savingDirectory + "DataScaleSmear.pdf" ) );



  return 0;
}

