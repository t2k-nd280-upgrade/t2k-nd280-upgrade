#include <iostream>
#include <sstream>
#include <fstream>
#include <list>
#include <vector>
#include <string>
#include <cstdlib>

#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "THStack.h"
#include "TChain.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TMath.h"

#include "TString.h"
#include "TLatex.h"

using namespace std;

TString INPDIR = "/nfs/neutrinos/cjesus/work/jobs/files/TDR_eff";
TString OUTDIR = "/nfs/neutrinos/cjesus/work/jobs/plots/Resolution_FSI";

int NTargets[4] = {2, 2, 3, 3};
TString branchNames[8] = {"CC0pi", "CC1pi", "CCoth", "FWD", "BWD", "HA", "Target", "ECal"};

const int NBins_CosTh = 20;
double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};

const int NBins_Mom = 29;
double BinEdges_Mom[NBins_Mom+1] = {0,50,100,150,200,250,300,350,400,450,500,550, 600,650,700,750,800,850,900,950,1000,1200, 1400,1600,1800,2000,2500,3000,4000,5000};

const int NBins_Q2 = 15;
double BinEdges_Q2[NBins_Q2+1] = {0, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1, 1.5, 2, 3, 5};

const int NBins_Enu = 14;
double BinEdges_Enu[NBins_Enu+1] = {0, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1200, 1500, 2000, 3000, 5000};

const int NbinsZ = 100;
double BinsZ[101];

const int NbinsZang = 20;
double BinsZang[21];
 
double PSRes_muonMom = 400;
double PSRes_muonCos = 0.4;

// plot the efficiency for a given file, configuration, target, event category
//   target: 1 = FGD-1 ; 2 = FGD-2 ; 3 = Super-FGD
//   categ:  0 = CC-0pi; 1 = CC-1pi; 2 = CC-other; 3 = FWD TPC; 4 = BWD TPC; 5 = HA TPC; 6 = iso-target; 7 = ECal;
//          -2 = only TPC and HA-TPC -1 = SFGD + TPC and HA-TPC
//   PS_Res: Phase space restriction. If set true reduces the phase-space with PSRes_muonMom and PSRes_muonCos
// files should be named as : config{confignumber)_Target(targetnumber)_suffix.root

TH1F* computeEff(TString suffix, int config, int target, int categ, int cut1, int cut2,
         TString var, TString var_title, 
         int nbins, double* xbins, bool PSRes=false) {
  
  TH1::AddDirectory(kFALSE);

  TFile f(TString::Format("%s/Target%i/config%i_Target%i%s.root", 
                  INPDIR.Data(),target, config, target, suffix.Data()));

  TTree* t = (TTree*)f.Get("truth");    

  TH1F* h_beforeCut = new TH1F(TString::Format("before_%s_%i_%i",var.Data(),config,target),
                   TString::Format("before_%s_%i_%i",var.Data(),config,target),
                   nbins, xbins);
  TH1F* h_afterCut = new TH1F(TString::Format("after_%s_%i_%i",var.Data(),config,target),
                  TString::Format("after_%s_%i_%i",var.Data(),config,target),
                  nbins, xbins);

  Int_t accum_level[8];
  Float_t variable;
  int topology;
  Float_t res_var;
  
  t->SetBranchAddress("accum_level", &accum_level);
  t->SetBranchAddress(var,           &variable);
  t->SetBranchAddress("topology",    &topology);

  if (PSRes && var=="true_costheta")
    t->SetBranchAddress("true_mom", &res_var);
  if (PSRes && var=="true_mom")
    t->SetBranchAddress("true_costheta", &res_var);
 
  vector<int> branches;
  if (categ == -2) {
    branches.push_back(3); 
    branches.push_back(4);
    branches.push_back(5);
  }
  if (categ == -1){
    branches.push_back(3); 
    branches.push_back(4);
    branches.push_back(5);
    branches.push_back(6);
  }
  else
    branches.push_back(categ);
    

  for (Int_t ient=0; ient < t->GetEntries(); ient++) {
      
    t->GetEntry(ient);

    // do we apply some phase space restriction for the efficiency computation
    if (PSRes && var=="true_costheta")
      if (res_var < PSRes_muonMom)
    continue;
    if (PSRes && var=="true_mom")
      if (res_var < PSRes_muonCos)
    continue;
      
    bool sel1 = false, sel2 = false;
    for (vector<int>::iterator it = branches.begin(); it != branches.end(); ++it) {
      bool topoCut = ((*it) >= 3) || (topology==(*it));   // If category is larger or equal to 3, the selection depends on the detector not in the topology.
      sel1 = sel1 || (accum_level[*it] > cut1 && topoCut);
      sel2 = sel2 || (accum_level[*it] > cut2 && topoCut);
    }

    if (sel1)
      h_beforeCut->Fill(variable);
    if (sel2)
      h_afterCut->Fill(variable);
    
  }

  // prepare the efficiency histogram
  TH1F *h_eff = new TH1F(TString::Format("h_eff_%s_%i_%i",var.Data(),config,target),
             TString::Format("h_eff_%s_%i_%i",var.Data(),config,target),
             nbins, xbins);
  h_eff->Divide(h_afterCut, h_beforeCut);

  for (int i=0; i<nbins; i++) {
    float eff = h_eff->GetBinContent(i+1);
    h_eff->SetBinError(i+1, sqrt(eff*(1-eff)/h_beforeCut->GetBinContent(i+1)));
  }
  
  h_eff->GetXaxis()->SetTitle(var_title);
  h_eff->GetXaxis()->SetTitleSize(0.05);
  h_eff->GetXaxis()->SetTitleOffset(0.9);
  h_eff->GetYaxis()->SetTitleSize(0.05);
  h_eff->GetYaxis()->SetTitle("efficiency");
  
  return h_eff;

}

