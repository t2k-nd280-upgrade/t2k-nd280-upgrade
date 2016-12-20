#include "testtimeAnalysis.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "CategoriesUtils.hxx"
#include "BasicUtils.hxx"
#include "baseSelection.hxx"
#include "numuCCMultiPiSelection.hxx"
#include "TreeConverterUtils.hxx"
#include "CutUtils.hxx"
#include "MomRangeCorrection.hxx"
#include "TruthUtils.hxx"
#include "PIDCorrection.hxx"
#include "PIDUtils.hxx"
//********************************************************************
testtimeAnalysis::testtimeAnalysis(AnalysisAlgorithm* ana) : baseAnalysis(ana) {
//********************************************************************

  // Add the package version
//  ND::versioning().AddPackage("testtimeAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("testtimeAnalysisROOT")));
}

//********************************************************************
bool testtimeAnalysis::Initialize() {
//********************************************************************

  // Initialize the baseAnalysis
  if (!baseAnalysis::Initialize()) return false;

  // Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("ndupAnalysis.MinAccumLevelToSave"));

  // Define categories
  anaUtils::AddStandardCategories();

  _detector = SubDetId::kTPCUp1;

  return true;
}

//********************************************************************
void testtimeAnalysis::DefineSelections() {
//********************************************************************

//  sel().AddSelection("ndup",    "inclusive ndup selection",     new baseSelection());     // true/false for forcing break
  //sel().AddSelection("numu",    "inclusive numu selection",     new numuCCMultiPiSelection());     // true/false for forcing break
sel().AddSelection("numu",    "inclusive numu selection",     new numuCCSelection());     // true/false for forcing break

}

//********************************************************************
void testtimeAnalysis::DefineCorrections() {
//********************************************************************

  // Some corrections are defined in baseAnalysis
  baseAnalysis::DefineCorrections();
  corr().AddCorrection("mom_corr",          new MomRangeCorrection(SubDetId::kTPC));
  corr().AddCorrection("pid_corr",          new PIDCorrection(SubDetId::kTPC));

}

//********************************************************************
void testtimeAnalysis::DefineSystematics() {
//********************************************************************

  // Some corrections are defined in baseAnalysis
  baseAnalysis::DefineSystematics();


//  eweight().AddEventWeight(SystId::kDetectorMass,           "DetectorMass",           new DetectorMassWeight());

//  evar().AddEventVariation(SystId::kdEdx,           "dEdx",           new dEdxVariation());
}

//********************************************************************
void testtimeAnalysis::DefineConfigurations() {
//********************************************************************

  // Some corrections are defined in baseAnalysis
  baseAnalysis::DefineConfigurations();

}

