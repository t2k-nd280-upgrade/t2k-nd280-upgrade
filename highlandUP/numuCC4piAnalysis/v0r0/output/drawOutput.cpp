///==================================================================================================================================================================
/// Author: Cesar Jesus
/// cjesus@ifae.es
/// numuCC4piMultiPi_Studies
///==================================================================================================================================================================
{   ///==============================================================================================================================================================
    ///===Initialize DrawingTools                         
    DrawingTools* draw = new DrawingTools("/nfs/neutrinos/cjesus/work/jobs/files/TDR_eff/Target1/config2_Target1_FHC_numu.root");
    draw->SetStackFillStyle(3254);
    gStyle->SetPadBorderMode(0);
    gROOT->ForceStyle();

    TString prefix = "/nfs/neutrinos/cjesus/work/";
    
    ///===Initialize Experiment class 
    Experiment exp("nd280");
    ///===Magnet MC samples
    DataSample* mc = new DataSample("/nfs/neutrinos/cjesus/work/jobs/files/TDR_eff/Target1/config2_Target1_FHC_numu.root");
    ///===Run6 sample
    SampleGroup run6("run6");
    run6.AddMCSample("magnet_neut",mc);
    exp.AddSampleGroup("run6",run6);        
    
    ///===============================================================================================================================================================      
    ///===Creating root file
    TFile *file1 = new TFile(prefix + "numuCC4piMultiPi_others.root","RECREATE");
    ///===============================================================================================================================================================      
    
    TCanvas *study1 = new TCanvas("study1","Efficiency",0,0,1200,800);
    study1->Divide(2,1);
    study1->cd(1);
    draw->SetLegendPos("tr");
    draw.SetTitleY("numuCC#pi^{+} Selection Efficiency1");
    draw.SetTitleX("truelepton_mom [GeV/c]");
    draw.DrawEff(exp,false,"truelepton_mom",100,0,5,"accum_level>7","topology==1","","","TOTAL");
    draw.DrawEff(exp,false,"truelepton_mom",100,0,5,"accum_level[0]>7","topology==1","same","","FWD");
//  draw.DrawEff(exp,false,"truelepton_mom/1000",100,0,5,"accum_level[1]>7","topology==1","same","","BWD");
//  draw.DrawEff(exp,false,"truelepton_mom/1000",100,0,5,"accum_level[2]>7","topology==1","same","","HAFWD");
//  draw.DrawEff(exp,false,"truelepton_mom/1000",100,0,5,"accum_level[3]>7","topology==1","same","","HABWD");
    study1->cd(2);
    draw->SetLegendPos("tl");
    draw.SetTitleY("numuCC#pi^{+} Selection Efficiency");
    draw.SetTitleX("truelepton_costheta");
    draw.DrawEff(exp,false,"truelepton_costheta",100,-1,1,"accum_level>7","topology==1","","","TOTAL");
    draw.DrawEff(exp,false,"truelepton_costheta",100,-1,1,"&& accum_level[0]>7","topology==1","same","","FWD");
//  draw.DrawEff(exp,false,"truelepton_costheta",100,-1,1,"accum_level[1]>7","topology==1","same","","BWD");
//  draw.DrawEff(exp,false,"truelepton_costheta",100,-1,1,"accum_level[2]>7","topology==1","same","","HAFWD");
//  draw.DrawEff(exp,false,"truelepton_costheta",100,-1,1,"accum_level[3]>7","topology==1","same","","HABWD");
    study1->Print(prefix + "Efficiency1.eps");
    study1->Write();
    study1->Close();
    
//     TCanvas *study2 = new TCanvas("study2","Efficiency2",0,0,1200,800);
//     study2->Divide(2,1);
//     study2->cd(1);
//     draw->SetLegendPos("tr");
//     draw.SetTitleY("numuCC#pi^{+} Selection Efficiency");
//     draw.SetTitleX("PosPionTrueMom [GeV/c]");
//     draw.DrawEff(exp,false,"PosPionTrueMom/1000",100,0,5,"accum_level>7","topology==1","","","TOTAL");
//     draw.DrawEff(exp,false,"PosPionTrueMom/1000",100,0,5,"accum_level[0]>7","topology==1","same","","FWD");
// //  draw.DrawEff(exp,false,"PosPionTrueMom/1000",100,0,5,"accum_level[1]>7","topology==1","same","","BWD");
// //  draw.DrawEff(exp,false,"PosPionTrueMom/1000",100,0,5,"accum_level[2]>7","topology==1","same","","HAFWD");
// //  draw.DrawEff(exp,false,"PosPionTrueMom/1000",100,0,5,"accum_level[3]>7","topology==1","same","","HABWD");
//     study2->cd(2);
//     draw->SetLegendPos("tl");
//     draw.SetTitleY("numuCC#pi^{+} Selection Efficiency");
//     draw.SetTitleX("PosPionTrueStartDir");
//     draw.DrawEff(exp,false,"PosPionTrueStartDir[2]",100,-1,1,"accum_level>7","topology==1","","","TOTAL");
//     draw.DrawEff(exp,false,"PosPionTrueStartDir[2]",100,-1,1,"(accum_level[0]>7","topology==1","same","","FWD");
// //  draw.DrawEff(exp,false,"PosPionTrueStartDir[2]",100,-1,1,"accum_level[1]>7","topology==1","same","","BWD");
// //  draw.DrawEff(exp,false,"PosPionTrueStartDir[2]",100,-1,1,"accum_level[2]>7","topology==1","same","","HAFWD");
// //  draw.DrawEff(exp,false,"PosPionTrueStartDir[2]",100,-1,1,"accum_level[3]>7","topology==1","same","","HABWD");
//     study2->Print(prefix + "Efficiency2.eps");
//     study2->Write();
//     study2->Close();
    
//     TCanvas *study3 = new TCanvas("study3","Efficiency3",0,0,1200,800);
//     study3->Divide(2,1);
//     study3->cd(1);
//     draw->SetLegendPos("tr");
//     draw.SetTitleY("numuCC#pi^{+} Selection Efficiency");
//     draw.SetTitleX("truepi_mom [GeV/c]");
//     draw.DrawEff(exp,false,"truepi_mom/1000",100,0,5,"accum_level>7","topology==1","","","TOTAL");
//     draw.DrawEff(exp,false,"truepi_mom/1000",100,0,5,"accum_level[0]>7","topology==1","same","","FWD");
// //  draw.DrawEff(exp,false,"truepi_mom/1000",100,0,5,"accum_level[1]>7","topology==1","same","","BWD");
// //  draw.DrawEff(exp,false,"truepi_mom/1000",100,0,5,"accum_level[2]>7","topology==1","same","","HAFWD");
// //  draw.DrawEff(exp,false,"truepi_mom/1000",100,0,5,"accum_level[3]>7","topology==1","same","","HABWD");
//     study3->cd(2);
//     draw->SetLegendPos("tl");
//     draw.SetTitleY("numuCC#pi^{+} Selection Efficiency");
//     draw.SetTitleX("truepi_costheta");
//     draw.DrawEff(exp,false,"truepi_costheta",100,-1,1,"accum_level>7","topology==1","","","TOTAL");
//     draw.DrawEff(exp,false,"truepi_costheta",100,-1,1,"accum_level[0]>7","topology==1","same","","FWD");
// //  draw.DrawEff(exp,false,"truepi_costheta",100,-1,1,"accum_level[1]>7","topology==1","same","","BWD");
// //  draw.DrawEff(exp,false,"truepi_costheta",100,-1,1,"accum_level[2]>7","topology==1","same","","HAFWD");
// //  draw.DrawEff(exp,false,"truepi_costheta",100,-1,1,"accum_level[3]>7","topology==1","same","","HABWD");
//     study3->Print(prefix + "Efficiency3.eps");
//     study3->Write();
//     study3->Close();

//     TCanvas *study4 = new TCanvas("study4","Reco and true momentum",0,0,1200,800);
//     study4->Divide(3,1);
//     study4->cd(1);
//     draw.SetTitleY("truelepton_mom [GeV/c]");
//     draw.SetTitleX("selmu_mom [GeV/c]");
//     draw.Draw(exp,"truelepton_mom/1000:selmu_mom/1000",100,0,5,100,0,5,"all","accum_level[][1]>7","colz");
//     study4->cd(2);
//     draw.SetTitleY("PosPionTrueMom [GeV/c]");
//     draw.SetTitleX("PosPionMom [GeV/c]");
//     draw.Draw(exp,"PosPionTrueMom/1000:PosPionMom/1000",100,0,5,100,0,5,"all","accum_level[][1]>7","colz");
//     study4->cd(3);
//     draw.SetTitleY("truepi_mom [GeV/c]");
//     draw.SetTitleX("PosPionMom [GeV/c]");
//     draw.Draw(exp,"truepi_mom/1000:PosPionMom/1000",100,0,5,100,0,5,"all","accum_level[][1]>7","colz");
//     study4->Print(prefix + "RTMomentum.eps");
//     study4->Write();
//     study4->Close();

//     TCanvas *study5 = new TCanvas("study5","Momentum vs Angle",0,0,1200,800);
//     study5->Divide(3,2);
//     study5->cd(1);
//     draw.SetTitleX("truelepton_costheta");
//     draw.SetTitleY("truelepton_mom [GeV/c]");
//     draw.Draw(exp,"truelepton_mom/1000:truelepton_costheta",100,-1,1,100,0,5,"all","accum_level[][1]>7","colz");
//     study5->cd(2);
//     draw.SetTitleX("selmu_costheta");
//     draw.SetTitleY("selmu_mom [GeV/c]");
//     draw.Draw(exp,"selmu_mom/1000:selmu_costheta",100,-1,1,100,0,5,"all","accum_level[][1]>7","colz");
//     study5->cd(3);
//     draw.SetTitleX("truepi_costheta");
//     draw.SetTitleY("truepi_mom [GeV/c]");
//     draw.Draw(exp,"truepi_mom/1000:truepi_costheta",100,-1,1,100,0,5,"all","accum_level[][1]>7","colz");
//     study5->cd(4);
//     draw.SetTitleX("PosPionTrueStartDir");
//     draw.SetTitleY("PosPionTrueMom [GeV/c]");
//     draw.Draw(exp,"PosPionTrueMom/1000:PosPionTrueStartDir[2]",100,-1,1,100,0,5,"all","accum_level[][1]>7","colz");
//     study5->cd(5);
//     draw.SetTitleX("PosPionDir[2]");
//     draw.SetTitleY("PosPionMom [GeV/c]");
//     draw.Draw(exp,"PosPionMom/1000:PosPionDir[2]",100,-1,1,100,0,5,"all","accum_level[][1]>7","colz");
//     study5->cd(6);
//     study5->Print(prefix + "Momentum_vs_Angle.eps");
//     study5->Write();
//     study5->Close();

//     TCanvas *study6 = new TCanvas("study6","Muon vs. pion",0,0,1200,800);
//     study6->Divide(2,2);
//     study6->cd(1);
//     draw.SetTitleX("PosPionTrueMom [GeV/c]");
//     draw.SetTitleY("truelepton_mom [GeV/c]");
//     draw.Draw(exp,"truelepton_mom/1000:PosPionTrueMom/1000",100,0,5,100,0,5,"all","accum_level[][1]>7","colz");
//     study6->cd(2);
//     draw.SetTitleX("PosPionTrueStartDir");
//     draw.SetTitleY("truelepton_costheta");
//     draw.Draw(exp,"truelepton_costheta:PosPionTrueStartDir[2]",100,-1,1,100,-1,1,"all","accum_level[][1]>7","colz");
//     study6->cd(3);
//     draw.SetTitleX("truepi_mom [GeV/c]");
//     draw.SetTitleY("truelepton_mom [GeV/c]");
//     draw.Draw(exp,"truelepton_mom/1000:truepi_mom/1000",100,0,5,100,0,5,"all","accum_level[][1]>7","colz");
//     study6->cd(4);
//     draw.SetTitleX("truepi_costheta");
//     draw.SetTitleY("truelepton_costheta");
//     draw.Draw(exp,"truelepton_costheta:truepi_costheta",100,-1,1,100,-1,1,"all","accum_level[][1]>7","colz");
//     study6->Print(prefix + "Muon_vs_pion.eps");
//     study6->Write();
//     study6->Close();
    
//     TCanvas *study7 = new TCanvas("study7","Diference between Reco and true momentum",0,0,1200,800);
//     study7->Divide(3,2);
//     study7->cd(1);
//     draw.SetTitleX("selmu_mom - truelepton_mom [GeV/c]");
//     draw.Draw(exp,"selmu_mom/1000 - truelepton_mom/1000",100,-5,5,"topology","accum_level[][1]>7");
//     study7->cd(2);
//     draw.SetTitleX("PosPionMom - PosPionTrueMom [GeV/c]");
//     draw.Draw(exp,"PosPionMom/1000 - PosPionTrueMom/1000",100,-7,1,"topology","accum_level[][1]>7");
//     study7->cd(3);
//     draw.SetTitleX("PosPionMom - truepi_mom [GeV/c]");
//     draw.Draw(exp,"PosPionMom/1000 - truepi_mom/1000",100,-7,1,"topology","accum_level[][1]>7");
//     study7->cd(4);
//     draw.SetTitleX("truelepton_mom [GeV/c]");
//     draw.SetTitleY("selmu_mom - truelepton_mom [GeV/c]");
//     draw.Draw(exp,"(selmu_mom/1000 - truelepton_mom/1000):(truelepton_mom/1000)",100,0,5,100,-5,5,"all","accum_level[][1]>7","colz");
//     study7->cd(5);
//     draw.SetTitleX("PosPionTrueMom [GeV/c]");
//     draw.SetTitleY("PosPionMom - PosPionTrueMom [GeV/c]");
//     draw.Draw(exp,"(PosPionMom/1000 - PosPionTrueMom/1000):(PosPionTrueMom/1000)",100,0,5,100,-7,1,"all","accum_level[][1]>7","colz");
//     study7->cd(6);
//     draw.SetTitleX("truepi_mom [GeV/c]");
//     draw.SetTitleY("PosPionMom - truepi_mom [GeV/c]");
//     draw.Draw(exp,"(PosPionMom/1000 - truepi_mom/1000):(truepi_mom/1000)",100,0,5,100,-7,1,"all","accum_level[][1]>7","colz");
//     study7->Print(prefix + "Dif-Momentum.eps");
//     study7->Write();
//     study7->Close();    
    
//     TCanvas *study8 = new TCanvas("study8","Diference between Reco and true cosine",0,0,1200,800);
//     study8->Divide(3,2);
//     study8->cd(1);
//     draw.SetTitleX("selmu_costheta - truelepton_costheta");
//     draw.Draw(exp,"selmu_costheta - truelepton_costheta",100,-0.4,0.4,"topology","accum_level[][1]>7");
//     study8->cd(2);
//     draw.SetTitleX("PosPionDir[2] - PosPionTrueStartDir");
//     draw.Draw(exp,"PosPionDir[2] - PosPionTrueStartDir[2]",100,-0.4,0.4,"topology","accum_level[][1]>7");
//     study8->cd(3);
//     draw.SetTitleX("PosPionDir[2] - truepi_costheta");
//     draw.Draw(exp,"PosPionDir[2] - truepi_costheta",100,-0.4,0.4,"topology","accum_level[][1]>7");
//     study8->cd(4);
//     draw.SetTitleX("truelepton_costheta");
//     draw.SetTitleY("selmu_costheta - truelepton_costheta");
//     draw.Draw(exp,"(selmu_costheta - truelepton_costheta):(truelepton_costheta)",100,-1,1,100,-0.4,0.4,"all","accum_level[][1]>7","colz");
//     study8->cd(5);
//     draw.SetTitleX("PosPionTrueStartDir");
//     draw.SetTitleY("PosPionDir[2] - PosPionTrueStartDir");
//     draw.Draw(exp,"(PosPionDir[2] - PosPionTrueStartDir[2]):(PosPionTrueStartDir[2])",100,-1,1,100,-0.4,0.4,"all","accum_level[][1]>7","colz");
//     study8->cd(6);
//     draw.SetTitleX("truepi_costheta");
//     draw.SetTitleY("PosPionDir[2] - truepi_costheta");
//     draw.Draw(exp,"(PosPionDir[2] - truepi_costheta):(truepi_costheta)",100,-1,1,100,-0.4,0.4,"all","accum_level[][1]>7","colz");
//     study8->Print(prefix + "Dif-Cosine.eps");
//     study8->Write();
//     study8->Close();        

//     TCanvas *study9 = new TCanvas("study9","Neutrino energy reco",0,0,1200,800);
//     study9->Divide(2,1);
//     study9->cd(1);
// //  draw.SetTitleX("Reconstructed neutrino energy"); 
// //  draw.Draw(exp,"(0.0006128705-(0.93856538*(sqrt(((truelepton_mom/1000)*(truelepton_mom/1000))+0.0111636915)+sqrt(((truepi_mom/1000)*(truepi_mom/1000))+0.0194798351))))/(sqrt(((truelepton_mom/1000)*(truelepton_mom/1000))+0.0111636915)+sqrt(((truepi_mom/1000)*(truepi_mom/1000))+0.0194798351)-((truelepton_mom/1000)*truelepton_costheta)-((truepi_mom/1000)*truepi_costheta)-0.93856538)",100,0,5,"topology","accum_level>7");
// //  study9->cd(2);
//     draw.SetTitleX("Reconstructed neutrino energy");
//     draw.Draw(exp,"(0.0006128705-(0.93856538*(sqrt(((truelepton_mom/1000)*(truelepton_mom/1000))+0.0111636915)+sqrt(((truepi_mom/1000)*(truepi_mom/1000))+0.0194798351))))/(sqrt(((truelepton_mom/1000)*(truelepton_mom/1000))+0.0111636915)+sqrt(((truepi_mom/1000)*(truepi_mom/1000))+0.0194798351)-((truelepton_mom/1000)*truelepton_costheta)-0.93856538)",200,0,10,"topology","accum_level>7");
// //  study9->cd(3);
// //  draw.SetTitleX("Reconstructed neutrino energy");
// //  draw.Draw(exp,"((0.8803544323-(0.938272046-0.024-sqrt((truelepton_mom/1000)*(truelepton_mom/1000)+0.0111636915)-sqrt((truepi_mom/1000)*(truepi_mom/1000)+0.0111636915))*(0.938272046-0.024-sqrt((truelepton_mom/1000)*(truelepton_mom/1000)+0.0111636915)-sqrt((truepi_mom/1000)*(truepi_mom/1000)+0.0111636915)))/(2*(0.938272046-0.024-sqrt((truelepton_mom/1000)*(truelepton_mom/1000)+0.0111636915)-sqrt((truepi_mom/1000)*(truepi_mom/1000)+0.0111636915)+(truelepton_mom/1000)+(truepi_mom/1000))))",100,0,5,"topology","accum_level>7");
//     study9->cd(2);
//     draw.SetTitleX("Tranfered momentum");
//     draw.Draw(exp,"((truelepton_mom/1000)-(0.0006128705-(0.93856538*(sqrt(((truelepton_mom/1000)*(truelepton_mom/1000))+0.0111636915)+sqrt(((truepi_mom/1000)*(truepi_mom/1000))+0.0194798351))))/(sqrt(((truelepton_mom/1000)*(truelepton_mom/1000))+0.0111636915)+sqrt(((truepi_mom/1000)*(truepi_mom/1000))+0.0194798351)-((truelepton_mom/1000)*truelepton_costheta)-((truepi_mom/1000)*truepi_costheta)-0.93856538))*((truelepton_mom/1000) - (0.0006128705-(0.93856538*(sqrt(((truelepton_mom/1000)*(truelepton_mom/1000))+0.0111636915)+sqrt(((truepi_mom/1000)*(truepi_mom/1000))+0.0194798351))))/(sqrt(((truelepton_mom/1000)*(truelepton_mom/1000))+0.0111636915)+sqrt(((truepi_mom/1000)*(truepi_mom/1000))+0.0194798351)-((truelepton_mom/1000)*truelepton_costheta)-((truepi_mom/1000)*truepi_costheta)-0.93856538))",100,0,5,"topology","accum_level>7");
//     study9->Print(prefix + "Nu-energy.eps");
//     study9->Write();
//     study9->Close();            

//     draw.PrintPurities(exp,"particle","accum_level>7"); 
//     draw.PrintPurities(exp,"reaction","accum_level[][0]>7");
//     draw.PrintPurities(exp,"reaction","accum_level[][1]>7");
//     draw.PrintPurities(exp,"reaction","accum_level[][2]>7");
//     draw.PrintPurities(exp,"reaction","accum_level[][3]>7");
//     draw.PrintPurities(exp,"reaction","accum_level[][4]>7");
//     draw.PrintPurities(exp,"reaction","accum_level[][5]>7");
//     draw.PrintPurities(exp,"reaction","accum_level>7 && topology==0");
//     draw.PrintPurities(exp,"reaction","accum_level>7 && topology==1");
//     draw.PrintPurities(exp,"reaction","accum_level>7 && topology==2");
    
    ///===============================================================================================================================================================
    file1->Close();
}
