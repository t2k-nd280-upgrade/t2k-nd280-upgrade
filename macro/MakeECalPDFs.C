
//
// TODO:
// - Inner / Outer bin --> attention to ECal local position (difference between my and nd280mc???)
// - test 2D mipem Vs emenergy/length --> do 1D if low stat
// - optimize binning
// - update code for FGD1 file --> variable fgd2reaction != 7
// - update code for all the ecals, local mom and costheta (brl ecal top,left,bottom,right)
//

void MakeECalPDFs(){
  //TString FGD, TString output){

  //TString FGD = "../files/SmearingFiles/ECal/test.root";
  TString FGD = "../files/SmearingFiles/ECal/test_FGD2_dsecal_entmom.root";
  TString output = "pdfs_dsecal.root";

  gStyle->SetOptStat(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  int nbins=50;
  int low=-60;
  int high=60;
  TFile *outf=new TFile(output.Data(),"RECREATE");
  DrawingTools *draw=new DrawingTools(FGD.Data(),false);
  //draw->DumpCuts();
  
  DataSample   *mc  =new DataSample(FGD.Data()); 
  
  TString treename="default";

  // Define variables
  TString dsecal_entr_mom = "sqrt( selmu_dsecal_entrance_mom[0]*selmu_dsecal_entrance_mom[0] + selmu_dsecal_entrance_mom[1]*selmu_dsecal_entrance_mom[1] + selmu_dsecal_entrance_mom[2]*selmu_dsecal_entrance_mom[2] )";
  TString dsecal_entr_costh = "(selmu_dsecal_entrance_mom[2]/"+dsecal_entr_mom+")";
  
  // Define cuts
  TString StdCutMuon = "(accum_level[][0]>-3 && particle == 13 && fgd2reaction != 7)"; // all events, muons, true in FV
  TString seldsecal = StdCutMuon+" && (selmu_ecal_det == 0)";  

  // Define binning
  const int NbinsMom = 8;
  const int NbinsCosTh = 4;
  const int Nbins = NbinsMom * NbinsCosTh;
  float BinMom[NbinsMom+1] = { 0,400,600,800,1000,1200,1500,2000,30000};
  float BinCosTh[NbinsCosTh+1] = { -1.,-0.5,0.,+0.5,+1.};

  TString strBinMom[NbinsMom+1];
  for(int imom=0;imom<NbinsMom+1;imom++){
    strBinMom[imom].Form("%1.0f",BinMom[imom]);
    //cout << strBinMom[imom] << endl;
  }
  
  TString strBinCosTh[NbinsCosTh+1];
  for(int icos=0;icos<NbinsCosTh+1;icos++){
    strBinCosTh[icos].Form("%1.1f",BinCosTh[icos]);
    //cout << strBinCosTh[icos] << endl;
  }
  
  TH2F *hBinning = new TH2F("hBinning","hBinning",NbinsCosTh,BinCosTh,NbinsMom,BinMom);

  TString nn_p[Nbins];
  TString opt[Nbins];
  int ibin=0;
  for(int imom=0;imom<NbinsMom;imom++){
    for(int icos=0;icos<NbinsCosTh;icos++){    
            
      opt[ibin] = seldsecal + " && (" + dsecal_entr_mom + ">=" + strBinMom[imom] + " && " + dsecal_entr_mom + "<" + strBinMom[imom+1] + ")" + " && (" + dsecal_entr_costh + ">=" + strBinCosTh[icos] + " && " + dsecal_entr_costh + "<" + strBinCosTh[icos+1] + ")";
      //cout << opt[ibin] << endl; 

      nn_p[ibin] = strBinMom[imom]+" <= p < "+strBinMom[imom+1]+" | "+strBinCosTh[icos]+" <= costh < "+strBinCosTh[icos+1];
      //cout << nn_p[ibin] << endl;

      ibin++;
    }
  }  
  
  
  TH2F *hECalMIPEM_Vs_EneLength[Nbins];
  TH1F *hECalMIPEM[Nbins];

  TH1F *hECalMIPEM_mom[Nbins],    *hECalMIPEM_cos[Nbins];
  TH1F *hECalEMenergy_mom[Nbins], *hECalEMenergy_cos[Nbins];
  TH1F *hECallength_mom[Nbins],   *hECallength_cos[Nbins];

  TH2F *hECalXY, TH2F *hECalXZ, TH2F *hECalYZ;

  // ///// PDFs of MIPEM (Mom Vs CosTheta)
  
  // for(int ih=0;ih<NbinsMom;ih++){
  //   std::cout<<" ih "<<ih<<" nn_p "<<nn_p[ih]<<std::endl;
  //   hECalMIPEM[ih]=new TH1F(Form("hh%s",nn_p[ih].Data()),"",nbins,low,high);
  //   TString scut=opt[ih];
  //   std::cout<<scut.Data()<<std::endl;
  //   std::cout<<treename.Data()<<std::endl;
  //   TTree *tree=(TTree*) mc->GetTree(treename.Data());
  //   tree->Project(hECalMIPEM[ih]->GetName(),"selmu_ecal_mipem",scut.Data());
  //   cout << "# of events = " << hECalMIPEM[ih]->GetEntries() << endl;
  //   cout << endl;
  //   hECalMIPEM[ih]->SetTitle(Form("%s ",nn_p[ih].Data()));
  //   hECalMIPEM[ih]->GetYaxis()->SetTitle("Entries");
  //   hECalMIPEM[ih]->GetXaxis()->SetTitle("ECal LLR MipEm");
  //   outf->cd();
  //   outf->cd(path.Data());
  //   hECalMIPEM[ih]->SetName(Form("mipem_%s",nn_p[ih].Data()));
  //   hECalMIPEM[ih]->Write();
  // }


  // ///// PDFs of EMenergy/TrackLength (Mom Vs CosTheta)
  
  // for(int ih=0;ih<NbinsMom;ih++){
  //   std::cout<<" ih "<<ih<<" nn_p "<<nn_p[ih]<<std::endl;
  //   hECalMIPEM[ih]=new TH1F(Form("hh%s",nn_p[ih].Data()),"",nbins,low,high);
  //   TString scut=opt[ih];
  //   std::cout<<scut.Data()<<std::endl;
  //   std::cout<<treename.Data()<<std::endl;
  //   TTree *tree=(TTree*) mc->GetTree(treename.Data());
  //   tree->Project(hECalMIPEM[ih]->GetName(),"selmu_ecal_mipem",scut.Data());
  //   cout << "# of events = " << hECalMIPEM[ih]->GetEntries() << endl;
  //   cout << endl;
  //   hECalMIPEM[ih]->SetTitle(Form("%s ",nn_p[ih].Data()));
  //   hECalMIPEM[ih]->GetYaxis()->SetTitle("Entries");
  //   hECalMIPEM[ih]->GetXaxis()->SetTitle("ECal LLR MipEm");
  //   outf->cd();
  //   outf->cd(path.Data());
  //   hECalMIPEM[ih]->SetName(Form("mipem_%s",nn_p[ih].Data()));
  //   hECalMIPEM[ih]->Write();
  // }



  ///// PDFs of MIPEM Vs EMenergy/TrackLength (Mom Vs CosTheta)
    
  int ih=0;
  for(int imom=0;imom<NbinsMom;imom++){
    for(int icos=0;icos<NbinsCosTh;icos++){    
      
      TString namehisto; namehisto.Form("mipem_Vs_EneOnL_%d",ih);

      cout << endl;
      std::cout<<"- ih "<<ih<<" nn_p "<<nn_p[ih]<<std::endl;
      
      // Fill binning histogram
      hBinning->SetBinContent(icos+1,imom+1,ih);
      
      double x = BinCosTh[icos] + (BinCosTh[icos+1] - BinCosTh[icos])/2.;
      double y = BinMom[imom]   + (BinMom[imom+1]   - BinMom[imom])/2.;
      int mybin = hBinning->FindBin(x,y);
      //cout << "costh = " << BinCosTh[icos] << " - " << BinCosTh[icos+1] << endl;
      //cout << "mom = " << BinMom[imom] << " - " << BinMom[imom+1] << endl;
      cout << " Find bin center: " << x << " , " << y << endl;
      //cout << " hBinning bin = " << mybin << endl;
      std::cout<<" hBinning content = " << hBinning->GetBinContent(mybin) << endl;
      std::cout<<" Name histo pdf: " << namehisto << endl;
      
      //
      
      hECalMIPEM_Vs_EneLength[ih]=new TH2F(Form("hh%s2D",nn_p[ih].Data()),"",nbins,low,high,nbins,-1,4);
      TString scut=opt[ih];
      std::cout<<scut.Data()<<std::endl;
      std::cout<<treename.Data()<<std::endl;
      TTree *tree=(TTree*) mc->GetTree(treename.Data());
      tree->Project(hECalMIPEM_Vs_EneLength[ih]->GetName(),"selmu_ecal_EMenergy/selmu_ecal_length:selmu_ecal_mipem",scut.Data());
      cout << "# of events = " << hECalMIPEM_Vs_EneLength[ih]->GetEntries() << endl;
      cout << endl;
      hECalMIPEM_Vs_EneLength[ih]->SetTitle(Form("%s ",nn_p[ih].Data()));
      hECalMIPEM_Vs_EneLength[ih]->GetYaxis()->SetTitle("Entries");
      hECalMIPEM_Vs_EneLength[ih]->GetXaxis()->SetTitle("ECal LLR MipEm");
      outf->cd();
      //outf->cd(path.Data());
      hECalMIPEM_Vs_EneLength[ih]->SetName(namehisto);
      hECalMIPEM_Vs_EneLength[ih]->Write();
      
      ih++;
    } 
  }
    
  hBinning->Write(); // Store p-costh binning histogram 
    

  
  ///// ECal entering position
  
  TTree *postree=(TTree*) mc->GetTree(treename.Data());

  outf->cd();

  hECalXY=new TH2F("hECalXY","hECalXY",800,-4000,4000,800,4000,-4000);
  postree->Project(hECalXY->GetName(),"selmu_dsecal_entrance_pos[1]:selmu_dsecal_entrance_pos[0]",seldsecal.Data());
  hECalXY->Write();

  hECalXZ=new TH2F("hECalXZ","hECalXZ",800,-4000,4000,800,4000,-4000);
  postree->Project(hECalXZ->GetName(),"selmu_dsecal_entrance_pos[2]:selmu_dsecal_entrance_pos[0]",seldsecal.Data());
  hECalXZ->Write();

  hECalYZ=new TH2F("hECalYZ","hECalYZ",800,-4000,4000,800,4000,-4000);
  postree->Project(hECalYZ->GetName(),"selmu_dsecal_entrance_pos[2]:selmu_dsecal_entrance_pos[1]",seldsecal.Data());
  hECalYZ->Write();
  
  TCanvas *cECalXY = new TCanvas("cECalXY","cECalXY");
  hECalXY->Draw("colz");
  TCanvas *cECalXZ = new TCanvas("cECalXZ","cECalXZ");
  hECalXZ->Draw("colz");
  TCanvas *cECalYZ = new TCanvas("cECalYZ","cECalYZ");
  hECalYZ->Draw("colz");





  // Do the same for EMenergy and track length
  //selmu_ecal_EMenergy
  //selmu_ecal_length
  //selmu_ecal_det
  //selmu_detectors


  // selmu_ecal_EMenergy
  // [1:39]  
  // selmu_ecal_det sarebbe l'ecal considerato (ds, top, bottom, etc)
  // [1:39]  
  // selmu_ecal_edeposit energia depositata
  // [1:39]  
  // selmu_ecal_mipem
  // [1:39]  
  // ci sarebbe anche selmu_ecal_mippion
  // [1:39]  
  // selmu_ecal_length che sarebbe la lunghezza del segmento in ecal (edited)
  // [1:41]  
  // selmu_dsecal_entrance_mom che è il tvector del momento vero in ingresso ad ecal
  // [1:41]  
  // selmu_dsecal_entrance_pos posizione vera dell'ingresso in ecal
  // [1:41]  
  // tutto è fatto solo per downstream ecal
  // [1:41]  
  // dimmi se va bene
  // [1:41]  
  // ah scusa
  // [1:42]  
  // per selezionare dsECal
  // [1:42]  
  // devi fare selmu_ecal_det == 0 quando plotti il TTree
  
  outf->Close();

}
