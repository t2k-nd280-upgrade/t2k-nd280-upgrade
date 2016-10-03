#include "SelectionUtils.hxx"
#include "EventBox.hxx"
#include "ToyBoxNDUP.hxx"

//********************************************************************
bool SelUtils::FindLeadingTracks(AnaEventC& eventBB, ToyBoxB& boxB){
//********************************************************************

    // Get the highest momentum track (HM),
    // the highest momentum negative track (HMN) and the 
    // highest momentum positive track (HMP)
    // from all tracks passing the quality and fiducial cut. 

    // this function cannot be in anaUtils because it needs the cuts

    // Cast the ToyBox to the appropriate type
    ToyBoxNDUP& box = *static_cast<ToyBoxNDUP*>(&boxB); 
    AnaEventB& event = *static_cast<AnaEventB*>(&eventBB); 

    //    SubDetId::SubDetEnum det = static_cast<SubDetId::SubDetEnum>(box.DetectorFV);

    EventBox::RecObjectGroupEnum groupID = EventBox::kLongTracks;

    // Retrieve the EventBoxDUNE
    EventBoxB* EventBoxNDUP = event.EventBoxes[EventBoxId::kEventBoxNDUP];

    AnaRecObjectC** selTracks = EventBoxNDUP->RecObjectsInGroup[groupID];
    int nTracks=EventBoxNDUP->nRecObjectsInGroup[groupID];
    // Reset to NULL all pointers
//    std::cout<<nTracks<<std::endl;
    box.HMT = box.HPlusPionT = box.HProtonT =box.HMinusPionT = NULL;

    Float_t mom_delta=0;
    Float_t pmmom_delta=0;
    Float_t protonmom_delta=0;
    Float_t ppmom_delta=0;

    for (Int_t i=0;i<event.nVertices;i++){
  
      AnaVertexB* vertex = static_cast<AnaVertexB*>(event.Vertices[i]);
      box.Vertex=vertex->Clone();
    }
   
  for (Int_t i = 0; i < nTracks; ++i) {
    AnaTrack* track = static_cast<AnaTrack*>(selTracks[i]);
    //     std::cout<<"track->Length"<<track->Length<<std::endl;
    if (track->GetTrueParticle()->ParentID == 0) {
      // Find the Longest and second longest tracks
      if (track->GetTrueParticle()->PDG == 13) {
        if (track->Momentum > mom_delta || !box.HMT) {
          // if (box.Ltrack){
          // box.SLtrack = box.Ltrack;
          // l_delta=track->Length;

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
