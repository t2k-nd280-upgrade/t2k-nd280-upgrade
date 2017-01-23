void makeplots(TString FGD, TString output){

  gStyle->SetOptStat(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  int nbins=50;
  int low=-60;
  int high=60;
  TFile *outf=new TFile(output.Data(),"UPDATE");
  DrawingTools *draw=new DrawingTools(FGD.Data(),false);
  DataSample   *mc  =new DataSample(FGD.Data()); 
  TString path="MomBins";
  outf->mkdir(path.Data(),path.Data());

  TH1F *hECalMIPEM_mom[4], *hECalMIPEM_cos[4];

  TString treename="default";
  TString opt_mom[4]={"accum_level[][0]>-1 && particle == 13 && selmu_ecal_det == 0 && (selmu_truemom>=0 && selmu_truemom<300)","accum_level[][0]>-1 && particle == 13 && selmu_ecal_det == 0 && (selmu_truemom>=300 && selmu_truemom<600)","accum_level[][0]>-1 && particle == 13 && selmu_ecal_det == 0 && (selmu_truemom>=600 && selmu_truemom<1000)","accum_level[][0]>-1 && particle == 13 && selmu_ecal_det == 0 && (selmu_truemom>=1000 && selmu_truemom<2000)"};

  TString opt_cos[4]={"(accum_level[][0]>-1 && particle == 13 && selmu_ecal_det == 0 && (selmu_truedir[2]>=-1 && selmu_truedir[2]<0))","(accum_level[][0]>-1 && particle == 13 && selmu_ecal_det == 0 && (selmu_truedir[2]>=0 && selmu_truedir[2]<0.4))","(accum_level[][0]>-1 && particle == 13 && selmu_ecal_det == 0 && (selmu_truedir[2]>=0.4 && selmu_truedir[2]<0.8))","(accum_level[][0]>-1 && particle == 13 && selmu_ecal_det == 0 && (selmu_truedir[2]>=0.8 && selmu_truedir[2]<1))"};


  TString nn_p[4]={"0-300","300-600","600-1000","1000-2000"};
  TString nn_c[4]={"-1-0","0-0.4","0.4-0.8","0.8-1"};

  for(int ih=0;ih<4;ih++){
    std::cout<<" ih "<<ih<<" nn_p "<<nn_p[ih]<<std::endl;
    hECalMIPEM_mom[ih]=new TH1F(Form("hh%s",nn_p[ih].Data()),"",nbins,low,high);
    TString scut=opt_mom[ih];
    std::cout<<scut.Data()<<std::endl;
    std::cout<<treename.Data()<<std::endl;
    TTree *tree=(TTree*) mc->GetTree(treename.Data());
    tree->Project(hECalMIPEM_mom[ih]->GetName(),"selmu_ecal_mipem",scut.Data());
    hECalMIPEM_mom[ih]->SetTitle(Form("%s ",nn_p[ih].Data()));
    hECalMIPEM_mom[ih]->GetYaxis()->SetTitle("Entries");
    hECalMIPEM_mom[ih]->GetXaxis()->SetTitle("ECal LLR MipEm");
    outf->cd();
    outf->cd(path.Data());
    hECalMIPEM_mom[ih]->SetName(Form("mipem_%s",nn_p[ih].Data()));
    hECalMIPEM_mom[ih]->Write();
  }

  TString path1="CosThetaBins";
  outf->mkdir(path1.Data(),path1.Data());

  for(int ih=0;ih<4;ih++){
    std::cout<<" ih "<<ih<<" nn_c "<<nn_c[ih]<<std::endl;
    hECalMIPEM_cos[ih]=new TH1F(Form("hh%s",nn_c[ih].Data()),"",nbins,low,high);
    TString scut=opt_cos[ih];
    std::cout<<scut.Data()<<std::endl;
    std::cout<<treename.Data()<<std::endl;
    TTree *tree=(TTree*) mc->GetTree(treename.Data());
    tree->Project(hECalMIPEM_cos[ih]->GetName(),"selmu_ecal_mipem",scut.Data());
    hECalMIPEM_mom[ih]->SetTitle(Form("%s ",nn_c[ih].Data()));
    hECalMIPEM_mom[ih]->GetYaxis()->SetTitle("Entries");
    hECalMIPEM_mom[ih]->GetXaxis()->SetTitle("ECal LLR MipEm");
    outf->cd();
    outf->cd(path1.Data());
    hECalMIPEM_cos[ih]->SetName(Form("mipem_%s",nn_c[ih].Data()));
    hECalMIPEM_cos[ih]->Write();
  }

  
  outf->Close();


}