void plot2D_Truth(TString suffix, int config, int target, int categ, int cut1, int cut2, 
      TString varX, TString var_titleX, TString varY, TString var_titleY, 
      int nbinsX, double* xbinsX, int nbinsY, double* xbinsY) {

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(1);
  gStyle->SetTitleAlign(23);
  TH1::AddDirectory(kFALSE);

  TFile f(TString::Format("%s/Target%i/config%i_Target%i%s.root",
        INPDIR.Data(),target, config, target, suffix.Data()));
  TTree* t = (TTree*)f.Get("truth");

  TH2F* hSig    = new TH2F("h1", "h2", nbinsX, xbinsX, nbinsY, xbinsY);
  TH2F* hSelSig = new TH2F("h2", "h2", nbinsX, xbinsX, nbinsY, xbinsY);
  TH2F* hSel    = new TH2F("h3", "h3", nbinsX, xbinsX, nbinsY, xbinsY);

  Int_t accum_level[8];
  Float_t variableX, variableY;
  int topology;
  int cntTimes=0;
  
  t->SetBranchAddress("accum_level", &accum_level);
  t->SetBranchAddress(varX,          &variableX);
  t->SetBranchAddress(varY,          &variableY);
  t->SetBranchAddress("topology",    &topology);

  vector<int> branches;
  if (categ == -2) {
    branches.push_back(3); 
    branches.push_back(4);
    branches.push_back(5);
  }
  else if (categ == -1){
    branches.push_back(3); 
    branches.push_back(4);
    branches.push_back(5);
    branches.push_back(6);
  }
  else
    branches.push_back(categ);

  cout <<endl << t->GetEntries() << endl;
  for (Int_t ient=0; ient < t->GetEntries(); ient++) {
    t->GetEntry(ient);

    bool sel1 = false, sel2 = false, sel3 = false;
    for (vector<int>::iterator it = branches.begin(); it != branches.end(); ++it) {
      bool topoCut = ((*it) >= 3) || (topology==(*it));
      sel1 = sel1 || (accum_level[*it] > cut1 && topoCut);
      sel2 = sel2 || (accum_level[*it] > cut2 && topoCut);
      sel3 = sel3 || (accum_level[*it] > cut2);
    }

    if (sel1)
      hSig->Fill(variableX, variableY);
    if (sel2)
      hSelSig->Fill(variableX, variableY);
    if (sel3)
      hSel->Fill(variableX, variableY);

  }
  f.Close();

  cout << "cntTimes: " << cntTimes << endl;

  TCanvas *c = new TCanvas("c", "c");

  TString suffixName = suffix;
  if (suffix == "")
    suffixName = "_nu";

  TString targetName="";
  if (config==0 || target<=2)
    targetName = TString::Format("FGD %i", target);
  else
    targetName = "SFGD";

  TString categName = "";
  if (categ == -1)
    categName = "SFGD_and_TPC";
  else if (categ == -2)
    categName = "TPC";
  else
    categName = branchNames[categ];

  TString namehist = TString::Format("HM_%s_%s",categName.Data(),targetName.Data());
  TH2F *h_eff = new TH2F(namehist, namehist, nbinsX, xbinsX, nbinsY, xbinsY);
  h_eff->Divide(hSelSig, hSig);
  TH2F *h_pur = new TH2F(namehist+"_pur", namehist, nbinsX, xbinsX, nbinsY, xbinsY);
  h_pur->Divide(hSelSig, hSel);

  h_eff->SetTitle(TString::Format("%s, %s, %s", 
          config==0 ? "Current":"Upgrade",
          targetName.Data(),
          categName.Data()));

  h_eff->GetXaxis()->SetTitle(var_titleX);
  h_eff->GetXaxis()->SetTitleSize(0.05);
  h_eff->GetXaxis()->SetTitleOffset(0.9);
  h_eff->GetYaxis()->SetTitle(var_titleY);
  h_eff->GetYaxis()->SetTitleSize(0.05);
  h_eff->GetYaxis()->SetTitleOffset(0.9);
  h_eff->GetZaxis()->SetRangeUser(0.,1.);
  
  h_eff->Draw("colz");
  c->Update();
  c->SaveAs(TString::Format("%s/2D_eff_config%i_target%i_total_%s_%s_%s%s.pdf", 
          OUTDIR.Data(), config, target, varX.Data(), varY.Data(), categName.Data(), suffixName.Data()));

  // TFile fW(TString::Format("%s/2D_eff_config%i_target%i_total_%s_%s_%s%s.root", 
  //        OUTDIR.Data(), config, target, varX.Data(), varY.Data(), categName.Data(), suffixName.Data()), "CREATE");

  h_eff->Write();
  h_pur->Write();

}


