
void ReadG4out
(
 const char *tag = "PiMinus_125MeV_Targ20cm",
 const char *filename = "/Users/davidesgalaberna/Desktop/GENEVA_postdoc/G4FILES/PLOTS/EffStudy_PiMinus_125MeV_Targ20cm.root",

 // const char *tag = "PiMinus_125MeV_Targ30cm",
 // const char *filename = "/Users/davidesgalaberna/Desktop/GENEVA_postdoc/G4FILES/PLOTS/EffStudy_PiMinus_125MeV_Targ30cm.root",

 )
{
  int NTotPrimInTPCup = 0;
  int NTotPrimInTPCdown = 0;
  int NTotPrimInTarget = 0;

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

  double PDGPrim=0;        // pdg of primary particle of each event
  double PDGFirstTPCup=0;  // pdg of primary particle in TPC up
  double PDGFirstTPCdown=0;  // pdg of primary particle in TPC down
  double PDGFirstTarget=0;  // pdg of primary particle in Target
  double EabsTarget=0.; // Energy deposited
  double EabsTPCup=0.;
  double EabsTPCdown=0.;

  treein->SetBranchAddress("PDGPrim",&PDGPrim);
  treein->SetBranchAddress("PDGFirstTPCup",&PDGFirstTPCup);
  treein->SetBranchAddress("PDGFirstTPCdown",&PDGFirstTPCdown);
  treein->SetBranchAddress("PDGFirstTarget",&PDGFirstTarget);
  treein->SetBranchAddress("EabsTarget",&EabsTarget);
  treein->SetBranchAddress("EabsTPCup",&EabsTPCup);
  treein->SetBranchAddress("EabsTPCdown",&EabsTPCdown);
  
  int Nentries = treein->GetEntries();
  for(int ientry=0;ientry<Nentries;ientry++){
    treein->GetEntry(ientry);
    
    double Etot = EabsTarget + EabsTPCup + EabsTPCdown;
    hEabsTot->Fill(Etot);
    hEabsTPCup->Fill(EabsTPCup);
    hEabsTPCdown->Fill(EabsTPCdown);
    hEabsTarget->Fill(EabsTarget);
 
    if(PDGPrim==PDGFirstTPCup) NTotPrimInTPCup++;
    if(PDGPrim==PDGFirstTPCdown) NTotPrimInTPCdown++;
    if(PDGPrim==PDGFirstTarget) NTotPrimInTarget++;
    
  }
  
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

  // Print 

  // Store plots

  char name[300];
  sprintf(name,"outputs/EabsTot_%s.pdf",tag);      cEabsTot->Print(name);
  sprintf(name,"outputs/cEabsTarget_%s.pdf",tag);  cEabsTarget->Print(name);
  sprintf(name,"outputs/cEabsTPCup_%s.pdf",tag);   cEabsTPCup->Print(name);
  sprintf(name,"outputs/cEabsTPCdown_%s.pdf",tag); cEabsTPCdown->Print(name);
   
  // Write text file
  
  double FracPrimInTPCup   = (double)NTotPrimInTPCup / (double)Nentries;
  double FracPrimInTPCdown = (double)NTotPrimInTPCdown / (double)Nentries;
  double FracPrimInTarget  = (double)NTotPrimInTarget / (double)Nentries;

  //TFile fout(Form("%s.txt",name));
  ofstream * outfilep = new ofstream(Form("outputs/%s.txt",tag),ios_base::trunc);
  ofstream & outfile = *outfilep;

  stringstream streamer;
  streamer << endl;
  streamer << "Fraction of Primary particles in TPC Up: "   << FracPrimInTPCup << endl;
  streamer << "Fraction of Primary particles in TPC Down: " << FracPrimInTPCdown / Nentries << endl;
  streamer << "Fraction of Primary particles in Target: "   << FracPrimInTarget  / Nentries << endl;
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
