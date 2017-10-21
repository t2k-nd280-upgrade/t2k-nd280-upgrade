#include <TVector3.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

#include "AnalysisUtils.hxx"
#include "EventBoxId.hxx"
#include "EventBox.hxx"
#include "ND280UPAnalysisUtils.hxx"
#include "CategoriesUtils.hxx"
#include "CutUtils.hxx"

#include "nueCCUtils.hxx"

#include "Parameters.hxx"

//********************************************************************
void nueCCUtils::AddCategories(bool AntiNu){
  //********************************************************************

  anaUtils::AddStandardCategories();

  ///  nuesimple  ///  GetNuESimpleCat  ///
  const int NNUE1 = 8;
  std::string reac_nue1[NNUE1]  = {"#nu_{e} CC0pi", "#nu_{e}CC1pi", "#nu_{e} CCOther", "OOFV #gamma background", "FV #gamma background", "#mu background", "Other background", "Proton background"};
  int reac_nue1code[NNUE1]      = {0,                1,             2,                 6,                        3,                      4,                5,                  8};
  int reac_nue1col[NNUE1]       = {2,                6,                7,                 4,                        5,                      3,                1,                  48};
  
  // for AntiNu selection rename the categories
  if (AntiNu) {
    reac_nue1[0] = "#bar{#nu}_{e} CC0pi";
    reac_nue1[1] = "#bar{#nu}_{e} CC1pi";
    reac_nue1[2] = "#bar{#nu}_{e} CCOther";
  }
  anaUtils::_categ->AddCategory("nueCC",       NNUE1, reac_nue1, reac_nue1code, reac_nue1col);

}

//********************************************************************
int nueCCUtils::nueCCCategory(AnaEventB* event, AnaTrack* track, const SubDetId::SubDetEnum det, bool AntiNu){
  //********************************************************************

  if (!track || !track->GetTrueParticle() || track->GetTrueParticle()->PDG == 0) return -1;

  AnaTrueVertex* vtx = static_cast<AnaTrueVertex*> (track->GetTrueParticle()->TrueVertex);
  if (!vtx) return -1; 

  int parent_pdg = 0;
  AnaTrueParticleB* parent = anaUtils::GetTrueParticleByID(*event, track->GetTrueParticle()->ParentID);
  if(parent)
    parent_pdg = parent->PDG;

  if (IsNuESignal(vtx, det, AntiNu) && abs(track->GetTrueParticle()->PDG) == 11 && parent_pdg != 111){

    if (IsNuECC0pi(vtx, det, AntiNu))
      return 0; // nue CC0pi
    else if (IsNuECC1pi(vtx, det, AntiNu))
      return 1; // nue CC1pi
    else return 2; // nue CCOther
  
  }

  if (abs(track->GetTrueParticle()->PDG) == 13) return 4; // muon
  if (AntiNu && (abs(track->GetTrueParticle()->PDG) == 2212)) return 8; // extra category for NuEBar selection
  if (IsGammaSignalOOFGD(track, det, parent_pdg)) return 6; // gamma OOFGD background
  if (IsGammaSignal(track,det, parent_pdg)) return 3; // gamma background
  
  return 5; // Other
}

//********************************************************************
bool nueCCUtils::IsNuESignal(AnaTrueVertex* vtx, const SubDetId::SubDetEnum det, bool AntiNu) {
  //********************************************************************

  if(!vtx) return 0;
 
  if (AntiNu)
    return (anaUtils::InFiducialVolume(det,vtx->Position) && vtx->NuPDG == -12  && vtx->ReacCode < 0 && vtx->ReacCode > -31);
  else
    return (anaUtils::InFiducialVolume(det,vtx->Position) && vtx->NuPDG == 12  && vtx->ReacCode > 0 && vtx->ReacCode < 31);
}

//********************************************************************
bool nueCCUtils::IsNuECC0pi(AnaTrueVertex* vtx, const SubDetId::SubDetEnum det, bool AntiNu){
  //********************************************************************

  if (IsNuESignal(vtx, det, AntiNu)) {
    Int_t Nmeson =    vtx->NPrimaryParticles[ParticleId::kMesons];
    if(Nmeson > 0) return 0;
    else return 1;
  }
  
  return 0; // NOT nue CC0pi
}

