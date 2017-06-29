#include <TVector3.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

#include "TRandom3.h"

#include "AnalysisUtils.hxx"
#include "EventBoxId.hxx"
#include "EventBox.hxx"
#include "ND280UPAnalysisUtils.hxx"
#include "CategoriesUtils.hxx"
#include "CutUtils.hxx"

#include "numuCCOOFVSelection.hxx"
#include "numuCCOOFVUtils.hxx"

#include "Parameters.hxx"

//********************************************************************
int numuCCOOFV_utils::CCOOFVCategory(AnaTrack* candidate){
  //********************************************************************
	
  if(!candidate) return -1;
  AnaTrueParticle* truetrack = static_cast<AnaTrueParticle*>( candidate->TrueObject );
  if (!truetrack) return -1;
  AnaTrueVertex* truevtx = static_cast<AnaTrueVertex*>( truetrack->TrueVertex );
  if (!truevtx) return -1;
	
  bool muon = false;
  bool FV = false;
  bool CC = false;
  muon =  truetrack->Momentum == truevtx->LeptonMom;
	
  bool useTarget1 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget1");
  bool useTarget2 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget2");
  bool useFGD1 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableFGD1");
  bool useFGD2 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableFGD2");
  FV = ((useTarget1 && anaUtils::InFiducialVolume( SubDetId::kTarget1, truevtx->Position )) ||
	(useTarget2 && anaUtils::InFiducialVolume( SubDetId::kTarget2, truevtx->Position )) ||
	(useFGD1 && anaUtils::InFiducialVolume( SubDetId::kFGD1, truevtx->Position )) ||
	(useFGD2 && anaUtils::InFiducialVolume( SubDetId::kFGD2, truevtx->Position )));
	
  int reac = anaUtils::GetReactionNoTargetFv( *static_cast<AnaTrueVertex*>(truevtx) );
  CC = ((reac==0) || (reac==1) || (reac==2) || (reac==3) || (reac==9)) && truevtx->LeptonMom>0 && truevtx->LeptonPDG==13;
	
  if (FV) {
    if(  muon &&  CC ) return 0;
    if( !muon &&  CC ) return 1;
    if( !CC )          return 2;
  }
  else return 3;
	
  return -1;
	
}



