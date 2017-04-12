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
//#include "TargetEffCorrection.hxx"
#include "PIDUtils.hxx"
#include "ToFUtils.hxx"
#include "AnaRecPackManager.hxx"

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
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("numuCCAnalysis.MinAccumLevelToSave"));

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
  //corr().AddCorrection("target_corr",          new TargetEffCorrection(SubDetId::kTarget));

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
  AddVarVI(output(), true_pdg,         "muon charge",true_in);
  AddVarVF(output(), true_mom,         "muon charge",true_in);
  AddVarVI(output(), true_parentID,         "muon charge",true_in);
  AddVarVF(output(), true_costheta,         "muon charge",true_in);
  AddVarVF(output(), true_length_BU,         "muon charge",true_in);
  AddVarVF(output(), true_length_FU,         "muon charge",true_in);
  AddVarVF(output(), true_length_BcD,         "muon charge",true_in);
  AddVarVF(output(), true_length_LU,         "muon charge",true_in);
  AddVarVF(output(), true_length_UE,         "muon charge",true_in);
  AddVarVF(output(), true_mass_BU,         "muon charge",true_in);
  AddVarVF(output(), true_mass_FU,         "muon charge",true_in);
  AddVarVF(output(), true_mass_BcD,         "muon charge",true_in);
  AddVarVF(output(), true_mass_LU,         "muon charge",true_in);
  AddVarVF(output(), true_mass_UE,         "muon charge",true_in);
  AddVarVF(output(), true_time_diff_BU,         "muon charge",true_in);
  AddVarVF(output(), true_time_diff_FU,         "muon charge",true_in);
  AddVarVF(output(), true_time_diff_BcD,         "muon charge",true_in);
  AddVarVF(output(), true_time_diff_LU,         "muon charge",true_in);
  AddVarVF(output(), true_time_diff_UE,         "muon charge",true_in);




}

