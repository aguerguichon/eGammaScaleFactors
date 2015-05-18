void plot() {
  
  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendFillColor(4);

  TCanvas* c1 = new TCanvas();

  TFile fu("iq2_66-116.root");
  TFile fd("iq1_66-116.root");

  TGraph* u = (TGraph*)fu.Get("wpow1");
  TGraph* d = (TGraph*)fd.Get("wpow1");

  d->SetTitle("Fixed #rightarrow Running mass and width");
  d->GetXaxis()->SetTitle("M [GeV]");
  d->GetYaxis()->SetTitle("Weight");
  u->SetLineWidth(2); u->SetLineColor(4); u->SetLineStyle(2);
  d->SetLineWidth(2); d->SetLineColor(2);
  d->SetMinimum(0.96); d->SetMaximum(1.04); 
  d->Draw("al");
  u->Draw("l");
  TLegend* legend = new TLegend(.7,.75,.85,.85);
  legend->AddEntry(u,"u-quarks","l");
  legend->AddEntry(d,"d-quarks","l");
  legend->SetFillColor(0);
  legend->Draw();
  
  c1->SaveAs("FixedRunningWidth.png");

  TGraph* fwu = (TGraph*)fu.Get("wpyt1");
  TGraph* fwd = (TGraph*)fd.Get("wpyt1");

  fwd->SetTitle("Width correction : 2.481 #rightarrow 2.495 GeV");
  fwd->GetXaxis()->SetTitle("M [GeV]");
  fwd->GetYaxis()->SetTitle("Weight");
  fwu->SetLineWidth(2); fwu->SetLineColor(4); fwu->SetLineStyle(2);
  fwd->SetLineWidth(2); fwd->SetLineColor(2);
  fwd->SetMinimum(0.96); fwd->SetMaximum(1.04); 
  fwd->Draw("al");
  fwu->Draw("l");
  TLegend* legend = new TLegend(.7,.75,.85,.85);
  legend->AddEntry(fwu,"u-quarks","l");
  legend->AddEntry(fwd,"d-quarks","l");
  legend->SetFillColor(0);
  legend->Draw();

  c1->SaveAs("WidthCorrection.png");



  TFile fuhm("iq2_100-2000.root");
  TFile fdhm("iq1_100-2000.root");

  TGraph* drat = (TGraph*)fdhm.Get("pytpow");
  TGraph* urat = (TGraph*)fuhm.Get("pytpow");
  TGraph* dpow = (TGraph*)fdhm.Get("wpow2");
  TGraph* upow = (TGraph*)fuhm.Get("wpow2");
  TGraph* dpyt = (TGraph*)fdhm.Get("wpyt2");
  TGraph* upyt = (TGraph*)fuhm.Get("wpyt2");

  drat->SetTitle("Pythia / Powheg");
  drat->GetXaxis()->SetTitle("M [GeV]");
  drat->GetYaxis()->SetTitle("Ratio");
  drat->SetLineWidth(2); drat->SetLineColor(2);
  urat->SetLineWidth(2); urat->SetLineColor(4); urat->SetLineStyle(2);
  drat->SetMinimum(0.99); drat->SetMaximum(1.1); 
  drat->Draw("al");
  urat->Draw("l");
  legend->Clear();
  legend->AddEntry(urat,"u-quarks","l");
  legend->AddEntry(drat,"d-quarks","l");
  legend->SetFillColor(0);
  legend->Draw();

  c1->SaveAs("PythiaToPowheg_100-2000.png");


  dpow->SetTitle("Powheg #rightarrow Improved Born");
  dpow->GetXaxis()->SetTitle("M [GeV]");
  dpow->GetYaxis()->SetTitle("Weight");
  dpow->SetLineWidth(2); dpow->SetLineColor(2);
  upow->SetLineWidth(2); upow->SetLineColor(4); upow->SetLineStyle(2);
  dpow->SetMinimum(.98); dpow->SetMaximum(1.06); 
  dpow->Draw("al");
  upow->Draw("l");
  legend->Clear();
  legend->AddEntry(upow,"u-quarks","l");
  legend->AddEntry(dpow,"d-quarks","l");
  legend->SetFillColor(0);
  legend->Draw();

  c1->SaveAs("PowhegToIBA_ud_100-2000.png");


  dpyt->SetTitle("Pythia #rightarrow Improved Born");
  dpyt->GetXaxis()->SetTitle("M [GeV]");
  dpyt->GetYaxis()->SetTitle("Weight");
  dpyt->SetLineWidth(2); dpyt->SetLineColor(2);
  upyt->SetLineWidth(2); upyt->SetLineColor(4); upyt->SetLineStyle(2);
  dpyt->SetMaximum(1.02); dpyt->SetMinimum(.94); 
  dpyt->Draw("al");
  upyt->Draw("l");
  legend->Clear();
  legend->AddEntry(upyt,"u-quarks","l");
  legend->AddEntry(dpyt,"d-quarks","l");
  legend->SetFillColor(0);
  legend->Draw();

   c1->SaveAs("PythiaToIBA_ud_100-2000.png");


  TFile fuhm("iq2_66-116.root");
  TFile fdhm("iq1_66-116.root");

  TGraph* drat = (TGraph*)fdhm.Get("pytpow");
  TGraph* urat = (TGraph*)fuhm.Get("pytpow");
  TGraph* dpow = (TGraph*)fdhm.Get("wpow2");
  TGraph* upow = (TGraph*)fuhm.Get("wpow2");
  TGraph* dpyt = (TGraph*)fdhm.Get("wpyt2");
  TGraph* upyt = (TGraph*)fuhm.Get("wpyt2");

  drat->SetTitle("Pythia / Powheg");
  drat->GetXaxis()->SetTitle("M [GeV]");
  drat->GetYaxis()->SetTitle("Ratio");
  drat->SetLineWidth(2); drat->SetLineColor(2);
  urat->SetLineWidth(2); urat->SetLineColor(4); urat->SetLineStyle(2);
  drat->SetMinimum(0.99); drat->SetMaximum(1.01); 
  drat->Draw("al");
  urat->Draw("l");
  legend->Clear();
  legend->AddEntry(urat,"u-quarks","l");
  legend->AddEntry(drat,"d-quarks","l");
  legend->SetFillColor(0);
  legend->Draw();

  c1->SaveAs("PythiaToPowheg_66-116.png");


  dpow->SetTitle("Powheg #rightarrow Improved Born");
  dpow->GetXaxis()->SetTitle("M [GeV]");
  dpow->GetYaxis()->SetTitle("Weight");
  dpow->SetLineWidth(2); dpow->SetLineColor(2);
  upow->SetLineWidth(2); upow->SetLineColor(4); upow->SetLineStyle(2);
  dpow->SetMinimum(.99); dpow->SetMaximum(1.01); 
  dpow->Draw("al");
  upow->Draw("l");
  legend->Clear();
  legend->AddEntry(upow,"u-quarks","l");
  legend->AddEntry(dpow,"d-quarks","l");
  legend->SetFillColor(0);
  legend->Draw();

  c1->SaveAs("PowhegToIBA_ud_66-116.png");


  dpyt->SetTitle("Pythia #rightarrow Improved Born");
  dpyt->GetXaxis()->SetTitle("M [GeV]");
  dpyt->GetYaxis()->SetTitle("Weight");
  dpyt->SetLineWidth(2); dpyt->SetLineColor(2);
  upyt->SetLineWidth(2); upyt->SetLineColor(4); upyt->SetLineStyle(2);
  dpyt->SetMaximum(1.01); dpyt->SetMinimum(.99); 
  dpyt->Draw("al");
  upyt->Draw("l");
  legend->Clear();
  legend->AddEntry(upyt,"u-quarks","l");
  legend->AddEntry(dpyt,"d-quarks","l");
  legend->SetFillColor(0);
  legend->Draw();

   c1->SaveAs("PythiaToIBA_ud_66-116.png");


}
void plot() {
  
  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendFillColor(4);

  TCanvas* c1 = new TCanvas();

  TFile fu("iq2_66-116.root");
  TFile fd("iq1_66-116.root");

  TGraph* u = (TGraph*)fu.Get("wpow1");
  TGraph* d = (TGraph*)fd.Get("wpow1");

  d->SetTitle("Fixed #rightarrow Running mass and width");
  d->GetXaxis()->SetTitle("M [GeV]");
  d->GetYaxis()->SetTitle("Weight");
  u->SetLineWidth(2); u->SetLineColor(4); u->SetLineStyle(2);
  d->SetLineWidth(2); d->SetLineColor(2);
  d->SetMinimum(0.96); d->SetMaximum(1.04); 
  d->Draw("al");
  u->Draw("l");
  TLegend* legend = new TLegend(.7,.75,.85,.85);
  legend->AddEntry(u,"u-quarks","l");
  legend->AddEntry(d,"d-quarks","l");
  legend->SetFillColor(0);
  legend->Draw();
  
  c1->SaveAs("FixedRunningWidth.png");

  TGraph* fwu = (TGraph*)fu.Get("wpyt1");
  TGraph* fwd = (TGraph*)fd.Get("wpyt1");

  fwd->SetTitle("Width correction : 2.481 #rightarrow 2.495 GeV");
  fwd->GetXaxis()->SetTitle("M [GeV]");
  fwd->GetYaxis()->SetTitle("Weight");
  fwu->SetLineWidth(2); fwu->SetLineColor(4); fwu->SetLineStyle(2);
  fwd->SetLineWidth(2); fwd->SetLineColor(2);
  fwd->SetMinimum(0.96); fwd->SetMaximum(1.04); 
  fwd->Draw("al");
  fwu->Draw("l");
  TLegend* legend = new TLegend(.7,.75,.85,.85);
  legend->AddEntry(fwu,"u-quarks","l");
  legend->AddEntry(fwd,"d-quarks","l");
  legend->SetFillColor(0);
  legend->Draw();

  c1->SaveAs("WidthCorrection.png");



  TFile fuhm("iq2_100-2000.root");
  TFile fdhm("iq1_100-2000.root");

  TGraph* drat = (TGraph*)fdhm.Get("pytpow");
  TGraph* urat = (TGraph*)fuhm.Get("pytpow");
  TGraph* dpow = (TGraph*)fdhm.Get("wpow2");
  TGraph* upow = (TGraph*)fuhm.Get("wpow2");
  TGraph* dpyt = (TGraph*)fdhm.Get("wpyt2");
  TGraph* upyt = (TGraph*)fuhm.Get("wpyt2");

  drat->SetTitle("Pythia / Powheg");
  drat->GetXaxis()->SetTitle("M [GeV]");
  drat->GetYaxis()->SetTitle("Ratio");
  drat->SetLineWidth(2); drat->SetLineColor(2);
  urat->SetLineWidth(2); urat->SetLineColor(4); urat->SetLineStyle(2);
  drat->SetMinimum(0.99); drat->SetMaximum(1.1); 
  drat->Draw("al");
  urat->Draw("l");
  legend->Clear();
  legend->AddEntry(urat,"u-quarks","l");
  legend->AddEntry(drat,"d-quarks","l");
  legend->SetFillColor(0);
  legend->Draw();

  c1->SaveAs("PythiaToPowheg_100-2000.png");


  dpow->SetTitle("Powheg #rightarrow Improved Born");
  dpow->GetXaxis()->SetTitle("M [GeV]");
  dpow->GetYaxis()->SetTitle("Weight");
  dpow->SetLineWidth(2); dpow->SetLineColor(2);
  upow->SetLineWidth(2); upow->SetLineColor(4); upow->SetLineStyle(2);
  dpow->SetMinimum(.98); dpow->SetMaximum(1.06); 
  dpow->Draw("al");
  upow->Draw("l");
  legend->Clear();
  legend->AddEntry(upow,"u-quarks","l");
  legend->AddEntry(dpow,"d-quarks","l");
  legend->SetFillColor(0);
  legend->Draw();

  c1->SaveAs("PowhegToIBA_ud_100-2000.png");


  dpyt->SetTitle("Pythia #rightarrow Improved Born");
  dpyt->GetXaxis()->SetTitle("M [GeV]");
  dpyt->GetYaxis()->SetTitle("Weight");
  dpyt->SetLineWidth(2); dpyt->SetLineColor(2);
  upyt->SetLineWidth(2); upyt->SetLineColor(4); upyt->SetLineStyle(2);
  dpyt->SetMaximum(1.02); dpyt->SetMinimum(.94); 
  dpyt->Draw("al");
  upyt->Draw("l");
  legend->Clear();
  legend->AddEntry(upyt,"u-quarks","l");
  legend->AddEntry(dpyt,"d-quarks","l");
  legend->SetFillColor(0);
  legend->Draw();

   c1->SaveAs("PythiaToIBA_ud_100-2000.png");


  TFile fuhm("iq2_66-116.root");
  TFile fdhm("iq1_66-116.root");

  TGraph* drat = (TGraph*)fdhm.Get("pytpow");
  TGraph* urat = (TGraph*)fuhm.Get("pytpow");
  TGraph* dpow = (TGraph*)fdhm.Get("wpow2");
  TGraph* upow = (TGraph*)fuhm.Get("wpow2");
  TGraph* dpyt = (TGraph*)fdhm.Get("wpyt2");
  TGraph* upyt = (TGraph*)fuhm.Get("wpyt2");

  drat->SetTitle("Pythia / Powheg");
  drat->GetXaxis()->SetTitle("M [GeV]");
  drat->GetYaxis()->SetTitle("Ratio");
  drat->SetLineWidth(2); drat->SetLineColor(2);
  urat->SetLineWidth(2); urat->SetLineColor(4); urat->SetLineStyle(2);
  drat->SetMinimum(0.99); drat->SetMaximum(1.01); 
  drat->Draw("al");
  urat->Draw("l");
  legend->Clear();
  legend->AddEntry(urat,"u-quarks","l");
  legend->AddEntry(drat,"d-quarks","l");
  legend->SetFillColor(0);
  legend->Draw();

  c1->SaveAs("PythiaToPowheg_66-116.png");


  dpow->SetTitle("Powheg #rightarrow Improved Born");
  dpow->GetXaxis()->SetTitle("M [GeV]");
  dpow->GetYaxis()->SetTitle("Weight");
  dpow->SetLineWidth(2); dpow->SetLineColor(2);
  upow->SetLineWidth(2); upow->SetLineColor(4); upow->SetLineStyle(2);
  dpow->SetMinimum(.99); dpow->SetMaximum(1.01); 
  dpow->Draw("al");
  upow->Draw("l");
  legend->Clear();
  legend->AddEntry(upow,"u-quarks","l");
  legend->AddEntry(dpow,"d-quarks","l");
  legend->SetFillColor(0);
  legend->Draw();

  c1->SaveAs("PowhegToIBA_ud_66-116.png");


  dpyt->SetTitle("Pythia #rightarrow Improved Born");
  dpyt->GetXaxis()->SetTitle("M [GeV]");
  dpyt->GetYaxis()->SetTitle("Weight");
  dpyt->SetLineWidth(2); dpyt->SetLineColor(2);
  upyt->SetLineWidth(2); upyt->SetLineColor(4); upyt->SetLineStyle(2);
  dpyt->SetMaximum(1.01); dpyt->SetMinimum(.99); 
  dpyt->Draw("al");
  upyt->Draw("l");
  legend->Clear();
  legend->AddEntry(upyt,"u-quarks","l");
  legend->AddEntry(dpyt,"d-quarks","l");
  legend->SetFillColor(0);
  legend->Draw();

   c1->SaveAs("PythiaToIBA_ud_66-116.png");


}
