  
int GetEventsWithHits(TString filename) {
  
  int passed = 0;

  for (Int_t fileID = 0; fileID < 10; ++fileID){
    TString name = filename;
    name += fileID*2500;
    name += "-Nexpt2500.root";

    TFile* file = new TFile(name, "READ");
    TTree* tree = (TTree*)file->Get("ND280upEvents");

    TND280UpEvent *nd280UpEvent = new TND280UpEvent();
    tree->SetBranchAddress("Event",&nd280UpEvent);

    cout << "File "  << name << endl;
    Int_t Nevt = tree->GetEntries();
    for(int ievt = 0; ievt < Nevt; ++ievt) {
      tree->GetEntry(ievt);
      cout << "Ev: " << ievt <<"\r";


      if (nd280UpEvent->GetNHits() > 0)
        ++passed;
    }
    cout << endl;
    file->Close();
    //delete tree;
    //delete file;
  }
  return passed;
}

void neutron(){
  TString input_path  = "/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/SFGD/";
  Int_t ene[8] = {500, 100, 200, 500, 50, 100, 200, 500};

  TGraph* eff = new TGraph();
  for (Int_t i = 0; i < 8; ++i) {
    TString filename = input_path + "n_";
    filename += ene[i];
    filename += "-Exp";

    int passed = GetEventsWithHits(filename);
    cout << ene[i] << "   " << passed / 25000. << endl;

    eff->SetPoint(i, ene[i], passed / 25000.);
  }

  TCanvas c1("canva","",50,50,1000,800);
  TString prefix = "/t2k/users/suvorov/figure/ndUP/";
  TFile* out_file = new TFile((prefix+"neutron.root", "RECREATE"));
  eff->Draw("apl");
  eff->Write();
  c1.Print((prefix + "neutron.pdf").Data());
  out_file->Close();
}