//********************************************************************
void testtimeAnalysis::DefineMicroTrees(bool addBase) {
//********************************************************************

  std::cout << "-------- Add variables to the analysis tree ----------------------" << std::endl;

  // Variables from baseAnalysis (run, event, ...)
  if (addBase) baseAnalysis::DefineMicroTrees(addBase);
  AddVarI(output(), Nu_pdg,         "nu pdg");
  AddVarF(output(), Nu_mom,         "nu momentum");
  AddVarI(output(), reaction_code,         "reaction_code");
  AddVarI(output(), current_code,         "current_code");

  AddVar4VF(output(), vertex_position,         "vertex position");

  AddVarVI(output(), muon_pdg,         "muon pdg",true_part);
  AddVarVF(output(), muon_SDLength,         "muon SD Length",true_part);
  AddVarVF(output(), muon_mom,         "muon momentum",true_part);
  AddVarVF(output(), muon_costheta,         "muon costheta",true_part);
  AddVarVF(output(), muon_cosphi,         "muon cosphi",true_part);
  AddVarVF(output(), muon_theta,         "muon theta",true_part);
  AddVarVF(output(), muon_phi,         "muon phi",true_part);
  AddVarVF(output(), muon_time,         "muon phi",true_part);
  AddVarVF(output(), muon_time_l,         "muon phi",true_part);
  AddVarVF(output(), muon_range,         "muon phi",true_part);

 // AddVarVF(output(), muon_length,         "muon phi",true_part);

  AddVarVF(output(), muon_pidlikelihood1,         "muon_pidlikelihood1",true_part);
  AddVarVF(output(), muon_pidlikelihood2,         "muon_pidlikelihood2",true_part);
  AddVarVF(output(), muon_pidlikelihood3,         "muon_pidlikelihood3",true_part);
  AddVarVF(output(), muon_pidlikelihood4,         "muon_pidlikelihood4",true_part);
 AddVarVF(output(), mip_pidlikelihood,         "mip_pidlikelihood",true_part);

  AddVarVF(output(), muon_charge,         "muon charge",true_part);
  AddVarVF(output(), muon_ekin,         "muon kinetic energy",true_part);
  AddVarVI(output(), muon_stopped,         "muon is length cut in tpc",true_part);
  AddVarVI(output(), muon_tpc,         "muon is length cut in tpc",true_part);

  AddVarMF(output(), muon_entrancepos,         "muon is length cut in tpc",true_part,-3000,4);
  AddVarMF(output(), muon_exitpos,         "muon is length cut in tpc",true_part,-3000,4);

  AddVarVI(output(), muon_istarget1,         "muon is length cut in target1",true_part);
  AddVarVI(output(), muon_istarget2,         "muon is length cut in target2",true_part);
 AddVarMF(output(), tpc_pull_muon,         "true_tpc_pull_muon", true_part, -4000, 7);
  AddVarMF(output(), tpc_pull_proton,         "true_tpc_pull_proton", true_part, -4000, 7);
  AddVarMF(output(), tpc_pull_pion,         "true_tpc_pull_pion", true_part, -4000, 7);
  AddVarMF(output(), tpc_pull_electron,         "true_tpc_pull_electron", true_part, -4000, 7);
 
//  AddVarMF(output(), muon_tpc_entrancepos,    "muon entrance position in tpc", tpc_muon, -300, 4);

 // AddVarMF(output(), muon_tpc_exitpos,    "muon exit position in tpc", tpc_muon, -300, 4);
 // AddVarVI(output(), muon_tpc_det,         "muon tpc det id", tpc_muon);
 // AddVarVF(output(), muon_LYZTPC,         "muon LYTPC", tpc_muon);
 
//  AddVarVF(output(), muon_tpc_length,         "muon tpc segmentLength", tpc_muon);
//  AddVarVF(output(), muon_tpc_EDep,         "muon tpc EDeposit", tpc_muon);/
//  AddVarVF(output(), muon_tpc_smeared_momentum,         "muon tpc smeared momentum", tpc_muon);
 // AddVarVF(output(), muon_tpc_momentum_error,         "muon tpc smeared momentum error", tpc_muon);
 // AddVarVF(output(), tpc_pull_muon,         "muon tpc pull ", tpc_muon);
 // AddVarVF(output(), muon_tpc_momentum,         "muon tpc true momentum", tpc_muon);

//  AddVarF(output(), muon_EDep_Target1,         "muon EDep in Target1");
 // AddVarF(output(), muon_EDep_Target2,         "muon EDep in Target2");
 // AddVarF(output(), muon_LTarget1,         "muon LTarget1");
 // AddVarF(output(), muon_LTarget2,         "muon LTarget2");
 // AddVarI(output(), muon_isLastinTarget1,         " is the muon track end postion in target1");
//  AddVarI(output(), muon_isLastinTarget2,         "is the muon track end postion in target2");

//  AddVar4VF(output(), muon_target1_entrancepos,         "muon target1 entrance position");
//  AddVar4VF(output(), muon_target1_exitpos,         "muon target1 exit position");
//  AddVar4VF(output(), muon_target2_entrancepos,         "muon target2 entrance position");
  //AddVar4VF(output(), muon_target2_exitpos,         "muon target2 exit position");


}

