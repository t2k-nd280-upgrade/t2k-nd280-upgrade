#include "numuCCMultiPiSelection.hxx"
#include "numuCCSelection.hxx"
#include "baseSelection.hxx"
//#include "SystematicTuning.hxx"
#include "CutUtils.hxx"
#include "EventBoxUtils.hxx"
#include "Parameters.hxx"
#include "SubDetId.hxx"
#include "SystId.hxx"
#include "SystematicUtils.hxx"
#include <DetectorDefinition.hxx>


/**
   \\! [numuCCMultiPiSelection_info]
   This selection adds tree branches on top of the numuCCSelection. As descrived in numuCCMultiPiSelection::DefineSteps "numuCCMultiPiSelection::DefineSteps "the cuts of the numuCCSelection are copied to this selection. 
   Then new cuts for each branch are added: 

   Cuts for Branch 0: This is the branch requiring no pions
   <table>
   <tr><th> #cut  <th> cut class                        <th>  from source file   <th>  Cut description
   <tr><td> 6     <td> ExternalVetoCut::Apply           <td> numuCCSelection.cxx <td> External veto cut 
   <tr><td> 7     <td> ExternalTarget1lastlayersCut::Apply <td> numuCCSelection.cxx <td> External Target1 last layers cut
   </table>

   Cuts for Branch 1: This is the branch requiring 1 and only 1 pion
   <table>
   <tr><th> #cut  <th> cut class                        <th>  from source file   <th>  Cut description
   <tr><td> 6     <td> ExternalVetoCut::Apply           <td> numuCCSelection.cxx <td> External veto cut 
   <tr><td> 7     <td> ExternalTarget1lastlayersCut::Apply <td> numuCCSelection.cxx <td> External Target1 last layers cut
   </table>

   Cuts for Branch 2: This is the branch requiring more than 1 pion
   <table>
   <tr><th> #cut  <th> cut class                        <th>  from source file   <th>  Cut description
   <tr><td> 6     <td> ExternalVetoCut::Apply           <td> numuCCSelection.cxx <td> External veto cut 
   <tr><td> 7     <td> ExternalTarget1lastlayersCut::Apply <td> numuCCSelection.cxx <td> External Target1 last layers cut
   </table>


   \\! [numuCCMultiPiSelection_info]
*/

//*******************************************************************************
numuCCMultiPiSelection::numuCCMultiPiSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxNDUP){
//*******************************************************************************

  // Initialize systematic tuning parameters
 // systTuning::Initialize();
}

/// This is the method where the actual steps are added to the selection
/// \anchor numuCCMultiPiSelection_DefineSteps
//*******************************************************************************
void numuCCMultiPiSelection::DefineSteps() {
//*******************************************************************************

  // Copy all steps from the numuCCSelection
  //  CopySteps(_numuCCSelection);
  AddStep(StepBase::kAction, "find true vertex",   new numuCCUtils::FindTrueVertexAction());
  AddStep(StepBase::kCut, " true vertex in target  ",   new numuCCUtils::TrueVertexInTarget1Cut(), true);

  AddStep(StepBase::kCut, "> 0 tracks ",   new numuCCUtils::TotalMultiplicityCut(), true);
  AddStep(StepBase::kAction, "find vertex",   new numuCCUtils::FindVertexAction());
  AddStep(StepBase::kAction, "find leading tracks", new numuCCUtils::FindLeadingTracksAction());
  AddStep(StepBase::kCut, " vertex in target  ",   new numuCCUtils::VertexInTarget1Cut(), true);
  AddStep(StepBase::kCut, "Track quality cut", new numuCCUtils::TrackQualityCut());
  AddStep(StepBase::kCut, "muon PID",  new numuCCUtils::MuonPIDCut(), true);
  


  //Additional actions for the multi-pi selection.
  AddStep( StepBase::kAction, "fill_summary_multiPi", new FillSummaryAction_numuCCMultiPi());
  AddStep( StepBase::kAction, "find_pions",           new FindPionsAction());
  //Add a split to the trunk with 3 branches.
  AddSplit(3);

  //First branch is for CC-0pi
  AddStep(0, StepBase::kCut, " true no pion cut  ",   new numuCCUtils::TrueNoPionCut());

  AddStep(0, StepBase::kCut, "CC-0pi",        new NoPionCut());
  AddStep(1, StepBase::kCut, " true one pion cut  ",   new numuCCUtils::TrueOnePionCut());

  //Second branch is for CC-1pi
  AddStep(1, StepBase::kCut, "CC-1pi",        new OnePionCut());
  AddStep(2, StepBase::kCut, " true other cut  ",   new numuCCUtils::TrueOtherCut());

  //Third branch is for CC-Other
  AddStep(2, StepBase::kCut, "CC-Other", new OthersCut());
  // Set the branch aliases to the three branches
  SetBranchAlias(0, "CC-0pi", 0);
  SetBranchAlias(1, "CC-1pi", 1);
  SetBranchAlias(2, "CC-Other", 2);

  // By default the preselection correspond to cuts 0-2
  SetPreSelectionAccumLevel(0);

  // Step and Cut numbers needed by CheckRedoSelection
  _MuonPIDCutIndex     = GetCutNumber("muon PID");
  _FindPionsStepIndex  = GetStepNumber("find_pions");
}