//********************************************************************
bool nueCCUtils::IsNuECC1pi(AnaTrueVertex* vtx, const SubDetId::SubDetEnum det, bool AntiNu) {
  //********************************************************************

  if (!IsNuESignal(vtx, det, AntiNu))
    return false;

  Int_t Nmeson =    vtx->NPrimaryParticles[ParticleId::kMesons];
  Int_t Npipos =    vtx->NPrimaryParticles[ParticleId::kPiPos];
  Int_t Npineg =    vtx->NPrimaryParticles[ParticleId::kPiNeg];

  if (!AntiNu && Npipos == 1 && Nmeson == 1)
    return true;
  else if (AntiNu && Npineg == 1 && Nmeson == 1)
    return true;
  
  return false;
}

//********************************************************************
bool nueCCUtils::IsGammaSignal(AnaTrack* track, const SubDetId::SubDetEnum det, int parent) {
//********************************************************************
  
  AnaTrueParticle* truetrack = static_cast<AnaTrueParticle*>( track->TrueObject );
  if(!truetrack) return false;

  if(parent == 22 && abs(truetrack->PDG) == 11 && anaUtils::InFiducialVolume(det,truetrack->Position) )
    return true;

  return false;
}

//********************************************************************
bool nueCCUtils::IsGammaSignalOOFGD(AnaTrack* track, const SubDetId::SubDetEnum det, int parent){
  //********************************************************************

  AnaTrueParticle* truetrack = static_cast<AnaTrueParticle*>( track->TrueObject );
  if(!truetrack) return false;

  AnaTrueVertex* vtx = static_cast<AnaTrueVertex*> (track->GetTrueParticle()->TrueVertex);
  if(!vtx) return false;

  if(parent == 22 && abs(truetrack->PDG) == 11 && !(anaUtils::InDetVolume(det, vtx->Position)))
    return true;

  return false;
}

//**************************************************
TLorentzVector nueCCUtils::GetMomLorentzVector(const AnaTrackB& track, Float_t mass) {
  //**************************************************

  TVector3 dir_start = anaUtils::ArrayToTVector3(track.DirectionStart);
  Float_t mom_start = track.SmearedMomentum;
  TVector3 momentum = dir_start * (mom_start / dir_start.Mag());
  Float_t energy = sqrt(mom_start * mom_start + mass * mass);
  TLorentzVector lv(momentum, energy);
  return lv;
}    

//**************************************************
AnaTrackB* nueCCUtils::FindPairTrack(const AnaEventB& event, const AnaTrackB& track, Float_t delta_pos_max){
  //**************************************************
  AnaTrackB* PairTrack = NULL;

  Float_t minv_postcuts = 1e6;
  Float_t mass = 0.511;
  
  // The start 4 momentum and position of the primary particle.
  TLorentzVector primary_PE = nueCCUtils::GetMomLorentzVector(track, mass);
  TVector3 primary_pos = anaUtils::ArrayToTVector3(track.PositionStart);
  
  // Get all TPC tracks
  EventBoxB* EventBox = event.EventBoxes[EventBoxId::kEventBoxNDUP];
  int nTPC=EventBox->nRecObjectsInGroup[EventBox::kTracksWithTPC];

  // Loop over all TPC tracks that isn't the primary one.
  for(Int_t i = 0; i < nTPC; i++ ) {
    AnaTrackB* secondary = static_cast<AnaTrackB*>(EventBox->RecObjectsInGroup[EventBox::kTracksWithTPC][i]);

    // Not the selected main track
    if (&track == secondary) continue;

    // The start 4 momentum and position of the secondry track
    TLorentzVector secondary_PE = nueCCUtils::GetMomLorentzVector(*secondary, mass);
    TVector3 secondary_pos = anaUtils::ArrayToTVector3(secondary->PositionStart);

    Float_t minv = 2. * mass * mass + 2. * (secondary_PE.Dot(primary_PE));
    if(minv > 0)
      minv = sqrt(minv);
    else
      continue;
    
    // Tracks start within a certain distance of each other.
    if((primary_pos - secondary_pos).Mag() > delta_pos_max) continue;

    // Tracks have opposite charge.
    if(track.Charge * secondary->Charge > 0) continue;

    // Track quality
    if(!cutUtils::DeltaLYZTPCCut(*secondary)) continue;

    Float_t GetElecL = anaUtils::GetPIDLikelihood(*secondary, 1);
    if(GetElecL < 0.5) continue;

    if(minv < minv_postcuts){
      PairTrack = secondary;
      minv_postcuts = minv;
    }
  }

  return PairTrack;
}

//********************************************************************
bool nueCCUtils::IsForward(const AnaTrackB& track){
  //********************************************************************

  // check if a track is forward (in Z) going
  // need something fancier?

  return track.PositionStart[2] < track.PositionEnd[2];  
}

