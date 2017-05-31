void nuePlots(Int_t target=7, std::string MCSelectionFile="../all11_ToFRes150.root"){//all11_ToFRes150.root

  bool drawarrows = true;

  TFile *FileMC = new TFile(MCSelectionFile.c_str(), "READ");

  // Colour code
  const Int_t pcolour = 6;
  Int_t parcolour[pcolour]      = {2 ,                3,                   6 ,                  4 ,                8,                48};
  std::string parname[pcolour]  = {"e-/e+",           "#mu-/#mu+",         "#pi-/#pi+",         "K-/K+",           "proton",         "other"};
  Int_t topcolour[pcolour]      = {6 ,                7,                   4 ,                  3 ,                5,                1};
  std::string toponame[pcolour] = {"#nu_{e} CC-0#pi", "#nu_{e} CC-Other",  "#gamma background", "#mu background",  "OOFV #gamma background", "Other background"};
  const Int_t icolour = 10;
  Int_t intcolour[icolour]      = {6 , 7 , 5 , 3 , 1 , 2 , 8, 48, 18, 25};

  // POT run for this MC
  const Float_t POT = 1e21;

  // Truth tree
  TTree *TruthSignal = (TTree*)FileMC->Get("truth");

  Float_t true_mom, true_costheta, true_Nu_mom;
  Int_t detector;
  TruthSignal->SetBranchAddress("true_mom",               &true_mom);
  TruthSignal->SetBranchAddress("true_costheta",          &true_costheta);
  TruthSignal->SetBranchAddress("true_Nu_mom",            &true_Nu_mom);
  TruthSignal->SetBranchAddress("detector",               &detector);

  // Selection tree
  TTree *TreeSignal  = (TTree*)FileMC->Get("default");
  Int_t selmu_longestTPC, accum_level, NNearTracks, NFarTracks;
  Float_t selmu_mom, selmu_costheta, selmu_charge;
  Float_t selmu_likemu, selmu_likeel, selmu_likepi, selmu_ecal_mipem, selmu_ToF_mass, selmu_ToF_lkl_muon, selmu_ToF_lkl_pion, selmu_ToF_lkl_electron, selmu_ToF_lkl_proton;
  Int_t selmu_PDG, selmu_ParentPDG, selmu_GParentPDG, nu_truereac;
  Float_t selmu_truemom, selmu_truecostheta, truelepton_mom;
  Int_t nueCC, nu_pdg;
  Float_t PairTrack_InvMass, TPCVeto_Deltaz, ECalVeto_Deltaz;
  
  TreeSignal->SetBranchAddress("selmu_mom",               &selmu_mom);
  TreeSignal->SetBranchAddress("selmu_costheta",          &selmu_costheta);
  TreeSignal->SetBranchAddress("selmu_charge",            &selmu_charge);
  TreeSignal->SetBranchAddress("selmu_longestTPC",        &selmu_longestTPC);
  TreeSignal->SetBranchAddress("selmu_likemu",            &selmu_likemu);
  TreeSignal->SetBranchAddress("selmu_likeel",            &selmu_likeel);
  TreeSignal->SetBranchAddress("selmu_likepi",            &selmu_likepi);
  TreeSignal->SetBranchAddress("selmu_ecal_mipem",        &selmu_ecal_mipem);
  TreeSignal->SetBranchAddress("selmu_ToF_mass",          &selmu_ToF_mass);
  TreeSignal->SetBranchAddress("selmu_PDG",               &selmu_PDG);
  TreeSignal->SetBranchAddress("selmu_ParentPDG",         &selmu_ParentPDG);
  TreeSignal->SetBranchAddress("selmu_GParentPDG",        &selmu_GParentPDG);
  TreeSignal->SetBranchAddress("selmu_truemom",           &selmu_truemom);
  TreeSignal->SetBranchAddress("selmu_truecostheta",      &selmu_truecostheta);
  TreeSignal->SetBranchAddress("nueCC",                   &nueCC);
  TreeSignal->SetBranchAddress("accum_level",             &accum_level);
  TreeSignal->SetBranchAddress("PairTrack_InvMass",       &PairTrack_InvMass);
  TreeSignal->SetBranchAddress("TPCVeto_Deltaz",          &TPCVeto_Deltaz);
  TreeSignal->SetBranchAddress("ECalVeto_Deltaz",         &ECalVeto_Deltaz);
  TreeSignal->SetBranchAddress("nu_truereac",             &nu_truereac);
  TreeSignal->SetBranchAddress("NNearTracks",             &NNearTracks);
  TreeSignal->SetBranchAddress("NFarTracks",              &NFarTracks);
  TreeSignal->SetBranchAddress("detector",                &detector);
  TreeSignal->SetBranchAddress("truelepton_mom",          &truelepton_mom);
  TreeSignal->SetBranchAddress("nu_pdg",                  &nu_pdg);
  TreeSignal->SetBranchAddress("selmu_ToF_lkl_muon",      &selmu_ToF_lkl_muon);
  TreeSignal->SetBranchAddress("selmu_ToF_lkl_pion",      &selmu_ToF_lkl_pion);
  TreeSignal->SetBranchAddress("selmu_ToF_lkl_electron",  &selmu_ToF_lkl_electron);
  TreeSignal->SetBranchAddress("selmu_ToF_lkl_proton",    &selmu_ToF_lkl_proton);
  // -----------------------------------------------------------------------------
  // Define histograms
  // -----------------------------------------------------------------------------
  // PID
  TH1D* TPCLikeElec[pcolour]; 
  TH1D* TPCLikeMuon[pcolour]; 
  TH1D* TPCLikePion[pcolour];
  TH1D* ECALPID[pcolour]; 
  TH1D* TOFPID[pcolour]; 
  TH1D* TOFLKL_Elec[pcolour];
  TH1D* TOFLKL_Pion[pcolour]; 
  TH1D* TOFLKL_Prot[pcolour]; 
  TH1D* ParMom[pcolour];

  TH1D* TopoMom[pcolour];
  TH1D* TopoAngle[pcolour];
  TH1D* InvMass[pcolour];
  TH1D* TPCVeto[pcolour]; 
  TH1D* ECalVeto[pcolour];

  TString histname;
  for(Int_t j = 0; j < pcolour; j++){
    histname.Form("tpclikeelec_%i",j);
    TPCLikeElec[j] = new TH1D(histname.Data(), histname.Data(), 100, 0, 1.1);
    histname.Form("tpclikemuon_%i",j);
    TPCLikeMuon[j] = new TH1D(histname.Data(), histname.Data(), 100, 0, 1.1);
    histname.Form("tpclikepion_%i",j);
    TPCLikePion[j] = new TH1D(histname.Data(), histname.Data(), 100, 0, 1.1);
    histname.Form("ecalpid_%i",j);
    ECALPID[j] = new TH1D(histname.Data(), histname.Data(), 60, -60, 70);
    histname.Form("tofpid_%i",j);
    TOFPID[j] = new TH1D(histname.Data(), histname.Data(), 200, 0, 4000);
    histname.Form("toflklElec_%i",j);
    TOFLKL_Elec[j] = new TH1D(histname.Data(), histname.Data(), 50, -0.01, 1.02);
    histname.Form("toflklion_%i",j);
    TOFLKL_Pion[j] = new TH1D(histname.Data(), histname.Data(), 50, -0.01, 1.02);
    histname.Form("toflklProt_%i",j);
    TOFLKL_Prot[j] = new TH1D(histname.Data(), histname.Data(), 50, -0.01, 1.02);
    histname.Form("parmom_%i",j);
    ParMom[j] = new TH1D(histname.Data(), histname.Data(), 50, 0, 5000);
    
    histname.Form("topomom_%i",j);
    TopoMom[j] = new TH1D(histname.Data(), histname.Data(), 50, 0, 5000);
    histname.Form("topoangle_%i",j);
    TopoAngle[j] = new TH1D(histname.Data(), histname.Data(), 50, -1, 1);
    histname.Form("invmass_%i",j);
    InvMass[j] = new TH1D(histname.Data(), histname.Data(), 50, 0, 2000);
    histname.Form("tpcveto_%i",j);
    TPCVeto[j] = new TH1D(histname.Data(), histname.Data(), 300, -3000, 3000);
    histname.Form("ecalveto_%i",j);
    ECalVeto[j] = new TH1D(histname.Data(), histname.Data(), 400, -4000, 8000);
  }

  // Efficiency-Purity
  const int nn = 9;
  Double_t parnum[nn] = {0,250,500,750,1000,1500,2000,5000,10000};
  TH1D* EffTrue                     = new TH1D("EffTrue"                   ,"EffTrue"                  ,nn-1,parnum);
  TH1D* PurTrue                     = new TH1D("PurTrue"                   ,"PurTrue"                  ,nn-1,parnum);
  TH1D* SEffTrue                    = new TH1D("SEffTrue"                  ,"SEffTrue"                 ,nn-1,parnum);
  TH1D* SPurTrue                    = new TH1D("SPurTrue"                  ,"SPurTrue"                 ,nn-1,parnum);

  Double_t angnum[nn] = {-1,0,0.2,0.4,0.6,0.7,0.8,0.9,1};
  TH1D* AngTrue                     = new TH1D("AngTrue"                   ,"AngTrue"                  ,nn-1,angnum);
  TH1D* SAngTrue                    = new TH1D("SAngTrue"                  ,"SAngTrue"                 ,nn-1,angnum);

  TH1D* GammaCC                     = new TH1D("GammaCC"                   ,"GammaCC"                  , 50 , 0     , 10000);
  TH1D* GammaNC                     = new TH1D("GammaNC"                   ,"GammaNC"                  , 50 , 0     , 10000);

  TH1D* MomentumRes1D               = new TH1D("MomentumRes1D"             ,"MomentumRes1D"            ,250 , -5    , 5);

  // -----------------------------------------------------------------------------
  // Fill histograms
  // -----------------------------------------------------------------------------
  // Truth tree
  for(Int_t i = 0; i < TruthSignal->GetEntries(); i++){
    TruthSignal->GetEntry(i);

    //if(detector != target) continue;
    
    EffTrue->Fill(true_mom);
    AngTrue->Fill(true_costheta);
  }

  Int_t minaccumlevel = 9;
  Float_t minmom = 600.0;
  Float_t maxmom = 1650000000000000000.0;

  // Selection tree
  for(Int_t i = 0; i < TreeSignal->GetEntries(); i++){
    TreeSignal->GetEntry(i);
    
    //if(detector != target && selmu_ParentPDG == -999) continue;
    //if(detector != target && (nueCC == 1 || nueCC == 2) ){
    //if(selmu_ParentPDG == 22)
    //	nueCC = 6;
    //else
    //	nueCC = 5;
    //}

    if(accum_level < minaccumlevel) continue;
    if(selmu_mom < minmom) continue;
    if(selmu_mom > maxmom) continue;
    //if(accum_level != 4) continue;
 
    //if(selmu_likeel < 0.5) continue;
    //if(selmu_ecal_mipem < 0.0 && selmu_ecal_mipem > -100.0) continue;
    //if( (selmu_ecal_mipem < -100.0 || selmu_mom < 300) && selmu_likemu > 0.03) continue;

    //if(TPCVeto_Deltaz < -30) continue;
    //if(PairTrack_InvMass > 0.0 && PairTrack_InvMass < 110) continue;
    //if(selmu_mom < 200.0) continue;

    //if(NNearTracks == 0 && NFarTracks == 0) continue;

    //if(nu_pdg == 12)
    //truelepton_mom = truelepton_mom*1000;

    //if(selmu_ToF_lkl_proton > 0.0 || selmu_ToF_lkl_pion > 0.0 || selmu_ToF_lkl_electron > 0.0) continue;
    //if(selmu_ToF_lkl_proton > 0.24) continue;
       
    if( abs(selmu_PDG) == 11 ){
      ParMom[0]->Fill(selmu_mom);
      TPCLikeElec[0]->Fill(selmu_likeel);
      TPCLikePion[0]->Fill(selmu_likepi);
      MomentumRes1D->Fill( (selmu_truemom-selmu_mom)/selmu_truemom );
      //MomentumRes1D->Fill( (truelepton_mom-selmu_mom)/truelepton_mom );

      if(selmu_ecal_mipem < -100 || selmu_mom < 300)
	TPCLikeMuon[0]->Fill(selmu_likemu);
 
      if(selmu_ecal_mipem > -100)
	ECALPID[0]->Fill(selmu_ecal_mipem);
      if(selmu_ToF_mass > 0.0)
	TOFPID[0]->Fill(selmu_ToF_mass);
      if(selmu_ToF_lkl_electron > 0.0)
	TOFLKL_Elec[0]->Fill(selmu_ToF_lkl_electron);
      if(selmu_ToF_lkl_proton > 0.0)
	TOFLKL_Prot[0]->Fill(selmu_ToF_lkl_proton);
      if(selmu_ToF_lkl_pion > 0.0)
	TOFLKL_Pion[0]->Fill(selmu_ToF_lkl_pion);
    }
    else if( abs(selmu_PDG) == 13 ){
      ParMom[1]->Fill(selmu_mom);
      TPCLikeElec[1]->Fill(selmu_likeel);
      TPCLikePion[1]->Fill(selmu_likepi);

      if(selmu_ecal_mipem < -100 || selmu_mom < 300)
	TPCLikeMuon[1]->Fill(selmu_likemu);

      if(selmu_ecal_mipem > -100)
	ECALPID[1]->Fill(selmu_ecal_mipem);
      if(selmu_ToF_mass > 0.0)
	TOFPID[1]->Fill(selmu_ToF_mass);
      if(selmu_ToF_lkl_electron > 0.0)
	TOFLKL_Elec[1]->Fill(selmu_ToF_lkl_electron);
      if(selmu_ToF_lkl_proton > 0.0)
	TOFLKL_Prot[1]->Fill(selmu_ToF_lkl_proton);
      if(selmu_ToF_lkl_pion > 0.0)
	TOFLKL_Pion[1]->Fill(selmu_ToF_lkl_pion);
    }
    else if( abs(selmu_PDG) == 211 ){
      ParMom[2]->Fill(selmu_mom);
      TPCLikeElec[2]->Fill(selmu_likeel);
      TPCLikePion[2]->Fill(selmu_likepi);

      if(selmu_ecal_mipem < -100 || selmu_mom < 300)
	TPCLikeMuon[2]->Fill(selmu_likemu);

      if(selmu_ecal_mipem > -100)
	ECALPID[2]->Fill(selmu_ecal_mipem);
      if(selmu_ToF_mass > 0.0)
	TOFPID[2]->Fill(selmu_ToF_mass);
      if(selmu_ToF_lkl_electron > 0.0)
	TOFLKL_Elec[2]->Fill(selmu_ToF_lkl_electron);
      if(selmu_ToF_lkl_proton > 0.0)
	TOFLKL_Prot[2]->Fill(selmu_ToF_lkl_proton);
      if(selmu_ToF_lkl_pion > 0.0)
	TOFLKL_Pion[2]->Fill(selmu_ToF_lkl_pion);
    }
    else if( abs(selmu_PDG) == 321 ){
      ParMom[3]->Fill(selmu_mom);
      TPCLikeElec[3]->Fill(selmu_likeel);
      TPCLikePion[3]->Fill(selmu_likepi);

      if(selmu_ecal_mipem < -100)
	TPCLikeMuon[3]->Fill(selmu_likemu);

      if(selmu_ecal_mipem > -100 || selmu_mom < 300)
	ECALPID[3]->Fill(selmu_ecal_mipem);
      if(selmu_ToF_mass > 0.0)
	TOFPID[3]->Fill(selmu_ToF_mass);
      if(selmu_ToF_lkl_electron > 0.0)
	TOFLKL_Elec[3]->Fill(selmu_ToF_lkl_electron);
      if(selmu_ToF_lkl_proton > 0.0)
	TOFLKL_Prot[3]->Fill(selmu_ToF_lkl_proton);
      if(selmu_ToF_lkl_pion > 0.0)
	TOFLKL_Pion[3]->Fill(selmu_ToF_lkl_pion);
    }
    else if( abs(selmu_PDG) == 2212 ){
      ParMom[4]->Fill(selmu_mom);
      TPCLikeElec[4]->Fill(selmu_likeel);
      TPCLikePion[4]->Fill(selmu_likepi);

      if(selmu_ecal_mipem < -100)
	TPCLikeMuon[4]->Fill(selmu_likemu);

      if(selmu_ecal_mipem > -100 || selmu_mom < 300)
	ECALPID[4]->Fill(selmu_ecal_mipem);
      if(selmu_ToF_mass > 0.0)
	TOFPID[4]->Fill(selmu_ToF_mass);
      if(selmu_ToF_lkl_electron > 0.0)
	TOFLKL_Elec[4]->Fill(selmu_ToF_lkl_electron);
      if(selmu_ToF_lkl_proton > 0.0)
	TOFLKL_Prot[4]->Fill(selmu_ToF_lkl_proton);
      if(selmu_ToF_lkl_pion > 0.0)
	TOFLKL_Pion[4]->Fill(selmu_ToF_lkl_pion);
    }
    else{
      ParMom[5]->Fill(selmu_mom);
      TPCLikeElec[5]->Fill(selmu_likeel);
      TPCLikePion[5]->Fill(selmu_likepi);

      if(selmu_ecal_mipem < -100 || selmu_mom < 300)
	TPCLikeMuon[5]->Fill(selmu_likemu);
 
      if(selmu_ecal_mipem > -100)
	ECALPID[5]->Fill(selmu_ecal_mipem);
      if(selmu_ToF_mass > 0.0)
	TOFPID[5]->Fill(selmu_ToF_mass);
      if(selmu_ToF_lkl_electron > 0.0)
	TOFLKL_Elec[5]->Fill(selmu_ToF_lkl_electron);
      if(selmu_ToF_lkl_proton > 0.0)
	TOFLKL_Prot[5]->Fill(selmu_ToF_lkl_proton);
      if(selmu_ToF_lkl_pion > 0.0)
	TOFLKL_Pion[5]->Fill(selmu_ToF_lkl_pion);
    }

    PurTrue->Fill(selmu_truemom);
    
    // Topology
    if(nueCC == 1){
      SEffTrue->Fill(selmu_truemom);
      SAngTrue->Fill(selmu_truecostheta);
      SPurTrue->Fill(selmu_truemom);

      TopoMom[0]->Fill(selmu_mom);
      TopoAngle[0]->Fill(selmu_costheta);

      if(PairTrack_InvMass > 0.0)
	InvMass[0]->Fill(PairTrack_InvMass);

      if(TPCVeto_Deltaz != -999)
	TPCVeto[0]->Fill(TPCVeto_Deltaz);
      if(ECalVeto_Deltaz != -999)
	ECalVeto[0]->Fill(ECalVeto_Deltaz);
    }
    else if(nueCC == 2){
      SEffTrue->Fill(selmu_truemom);
      SAngTrue->Fill(selmu_truecostheta);
      SPurTrue->Fill(selmu_truemom);

      TopoMom[1]->Fill(selmu_mom);
      TopoAngle[1]->Fill(selmu_costheta);

      if(PairTrack_InvMass > 0.0)
	InvMass[1]->Fill(PairTrack_InvMass);

      if(TPCVeto_Deltaz != -999)
	TPCVeto[1]->Fill(TPCVeto_Deltaz);
      if(ECalVeto_Deltaz != -999)
	ECalVeto[1]->Fill(ECalVeto_Deltaz);
    }
    else if(nueCC == 3){
      TopoMom[2]->Fill(selmu_mom);
      TopoAngle[2]->Fill(selmu_costheta);

      if(PairTrack_InvMass > 0.0)
	InvMass[2]->Fill(PairTrack_InvMass);

      if(TPCVeto_Deltaz != -999)
	TPCVeto[2]->Fill(TPCVeto_Deltaz);
      if(ECalVeto_Deltaz != -999)
	ECalVeto[2]->Fill(ECalVeto_Deltaz);

      if(abs(nu_truereac) <= 30)
	GammaCC->Fill(selmu_mom);
      else 
	GammaNC->Fill(selmu_mom);
    }
    else if(nueCC == 4){
      TopoMom[3]->Fill(selmu_mom);
      TopoAngle[3]->Fill(selmu_costheta);

      if(PairTrack_InvMass > 0.0)
	InvMass[3]->Fill(PairTrack_InvMass);

      if(TPCVeto_Deltaz != -999)
	TPCVeto[3]->Fill(TPCVeto_Deltaz);
      if(ECalVeto_Deltaz != -999)
	ECalVeto[3]->Fill(ECalVeto_Deltaz);
    }
    else if(nueCC == 5){
      TopoMom[4]->Fill(selmu_mom);
      TopoAngle[4]->Fill(selmu_costheta);

      if(PairTrack_InvMass > 0.0)
	InvMass[4]->Fill(PairTrack_InvMass);

      if(TPCVeto_Deltaz != -999)
	TPCVeto[4]->Fill(TPCVeto_Deltaz);
      if(ECalVeto_Deltaz != -999)
	ECalVeto[4]->Fill(ECalVeto_Deltaz);
    }
    else if(nueCC == 6){
      TopoMom[5]->Fill(selmu_mom);
      TopoAngle[5]->Fill(selmu_costheta);

      if(PairTrack_InvMass > 0.0)
	InvMass[5]->Fill(PairTrack_InvMass);

      if(TPCVeto_Deltaz != -999)
	TPCVeto[5]->Fill(TPCVeto_Deltaz);
      if(ECalVeto_Deltaz != -999)
	ECalVeto[5]->Fill(ECalVeto_Deltaz);
    }
    else{
      TopoMom[4]->Fill(selmu_mom);
      TopoAngle[4]->Fill(selmu_costheta);

      if(PairTrack_InvMass > 0.0)
	InvMass[4]->Fill(PairTrack_InvMass);

      if(TPCVeto_Deltaz != -999)
	TPCVeto[4]->Fill(TPCVeto_Deltaz);
      if(ECalVeto_Deltaz != -999)
	ECalVeto[4]->Fill(ECalVeto_Deltaz);
    }
 
  }

  Double_t allevents = 0;
  for(Int_t j = 0; j < pcolour; j++)
    allevents += ParMom[j]->Integral();

  std::cout << "Particle composition:: Electrons = " << ParMom[0]->Integral() 
	    << " , Muons = " << ParMom[1]->Integral()
	    << " , Pions = " << ParMom[2]->Integral()
	    << " , Kaons = " << ParMom[3]->Integral()
	    << " , Protons = " << ParMom[4]->Integral()
	    << " , Other = " << ParMom[5]->Integral() << std::endl;

  std::cout << "Particle fraction:: Electrons = " << ParMom[0]->Integral()/allevents 
	    << " , Muons = " << ParMom[1]->Integral()/allevents
	    << " , Pions = " << ParMom[2]->Integral()/allevents
	    << " , Kaons = " << ParMom[3]->Integral()/allevents
	    << " , Protons = " << ParMom[4]->Integral()/allevents
	    << " , Other = " << ParMom[5]->Integral()/allevents << std::endl;

  std::cout << "Topology fraction:: CC-0pi = " << TopoMom[0]->Integral()/allevents 
	    << " , CC-Other = " << TopoMom[1]->Integral()/allevents
	    << " , Gamma = " << TopoMom[2]->Integral()/allevents
	    << " , OOFV gamma = " << TopoMom[5]->Integral()/allevents
	    << " , Muon = " << TopoMom[3]->Integral()/allevents
	    << " , Other = " << TopoMom[4]->Integral()/allevents << std::endl;

  //std::cout << "CC-nue efficiency = " <<  (TopoMom[0]->Integral() + TopoMom[1]->Integral()) / EffTrue->Integral() << std::endl;
  //std::cout << "CC-nue purity = " << (TopoMom[0]->Integral() + TopoMom[1]->Integral()) / allevents  << std::endl;
  //std::cout << "CC-nue efficiency*purity = " << ( (TopoMom[0]->Integral() + TopoMom[1]->Integral()) / allevents)*((TopoMom[0]->Integral() + TopoMom[1]->Integral()) / EffTrue->Integral())  << std::endl;
  std::cout << "CC-nue efficiency = " <<  SEffTrue->Integral() / EffTrue->Integral() << std::endl;
  std::cout << "CC-nue purity = " << SEffTrue->Integral() / allevents  << std::endl;
  std::cout << "CC-nue efficiency*purity = " << (  SEffTrue->Integral() / allevents )*( SEffTrue->Integral() / EffTrue->Integral() )  << std::endl;
  std::cout << EffTrue->Integral() << " , " << SEffTrue->Integral() << std::endl;
  std::cout << "-------------------------------------" << std::endl;
  std::cout << "CC Gamma events = " << GammaCC->Integral() << " , NC Gamma = " << GammaNC->Integral() << " , NC/CC = " << GammaNC->Integral()/(GammaNC->Integral() + GammaCC->Integral()) << std::endl;
  // -------------------------------------------------------------------------
  TLegend *mylegend0 = new TLegend(0.53,0.55,0.78,0.81);
  mylegend0->SetFillColor(0); // white background
  mylegend0->SetBorderSize(0);
  mylegend0->SetTextSize(0.036);

  THStack *topohs0 = new THStack("NueTopoMom" ,"NueTopoMom");
  THStack *topohs1 = new THStack("NueTopoAng" ,"NueTopoAng");
  THStack *topohs2 = new THStack("NueTopoIM"  ,"NueTopoIM");
  THStack *topohs3 = new THStack("TPCVetoCut" ,"TPCVetoCut");
  THStack *topohs4 = new THStack("ECalVetoCut","ECalVetoCut");
  for(Int_t j = pcolour-1; j >= 0; j--){
    TopoMom[j]->SetLineColor(topcolour[j]);
    TopoMom[j]->SetFillColor(topcolour[j]);
    TopoMom[j]->SetFillStyle(3000 + topcolour[j]);

    TopoAngle[j]->SetLineColor(topcolour[j]);
    TopoAngle[j]->SetFillColor(topcolour[j]);
    TopoAngle[j]->SetFillStyle(3000 + topcolour[j]);

    InvMass[j]->SetLineColor(topcolour[j]);
    InvMass[j]->SetFillColor(topcolour[j]);
    InvMass[j]->SetFillStyle(3000 + topcolour[j]);
    
    TPCVeto[j]->SetLineColor(topcolour[j]);
    TPCVeto[j]->SetFillColor(topcolour[j]);
    TPCVeto[j]->SetFillStyle(3000 + topcolour[j]);

    ECalVeto[j]->SetLineColor(topcolour[j]);
    ECalVeto[j]->SetFillColor(topcolour[j]);
    ECalVeto[j]->SetFillStyle(3000 + topcolour[j]);

    topohs0->Add(TopoMom[j],   "");
    topohs1->Add(TopoAngle[j], "");
    topohs2->Add(InvMass[j],   "");
    topohs3->Add(TPCVeto[j],   "");
    topohs4->Add(ECalVeto[j],   "");
  }

  for(Int_t j = 0; j < pcolour; j++){
     mylegend0->AddEntry(TopoMom[j], toponame[j].c_str(), "f");
  }

  TCanvas *cths0 = new TCanvas("C-TopoMom","C-TopoMom");
  topohs0->Draw("");
  topohs0->GetXaxis()->SetTitle("Momentum [MeV/c]");
  topohs0->GetYaxis()->SetTitle("Events / 100 MeV/c / 10^{21} POT");
  topohs0->GetYaxis()->SetTitleOffset(1.2);
  mylegend0->Draw();

  TCanvas *cths1 = new TCanvas("C-TopoAng","C-TopoAng");
  topohs1->Draw("");
  topohs1->GetXaxis()->SetTitle("Cos(#theta)");
  topohs1->GetYaxis()->SetTitle("Events / 10^{21} POT");
  topohs1->GetYaxis()->SetTitleOffset(1.2);
  mylegend0->Draw();

  TCanvas *cths2 = new TCanvas("C-IM","C-IM");
  topohs2->Draw("");
  topohs2->GetXaxis()->SetTitle("Gamma Invariant Mass [MeV/c^{2}]");
  topohs2->GetYaxis()->SetTitle("Events / 10^{21} POT");
  topohs2->GetYaxis()->SetTitleOffset(1.2);
  mylegend0->Draw();
  if(drawarrows){
    Double_t hsmax  = topohs2->GetMaximum();
    TLine *line = new TLine(110.0,0,110.0,hsmax+2);
    line->SetLineColor(kRed);
    TArrow *arrow = new TArrow(110.0, hsmax+1, 200.0, hsmax+1, 0.025, ">");
    arrow->SetLineColor(kRed);
    line->Draw();
    arrow->Draw();
  }

  TCanvas *cths3 = new TCanvas("C-TPCVeto","C-TPCVeto");
  topohs3->Draw("");
  topohs3->GetXaxis()->SetTitle("TPC Veto [mm]");
  topohs3->GetYaxis()->SetTitle("Events / 10^{21} POT");
  topohs3->GetYaxis()->SetTitleOffset(1.2);
  mylegend0->Draw();
  if(drawarrows){
    Double_t hsmax  = topohs3->GetMaximum();
    TLine *line = new TLine(-30.0,0,-30.0,hsmax+2);
    line->SetLineColor(kRed);
    TArrow *arrow = new TArrow(-30.0, hsmax+1, 130.0, hsmax+1, 0.025, ">");
    arrow->SetLineColor(kRed);
    line->Draw();
    arrow->Draw();
  }

  TCanvas *cths4 = new TCanvas("C-ECalVeto","C-ECalVeto");
  topohs4->Draw("");
  topohs4->GetXaxis()->SetTitle("ECal Veto [mm]");
  topohs4->GetYaxis()->SetTitle("Events / 10^{21} POT");
  topohs4->GetYaxis()->SetTitleOffset(1.2);
  mylegend0->Draw();

  // -------------------------------------------------------------------------
  TLegend *mylegend1 = new TLegend(0.53,0.55,0.78,0.81);
  mylegend1->SetFillColor(0); // white background
  mylegend1->SetBorderSize(0);
  mylegend1->SetTextSize(0.036);

  THStack *hs0 = new THStack("TPCLikeElec","TPCLikeElec");
  THStack *hs1 = new THStack("TPCLikeMuon","TPCLikeMuon");
  THStack *hs2 = new THStack("TPCLikePion","TPCLikePion");
  THStack *hs3 = new THStack("ECALPID",    "ECALPID");
  THStack *hs4 = new THStack("TOFPID",     "TOFPID");
  THStack *hs5 = new THStack("ParMomentum","ParMomentum");
  THStack *hs6 = new THStack("TOFLKLElec", "TOFLKLElec");
  THStack *hs7 = new THStack("TOFLKLProt", "TOFLKLProt");
  THStack *hs8 = new THStack("TOFLKLPion", "TOFLKLPion");
  for(Int_t j = 0; j < pcolour; j++){
    TPCLikeElec[j]->SetLineColor(parcolour[j]);
    TPCLikeElec[j]->SetFillColor(parcolour[j]);
    TPCLikeElec[j]->SetFillStyle(3000 + parcolour[j]);

    TPCLikeMuon[j]->SetLineColor(parcolour[j]);
    TPCLikeMuon[j]->SetFillColor(parcolour[j]);
    TPCLikeMuon[j]->SetFillStyle(3000 + parcolour[j]);

    TPCLikePion[j]->SetLineColor(parcolour[j]);
    TPCLikePion[j]->SetFillColor(parcolour[j]);
    TPCLikePion[j]->SetFillStyle(3000 + parcolour[j]);

    ECALPID[j]->SetLineColor(parcolour[j]);
    ECALPID[j]->SetFillColor(parcolour[j]);
    ECALPID[j]->SetFillStyle(3000 + parcolour[j]);

    TOFPID[j]->SetLineColor(parcolour[j]);
    TOFPID[j]->SetFillColor(parcolour[j]);
    TOFPID[j]->SetFillStyle(3000 + parcolour[j]);

    ParMom[j]->SetLineColor(parcolour[j]);
    ParMom[j]->SetFillColor(parcolour[j]);
    ParMom[j]->SetFillStyle(3000 + parcolour[j]);

    TOFLKL_Elec[j]->SetLineColor(parcolour[j]);
    TOFLKL_Elec[j]->SetFillColor(parcolour[j]);
    TOFLKL_Elec[j]->SetFillStyle(3000 + parcolour[j]);

    TOFLKL_Prot[j]->SetLineColor(parcolour[j]);
    TOFLKL_Prot[j]->SetFillColor(parcolour[j]);
    TOFLKL_Prot[j]->SetFillStyle(3000 + parcolour[j]);

    TOFLKL_Pion[j]->SetLineColor(parcolour[j]);
    TOFLKL_Pion[j]->SetFillColor(parcolour[j]);
    TOFLKL_Pion[j]->SetFillStyle(3000 + parcolour[j]);

    mylegend1->AddEntry(TPCLikeElec[j], parname[j].c_str(), "f");
    hs0->Add(TPCLikeElec[j], "");
    hs1->Add(TPCLikeMuon[j], "");
    hs2->Add(TPCLikePion[j], "");
    hs3->Add(ECALPID[j],     "");
    hs4->Add(TOFPID[j],      "");
    hs5->Add(ParMom[j],      "");
    hs6->Add(TOFLKL_Elec[j], "");
    hs7->Add(TOFLKL_Prot[j], "");
    hs8->Add(TOFLKL_Pion[j], "");
  }

   TCanvas *chs0 = new TCanvas("C-TPCLikeElec","C-TPCLikeElec");
   hs0->Draw("");
   hs0->GetXaxis()->SetTitle("TPC Electron Likelihood");
   hs0->GetYaxis()->SetTitle("Events / 10^{21} POT");
   hs0->GetYaxis()->SetTitleOffset(1.2);
   mylegend1->Draw();
   if(drawarrows){
     Double_t hsmax  = hs0->GetMaximum();
     TLine *line = new TLine(0.5,0,0.5,hsmax+2);
     line->SetLineColor(kRed);
     TArrow *arrow = new TArrow(0.5, hsmax+1, 0.6, hsmax+1, 0.025, ">");
     arrow->SetLineColor(kRed);
     line->Draw();
     arrow->Draw();
   }

   TCanvas *chs1 = new TCanvas("C-TPCLikeMuon","C-TPCLikeMuon");
   hs1->Draw("");
   hs1->GetXaxis()->SetTitle("TPC Muon Likelihood");
   hs1->GetYaxis()->SetTitle("Events / 10^{21} POT");
   hs1->GetYaxis()->SetTitleOffset(1.2);
   mylegend1->Draw();
   if(drawarrows){
     Double_t hsmax  = hs1->GetMaximum();
     TLine *line = new TLine(0.03,0,0.03,hsmax+2);
     line->SetLineColor(kRed);
     TArrow *arrow = new TArrow(0.01, hsmax+1, 0.03, hsmax+1, 0.025, "<");
     arrow->SetLineColor(kRed);
     line->Draw();
     arrow->Draw();
   }
   
   TCanvas *chs2 = new TCanvas("C-TPCLikePion","C-TPCLikePion");
   hs2->Draw("");
   hs2->GetXaxis()->SetTitle("TPC Pion Likelihood");
   hs2->GetYaxis()->SetTitle("Events / 10^{21} POT");
   hs2->GetYaxis()->SetTitleOffset(1.2);
   mylegend1->Draw();

   TCanvas *chs3 = new TCanvas("C-ECALPID","C-ECALPID");
   hs3->Draw("");
   hs3->GetXaxis()->SetTitle("ECAL PID");
   hs3->GetYaxis()->SetTitle("Events / 10^{21} POT");
   hs3->GetYaxis()->SetTitleOffset(1.2);
   mylegend1->Draw();
   if(drawarrows){
     Double_t hsmax  = hs3->GetMaximum();
     TLine *line = new TLine(0.0,0,0.0,hsmax+2);
     line->SetLineColor(kRed);
     TArrow *arrow = new TArrow(0.0, hsmax+1, 5.0, hsmax+1, 0.025, ">");
     arrow->SetLineColor(kRed);
     line->Draw();
     arrow->Draw();
   }

   TCanvas *chs4 = new TCanvas("C-TOFPID","C-TOFPID");
   hs4->Draw("");
   hs4->GetXaxis()->SetTitle("ToF Mass [MeV^{2}/c^{2}]");
   hs4->GetYaxis()->SetTitle("Events / 10^{21} POT");
   hs4->GetYaxis()->SetTitleOffset(1.2);
   mylegend1->Draw();

   TCanvas *chs5 = new TCanvas("C-ParticleMomentum","C-ParticleMomentum");
   hs5->Draw("");
   hs5->GetXaxis()->SetTitle("Momentum [MeV/c]");
   hs5->GetYaxis()->SetTitle("Events / 100 MeV/c / 10^{21} POT");
   hs5->GetYaxis()->SetTitleOffset(1.2);
   mylegend1->Draw();

   TCanvas *chs6 = new TCanvas("C-TOFLKLELEC","C-TOFLKLELEC");
   hs6->Draw("");
   hs6->GetXaxis()->SetTitle("ToF Likelihood Electron hypothesis");
   hs6->GetYaxis()->SetTitle("Events / 10^{21} POT");
   hs6->GetYaxis()->SetTitleOffset(1.2);
   mylegend1->Draw();

   TCanvas *chs7 = new TCanvas("C-TOFLKLPROT","C-TOFLKLPROT");
   hs7->Draw("");
   hs7->GetXaxis()->SetTitle("ToF Likelihood Proton hypothesis");
   hs7->GetYaxis()->SetTitle("Events / 10^{21} POT");
   hs7->GetYaxis()->SetTitleOffset(1.2);
   mylegend1->Draw();

   TCanvas *chs8 = new TCanvas("C-TOFLKLPION","C-TOFLKLPION");
   hs8->Draw("");
   hs8->GetXaxis()->SetTitle("ToF Likelihood Pion hypothesis");
   hs8->GetYaxis()->SetTitle("Events / 10^{21} POT");
   hs8->GetYaxis()->SetTitleOffset(1.2);
   mylegend1->Draw();

   // -------------------------------------------------------------------------
   TGraphAsymmErrors *Purity      = new TGraphAsymmErrors(SPurTrue, PurTrue);
   TGraphAsymmErrors *Efficiency  = new TGraphAsymmErrors(SEffTrue, EffTrue);
   TGraphAsymmErrors *AEfficiency = new TGraphAsymmErrors(SAngTrue, AngTrue);

   TLegend *mylegend3 = new TLegend(0.63,0.65,0.85,0.88);
   mylegend3->SetFillColor(0); // white background
   mylegend3->SetBorderSize(0);
   mylegend3->SetTextSize(0.036);

   mylegend3->AddEntry(Purity,"Purity","l");
   mylegend3->AddEntry(Efficiency,"Efficiency","l");

   TCanvas *c91 = new TCanvas("Purity-Efficiency","Purity-Efficiency");
   Purity->Draw("*a");
   Purity->SetMarkerColor(2);
   Purity->SetMarkerStyle(25);
   Purity->SetLineColor(2);
   Purity->GetYaxis()->SetTitleOffset(1.2);
   Purity->GetXaxis()->SetTitle("p_{true} [MeV/c]");
   
   Efficiency->Draw("*same");
   Efficiency->SetMarkerColor(4);
   Efficiency->SetMarkerStyle(4);
   Efficiency->SetLineColor(4);
   
   mylegend3->Draw();
   
   TCanvas *c92 = new TCanvas("Angle-Purity-Efficiency","Angle-Purity-Efficiency");
   AEfficiency->Draw("*a");
   AEfficiency->SetMarkerColor(4);
   AEfficiency->SetMarkerStyle(4);
   AEfficiency->SetLineColor(4);
   AEfficiency->GetXaxis()->SetTitle("Cos(#theta)");
   mylegend3->Draw();

   TCanvas *c93 = new TCanvas("C-MomentumRes","C-MomentumRes");
   MomentumRes1D->Draw();
   MomentumRes1D->GetXaxis()->SetTitle("(p_{true}-p_{recon})/p_{true}");
   
   
   // -------------------------------------------------------------------------
   TFile *ftemp = new TFile("/hepstore/georgios/nd280v11r31/temp/files/fgd1_nue.root","READ");
   TGraphAsymmErrors *nue_AEfficiency = (TGraphAsymmErrors*)ftemp->Get("AEfficiency_Graph");
   
   TLegend *mylegend4 = new TLegend(0.63,0.65,0.85,0.88);
   mylegend4->SetFillColor(0); // white background
   mylegend4->SetBorderSize(0);
   mylegend4->SetTextSize(0.036);

   mylegend4->AddEntry(nue_AEfficiency,"CC-#nu_{e} Efficiency (From official FGD1 Selection)", "l");
   mylegend4->AddEntry(Efficiency,"CC-#nu_{e} Efficiency ND280 Upgrade", "l");
   
   TCanvas *c92a = new TCanvas("Angle-Purity-Efficiency-A","Angle-Purity-Efficiency-A");
   AEfficiency->Draw("*a");
   AEfficiency->SetMarkerColor(4);
   AEfficiency->SetMarkerStyle(4);
   AEfficiency->SetLineColor(4);
   AEfficiency->GetXaxis()->SetTitle("Cos(#theta)");
   
   nue_AEfficiency->Draw("*same");
   nue_AEfficiency->SetMarkerColor(6);
   nue_AEfficiency->SetMarkerStyle(6);
   nue_AEfficiency->SetLineColor(6);
   
   mylegend4->Draw();
}