//*******************************************************************************
void numuCCMultiPiSelection::DefineDetectorFV(){
//*******************************************************************************

    // The detector in which the selection is applied
   // _numuCCSelection.SetDetectorFV(SubDetId::kTarget1);
    SetDetectorFV(SubDetId::kTarget1);
}

//********************************************************************
bool numuCCMultiPiSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

    //CC0pi
    if(allCutsPassed[0]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC0Pi;
    }  
    //CC1pi
    else if (allCutsPassed[1]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC1Pi;
    }
    //CCOther
    else if (allCutsPassed[2]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCCOther;
    }

    // otherwise kUnassigned is used from the EventSummary constructor
    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}


//*********************************************************************
bool FillSummaryAction_numuCCMultiPi::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//*********************************************************************

    // Cast the ToyBox to the appropriate type
    ToyBoxNDUP& box = *static_cast<ToyBoxNDUP*>(&boxB); 


    if(!box.HMNtrack) return 1;

    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kTarget1NuMuCC0Pi] = box.HMNtrack;
    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kTarget1NuMuCC1Pi] = box.HMNtrack;
    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kTarget1NuMuCCOther] = box.HMNtrack;

    for(int i = 0; i < 4; ++i){
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kTarget1NuMuCC0Pi][i] = box.HMNtrack->PositionStart[i];
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kTarget1NuMuCC1Pi][i] = box.HMNtrack->PositionStart[i];
        static_cast<AnaEventSummaryB*>(event.Summary)->VertexPosition[SampleId::kTarget1NuMuCCOther][i] = box.HMNtrack->PositionStart[i];
    }
    if(box.HMNtrack->GetTrueParticle()){
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kTarget1NuMuCC0Pi] = box.HMNtrack->GetTrueParticle()->TrueVertex;
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kTarget1NuMuCC1Pi] = box.HMNtrack->GetTrueParticle()->TrueVertex;
        static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kTarget1NuMuCCOther] = box.HMNtrack->GetTrueParticle()->TrueVertex;
    }
    return 1;
}

//*********************************************************************
bool FindPionsAction::Apply(AnaEventC& event, ToyBoxB& box) const{
//*********************************************************************

    SubDetId::SubDetEnum det = static_cast<SubDetId::SubDetEnum>(box.DetectorFV);

    ToyBoxCCMultiPi * ccmultipibox = static_cast<ToyBoxCCMultiPi*>(&box);
    EventBox* EventBox_ = static_cast<EventBox*>(event.EventBoxes[EventBoxId::kEventBoxNDUP]);

    if (useTPCPions) numuCCMultiPiUtils::FindTPCPions(event, box, det, useOldSecondaryPID); 
//    if (useME)       numuCCMultiPiUtils::FindMEPions(event,det, prod5Cut);
  //  if (useTargetPions) numuCCMultiPiUtils::FindIsoTargetPions(event, box, det);

    int nnegpions        =  ccmultipibox->nNegativePionTPCtracks; 
    int npospions        =  ccmultipibox->nPositivePionTPCtracks; 
    int nisoTargetpions     =  ccmultipibox->nIsoTargetPiontracks;  
    //int nmichelelectrons =  EventBox_->nTargetMichelElectrons[det];
    int npi0 = ccmultipibox->nPosPi0TPCtracks+ ccmultipibox->nElPi0TPCtracks;

  //  int pionTarget = nmichelelectrons;
  //  if (!nmichelelectrons && nisoTargetpions>0) pionTarget = 1;
    ccmultipibox->nPosPions   = npospions;//+pionTarget;
    ccmultipibox->nOtherPions = nnegpions+npi0;
    return true;
}

