#include "truenumuCCAnalysis.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "CategoriesUtils.hxx"
#include "BasicUtils.hxx"
#include "baseSelection.hxx"
#include "truenumuCCSelection.hxx"
#include "TreeConverterUtils.hxx"
#include "CutUtils.hxx"
#include "MomRangeCorrection.hxx"
#include "TruthUtils.hxx"
#include "PIDCorrection.hxx"
#include "PIDUtils.hxx"
//********************************************************************
truenumuCCAnalysis::truenumuCCAnalysis(AnalysisAlgorithm* ana) : baseAnalysis(ana) {
//********************************************************************

  // Add the package version
//  ND::versioning().AddPackage("ndupAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("NDUPANALYSISROOT")));
}

//********************************************************************
bool truenumuCCAnalysis::Initialize() {
//********************************************************************

  // Initialize the baseAnalysis
  if (!baseAnalysis::Initialize()) return false;

  // Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("ndupAnalysis.MinAccumLevelToSave"));

  // Define categories
  anaUtils::AddStandardCategories();

  _detector = SubDetId::kTarget1;

  return true;
}

//********************************************************************
void truenumuCCAnalysis::DefineSelections() {
//********************************************************************

  //sel().AddSelection("ndup",    "inclusive ndup selection",     new baseSelection());     // true/false for forcing break
  sel().AddSelection("numu",    "inclusive true numu selection",     new truenumuCCSelection());     // true/false for forcing break

}

//********************************************************************
void truenumuCCAnalysis::DefineCorrections() {
//********************************************************************

  // Some corrections are defined in baseAnalysis
  baseAnalysis::DefineCorrections();
  corr().AddCorrection("mom_corr",          new MomRangeCorrection(SubDetId::kTPC));
  corr().AddCorrection("pid_corr",          new PIDCorrection(SubDetId::kTPC));

}

//********************************************************************
void truenumuCCAnalysis::DefineSystematics() {
//********************************************************************

  // Some corrections are defined in baseAnalysis
  baseAnalysis::DefineSystematics();


//  eweight().AddEventWeight(SystId::kDetectorMass,           "DetectorMass",           new DetectorMassWeight());

//  evar().AddEventVariation(SystId::kdEdx,           "dEdx",           new dEdxVariation());
}

//********************************************************************
void truenumuCCAnalysis::DefineConfigurations() {
//********************************************************************

  // Some corrections are defined in baseAnalysis
  baseAnalysis::DefineConfigurations();

}

