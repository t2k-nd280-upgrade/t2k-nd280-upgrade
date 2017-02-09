#include "EventBoxUtils.hxx"
#include "CutUtils.hxx"
#include "ND280UPAnalysisUtils.hxx"

//********************************************************************
void boxUtils::FillTracksWithTPC(AnaEventB& event, SubDetId::SubDetEnum det){
//********************************************************************

  bool processTarget1 = false;
  bool processTarget2 = false;
  bool processTPC  = false;

  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];

  // Don't fill it when already filled by other selection
  if ((det==SubDetId::kTarget1 || det==SubDetId::kTarget) && !EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCAndTarget1]) processTarget1 = true;
  if ((det==SubDetId::kTarget2 || det==SubDetId::kTarget) && !EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCAndTarget2]) processTarget2 = true;

  if (!processTarget1 && !processTarget2) return;

  AnaTrackB* selTracks[NMAXPARTICLES];
  int nTPC = anaUtils::GetAllTracksUsingTPC(event, selTracks);
  if (!EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPC]){
    processTPC= true;
    EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPC]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTPC], nTPC);
  }
  if (processTarget1){
    EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCInTarget1FV]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCInTarget1FV], nTPC);

    EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV], nTPC);

    EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCAndTarget1]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCAndTarget1], nTPC);
    if(!EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCorTarget1]){
      EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCorTarget1]=0;
      anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCorTarget1], nTPC);
    }else{
      anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCorTarget1], 
                         EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCorTarget1]+nTPC);
    }

  }
  if (processTarget2){
    EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCInTarget2FV]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCInTarget2FV], nTPC);

    EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV], nTPC);

    EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCAndTarget2]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCAndTarget2], nTPC);
    if(!EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCorTarget2]){
      EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCorTarget2]=0;
      anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCorTarget2], nTPC);
    }else{
      anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCorTarget2], 
                         EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCorTarget2]+nTPC);
    }
  }

  
  bool inTarget1 = false;
  bool inTarget2 = false;

  //loop over tpc tracks
  for (Int_t i=0;i<nTPC; ++i){
    AnaTrackB* track = selTracks[i];

///    if (track->nTPCSegments==0){
   //   std::cout << "Warning. This track has no TPC segments" << std::endl;
    //  continue;
   // }
    if (processTPC){
      EventBox->RecObjectsInGroup[EventBox::kTracksWithTPC][EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPC]++] = track;

      if(processTarget1)
        EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCorTarget1][EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCorTarget1]++] = track;
      if(processTarget2)
        EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCorTarget2][EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCorTarget2]++] = track;
    }
    // Does it have Target as well ? here not...
    if (processTarget1 && anaUtils::TrackUsesDet(*track, SubDetId::kTarget1))
      EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCAndTarget1][EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCAndTarget1]++] = track;

    if (processTarget2 && anaUtils::TrackUsesDet(*track, SubDetId::kTarget2))
      EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCAndTarget2][EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCAndTarget2]++] = track;
    // Apply the fiducial cut
    if (processTarget1)
      inTarget1 = cutUtils::FiducialCut(*track, SubDetId::kTarget1);
    if (processTarget2 && !inTarget1 )
      inTarget2 = cutUtils::FiducialCut(*track, SubDetId::kTarget2);
 
    if      (inTarget1)
      EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCInTarget1FV][EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCInTarget1FV]++] = track;
    else if (inTarget2)
      EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCInTarget2FV][EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCInTarget2FV]++] = track;

    // Apply the track quality cut
   if (!cutUtils::DeltaLYZTPCCut(*track)) continue;
          AnaTrueParticle *trueP = dynamic_cast<AnaTrueParticle*>(track->GetTrueParticle());

    if      (inTarget1)
      EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV][EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV]++] = track;
    else if (inTarget2)
      EventBox->RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV][EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV]++] = track;

    
  }

  if (processTarget1){
    anaUtils::ResizeArray(EventBox-> RecObjectsInGroup[EventBox::kTracksWithTPCInTarget1FV],
                       EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCInTarget1FV]);
    anaUtils::ResizeArray(EventBox-> RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV],
                       EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget1FV]);
    anaUtils::ResizeArray(EventBox-> RecObjectsInGroup[EventBox::kTracksWithTPCAndTarget1],
                       EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCAndTarget1]);
    anaUtils::ResizeArray(EventBox-> RecObjectsInGroup[EventBox::kTracksWithTPCorTarget1],
                       EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCorTarget1]);

  }
  if (processTarget2){
    anaUtils::ResizeArray(EventBox-> RecObjectsInGroup[EventBox::kTracksWithTPCInTarget2FV],
                       EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCInTarget2FV]);
    anaUtils::ResizeArray(EventBox-> RecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV],
                       EventBox->nRecObjectsInGroup[EventBox::kTracksWithGoodQualityTPCInTarget2FV]);
    anaUtils::ResizeArray(EventBox-> RecObjectsInGroup[EventBox::kTracksWithTPCAndTarget2],
                       EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCAndTarget2]);
    anaUtils::ResizeArray(EventBox-> RecObjectsInGroup[EventBox::kTracksWithTPCorTarget2],
                       EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCorTarget2]);

  }

}