//*********************************************************************
bool NoPionCut::Apply(AnaEventC& event, ToyBoxB& box) const{
//*********************************************************************

    (void)event;

    ToyBoxCCMultiPi * ccmultipibox = static_cast<ToyBoxCCMultiPi*>(&box);
    AnaEventB& eventB = *static_cast<AnaEventB*>(&event); 

    SubDetId::SubDetEnum det = static_cast<SubDetId::SubDetEnum>(box.DetectorFV);

    EventBox::RecObjectGroupEnum groupID;
    if      (det==SubDetId::kTarget1) groupID = EventBox::kTracksWithTPCAndTarget1;
    else if (det==SubDetId::kTarget2) groupID = EventBox::kTracksWithTPCAndTarget2;
  //  if      (det==SubDetId::kTarget1) groupID = EventBox::kTracksWithGoodQualityTPCInTarget1FV;
   // else if (det==SubDetId::kTarget2) groupID = EventBox::kTracksWithGoodQualityTPCInTarget2FV;
    //else return;

    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];
  
    if( ccmultipibox->nPosPions + ccmultipibox->nOtherPions == 0 ) return true; 
    return false; 
}


//*********************************************************************
bool OnePionCut::Apply(AnaEventC& event, ToyBoxB& box) const{
//*********************************************************************

    (void)event;

    ToyBoxCCMultiPi * ccmultipibox = static_cast<ToyBoxCCMultiPi*>(&box);
    
    // one Positive pion and no other pions
    if( ccmultipibox->nOtherPions != 0 ) return false; 
    if( ccmultipibox->nPosPions == 1 )   return true; 
    return false; 
}


//*********************************************************************
bool OthersCut::Apply(AnaEventC& event, ToyBoxB& box) const{
//*********************************************************************

    (void)event;

    ToyBoxCCMultiPi * ccmultipibox = static_cast<ToyBoxCCMultiPi*>(&box);

    // More than one positive pion or more than zero other pions or event doesn't pass the pi0 veto
    if( ccmultipibox->nOtherPions != 0 )  return true; 
    if( ccmultipibox->nPosPions > 1 )     return true; 
    if( ccmultipibox->nPi0Ecaltracks > 0) return true;

    return false; 
}

