void proton(){
  // Chaining input files
  TChain* chain = new TChain("ND280upEvents");
  TString input_path  = "/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/";
  TString geom        = "proto2";
  TString particle    = "p";
  TString mom         = "600";

  chain->Add((input_path + geom + "/" + geom + "_" + particle + "_" + mom + "MeV.root").Data());

  TString rootfilename = input_path + geom + "/" + geom + "_" + particle + "_" + mom + "MeV.root";
  TFile *finput = new TFile(rootfilename.Data(),"READ");
  cout << "Reading the input ROOT file: " << rootfilename << endl;

  Int_t DEBUG = 0;

  // define output path
  TString prefix = "/t2k/users/suvorov/figure/ndUP/proton/";
  TCanvas c1("canva","",50,50,1000,800);

  // Read ReadOut geometry
  TH2F* h2d_xy; TH2F* h2d_xz; TH2F* h2d_yz;
  h2d_xy = (TH2F*)finput->Get("OutMPPCProj2D_XY");
  h2d_xz = (TH2F*)finput->Get("OutMPPCProj2D_XZ");
  h2d_yz = (TH2F*)finput->Get("OutMPPCProj2D_YZ");

  TH2F* stop_xy = h2d_xy->Clone("stop_XY");

  // define output histos
  TH2F* stop_xz = h2d_xz->Clone("stop_xz");
  TH2F* stop_yz = h2d_yz->Clone("stop_yz");

  TH1F* E_dep = new TH1F("edep", "Deposited energy", 24, 0., 24.);

  TND280UpEvent *nd280UpEvent = new TND280UpEvent();
  chain->SetBranchAddress("Event",&nd280UpEvent);

  int NTreeEntries = chain->GetEntries();

  int first_entry = 0;
  int evtlasttree = NTreeEntries-1;

  cout << endl;
  cout << "# of entries in the Tree: " << NTreeEntries << endl;
  cout << "last tree event ID: " << evtlasttree << endl;
  cout << "first event ID: " << 0 << endl;
  cout << endl;

  Int_t threshold = -1;
  cout << "[          ] Nev = " << first_entry << "...." << evtlasttree << " From " << NTreeEntries <<"\r[";

  for(int ievt = first_entry; ievt < evtlasttree; ++ievt) {

    if ((ievt - first_entry)%((evtlasttree-first_entry)/10) == 0) std::cout << "." << std::flush;

    chain->GetEntry(ievt);

    bool stopped_primary = true;
    for (int itrack = 0; itrack < nd280UpEvent->GetNTracks(); ++itrack) {
      TND280UpTrack* track = nd280UpEvent->GetTrack(itrack);
      if (!track->GetPoint(track->GetNPoints() - 1)) {
        stopped_primary = false;
        break;
      }
      if (track->GetTrackID() != 1 || track->GetPDG() != 2212)
        continue;
      if (track->GetPoint(track->GetNPoints() - 1)->GetPostPosition().Z() > 300) {
        stopped_primary = false;
        break;
      }
    } // loop over tracks

    if (!stopped_primary) continue;

    Double_t stop_x, stop_y, stop_z;
    Int_t stop_x_bin, stop_y_bin, stop_z_bin;

    for (int itrack = 0; itrack < nd280UpEvent->GetNTracks(); ++itrack) {

      TND280UpTrack* track = nd280UpEvent->GetTrack(itrack);

      if (track->GetTrackID() != 1) continue;
      int Npoints = track->GetNPoints();

        stop_x = track->GetPoint(Npoints - 1)->GetPostPosition().X();
        stop_y = track->GetPoint(Npoints - 1)->GetPostPosition().Y();
        stop_z = track->GetPoint(Npoints - 1)->GetPostPosition().Z();

        stop_x_bin = stop_xy->GetXaxis()->FindBin(stop_x);
        stop_y_bin = stop_xy->GetYaxis()->FindBin(stop_y);
        stop_z_bin = stop_xz->GetYaxis()->FindBin(stop_z);

        stop_xy->Fill(stop_x, stop_z, 1/25000.);
        stop_yz->Fill(stop_y, stop_z, 1/25000.);
        stop_xz->Fill(stop_x, stop_z, 1/25000.);
    
    } // loop over tracks

    //cout << stop_z << "    " << stop_z_bin << endl;

    // look at hits for proton stopped at 1 1 21
    if (stop_x_bin != 13 || stop_y_bin != 5 || stop_z_bin != 21)
      continue;

    for (int ihit = 0; ihit < nd280UpEvent->GetNHits(); ++ihit) {
      TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);

      bool proton_hit = false;
      // store only proton hits
      for (UInt_t i = 0; i < nd280UpHit->fContributors.size(); ++i) {
        if (nd280UpHit->fContributors[i] == 1)
          proton_hit = true;
      }

      if (!proton_hit)
        continue;

      //double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
      //double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2.; // middle step Y 
      double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2.; // middle step Z
      //double posT = (nd280UpHit->GetStartT() + nd280UpHit->GetStopT())/2.; // middle step T

      Int_t z_bin = stop_xz->GetYaxis()->FindBin(posZ);

      E_dep->Fill(z_bin, nd280UpHit->GetEnergyDeposit());

    }


  } // end of event loop
  std::cout << std::endl;

  // print figures
  stop_xy->Draw("colz");
  c1.Print((prefix + mom + "_stop_xy.png").Data());

  stop_xz->GetZaxis()->SetRangeUser(0., 0.35);
  stop_xz->Draw("colz");
  c1.Print((prefix + mom + "_stop_xz.png").Data());

  stop_yz->GetZaxis()->SetRangeUser(0., 0.35);
  stop_yz->Draw("colz");
  c1.Print((prefix + mom + "_stop_yz.png").Data());

  E_dep->Draw();
  c1.Print((prefix + mom + "_Edep.png").Data());
}