#include "trueEventBox.hxx"
#include "CutUtils.hxx"
//********************************************************************
trueEventBox::trueEventBox():EventBoxB(){
//******************************************************************** 

}

//********************************************************************
trueEventBox::~trueEventBox(){
//********************************************************************

} 

//********************************************************************
void trueboxUtils::FillLongTracks(AnaEventB& event, SubDetId::SubDetEnum det){
//********************************************************************

  EventBoxB* trueEventBox = event.EventBoxes[EventBoxId::ktrueEventBoxNDUP];

  // TODO. We need a better method to get the tracks (returning a variable size array). Otherwise we have to guess how many tracks we have
  AnaTrackB* selTracks[NMAXPARTICLES*10];
  int nLong =anaUtils::GetAllTracks(event,selTracks);
  //nLong = std::min(nLong, (Int_t)NMAXPARTICLES);

  trueEventBox->nRecObjectsInGroup[trueEventBox::kLongTracks]=0;
  anaUtils::CreateArray(trueEventBox->RecObjectsInGroup[trueEventBox::kLongTracks], nLong);

  trueEventBox->nRecObjectsInGroup[trueEventBox::kLongTracksInFV]=0;
  anaUtils::CreateArray(trueEventBox->RecObjectsInGroup[trueEventBox::kLongTracksInFV], nLong);


  //loop over fgd tracks
  for (Int_t i=0;i<nLong; ++i){
    AnaParticleB* track = selTracks[i];
    if(track){

      trueEventBox->RecObjectsInGroup[trueEventBox::kLongTracks][trueEventBox->nRecObjectsInGroup[trueEventBox::kLongTracks]++] = track;
      if (cutUtils::FiducialCut(*track, det))
        trueEventBox->RecObjectsInGroup[trueEventBox::kLongTracksInFV][trueEventBox->nRecObjectsInGroup[trueEventBox::kLongTracksInFV]++] = track;
    }
  }

  anaUtils::ResizeArray(trueEventBox->RecObjectsInGroup [trueEventBox::kLongTracks],
  trueEventBox->nRecObjectsInGroup[trueEventBox::kLongTracks]);

  anaUtils::ResizeArray(trueEventBox->RecObjectsInGroup [trueEventBox::kLongTracksInFV],
  trueEventBox->nRecObjectsInGroup[trueEventBox::kLongTracksInFV]);
 }

