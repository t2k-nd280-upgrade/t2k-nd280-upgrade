void MIEle(Int_t first_entry, Int_t Nentries, Int_t run, TString  out_file_name, TString input_file) {
  TString input_file_path = input_file;
  TFile* file = new TFile(input_file_path.Data(), "READ");

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
  TH1D* T_delta   = new TH1D("t_delta",   "", 80, 0., 8000.);
  TH1D* Cubes_ele = new TH1D("cubes_ele", "", 50, 0., 50.);
  TH2D* time_distance   = new TH2D("time_distance", "", 800, 0., 8000.,  50, 0., 50.);
  TH1D* stop_z        = new TH1D("stopZ", "", 200, 0., 200.);
  TH1D* angle         = new TH1D("deg_angle", "", 200, 0.5, 1.);
  TH2D* angle_z       = new TH2D("angle_z", "", 200, 0.5, 1., 530, 0., 5300.);


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

    Double_t max_ele_mom    = -1.;
    Double_t max_ele_range  = -1.;
    int EleID               = -1;
    Double_t t_ele_first   = 1e9;
    Double_t t_mu_last     = -1.;

    for (int itrack = 0; itrack < nd280UpEvent->GetNTracks(); ++itrack) {
      TND280UpTrack* track = nd280UpEvent->GetTrack(itrack);

      if (track->GetTrackID() == 1) {
        Double_t X = track->GetPoint(track->GetNPoints() - 1)->GetPostPosition().X();
        Double_t Y = track->GetPoint(track->GetNPoints() - 1)->GetPostPosition().Y();
        Double_t Z = track->GetPoint(track->GetNPoints() - 1)->GetPostPosition().Z();
        //cout << X << "   " << Y << "    " << Z << endl;
        stop_z->Fill((Z + 490.)/10.);

        int Np = track->GetNPoints();

        for (int ipoint = 0; ipoint < Np; ++ipoint) {
          TND280UpTrackPoint* point = track->GetPoint(ipoint);
          if (!point)
            continue;
          Double_t poZ = track->GetPoint(ipoint)->GetPostPosition().Z();
          Double_t cosZ = point->GetMomentum().Z() / point->GetMomentum().Mag();

          angle_z->Fill(cosZ, poZ);
          //cout << point->GetPostPosition().Z() << endl;
          if (poZ > 250. - 490. && poZ < 0.) {
            //cout << poZ << "    " << point->GetMomentum().Z() / point->GetMomentum().Mag() << endl;
            angle->Fill(cosZ);
            break;
          }
        }
      }

      if (track->GetParentID() != 1)
        continue;

      int PDG = track->GetPDG();

      if (PDG != 11)
        continue;

      string process = track->GetProcessName();

      if (process != "Decay" && process !="muMinusCaptureAtRest")
        continue;

      double mom    = track->GetInitMom().Mag();
      double range  = track->GetRange();

      if (mom > max_ele_mom) {
        max_ele_mom   = mom;
        max_ele_range = range;
        EleID = track->GetTrackID();
      }    
    } // loop over track

    int NcubesEle = 0;

    for(int ihit = 0; ihit < nd280UpEvent->GetNHits(); ++ihit) {
      TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);

      double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
      double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2.; // middle step Y 
      double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2.; // middle step Z
      double time = nd280UpHit->GetStartT();

      bool foundEle = false;
      bool foundMu = false;

      for (UInt_t i = 0; i < nd280UpHit->fContributors.size(); ++i) {
        if (nd280UpHit->fContributors[i] == EleID)
          foundEle = true;
        if (nd280UpHit->fContributors[i] == 1)
          foundMu = true;
      }

      if (foundMu) {
        if (t_mu_last < time)
          t_mu_last = time;
      }
      if (foundEle) {
        if (t_ele_first > time)
          t_ele_first = time;
      }

      Int_t bin = h3d->FindBin(posX, posY, posZ);
      if (!h3d->GetBinContent(bin) && foundEle) {
        h3d->Fill(posX, posY, posZ);
      }
    } // loop over hit

    if (max_ele_mom > 0.) {
      Range_hi->Fill(max_ele_range);
      Mom_hi->Fill(max_ele_mom);
    }

    if (foundEle) {
      Range_hi->Fill(max_ele_range);
      Mom_hi->Fill(max_ele_mom);
      T_delta->Fill(t_ele_first - t_mu_last);
      Cubes_ele->Fill(h3d->Integral());
      time_distance->Fill(t_ele_first - t_mu_last, h3d->Integral());
    }
    h3d->Reset();

  } // loop over events
  cout << "]" << endl;

  //Range_hi->SetBinContent(Range_hi->GetNbinsX(), Range_hi->GetBinContent(Range_hi->GetNbinsX()) + Range_hi->GetBinContent(Range_hi->GetNbinsX() + 1));
  //T_delta->SetBinContent(T_delta->GetNbinsX(), T_delta->GetBinContent(T_delta->GetNbinsX()) + T_delta->GetBinContent(T_delta->GetNbinsX() + 1));
  //Cubes_ele->SetBinContent(Cubes_ele->GetNbinsX(), Cubes_ele->GetBinContent(Cubes_ele->GetNbinsX()) + Cubes_ele->GetBinContent(Cubes_ele->GetNbinsX() + 1));

  out_file_name = prefix + "/" + out_file_name + "_";
  out_file_name += run;
  out_file_name += ".root";
  cout << "Writing to file " << out_file_name << endl;
  TFile* out = new TFile(out_file_name.Data(), "RECREATE");
  Range_hi->Write();
  Mom_hi->Write();
  T_delta->Write();
  Cubes_ele->Write();
  time_distance->Write();
  stop_z->Write();
  angle->Write();
  angle_z->Write();
  out->Close();
}