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

  return true;

}

//********************************************************************
void numuCC4piAnalysis::DefineSelections(){
  //********************************************************************

  //	bool forceBreak = (bool)ND::params().GetParameterI("numuCC4piAnalysis.ForceBreak");
  //bool forceBreak=0;
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
  sel().AddSelection("kTrackerNumuCC4pi", "inclusive numuCC4pi selection", new numuCC4piSimpleSelection()); //true/false for forcing break

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
  /*
  //--- toy variables
  AddToyVarI(output(),  NLowAngle,          "");
  AddToyVarI(output(),  NHighAngle,         "");
  AddToyVarI(output(),  NFwdMuLike,         "");
  AddToyVarI(output(),  NBwdMuLike,         "");
  AddToyVarI(output(),  NHAFwdMuLike,       "");
  AddToyVarI(output(),  NHABwdMuLike,       "");
  AddToyVarI(output(),  Toy_CC4pi,          "");
  AddToyVarI(output(),  selmu_HMNT,         "");
  AddToyVarF(output(),  selmu_mom,          "");
  AddToyVarF(output(),  selmu_MomRangeMuon, "");
  AddToyVarF(output(),  selmu_costheta,     "");
  AddToyVarF(output(),  selmu_ToF_PoD,      "");
  AddToyVarF(output(),  selmu_ToF_ECAL,     "");
  AddToyVarF(output(),  selmu_ToF_FGD2,     "");
  AddToyVarF(output(),  selmu_likemu,       ""); 
  AddToyVarF(output(),  selmu_likemip,      ""); 
  AddToyVarI(output(),  selmu_end_ecal,     "");
  */
  //--- info from true vertex
  AddVarI(output(),   truelepton_PDG,      "");
  AddVarF(output(),   truelepton_mom,      "");
  AddVar3VF(output(), truelepton_dir,      "");
  AddVarF(output(),   truelepton_costheta, "");
  AddVar4VF(output(), truelepton_pos,      "");

  //--- info from true track
  AddVarI(output(),   selmu_ID,           "");
  AddVarI(output(),   selmu_PDG,          "");
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

  //--- ECal PID
  AddVarF(output(),   selmu_ecal_mipem,     "");
  AddVarF(output(),   selmu_ecal_EneOnL,    "");
}

//********************************************************************
void numuCC4piAnalysis::DefineTruthTree(){
  //********************************************************************

  baseAnalysis::DefineTruthTree();

  AddVarI(output(), true_Nu_pdg,         "nu pdg");
  AddVarF(output(), true_Nu_mom,         "nu momentum");
  AddVarI(output(), true_reaction_code,         "reaction_code");
  AddVar4VF(output(), true_vertex_position,         "vertex position");

  AddVarI(output(), true_parentID,         "  true_parentID");                  

  AddVarI(output(), true_pdg,         "  pdg");
  AddVarF(output(), true_SDlength,  " SD length  ");
  AddVarF(output(), true_Edep,  " Edeposite  ");
  AddVarF(output(), true_mom,         " mom");
  AddVarF(output(), true_costheta,         " costheta");
  AddVarF(output(), true_phi,         " phi");
  AddVarF(output(), true_charge,         " charge");
  AddVarF(output(), true_ekin,         " kinetic energy");

}

//********************************************************************
void numuCC4piAnalysis::FillMicroTrees(bool addBase){
  //********************************************************************

  if (addBase) baseAnalysis::FillMicroTreesBase(addBase);
  if ( cc4pibox().MainTrack ){

    EventBoxB* EventBox = (*_event).EventBoxes[EventBoxId::kEventBoxNDUP];

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

    AnaTrack* track = static_cast<AnaTrack*>( cc4pibox().MainTrack );

    //		output().FillVar(selmu_detectors,                 track->Detectors);
    output().FillVar(selmu_charge,                    track->Charge);
    output().FillVectorVarFromArray(selmu_dir,        track->DirectionStart, 3);
    output().FillVectorVarFromArray(selmu_enddir,     track->DirectionEnd, 3);
    output().FillVectorVarFromArray(selmu_pos,        track->PositionStart, 4);
    output().FillVectorVarFromArray(selmu_endpos,     track->PositionEnd, 4);

    output().FillVar(selmu_ecal_mipem,                cc4pibox().track_ECal_MipEM);
    output().FillVar(selmu_ecal_EneOnL,                    cc4pibox().track_ECal_EneOnL);
    
  }

}

