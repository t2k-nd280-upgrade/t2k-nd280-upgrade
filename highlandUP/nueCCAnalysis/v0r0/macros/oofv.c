{
  std::string file;
  file = "$bars/AnalysisResults/ndUP/nue/Alternative_Target_TPC/Alternative_Target_TPC_ToF_oofv.root";
  DrawingTools draw(file.c_str());
  DataSample sample(file.c_str());

  TCanvas c1("canva","",50,50,1000,800);

  std::string nuEsignal = "nueCC < 3";
  std::string nuEoofvGamma = "nueCC == 6";
  std::string nuEinfvGamma = "nueCC == 3";

  draw.Draw(sample, "selelec_mom", 50, 0., 5000., "nueCC", "accum_level[][0] > 8" , "", "OVER");
  c1.Print("$bars/dev/t2k-nd280-upgrade/highlandUP/figures/oofv/mom.png");

  draw.SetMinY(0.5);
  draw.SetLogY(1);
  draw.DrawEventsVSCut(sample, nuEoofvGamma);
  c1.Print("$bars/dev/t2k-nd280-upgrade/highlandUP/figures/oofv/oofv_ev.png");
  draw.SetLogY(0);
  draw.SetMinY();

  draw.DrawEventsVSCut(sample, nuEinfvGamma);
  c1.Print("$bars/dev/t2k-nd280-upgrade/highlandUP/figures/oofv/infv_ev.png");

  draw.DrawEventsVSCut(sample, nuEsignal);
  c1.Print("$bars/dev/t2k-nd280-upgrade/highlandUP/figures/oofv/sig_ev.png");

  draw.SetMinY(0.5);
  draw.SetLogY(1);
  draw.DrawEventsVSCut(sample);
  c1.Print("$bars/dev/t2k-nd280-upgrade/highlandUP/figures/oofv/all_ev.png");
  draw.SetLogY(0);
  draw.SetMinY();
}