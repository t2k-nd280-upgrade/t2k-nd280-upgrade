void draw_efficiencies(std::string FWD,std::string BWD,std::string HaECal) {
	TStyle *t2kStyle = new TStyle("T2K", "T2K approved plots style");

	// use plain black on white colors
	t2kStyle->SetFrameBorderMode(0);
	t2kStyle->SetCanvasBorderMode(0);
	t2kStyle->SetPadBorderMode(0);
	t2kStyle->SetPadColor(0);
	t2kStyle->SetCanvasColor(0);
	t2kStyle->SetStatColor(0);
	t2kStyle->SetLegendBorderSize(1);

	// set the paper & margin sizes
	t2kStyle->SetPaperSize(20, 26);
	t2kStyle->SetPadTopMargin(0.10);
	t2kStyle->SetPadRightMargin(0.16);
	t2kStyle->SetPadBottomMargin(0.16);
	t2kStyle->SetPadLeftMargin(0.13);

	// use large Times-Roman fonts
	t2kStyle->SetTextFont(132);
	t2kStyle->SetTextSize(0.06);
	t2kStyle->SetLabelFont(132, "x");
	t2kStyle->SetLabelFont(132, "y");
	t2kStyle->SetLabelFont(132, "z");
	t2kStyle->SetLabelSize(0.05, "x");
	t2kStyle->SetTitleSize(0.06, "x");
	t2kStyle->SetLabelSize(0.05, "y");
	t2kStyle->SetTitleSize(0.06, "y");
	t2kStyle->SetTitleOffset(0.9, "y");
	t2kStyle->SetTitleOffset(0.75, "z");
	t2kStyle->SetLabelSize(0.05, "z");
	t2kStyle->SetTitleSize(0.06, "z");
	t2kStyle->SetLabelFont(132, "t");
	t2kStyle->SetTitleFont(132, "x");
	t2kStyle->SetTitleFont(132, "y");
	t2kStyle->SetTitleFont(132, "z");
	t2kStyle->SetTitleFont(132, "t");
	t2kStyle->SetTitleFillColor(0);
	t2kStyle->SetTitleX(0.25);
	t2kStyle->SetTitleFontSize(0.06);
	t2kStyle->SetTitleFont(132, "pad");

	t2kStyle->SetTitleBorderSize(1);
	t2kStyle->SetPadBorderSize(1);
	t2kStyle->SetCanvasBorderSize(1);

	// use bold lines and markers
	t2kStyle->SetMarkerStyle(20);
	t2kStyle->SetHistLineWidth(1.85);
	t2kStyle->SetLineStyleString(2, "[12 12]"); // postscript dashes

	// get rid of X error bars and y error bar caps
	t2kStyle->SetErrorX(0.001);

	// do not display any of the standard histogram decorations
//    t2kStyle->SetOptTitle(0);
	//t2kStyle->SetOptStat(0);
	t2kStyle->SetOptFit(0);

	// put tick marks on top and RHS of plots
	t2kStyle->SetPadTickX(1);
	t2kStyle->SetPadTickY(1);

	// Add a greyscale palette for 2D plots
	int ncol = 50;
	double dcol = 1. / float(ncol);
	double gray = 0;

	TColor **theCols = new TColor*[ncol];

	for (int i = 0; i < ncol; i++) theCols[i] = new TColor(999 - i, 0.0, 1.0, 0.0);

	for (int j = 0; j < ncol; j++) {
		theCols[j]->SetRGB(gray, 0, 0);
		gray += dcol;
	}

	int ColJul[100];
	for  (int i = 0; i < 100; i++) ColJul[i] = 999 - i;
	t2kStyle->SetPalette(ncol, ColJul);
	gROOT->SetStyle("T2K");
// 	gROOT->ForceStyle();
	gStyle->SetPadLeftMargin(0.14);
	gStyle->SetPadRightMargin(0.15);
	//gStyle->SetStatX(.4); gStyle->SetStatY(.6);


//	std::string cuttarget = Cut(target);
//  std::string cuttargetortpc=Cut(target,pdg,1);
	// For efficiency
  const int NBins_Mom = 30;
  double BinEdges_Mom[NBins_Mom+1] = {0,1,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1200,1400,
  				      1600,1800,2000,2500,3000,4000,5000};
  const int NBins_CosTh = 20;
  double BinEdges_CosTh[NBins_CosTh+1] = {-1,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};
	const int NBins_Mom2 = 14;

	double BinEdges_Mom2[NBins_Mom2 + 1] = {
		0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000,1400, 2000, 2500, 3000};

	const int NBins_Phi = 20;
	double BinEdges_Phi[NBins_Phi + 1];
	double phi_width = 2 * TMath::Pi() / NBins_Phi;
	for (int iphi = 0; iphi < NBins_Phi + 1; iphi++) {
		BinEdges_Phi[iphi] = -TMath::Pi() + iphi * phi_width;
		cout << BinEdges_Phi[iphi] << ", ";
	}





	DrawingTools drawFwd(FWD.c_str());
	DataSample sampleFwd(FWD.c_str());
	DataSample samplebwd(BWD.c_str());

	DataSample sampleHaFwd(HaECal.c_str());
	TFile * hfile = new TFile("numuCC_4pi_efficiencies_plots.root","RECREATE","ROOT file");
	drawFwd.SetTitleX("P_{#mu}, MeV");
	drawFwd.SetTitleY("Efficiency");
	TCanvas *BGcanvas = new TCanvas("Efficiency_mom", "", 50, 50, 1000, 800);
 	drawFwd.DrawEff(sampleFwd.GetTree("truth"), "true_mom", NBins_Mom, BinEdges_Mom, "accum_level[][]>4", "accum_level[][]>1", "", "","FWD UP");
	drawFwd.DrawEff(samplebwd.GetTree("truth"), "true_mom", NBins_Mom, BinEdges_Mom, "accum_level[][]>4", "accum_level[][]>1", "same","","BWD UP");
	drawFwd.DrawEff(sampleHaFwd.GetTree("truth"), "true_mom", NBins_Mom, BinEdges_Mom, "accum_level[][]>3", "accum_level[][]>1", "same","","ECAl UP");

	BGcanvas->Modified();

	BGcanvas->Update();

	hfile->Append(BGcanvas);
	//BGcanvas->Print("Efficiency_mom_pid_all_4Pi.pdf");

	TCanvas *BGcanvas_2 = new TCanvas("Efficiency_cos", "", 50, 50, 1000, 800);
	drawFwd.SetTitleX("cos#theta");

	drawFwd.DrawEff(sampleFwd.GetTree("truth"), "true_costheta", NBins_CosTh, BinEdges_CosTh, "accum_level[][]>4", "accum_level[][]>1", "", "","FWD UP");
	drawFwd.DrawEff(samplebwd.GetTree("truth"), "true_costheta", NBins_CosTh, BinEdges_CosTh, "accum_level[][]>4", "accum_level[][]>1",  "same","","BWD UP");
	drawFwd.DrawEff(sampleHaFwd.GetTree("truth"), "true_costheta", NBins_CosTh, BinEdges_CosTh, "accum_level[][]>3", "accum_level[][]>1",  "same","","ECAl UP ");

	BGcanvas_2->Modified();

	BGcanvas_2->Update();

	hfile->Append(BGcanvas_2);
	hfile->Write();
	hfile->Close();
}
