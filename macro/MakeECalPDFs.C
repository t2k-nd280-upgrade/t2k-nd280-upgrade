
//
// TODO:
// - 2D binning {mom,costheta_loc}
// - Inner / Outer bin 
// - test 2D mipem Vs emenergy/length --> do 1D if low stat
// - optimize binning
// - update code for FGD1 file --> variable fgd2reaction != 7

void MakeECalPDFs(){
  //TString FGD, TString output){

  //TString FGD = "../files/SmearingFiles/ECal/test.root";
  TString FGD = "../files/SmearingFiles/ECal/test_FGD2_dsecal_entmom.root";
  TString output = "ciao.root";

  gStyle->SetOptStat(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  int nbins=50;
  int low=-60;
  int high=60;
  TFile *outf=new TFile(output.Data(),"RECREATE");
  DrawingTools *draw=new DrawingTools(FGD.Data(),false);
  
  draw->DumpCuts();
  
  DataSample   *mc  =new DataSample(FGD.Data()); 
  TString path="MomBins";
  outf->mkdir(path.Data(),path.Data());
  
  // Define variables
  TString dsecal_entr_mom = "sqrt( selmu_dsecal_entrance_mom[0]*selmu_dsecal_entrance_mom[0] + selmu_dsecal_entrance_mom[1]*selmu_dsecal_entrance_mom[1] + selmu_dsecal_entrance_mom[2]*selmu_dsecal_entrance_mom[2] )";
  TString dsecal_entr_costh = "(selmu_dsecal_entrance_mom[2]/"+dsecal_entr_mom+")";
  
  // Define cuts
  TString StdCutMuon = "(accum_level[][0]>-1 && particle == 13 && fgd2reaction != 7)"; // all events, muons, true in FV
  TString seldsecal = "(selmu_ecal_det == 0)";
  
  TString treename="default";
  const int NbinsMom = 32;
  TString opt[NbinsMom]={
 
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=0    && "+dsecal_entr_mom+"<400) && ("+dsecal_entr_costh+">=-1   && "+dsecal_entr_costh+"<-0.5)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=0    && "+dsecal_entr_mom+"<400) && ("+dsecal_entr_costh+">=-0.5 && "+dsecal_entr_costh+"<0.)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=0    && "+dsecal_entr_mom+"<400) && ("+dsecal_entr_costh+">=0.   && "+dsecal_entr_costh+"<0.5)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=0    && "+dsecal_entr_mom+"<400) && ("+dsecal_entr_costh+">=0.5  && "+dsecal_entr_costh+"<1.)",

    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=400  && "+dsecal_entr_mom+"<600) && ("+dsecal_entr_costh+">=-1   && "+dsecal_entr_costh+"<-0.5)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=400  && "+dsecal_entr_mom+"<600) && ("+dsecal_entr_costh+">=-0.5 && "+dsecal_entr_costh+"<0.)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=400  && "+dsecal_entr_mom+"<600) && ("+dsecal_entr_costh+">=0.   && "+dsecal_entr_costh+"<0.5)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=400  && "+dsecal_entr_mom+"<600) && ("+dsecal_entr_costh+">=0.5  && "+dsecal_entr_costh+"<1.)",

    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=600  && "+dsecal_entr_mom+"<800) && ("+dsecal_entr_costh+">=-1   && "+dsecal_entr_costh+"<-0.5)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=600  && "+dsecal_entr_mom+"<800) && ("+dsecal_entr_costh+">=-0.5 && "+dsecal_entr_costh+"<0.)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=600  && "+dsecal_entr_mom+"<800) && ("+dsecal_entr_costh+">=0.   && "+dsecal_entr_costh+"<0.5)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=600  && "+dsecal_entr_mom+"<800) && ("+dsecal_entr_costh+">=0.5  && "+dsecal_entr_costh+"<1.)",

    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=800  && "+dsecal_entr_mom+"<1000)&& ("+dsecal_entr_costh+">=-1.  && "+dsecal_entr_costh+"<-0.5)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=800  && "+dsecal_entr_mom+"<1000)&& ("+dsecal_entr_costh+">=-0.5 && "+dsecal_entr_costh+"<0.)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=800  && "+dsecal_entr_mom+"<1000)&& ("+dsecal_entr_costh+">=0.   && "+dsecal_entr_costh+"<0.5)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=800  && "+dsecal_entr_mom+"<1000)&& ("+dsecal_entr_costh+">=0.5  && "+dsecal_entr_costh+"<1.)",

    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=1000  && "+dsecal_entr_mom+"<1200)&& ("+dsecal_entr_costh+">=-1.  && "+dsecal_entr_costh+"<-0.5)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=1000  && "+dsecal_entr_mom+"<1200)&& ("+dsecal_entr_costh+">=-0.5 && "+dsecal_entr_costh+"<0.)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=1000  && "+dsecal_entr_mom+"<1200)&& ("+dsecal_entr_costh+">=0.   && "+dsecal_entr_costh+"<0.5)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=1000  && "+dsecal_entr_mom+"<1200)&& ("+dsecal_entr_costh+">=0.5  && "+dsecal_entr_costh+"<1.)",

    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=1200  && "+dsecal_entr_mom+"<1500)&& ("+dsecal_entr_costh+">=-1.  && "+dsecal_entr_costh+"<-0.5)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=1200  && "+dsecal_entr_mom+"<1500)&& ("+dsecal_entr_costh+">=-0.5 && "+dsecal_entr_costh+"<0.)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=1200  && "+dsecal_entr_mom+"<1500)&& ("+dsecal_entr_costh+">=0.   && "+dsecal_entr_costh+"<0.5)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=1200  && "+dsecal_entr_mom+"<1500)&& ("+dsecal_entr_costh+">=0.5  && "+dsecal_entr_costh+"<1.)",

    StdCutMuon+"&&"+seldsecal+ "&& ("+dsecal_entr_mom+">=1500  && "+dsecal_entr_mom+"<2000)&& ("+dsecal_entr_costh+">=-1.  && "+dsecal_entr_costh+"<-0.5)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=1500  && "+dsecal_entr_mom+"<2000)&& ("+dsecal_entr_costh+">=-0.5 && "+dsecal_entr_costh+"<0.)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=1500  && "+dsecal_entr_mom+"<2000)&& ("+dsecal_entr_costh+">=0.   && "+dsecal_entr_costh+"<0.5)",
    StdCutMuon+"&&"+seldsecal+ "&& ("+dsecal_entr_mom+">=1500  && "+dsecal_entr_mom+"<2000)&& ("+dsecal_entr_costh+">=0.5  && "+dsecal_entr_costh+"<1.)",

    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=2000) && ("+dsecal_entr_costh+">=-1.  && "+dsecal_entr_costh+"<-0.5)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=2000) && ("+dsecal_entr_costh+">=-0.5 && "+dsecal_entr_costh+"<0.)",
    StdCutMuon+"&&"+seldsecal+" && ("+dsecal_entr_mom+">=2000) && ("+dsecal_entr_costh+">=0.   && "+dsecal_entr_costh+"<0.5)",
    StdCutMuon+"&&"+seldsecal+ "&& ("+dsecal_entr_mom+">=2000) && ("+dsecal_entr_costh+">=0.5  && "+dsecal_entr_costh+"<1.)"
  };
  TString nn_p[NbinsMom]={
    "p=0,400, costh=-1,-0.5",
    "p=0,400, costh=-0.5,0",
    "p=0,400, costh=0,0.5",
    "p=0,400, costh=0.5,1",

    "p=400,600, costh=-1,-0.5",
    "p=400,600, costh=-0.5,0",
    "p=400,600, costh=0,0.5",
    "p=400,600, costh=0.5,1",

    "p=600,800, costh=-1,-0.5",
    "p=600,800, costh=-0.5,0",
    "p=600,800, costh=0,0.5",
    "p=600,800, costh=0.5,1",

    "p=800,1000, costh=-1,-0.5",
    "p=800,1000, costh=-0.5,0",
    "p=800,1000, costh=0,0.5",
    "p=800,1000, costh=0.5,1",

    "p=1000,1200, costh=-1,-0.5",
    "p=1000,1200, costh=-0.5,0",
    "p=1000,1200, costh=0,0.5",
    "p=1000,1200, costh=0.5,1",

    "p=1200,1500, costh=-1,-0.5",
    "p=1200,1500, costh=-0.5,0",
    "p=1200,1500, costh=0,0.5",
    "p=1200,1500, costh=0.5,1",

    "p=1500,2000, costh=-1,-0.5",
    "p=1500,2000, costh=-0.5,0",
    "p=1500,2000, costh=0,0.5",
    "p=1500,2000, costh=0.5,1",

    "p>2000, costh=-1,-0.5",
    "p>2000, costh=-0.5,0",
    "p>2000, costh=0,0.5",
    "p>2000, costh=0.5,1"
   };

  TH2F *hECalMIPEM_Vs_EneLength[NbinsMom];
  TH1F *hECalMIPEM[NbinsMom];

  TH1F *hECalMIPEM_mom[NbinsMom],    *hECalMIPEM_cos[NbinsMom];
  TH1F *hECalEMenergy_mom[NbinsMom], *hECalEMenergy_cos[NbinsMom];
  TH1F *hECallength_mom[NbinsMom],   *hECallength_cos[NbinsMom];





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
    
  for(int ih=0;ih<NbinsMom;ih++){
    std::cout<<" ih "<<ih<<" nn_p "<<nn_p[ih]<<std::endl;
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
    outf->cd(path.Data());
    hECalMIPEM_Vs_EneLength[ih]->SetName(Form("mipem_%s",nn_p[ih].Data()));
    hECalMIPEM_Vs_EneLength[ih]->Write();
  } 



  
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