//********************************************************************
void truenumuCCAnalysis::DefineMicroTrees(bool addBase) {
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
  //AddVarI(output(), muon_istpccut,         "muon is length cut in tpc");

 // AddVarI(output(), muon_istarget1,         "muon is length cut in target1");
 // AddVarI(output(), muon_istarget2,         "muon is length cut in target2");

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




  AddVarI(output(), ppion_pdg,         "pi+ pdg");
  AddVarF(output(), ppion_SDLength,         "pi+ SD Length");
  AddVarF(output(), ppion_mom,         "pi+ mom");
  AddVarF(output(), ppion_costheta,         "pi+ costheta");
  AddVarF(output(), ppion_cosphi,         "pi+ cosphi");
  AddVarF(output(), ppion_theta,         "pi+ theta");
  AddVarF(output(), ppion_phi,         "pi+ phi");
 // AddVarI(output(), ppion_istpccut,         "pi+ is length cut in tpc");

  //AddVarI(output(), ppion_istarget1,         "pi+ is length cut in target1");
 // AddVarI(output(), ppion_istarget2,         "pi+ is length cut in target1");

  AddVarF(output(), ppion_charge,         "pi+ charge");
  AddVarF(output(), ppion_ekin,         "pi+ kinetic energy");

  AddVarMF(output(), ppion_tpc_entrancepos,    "pi+ tpc entrance position", tpc_ppion, -300, 4);
  AddVarVF(output(), tpc_pull_pion,         "pi+ tpc pull ", tpc_ppion);

  AddVarMF(output(), ppion_tpc_exitpos,    "pi+ tpc exit position", tpc_ppion, -300, 4);

  AddVarVI(output(), ppion_tpc_det,         "pi+ tpc det id", tpc_ppion);
  AddVarVF(output(), ppion_LYZTPC,         "pi+ LYZYPC ", tpc_ppion);
  AddVarVF(output(), ppion_tpc_smeared_momentum,         "pi+ tpc smeared momentum", tpc_ppion);
  AddVarVF(output(), ppion_tpc_momentum_error,         "pi+ tpc smeared momentum error", tpc_ppion);
  AddVarVF(output(), ppion_tpc_momentum,         "pi+ tpc true momentum", tpc_ppion);

  AddVarVF(output(), ppion_tpc_length,         "pi+ Length in TPC", tpc_ppion);
  AddVarVF(output(), ppion_tpc_EDep,         " pi+ TPC EDeposit", tpc_ppion);

  AddVarF(output(), ppion_EDep_Target1,         "pi+ EDep in Target1");
  AddVarF(output(), ppion_EDep_Target2,         "pi+ EDep in Target2");
  AddVarF(output(), ppion_LTarget1,         "pi+ Length in Target1");
  AddVarF(output(), ppion_LTarget2,         "pi+ Length in Target2");
  AddVarI(output(), ppion_isLastinTarget1,         "is the pi+ track end postion in target1");
  AddVarI(output(), ppion_isLastinTarget2,         "is the pi+ track end postion in target2");

  AddVar4VF(output(), ppion_target1_entrancepos,         "pi+ target1 entrance position");
  AddVar4VF(output(), ppion_target1_exitpos,         "pi+ target1 exit position");
  AddVar4VF(output(), ppion_target2_entrancepos,         "pi+ target2 entrance position");
  AddVar4VF(output(), ppion_target2_exitpos,         "pi+ target2 exit position");




  AddVarI(output(), mpion_pdg,         "pi- pdg");
  AddVarF(output(), mpion_SDLength,         "pi- SD Length");
  AddVarF(output(), mpion_mom,         "pi- mom");
  AddVarF(output(), mpion_costheta,         "pi- costheta");
  AddVarF(output(), mpion_cosphi,         "pi- cosphi");
  AddVarF(output(), mpion_theta,         "pi- theta");
  AddVarF(output(), mpion_phi,         "pi- phi");
 // AddVarI(output(), mpion_istpccut,         "pi- is length cut in tpc");

 // AddVarI(output(), mpion_istarget1,         "pi- is length cut in target1");
 // AddVarI(output(), mpion_istarget2,         "pi- is length cut in target2");

  AddVarF(output(), mpion_charge,         "pi- charge");
  AddVarF(output(), mpion_ekin,         "pi- kinetic energy");

  AddVarMF(output(), mpion_tpc_entrancepos,    "pi- tpc entrance position", tpc_mpion, -300, 4);

  AddVarMF(output(), mpion_tpc_exitpos,    "pi- tpc exit position", tpc_mpion, -300, 4);
  AddVarVI(output(), mpion_tpc_det,         "pi- tpc det id", tpc_mpion);
  AddVarVF(output(), mpion_LYZTPC,         "pi- LYTPC", tpc_mpion);
  AddVarVF(output(), mpion_tpc_smeared_momentum,         "pi- tpc smeared momentum", tpc_mpion);
  AddVarVF(output(), mpion_tpc_momentum_error,         "pi- tpc smeared momentum error", tpc_mpion);
  AddVarVF(output(), mpion_tpc_momentum,         "pi-  tpc true momentum", tpc_mpion);

  AddVarVF(output(), mpion_tpc_length,         "pi-  Length in TPC", tpc_mpion);
  AddVarVF(output(), mpion_tpc_EDep,         "pi- TPC EDeposit", tpc_mpion);

  AddVarF(output(), mpion_EDep_Target1,         "pi- EDep in Target1");
  AddVarF(output(), mpion_EDep_Target2,         "pi- EDep in Target2");
  AddVarF(output(), mpion_LTarget1,         "pi- Length in Target1");
  AddVarF(output(), mpion_LTarget2,         "pi- Length in Target2");
  AddVarI(output(), mpion_isLastinTarget1,         "is the pi- track end postion in target1");
  AddVarI(output(), mpion_isLastinTarget2,         "is the pi- track end postion in target2");

  AddVar4VF(output(), mpion_target1_entrancepos,         "pi- target1 entrance position");
  AddVar4VF(output(), mpion_target1_exitpos,         "pi- target1 exit position");
  AddVar4VF(output(), mpion_target2_entrancepos,         "pi- target2 entrance position");
  AddVar4VF(output(), mpion_target2_exitpos,         "pi- target2 entrance position");



  AddVarI(output(), proton_pdg,         "proton pdg");
  AddVarF(output(), proton_SDLength,         "proton SD Length");
  AddVarF(output(), proton_mom,         "proton mom");
  AddVarF(output(), proton_costheta,         "proton costheta");
  AddVarF(output(), proton_cosphi,         "proton cosphi");
  AddVarF(output(), proton_theta,         "proton theta");
  AddVarF(output(), proton_phi,         "proton phi");
 // AddVarI(output(), proton_istpccut,         "proton is length cut in tpc");

 /// AddVarI(output(), proton_istarget1,         "proton is length cut in target1");
 // AddVarI(output(), proton_istarget2,         "proton is length cut in target2");

  AddVarF(output(), proton_charge,         "proton charge");
  AddVarF(output(), proton_ekin,         "proton kinetic energy");

  AddVarMF(output(), proton_tpc_entrancepos,    "proton tpc entrance position", tpc_proton, -300, 4);

  AddVarMF(output(), proton_tpc_exitpos,    "proton tpc exit position", tpc_proton, -300, 4);
  AddVarVI(output(), proton_tpc_det,         "proton tpc det id", tpc_proton);
  AddVarVF(output(), proton_LYZTPC,         "proton LYTPC", tpc_proton);
  AddVarVF(output(), tpc_pull_proton,         "proton tpc pull", tpc_proton);
  AddVarVF(output(), proton_tpc_momentum,         "proton tpc smeared momentum", tpc_proton);

  AddVarVF(output(), proton_tpc_smeared_momentum,         "proton tpc smeared momentum error", tpc_proton);
  AddVarVF(output(), proton_tpc_momentum_error,         "proton tpc true momentum", tpc_proton);

  AddVarVF(output(), proton_tpc_length,         "proton Length in TPC", tpc_proton);
  AddVarVF(output(), proton_tpc_EDep,         "proton TPC EDeposit", tpc_proton);

  AddVarF(output(), proton_EDep_Target1,         "proton EDep in Target1");
  AddVarF(output(), proton_EDep_Target2,         "proton EDep in Target2");
  AddVarF(output(), proton_LTarget1,         "proton Length in Target1");
  AddVarF(output(), proton_LTarget2,         "proton Length in Target2");
  AddVarI(output(), proton_isLastinTarget1,         "is the proton track end postion in target1");
  AddVarI(output(), proton_isLastinTarget2,         "is the proton track end postion in target2");

  AddVar4VF(output(), proton_target1_entrancepos,         "proton target1 entrance position");
  AddVar4VF(output(), proton_target1_exitpos,         "proton target1 exit position");
  AddVar4VF(output(), proton_target2_entrancepos,         "proton target2 entrance position");
  AddVar4VF(output(), proton_target2_exitpos,         "proton target2 exit position");


}

