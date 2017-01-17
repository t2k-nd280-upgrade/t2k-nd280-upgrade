#include "trueSelectionUtils.hxx"
#include "trueEventBox.hxx"
#include "trueToyBoxNDUP.hxx"

//********************************************************************
bool trueSelUtils::FindLeadingTracks(AnaEventC& eventBB, ToyBoxB& boxB){
//********************************************************************

    // Get the highest momentum track (HM),
    // the highest momentum negative track (HMN) and the 
    // highest momentum positive track (HMP)
    // from all tracks passing the quality and fiducial cut. 

    // this function cannot be in anaUtils because it needs the cuts

    // Cast the ToyBox to the appropriate type
    trueToyBoxNDUP& box = *static_cast<trueToyBoxNDUP*>(&boxB); 
    AnaEventB& event = *static_cast<AnaEventB*>(&eventBB); 

    //    SubDetId::SubDetEnum det = static_cast<SubDetId::SubDetEnum>(box.DetectorFV);

    trueEventBox::RecObjectGroupEnum groupID = trueEventBox::kLongTracks;

    // Retrieve the EventBoxDUNE
    EventBoxB* trueEventBoxNDUP = event.EventBoxes[EventBoxId::ktrueEventBoxNDUP];

    AnaRecObjectC** selTracks = trueEventBoxNDUP->RecObjectsInGroup[groupID];
    int nTracks=trueEventBoxNDUP->nRecObjectsInGroup[groupID];
    // Reset to NULL all pointers
    box.HMT = box.HPlusPionT = box.HProtonT =box.HMinusPionT = NULL;

    Float_t mom_delta=0;
    Float_t pmmom_delta=0;
    Float_t protonmom_delta=0;
    Float_t ppmom_delta=0;

    for (Int_t i=0;i<event.nVertices;i++){
  
      AnaVertexB* vertex = static_cast<AnaVertexB*>(event.Vertices[i]);
      box.Vertex=vertex->Clone();
    }
   std::cout<<"nTracks "<<nTracks<<std::endl;
  for (Int_t i = 0; i < nTracks; ++i) {
    AnaTrack* track = static_cast<AnaTrack*>(selTracks[i]);
    if (track->GetTrueParticle()->ParentID == 0) {
      // Find the Longest and second longest tracks
      if (track->GetTrueParticle()->PDG == 13) {
        if (track->Momentum > mom_delta || !box.HMT) {

          //}
          box.HMT = track;
          mom_delta = track->Momentum;
        }
      } else      if (track->GetTrueParticle()->PDG == -211) {

        if (track->Momentum > pmmom_delta || !box.HMinusPionT) {
          box.HMinusPionT = track;
          pmmom_delta = track->Momentum;
        }
      }  else      if (track->GetTrueParticle()->PDG == 2212) {

        if (track->Momentum > protonmom_delta || !box.HProtonT) {
          box.HProtonT = track;
          protonmom_delta = track->Momentum;
        }
      } else      if (track->GetTrueParticle()->PDG == 211) {

        if (track->Momentum > ppmom_delta || !box.HPlusPionT) {
          box.HPlusPionT = track;
          ppmom_delta = track->Momentum;
        }
      }
    }
  }

//    box.Ltrack->Print();
    return (box.HMT);  
}
