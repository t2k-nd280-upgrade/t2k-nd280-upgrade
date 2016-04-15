
void ReadG4out
(
 const char *tag = "prova",
 const char *filename = "../bin/EffStudy.root",
 )
{

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
  vector<int>   * VecTrackID  = new vector<int>();
  vector<int>   * VecTrackPDG = new vector<int>();
  vector<double>* VecTrackE   = new vector<double>();
  vector<double>* VecTrackMomX = new vector<double>();
  vector<double>* VecTrackMomY = new vector<double>();
  vector<double>* VecTrackMomZ = new vector<double>();
  vector<double>* VecTrackMom  = new vector<double>();
  vector<int>* VecTPCUp_TrackID = new vector<int>();

  // From all the volume
  treein->SetBranchAddress("NTracks",    &NTracks);  
  treein->SetBranchAddress("VecTrackID",  &VecTrackID);
  treein->SetBranchAddress("VecTrackPDG", &VecTrackPDG);
  treein->SetBranchAddress("VecTrackE",   &VecTrackE);
  treein->SetBranchAddress("VecTrackMomX",   &VecTrackMomX);
  treein->SetBranchAddress("VecTrackMomY",   &VecTrackMomY);
  treein->SetBranchAddress("VecTrackMomZ",   &VecTrackMomZ);
  treein->SetBranchAddress("VecTrackMomMag", &VecTrackMom);
  // From sensitive detector
  treein->SetBranchAddress("EabsTarget", &EabsTarget);
  treein->SetBranchAddress("EabsTPCup",  &EabsTPCup);
  treein->SetBranchAddress("EabsTPCdown",&EabsTPCdown);
  treein->SetBranchAddress("VecTPCUp_TrackID", &VecTPCUp_TrackID);
  
  // Loop over the ntuple events
  
  int Nentries = treein->GetEntries();
  for(int ientry=0;ientry<Nentries;ientry++){
    treein->GetEntry(ientry);

    cout << "Event " << ientry << endl;
        
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

    // TPC Up tracks
    for(int itrk=0;itrk<VecTPCUp_TrackID.size();itrk++){
      cout << VecTPCUp_TrackID->at(itrk) << ", ";
    }
    cout << endl;
    
    
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

  // Print 

  // Store plots

  char name[300];
  sprintf(name,"outputs/EabsTot_%s.pdf",tag);      cEabsTot->Print(name);
  sprintf(name,"outputs/cEabsTarget_%s.pdf",tag);  cEabsTarget->Print(name);
  sprintf(name,"outputs/cEabsTPCup_%s.pdf",tag);   cEabsTPCup->Print(name);
  sprintf(name,"outputs/cEabsTPCdown_%s.pdf",tag); cEabsTPCdown->Print(name);
   
  // Write text file
  
  ofstream * outfilep = new ofstream(Form("outputs/%s.txt",tag),ios_base::trunc);
  ofstream & outfile = *outfilep;

  stringstream streamer;
  streamer << endl;
  streamer << endl;
  cout << streamer.str() << endl;
  
  // get size of file
  long size;
  streamer.seekg(0,ifstream::end);
  size=streamer.tellg();
  streamer.seekg(0);

  outfile.write (streamer.str().c_str(),size);
  outfile.close();
}