//********************************************************************
void truenumuCCAnalysis::DefineTruthTree() {
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
void truenumuCCAnalysis::FillMicroTrees(bool addBase) {
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
  if (box().HMT) {
//    std::cout << "box().HMT t " << std::endl;
    AnaTrueParticle *trueP = dynamic_cast<AnaTrueParticle*>(box().HMT->GetTrueParticle());
    output().FillVar(muon_pdg, trueP->PDG);
    output().FillVar(muon_charge, trueP->Charge);
    output().FillVar(muon_mom, box().HMT->SmearedMomentum);
    output().FillVar(muon_costheta, (box().HMT)->DirectionStart[2]);
    output().FillVar(muon_theta, (float)acos((box().HMT)->DirectionStart[2]));
   // output().FillVar(muon_istpccut, (int)cutUtils::DeltaLYZTPCCut(*box().HMT));
//    output().FillVar(muon_istarget1, (int)cutUtils::DeltaLYZTargetCut(*box().HMT, SubDetId::kTarget1));
  //  output().FillVar(muon_istarget2, (int)cutUtils::DeltaLYZTargetCut(*box().HMT, SubDetId::kTarget2));


    float phi = atan2((box().HMT)->DirectionStart[1], (box().HMT)->DirectionStart[0]);
    float cosphi = cos(phi);

    output().FillVar(muon_cosphi, cosphi);
    output().FillVar(muon_phi, phi);

    AnaTrack *track = dynamic_cast<AnaTrack*>(box().HMT);
    Float_t PIDLikelihood[4];
  //  std::cout << "tpc" << std::endl;
    anaUtils::GetPIDLikelihood(*box().HMT, PIDLikelihood, 0);
    output().FillVar (mip_pidlikelihood, (PIDLikelihood[0] + PIDLikelihood[3]) / (1 - PIDLikelihood[2]));
    output().FillVar(muon_pidlikelihood1, PIDLikelihood[0]);
    output().FillVar(muon_pidlikelihood2, PIDLikelihood[1]);
    output().FillVar(muon_pidlikelihood3, PIDLikelihood[2]);
    output().FillVar(muon_pidlikelihood4, PIDLikelihood[3]);

    output().FillVar(muon_ekin, (box().HMT)->EKin);
    output().FillVar(muon_SDLength, track->Length);
    Float_t pulls[4];
    for (int subdet = 0; subdet < 7; subdet++) {
      AnaTPCParticleB* TPCSegment = dynamic_cast<AnaTPCParticleB*>(anaUtils::GetSegmentInDet( *box().HMT, static_cast<SubDetId::SubDetEnum >(subdet)));
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

    AnaTargetParticleB* TargetSegment1 = dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet( *box().HMT, SubDetId::kTarget1));
    if (TargetSegment1) {
      output().FillVar(muon_LTarget1, TargetSegment1->DeltaLYZ);
      output().FillVar(muon_EDep_Target1, TargetSegment1->EDeposit);
      output().FillVectorVarFromArray(muon_target1_entrancepos,    TargetSegment1->PositionStart, 4);
      output().FillVectorVarFromArray(muon_target1_exitpos,    TargetSegment1->PositionEnd, 4);
      output().FillVar(muon_isLastinTarget1, (int)cutUtils::FiducialCut((box().HMT)->PositionEnd, SubDetId::kTarget1) );


    }
    AnaTargetParticleB* TargetSegment2 = dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet( *box().HMT, SubDetId::kTarget2));
    if (TargetSegment2) {
      output().FillVar(muon_LTarget2, TargetSegment2->DeltaLYZ);
      output().FillVar(muon_EDep_Target2, TargetSegment2->EDeposit);
      output().FillVectorVarFromArray(muon_target2_entrancepos,    TargetSegment2->PositionStart, 4);
      output().FillVectorVarFromArray(muon_target2_exitpos,    TargetSegment2->PositionEnd, 4);
      output().FillVar(muon_isLastinTarget2, (int)cutUtils::FiducialCut((box().HMT)->PositionEnd, SubDetId::kTarget2) );

    }

  }

// Minus Pion information
  if (box().HMinusPionT) {

    AnaTrueParticle *trueP = dynamic_cast<AnaTrueParticle*>(box().HMinusPionT->GetTrueParticle());
    output().FillVar(mpion_pdg, trueP->PDG);
    output().FillVar(mpion_charge, trueP->Charge);
    output().FillVar(mpion_mom, box().HMinusPionT->Momentum);
    output().FillVar(mpion_costheta, (box().HMinusPionT)->DirectionStart[2]);
    output().FillVar(mpion_theta, (float)acos((box().HMinusPionT)->DirectionStart[2]));
   // output().FillVar(mpion_istpccut, cutUtils::DeltaLYZTPCCut(*box().HMinusPionT));
//    output().FillVar(mpion_istarget1, cutUtils::DeltaLYZTargetCut(*box().HMinusPionT, SubDetId::kTarget1));
  //  output().FillVar(mpion_istarget2, cutUtils::DeltaLYZTargetCut(*box().HMinusPionT, SubDetId::kTarget2));

    float phi = atan2((box().HMinusPionT)->DirectionStart[1], (box().HMinusPionT)->DirectionStart[0]);
    float cosphi = cos(phi);

    output().FillVar(mpion_cosphi, cosphi);
    output().FillVar(mpion_phi, phi);

    AnaTrack *track = dynamic_cast<AnaTrack*>(box().HMinusPionT);
    output().FillVar(mpion_ekin, (box().HMinusPionT)->EKin);
    output().FillVar(mpion_SDLength, track->Length);
    Float_t pulls[4];

    for (int subdet = 0; subdet < 7; subdet++) {
      AnaTPCParticleB* TPCSegment = dynamic_cast<AnaTPCParticleB*>(anaUtils::GetSegmentInDet( *box().HMinusPionT, static_cast<SubDetId::SubDetEnum >(subdet)));
      if (TPCSegment) {
        output().FillVectorVar(mpion_tpc_det, subdet);
        output().FillVectorVar(mpion_LYZTPC, TPCSegment->DeltaLYZ);
        output().FillVectorVar(mpion_tpc_EDep, TPCSegment->EDeposit);
        output().FillVectorVar(mpion_tpc_length, TPCSegment->SegLength);
        output().FillMatrixVarFromArray(mpion_tpc_entrancepos,    TPCSegment->PositionStart, 4);
        output().FillMatrixVarFromArray(mpion_tpc_exitpos,    TPCSegment->PositionEnd, 4);
        output().FillVectorVar(mpion_tpc_smeared_momentum, TPCSegment->SmearedMomentum);
        output().FillVectorVar(mpion_tpc_momentum_error, TPCSegment->MomentumError);
        output().FillVectorVar(mpion_tpc_momentum, TPCSegment->Momentum);

        output().IncrementCounterForVar(mpion_tpc_det);

      }
    }

    AnaTargetParticleB* TargetSegment1 = dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet( *box().HMinusPionT, SubDetId::kTarget1));
    if (TargetSegment1) {
      output().FillVar(mpion_LTarget1, TargetSegment1->DeltaLYZ);
      output().FillVar(mpion_EDep_Target1, TargetSegment1->EDeposit);
      output().FillVectorVarFromArray(mpion_target1_entrancepos,    TargetSegment1->PositionStart, 4);
      output().FillVectorVarFromArray(mpion_target1_exitpos,    TargetSegment1->PositionEnd, 4);
      output().FillVar(mpion_isLastinTarget1, (int)cutUtils::FiducialCut((box().HMinusPionT)->PositionEnd, SubDetId::kTarget1) );


    }
    AnaTargetParticleB* TargetSegment2 = dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet( *box().HMinusPionT, SubDetId::kTarget2));
    if (TargetSegment2) {
      output().FillVar(mpion_LTarget2, TargetSegment2->DeltaLYZ);
      output().FillVar(mpion_EDep_Target2, TargetSegment2->EDeposit);
      output().FillVectorVarFromArray(mpion_target2_entrancepos,    TargetSegment2->PositionStart, 4);
      output().FillVectorVarFromArray(mpion_target2_exitpos,    TargetSegment2->PositionEnd, 4);
      output().FillVar(mpion_isLastinTarget2, (int)cutUtils::FiducialCut((box().HMinusPionT)->PositionEnd, SubDetId::kTarget2) );

    }

  }

