void PiMuEle_plot() {
  TString prefix    = "/t2k/users/suvorov/figure/ndUP/Michel/";
  TString file_name = prefix + "out_0.root";
  TFile* file       = new TFile(file_name.Data(), "READ");
  TCanvas c1("canva","",50,50,1000,800);

  TH1D* T_delta_mu    = (TH1D*)file->Get("T_delta_mu");
  TH1D* T_delta_ele   = (TH1D*)file->Get("t_delta_ele");
  TH1D* Cubes_ele     = (TH1D*)file->Get("cubes_ele");
  TH1D* Cubes_mu      = (TH1D*)file->Get("cubes_mu");
  TH2D* energy_time   = (TH2D*)file->Get("energy_time");
  TH1D* stop_z        = (TH1D*)file->Get("stopZ");

  stop_z->Draw();
  c1.Print((prefix + "stop_z.pdf").Data());

  
  /*const Int_t ED = 100;
  TH2D* energy_time_ED[ED];
  for (Int_t iED = 0; iED < ED; ++iED) {
     energy_time_ED[iED]  = (TH2D*)file->Get(Form("energy_time_%i", iED));
     TH1D* test = new TH1D("test", "", 1600, 0., 8000.);
     for (bin = 1; bin <= energy_time_ED[iED]->GetYaxis()->GetNbins(); ++bin) {
      Double_t count = 0;
      for (bin2 = 1; bin2 <= energy_time_ED[iED]->GetXaxis()->GetNbins(); ++bin2) {
        count += energy_time_ED[iED]->GetBinContent(bin2, bin);
      }
      test->SetBinContent(bin, count);
     }
     test->Draw();
     c1.Print((prefix + Form("energy_%i.pdf", iED)).Data());
     delete test;
   }*/



/*

  Range_hi->GetXaxis()->SetTitle("e range, mm");
  Mom_hi->GetXaxis()->SetTitle("e momentum, MeV/c");
  T_delta->GetXaxis()->SetTitle("time delay, ns");
  Cubes_ele->GetXaxis()->SetTitle("cubes hited by e");
  Cubes_ele_only->GetXaxis()->SetTitle("cubes hited mainly by e");

  for (Int_t i = 0; i < time_distance->GetYaxis()->GetNbins(); ++i) {
    time_distance->SetBinContent(80, i, time_distance->GetBinContent(80, i) + time_distance->GetBinContent(81, i));
  }
*/
  /*T_delta_mu->Draw();
  c1.Print((prefix + "T_delta_mu.pdf").Data());
  T_delta_ele->Draw();
  c1.Print((prefix + "T_delta_ele.pdf").Data());
  Cubes_ele->Draw();
  c1.Print((prefix + "Cubes_ele.pdf").Data());
  Cubes_ele->SetBinContent(1, 0);
  Cubes_ele->Draw();
  c1.Print((prefix + "Cubes_ele_wo0.pdf").Data());
  Cubes_mu->Draw();
  c1.Print((prefix + "Cubes_mu.pdf").Data());
  Cubes_mu->SetBinContent(1, 0);
  Cubes_mu->Draw();
  c1.Print((prefix + "Cubes_mu_wo0.pdf").Data());
  energy_time->Draw("colz");
  c1.Print((prefix + "energy_time.pdf").Data());
  energy_time->ProjectionY()->Draw();
  c1.Print((prefix + "time.pdf").Data());
  energy_time->ProjectionX()->Draw();
  c1.Print((prefix + "energy.pdf").Data());*/


/*
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
*/
  exit();


}