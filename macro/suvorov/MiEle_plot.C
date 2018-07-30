void MiEle_plot() {
  TString prefix    = "/t2k/users/suvorov/figure/ndUP/Michel/";
  TString file_name = prefix + "out.root";
  TFile* file       = new TFile(file_name.Data(), "READ");
  TCanvas c1("canva","",50,50,1000,800);

  TH1D* Range_hi        =  (TH1D*)file->Get("range");
  TH1D* Mom_hi          =  (TH1D*)file->Get("mom");
  TH1D* T_MiEle         =  (TH1D*)file->Get("t_MiEle");
  TH1D* T_delta         =  (TH1D*)file->Get("t_delta");
  TH1D* Cubes_ele       =  (TH1D*)file->Get("cubes_ele");
  TH1D* Cubes_ele_only  =  (TH1D*)file->Get("cubes_ele_only");
  TH2D* time_distance   =  (TH2D*)file->Get("time_distance");

  cout << "time_distance dump X " << time_distance->GetXaxis()->GetNbins() << endl;

  Range_hi->GetXaxis()->SetTitle("e range, mm");
  Mom_hi->GetXaxis()->SetTitle("e momentum, MeV/c");
  T_delta->GetXaxis()->SetTitle("time delay, ns");
  Cubes_ele->GetXaxis()->SetTitle("cubes hited by e");
  Cubes_ele_only->GetXaxis()->SetTitle("cubes hited mainly by e");

  for (Int_t i = 0; i < time_distance->GetYaxis()->GetNbins(); ++i) {
    time_distance->SetBinContent(80, i, time_distance->GetBinContent(80, i) + time_distance->GetBinContent(81, i));
  }

  Range_hi->Draw();
  c1.Print((prefix + "MiEle_range.pdf").Data());
  Mom_hi->Draw();
  c1.Print((prefix + "MiEle_mom.pdf").Data());
  T_MiEle->Draw();
  c1.Print((prefix + "MiEle_time.pdf").Data());
  T_delta->Draw();
  c1.Print((prefix + "MiEle_delta_time.pdf").Data());
  Cubes_ele->Draw();
  c1.Print((prefix + "Cubes_ele.pdf").Data());
  Cubes_ele_only->Draw();
  c1.Print((prefix + "Cubes_ele_only.pdf").Data());
  time_distance->Draw("colz");
  c1.Print((prefix + "time_distance.pdf").Data());


  T_delta->SetBinContent(1, 0.);
  TF1* f1 = new TF1("f1", "[0]*exp(-x/[1])", 0, 8000);
  f1->SetParameters(600, 2000);

  T_delta->Draw();
  T_delta->Fit("f1", "", "", 100, 7000);
  c1.Print((prefix + "MiEle_delta_time_wo_first.pdf").Data());

  Int_t cubes_spatial = 5;

  Double_t all  = time_distance->Integral();
  Double_t BM   = time_distance->Integral(6, 800, 1, 30) + time_distance->Integral(0, 5, cubes_spatial, 30);
  Double_t CA   = time_distance->Integral(61, 800, 1, 30) + time_distance->Integral(0, 60, cubes_spatial, 30);

  cout << all << endl;
  cout << BM / all << endl;
  cout << CA / all << endl;

  cout << BM / CA << endl;

  BM   = time_distance->Integral(6, 800, 1, cubes_spatial-1) + time_distance->Integral(0, 800, cubes_spatial, 30);
  CA   = time_distance->Integral(61, 800, 1, cubes_spatial-1) + time_distance->Integral(0, 800, cubes_spatial, 30);

  cout << all << endl;
  cout << BM / all << endl;
  cout << CA / all << endl;

  cout << BM / CA << endl;

  exit();


}