#include "TruthUtils.hxx"
#include "ConstituentsUtils.hxx"

//**************************************************
Float_t anaUtils::GetEntranceMomentum(const AnaDetCrossingB& cross){
  //**************************************************
  return sqrt(pow(cross.EntranceMomentum[0],2) +
              pow(cross.EntranceMomentum[1],2) +
              pow(cross.EntranceMomentum[2],2));
}

//**************************************************
Float_t anaUtils::GetExitMomentum(const AnaDetCrossingB& cross){
  //**************************************************

  return sqrt(pow(cross.ExitMomentum[0],2) +
              pow(cross.ExitMomentum[1],2) +
              pow(cross.ExitMomentum[2],2));
}


//**************************************************
Int_t anaUtils::GetNMichelElectrons(const AnaTrueVertexB& trueVertex, SubDetId::SubDetEnum det){
  //**************************************************

  // out of FV
  if ( ! InFiducialVolume(det, trueVertex.Position)) return -999;

  /// particles coming out the vertex
  Int_t NMich=0;

  for (int tit = 0; tit < trueVertex.nTrueParticles; tit++) {
    AnaTrueParticleB* true_track = trueVertex.TrueParticles[tit];

    //if not a pi+/-, a e+/-, a mu+/-
    if( abs(true_track->GParentPDG)!=211) continue;
    if( abs(true_track->ParentPDG)!=13  ) continue;
    if( abs(true_track->PDG)!=11  )       continue;

    NMich++;
  }

  //  std::cout<<" nb of michel electrons "<<NMich << " " << trueVertex.nTrueParticles <<std::endl;

  return  NMich;
}

//**************************************************
Float_t anaUtils::GetTrueLinearLengthInTarget(const AnaTrueParticleB& trueTrack, Float_t& distz){
  //**************************************************

  Float_t dist = -9999999;
  distz = -999;
  for (Int_t idet = 0; idet < trueTrack.nDetCrossings; idet++) {
    //i.e crossing the active part of the tpc
    if (!trueTrack.DetCrossings[idet]->Detector)
      continue;
    if (SubDetId::GetDetectorUsed(trueTrack.DetCrossings[idet]->Detector, SubDetId::kTarget) && trueTrack.DetCrossings[idet]->InActive) {
      Float_t dist_temp = anaUtils::GetSeparationSquared(trueTrack.DetCrossings[idet]->ExitPosition, trueTrack.DetCrossings[idet]->EntrancePosition);
      Float_t distz_temp = fabs(trueTrack.DetCrossings[idet]->ExitPosition[2] - trueTrack.DetCrossings[idet]->EntrancePosition[2]);
      if (dist_temp > dist)
        dist = dist_temp;
      if (distz_temp > distz)
        distz = distz_temp;

    }
  }

  return sqrt(dist);
}

//**************************************************
Float_t anaUtils::GetTrueLinearLengthInFGD(const AnaTrueParticleB& trueTrack, Float_t& distz){
  //**************************************************

  Float_t dist = -9999999;
  distz = -999;
  for (Int_t idet = 0; idet < trueTrack.nDetCrossings; idet++) {
    //i.e crossing the active part of the tpc
    if (!trueTrack.DetCrossings[idet]->Detector)
      continue;
    if (SubDetId::GetDetectorUsed(trueTrack.DetCrossings[idet]->Detector, SubDetId::kFGD) && trueTrack.DetCrossings[idet]->InActive) {
      Float_t dist_temp = anaUtils::GetSeparationSquared(trueTrack.DetCrossings[idet]->ExitPosition, trueTrack.DetCrossings[idet]->EntrancePosition);
      Float_t distz_temp = fabs(trueTrack.DetCrossings[idet]->ExitPosition[2] - trueTrack.DetCrossings[idet]->EntrancePosition[2]);
      if (dist_temp > dist)
        dist = dist_temp;
      if (distz_temp > distz)
        distz = distz_temp;

    }
  }

  return sqrt(dist);
}

