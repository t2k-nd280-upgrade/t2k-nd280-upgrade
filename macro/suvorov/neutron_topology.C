#include "TFile.h"
#include "TTree.h"
#include "TH2F.h"
#include "TF1.h"
#include "TStyle.h"
#include "TPad.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TRandom3.h"
//#include "TND280UpEvent.hh"

#include <iostream>
#include <sstream>

#include "TChain.h"
#include "TCanvas.h"

using namespace std;;

void neutron_topology() {
	TString input_file_path_prefix = "/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/neutron_v11-Exp";
  //TFile* file = new TFile(input_file_path.Data(), "READ");
  TString file_name;
  TChain* chain = new TChain("ND280upEvents");
  for (Int_t i = 0; i < 1000; ++i) {
  	file_name = Form("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/neutron_v11-Exp%i-Nexpt1000.root", 1000*i);
  	//cout << file_name << endl;
  	chain->Add(file_name);
  }

  TCanvas c1("canva","",50,50,1000,800);

  TFile* out_file = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/neutron/top.root", "RECREATE");

  TString prefix = "/t2k/users/suvorov/figure/ndUP/Michel/";

  //TTree* tree = (TTree*)chain->Get("ND280upEvents");

  TND280UpEvent *nd280UpEvent = new TND280UpEvent();
  chain->SetBranchAddress("Event",&nd280UpEvent);

  TH1F* nCelastic = new TH1F("ncElastic", "", 100, 0., 500);
  TH1F* npElastic = new TH1F("npElastic", "", 100, 0., 500);
  TH1F* nInElastic = new TH1F("nInElastic", "", 100, 0., 500);

  npElastic->SetLineColor(kRed);
  nInElastic->SetLineColor(kGreen);

  Int_t entr = chain->GetEntries();
  for(int ievt = 0; ievt < entr; ++ievt) {

  	if (ievt%10000 == 0) cout << ".";

    chain->GetEntry(ievt);

    vector<pair<int, float > > neutrons;
    vector<vector <int> > daughter_ID;

    for (int itrack = 0; itrack < nd280UpEvent->GetNTracks(); ++itrack) {
      TND280UpTrack* track = nd280UpEvent->GetTrack(itrack);
      if (track->GetPDG() == 2112) {
      	if (track->GetPoint(track->GetNPoints() - 2)) {
      		float mom = track->GetPoint(track->GetNPoints() - 2)->GetMomentum().Mag();
      		float e_kin = sqrt(mom*mom + 939.5654*939.5654) - 939.5654;
      		neutrons.push_back(make_pair(track->GetTrackID(), e_kin));
      	}
      }
    }

    for (UInt_t i = 0; i < neutrons.size(); ++i) {
    	vector<int> daughter;
    	for (int itrack = 0; itrack < nd280UpEvent->GetNTracks(); ++itrack) {
      	TND280UpTrack* track = nd280UpEvent->GetTrack(itrack);
      	if (track->GetParentID() == neutrons[i].first)
      		daughter.push_back(itrack);
      }
      daughter_ID.push_back(daughter);
    }

    if (neutrons.size() == 0)
    	continue;
    //cout << "size " << neutrons.size() << "   " << daughter_ID.size()  << "    " << daughter_ID[0].size() << endl;

    for (UInt_t i = 0; i < neutrons.size(); ++i) {
    	if (daughter_ID[i].size() == 0)
    		continue;
    	// TODO study far all daughters
    	//cout << ievt << "   " << i << "    " << daughter_ID[i][0] << endl;
    	
    	int Li = 0;
    	int Be8 = 0; // 1000040080
    	int alpha = 0; // 1000020040
    	int neutron = 0; // 2112
    	int B = 0;
    	bool Inelastic = false;

    	for (UInt_t itrackID = 0; itrackID < daughter_ID[i].size(); ++itrackID) {
				// get daughter    	
    		TND280UpTrack* track = nd280UpEvent->GetTrack(daughter_ID[i][itrackID]);

    		// elastic
    		if (track->GetPDG() == 1000060120 && track->GetProcessName() == "hadElastic") {
    			nCelastic->Fill(neutrons[i].second);
    		} else if (track->GetPDG() == 2212 && track->GetProcessName() == "hadElastic") {
    			npElastic->Fill(neutrons[i].second);
    		} 

    		// inelastic
    		if (track->GetProcessName() == "neutronInelastic")
    			Inelastic = true;
      	if (track->GetPDG() == 1000040080)
      		++Be8;
      	else if (track->GetPDG() == 1000020040)
      		++alpha;
      	else if (track->GetPDG() == 2112)
      		++neutron;
      }

    	if (Inelastic)
    		nInElastic->Fill(neutrons[i].second);

    }
  }

  cout << endl;

  out_file->cd();
  nCelastic->Write();
  npElastic->Write();
  nInElastic->Write();

	c1.SetLogy();
	c1.SetLogx();
	nCelastic->Draw();
	npElastic->Draw("same");
	nInElastic->Draw("same");
	c1.Write();

  out_file->Close();
}