//********************************************************************
void boxUtils::FillTracksWithTarget(AnaEventB& event, SubDetId::SubDetEnum det){
//********************************************************************

  bool processTarget1 = false;
  bool processTarget2 = false;

  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];

  // Don't fill it when already filled by other selection
  if ((det==SubDetId::kTarget1 || det==SubDetId::kTarget) && !EventBox->RecObjectsInGroup[EventBox::kTracksWithTarget1AndNoTPC]) processTarget1 = true;
  if ((det==SubDetId::kTarget2 || det==SubDetId::kTarget) && !EventBox->RecObjectsInGroup[EventBox::kTracksWithTarget2AndNoTPC]) processTarget2 = true;
  if (!processTarget1 && !processTarget2) return;

  AnaTrackB* selTracks[NMAXPARTICLES];
  int nTarget =anaUtils::GetAllTracksUsingDet(event,det, selTracks);
  
  if (processTarget1){
    EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget1AndNoTPC]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTarget1AndNoTPC], nTarget);
    EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget1]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTarget1], nTarget);
    if(!EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCorTarget1]){
      EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCorTarget1]=0;
      anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCorTarget1], nTarget);
    }
    else{
      anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCorTarget1], 
                         EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCorTarget1]+nTarget);
    }
  }
  if (processTarget2){
    EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget2AndNoTPC]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTarget2AndNoTPC], nTarget);
    EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget2]=0;
    anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTarget2], nTarget);
    if(!EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCorTarget2]){
      EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCorTarget2]=0;
      anaUtils::CreateArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCorTarget2], nTarget);
    }
    else{
      anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCorTarget2], EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCorTarget2]+nTarget);
    }
  }
  

  //loop over Target tracks
  for (Int_t i=0;i<nTarget; ++i){
    AnaTrackB* track = selTracks[i];
    if (track->nTargetSegments==0){
      std::cout << "Warning. This track has no Target segments" << std::endl;
      continue;
    }
    if (processTarget1){
      if (SubDetId::GetDetectorUsed(track->Detectors, SubDetId::kTarget1)){
        if (!SubDetId::GetDetectorUsed(track->Detectors, SubDetId::kTPC)){
          EventBox->RecObjectsInGroup[EventBox::kTracksWithTarget1AndNoTPC][EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget1AndNoTPC]++] = track;
          //tracks with TPC already saved, we only need to add those that are not in the tpc
          EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCorTarget1][EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCorTarget1]++] = track;
        }
        EventBox->RecObjectsInGroup[EventBox::kTracksWithTarget1][EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget1]++] = track;

      }
    }
    if (processTarget2){
      if (SubDetId::GetDetectorUsed(track->Detectors, SubDetId::kTarget2)){
        if (!SubDetId::GetDetectorUsed(track->Detectors, SubDetId::kTPC)){
          EventBox->RecObjectsInGroup[EventBox::kTracksWithTarget2AndNoTPC][EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget2AndNoTPC]++] = track;
          //tracks with TPC already saved, we only need to add those that are not in the tpc
          EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCorTarget2][EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCorTarget2]++] = track;
        }
        EventBox->RecObjectsInGroup[EventBox::kTracksWithTarget2][EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget2]++] = track;
        
      }
    }
  }
  if (processTarget1){
    anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTarget1AndNoTPC], 
                       EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget1AndNoTPC]);
    anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTarget1],         
                       EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget1]);
    anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCorTarget1],    
                       EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCorTarget1]);
  }
  if (processTarget2){
    anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTarget2AndNoTPC], 
                       EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget2AndNoTPC]);
    anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTarget2],         
                       EventBox->nRecObjectsInGroup[EventBox::kTracksWithTarget2]);
    anaUtils::ResizeArray(EventBox->RecObjectsInGroup[EventBox::kTracksWithTPCorTarget2],    
                       EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPCorTarget2]);
  }

}

//********************************************************************
void boxUtils::FillTrajsChargedInTPC(AnaEventB& event){
//********************************************************************

  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];

  // Don't fill it when already filled by other selection
  if (EventBox->TrueObjectsInGroup[EventBox::kTrueParticlesChargedInTPCInBunch]) return;

  AnaTrueParticleB* trajs[NMAXTRUEPARTICLES];
  Int_t nTPC = anaUtils::GetAllChargedTrajInTPCInBunch(event, trajs);
  if(NMAXTRUEPARTICLES<(UInt_t)nTPC) nTPC=NMAXTRUEPARTICLES;
  anaUtils::CreateArray(EventBox->TrueObjectsInGroup[EventBox::kTrueParticlesChargedInTPCInBunch],nTPC);
  anaUtils::CopyArray(EventBox->TrueObjectsInGroup[EventBox::kTrueParticlesChargedInTPCInBunch],trajs,nTPC);
  EventBox->nTrueObjectsInGroup[EventBox::kTrueParticlesChargedInTPCInBunch] = nTPC;
}

