#include "numuCCOOFVAnalysis.hxx"
#include "CategoriesUtils.hxx"
#include "numuCCOOFVUtils.hxx"
//#include "baseToyMaker.hxx"
#include "CutUtils.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "BasicUtils.hxx"
#include "MomRangeCorrection.hxx"
#include "PIDCorrection.hxx"
#include "CutUtils.hxx"
#include "NuDirUtils.hxx"

const UInt_t NMAXMU = 500;


//********************************************************************
numuCCOOFVAnalysis::numuCCOOFVAnalysis(AnalysisAlgorithm* ana) : baseAnalysis(ana) {
  //********************************************************************

  //Add the package version
  //	ND::versioning().AddPackage("numuCCOOFVAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("NUMUCCOOFVANALYSISROOT")));

}

//********************************************************************
bool numuCCOOFVAnalysis::Initialize(){
  //********************************************************************

  // Initialize the baseAnalysis
  if(!baseAnalysis::Initialize()) return false;

  //Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("numuCCOOFVAnalysis.MinAccumLevelToSave"));

  //Add muon candidate categories
  numuCCOOFVAnalysis::AddCategories();


  if (ND::params().GetParameterI("numuCCOOFVAnalysis.Configuration") == 1) {
    if (ND::params().GetParameterI("numuCCOOFVAnalysis.ToF") == 1) {
      DetDef::Target1min[2] =  -1737.00;
      DetDef::Target1max[2] =   -537.00;
      DetDef::Target2min[2] =    537.00;
      DetDef::Target2max[2] =   1737.00;
    }
  }
  
  if (ND::params().GetParameterI("numuCCOOFVAnalysis.Configuration") >= 2) {
    
    DetDef::FGD1min[0] = -932.15;
    DetDef::FGD1max[0] =  932.15;	
    DetDef::FGD1min[1] = -948.15;
    DetDef::FGD1max[1] =  916.15;	
    DetDef::FGD1min[2] =  276.00;
    DetDef::FGD1max[2] =  579.00;

    DetDef::FGD2min[0] = -932.15;
    DetDef::FGD2max[0] =  932.15;	
    DetDef::FGD2min[1] = -948.15;
    DetDef::FGD2max[1] =  916.15;	
    DetDef::FGD2min[2] = 1553.00;
    DetDef::FGD2max[2] = 1856.00;
	
  }
  
  if (ND::params().GetParameterI("numuCCOOFVAnalysis.Configuration") == 2) {
    if (ND::params().GetParameterI("numuCCOOFVAnalysis.ToF") == 1) {
      DetDef::Target1min[2] =  -2642.00;
      DetDef::Target1max[2] =   -748.00;
    }
    else {
      DetDef::Target1min[2] =  -2692.00;
      DetDef::Target1max[2] =   -698.00;
    }
  }
  if (ND::params().GetParameterI("numuCCOOFVAnalysis.Configuration") == 3) {
    if (ND::params().GetParameterI("numuCCOOFVAnalysis.ToF") == 1) {
      DetDef::Target1min[2] =  -1668.00;
      DetDef::Target1max[2] =    226.00;
    }
    else {
      DetDef::Target1min[2] =  -1718.00;
      DetDef::Target1max[2] =    276.00;
    }
  }

  /*
    if (ND::params().GetParameterI("numuCCOOFVAnalysis.Configuration") == 4) {
    DetDef::Target1min[1] =   -216.00;
    DetDef::Target1max[1] =    184.00;  
    DetDef::Target1min[2] =  -2692.00;
    DetDef::Target1max[2] =   -698.00;
    }
    if (ND::params().GetParameterI("numuCCOOFVAnalysis.Configuration") == 5) {
    DetDef::Target1min[1] =   -216.00;
    DetDef::Target1max[1] =    184.00;  
    DetDef::Target1min[2] =  -1718.00;
    DetDef::Target1max[2] =    276.00;
    }
  */
  
  return true;

}

//********************************************************************
void numuCCOOFVAnalysis::DefineSelections(){
  //********************************************************************

  //bool forceBreak = (bool)ND::params().GetParameterI("numuCCOOFVAnalysis.ForceBreak");

  // ----- Inclusive CC -----------
  sel().AddSelection("kTrackerNumuCCOOFV", "inclusive numuCCOOFV selection", new numuCCOOFVSelection()); //true/false for forcing break

}