// ********************************************************************
Float_t* nueCCUtils::GetFittedPos(Float_t* start, Float_t* end, double z){
  // ********************************************************************

  double t = (z-start[2])/(end[2]-start[2]);
  Float_t* pos = new Float_t(3);
  pos[0] = start[0]+(end[0]-start[0])*t;
  pos[1] = start[1]+(end[1]-start[1])*t;
  pos[2] = z;
	
  return pos;

}

// ********************************************************************
Float_t nueCCUtils::GetDeltaPos(Float_t* hit, Float_t* fit){
  // ********************************************************************

  Float_t delta = 0;
  if ( hit[0]>-4.6 && hit[0]<0.2 )  delta = hit[1] - fit[1];
  if ( hit[1]>50.4 && hit[1]<55.2 ) delta = hit[0] - fit[0];

  return delta;

}

// ********************************************************************
Float_t nueCCUtils::GetDeltaPos2(Float_t* hit, Float_t* fit){
  // ********************************************************************

  Float_t delta = 0;
  if ( hit[0]>-4.6 && hit[0]<0.2 )  delta = hit[1]*hit[1] - fit[1]*fit[1];
  if ( hit[1]>50.4 && hit[1]<55.2 ) delta = hit[0]*hit[0] - fit[0]*fit[0];

  return delta;

}

//********************************************************************
bool nueCCUtils::HGlobalMomFirst(AnaTrackB* a, AnaTrackB* b){
  //********************************************************************

  return a->SmearedMomentum > b->SmearedMomentum;

}

//********************************************************************
bool nueCCUtils::isHMNT(const AnaTrackB* candidate, std::vector<AnaTrackB*> GroupTracks, bool lowangle) {
  //********************************************************************

  bool useTarget1 = ND::params().GetParameterI("nueCCAnalysis.EnableTarget1");
  bool useTarget2 = ND::params().GetParameterI("nueCCAnalysis.EnableTarget2");

  for (UInt_t i=0;i<GroupTracks.size(); ++i){
    if (lowangle) {
      if ( useTarget1 && anaUtils::InFiducialVolume(SubDetId::kTarget1, GroupTracks[i]->PositionStart, LAFVmin, LAFVmax) ) return GroupTracks[i]->SmearedMomentum == candidate->SmearedMomentum;
      if ( useTarget2 && anaUtils::InFiducialVolume(SubDetId::kTarget2, GroupTracks[i]->PositionStart, LAFVmin, LAFVmax) ) return GroupTracks[i]->SmearedMomentum == candidate->SmearedMomentum;
    }
    else {
      if ( useTarget1 && anaUtils::InFiducialVolume(SubDetId::kTarget1, GroupTracks[i]->PositionStart, HAFVmin, HAFVmax) ) return GroupTracks[i]->SmearedMomentum == candidate->SmearedMomentum;
      if ( useTarget2 && anaUtils::InFiducialVolume(SubDetId::kTarget2, GroupTracks[i]->PositionStart, HAFVmin, HAFVmax) ) return GroupTracks[i]->SmearedMomentum == candidate->SmearedMomentum;
    }
  }

  return false;

}

//**************************************************
bool nueCCUtils::TPCElectronPIDCut(const AnaTrackB& candidate, Float_t Lelec) {
  //**************************************************

  Float_t GetElecL = anaUtils::GetPIDLikelihood(candidate, 1);

  if(GetElecL < Lelec) return false;

  return true;

}

//**************************************************
bool nueCCUtils::TPCMuonPIDCut(const AnaTrackB& candidate, Float_t Lmu) {
  //**************************************************

  Float_t GetMuonL = anaUtils::GetPIDLikelihood(candidate, 0);

  if(GetMuonL > Lmu) return false;

  return true;

}

//**************************************************
bool nueCCUtils::compare_length( AnaTrackB* lhs,  AnaTrackB* rhs) {
  //**************************************************
  Float_t left_length  = nueCCUtils::GetDistance(lhs->PositionStart, lhs->PositionEnd);
  Float_t right_length = nueCCUtils::GetDistance(rhs->PositionStart, rhs->PositionEnd);
  return (left_length > right_length);
}

//**************************************************
Float_t nueCCUtils::GetDistance(const Float_t* pos1, const Float_t* pos2) {
  //**************************************************
  TVector3 vec1(pos1[0], pos1[1], pos1[2]);
  TVector3 vec2(pos2[0], pos2[1], pos2[2]);

  return (vec1 - vec2).Mag();
}