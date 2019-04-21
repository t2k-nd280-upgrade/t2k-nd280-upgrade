int neutron2() {
  TGraph* graph = new TGraph();

  graph->SetPoint(0, 2, 0.92512);
  graph->SetPoint(1, 5, 0.91968);
  graph->SetPoint(2, 10, 0.97328);
  graph->SetPoint(3, 20, 0.97296);
  graph->SetPoint(4, 50, 0.81448);
  graph->SetPoint(5, 100, 0.6358);
  graph->SetPoint(6, 200, 0.54148);
  graph->SetPoint(7, 500, 0.55644);
  graph->SetMinimum(0.);

  TCanvas c1("canva","",50,50,1000,800);
  TString prefix = "/t2k/users/suvorov/figure/ndUP/proton/";
  graph->Draw("apl");
  c1.Print((prefix + "neutron.pdf").Data());
  
}