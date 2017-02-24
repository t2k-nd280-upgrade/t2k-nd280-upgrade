#include "EventBox.hxx"
#include "CutUtils.hxx"
//********************************************************************
EventBox::EventBox():EventBoxB(){
//******************************************************************** 

}

//********************************************************************
EventBox::~EventBox(){
//********************************************************************

} 

//********************************************************************
void boxUtils::FillLongTracks(AnaEventB& event, SubDetId::SubDetEnum det){
//********************************************************************

  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];

  // TODO. We need a better method to get the tracks (returning a variable size array). Otherwise we have to guess how many tracks we have
  AnaTrackB* selTracks[NMAXPARTICLES*5];
  int nLong =anaUtils::GetAllTracks(event,selTracks);
  //nLong = std::min(nLong, (Int_t)NMAXPARTICLES);

  EventBox->nRecObjectsInGroup[EventBox::kLongTracks]=0;
  anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBox::kLongTracks], nLong);

  EventBox->nRecObjectsInGroup[EventBox::kLongTracksInFV]=0;
  anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBox::kLongTracksInFV], nLong);


  //loop over fgd tracks
  for (Int_t i=0;i<nLong; ++i){
    AnaParticleB* track = selTracks[i];
    if(track){

      EventBox->RecObjectsInGroup[EventBox::kLongTracks][EventBox->nRecObjectsInGroup[EventBox::kLongTracks]++] = track;
      if (cutUtils::FiducialCut(*track, det))
        EventBox->RecObjectsInGroup[EventBox::kLongTracksInFV][EventBox->nRecObjectsInGroup[EventBox::kLongTracksInFV]++] = track;
    }
  }

  anaUtils::ResizeArray(EventBox->RecObjectsInGroup [EventBox::kLongTracks],
			EventBox->nRecObjectsInGroup[EventBox::kLongTracks]);

  anaUtils::ResizeArray(EventBox->RecObjectsInGroup [EventBox::kLongTracksInFV],
			EventBox->nRecObjectsInGroup[EventBox::kLongTracksInFV]);
}

