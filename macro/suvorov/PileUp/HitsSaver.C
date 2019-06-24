void HitsSaver(Int_t first_entry, Int_t Nentries, Int_t run, TString  out_file_name, TString input_file) {
	TString prefix = "/t2k/users/suvorov/AnalysisResults/ndUP/PileUp/";
	TFile* file = new TFile(input_file.Data(), "READ");

	TTree* tree = (TTree*)file->Get("ND280upEvents");

  TND280UpEvent *nd280UpEvent = new TND280UpEvent();
  tree->SetBranchAddress("Event",&nd280UpEvent);

	TH2F* h2d_xy; TH2F* h2d_xz; TH2F* h2d_yz;
  h2d_xy = (TH2F*)file->Get("OutMPPCProj2D_XY");
  h2d_xz = (TH2F*)file->Get("OutMPPCProj2D_XZ");
  h2d_yz = (TH2F*)file->Get("OutMPPCProj2D_YZ");

  Int_t binX = h2d_xy->GetXaxis()->GetNbins();
  Int_t binY = h2d_xy->GetYaxis()->GetNbins();
  Int_t binZ = h2d_xz->GetYaxis()->GetNbins();

  Float_t X_min = h2d_xy->GetXaxis()->GetBinLowEdge(0);
  Float_t X_max = h2d_xy->GetXaxis()->GetBinUpEdge(binX);
  Float_t Y_min = h2d_xy->GetYaxis()->GetBinLowEdge(0);
  Float_t Y_max = h2d_xy->GetYaxis()->GetBinUpEdge(binX);
  Float_t Z_min = h2d_xz->GetYaxis()->GetBinLowEdge(0);
  Float_t Z_max = h2d_xz->GetYaxis()->GetBinUpEdge(binX);

  out_file_name = prefix + "/" + out_file_name + "_";
  out_file_name += run;
  out_file_name += ".root";
  TFile* out = new TFile(out_file_name.Data(), "RECREATE");

  TH3C* h3d = new TH3C("3d", "", binX, X_min, X_max, binY, Y_min, Y_max, binZ, Z_min, Z_max);
  TTree* out_tree = new TTree("hits_tree", "");
  out_tree->Branch("histo", &h3d);

  TH3C* h3d_ar[8];
  for (Int_t i = 0; i < 8; ++i)
    h3d_ar[i] = new TH3C(Form("3d%i", i), "", binX, X_min, X_max, binY, Y_min, Y_max, binZ, Z_min, Z_max);

  TH1D* bunch_histo = new TH1D("BH", "", 8, -230.8, 4425.2);

  int NTreeEntries = tree->GetEntries();
  int evtlasttree = first_entry + Nentries;

  if (evtlasttree > NTreeEntries-1) {
    cout << "Oveflow" << endl;
    evtlasttree = NTreeEntries-1;
  }

  cout << "[          ] Nev = " << first_entry << "...." << evtlasttree << " From " << NTreeEntries <<"\r[";
  for(int ievt = first_entry; ievt < evtlasttree; ++ievt) {
  	if (ievt % ((evtlasttree-first_entry)/10) == 0) cout << "." << flush;
  	tree->GetEntry(ievt);
  	h3d->Reset();
    for (Int_t i = 0; i < 8; ++i)
      h3d_ar[i]->Reset();
  	Int_t track_OI = -1;

  	/*for (int itrack = 0; itrack < nd280UpEvent->GetNTracks(); ++itrack) {
      TND280UpTrack* track = nd280UpEvent->GetTrack(itrack);

      if (track->GetPDG() == 13)
      	track_OI = track->GetTrackID();
    }*/

  	for(int ihit = 0; ihit < nd280UpEvent->GetNHits(); ++ihit) { // get last entry
      TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);

      /*bool muon_hit = false;
      // store only muon hits
      for (UInt_t i = 0; i < nd280UpHit->fContributors.size(); ++i) {
        if (nd280UpHit->fContributors[i] == track_OI)
          muon_hit = true;
      }

      if (!muon_hit)
      	continue;*/

      double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
      double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2.; // middle step Y 
      double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2.; // middle step Z
      double posT = (nd280UpHit->GetStartT() + nd280UpHit->GetStopT())/2.; // middle step T

      Int_t bunch = bunch_histo->FindBin(posT) - 1;
      if (bunch < 0 || bunch > 7) continue;

      Int_t bin = h3d->FindBin(posX, posY, posZ);
      if (!h3d_ar[bunch]->GetBinContent(bin)) {
        h3d_ar[bunch]->Fill(posX, posY, posZ);
      }
    } // loop over hits

    for (Int_t i = 0; i < 8; ++i) {
      if (h3d_ar[i]->Integral()) {
        h3d->Reset();
        h3d->Add(h3d_ar[i]);
    	 out_tree->Fill();
      }
    }
  } // loop over events
  cout << "]" << endl;
  out_tree->Write("", TObject::kOverwrite);
  out->Close();
}