//********************************************************************
void testtimeAnalysis::DefineTruthTree() {
//********************************************************************

  // Variables from baseAnalysis (run, event, ...)
  baseAnalysis::DefineTruthTree();
  AddVarI(output(), true_Nu_pdg,         "nu pdg");
  AddVarF(output(), true_Nu_mom,         "nu momentum");
  AddVarI(output(), true_reaction_code,         "reaction_code");
  AddVar4VF(output(), true_vertex_position,         "vertex position");



}

//********************************************************************
void testtimeAnalysis::FillMicroTrees(bool addBase) {
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
    std::vector<AnaParticleB*> Particles = box().Vertex->ParticlesVect;
    AnaSpill& spill = GetSpill();
    for (UInt_t i = 0; i < spill.Bunches.size(); i++) {
      AnaBunch* bunch = static_cast<AnaBunch*>(spill.Bunches[i]);
      for (UInt_t j = 0; j < bunch->Particles.size(); j++) {

        AnaTrackB* track = static_cast<AnaTrackB*>(bunch->Particles[j]);
        AnaTrueParticle *trueP = dynamic_cast<AnaTrueParticle*>(track->GetTrueParticle());
        ///   output().FillVectorVar(muon_pdg, trueP->PDG);
        // output().FillVectorVar(muon_charge, trueP->Charge);
        // output().FillVectorVar(muon_mom, track->SmearedMomentum);
        // output().FillVectorVar(muon_costheta, (track)->DirectionStart[2]);
        // output().FillVectorVar(muon_theta, (float)acos((track)->DirectionStart[2]));
        // output().FillVectorVar(muon_SDLength, trueP->Length);

        //  output().FillVectorVar(muon_istpccut, cutUtils::DeltaLYZTPCCut(*track));
//      output().FillVectorVar(muon_istarget1, (int)cutUtils::DeltaLYZTargetCut(*track, SubDetId::kTarget1));
        // float tmpp = -9999;
        //float tmpd = 9999;
        //  int ip=-1;
        //bool tpcnon=-999;
        /*        for (int i = 0; i < trueP->DetCrossingsVect.size(); i++) {
                  if (SubDetId::GetDetectorUsed(trueP->DetCrossingsVect[i]->Detector, SubDetId::kTPCUp1) || SubDetId::GetDetectorUsed(trueP->DetCrossingsVect[i]->Detector, SubDetId::kTPCUp2 || SubDetId::GetDetectorUsed(trueP->DetCrossingsVect[i]->Detector, SubDetId::kTPCDown1) || SubDetId::GetDetectorUsed(trueP->DetCrossingsVect[i]->Detector, SubDetId::kTPCDown2 || SubDetId::GetDetectorUsed(trueP->DetCrossingsVect[i]->Detector, SubDetId::kForwTPC1) || SubDetId::GetDetectorUsed(trueP->DetCrossingsVect[i]->Detector, SubDetId::kForwTPC2) || SubDetId::GetDetectorUsed(trueP->DetCrossingsVect[i]->Detector, SubDetId::kForwTPC3)  )  ) ) {
                    tpcnon=0;
                  }else{
                    tpcnon=1;
                  }
                  if (trueP->DetCrossingsVect[i]->ExitPosition[3] > tmpp ) {
                    tmpp = trueP->DetCrossingsVect[i]->ExitPosition[3];
                    ip=i;
                  }
                  if (trueP->DetCrossingsVect[i]->EntrancePosition[3] < tmpd) {
                    tmpd = trueP->DetCrossingsVect[i]->EntrancePosition[3];

                  }

                  // output().IncrementCounterForVar(true_target1_det);
                }*/

        //  output().FillVectorVar(muon_time, tmpp - tmpd);
        //  output().FillVectorVar(muon_stopped, trueP->stopped);
        //  output().FillVectorVar(muon_tpc,tpcnon);

        // output().FillMatrixVarFromArray(muon_entrancepos, track->PositionStart, 4);
        // if (ip >= 0) {
        // output().FillMatrixVarFromArray(muon_exitpos, trueP->DetCrossingsVect[ip]->ExitPosition, 4);
        //}
        // Float_t range=anaUtils::ScalarProduct(trueP->DetCrossingsVect[ip]->ExitPosition,track->PositionStart,3);
        //  output().FillVectorVar(muon_range, range);
        /*
                double mass=0;
                if     (trueP->PDG == 211)  mass = 139.570; // pi+-
                else if (trueP->PDG == 13)   mass = 105.658; // muon
                else if (trueP->PDG == 2212) mass = 938.272; // proton
                else if (trueP->PDG == 11)   mass = 0.511; // electron

                double bg =trueP->Momentum / mass;
                double betta = bg / sqrt(1. + bg * bg);
                float v=betta*3*1000;*/
        //  if (trueP->Length) {
        //   if(trueP->PDG==2212 && trueP->ParentID==0){
        //  std::cout<<"trueP->Length/v"<<trueP->Length/v<<std::endl;
        // }
        //  output().FillVectorVar(muon_time_l, (float)trueP->Length/v);
        // }

        //    output().FillVectorVar(muon_istarget2, (int)cutUtils::DeltaLYZTargetCut(*track, SubDetId::kTarget2));
        //   Float_t PIDLikelihood[4];
//          std::cout << "tpc" << std::endl;
        Float_t pulls[4];

        //  anaUtils::GetPIDLikelihood(*track, PIDLikelihood, 0);
        /* output().FillVectorVar(mip_pidlikelihood, (PIDLikelihood[0] + PIDLikelihood[3]) / (1 - PIDLikelihood[2]));
         output().FillVectorVar(muon_pidlikelihood1, PIDLikelihood[0]);
         output().FillVectorVar(muon_pidlikelihood2, PIDLikelihood[1]);
         output().FillVectorVar(muon_pidlikelihood3, PIDLikelihood[2]);
         output().FillVectorVar(muon_pidlikelihood4, PIDLikelihood[3]);*/
        if (trueP->PDG == 13) {
          for (int subdet = 0; subdet < 7; subdet++) {
            AnaTPCParticleB* TPCSegment = dynamic_cast<AnaTPCParticleB*>(anaUtils::GetSegmentInDet( *track, static_cast<SubDetId::SubDetEnum >(subdet)));
            if (TPCSegment) {
              //      output().FillVectorVar(muon_tpc_det, subdet);
//
              anaUtils::ComputeTPCPull(*TPCSegment,*track);
//        output().FillVectorVar(tpc_pull_muon, pulls[0]);
              //   output().FillMatrixVar(tpc_pull_muon, pulls[0], -1, subdet);
              //  output().FillMatrixVar(tpc_pull_electron, pulls[1], -1, subdet);
              //  output().FillMatrixVar(tpc_pull_pion, pulls[3], -1, subdet);
              // output().FillMatrixVar(tpc_pull_proton, pulls[2], -1, subdet);


            }

          }
              for (int i = 0; i < trueP->DetCrossingsVect.size(); i++) {
               // std::cout<<"trutP"<<std::endl;
                if(SubDetId::GetDetectorUsed(trueP->DetCrossingsVect[i]->Detector, SubDetId::kTPCUp2) ||SubDetId::GetDetectorUsed(trueP->DetCrossingsVect[i]->Detector, SubDetId::kTPCUp1) || SubDetId::GetDetectorUsed(trueP->DetCrossingsVect[i]->Detector, SubDetId::kTPCDown1) || SubDetId::GetDetectorUsed(trueP->DetCrossingsVect[i]->Detector, SubDetId::kTPCDown2) || SubDetId::GetDetectorUsed(trueP->DetCrossingsVect[i]->Detector, SubDetId::kForwTPC1) || SubDetId::GetDetectorUsed(trueP->DetCrossingsVect[i]->Detector, SubDetId::kForwTPC2) || SubDetId::GetDetectorUsed(trueP->DetCrossingsVect[i]->Detector, SubDetId::kForwTPC3)  ){
                  anaUtils::ComputeTPCPull(*trueP->DetCrossingsVect[i], *trueP, pulls);
              
                }
              }

        }

        //  output().IncrementCounterForVar(muon_pdg);

      }
    }
  }
