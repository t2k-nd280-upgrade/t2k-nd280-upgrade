#include "SubDetUtils.hxx"


//**************************************************
Int_t anaUtils::GetNSegmentsInDet(const AnaTrackB& track, SubDetId::SubDetEnum det){
//**************************************************
    // for a given track compute the number of segments in a given detector
    return SubDetId::GetNSegmentsInDet(track.Detectors, det);
}

//**************************************************
int anaUtils::GetAllTrajInBunch(const AnaEventB& event, AnaTrueParticleB* trajInBunch[]) {
//**************************************************
    int count = 0;
    for(Int_t i=0;i< event.nTrueParticles;i++){
      if(!event.TrueParticles[i]->TrueVertex) continue;
      if(event.TrueParticles[i]->TrueVertex->Bunch!=event.Bunch) continue;
      trajInBunch[count] = event.TrueParticles[i];
      ++count;
    }
    return count;
}


//**************************************************
int anaUtils::GetAllChargedTrajInBunch(const AnaEventB& event, AnaTrueParticleB* chargedtrajInBunch[]) {
//**************************************************
    int count = 0;
    for(Int_t i=0;i< event.nTrueParticles;i++){
      if(!event.TrueParticles[i]->TrueVertex) continue;
      if(event.TrueParticles[i]->TrueVertex->Bunch!=event.Bunch) continue;
      
      if(event.TrueParticles[i]->Charge!=0){
        chargedtrajInBunch[count] = event.TrueParticles[i];
        ++count;
      }
    }
    return count;
}

//**************************************************
bool anaUtils::HasTrackUsingDet(const AnaEventB& event, SubDetId::SubDetEnum det) {
//**************************************************

    for (int it = 0; it < event.nParticles; it++) {
        AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[it]);
        if (SubDetId::GetDetectorUsed(track->Detectors, det)){
            return true;
        }
    }

    return false;
}

//**************************************************
int anaUtils::GetAllTracksUsingDet(const AnaEventB& event, SubDetId::SubDetEnum det, AnaTrackB* selTracks[]) {
//**************************************************

    int count = 0;
    for (int it = 0; it < event.nParticles; it++) {
        AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[it]);
        if (SubDetId::GetDetectorUsed(track->Detectors, det)){
            selTracks[count] = track;
            count++;
        }
    }

    return count;
}
//**************************************************
int anaUtils::GetAllTracks(const AnaEventB& event, AnaTrackB* selTracks[]) {
//**************************************************

    int count = 0;
    for (int it = 0; it < event.nParticles; it++) {
        AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[it]);
//        if (SubDetId::GetDetectorUsed(track->Detector, det)){
            selTracks[count] = track;
            count++;
  //      }
    }

    return count;
}

//**************************************************
int anaUtils::GetAllTracksUsingDet(const AnaEventB& event, SubDetId::SubDetEnum det, std::vector<AnaTrackB*>& selTracks) {
//**************************************************

    int count = 0;
    for (int it = 0; it < event.nParticles; it++) {
        AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[it]);
        if (SubDetId::GetDetectorUsed(track->Detectors, det)){
            selTracks.push_back(track);
            count++;
        }
    }
    return count;
}

//**************************************************
int anaUtils::GetAllTracksNotUsingDet(const AnaEventB& event, SubDetId::SubDetEnum det, AnaTrackB* selTracks[]) {
//**************************************************

    int count = 0;

    for (int it = 0; it < event.nParticles; it++) {
        AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[it]);
        if (!SubDetId::GetDetectorUsed(track->Detectors, det)) {
            selTracks[count] = track;
            ++count;
        }
    }

    return count;
}

//**************************************************
int anaUtils::GetAllTracksUsingOnlyDet(const AnaEventB& event, SubDetId::SubDetEnum det, AnaTrackB* selTracks[]) {
//**************************************************

    int count = 0;

    for (int it = 0; it < event.nParticles; it++) {
        AnaTrackB* track = static_cast<AnaTrackB*>(event.Particles[it]);
        if (TrackUsesOnlyDet(*track, det)) {
            selTracks[count] = track;
            ++count;
        }
    }

    return count;
}


//**************************************************
bool anaUtils::TrackUsesDets(const AnaTrackB& track, SubDetId::SubDetEnum dets[], int nDets) {
//**************************************************
    return SubDetId::GetDetectorArrayUsed(track.Detectors, dets, nDets);
}

//**************************************************
bool anaUtils::TrackUsesDet(const AnaTrackB& track, SubDetId::SubDetEnum det) {
//**************************************************
    return SubDetId::GetDetectorUsed(track.Detectors, det);
}

//**************************************************
bool anaUtils::TrackUsesDet(const AnaParticleB& track, SubDetId::SubDetEnum det) {
//**************************************************
    return SubDetId::GetDetectorUsed(track.Detectors, det);
}

//**************************************************
bool anaUtils::TrackUsesOnlyDet(const AnaTrackB& track, SubDetId::SubDetEnum det) {
//**************************************************
    return SubDetId::TrackUsesOnlyDet(track.Detectors, det);
}

//************************************************** 
bool anaUtils::TrackUsesOnlyDet(const AnaParticleB& track, SubDetId::SubDetEnum det) {
//**************************************************
    return SubDetId::TrackUsesOnlyDet(track.Detectors, det);
}