//*********************************************************************
void numuCCMultiPiUtils::FindTPCPions(AnaEventC& event, ToyBoxB& box, SubDetId::SubDetEnum det, bool useOldSecondaryPID){
//*********************************************************************

    ToyBoxCCMultiPi * ccmultipibox = static_cast<ToyBoxCCMultiPi*>(&box);

    ccmultipibox->nPositivePionTPCtracks = 0;
    ccmultipibox->nPosPi0TPCtracks = 0;
    ccmultipibox->nNegativePionTPCtracks = 0;
    ccmultipibox->nElPi0TPCtracks = 0;
          AnaEventB& eventB = *static_cast<AnaEventB*>(&event); 


    EventBox::RecObjectGroupEnum groupID;
    if      (det==SubDetId::kTarget1) groupID = EventBox::kTracksWithTPCAndTarget1;
    else if (det==SubDetId::kTarget2) groupID = EventBox::kTracksWithTPCAndTarget1;
  //  if      (det==SubDetId::kTarget1) groupID = EventBox::kTracksWithGoodQualityTPCInTarget1FV;
   // else if (det==SubDetId::kTarget2) groupID = EventBox::kTracksWithGoodQualityTPCInTarget2FV;
    else return;

    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];
  
    // Look for pions in positive tracks 
    for(int i = 0; i < EventBox->nRecObjectsInGroup[groupID]; i++ ) {
      AnaTrackB *ptrack = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);
      if(ptrack->GetTrueParticle()->PDG==111){
                ccmultipibox->PosPi0TPCtracks[ccmultipibox->nPosPi0TPCtracks] = ptrack; 
                ccmultipibox->nPosPi0TPCtracks++;

      }
           if (ptrack->Charge>0){
        if(useOldSecondaryPID){
            if ( numuCCMultiPiUtils::TPCpionSelection(ptrack) ) {
                ccmultipibox->PositivePionTPCtracks[ccmultipibox->nPositivePionTPCtracks] = ptrack;
                ccmultipibox->nPositivePionTPCtracks++;
            }
            else if ( numuCCMultiPiUtils::TPCElPi0Selection(ptrack) ) {
                ccmultipibox->PosPi0TPCtracks[ccmultipibox->nPosPi0TPCtracks] = ptrack; 
                ccmultipibox->nPosPi0TPCtracks++;
            }
        }
        else {
     
            Float_t PIDLikelihood[4];
            anaUtils::GetPIDLikelihood(*ptrack, PIDLikelihood);

            // For Positive tracks we distinguish pions, electrons and protons.
            double ElLklh = PIDLikelihood[1];  
            double ProtonLklh = PIDLikelihood[2];  
            double PionLklh = PIDLikelihood[3];  
            double norm = ElLklh+ProtonLklh+PionLklh;
            ProtonLklh /= norm; 
            ElLklh /= norm; 
            PionLklh /= norm; 

            if( ProtonLklh > ElLklh && ProtonLklh > PionLklh ) continue; // If the highest probability is a proton continue. 

            // Id associated to the largest of the two probabilities.
            if( PionLklh > ElLklh ){
 
                ccmultipibox->PositivePionTPCtracks[ccmultipibox->nPositivePionTPCtracks] = ptrack;
                ccmultipibox->nPositivePionTPCtracks++;
            }
            else {
              //  if( ptrack->Momentum > 900. ) continue; // This is mainly protons.
               // ccmultipibox->PosPi0TPCtracks[ccmultipibox->nPosPi0TPCtracks] = ptrack; 
               // ccmultipibox->nPosPi0TPCtracks++;
            }
        }
      }
      else{
        if( ccmultipibox->HMNtrack == ptrack ) continue; // Same as the leading track.

        if(useOldSecondaryPID) {
            if ( numuCCMultiPiUtils::TPCpionSelection(ptrack) ) {
                ccmultipibox->NegativePionTPCtracks[ccmultipibox->nNegativePionTPCtracks] = ptrack;
                ccmultipibox->nNegativePionTPCtracks++;
            } 
            else if (  numuCCMultiPiUtils::TPCElPi0Selection(ptrack) ) {
                ccmultipibox->ElPi0TPCtracks[ccmultipibox->nElPi0TPCtracks] = ptrack; 
                ccmultipibox->nElPi0TPCtracks++;

            }
        }
        else {
            // For Negative tracks we distinguish pions, electrons and protons.
            Float_t PIDLikelihood[4];
            anaUtils::GetPIDLikelihood(*ptrack, PIDLikelihood);

            // For Positive tracks we distinguish pions, electrons and protons.
            double ElLklh = PIDLikelihood[1];  
            double PionLklh = PIDLikelihood[3];  
            double norm = ElLklh+PionLklh;
            ElLklh /= norm; 
            PionLklh /= norm;

            if( PionLklh > 0.8 ){ // Id associated to the largest of the two probabilities.
                ccmultipibox->NegativePionTPCtracks[ccmultipibox->nNegativePionTPCtracks] = ptrack;
                ccmultipibox->nNegativePionTPCtracks++;
            }
            else{ 
        //  if ( ptrack->SmearedMomentum <100. ) {

          //  ccmultipibox->ElPi0TPCtracks[ccmultipibox->nElPi0TPCtracks] = ptrack;
           // ccmultipibox->nElPi0TPCtracks++;
         // }
        }
        }
      }
    }
   
}

