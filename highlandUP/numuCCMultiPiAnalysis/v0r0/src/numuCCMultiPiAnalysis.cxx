#include "numuCCMultiPiAnalysis.hxx"
#include "Parameters.hxx"
//#include "numuCCMultiPiTarget2Selection.hxx"
//#include "oaAnalysisTreeConverter.hxx"
#include "CategoriesUtils.hxx"
#include "EventBox.hxx"
#include "CoreUtils.hxx"
#include "ToyBoxNDUP.hxx"
#include "MomRangeCorrection.hxx"
#include "TruthUtils.hxx"
#include "PIDCorrection.hxx"

const bool DEBUG = false;    // debugging flag for FillTruthTree (increases verbosity)

const unsigned int NMAXTPCSEC = 2000;
const unsigned int NMAXTargetSEC = 2000;
const unsigned int NMAXTPC1 = 2000;

//********************************************************************
numuCCMultiPiAnalysis::numuCCMultiPiAnalysis(AnalysisAlgorithm* ana) : baseAnalysis(ana){
//********************************************************************

  // Add the package version
  ND::versioning().AddPackage("numuCCMultiPiAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("NUMUCCMULTIPIANALYSISROOT")));

  // Create a numuCCAnalysis passing this analysis to the constructor. In that way the same managers are used
  _ndupAnalysis = new ndupAnalysis(this);

  // Use the numuCCAnalysis (in practice that means that the same box and event will be used for the numuCCAnalysis as for this analysis)
  UseAnalysis(_ndupAnalysis);
}

//********************************************************************
bool numuCCMultiPiAnalysis::Initialize(){
//********************************************************************

  // Initialize the numuCCAnalysis
  if (!_ndupAnalysis->Initialize()) return false;

  // Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("numuCCMultiPiAnalysis.MinAccumLevelToSave"));

  // which analysis: Target1, Target2 or Targets
  //_whichTarget = ND::params().GetParameterI("numuCCMultiPiAnalysis.Selections.whichTarget");
  if (_whichTarget == 3) {
    std::cout << "----------------------------------------------------" << std::endl;
    std::cout << "WARNING: only for events with accum_level > 5 the vars in the output microtree will surely refer to the muon candidate in that Target" << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;
  }

  // Select the Target
  if (_whichTarget == 1) _TargetID = SubDetId::kTarget1;
  if (_whichTarget == 2) _TargetID = SubDetId::kTarget2;
  if (_whichTarget >  2) _TargetID = SubDetId::kTarget;

  // Add Pion SI info to the micro-trees
  //_addPionSIinfo = (bool)ND::params().GetParameterI("numuCCMultiPiAnalysis.MicroTrees.AddPionSIinfo");

  return true;
}

//********************************************************************
void numuCCMultiPiAnalysis::DefineSelections(){
//********************************************************************

  // Add the selection to the Selection Manager
 // if      (_whichTarget==1) // Target1
    sel().AddSelection("kTrackerNumuCCMultiPi",        "numuCC multi-pionselection",     new numuCCMultiPiSelection());
 // else if (_whichTarget==2) // Target2
 //   sel().AddSelection("kTrackerNumuCCMultiPiTarget2",    "numuCCTarget2 multi-pionselection", new numuCCMultiPiTarget2Selection());
 /// else if (_whichTarget==3) { // both Targets, in 2 selections
    //sel().AddSelection("kTrackerNumuCCMultiPi",        "numuCC multi-pionselection",     new numuCCMultiPiSelection());
   // sel().AddSelection("kTrackerNumuCCMultiPiTarget2",    "numuCCTarget2 multi-pionselection", new numuCCMultiPiTarget2Selection());
 // }
}

//********************************************************************
void numuCCMultiPiAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************

  // -------- Add variables to the analysis tree ----------------------
  
  // Variables from the numuCCAnalysis analysis (including the ones in baseTrackerAnalysis by default, otherwise addBase should be false)
  if (addBase) _ndupAnalysis->DefineMicroTrees(addBase);

  // --- Add here more variables for the CCMultiPi analysis -----

  AddVarI(output(),MuonVertexId, "");
  AddVarI(output(),MuonIndex,"");

  // --- Pion candidates 
  AddVarVF(output(),NegPionMom,"",       NNegPion);
  AddVarVF(output(),NegPionPidLik,"",    NNegPion);
  AddVarVF(output(),NegPionElPidLik,"",  NNegPion);
  AddVarVI(output(),NegPionTId,"",       NNegPion);
  AddVarVI(output(),NegPionIndex,"",     NNegPion);
  AddVarVI(output(),NegPionParentTId,"", NNegPion);
  AddVarVI(output(),NegPionGParentTId,"",NNegPion);
  AddVarVI(output(),NegPionVId,"",       NNegPion);
  AddVarMF(output(),NegPionDir,"",       NNegPion,-NMAXTPCSEC,3);
  AddVarMF(output(),NegPionPosStart,"",  NNegPion,-NMAXTPCSEC,3);
  AddVarMF(output(),NegPionPosEnd,"",    NNegPion,-NMAXTPCSEC,3);
  
  AddToyVarVF(output(),PosPionMom,"",    NPosPion);
  AddToyVarVF(output(),PosPionTheta,"",  NPosPion);
  AddVarVF(output(),PosPionPidLik,"",    NPosPion);
  AddVarVF(output(),PosPionElPidLik,"",  NPosPion);
  AddVarVI(output(),PosPionTId,"",       NPosPion);
  AddVarVI(output(),PosPionIndex,"",     NPosPion);
  AddVarVI(output(),PosPionParentTId,"", NPosPion);
  AddVarVI(output(),PosPionGParentTId,"",NPosPion);
  AddVarVI(output(),PosPionVId,"",       NPosPion);
  AddVarMF(output(),PosPionDir,"",       NPosPion,-NMAXTPCSEC,3);
  AddVarMF(output(),PosPionPosStart,"",  NPosPion,-NMAXTPCSEC,3);
  AddVarMF(output(),PosPionPosEnd,"",    NPosPion,-NMAXTPCSEC,3);
  
  // --- Pi0 candidates
  AddVarVF(output(),Pi0ElMom,"",       NPi0El);
  AddVarVF(output(),Pi0ElPull,"",      NPi0El);
  AddVarVI(output(),Pi0ElTId,"",       NPi0El);
  AddVarVI(output(),Pi0ElIndex,"",     NPi0El);
  AddVarVI(output(),Pi0ElParentTId,"", NPi0El);
  AddVarVI(output(),Pi0ElGParentTId,"",NPi0El);
  AddVarVI(output(),Pi0ElVId,"",       NPi0El);
  AddVarMF(output(),Pi0ElDir,"",       NPi0El,-NMAXTPCSEC,3);
  AddVarMF(output(),Pi0ElPosStart,"",  NPi0El,-NMAXTPCSEC,3);
  AddVarMF(output(),Pi0ElPosEnd,"",    NPi0El,-NMAXTPCSEC,3);

  AddVarVF(output(),Pi0PosMom,"",       NPi0Pos);
  AddVarVF(output(),Pi0PosPull,"",      NPi0Pos);
  AddVarVI(output(),Pi0PosTId,"",       NPi0Pos);
  AddVarVI(output(),Pi0PosIndex,"",     NPi0Pos);
  AddVarVI(output(),Pi0PosParentTId,"", NPi0Pos);
  AddVarVI(output(),Pi0PosGParentTId,"",NPi0Pos);
  AddVarVI(output(),Pi0PosVId,"",       NPi0Pos);
  AddVarMF(output(),Pi0PosDir,"",       NPi0Pos,-NMAXTPCSEC,3);
  AddVarMF(output(),Pi0PosPosStart,"",  NPi0Pos,-NMAXTPCSEC,3);
  AddVarMF(output(),Pi0PosPosEnd,"",    NPi0Pos,-NMAXTPCSEC,3);
 
  // --- ALL TPC sec tracks 
  AddVarVF(output(),TPCSecMom,"",       NTPCSec);
  AddVarVF(output(),TPCSecMuPidLik,"",  NTPCSec);
  AddVarVF(output(),TPCSecPiPidLik,"",  NTPCSec);
  AddVarVF(output(),TPCSecPrPidLik,"",  NTPCSec);
  AddVarVF(output(),TPCSecElPidLik,"",  NTPCSec);
  AddVarVI(output(),TPCSecDetectors,"", NTPCSec);
  AddVarVI(output(),TPCSecQ,"",         NTPCSec);
  AddVarVI(output(),TPCSecTId,"",       NTPCSec);
  AddVarVI(output(),TPCSecIndex,"",     NTPCSec);
  AddVarVI(output(),TPCSecParentTId,"", NTPCSec);
  AddVarVI(output(),TPCSecGParentTId,"",NTPCSec);
  AddVarMF(output(),TPCSecDir,"",       NTPCSec,-NMAXTPCSEC,3);
  AddVarMF(output(),TPCSecPosStart,"",  NTPCSec,-NMAXTPCSEC,3);
  AddVarMF(output(),TPCSecPosEnd,"",    NTPCSec,-NMAXTPCSEC,3);
  
  // ---- IsoTarget Pion tracks -----
  AddVarVF(output(),TargetPiLength,"",           NTargetPi); 
  AddVarVF(output(),TargetPiPiPull,"",           NTargetPi); 
  AddVarVF(output(),TargetPiAvgTime,"",          NTargetPi); 
  AddVarVF(output(),TargetPiDistance,"",         NTargetPi);
  AddVarVF(output(),TargetPiMuonangle,"",        NTargetPi);  
  AddVarVF(output(),TargetPiCosTheta,"",         NTargetPi);
  AddVarVI(output(),TargetPiTId,"",              NTargetPi);
  AddVarVI(output(),TargetPiIndex,"",            NTargetPi);
  AddVarVI(output(),TargetPiParentTId,"",        NTargetPi);
  AddVarVI(output(),TargetPiGParentTId,"",       NTargetPi);
  AddVarVI(output(),TargetPiVId,"",              NTargetPi);
  AddVarMF(output(),TargetPiDir,"",              NTargetPi,-NMAXTPCSEC,3);
  AddVarMF(output(),TargetPiPosStart,"",         NTargetPi,-NMAXTPCSEC,3);
  AddVarMF(output(),TargetPiPosEnd,"",           NTargetPi,-NMAXTPCSEC,3);
  AddVarVF(output(),TargetPiTimeIni,"",          NTargetPi); 
  AddVarVF(output(),TargetPiTimeEnd,"",          NTargetPi); 
  AddVarVF(output(),TargetPiDeltaTimeIniSelMu,"",NTargetPi);  
  AddVarVF(output(),TargetPiDeltaTimeEndSelMu,"",NTargetPi); 

  // ---- IsoTarget electron and positron tracks -----
  AddVarVF(output(),TargetElLength,"",           NTargetEl); 
  AddVarVF(output(),TargetElPiPull,"",           NTargetEl); 
  AddVarVF(output(),TargetElAvgTime,"",          NTargetEl); 
  AddVarVI(output(),TargetElTId,"",              NTargetEl); 
  AddVarVI(output(),TargetElIndex,"",            NTargetEl); 
  AddVarVI(output(),TargetElParentTId,"",        NTargetEl); 
  AddVarVI(output(),TargetElGParentTId,"",       NTargetEl); 
  AddVarVI(output(),TargetElVId,"",              NTargetEl);
  AddVarVF(output(),TargetElTimeIni,"",          NTargetEl);  
  AddVarVF(output(),TargetElTimeEnd,"",          NTargetEl); 
  AddVarVF(output(),TargetElDeltaTimeIniSelMu,"",NTargetEl);  
  AddVarVF(output(),TargetElDeltaTimeEndSelMu,"",NTargetEl); 
  AddVarVF(output(),TargetElDistance,"",         NTargetEl);
  AddVarVF(output(),TargetElMuonangle,"",        NTargetEl); 
  AddVarVF(output(),TargetElCosTheta,"",         NTargetEl);
  AddVarMF(output(),TargetElDir,"",              NTargetEl,-NMAXTargetSEC,3);
  AddVarMF(output(),TargetElPosStart,"",         NTargetEl,-NMAXTargetSEC,3);
  AddVarMF(output(),TargetElPosEnd,"",           NTargetEl,-NMAXTargetSEC,3);


  // ---- IsoTarget tracks -----
  AddVarVF(output(),TargetSecLength,"",           NTargetSec); 
  AddVarVF(output(),TargetSecPiPull,"",           NTargetSec); 
  AddVarVF(output(),TargetSecAvgTime,"",          NTargetSec); 
  AddVarVI(output(),TargetSecContained,"",        NTargetSec); 
  AddVarVI(output(),TargetSecTId,"",              NTargetSec); 
  AddVarVI(output(),TargetSecIndex,"",            NTargetSec); 
  AddVarVI(output(),TargetSecParentTId,"",        NTargetSec); 
  AddVarVI(output(),TargetSecGParentTId,"",       NTargetSec);
  AddVarVF(output(),TargetSecTimeIni,"",          NTargetSec); 
  AddVarVF(output(),TargetSecTimeEnd,"",          NTargetSec); 
  AddVarVF(output(),TargetSecDeltaTimeIniSelMu,"",NTargetSec);  
  AddVarVF(output(),TargetSecDeltaTimeEndSelMu,"",NTargetSec); 
  AddVarVF(output(),TargetSecCosTheta,"",         NTargetSec);
  AddVarMF(output(),TargetSecDir,"",              NTargetSec,-NMAXTargetSEC,3);
  AddVarMF(output(),TargetSecPosStart,"",         NTargetSec,-NMAXTargetSEC,3);
  AddVarMF(output(),TargetSecPosEnd,"",           NTargetSec,-NMAXTargetSEC,3);
 
  
  // ---- Michel Eletrons ----
  AddVarVI(output(),MENHits,"",    NME); 
  AddVarVF(output(),MERawCharge,"",NME); 
  AddVarVF(output(),MEMinTime,"",  NME); 
  AddVarVF(output(),MEMaxTime,"",  NME); 
  
  // ---- TPC1 Tracks ----
  AddVarVF(output(),TPC1TrackMom,"",       NTPC1Track); 
  AddVarVF(output(),TPC1TrackCosTheta,"",  NTPC1Track);
  AddVarVF(output(),TPC1TrackPhi,"",       NTPC1Track);
  AddVarVI(output(),TPC1TrackVId,"",       NTPC1Track); 
  AddVarVI(output(),TPC1TrackTId,"",       NTPC1Track); 
  AddVarVI(output(),TPC1TrackParentTId,"", NTPC1Track); 
  AddVarVI(output(),TPC1TrackGParentTId,"",NTPC1Track); 
  AddVarMF(output(),TPC1TrackPosStart,"",  NTPC1Track,-NMAXTPC1,3); 
  AddVarMF(output(),TPC1TrackPosEnd,"",    NTPC1Track,-NMAXTPC1,3); 

  // --- Info to understand Pion SI -----
 // if (_addPionSIinfo){
   // AddVarVI(output(),IntType,  "", NInts); 
   // AddVarVI(output(),PionType, "", NPions); 
  //  AddVarVF(output(),PionMom,  "", NPions); 
 // }
  
  //--- true pion variables -------
  AddVarF(  output(), truepi_mom,      "true pion momentum");
  AddVarF(  output(), truepi_costheta, "true pion cos(theta)");
  AddVar3VF(output(), truepi_dir,      "true pion direction");
}

