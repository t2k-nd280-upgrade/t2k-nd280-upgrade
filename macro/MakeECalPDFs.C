
//
// TODO:
// - 2D binning {mom,costheta_loc}
// - Inner / Outer bin 
// - test 3D mipem/emenergy/length --> do 1D if low stat
// - optimize binning
//

void MakeECalPDFs(){
  //TString FGD, TString output){

  TString FGD = "/neutrino/data7/davide/files/ND280Upgrade/SmearingFiles/ECal/test.root";
  TString output = "ciao.root";

  gStyle->SetOptStat(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  int nbins=50;
  int low=-60;
  int high=60;
  TFile *outf=new TFile(output.Data(),"RECREATE");
  DrawingTools *draw=new DrawingTools(FGD.Data(),false);
  DataSample   *mc  =new DataSample(FGD.Data()); 
  TString path="MomBins";
  outf->mkdir(path.Data(),path.Data());

  TH1F *hECalMIPEM_mom[4],    *hECalMIPEM_cos[4];
  TH1F *hECalEMenergy_mom[4], *hECalEMenergy_cos[4];
  TH1F *hECallength_mom[4],   *hECallength_cos[4];
  
  TString treename="default";
  const int NbinsMom = 11;
  TString opt_mom[NbinsMom]={
    "accum_level[][0]>-1 && selvtx_truepos[2] > 2665 && particle == 13 && selmu_ecal_det == 0 && (selmu_truemom>=0    && selmu_truemom<300)",
    "accum_level[][0]>-1 && selvtx_truepos[2] > 2665 && particle == 13 && selmu_ecal_det == 0 && (selmu_truemom>=300  && selmu_truemom<400)",
    "accum_level[][0]>-1 && selvtx_truepos[2] > 2665 && particle == 13 && selmu_ecal_det == 0 && (selmu_truemom>=400  && selmu_truemom<500)",
    "accum_level[][0]>-1 && selvtx_truepos[2] > 2665 && particle == 13 && selmu_ecal_det == 0 && (selmu_truemom>=500  && selmu_truemom<2000)"
    "accum_level[][0]>-1 && selvtx_truepos[2] > 2665 && particle == 13 && selmu_ecal_det == 0 && (selmu_truemom>=600  && selmu_truemom<700)",
    "accum_level[][0]>-1 && selvtx_truepos[2] > 2665 && particle == 13 && selmu_ecal_det == 0 && (selmu_truemom>=700  && selmu_truemom<800)",
    "accum_level[][0]>-1 && selvtx_truepos[2] > 2665 && particle == 13 && selmu_ecal_det == 0 && (selmu_truemom>=800  && selmu_truemom<900)",
    "accum_level[][0]>-1 && selvtx_truepos[2] > 2665 && particle == 13 && selmu_ecal_det == 0 && (selmu_truemom>=900  && selmu_truemom<1000)",
    "accum_level[][0]>-1 && selvtx_truepos[2] > 2665 && particle == 13 && selmu_ecal_det == 0 && (selmu_truemom>=1000 && selmu_truemom<1200)",
    "accum_level[][0]>-1 && selvtx_truepos[2] > 2665 && particle == 13 && selmu_ecal_det == 0 && (selmu_truemom>=1500 && selmu_truemom<2000)",
    "accum_level[][0]>-1 && selvtx_truepos[2] > 2665 && particle == 13 && selmu_ecal_det == 0 && (selmu_truemom>=2000)"
  };
  TString nn_p[NbinsMom]={
    "0-300",
    "300-400",
    "400-500",
    "500-600"
    // "600-700",
    // "700-800",
    // "800-900",
    // "900-1000",
    // "1000-1200",
    // "1500-2000",
    // ">2000"
  };

  const int NbinsCosTh = 4;
  TString opt_cos[NbinsCosTh]={
    "(accum_level[][0]>-1 && particle == 13 && selmu_ecal_det == 0 && (selmu_truedir[2]>=-1 && selmu_truedir[2]<0))",
    "(accum_level[][0]>-1 && particle == 13 && selmu_ecal_det == 0 && (selmu_truedir[2]>=0 && selmu_truedir[2]<0.4))",
    "(accum_level[][0]>-1 && particle == 13 && selmu_ecal_det == 0 && (selmu_truedir[2]>=0.4 && selmu_truedir[2]<0.8))",
    "(accum_level[][0]>-1 && particle == 13 && selmu_ecal_det == 0 && (selmu_truedir[2]>=0.8 && selmu_truedir[2]<1))"
  };  
  TString nn_c[NbinsCosTh]={
    "-1-0",
    "0-0.4",
    "0.4-0.8",
    "0.8-1"
  };


  ///// PDFs of Mom
  
  for(int ih=0;ih<NbinsMom;ih++){
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



  ///// PDFs of CosTheta

  TString path1="CosThetaBins";
  outf->mkdir(path1.Data(),path1.Data());

  for(int ih=0;ih<NbinsCosTh;ih++){
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

  
  // Do the same for EMenergy and track length
  //selmu_ecal_EMenergy
  //selmu_ecal_length
  //selmu_ecal_det
  //selmu_detectors


  outf->Close();


}
