
{

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetTitleXSize(0.05);
  gStyle->SetTitleXOffset(0.8);

  TString var[4] = {"selmu_likemip", "selmu_likemu", "selmu_ecal_mipem", "selmu_ecal_EneOnL"};
  string var_title[4] = {"L_{MIP}", "L_{#mu}", "MipEM", "E/L"};
  float cut[4] = {0.8, 0.05, 0, 1.25};
  bool below[4] = {false, false, true, true};
  
  TString samplename[4] = {"FwdTPC", "BwdTPC", "HATPC", "ECal"};
  DrawingTools draw("jobs/files/config1_Target1.root");

  // loop on configurations
  for (int ic=0; ic<2; ic++) {
    // loop on target
    for (int it=1; it<=2; it++) {
      // loop on branches

      char filename[200];
      sprintf(filename, "jobs/files/config%i_Target%i.root", ic, it);
      DataSample sample(filename);

      for (int i=0; i<4; i++) {

	for (int il=0; il<4; il++) {

	  if (i<3  && (il==2 || il==3)) continue;
	  if (i==3 && (il==0 || il==1)) continue;
	  
	  TCanvas* c = new TCanvas();

	  if (il==0) draw.SetLegendPos(0.1, 0.7);
	  else       draw.SetLegendPos(0.7, 0.7);
	  draw.SetLegendSize(.2,.2);
	  draw.SetTitleX(var_title[il]);

	  
	  string accum;

	  if (il==1)
	    accum = (TString::Format("accum_level[0][%i]>3 && selmu_mom<500",i+12));
	  else
	    accum = (TString::Format("accum_level[0][%i]>3",i+12));
	  
	  if (il<2)
	    draw.Draw(sample, var[il].Data(), 50, -0.01, 1.01, "particle", accum);
	  else if (il==2)
	    draw.Draw(sample, var[il].Data(), 50, -40, 40, "particle", accum);
	  else if (il==3)
	    draw.Draw(sample, var[il].Data(), 50, 0, 2, "particle", accum);

	  draw.DrawCutLineVertical(cut[il], true, below[il] ? "l":"r");
	  c->Update();

	  c->SaveAs(TString::Format("fig/lkl/%s_config%i_Target%i_%s.eps", 
				      var[il].Data(), ic, it, samplename[i].Data()));
    
	}

	draw.Draw(sample, "selmu_ecal_mipem:selmu_ecal_EneOnL", 50, 0, 2, 50, -40, 40, "all", accum, "colz");
	
      }
    }
  }


}
