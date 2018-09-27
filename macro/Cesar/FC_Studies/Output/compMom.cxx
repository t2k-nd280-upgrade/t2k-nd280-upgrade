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

   TFile f1("/software/neutrinos/t2k-nd280-upgrade/macro/Cesar/FC_Studies/Output/Histo0_Solid_Evt0_NEvt400000.root");
   NMomEl3DProj = new TProfile2D;
   NMomEl3DProj = hMomEl3D_pxy;
   NMomMu3DProj = new TProfile2D;
   NMomMu3DProj = hMomMu3D_pxy;
   NMomPi3DProj = new TProfile2D;
   NMomPi3DProj = hMomPi3D_pxy;
   NMomPr3DProj = new TProfile2D;
   NMomPr3DProj = hMomPr3D_pxy;

   NMomEl0Proj = new TProfile;
   NMomEl0Proj = hMomEl0Proj;
   NMomMu0Proj = new TProfile;
   NMomMu0Proj = hMomMu0Proj;
   NMomPi0Proj = new TProfile;
   NMomPi0Proj = hMomPi0Proj;
   NMomPr0Proj = new TProfile;
   NMomPr0Proj = hMomPr0Proj;

   NMomEl30Proj = new TProfile;
   NMomEl30Proj = hMomEl30Proj;
   NMomMu30Proj = new TProfile;
   NMomMu30Proj = hMomMu30Proj;
   NMomPi30Proj = new TProfile;
   NMomPi30Proj = hMomPi30Proj;
   NMomPr30Proj = new TProfile;
   NMomPr30Proj = hMomPr30Proj;

   NMomEl60Proj = new TProfile;
   NMomEl60Proj = hMomEl60Proj;
   NMomMu60Proj = new TProfile;
   NMomMu60Proj = hMomMu60Proj;
   NMomPi60Proj = new TProfile;
   NMomPi60Proj = hMomPi60Proj;
   NMomPr60Proj = new TProfile;
   NMomPr60Proj = hMomPr60Proj;

   TFile f2("AnalysisOutput_Solid_Evt0_NEvt400000.root");
   GMomEl3DProj = new TProfile2D;
   GMomEl3DProj = hMomEl3D_pxy;
   GMomMu3DProj = new TProfile2D;
   GMomMu3DProj = hMomMu3D_pxy;
   GMomPi3DProj = new TProfile2D;
   GMomPi3DProj = hMomPi3D_pxy;
   GMomPr3DProj = new TProfile2D;
   GMomPr3DProj = hMomPr3D_pxy;

   GMomEl0Proj = new TProfile;
   GMomEl0Proj = hMomEl0Proj;
   GMomMu0Proj = new TProfile;
   GMomMu0Proj = hMomMu0Proj;
   GMomPi0Proj = new TProfile;
   GMomPi0Proj = hMomPi0Proj;
   GMomPr0Proj = new TProfile;
   GMomPr0Proj = hMomPr0Proj;

   GMomEl30Proj = new TProfile;
   GMomEl30Proj = hMomEl30Proj;
   GMomMu30Proj = new TProfile;
   GMomMu30Proj = hMomMu30Proj;
   GMomPi30Proj = new TProfile;
   GMomPi30Proj = hMomPi30Proj;
   GMomPr30Proj = new TProfile;
   GMomPr30Proj = hMomPr30Proj;

   GMomEl60Proj = new TProfile;
   GMomEl60Proj = hMomEl60Proj;
   GMomMu60Proj = new TProfile;
   GMomMu60Proj = hMomMu60Proj;
   GMomPi60Proj = new TProfile;
   GMomPi60Proj = hMomPi60Proj;
   GMomPr60Proj = new TProfile;
   GMomPr60Proj = hMomPr60Proj;

   TFile f3("AnalysisOutput_Solid_Evt0_NEvt400000.root");
   SMomEl3DProj = new TProfile2D;
   SMomEl3DProj = hMomEl3D_pxy;
   SMomMu3DProj = new TProfile2D;
   SMomMu3DProj = hMomMu3D_pxy;
   SMomPi3DProj = new TProfile2D;
   SMomPi3DProj = hMomPi3D_pxy;
   SMomPr3DProj = new TProfile2D;
   SMomPr3DProj = hMomPr3D_pxy;

   SMomEl0Proj = new TProfile;
   SMomEl0Proj = hMomEl0Proj;
   SMomMu0Proj = new TProfile;
   SMomMu0Proj = hMomMu0Proj;
   SMomPi0Proj = new TProfile;
   SMomPi0Proj = hMomPi0Proj;
   SMomPr0Proj = new TProfile;
   SMomPr0Proj = hMomPr0Proj;

   SMomEl30Proj = new TProfile;
   SMomEl30Proj = hMomEl30Proj;
   SMomMu30Proj = new TProfile;
   SMomMu30Proj = hMomMu30Proj;
   SMomPi30Proj = new TProfile;
   SMomPi30Proj = hMomPi30Proj;
   SMomPr30Proj = new TProfile;
   SMomPr30Proj = hMomPr30Proj;

   SMomEl60Proj = new TProfile;
   SMomEl60Proj = hMomEl60Proj;
   SMomMu60Proj = new TProfile;
   SMomMu60Proj = hMomMu60Proj;
   SMomPi60Proj = new TProfile;
   SMomPi60Proj = hMomPi60Proj;
   SMomPr60Proj = new TProfile;
   SMomPr60Proj = hMomPr60Proj;

   c1->cd(1);
   NMomEl3DProj->Draw("COLZ");
   c1->cd(2);
   NMomMu3DProj->Draw("COLZ");
   c1->cd(3);
   NMomPi3DProj->Draw("COLZ");
   c1->cd(4);
   NMomPr3DProj->Draw("COLZ");
   c1->cd(5);
   GMomMu3DProj->Draw("COLZ");
   c1->cd(6);
   GMomMu3DProj->Draw("COLZ");
   c1->cd(7);
   GMomPi3DProj->Draw("COLZ");
   c1->cd(8);
   GMomPr3DProj->Draw("COLZ");
   c1->cd(9);
   SMomEl3DProj->Draw("COLZ");
   c1->cd(10);
   SMomMu3DProj->Draw("COLZ");
   c1->cd(11);
   SMomPi3DProj->Draw("COLZ");
   c1->cd(12);
   SMomPr3DProj->Draw("COLZ");
   

   c2->cd(1);
   NMomEl0Proj->SetLineColor(kBlue);
   GMomEl0Proj->SetLineColor(kGreen);
   SMomEl0Proj->SetLineColor(kRed);
   NMomEl0Proj->GetYaxis()->SetRangeUser(0,100);
   GMomEl0Proj->GetYaxis()->SetRangeUser(0,100);
   SMomEl0Proj->GetYaxis()->SetRangeUser(0,100);
   NMomEl0Proj->Draw("HIST E1");
   GMomEl0Proj->Draw("HIST E1 SAME");
   SMomEl0Proj->Draw("HIST E1 SAME");
   TPaveText *t1 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t1->AddText("Electrons at 0 deg");
   t1->SetTextFont(22);
   t1->SetTextSize(0.050);
   t1->SetFillColor(kWhite);
   t1->Draw();
   auto legend1 = new TLegend(0.70,0.2,0.92,0.35);
   legend1->AddEntry(NMomEl0Proj," Nexus","l");
   legend1->AddEntry(GMomEl0Proj," GFiber","l");
   legend1->AddEntry(SMomEl0Proj," Solid","l");
   legend1->SetTextSize(0.045);
   legend1->SetTextFont(22);
   legend1->Draw();

   c2->cd(2);
   NMomMu0Proj->SetLineColor(kBlue);
   GMomMu0Proj->SetLineColor(kGreen);
   SMomMu0Proj->SetLineColor(kRed);
   NMomMu0Proj->GetYaxis()->SetRangeUser(0,100);
   GMomMu0Proj->GetYaxis()->SetRangeUser(0,100);
   SMomMu0Proj->GetYaxis()->SetRangeUser(0,100);
   NMomMu0Proj->Draw("HIST E1");
   GMomMu0Proj->Draw("HIST E1 SAME");
   SMomMu0Proj->Draw("HIST E1 SAME");
   TPaveText *t2 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t2->AddText("Muons at 0 deg");
   t2->SetTextFont(22);
   t2->SetTextSize(0.050);
   t2->SetFillColor(kWhite);
   t2->Draw();
   auto legend2 = new TLegend(0.70,0.2,0.92,0.35);
   legend2->AddEntry(NMomMu0Proj," Nexus","l");
   legend2->AddEntry(GMomMu0Proj," GFiber","l");
   legend2->AddEntry(SMomMu0Proj," Solid","l");
   legend2->SetTextSize(0.045);
   legend2->SetTextFont(22);
   legend2->Draw();

   c2->cd(3);
   NMomPi0Proj->SetLineColor(kBlue);
   GMomPi0Proj->SetLineColor(kGreen);
   SMomPi0Proj->SetLineColor(kRed);
   NMomPi0Proj->GetYaxis()->SetRangeUser(0,100);
   GMomPi0Proj->GetYaxis()->SetRangeUser(0,100);
   SMomPi0Proj->GetYaxis()->SetRangeUser(0,100);
   NMomPi0Proj->Draw("HIST E1");
   GMomPi0Proj->Draw("HIST E1 SAME");
   SMomPi0Proj->Draw("HIST E1 SAME");
   TPaveText *t3 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t3->AddText("Pions at 0 deg");
   t3->SetTextFont(22);
   t3->SetTextSize(0.050);
   t3->SetFillColor(kWhite);
   t3->Draw();
   auto legend3 = new TLegend(0.70,0.2,0.92,0.35);
   legend3->AddEntry(NMomPi0Proj," Nexus","l");
   legend3->AddEntry(GMomPi0Proj," GFiber","l");
   legend3->AddEntry(SMomPi0Proj," Solid","l");
   legend3->SetTextSize(0.045);
   legend3->SetTextFont(22);
   legend3->Draw();

   c2->cd(4);
   NMomPr0Proj->SetLineColor(kBlue);
   GMomPr0Proj->SetLineColor(kGreen);
   SMomPr0Proj->SetLineColor(kRed);
   NMomPi0Proj->GetYaxis()->SetRangeUser(0,100);
   GMomPi0Proj->GetYaxis()->SetRangeUser(0,100);
   SMomPi0Proj->GetYaxis()->SetRangeUser(0,100);
   NMomPr0Proj->Draw("HIST E1");
   GMomPr0Proj->Draw("HIST E1 SAME");
   SMomPr0Proj->Draw("HIST E1 SAME");
   TPaveText *t4 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t4->AddText("Protons at 0 deg");
   t4->SetTextFont(22);
   t4->SetTextSize(0.050);
   t4->SetFillColor(kWhite);
   t4->Draw();
   auto legend4 = new TLegend(0.70,0.2,0.92,0.35);
   legend4->AddEntry(NMomPr0Proj," Nexus","l");
   legend4->AddEntry(GMomPr0Proj," GFiber","l");
   legend4->AddEntry(SMomPr0Proj," Solid","l");
   legend4->SetTextSize(0.045);
   legend4->SetTextFont(22);
   legend4->Draw();

   c2->cd(5);
   NMomEl30Proj->SetLineColor(kBlue);
   GMomEl30Proj->SetLineColor(kGreen);
   SMomEl30Proj->SetLineColor(kRed);
   NMomEl30Proj->GetYaxis()->SetRangeUser(0,100);
   GMomEl30Proj->GetYaxis()->SetRangeUser(0,100);
   SMomEl30Proj->GetYaxis()->SetRangeUser(0,100);
   NMomEl30Proj->Draw("HIST E1");
   GMomEl30Proj->Draw("HIST E1 SAME");
   SMomEl30Proj->Draw("HIST E1 SAME");
   TPaveText *t5 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t5->AddText("Electrons at 30 deg");
   t5->SetTextFont(22);
   t5->SetTextSize(0.050);
   t5->SetFillColor(kWhite);
   t5->Draw();
   auto legend5 = new TLegend(0.70,0.2,0.92,0.35);
   legend5->AddEntry(NMomEl30Proj," Nexus","l");
   legend5->AddEntry(GMomEl30Proj," GFiber","l");
   legend5->AddEntry(SMomEl30Proj," Solid","l");
   legend5->SetTextSize(0.045);
   legend5->SetTextFont(22);
   legend5->Draw();

   c2->cd(6);
   NMomMu30Proj->SetLineColor(kBlue);
   GMomMu30Proj->SetLineColor(kGreen);
   SMomMu30Proj->SetLineColor(kRed);
   NMomMu30Proj->GetYaxis()->SetRangeUser(0,100);
   GMomMu30Proj->GetYaxis()->SetRangeUser(0,100);
   SMomMu30Proj->GetYaxis()->SetRangeUser(0,100);
   NMomMu30Proj->Draw("HIST E1");
   GMomMu30Proj->Draw("HIST E1 SAME");
   SMomMu30Proj->Draw("HIST E1 SAME");
   TPaveText *t6 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t6->AddText("Muons at 30 deg");
   t6->SetTextFont(22);
   t6->SetTextSize(0.050);
   t6->SetFillColor(kWhite);
   t6->Draw();
   auto legend6 = new TLegend(0.70,0.2,0.92,0.35);
   legend6->AddEntry(NMomMu30Proj," Nexus","l");
   legend6->AddEntry(GMomMu30Proj," GFiber","l");
   legend6->AddEntry(SMomMu30Proj," Solid","l");
   legend6->SetTextSize(0.045);
   legend6->SetTextFont(22);
   legend6->Draw();

   c2->cd(7);
   NMomPi30Proj->SetLineColor(kBlue);
   GMomPi30Proj->SetLineColor(kGreen);
   SMomPi30Proj->SetLineColor(kRed);
   NMomPi30Proj->GetYaxis()->SetRangeUser(0,100);
   GMomPi30Proj->GetYaxis()->SetRangeUser(0,100);
   SMomPi30Proj->GetYaxis()->SetRangeUser(0,100);
   NMomPi30Proj->Draw("HIST E1");
   GMomPi30Proj->Draw("HIST E1 SAME");
   SMomPi30Proj->Draw("HIST E1 SAME");
   TPaveText *t7 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t7->AddText("Pions at 30 deg");
   t7->SetTextFont(22);
   t7->SetTextSize(0.050);
   t7->SetFillColor(kWhite);
   t7->Draw();
   auto legend7 = new TLegend(0.70,0.2,0.92,0.35);
   legend7->AddEntry(NMomMu30Proj," Nexus","l");
   legend7->AddEntry(GMomMu30Proj," GFiber","l");
   legend7->AddEntry(SMomMu30Proj," Solid","l");
   legend7->SetTextSize(0.045);
   legend7->SetTextFont(22);
   legend7->Draw();

   c2->cd(8);
   NMomPr30Proj->SetLineColor(kBlue);
   GMomPr30Proj->SetLineColor(kGreen);
   SMomPr30Proj->SetLineColor(kRed);
   NMomPr30Proj->GetYaxis()->SetRangeUser(0,100);
   GMomPr30Proj->GetYaxis()->SetRangeUser(0,100);
   SMomPr30Proj->GetYaxis()->SetRangeUser(0,100);
   NMomPr30Proj->Draw("HIST E1");
   GMomPr30Proj->Draw("HIST E1 SAME");
   SMomPr30Proj->Draw("HIST E1 SAME");
   TPaveText *t8 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t8->AddText("Protons at 30 deg");
   t8->SetTextFont(22);
   t8->SetTextSize(0.050);
   t8->SetFillColor(kWhite);
   t8->Draw();
   auto legend8 = new TLegend(0.70,0.2,0.92,0.35);
   legend8->AddEntry(NMomPr30Proj," Nexus","l");
   legend8->AddEntry(GMomPr30Proj," GFiber","l");
   legend8->AddEntry(SMomPr30Proj," Solid","l");
   legend8->SetTextSize(0.045);
   legend8->SetTextFont(22);
   legend8->Draw();

    c2->cd(9);
   NMomEl60Proj->SetLineColor(kBlue);
   GMomEl60Proj->SetLineColor(kGreen);
   SMomEl60Proj->SetLineColor(kRed);
   NMomEl60Proj->GetYaxis()->SetRangeUser(0,100);
   GMomEl60Proj->GetYaxis()->SetRangeUser(0,100);
   SMomEl60Proj->GetYaxis()->SetRangeUser(0,100);
   NMomEl60Proj->Draw("HIST E1");
   GMomEl60Proj->Draw("HIST E1 SAME");
   SMomEl60Proj->Draw("HIST E1 SAME");
   TPaveText *t9 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t9->AddText("Electrons at 60 deg");
   t9->SetTextFont(22);
   t9->SetTextSize(0.050);
   t9->SetFillColor(kWhite);
   t9->Draw();
   auto legend9 = new TLegend(0.70,0.2,0.92,0.35);
   legend9->AddEntry(NMomEl60Proj," Nexus","l");
   legend9->AddEntry(GMomEl60Proj," GFiber","l");
   legend9->AddEntry(SMomEl60Proj," Solid","l");
   legend9->SetTextSize(0.045);
   legend9->SetTextFont(22);
   legend9->Draw();

   c2->cd(10);
   NMomMu60Proj->SetLineColor(kBlue);
   GMomMu60Proj->SetLineColor(kGreen);
   SMomMu60Proj->SetLineColor(kRed);
   NMomMu60Proj->GetYaxis()->SetRangeUser(0,100);
   GMomMu60Proj->GetYaxis()->SetRangeUser(0,100);
   SMomMu60Proj->GetYaxis()->SetRangeUser(0,100);
   NMomMu60Proj->Draw("HIST E1");
   GMomMu60Proj->Draw("HIST E1 SAME");
   SMomMu60Proj->Draw("HIST E1 SAME");
   TPaveText *t10 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t10->AddText("Muons at 60 deg");
   t10->SetTextFont(22);
   t10->SetTextSize(0.050);
   t10->SetFillColor(kWhite);
   t10->Draw();
   auto legend10 = new TLegend(0.70,0.2,0.92,0.35);
   legend10->AddEntry(NMomMu60Proj," Nexus","l");
   legend10->AddEntry(GMomMu60Proj," GFiber","l");
   legend10->AddEntry(SMomMu60Proj," Solid","l");
   legend10->SetTextSize(0.045);
   legend10->SetTextFont(22);
   legend10->Draw();

   c2->cd(11);
   NMomPi60Proj->SetLineColor(kBlue);
   GMomPi60Proj->SetLineColor(kGreen);
   SMomPi60Proj->SetLineColor(kRed);
   NMomPi60Proj->GetYaxis()->SetRangeUser(0,100);
   GMomPi60Proj->GetYaxis()->SetRangeUser(0,100);
   SMomPi60Proj->GetYaxis()->SetRangeUser(0,100);
   NMomPi60Proj->Draw("HIST E1");
   GMomPi60Proj->Draw("HIST E1 SAME");
   SMomPi60Proj->Draw("HIST E1 SAME");
   TPaveText *t11 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t11->AddText("Pions at 60 deg");
   t11->SetTextFont(22);
   t11->SetTextSize(0.050);
   t11->SetFillColor(kWhite);
   t11->Draw();
   auto legend11 = new TLegend(0.70,0.2,0.92,0.35);
   legend11->AddEntry(NMomPi60Proj," Nexus","l");
   legend11->AddEntry(GMomPi60Proj," GFiber","l");
   legend11->AddEntry(SMomPi60Proj," Solid","l");
   legend11->SetTextSize(0.045);
   legend11->SetTextFont(22);
   legend11->Draw();

   c2->cd(12);
   // for(int i=1; i<=NMomPr60Proj->GetSize(); i++){
   // if(NMomPr60Proj->GetBinEntries(i) == 1) {
   // NMomPr60Proj->SetBinContent(i,0);  
   // NMomPr60Proj->SetBinError(i,0);  
   // }
   // }
   // for(int i=1; i<=GMomPr60Proj->GetSize(); i++){
   // if(GMomPr60Proj->GetBinEntries(i) == 1) {
   // GMomPr60Proj->SetBinContent(i,0);  
   // GMomPr60Proj->SetBinError(i,0);  
   // } 
   // }
   // for(int i=1; i<=SMomPr60Proj->GetSize(); i++){
   // if(SMomPr60Proj->GetBinEntries(i) == 1) {
   // SMomPr60Proj->SetBinContent(i,0);  
   // SMomPr60Proj->SetBinError(i,0);  
   // } 
   // }
   NMomPr60Proj->SetLineColor(kBlue);
   GMomPr60Proj->SetLineColor(kGreen);
   SMomPr60Proj->SetLineColor(kRed);
   NMomPr60Proj->GetYaxis()->SetRangeUser(0,100);
   GMomPr60Proj->GetYaxis()->SetRangeUser(0,100);
   SMomPr60Proj->GetYaxis()->SetRangeUser(0,100);
   NMomPr60Proj->Draw("HIST E1");
   GMomPr60Proj->Draw("HIST E1 SAME");
   SMomPr60Proj->Draw("HIST E1 SAME");
   TPaveText *t12 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t12->AddText("Protons at 60 deg");
   t12->SetTextFont(22);
   t12->SetTextSize(0.050);
   t12->SetFillColor(kWhite);
   t12->Draw();
   auto legend12 = new TLegend(0.70,0.2,0.92,0.35);
   legend12->AddEntry(NMomPr60Proj," Nexus","l");
   legend12->AddEntry(GMomPr60Proj," GFiber","l");
   legend12->AddEntry(SMomPr60Proj," Solid","l");
   legend12->SetTextSize(0.045);
   legend12->SetTextFont(22);
   legend12->Draw();

   cout << endl;
   cout << "--------------------------------------------------" << endl;
   cout << endl;

//c1->Print("~/Desktop/FCStudies/Plots/AllMom.pdf");
c2->Print("/software/neutrinos/t2k-nd280-upgrade/macro/Cesar/FC_Studies/Plots/compMom.pdf");

}