// Muon information
}

//********************************************************************
void testtimeAnalysis::FillToyVarsInMicroTrees(bool addBase) {
//********************************************************************

  // Fill all tree variables that vary for each toy experiment

  // Fill the common variables
  if (addBase) baseAnalysis::FillToyVarsInMicroTreesBase(addBase);

}

//********************************************************************
bool testtimeAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx) {
//********************************************************************
  // GetReactionCC already takes into account the outFV
  bool numuCCInFV = true; //(anaUtils::GetReactionCC(vtx, fgdID) == 1);
  return (numuCCInFV);
}

//********************************************************************
void testtimeAnalysis::FillTruthTree(const AnaTrueVertex& vtx) {
//********************************************************************

  // workaround to use the same code for the antuNumu package
  // calling testtimeAnalysis::FillTruthTreeBase(vtx,true)
  bool IsAntinu = false;
  std::vector<AnaTrueParticleB*> TrueParticles = vtx.TrueParticlesVect;
  output().FillVar(true_Nu_pdg, vtx.NuPDG);
  output().FillVar(true_Nu_mom, vtx.NuMom);

  output().FillVar(true_reaction_code, vtx.ReacCode);
  output().FillVectorVarFromArray(true_vertex_position, vtx.Position, 4);
/*
  for (Int_t i = 0; i < TrueParticles.size(); i++) {

    AnaTrueParticleB* trueTrack = dynamic_cast<AnaTrueParticleB*>(TrueParticles[i]);
    output().FillVectorVar(true_parentID, TrueParticles[i]->ParentID);
    output().FillVectorVar(true_pdg, trueTrack->PDG);
    output().FillVectorVar(true_mom, trueTrack->Momentum);
    output().FillVectorVar(true_Edep, trueTrack->EDeposit);
    output().FillVectorVar(true_SDlength, trueTrack->Length);
    output().FillVectorVar(true_costheta, trueTrack->CosTheta);
    output().FillVectorVar(true_ekin, trueTrack->EKin);
    output().FillVectorVar(true_charge, trueTrack->Charge);
    Float_t pulls[4];

    for (int i = 0; i < trueTrack->DetCrossingsVect.size(); i++) {

      if (SubDetId::GetDetectorUsed(trueTrack->DetCrossingsVect[i]->Detector, SubDetId::kTarget1)   ) {

        output().FillVectorVar(true_target1_det, SubDetId::GetTarget(trueTrack->DetCrossingsVect[i]->Detector));
        output().FillVectorVar(true_target1_length,  trueTrack->DetCrossingsVect[i]->DeltaLYZ);
        output().FillVectorVar(true_target1_Edep, trueTrack->DetCrossingsVect[i]->EDeposit);
        // output().IncrementCounterForVar(true_target1_det);
      }
      if (SubDetId::GetDetectorUsed(trueTrack->DetCrossingsVect[i]->Detector, SubDetId::kTarget2)) {

        output().FillVectorVar(true_target2_det, SubDetId::GetTarget(trueTrack->DetCrossingsVect[i]->Detector));
        output().FillVectorVar(true_target2_length,  trueTrack->DetCrossingsVect[i]->DeltaLYZ);
        output().FillVectorVar(true_target2_Edep, trueTrack->DetCrossingsVect[i]->EDeposit);
        // output().IncrementCounterForVar(true_target2_det);
      }

      if (SubDetId::GetDetectorUsed(trueTrack->DetCrossingsVect[i]->Detector, SubDetId::kTPCUp1)) {
        output().FillVectorVar(true_tpcup1_det, SubDetId::GetTPC(trueTrack->DetCrossingsVect[i]->Detector));
        output().FillVectorVar(true_tpcup1_DeltaLYZ,  trueTrack->DetCrossingsVect[i]->DeltaLYZ);
        float de_dx = (trueTrack->DetCrossingsVect[i]->EDeposit / units::keV) / (trueTrack->DetCrossingsVect[i]->SegLength / units::cm);
        output().FillMatrixVar(true_tpc_de_dx, de_dx, -1, 0);
        output().FillMatrixVar(true_tpc_length, trueTrack->DetCrossingsVect[i]->SegLength, -1, 0);

        output().FillMatrixVar(true_tpc_momentum, anaUtils::GetEntranceMomentum(*(trueTrack->DetCrossingsVect[i])), -1, 0);
        output().FillMatrixVar(true_tpc_exitmomentum, anaUtils::GetExitMomentum(*(trueTrack->DetCrossingsVect[i])), -1, 0);
        anaUtils::ComputeTPCPull(*trueTrack->DetCrossingsVect[i], *trueTrack, pulls);
        output().FillMatrixVar(true_tpc_pull_muon, pulls[0], -1, 0);
        output().FillMatrixVar(true_tpc_pull_electron, pulls[1], -1, 0);
        output().FillMatrixVar(true_tpc_pull_pion, pulls[3], -1, 0);
        output().FillMatrixVar(true_tpc_pull_proton, pulls[2], -1, 0);
      }
      if (SubDetId::GetDetectorUsed(trueTrack->DetCrossingsVect[i]->Detector, SubDetId::kTPCUp2)) {
        output().FillVectorVar(true_tpcup2_det, SubDetId::GetTPC(trueTrack->DetCrossingsVect[i]->Detector));
        output().FillVectorVar(true_tpcup2_DeltaLYZ,  trueTrack->DetCrossingsVect[i]->DeltaLYZ);
        float de_dx = (trueTrack->DetCrossingsVect[i]->EDeposit / units::keV) / (trueTrack->DetCrossingsVect[i]->SegLength / units::cm);
        output().FillMatrixVar(true_tpc_de_dx, de_dx, -1, 1);
        output().FillMatrixVar(true_tpc_length, trueTrack->DetCrossingsVect[i]->SegLength, -1, 1);

        output().FillMatrixVar(true_tpc_momentum, anaUtils::GetEntranceMomentum(*(trueTrack->DetCrossingsVect[i])), -1, 1);
        output().FillMatrixVar(true_tpc_exitmomentum, anaUtils::GetExitMomentum(*(trueTrack->DetCrossingsVect[i])), -1, 1);
        anaUtils::ComputeTPCPull(*trueTrack->DetCrossingsVect[i], *trueTrack, pulls);
        output().FillMatrixVar(true_tpc_pull_muon, pulls[0], -1, 1);
        output().FillMatrixVar(true_tpc_pull_electron, pulls[1], -1, 1);
        output().FillMatrixVar(true_tpc_pull_pion, pulls[3], -1, 1);
        output().FillMatrixVar(true_tpc_pull_proton, pulls[2], -1, 1);


      }
      if (SubDetId::GetDetectorUsed(trueTrack->DetCrossingsVect[i]->Detector, SubDetId::kTPCDown1)) {
        output().FillVectorVar(true_tpcdown1_det, SubDetId::GetTPC(trueTrack->DetCrossingsVect[i]->Detector));
        output().FillVectorVar(true_tpcdown1_DeltaLYZ,  trueTrack->DetCrossingsVect[i]->DeltaLYZ);
        float de_dx = (trueTrack->DetCrossingsVect[i]->EDeposit / units::keV) / (trueTrack->DetCrossingsVect[i]->SegLength / units::cm);
        output().FillMatrixVar(true_tpc_de_dx, de_dx, -1, 2);
        output().FillMatrixVar(true_tpc_length, trueTrack->DetCrossingsVect[i]->SegLength, -1, 2);

        output().FillMatrixVar(true_tpc_momentum, anaUtils::GetEntranceMomentum(*(trueTrack->DetCrossingsVect[i])), -1, 2);
        output().FillMatrixVar(true_tpc_exitmomentum, anaUtils::GetExitMomentum(*(trueTrack->DetCrossingsVect[i])), -1, 2);
        anaUtils::ComputeTPCPull(*trueTrack->DetCrossingsVect[i], *trueTrack, pulls);
        output().FillMatrixVar(true_tpc_pull_muon, pulls[0], -1, 2);
        output().FillMatrixVar(true_tpc_pull_electron, pulls[1], -1, 2);
        output().FillMatrixVar(true_tpc_pull_pion, pulls[3], -1, 2);
        output().FillMatrixVar(true_tpc_pull_proton, pulls[2], -1, 2);



      }
      if (SubDetId::GetDetectorUsed(trueTrack->DetCrossingsVect[i]->Detector, SubDetId::kTPCDown2)) {
        output().FillVectorVar(true_tpcdown2_det, SubDetId::GetTPC(trueTrack->DetCrossingsVect[i]->Detector));
        output().FillVectorVar(true_tpcdown2_DeltaLYZ,  trueTrack->DetCrossingsVect[i]->DeltaLYZ);
        float de_dx = (trueTrack->DetCrossingsVect[i]->EDeposit / units::keV) / (trueTrack->DetCrossingsVect[i]->SegLength / units::cm);
        output().FillMatrixVar(true_tpc_de_dx, de_dx, -1, 3);
        output().FillMatrixVar(true_tpc_length, trueTrack->DetCrossingsVect[i]->SegLength, -1, 3);
        output().FillMatrixVar(true_tpc_momentum, anaUtils::GetEntranceMomentum(*(trueTrack->DetCrossingsVect[i])), -1, 3);
        output().FillMatrixVar(true_tpc_exitmomentum, anaUtils::GetExitMomentum(*(trueTrack->DetCrossingsVect[i])), -1, 3);
        anaUtils::ComputeTPCPull(*trueTrack->DetCrossingsVect[i], *trueTrack, pulls);
        output().FillMatrixVar(true_tpc_pull_muon, pulls[0], -1, 3);
        output().FillMatrixVar(true_tpc_pull_electron, pulls[1], -1, 3);
        output().FillMatrixVar(true_tpc_pull_pion, pulls[3], -1, 3);
        output().FillMatrixVar(true_tpc_pull_proton, pulls[2], -1, 3);


      }
      if (SubDetId::GetDetectorUsed(trueTrack->DetCrossingsVect[i]->Detector, SubDetId::kForwTPC1)) {
        output().FillVectorVar(true_forwtpc1_det, SubDetId::GetTPC(trueTrack->DetCrossingsVect[i]->Detector));
        output().FillVectorVar(true_forwtpc1_DeltaLYZ,  trueTrack->DetCrossingsVect[i]->DeltaLYZ);
        float de_dx = (trueTrack->DetCrossingsVect[i]->EDeposit / units::keV) / (trueTrack->DetCrossingsVect[i]->SegLength / units::cm);
        output().FillMatrixVar(true_tpc_de_dx, de_dx, -1, 4);
        output().FillMatrixVar(true_tpc_length, trueTrack->DetCrossingsVect[i]->SegLength, -1, 4);

        output().FillMatrixVar(true_tpc_momentum, anaUtils::GetEntranceMomentum(*(trueTrack->DetCrossingsVect[i])), -1, 4);
        output().FillMatrixVar(true_tpc_exitmomentum, anaUtils::GetExitMomentum(*(trueTrack->DetCrossingsVect[i])), -1, 4);
        anaUtils::ComputeTPCPull(*trueTrack->DetCrossingsVect[i], *trueTrack, pulls);
        output().FillMatrixVar(true_tpc_pull_muon, pulls[0], -1, 4);
        output().FillMatrixVar(true_tpc_pull_electron, pulls[1], -1, 4);
        output().FillMatrixVar(true_tpc_pull_pion, pulls[3], -1, 4);
        output().FillMatrixVar(true_tpc_pull_proton, pulls[2], -1, 4);


      }
      if (SubDetId::GetDetectorUsed(trueTrack->DetCrossingsVect[i]->Detector, SubDetId::kForwTPC2)) {
        output().FillVectorVar(true_forwtpc2_det, SubDetId::GetTPC(trueTrack->DetCrossingsVect[i]->Detector));
        output().FillVectorVar(true_forwtpc2_DeltaLYZ,  trueTrack->DetCrossingsVect[i]->DeltaLYZ);
        float de_dx = (trueTrack->DetCrossingsVect[i]->EDeposit / units::keV) / (trueTrack->DetCrossingsVect[i]->SegLength / units::cm);
        output().FillMatrixVar(true_tpc_de_dx, de_dx, -1, 5);
        output().FillMatrixVar(true_tpc_length, trueTrack->DetCrossingsVect[i]->SegLength, -1, 5);

        output().FillMatrixVar(true_tpc_momentum, anaUtils::GetEntranceMomentum(*(trueTrack->DetCrossingsVect[i])), -1, 5);
        output().FillMatrixVar(true_tpc_exitmomentum, anaUtils::GetExitMomentum(*(trueTrack->DetCrossingsVect[i])), -1, 5);
        anaUtils::ComputeTPCPull(*trueTrack->DetCrossingsVect[i], *trueTrack, pulls);
        output().FillMatrixVar(true_tpc_pull_muon, pulls[0], -1, 5);
        output().FillMatrixVar(true_tpc_pull_electron, pulls[1], -1, 5);
        output().FillMatrixVar(true_tpc_pull_pion, pulls[3], -1, 5);
        output().FillMatrixVar(true_tpc_pull_proton, pulls[2], -1, 5);


      }
      if (SubDetId::GetDetectorUsed(trueTrack->DetCrossingsVect[i]->Detector, SubDetId::kForwTPC3)) {
        output().FillVectorVar(true_forwtpc3_det, SubDetId::GetTPC(trueTrack->DetCrossingsVect[i]->Detector));
        output().FillVectorVar(true_forwtpc3_DeltaLYZ,  trueTrack->DetCrossingsVect[i]->DeltaLYZ);
        float de_dx = (trueTrack->DetCrossingsVect[i]->EDeposit / units::keV) / (trueTrack->DetCrossingsVect[i]->SegLength / units::cm);
        output().FillMatrixVar(true_tpc_de_dx, de_dx, -1, 6);
        output().FillMatrixVar(true_tpc_length, trueTrack->DetCrossingsVect[i]->SegLength, -1, 6);

        output().FillMatrixVar(true_tpc_momentum, anaUtils::GetEntranceMomentum(*(trueTrack->DetCrossingsVect[i])), -1, 6);
        output().FillMatrixVar(true_tpc_exitmomentum, anaUtils::GetExitMomentum(*(trueTrack->DetCrossingsVect[i])), -1, 6);

        anaUtils::ComputeTPCPull(*trueTrack->DetCrossingsVect[i], *trueTrack, pulls);
        output().FillMatrixVar(true_tpc_pull_muon, pulls[0], -1, 6);
        output().FillMatrixVar(true_tpc_pull_electron, pulls[1], -1, 6);
        output().FillMatrixVar(true_tpc_pull_pion, pulls[3], -1, 6);
        output().FillMatrixVar(true_tpc_pull_proton, pulls[2], -1, 6);

      }

    }
    output().IncrementCounterForVar(true_parentID);

  }*/
  return FillTruthTreeBase(vtx, IsAntinu);
}


//********************************************************************
void testtimeAnalysis::FillTruthTreeBase(const AnaTrueVertex& vtx, bool IsAntinu) {
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
void testtimeAnalysis::FillCategories() {
//********************************************************************

  // Fill the track categories for color drawing

  // For the muon candidate
}
