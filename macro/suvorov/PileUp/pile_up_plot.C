void pile_up_plot() {
	TString prefix    = "/t2k/users/suvorov/figure/ndUP/PileUp/plot/";
  TString file_name = prefix + "neutron_4w.root";
  TFile* file       = new TFile(file_name.Data(), "READ");

  TCanvas c1("canva","",50,50,1000,800);

  TH1D* cubes_pileup        =  (TH1D*)file->Get("cubes_pileup");

  cout << "Totsl events         : " << cubes_pileup->Integral() << endl;
  cout << "Events with hits     : " << cubes_pileup->Integral(2, cubes_pileup->GetNbinsX()) << endl;
  cout << "Events with 1 hit    : " << cubes_pileup->GetBinContent(2) << endl;
  cout << "Events with > 5 hits : " << cubes_pileup->Integral(7, cubes_pileup->GetNbinsX()) << endl;

  gStyle->SetOptStat(0);

  cubes_pileup->SetBinContent(1, 0.);
  cubes_pileup->Draw();
  c1.Print((prefix + "hits.pdf").Data());
  exit(1);
}