/*
//*********************************************************************
void numuCCMultiPiUtils::FindIsoTargetPions(AnaEventC& event, ToyBoxB& box, SubDetId::SubDetEnum det){
//*********************************************************************

    ToyBoxCCMultiPi * ccmultipibox = static_cast<ToyBoxCCMultiPi*>(&box);
    ccmultipibox->nIsoTargetPiontracks = 0;
    ccmultipibox->nIsoTargetElPi0tracks = 0;

    EventBox::RecObjectGroupEnum groupID;
    if      (det==SubDetId::kTarget1) groupID = EventBox::kTracksWithTarget1AndNoTPC;
    else if (det==SubDetId::kTarget2) groupID = EventBox::kTracksWithTarget2AndNoTPC;
    else return;

    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];

    //loop over tracks
    for (int i=0;i<EventBox->nRecObjectsInGroup[groupID]; i++ ){
        AnaTrackB* track = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);
        // Apply the fiducial cut and PID
        if( numuCCMultiPiUtils::TargetpionSelection(track,det) ){ 
            ccmultipibox->IsoTargetPiontracks[ccmultipibox->nIsoTargetPiontracks] = track;
            ccmultipibox->nIsoTargetPiontracks++;
        }
        else if( numuCCMultiPiUtils::TargetElPi0Selection(event,box,track,det) ){
            ccmultipibox->IsoTargetElPi0tracks[ccmultipibox->nIsoTargetElPi0tracks] = track;
            ccmultipibox->nIsoTargetElPi0tracks++;
        }
    }
}

//*********************************************************************
void numuCCMultiPiUtils::FindMEPions(AnaEventC& eventBB, SubDetId::SubDetEnum det, bool prod5Cut){
//*********************************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventBB); 

  EventBox* EventBox = static_cast<EventBox*>(event.EventBoxes[EventBoxId::kEventBoxNDUP]);

  // Fill the box only the first time this is used
  if (EventBox->TargetMichelElectrons[det]) return;

  anaUtils::CreateArray(EventBox->TargetMichelElectrons[det], NMAXTargetTIMEBINS);
  EventBox->nTargetMichelElectrons[det] = anaUtils::GetTargetMichelElectrons(event, det, EventBox->TargetMichelElectrons[det], prod5Cut);
  anaUtils::ResizeArray(EventBox->TargetMichelElectrons[det], EventBox->nTargetMichelElectrons[det], NMAXTargetTIMEBINS);

}
*/
//*********************************************************************
bool numuCCMultiPiUtils::TPCpionSelection(AnaTrackB *track){
//*********************************************************************

    Float_t PIDLikelihood[4];
    anaUtils::GetPIDLikelihood(*track, PIDLikelihood);
  
    if ( PIDLikelihood[3] < 0.3  ) return false; 

    double cut1 = (PIDLikelihood[0]+PIDLikelihood[3])/(1.-PIDLikelihood[2]); 

    if( track->Momentum < 500.  && cut1 < 0.8 ) return false;

    return true;


}
//*********************************************************************
bool numuCCMultiPiUtils::TPCElPi0Selection(AnaTrackB *track){
//*********************************************************************

    if( track->nTPCSegments == 0 ) return false; // There is no TPC segment

    bool seltrack = false; 

    //    if (!cutUtils::TrackQualityCut(*track)) return seltrack;

    if( track->Momentum < 50. ) return seltrack; 

    //  if( track->Charge > 0. && track->Momentum > 800.) return seltrack; 

    Float_t pulls[4];

    for(int i = 0; i < track->nTPCSegments ; i++ ){

        AnaTPCParticleB *tpcTrack = track->TPCSegments[i];

        if( !tpcTrack  ) continue; // Extra protection.

        // Pulls are: Muon, Electron, Proton, Pion
        anaUtils::ComputeTPCPull(*tpcTrack,pulls);

        if (TMath::Abs(pulls[0]) > 1.e+6 // muon pull
                || TMath::Abs(pulls[1]) > 1.e+6 // electron pull
                || TMath::Abs(pulls[2]) > 1.e+6 // proton pull
                || TMath::Abs(pulls[3]) > 1.e+6) continue; // pion pull

        if( pulls[1] < -2.0 || pulls[1] > 2.0  ) break;

        if( track->Charge > 0. &&  ( pulls[2] > -4.0 && pulls[2] < 8.0 )  ) break;

        seltrack = true; 

    }
    return seltrack;
}
/*
//*********************************************************************
bool numuCCMultiPiUtils::TargetpionSelection(AnaTrackB *track, SubDetId::SubDetEnum det){
//*********************************************************************
    if( track->nTargetSegments != 1 ) return false; // There is no Target segment or it has more than one Target 

    AnaTargetParticleB *TargetTrack = track->TargetSegments[0];

    if( !TargetTrack  ) return false; // Extra protection. 

    if( TMath::Abs(TargetTrack->Pullp) > 1.e+6 || TMath::Abs(TargetTrack->Pullmu) > 1.e+6 || TMath::Abs(TargetTrack->Pullpi) > 1.e+6  ) return false; 

    // Check the pullnotset variable. 
    // This tells us if the pull was calculated by the algorithm. 
    // If it was not calculated this variable will be 1, so for calculated pulls Targetpulls!=1. 
    if(  TargetTrack->Pullno == 1 ) return false;

    // This tell us something about the geometry. 
    // If the algorithm says the particle started and stopped in 1st/ 2nd Target this is equal 1 or 2. 
    //  If the particle stopped in 1st/2nd Target, this is equal -1/-2. Other cases: 0.
    if( TargetTrack->Containment == 0 ) return false;
    if     ( det == SubDetId::kTarget1 && TargetTrack->Containment != 1 ) return false;
    else if( det == SubDetId::kTarget2 && TargetTrack->Containment != 2 ) return false; 
    else if( det == SubDetId::kTarget  && TargetTrack->Containment != 1 && TargetTrack->Containment != 2) return false; 

    double cosTargetpion = TargetTrack->DirectionStart[2];

    if(cosTargetpion > -0.3 && cosTargetpion < 0.3) return false;/// only for systematics issues. Must be remove in the future!!

    if( TargetTrack->Pullpi < 2.5 && TargetTrack->Pullpi > -2.0 ) return true; 

    return false;


}

//*********************************************************************
bool numuCCMultiPiUtils::TargetElPi0Selection(AnaEventC& event, ToyBoxB& box, AnaTrackB *track, SubDetId::SubDetEnum det){
//*********************************************************************

  (void)box;

    if( track->nTargetSegments != 1 ) return false; // There is no Target segment or it has more than one Target 

    //    ToyBoxCCMultiPi * ccmultipibox = static_cast<ToyBoxCCMultiPi*>(&box);

    AnaTargetParticleB *TargetTrack = track->TargetSegments[0];

    if( !TargetTrack  ) return false; // Extra protection. 

    if( TMath::Abs(TargetTrack->Pullp) > 1.e+6 || TMath::Abs(TargetTrack->Pullmu) > 1.e+6 || TMath::Abs(TargetTrack->Pullpi) > 1.e+6  ) return false; 

    // Check the pullnotset variable. 
    // This tells us if the pull was calculated by the algorithm. 
    // If it was not calculated this variable will be 1, so for calculated pulls Targetpulls!=1. 
    if(  TargetTrack->Pullno == 1 ) return false;

    // This tell us something about the geometry. 
    // If the algorithm says the particle started and stopped in 1st/ 2nd Target this is equal 1 or 2. 
    //  If the particle stopped in 1st/2nd Target, this is equal -1/-2. Other cases: 0.
    if( TargetTrack->Containment == 0 ) return false; 
    if     ( det == SubDetId::kTarget1 && TargetTrack->Containment != 1 ) return false; 
    else if( det == SubDetId::kTarget2 && TargetTrack->Containment != 2 ) return false; 
    else if( det == SubDetId::kTarget  && TargetTrack->Containment != 1 && TargetTrack->Containment != 2) return false; 

    EventBox* EventBox = static_cast<EventBox*>(event.EventBoxes[EventBoxId::kEventBoxNDUP]);

    if(EventBox->nTargetMichelElectrons[det] > 0  && TargetTrack->Pullpi < -3.0 ) return true;
    if(EventBox->nTargetMichelElectrons[det] == 0 && TargetTrack->Pullpi < -1.5 ) return true;


    return false;
}
*/
//**************************************************
bool numuCCMultiPiSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************

 // return _numuCCSelection.IsRelevantRecObjectForSystematic(event,track,systId,branch);