//********************************************************************
int numuCCOOFV_utils::OOFVCategory(AnaTrack* candidate, int topo){
  //********************************************************************
	
  if(!candidate) return -1;
  AnaTrueParticle* truetrack = static_cast<AnaTrueParticle*>( candidate->TrueObject );
  if (!truetrack) return -1;
  AnaTrueVertex* truevtx = static_cast<AnaTrueVertex*>( truetrack->TrueVertex );
  if (!truevtx) return -1;
	
  Float_t* tvertex = truevtx->Position;
	
  bool useTarget1 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget1");
  bool useTarget2 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget2");
	
  if((useTarget1 && anaUtils::InFiducialVolume( SubDetId::kTarget1, truevtx->Position )) ||
     (useTarget2 && anaUtils::InFiducialVolume( SubDetId::kTarget2, truevtx->Position )))
    return 0;
	
  Float_t* tdir = truetrack->Direction;
  Float_t* pos  = truetrack->Position;
	
  Float_t* fgd_det_min;
  Float_t* fgd_det_max;
  double zup,zdown;
  bool lastmodule;
  fgd_det_min     = DetDef::Target1min;
  fgd_det_max     = DetDef::Target1max;
  zup             =-60;
  zdown           = 623;
  bool layer28=( pos[2] >429 && pos[2] <435 );
  bool layer29=( pos[2] >442 && pos[2] <445 );
  lastmodule=(layer28 || layer29);
	
  //inTarget1scint. Put z condition first to be faster
  if ( tvertex[2]>fgd_det_min[2] && tvertex[2]<fgd_det_max[2] && tvertex[0]>fgd_det_min[0] && tvertex[0]<fgd_det_max[0] && tvertex[1]>fgd_det_min[1] && tvertex[1]<fgd_det_max[1] )	{
    if (tvertex[2]<candidate->PositionStart[2]) return 1;
    else return 2;
  }
  //upstreamTarget1scint
  if ( tvertex[2]>zup && tvertex[2]<fgd_det_min[2] && tvertex[0]>-1335 && tvertex[0]<1335 && tvertex[1]>-1280.5 && tvertex[1]<1280.5 ) return 3;
  //downstreamTarget1scint
  if ( tvertex[2]>fgd_det_max[2] && tvertex[2]<zdown && tvertex[0]>-1335 && tvertex[0]<1335 && tvertex[1]>-1280.5 && tvertex[1]<1280.5 ) return 4;
  //neutralparent
  if ( truetrack->ParentPDG  == 2112 || truetrack->ParentPDG  == 22 ||  truetrack->GParentPDG == 2112 || truetrack->GParentPDG == 22) return 5;
  if (topo==0) {
    //backwards
    if (tdir[2]<0) return 6;
    //highangle
    if (tdir[2]>=0 && tdir[2]<0.5) return 7;
    //veryforward
    if(fabs(tdir[0]/tdir[2])<0.07 || fabs(tdir[1]/tdir[2])<0.07) return 8;
  }
  if (topo==1) {
    //forwards
    if (tdir[2]>0) return 6;
    //highangle
    if (tdir[2]<=0 && tdir[2]>-0.5) return 7;
    //verybackward
    if(fabs(tdir[0]/tdir[2])<0.07 || fabs(tdir[1]/tdir[2])<0.07) return 8;
  }
  //layer2829
  if ( lastmodule ) return 9;
  //other
  return 10;
	
}
//********************************************************************
bool numuCCOOFV_utils::IsForward(const AnaTrackB& track){
  //********************************************************************
	
  // check if a track is forward (in Z) going
  // need something fancier?
	
  return track.PositionStart[2] < track.PositionEnd[2];  
}

//********************************************************************
//int numuCCOOFV_utils::GetFgdLayer(Float_t* pos){
//********************************************************************
/*
  Float_t initZ=0;
  Float_t finalZ=0;
  Float_t barCenter[30];
	
  for (Int_t i=0; i<30; i++) {
  barCenter[i] = DetDef::Target1BarCenter[i];
  if (i==0) initZ = barCenter[0]-(DetDef::TargetXYBarWidth/2)-DetDef::TargetXYGlueWidth-DetDef::TargetXYSkinWidth-DetDef::TargetXYAirWidth;
  else initZ = finalZ;
  if      (i%2==0) finalZ = barCenter[i]+(DetDef::TargetXYBarWidth/2)+(DetDef::TargetXYMiddleGlueWidth/2);
  else if (i%2==1) finalZ = barCenter[i]+(DetDef::TargetXYBarWidth/2)+DetDef::TargetXYGlueWidth+DetDef::TargetXYSkinWidth+(DetDef::TargetXYAirWidth/2);
  if (i==29) finalZ += (DetDef::TargetXYAirWidth/2);
  if (pos[2]>initZ && pos[2]<finalZ) return i;
  }
	
  return -1;
	
  }
*/

// ********************************************************************
Float_t* numuCCOOFV_utils::GetFittedPos(Float_t* start, Float_t* end, double z){
  // ********************************************************************
	
  double t = (z-start[2])/(end[2]-start[2]);
  Float_t* pos = new Float_t(3);
  pos[0] = start[0]+(end[0]-start[0])*t;
  pos[1] = start[1]+(end[1]-start[1])*t;
  pos[2] = z;
	
  return pos;
	
}

// ********************************************************************
Float_t numuCCOOFV_utils::GetDeltaPos(Float_t* hit, Float_t* fit){
  // ********************************************************************
	
  Float_t delta = 0;
  if ( hit[0]>-4.6 && hit[0]<0.2 )  delta = hit[1] - fit[1];
  if ( hit[1]>50.4 && hit[1]<55.2 ) delta = hit[0] - fit[0];
	
  return delta;
	
}