//********************************************************************
void numuCCOOFVAnalysis::DefineCorrections(){
  //********************************************************************

  //Some corrections are defined in baseAnalysis
  baseAnalysis::DefineCorrections();
  corr().AddCorrection("mom_corr",          new MomRangeCorrection(SubDetId::kTPC));
  corr().AddCorrection("pid_corr",          new PIDCorrection(SubDetId::kTPC));


}

//********************************************************************
void numuCCOOFVAnalysis::DefineSystematics(){
  //********************************************************************

  //Some systematics are defined in baseAnalysis
  baseAnalysis::DefineSystematics();

}

//********************************************************************
void numuCCOOFVAnalysis::DefineConfigurations(){
  //********************************************************************

  baseAnalysis::DefineConfigurations();

}

//********************************************************************
void numuCCOOFVAnalysis::DefineMicroTrees(bool addBase){
  //********************************************************************

  //-------- Add variables to the analysis tree ----------------------

  //Variables from baseAnalysis (run, event, ...)
  if (addBase) baseAnalysis::DefineMicroTrees(addBase);
  
  //--- toy variables
  AddToyVarI(output(),  NTracks,         "");
  AddToyVarI(output(),  Toy_CCOOFV,      "");
  AddToyVarF(output(),  selmu_costheta,  "");
  AddVarF(output(),  selmu_likemu,       ""); 
  AddVarF(output(),  selmu_likemip,      ""); 
  
  //--- info from true vertex
  AddVarI(output(),   truelepton_PDG,      "");
  AddVarF(output(),   truelepton_mom,      "");
  AddVar3VF(output(), truelepton_dir,      "");
  AddVarF(output(),   truelepton_costheta, "");
  AddVar4VF(output(), truelepton_pos,      "");

  //--- info from true track
  AddVarI(output(),   selmu_ID,           "");
  AddVarI(output(),   selmu_PDG,          "");
  AddVarF(output(),   selmu_mom,          "");
  AddVarF(output(),   selmu_truemom,      "");
  AddVar4VF(output(), selmu_truepos,      "");
  AddVar4VF(output(), selmu_trueendpos,   "");
  AddVar3VF(output(), selmu_truedir,      "");

  //--- info by global
  AddVarI(output(),   selmu_detectors, "");
  AddVarF(output(),   selmu_charge,    "");
  AddVar3VF(output(), selmu_dir,       "");
  AddVar3VF(output(), selmu_enddir,    "");
  AddVar4VF(output(), selmu_pos,       "");
  AddVar4VF(output(), selmu_endpos,    "");
  AddVarI(output(),   selmu_longestTPC,"");
  AddVarI(output(),   selmu_OOFV,      "");

  //--- ECal
  AddVarF(output(),   selmu_ecal_mipem,     "");
  AddVarF(output(),   selmu_ecal_EneOnL,    "");
  AddVarI(output(),   selmu_ecal_stopping,  "");

}

//********************************************************************
void numuCCOOFVAnalysis::DefineTruthTree(){
  //********************************************************************

  baseAnalysis::DefineTruthTree();

  AddVarI(output(), true_Nu_pdg,            "nu pdg");
  AddVarI(output(), true_Target_pdg,        "target pdg");
  AddVarF(output(), true_Nu_mom,            "nu momentum");
  AddVarI(output(), true_reaction_code,     "reaction_code");
  AddVar4VF(output(), true_vertex_position, "vertex position");

  AddVarI(output(), true_parentID,          "true_parentID");                  

  AddVarI(output(), true_pdg,         "pdg");
  AddVarF(output(), true_SDlength,    "SD length");
  AddVarF(output(), true_Edep,        "Edeposite");
  AddVarF(output(), true_mom,         "mom");
  AddVarF(output(), true_costheta,    "costheta");
  AddVarF(output(), true_phi,         "phi");
  AddVarF(output(), true_charge,      "charge");
  AddVarF(output(), true_ekin,        "kinetic energy");
  AddVarF(output(), true_Q2,          "Q2");
  AddVarI(output(), true_TPC,         "TPC detector");

  AddVarI(output(), trueHMN_pdg,         "pdg");

}