return true;
}

//**************************************************
bool numuCCMultiPiSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueTrack, SystId_h systId, Int_t branch) const{
//**************************************************

//  return _numuCCSelection.IsRelevantTrueObjectForSystematic(event,trueTrack,systId,branch);
return true;
}

//**************************************************
bool numuCCMultiPiSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaRecObjectC* track, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;

  // all CC inclusive cases
  // APPLY_SYST_FINE_TUNING is taken into account as well (will return true if not used), hence in what follows assume
  // APPLY_SYST_FINE_TUNING case ON
  //if (_numuCCSelection.IsRelevantRecObjectForSystematicInToy(event,box,track,systId,branch)) return true;
  
  const ToyBoxCCMultiPi* ccmultipibox = static_cast<const ToyBoxCCMultiPi*>(&box);
  
  switch (systId){
    // Fall back through the relevant ones
    // TPC reco
    case SystId::kChargeIDEff:
    // TPC matching
    case SystId::kTpcTargetMatchEff:
      
      // Use the APPLY_SYST_FINE_TUNING concept as well,  since we worry about the "main" track that defines the topology 
      if (branch == 1){
        // For CC-1pi also the positive Pion in the TPC matters
        if (ccmultipibox->nPositivePionTPCtracks==1 && track==ccmultipibox->PositivePionTPCtracks[0]) return true; 
      }
      else if (branch == 2){
      // For CC-Other consider the negative pion when there are no other pions
      if (ccmultipibox->nNegativePionTPCtracks==1 && track==ccmultipibox->NegativePionTPCtracks[0] && 
          ccmultipibox->nOtherPions==1 && ccmultipibox->nPosPions == 0) return true;  
    
      }
      // Failed above + 
      // CC0pi explicit false (should have been covered by numuCC if tuning is ON)
      return false;
      break;
      
    // TPC cluster,  consider  a potential HMN track,  can affect the selection  
    case SystId::kTpcClusterEff:
      // Rare case it can affect,  need to consider PID as well,  apply for the moment...
      return true;
      break;
    
    // The rest of the systematics go here, tuning to come
    default:
      return true;
      break;
  }
  
  return true;
  
}

  
//**************************************************
bool numuCCMultiPiSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& box, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;

  AnaTrueParticleB* trueTrack = static_cast<AnaTrueParticleB*>(trueObj);

  // all CC inclusive cases
  // MAIN track mode is taken into account there
