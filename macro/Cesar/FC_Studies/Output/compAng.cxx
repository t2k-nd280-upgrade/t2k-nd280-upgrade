{
   cout << endl;
   cout << "--------------------------------------------------" << endl;
   cout << endl;

   // CORRECT ERRORS:  ERROR HAS TO BE ASYMMETRIC (N*Eff*(1-Eff))^(1/2)

   TCanvas *c1 = new TCanvas("c1","c1",1000,1000);
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
  
   c1->Divide(4,3);
   c2->Divide(4,3);

   TFile f1("AnalysisOutput_Nexus_Evt0_NEvt400000.root");
   NAngEl3DProj = new TProfile2D;
   NAngEl3DProj = hAngEl3D_pxy;
   NAngMu3DProj = new TProfile2D;
   NAngMu3DProj = hAngMu3D_pxy;
   NAngPi3DProj = new TProfile2D;
   NAngPi3DProj = hAngPi3D_pxy;
   NAngPr3DProj = new TProfile2D;
   NAngPr3DProj = hAngPr3D_pxy;

   NAngEl0Proj = new TProfile;
   NAngEl0Proj = hAngEl0Proj;
   NAngMu0Proj = new TProfile;
   NAngMu0Proj = hAngMu0Proj;
   NAngPi0Proj = new TProfile;
   NAngPi0Proj = hAngPi0Proj;
   NAngPr0Proj = new TProfile;
   NAngPr0Proj = hAngPr0Proj;

   NAngEl30Proj = new TProfile;
   NAngEl30Proj = hAngEl30Proj;
   NAngMu30Proj = new TProfile;
   NAngMu30Proj = hAngMu30Proj;
   NAngPi30Proj = new TProfile;
   NAngPi30Proj = hAngPi30Proj;
   NAngPr30Proj = new TProfile;
   NAngPr30Proj = hAngPr30Proj;

   NAngEl60Proj = new TProfile;
   NAngEl60Proj = hAngEl60Proj;
   NAngMu60Proj = new TProfile;
   NAngMu60Proj = hAngMu60Proj;
   NAngPi60Proj = new TProfile;
   NAngPi60Proj = hAngPi60Proj;
   NAngPr60Proj = new TProfile;
   NAngPr60Proj = hAngPr60Proj;

   TFile f2("AnalysisOutput_GF_Evt0_NEvt400000.root");
   GAngEl3DProj = new TProfile2D;
   GAngEl3DProj = hAngEl3D_pxy;
   GAngMu3DProj = new TProfile2D;
   GAngMu3DProj = hAngMu3D_pxy;
   GAngPi3DProj = new TProfile2D;
   GAngPi3DProj = hAngPi3D_pxy;
   GAngPr3DProj = new TProfile2D;
   GAngPr3DProj = hAngPr3D_pxy;

   GAngEl0Proj = new TProfile;
   GAngEl0Proj = hAngEl0Proj;
   GAngMu0Proj = new TProfile;
   GAngMu0Proj = hAngMu0Proj;
   GAngPi0Proj = new TProfile;
   GAngPi0Proj = hAngPi0Proj;
   GAngPr0Proj = new TProfile;
   GAngPr0Proj = hAngPr0Proj;

   GAngEl30Proj = new TProfile;
   GAngEl30Proj = hAngEl30Proj;
   GAngMu30Proj = new TProfile;
   GAngMu30Proj = hAngMu30Proj;
   GAngPi30Proj = new TProfile;
   GAngPi30Proj = hAngPi30Proj;
   GAngPr30Proj = new TProfile;
   GAngPr30Proj = hAngPr30Proj;

   GAngEl60Proj = new TProfile;
   GAngEl60Proj = hAngEl60Proj;
   GAngMu60Proj = new TProfile;
   GAngMu60Proj = hAngMu60Proj;
   GAngPi60Proj = new TProfile;
   GAngPi60Proj = hAngPi60Proj;
   GAngPr60Proj = new TProfile;
   GAngPr60Proj = hAngPr60Proj;

   TFile f3("AnalysisOutput_Solid_Evt0_NEvt400000.root");
   SAngEl3DProj = new TProfile2D;
   SAngEl3DProj = hAngEl3D_pxy;
   SAngMu3DProj = new TProfile2D;
   SAngMu3DProj = hAngMu3D_pxy;
   SAngPi3DProj = new TProfile2D;
   SAngPi3DProj = hAngPi3D_pxy;
   SAngPr3DProj = new TProfile2D;
   SAngPr3DProj = hAngPr3D_pxy;

   SAngEl0Proj = new TProfile;
   SAngEl0Proj = hAngEl0Proj;
   SAngMu0Proj = new TProfile;
   SAngMu0Proj = hAngMu0Proj;
   SAngPi0Proj = new TProfile;
   SAngPi0Proj = hAngPi0Proj;
   SAngPr0Proj = new TProfile;
   SAngPr0Proj = hAngPr0Proj;

   SAngEl30Proj = new TProfile;
   SAngEl30Proj = hAngEl30Proj;
   SAngMu30Proj = new TProfile;
   SAngMu30Proj = hAngMu30Proj;
   SAngPi30Proj = new TProfile;
   SAngPi30Proj = hAngPi30Proj;
   SAngPr30Proj = new TProfile;
   SAngPr30Proj = hAngPr30Proj;

   SAngEl60Proj = new TProfile;
   SAngEl60Proj = hAngEl60Proj;
   SAngMu60Proj = new TProfile;
   SAngMu60Proj = hAngMu60Proj;
   SAngPi60Proj = new TProfile;
   SAngPi60Proj = hAngPi60Proj;
   SAngPr60Proj = new TProfile;
   SAngPr60Proj = hAngPr60Proj;

   c1->cd(1);
   NAngEl3DProj->Draw("COLZ");
   c1->cd(2);
   NAngMu3DProj->Draw("COLZ");
   c1->cd(3);
   NAngPi3DProj->Draw("COLZ");
   c1->cd(4);
   NAngPr3DProj->Draw("COLZ");
   c1->cd(5);
   GAngMu3DProj->Draw("COLZ");
   c1->cd(6);
   GAngMu3DProj->Draw("COLZ");
   c1->cd(7);
   GAngPi3DProj->Draw("COLZ");
   c1->cd(8);
   GAngPr3DProj->Draw("COLZ");
   c1->cd(9);
   SAngEl3DProj->Draw("COLZ");
   c1->cd(10);
   SAngMu3DProj->Draw("COLZ");
   c1->cd(11);
   SAngPi3DProj->Draw("COLZ");
   c1->cd(12);
   SAngPr3DProj->Draw("COLZ");
   

   c2->cd(1);
   NAngEl0Proj->SetLineColor(kBlue);
   GAngEl0Proj->SetLineColor(kGreen);
   SAngEl0Proj->SetLineColor(kRed);
   SAngEl0Proj->Draw("HIST E1");
   NAngEl0Proj->Draw("HIST E1 SAME");
   GAngEl0Proj->Draw("HIST E1 SAME");
   TPaveText *t1 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t1->AddText("Electrons at 0 deg");
   t1->SetTextFont(22);
   t1->SetTextSize(0.050);
   t1->SetFillColor(kWhite);
   t1->Draw();
   auto legend1 = new TLegend(0.70,0.75,0.92,0.9);
   legend1->AddEntry(NAngEl0Proj," Nexus","l");
   legend1->AddEntry(GAngEl0Proj," GFiber","l");
   legend1->AddEntry(SAngEl0Proj," Solid","l");
   legend1->SetTextSize(0.045);
   legend1->SetTextFont(22);
   legend1->Draw();

   c2->cd(2);
   NAngMu0Proj->SetLineColor(kBlue);
   GAngMu0Proj->SetLineColor(kGreen);
   SAngMu0Proj->SetLineColor(kRed);
   GAngMu0Proj->Draw("HIST E1");
   NAngMu0Proj->Draw("HIST E1 SAME");
   SAngMu0Proj->Draw("HIST E1 SAME");
   TPaveText *t2 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t2->AddText("Muons at 0 deg");
   t2->SetTextFont(22);
   t2->SetTextSize(0.050);
   t2->SetFillColor(kWhite);
   t2->Draw();
   auto legend2 = new TLegend(0.70,0.75,0.92,0.9);
   legend2->AddEntry(NAngMu0Proj," Nexus","l");
   legend2->AddEntry(GAngMu0Proj," GFiber","l");
   legend2->AddEntry(SAngMu0Proj," Solid","l");
   legend2->SetTextSize(0.045);
   legend2->SetTextFont(22);
   legend2->Draw();

   c2->cd(3);
   GAngPi0Proj->SetLineColor(kBlue);
   NAngPi0Proj->SetLineColor(kGreen);
   SAngPi0Proj->SetLineColor(kRed);
   NAngPi0Proj->Draw("HIST E1");
   GAngPi0Proj->Draw("HIST E1 SAME");
   SAngPi0Proj->Draw("HIST E1 SAME");
   TPaveText *t3 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t3->AddText("Pions at 0 deg");
   t3->SetTextFont(22);
   t3->SetTextSize(0.050);
   t3->SetFillColor(kWhite);
   t3->Draw();
   auto legend3 = new TLegend(0.70,0.75,0.92,0.9);
   legend3->AddEntry(NAngPi0Proj," Nexus","l");
   legend3->AddEntry(GAngPi0Proj," GFiber","l");
   legend3->AddEntry(SAngPi0Proj," Solid","l");
   legend3->SetTextSize(0.045);
   legend3->SetTextFont(22);
   legend3->Draw();

   c2->cd(4);
   NAngPr0Proj->SetLineColor(kBlue);
   GAngPr0Proj->SetLineColor(kGreen);
   SAngPr0Proj->SetLineColor(kRed);
   GAngPr0Proj->Draw("HIST E1");
   NAngPr0Proj->Draw("HIST E1 SAME");
   SAngPr0Proj->Draw("HIST E1 SAME");
   TPaveText *t4 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t4->AddText("Protons at 0 deg");
   t4->SetTextFont(22);
   t4->SetTextSize(0.050);
   t4->SetFillColor(kWhite);
   t4->Draw();
   auto legend4 = new TLegend(0.70,0.75,0.92,0.9);
   legend4->AddEntry(NAngPr0Proj," Nexus","l");
   legend4->AddEntry(GAngPr0Proj," GFiber","l");
   legend4->AddEntry(SAngPr0Proj," Solid","l");
   legend4->SetTextSize(0.045);
   legend4->SetTextFont(22);
   legend4->Draw();

   c2->cd(5);
   NAngEl30Proj->SetLineColor(kBlue);
   GAngEl30Proj->SetLineColor(kGreen);
   SAngEl30Proj->SetLineColor(kRed);
   GAngEl30Proj->Draw("HIST E1");
   NAngEl30Proj->Draw("HIST E1 SAME");
   SAngEl30Proj->Draw("HIST E1 SAME");
   TPaveText *t5 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t5->AddText("Electrons at 30 deg");
   t5->SetTextFont(22);
   t5->SetTextSize(0.050);
   t5->SetFillColor(kWhite);
   t5->Draw();
   auto legend5 = new TLegend(0.70,0.75,0.92,0.9);
   legend5->AddEntry(NAngEl30Proj," Nexus","l");
   legend5->AddEntry(GAngEl30Proj," GFiber","l");
   legend5->AddEntry(SAngEl30Proj," Solid","l");
   legend5->SetTextSize(0.045);
   legend5->SetTextFont(22);
   legend5->Draw();

   c2->cd(6);
   NAngMu30Proj->SetLineColor(kBlue);
   GAngMu30Proj->SetLineColor(kGreen);
   SAngMu30Proj->SetLineColor(kRed);
   SAngMu30Proj->Draw("HIST E1");
   GAngMu30Proj->Draw("HIST E1 SAME");
   NAngMu30Proj->Draw("HIST E1 SAME");
   TPaveText *t6 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t6->AddText("Muons at 30 deg");
   t6->SetTextFont(22);
   t6->SetTextSize(0.050);
   t6->SetFillColor(kWhite);
   t6->Draw();
   auto legend6 = new TLegend(0.70,0.75,0.92,0.9);
   legend6->AddEntry(NAngMu30Proj," Nexus","l");
   legend6->AddEntry(GAngMu30Proj," GFiber","l");
   legend6->AddEntry(SAngMu30Proj," Solid","l");
   legend6->SetTextSize(0.045);
   legend6->SetTextFont(22);
   legend6->Draw();

   c2->cd(7);
   NAngPi30Proj->SetLineColor(kBlue);
   GAngPi30Proj->SetLineColor(kGreen);
   SAngPi30Proj->SetLineColor(kRed);
   GAngPi30Proj->Draw("HIST E1");
   NAngPi30Proj->Draw("HIST E1 SAME");
   SAngPi30Proj->Draw("HIST E1 SAME");
   TPaveText *t7 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t7->AddText("Pions at 30 deg");
   t7->SetTextFont(22);
   t7->SetTextSize(0.050);
   t7->SetFillColor(kWhite);
   t7->Draw();
   auto legend7 = new TLegend(0.70,0.75,0.92,0.9);
   legend7->AddEntry(NAngMu30Proj," Nexus","l");
   legend7->AddEntry(GAngMu30Proj," GFiber","l");
   legend7->AddEntry(SAngMu30Proj," Solid","l");
   legend7->SetTextSize(0.045);
   legend7->SetTextFont(22);
   legend7->Draw();

   c2->cd(8);
   NAngPr30Proj->SetLineColor(kBlue);
   GAngPr30Proj->SetLineColor(kGreen);
   SAngPr30Proj->SetLineColor(kRed);
   GAngPr30Proj->Draw("HIST E1");
   NAngPr30Proj->Draw("HIST E1 SAME");
   SAngPr30Proj->Draw("HIST E1 SAME");
   TPaveText *t8 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t8->AddText("Protons at 30 deg");
   t8->SetTextFont(22);
   t8->SetTextSize(0.050);
   t8->SetFillColor(kWhite);
   t8->Draw();
   auto legend8 = new TLegend(0.70,0.75,0.92,0.9);
   legend8->AddEntry(NAngPr30Proj," Nexus","l");
   legend8->AddEntry(GAngPr30Proj," GFiber","l");
   legend8->AddEntry(SAngPr30Proj," Solid","l");
   legend8->SetTextSize(0.045);
   legend8->SetTextFont(22);
   legend8->Draw();

    c2->cd(9);
   NAngEl60Proj->SetLineColor(kBlue);
   GAngEl60Proj->SetLineColor(kGreen);
   SAngEl60Proj->SetLineColor(kRed);
   SAngEl60Proj->Draw("HIST E1");
   GAngEl60Proj->Draw("HIST E1 SAME");
   NAngEl60Proj->Draw("HIST E1 SAME");
   TPaveText *t9 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t9->AddText("Electrons at 60 deg");
   t9->SetTextFont(22);
   t9->SetTextSize(0.050);
   t9->SetFillColor(kWhite);
   t9->Draw();
   auto legend9 = new TLegend(0.70,0.75,0.92,0.9);
   legend9->AddEntry(NAngEl60Proj," Nexus","l");
   legend9->AddEntry(GAngEl60Proj," GFiber","l");
   legend9->AddEntry(SAngEl60Proj," Solid","l");
   legend9->SetTextSize(0.045);
   legend9->SetTextFont(22);
   legend9->Draw();

   c2->cd(10);
   NAngMu60Proj->SetLineColor(kBlue);
   GAngMu60Proj->SetLineColor(kGreen);
   SAngMu60Proj->SetLineColor(kRed);
   NAngMu60Proj->Draw("HIST E1");
   GAngMu60Proj->Draw("HIST E1 SAME");
   SAngMu60Proj->Draw("HIST E1 SAME");
   TPaveText *t10 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t10->AddText("Muons at 60 deg");
   t10->SetTextFont(22);
   t10->SetTextSize(0.050);
   t10->SetFillColor(kWhite);
   t10->Draw();
   auto legend10 = new TLegend(0.70,0.75,0.92,0.9);
   legend10->AddEntry(NAngMu60Proj," Nexus","l");
   legend10->AddEntry(GAngMu60Proj," GFiber","l");
   legend10->AddEntry(SAngMu60Proj," Solid","l");
   legend10->SetTextSize(0.045);
   legend10->SetTextFont(22);
   legend10->Draw();

   c2->cd(11);
   NAngPi60Proj->SetLineColor(kBlue);
   GAngPi60Proj->SetLineColor(kGreen);
   SAngPi60Proj->SetLineColor(kRed);
   GAngPi60Proj->Draw("HIST E1");
   NAngPi60Proj->Draw("HIST E1 SAME");
   SAngPi60Proj->Draw("HIST E1 SAME");
   TPaveText *t11 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t11->AddText("Pions at 60 deg");
   t11->SetTextFont(22);
   t11->SetTextSize(0.050);
   t11->SetFillColor(kWhite);
   t11->Draw();
   auto legend11 = new TLegend(0.70,0.75,0.92,0.9);
   legend11->AddEntry(NAngPi60Proj," Nexus","l");
   legend11->AddEntry(GAngPi60Proj," GFiber","l");
   legend11->AddEntry(SAngPi60Proj," Solid","l");
   legend11->SetTextSize(0.045);
   legend11->SetTextFont(22);
   legend11->Draw();

   c2->cd(12);
   for(int i=1; i<=NAngPr60Proj->GetSize(); i++){
   if(NAngPr60Proj->GetBinEntries(i) == 1) {
   NAngPr60Proj->SetBinContent(i,0);  
   NAngPr60Proj->SetBinError(i,0);  
   }
   }
   for(int i=1; i<=GAngPr60Proj->GetSize(); i++){
   if(GAngPr60Proj->GetBinEntries(i) == 1) {
   GAngPr60Proj->SetBinContent(i,0);  
   GAngPr60Proj->SetBinError(i,0);  
   } 
   }
   for(int i=1; i<=SAngPr60Proj->GetSize(); i++){
   if(SAngPr60Proj->GetBinEntries(i) == 1) {
   SAngPr60Proj->SetBinContent(i,0);  
   SAngPr60Proj->SetBinError(i,0);  
   } 
   }
   NAngPr60Proj->SetLineColor(kBlue);
   GAngPr60Proj->SetLineColor(kGreen);
   SAngPr60Proj->SetLineColor(kRed);
   SAngPr60Proj->Draw("HIST E1");
   GAngPr60Proj->Draw("HIST E1 SAME");
   NAngPr60Proj->Draw("HIST E1 SAME");
   TPaveText *t12 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t12->AddText("Protons at 60 deg");
   t12->SetTextFont(22);
   t12->SetTextSize(0.050);
   t12->SetFillColor(kWhite);
   t12->Draw();
   auto legend12 = new TLegend(0.70,0.75,0.92,0.9);
   legend12->AddEntry(NAngPr60Proj," Nexus","l");
   legend12->AddEntry(GAngPr60Proj," GFiber","l");
   legend12->AddEntry(SAngPr60Proj," Solid","l");
   legend12->SetTextSize(0.045);
   legend12->SetTextFont(22);
   legend12->Draw();

   cout << endl;
   cout << "--------------------------------------------------" << endl;
   cout << endl;

//c1->Print("~/Desktop/FCStudies/Plots/AllAng.pdf");
c2->Print("~/Desktop/FCStudies/Plots/NewProfilesAng.pdf");

}
