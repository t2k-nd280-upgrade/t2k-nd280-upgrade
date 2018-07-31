{
   cout << endl;
   cout << "--------------------------------------------------" << endl;
   cout << endl;

   // CORRECT ERRORS:  ERROR HAS TO BE ASYMMETRIC (N*Eff*(1-Eff))^(1/2)

   TCanvas *c1 = new TCanvas("c1","c1",1200,900);
   TCanvas *c2 = new TCanvas("c2","c2",1200,900);
   gStyle->SetOptStat(0000);
   gStyle->SetPadBorderMode(0);
   gStyle->SetPadLeftMargin(0.13);
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
  
   c1->Divide(3,2);
   c2->Divide(3,2);
//   c2->Divide(4,3);

   TFile f1("PiMinus_Nexus_Evt0_NEvt400000.root");
   NPiMMu0deg = new TH1D;
   NPiMMu0deg = hPiMu0deg;
   NPiMMu30deg = new TH1D;
   NPiMMu30deg = hPiMu30deg;
   NPiMMu60deg = new TH1D;
   NPiMMu60deg = hPiMu30deg;

   NPiMStop0deg = new TH1D;
   NPiMStop0deg = hPiStop0deg;
   NPiMStop30deg = new TH1D;
   NPiMStop30deg = hPiStop30deg;
   NPiMStop60deg = new TH1D;
   NPiMStop60deg = hPiStop60deg;

   TFile f2("PiPlus_Nexus_Evt0_NEvt400000.root");
   NPiPMu0deg = new TH1D;
   NPiPMu0deg = hPiMu0deg;
   NPiPMu30deg = new TH1D;
   NPiPMu30deg = hPiMu30deg;
   NPiPMu60deg = new TH1D;
   NPiPMu60deg = hPiMu30deg;

   NPiPStop0deg = new TH1D;
   NPiPStop0deg = hPiStop0deg;
   NPiPStop30deg = new TH1D;
   NPiPStop30deg = hPiStop30deg;
   NPiPStop60deg = new TH1D;
   NPiPStop60deg = hPiStop60deg;

   TFile f3("PiMinus_GF_Evt0_NEvt400000.root");
   GPiMMu0deg = new TH1D;
   GPiMMu0deg = hPiMu0deg;
   GPiMMu30deg = new TH1D;
   GPiMMu30deg = hPiMu30deg;
   GPiMMu60deg = new TH1D;
   GPiMMu60deg = hPiMu30deg;

   GPiMStop0deg = new TH1D;
   GPiMStop0deg = hPiStop0deg;
   GPiMStop30deg = new TH1D;
   GPiMStop30deg = hPiStop30deg;
   GPiMStop60deg = new TH1D;
   GPiMStop60deg = hPiStop60deg;

   TFile f4("PiPlus_GF_Evt0_NEvt400000.root");
   GPiPMu0deg = new TH1D;
   GPiPMu0deg = hPiMu0deg;
   GPiPMu30deg = new TH1D;
   GPiPMu30deg = hPiMu30deg;
   GPiPMu60deg = new TH1D;
   GPiPMu60deg = hPiMu30deg;

   GPiPStop0deg = new TH1D;
   GPiPStop0deg = hPiStop0deg;
   GPiPStop30deg = new TH1D;
   GPiPStop30deg = hPiStop30deg;
   GPiPStop60deg = new TH1D;
   GPiPStop60deg = hPiStop60deg;

   TFile f5("PiMinus_Solid_Evt0_NEvt400000.root");
   SPiMMu0deg = new TH1D;
   SPiMMu0deg = hPiMu0deg;
   SPiMMu30deg = new TH1D;
   SPiMMu30deg = hPiMu30deg;
   SPiMMu60deg = new TH1D;
   SPiMMu60deg = hPiMu30deg;

   SPiMStop0deg = new TH1D;
   SPiMStop0deg = hPiStop0deg;
   SPiMStop30deg = new TH1D;
   SPiMStop30deg = hPiStop30deg;
   SPiMStop60deg = new TH1D;
   SPiMStop60deg = hPiStop60deg;

   TFile f6("PiPlus_Solid_Evt0_NEvt400000.root");
   SPiPMu0deg = new TH1D;
   SPiPMu0deg = hPiMu0deg;
   SPiPMu30deg = new TH1D;
   SPiPMu30deg = hPiMu30deg;
   SPiPMu60deg = new TH1D;
   SPiPMu60deg = hPiMu30deg;

   SPiPStop0deg = new TH1D;
   SPiPStop0deg = hPiStop0deg;
   SPiPStop30deg = new TH1D;
   SPiPStop30deg = hPiStop30deg;
   SPiPStop60deg = new TH1D;
   SPiPStop60deg = hPiStop60deg;
   

   c1->cd(1);
   NPiMStop0deg->SetLineColor(kBlue);
   GPiMStop0deg->SetLineColor(kGreen);
   SPiMStop0deg->SetLineColor(kRed);
   NPiMStop0deg->Draw("HIST E1");
   GPiMStop0deg->Draw("HIST E1 SAME");
   SPiMStop0deg->Draw("HIST E1 SAME");
   TPaveText *t4 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t4->AddText("Pions Stopped 1 track at 0 deg");
   t4->SetTextFont(22);
   t4->SetTextSize(0.050);
   t4->SetFillColor(kWhite);
   t4->Draw();
   auto legend1 = new TLegend(0.70,0.75,0.92,0.9);
   legend1->AddEntry(NPiMStop0deg," Nexus","l");
   legend1->AddEntry(GPiMStop0deg," GFiber","l");
   legend1->AddEntry(SPiMStop0deg," Solid","l");
   legend1->SetTextSize(0.045);
   legend1->SetTextFont(22);
   legend1->Draw();

   c1->cd(2);
   NPiMStop30deg->SetLineColor(kBlue);
   GPiMStop30deg->SetLineColor(kGreen);
   SPiMStop30deg->SetLineColor(kRed);
   NPiMStop30deg->Draw("HIST E1");
   GPiMStop30deg->Draw("HIST E1 SAME");
   SPiMStop30deg->Draw("HIST E1 SAME");
   TPaveText *t5 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t5->AddText("Pions Stopped 1 track at 30 deg");
   t5->SetTextFont(22);
   t5->SetTextSize(0.050);
   t5->SetFillColor(kWhite);
   t5->Draw();
   legend1->Draw();

   c1->cd(3);
   NPiMStop60deg->SetLineColor(kBlue);
   GPiMStop60deg->SetLineColor(kGreen);
   SPiMStop60deg->SetLineColor(kRed);
   NPiMStop60deg->Draw("HIST E1");
   GPiMStop60deg->Draw("HIST E1 SAME");
   SPiMStop60deg->Draw("HIST E1 SAME");
   TPaveText *t6 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t6->AddText("Pions Stopped 1 track at 60 deg");
   t6->SetTextFont(22);
   t6->SetTextSize(0.050);
   t6->SetFillColor(kWhite);
   t6->Draw();
   legend1->Draw();

   c1->cd(4);
   NPiMMu0deg->SetLineColor(kBlue);
   GPiMMu0deg->SetLineColor(kGreen);
   SPiMMu0deg->SetLineColor(kRed);
   NPiMMu0deg->Draw("HIST E1");
   GPiMMu0deg->Draw("HIST E1 SAME");
   SPiMMu0deg->Draw("HIST E1 SAME");
   TPaveText *t1 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t1->AddText("Pions to Muons at 0 deg");
   t1->SetTextFont(22);
   t1->SetTextSize(0.050);
   t1->SetFillColor(kWhite);
   t1->Draw();
   legend1->Draw();

   c1->cd(5);
   NPiMMu30deg->SetLineColor(kBlue);
   GPiMMu30deg->SetLineColor(kGreen);
   SPiMMu30deg->SetLineColor(kRed);
   NPiMMu30deg->Draw("HIST E1");
   GPiMMu30deg->Draw("HIST E1 SAME");
   SPiMMu30deg->Draw("HIST E1 SAME");
   TPaveText *t2 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t2->AddText("Pions to Muons at 30 deg");
   t2->SetTextFont(22);
   t2->SetTextSize(0.050);
   t2->SetFillColor(kWhite);
   t2->Draw();
   legend1->Draw();

   c1->cd(6);
   NPiMMu60deg->SetLineColor(kBlue);
   GPiMMu60deg->SetLineColor(kGreen);
   SPiMMu60deg->SetLineColor(kRed);
   NPiMMu60deg->Draw("HIST E1");
   GPiMMu60deg->Draw("HIST E1 SAME");
   SPiMMu60deg->Draw("HIST E1 SAME");
   TPaveText *t3 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t3->AddText("Pions to Muons at 60 deg");
   t3->SetTextFont(22);
   t3->SetTextSize(0.050);
   t3->SetFillColor(kWhite);
   t3->Draw();
   legend1->Draw();

   /////////////////////////////////

   c2->cd(1);
   NPiPStop0deg->SetLineColor(kBlue);
   GPiPStop0deg->SetLineColor(kGreen);
   SPiPStop0deg->SetLineColor(kRed);
   NPiPStop0deg->Draw("HIST E1");
   GPiPStop0deg->Draw("HIST E1 SAME");
   SPiPStop0deg->Draw("HIST E1 SAME");
   TPaveText *t7 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t7->AddText("Pions Stopped 1 track at 0 deg");
   t7->SetTextFont(22);
   t7->SetTextSize(0.050);
   t7->SetFillColor(kWhite);
   t7->Draw();
   legend1->Draw();

   c2->cd(2);
   NPiPStop30deg->SetLineColor(kBlue);
   GPiPStop30deg->SetLineColor(kGreen);
   SPiPStop30deg->SetLineColor(kRed);
   NPiPStop30deg->Draw("HIST E1");
   GPiPStop30deg->Draw("HIST E1 SAME");
   SPiPStop30deg->Draw("HIST E1 SAME");
   TPaveText *t8 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t8->AddText("Pions Stopped 1 track at 30 deg");
   t8->SetTextFont(22);
   t8->SetTextSize(0.050);
   t8->SetFillColor(kWhite);
   t8->Draw();
   legend1->Draw();

   c2->cd(3);
   NPiPStop60deg->SetLineColor(kBlue);
   GPiPStop60deg->SetLineColor(kGreen);
   SPiPStop60deg->SetLineColor(kRed);
   NPiPStop60deg->Draw("HIST E1");
   GPiPStop60deg->Draw("HIST E1 SAME");
   SPiPStop60deg->Draw("HIST E1 SAME");
   TPaveText *t9 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t9->AddText("Pions Stopped 1 track at 60 deg");
   t9->SetTextFont(22);
   t9->SetTextSize(0.050);
   t9->SetFillColor(kWhite);
   t9->Draw();
   legend1->Draw();

   c2->cd(4);
   NPiPMu0deg->SetLineColor(kBlue);
   GPiPMu0deg->SetLineColor(kGreen);
   SPiPMu0deg->SetLineColor(kRed);
   NPiPMu0deg->Draw("HIST E1");
   GPiPMu0deg->Draw("HIST E1 SAME");
   SPiPMu0deg->Draw("HIST E1 SAME");
   TPaveText *t10 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t10->AddText("Pions to Muons at 0 deg");
   t10->SetTextFont(22);
   t10->SetTextSize(0.050);
   t10->SetFillColor(kWhite);
   t10->Draw();
   legend1->Draw();

   c2->cd(5);
   NPiPMu30deg->SetLineColor(kBlue);
   GPiPMu30deg->SetLineColor(kGreen);
   SPiPMu30deg->SetLineColor(kRed);
   NPiPMu30deg->Draw("HIST E1");
   GPiPMu30deg->Draw("HIST E1 SAME");
   SPiPMu30deg->Draw("HIST E1 SAME");
   TPaveText *t11 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t11->AddText("Pions to Muons at 30 deg");
   t11->SetTextFont(22);
   t11->SetTextSize(0.050);
   t11->SetFillColor(kWhite);
   t11->Draw();
   legend1->Draw();

   c2->cd(6);
   NPiPMu60deg->SetLineColor(kBlue);
   GPiPMu60deg->SetLineColor(kGreen);
   SPiPMu60deg->SetLineColor(kRed);
   NPiPMu60deg->Draw("HIST E1");
   GPiPMu60deg->Draw("HIST E1 SAME");
   SPiPMu60deg->Draw("HIST E1 SAME");
   TPaveText *t12 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t12->AddText("Pions to Muons at 60 deg");
   t12->SetTextFont(22);
   t12->SetTextSize(0.050);
   t12->SetFillColor(kWhite);
   t12->Draw();
   legend1->Draw();

   cout << endl;
   cout << "--------------------------------------------------" << endl;
   cout << endl;

c1->Print("~/Desktop/FCStudies/Plots/PiMinus.pdf");
c2->Print("~/Desktop/FCStudies/Plots/PiPlus.pdf");

}