void plotResolution(TString suffix, int config, int target, int categ, int cut1, int cut2, 
      TString varX, TString var_titleX, TString varY, TString var_titleY, 
      int nbinsX, double* xbinsX, int nbinsY, double* xbinsY) {

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(1);
  gStyle->SetTitleAlign(23);
  TH1::AddDirectory(kFALSE);

  TFile f(TString::Format("%s/Target%i/config%i_Target%i%s.root",
        INPDIR.Data(),target, config, target, suffix.Data()));
  TTree* t = (TTree*)f.Get("default");


  for(int j=0; j<NbinsZ+1; j++){
    BinsZ[j] = -200+j*4;
  }

  for(int j=0; j<NbinsZang+1; j++){
    BinsZang[j] = -1+double(j)*2/NbinsZang;
    cout << BinsZang[j] << endl;
  }

  // for(int j=0; j<NbinsZ; j++){
  //   cout << BinsZ[j] << endl;
  // }

  TH3F* hMomRes    = new TH3F("momRes3D", "momRes3D", nbinsX, xbinsX, nbinsY, xbinsY,NbinsZ,BinsZ);
  TH2F* hMomRes2D  = new TH2F("hMomRes2D", "hMomRes2D", nbinsX, xbinsX, nbinsY, xbinsY);

  TH3F* hAngRes    = new TH3F("hAngRes", "hAngRes", nbinsX, xbinsX, nbinsY, xbinsY,NbinsZang,BinsZang);
  TH2F* hAngRes2D  = new TH2F("hAngRes2D", "hAngRes2D", nbinsX, xbinsX, nbinsY, xbinsY);


  TH2F* hSig   = new TH2F("h1", "h1", nbinsX, xbinsX, nbinsY, xbinsY);
  TH2F* hSelSig = new TH2F("h2", "h2", nbinsX, xbinsX, nbinsY, xbinsY);
  TH2F* hSel    = new TH2F("h3", "h3", nbinsX, xbinsX, nbinsY, xbinsY);
  TH1F *h_res_1d = new TH1F("h1", "h1", 100,-100,100);

  Int_t accum_level[8];
  Float_t variableX, variableY;
  Int_t reaction;
  Int_t nu_pdg;
  Int_t selmu_PDG;
  Int_t selmu_ID;
  Float_t selmu_truemom;
  Float_t selmu_mom;
  Int_t topology;
  Float_t selmu_truedir[3];
  int cntTimes=0;
  
  t->SetBranchAddress("accum_level", &accum_level);
  t->SetBranchAddress(varX,          &variableX);
  t->SetBranchAddress(varX,          &variableY);
  t->SetBranchAddress("topology",    &topology);

  t->SetBranchAddress("accum_level", &accum_level);
  t->SetBranchAddress("topology",    &topology);
  t->SetBranchAddress("reaction", &reaction);

  t->SetBranchAddress("selmu_PDG", &selmu_PDG); 
  t->SetBranchAddress("selmu_ID", &selmu_ID);
  t->SetBranchAddress("selmu_truemom", &selmu_truemom);
  t->SetBranchAddress("selmu_mom", &selmu_mom);
  t->SetBranchAddress("nu_pdg", &nu_pdg);
  t->SetBranchAddress("selmu_truedir", &selmu_truedir);

  vector<int> branches;
  if (categ == -2) {
    branches.push_back(3); 
    branches.push_back(4);
    branches.push_back(5);
  }
  else if (categ == -1){
    branches.push_back(3); 
    branches.push_back(4);
    branches.push_back(6);
  }
  else if (categ == -3){
    branches.push_back(3); 
    branches.push_back(4);
    branches.push_back(5);
    branches.push_back(6);
  }
  else if (categ == -4){
    branches.push_back(3); 
    branches.push_back(4);
  }
  else if (categ == -5){
    branches.push_back(5);
  }
  else if (categ == -6){
    branches.push_back(6);
  }
  else
    branches.push_back(categ);

  cout <<endl << t->GetEntries() << endl;
  for (Int_t ient=0; ient < t->GetEntries(); ient++) {
    t->GetEntry(ient);

    bool sel1 = false, sel2 = false, sel3 = false;
    for (vector<int>::iterator it = branches.begin(); it != branches.end(); ++it) {
      bool topoCut = ((*it) >= 3) || (topology==(*it));
      if(reaction>=0 && selmu_ID ==1){
      sel1 = sel1 || (accum_level[*it] > cut2 && topoCut);
      }
    }
    Double_t theta = atan(selmu_truedir[1]/selmu_truedir[2]);
    Double_t costheta;
    if(selmu_truedir[2] < 0) costheta = -cos(theta);
    else costheta = cos(theta);

    if (sel1){
    // cout << "true_costheta: " << costheta << endl;
    // cout << "selmu_costheta: " << variableY << endl << endl;
      h_res_1d->Fill(selmu_truemom-selmu_mom);
      hMomRes->Fill(costheta, selmu_truemom, selmu_truemom-selmu_mom);
      hAngRes->Fill(costheta, selmu_truemom, costheta-variableY);
      // cout <<"dif: " << costheta-variableY << endl;
      hSel->Fill(costheta, selmu_truemom, 1);
    }

  }
  f.Close();

  cout << "cntTimes: " << cntTimes << endl;

  TCanvas *c = new TCanvas("c", "c");

  TString suffixName = suffix;
  if (suffix == "")
    suffixName = "_nu";

  TString targetName="";
  if (config==0 || target<=2)
    targetName = TString::Format("FGD %i", target);
  else
    targetName = "SFGD";

  TString categName = "";
  if (categ == -1)
    categName = "SFGD_and_TPC";
  else if (categ == -2)
    categName = "TPC_and_HATPC";
  else if (categ == -3)
    categName = "SFGD_TPC_and_HATPC";
  else if (categ == -4)
    categName = "TPC";
  else if (categ == -5)
    categName = "HATPC";
  else if (categ == -6)
    categName = "SFGD";
  else
    categName = branchNames[categ];

  TString namehist = TString::Format("categ_%s_target_%s",categName.Data(),targetName.Data());
  TH2F *h_res = new TH2F(namehist, namehist, nbinsX, xbinsX, nbinsY, xbinsY);
  //h_res->Divide(hSig, hSel);

  h_res->SetTitle(TString::Format("%s, %s, %s", 
          config==0 ? "Current":"Upgrade",
          targetName.Data(),
          categName.Data()));

  h_res->GetXaxis()->SetTitle(var_titleX);
  h_res->GetXaxis()->SetTitleSize(0.05);
  h_res->GetXaxis()->SetTitleOffset(0.9);
  h_res->GetYaxis()->SetTitle(var_titleY);
  h_res->GetYaxis()->SetTitleSize(0.05);
  h_res->GetYaxis()->SetTitleOffset(0.9);
  
  h_res->Draw("colz");
  c->Update();
  c->SaveAs(TString::Format("%s/2D_resolution_config%i_target%i_total_%s%s.pdf", 
          OUTDIR.Data(), config, target, categName.Data(), suffixName.Data()));

  TCanvas *c2 = new TCanvas("c2", "c2");
  h_res_1d->Draw();
  c2->Update();
  c2->SaveAs(TString::Format("%s/1D_resolution_config%i_target%i_total_%s%s.pdf", 
          OUTDIR.Data(), config, target, categName.Data(), suffixName.Data()));

  TFile fW(TString::Format("%s/input_FSI_%s.root", 
          OUTDIR.Data(),namehist.Data()),"RECREATE");

  for(int aa=0; aa<nbinsX; aa++){
    for(int bb=0; bb<nbinsY; bb++){
    TH1D* AuxH = hMomRes->ProjectionZ("",aa+1,aa+1,bb+1,bb+1);
    TH1D* AuxHang = hAngRes->ProjectionZ("",aa+1,aa+1,bb+1,bb+1);
    cout << "a:" << aa << " b: " << bb << " RMS:" <<  AuxH->GetRMS() << endl;
    cout << "a:" << aa << " b: " << bb << " angRMS:" <<  AuxHang->GetRMS() << endl << endl;
    hMomRes2D->SetBinContent(aa+1,bb+1, AuxH->GetRMS());
    hAngRes2D->SetBinContent(aa+1,bb+1, AuxHang->GetRMS());
    }
  }

  cout << endl << hMomRes2D->GetEntries() << endl << endl;
  TCanvas *c3 = new TCanvas("c3", "c3");
  hMomRes2D->Draw("COLZ");
  c3->Update();
  c3->SaveAs(TString::Format("%s/2D_mom_resolution_%s.pdf", 
          OUTDIR.Data(), namehist.Data()));

  TCanvas *c4 = new TCanvas("c4", "c4");
  hAngRes2D->Draw("COLZ");
  c4->Update();
  c4->SaveAs(TString::Format("%s/2D_ang_resolution_%s.pdf", 
          OUTDIR.Data(), namehist.Data()));

  hMomRes->Write();
  hMomRes2D->Write();
  hAngRes->Write();
  hAngRes2D->Write();
  h_res_1d->Write();

}



