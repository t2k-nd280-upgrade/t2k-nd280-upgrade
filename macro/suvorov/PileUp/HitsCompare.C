#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH3C.h"
#include "TH2C.h"
#include "TRandom3.h"

using namespace std;

void HitsCompare(){
  //TString file_name1 = "/t2k/users/suvorov/AnalysisResults/ndUP/PileUp/anumu_1e21_hits.root";
  TString file_name1 = "/t2k/users/suvorov/AnalysisResults/ndUP/PileUp/numu_1e21_hits.root";
  TString file_name2 = "/t2k/users/suvorov/AnalysisResults/ndUP/PileUp/neutron_4w_hits.root";
  //TString file_name2 = "/t2k/users/suvorov/AnalysisResults/ndUP/PileUp/sand_muon_hits.root";

  TFile* file1 = new TFile(file_name1.Data(), "READ");
  TTree* tree1 = (TTree*)file1->Get("hits_tree");
  TH3C* h3d1;

  tree1->SetBranchAddress("histo",  &h3d1);

  TFile* file2 = new TFile(file_name2.Data(), "READ");
  TTree* tree2 = (TTree*)file2->Get("hits_tree");
  TH3C* h3d2;

  tree2->SetBranchAddress("histo",  &h3d2);

  tree1->GetEntry(0);
  tree2->GetEntry(0);

  Int_t binX;
  if (h3d1->GetXaxis()->GetNbins() > h3d2->GetXaxis()->GetNbins())
    binX = h3d2->GetXaxis()->GetNbins();
  else binX = h3d1->GetXaxis()->GetNbins();
  Int_t binY;
  if (h3d1->GetYaxis()->GetNbins() > h3d2->GetYaxis()->GetNbins())
    binY = h3d2->GetYaxis()->GetNbins();
  else binY = h3d1->GetYaxis()->GetNbins();
  Int_t binZ;
  if (h3d1->GetYaxis()->GetNbins() > h3d2->GetYaxis()->GetNbins())
    binZ = h3d2->GetYaxis()->GetNbins();
  else binZ = h3d1->GetYaxis()->GetNbins();

  TH2C* hits_XY_1 = new TH2C("proj_XY_1","", binX, 0., binX, binY, 0., binY);
  TH2C* hits_XZ_1 = new TH2C("proj_XZ_1","", binX, 0., binX, binZ, 0., binZ);
  TH2C* hits_YZ_1 = new TH2C("proj_YZ_1","", binY, 0., binY, binZ, 0., binZ);

  TH2C* hits_XY_2 = new TH2C("proj_XY_2","", binX, 0., binX, binY, 0., binY);
  TH2C* hits_XZ_2 = new TH2C("proj_XZ_2","", binX, 0., binX, binZ, 0., binZ);
  TH2C* hits_YZ_2 = new TH2C("proj_YZ_2","", binY, 0., binY, binZ, 0., binZ);

  TH2C* hits_XY_test = new TH2C("proj_XY_test","", binX, 0., binX, binY, 0., binY);
  TH2C* hits_XZ_test = new TH2C("proj_XZ_test","", binX, 0., binX, binZ, 0., binZ);
  TH2C* hits_YZ_test = new TH2C("proj_YZ_test","", binY, 0., binY, binZ, 0., binZ);

  Int_t pileuped_cubes = 0;
  Int_t pileuped_events = 0;
  Int_t pileuped_ch1_XY = 0;
  Int_t pileuped_ch1_XZ = 0;
  Int_t pileuped_ch1_YZ = 0;

  Int_t Nevents = tree1->GetEntries();
  Int_t Npileup = tree2->GetEntries();

  // in order to avoid reusing of the same events
  // create a vector which will be erasing during the run
  vector<int> Npileup_v;
  for (Int_t i = 0; i < Npileup; ++i)
    Npileup_v.push_back(i);

  TRandom3* gen = new TRandom3();

  Int_t first = 0;
  Int_t last  = Nevents;

  cout << "[                                                  ]  first " << first << "   last " << last << "  Evt = " << Nevents << "\r[";

  for (Int_t ievt = first; ievt < last; ++ievt) {
    if (ievt % (Nevents / 50) == 0) cout << "." << flush;
    tree1->GetEntry(ievt);
    Int_t ipileup = (int)gen->Uniform(Npileup_v.size());
    tree2->GetEntry(Npileup_v[ipileup]);
    Npileup_v.erase(Npileup_v.begin()+ipileup);

    bool pileup = false;

    bool pileuped_ch1_XY_b = false;
    bool pileuped_ch1_XZ_b = false;
    bool pileuped_ch1_YZ_b = false;

    hits_XY_1->Reset();  hits_XY_2->Reset();
    hits_XZ_1->Reset();  hits_XZ_2->Reset();
    hits_YZ_1->Reset();  hits_YZ_2->Reset();

    for (Int_t z = 1; z <= binZ; ++z) {
      for (Int_t y = 1; y <= binY; ++y) {
        for (Int_t x = 1; x <= binX; ++x) {

          if (h3d1->GetBinContent(x, y, z)) {
            if (!hits_XY_1->GetBinContent(x, y))
              hits_XY_1->SetBinContent(x, y, 1);
            if (!hits_XZ_1->GetBinContent(x, z))
              hits_XZ_1->SetBinContent(x, z, 1);
            if (!hits_YZ_1->GetBinContent(y, z))
              hits_YZ_1->SetBinContent(y, z, 1);
          }

          if (h3d2->GetBinContent(x, y, z)) {
            if (!hits_XY_2->GetBinContent(x, y))
              hits_XY_2->SetBinContent(x, y, 1);
            if (!hits_XZ_2->GetBinContent(x, z))
              hits_XZ_2->SetBinContent(x, z, 1);
            if (!hits_YZ_2->GetBinContent(y, z))
              hits_YZ_2->SetBinContent(y, z, 1);
          }

          if (h3d1->GetBinContent(x, y, z) * h3d2->GetBinContent(x, y, z)) {
            pileup = true;
            ++pileuped_cubes;
            //cout << "PileUp at " << x << "   " << y << "   " << z << endl;
            //break;
          }
        }
        //if (pileup) break;
      }
      //if (pileup) break;
    } // z
    if (pileup) 
      ++pileuped_events;

    hits_XY_test->Multiply(hits_XY_1, hits_XY_2);
    hits_XZ_test->Multiply(hits_XZ_1, hits_XZ_2);
    hits_YZ_test->Multiply(hits_YZ_1, hits_YZ_2);

    if (hits_XY_test->Integral() && !pileuped_ch1_XY_b){
      ++pileuped_ch1_XY;
      pileuped_ch1_XY_b = true;
    }
    if (hits_XZ_test->Integral() && !pileuped_ch1_XZ_b){
      ++pileuped_ch1_XZ;
      pileuped_ch1_XZ_b = true;
    }
    if (hits_YZ_test->Integral() && !pileuped_ch1_YZ_b){
      ++pileuped_ch1_YZ;
      pileuped_ch1_YZ_b = true;
    }

    //if (pileuped_ch1_YZ_b + pileuped_ch1_XZ_b + pileuped_ch1_XY_b) {
     // cout << "PileUp proj  at " << pileuped_ch1_XY << "   " << pileuped_ch1_XZ << "   " << pileuped_ch1_YZ << endl;
   //}
  } // loop over events

  cout << "]" << endl;
  cout << "Pileuped events:\t" << pileuped_events << endl;
  cout << "Ratio:\t" << 100. * pileuped_events/Nevents  << "%" << endl;
  cout << endl;
  cout << "Pileup for XY \t" << pileuped_ch1_XY << "\t ratio:\t" << 100. * pileuped_ch1_XY / Nevents << "%" << endl;
  cout << "Pileup for XZ \t" << pileuped_ch1_XZ << "\t ratio:\t" << 100. * pileuped_ch1_XZ / Nevents << "%" << endl;
  cout << "Pileup for YZ \t" << pileuped_ch1_YZ << "\t ratio:\t" << 100. * pileuped_ch1_YZ / Nevents << "%" << endl;
}