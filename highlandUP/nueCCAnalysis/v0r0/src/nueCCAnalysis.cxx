#include "nueCCAnalysis.hxx"
#include "nueCCUtils.hxx"

#include "CategoriesUtils.hxx"
#include "CutUtils.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "BasicUtils.hxx"
#include "MomRangeCorrection.hxx"
#include "PIDCorrection.hxx"
#include "CutUtils.hxx"
#include "NuDirUtils.hxx"
#include "ConstituentsUtils.hxx"
#include "TreeConverterUtils.hxx"

const UInt_t NMAXMU = 500;

//********************************************************************
nueCCAnalysis::nueCCAnalysis(AnalysisAlgorithm* ana) : baseAnalysis(ana) {
  //********************************************************************

  //Add the package version
  //	ND::versioning().AddPackage("numuCC4piAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("NUMUCC4PIANALYSISROOT")));

}

//********************************************************************
bool nueCCAnalysis::Initialize(){
  //********************************************************************

  // Initialize the baseAnalysis
  if(!baseAnalysis::Initialize()) return false;

  //Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("nueCCAnalysis.MinAccumLevelToSave"));

  // Run nuE / nuE bar selection
  _AntiNu = false;
  if (ND::params().GetParameterI("nueCCAnalysis.SelectCharge") > 0)
    _AntiNu = true;
  //Add electron candidate categories
  nueCCUtils::AddCategories(_AntiNu);

  // Target for the selection
  _useTarget1 = ND::params().GetParameterI("nueCCAnalysis.EnableTarget1");
  _useTarget2 = ND::params().GetParameterI("nueCCAnalysis.EnableTarget2");
  _useFGD1    = ND::params().GetParameterI("nueCCAnalysis.EnableFGD1");
  _useFGD2    = ND::params().GetParameterI("nueCCAnalysis.EnableFGD2");

  DetDef::Initialize();

  return true;

}

//********************************************************************
void nueCCAnalysis::DefineSelections(){
  //********************************************************************

  bool Inclusive = (bool)ND::params().GetParameterI("nueCCAnalysis.Inclusive");
  
  if (Inclusive)
    sel().AddSelection("kTrackerNueCC",        "inclusive nueCC4pi selection", new nueCCSelection()); //true/false for forcing break
  else 
    sel().AddSelection("kTrackerNueCCMultiPi", "MultiPi nueCC4pi selection",    new nueCCMultiPiSelection()); //true/false for forcing break

}

//********************************************************************
void nueCCAnalysis::DefineCorrections(){
  //********************************************************************

  //Some corrections are defined in baseAnalysis
  baseAnalysis::DefineCorrections();
  corr().AddCorrection("mom_corr",          new MomRangeCorrection(SubDetId::kTPC));
  corr().AddCorrection("pid_corr",          new PIDCorrection(SubDetId::kTPC));

}

//********************************************************************
void nueCCAnalysis::DefineSystematics(){
  //********************************************************************

  //Some systematics are defined in baseAnalysis
  baseAnalysis::DefineSystematics();

}

//********************************************************************
void nueCCAnalysis::DefineConfigurations(){
  //********************************************************************

  baseAnalysis::DefineConfigurations();

}

