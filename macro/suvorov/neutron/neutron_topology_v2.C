#include "TFile.h"
#include "TTree.h"
#include "TH2F.h"
#include "TF1.h"
#include "TStyle.h"
#include "TPad.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TRandom3.h"
#include "TLegend.h"
//#include "TND280UpEvent.hh"

#include <iostream>
#include <sstream>

#include "TChain.h"
#include "TCanvas.h"

using namespace std;;

void neutron_topology_v2() {
	TString input_file_path = "/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/neutron_step_v4.root";
  TFile* file = new TFile(input_file_path.Data(), "READ");
  TString file_name;
  

  TCanvas c1("canva","",50,50,1000,800);

  TFile* out_file = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/top_v4.root", "RECREATE");

  TH1F* nCelastic   = (TH1F*)file->Get("nC");
  TH1F* npElastic   = (TH1F*)file->Get("np");
  TH1F* nInElastic  = (TH1F*)file->Get("nInelastic");
  TH1F* nB          = (TH1F*)file->Get("B");
  TH1F* nLi         = (TH1F*)file->Get("Li");
  TH1F* treealpha   = (TH1F*)file->Get("3alpha");
  TH1F* nBe         = (TH1F*)file->Get("Be");

  TH1F* ZpZaE         = (TH1F*)file->Get("0p0aE");
  TH1F* ZpZaIE        = (TH1F*)file->Get("0p0aIE");
  TH1F* ZpOa          = (TH1F*)file->Get("0p1a");
  TH1F* ZpNa          = (TH1F*)file->Get("0pNa");
  TH1F* OpZa          = (TH1F*)file->Get("1p0a");
  TH1F* OpOa          = (TH1F*)file->Get("1p1a");
  TH1F* OpNa          = (TH1F*)file->Get("1pNa");
  TH1F* NpNa          = (TH1F*)file->Get("NpNa");

  TH1F* norm          = (TH1F*)file->Get("XsecNorm");

  nCelastic->Divide(norm);
  npElastic->Divide(norm);
  nInElastic->Divide(norm);
  nB->Divide(norm);       
  nLi->Divide(norm);      
  treealpha->Divide(norm);
  nBe->Divide(norm);  
  ZpZaE->Divide(norm); 
  ZpZaIE->Divide(norm);
  ZpOa->Divide(norm);  
  ZpNa->Divide(norm);  
  OpZa->Divide(norm);  
  OpOa->Divide(norm);  
  OpNa->Divide(norm);  
  NpNa->Divide(norm);      

  nCelastic->SetLineColor(kBlue);
  npElastic->SetLineColor(kRed);
  nInElastic->SetLineColor(kGreen);
  nB->SetLineColor(kBlack);
  nLi->SetLineColor(kYellow);
  treealpha->SetLineColor(kViolet);
  nBe->SetLineColor(kCyan);

  TH1F* h_tot = (TH1F*)nCelastic->Clone("total");
  h_tot->Add(npElastic);
  h_tot->Add(nInElastic);

  TH1F* h_tot_C = (TH1F*)nCelastic->Clone("total_C");
  h_tot_C->Add(nInElastic);

  out_file->cd();
  nCelastic->Write();
  npElastic->Write();
  nInElastic->Write();
  nB->Write();
  nLi->Write();
  treealpha->Write();
  nBe->Write();
  h_tot->Write();
  h_tot_C->Write();

	c1.SetLogy();
	c1.SetLogx();
  gStyle->SetOptTitle(kFALSE);
  gStyle->SetOptStat(kFALSE);
  auto legend = new TLegend(0.1,0.7,0.48,0.9);
	nCelastic->Draw(" hist");
  nCelastic->GetXaxis()->SetRangeUser(5., 900.);
	npElastic->Draw("same hist");
	nInElastic->Draw("same hist");
  nB->Draw("same hist");
  nLi->Draw("same hist");
  nBe->Draw("same hist");
  treealpha->Draw("same hist");
  legend->AddEntry(nCelastic, "nC Elastic");
  legend->AddEntry(npElastic, "np Elastic");
  legend->AddEntry(nInElastic, "n InElastic total");
  legend->AddEntry(nB, "Inelastic with B");
  legend->AddEntry(nLi, "Inelastic with Li");
  legend->AddEntry(treealpha, "Inelastic with 3 alpha");
  legend->AddEntry(nBe, "Inelastic with Be");
  legend->Draw();
  c1.SetName("TotalRate");
  c1.SetTitle("Total rate");
	c1.Write();

  c1.SetLogx(0);
  c1.SetLogy(0);
  npElastic->GetXaxis()->SetRangeUser(150., 900.);
  npElastic->Draw("hist");
  c1.SetName("nH");
  c1.SetTitle("np elastic");
  c1.Write();

  c1.SetLogx(1);
  c1.SetLogy(1);
  h_tot_C->GetXaxis()->SetRangeUser(5., 900.);
  h_tot_C->Draw("hist");
  c1.SetName("nC");
  c1.SetTitle("nC total");
  c1.Write();

  npElastic->GetXaxis()->SetRangeUser(5., 900.);
  npElastic->SetLineColor(kRed);
  npElastic->Draw("hist");
  h_tot_C->Draw("hist same");

  c1.SetName("nCnp");
  c1.SetTitle("nCnp total");
  c1.Write();

  auto legend2 = new TLegend(0.1,0.7,0.48,0.9);
  ZpZaE->SetLineColor(kBlack);
  ZpZaIE->SetLineColor(kRed);
  ZpOa->SetLineColor(kGreen);
  ZpNa->SetLineColor(kYellow);
  OpZa->SetLineColor(kBlue);
  OpOa->SetLineColor(kViolet);
  NpNa->SetLineColor(kCyan);

  legend2->AddEntry(ZpZaE, "0p 0alphs Elastic");
  legend2->AddEntry(ZpZaIE, "0p 0alphs inElastic");
  legend2->AddEntry(ZpOa, "0p 1alphs");
  legend2->AddEntry(ZpNa, "0p Nalphs");
  legend2->AddEntry(OpZa, "1p 0alphs");
  legend2->AddEntry(OpOa, "1p 1alphs");
  legend2->AddEntry(NpNa, "Np Nalphs");

  ZpZaE->Draw("hist");
  ZpZaIE->Draw("hist same");
  ZpOa->Draw("hist same");
  ZpNa->Draw("hist same");
  OpZa->Draw("hist same");
  OpOa->Draw("hist same");
  NpNa->Draw("hist same");

  legend2->Draw();

  c1.SetName("Topologies");
  c1.SetTitle("Topologies total");
  c1.Write();

  ZpZaE->Write();
  ZpZaIE->Write();
  ZpOa->Write();
  ZpNa->Write();
  OpZa->Write();
  OpOa->Write();
  OpNa->Write();
  NpNa->Write();

  out_file->Close();
}