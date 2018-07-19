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

  _nuMode     = ND::params().GetParameterI("numuCC4piAnalysis.NeutrinoMode");
  _useTarget1 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget1");
  _useTarget2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableTarget2");
  _useFGD1    = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD1");
  _useFGD2 = ND::params().GetParameterI("numuCC4piAnalysis.EnableFGD2");

  DetDef::Initialize();
  
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
  AddVarF(output(),  selmu_likemu,       ""); 
  AddVarF(output(),  selmu_likee,       ""); 
  AddVarF(output(),  selmu_likemip,      ""); 

  AddVarF(output(), selmu_ToF_time,        "");
  AddVarF(output(), selmu_ToF_true_time,   "");
  AddVarF(output(), selmu_ToF_mass,        "");
  AddVarF(output(), selmu_ToF_true_mass,   "");
  AddVarF(output(), selmu_ToF_mulkl,       "");
  AddVarF(output(), selmu_ToF_elkl,        "");
  AddVarF(output(), selmu_ToF_length,      "");
  AddVarF(output(), selmu_ToF_true_length, "");
  AddVarI(output(), selmu_ToF_firstDet,    "");
  AddVarI(output(), selmu_ToF_secondDet,   "");
  
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
  AddVarF(output(),   selmu_target_length,    "");
  AddVar3VF(output(), selmu_dir,       "");
  AddVar3VF(output(), selmu_enddir,    "");
  AddVar4VF(output(), selmu_pos,       "");
  AddVar4VF(output(), selmu_endpos,    "");
  AddVarI(output(),   selmu_longestTPC,"");
  AddVarF(output(),   selmu_dedx_tpc,  "");
  AddVarI(output(),   selmu_categ,"");

  //--- ECal
  AddVarF(output(),   selmu_ecal_mipem,     "");
  AddVarF(output(),   selmu_ecal_EneOnL,    "");
  AddVarI(output(),   selmu_ecal_stopping,  "");

  //--- Protons information
  AddVarF(output(),   selp_mom,             "");

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
  AddVarVI(output(),  sel_TruePiPlus_merged,  "", sel_nTruePiPlus);
  AddVarVI(output(),  sel_TruePiMinus_merged, "", sel_nTruePiMinus);
  AddVarVI(output(),  sel_TruePiZero_merged,  "", sel_nTruePiZero);

  AddVarI(output(),   sel_nTPCProtons,  "");
  AddVarI(output(),   sel_nTargetProtons,  "");


  if ((bool)ND::params().GetParameterI("numuCC4piAnalysis.StoreAll")) {
    AddVarVI(output(), all_ID, "track ID", all_NTracks);
    AddVarVI(output(), all_parentID, "track parent ID", all_NTracks);
    AddVarVI(output(), all_true_pdg, "true pdg", all_NTracks);
    AddVarVF(output(), all_true_mom, "true start momentum", all_NTracks);
    AddVarVF(output(), all_true_stop_mom, "true stop momentum", all_NTracks);
    AddVarVF(output(), all_reco_mom, "reco momentum (for TPC)", all_NTracks);
    AddVarVF(output(), all_true_dirX, "true direction (X)", all_NTracks);
    AddVarVF(output(), all_true_dirY, "true direction (Y)", all_NTracks);
    AddVarVF(output(), all_true_dirZ, "true direction (Z)", all_NTracks);
    AddVarVF(output(), all_true_posX, "true position (X)", all_NTracks);
    AddVarVF(output(), all_true_posY, "true position (Y)", all_NTracks);
    AddVarVF(output(), all_true_posZ, "true position (Z)", all_NTracks);
    AddVarVF(output(), all_true_target_length, "true length in target", all_NTracks);
    AddVarVF(output(), all_true_total_length, "true total length", all_NTracks);
    AddVarVF(output(), all_TPClkl_mu, "reco PID likelihood (mu hypo)", all_NTracks);
    AddVarVF(output(), all_TPClkl_pi, "reco PID likelihood (pi hypo)", all_NTracks);
    AddVarVF(output(), all_TPClkl_p, "reco PID likelihood (p hypo)", all_NTracks);
    AddVarVF(output(), all_TPClkl_ele, "reco PID likelihood (ele hypo)", all_NTracks);
    AddVarVI(output(), all_reco_inTPC, "is it reco in TPC", all_NTracks);
    AddVarVI(output(), all_reco_inTarget, "is it reco in target", all_NTracks);
    AddVarVI(output(), all_reco_target_asproton, "is it reco as proton in target", all_NTracks);
  }
    

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
  AddVarI(output(), true_contained,   "is track contained");

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
    if (track->nTargetSegments>0 && track->TargetSegments[0])
      output().FillVar(selmu_target_length,           (float)track->TargetSegments[0]->SegLength);
    output().FillVectorVarFromArray(selmu_dir,        track->DirectionStart, 3);
    output().FillVectorVarFromArray(selmu_enddir,     track->DirectionEnd, 3);
    output().FillVectorVarFromArray(selmu_pos,        track->PositionStart, 4);
    output().FillVectorVarFromArray(selmu_endpos,     track->PositionEnd, 4);
    output().FillVar(selmu_likemu,        anaUtils::GetPIDLikelihood( *track, 0));
    output().FillVar(selmu_likee,         anaUtils::GetPIDLikelihood( *track, 1));
    output().FillVar(selmu_likemip,       anaUtils::GetPIDLikelihoodMIP( *track));

    output().FillVar(selmu_ecal_stopping,             cutUtils::StoppingECALCut( *track ));
    output().FillVar(selmu_ecal_mipem,                cc4pibox().track_ECal_MipEM);
    output().FillVar(selmu_ecal_EneOnL,               cc4pibox().track_ECal_EneOnL);
    output().FillVar(selmu_longestTPC,                cc4pibox().TPC_det);
	
    output().FillVar(selmu_categ,                cc4pibox().categMuon);

    if (track->nTPCSegments > 0) {
      AnaTPCParticleB *TPCSegment = static_cast<AnaTPCParticleB*>(track->TPCSegments[0]);
      output().FillVar(selmu_dedx_tpc, TPCSegment->dEdxMeas);
    }

    output().FillVar(selmu_ToF_time,         track->ToF_reco_time);
    output().FillVar(selmu_ToF_true_time,    track->ToF_true_time);
    output().FillVar(selmu_ToF_length,       track->ToF_reco_length);
    output().FillVar(selmu_ToF_true_length,  track->ToF_true_length);
    output().FillVar(selmu_ToF_mass,         anaUtils::ComputeToFMass(track->SmearedMomentum, 
								      track->ToF_reco_time,
								      track->ToF_reco_length));

    output().FillVar(sel_nTPCProtons,    cc4pibox().nProtonTPCtracks);
    output().FillVar(sel_nTargetProtons, cc4pibox().nIsoTargetProtontracks);

    output().FillVar(selp_mom,           cc4pibox().HMProton_mom);

    if (track->TrueObject)
      output().FillVar(selmu_ToF_true_mass,    anaUtils::ComputeToFMass(track->GetTrueParticle()->Momentum, 
									track->ToF_true_time,
									track->ToF_true_length));
    output().FillVar(selmu_ToF_mulkl,        anaUtils::GetToFLikelihood(track, 0, true));
    output().FillVar(selmu_ToF_elkl,         anaUtils::GetToFLikelihood(track, 1, true));

    output().FillVar(selmu_ToF_firstDet,     (int)track->ToF_firstDet);
    output().FillVar(selmu_ToF_secondDet,    (int)track->ToF_secondDet);

    //--

  }

  if (cc4pibox().categMuon >= 0) {
    
    int nu_mode = ND::params().GetParameterI("numuCC4piAnalysis.NeutrinoMode");

    for (unsigned j=0; j<cc4pibox().TruePiPlus.size(); j++) {
      AnaTrueParticleB *part = cc4pibox().TruePiPlus[j];
      if (!part) continue;
      int found=0;

      for (int i=0; i<cc4pibox().nMichelElectrons; i++) {
	AnaTrackB *track = cc4pibox().MichelElectrons[i];
	if (!track->TrueObject) continue;
	if (!track->GetTrueParticle()) continue;
	if (track->GetTrueParticle()->ID        == part->ID ||
	    track->GetTrueParticle()->ParentID  == part->ID ||
	    track->GetTrueParticle()->GParentID == part->ID)
	  found=nu_mode*3;
      }
      for (int i=0; i<cc4pibox().nIsoTargetPiontracks; i++) {
	AnaTrackB *track = cc4pibox().IsoTargetPiontracks[i];
	if (!track->TrueObject) continue;
	if (!track->GetTrueParticle()) continue;
	if (track->GetTrueParticle()->ID == part->ID)
	  found=nu_mode*2;
      }
    
      for (int i=0; i<cc4pibox().nPositivePionTPCtracks; i++) {
	AnaTrackB *track = cc4pibox().PositivePionTPCtracks[i];
	if (!track->TrueObject) continue;
	if (!track->GetTrueParticle()) continue;
	if (track->GetTrueParticle()->ID == part->ID)
	  found=1;
      }
  
      output().FillVectorVar(sel_TruePiPlus_reco, found);
      output().FillVectorVar(sel_TruePiPlus_mom, part->Momentum);
      output().FillVectorVar(sel_TruePiPlus_cos, part->Direction[2]);
      output().FillVectorVar(sel_TruePiPlus_merged, part->Merged);
      output().IncrementCounter(sel_nTruePiPlus);
   
    }


    //--

    for (unsigned j=0; j<cc4pibox().TruePiMinus.size(); j++) {
      AnaTrueParticleB *part = cc4pibox().TruePiMinus[j];
      if (!part) continue;
      int found=0;

      for (int i=0; i<cc4pibox().nMichelElectrons; i++) {
	AnaTrackB *track = cc4pibox().MichelElectrons[i];
	if (!track->TrueObject) continue;
	if (!track->GetTrueParticle()) continue;
	if (track->GetTrueParticle()->ID       == part->ID ||
	    track->GetTrueParticle()->ParentID == part->ID ||
	    track->GetTrueParticle()->GParentID == part->ID)
	  found=nu_mode*(-3);
      }
      for (int i=0; i<cc4pibox().nIsoTargetPiontracks; i++) {
	AnaTrackB *track = cc4pibox().IsoTargetPiontracks[i];
	if (!track->TrueObject) continue;
	if (!track->GetTrueParticle()) continue;
	if (track->GetTrueParticle()->ID       == part->ID)
	  found=nu_mode*(-2);
      }
      for (int i=0; i<cc4pibox().nNegativePionTPCtracks; i++) {
	AnaTrackB *track = cc4pibox().NegativePionTPCtracks[i];
	if (!track->TrueObject) continue;
	if (!track->GetTrueParticle()) continue;
	if (track->GetTrueParticle()->ID       == part->ID)
	  found=1;
      }

      output().FillVectorVar(sel_TruePiMinus_reco, found);
      output().FillVectorVar(sel_TruePiMinus_mom, part->Momentum);
      output().FillVectorVar(sel_TruePiMinus_cos, part->Direction[2]);
      output().FillVectorVar(sel_TruePiMinus_merged, part->Merged);
      output().IncrementCounter(sel_nTruePiMinus);
    }

    //--

    for (unsigned j=0; j<cc4pibox().TruePiZero.size(); j++) {
      AnaTrueParticleB *part = cc4pibox().TruePiZero[j];
      if (!part) continue;
      int found=0;

      for (int i=0; i<cc4pibox().nPosPi0TPCtracks; i++) {
	AnaTrackB *track = cc4pibox().PosPi0TPCtracks[i];
	if (!track->TrueObject) continue;
	if (!track->GetTrueParticle()) continue;
	if (track->GetTrueParticle()->ParentID  == part->ID ||
	    track->GetTrueParticle()->GParentID == part->ID )
	  found=1;
      }
      for (int i=0; i<cc4pibox().nElPi0TPCtracks; i++) {
	AnaTrackB *track = cc4pibox().ElPi0TPCtracks[i];
	if (!track->TrueObject) continue;
	if (!track->GetTrueParticle()) continue;
	if (track->GetTrueParticle()->ParentID  == part->ID ||
	    track->GetTrueParticle()->GParentID == part->ID )
	  found=2;
      }

      output().FillVectorVar(sel_TruePiZero_reco, found);
      output().FillVectorVar(sel_TruePiZero_mom, part->Momentum);
      output().FillVectorVar(sel_TruePiZero_cos, part->Direction[2]);
      output().FillVectorVar(sel_TruePiZero_merged, part->Merged);
      output().IncrementCounter(sel_nTruePiZero);
    }
   
    //--
  }


  if ((bool)ND::params().GetParameterI("numuCC4piAnalysis.StoreAll")) {

    AnaEventB& eventB = *static_cast<AnaEventB*>(_event);
    for (unsigned i=0; i<eventB.nParticles; i++) {

      AnaTrackB* track = static_cast<AnaTrackB*>(eventB.Particles[i]);
      if (!track) continue;
      if (!track->TrueObject) continue;

      output().FillVectorVar(all_ID, track->GetTrueParticle()->ID);
      output().FillVectorVar(all_parentID, track->GetTrueParticle()->ParentID);
      output().FillVectorVar(all_true_pdg, track->GetTrueParticle()->PDG);
      output().FillVectorVar(all_true_mom, track->Momentum);
      output().FillVectorVar(all_true_stop_mom, track->MomentumEnd);
      output().FillVectorVar(all_reco_mom, track->SmearedMomentum);
      output().FillVectorVar(all_true_dirX, track->DirectionStart[0]);
      output().FillVectorVar(all_true_dirY, track->DirectionStart[1]);
      output().FillVectorVar(all_true_dirZ, track->DirectionStart[2]);
      output().FillVectorVar(all_true_posX, track->PositionStart[0]);
      output().FillVectorVar(all_true_posY, track->PositionStart[1]);
      output().FillVectorVar(all_true_posZ, track->PositionStart[2]);

      Float_t target_length=-999.;
      if (track->nTargetSegments>0 && track->TargetSegments[0])
	target_length = track->TargetSegments[0]->SegLength;
      output().FillVectorVar(all_true_target_length, target_length);

      output().FillVectorVar(all_true_total_length, (float)(-999.));
      output().FillVectorVar(all_TPClkl_mu, anaUtils::GetPIDLikelihood( *track, 0));
      output().FillVectorVar(all_TPClkl_ele, anaUtils::GetPIDLikelihood( *track, 1));
      output().FillVectorVar(all_TPClkl_p, anaUtils::GetPIDLikelihood( *track, 2));
      output().FillVectorVar(all_TPClkl_pi, anaUtils::GetPIDLikelihood( *track, 3));

      output().FillVectorVar(all_reco_inTPC, cutUtils::DeltaLYZTPCCut(*track));
      Bool_t target_reco = false, target_asproton = false;
      if (track->nTargetSegments>0 && track->TargetSegments[0]) {
	target_reco = track->TargetSegments[0]->IsReconstructed;
	target_asproton = track->TargetSegments[0]->IdAsProton;
      }
      output().FillVectorVar(all_reco_inTarget, target_reco);
      output().FillVectorVar(all_reco_target_asproton, target_asproton);

      output().IncrementCounter(all_NTracks);

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

  bool TrueNuMuCC = _nuMode*vtx.ReacCode > 0 && _nuMode*vtx.ReacCode < 30 && _nuMode*vtx.NuPDG == 14;
  bool TrueVtxFV = ( (_useTarget1 && anaUtils::InFiducialVolume( SubDetId::kTarget1, vtx.Position )) ||
		     (_useTarget2 && anaUtils::InFiducialVolume( SubDetId::kTarget2, vtx.Position )) ||
		     (_useFGD1    && anaUtils::InFiducialVolume( SubDetId::kFGD1,    vtx.Position )) ||
		     (_useFGD2    && anaUtils::InFiducialVolume( SubDetId::kFGD2,    vtx.Position )) );
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
  AnaTrueParticleB* trueHMTrack=NULL;
  float maxHM_mom=0, max_mom=0;

  int nu_mode = ND::params().GetParameterI("numuCC4piAnalysis.NeutrinoMode");

  for (unsigned int i = 0; i < TrueParticles.size(); i++) {
	if (TrueParticles[i]->ParentID != 0)
		continue;
  
    if (TrueParticles[i]->Momentum > max_mom && TrueParticles[i]->PDG == nu_mode*13){
      trueTrack = dynamic_cast<AnaTrueParticleB*>(TrueParticles[i]);
	  max_mom = TrueParticles[i]->Momentum;
	}
    if (TrueParticles[i]->Momentum > maxHM_mom && nu_mode*TrueParticles[i]->Charge<0){
      trueHMTrack = TrueParticles[i];
      maxHM_mom = TrueParticles[i]->Momentum;
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
	
	bool contained = true;
	if (trueTrack->DetCrossingsVect.size()>1) contained = false;
	if ( !((_useTarget1 && anaUtils::InFiducialVolume( SubDetId::kTarget1, trueTrack->PositionEnd )) ||
		   (_useTarget2 && anaUtils::InFiducialVolume( SubDetId::kTarget2, trueTrack->PositionEnd )) ||
		   (_useFGD1    && anaUtils::InFiducialVolume( SubDetId::kFGD1,    trueTrack->PositionEnd )) ||
		   (_useFGD2    && anaUtils::InFiducialVolume( SubDetId::kFGD2,    trueTrack->PositionEnd ))) )
		contained = false;
	output().FillVar(true_contained, contained);
	
    float phi = atan2(trueTrack->Direction[1], trueTrack->Direction[0]);
    output().FillVar(true_phi, phi);

    TLorentzVector P_nu(0,0,0,0), P_mu(0,0,0,0);
    P_nu.SetVectM(vtx.NuMom*anaUtils::ArrayToTVector3(vtx.NuDir), 0);
    P_mu.SetVectM(trueTrack->Momentum*anaUtils::ArrayToTVector3(trueTrack->Direction), 105.66);

    float Q2 = -(P_mu-P_nu).Mag2()/1e6; // in GeV^2/c^2
    output().FillVar(true_Q2, Q2);

  }
  if (trueHMTrack)
    output().FillVar(trueHMN_pdg, trueHMTrack->PDG);


  if ( _useTarget1 && !_useTarget2)
    anaUtils::FillCategories(&vtx, "", SubDetId::kTarget1, IsAntinu,GetSpill().GetIsSandMC());
  if (!_useTarget1 &&  _useTarget2)
    anaUtils::FillCategories(&vtx, "", SubDetId::kTarget2, IsAntinu,GetSpill().GetIsSandMC());
  if ( _useTarget1 &&  _useTarget2)
    anaUtils::FillCategories(&vtx, "", SubDetId::kTarget, IsAntinu,GetSpill().GetIsSandMC());
  if ( _useFGD1 && !_useFGD2)
    anaUtils::FillCategories(&vtx, "", SubDetId::kFGD1, IsAntinu,GetSpill().GetIsSandMC());
  if (!_useFGD1 &&  _useFGD2)
    anaUtils::FillCategories(&vtx, "", SubDetId::kFGD2, IsAntinu,GetSpill().GetIsSandMC());
  if ( _useFGD1 &&  _useFGD2)
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

  bool IsAntinu = (_nuMode==-1);

  if ( _useTarget1 && !_useTarget2)
    anaUtils::FillCategories(&GetEvent(), track, "", SubDetId::kTarget1, IsAntinu);
  if (!_useTarget1 &&  _useTarget2)
    anaUtils::FillCategories(&GetEvent(), track, "", SubDetId::kTarget2, IsAntinu);
  if ( _useTarget1 &&  _useTarget2)
    anaUtils::FillCategories(&GetEvent(), track, "", SubDetId::kTarget, IsAntinu);

  if ( _useFGD1 && !_useFGD2)
    anaUtils::FillCategories(&GetEvent(), track, "", SubDetId::kFGD1, IsAntinu);
  if (!_useFGD1 &&  _useFGD2)
    anaUtils::FillCategories(&GetEvent(), track, "", SubDetId::kFGD2, IsAntinu);
  if ( _useFGD1 &&  _useFGD2)
    anaUtils::FillCategories(&GetEvent(), track, "", SubDetId::kFGD, IsAntinu);
  
  anaUtils::_categ->SetCode("CC4pi",      numuCC4pi_utils::CC4piCategory(track) );
  //	anaUtils::_categ->SetCode("OOFV_Fwd",   numuCC4pi_utils::OOFVCategory(track,0) );
  //	anaUtils::_categ->SetCode("OOFV_Bwd",   numuCC4pi_utils::OOFVCategory(track,1) );

}