//********************************************************************
void numuCCMultiPiAnalysis::DefineTruthTree(){
//********************************************************************

  // Variables from the numuCCAnalysis analysis
  _ndupAnalysis->DefineTruthTree();
  
  //--- true pion variables -------
  AddVarF(  output(), truepi_mom,      "true pion momentum");
  AddVarF(  output(), truepi_costheta, "true pion cos(theta)");
  AddVar3VF(output(), truepi_dir,      "true pion direction");
  
}

//********************************************************************
void numuCCMultiPiAnalysis::FillMicroTrees(bool addBase){
//********************************************************************
  
  // Variables from the numuCCAnalysis analysis (including the ones in baseTrackerAnalysis by default, otherwise addBase should be false)
  if (addBase) _ndupAnalysis->FillMicroTrees(addBase);
  
  if (mybox().HMNtrack  ) {
    if( mybox().HMNtrack->TrueObject ){
      output().FillVar(MuonIndex,mybox().HMNtrack->TrueObject->ID);
      if( mybox().HMNtrack->GetTrueParticle()->TrueVertex ) 
        output().FillVar(MuonVertexId,static_cast<AnaTrueVertex*>(mybox().HMNtrack->GetTrueParticle()->TrueVertex)->RooVtxIndex);
    }
  }

  // --- Fill here more variables for the CCMultiPi analysis -----
  // Selected negative pions 
  for( Int_t i = 0; i < mybox().nNegativePionTPCtracks; i++ ){
    AnaTrackB *track = mybox().NegativePionTPCtracks[i];
    Float_t ellklh = (anaUtils::GetPIDLikelihood(*track,0)+anaUtils::GetPIDLikelihood(*track,3))/(1.-anaUtils::GetPIDLikelihood(*track,2)); 
    output().FillVectorVar(NegPionMom,      track->Momentum);
    output().FillVectorVar(NegPionPidLik,   anaUtils::GetPIDLikelihood(*track,3));    
    output().FillVectorVar(NegPionPidLik,   ellklh);
    if( track->TrueObject ){
      output().FillVectorVar(NegPionTId,  track->GetTrueParticle()->PDG);
      output().FillVectorVar(NegPionIndex,track->GetTrueParticle()->ID); 
      output().FillVectorVar(NegPionParentTId, track->GetTrueParticle()->ParentPDG);
      output().FillVectorVar(NegPionGParentTId,track->GetTrueParticle()->GParentPDG);
      if( track->GetTrueParticle()->TrueVertex )
        output().FillVectorVar(NegPionVId,static_cast<AnaTrueVertex*>(track->GetTrueParticle()->TrueVertex)->RooVtxIndex);
    }
    output().FillMatrixVarFromArray(NegPionPosStart, track->PositionStart, 3);
    output().FillMatrixVarFromArray(NegPionPosEnd,   track->PositionEnd,3);
    output().FillMatrixVarFromArray(NegPionDir,      track->DirectionStart,3);
    output().IncrementCounter(NNegPion);
  }

  // Selected positive pions 
  for( Int_t i = 0; i <mybox().nPositivePionTPCtracks; i++ ){
    AnaTrackB *track = mybox().PositivePionTPCtracks[i];
    Float_t ellklh = (anaUtils::GetPIDLikelihood(*track,0)+anaUtils::GetPIDLikelihood(*track,3))/(1.-anaUtils::GetPIDLikelihood(*track,2)); 
    // output().FillVectorVar(PosPionMom,      track->Momentum); Filled in FillToyVarsInMicroTree
    output().FillVectorVar(PosPionPidLik,   anaUtils::GetPIDLikelihood(*track,3));    
    output().FillVectorVar(PosPionPidLik,   ellklh);

    if( track->TrueObject ){
      output().FillVectorVar(PosPionTId,  track->GetTrueParticle()->PDG);
      output().FillVectorVar(PosPionIndex,track->GetTrueParticle()->ID); 
      output().FillVectorVar(PosPionParentTId, track->GetTrueParticle()->ParentPDG);
      output().FillVectorVar(PosPionGParentTId,track->GetTrueParticle()->GParentPDG);
      if( track->GetTrueParticle()->TrueVertex ) {
        AnaTrueVertex *vtx = static_cast<AnaTrueVertex*>(track->GetTrueParticle()->TrueVertex);
        output().FillVectorVar(PosPionVId,vtx->RooVtxIndex);
        if (vtx->PionMom > 0) {
          // variables for primary pion were filled previously by oaAnalysisTreeConverter
          output().FillVar(truepi_mom, vtx->PionMom);
          output().FillVar(truepi_costheta, (Float_t)cos(anaUtils::ArrayToTVector3(vtx->PionDir).Angle(anaUtils::ArrayToTVector3(vtx->NuDir))));
          output().FillVectorVarFromArray(truepi_dir, vtx->PionDir, 3);
        } else if (vtx->NPrimaryParticles[ParticleId::kPiPos] > 0) {
          // variables not filled previously, look for pion in AnaTrueVertex
          AnaTrueParticleB* trk = NULL;
          for (int it=0; it<vtx->nTrueParticles; it++) {
            trk = vtx->TrueParticles[it];
            if (!trk) continue;
            if (trk->PDG == 211) {
              output().FillVar(truepi_mom, trk->Momentum);
              output().FillVar(truepi_costheta, (Float_t)cos(anaUtils::ArrayToTVector3(trk->Direction).Angle(anaUtils::ArrayToTVector3(vtx->NuDir))));
              output().FillVectorVarFromArray(truepi_dir, trk->Direction, 3);
              break;
            }
          }
        }
      }
      
    }

    output().FillMatrixVarFromArray(PosPionPosStart, track->PositionStart, 3); 
    output().FillMatrixVarFromArray(PosPionPosEnd,   track->PositionEnd,3);
    output().FillMatrixVarFromArray(PosPionDir,      track->DirectionStart,3); // Use PosPionTheta to access the angle, it is fille in FillToyVarsInMicroTree
    output().IncrementCounter(NPosPion);
  }

  // Selected pi0 electron tracks 
  for( Int_t i = 0; i < mybox().nElPi0TPCtracks; i++ ) {
    AnaTrackB *track = mybox().ElPi0TPCtracks[i];
    output().FillVectorVar(Pi0ElMom, track->Momentum);
    output().FillVectorVar(Pi0ElPull, anaUtils::GetPIDLikelihood(*track,1)); 
    if( track->GetTrueParticle() ){
      output().FillVectorVar(Pi0ElTId,       track->GetTrueParticle()->PDG);
      output().FillVectorVar(Pi0ElIndex,     track->GetTrueParticle()->ID);
      output().FillVectorVar(Pi0ElParentTId, track->GetTrueParticle()->ParentPDG);
      output().FillVectorVar(Pi0ElGParentTId,track->GetTrueParticle()->GParentPDG);
      if( track->GetTrueParticle()->TrueVertex  )
        output().FillVectorVar(Pi0ElVId,static_cast<AnaTrueVertex*>(track->GetTrueParticle()->TrueVertex)->RooVtxIndex);
    } 

    output().FillMatrixVarFromArray(Pi0ElPosStart, track->PositionStart, 3);
    output().FillMatrixVarFromArray(Pi0ElPosEnd,   track->PositionEnd,   3);
    output().FillMatrixVarFromArray(Pi0ElDir,      track->DirectionStart,3);
    output().IncrementCounter(NPi0El);
  }

  // Selected pi0 positron tracks 
  for( Int_t i = 0; i < mybox().nPosPi0TPCtracks; i++ ) {
    AnaTrackB *track = mybox().PosPi0TPCtracks[i];
    output().FillVectorVar(Pi0PosMom, track->Momentum);
    output().FillVectorVar(Pi0PosPull, anaUtils::GetPIDLikelihood(*track,1)); 
    if( track->GetTrueParticle() ){
      output().FillVectorVar(Pi0PosTId,       track->GetTrueParticle()->PDG);
      output().FillVectorVar(Pi0PosIndex,     track->GetTrueParticle()->ID);
      output().FillVectorVar(Pi0PosParentTId, track->GetTrueParticle()->ParentPDG);
      output().FillVectorVar(Pi0PosGParentTId,track->GetTrueParticle()->GParentPDG);
      if( track->GetTrueParticle()->TrueVertex  )
        output().FillVectorVar(Pi0PosVId,static_cast<AnaTrueVertex*>(track->GetTrueParticle()->TrueVertex)->RooVtxIndex);
    } 
    
    output().FillMatrixVarFromArray(Pi0PosPosStart, track->PositionStart, 3);
    output().FillMatrixVarFromArray(Pi0PosPosEnd,   track->PositionEnd,   3);
    output().FillMatrixVarFromArray(Pi0PosDir,      track->DirectionStart,3);
    output().IncrementCounter(NPi0Pos);
  }
 

  // All TPC secondary tracks 
  for( Int_t i = 0; i < mybox().nPositiveTPCtracks; i++ ) {
    AnaTrackB *track = mybox().PositiveTPCtracks[i];

    if( mybox().HMNtrack == track ) continue; // This is the muon

    output().FillVectorVar(TPCSecMom,               track->Momentum);
    output().FillVectorVar(TPCSecPiPidLik,          anaUtils::GetPIDLikelihood(*track,3));
    output().FillVectorVar(TPCSecMuPidLik,          anaUtils::GetPIDLikelihood(*track,0));
    output().FillVectorVar(TPCSecElPidLik,          anaUtils::GetPIDLikelihood(*track,1));
    output().FillVectorVar(TPCSecPrPidLik,          anaUtils::GetPIDLikelihood(*track,2));
    output().FillMatrixVarFromArray(TPCSecPosStart, track->PositionStart,3);
    output().FillMatrixVarFromArray(TPCSecPosEnd,   track->PositionEnd,3);
    output().FillMatrixVarFromArray(TPCSecDir,      track->DirectionStart,3);
    output().FillVectorVar(TPCSecQ,1); 
    if( track->GetTrueParticle() ){
      output().FillVectorVar(TPCSecTId,       track->GetTrueParticle()->PDG);
      output().FillVectorVar(TPCSecIndex,     track->GetTrueParticle()->ID);
      output().FillVectorVar(TPCSecParentTId, track->GetTrueParticle()->ParentPDG);
      output().FillVectorVar(TPCSecGParentTId,track->GetTrueParticle()->GParentPDG);
    }
    
    output().IncrementCounter(NTPCSec); 
  }


   for( Int_t i = 0; i < mybox().nNegativeTPCtracks; i++ ) {
    AnaTrackB *track = mybox().NegativeTPCtracks[i];
    if( mybox().HMNtrack == track ) continue; // This is the muon

    output().FillVectorVar(TPCSecMom,               track->Momentum);
    output().FillVectorVar(TPCSecPiPidLik,          anaUtils::GetPIDLikelihood(*track,3));
    output().FillVectorVar(TPCSecMuPidLik,          anaUtils::GetPIDLikelihood(*track,0));
    output().FillVectorVar(TPCSecElPidLik,          anaUtils::GetPIDLikelihood(*track,1));
    output().FillVectorVar(TPCSecPrPidLik,          anaUtils::GetPIDLikelihood(*track,2));
    output().FillMatrixVarFromArray(TPCSecPosStart, track->PositionStart,3);
    output().FillMatrixVarFromArray(TPCSecPosEnd,   track->PositionEnd,3);
    output().FillMatrixVarFromArray(TPCSecDir,      track->DirectionStart,3);
    output().FillVectorVar(TPCSecQ,-1); 
    if( track->GetTrueParticle() ){
      output().FillVectorVar(TPCSecTId,       track->GetTrueParticle()->PDG);
      output().FillVectorVar(TPCSecIndex,     track->GetTrueParticle()->ID);
      output().FillVectorVar(TPCSecParentTId, track->GetTrueParticle()->ParentPDG);
      output().FillVectorVar(TPCSecGParentTId,track->GetTrueParticle()->GParentPDG);
    }
    output().IncrementCounter(NTPCSec); 
  }
  /*
  // isoTarget pion candidates 
  for (Int_t i=0;i < mybox().nIsoTargetPiontracks;i++){
    AnaTrackB* track =  mybox().IsoTargetPiontracks[i];
    AnaTargetParticle *TargetTrack = static_cast<AnaTargetParticle*>(track->TargetSegments[0]);
    if( !TargetTrack ) continue; 
    ///checking times
    Float_t timeinipi = TargetTrack->PositionStart[3];
    Float_t timeendpi = TargetTrack->PositionEnd[3];

    output().FillVectorVar(TargetPiPiPull,  TargetTrack->Pullpi); 
    output().FillVectorVar(TargetPiAvgTime, TargetTrack->AvgTime); 
    // Direction of the segment 
    output().FillVectorVar(TargetPiCosTheta,(Float_t)(anaUtils::ArrayToTVector3(TargetTrack->DirectionStart).CosTheta()));
    //output().FillVectorVar(TargetPiLength,TargetTrack->Length); 
    output().FillVectorVar(TargetPiLength,  TargetTrack->X); 
    output().FillVectorVar(TargetPiTimeIni, timeinipi);  
    output().FillVectorVar(TargetPiTimeEnd, timeendpi); 
    
    output().FillMatrixVarFromArray(TargetPiDir,     TargetTrack->DirectionStart,3);
    output().FillMatrixVarFromArray(TargetPiPosStart,TargetTrack->PositionStart, 3);
    output().FillMatrixVarFromArray(TargetPiPosEnd,  TargetTrack->PositionEnd,   3);
    if( track->GetTrueParticle() ){
      output().FillVectorVar(TargetPiTId,       track->GetTrueParticle()->PDG);
      output().FillVectorVar(TargetPiIndex,     track->GetTrueParticle()->ID); 
      output().FillVectorVar(TargetPiParentTId, track->GetTrueParticle()->ParentPDG);
      output().FillVectorVar(TargetPiGParentTId,track->GetTrueParticle()->GParentPDG);
      if( track->GetTrueParticle()->TrueVertex )
      	output().FillVectorVar(TargetPiVId,static_cast<AnaTrueVertex*>(track->GetTrueParticle()->TrueVertex)->RooVtxIndex);
     }
     if( mybox().HMNtrack ) { 
       Float_t deltainimuinipi = TMath::Abs(timeinipi-mybox().HMNtrack->PositionStart[3]);
       Float_t deltaendmuinipi = TMath::Abs(timeendpi-mybox().HMNtrack->PositionStart[3]); 
       output().FillVectorVar(TargetPiDeltaTimeIniSelMu,deltainimuinipi);  
       output().FillVectorVar(TargetPiDeltaTimeEndSelMu,deltaendmuinipi); 
       // Minimum distance to the muon vertex 
       Float_t dist1 = (anaUtils::ArrayToTVector3(TargetTrack->PositionStart) - anaUtils::ArrayToTVector3(mybox().HMNtrack->PositionStart)).Mag();
       Float_t dist2 = (anaUtils::ArrayToTVector3(TargetTrack->PositionEnd)   - anaUtils::ArrayToTVector3(mybox().HMNtrack->PositionStart)).Mag();
       output().FillVectorVar(TargetPiDistance,TMath::Min(dist1,dist2)); 
     }
     output().IncrementCounter(NTargetPi);
  }

  // isoTarget electron and positron candidates 
  for (Int_t i=0;i < mybox().nIsoTargetElPi0tracks;i++){
    AnaTrackB* track =  mybox().IsoTargetElPi0tracks[i];
    AnaTargetParticle *TargetTrack = static_cast<AnaTargetParticle*>(track->TargetSegments[0]);
    if( !TargetTrack ) continue; 
    Float_t timeiniEl = TargetTrack->PositionStart[3];
    Float_t timeendEl = TargetTrack->PositionEnd[3];
    output().FillVectorVar(TargetElCosTheta,(Float_t)(anaUtils::ArrayToTVector3(TargetTrack->DirectionStart).CosTheta()));
    output().FillVectorVar(TargetElPiPull,TargetTrack->Pullpi); 
    output().FillVectorVar(TargetElAvgTime,TargetTrack->AvgTime); 
    //output().FillVectorVar(TargetElLength,TargetTrack->Length); 
    output().FillVectorVar(TargetElLength,TargetTrack->X); 
    output().FillVectorVar(TargetElTimeIni,timeiniEl);  
    output().FillVectorVar(TargetElTimeEnd,timeendEl); 
    //  Minimum distance to the muon vertex 
    if( mybox().HMNtrack ) {
      Float_t deltainimuiniEl = TMath::Abs(timeiniEl-mybox().HMNtrack->PositionStart[3]);
      Float_t deltaendmuiniEl = TMath::Abs(timeendEl-mybox().HMNtrack->PositionStart[3]);
      Float_t dist1 = (anaUtils::ArrayToTVector3(TargetTrack->PositionStart) - anaUtils::ArrayToTVector3(mybox().HMNtrack->PositionStart)).Mag();
      Float_t dist2 = (anaUtils::ArrayToTVector3(TargetTrack->PositionEnd)   - anaUtils::ArrayToTVector3(mybox().HMNtrack->PositionStart)).Mag();
      Float_t prodEl1 = anaUtils::ArrayToTVector3(TargetTrack->DirectionStart)*anaUtils::ArrayToTVector3(mybox().HMNtrack->DirectionStart);
      
      output().FillVectorVar(TargetElDeltaTimeIniSelMu,deltainimuiniEl);  
      output().FillVectorVar(TargetElDeltaTimeEndSelMu,deltaendmuiniEl);  
      output().FillVectorVar(TargetElDistance,TMath::Min(dist1,dist2));
      output().FillVectorVar(TargetElMuonangle,prodEl1);

    }
    // Direction and position  of the segment
    output().FillMatrixVarFromArray(TargetElDir,     TargetTrack->DirectionStart,3);
    output().FillMatrixVarFromArray(TargetElPosStart,TargetTrack->PositionStart, 3);
    output().FillMatrixVarFromArray(TargetElPosEnd,  TargetTrack->PositionEnd,   3);
 
    if( track->GetTrueParticle() ){
      output().FillVectorVar(TargetElTId,track->GetTrueParticle()->PDG);
      output().FillVectorVar(TargetElIndex,track->GetTrueParticle()->ID);       
      output().FillVectorVar(TargetElParentTId,track->GetTrueParticle()->ParentPDG);
      output().FillVectorVar(TargetElGParentTId,track->GetTrueParticle()->GParentPDG);
      if( track->GetTrueParticle()->TrueVertex )
        output().FillVectorVar(TargetElVId,static_cast<AnaTrueVertex*>(track->GetTrueParticle()->TrueVertex)->RooVtxIndex);
    }
    output().IncrementCounter(NTargetEl); 
  }

  // All Target tracks. 
  AnaRecObjectC** allTargetParticles = NULL;
  int nTarget=0;
  if (_TargetID == SubDetId::kTarget1){
    nTarget         = _event->EventBoxes[EventBoxId::kEventBoxTracker]->nRecObjectsInGroup[EventBoxTracker::kTracksWithTarget1AndNoTPC];
    allTargetParticles = _event->EventBoxes[EventBoxId::kEventBoxTracker]->RecObjectsInGroup[EventBoxTracker::kTracksWithTarget1AndNoTPC];  
  }
  else if (_TargetID == SubDetId::kTarget2){
    nTarget         = _event->EventBoxes[EventBoxId::kEventBoxTracker]->nRecObjectsInGroup[EventBoxTracker::kTracksWithTarget2AndNoTPC];
    allTargetParticles = _event->EventBoxes[EventBoxId::kEventBoxTracker]->RecObjectsInGroup[EventBoxTracker::kTracksWithTarget2AndNoTPC];  
  }
  for (Int_t i=0;i<nTarget;i++){
    AnaTrackB* track = static_cast<AnaTrackB*>(allTargetParticles[i]);
    AnaTargetParticle *TargetTrack = static_cast<AnaTargetParticle*>(track->TargetSegments[0]);
    if( !TargetTrack ) continue; 
    ///checking times
    Float_t timeini = TargetTrack->PositionStart[3];
    Float_t timeend = TargetTrack->PositionEnd[3];
    output().FillVectorVar(TargetSecTimeIni,timeini);  
    output().FillVectorVar(TargetSecTimeEnd,timeend); 

    if( track->GetTrueParticle() ){
      output().FillVectorVar(TargetSecTId,track->GetTrueParticle()->PDG);
      output().FillVectorVar(TargetSecIndex,track->GetTrueParticle()->ID); 
      output().FillVectorVar(TargetSecParentTId,track->GetTrueParticle()->ParentPDG);
      output().FillVectorVar(TargetSecGParentTId,track->GetTrueParticle()->GParentPDG);
    }

    if( mybox().HMNtrack ) {
      Float_t deltainimuini = TMath::Abs(timeini-mybox().HMNtrack->PositionStart[3]);
      Float_t deltaendmuini = TMath::Abs(timeend-mybox().HMNtrack->PositionStart[3]);
      output().FillVectorVar(TargetSecDeltaTimeIniSelMu,deltainimuini);  
      output().FillVectorVar(TargetSecDeltaTimeEndSelMu,deltaendmuini); 
    }

    // Direction of the segment 
    output().FillVectorVar(TargetSecCosTheta,(Float_t)(anaUtils::ArrayToTVector3(TargetTrack->DirectionStart).CosTheta()));
    output().FillMatrixVarFromArray(TargetSecDir,     TargetTrack->DirectionStart,3);
    output().FillMatrixVarFromArray(TargetSecPosStart,TargetTrack->PositionStart, 3);
    output().FillMatrixVarFromArray(TargetSecPosEnd,  TargetTrack->PositionEnd,   3);
    output().FillVectorVar(TargetSecContained,        TargetTrack->Containment);
    output().FillVectorVar(TargetSecPiPull,           TargetTrack->Pullpi); 
    output().FillVectorVar(TargetSecAvgTime,          TargetTrack->AvgTime); 
    output().FillVectorVar(TargetSecLength,           TargetTrack->X); 
    //output().FillVectorVar(TargetSecLength,           TargetTrack->Length); 
    output().IncrementCounter(NTargetSec); 
    
  }*/

  // Retrieve the EventBox such that we can get the number of michel electrons
  EventBox* EventBox_ = static_cast<EventBox*>(_event->EventBoxes[EventBoxId::kEventBoxNDUP]);
  SubDetId::SubDetEnum det = static_cast<SubDetId::SubDetEnum>(mybox().DetectorFV);
/*
  // Fill ME variables. 
  for (Int_t i=0;i < EventBox->nTargetMichelElectrons[det] ;i++){
    AnaTargetTimeBin* me =  static_cast<AnaTargetTimeBin*>(EventBox->TargetMichelElectrons[det][i]);
    if( me->NHits[0] != 0 ) {
      output().FillVectorVar(MENHits,(Int_t)me->NHits[0]);
      output().FillVectorVar(MERawCharge,me->RawChargeSum[0]);
    }
    else {
      output().FillVectorVar(MENHits,(Int_t)me->NHits[1]); 
      output().FillVectorVar(MERawCharge,me->RawChargeSum[1]);
    }
    output().FillVectorVar(MEMinTime,me->MinTime);
    output().FillVectorVar(MEMaxTime,me->MaxTime);
    output().IncrementCounter(NME);
  }
*/
  /*// Fill TPC1 track variables 
  SubDetId::SubDetEnum tpc = SubDetId::kTPC1;
  if (box().DetectorFV == SubDetId::kTarget1) tpc = SubDetId::kTPC2;  

  AnaTrackB** TPCtracks;
  anaUtils::CreateArray(TPCtracks, NMAXPARTICLES);
  Int_t nTPCtracks = FindTPCTracks(mybox(), tpc, TPCtracks);

  for( Int_t i = 0; i < nTPCtracks; i++ ) {
    AnaTrackB *track = TPCtracks[i];
    if( !track ) continue;
    Float_t phi=TMath::ATan2(track->DirectionStart[1],track->DirectionStart[0]);
    output().FillVectorVar(TPC1TrackPhi,phi );
    if( track->GetTrueParticle() ){
      output().FillVectorVar(TPC1TrackTId,track->GetTrueParticle()->PDG);
      output().FillVectorVar(TPC1TrackParentTId,track->GetTrueParticle()->ParentPDG);
      output().FillVectorVar(TPC1TrackGParentTId,track->GetTrueParticle()->GParentPDG);
      if( track->GetTrueParticle()->TrueVertex )	
      	output().FillVectorVar(TPC1TrackVId,static_cast<AnaTrueVertex*>(track->GetTrueParticle()->TrueVertex)->RooVtxIndex);
    }
    output().FillMatrixVarFromArray(TPC1TrackPosStart,track->PositionStart,3);
    output().FillMatrixVarFromArray(TPC1TrackPosEnd,  track->PositionEnd,  3);
    output().FillVectorVar(TPC1TrackMom, track->Momentum);
    output().IncrementCounter(NTPC1Track);
  }
  delete TPCtracks;*/
/*
  // --- Info to understand Pion SI -----
  if (_addPionSIinfo){
    PionInteractionSystematic* pionSI = _pionSIManager.ComputePionWeightInfo(GetEvent(), *sel().GetSelection("kTrackerNumuCCMultiPi"), 0);
    
    for(int ii = 0; ii < pionSI->nInteractions; ii++){
      
      int intType = pionSI->typeInteraction[ii];          
      int mech;
      if      (intType%10 == 0) mech = 0;
      else if (intType%10 == 1) mech = 1;
      else if (intType%10 == 4) mech = 2;
      else mech=-1;
      output().FillVectorVar(IntType,mech);
      output().IncrementCounter(NInts);
    }
    for(int ns = 0; ns < pionSI->nPions; ns++){
      output().FillVectorVar(PionType,pionSI->pionType[ns]);
      output().FillVectorVar(PionMom,pionSI->initMom[ns]);
      output().IncrementCounter(NPions);
    }
  }*/
}