//********************************************************************
void nueCCAnalysis::DefineMicroTrees(bool addBase){
  //********************************************************************

  //-------- Add variables to the analysis tree ----------------------

  //Variables from baseAnalysis (run, event, ...)
  if (addBase) baseAnalysis::DefineMicroTrees(addBase);
  
  //--- toy variables
  AddToyVarI(output(),  NTPC,                "");
  AddToyVarI(output(),  NECal,               "");
  AddToyVarI(output(),  Toy_CC4pi,           "");

  AddToyVarF(output(),  selelec_costheta,      "");
  AddVarF(output(),     selelec_pull_mu,       ""); 
  AddVarF(output(),     selelec_pull_ele,      ""); 
  AddVarF(output(),     selelec_pull_pi,       ""); 
  AddVarF(output(),     selelec_pull_pr,       ""); 
  AddVarF(output(),     selelec_likemu,        ""); 
  AddVarF(output(),     selelec_likeel,        ""); 
  AddVarF(output(),     selelec_likepi,        ""); 
  AddVarF(output(),     selelec_likepr,        ""); 
  AddVarF(output(),     selelec_likemip,       "");

  // fill the ToF info
  AddVarI(output(),     selelec_first_ToF,          "");
  AddVarI(output(),     selelec_second_ToF,         "");
  AddVarF(output(),     Beta_ToF,                   "");
  AddVarF(output(),     Beta_true_ToF,              "");
  AddVarF(output(),     selelec_ToF,                "");
  AddVarF(output(),     selelec_ToF_true,           "");
  AddVarF(output(),     selelec_ToF_mass,           "");
  AddVarF(output(),     selelec_ToF_true_mass,      "");
  AddVarF(output(),     selelec_ToF_lkl_muon,       "");
  AddVarF(output(),     selelec_ToF_lkl_pion,       "");
  AddVarF(output(),     selelec_ToF_lkl_electron,   "");
  AddVarF(output(),     selelec_ToF_lkl_proton,     "");
  
  //--- info from true vertex
  AddVarI(output(),     truelepton_PDG,       "");
  AddVarF(output(),     truelepton_mom,       "");
  AddVar3VF(output(),   truelepton_dir,       "");
  AddVarF(output(),     truelepton_costheta,  "");
  AddVar4VF(output(),   truelepton_pos,       "");
  AddVarI(output(),     truelepton_det,       "");

  //--- info from true track
  AddVarI(output(),     selelec_ID,             "");
  AddVarI(output(),     selelec_PDG,            "");
  AddVarI(output(),     selelec_ParentPDG,      "");
  AddVarI(output(),     selelec_GParentPDG,     "");
  AddVarF(output(),     selelec_mom,            "");
  AddVarF(output(),     selelec_truemom,        "");
  AddVarF(output(),     selelec_truecostheta,   "");
  AddVarI(output(),     selelec_stopped,        "");
  AddVar4VF(output(),   selelec_truepos,        "");
  AddVar4VF(output(),   selelec_trueendpos,     "");
  AddVar3VF(output(),   selelec_truedir,        "");
  AddVarI(output(),     selelec_flipped,        "");
  AddVarI(output(),     selelec_true_isoTar,    "");

  //--- info by global
  AddVarI(output(),     selelec_detectors,      "");
  AddVarF(output(),     selelec_charge,         "");
  AddVar3VF(output(),   selelec_dir,            "");
  AddVar3VF(output(),   selelec_enddir,         "");
  AddVar4VF(output(),   selelec_pos,            "");
  AddVar4VF(output(),   selelec_endpos,         "");
  AddVarI(output(),     selelec_longestTPC,     "");
  AddVarF(output(),     selelec_dedx_tpc,       "");

  AddVarF(output(),     true_target_ele_length,        "");
  AddVarF(output(),     true_target_pi_length,        "");
  AddVarF(output(),     true_target_mu_length,        "");
  AddVarF(output(),     true_target_pr_length,        "");


  //--- ECal
  AddVarF(output(),     selelec_ecal_mipem,     "");
  AddVarF(output(),     selelec_ecal_EneOnL,    "");
  AddVarI(output(),     selelec_ecal_stopping,  "");

  AddVarI(output(),     true_Nu_pdg,          "");
  AddVarF(output(),     true_Nu_mom,          "");

  AddVarF(output(),     TPCVeto_Deltaz,       "");
  AddVarF(output(),     ECalVeto_Deltaz,      "");
  AddVarI(output(),     NNearTracks,          "");
  AddVarI(output(),     NFarTracks,           "");

  //--- Pair track
  AddVarF(output(),   PairTrack_mom,                   "Momentum of the matched e-/e+ pair");
  AddVarF(output(),   PairTrack_costheta,              "Angle of the matched e-/e+ pair");
  AddVarI(output(),   PairTrack_pdg,                   "Pdg of the matched e-/e+ pair");
  AddVarF(output(),   PairTrack_charge,                "Charge of the matched e-/e+ pair");
  AddVarF(output(),   PairTrack_InvMass,               "Invariant mass of the matched e-/e+ pair");
  AddVar4VF(output(), PairTrack_startdir,              "Recon direction of the matched e-/e+ paired track");
  AddVarF(output(),   PairTrack_DistToMainTrack,       "Distance of the matched e-/e+ pair");
  AddVar4VF(output(), PairTrack_startpos,              "Recon position of the matched e-/e+ paired track");  
  AddVar4VF(output(), PairTrack_true_startdir,         "True direction of the matched e-/e+ paired track"); 
  AddVar4VF(output(), PairTrack_true_startpos,         "True position of the matched e-/e+ paired track");
  AddVarF(output(),   PairTrack_likemuon,              "Paired track Muon pull in the most upstream TPC");
  AddVarF(output(),   PairTrack_likeelec,              "Paired track Electron pull in the most upstream TPC");
  AddVarF(output(),   PairTrack_likepion,              "Paired track Pion pull in the most upstream TPC");
  AddVarF(output(),   PairTrack_likeprot,              "Paired track Proton pull in the most upstream TPC");
  AddVar3VF(output(), PairTrack_gamma_dir, 	       "Direction of (PairTrack + SelectedTrack) ") ;
  AddVar3VF(output(), PairTrack_gamma_true_dir,        "True direction of (PairTrack + SelectedTrack) ") ;
  AddVarF(output(),   TruePairTrack_true_mom,          "Momentum of the true paired track from gamma background");
  AddVarI(output(),   TruePairTrack_pdg,               "PDG of the true paired track from gamma background");

}