//********************************************************************
void numuCCOOFVAnalysis::FillMicroTrees(bool addBase){
  //********************************************************************

  if (addBase) baseAnalysis::FillMicroTreesBase(addBase);
  if ( ccoofvbox().MainTrack ){

    EventBoxB* EventBox = (*_event).EventBoxes[EventBoxId::kEventBoxNDUP];
    (void)EventBox;
    
    if (ccoofvbox().MainTrack->TrueObject) {
      AnaTrueVertex *vtx = static_cast<AnaTrueVertex*>(ccoofvbox().MainTrack->GetTrueParticle()->TrueVertex);
      if(vtx) {
	output().FillVar(truelepton_PDG,                    vtx->LeptonPDG);
	output().FillVar(truelepton_mom,                vtx->LeptonMom);
	output().FillVectorVarFromArray(truelepton_dir, vtx->LeptonDir,3);
	double costheta_mu_nu = cos(anaUtils::ArrayToTVector3(vtx->LeptonDir).Angle(anaUtils::ArrayToTVector3(vtx->NuDir)));
	output().FillVar(truelepton_costheta, (Float_t)costheta_mu_nu);
	output().FillVectorVarFromArray(truelepton_pos, vtx->Position,4);
      }
      output().FillVar(selmu_ID,                        ccoofvbox().MainTrack->GetTrueParticle()->ID);
      output().FillVar(selmu_PDG,                       ccoofvbox().MainTrack->GetTrueParticle()->PDG);
      output().FillVar(selmu_truemom,                   ccoofvbox().MainTrack->GetTrueParticle()->Momentum);
      output().FillVectorVarFromArray(selmu_truedir,    ccoofvbox().MainTrack->GetTrueParticle()->Direction, 3);
      output().FillVectorVarFromArray(selmu_truepos,    ccoofvbox().MainTrack->GetTrueParticle()->Position, 4);
      output().FillVectorVarFromArray(selmu_trueendpos, ccoofvbox().MainTrack->GetTrueParticle()->PositionEnd, 4);
      output().FillVar(selmu_OOFV,                      ccoofvbox().OOFV);

    }

    AnaTrackB* track = ccoofvbox().MainTrack;

    //output().FillVar(selmu_detectors,                 track->Detectors);
    output().FillVar(selmu_charge,                    track->Charge);
    output().FillVar(selmu_mom,                       track->SmearedMomentum);
    output().FillVectorVarFromArray(selmu_dir,        track->DirectionStart, 3);
    output().FillVectorVarFromArray(selmu_enddir,     track->DirectionEnd, 3);
    output().FillVectorVarFromArray(selmu_pos,        track->PositionStart, 4);
    output().FillVectorVarFromArray(selmu_endpos,     track->PositionEnd, 4);
    output().FillVar(selmu_likemu,        anaUtils::GetPIDLikelihood( *track, 0));
    output().FillVar(selmu_likemip,       anaUtils::GetPIDLikelihoodMIP( *track));

    output().FillVar(selmu_ecal_stopping,             cutUtils::StoppingECALCut( *track ));
    output().FillVar(selmu_ecal_mipem,                ccoofvbox().track_ECal_MipEM);
    output().FillVar(selmu_ecal_EneOnL,               ccoofvbox().track_ECal_EneOnL);
    output().FillVar(selmu_longestTPC,                ccoofvbox().TPC_det);

  }

}

//********************************************************************
void numuCCOOFVAnalysis::FillToyVarsInMicroTrees(bool addBase){
  //********************************************************************


  if (addBase) baseAnalysis::FillToyVarsInMicroTreesBase(addBase);
 
  if ( ccoofvbox().MainTrack ){
    output().FillToyVar(NTracks,    (int)ccoofvbox().Tracks.size() );

    AnaTrack* track = static_cast<AnaTrack*>( ccoofvbox().MainTrack );
    output().FillToyVar(Toy_CCOOFV, numuCCOOFV_utils::CCOOFVCategory(track));

    //output().FillToyVar(selmu_MomRangeMuon,  track->RangeMomentumMuon);

    TVector3 nuDirVec = anaUtils::GetNuDirRec(track->PositionStart);
    TVector3 muDirVec = anaUtils::ArrayToTVector3(track->DirectionStart);
    double costheta_mu_nu = nuDirVec.Dot(muDirVec);
    output().FillToyVar(selmu_costheta,      (Float_t)costheta_mu_nu);

  }
  

}

