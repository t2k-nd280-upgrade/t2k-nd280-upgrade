void PiMuEle(Int_t first_entry, Int_t Nentries, Int_t run) {
  //TString input_file = "$bars/AnalysisResults/ndUP/SuperFGD/proto2/proto2_pi500_deg-Exp0-Nexpt1000.root";
  TString input_file = "$bars/AnalysisResults/ndUP/SuperFGD/proto2/proto2_pi500_deg2_25000.root";
  //TString input_file = "$bars/AnalysisResults/ndUP/SuperFGD/SFGD/sfgd_pip_300_25000.root";
  // 1 for pi+ / -1 for pi-
  Int_t mode = 1;
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

  TH3F* h3dMu = new TH3F("3d", "", binX, X_min, X_max, binY, Y_min, Y_max, binZ, Z_min, Z_max);
  TH3F* h3dEle = new TH3F("3d1", "", binX, X_min, X_max, binY, Y_min, Y_max, binZ, Z_min, Z_max);

  int NTreeEntries = tree->GetEntries();
  int evtlasttree = first_entry + Nentries;

  if (evtlasttree > NTreeEntries-1) {
    cout << "Oveflow" << endl;
    evtlasttree = NTreeEntries-1;
  }

  Int_t DEBUG = 0;

  TH1D* T_delta_mu    = new TH1D("T_delta_mu",   "", 40, 0., 200.);
  TH1D* T_delta_ele   = new TH1D("t_delta_ele",   "", 80, 0., 8000.);
  TH1D* Cubes_ele     = new TH1D("cubes_ele", "", 50, 0., 50.);
  TH1D* Cubes_mu      = new TH1D("cubes_mu",  "", 50, 0., 50.);
  TH2D* energy_time   = new TH2D("energy_time", "", 100, 0., 10.,  1600, 0., 8000.);
  TH1D* stop_z        = new TH1D("stopZ", "", 200, 0., 200.);

  const Int_t ED = 100;
  TH2D* energy_time_ED[ED];
  Int_t energy_time_ED_counter = 0;
  for (Int_t iED = 0; iED < ED; ++iED)
     energy_time_ED[iED]  = new TH2D(Form("energy_time_%i", iED), "", 100, 0., 10.,  1600, 0., 8000.);

  cout << "*************************************************************************************************" << endl;
  cout << "Run        " << run << endl;
  cout << "First      " << first_entry << endl;
  cout << "Nentries   " << Nentries << endl;

  cout << "[          ] Nev = " << first_entry << "...." << evtlasttree << " From " << NTreeEntries <<"\r[";
  for(int ievt = first_entry; ievt < evtlasttree; ++ievt) {

    if ((ievt - first_entry) * 10 % (evtlasttree - first_entry) == 0)
      std::cout << "." << std::flush;
    tree->GetEntry(ievt);

    Double_t max_ele_mom   = -1.;
    Double_t t_ele_first   = 1e9;
    Double_t t_mu_first    = 1e9;
    Double_t t_mu_last     = -1.;
    Double_t t_pi_last     = -1.; 
    int EleID              = -1;
    int MuID               = -1;
    int PionID             = 0.;
    Int_t pion_decay_cube  = -1;

    bool pion_found = false;
    bool verbose = false;

    Int_t Ntracks = nd280UpEvent->GetNTracks();

    // perform 3 loops. 
    // 1 until stopping pion found
    // 2 until muon found
    // 3 until michel electron found
    int itrack = 0;
    for (itrack = 0; itrack < Ntracks; ++itrack) {
      TND280UpTrack* track = nd280UpEvent->GetTrack(itrack);

      // detemine the stopped pion
      if (track->GetParentID() == PionID && track->GetPDG() == 211*mode) {
        Double_t X = track->GetPoint(track->GetNPoints() - 1)->GetPostPosition().X();
        Double_t Y = track->GetPoint(track->GetNPoints() - 1)->GetPostPosition().Y();
        Double_t Z = track->GetPoint(track->GetNPoints() - 1)->GetPostPosition().Z();
        pion_decay_cube = h3dMu->FindBin(X, Y, Z + 1000.);
        if (PionID == 0)
          cout << "Parent: " << endl;
        else cout << "Daughters: " << endl;
        cout << track->GetTrackID() << "    " << X << "   " << Y << "   " << Z << endl;
        PionID = track->GetTrackID();
        stop_z->Fill((Z + 440.)/10.);
      }

      int PDG = track->GetPDG();
      string process = track->GetProcessName();

      // look at muon
      if (PDG == -13*mode && process == "Decay" && track->GetParentID() == 1) {
        cout << process << "  " << PDG << "    " << track->GetParentID() << "   " << track->GetPoint(1)->GetPostPosition().Z() << endl;
        MuID = track->GetTrackID();
      }

      // look at the electron from decay

      if (process != "Decay" && process !="muMinusCaptureAtRest")
        continue;

      if (PDG != -11*mode)
        continue;

      if (track->GetParentID() != MuID)
        continue;

      double mom    = track->GetInitMom().Mag();
    
      if (mom > max_ele_mom) {
        max_ele_mom   = mom;
        EleID = track->GetTrackID();
      }    
    } // loop over track

    int NcubesEle = 0;
    int NEleOnlyCubes = 0;

    // loop over hits
    for(int ihit = 0; ihit < nd280UpEvent->GetNHits(); ++ihit) {
      TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);

      double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
      double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2.; // middle step Y 
      double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2.; // middle step Z
      double posT = (nd280UpHit->GetStartT() + nd280UpHit->GetStopT())/2.; // middle step T

      Int_t bin = h3dMu->FindBin(posX, posY, posZ);

      if (pion_decay_cube == bin) {
        energy_time->Fill(nd280UpHit->GetEnergyDeposit(), posT);  
        if (ievt < iED)
          energy_time_ED[ievt]->Fill(nd280UpHit->GetEnergyDeposit(), posT);
      }

      bool foundPi = false;
      bool foundMu = false;
      bool foundEle = false;

      for (UInt_t i = 0; i < nd280UpHit->fContributors.size(); ++i) {
        if (nd280UpHit->fContributors[i] == 1) {
          foundPi = true;
        } else if (nd280UpHit->fContributors[i] == MuID) {
          foundMu = true;
        } else if (nd280UpHit->fContributors[i] == EleID) {
          foundEle = true;
        }
      }

      if (foundPi) {
        if (t_pi_last < nd280UpHit->GetStartT())
          t_pi_last = nd280UpHit->GetStartT();
      }
      if (foundMu) {
        if (t_mu_last < nd280UpHit->GetStartT())
          t_mu_last = nd280UpHit->GetStartT();
        if (t_mu_first > nd280UpHit->GetStartT())
          t_mu_first = nd280UpHit->GetStartT();
      }
      if (foundEle) {
        if (t_ele_first > nd280UpHit->GetStartT())
          t_ele_first = nd280UpHit->GetStartT();
      }

      if (!h3dMu->GetBinContent(bin) && foundMu) {
        h3dMu->Fill(posX, posY, posZ);
      }

      if (!h3dEle->GetBinContent(bin) && foundEle) {
        h3dEle->Fill(posX, posY, posZ);
      }

      
    } // loop over hit


    if (EleID != -1) {
      T_delta_ele->Fill(t_ele_first - t_mu_last);
      Cubes_ele->Fill(h3dEle->Integral());
    }

    if (MuID != -1) {
      T_delta_mu->Fill(t_mu_first - t_pi_last);
      Cubes_mu->Fill(h3dMu->Integral());
    }

    h3dMu->Reset();
    h3dEle->Reset();

  } // loop over events
  cout << "]" << endl;

  T_delta_ele->SetBinContent(T_delta_ele->GetNbinsX(), T_delta_ele->GetBinContent(T_delta_ele->GetNbinsX()) + T_delta_ele->GetBinContent(T_delta_ele->GetNbinsX() + 1));
  T_delta_mu->SetBinContent(T_delta_mu->GetNbinsX(), T_delta_mu->GetBinContent(T_delta_mu->GetNbinsX()) + T_delta_mu->GetBinContent(T_delta_mu->GetNbinsX() + 1));

  TString out_file_name = prefix + "out_";
  out_file_name += run;
  out_file_name += ".root";
  TFile* out = new TFile(out_file_name.Data(), "RECREATE");
  T_delta_ele->Write();
  T_delta_mu->Write();
  Cubes_ele->Write();
  Cubes_mu->Write();
  energy_time->Write();
  stop_z->Write();

  for (Int_t iED = 0; iED < ED; ++iED)
     energy_time_ED[iED]->Write();

  out->Close();
}