//********************************************************************
void nueCCAnalysis::DefineTruthTree(){
  //********************************************************************

  baseAnalysis::DefineTruthTree();

  AddVarI(output(),   true_Nu_pdg,           "nu pdg");
  AddVarI(output(),   true_Target_pdg,       "target pdg");
  AddVarF(output(),   true_Nu_mom,           "nu momentum");
  AddVarI(output(),   true_reaction_code,    "reaction_code");
  AddVar4VF(output(), true_vertex_position,  "vertex position");

  AddVarI(output(),   true_parentID,         "true_parentID");                  
  AddVarI(output(),   true_det,              "true detector"); 
  AddVarI(output(),   true_pdg,              "pdg");
  AddVarF(output(),   true_SDlength,         "SD length");
  AddVarF(output(),   true_mom,              "mom");
  AddVarF(output(),   true_costheta,         "costheta");
  AddVarF(output(),   true_phi,              "phi");
  AddVarF(output(),   true_charge,           "charge");
  AddVarF(output(),   true_ekin,             "kinetic energy");
  AddVar4VF(output(), true_position,         "start position");
  AddVar4VF(output(), true_end,              "end position");
  AddVarF(output(),   true_Q2,               "Q2");
  AddVarI(output(),   true_TPC,              "TPC detector");

  AddVarI(output(),     selelec_true_isoTar,    "");

  AddVarI(output(),     truelepton_PDG,       "");
  AddVarF(output(),     truelepton_mom,       "");
  AddVar3VF(output(),   truelepton_dir,       "");
  AddVarF(output(),     truelepton_costheta,  "");
  AddVar4VF(output(),   truelepton_pos,       "");
  AddVarI(output(),     truelepton_det,       "");

}