//*********************************************************************
Int_t numuCCMultiPiAnalysis::FindTPCTracks(const ToyBoxB& boxB, SubDetId::SubDetEnum det, AnaTrackB** TPCtracks ){
//*********************************************************************

    // Get tracks in a specific TPC
    
    const ToyBoxNDUP& box = *static_cast<const ToyBoxNDUP*>(&boxB);
  
    Int_t nTPCtracks = 0;

    for(int i = 0; i < box.nPositiveTPCtracks; i++ ) {
        AnaTrackB *ptrack = box.PositiveTPCtracks[i];
        if( SubDetId::GetDetectorUsed(ptrack->Detector, det) ){
            TPCtracks[nTPCtracks++] = ptrack; 
        }
    }

    for(int i = 0; i < box.nNegativeTPCtracks; i++ ) {
        AnaTrackB *ptrack = box.NegativeTPCtracks[i];
        if( SubDetId::GetDetectorUsed(ptrack->Detector, det) ){
            TPCtracks[nTPCtracks++] = ptrack; 
        }
    }

    return nTPCtracks;
}

//********************************************************************
void numuCCMultiPiAnalysis::FillToyVarsInMicroTrees(bool addBase){
//********************************************************************

  // Variables from the numuCCAnalysis analysis (including the ones in baseTrackerAnalysis by default, otherwise addBase should be false)
    if (addBase) _ndupAnalysis->FillToyVarsInMicroTrees(addBase);

  // Fill here Variables specific for the numuCCMultiPi analysis - pion momentum and angle for CC-1pi analysis
    for( Int_t i = 0; i <mybox().nPositivePionTPCtracks; i++ ){
      AnaTrackB *track = mybox().PositivePionTPCtracks[i];
      output().FillToyVectorVar(PosPionMom,    track->Momentum, i);
      output().FillToyVectorVar(PosPionTheta,  (Float_t)acos(track->DirectionStart[2]), i);
    }

    // Fill whether it is signal or bkg
    // Filled also in numuCCAnalysis, overwritten here
   /* if(mybox().MainTrack && mybox().MainTrack->TrueObject) {
      AnaTrueVertex *vtx = static_cast<AnaTrueVertex*>(mybox().MainTrack->GetTrueParticle()->TrueVertex);
      if (vtx) {
        int ts = 0; // BKG
        // in Target1 FV
             if (anaUtils::GetTopology(*vtx, SubDetId::kTarget1) == 0) ts = 10;
        else if (anaUtils::GetTopology(*vtx, SubDetId::kTarget1) == 1) ts = 11;
        else if (anaUtils::GetTopology(*vtx, SubDetId::kTarget1) == 2) ts = 12;
        // in Target2 FV
        else if (anaUtils::GetTopology(*vtx, SubDetId::kTarget2) == 0) ts = 20;
        else if (anaUtils::GetTopology(*vtx, SubDetId::kTarget2) == 1) ts = 21;
        else if (anaUtils::GetTopology(*vtx, SubDetId::kTarget2) == 2) ts = 22;
        output().FillToyVar(true_signal, ts);
      }
    }*/
}