//********************************************************************
void boxUtils::FillTrajsChargedInTargetAndNoTPC(AnaEventB& event, SubDetId::SubDetEnum det){
//********************************************************************

  bool processTarget1 = false;
  bool processTarget2 = false;

  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];

  // Don't fill it when already filled by other selection
  if ((det==SubDetId::kTarget1 || det==SubDetId::kTarget) && !EventBox->TrueObjectsInGroup[EventBox::kTrueParticlesChargedInTarget1AndNoTPCInBunch]) processTarget1 = true;
  if ((det==SubDetId::kTarget2 || det==SubDetId::kTarget) && !EventBox->TrueObjectsInGroup[EventBox::kTrueParticlesChargedInTarget2AndNoTPCInBunch]) processTarget2 = true;

  if (!processTarget1 && !processTarget2) return;

  if (processTarget1){
    AnaTrueParticleB* trajs[NMAXTRUEPARTICLES];
    Int_t nTarget = anaUtils::GetAllChargedTrajInTargetAndNoTPCInBunch(event, trajs, SubDetId::kTarget1);
    if(NMAXTRUEPARTICLES<(UInt_t)nTarget) nTarget=NMAXTRUEPARTICLES;    
    anaUtils::CreateArray(EventBox->TrueObjectsInGroup[EventBox::kTrueParticlesChargedInTarget1AndNoTPCInBunch],nTarget);
    anaUtils::CopyArray(EventBox->TrueObjectsInGroup[EventBox::kTrueParticlesChargedInTarget1AndNoTPCInBunch],trajs,nTarget);
    EventBox->nTrueObjectsInGroup[EventBox::kTrueParticlesChargedInTarget1AndNoTPCInBunch] = nTarget;
  }
  if (processTarget2){
    AnaTrueParticleB* trajs[NMAXTRUEPARTICLES];
    Int_t nTarget = anaUtils::GetAllChargedTrajInTargetAndNoTPCInBunch(event, trajs, SubDetId::kTarget2);
    if(NMAXTRUEPARTICLES<(UInt_t)nTarget) nTarget=NMAXTRUEPARTICLES;    
    anaUtils::CreateArray(EventBox->TrueObjectsInGroup[EventBox::kTrueParticlesChargedInTarget2AndNoTPCInBunch],nTarget);
    anaUtils::CopyArray(EventBox->TrueObjectsInGroup[EventBox::kTrueParticlesChargedInTarget2AndNoTPCInBunch],trajs,nTarget);
    EventBox->nTrueObjectsInGroup[EventBox::kTrueParticlesChargedInTarget2AndNoTPCInBunch] = nTarget;
  }
}

//********************************************************************
void boxUtils::FillTrajsChargedInTPCorTarget(AnaEventB& event, SubDetId::SubDetEnum det){
//********************************************************************
  
  (void)det;
  
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];

  // Don't fill it when already filled by other selection
  if (EventBox->TrueObjectsInGroup[EventBox::kTrueParticlesChargedInTPCorTargetInBunch]) return;

  AnaTrueParticleB* trajs[NMAXTRUEPARTICLES];
  Int_t count = anaUtils::GetAllChargedTrajInTPCTargetInBunch(event, trajs);
  if((UInt_t)count>NMAXTRUEPARTICLES) count = NMAXTRUEPARTICLES;
  anaUtils::CreateArray(EventBox->TrueObjectsInGroup[EventBox::kTrueParticlesChargedInTPCorTargetInBunch],count);
  anaUtils::CopyArray(EventBox->TrueObjectsInGroup[EventBox::kTrueParticlesChargedInTPCorTargetInBunch],trajs,count);
  EventBox->nTrueObjectsInGroup[EventBox::kTrueParticlesChargedInTPCorTargetInBunch] = count;
}

//********************************************************************
void boxUtils::FillTracksWithECal(AnaEventB& event){
//********************************************************************

  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];

  // Don't fill it when already filled by other selection
 // if (EventBox->RecObjectsInGroup[EventBox::kTracksWithECal]) return;

  AnaTrackB* tracks[NMAXPARTICLES];

  Int_t nECal = anaUtils::GetAllTracksUsingECAL(event, tracks);
  if(NMAXPARTICLES<(UInt_t)nECal) nECal = NMAXPARTICLES;    
    
  // Fill the box
  anaUtils::CreateArray( EventBox->RecObjectsInGroup[EventBox::kTracksWithECal], nECal);
    for (Int_t i=0;i<nECal; ++i){
    AnaTrackB* track = tracks[i];
          EventBox->RecObjectsInGroup[EventBox::kTracksWithECal][EventBox->nRecObjectsInGroup[EventBox::kTracksWithECal]++] = track;
    }
}
