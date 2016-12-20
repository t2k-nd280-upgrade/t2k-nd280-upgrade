#include "SelectionUtils.hxx"
#include "EventBox.hxx"
#include "ToyBoxNDUP.hxx"

//********************************************************************
bool SelUtils::FindLeadingTracks(AnaEventC& event, ToyBoxB& boxB){
//********************************************************************

    // Get the highest momentum track (HM),
    // the highest momentum negative track (HMN) and the 
    // highest momentum positive track (HMP)
    // from all tracks passing the quality and fiducial cut. 

    // this function cannot be in anaUtils because it needs the cuts

    // Cast the ToyBox to the appropriate type
    ToyBoxNDUP& box = *static_cast<ToyBoxNDUP*>(&boxB); 

    SubDetId::SubDetEnum det = static_cast<SubDetId::SubDetEnum>(box.DetectorFV);

    EventBox::RecObjectGroupEnum groupID;
    if   (det == SubDetId::kTarget1)
      groupID = EventBox::kTracksWithTPCAndTarget1;
    else if (det == SubDetId::kTarget2)
      groupID = EventBox::kTracksWithTPCAndTarget2;
    else 
      return false;

    // Retrieve the EventBoxNDUP
    EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];
    AnaRecObjectC** selTracks = EventBox->RecObjectsInGroup[groupID];
    int nTPC=EventBox->nRecObjectsInGroup[groupID];
  
    box.nNegativeTPCtracks=0;
//    if (box.NegativeTPCtracks) delete [] box.NegativeTPCtracks;
 
    anaUtils::CreateArray(box.NegativeTPCtracks, nTPC);

    box.nPositiveTPCtracks=0;
  //  if (box.PositiveTPCtracks) delete [] box.PositiveTPCtracks;
    anaUtils::CreateArray(box.PositiveTPCtracks, nTPC);

    // Reset to NULL all pointers
    box.HMNtrack = box.HMPtrack = box.SHMNtrack = box.SHMPtrack = box.SHMtrack = box.HMtrack = NULL;

    Float_t hm_mom=0;
    Float_t shm_mom=0;
    Float_t hmn_mom=0;
    Float_t shmn_mom=0;
    Float_t hmp_mom=0;
    Float_t shmp_mom=0;

    for (Int_t i=0;i<nTPC; ++i){
      AnaTrackB* track = dynamic_cast<AnaTrackB*>(selTracks[i]);
      // Find the HM and SHM tracks
      if (track->Momentum > hm_mom){

        if (box.HMtrack){
          box.SHMtrack = box.HMtrack;
          shm_mom= hm_mom;
        }
        box.HMtrack = track;
        hm_mom= track->Momentum;
      }
      else if (track->Momentum >shm_mom || !box.SHMtrack){
        box.SHMtrack = track;
        shm_mom= track->Momentum;
      }
      
      // Find the HMN and SHMN tracks
      if (track->Charge == -1){
        box.NegativeTPCtracks[box.nNegativeTPCtracks++] = track;
        
        if (track->Momentum > hmn_mom){
          if (box.HMNtrack){
            box.SHMNtrack = box.HMNtrack;
            shmn_mom= hmn_mom;
          }
          box.HMNtrack = track;
          hmn_mom= track->Momentum;
          
        }
        else if (track->Momentum >shmn_mom){
          box.SHMNtrack = track;
          shmn_mom= track->Momentum;
        }
      }
      // Find the HMP and SHMP tracks
      else if (track->Charge == +1){
        box.PositiveTPCtracks[box.nPositiveTPCtracks++] = track;
        
        if (track->Momentum > hmp_mom){
          if (box.HMPtrack){
            box.SHMPtrack = box.HMPtrack;
            shmp_mom= hmp_mom;
          }
          box.HMPtrack = track;
          hmp_mom= track->Momentum;
        }
        else if (track->Momentum >shmp_mom || !box.SHMPtrack){
          box.SHMPtrack = track;
          shmp_mom= track->Momentum;
        }
      }
    }

    // Resize the arrays
    anaUtils::ResizeArray(box.NegativeTPCtracks, box.nNegativeTPCtracks);
    anaUtils::ResizeArray(box.PositiveTPCtracks, box.nPositiveTPCtracks);
     return (box.HMtrack);  
}