// Pion information
  if (box().HPlusPionT) {

    AnaTrueParticle *trueP = dynamic_cast<AnaTrueParticle*>(box().HPlusPionT->GetTrueParticle());
    output().FillVar(ppion_pdg, trueP->PDG);
    output().FillVar(ppion_charge, trueP->Charge);
    output().FillVar(ppion_mom, box().HPlusPionT->Momentum);
    output().FillVar(ppion_costheta, (box().HPlusPionT)->DirectionStart[2]);
    output().FillVar(ppion_theta, (float)acos((box().HPlusPionT)->DirectionStart[2]));
   // output().FillVar(ppion_istpccut, cutUtils::DeltaLYZTPCCut(*box().HPlusPionT));
   // output().FillVar(ppion_istarget1, cutUtils::DeltaLYZTargetCut(*box().HPlusPionT, SubDetId::kTarget1));
    //output().FillVar(ppion_istarget2, cutUtils::DeltaLYZTargetCut(*box().HPlusPionT, SubDetId::kTarget2));


    float phi = atan2((box().HPlusPionT)->DirectionStart[1], (box().HPlusPionT)->DirectionStart[0]);
    float cosphi = cos(phi);

    output().FillVar(ppion_cosphi, cosphi);
    output().FillVar(ppion_phi, phi);

    AnaTrack *track = dynamic_cast<AnaTrack*>(box().HPlusPionT);
    output().FillVar(ppion_ekin, (box().HPlusPionT)->EKin);
    output().FillVar(ppion_SDLength, track->Length);
    Float_t pulls[4];

    for (int subdet = 0; subdet < 7; subdet++) {
      AnaTPCParticleB* TPCSegment = dynamic_cast<AnaTPCParticleB*>(anaUtils::GetSegmentInDet( *box().HPlusPionT, static_cast<SubDetId::SubDetEnum >(subdet)));
      if (TPCSegment) {

        output().FillVectorVar(ppion_tpc_det, subdet);
        output().FillVectorVar(ppion_LYZTPC, TPCSegment->DeltaLYZ);
        output().FillVectorVar(ppion_tpc_EDep, TPCSegment->EDeposit);
        output().FillVectorVar(ppion_tpc_length, TPCSegment->SegLength);
        output().FillMatrixVarFromArray(ppion_tpc_entrancepos,    TPCSegment->PositionStart, 4);
        output().FillMatrixVarFromArray(ppion_tpc_exitpos,    TPCSegment->PositionEnd, 4);
        output().FillVectorVar(ppion_tpc_smeared_momentum, TPCSegment->SmearedMomentum);
        output().FillVectorVar(ppion_tpc_momentum_error, TPCSegment->MomentumError);
        //  anaUtils::ComputeTPCPull(*TPCSegment, *box().HPlusPionT);
        output().FillVectorVar(ppion_tpc_momentum, TPCSegment->Momentum);

        anaUtils::ComputeTPCPull(*TPCSegment, pulls);
        output().FillVectorVar(tpc_pull_pion, pulls[3]);

        output().IncrementCounterForVar(ppion_tpc_c);


      }
    }



    AnaTargetParticleB* TargetSegment1 = dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet( *box().HPlusPionT, SubDetId::kTarget1));
    if (TargetSegment1) {
      output().FillVar(ppion_LTarget1, TargetSegment1->DeltaLYZ);
      output().FillVar(ppion_EDep_Target1, TargetSegment1->EDeposit);
      output().FillVectorVarFromArray(ppion_target1_entrancepos,    TargetSegment1->PositionStart, 4);
      output().FillVectorVarFromArray(ppion_target1_exitpos,    TargetSegment1->PositionEnd, 4);
      // output().FillVar(ppion_isLastinTarget1, (int)cutUtils::FiducialCut(*box().HPlusPionT->PositionEnd) );
      output().FillVar(ppion_isLastinTarget1, (int)cutUtils::FiducialCut((box().HPlusPionT)->PositionEnd, SubDetId::kTarget1) );


    }
    AnaTargetParticleB* TargetSegment2 = dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet( *box().HPlusPionT, SubDetId::kTarget2));
    if (TargetSegment2) {
      output().FillVar(ppion_LTarget2, TargetSegment2->DeltaLYZ);
      output().FillVar(ppion_EDep_Target2, TargetSegment2->EDeposit);
      output().FillVectorVarFromArray(ppion_target2_entrancepos,    TargetSegment2->PositionStart, 4);
      output().FillVectorVarFromArray(ppion_target2_exitpos,    TargetSegment2->PositionEnd, 4);
      output().FillVar(ppion_isLastinTarget2, (int)cutUtils::FiducialCut((box().HPlusPionT)->PositionEnd, SubDetId::kTarget2) );

    }

  }

