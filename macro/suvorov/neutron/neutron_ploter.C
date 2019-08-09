#include "TStyle.h"
#include "TROOT.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"

TStyle* SetT2KStyle(Int_t WhichStyle, TString styleName);

void neutron_ploter() {
	// setup the T2K style
  Int_t T2KstyleIndex = 3;
  // Official T2K style as described in http://www.t2k.org/comm/pubboard/style/index_html
  TString localStyleName = "T2K";
  // -- WhichStyle --
  // 1 = presentation large fonts
  // 2 = presentation small fonts
  // 3 = publication/paper
  TStyle* t2kstyle = SetT2KStyle(T2KstyleIndex, localStyleName);

  gROOT->SetStyle(t2kstyle->GetName());
  gROOT->ForceStyle();

  TCanvas* c1 = new TCanvas("c1", "", 800, 630);

  TFile* fi = new TFile("~/Downloads/neutron_ekin_spectrum.root", "READ");
  TH1F* h = (TH1F*)fi->Get("h1");

  TFile* f = new TFile("~/node_out/ndUP/SuperFGD/neutron/plot/plot_neutron_v36.root", "READ");

  TLegend* leg = new TLegend(0.1,0.7,0.48,0.9);

  TGraphAsymmErrors* t1_10 = (TGraphAsymmErrors*)f->Get("CutValue_10/resol_graph_5");
  TGraphAsymmErrors* t1_20 = (TGraphAsymmErrors*)f->Get("CutValue_20/resol_graph_5");
  TGraphAsymmErrors* t1_40 = (TGraphAsymmErrors*)f->Get("CutValue_40/resol_graph_5");
  TGraphAsymmErrors* t1_70 = (TGraphAsymmErrors*)f->Get("CutValue_70/resol_graph_5");

  TGraphAsymmErrors* t2_10 = (TGraphAsymmErrors*)f->Get("CutValue_10/resol_graph_6");
  TGraphAsymmErrors* t2_20 = (TGraphAsymmErrors*)f->Get("CutValue_20/resol_graph_6");
  TGraphAsymmErrors* t2_40 = (TGraphAsymmErrors*)f->Get("CutValue_40/resol_graph_6");
  TGraphAsymmErrors* t2_70 = (TGraphAsymmErrors*)f->Get("CutValue_70/resol_graph_6");

  t1_20->GetXaxis()->SetTitle("T_{neutron} [MeV]");
  t1_20->GetYaxis()->SetTitle("Resolution");

  int g_color = kGreen+3;
  int b_color = kBlue+3;

  t1_10->SetMarkerColor(g_color);
  t1_10->SetLineColor(g_color);
  t1_10->SetMarkerSize(0.8);
  t1_10->SetMarkerStyle(21);

	t2_10->SetMarkerColor(g_color);
	t2_10->SetLineColor(g_color);
	t2_10->SetMarkerSize(0.8);
  t2_10->SetMarkerStyle(25);

  t1_20->SetMarkerColor(3);
  t1_20->SetLineColor(3);
  t1_20->SetMarkerSize(0.8);
  t1_20->SetMarkerStyle(21);

	t2_20->SetMarkerColor(3);
	t2_20->SetLineColor(3);
	t2_20->SetMarkerSize(0.8);
  t2_20->SetMarkerStyle(25);

  t1_70->SetMarkerColor(b_color);
  t1_70->SetLineColor(b_color);
  t1_70->SetMarkerSize(0.8);
  t1_70->SetMarkerStyle(21);

	t2_70->SetMarkerColor(b_color);
	t2_70->SetLineColor(b_color);
	t2_70->SetMarkerSize(0.8);
  t2_70->SetMarkerStyle(25);



  //t1_10->Draw("ap");
  //t2_10->Draw("same p");

  t1_20->Draw("ap");
  t2_20->Draw("same p");

  t1_70->Draw("same p");
  t2_70->Draw("same p");

  h->Scale(0.5 / h->GetMaximum());
  h->SetFillColor(17);
  h->SetLineColor(17);
  h->SetFillStyle(3004);
  h->Draw("same hist");

  leg->AddEntry(t1_20, "20 cm", "p");
  leg->AddEntry(t1_70, "70 cm", "p");

  leg->AddEntry(h, "Neutron spectrum", "f");
  leg->Draw();

}