//********************************************************************
void nueCCAnalysis::FillMicroTrees(bool addBase){
  //********************************************************************


  if (addBase) baseAnalysis::FillMicroTreesBase(addBase);
  if ( !cc4pibox().MainTrack ) return;

  EventBoxB* EventBox = (*_event).EventBoxes[EventBoxId::kEventBoxNDUP];
  (void)EventBox;

  
  if (cc4pibox().MainTrack->TrueObject) {
    AnaTrueVertex *vtx = static_cast<AnaTrueVertex*>(cc4pibox().MainTrack->GetTrueParticle()->TrueVertex);
    if(vtx) {
      output().FillVar(true_Nu_pdg,                   vtx->NuPDG);
      output().FillVar(true_Nu_mom,                   vtx->NuMom);
      output().FillVar(truelepton_PDG,                vtx->LeptonPDG);
      output().FillVar(truelepton_mom,                vtx->LeptonMom);
      output().FillVectorVarFromArray(truelepton_dir, vtx->LeptonDir,3);

      double costheta_mu_nu = cos(anaUtils::ArrayToTVector3(vtx->LeptonDir).Angle(anaUtils::ArrayToTVector3(vtx->NuDir)));

      output().FillVar(truelepton_costheta,           (Float_t)costheta_mu_nu);
      output().FillVectorVarFromArray(truelepton_pos, vtx->Position,4);
      output().FillVar(truelepton_det,                anaUtils::GetDetector(vtx->Position));

      AnaTrueParticleB* ele = NULL;
      for (UInt_t i = 0; i < vtx->TrueParticlesVect.size(); ++i) {
        if (vtx->TrueParticlesVect[i]->ParentID == 0 && abs(vtx->TrueParticlesVect[i]->PDG) == 11 && abs(vtx->TrueParticlesVect[i]->TrueVertex->NuPDG) == 12 ) {
          ele = vtx->TrueParticlesVect[i];
          break;
        }
      }

      if (ele) {
        bool ele_iso_tar = (anaUtils::InDetVolume(SubDetId::kTarget, ele->Position) && anaUtils::InDetVolume(SubDetId::kTarget, ele->PositionEnd));

        output().FillVar(selelec_true_isoTar, ele_iso_tar);
      }

    }
    
    output().FillVar(selelec_ID,                        cc4pibox().MainTrack->GetTrueParticle()->ID);
    output().FillVar(selelec_PDG,                       cc4pibox().MainTrack->GetTrueParticle()->PDG);
    output().FillVar(selelec_truemom,                   cc4pibox().MainTrack->GetTrueParticle()->Momentum);
    output().FillVar(selelec_truecostheta,              cc4pibox().MainTrack->GetTrueParticle()->CosTheta);
    output().FillVar(selelec_stopped,                   cc4pibox().MainTrack->GetTrueParticle()->stopped);
    output().FillVar(selelec_flipped,                   cc4pibox().MainTrack->HasFlipped);
    
    output().FillVar(true_target_ele_length,               cc4pibox().EleTargetLength);
    output().FillVar(true_target_pi_length,                cc4pibox().PiTargetLength);
    output().FillVar(true_target_mu_length,                cc4pibox().MuTargetLength);
    output().FillVar(true_target_pr_length,                cc4pibox().PrTargetLength);
    
    output().FillVectorVarFromArray(selelec_truedir,    cc4pibox().MainTrack->GetTrueParticle()->Direction, 3);
    output().FillVectorVarFromArray(selelec_truepos,    cc4pibox().MainTrack->GetTrueParticle()->Position, 4);
    output().FillVectorVarFromArray(selelec_trueendpos, cc4pibox().MainTrack->GetTrueParticle()->PositionEnd, 4);
 
    AnaTrueParticle* parent = static_cast<AnaTrueParticle*> ( anaUtils::GetTrueParticleByID(static_cast<const AnaEventB&> (GetEvent()), cc4pibox().MainTrack->GetTrueParticle()->ParentID) );
    if(parent){
      output().FillVar(selelec_ParentPDG,               parent->PDG);
      AnaTrueVertex *vtx = static_cast<AnaTrueVertex*>(cc4pibox().MainTrack->GetTrueParticle()->TrueVertex);
      if(vtx) {
	std::vector<AnaTrueParticleB*> TrueParticles = vtx->TrueParticlesVect;
	for (unsigned int i = 0; i < TrueParticles.size(); i++) {
	  AnaTrueParticle* ttruth = static_cast<AnaTrueParticle*> (TrueParticles[i]);
	  
	    if(ttruth == cc4pibox().MainTrack->TrueObject) continue;
	    if(ttruth->ParentID == parent->ID){
	      output().FillVar(TruePairTrack_true_mom,      ttruth->Momentum);
	      output().FillVar(TruePairTrack_pdg,           ttruth->PDG);
	      break;
	    }
	}
      }
      
      // Grandparent
      AnaTrueParticle* gparent = static_cast<AnaTrueParticle*> ( anaUtils::GetTrueParticleByID(static_cast<const AnaEventB&> (GetEvent()), parent->ParentID) );
      if(gparent)
	output().FillVar(selelec_GParentPDG,            gparent->PDG);
    }
    
    AnaTrackB* track = cc4pibox().MainTrack;

    //output().FillVar(selelec_detectors,                 track->Detectors);
    output().FillVar(selelec_charge,                    track->Charge);
    output().FillVar(selelec_mom,                       track->SmearedMomentum);
    output().FillVectorVarFromArray(selelec_dir,        track->DirectionStart, 3);
    output().FillVectorVarFromArray(selelec_enddir,     track->DirectionEnd, 3);
    output().FillVectorVarFromArray(selelec_pos,        track->PositionStart, 4);
    output().FillVectorVarFromArray(selelec_endpos,     track->PositionEnd, 4);

    AnaTPCParticleB* longCloseTPC = static_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInDet(*track, anaUtils::GetClosestTPC(*track)));

    if(longCloseTPC) {    
      output().FillVar(selelec_pull_mu,   (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpMuon)/longCloseTPC->dEdxSigmaMuon);
      output().FillVar(selelec_pull_ele,  (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpEle)/longCloseTPC->dEdxSigmaEle);
      output().FillVar(selelec_pull_pi,   (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpPion)/longCloseTPC->dEdxSigmaPion);
      output().FillVar(selelec_pull_pr,   (longCloseTPC->dEdxMeas-longCloseTPC->dEdxexpProton)/longCloseTPC->dEdxSigmaProton);
    }
    
    output().FillVar(selelec_likemu,                    anaUtils::GetPIDLikelihood( *track, 0));
    output().FillVar(selelec_likeel,                    anaUtils::GetPIDLikelihood( *track, 1));
    output().FillVar(selelec_likepi,                    anaUtils::GetPIDLikelihood( *track, 3));
    output().FillVar(selelec_likepr,                    anaUtils::GetPIDLikelihood( *track, 2));
    output().FillVar(selelec_likemip,                   anaUtils::GetPIDLikelihoodMIP( *track));

    output().FillVar(selelec_ecal_stopping,             cutUtils::StoppingECALCut( *track ));
    output().FillVar(selelec_ecal_mipem,                cc4pibox().track_ECal_MipEM);
    output().FillVar(selelec_ecal_EneOnL,               cc4pibox().track_ECal_EneOnL);
    output().FillVar(selelec_longestTPC,                cc4pibox().TPC_det);

    if (track->nTPCSegments > 0) {
      AnaTPCParticleB *TPCSegment = static_cast<AnaTPCParticleB*>(track->TPCSegments[0]);
      output().FillVar(selelec_dedx_tpc, TPCSegment->dEdxMeas);                 
    }

    // ToF
    output().FillVar(selelec_first_ToF,                track->ToF_firstDet);
    output().FillVar(selelec_second_ToF,               track->ToF_secondDet);
    output().FillVar(Beta_ToF,                         cc4pibox().beta_ToF);
    output().FillVar(Beta_true_ToF,                    cc4pibox().beta_true_ToF);
    output().FillVar(selelec_ToF,                      track->ToF_reco_time);
    output().FillVar(selelec_ToF_true,                 track->ToF_true_time);
    output().FillVar(selelec_ToF_mass,                 cc4pibox().ToF_mass);
    output().FillVar(selelec_ToF_true_mass,            cc4pibox().ToF_true_mass);
    output().FillVar(selelec_ToF_lkl_muon,             cc4pibox().ToF_lkl_muon);
    output().FillVar(selelec_ToF_lkl_pion,             cc4pibox().ToF_lkl_pion);
    output().FillVar(selelec_ToF_lkl_electron,         cc4pibox().ToF_lkl_electron);
    output().FillVar(selelec_ToF_lkl_proton,           cc4pibox().ToF_lkl_proton);
    output().FillVar(selelec_ToF_pull_muon,            cc4pibox().ToF_pull_muon);
    output().FillVar(selelec_ToF_pull_pion,            cc4pibox().ToF_pull_pion);
    output().FillVar(selelec_ToF_pull_electron,        cc4pibox().ToF_pull_electron);
    output().FillVar(selelec_ToF_pull_proton,          cc4pibox().ToF_pull_proton);

    // Fill pair track info
    if ( cc4pibox().TPCVetoTrack ){
      output().FillVar(TPCVeto_Deltaz, cc4pibox().TPCVetoTrack->PositionStart[2] - track->PositionStart[2]);
    }

    if ( cc4pibox().ECalVetoTrack ){
      output().FillVar(ECalVeto_Deltaz, cc4pibox().ECalVetoTrack->PositionStart[2] - track->PositionStart[2]);
    }

    output().FillVar(NNearTracks, cc4pibox().NNearTracks);
    output().FillVar(NFarTracks,  cc4pibox().NFarTracks);

    // Fill pair track info
    if ( cc4pibox().PairTrack ){
      AnaTrackB* pairtrack = cc4pibox().PairTrack;

      output().FillVar(PairTrack_mom, pairtrack->SmearedMomentum);
      output().FillVar(PairTrack_costheta, pairtrack->DirectionStart[2]);
      output().FillVar(PairTrack_charge, pairtrack->Charge);
      output().FillVar(PairTrack_InvMass, cc4pibox().InvariantMass);
      output().FillVectorVarFromArray(PairTrack_startdir, pairtrack->DirectionStart, 4);
      output().FillVectorVarFromArray(PairTrack_startpos, pairtrack->PositionStart, 4);

      // Distance of the pair track to main track
      TVector3 pairposition  = anaUtils::ArrayToTVector3(pairtrack->PositionStart);
      TVector3 trackposition = anaUtils::ArrayToTVector3(track->PositionStart);
      Float_t tempdist = (trackposition-pairposition).Mag();
      output().FillVar(PairTrack_DistToMainTrack, tempdist);

      output().FillVar(PairTrack_likemuon, anaUtils::GetPIDLikelihood( *pairtrack, 0));
      output().FillVar(PairTrack_likeelec, anaUtils::GetPIDLikelihood( *pairtrack, 1));
      output().FillVar(PairTrack_likepion, anaUtils::GetPIDLikelihood( *pairtrack, 2));
      output().FillVar(PairTrack_likeprot, anaUtils::GetPIDLikelihood( *pairtrack, 3));

      // Direction of the gamma
      Float_t gamma_dir[3];
      for(int i = 0; i < 3; i++){
	gamma_dir[i] = track->DirectionStart[i] + pairtrack->DirectionStart[i];
      }
      output().FillVectorVarFromArray(PairTrack_gamma_dir, gamma_dir, 3);

      // pair track truth
      AnaTrueParticle* pair_truth = static_cast<AnaTrueParticle*> (pairtrack->TrueObject);

      if(pair_truth){
	output().FillVectorVarFromArray(PairTrack_true_startdir, pair_truth->Direction, 4);
	output().FillVectorVarFromArray(PairTrack_true_startpos, pair_truth->Position, 4);
	output().FillVar(PairTrack_pdg, pair_truth->PDG);
	if (cc4pibox().MainTrack->GetTrueParticle()){
	  for(int i = 0; i < 3; i++){
	    gamma_dir[i] = cc4pibox().MainTrack->GetTrueParticle()->Direction[i] + pair_truth->Direction[i];
	  }
	  output().FillVectorVarFromArray(PairTrack_gamma_true_dir, gamma_dir, 3);
	}
      }

    }

  }

}