//********************************************************************
bool anaUtils::TrueParticleEntersDet(const AnaTrueParticleB* track, SubDetId::SubDetEnum det){
  //********************************************************************

  if(!track)
    return false;

  //check that a trajectory crosses volume
  for(Int_t idet=0;idet<track->nDetCrossings;idet++){

    if(!SubDetId::GetDetectorUsed(track->DetCrossings[idet]->Detector, det)) continue;

    return true;

  }

  return false;

}

//********************************************************************
bool anaUtils::TrueParticleCrossesTPC(const AnaTrueParticleB* track, SubDetId::SubDetEnum det){
  //********************************************************************

  if(!track)
    return false;

  if  (!SubDetId::IsTPC(det))
    return false;

  //loop through det crossings
  for(Int_t idet=0;idet<track->DetCrossingsVect.size();idet++){

    AnaDetCrossingB* cross = track->DetCrossingsVect[idet];
    if(!cross)
      continue;

    // i.e crossing the active part of the TPC
    if (!SubDetId::GetDetectorUsed(cross->Detector, det))
      continue;

    return true;
  }

  return false;

}

//********************************************************************
bool anaUtils::TrueParticleCrossesTarget(const AnaTrueParticleB* track, SubDetId::SubDetEnum det){
  //********************************************************************

  if(!track)
    return false;

  if  (det!=SubDetId::kTarget && !SubDetId::IsTarget(det))
    return false;

  Float_t dist=-999999.;

  //loop through det crossings
  for(Int_t idet=0;idet<track->nDetCrossings;idet++){

    AnaDetCrossingB* cross = track->DetCrossings[idet];
    if(!cross)
      continue;

    // i.e crossing the active part of the TPC
    if (!SubDetId::GetDetectorUsed(track->DetCrossings[idet]->Detector, det) || !track->DetCrossings[idet]->InActive)
      continue;

    //i.e crossing the active part of the tpc
    if(SubDetId::GetDetectorUsed(track->DetCrossings[idet]->Detector, det) && track->DetCrossings[idet]->InActive){
        Float_t sep = anaUtils::GetSeparationSquared(track->DetCrossings[idet]->EntrancePosition, track->DetCrossings[idet]->ExitPosition);
        if(sep>dist) dist=sep;
      }
  }

  if(dist>62500)//bigger than the ~1/4 of the width of the Target
      return true;

  return false;

}

//********************************************************************
bool anaUtils::TrueParticleCrossesFGD(const AnaTrueParticleB* track, SubDetId::SubDetEnum det){
  //********************************************************************

  if(!track)
    return false;

  if  (det!=SubDetId::kFGD && !SubDetId::IsFGD(det))
    return false;

  Float_t dist=-999999.;

  //loop through det crossings
  for(Int_t idet=0;idet<track->nDetCrossings;idet++){

    AnaDetCrossingB* cross = track->DetCrossings[idet];
    if(!cross)
      continue;

    // i.e crossing the active part of the TPC
    if (!SubDetId::GetDetectorUsed(track->DetCrossings[idet]->Detector, det) || !track->DetCrossings[idet]->InActive)
      continue;

    //i.e crossing the active part of the tpc
    if(SubDetId::GetDetectorUsed(track->DetCrossings[idet]->Detector, det) && track->DetCrossings[idet]->InActive){
        Float_t sep = anaUtils::GetSeparationSquared(track->DetCrossings[idet]->EntrancePosition, track->DetCrossings[idet]->ExitPosition);
        if(sep>dist) dist=sep;
      }
  }

  if(dist>62500)//bigger than the ~1/4 of the width of the FGD
      return true;

  return false;

}

