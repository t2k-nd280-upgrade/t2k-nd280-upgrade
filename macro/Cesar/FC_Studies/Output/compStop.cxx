{
   cout << endl;
   cout << "--------------------------------------------------" << endl;
   cout << endl;

   // CORRECT ERRORS:  ERROR HAS TO BE ASYMMETRIC (N*Eff*(1-Eff))^(1/2)

   TCanvas *c1 = new TCanvas("c1","c1",1200,900);
//   TCanvas *c2 = new TCanvas("c2","c2",1200,900);
   gStyle->SetOptStat(0000);
   gStyle->SetPadBorderMode(0);
   gStyle->SetPadLeftMargin(0.14);
   gStyle->SetPadRightMargin(0.03);
   gStyle->SetPadTopMargin(0.06);
   gStyle->SetPadBottomMargin(0.14);
   gStyle->SetTitleX(0.57f);
   gStyle->SetTitleW(0.8f);
   gStyle->SetTextFont(2);
   gStyle->SetLabelFont(22,"xyz");
   gStyle->SetTitleFont(22,"xyz");
   gStyle->SetTitleFont(22);
   gStyle->SetOptTitle(0);
   gStyle->SetErrorX(0.);
  
   c1->Divide(4,3);
 //  c2->Divide(4,3);

   TFile f1("AnalysisOutput_Nexus_Evt0_NEvt400000.root");
   
   NStopGrEl0 = new TGraphAsymmErrors;
   NStopGrEl0 =grStopEl0;
   NStopHEl0 = new TH1F;
   NStopHEl0 = hStopEl0;
   NStopGrEl30 = new TGraphAsymmErrors;
   NStopGrEl30 =grStopEl30;
   NStopHEl30 = new TH1F;
   NStopHEl30 = hStopEl30;
   NStopGrEl60 = new TGraphAsymmErrors;
   NStopGrEl60 =grStopEl60;
   NStopHEl60 = new TH1F;
   NStopHEl60 = hStopEl60;

   NStopGrMu0 = new TGraphAsymmErrors;
   NStopGrMu0 =grStopMu0;
   NStopHMu0 = new TH1F;
   NStopHMu0 = hStopMu0;
   NStopGrMu30 = new TGraphAsymmErrors;
   NStopGrMu30 =grStopMu30;
   NStopHMu30 = new TH1F;
   NStopHMu30 = hStopMu30;
   NStopGrMu60 = new TGraphAsymmErrors;
   NStopGrMu60 =grStopMu60;
   NStopHMu60 = new TH1F;
   NStopHMu60 = hStopMu60;

   NStopGrPi0 = new TGraphAsymmErrors;
   NStopGrPi0 =grStopPi0;
   NStopHPi0 = new TH1F;
   NStopHPi0 = hStopPi0;
   NStopGrPi30 = new TGraphAsymmErrors;
   NStopGrPi30 =grStopPi30;
   NStopHPi30 = new TH1F;
   NStopHPi30 = hStopPi30;
   NStopGrPi60 = new TGraphAsymmErrors;
   NStopGrPi60 =grStopPi60;
   NStopHPi60 = new TH1F;
   NStopHPi60 = hStopPi60;

   NStopGrPr0 = new TGraphAsymmErrors;
   NStopGrPr0 =grStopPr0;
   NStopHPr0 = new TH1F;
   NStopHPr0 = hStopPr0;
   NStopGrPr30 = new TGraphAsymmErrors;
   NStopGrPr30 =grStopPr30;
   NStopHPr30 = new TH1F;
   NStopHPr30 = hStopPr30;
   NStopGrPr60 = new TGraphAsymmErrors;
   NStopGrPr60 =grStopPr60;
   NStopHPr60 = new TH1F;
   NStopHPr60 = hStopPr60;

   TFile f2("AnalysisOutput_GF_Evt0_NEvt400000.root");

   GStopGrEl0 = new TGraphAsymmErrors;
   GStopGrEl0 =grStopEl0;
   GStopHEl0 = new TH1F;
   GStopHEl0 = hStopEl0;
   GStopGrEl30 = new TGraphAsymmErrors;
   GStopGrEl30 =grStopEl30;
   GStopHEl30 = new TH1F;
   GStopHEl30 = hStopEl30;
   GStopGrEl60 = new TGraphAsymmErrors;
   GStopGrEl60 =grStopEl60;
   GStopHEl60 = new TH1F;
   GStopHEl60 = hStopEl60;

   GStopGrMu0 = new TGraphAsymmErrors;
   GStopGrMu0 =grStopMu0;
   GStopHMu0 = new TH1F;
   GStopHMu0 = hStopMu0;
   GStopGrMu30 = new TGraphAsymmErrors;
   GStopGrMu30 =grStopMu30;
   GStopHMu30 = new TH1F;
   GStopHMu30 = hStopMu30;
   GStopGrMu60 = new TGraphAsymmErrors;
   GStopGrMu60 =grStopMu60;
   GStopHMu60 = new TH1F;
   GStopHMu60 = hStopMu60;

   GStopGrPi0 = new TGraphAsymmErrors;
   GStopGrPi0 =grStopPi0;
   GStopHPi0 = new TH1F;
   GStopHPi0 = hStopPi0;
   GStopGrPi30 = new TGraphAsymmErrors;
   GStopGrPi30 =grStopPi30;
   GStopHPi30 = new TH1F;
   GStopHPi30 = hStopPi30;
   GStopGrPi60 = new TGraphAsymmErrors;
   GStopGrPi60 =grStopPi60;
   GStopHPi60 = new TH1F;
   GStopHPi60 = hStopPi60;

   GStopGrPr0 = new TGraphAsymmErrors;
   GStopGrPr0 =grStopPr0;
   GStopHPr0 = new TH1F;
   GStopHPr0 = hStopPr0;
   GStopGrPr30 = new TGraphAsymmErrors;
   GStopGrPr30 =grStopPr30;
   GStopHPr30 = new TH1F;
   GStopHPr30 = hStopPr30;
   GStopGrPr60 = new TGraphAsymmErrors;
   GStopGrPr60 =grStopPr60;
   GStopHPr60 = new TH1F;
   GStopHPr60 = hStopPr60;

   TFile f3("AnalysisOutput_Solid_Evt0_NEvt400000.root");

   SStopGrEl0 = new TGraphAsymmErrors;
   SStopGrEl0 =grStopEl0;
   SStopHEl0 = new TH1F;
   SStopHEl0 = hStopEl0;
   SStopGrEl30 = new TGraphAsymmErrors;
   SStopGrEl30 =grStopEl30;
   SStopHEl30 = new TH1F;
   SStopHEl30 = hStopEl30;
   SStopGrEl60 = new TGraphAsymmErrors;
   SStopGrEl60 =grStopEl60;
   SStopHEl60 = new TH1F;
   SStopHEl60 = hStopEl60;

   SStopGrMu0 = new TGraphAsymmErrors;
   SStopGrMu0 =grStopMu0;
   SStopHMu0 = new TH1F;
   SStopHMu0 = hStopMu0;
   SStopGrMu30 = new TGraphAsymmErrors;
   SStopGrMu30 =grStopMu30;
   SStopHMu30 = new TH1F;
   SStopHMu30 = hStopMu30;
   SStopGrMu60 = new TGraphAsymmErrors;
   SStopGrMu60 =grStopMu60;
   SStopHMu60 = new TH1F;
   SStopHMu60 = hStopMu60;

   SStopGrPi0 = new TGraphAsymmErrors;
   SStopGrPi0 =grStopPi0;
   SStopHPi0 = new TH1F;
   SStopHPi0 = hStopPi0;
   SStopGrPi30 = new TGraphAsymmErrors;
   SStopGrPi30 =grStopPi30;
   SStopHPi30 = new TH1F;
   SStopHPi30 = hStopPi30;
   SStopGrPi60 = new TGraphAsymmErrors;
   SStopGrPi60 =grStopPi60;
   SStopHPi60 = new TH1F;
   SStopHPi60 = hStopPi60;

   SStopGrPr0 = new TGraphAsymmErrors;
   SStopGrPr0 =grStopPr0;
   SStopHPr0 = new TH1F;
   SStopHPr0 = hStopPr0;
   SStopGrPr30 = new TGraphAsymmErrors;
   SStopGrPr30 =grStopPr30;
   SStopHPr30 = new TH1F;
   SStopHPr30 = hStopPr30;
   SStopGrPr60 = new TGraphAsymmErrors;
   SStopGrPr60 =grStopPr60;
   SStopHPr60 = new TH1F;
   SStopHPr60 = hStopPr60;

   auto legend1 = new TLegend(0.70,0.20,0.92,0.35);
   legend1->AddEntry(NStopHEl0," Nexus","l");
   legend1->AddEntry(GStopHEl0," GFiber","l");
   legend1->AddEntry(SStopHEl0," Solid","l");
   legend1->SetTextSize(0.045);
   legend1->SetTextFont(22);

   c1->cd(1);
   NStopHEl0->SetLineColor(kBlue);
   NStopGrEl0->SetLineColor(kBlue);
   GStopHEl0->SetLineColor(kGreen);
   GStopGrEl0->SetLineColor(kGreen);
   SStopHEl0->SetLineColor(kRed);
   SStopGrEl0->SetLineColor(kRed);

   NStopHEl0->Draw("HIST");
   NStopGrEl0->Draw("P SAME");
   GStopHEl0->Draw("HIST SAME");
   GStopGrEl0->Draw("P SAME");
   SStopHEl0->Draw("HIST SAME");
   SStopGrEl0->Draw("P SAME");

   TPaveText *t1 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t1->AddText("Electrons at 0 deg");
   t1->SetTextFont(22);
   t1->SetTextSize(0.050);
   t1->SetFillColor(kWhite);
   t1->Draw();

   legend1->Draw();

   c1->cd(2);
   NStopHMu0->SetLineColor(kBlue);
   NStopGrMu0->SetLineColor(kBlue);
   GStopHMu0->SetLineColor(kGreen);
   GStopGrMu0->SetLineColor(kGreen);
   SStopHMu0->SetLineColor(kRed);
   SStopGrMu0->SetLineColor(kRed);

   NStopHMu0->Draw("HIST");
   NStopGrMu0->Draw("P SAME");
   GStopHMu0->Draw("HIST SAME");
   GStopGrMu0->Draw("P SAME");
   SStopHMu0->Draw("HIST SAME");
   SStopGrMu0->Draw("P SAME");

   TPaveText *t2 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t2->AddText("Muons at 0 deg");
   t2->SetTextFont(22);
   t2->SetTextSize(0.050);
   t2->SetFillColor(kWhite);
   t2->Draw();

   legend1->Draw();

   c1->cd(3);
   NStopHPi0->SetLineColor(kBlue);
   NStopGrPi0->SetLineColor(kBlue);
   GStopHPi0->SetLineColor(kGreen);
   GStopGrPi0->SetLineColor(kGreen);
   SStopHPi0->SetLineColor(kRed);
   SStopGrPi0->SetLineColor(kRed);

   NStopHPi0->Draw("HIST");
   NStopGrPi0->Draw("P SAME");
   GStopHPi0->Draw("HIST SAME");
   GStopGrPi0->Draw("P SAME");
   SStopHPi0->Draw("HIST SAME");
   SStopGrPi0->Draw("P SAME");

   TPaveText *t3 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t3->AddText("Pions at 0 deg");
   t3->SetTextFont(22);
   t3->SetTextSize(0.050);
   t3->SetFillColor(kWhite);
   t3->Draw();

   legend1->Draw();


   c1->cd(4);
   NStopHPr0->SetLineColor(kBlue);
   NStopGrPr0->SetLineColor(kBlue);
   GStopHPr0->SetLineColor(kGreen);
   GStopGrPr0->SetLineColor(kGreen);
   SStopHPr0->SetLineColor(kRed);
   SStopGrPr0->SetLineColor(kRed);

   NStopHPr0->Draw("HIST");
   NStopGrPr0->Draw("P SAME");
   GStopHPr0->Draw("HIST SAME");
   GStopGrPr0->Draw("P SAME");
   SStopHPr0->Draw("HIST SAME");
   SStopGrPr0->Draw("P SAME");

   TPaveText *t4 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t4->AddText("Protons at 0 deg");
   t4->SetTextFont(22);
   t4->SetTextSize(0.050);
   t4->SetFillColor(kWhite);
   t4->Draw();

   legend1->Draw();

   c1->cd(5);
   NStopHEl30->SetLineColor(kBlue);
   NStopGrEl30->SetLineColor(kBlue);
   GStopHEl30->SetLineColor(kGreen);
   GStopGrEl30->SetLineColor(kGreen);
   SStopHEl30->SetLineColor(kRed);
   SStopGrEl30->SetLineColor(kRed);

   NStopHEl30->Draw("HIST");
   NStopGrEl30->Draw("P SAME");
   GStopHEl30->Draw("HIST SAME");
   GStopGrEl30->Draw("P SAME");
   SStopHEl30->Draw("HIST SAME");
   SStopGrEl30->Draw("P SAME");

   TPaveText *t5 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t5->AddText("Electrons at 30 deg");
   t5->SetTextFont(22);
   t5->SetTextSize(0.050);
   t5->SetFillColor(kWhite);
   t5->Draw();

   legend1->Draw();

   c1->cd(6);
   NStopHMu30->SetLineColor(kBlue);
   NStopGrMu30->SetLineColor(kBlue);
   GStopHMu30->SetLineColor(kGreen);
   GStopGrMu30->SetLineColor(kGreen);
   SStopHMu30->SetLineColor(kRed);
   SStopGrMu30->SetLineColor(kRed);

   NStopHMu30->Draw("HIST");
   NStopGrMu30->Draw("P SAME");
   GStopHMu30->Draw("HIST SAME");
   GStopGrMu30->Draw("P SAME");
   SStopHMu30->Draw("HIST SAME");
   SStopGrMu30->Draw("P SAME");

   TPaveText *t6 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t6->AddText("Muons at 30 deg");
   t6->SetTextFont(22);
   t6->SetTextSize(0.050);
   t6->SetFillColor(kWhite);
   t6->Draw();

   legend1->Draw();

   c1->cd(7);
   NStopHPi30->SetLineColor(kBlue);
   NStopGrPi30->SetLineColor(kBlue);
   GStopHPi30->SetLineColor(kGreen);
   GStopGrPi30->SetLineColor(kGreen);
   SStopHPi30->SetLineColor(kRed);
   SStopGrPi30->SetLineColor(kRed);

   NStopHPi30->Draw("HIST");
   NStopGrPi30->Draw("P SAME");
   GStopHPi30->Draw("HIST SAME");
   GStopGrPi30->Draw("P SAME");
   SStopHPi30->Draw("HIST SAME");
   SStopGrPi30->Draw("P SAME");

   TPaveText *t7 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t7->AddText("Pions at 30 deg");
   t7->SetTextFont(22);
   t7->SetTextSize(0.050);
   t7->SetFillColor(kWhite);
   t7->Draw();

   legend1->Draw();


   c1->cd(8);
   NStopHPr30->SetLineColor(kBlue);
   NStopGrPr30->SetLineColor(kBlue);
   GStopHPr30->SetLineColor(kGreen);
   GStopGrPr30->SetLineColor(kGreen);
   SStopHPr30->SetLineColor(kRed);
   SStopGrPr30->SetLineColor(kRed);

   NStopHPr30->Draw("HIST");
   NStopGrPr30->Draw("P SAME");
   GStopHPr30->Draw("HIST SAME");
   GStopGrPr30->Draw("P SAME");
   SStopHPr30->Draw("HIST SAME");
   SStopGrPr30->Draw("P SAME");

   TPaveText *t8 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t8->AddText("Protons at 30 deg");
   t8->SetTextFont(22);
   t8->SetTextSize(0.050);
   t8->SetFillColor(kWhite);
   t8->Draw();

   legend1->Draw();

   c1->cd(9);
   NStopHEl60->SetLineColor(kBlue);
   NStopGrEl60->SetLineColor(kBlue);
   GStopHEl60->SetLineColor(kGreen);
   GStopGrEl60->SetLineColor(kGreen);
   SStopHEl60->SetLineColor(kRed);
   SStopGrEl60->SetLineColor(kRed);

   NStopHEl60->Draw("HIST");
   NStopGrEl60->Draw("P SAME");
   GStopHEl60->Draw("HIST SAME");
   GStopGrEl60->Draw("P SAME");
   SStopHEl60->Draw("HIST SAME");
   SStopGrEl60->Draw("P SAME");

   TPaveText *t9 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t9->AddText("Electrons at 60 deg");
   t9->SetTextFont(22);
   t9->SetTextSize(0.050);
   t9->SetFillColor(kWhite);
   t9->Draw();

   legend1->Draw();

   c1->cd(10);
   NStopHMu60->SetLineColor(kBlue);
   NStopGrMu60->SetLineColor(kBlue);
   GStopHMu60->SetLineColor(kGreen);
   GStopGrMu60->SetLineColor(kGreen);
   SStopHMu60->SetLineColor(kRed);
   SStopGrMu60->SetLineColor(kRed);

   NStopHMu60->Draw("HIST");
   NStopGrMu60->Draw("P SAME");
   GStopHMu60->Draw("HIST SAME");
   GStopGrMu60->Draw("P SAME");
   SStopHMu60->Draw("HIST SAME");
   SStopGrMu60->Draw("P SAME");

   TPaveText *t10 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t10->AddText("Muons at 60 deg");
   t10->SetTextFont(22);
   t10->SetTextSize(0.050);
   t10->SetFillColor(kWhite);
   t10->Draw();

   legend1->Draw();

   c1->cd(11);
   NStopHPi60->SetLineColor(kBlue);
   NStopGrPi60->SetLineColor(kBlue);
   GStopHPi60->SetLineColor(kGreen);
   GStopGrPi60->SetLineColor(kGreen);
   SStopHPi60->SetLineColor(kRed);
   SStopGrPi60->SetLineColor(kRed);

   NStopHPi60->Draw("HIST");
   NStopGrPi60->Draw("P SAME");
   GStopHPi60->Draw("HIST SAME");
   GStopGrPi60->Draw("P SAME");
   SStopHPi60->Draw("HIST SAME");
   SStopGrPi60->Draw("P SAME");

   TPaveText *t11 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t11->AddText("Pions at 60 deg");
   t11->SetTextFont(22);
   t11->SetTextSize(0.050);
   t11->SetFillColor(kWhite);
   t11->Draw();

   legend1->Draw();


   c1->cd(12);
   NStopHPr60->SetLineColor(kBlue);
   NStopGrPr60->SetLineColor(kBlue);
   GStopHPr60->SetLineColor(kGreen);
   GStopGrPr60->SetLineColor(kGreen);
   SStopHPr60->SetLineColor(kRed);
   SStopGrPr60->SetLineColor(kRed);

   NStopHPr60->Draw("HIST");
   NStopGrPr60->Draw("P SAME");
   GStopHPr60->Draw("HIST SAME");
   GStopGrPr60->Draw("P SAME");
   SStopHPr60->Draw("HIST SAME");
   SStopGrPr60->Draw("P SAME");

   TPaveText *t12 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t12->AddText("Protons at 60 deg");
   t12->SetTextFont(22);
   t12->SetTextSize(0.050);
   t12->SetFillColor(kWhite);
   t12->Draw();

   legend1->Draw();

   cout << endl;
   cout << "--------------------------------------------------" << endl;
   cout << endl;

c1->Print("~/Desktop/FCStudies/Plots/NewProfilesStop.pdf");

}