//********************************************************************
bool numuCCOOFVAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx) {
  //********************************************************************

  //int branch = ND::params().GetParameterI("numuCCOOFVAnalysis.Branch");

  bool useTarget1 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget1");
  bool useTarget2 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget2");
  bool useFGD1 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableFGD1");
  bool useFGD2 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableFGD2");

  int nu_mode = ND::params().GetParameterI("numuCCOOFVAnalysis.NeutrinoMode");

  bool TrueNuMuCC = nu_mode*vtx.ReacCode > 0 && nu_mode*vtx.ReacCode < 30 && nu_mode*vtx.NuPDG == 14;
  /*
  bool TrueVtxFV = ( (useTarget1 && anaUtils::InFiducialVolume( SubDetId::kTarget1, vtx.Position )) ||
		     (useTarget2 && anaUtils::InFiducialVolume( SubDetId::kTarget2, vtx.Position )) ||
		     (useFGD1    && anaUtils::InFiducialVolume( SubDetId::kFGD1,    vtx.Position )) ||
		     (useFGD2    && anaUtils::InFiducialVolume( SubDetId::kFGD2,    vtx.Position )) );
  */
  
  return TrueNuMuCC;

  return false;
}

//********************************************************************
void numuCCOOFVAnalysis::FillTruthTree(const AnaTrueVertex& vtx) {
  //********************************************************************

  bool IsAntinu = (ND::params().GetParameterI("numuCCOOFVAnalysis.NeutrinoMode")==-1);
  baseAnalysis::FillTruthTreeBase(vtx, SubDetId::kTarget, IsAntinu);

  std::vector<AnaTrueParticleB*> TrueParticles = vtx.TrueParticlesVect;
  output().FillVar(true_Nu_pdg, vtx.NuPDG);
  output().FillVar(true_Nu_mom, vtx.NuMom);
  output().FillVar(true_Target_pdg, vtx.TargetPDG);

  output().FillVar(true_reaction_code, vtx.ReacCode);
  output().FillVectorVarFromArray(true_vertex_position, vtx.Position, 4);
  AnaTrueParticleB* trueTrack=NULL;
  AnaTrueParticleB* trueHMNTrack=NULL; float max_mom=0;

  int nu_mode = ND::params().GetParameterI("numuCCOOFVAnalysis.NeutrinoMode");

  for (unsigned int i = 0; i < TrueParticles.size(); i++) {
    if (TrueParticles[i]->PDG == nu_mode*13)
      trueTrack = dynamic_cast<AnaTrueParticleB*>(TrueParticles[i]);
    if (TrueParticles[i]->Momentum > max_mom && nu_mode*TrueParticles[i]->Charge<0){
      trueHMNTrack = TrueParticles[i];
      max_mom = TrueParticles[i]->Momentum;
    }
     
  }
  if (trueTrack) {
    output().FillVar(true_parentID, trueTrack->ParentID);
    output().FillVar(true_pdg, trueTrack->PDG);
    output().FillVar(true_mom, trueTrack->Momentum);
    output().FillVar(true_Edep, trueTrack->EDeposit);
    output().FillVar(true_SDlength, trueTrack->Length);
    output().FillVar(true_costheta, trueTrack->CosTheta);
    output().FillVar(true_ekin, trueTrack->EKin);
    output().FillVar(true_charge, trueTrack->Charge);

    for (int i=0; i<trueTrack->DetCrossingsVect.size(); i++) {
      int tpc = SubDetId::GetTPC(trueTrack->DetCrossingsVect[i]->Detector);
      if (tpc > 0) { output().FillVar(true_TPC, tpc); break; }
    }

    float phi = atan2(trueTrack->Direction[1], trueTrack->Direction[0]);
    //float cosphi = cos(phi);

    output().FillVar(true_phi, phi);

    TLorentzVector P_nu(0,0,0,0), P_mu(0,0,0,0);
    P_nu.SetVectM(vtx.NuMom*anaUtils::ArrayToTVector3(vtx.NuDir), 0);
    P_mu.SetVectM(trueTrack->Momentum*anaUtils::ArrayToTVector3(trueTrack->Direction), 105.66);

    float Q2 = -(P_mu-P_nu).Mag2()/1e6; // in GeV^2/c^2
    output().FillVar(true_Q2, Q2);

  }
  if (trueHMNTrack) {
    output().FillVar(trueHMN_pdg, trueHMNTrack->PDG);
  }

  bool useTarget1 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget1");
  bool useTarget2 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget2");
  bool useFGD1 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableFGD1");
  bool useFGD2 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableFGD2");

  if ( useTarget1 && !useTarget2)
    anaUtils::FillCategories(&vtx, "", SubDetId::kTarget1, IsAntinu,GetSpill().GetIsSandMC());
  if (!useTarget1 &&  useTarget2)
    anaUtils::FillCategories(&vtx, "", SubDetId::kTarget2, IsAntinu,GetSpill().GetIsSandMC());
  if ( useTarget1 &&  useTarget2)
    anaUtils::FillCategories(&vtx, "", SubDetId::kTarget, IsAntinu,GetSpill().GetIsSandMC());
  if ( useFGD1 && !useFGD2)
    anaUtils::FillCategories(&vtx, "", SubDetId::kFGD1, IsAntinu,GetSpill().GetIsSandMC());
  if (!useFGD1 &&  useFGD2)
    anaUtils::FillCategories(&vtx, "", SubDetId::kFGD2, IsAntinu,GetSpill().GetIsSandMC());
  if ( useFGD1 &&  useFGD2)
    anaUtils::FillCategories(&vtx, "", SubDetId::kFGD, IsAntinu,GetSpill().GetIsSandMC());

}