//  if (_numuCCSelection.IsRelevantTrueObjectForSystematicInToy(event,box,trueTrack,systId,branch)) return true;
  
  if(systId == SystId::kTpcTrackEff){
    const ToyBoxCCMultiPi* ccmultipibox = static_cast<const ToyBoxCCMultiPi*>(&box);

    if (branch==0 || branch==1){
      // For CC-0pi and CC-1pi all true pions and muons matter
      if (abs(trueTrack->PDG)==211 || abs(trueTrack->PDG)==13) return true; 
    }
    else if (branch==2){
      // Don't consider pions when there are many

      // cases in which we must loose two or more pions are excluded (0.01*0.01 probability)
      if (ccmultipibox->nNegativePionTPCtracks+ccmultipibox->nPositivePionTPCtracks>2 || ccmultipibox->nNegativePionTPCtracks>1) return false; 

      // If the are few pions consider them. Also true muons
      if (abs(trueTrack->PDG)==211 || abs(trueTrack->PDG)==13) return true; 
    }
    return false;
  } 

  // For the rest of the systematic return true, tuning to come
  return true;
}

//**************************************************
bool numuCCMultiPiSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const{
//**************************************************

  (void)event;
  (void)box;
  (void)systId;
  (void)branch;

  /* THIS IS JUST AN EXAMPLE. DO NOT TAKE SERIOUSLY !!!! */

  // CC-Other branch
  /*  if (branch==2){
    if      (systId==SystId::kTargetTrackEff)   // No Target track eff
      return false;
    else if (systId==SystId::kTargetHybridTrackEff)  // No Target hybrid track eff
      return false;
    else if (systId==SystId::kMichelEleEff)  // No michel electron Systematic
      return false;
    else if (systId==SystId::kSIPion){  // No Pion SI systematic when there are many pions 
      const ToyBoxCCMultiPi* box2 = static_cast<const ToyBoxCCMultiPi*>(&box);
      if (box2->nOtherPions >4 ) return false; 
      if (box2->nPosPions > 4 ) return false; 
    }
  }
  */
/*
  if (branch==2){
    if (systId==SystId::kSIPion){  
      // No Pion SI systematic when there are many pions 
      if (!systTuning::APPLY_SYST_FINE_TUNING) return true;
      const ToyBoxCCMultiPi* box2 = static_cast<const ToyBoxCCMultiPi*>(&box);
      if (box2->nOtherPions >1 || box2->nPosPions > 2 ) return false; 
    }
  }
*/
  return true;
}