//********************************************************************
void nueCCAnalysis::FillToyVarsInMicroTrees(bool addBase){
  //********************************************************************


  if (addBase) baseAnalysis::FillToyVarsInMicroTreesBase(addBase);
 
  if ( cc4pibox().MainTrack ){
    output().FillToyVar(NTPC,    (int)cc4pibox().TPCTracks.size() );
    output().FillToyVar(NECal,   (int)cc4pibox().ECalTracks.size() );

    AnaTrack* track = static_cast<AnaTrack*>( cc4pibox().MainTrack );

    TVector3 nuDirVec = anaUtils::GetNuDirRec(track->PositionStart);
    TVector3 muDirVec = anaUtils::ArrayToTVector3(track->DirectionStart);
    double costheta_mu_nu = nuDirVec.Dot(muDirVec);
    output().FillToyVar(selelec_costheta,      (Float_t)costheta_mu_nu);
  }
  
}

//********************************************************************
bool nueCCAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx) {
  //********************************************************************

  bool TrueNuECC = abs(vtx.ReacCode) > 0 && abs(vtx.ReacCode) < 30 && abs(vtx.NuPDG) == 12;
  bool TrueVtxFV = ( (_useTarget1 && anaUtils::InFiducialVolume( SubDetId::kTarget1, vtx.Position)) ||
  		     (_useTarget2 && anaUtils::InFiducialVolume( SubDetId::kTarget2, vtx.Position)) ||
  		     (_useFGD1    && anaUtils::InFiducialVolume( SubDetId::kFGD1,    vtx.Position)) ||
  		     (_useFGD2    && anaUtils::InFiducialVolume( SubDetId::kFGD2,    vtx.Position)) );
  
  return (TrueNuECC && TrueVtxFV) ;

}

