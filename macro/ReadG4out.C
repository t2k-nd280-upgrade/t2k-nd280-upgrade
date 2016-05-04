
void ReadG4out
(
 const char *tag = "prova",
 const char *filename = "../bin/EffStudy.root",
 )
{
  // Vertex
  TH2D *hVtx_XY = new TH2D("hVtx_XY","hVtx_XY",400,-4,4,400,-4,4);
  TH2D *hVtx_XZ = new TH2D("hVtx_XZ","hVtx_XZ",400,-4,4,400,-4,4);
  TH2D *hVtx_YZ = new TH2D("hVtx_YZ","hVtx_YZ",400,-4,4,400,-4,4);
  TH1D *hVtx_X = new TH1D("hVtx_X","hVtx_X",400,-4,4);
  TH1D *hVtx_Y = new TH1D("hVtx_Y","hVtx_Y",400,-4,4);
  TH1D *hVtx_Z = new TH1D("hVtx_Z","hVtx_Z",400,-4,4);
  TH1D *hVtx_NuPDG = new TH1D("hVtx_NuPDG","hVtx_NuPDG",10,10,20);
  TH1D *hVtx_ReacMode = new TH1D("hVtx_ReacMode","hVtx_ReacMode",100,-50,50);
  
  // All the volume
  TH1D *hNTracks = new TH1D("hNTracks","hNTracks",100,0,100);
  TH1D *hTrackE = new TH1D("hTrackE","hTrackE",100,0,2000);
  TH1D *hTrackPDG = new TH1D("hTrackPDG","hTrackPDG",500,-500,500);
  TH1D *hTrackMomX = new TH1D("hTrackMomX","hTrackMomX",100,-1000,+1000);
  TH1D *hTrackMomY = new TH1D("hTrackMomY","hTrackMomY",100,-1000,+1000);
  TH1D *hTrackMomZ = new TH1D("hTrackMomZ","hTrackMomZ",100,-1000,+1000);
  TH1D *hTrackMom  = new TH1D("hTrackMom","hTrackMom",100,0,1000);
  
  // From Sensitive detector
  TH1D *hEabsTot = new TH1D("hEabsTot","hEabsTot",100,0,150);
  TH1D *hEabsTarget = new TH1D("hEabsTarget","hEabsTarget",100,0,150);
  TH1D *hEabsTPCup = new TH1D("hEabsTPCup","hEabsTPCup",100,0,150);
  TH1D *hEabsTPCdown = new TH1D("hEabsTPCdown","hEabsTPCdown",100,0,150);
  
  TH1D *hTPCUp_NTrack = new TH1D("hTPCUp_NTrack","hTPCUp_NTrack",100,0,300);
  TH1D *hTPCUp_MomX = new TH1D("hTPCUp_MomX","hTPCUp_MomX",100,0,300);
  TH1D *hTPCUp_MomY = new TH1D("hTPCUp_MomY","hTPCUp_MomY",100,0,300);
  TH1D *hTPCUp_MomZ = new TH1D("hTPCUp_MomZ","hTPCUp_MomZ",100,0,300);
  TH1D *hTPCUp_Mom = new TH1D("hTPCUp_Mom","hTPCUp_Mom",100,0,300);

  TH1D *hTPCDown_NTrack = new TH1D("hTPCDown_NTrack","hTPCDown_NTrack",100,0,300);
  TH1D *hTPCDown_MomX = new TH1D("hTPCDown_MomX","hTPCDown_MomX",100,0,300);
  TH1D *hTPCDown_MomY = new TH1D("hTPCDown_MomY","hTPCDown_MomY",100,0,300);
  TH1D *hTPCDown_MomZ = new TH1D("hTPCDown_MomZ","hTPCDown_MomZ",100,0,300);
  TH1D *hTPCDown_Mom = new TH1D("hTPCDown_Mom","hTPCDown_Mom",100,0,300);

  TH1D *hTarget_NTrack = new TH1D("hTarget_NTrack","hTarget_NTrack",100,0,300);
  TH1D *hTarget_MomX = new TH1D("hTarget_MomX","hTarget_MomX",100,0,300);
  TH1D *hTarget_MomY = new TH1D("hTarget_MomY","hTarget_MomY",100,0,300);
  TH1D *hTarget_MomZ = new TH1D("hTarget_MomZ","hTarget_MomZ",100,0,300);
  TH1D *hTarget_Mom = new TH1D("hTarget_Mom","hTarget_Mom",100,0,300);


  cout << endl;
  cout << "Reading file:" << endl;
  cout << filename << endl;
  cout << endl;
  
  TFile *filein = new TFile(filename,"READ");
  TTree *treein = (TTree*)filein->Get("EffStudy");


  // Set ntuple branch addresses

  double EabsTarget=0.; // Energy deposited
  double EabsTPCup=0.;
  double EabsTPCdown=0.;
  int NTracks=0;

  // Global informations
  vector<int>   * VecTrackID  = new vector<int>();
  vector<int>   * VecTrackPDG = new vector<int>();
  vector<double>* VecTrackE   = new vector<double>();
  vector<double>* VecTrackMomX = new vector<double>();
  vector<double>* VecTrackMomY = new vector<double>();
  vector<double>* VecTrackMomZ = new vector<double>();
  vector<double>* VecTrackMom  = new vector<double>();

  // TPC up informations
  int     TPCUp_NTracks;          // # of tracks in TPC Up
  std::vector<int>   * VecTPCUp_TrackID; // Vector of trackID in TPCup
  std::vector<double>* VecTPCUp_TrackMomX; // Vector of initial mom in TPC Up
  std::vector<double>* VecTPCUp_TrackMomY; // Vector of initial mom in TPC Up
  std::vector<double>* VecTPCUp_TrackMomZ; // Vector of initial mom in TPC Up

  // TPC down informations
  int     TPCDown_NTracks;          // # of tracks in TPC Down
  std::vector<int>   * VecTPCDown_TrackID; // Vector of trackID in TPCDown
  std::vector<double>* VecTPCDown_TrackMomX; // Vector of initial mom in TPC Down
  std::vector<double>* VecTPCDown_TrackMomY; // Vector of initial mom in TPC Down
  std::vector<double>* VecTPCDown_TrackMomZ; // Vector of initial mom in TPC Down

  // Target informations
  int     Target_NTracks;          // # of tracks in Target
  std::vector<int>   * VecTarget_TrackID; // Vector of trackID in Target
  std::vector<double>* VecTarget_TrackMomX; // Vector of initial mom in Target
  std::vector<double>* VecTarget_TrackMomY; // Vector of initial mom in Target 
  std::vector<double>* VecTarget_TrackMomZ; // Vector of initial mom in Target
  
  // Vertex informations
  int     NVtx;
  std::vector<double> *VecVtx_X; // Vector of vertex X position
  std::vector<double> *VecVtx_Y; // Vector of vertex Y position
  std::vector<double> *VecVtx_Z; // Vector of vertex Z position
  std::vector<int>    *VecVtx_NuPDG;     // Vector of vertex Nu PDG
  std::vector<int>    *VecVtx_ReacMode;  // Vector of vertex reaction mode
  std::vector<double> *VecVtx_EvtProb;   // Vector of vertex event prob
  std::vector<double> *VecVtx_EvtWeight; // Vector of vertex event weight


  //
  // From all the volume
  //

  treein->SetBranchAddress("NTracks",    &NTracks);  
  treein->SetBranchAddress("VecTrackID",  &VecTrackID);
  treein->SetBranchAddress("VecTrackPDG", &VecTrackPDG);
  treein->SetBranchAddress("VecTrackE",   &VecTrackE);
  treein->SetBranchAddress("VecTrackMomX",   &VecTrackMomX);
  treein->SetBranchAddress("VecTrackMomY",   &VecTrackMomY);
  treein->SetBranchAddress("VecTrackMomZ",   &VecTrackMomZ);
  treein->SetBranchAddress("VecTrackMomMag", &VecTrackMom);

  //
  // From sensitive detector
  //
  
  treein->SetBranchAddress("EabsTarget", &EabsTarget);
  treein->SetBranchAddress("EabsTPCup",  &EabsTPCup);
  treein->SetBranchAddress("EabsTPCdown",&EabsTPCdown);

  treein->SetBranchAddress("TPCUp_NTracks",      &TPCUp_NTracks);
  treein->SetBranchAddress("VecTPCUp_TrackID",   &VecTPCUp_TrackID);
  treein->SetBranchAddress("VecTPCUp_TrackMomX", &VecTPCUp_TrackMomX);
  treein->SetBranchAddress("VecTPCUp_TrackMomY", &VecTPCUp_TrackMomY);
  treein->SetBranchAddress("VecTPCUp_TrackMomZ", &VecTPCUp_TrackMomZ);

  treein->SetBranchAddress("TPCDown_NTracks",      &TPCDown_NTracks);
  treein->SetBranchAddress("VecTPCDown_TrackID",   &VecTPCDown_TrackID);
  treein->SetBranchAddress("VecTPCDown_TrackMomX", &VecTPCDown_TrackMomX);
  treein->SetBranchAddress("VecTPCDown_TrackMomY", &VecTPCDown_TrackMomY);
  treein->SetBranchAddress("VecTPCDown_TrackMomZ", &VecTPCDown_TrackMomZ);

  treein->SetBranchAddress("Target_NTracks",      &Target_NTracks);
  treein->SetBranchAddress("VecTarget_TrackID",   &VecTarget_TrackID);
  treein->SetBranchAddress("VecTarget_TrackMomX", &VecTarget_TrackMomX);
  treein->SetBranchAddress("VecTarget_TrackMomY", &VecTarget_TrackMomY);
  treein->SetBranchAddress("VecTarget_TrackMomZ", &VecTarget_TrackMomZ);

  treein->SetBranchAddress("NVtx", &NVtx);
  treein->SetBranchAddress("VtxX", &VecVtx_X);
  treein->SetBranchAddress("VtxY", &VecVtx_Y);
  treein->SetBranchAddress("VtxZ", &VecVtx_Z);
  treein->SetBranchAddress("VecVtx_NuPDG", &VecVtx_NuPDG);
  treein->SetBranchAddress("VecVtx_ReacMode", &VecVtx_ReacMode);
  treein->SetBranchAddress("VecVtx_EvtProb", &VecVtx_EvtProb);
  treein->SetBranchAddress("VecVtx_EvtWeight", &VecVtx_EvtWeight);



  // Loop over the ntuple events
  
  int Nentries = treein->GetEntries();
  for(int ientry=0;ientry<Nentries;ientry++){
    treein->GetEntry(ientry);

    cout << endl;
    cout << "Event " << ientry << endl;

    //
    // Read the vertices
    //
    
    if(NVtx!=1){
      cerr << endl;
      cerr << "The # of vertices is " << NVtx << "!!!" << endl;
      cerr << endl;
      exit(1);
    }
    
    if( VecVtx_X->size()!=NVtx){
      cerr << endl;
      cerr << "The vector size is larger than the # of vertices:" << endl;
      cerr << "NVtx = " << NVtx << endl;
      cerr << "VecVtx_X->size() = " << VecVtx_X->size() << endl;
      cerr << endl;
      exit(1);
    }
    
    for(int ivtx=0;ivtx<NVtx;ivtx++){

      double vtx_x = VecVtx_X->at(ivtx);
      double vtx_y = VecVtx_Y->at(ivtx);
      double vtx_z = VecVtx_Z->at(ivtx);

      hVtx_XY->Fill(vtx_x,vtx_y);
      hVtx_XZ->Fill(vtx_x,vtx_z);
      hVtx_YZ->Fill(vtx_y,vtx_z);      
      hVtx_X->Fill(vtx_x);
      hVtx_Y->Fill(vtx_y);
      hVtx_Z->Fill(vtx_z);

      hVtx_NuPDG->Fill( VecVtx_NuPDG->at(ivtx));
      hVtx_ReacMode->Fill( VecVtx_ReacMode->at(ivtx));
    }

    
    //
    // Read all the tracks in the event (all volume)
    //

    if(NTracks!=VecTrackID->size()){
      cerr << endl;
      cerr << "VecTrackID.size() != NTracks" << endl;
      cerr << "VecTrackID.size() = " << VecTrackID->size() << endl;
      cerr << "NTracks = " << NTracks << endl;
      cerr << endl;
      exit(1);
    }

    for(int itrk=0;itrk<NTracks;itrk++){

      int trackid = VecTrackID->at(itrk);      
      int trackPDG = VecTrackPDG->at(itrk);
      double trackE = VecTrackE->at(itrk);
      
      double trackMomX = VecTrackMomX->at(itrk);
      double trackMomY = VecTrackMomY->at(itrk);
      double trackMomZ = VecTrackMomZ->at(itrk);
      double trackMom  = VecTrackMom ->at(itrk);

      hTrackPDG->Fill(trackPDG);
      hTrackE->Fill(trackE);
      hTrackMomX->Fill(trackMomX);
      hTrackMomY->Fill(trackMomY);
      hTrackMomZ->Fill(trackMomZ);
      hTrackMom->Fill(trackMom);
    }

    hNTracks->Fill(NTracks);
    
    //
    // Get info from each single detector
    //
    
    double Etot = EabsTarget + EabsTPCup + EabsTPCdown;
    hEabsTot->Fill(Etot);
    hEabsTPCup->Fill(EabsTPCup);
    hEabsTPCdown->Fill(EabsTPCdown);
    hEabsTarget->Fill(EabsTarget);

    //
    // TPC Up tracks
    //

    hTPCUp_NTrack->Fill(TPCUp_NTracks);
    
    for(int itrk=0;itrk<VecTPCUp_TrackID->size();itrk++){
     //cout << VecTPCUp_TrackID->at(itrk) << ", ";

      double momX = VecTPCUp_TrackMomX->at(itrk);
      double momY = VecTPCUp_TrackMomY->at(itrk);
      double momZ = VecTPCUp_TrackMomZ->at(itrk);
      hTPCUp_MomX->Fill(momX);
      hTPCUp_MomY->Fill(momY);
      hTPCUp_MomZ->Fill(momZ);
      
      double mom = sqrt( momX*momX + momY*momY + momZ*momZ );
      hTPCUp_Mom->Fill(mom);
    }
    //cout << endl;

    //
    // TPC Down tracks
    //

    hTPCDown_NTrack->Fill(TPCDown_NTracks);
    
    for(int itrk=0;itrk<VecTPCDown_TrackID->size();itrk++){
     //cout << VecTPCDown_TrackID->at(itrk) << ", ";

      double momX = VecTPCDown_TrackMomX->at(itrk);
      double momY = VecTPCDown_TrackMomY->at(itrk);
      double momZ = VecTPCDown_TrackMomZ->at(itrk);
      hTPCDown_MomX->Fill(momX);
      hTPCDown_MomY->Fill(momY);
      hTPCDown_MomZ->Fill(momZ);
      
      double mom = sqrt( momX*momX + momY*momY + momZ*momZ );
      hTPCDown_Mom->Fill(mom);
    }
    //cout << endl;

    //
    // Target tracks
    //

    hTarget_NTrack->Fill(Target_NTracks);
    
    for(int itrk=0;itrk<VecTarget_TrackID->size();itrk++){
     //cout << VecTarget_TrackID->at(itrk) << ", ";

      double momX = VecTarget_TrackMomX->at(itrk);
      double momY = VecTarget_TrackMomY->at(itrk);
      double momZ = VecTarget_TrackMomZ->at(itrk);
      hTarget_MomX->Fill(momX);
      hTarget_MomY->Fill(momY);
      hTarget_MomZ->Fill(momZ);
      
      double mom = sqrt( momX*momX + momY*momY + momZ*momZ );
      hTarget_Mom->Fill(mom);
    }
    //cout << endl;
    
  } // end loop over events

  
  
  // Draw histograms
  
  TCanvas *cEabsTot = new TCanvas("cEabsTot","cEabsTot");
  hEabsTot->GetXaxis()->SetTitle("Deposited energy (MeV)");
  hEabsTot->GetYaxis()->SetTitle("Entries");
  hEabsTot->Draw();

  TCanvas *cEabsTarget = new TCanvas("cEabsTarget","cEabsTarget");
  hEabsTarget->GetXaxis()->SetTitle("Deposited energy (MeV)");
  hEabsTarget->GetYaxis()->SetTitle("Entries");
  hEabsTarget->Draw();

  TCanvas *cEabsTPCup = new TCanvas("cEabsTPCup","cEabsTPCup");
  hEabsTPCup->GetXaxis()->SetTitle("Deposited energy (MeV)");
  hEabsTPCup->GetYaxis()->SetTitle("Entries");
  hEabsTPCup->Draw();

  TCanvas *cEabsTPCdown = new TCanvas("cEabsTPCdown","cEabsTPCdown");
  hEabsTPCdown->GetXaxis()->SetTitle("Deposited energy (MeV)");
  hEabsTPCdown->GetYaxis()->SetTitle("Entries");
  hEabsTPCdown->Draw();

  TCanvas *cNTracks = new TCanvas("cNTracks","cNTracks");
  hNTracks->Draw();
  
  TCanvas *cTrackPDG = new TCanvas("cTrackPDG","cTrackPDG");
  hTrackPDG->Draw();
  
  TCanvas *cTrackE = new TCanvas("cTrackE","cTrackE");
  hTrackE->Draw();

  TCanvas *cTrackMomX = new TCanvas("cTrackMomX","cTrackMomX");
  hTrackMomX->Draw();
  TCanvas *cTrackMomY = new TCanvas("cTrackMomY","cTrackMomY");
  hTrackMomY->Draw();
  TCanvas *cTrackMomZ = new TCanvas("cTrackMomZ","cTrackMomZ");
  hTrackMomZ->Draw();
  TCanvas *cTrackMom = new TCanvas("cTrackMom","cTrackMom");
  hTrackMom->Draw();

  TCanvas *cTPCUp_TrackMom = new TCanvas("cTPCUp_TrackMom","cTPCUp_TrackMom");
  hTPCUp_Mom->Draw();

  TCanvas *cTPCDown_TrackMom = new TCanvas("cTPCDown_TrackMom","cTPCDown_TrackMom");
  hTPCDown_Mom->Draw();

  TCanvas *cTarget_TrackMom = new TCanvas("cTarget_TrackMom","cTarget_TrackMom");
  hTarget_Mom->Draw();

  
  TCanvas *cVtx_X = new TCanvas("cVtx_X","cVtx_X");
  hVtx_X->Draw();
  TCanvas *cVtx_Y = new TCanvas("cVtx_Y","cVtx_Y");
  hVtx_Y->Draw();
  TCanvas *cVtx_Z = new TCanvas("cVtx_Z","cVtx_Z");
  hVtx_Z->Draw();

  TCanvas *cVtx_XY = new TCanvas("cVtx_XY","cVtx_XY");
  hVtx_XY->Draw("colz");
  TCanvas *cVtx_XZ = new TCanvas("cVtx_XZ","cVtx_XZ");
  hVtx_XZ->Draw("colz");
  TCanvas *cVtx_YZ = new TCanvas("cVtx_YZ","cVtx_YZ");
  hVtx_YZ->Draw("colz");
  


  // Print 

  // Store plots

  char name[300];
  sprintf(name,"outputs/EabsTot_%s.pdf",tag);      cEabsTot->Print(name);
  sprintf(name,"outputs/cEabsTarget_%s.pdf",tag);  cEabsTarget->Print(name);
  sprintf(name,"outputs/cEabsTPCup_%s.pdf",tag);   cEabsTPCup->Print(name);
  sprintf(name,"outputs/cEabsTPCdown_%s.pdf",tag); cEabsTPCdown->Print(name);
   
  sprintf(name,"outputs/cVtx_X_%s.pdf",tag); cVtx_X->Print(name);
  sprintf(name,"outputs/cVtx_Y_%s.pdf",tag); cVtx_Y->Print(name);
  sprintf(name,"outputs/cVtx_Z_%s.pdf",tag); cVtx_Z->Print(name);
  sprintf(name,"outputs/cVtx_XY_%s.pdf",tag); cVtx_XY->Print(name);
  sprintf(name,"outputs/cVtx_XZ_%s.pdf",tag); cVtx_XZ->Print(name);
  sprintf(name,"outputs/cVtx_YZ_%s.pdf",tag); cVtx_YZ->Print(name);

  // Write text file
  
  ofstream * outfilep = new ofstream(Form("outputs/%s.txt",tag),ios_base::trunc);
  ofstream & outfile = *outfilep;

  stringstream streamer;
  //streamer << endl;
  //streamer << endl;
  cout << streamer.str() << endl;
  
  // get size of file
  long size;
  streamer.seekg(0,ifstream::end);
  size=streamer.tellg();
  streamer.seekg(0);

  outfile.write (streamer.str().c_str(),size);
  outfile.close();
}
