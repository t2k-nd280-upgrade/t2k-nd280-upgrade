void MIEle(Int_t first_entry, Int_t Nentries, Int_t run) {
  TString input_file = "$bars/AnalysisResults/ndUP/SuperFGD/SFGD/sfgd_mu_300-25000.root";
  TFile* file = new TFile(input_file.Data(), "READ");

  TString prefix = "/t2k/users/suvorov/figure/ndUP/Michel/";

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

  TH1D* Range_hi  = new TH1D("range", "", 100, 0., 300.);
  TH1D* Mom_hi    = new TH1D("mom",   "", 50, 0., 100.);
  TH1D* T_MiEle   = new TH1D("t_MiEle",   "", 80, 0., 8000.);
  TH1D* T_delta   = new TH1D("t_delta",   "", 80, 0., 8000.);
  TH1D* Cubes_ele = new TH1D("cubes_ele", "", 50, 0., 50.);
  TH1D* Cubes_ele_only  = new TH1D("cubes_ele_only", "", 50, 0., 50.);
  TH2D* time_distance   = new TH2D("time_distance", "", 800, 0., 8000.,  50, 0., 50.);

  cout << "*************************************************************************************************" << endl;
  cout << "Run        " << run << endl;
  cout << "First      " << first_entry << endl;
  cout << "Nentries   " << Nentries << endl;

  cout << "[          ] Nev = " << first_entry << "...." << evtlasttree << " From " << NTreeEntries <<"\r[";
  for(int ievt = first_entry; ievt < evtlasttree; ++ievt) {

    if ((ievt - first_entry) * 10 % (evtlasttree - first_entry) == 0)
      std::cout << "." << std::flush;
    tree->GetEntry(ievt);

    if (DEBUG >= 1) {
      cout << endl;
      cout << "//////////////////////" << endl;
      cout << "Event: " << ievt << endl;
    }

    Double_t max_ele_mom    = -1.;
    Double_t max_ele_range  = -1.;
    Double_t t_mu_end       = -1.;
    Double_t t_mu_decay     = -1.; 
    int EleID               = -1;

    for (int itrack = 0; itrack < nd280UpEvent->GetNTracks(); ++itrack) {
      TND280UpTrack* track = nd280UpEvent->GetTrack(itrack);

      if (track->GetTrackID() == 1)
        t_mu_end = track->GetPoint(track->GetNPoints() - 1)->GetTime();

      if (track->GetParentID() != 1)
        continue;

      int PDG = track->GetPDG();
      string process = track->GetProcessName();

      if (process != "Decay" && process !="muMinusCaptureAtRest")
        continue;

      if (PDG != 11)
        continue;

      double mom    = track->GetInitMom().Mag();
      double range  = track->GetRange();
      int Npoints   = track->GetNPoints();

      if (mom > max_ele_mom) {
        max_ele_mom   = mom;
        max_ele_range = range;
        t_mu_decay    = track->GetPoint(0)->GetTime();
        EleID = track->GetTrackID();
      }    

      if (DEBUG >= 2) {
        cout << endl;
        cout << "Track:   " << itrack << endl;
        cout << "Id:      " << track->GetTrackID() << endl;
        cout << "Parent:  " << track->GetParentID() << endl;
        cout << "PDG:     " << track->GetPDG() << endl;
        cout << "Ekin:    " << track->GetInitKinEnergy() << endl;
        cout << "Range:   " << track->GetRange() << endl;
        cout << "Process  " << track->GetProcessName() << endl;
        cout << "Mom:     " << track->GetInitMom().Mag() << endl;
      }

      if (DEBUG >= 3 && track->GetPoint(0) && track->GetPoint(Npoints-1)) {
        cout << "Start:   " << track->GetPoint(0)->GetPrePosition().X() << "      " << track->GetPoint(0)->GetPrePosition().Y() << "      " << track->GetPoint(0)->GetPrePosition().Z()  << "      " << track->GetPoint(0)->GetTime() << endl;
         cout << "End:     " << track->GetPoint(Npoints - 1)->GetPostPosition().X() << "      " << track->GetPoint(Npoints - 1)->GetPostPosition().Y() << "      " << track->GetPoint(Npoints - 1)->GetPostPosition().Z()  << "      " << track->GetPoint(Npoints - 1)->GetTime() << endl;
      }
    } // loop over track

    int NcubesEle = 0;
    int NEleOnlyCubes = 0;

    for(int ihit = 0; ihit < nd280UpEvent->GetNHits(); ++ihit) { // get last entry
      TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);

      double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
      double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2.; // middle step Y 
      double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2.; // middle step Z

      bool found = false;
      for (UInt_t i = 0; i < nd280UpHit->fContributors.size(); ++i) {
        if (nd280UpHit->fContributors[i] == EleID) {
          found = true;
          break;
        }
      }

      if (!found)
        continue;

      Int_t bin = h3d->FindBin(posX, posY, posZ);
      if (!h3d->GetBinContent(bin)) {
        if (nd280UpHit->fContributors.front() == EleID)
          ++NEleOnlyCubes;
        h3d->Fill(posX, posY, posZ);
      }

      
    } // loop over hit
    

    

    if (max_ele_mom > 0.) {
      Range_hi->Fill(max_ele_range);
      Mom_hi->Fill(max_ele_mom);
      T_MiEle->Fill(t_mu_decay);
      T_delta->Fill(t_mu_decay - t_mu_end);
      Cubes_ele->Fill(h3d->Integral());
      Cubes_ele_only->Fill(NEleOnlyCubes);
      time_distance->Fill(t_mu_decay - t_mu_end, h3d->Integral());
    }
    h3d->Reset();

  } // loop over events
  cout << "]" << endl;

  Range_hi->SetBinContent(Range_hi->GetNbinsX(), Range_hi->GetBinContent(Range_hi->GetNbinsX()) + Range_hi->GetBinContent(Range_hi->GetNbinsX() + 1));
  T_MiEle->SetBinContent(T_MiEle->GetNbinsX(), T_MiEle->GetBinContent(T_MiEle->GetNbinsX()) + T_MiEle->GetBinContent(T_MiEle->GetNbinsX() + 1));
  T_delta->SetBinContent(T_delta->GetNbinsX(), T_delta->GetBinContent(T_delta->GetNbinsX()) + T_delta->GetBinContent(T_delta->GetNbinsX() + 1));
  Cubes_ele->SetBinContent(Cubes_ele->GetNbinsX(), Cubes_ele->GetBinContent(Cubes_ele->GetNbinsX()) + Cubes_ele->GetBinContent(Cubes_ele->GetNbinsX() + 1));
  Cubes_ele_only->SetBinContent(Cubes_ele_only->GetNbinsX(), Cubes_ele_only->GetBinContent(Cubes_ele_only->GetNbinsX()) + Cubes_ele_only->GetBinContent(Cubes_ele_only->GetNbinsX() + 1));

  TString out_file_name = prefix + "out_";
  out_file_name += run;
  out_file_name += ".root";
  TFile* out = new TFile(out_file_name.Data(), "RECREATE");
  Range_hi->Write();
  Mom_hi->Write();
  T_MiEle->Write();
  T_delta->Write();
  Cubes_ele->Write();
  Cubes_ele_only->Write();
  time_distance->Write();
  out->Close();
}