//**************************************************
void numuCCMultiPiSelection::InitializeEvent(AnaEventC& eventBB){
//**************************************************

  AnaEventB& event = *static_cast<AnaEventB*>(&eventBB); 

  if (!event.EventBoxes[EventBoxId::kEventBoxNDUP])
    event.EventBoxes[EventBoxId::kEventBoxNDUP] = new EventBox();


  boxUtils::FillTracksWithTPC(event,             static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTracksWithTarget(event,             static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  boxUtils::FillTrajsChargedInTPC(event);
  boxUtils::FillTrajsChargedInTargetAndNoTPC(event, static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));

 // _numuCCSelection.InitializeEvent(event);
  ///boxUtils::FillTracksWithECal(event);
}

//********************************************************************
bool numuCCMultiPiSelection::CheckRedoSelection(const AnaEventC& event, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep){
//********************************************************************

  // Must redo selection if numuCCSelection decides so
  if( _numuCCSelection.CheckRedoSelection(event,PreviousToyBox,redoFromStep)) return true;

  // Otherwise selection must be redone From the FindPionsAction, but only when the Muon PID cut is passed in the previous toy 
  if (PreviousToyBox.MaxAccumLevel>_MuonPIDCutIndex){
    redoFromStep=_FindPionsStepIndex;
    return true;
  }
  return false;
}
/*
//*********************************************************************
bool FindEcalPhotonsAction::Apply( AnaEventC& event, ToyBoxB& box) const{
//*********************************************************************
    // Find whether there is a pi0 track in the ECal or not 
    // In order to apply a pi0 veto cut later 
    ToyBoxCCMultiPi * ccmultipibox = static_cast<ToyBoxCCMultiPi*>(&box);
    SubDetId::SubDetEnum det = static_cast<SubDetId::SubDetEnum>(box.DetectorFV);

    // Get all Ecal objects
    EventBox::RecObjectGroupEnum groupID;
    groupID = EventBox::kTracksWithECal;
    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];
    
    AnaTrackB *highestObject = NULL;
    double higherEnergyObject = 0;
  
    ccmultipibox->nIsoEcals      = 0;
    ccmultipibox->nPi0Ecaltracks = 0;

    //loop over all ECal objects
    for(int i = 0; i < EventBox->nRecObjectsInGroup[groupID]; i++ ) {
        AnaTrackB *allECALObjects = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[groupID][i]);
        if(!(allECALObjects->nECALSegments)) continue;
        // Check for isolated ecal object
        if(!(anaUtils::TrackUsesOnlyDet(*allECALObjects,SubDetId::kDSECAL) ||
             anaUtils::TrackUsesOnlyDet(*allECALObjects,SubDetId::kTopTECAL) ||
             anaUtils::TrackUsesOnlyDet(*allECALObjects,SubDetId::kBottomTECAL) ||
             anaUtils::TrackUsesOnlyDet(*allECALObjects,SubDetId::kLeftTECAL) ||
             anaUtils::TrackUsesOnlyDet(*allECALObjects,SubDetId::kRightTECAL))
            )  continue;

        AnaECALParticleB* ecalBase = allECALObjects->ECALSegments[0];    
        AnaTrackB* object =  allECALObjects; 
        ccmultipibox->IsoEcals[ccmultipibox->nIsoEcals++] = object;   
        
        // Find the most energetic object in ECal 
        if(ecalBase->EMEnergy > higherEnergyObject){
            higherEnergyObject = ecalBase->EMEnergy;
            highestObject = object;
        } 
    }// end of loop on allECALTracks

    // Save the highest energetic ecal object in the box and the pi0 track
    if (highestObject){
        ccmultipibox->HObject  = highestObject;
        // Check if the highest energetic ecal object is shower-like
        if (cutUtils::IsECALPi0Photon(event, highestObject, _MostUpstreamLayerHitCut, det)) {
            ccmultipibox->Pi0Ecaltrack  = highestObject;
            ccmultipibox->nPi0Ecaltracks = 1; 
        }
    }  
    return true;
}

//*********************************************************************
bool EcalPi0VetoCut::Apply( AnaEventC& event, ToyBoxB& box) const{
//*********************************************************************

    (void)event;

    // Select event without pi0 track (see FindEcalPhotonsAction)
    ToyBoxCCMultiPi * ccmultipibox = static_cast<ToyBoxCCMultiPi*>(&box);
    if(ccmultipibox->nPi0Ecaltracks > 0) return false;
    else return true;
}
*/