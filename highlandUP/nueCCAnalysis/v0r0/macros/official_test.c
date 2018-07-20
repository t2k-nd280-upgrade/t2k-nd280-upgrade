{
  std::string file;
  file = "$bars/AnalysisResults/ndUP/nue/Official/NuE_off_run2a.root";
  DrawingTools draw(file.c_str());

  Experiment* exp = new Experiment("nd280");

  SampleGroup run2a("run2a");
  SampleGroup run2w("run2w");
  SampleGroup run3b("run3b");
  SampleGroup run3c("run3c");
  SampleGroup run4a("run4a");
  SampleGroup run4w("run4w");

  DataSample* run2aDS  = new DataSample("$bars/AnalysisResults/ndUP/nue/Official/NuE_off_run2a.root");
  DataSample* run2wDS  = new DataSample("$bars/AnalysisResults/ndUP/nue/Official/NuE_off_run2w.root");
  DataSample* run3bDS  = new DataSample("$bars/AnalysisResults/ndUP/nue/Official/NuE_off_run3b.root");
  DataSample* run3cDS  = new DataSample("$bars/AnalysisResults/ndUP/nue/Official/NuE_off_run3c.root");
  DataSample* run4aDS  = new DataSample("$bars/AnalysisResults/ndUP/nue/Official/NuE_off_run4a.root");
  DataSample* run4wDS  = new DataSample("$bars/AnalysisResults/ndUP/nue/Official/NuE_off_run4w.root");

  run2a.AddMCSample("run2aDS", run2aDS);
  run2w.AddMCSample("run2wDS", run2wDS);
  run3b.AddMCSample("run3bDS", run3bDS);
  run3c.AddMCSample("run3cDS", run3cDS);
  run4a.AddMCSample("run4aDS", run4aDS);
  run4w.AddMCSample("run4wDS", run4wDS);

  exp->AddSampleGroup("run2a", run2a);
  exp->AddSampleGroup("run2w", run2w);
  exp->AddSampleGroup("run3b", run3b);
  exp->AddSampleGroup("run3c", run3c);
  exp->AddSampleGroup("run4a", run4a);
  exp->AddSampleGroup("run4w", run4w);
  
  TCanvas c1("canva","",50,50,1000,800);
  /*
  if (IsNuESignal(*vtx, det, AntiNu) && abs(track->GetTrueParticle()->PDG) == 11 && track->GetTrueParticle()->ParentPDG != 111 && track->GetTrueParticle()->GParentPDG != 111){

    if (IsNuECC0pi(*vtx, det, AntiNu))
      return 1; // nue CC0pi
    else 
      return 2; // nue CCother
  }

  if (abs(track->GetTrueParticle()->PDG) == 13) return 4; // muon

  if (AntiNu && (abs(track->GetTrueParticle()->PDG) == 2212)) return 8; // extra category for NuEBar selection

  if (IsGammaSignalOOFGD(track, det)) return 6; // gamma OOFGD background

  if (IsGammaSignal(track, det)) return 3; // gamma inFGD background

  return 5; // Other
  */

  std::string nuEsignal = "nuesimple < 3";
  std::string nuEoofvGamma = "nuesimple == 6";
  std::string nuEinfvGamma = "nuesimple == 3";


  /*draw.Draw(*exp, "selelec_mom", 50, 0., 5000., "nuesimple", "accum_level[][] > 17" , "", "OVER");
  c1.Print("$bars/dev/t2k-nd280-upgrade/highlandUP/figures/official/mom.png");

  draw.Draw(*exp, "selelec_mom", 50, 0., 5000., "particle", "accum_level[][] > 5 && selelec_mom > 200" , "", "OVER");
  c1.Print("$bars/dev/t2k-nd280-upgrade/highlandUP/figures/official/mom5.png");
  draw.Draw(*exp, "selelec_mom", 50, 0., 5000., "particle", "accum_level[][] > 7 && selelec_mom > 200" , "", "OVER");
  c1.Print("$bars/dev/t2k-nd280-upgrade/highlandUP/figures/official/mom7.png");
  draw.Draw(*exp, "selelec_mom", 50, 0., 5000., "particle", "accum_level[][] > 8 && selelec_mom > 200" , "", "OVER");
  c1.Print("$bars/dev/t2k-nd280-upgrade/highlandUP/figures/official/mom8.png");*/

  //draw.DrawEffPurVSCut(*exp, nuEsignal, "", 0, 17);

  /*draw.SetMinY(100);
  draw.SetLogY(1);
  draw.DrawEventsVSCut(*exp, nuEoofvGamma);
  c1.Print("$bars/dev/t2k-nd280-upgrade/highlandUP/figures/official/oofv_ev.png");
  draw.SetLogY(0);
  draw.SetMinY();

  draw.DrawEventsVSCut(*exp, nuEinfvGamma);
  c1.Print("$bars/dev/t2k-nd280-upgrade/highlandUP/figures/official/infv_ev.png");
*/
  draw.DrawEventsVSCut(*exp, "abs(particle) == 11");
  c1.Print("$bars/dev/t2k-nd280-upgrade/highlandUP/figures/official/sig_ev.png");

  draw.SetMinY(1000);
  draw.SetLogY(1);
  draw.DrawEventsVSCut(*exp);
  c1.Print("$bars/dev/t2k-nd280-upgrade/highlandUP/figures/official/all_ev.png");
  draw.SetLogY(0);
  draw.SetMinY();
  
}