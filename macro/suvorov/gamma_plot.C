void gamma_plot() {
  TString prefix = "/t2k/users/suvorov/figure/ndUP/gamma/";
  TString file_name = prefix + "out_800.root";
  TFile* file       = new TFile(file_name.Data(), "READ");
  TCanvas c1("canva","",50,50,1000,800);

  TH1D* gamma_ini        =  (TH1D*)file->Get("gamma_ini");
  TH1D* gamma_ini_e          =  (TH1D*)file->Get("gamma_ini_e");
  TH1D* gamma_enter         =  (TH1D*)file->Get("gamma_enter");
  TH1D* gamma_enter_e       =  (TH1D*)file->Get("gamma_enter_e");

  //gStyle->SetOptStat(0);

  Int_t Nbin = gamma_enter_e->GetNbinsX();

  cout << " total gammas in proto " << gamma_enter_e->Integral(1, Nbin) << endl;
  cout << " > 4 MeV   " << gamma_enter_e->Integral(2, Nbin) << endl;
  cout << " > 20 MeV  " << gamma_enter_e->Integral(6, Nbin) << endl;
  cout << " > 48 MeV  " << gamma_enter_e->Integral(13, Nbin) << endl;
  cout << " > 100 MeV " << gamma_enter_e->Integral(26, Nbin) << endl;

  gamma_ini_e->SetBinContent(1, 0.);
  gamma_enter_e->SetBinContent(1, 0.);

  gPad->SetLogy();

  gamma_ini->Draw();
  c1.Print((prefix + "gamma_ini.pdf").Data());
  gamma_ini_e->Draw();
  c1.Print((prefix + "gamma_ini_e.pdf").Data());
  gamma_enter->Draw();
  c1.Print((prefix + "gamma_enter.pdf").Data());
  gamma_enter_e->Draw();
  c1.Print((prefix + "gamma_enter_e.pdf").Data());



  exit();
}