//********************************************************************
void nueCCAnalysis::FillTruthTree(const AnaTrueVertex& vtx) {
  //********************************************************************

  bool IsAntinu = false;
  baseAnalysis::FillTruthTreeBase(vtx, SubDetId::kTarget, IsAntinu);
  int Detector_tmp;
  convUtils::ConvertBitFieldToTrueParticleDetEnum(vtx.Detector, Detector_tmp);

  std::vector<AnaTrueParticleB*> TrueParticles = vtx.TrueParticlesVect;
  output().FillVar(true_Nu_pdg, vtx.NuPDG);
  output().FillVar(true_Nu_mom, vtx.NuMom);
  output().FillVar(true_Target_pdg, vtx.TargetPDG);
  //output().FillVar(true_det, anaUtils::GetDetector(vtx.Position));
  output().FillVar(true_det, Detector_tmp);

  output().FillVar(true_reaction_code, vtx.ReacCode);
  output().FillVectorVarFromArray(true_vertex_position, vtx.Position, 4);
  AnaTrueParticleB* trueTrack=NULL;
  for (unsigned int i = 0; i < TrueParticles.size(); i++) {
    if (abs(TrueParticles[i]->PDG) == 11 && TrueParticles[i]->ParentID == 0) {
      trueTrack = dynamic_cast<AnaTrueParticleB*>(TrueParticles[i]);
      break;
    }
  }

  bool ele_iso_tar = false;
  if (trueTrack)
    ele_iso_tar = (anaUtils::InDetVolume(SubDetId::kTarget, trueTrack->Position) && anaUtils::InDetVolume(SubDetId::kTarget, trueTrack->PositionEnd));

  output().FillVar(selelec_true_isoTar, ele_iso_tar);


  output().FillVar(true_Nu_pdg,                   vtx.NuPDG);
  output().FillVar(true_Nu_mom,                   vtx.NuMom);
  output().FillVar(truelepton_PDG,                vtx.LeptonPDG);
  output().FillVar(truelepton_mom,                vtx.LeptonMom);
  output().FillVectorVarFromArray(truelepton_dir, vtx.LeptonDir,3);
  double costheta_mu_nu = cos(anaUtils::ArrayToTVector3(vtx.LeptonDir).Angle(anaUtils::ArrayToTVector3(vtx.NuDir)));
  output().FillVar(truelepton_costheta, (Float_t)costheta_mu_nu);
  output().FillVectorVarFromArray(truelepton_pos, vtx.Position,4);
  output().FillVar(truelepton_det, anaUtils::GetDetector(vtx.Position));

  if (trueTrack) {
    output().FillVar(true_parentID, trueTrack->ParentID);
    output().FillVar(truelepton_PDG, trueTrack->PDG);
    output().FillVar(true_mom, trueTrack->Momentum);
    output().FillVar(true_SDlength, trueTrack->Length);
    output().FillVar(true_costheta, trueTrack->CosTheta);
    output().FillVar(true_ekin, trueTrack->EKin);
    output().FillVar(true_charge, trueTrack->Charge);
    output().FillVectorVarFromArray(true_position, trueTrack->Position, 4);
    output().FillVectorVarFromArray(true_end, trueTrack->PositionEnd, 4);

    for (unsigned int i=0; i<trueTrack->DetCrossingsVect.size(); i++) {
      int tpc = SubDetId::GetTPC(trueTrack->DetCrossingsVect[i]->Detector);
      if (tpc > 0) { output().FillVar(true_TPC, tpc); break; }
    }

    float phi = atan2(trueTrack->Direction[1], trueTrack->Direction[0]);

    output().FillVar(true_phi, phi);

    TLorentzVector P_nu(0,0,0,0), P_mu(0,0,0,0);
    P_nu.SetVectM(vtx.NuMom*anaUtils::ArrayToTVector3(vtx.NuDir), 0);
    P_mu.SetVectM(trueTrack->Momentum*anaUtils::ArrayToTVector3(trueTrack->Direction), 105.66);

    float Q2 = -(P_mu-P_nu).Mag2()/1e6; // in GeV^2/c^2
    output().FillVar(true_Q2, Q2);

  }

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
void nueCCAnalysis::FillCategories(){
  //********************************************************************

  AnaTrack* track = static_cast<AnaTrack*>( cc4pibox().MainTrack );
  if(!track){return;}

  // fill the categories properly for the target & FGD joint analysis
  // so is the selected track came from FGD2, but it was originated by the gamma from FGD1 is will be tagged as "gamma OOFV"  
  
  if ( _useTarget1 && anaUtils::InFiducialVolume(SubDetId::kTarget1, track->PositionStart)) {
    anaUtils::FillCategories(&GetEvent(), track, "", SubDetId::kTarget1);
    anaUtils::_categ->SetCode("nueCC",      nueCCUtils::nueCCCategory(&GetEvent(), track, SubDetId::kTarget1, _AntiNu));
  }

  if ( _useTarget2 && anaUtils::InFiducialVolume(SubDetId::kTarget2, track->PositionStart)) {
    anaUtils::FillCategories(&GetEvent(), track, "", SubDetId::kTarget2);
    anaUtils::_categ->SetCode("nueCC",      nueCCUtils::nueCCCategory(&GetEvent(), track, SubDetId::kTarget2, _AntiNu));
  }

  if ( _useFGD1 && anaUtils::InFiducialVolume(SubDetId::kFGD1, track->PositionStart)) {
    anaUtils::FillCategories(&GetEvent(), track, "", SubDetId::kFGD1);
    anaUtils::_categ->SetCode("nueCC",      nueCCUtils::nueCCCategory(&GetEvent(), track, SubDetId::kFGD1, _AntiNu));
  }

  if ( _useFGD2 && anaUtils::InFiducialVolume(SubDetId::kFGD2, track->PositionStart)) {
    anaUtils::FillCategories(&GetEvent(), track, "", SubDetId::kFGD2);
    anaUtils::_categ->SetCode("nueCC",      nueCCUtils::nueCCCategory(&GetEvent(), track, SubDetId::kFGD2, _AntiNu));
  }
}
