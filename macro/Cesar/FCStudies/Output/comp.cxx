{
   cout << endl;
   cout << "--------------------------------------------------" << endl;
   cout << endl;

   TCanvas *c1 = new TCanvas("c1","c1",1000,1000);
   TCanvas *c2 = new TCanvas("c2","c2",1000,1200);
   gStyle->SetOptStat(0000);
   gStyle->SetPadBorderMode(0);
   gStyle->SetPadLeftMargin(0.12);
   gStyle->SetPadRightMargin(0.03);
   gStyle->SetPadTopMargin(0.06);
   gStyle->SetPadBottomMargin(0.1);
   gStyle->SetTitleX(0.57f);
   gStyle->SetTitleW(0.8f);
   gStyle->SetTextFont(2);
   gStyle->SetLabelFont(22,"xyz");
   gStyle->SetTitleFont(22,"xyz");
   gStyle->SetTitleFont(22);
   gStyle->SetOptTitle(0);
  
   c1->Divide(4,3);
   c2->Divide(4,3);

   TFile f1("Histos_Nexus_Evt0_NEvt500000.root");
   NMomEl = new TH2D;
   NMomEl = hMomEl;
   NMomMu = new TH2D;
   NMomMu = hMomMu;
   NMomPi = new TH2D;
   NMomPi = hMomPi;
   NMomPr = new TH2D;
   NMomPr = hMomPr;

   TFile f2("Histos_GF_Evt0_NEvt500000.root");
   GMomEl = new TH2D;
   GMomEl = hMomEl;
   GMomMu = new TH2D;
   GMomMu = hMomMu;
   GMomPi = new TH2D;
   GMomPi = hMomPi;
   GMomPr = new TH2D;
   GMomPr = hMomPr;

   TFile f3("Histos_Solid_Evt0_NEvt500000.root");
   SMomEl = new TH2D;
   SMomEl = hMomEl;
   SMomMu = new TH2D;
   SMomMu = hMomMu;
   SMomPi = new TH2D;
   SMomPi = hMomPi;
   SMomPr = new TH2D;
   SMomPr = hMomPr;

   c1->cd(1);
   NMomEl->Draw("COLZ");
   c1->cd(2);
   NMomMu->Draw("COLZ");
   c1->cd(3);
   NMomPi->Draw("COLZ");
   c1->cd(4);
   NMomPr->Draw("COLZ");
   c1->cd(5);
   GMomEl->Draw("COLZ");
   c1->cd(6);
   GMomMu->Draw("COLZ");
   c1->cd(7);
   GMomPi->Draw("COLZ");
   c1->cd(8);
   GMomPr->Draw("COLZ");
   c1->cd(9);
   SMomEl->Draw("COLZ");
   c1->cd(10);
   SMomMu->Draw("COLZ");
   c1->cd(11);
   SMomPi->Draw("COLZ");
   c1->cd(12);
   SMomPr->Draw("COLZ");
   
   c2->cd(1);
   TH1D *NMomEl0 = NMomEl->ProjectionY("NEl0",15,15);
   NMomEl0->SetLineColor(kPink);
   NMomEl0->GetXaxis()->SetTitleOffset(0.9);
   NMomEl0->GetYaxis()->SetTitleOffset(1.2);
   NMomEl0->GetYaxis()->SetTitle("Final True Momentum [%]"); 
   NMomEl0->GetYaxis()->CenterTitle(true);
   NMomEl0->GetYaxis()->SetTitleSize(0.050);
   NMomEl0->GetYaxis()->SetLabelSize(0.050);
   NMomEl0->DrawCopy("HIST");
   TH1D *GMomEl0 = GMomEl->ProjectionY("GEl0",15,15);
   GMomEl0->SetLineColor(kGreen);
   GMomEl0->DrawCopy("HIST same");
   TH1D *SMomEl0 = SMomEl->ProjectionY("SEl0",15,15);
   SMomEl0->SetLineColor(kBlue);
   SMomEl0->DrawCopy("HIST same");

   TPaveText *t1 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t1->AddText("Electrons at 0 deg");
   t1->SetTextFont(22);
   t1->SetTextSize(0.050);
   t1->SetFillColor(kWhite);
   t1->Draw();

   auto legend1 = new TLegend(0.70,0.15,0.92,0.25);
   legend1->AddEntry(NMomEl0," Nexus","l");
   legend1->AddEntry(GMomEl0," GFiber","l");
   legend1->AddEntry(SMomEl0," Solid","l");
   legend1->SetTextSize(0.045);
   legend1->SetTextFont(22);
   legend1->Draw();

   c2->cd(2);
   TH1D *NMomMu0 = NMomMu->ProjectionY("NMu0",15,15);
   NMomMu0->SetLineColor(kPink);
   NMomMu0->GetXaxis()->SetTitleOffset(0.9);
   NMomMu0->GetYaxis()->SetTitleOffset(1.2);
   NMomMu0->GetYaxis()->SetTitle("Muon Final True Momentum [%]"); 
   NMomMu0->GetYaxis()->CenterTitle(true);
   NMomMu0->GetYaxis()->SetTitleSize(0.050);
   NMomMu0->GetYaxis()->SetLabelSize(0.050);
   NMomMu0->GetXaxis()->SetRangeUser(0, 300);
   NMomMu0->DrawCopy("HIST");
   TH1D *GMomMu0 = GMomMu->ProjectionY("GMu0",15,15);
   GMomMu0->SetLineColor(kGreen);
   GMomMu0->DrawCopy("HIST same");
   TH1D *SMomMu0 = SMomMu->ProjectionY("SMu0",15,15);
   SMomMu0->SetLineColor(kBlue);
   SMomMu0->DrawCopy("HIST same");

   TPaveText *t2 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t2->AddText("Muons at 0 deg");
   t2->SetTextFont(22);
   t2->SetTextSize(0.050);
   t2->SetFillColor(kWhite);
   t2->Draw();

   auto legend2 = new TLegend(0.70,0.15,0.92,0.25);
   legend2->AddEntry(NMomMu0," Nexus","l");
   legend2->AddEntry(GMomMu0," GFiber","l");
   legend2->AddEntry(SMomMu0," Solid","l");
   legend2->SetTextSize(0.045);
   legend2->SetTextFont(22);
   legend2->Draw();

   c2->cd(3);
   TH1D *NMomPi0 = NMomPi->ProjectionY("NPi0",15,15);
   NMomPi0->SetLineColor(kPink);
   NMomPi0->GetXaxis()->SetTitleOffset(0.9);
   NMomPi0->GetYaxis()->SetTitleOffset(1.2);
   NMomPi0->GetYaxis()->SetTitle("Pion Final True Momentum [%]"); 
   NMomPi0->GetYaxis()->CenterTitle(true);
   NMomPi0->GetYaxis()->SetTitleSize(0.050);
   NMomPi0->GetYaxis()->SetLabelSize(0.050);
   NMomPi0->GetXaxis()->SetRangeUser(0, 300);
   NMomPi0->DrawCopy("HIST");
   TH1D *GMomPi0 = GMomPi->ProjectionY("GPi0",15,15);
   GMomPi0->SetLineColor(kGreen);
   GMomPi0->DrawCopy("HIST same");
   TH1D *SMomPi0 = SMomPi->ProjectionY("SPi0",15,15);
   SMomPi0->SetLineColor(kBlue);
   SMomPi0->DrawCopy("HIST same");

   TPaveText *t3 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t3->AddText("Pions at 0 deg");
   t3->SetTextFont(22);
   t3->SetTextSize(0.050);
   t3->SetFillColor(kWhite);
   t3->Draw();

   auto legend3 = new TLegend(0.70,0.15,0.92,0.25);
   legend3->AddEntry(NMomPi0," Nexus","l");
   legend3->AddEntry(GMomPi0," GFiber","l");
   legend3->AddEntry(SMomPi0," Solid","l");
   legend3->SetTextSize(0.045);
   legend3->SetTextFont(22);
   legend3->Draw();

   c2->cd(4);
   TH1D *NMomPr0 = NMomPr->ProjectionY("NPr0",15,15);
   NMomPr0->SetLineColor(kPink);
   NMomPr0->GetXaxis()->SetTitleOffset(0.9);
   NMomPr0->GetYaxis()->SetTitleOffset(1.2);
   NMomPr0->GetYaxis()->SetTitle("Proton Final True Momentum [%]"); 
   NMomPr0->GetYaxis()->CenterTitle(true);
   NMomPr0->GetYaxis()->SetTitleSize(0.050);
   NMomPr0->GetYaxis()->SetLabelSize(0.050);
   NMomPr0->DrawCopy("HIST");
   TH1D *GMomPr0 = GMomPr->ProjectionY("GPr0",15,15);
   GMomPr0->SetLineColor(kGreen);
   GMomPr0->DrawCopy("HIST same");
   TH1D *SMomPr0 = SMomPr->ProjectionY("SPr0",15,15);
   SMomPr0->SetLineColor(kBlue);
   SMomPr0->DrawCopy("HIST same");

   TPaveText *t4 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t4->AddText("Protons at 0 deg");
   t4->SetTextFont(22);
   t4->SetTextSize(0.050);
   t4->SetFillColor(kWhite);
   t4->Draw();

   auto legend4 = new TLegend(0.70,0.15,0.92,0.25);
   legend4->AddEntry(NMomPr0," Nexus","l");
   legend4->AddEntry(GMomPr0," GFiber","l");
   legend4->AddEntry(SMomPr0," Solid","l");
   legend4->SetTextSize(0.045);
   legend4->SetTextFont(22);
   legend4->Draw();

   c2->cd(5);
   TH1D *NMomEl30 = NMomEl->ProjectionY("NEl30",21,21);
   NMomEl30->SetLineColor(kPink);
   NMomEl30->GetXaxis()->SetTitleOffset(0.9);
   NMomEl30->GetYaxis()->SetTitleOffset(1.2);
   NMomEl30->GetYaxis()->SetTitle("Final True Momentum [%]"); 
   NMomEl30->GetYaxis()->CenterTitle(true);
   NMomEl30->GetYaxis()->SetTitleSize(0.050);
   NMomEl30->GetYaxis()->SetLabelSize(0.050);
   NMomEl30->DrawCopy("HIST");
   TH1D *GMomEl30 = GMomEl->ProjectionY("GEl30",21,21);
   GMomEl30->SetLineColor(kGreen);
   GMomEl30->DrawCopy("HIST same");
   TH1D *SMomEl30 = SMomEl->ProjectionY("SEl30",21,21);
   SMomEl30->SetLineColor(kBlue);
   SMomEl30->DrawCopy("HIST same");

   TPaveText *t5 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t5->AddText("Electrons at 30 deg");
   t5->SetTextFont(22);
   t5->SetTextSize(0.050);
   t5->SetFillColor(kWhite);
   t5->Draw();

   auto legend5 = new TLegend(0.70,0.15,0.92,0.25);
   legend5->AddEntry(NMomEl30," Nexus","l");
   legend5->AddEntry(GMomEl30," GFiber","l");
   legend5->AddEntry(SMomEl30," Solid","l");
   legend5->SetTextSize(0.045);
   legend5->SetTextFont(22);
   legend5->Draw();

   c2->cd(6);
   TH1D *NMomMu30 = NMomMu->ProjectionY("NMu30",21,21);
   NMomMu30->SetLineColor(kPink);
   NMomMu30->GetXaxis()->SetTitleOffset(0.9);
   NMomMu30->GetYaxis()->SetTitleOffset(1.2);
   NMomMu30->GetYaxis()->SetTitle("Muon Final True Momentum [%]"); 
   NMomMu30->GetYaxis()->CenterTitle(true);
   NMomMu30->GetYaxis()->SetTitleSize(0.050);
   NMomMu30->GetYaxis()->SetLabelSize(0.050);
   NMomMu30->GetXaxis()->SetRangeUser(0, 300);
   NMomMu30->DrawCopy("HIST");
   TH1D *GMomMu30 = GMomMu->ProjectionY("GMu30",21,21);
   GMomMu30->SetLineColor(kGreen);
   GMomMu30->DrawCopy("HIST same");
   TH1D *SMomMu30 = SMomMu->ProjectionY("SMu30",21,21);
   SMomMu30->SetLineColor(kBlue);
   SMomMu30->DrawCopy("HIST same");

   TPaveText *t6 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t6->AddText("Muons at 30 deg");
   t6->SetTextFont(22);
   t6->SetTextSize(0.050);
   t6->SetFillColor(kWhite);
   t6->Draw();

   auto legend6 = new TLegend(0.70,0.15,0.92,0.25);
   legend6->AddEntry(NMomMu30," Nexus","l");
   legend6->AddEntry(GMomMu30," GFiber","l");
   legend6->AddEntry(SMomMu30," Solid","l");
   legend6->SetTextSize(0.045);
   legend6->SetTextFont(22);
   legend6->Draw();

   c2->cd(7);
   TH1D *NMomPi30 = NMomPi->ProjectionY("NPi30",21,21);
   NMomPi30->SetLineColor(kPink);
   NMomPi30->GetXaxis()->SetTitleOffset(0.9);
   NMomPi30->GetYaxis()->SetTitleOffset(1.2);
   NMomPi30->GetYaxis()->SetTitle("Pion Final True Momentum [%]"); 
   NMomPi30->GetYaxis()->CenterTitle(true);
   NMomPi30->GetYaxis()->SetTitleSize(0.050);
   NMomPi30->GetYaxis()->SetLabelSize(0.050);
   NMomPi30->GetXaxis()->SetRangeUser(0, 300);
   NMomPi30->DrawCopy("HIST");
   TH1D *GMomPi30 = GMomPi->ProjectionY("GPi30",21,21);
   GMomPi30->SetLineColor(kGreen);
   GMomPi30->DrawCopy("HIST same");
   TH1D *SMomPi30 = SMomPi->ProjectionY("SPi30",21,21);
   SMomPi30->SetLineColor(kBlue);
   SMomPi30->DrawCopy("HIST same");

   TPaveText *t7 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t7->AddText("Pions at 30 deg");
   t7->SetTextFont(22);
   t7->SetTextSize(0.050);
   t7->SetFillColor(kWhite);
   t7->Draw();

   auto legend7 = new TLegend(0.70,0.15,0.92,0.25);
   legend7->AddEntry(NMomPi30," Nexus","l");
   legend7->AddEntry(GMomPi30," GFiber","l");
   legend7->AddEntry(SMomPi30," Solid","l");
   legend7->SetTextSize(0.045);
   legend7->SetTextFont(22);
   legend7->Draw();

   c2->cd(8);
   TH1D *NMomPr30 = NMomPr->ProjectionY("NPr30",21,21);
   NMomPr30->SetLineColor(kPink);
   NMomPr30->GetXaxis()->SetTitleOffset(0.9);
   NMomPr30->GetYaxis()->SetTitleOffset(1.2);
   NMomPr30->GetYaxis()->SetTitle("Proton Final True Momentum [%]"); 
   NMomPr30->GetYaxis()->CenterTitle(true);
   NMomPr30->GetYaxis()->SetTitleSize(0.050);
   NMomPr30->GetYaxis()->SetLabelSize(0.050);
   NMomPr30->DrawCopy("HIST");
   TH1D *GMomPr30 = GMomPr->ProjectionY("GPr30",21,21);
   GMomPr30->SetLineColor(kGreen);
   GMomPr30->DrawCopy("HIST same");
   TH1D *SMomPr30 = SMomPr->ProjectionY("SPr30",21,21);
   SMomPr30->SetLineColor(kBlue);
   SMomPr30->DrawCopy("HIST same");

   TPaveText *t8 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t8->AddText("Protons at 30 deg");
   t8->SetTextFont(22);
   t8->SetTextSize(0.050);
   t8->SetFillColor(kWhite);
   t8->Draw();

   auto legend8 = new TLegend(0.70,0.15,0.92,0.25);
   legend8->AddEntry(NMomPr30," Nexus","l");
   legend8->AddEntry(GMomPr30," GFiber","l");
   legend8->AddEntry(SMomPr30," Solid","l");
   legend8->SetTextSize(0.045);
   legend8->SetTextFont(22);
   legend8->Draw();

   c2->cd(9);
   TH1D *NMomEl60 = NMomEl->ProjectionY("NEl60",27,27);
   NMomEl60->SetLineColor(kPink);
   NMomEl60->GetXaxis()->SetTitleOffset(0.9);
   NMomEl60->GetYaxis()->SetTitleOffset(1.2);
   NMomEl60->GetYaxis()->SetTitle("Final True Momentum [%]"); 
   NMomEl60->GetYaxis()->CenterTitle(true);
   NMomEl60->GetYaxis()->SetTitleSize(0.050);
   NMomEl60->GetYaxis()->SetLabelSize(0.050);
   NMomEl60->DrawCopy("HIST");
   TH1D *GMomEl60 = GMomEl->ProjectionY("GEl60",27,27);
   GMomEl60->SetLineColor(kGreen);
   GMomEl60->DrawCopy("HIST same");
   TH1D *SMomEl60 = SMomEl->ProjectionY("SEl60",27,27);
   SMomEl60->SetLineColor(kBlue);
   SMomEl60->DrawCopy("HIST same");

   TPaveText *t9 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t9->AddText("Electrons at 60 deg");
   t9->SetTextFont(22);
   t9->SetTextSize(0.050);
   t9->SetFillColor(kWhite);
   t9->Draw();

   auto legend9 = new TLegend(0.70,0.15,0.92,0.25);
   legend9->AddEntry(NMomEl60," Nexus","l");
   legend9->AddEntry(GMomEl60," GFiber","l");
   legend9->AddEntry(SMomEl60," Solid","l");
   legend9->SetTextSize(0.045);
   legend9->SetTextFont(22);
   legend9->Draw();

   c2->cd(10);
   TH1D *NMomMu60 = NMomMu->ProjectionY("NMu60",27,27);
   NMomMu60->SetLineColor(kPink);
   NMomMu60->GetXaxis()->SetTitleOffset(0.9);
   NMomMu60->GetYaxis()->SetTitleOffset(1.2);
   NMomMu60->GetYaxis()->SetTitle("Muon Final True Momentum [%]"); 
   NMomMu60->GetYaxis()->CenterTitle(true);
   NMomMu60->GetYaxis()->SetTitleSize(0.050);
   NMomMu60->GetYaxis()->SetLabelSize(0.050);
   NMomMu60->GetXaxis()->SetRangeUser(0, 300);
   NMomMu60->DrawCopy("HIST");
   TH1D *GMomMu60 = GMomMu->ProjectionY("GMu60",27,27);
   GMomMu60->SetLineColor(kGreen);
   GMomMu60->DrawCopy("HIST same");
   TH1D *SMomMu60 = SMomMu->ProjectionY("SMu60",27,27);
   SMomMu60->SetLineColor(kBlue);
   SMomMu60->DrawCopy("HIST same");

   TPaveText *t10 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t10->AddText("Muons at 60 deg");
   t10->SetTextFont(22);
   t10->SetTextSize(0.050);
   t10->SetFillColor(kWhite);
   t10->Draw();

   auto legend10 = new TLegend(0.70,0.15,0.92,0.25);
   legend10->AddEntry(NMomMu60," Nexus","l");
   legend10->AddEntry(GMomMu60," GFiber","l");
   legend10->AddEntry(SMomMu60," Solid","l");
   legend10->SetTextSize(0.045);
   legend10->SetTextFont(22);
   legend10->Draw();

   c2->cd(11);
   TH1D *NMomPi60 = NMomPi->ProjectionY("NPi60",27,27);
   NMomPi60->SetLineColor(kPink);
   NMomPi60->GetXaxis()->SetTitleOffset(0.9);
   NMomPi60->GetYaxis()->SetTitleOffset(1.2);
   NMomPi60->GetYaxis()->SetTitle("Pion Final True Momentum [%]"); 
   NMomPi60->GetYaxis()->CenterTitle(true);
   NMomPi60->GetYaxis()->SetTitleSize(0.050);
   NMomPi60->GetYaxis()->SetLabelSize(0.050);
   NMomPi60->GetXaxis()->SetRangeUser(0, 300);
   NMomPi60->DrawCopy("HIST");
   TH1D *GMomPi60 = GMomPi->ProjectionY("GPi60",27,27);
   GMomPi60->SetLineColor(kGreen);
   GMomPi60->DrawCopy("HIST same");
   TH1D *SMomPi60 = SMomPi->ProjectionY("SPi60",27,27);
   SMomPi60->SetLineColor(kBlue);
   SMomPi60->DrawCopy("HIST same");

   TPaveText *t11 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t11->AddText("Pions at 60 deg");
   t11->SetTextFont(22);
   t11->SetTextSize(0.050);
   t11->SetFillColor(kWhite);
   t11->Draw();

   auto legend11 = new TLegend(0.70,0.15,0.92,0.25);
   legend11->AddEntry(NMomPi60," Nexus","l");
   legend11->AddEntry(GMomPi60," GFiber","l");
   legend11->AddEntry(SMomPi60," Solid","l");
   legend11->SetTextSize(0.045);
   legend11->SetTextFont(22);
   legend11->Draw();

   c2->cd(12);
   TH1D *NMomPr60 = NMomPr->ProjectionY("NPr60",27,27);
   NMomPr60->SetLineColor(kPink);
   NMomPr60->GetXaxis()->SetTitleOffset(0.9);
   NMomPr60->GetYaxis()->SetTitleOffset(1.2);
   NMomPr60->GetYaxis()->SetTitle("Proton Final True Momentum [%]"); 
   NMomPr60->GetYaxis()->CenterTitle(true);
   NMomPr60->GetYaxis()->SetTitleSize(0.050);
   NMomPr60->GetYaxis()->SetLabelSize(0.050);
   NMomPr60->DrawCopy("HIST");
   TH1D *GMomPr60 = GMomPr->ProjectionY("GPr60",27,27);
   GMomPr60->SetLineColor(kGreen);
   GMomPr60->DrawCopy("HIST same");
   TH1D *SMomPr60 = SMomPr->ProjectionY("SPr60",27,27);
   SMomPr60->SetLineColor(kBlue);
   SMomPr60->DrawCopy("HIST same");

   TPaveText *t12 = new TPaveText(0.3, 0.95, 0.81, 1.0, "bINDC");
   t12->AddText("Protons at 60 deg");
   t12->SetTextFont(22);
   t12->SetTextSize(0.050);
   t12->SetFillColor(kWhite);
   t12->Draw();

   auto legend12 = new TLegend(0.70,0.15,0.92,0.25);
   legend12->AddEntry(NMomEl60," Nexus","l");
   legend12->AddEntry(GMomEl60," GFiber","l");
   legend12->AddEntry(SMomEl60," Solid","l");
   legend12->SetTextSize(0.045);
   legend12->SetTextFont(22);
   legend12->Draw();

   cout << endl;
   cout << "--------------------------------------------------" << endl;
   cout << endl;

c1->Print("~/Desktop/FCStudies/Plots/All.pdf");
c2->Print("~/Desktop/FCStudies/Plots/Profiles.pdf");

}
