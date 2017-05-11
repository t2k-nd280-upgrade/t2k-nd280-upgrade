#include "numuCC4piAnalysis.hxx"
#include "CategoriesUtils.hxx"
#include "numuCC4piUtils.hxx"
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
numuCC4piAnalysis::numuCC4piAnalysis(AnalysisAlgorithm* ana) : baseAnalysis(ana) {
  //********************************************************************

  //Add the package version
  //	ND::versioning().AddPackage("numuCC4piAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("NUMUCC4PIANALYSISROOT")));

}

//********************************************************************
bool numuCC4piAnalysis::Initialize(){
  //********************************************************************

  // Initialize the baseAnalysis
  if(!baseAnalysis::Initialize()) return false;

  //Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("numuCC4piAnalysis.MinAccumLevelToSave"));

  //Add muon candidate categories
  numuCC4piAnalysis::AddCategories();


  if (ND::params().GetParameterI("numuCC4piAnalysis.Configuration") == 1) {
    if (ND::params().GetParameterI("numuCC4piAnalysis.ToF") == 1) {
      DetDef::Target1min[2] =  -1737.00;
      DetDef::Target1max[2] =   -537.00;
      DetDef::Target2min[2] =    537.00;
      DetDef::Target2max[2] =   1737.00;
    }
  }
  
  if (ND::params().GetParameterI("numuCC4piAnalysis.Configuration") >= 2) {
    
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
  
  if (ND::params().GetParameterI("numuCC4piAnalysis.Configuration") == 2) {
    if (ND::params().GetParameterI("numuCC4piAnalysis.ToF") == 1) {
      DetDef::Target1min[2] =  -2642.00;
      DetDef::Target1max[2] =   -748.00;
    }
    else {
      DetDef::Target1min[2] =  -2692.00;
      DetDef::Target1max[2] =   -698.00;
    }
  }
  if (ND::params().GetParameterI("numuCC4piAnalysis.Configuration") == 3) {
    if (ND::params().GetParameterI("numuCC4piAnalysis.ToF") == 1) {
      DetDef::Target1min[2] =  -1668.00;
      DetDef::Target1max[2] =    226.00;
    }
    else {
      DetDef::Target1min[2] =  -1718.00;
      DetDef::Target1max[2] =    276.00;
    }
  }

  /*
    if (ND::params().GetParameterI("numuCC4piAnalysis.Configuration") == 4) {
    DetDef::Target1min[1] =   -216.00;
    DetDef::Target1max[1] =    184.00;  
    DetDef::Target1min[2] =  -2692.00;
    DetDef::Target1max[2] =   -698.00;
    }
    if (ND::params().GetParameterI("numuCC4piAnalysis.Configuration") == 5) {
    DetDef::Target1min[1] =   -216.00;
    DetDef::Target1max[1] =    184.00;  
    DetDef::Target1min[2] =  -1718.00;
    DetDef::Target1max[2] =    276.00;
    }
  */
  
  return true;

}

//********************************************************************
void numuCC4piAnalysis::DefineSelections(){
  //********************************************************************

  //bool forceBreak = (bool)ND::params().GetParameterI("numuCC4piAnalysis.ForceBreak");

  // ----- Inclusive CC -----------
  /*
    int branch = ND::params().GetParameterI("numuCC4piAnalysis.Branch");
    if (branch == 0) {
    sel().AddSelection("kTrackerNumuCC4pi", "inclusive numuCC4pi selection", new numuCC4piFwdCanSelection()); //true/false for forcing break
    } else if (branch == 1) {
    sel().AddSelection("kTrackerNumuCC4pi", "inclusive numuCC4pi selection", new numuCC4piBwdCanSelection()); //true/false for forcing break

    } else if (branch == 2) {
    sel().AddSelection("kTrackerNumuCC4pi", "inclusive numuCC4pi selection", new numuCC4piECalCanSelection()); //true/false for forcing break

    }
  */
  sel().AddSelection("kTrackerNumuCC4pi", "inclusive numuCC4pi selection", new numuCC4piSelection()); //true/false for forcing break

}

//********************************************************************
void numuCC4piAnalysis::DefineCorrections(){
  //********************************************************************

  //Some corrections are defined in baseAnalysis
  baseAnalysis::DefineCorrections();
  corr().AddCorrection("mom_corr",          new MomRangeCorrection(SubDetId::kTPC));
  corr().AddCorrection("pid_corr",          new PIDCorrection(SubDetId::kTPC));


}

//********************************************************************
void numuCC4piAnalysis::DefineSystematics(){
  //********************************************************************

  //Some systematics are defined in baseAnalysis
  baseAnalysis::DefineSystematics();

}

//********************************************************************
void numuCC4piAnalysis::DefineConfigurations(){
  //********************************************************************

  baseAnalysis::DefineConfigurations();

}

//********************************************************************
void numuCC4piAnalysis::DefineMicroTrees(bool addBase){
  //********************************************************************

  //-------- Add variables to the analysis tree ----------------------

  //Variables from baseAnalysis (run, event, ...)
  if (addBase) baseAnalysis::DefineMicroTrees(addBase);
  
  //--- toy variables
  AddToyVarI(output(),  NTPC,               "");
  AddToyVarI(output(),  NECal,              "");
  AddToyVarI(output(),  Toy_CC4pi,          "");

  //AddToyVarF(output(),  selmu_MomRangeMuon, "");
  AddToyVarF(output(),  selmu_costheta,     "");
  //AddToyVarF(output(),  selmu_ToF_PoD,      "");
  //AddToyVarF(output(),  selmu_ToF_ECAL,     "");
  //AddToyVarF(output(),  selmu_ToF_FGD2,     "");
  AddVarF(output(),  selmu_likemu,       ""); 
  AddVarF(output(),  selmu_likemip,      ""); 
  //AddToyVarI(output(),  selmu_end_ecal,     "");
  AddVarF(output(), selmu_ToF_mass,      "");
  AddVarF(output(), selmu_ToF_true_mass, "");
  
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
  AddVarF(output(),   selmu_dedx_tpc,  "");

  //--- ECal
  AddVarF(output(),   selmu_ecal_mipem,     "");
  AddVarF(output(),   selmu_ecal_EneOnL,    "");
  AddVarI(output(),   selmu_ecal_stopping,  "");

  //--- All tracks (used for ToF check)

  AddVarVF(output(), selAll_ToF_mass,       "", selAll_nTracks);
  AddVarVF(output(), selAll_ToF_true_mass,  "", selAll_nTracks);
  AddVarVF(output(), selAll_mom,            "", selAll_nTracks);
  AddVarVF(output(), selAll_cos,            "", selAll_nTracks);
  AddVarVF(output(), selAll_true_mom,       "", selAll_nTracks);
  AddVarVI(output(), selAll_PDG,            "", selAll_nTracks);
  AddVarVI(output(), selAll_ToF_det_used1,  "", selAll_nTracks);
  AddVarVI(output(), selAll_ToF_det_used2,  "", selAll_nTracks);
  AddVarVF(output(), selAll_ToF_time_reco,  "", selAll_nTracks);
  AddVarVF(output(), selAll_ToF_pull_muon,     "", selAll_nTracks);
  AddVarVF(output(), selAll_ToF_pull_pion,     "", selAll_nTracks);
  AddVarVF(output(), selAll_ToF_pull_electron, "", selAll_nTracks);
  AddVarVF(output(), selAll_ToF_pull_proton,   "", selAll_nTracks);
  AddVarVF(output(), selAll_ToF_lkl_muon,      "", selAll_nTracks);
  AddVarVF(output(), selAll_ToF_lkl_pion,      "", selAll_nTracks);
  AddVarVF(output(), selAll_ToF_lkl_electron,  "", selAll_nTracks);
  AddVarVF(output(), selAll_ToF_lkl_proton,    "", selAll_nTracks);

  AddVarVF(output(), selAll_TPC_pull_muon,     "", selAll_nTracks);
  AddVarVF(output(), selAll_TPC_pull_pion,     "", selAll_nTracks);
  AddVarVF(output(), selAll_TPC_pull_electron, "", selAll_nTracks);
  AddVarVF(output(), selAll_TPC_pull_proton,   "", selAll_nTracks);
  AddVarVF(output(), selAll_TPC_lkl_muon,      "", selAll_nTracks);
  AddVarVF(output(), selAll_TPC_lkl_pion,      "", selAll_nTracks);
  AddVarVF(output(), selAll_TPC_lkl_electron,  "", selAll_nTracks);
  AddVarVF(output(), selAll_TPC_lkl_proton,    "", selAll_nTracks);
  AddVarVF(output(), selAll_TPC_dedx,          "", selAll_nTracks);

  //--- Pion multiplicity
  
  AddVarVI(output(),  sel_TruePiPlus_reco,  "", sel_nTruePiPlus);
  AddVarVI(output(),  sel_TruePiMinus_reco, "", sel_nTruePiMinus);
  AddVarVI(output(),  sel_TruePiZero_reco,  "", sel_nTruePiZero);
  AddVarVF(output(),  sel_TruePiPlus_mom,   "", sel_nTruePiPlus);
  AddVarVF(output(),  sel_TruePiMinus_mom,  "", sel_nTruePiMinus);
  AddVarVF(output(),  sel_TruePiZero_mom,   "", sel_nTruePiZero);
  AddVarVF(output(),  sel_TruePiPlus_cos,   "", sel_nTruePiPlus);
  AddVarVF(output(),  sel_TruePiMinus_cos,  "", sel_nTruePiMinus);
  AddVarVF(output(),  sel_TruePiZero_cos,   "", sel_nTruePiZero);

  AddVarI(output(),   sel_nOtherPions,      "");
  AddVarI(output(),   sel_nPosPions,        "");
  
  //AddVarVI(output(),   sel_OtherPions_PDG,             "", sel_nOtherPions);
  //AddVarVI(output(),   sel_PosPions_PDG,               "", sel_nPosPions);
  AddVarVI(output(),   sel_MichelElectrons_PDG,        "", sel_nMichelElectrons);
  AddVarVI(output(),   sel_NegativePionTPCtracks_PDG,  "", sel_nNegativePionTPCtracks);
  AddVarVI(output(),   sel_PositivePionTPCtracks_PDG,  "", sel_nPositivePionTPCtracks);
  AddVarVI(output(),   sel_IsoTargetPiontracks_PDG,    "", sel_nIsoTargetPiontracks);
  AddVarVI(output(),   sel_PosPi0TPCtracks_PDG,        "", sel_nPosPi0TPCtracks);
  AddVarVI(output(),   sel_ElPi0TPCtracks_PDG,         "", sel_nElPi0TPCtracks);

  AddVarVI(output(),   sel_MichelElectrons_ParPDG,        "", sel_nMichelElectrons);
  AddVarVI(output(),   sel_NegativePionTPCtracks_ParPDG,  "", sel_nNegativePionTPCtracks);
  AddVarVI(output(),   sel_PositivePionTPCtracks_ParPDG,  "", sel_nPositivePionTPCtracks);
  AddVarVI(output(),   sel_IsoTargetPiontracks_ParPDG,    "", sel_nIsoTargetPiontracks);
  AddVarVI(output(),   sel_PosPi0TPCtracks_ParPDG,        "", sel_nPosPi0TPCtracks);
  AddVarVI(output(),   sel_ElPi0TPCtracks_ParPDG,         "", sel_nElPi0TPCtracks);

  //AddVarVF(output(),   sel_OtherPions_mom,             "", sel_nOtherPions);
  //AddVarVF(output(),   sel_PosPions_mom,               "", sel_nPosPions);
  AddVarVF(output(),   sel_MichelElectrons_mom,        "", sel_nMichelElectrons);
  AddVarVF(output(),   sel_NegativePionTPCtracks_mom,  "", sel_nNegativePionTPCtracks);
  AddVarVF(output(),   sel_PositivePionTPCtracks_mom,  "", sel_nPositivePionTPCtracks);
  AddVarVF(output(),   sel_IsoTargetPiontracks_mom,    "", sel_nIsoTargetPiontracks);
  AddVarVF(output(),   sel_PosPi0TPCtracks_mom,        "", sel_nPosPi0TPCtracks);
  AddVarVF(output(),   sel_ElPi0TPCtracks_mom,         "", sel_nElPi0TPCtracks);

  AddVar4VF(output(),   sel_MichelElectrons_pos,        "");
  AddVar4VF(output(),   sel_NegativePionTPCtracks_pos,  "");
  AddVar4VF(output(),   sel_PositivePionTPCtracks_pos,  "");
  AddVar4VF(output(),   sel_IsoTargetPiontracks_pos,    "");
  AddVar4VF(output(),   sel_PosPi0TPCtracks_pos,        "");
  AddVar4VF(output(),   sel_ElPi0TPCtracks_pos,         "");

}

//********************************************************************
void numuCC4piAnalysis::DefineTruthTree(){
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
void numuCC4piAnalysis::FillMicroTrees(bool addBase){
  //********************************************************************

  if (addBase) baseAnalysis::FillMicroTreesBase(addBase);
  if ( cc4pibox().MainTrack ){

    EventBoxB* EventBox = (*_event).EventBoxes[EventBoxId::kEventBoxNDUP];
    (void)EventBox;
    
    if (cc4pibox().MainTrack->TrueObject) {
      AnaTrueVertex *vtx = static_cast<AnaTrueVertex*>(cc4pibox().MainTrack->GetTrueParticle()->TrueVertex);
      if(vtx) {
	output().FillVar(truelepton_PDG,                    vtx->LeptonPDG);
	output().FillVar(truelepton_mom,                vtx->LeptonMom);
	output().FillVectorVarFromArray(truelepton_dir, vtx->LeptonDir,3);
	double costheta_mu_nu = cos(anaUtils::ArrayToTVector3(vtx->LeptonDir).Angle(anaUtils::ArrayToTVector3(vtx->NuDir)));
	output().FillVar(truelepton_costheta, (Float_t)costheta_mu_nu);
	output().FillVectorVarFromArray(truelepton_pos, vtx->Position,4);
      }
      output().FillVar(selmu_ID,                        cc4pibox().MainTrack->GetTrueParticle()->ID);
      output().FillVar(selmu_PDG,                       cc4pibox().MainTrack->GetTrueParticle()->PDG);
      output().FillVar(selmu_truemom,                   cc4pibox().MainTrack->GetTrueParticle()->Momentum);
      output().FillVectorVarFromArray(selmu_truedir,    cc4pibox().MainTrack->GetTrueParticle()->Direction, 3);
      output().FillVectorVarFromArray(selmu_truepos,    cc4pibox().MainTrack->GetTrueParticle()->Position, 4);
      output().FillVectorVarFromArray(selmu_trueendpos, cc4pibox().MainTrack->GetTrueParticle()->PositionEnd, 4);

    }

    AnaTrackB* track = cc4pibox().MainTrack;

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
    output().FillVar(selmu_ecal_mipem,                cc4pibox().track_ECal_MipEM);
    output().FillVar(selmu_ecal_EneOnL,               cc4pibox().track_ECal_EneOnL);
    output().FillVar(selmu_longestTPC,                cc4pibox().TPC_det);

    if (track->nTPCSegments > 0) {
      AnaTPCParticleB *TPCSegment = static_cast<AnaTPCParticleB*>(track->TPCSegments[0]);
      output().FillVar(selmu_dedx_tpc, TPCSegment->dEdxMeas);                 
    }

    output().FillVar(selmu_ToF_mass,                  cc4pibox().ToF_mass);
    output().FillVar(selmu_ToF_true_mass,             cc4pibox().ToF_true_mass);

    output().FillVar(sel_nOtherPions,      cc4pibox().nOtherPions);
    output().FillVar(sel_nPosPions,        cc4pibox().nPosPions);


    //--

    int nu_mode = ND::params().GetParameterI("numuCC4piAnalysis.NeutrinoMode");


    for (unsigned j=0; j<cc4pibox().TruePiPlus.size(); j++) {
      AnaTrueParticleB *part = cc4pibox().TruePiPlus[j];
      int found=0;

      for (int i=0; i<cc4pibox().nMichelElectrons; i++) {
	AnaTrackB *track = cc4pibox().MichelElectrons[i];
	if (!track->TrueObject) continue;
	if (track->GetTrueParticle()->ID        == part->ID ||
	    track->GetTrueParticle()->ParentID  == part->ID ||
	    track->GetTrueParticle()->GParentID == part->ID)
	  found=nu_mode*3;
      }
      for (int i=0; i<cc4pibox().nIsoTargetPiontracks; i++) {
	AnaTrackB *track = cc4pibox().IsoTargetPiontracks[i];
	if (!track->TrueObject) continue;
	if (track->GetTrueParticle()->ID       == part->ID ||
	    track->GetTrueParticle()->ParentID == part->ID )
	  found=nu_mode*2;
      }
      for (int i=0; i<cc4pibox().nPositivePionTPCtracks; i++) {
	AnaTrackB *track = cc4pibox().PositivePionTPCtracks[i];
	if (!track->TrueObject) continue;
	if (track->GetTrueParticle()->ID       == part->ID ||
	    track->GetTrueParticle()->ParentID == part->ID )
	  found=1;
      }

      output().FillVectorVar(sel_TruePiPlus_reco, found);
      output().FillVectorVar(sel_TruePiPlus_mom, part->Momentum);
      output().FillVectorVar(sel_TruePiPlus_cos, part->Direction[2]);
      output().IncrementCounter(sel_nTruePiPlus);
    }

    //--

    for (unsigned j=0; j<cc4pibox().TruePiMinus.size(); j++) {
      AnaTrueParticleB *part = cc4pibox().TruePiMinus[j];
      int found=0;

      for (int i=0; i<cc4pibox().nMichelElectrons; i++) {
	AnaTrackB *track = cc4pibox().MichelElectrons[i];
	if (!track->TrueObject) continue;
	if (track->GetTrueParticle()->ID       == part->ID ||
	    track->GetTrueParticle()->ParentID == part->ID ||
	    track->GetTrueParticle()->GParentID == part->ID)
	  found=nu_mode*(-3);
      }
      for (int i=0; i<cc4pibox().nIsoTargetPiontracks; i++) {
	AnaTrackB *track = cc4pibox().IsoTargetPiontracks[i];
	if (!track->TrueObject) continue;
	if (track->GetTrueParticle()->ID       == part->ID ||
	    track->GetTrueParticle()->ParentID == part->ID )
	  found=nu_mode*(-2);
      }
      for (int i=0; i<cc4pibox().nNegativePionTPCtracks; i++) {
	AnaTrackB *track = cc4pibox().NegativePionTPCtracks[i];
	if (!track->TrueObject) continue;
	if (track->GetTrueParticle()->ID       == part->ID ||
	    track->GetTrueParticle()->ParentID == part->ID )
	  found=1;
      }

      output().FillVectorVar(sel_TruePiMinus_reco, found);
      output().FillVectorVar(sel_TruePiMinus_mom, part->Momentum);
      output().FillVectorVar(sel_TruePiMinus_cos, part->Direction[2]);
      output().IncrementCounter(sel_nTruePiMinus);
    }

    //--

    for (unsigned j=0; j<cc4pibox().TruePiZero.size(); j++) {
      AnaTrueParticleB *part = cc4pibox().TruePiZero[j];
      int found=0;

      for (int i=0; i<cc4pibox().nPosPi0TPCtracks; i++) {
	AnaTrackB *track = cc4pibox().PosPi0TPCtracks[i];
	if (!track->TrueObject) continue;
	if (track->GetTrueParticle()->ParentID  == part->ID ||
	    track->GetTrueParticle()->GParentID == part->ID )
	  found=1;
      }
      for (int i=0; i<cc4pibox().nElPi0TPCtracks; i++) {
	AnaTrackB *track = cc4pibox().ElPi0TPCtracks[i];
	if (!track->TrueObject) continue;
	if (track->GetTrueParticle()->ParentID  == part->ID ||
	    track->GetTrueParticle()->GParentID == part->ID )
	  found=2;
      }

      output().FillVectorVar(sel_TruePiZero_reco, found);
      output().FillVectorVar(sel_TruePiZero_mom, part->Momentum);
      output().FillVectorVar(sel_TruePiZero_cos, part->Direction[2]);
      output().IncrementCounter(sel_nTruePiZero);
    }

    //--


    for (int i=0; i<cc4pibox().nMichelElectrons; i++) {
      AnaTrackB *track = cc4pibox().MichelElectrons[i];
      output().FillVectorVar(sel_MichelElectrons_mom, track->GetTrueParticle()->Momentum);
      if (i==0)
	output().FillVectorVarFromArray(sel_MichelElectrons_pos, track->PositionStart, 4);
      if (track->TrueObject){
	output().FillVectorVar(sel_MichelElectrons_PDG, track->GetTrueParticle()->PDG);
	output().FillVectorVar(sel_MichelElectrons_ParPDG, track->GetTrueParticle()->ParentPDG);
      }
      output().IncrementCounter(sel_nMichelElectrons);
    }

    for (int i=0; i<cc4pibox().nNegativePionTPCtracks; i++) {
      AnaTrackB *track = cc4pibox().NegativePionTPCtracks[i];
      output().FillVectorVar(sel_NegativePionTPCtracks_mom, track->GetTrueParticle()->Momentum);
      if (i==0)
	output().FillVectorVarFromArray(sel_NegativePionTPCtracks_pos, track->PositionStart, 4);
      if (track->TrueObject){
	output().FillVectorVar(sel_NegativePionTPCtracks_PDG, track->GetTrueParticle()->PDG);
	output().FillVectorVar(sel_NegativePionTPCtracks_ParPDG, track->GetTrueParticle()->ParentPDG);
      }
      output().IncrementCounter(sel_nNegativePionTPCtracks);
    }

    for (int i=0; i<cc4pibox().nPositivePionTPCtracks; i++) {
      AnaTrackB *track = cc4pibox().PositivePionTPCtracks[i];
      output().FillVectorVar(sel_PositivePionTPCtracks_mom, track->GetTrueParticle()->Momentum);
      if (track->TrueObject){
	output().FillVectorVar(sel_PositivePionTPCtracks_PDG, track->GetTrueParticle()->PDG);
	output().FillVectorVar(sel_PositivePionTPCtracks_ParPDG, track->GetTrueParticle()->ParentPDG);
      }
      output().IncrementCounter(sel_nPositivePionTPCtracks);
    }

    for (int i=0; i<cc4pibox().nIsoTargetPiontracks; i++) {
      AnaTrackB *track = cc4pibox().IsoTargetPiontracks[i];
      output().FillVectorVar(sel_IsoTargetPiontracks_mom, track->GetTrueParticle()->Momentum);
      if (i==0)
	output().FillVectorVarFromArray(sel_IsoTargetPiontracks_pos, track->PositionStart, 4);
      if (track->TrueObject){
	output().FillVectorVar(sel_IsoTargetPiontracks_PDG, track->GetTrueParticle()->PDG);
	output().FillVectorVar(sel_IsoTargetPiontracks_ParPDG, track->GetTrueParticle()->ParentPDG);
      }
      output().IncrementCounter(sel_nIsoTargetPiontracks);
    }

    for (int i=0; i<cc4pibox().nPosPi0TPCtracks; i++) {
      AnaTrackB *track = cc4pibox().PosPi0TPCtracks[i];
      output().FillVectorVar(sel_PosPi0TPCtracks_mom, track->GetTrueParticle()->Momentum);
      if (i==0)
	output().FillVectorVarFromArray(sel_PosPi0TPCtracks_pos, track->PositionStart, 4);
      if (track->TrueObject){
	output().FillVectorVar(sel_PosPi0TPCtracks_PDG, track->GetTrueParticle()->PDG);
	output().FillVectorVar(sel_PosPi0TPCtracks_ParPDG, track->GetTrueParticle()->ParentPDG);
      }
      output().IncrementCounter(sel_nPosPi0TPCtracks);
    }

    for (int i=0; i<cc4pibox().nElPi0TPCtracks; i++) {
      AnaTrackB *track = cc4pibox().ElPi0TPCtracks[i];
      output().FillVectorVar(sel_ElPi0TPCtracks_mom, track->GetTrueParticle()->Momentum);
      if (i==0)
	output().FillVectorVarFromArray(sel_ElPi0TPCtracks_pos, track->PositionStart, 4);
      if (track->TrueObject){
	output().FillVectorVar(sel_ElPi0TPCtracks_PDG, track->GetTrueParticle()->PDG);
	output().FillVectorVar(sel_ElPi0TPCtracks_ParPDG, track->GetTrueParticle()->ParentPDG);
      }
      output().IncrementCounter(sel_nElPi0TPCtracks);
    }

    for (int i=0; i<cc4pibox().All_ToF_mass.size(); i++) {
      output().FillVectorVar(selAll_ToF_mass,          cc4pibox().All_ToF_mass[i]);
      output().FillVectorVar(selAll_ToF_true_mass,     cc4pibox().All_ToF_true_mass[i]);
      output().FillVectorVar(selAll_PDG,               cc4pibox().All_PDG[i]);
      output().FillVectorVar(selAll_mom,               cc4pibox().All_mom[i]);
      output().FillVectorVar(selAll_cos,               cc4pibox().All_cos[i]);
      output().FillVectorVar(selAll_true_mom,          cc4pibox().All_true_mom[i]);
      output().FillVectorVar(selAll_ToF_det_used1,     cc4pibox().All_ToF_det_used1[i]);
      output().FillVectorVar(selAll_ToF_det_used2,     cc4pibox().All_ToF_det_used2[i]);
      output().FillVectorVar(selAll_ToF_time_reco,     cc4pibox().All_ToF_time_reco[i]);

      output().FillVectorVar(selAll_ToF_pull_muon,     cc4pibox().All_ToF_pull_muon[i]);
      output().FillVectorVar(selAll_ToF_pull_pion,     cc4pibox().All_ToF_pull_pion[i]);
      output().FillVectorVar(selAll_ToF_pull_electron, cc4pibox().All_ToF_pull_electron[i]);
      output().FillVectorVar(selAll_ToF_pull_proton,   cc4pibox().All_ToF_pull_proton[i]);
      output().FillVectorVar(selAll_ToF_lkl_muon,      cc4pibox().All_ToF_lkl_muon[i]);
      output().FillVectorVar(selAll_ToF_lkl_pion,      cc4pibox().All_ToF_lkl_pion[i]);
      output().FillVectorVar(selAll_ToF_lkl_electron,  cc4pibox().All_ToF_lkl_electron[i]);
      output().FillVectorVar(selAll_ToF_lkl_proton,    cc4pibox().All_ToF_lkl_proton[i]);

      output().FillVectorVar(selAll_TPC_pull_muon,     cc4pibox().All_TPC_pull_muon[i]);
      output().FillVectorVar(selAll_TPC_pull_pion,     cc4pibox().All_TPC_pull_pion[i]);
      output().FillVectorVar(selAll_TPC_pull_electron, cc4pibox().All_TPC_pull_electron[i]);
      output().FillVectorVar(selAll_TPC_pull_proton,   cc4pibox().All_TPC_pull_proton[i]);
      output().FillVectorVar(selAll_TPC_lkl_muon,      cc4pibox().All_TPC_lkl_muon[i]);
      output().FillVectorVar(selAll_TPC_lkl_pion,      cc4pibox().All_TPC_lkl_pion[i]);
      output().FillVectorVar(selAll_TPC_lkl_electron,  cc4pibox().All_TPC_lkl_electron[i]);
      output().FillVectorVar(selAll_TPC_lkl_proton,    cc4pibox().All_TPC_lkl_proton[i]);
      output().FillVectorVar(selAll_TPC_dedx,          cc4pibox().All_TPC_dedx[i]);

      output().IncrementCounter(selAll_nTracks);
    }

  }

}

//********************************************************************
void numuCC4piAnalysis::FillToyVarsInMicroTrees(bool addBase){
  //********************************************************************


  if (addBase) baseAnalysis::FillToyVarsInMicroTreesBase(addBase);
 
  if ( cc4pibox().MainTrack ){
    output().FillToyVar(NTPC,    (int)cc4pibox().TPCTracks.size() );
    output().FillToyVar(NECal,   (int)cc4pibox().ECalTracks.size() );

    AnaTrack* track = static_cast<AnaTrack*>( cc4pibox().MainTrack );
    output().FillToyVar(Toy_CC4pi, numuCC4pi_utils::CC4piCategory(track));

    //output().FillToyVar(selmu_MomRangeMuon,  track->RangeMomentumMuon);

    TVector3 nuDirVec = anaUtils::GetNuDirRec(track->PositionStart);
    TVector3 muDirVec = anaUtils::ArrayToTVector3(track->DirectionStart);
    double costheta_mu_nu = nuDirVec.Dot(muDirVec);
    output().FillToyVar(selmu_costheta,      (Float_t)costheta_mu_nu);
    //output().FillToyVar(selmu_ToF_PoD,       track->ToF.P0D_FGD1);
    //output().FillToyVar(selmu_ToF_ECAL,      track->ToF.ECal_FGD1);
    //output().FillToyVar(selmu_ToF_FGD2,      track->ToF.FGD1_FGD2);
    //output().FillToyVar(selmu_end_ecal,      cutUtils::StoppingBrECALorSMRDCut(track->PositionEnd));

  }
  

}

//********************************************************************
bool numuCC4piAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx) {
  //********************************************************************

  //int branch = ND::params().GetParameterI("numuCC4piAnalysis.Branch");

  bool useTarget1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget1");
  bool useTarget2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget2");
  bool useFGD1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD1");
  bool useFGD2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD2");

  int nu_mode = ND::params().GetParameterI("numuCC4piAnalysis.NeutrinoMode");

  bool TrueNuMuCC = nu_mode*vtx.ReacCode > 0 && nu_mode*vtx.ReacCode < 30 && nu_mode*vtx.NuPDG == 14;
  bool TrueVtxFV = ( (useTarget1 && anaUtils::InFiducialVolume( SubDetId::kTarget1, vtx.Position )) ||
		     (useTarget2 && anaUtils::InFiducialVolume( SubDetId::kTarget2, vtx.Position )) ||
		     (useFGD1    && anaUtils::InFiducialVolume( SubDetId::kFGD1,    vtx.Position )) ||
		     (useFGD2    && anaUtils::InFiducialVolume( SubDetId::kFGD2,    vtx.Position )) );
  
  /*
    std::vector<AnaTrueParticleB*> TrueParticles = vtx.TrueParticlesVect;

    AnaTrueParticleB* trueTrack = NULL;
    bool top = false;
    for (Int_t i = 0; i < TrueParticles.size(); i++) {
    if (TrueParticles[i]->PDG == 13) {
    trueTrack = dynamic_cast<AnaTrueParticleB*>(TrueParticles[i]);
    if (branch == 0) {
    if (trueTrack->CosTheta >= 0) {
    top = true;
    }
    } else if (branch == 1) {
    if (trueTrack->CosTheta < 0) {
    top = true;
    }

    } else if (branch == 2) {
    for (int i = 0; i < trueTrack->DetCrossingsVect.size(); i++) {
    if (SubDetId::GetDetectorUsed(trueTrack->DetCrossingsVect[i]->Detector, SubDetId::kDsECal) || SubDetId::GetDetectorUsed(trueTrack->DetCrossingsVect[i]->Detector, SubDetId::kBrlECal) || SubDetId::GetDetectorUsed(trueTrack->DetCrossingsVect[i]->Detector, SubDetId::kP0DECal) ) {
    if ( !cutUtils::DeltaLYZTPCCut(*trueTrack)) {
    top = true;
    }
    }
    }
    }
    }
    }
  */
  return TrueNuMuCC && TrueVtxFV ;

  return false;
}

//********************************************************************
void numuCC4piAnalysis::FillTruthTree(const AnaTrueVertex& vtx) {
  //********************************************************************

  bool IsAntinu = (ND::params().GetParameterI("numuCC4piAnalysis.NeutrinoMode")==-1);
  baseAnalysis::FillTruthTreeBase(vtx, SubDetId::kTarget, IsAntinu);

  std::vector<AnaTrueParticleB*> TrueParticles = vtx.TrueParticlesVect;
  output().FillVar(true_Nu_pdg, vtx.NuPDG);
  output().FillVar(true_Nu_mom, vtx.NuMom);
  output().FillVar(true_Target_pdg, vtx.TargetPDG);

  output().FillVar(true_reaction_code, vtx.ReacCode);
  output().FillVectorVarFromArray(true_vertex_position, vtx.Position, 4);
  AnaTrueParticleB* trueTrack=NULL;
  AnaTrueParticleB* trueHMNTrack=NULL; float max_mom=0;

  int nu_mode = ND::params().GetParameterI("numuCC4piAnalysis.NeutrinoMode");

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

  bool useTarget1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget1");
  bool useTarget2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget2");
  bool useFGD1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD1");
  bool useFGD2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD2");

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
void numuCC4piAnalysis::AddCategories(){
  //********************************************************************

  anaUtils::AddStandardCategories();

  const int NCC4PITYPES=4;
  std::string cc4pi_types[NCC4PITYPES] = { "#nu_{#mu}CC-#mu", "#nu_{#mu}CC-no#mu", "no#nu_{#mu}CC" , "out FV" };
  int cc4pi_code[NCC4PITYPES]          = { 0, 1, 2, 3 };
  int cc4pi_colors[NCC4PITYPES]        = { 2, 4, 3, 1 };
  anaUtils::_categ->AddCategory("CC4pi",   NCC4PITYPES, cc4pi_types, cc4pi_code, cc4pi_colors);

  //	const int NOOFVTYPES=11;
  //	int oofv_code[NOOFVTYPES]          = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  //	int oofv_colors[NOOFVTYPES]        = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 28, 29 };
  //	std::string oofvFwd_types[NOOFVTYPES] = {"FV", "OOFV Target1 true<sel", "OOFV Target1 true>sel", "OOFV Upstream", "OOFV Downstream", "OOFV Neutral", "OOFV Bwd", "OOFV HA", "OOFV VeryFwd", "OOFV Last", "OOFV Other"};
  //	std::string oofvBwd_types[NOOFVTYPES] = {"FV", "OOFV Target1 true<sel", "OOFV Target1 true>sel", "OOFV Upstream", "OOFV Downstream", "OOFV Neutral", "OOFV Fwd", "OOFV HA", "OOFV VeryBwd", "OOFV Last", "OOFV Other"};
  //anaUtils::_categ->AddCategory("OOFV_Fwd",   NOOFVTYPES, oofvFwd_types, oofv_code, oofv_colors);
  //	anaUtils::_categ->AddCategory("OOFV_Bwd",   NOOFVTYPES, oofvBwd_types, oofv_code, oofv_colors);

}

//********************************************************************
void numuCC4piAnalysis::FillCategories(){
  //********************************************************************

  AnaTrack* track = static_cast<AnaTrack*>( cc4pibox().MainTrack );
  if(!track){return;}

  bool IsAntinu = (ND::params().GetParameterI("numuCC4piAnalysis.NeutrinoMode")==-1);

  bool useTarget1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget1");
  bool useTarget2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget2");
  bool useFGD1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD1");
  bool useFGD2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD2");

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
  
  anaUtils::_categ->SetCode("CC4pi",      numuCC4pi_utils::CC4piCategory(track) );
  //	anaUtils::_categ->SetCode("OOFV_Fwd",   numuCC4pi_utils::OOFVCategory(track,0) );
  //	anaUtils::_categ->SetCode("OOFV_Bwd",   numuCC4pi_utils::OOFVCategory(track,1) );

}