//**************************************************
int anaUtils::GetTPCDetCrossed(const AnaTrueParticleB* track, SubDetId::SubDetEnum det[]) {
  //**************************************************
  int count = 0;

  if (!track)
    return count;

  for(Int_t idet=0;idet<track->nDetCrossings;idet++){

    // i.e crossing the active part of the TPC
    if (SubDetId::GetDetectorUsed(track->DetCrossings[idet]->Detector, SubDetId::kTPCUp1)
        && track->DetCrossings[idet]->InActive)
      det[count++] = SubDetId::kTPCUp1;

    if (SubDetId::GetDetectorUsed(track->DetCrossings[idet]->Detector, SubDetId::kTPCUp2)
        && track->DetCrossings[idet]->InActive)
      det[count++] = SubDetId::kTPCUp2;

    if (SubDetId::GetDetectorUsed(track->DetCrossings[idet]->Detector, SubDetId::kTPCDown1)
        && track->DetCrossings[idet]->InActive)
      det[count++] = SubDetId::kTPCDown1;

    if (SubDetId::GetDetectorUsed(track->DetCrossings[idet]->Detector, SubDetId::kTPCDown2)
        && track->DetCrossings[idet]->InActive)
      det[count++] = SubDetId::kTPCDown2;

    if (SubDetId::GetDetectorUsed(track->DetCrossings[idet]->Detector, SubDetId::kForwTPC1)
        && track->DetCrossings[idet]->InActive)
      det[count++] = SubDetId::kForwTPC1;

    if (SubDetId::GetDetectorUsed(track->DetCrossings[idet]->Detector, SubDetId::kForwTPC2)
        && track->DetCrossings[idet]->InActive)
      det[count++] = SubDetId::kForwTPC2;
    if (SubDetId::GetDetectorUsed(track->DetCrossings[idet]->Detector, SubDetId::kForwTPC3)
        && track->DetCrossings[idet]->InActive)
      det[count++] = SubDetId::kForwTPC3;

  }

  return count;
}




//___________________________________________________________________________
bool anaUtils::IsCCQE(int neut_reaction_mode)
{
  return neut_reaction_mode ==  1;
}
//___________________________________________________________________________
bool anaUtils::IsCC1pi(int neut_reaction_mode)
{
  bool is_cc1pi = (neut_reaction_mode == 11 ||
                   neut_reaction_mode == 12 ||
                   neut_reaction_mode == 13);
  return is_cc1pi;
}
//___________________________________________________________________________
bool anaUtils::IsCCcoh(int neut_reaction_mode)
{
  return neut_reaction_mode ==  16;
}
//___________________________________________________________________________
bool anaUtils::Is2p2h(int neut_reaction_mode)
{
  return neut_reaction_mode ==  2;
}
//___________________________________________________________________________
bool anaUtils::IsCCoth(int neut_reaction_mode)
{
  bool is_ccoth = (neut_reaction_mode == 17 ||
                     neut_reaction_mode == 21 ||
                     neut_reaction_mode == 22 ||
                     neut_reaction_mode == 23 ||
                     neut_reaction_mode == 26);
  return is_ccoth;
}
//___________________________________________________________________________
bool anaUtils::IsNC(int neut_reaction_mode)
{
  bool is_not_nc =  IsCCoth(neut_reaction_mode);
  is_not_nc = Is2p2h(neut_reaction_mode);
  is_not_nc = IsCCcoh(neut_reaction_mode);
  is_not_nc = IsCC1pi(neut_reaction_mode);
  is_not_nc = IsCCQE(neut_reaction_mode);

  bool is_nc = !is_not_nc;
  return is_nc;
}
//___________________________________________________________________________
int anaUtils::GetReacAll(int neut_reaction_mode){
  if(IsCCQE(neut_reaction_mode)){
    return 0;
  }
  else if(Is2p2h(neut_reaction_mode)){
    return 1;
  }
  else if(IsCC1pi(neut_reaction_mode)){
    return 2;
  }
  else if(IsCCcoh(neut_reaction_mode)){
    return 3;
  }
  else if(IsCCoth(neut_reaction_mode)){
    return 4;
  }
  else {
    return 5;
  }
}