TStyle* SetT2KStyle(Int_t WhichStyle, TString styleName) {
  TStyle *t2kStyle= new TStyle(styleName, "T2K approved plots style");

  // -- WhichStyle --
  // 1 = presentation large fonts
  // 2 = presentation small fonts
  // 3 = publication/paper

  Int_t FontStyle = 22;
  Float_t FontSizeLabel = 0.035;
  Float_t FontSizeTitle = 0.05;
  Float_t YOffsetTitle = 1.3;

  switch(WhichStyle) {
  case 1:
    FontStyle = 42;
    FontSizeLabel = 0.05;
    FontSizeTitle = 0.065;
    YOffsetTitle = 1.19;
    break;
  case 2:
    FontStyle = 42;
    FontSizeLabel = 0.035;
    FontSizeTitle = 0.05;
    YOffsetTitle = 1.6;
    break;
  case 3:
    FontStyle = 132;
    FontSizeLabel = 0.035;
    FontSizeTitle = 0.05;
    YOffsetTitle = 1.6;
    break;
  }

  // use plain black on white colors
  t2kStyle->SetFrameBorderMode(0);
  t2kStyle->SetCanvasBorderMode(0);
  t2kStyle->SetPadBorderMode(0);
  t2kStyle->SetCanvasBorderSize(0);
  t2kStyle->SetFrameBorderSize(0);
  t2kStyle->SetDrawBorder(0);
  t2kStyle->SetTitleBorderSize(0);

  t2kStyle->SetPadColor(0);
  t2kStyle->SetCanvasColor(0);
  t2kStyle->SetStatColor(0);
  t2kStyle->SetFillColor(0);

  t2kStyle->SetEndErrorSize(4);
  t2kStyle->SetStripDecimals(kFALSE);

  t2kStyle->SetLegendBorderSize(0);
  t2kStyle->SetLegendFont(FontStyle);

  // set the paper & margin sizes
  t2kStyle->SetPaperSize(25, 26);
  t2kStyle->SetPadTopMargin(0.1);
  t2kStyle->SetPadBottomMargin(0.15);
  t2kStyle->SetPadRightMargin(0.15); // 0.075 -> 0.13 for colz option
  t2kStyle->SetPadLeftMargin(0.16);//to include both large/small font options

  // Fonts, sizes, offsets
  t2kStyle->SetTextFont(FontStyle);
  t2kStyle->SetTextSize(0.08);

  t2kStyle->SetLabelFont(FontStyle, "x");
  t2kStyle->SetLabelFont(FontStyle, "y");
  t2kStyle->SetLabelFont(FontStyle, "z");
  t2kStyle->SetLabelFont(FontStyle, "t");
  t2kStyle->SetLabelSize(FontSizeLabel, "x");
  t2kStyle->SetLabelSize(FontSizeLabel, "y");
  t2kStyle->SetLabelSize(FontSizeLabel, "z");
  t2kStyle->SetLabelOffset(0.015, "x");
  t2kStyle->SetLabelOffset(0.015, "y");
  t2kStyle->SetLabelOffset(0.015, "z");

  t2kStyle->SetTitleFont(FontStyle, "x");
  t2kStyle->SetTitleFont(FontStyle, "y");
  t2kStyle->SetTitleFont(FontStyle, "z");
  t2kStyle->SetTitleFont(FontStyle, "t");
  t2kStyle->SetTitleSize(FontSizeTitle, "y");
  t2kStyle->SetTitleSize(FontSizeTitle, "x");
  t2kStyle->SetTitleSize(FontSizeTitle, "z");
  t2kStyle->SetTitleOffset(1.14, "x");
  t2kStyle->SetTitleOffset(YOffsetTitle, "y");
  t2kStyle->SetTitleOffset(1.2, "z");

  t2kStyle->SetTitleStyle(0);
  t2kStyle->SetTitleFontSize(0.06);//0.08
  t2kStyle->SetTitleFont(FontStyle, "pad");
  t2kStyle->SetTitleBorderSize(0);
  t2kStyle->SetTitleX(0.1f);
  t2kStyle->SetTitleW(0.8f);

  // use bold lines and markers
  t2kStyle->SetMarkerStyle(25);
  t2kStyle->SetHistLineWidth( Width_t(2.5) );
  t2kStyle->SetLineStyleString(2, "[12 12]"); // postscript dashes

  // get rid of X error bars and y error bar caps
  t2kStyle->SetErrorX(0.001);

  // do not display any of the standard histogram decorations
  t2kStyle->SetOptTitle(0);
  t2kStyle->SetOptStat(0);
  t2kStyle->SetOptFit(1); // switch fitter pad on

  // put tick marks on top and RHS of plots
  t2kStyle->SetPadTickX(1);
  t2kStyle->SetPadTickY(1);

  // -- color --
  // functions blue
  t2kStyle->SetFuncColor(600-4);

  t2kStyle->SetFillColor(1); // make color fillings (not white)
  // - color setup for 2D -
  // - "cold"/ blue-ish -
  Double_t red[]   = { 0.00, 0.00, 0.00 };
  Double_t green[] = { 1.00, 0.00, 0.00 };
  Double_t blue[]  = { 1.00, 1.00, 0.25 };
  // - "warm" red-ish colors -
  //  Double_t red[]   = {1.00, 1.00, 0.25 };
  //  Double_t green[] = {1.00, 0.00, 0.00 };
  //  Double_t blue[]  = {0.00, 0.00, 0.00 };

  Double_t stops[] = { 0.25, 0.75, 1.00 };
  const Int_t NRGBs = 3;
  const Int_t NCont = 500;

  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  t2kStyle->SetNumberContours(NCont);

  // - Rainbow -
  //  t2kStyle->SetPalette(1);  // use the rainbow color set

  // Setup light blue - bright yellow pallete
  gStyle->SetPalette(kBird);

  // -- axis --
  t2kStyle->SetStripDecimals(kFALSE); // don't do 0.8 -> 1
  //  TGaxis::SetMaxDigits(3); // doesn't have an effect
  // no supressed zeroes!
  t2kStyle->SetHistMinimumZero(kTRUE);


 return(t2kStyle);
}