// superimpose the efficiencies for a given set of files, category and variable
void plotTotal(TString suffix, int categ, int cut1, int cut2, TString var, TString var_title, int nbins, double* xbins, bool PSRes=false){
  

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  vector<TH1F*> histos;
  
  TCanvas *c = new TCanvas("c");
  TH1F* h_dummy = c->DrawFrame(xbins[0], 0, xbins[nbins], 1.02);
  h_dummy->GetXaxis()->SetTitle(var_title);
  h_dummy->GetXaxis()->SetTitleSize(0.05);
  h_dummy->GetXaxis()->SetTitleOffset(0.9);
  h_dummy->GetYaxis()->SetTitleSize(0.05);
  h_dummy->GetYaxis()->SetTitle("efficiency");

  TLegend *leg = new TLegend(0.1, 0.905, 0.9, 0.98);
  leg->SetBorderSize(0);
  leg->SetNColumns(3);

  int configs[2] = {0,2};
  int linestyles[2] = {2,1};

  int colors[3] = {kBlack, kRed, kBlue};
      
  for (int it=1; it<=3; it++) {
      TH1F* h = computeEff(suffix, 2, it, categ, cut1, cut2, 
               var, var_title, nbins, xbins, PSRes);
      h->SetLineColor(colors[it-1]);
      h->SetLineWidth(3);
      h->Draw("esame");

    TString targetName="";
      if (it<=2)
    targetName = TString::Format("FGD %i", it);
      else
    targetName = "SuperFGD";
      leg->AddEntry(h, TString::Format("%s", targetName.Data()), "el");
      histos.push_back(h);
    }

  leg->Draw("same");

  c->Update();
  cout << endl << "Writing: " << TString::Format("%s/resolution_%s.root", OUTDIR.Data(), var.Data()) << endl;

  c->SaveAs(TString::Format("%s/resolution_%s.pdf", OUTDIR.Data(), var.Data()));
  TFile fW(TString::Format("%s/resolution_%s.root", OUTDIR.Data(), var.Data()), "RECREATE");

  
  for(int w=0; w<histos.size(); w++){
  histos[w]->Write();
  }

}


