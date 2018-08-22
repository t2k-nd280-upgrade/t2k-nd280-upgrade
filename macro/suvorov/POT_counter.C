#include "TFile.h"
#include "TTree.h"
#include <iostream>

void POT_counter() {
	TFile* file = new TFile("/t2k/users/suvorov/AnalysisResults/ndUP/PileUp/gRooTracher_muon_3sand_vv.root", "READ");
	TTree* tree = (TTree*)file->Get("header");

	Float_t POT_per_file = 0;
	tree->SetBranchAddress("POT_per_file",		&POT_per_file);

	Float_t Total_POT = 0;
	Int_t Nentries = tree->GetEntries();
	for (Int_t i = 0; i < Nentries; ++i) {
		tree->GetEntry(i);
		Total_POT += POT_per_file;
	}

	std::cout << Total_POT << std::endl;
}