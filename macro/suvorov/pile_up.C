void pile_up(Int_t first_entry, Int_t Nentries, Int_t run) {
	TString input_file = "/t2k/users/suvorov/AnalysisResults/ndUP/PileUp//g4_4w_v2-Exp744000-Nexpt736.root";
  TFile* file = new TFile(input_file.Data(), "READ");

  TString prefix = "/t2k/users/suvorov/figure/ndUP/PileUp/";

  TTree* tree = (TTree*)file->Get("ND280upEvents");
  TCanvas c1("canva","",50,50,1000,800);

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

  double BCT[8] = {2750.2, 333.2, 3914.7, 4497., 5078.4, 5659.7, 6243.4, 6824.2}; 

  TH1D* cubes_pileup = new TH1D("cubes_pileup", "", 100, 0., 100.);

  TH3F* h3d = new TH3F("3d", "", binX, X_min, X_max, binY, Y_min, Y_max, binZ, Z_min, Z_max);

  int NTreeEntries = tree->GetEntries();
  int evtlasttree = first_entry + Nentries;

  if (evtlasttree > NTreeEntries-1) {
    cout << "Oveflow" << endl;
    evtlasttree = NTreeEntries-1;
  }

  cout << "[          ] Nev = " << first_entry << "...." << evtlasttree << " From " << NTreeEntries <<"\r[";
  Int_t threshold = -1;
  for(int ievt = first_entry; ievt < evtlasttree; ++ievt) {

  	
  	if ((ievt - first_entry) * 10 / (evtlasttree-first_entry) > threshold) {
      std::cout << "." << std::flush;
      ++threshold;
  	}

    tree->GetEntry(ievt);

    TRandom3* rand = new TRandom3();
    double muon_time = rand->Gaus(0, 18.);

    for(int ihit = 0; ihit < nd280UpEvent->GetNHits(); ++ihit) { // get last entry
      TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);

      double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
      double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2.; // middle step Y 
      double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2.; // middle step Z
      double posT = (nd280UpHit->GetStartT() + nd280UpHit->GetStopT())/2.; // middle step T

      bool pile_up = false;

      for (Int_t bunch = 0; bunch < 8; ++bunch) {
        if (abs(posT - BCT[bunch] - muon_time) < 50) {
          pile_up = true;
          break;
        }
      }

      if (!pile_up)
        continue;

      Int_t bin = h3d->FindBin(posX, posY, posZ);
      if (!h3d->GetBinContent(bin)) {
        h3d->Fill(posX, posY, posZ);
      }
    } // loop over hit
    cubes_pileup->Fill(h3d->Integral());

    h3d->Reset();
  } // loop over events
  cout << "]" << endl;

  cubes_pileup->SetBinContent(cubes_pileup->GetNbinsX(), cubes_pileup->GetBinContent(cubes_pileup->GetNbinsX()) + cubes_pileup->GetBinContent(cubes_pileup->GetNbinsX() + 1));

  TString out_file_name = prefix + "out_4w_time_";
  out_file_name += run;
  out_file_name += ".root";
  TFile* out = new TFile(out_file_name.Data(), "RECREATE");
  cubes_pileup->Write();
  out->Close();

  exit();
}