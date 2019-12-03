#define THIS_NAME chargeDist
#define NOINTERACTIVE_OUTPUT
#define OVERRIDE_OPTIONS

#include "../../utils/global_header.h"
#include "../../utils/global_tools.C"

void chargeDist() {

  gStyle->SetOptFit(1);
  gStyle->SetOptStat(1000000010);

  Int_t T2KstyleIndex = 3;
  // Official T2K style as described in http://www.t2k.org/comm/pubboard/style/index_html
  TString localStyleName = "T2K";
  // -- WhichStyle --
  // 1 = presentation large fonts
  // 2 = presentation small fonts
  // 3 = publication/paper

  TStyle* t2kstyle = SetT2KStyle(T2KstyleIndex, localStyleName);
  gROOT->SetStyle(t2kstyle->GetName());
  gROOT->ForceStyle(t2kstyle);

  TString prefix   = "/Users/cjesus/Documents/Data/MCvsData/";

  for (int iarg=0; iarg<gApplication->Argc(); iarg++){
    if (string( gApplication->Argv(iarg))=="-i" || string( gApplication->Argv(iarg))=="--input" ){
        iarg++;
        prefix = gApplication->Argv(iarg);
        cout << "path to analysis files: " << prefix << endl;
    }
    else if (string( gApplication->Argv(iarg))=="-l" || string( gApplication->Argv(iarg))=="--local" ){
      prefix = "/home/cjesus/Work/Data/SFGD_prototype/ANALYSIS/";
    }
  }

  TString fname1   = prefix + "ana_output_MC.root";
  TString fname2   = prefix + "ana_output_Data.root";

  TFile* fInput1   = new TFile(fname1.Data(), "READ");
  TFile* fInput2   = new TFile(fname2.Data(), "READ");

  TH1F* h_PE_ALL_xz_LI_MC  = (TH1F*) fInput1->Get("h_PE_ALL_xz_LI");
  TH1F* h_PE_ALL_xz_LI_DT  = (TH1F*) fInput2->Get("h_PE_ALL_xz_LI");
  TH1F* h_PE_ALL_xz_HI_MC  = (TH1F*) fInput1->Get("h_PE_ALL_xz_HI");
  TH1F* h_PE_ALL_xz_HI_DT  = (TH1F*) fInput2->Get("h_PE_ALL_xz_HI");

  TH1F* h_PE_ALL_yz_LI_MC  = (TH1F*) fInput1->Get("h_PE_ALL_yz_LI");
  TH1F* h_PE_ALL_yz_LI_DT  = (TH1F*) fInput2->Get("h_PE_ALL_yz_LI");
  TH1F* h_PE_ALL_yz_HI_MC  = (TH1F*) fInput1->Get("h_PE_ALL_yz_HI");
  TH1F* h_PE_ALL_yz_HI_DT  = (TH1F*) fInput2->Get("h_PE_ALL_yz_HI");

  if(h_PE_ALL_xz_LI_MC->GetEntries()) h_PE_ALL_xz_LI_MC->Scale(1/h_PE_ALL_xz_LI_MC->GetEntries(), "width");
  if(h_PE_ALL_xz_LI_DT->GetEntries()) h_PE_ALL_xz_LI_DT->Scale(1/h_PE_ALL_xz_LI_DT->GetEntries(), "width");
  if(h_PE_ALL_xz_HI_MC->GetEntries()) h_PE_ALL_xz_HI_MC->Scale(1/h_PE_ALL_xz_HI_MC->GetEntries(), "width");
  if(h_PE_ALL_xz_HI_DT->GetEntries()) h_PE_ALL_xz_HI_DT->Scale(1/h_PE_ALL_xz_HI_DT->GetEntries(), "width");

  if(h_PE_ALL_yz_LI_MC->GetEntries()) h_PE_ALL_yz_LI_MC->Scale(1/h_PE_ALL_yz_LI_MC->GetEntries(), "width");
  if(h_PE_ALL_yz_LI_DT->GetEntries()) h_PE_ALL_yz_LI_DT->Scale(1/h_PE_ALL_yz_LI_DT->GetEntries(), "width");
  if(h_PE_ALL_yz_HI_MC->GetEntries()) h_PE_ALL_yz_HI_MC->Scale(1/h_PE_ALL_yz_HI_MC->GetEntries(), "width");
  if(h_PE_ALL_yz_HI_DT->GetEntries()) h_PE_ALL_yz_HI_DT->Scale(1/h_PE_ALL_yz_HI_DT->GetEntries(), "width");

  h_PE_ALL_xz_LI_MC->SetLineColor(kBlue);
  h_PE_ALL_xz_LI_DT->SetLineColor(kRed);
  h_PE_ALL_xz_HI_MC->SetLineColor(kBlue);
  h_PE_ALL_xz_HI_DT->SetLineColor(kRed);

  h_PE_ALL_yz_LI_MC->SetLineColor(kBlue);
  h_PE_ALL_yz_LI_DT->SetLineColor(kRed);
  h_PE_ALL_yz_HI_MC->SetLineColor(kBlue);
  h_PE_ALL_yz_HI_DT->SetLineColor(kRed);

  TCanvas* c1 = new TCanvas("c1");
  c1->Divide(2,2);
  c1->cd(1);
  h_PE_ALL_xz_LI_MC->Draw("HIST");
  h_PE_ALL_xz_LI_DT->Draw("HIST same");
  c1->cd(2);
  h_PE_ALL_xz_HI_MC->Draw("HIST");
  h_PE_ALL_xz_HI_DT->Draw("HIST same");

  c1->cd(3);
  h_PE_ALL_yz_LI_MC->Draw("HIST");
  h_PE_ALL_yz_LI_DT->Draw("HIST same");
  c1->cd(4);
  h_PE_ALL_yz_HI_MC->Draw("HIST");
  h_PE_ALL_yz_HI_DT->Draw("HIST same");

  c1->Update();
  c1->WaitPrimitive();


}