//********************************************************************
void numuCCOOFVAnalysis::AddCategories(){
  //********************************************************************

  anaUtils::AddStandardCategories();

  const int NCCOOFVTYPES=4;
  std::string ccOOFV_types[NCCOOFVTYPES] = { "#nu_{#mu}CC-#mu", "#nu_{#mu}CC-no#mu", "no#nu_{#mu}CC" , "out FV" };
  int ccOOFV_code[NCCOOFVTYPES]          = { 0, 1, 2, 3 };
  int ccOOFV_colors[NCCOOFVTYPES]        = { 2, 4, 3, 1 };
  anaUtils::_categ->AddCategory("CCOOFV",   NCCOOFVTYPES, ccOOFV_types, ccOOFV_code, ccOOFV_colors);

  //	const int NOOFVTYPES=11;
  //	int oofv_code[NOOFVTYPES]          = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  //	int oofv_colors[NOOFVTYPES]        = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 28, 29 };
  //	std::string oofvFwd_types[NOOFVTYPES] = {"FV", "OOFV Target1 true<sel", "OOFV Target1 true>sel", "OOFV Upstream", "OOFV Downstream", "OOFV Neutral", "OOFV Bwd", "OOFV HA", "OOFV VeryFwd", "OOFV Last", "OOFV Other"};
  //	std::string oofvBwd_types[NOOFVTYPES] = {"FV", "OOFV Target1 true<sel", "OOFV Target1 true>sel", "OOFV Upstream", "OOFV Downstream", "OOFV Neutral", "OOFV Fwd", "OOFV HA", "OOFV VeryBwd", "OOFV Last", "OOFV Other"};
  //anaUtils::_categ->AddCategory("OOFV_Fwd",   NOOFVTYPES, oofvFwd_types, oofv_code, oofv_colors);
  //	anaUtils::_categ->AddCategory("OOFV_Bwd",   NOOFVTYPES, oofvBwd_types, oofv_code, oofv_colors);

}

//********************************************************************
void numuCCOOFVAnalysis::FillCategories(){
  //********************************************************************

  AnaTrack* track = static_cast<AnaTrack*>( ccoofvbox().MainTrack );
  if(!track){return;}

  bool IsAntinu = (ND::params().GetParameterI("numuCCOOFVAnalysis.NeutrinoMode")==-1);

  bool useTarget1 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget1");
  bool useTarget2 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget2");
  bool useFGD1 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableFGD1");
  bool useFGD2 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableFGD2");

  if ( useTarget1 && !useTarget2)
    anaUtils::FillCategories(&GetEvent(), track, "", SubDetId::kTarget1, IsAntinu);
  if (!useTarget1 &&  useTarget2)
    anaUtils::FillCategories(&GetEvent(), track, "", SubDetId::kTarget2, IsAntinu);
  if ( useTarget1 &&  useTarget2)
    anaUtils::FillCategories(&GetEvent(), track, "", SubDetId::kTarget, IsAntinu);

  if ( useFGD1 && !useFGD2)
    anaUtils::FillCategories(&GetEvent(), track, "", SubDetId::kFGD1, IsAntinu);
  if (!useFGD1 &&  useFGD2)
    anaUtils::FillCategories(&GetEvent(), track, "", SubDetId::kFGD2, IsAntinu);
  if ( useFGD1 &&  useFGD2)
    anaUtils::FillCategories(&GetEvent(), track, "", SubDetId::kFGD, IsAntinu);
  
  anaUtils::_categ->SetCode("CCOOFV",      numuCCOOFV_utils::CCOOFVCategory(track) );
  //	anaUtils::_categ->SetCode("OOFV_Fwd",   numuCCOOFV_utils::OOFVCategory(track,0) );
  //	anaUtils::_categ->SetCode("OOFV_Bwd",   numuCCOOFV_utils::OOFVCategory(track,1) );

}
