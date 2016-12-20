#include "numuCCAnalysis.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "CategoriesUtils.hxx"
#include "BasicUtils.hxx"
#include "baseSelection.hxx"
#include "numuCCSelection.hxx"
#include "TreeConverterUtils.hxx"
#include "CutUtils.hxx"
#include "MomRangeCorrection.hxx"
#include "TruthUtils.hxx"
#include "PIDCorrection.hxx"
#include "PIDUtils.hxx"
//********************************************************************
numuCCAnalysis::numuCCAnalysis(AnalysisAlgorithm* ana) : baseAnalysis(ana) {
//********************************************************************

  // Add the package version
  ND::versioning().AddPackage("numuCCAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("NUMUCCANALYSISROOT")));
}

//********************************************************************
bool numuCCAnalysis::Initialize() {
//********************************************************************

  // Initialize the baseAnalysis
  if (!baseAnalysis::Initialize()) return false;

  // Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("numuCCAnalysis.MinAccumLevelToSave"));

  // Define categories
  anaUtils::AddStandardCategories();

  _detector = SubDetId::kTPCUp1;

  return true;
}

//********************************************************************
void numuCCAnalysis::DefineSelections() {
//********************************************************************

  sel().AddSelection("numuCC",    "inclusive numu selection",     new numuCCSelection());     // true/false for forcing break

}

//********************************************************************
void numuCCAnalysis::DefineCorrections() {
//********************************************************************

  // Some corrections are defined in baseAnalysis
  baseAnalysis::DefineCorrections();
  corr().AddCorrection("mom_corr",          new MomRangeCorrection(SubDetId::kTPC));

}

//********************************************************************
void numuCCAnalysis::DefineSystematics() {
//********************************************************************

  // Some corrections are defined in baseAnalysis
  baseAnalysis::DefineSystematics();


//  eweight().AddEventWeight(SystId::kDetectorMass,           "DetectorMass",           new DetectorMassWeight());

//  evar().AddEventVariation(SystId::kdEdx,           "dEdx",           new dEdxVariation());
}

//********************************************************************
void numuCCAnalysis::DefineConfigurations() {
//********************************************************************

  // Some corrections are defined in baseAnalysis
  baseAnalysis::DefineConfigurations();

}

//********************************************************************
void numuCCAnalysis::DefineMicroTrees(bool addBase) {
//********************************************************************

  std::cout << "-------- Add variables to the analysis tree ----------------------" << std::endl;

  // Variables from baseAnalysis (run, event, ...)
  if (addBase) baseAnalysis::DefineMicroTrees(addBase);
  AddVarI(output(), Nu_pdg,         "nu pdg");
  AddVarF(output(), Nu_mom,         "nu momentum");
  AddVarI(output(), reaction_code,         "reaction_code");
  AddVarI(output(), current_code,         "current_code");

  AddVar4VF(output(), vertex_position,         "vertex position");

  AddVarI(output(), muon_pdg,         "muon pdg");
  AddVarF(output(), muon_SDLength,         "muon SD Length");
  AddVarF(output(), muon_mom,         "muon momentum");
  AddVarF(output(), muon_costheta,         "muon costheta");
  AddVarF(output(), muon_cosphi,         "muon cosphi");
  AddVarF(output(), muon_theta,         "muon theta");
  AddVarF(output(), muon_phi,         "muon phi");
  AddVarF(output(), muon_pidlikelihood1,         "muon_pidlikelihood1");
  AddVarF(output(), muon_pidlikelihood2,         "muon_pidlikelihood2");
  AddVarF(output(), muon_pidlikelihood3,         "muon_pidlikelihood3");
  AddVarF(output(), muon_pidlikelihood4,         "muon_pidlikelihood4");
 AddVarF(output(), mip_pidlikelihood,         "mip_pidlikelihood");

  AddVarF(output(), muon_charge,         "muon charge");
  AddVarF(output(), muon_ekin,         "muon kinetic energy");
  AddVarI(output(), muon_istpccut,         "muon is length cut in tpc");

  AddVarI(output(), muon_istarget1,         "muon is length cut in target1");
  AddVarI(output(), muon_istarget2,         "muon is length cut in target2");

  AddVarMF(output(), muon_tpc_entrancepos,    "muon entrance position in tpc", tpc_muon, -300, 4);

  AddVarMF(output(), muon_tpc_exitpos,    "muon exit position in tpc", tpc_muon, -300, 4);
  AddVarVI(output(), muon_tpc_det,         "muon tpc det id", tpc_muon);
  AddVarVF(output(), muon_LYZTPC,         "muon LYTPC", tpc_muon);
 
  AddVarVF(output(), muon_tpc_length,         "muon tpc segmentLength", tpc_muon);
  AddVarVF(output(), muon_tpc_EDep,         "muon tpc EDeposit", tpc_muon);
  AddVarVF(output(), muon_tpc_smeared_momentum,         "muon tpc smeared momentum", tpc_muon);
  AddVarVF(output(), muon_tpc_momentum_error,         "muon tpc smeared momentum error", tpc_muon);
  AddVarVF(output(), tpc_pull_muon,         "muon tpc pull ", tpc_muon);
  AddVarVF(output(), muon_tpc_momentum,         "muon tpc true momentum", tpc_muon);

  AddVarF(output(), muon_EDep_Target1,         "muon EDep in Target1");
  AddVarF(output(), muon_EDep_Target2,         "muon EDep in Target2");
  AddVarF(output(), muon_LTarget1,         "muon LTarget1");
  AddVarF(output(), muon_LTarget2,         "muon LTarget2");
  AddVarI(output(), muon_isLastinTarget1,         " is the muon track end postion in target1");
  AddVarI(output(), muon_isLastinTarget2,         "is the muon track end postion in target2");

  AddVar4VF(output(), muon_target1_entrancepos,         "muon target1 entrance position");
  AddVar4VF(output(), muon_target1_exitpos,         "muon target1 exit position");
  AddVar4VF(output(), muon_target2_entrancepos,         "muon target2 entrance position");
  AddVar4VF(output(), muon_target2_exitpos,         "muon target2 exit position");





}

//********************************************************************
void numuCCAnalysis::DefineTruthTree() {
//********************************************************************

  // Variables from baseAnalysis (run, event, ...)
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
void numuCCAnalysis::FillMicroTrees(bool addBase) {
//********************************************************************

  // WARNING: IF YOU RUN FGD1 AND FGD2 AT ONCE ( "whichFGD" parameter = 3), only for events with accum_level > 5
  // the vars in the output microtree will surely refer to the muon candidate in that FGD

  // Variables from baseAnalysis (run, event, ...)
  if (addBase) baseAnalysis::FillMicroTreesBase(addBase);
  if (box().Vertex) {
    output().FillVar(Nu_pdg, (box().Vertex->TrueVertex)->NuPDG);
    AnaTrueVertex *vtx = dynamic_cast<AnaTrueVertex*>(box().Vertex->TrueVertex);
    output().FillVar(Nu_mom, (box().Vertex->TrueVertex)->NuMom);

    output().FillVar(reaction_code, vtx->ReacCode);
    output().FillVar(current_code, anaUtils::GetReacAll(vtx->ReacCode));
    output().FillVectorVarFromArray(vertex_position, (box().Vertex)->Position, 4);
  }
// Muon information
  if (box().MainTrack) {
    AnaTrueParticle *trueP = dynamic_cast<AnaTrueParticle*>(box().MainTrack->GetTrueParticle());
    output().FillVar(muon_pdg, trueP->PDG);
    output().FillVar(muon_charge, trueP->Charge);
    output().FillVar(muon_mom, box().MainTrack->SmearedMomentum);
    output().FillVar(muon_costheta, (box().MainTrack)->DirectionStart[2]);
    output().FillVar(muon_theta, (float)acos((box().MainTrack)->DirectionStart[2]));
    output().FillVar(muon_istpccut, cutUtils::DeltaLYZTPCCut(*box().MainTrack));
    output().FillVar(muon_istarget1, (int)cutUtils::DeltaLYZTargetCut(*box().MainTrack, SubDetId::kTarget1));
    output().FillVar(muon_istarget2, (int)cutUtils::DeltaLYZTargetCut(*box().MainTrack, SubDetId::kTarget2));


    float phi = atan2((box().MainTrack)->DirectionStart[1], (box().MainTrack)->DirectionStart[0]);
    float cosphi = cos(phi);

    output().FillVar(muon_cosphi, cosphi);
    output().FillVar(muon_phi, phi);

    AnaTrack *track = dynamic_cast<AnaTrack*>(box().MainTrack);
    Float_t PIDLikelihood[4];
    anaUtils::GetPIDLikelihood(*box().MainTrack, PIDLikelihood, 0);
    output().FillVar (mip_pidlikelihood, (PIDLikelihood[0] + PIDLikelihood[3]) / (1 - PIDLikelihood[2]));
    output().FillVar(muon_pidlikelihood1, PIDLikelihood[0]);
    output().FillVar(muon_pidlikelihood2, PIDLikelihood[1]);
    output().FillVar(muon_pidlikelihood3, PIDLikelihood[2]);
    output().FillVar(muon_pidlikelihood4, PIDLikelihood[3]);

    output().FillVar(muon_ekin, (box().MainTrack)->EKin);
    output().FillVar(muon_SDLength, track->Length);
    Float_t pulls[4];
    for (int subdet = 0; subdet < 7; subdet++) {
      AnaTPCParticleB* TPCSegment = dynamic_cast<AnaTPCParticleB*>(anaUtils::GetSegmentInDet( *box().MainTrack, static_cast<SubDetId::SubDetEnum >(subdet)));
      if (TPCSegment) {
        output().FillVectorVar(muon_tpc_det, subdet);
        output().FillVectorVar(muon_LYZTPC, TPCSegment->DeltaLYZ);
        output().FillVectorVar(muon_tpc_EDep, TPCSegment->EDeposit);
        output().FillVectorVar(muon_tpc_length, TPCSegment->SegLength);
        output().FillVectorVar(muon_tpc_smeared_momentum, TPCSegment->SmearedMomentum);
        output().FillVectorVar(muon_tpc_momentum, TPCSegment->Momentum);

        output().FillVectorVar(muon_tpc_momentum_error, TPCSegment->MomentumError);
        //  anaUtils::ComputeTPCPull(*TPCSegment, *box().HMT);

        anaUtils::ComputeTPCPull(*TPCSegment, pulls);
        output().FillVectorVar(tpc_pull_muon, pulls[0]);

        output().FillMatrixVarFromArray(muon_tpc_entrancepos,    TPCSegment->PositionStart, 4);
        output().FillMatrixVarFromArray(muon_tpc_exitpos,    TPCSegment->PositionEnd, 4);
        output().IncrementCounterForVar(muon_tpc_det);

      }

    }

    AnaTargetParticleB* TargetSegment1 = dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet( *box().MainTrack, SubDetId::kTarget1));
    if (TargetSegment1) {
      output().FillVar(muon_LTarget1, TargetSegment1->DeltaLYZ);
      output().FillVar(muon_EDep_Target1, TargetSegment1->EDeposit);
      output().FillVectorVarFromArray(muon_target1_entrancepos,    TargetSegment1->PositionStart, 4);
      output().FillVectorVarFromArray(muon_target1_exitpos,    TargetSegment1->PositionEnd, 4);
      output().FillVar(muon_isLastinTarget1, (int)cutUtils::FiducialCut((box().MainTrack)->PositionEnd, SubDetId::kTarget1) );


    }
    AnaTargetParticleB* TargetSegment2 = dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet( *box().MainTrack, SubDetId::kTarget2));
    if (TargetSegment2) {
      output().FillVar(muon_LTarget2, TargetSegment2->DeltaLYZ);
      output().FillVar(muon_EDep_Target2, TargetSegment2->EDeposit);
      output().FillVectorVarFromArray(muon_target2_entrancepos,    TargetSegment2->PositionStart, 4);
      output().FillVectorVarFromArray(muon_target2_exitpos,    TargetSegment2->PositionEnd, 4);
      output().FillVar(muon_isLastinTarget2, (int)cutUtils::FiducialCut((box().MainTrack)->PositionEnd, SubDetId::kTarget2) );

    }

  }


}