// ********************************************************************
Float_t numuCCOOFV_utils::GetDeltaPos2(Float_t* hit, Float_t* fit){
  // ********************************************************************
	
  Float_t delta = 0;
  if ( hit[0]>-4.6 && hit[0]<0.2 )  delta = hit[1]*hit[1] - fit[1]*fit[1];
  if ( hit[1]>50.4 && hit[1]<55.2 ) delta = hit[0]*hit[0] - fit[0]*fit[0];
	
  return delta;
	
}

//********************************************************************
bool numuCCOOFV_utils::HGlobalMomFirst(AnaTrackB* a, AnaTrackB* b){
  //********************************************************************
	
  return a->SmearedMomentum > b->SmearedMomentum;
	
}

//********************************************************************
//bool numuCCOOFV_utils::HRangeMomFirst(AnaTrackB* a, AnaTrackB* b){
//********************************************************************
/*
  return a->RangeMomentumMuon > b->RangeMomentumMuon;
  }
*/

//********************************************************************
bool numuCCOOFV_utils::isHMNT(const AnaTrackB* candidate, std::vector<AnaTrackB*> GroupTracks, bool lowangle) {
  //********************************************************************
	
  bool useTarget1 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget1");
  bool useTarget2 = ND::params().GetParameterI("numuCCOOFVAnalysis.EnableTarget2");
	
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
bool numuCCOOFV_utils::MuonPIDCut(const AnaTrackB& candidate, bool ApplyMIP) {
  //**************************************************
	
  Float_t cut1 = ND::params().GetParameterD("numuCCOOFVAnalysis.MuonPID.Cut.Lmip");
  Float_t cut2 = ND::params().GetParameterD("numuCCOOFVAnalysis.MuonPID.Cut.Lmu");
  Float_t pcut = ND::params().GetParameterD("numuCCOOFVAnalysis.MuonPID.Cut.p_mip");
	
  if (ApplyMIP) {
    if ( (anaUtils::GetPIDLikelihoodMIP(candidate) > cut1 || candidate.Momentum > pcut) &&
	 (anaUtils::GetPIDLikelihood(candidate, 0) > cut2) ) {
      return true;
    }
  }
  else
    if ( anaUtils::GetPIDLikelihood(candidate, 0) > cut2 ) {
      return true;
    }
	
  return false;
	
}


//*********************************************************************
float numuCCOOFV_utils::GetToF(const AnaTrackB* track, AnaParticleB*& seg1, AnaParticleB*& seg2, float& sigma, TRandom3* gen){
  //*********************************************************************
	
  if (!track) return -1.;
	
  Float_t ToF_time_resolution = ND::params().GetParameterD("numuCCOOFVAnalysis.ToF.TimeResolution");
  Int_t add_ToF_on_targets    = ND::params().GetParameterI("numuCCOOFVAnalysis.AddToFOnTargets");
  std::vector< std::pair<AnaParticleB*, std::vector<float> > > detTiming;
	
  // find timing information in Targets
  for (int i=0; i<track->nTargetSegments; i++)
    if (track->TargetSegments[i]->IsReconstructed) {
      float true_time = track->TargetSegments[i]->PositionStart[3];
      float     sigma = sqrt(pow(3/sqrt(track->TargetSegments[i]->DeltaLYZ/25),2)+0.6*0.6); // 3ns/sqrt(NHits)
      float      time = gen->Gaus(true_time, sigma);
		
      std::vector<float> vec;
      vec.push_back(time); vec.push_back(sigma);
      detTiming.push_back(std::make_pair(track->TargetSegments[i], vec));
    }
	
  if (add_ToF_on_targets) {
    // find timing information in additionnal ToF
    if (track->nTPCSegments && track->TPCSegments[0]) {
      if (SubDetId::GetSubdetectorEnum(track->TPCSegments[0]->Detectors) == SubDetId::kTPCUp1   ||
	  SubDetId::GetSubdetectorEnum(track->TPCSegments[0]->Detectors) == SubDetId::kTPCUp2   ||
	  SubDetId::GetSubdetectorEnum(track->TPCSegments[0]->Detectors) == SubDetId::kTPCDown1 ||
	  SubDetId::GetSubdetectorEnum(track->TPCSegments[0]->Detectors) == SubDetId::kTPCDown2 ){
				
	if ( fabs(track->TPCSegments[0]->PositionStart[1]+316)<1 ||
	     fabs(track->TPCSegments[0]->PositionStart[1]-284)<1 ) {
					
	  float true_time = track->TPCSegments[0]->PositionStart[3];
	  float     sigma = ToF_time_resolution;
	  float      time = gen->Gaus(true_time, sigma);
					
	  std::vector<float> vec;
	  vec.push_back(time); vec.push_back(sigma);
	  detTiming.push_back(std::make_pair(track->TPCSegments[0], vec));
					
	}
      }
    }
  }
	
  // find timing information in FGDs
  for (int i=0; i<track->nFGDSegments; i++)
    if (track->FGDSegments[i]->IsReconstructed) {
      float true_time = track->FGDSegments[i]->PositionStart[3];
      float     sigma = sqrt(pow(3/sqrt(track->FGDSegments[i]->DeltaLYZ/25),2)+0.6+0.6); // 3ns/sqrt(NHits)
      float      time = gen->Gaus(true_time, sigma);
		
      std::vector<float> vec;
      vec.push_back(time); vec.push_back(sigma);
      detTiming.push_back(std::make_pair(track->FGDSegments[i], vec));
    }
	
  // find timing information in ECal
  for (int i=0; i<track->nECalSegments; i++) {
    if (track->ECalSegments[i]->IsReconstructed) {
      float true_time = track->ECalSegments[i]->PositionStart[3];
      float     sigma = 5; // 5ns
      float      time = gen->Gaus(true_time, sigma);
			
      std::vector<float> vec;
      vec.push_back(time); vec.push_back(sigma);
      detTiming.push_back(std::make_pair(track->ECalSegments[i], vec));
    }
  }
	
  // find timing information in ToF counters
  for (int i=0; i<track->nToFSegments; i++) {
    float true_time = track->ToFSegments[i]->PositionStart[3];
    float     sigma = ToF_time_resolution; // 0.6ns by default
    float      time = gen->Gaus(true_time, sigma);
		
    std::vector<float> vec;
    vec.push_back(time); vec.push_back(sigma);
    detTiming.push_back(std::make_pair(track->ToFSegments[i], vec));
  }
    
  // if we don't have 2, give up
  if (detTiming.size() < 2)
    return -999;
	
  float ToF = -999, max_t_over_s = 0;
  for (unsigned int i=0; i<detTiming.size(); i++){
    for (unsigned int j=i+1; j<detTiming.size(); j++){
			
      float  t1 = detTiming[i].second[0],  t2 = detTiming[j].second[0];
      float  s1 = detTiming[i].second[1],  s2 = detTiming[j].second[1];
      float t_over_s = fabs( (t2-t1)/sqrt(s1*s1+s2*s2) );
			
      if ( t_over_s > max_t_over_s ){
	max_t_over_s = t_over_s;
	ToF = t2-t1;
	seg1 = detTiming[i].first;
	seg2 = detTiming[j].first;
	sigma = sqrt(s1*s1+s2*s2);
      }
			
    } // end second loop on timing info
  } // end first loop on timing info
	
	
  // order with true time (first detector hit -> second detector hit)
  if (seg2->PositionStart[3]-seg1->PositionStart[3] < 0) {
    AnaParticleB* temp;
    temp = seg1;
    seg1 = seg2;
    seg2 = temp;
    ToF  = -ToF;
  }
	
  return ToF;
	
}

//*********************************************************************
float numuCCOOFV_utils::ComputeToFMass(float mom, float ToF, float length){
  //*********************************************************************
	
  double c = 299.792458; // mm/ns
  return mom*sqrt(pow(c*ToF/length, 2) -1);
	
}

//*********************************************************************
float numuCCOOFV_utils::ComputeToFTime(float mom, float mass, float length){
  //*********************************************************************
	
  double c = 299.792458; // mm/ns
  return length/c*sqrt(pow(mass/mom, 2) +1);
	
}