void muons_resolution() {

  TString suffixes[1] = {"_FHC_numu"};
  int conf[2] = {0,2};

    // for (int s=0; s<1; s++){
    //   int c = -2;
    //   plotTotal(suffixes[s], c, -1, 4, "true_costheta", "true cos #theta", NBins_CosTh, BinEdges_CosTh);
    //   plotTotal(suffixes[s], c, -1, 4, "true_mom", "true p_{#mu} [MeV/c]", NBins_Mom, BinEdges_Mom);
    //   plotTotal(suffixes[s], c, -1, 4, "true_Q2", "true Q^{2} [GeV^{2}/c^{2}]", NBins_Q2, BinEdges_Q2);
    // }

    // plot2D_Default("_FHC_numu", 2, 1, -2, -1, 4, 
    // "selmu_costheta", "true cos #theta", "selmu_truemom", "true p_{#mu} [MeV/c]", 
    // NBins_CosTh, BinEdges_CosTh, NBins_Mom, BinEdges_Mom);

    // plot2D_Default("_FHC_numu", 2, 2, -2, -1, 4, 
    // "selmu_costheta", "true cos #theta", "selmu_truemom", "true p_{#mu} [MeV/c]", 
    // NBins_CosTh, BinEdges_CosTh, NBins_Mom, BinEdges_Mom);

    // plotResolution("_FHC_numu", 2, 3, -6, -1, 4, 
    // "selmu_costheta", "true cos #theta", "selmu_truemom", "true p_{#mu} [MeV/c]", 
    // NBins_CosTh, BinEdges_CosTh, NBins_Mom, BinEdges_Mom);

    plotResolution("_FHC_numu", 2, 3, -5, -1, 4, 
    "selmu_costheta", "true cos #theta", "selmu_truemom", "true p_{#mu} [MeV/c]", 
    NBins_CosTh, BinEdges_CosTh, NBins_Mom, BinEdges_Mom);
    
    plotResolution("_FHC_numu", 2, 3, -4, -1, 4, 
    "selmu_costheta", "true cos #theta", "selmu_truemom", "true p_{#mu} [MeV/c]", 
    NBins_CosTh, BinEdges_CosTh, NBins_Mom, BinEdges_Mom);
    
    // plotResolution("_FHC_numu", 2, 3, -3, -1, 4, 
    // "selmu_costheta", "true cos #theta", "selmu_truemom", "true p_{#mu} [MeV/c]", 
    // NBins_CosTh, BinEdges_CosTh, NBins_Mom, BinEdges_Mom);
    
    plotResolution("_FHC_numu", 2, 3, -2, -1, 4, 
    "selmu_costheta", "true cos #theta", "selmu_truemom", "true p_{#mu} [MeV/c]", 
    NBins_CosTh, BinEdges_CosTh, NBins_Mom, BinEdges_Mom);
    
    // plotResolution("_FHC_numu", 2, 3, -1, -1, 4, 
    // "selmu_costheta", "true cos #theta", "selmu_truemom", "true p_{#mu} [MeV/c]", 
    // NBins_CosTh, BinEdges_CosTh, NBins_Mom, BinEdges_Mom);

    // plot2D_Default("_FHC_numu", 2, 3, -1, -1, 4, 
    // "selmu_costheta", "true cos #theta", "selmu_truemom", "true p_{#mu} [MeV/c]", 
    // NBins_CosTh, BinEdges_CosTh, NBins_Mom, BinEdges_Mom);

    // plot2D_Truth("_FHC_numu", 2, 1, -2, -1, 4, 
    // "true_costheta", "true cos #theta", "true_mom", "true p_{#mu} [MeV/c]", 
    // NBins_CosTh, BinEdges_CosTh, NBins_Mom, BinEdges_Mom);

    // plot2D_Truth("_FHC_numu", 2, 2, -2, -1, 4, 
    // "true_costheta", "true cos #theta", "true_mom", "true p_{#mu} [MeV/c]", 
    // NBins_CosTh, BinEdges_CosTh, NBins_Mom, BinEdges_Mom);

    // plot2D_Truth("_FHC_numu", 2, 3, -2, -1, 4, 
    // "true_costheta", "true cos #theta", "true_mom", "true p_{#mu} [MeV/c]", 
    // NBins_CosTh, BinEdges_CosTh, NBins_Mom, BinEdges_Mom);

    // plot2D_Truth("_FHC_numu", 2, 3, -1, -1, 4, 
    // "true_costheta", "true cos #theta", "true_mom", "true p_{#mu} [MeV/c]", 
    // NBins_CosTh, BinEdges_CosTh, NBins_Mom, BinEdges_Mom);

}

void SimpleDraw(){
  
TFile f("/nfs/neutrinos/cjesus/work/jobs/files/TDR_eff/Target1/config2_Target1_FHC_numu.root");
TTree *t = (TTree*)f.Get("default");
t->Draw("selmu_mom", "accum_level[][0]>4");

}