// Proton information
  if (box().HProtonT) {

    AnaTrueParticle *trueP = dynamic_cast<AnaTrueParticle*>(box().HProtonT->GetTrueParticle());
    output().FillVar(proton_pdg, trueP->PDG);
    output().FillVar(proton_charge, trueP->Charge);
    output().FillVar(proton_mom, box().HProtonT->Momentum);
    output().FillVar(proton_costheta, (box().HProtonT)->DirectionStart[2]);
    output().FillVar(proton_theta, (float)acos((box().HProtonT)->DirectionStart[2]));
  //  output().FillVar(proton_istpccut, cutUtils::DeltaLYZTPCCut(*box().HProtonT));
   // output().FillVar(proton_istarget1, cutUtils::DeltaLYZTargetCut(*box().HProtonT, SubDetId::kTarget1));
   // output().FillVar(proton_istarget2, cutUtils::DeltaLYZTargetCut(*box().HProtonT, SubDetId::kTarget2));

    float phi = atan2((box().HProtonT)->DirectionStart[1], (box().HProtonT)->DirectionStart[0]);
    float cosphi = cos(phi);

    output().FillVar(proton_cosphi, cosphi);
    output().FillVar(proton_phi, phi);

    AnaTrack *track = dynamic_cast<AnaTrack*>(box().HProtonT);
    output().FillVar(proton_ekin, (box().HProtonT)->EKin);
    output().FillVar(proton_SDLength, track->Length);
    Float_t pulls[4];
    for (int subdet = 0; subdet < 7; subdet++) {
      AnaTPCParticleB* TPCSegment = dynamic_cast<AnaTPCParticleB*>(anaUtils::GetSegmentInDet( *box().HProtonT, static_cast<SubDetId::SubDetEnum >(subdet)));
      if (TPCSegment) {
        output().FillVectorVar(proton_tpc_det, subdet);
        output().FillVectorVar(proton_LYZTPC, TPCSegment->DeltaLYZ);
        output().FillVectorVar(proton_tpc_EDep, TPCSegment->EDeposit);
        output().FillVectorVar(proton_tpc_length, TPCSegment->SegLength);
        output().FillMatrixVarFromArray(proton_tpc_entrancepos,    TPCSegment->PositionStart, 4);
        output().FillMatrixVarFromArray(proton_tpc_exitpos,    TPCSegment->PositionEnd, 4);
        output().FillVectorVar(proton_tpc_smeared_momentum, TPCSegment->SmearedMomentum);
        output().FillVectorVar(proton_tpc_momentum_error, TPCSegment->MomentumError);
        //  anaUtils::ComputeTPCPull(*TPCSegment, *box().HProtonT);
        output().FillVectorVar(proton_tpc_momentum, TPCSegment->Momentum);

        anaUtils::ComputeTPCPull(*TPCSegment, pulls);
        output().FillVectorVar(tpc_pull_proton, pulls[2]);

        output().IncrementCounterForVar(proton_tpc_det);

      }
    }



    AnaTargetParticleB* TargetSegment1 = dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet( *box().HProtonT, SubDetId::kTarget1));
    if (TargetSegment1) {
      output().FillVar(proton_LTarget1, TargetSegment1->DeltaLYZ);
      output().FillVar(proton_EDep_Target1, TargetSegment1->EDeposit);
      output().FillVectorVarFromArray(proton_target1_entrancepos,    TargetSegment1->PositionStart, 4);
      output().FillVectorVarFromArray(proton_target1_exitpos,    TargetSegment1->PositionEnd, 4);
      output().FillVar(proton_isLastinTarget1, (int)cutUtils::FiducialCut((box().HProtonT)->PositionEnd, SubDetId::kTarget1) );


    }
    AnaTargetParticleB* TargetSegment2 = dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet( *box().HProtonT, SubDetId::kTarget2));
    if (TargetSegment2) {
      output().FillVar(proton_LTarget2, TargetSegment2->DeltaLYZ);
      output().FillVar(proton_EDep_Target2, TargetSegment2->EDeposit);
      output().FillVectorVarFromArray(proton_target2_entrancepos,    TargetSegment2->PositionStart, 4);
      output().FillVectorVarFromArray(proton_target2_exitpos,    TargetSegment2->PositionEnd, 4);
      output().FillVar(proton_isLastinTarget2, (int)cutUtils::FiducialCut((box().HProtonT)->PositionEnd, SubDetId::kTarget2) );

    }

  }


}

//********************************************************************
void truenumuCCAnalysis::FillToyVarsInMicroTrees(bool addBase) {
//********************************************************************

  // Fill all tree variables that vary for each toy experiment

  // Fill the common variables
  if (addBase) baseAnalysis::FillToyVarsInMicroTreesBase(addBase);

}

//********************************************************************
bool truenumuCCAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx) {
//********************************************************************
  // GetReactionCC already takes into account the outFV
  bool numuCCInFV = true; //(anaUtils::GetReactionCC(vtx, fgdID) == 1);
  return (numuCCInFV);
}

//********************************************************************
void truenumuCCAnalysis::FillTruthTree(const AnaTrueVertex& vtx) {
//********************************************************************

  // workaround to use the same code for the antuNumu package
  // calling truenumuCCAnalysis::FillTruthTreeBase(vtx,true)
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
void truenumuCCAnalysis::FillTruthTreeBase(const AnaTrueVertex& vtx, bool IsAntinu) {
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
void truenumuCCAnalysis::FillCategories() {
//********************************************************************

  // Fill the track categories for color drawing

  // For the muon candidate
}