//********************************************************************
void numuCCAnalysis::FillToyVarsInMicroTrees(bool addBase) {
//********************************************************************

  // Fill all tree variables that vary for each toy experiment

  // Fill the common variables
  if (addBase) baseAnalysis::FillToyVarsInMicroTreesBase(addBase);

}

//********************************************************************
bool numuCCAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx) {
//********************************************************************
  // GetReactionCC already takes into account the outFV
  bool numuCCInFV = true; //(anaUtils::GetReactionCC(vtx, fgdID) == 1);
  return (numuCCInFV);
}

//********************************************************************
void numuCCAnalysis::FillTruthTree(const AnaTrueVertex& vtx) {
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
void numuCCAnalysis::FillTruthTreeBase(const AnaTrueVertex& vtx, bool IsAntinu) {
//********************************************************************

  // this method is also called from the antiNumu package with IsAntinu = true

  // Fill the common variables
  baseAnalysis::FillTruthTreeBase(vtx, _detector, IsAntinu);
  // Muon true-rec association.
  //  if (box().MainTrack)
  //    if (box().MainTrack->TrueObject)
  //      output().FillVar("mu_true_rec", (box().MainTrack->GetTrueParticle()->PDG==13));   // Is the recon muon a true muon ?

  // Fill truth categories for FGD2, same categories with prefix "fgd2", i,e, "fgd2reaction" etc.
  // We could directly set which FGD in the usual categories, but this is not possible for the true vertices
  //  anaUtils::FillCategories(&vtx, "fgd2", _detector, IsAntinu,GetSpill().GetIsSandMC());

}

//********************************************************************
void numuCCAnalysis::FillCategories() {
//********************************************************************

  // Fill the track categories for color drawing

  // For the muon candidate
}
