{
  std::string file;
  file = "$bars/AnalysisResults/ndUP/nue/AntiNuE_official_1.root";
  DrawingTools draw(file.c_str());
  DataSample sample(file.c_str());

  TCanvas c1("canva","",50,50,1000,800);

  draw.DrawEffVSCut(sample);
  c1.Print("$bars/dev/t2k-nd280-upgrade/highlandUP/figures/AntiNu.png");
}