//********************************************************************
void numuCC4piAnalysis::FillToyVarsInMicroTrees(bool addBase){
  //********************************************************************


  if (addBase) baseAnalysis::FillToyVarsInMicroTreesBase(addBase);
  /*
    if ( cc4pibox().MainTrack ){
    output().FillToyVar(NLowAngle,    (int)cc4pibox().LowAngle.size() );
    output().FillToyVar(NHighAngle,   (int)cc4pibox().HighAngle.size() );
    output().FillToyVar(NFwdMuLike,   (int)cc4pibox().FwdTracks.size() );
    output().FillToyVar(NBwdMuLike,   (int)cc4pibox().BwdTracks.size() );
    output().FillToyVar(NHAFwdMuLike, (int)cc4pibox().HAFwdTracks.size() );
    output().FillToyVar(NHABwdMuLike, (int)cc4pibox().HABwdTracks.size() );

    AnaTrack* track = static_cast<AnaTrack*>( cc4pibox().MainTrack );
    output().FillToyVar(Toy_CC4pi,           numuCC4pi_utils::CC4piCategory(track));
    output().FillToyVar(selmu_HMNT,          numuCC4pi_utils::isHMNT(cc4pibox().MainTrack,cc4pibox().LowAngle,true));
    output().FillToyVar(selmu_mom,           track->Momentum);
    output().FillToyVar(selmu_MomRangeMuon,  track->RangeMomentumMuon);
    TVector3 nuDirVec = anaUtils::GetNuDirRec(track->PositionStart);
    TVector3 muDirVec = anaUtils::ArrayToTVector3(track->DirectionStart);
    double costheta_mu_nu = nuDirVec.Dot(muDirVec);
    output().FillToyVar(selmu_costheta, (Float_t)costheta_mu_nu);
    //		output().FillToyVar(selmu_ToF_PoD,       track->ToF.P0D_FGD1);
    output().FillToyVar(selmu_ToF_ECAL,      track->ToF.ECal_FGD1);
    output().FillToyVar(selmu_ToF_FGD2,      track->ToF.FGD1_FGD2);
    output().FillToyVar(selmu_likemu,        anaUtils::GetPIDLikelihood( *(cc4pibox().MainTrack),0));
    output().FillToyVar(selmu_likemip,       anaUtils::GetPIDLikelihoodMIP( *(cc4pibox().MainTrack)));
    output().FillToyVar(selmu_end_ecal,      cutUtils::StoppingBrECALorSMRDCut(track->PositionEnd));

    }
  */

}

//********************************************************************
bool numuCC4piAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx) {
  //********************************************************************
  int branch = ND::params().GetParameterI("numuCC4piAnalysis.Branch");

  bool TrueNuMuCC = vtx.ReacCode > 0 && vtx.ReacCode < 30 && vtx.NuPDG == 14;
  bool TrueVtxFV = anaUtils::InFiducialVolume( SubDetId::kTarget1, vtx.Position );
  /*std::vector<AnaTrueParticleB*> TrueParticles = vtx.TrueParticlesVect;

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
    }*/
  return TrueNuMuCC && TrueVtxFV ;

  return false;
}

//********************************************************************
void numuCC4piAnalysis::FillTruthTree(const AnaTrueVertex& vtx) {
  //********************************************************************

  // workaround to use the same code for the antuNumu package
  // calling numuCCAnalysis::FillTruthTreeBase(vtx,true)
  bool IsAntinu = false;
  std::vector<AnaTrueParticleB*> TrueParticles = vtx.TrueParticlesVect;
  output().FillVar(true_Nu_pdg, vtx.NuPDG);
  output().FillVar(true_Nu_mom, vtx.NuMom);

  output().FillVar(true_reaction_code, vtx.ReacCode);
  output().FillVectorVarFromArray(true_vertex_position, vtx.Position, 4);
  AnaTrueParticleB* trueTrack=NULL;
  for (Int_t i = 0; i < TrueParticles.size(); i++) {
    if (TrueParticles[i]->PDG == 13) {
      trueTrack = dynamic_cast<AnaTrueParticleB*>(TrueParticles[i]);
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

    float phi = atan2(trueTrack->Direction[1], trueTrack->Direction[0]);
    float cosphi = cos(phi);

    output().FillVar(true_phi, phi);


  }
  return FillTruthTreeBase(vtx, IsAntinu);
}


//********************************************************************
void numuCC4piAnalysis::FillTruthTreeBase(const AnaTrueVertex& vtx, bool IsAntinu) {
  //********************************************************************

  // this method is also called from the antiNumu package with IsAntinu = true

  // Fill the common variables
  baseAnalysis::FillTruthTreeBase(vtx, SubDetId::kTPCUp1, IsAntinu);
  // Muon true-rec association.
  //  if (box().MainTrack)
  //    if (box().MainTrack->TrueObject)
  //      output().FillVar("mu_true_rec", (box().MainTrack->GetTrueParticle()->PDG==13));   // Is the recon muon a true muon ?

  // Fill truth categories for FGD2, same categories with prefix "fgd2", i,e, "fgd2reaction" etc.
  // We could directly set which FGD in the usual categories, but this is not possible for the true vertices
  //  anaUtils::FillCategories(&vtx, "fgd2", _detector, IsAntinu,GetSpill().GetIsSandMC());

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
  //anaUtils::FillCategories(&GetEvent(), track, "", SubDetId::kTarget1);
  //	anaUtils::_categ->SetCode("CC4pi",      numuCC4pi_utils::CC4piCategory(track) );
  //	anaUtils::_categ->SetCode("OOFV_Fwd",   numuCC4pi_utils::OOFVCategory(track,0) );
  //	anaUtils::_categ->SetCode("OOFV_Bwd",   numuCC4pi_utils::OOFVCategory(track,1) );

}
