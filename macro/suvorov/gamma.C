void gamma(Int_t first_entry, Int_t Nentries, Int_t run, TString  out_file_name, TString input_file) {
  TString input_file_path = input_file;
  TFile* file = new TFile(input_file_path.Data(), "READ");

  TString prefix = "/t2k/users/suvorov/figure/ndUP/gamma/";

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

  TH3F* h3d = new TH3F("3d", "", binX, X_min, X_max, binY, Y_min, Y_max, binZ, Z_min, Z_max);
  TH3F* h3d1 = new TH3F("3d1", "", binX, X_min, X_max, binY, Y_min, Y_max, binZ, Z_min, Z_max);

  int NTreeEntries = tree->GetEntries();
  int evtlasttree = first_entry + Nentries;

  if (evtlasttree > NTreeEntries-1) {
    cout << "Oveflow" << endl;
    evtlasttree = NTreeEntries-1;
  }

  Int_t DEBUG = 0;

  TH1D* gamma_ini     = new TH1D("gamma_ini", "", 200, 0.9, 1);
  TH1D* gamma_ini_e     = new TH1D("gamma_ini_e", "", 200, 0., 800.);
  TH1D* gamma_enter     = new TH1D("gamma_enter", "", 200, 0.99, 1);
  TH1D* gamma_enter_e     = new TH1D("gamma_enter_e", "", 200, 0., 800.);

  TVector3 det_plane_pos(0., 0., 8760.);
  TVector3 det_plane_dif(120., 40., 0.);


  cout << "*************************************************************************************************" << endl;
  cout << "Run        " << run << endl;
  cout << "First      " << first_entry << endl;
  cout << "Nentries   " << Nentries << endl;

  cout << "[          ] Nev = " << first_entry << "...." << evtlasttree << " From " << NTreeEntries <<"\r[";
  Int_t threshold = -1;
  for(int ievt = first_entry; ievt < evtlasttree; ++ievt) {

    
    if ((ievt - first_entry) * 10 / (evtlasttree-first_entry) > threshold) {
      std::cout << "." << std::flush;
      ++threshold;
    }

    tree->GetEntry(ievt);

    for (int itrack = 0; itrack < nd280UpEvent->GetNTracks(); ++itrack) {
      TND280UpTrack* track = nd280UpEvent->GetTrack(itrack);

      if (track->GetPDG() == 22) {
        TND280UpTrackPoint* point_first = track->GetPoint(0);
        TND280UpTrackPoint* point_last  = track->GetPoint(track->GetNPoints() - 1);

        TVector3 pos_ini = point_first->GetPrePosition();
        TVector3 dir_ini = track->GetInitMom().Unit();
        if (pos_ini.Z() > 7.18)
          continue;

        // if exit degrader
        if (point_last->GetPostPosition().Z() > 8) {
          gamma_ini->Fill(track->GetInitCosTheta());
          gamma_ini_e->Fill(track->GetInitMom().Mag());
        }

        // if enter target
        Double_t dist = (det_plane_pos.Z() - pos_ini.Z()) / dir_ini.Z();
        Double_t x = (pos_ini + dir_ini * dist).X();
        Double_t y = (pos_ini + dir_ini * dist).X();

        if (abs(x) > 120. || abs(y) > 40.) 
          continue;
            // enter target
        gamma_enter->Fill(track->GetInitCosTheta());
        gamma_enter_e->Fill(track->GetInitMom().Mag());

      }
    } // loop over tracks
    h3d->Reset();
  } // loop over events
  cout << "]" << endl;

  out_file_name = prefix + "/" + out_file_name + "_";
  out_file_name += run;
  out_file_name += ".root";
  cout << "Writing to file " << out_file_name << endl;
  TFile* out = new TFile(out_file_name.Data(), "RECREATE");
  gamma_ini->Write();
  gamma_ini_e->Write();
  gamma_enter->Write();
  gamma_enter_e->Write();
  out->Close();
}