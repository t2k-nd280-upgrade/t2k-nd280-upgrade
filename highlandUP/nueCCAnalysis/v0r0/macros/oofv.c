{
  std::string file;
  file = "$bars/AnalysisResults/ndUP/nue/Alternative_Target_TPC/Alternative_Target_TPC_ToF_oofv.root";
  DrawingTools draw(file.c_str());
  DataSample sample(file.c_str());

  TCanvas c1("canva","",50,50,1000,800);

  draw.DrawEventsVSCut(sample);
  c1.Print("$bars/dev/t2k-nd280-upgrade/highlandUP/figures/oofv.png");
}