//********************************************************************
bool numuCCMultiPiAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  // GetReactionCC already takes into account the outFV and also
  // the NuWro reaction code for 2p2h in prod5 (that is 70)
  bool numuCCinFV = (anaUtils::GetReactionCC(vtx, _TargetID) == 1);

  // Since our topology definition doesn't consider the reaction code
  // and since a muon pair can be created in the FSI (likely in DIS)
  // in principle we might have a non-CC vertex categorized as CCother
  // (nevertheless I didn't find any.)
  int topo = anaUtils::GetTopology(vtx, _TargetID);
  bool topoCCinFV = (topo == 0 || topo == 1 || topo == 2);

  return (numuCCinFV || topoCCinFV);
}

//********************************************************************
void numuCCMultiPiAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  // Variables from the numuCCAnalysis analysis
  _ndupAnalysis->FillTruthTree(vtx);

  // Variables for true pion
  if (vtx.PionMom > 0) {
    // variables for primary pion were filled previously by oaAnalysisTreeConverter
    if (DEBUG) std::cout << "INFO: Pion info found: vtx.PionMom = " << vtx.PionMom << std::endl;
    output().FillVar(truepi_mom, vtx.PionMom);
    output().FillVar(truepi_costheta, (Float_t)cos(anaUtils::ArrayToTVector3(vtx.PionDir).Angle(anaUtils::ArrayToTVector3(vtx.NuDir))));
    output().FillVectorVarFromArray(truepi_dir, vtx.PionDir, 3);
  } else if (vtx.NPrimaryParticles[ParticleId::kPiPos] > 0) {
    // variables not filled previously, look for pion in AnaTrueVertex
    AnaTrueParticleB* trk = NULL;
    for (int it=0; it<vtx.nTrueParticles; it++) {
      trk = vtx.TrueParticles[it];
      if (!trk) continue;
      if (trk->PDG == 211  ) {
        if (DEBUG) std::cout << "INFO: No pion info (AnaTrueVertex::PionMom), but found pion (AnaTrueVertexB::TrueParticles) with trk->Momentum = " << trk->Momentum << std::endl;
        //if (DEBUG) std::cout << "      vtx.TruthVertexID = " << vtx.TruthVertexID << ", trk.ID = " << trk->ID << std::endl;     // debugging with Enrico
        output().FillVar(truepi_mom, trk->Momentum);
        output().FillVar(truepi_costheta, (Float_t)cos(anaUtils::ArrayToTVector3(trk->Direction).Angle(anaUtils::ArrayToTVector3(vtx.NuDir))));
        output().FillVectorVarFromArray(truepi_dir, trk->Direction, 3);
        break;
      }
    }
  }

  // Fill whether it is signal or bkg
  // Filled also in numuCCAnalysis, overwritten here
 /* int ts = 0; // BKG
  // in Target1 FV
       if (anaUtils::GetTopology(vtx, SubDetId::kTarget1) == 0) ts = 10;
  else if (anaUtils::GetTopology(vtx, SubDetId::kTarget1) == 1) ts = 11;
  else if (anaUtils::GetTopology(vtx, SubDetId::kTarget1) == 2) ts = 12;
  // in Target2 FV
  else if (anaUtils::GetTopology(vtx, SubDetId::kTarget2) == 0) ts = 20;
  else if (anaUtils::GetTopology(vtx, SubDetId::kTarget2) == 1) ts = 21;
  else if (anaUtils::GetTopology(vtx, SubDetId::kTarget2) == 2) ts = 22;
  output().FillVar(true_signal, ts);*/
}