//********************************************************************
void testtimeAnalysis::FillMicroTrees(bool addBase) {
//********************************************************************

  // WARNING: IF YOU RUN FGD1 AND FGD2 AT ONCE ( "whichFGD" parameter = 3), only for events with accum_level > 5
  // the vars in the output microtree will surely refer to the muon candidate in that FGD

  // Variables from baseAnalysis (run, event, ...)
  if (addBase) baseAnalysis::FillMicroTreesBase(addBase);
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
  for (Int_t i = 0; i < TrueParticles.size(); i++) {
    AnaTrueParticleB* trueTrack = dynamic_cast<AnaTrueParticleB*>(TrueParticles[i]);
    output().FillVectorVar(true_pdg, trueTrack->PDG);
    output().FillVectorVar(true_mom, trueTrack->Momentum);
    output().FillVectorVar(true_costheta, trueTrack->CosTheta);
    double mom=trueTrack->Momentum;
    std::cout<<trueTrack->PDG<<" "<<trueTrack->Charge<<std::endl;
      if (cutUtils::FiducialCut(trueTrack->Position, SubDetId::kTarget1)) {

    double  DeltaLYZ = -999;
    for (int i = 0; i < trueTrack->DetCrossingsVect.size(); i++) {
        //FrontUP
      std::cout<<trueTrack->DetCrossingsVect[i]->Detector_name<<std::endl;
        if (SubDetId::GetDetectorUsed(trueTrack->DetCrossingsVect[i]->Detector, SubDetId::kToFFrontUp)) {
            std::cout<<"front ToF"<<std::endl;
                     double dx = trueTrack->Position[0]-trueTrack->DetCrossingsVect[i]->EntrancePosition[0];
          double dy = trueTrack->Position[1]-trueTrack->DetCrossingsVect[i]->EntrancePosition[1];
          double dz = trueTrack->Position[2]-trueTrack->DetCrossingsVect[i]->EntrancePosition[2];
          double l_curr_straight = sqrt(dx*dx + dy*dy + dz*dz); 

         // double mom = anaUtils::GetEntranceMomentum(*(trueTrack->DetCrossingsVect[i]));
          TVector3 point(trueTrack->DetCrossingsVect[i]->EntrancePosition[0], trueTrack->DetCrossingsVect[i]->EntrancePosition[1], trueTrack->DetCrossingsVect[i]->EntrancePosition[2]);
          TVector3 normal(0, 0, 1);
          RP::State state1;

          double length = -9999;
          double time_diff = -9999;
          time_diff = trueTrack->DetCrossingsVect[i]->EntrancePosition[3] - trueTrack->Position[3];
          if (ND::tman().AnaTrueParticleB_to_RPState(*trueTrack, state1)) {
                      //  std::cout<<"convert "<<l_curr_straight<<std::endl;

            if (ND::tman().PropagateToSurface( point, normal, state1, length)) {
                        //    std::cout<<"length "<<length<<std::endl;

              output().FillVectorVar(true_length_FU, (float)length);
              output().FillVectorVar(true_time_diff_FU, (float)time_diff);
              double  mass = ToFUtils::GetToFMass(mom, length, time_diff);
              output().FillVectorVar(true_mass_FU, (float)mass);

            };
          }

        }

        //Bottom up
        if (SubDetId::GetDetectorUsed(trueTrack->DetCrossingsVect[i]->Detector, SubDetId::kToFBotUp)) {
           std::cout<<"bot ToF"<<std::endl;
                     double dx = trueTrack->Position[0]-trueTrack->DetCrossingsVect[i]->EntrancePosition[0];
          double dy = trueTrack->Position[1]-trueTrack->DetCrossingsVect[i]->EntrancePosition[1];
          double dz = trueTrack->Position[2]-trueTrack->DetCrossingsVect[i]->EntrancePosition[2];
          double l_curr_straight = sqrt(dx*dx + dy*dy + dz*dz); 

         //// double mom = anaUtils::GetEntranceMomentum(*(trueTrack->DetCrossingsVect[i]));
          TVector3 point(trueTrack->DetCrossingsVect[i]->EntrancePosition[0], trueTrack->DetCrossingsVect[i]->EntrancePosition[1], trueTrack->DetCrossingsVect[i]->EntrancePosition[2]);
          TVector3 normal(0, 1, 0);
          RP::State state1;
          double length = -9999;
          double time_diff = -9999;
          time_diff = trueTrack->DetCrossingsVect[i]->EntrancePosition[3] - trueTrack->Position[3];
          if (ND::tman().AnaTrueParticleB_to_RPState(*trueTrack, state1)) {
                   //     std::cout<<"convert "<<l_curr_straight<<std::endl;

            if (ND::tman().PropagateToSurface( point, normal, state1, length)) {
             // std::cout<<"length "<<length<<std::endl;
              output().FillVectorVar(true_length_BU, (float)length);
              output().FillVectorVar(true_time_diff_BU, (float)time_diff);
              double  mass = ToFUtils::GetToFMass(mom, length, time_diff);
              output().FillVectorVar(true_mass_BU, (float)mass);

            };
          }

        }

        //Left up
        if (SubDetId::GetDetectorUsed(trueTrack->DetCrossingsVect[i]->Detector, SubDetId::kToFLeftUp)) {
         // double mom = anaUtils::GetEntranceMomentum(*(trueTrack->DetCrossingsVect[i]));
          std::cout<<"left ToF"<<std::endl;
          TVector3 point(trueTrack->DetCrossingsVect[i]->ExitPosition[0], trueTrack->DetCrossingsVect[i]->ExitPosition[1], trueTrack->DetCrossingsVect[i]->ExitPosition[2]);
          TVector3 normal(1, 0, 0);
          RP::State state1;
          double length = -9999;
          double time_diff = -9999;
                    double dx = trueTrack->Position[0]-trueTrack->DetCrossingsVect[i]->EntrancePosition[0];
          double dy = trueTrack->Position[1]-trueTrack->DetCrossingsVect[i]->EntrancePosition[1];
          double dz = trueTrack->Position[2]-trueTrack->DetCrossingsVect[i]->EntrancePosition[2];
          double l_curr_straight = sqrt(dx*dx + dy*dy + dz*dz); 

          time_diff = trueTrack->DetCrossingsVect[i]->EntrancePosition[3] - trueTrack->Position[3];
          if (ND::tman().AnaTrueParticleB_to_RPState(*trueTrack, state1)) {
           // std::cout<<"convert "<<l_curr_straight<<std::endl;
            if (ND::tman().PropagateToSurface( point, normal, state1, length)) {
              output().FillVectorVar(true_length_LU, (float)length);
              output().FillVectorVar(true_time_diff_LU, (float)time_diff);
              double  mass = ToFUtils::GetToFMass(mom, length, time_diff);
           //   std::cout<<"length "<<length<<std::endl;

              output().FillVectorVar(true_mass_LU, (float)mass);

            };
          }

        }


        //Back down
        if (SubDetId::GetDetectorUsed(trueTrack->DetCrossingsVect[i]->Detector, SubDetId::kToFBackDown)) {
         // double mom = anaUtils::GetEntranceMomentum(*(trueTrack->DetCrossingsVect[i]));
           std::cout<<"back ToF"<<std::endl;
                     double dx = trueTrack->Position[0]-trueTrack->DetCrossingsVect[i]->EntrancePosition[0];
          double dy = trueTrack->Position[1]-trueTrack->DetCrossingsVect[i]->EntrancePosition[1];
          double dz = trueTrack->Position[2]-trueTrack->DetCrossingsVect[i]->EntrancePosition[2];
          double l_curr_straight = sqrt(dx*dx + dy*dy + dz*dz); 

          TVector3 point(trueTrack->DetCrossingsVect[i]->EntrancePosition[0], trueTrack->DetCrossingsVect[i]->EntrancePosition[1], trueTrack->DetCrossingsVect[i]->EntrancePosition[2]);
          TVector3 normal(0, 0, 1);
          RP::State state1;
          double length = -9999;
          double time_diff = -9999;
          time_diff = trueTrack->DetCrossingsVect[i]->EntrancePosition[3] - trueTrack->Position[3];
          if (ND::tman().AnaTrueParticleB_to_RPState(*trueTrack, state1)) {
                 //       std::cout<<"convert "<<l_curr_straight<<std::endl;

            if (ND::tman().PropagateToSurface( point, normal, state1, length)) {
                     //       std::cout<<"length "<<length<<std::endl;

              output().FillVectorVar(true_length_BcD, (float)length);
              output().FillVectorVar(true_time_diff_BcD, (float)time_diff);
              double  mass = ToFUtils::GetToFMass(mom, length, time_diff);
              output().FillVectorVar(true_mass_BcD, (float)mass);

            };
          }


        }
        //Back down

        if (SubDetId::GetDetectorUsed(trueTrack->DetCrossingsVect[i]->Detector, SubDetId::kP0D)) {
         //// double mom = anaUtils::GetEntranceMomentum(*(trueTrack->DetCrossingsVect[i]));
              std::cout<<"P0D ToF"<<std::endl;
                     double dx = trueTrack->Position[0]-trueTrack->DetCrossingsVect[i]->EntrancePosition[0];
          double dy = trueTrack->Position[1]-trueTrack->DetCrossingsVect[i]->EntrancePosition[1];
          double dz = trueTrack->Position[2]-trueTrack->DetCrossingsVect[i]->EntrancePosition[2];
          double l_curr_straight = sqrt(dx*dx + dy*dy + dz*dz); 

          TVector3 point(trueTrack->DetCrossingsVect[i]->EntrancePosition[0], trueTrack->DetCrossingsVect[i]->EntrancePosition[1], trueTrack->DetCrossingsVect[i]->EntrancePosition[2]);
          TVector3 normal(0, 0, 1);
          RP::State state1;
          double length = -9999;
          double time_diff = -9999;
          time_diff = trueTrack->DetCrossingsVect[i]->EntrancePosition[3] - trueTrack->Position[3];
          if (ND::tman().AnaTrueParticleB_to_RPState(*trueTrack, state1)) {
                    //    std::cout<<"convert "<<l_curr_straight<<std::endl;

            if (ND::tman().PropagateToSurface( point, normal, state1, length)) {
              output().FillVectorVar(true_length_UE, (float)length);
              output().FillVectorVar(true_time_diff_UE, (float)time_diff);
              double  mass = ToFUtils::GetToFMass(mom, length, time_diff);
              output().FillVectorVar(true_mass_UE, (float)mass);

            };
          }

        }


      }

    }

      ///next track
      output().IncrementCounterForVar(true_pdg);

    return FillTruthTreeBase(vtx, IsAntinu);
  }

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
