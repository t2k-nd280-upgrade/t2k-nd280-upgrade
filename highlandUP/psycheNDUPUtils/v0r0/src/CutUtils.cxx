#include "CutUtils.hxx"
#include "FiducialVolumeDefinition.hxx"
//#include "EventBoxId.hxx"
//#include "EventBoxTracker.hxx"
#include <TVector3.h>
#include <TH1F.h>
#include <TFile.h>

#include "Parameters.hxx"
#include "ConstituentsUtils.hxx"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


//**************************************************
bool cutUtils::TrackQualityCut(AnaTrackB& track){
//**************************************************

    if(track.TPCQualityCut > -1){
        return track.TPCQualityCut;
    }
    // Gets all segments in the closest TPC
  //  AnaTPCParticleB* TPCSegment = dynamic_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTPC(track));
  bool passed=false;
  if(cutUtils::DeltaLYZTPCCut(track)){

      passed=true;
        track.TPCQualityCut = passed;
        return passed;
    }
    else{
        track.TPCQualityCut = 0;
        return false;
    }
}


//**************************************************
bool cutUtils::TrackQualityCut(const AnaTrackB& track){
//**************************************************

    // Gets all segments in the closest TPC
    AnaTPCParticleB* TPCSegment = dynamic_cast<AnaTPCParticleB*>(anaUtils::GetSegmentWithMostNodesInClosestTPC(track));

    if (TPCSegment){
        bool passed = TPCTrackQualityCut(*TPCSegment);

        return passed;
    }
    else{
        return false;
    }
}
//**************************************************
bool cutUtils::TPCTrackQualityCut(const AnaTPCParticleB& tpcSeg){
//**************************************************

    Float_t cut = 200;  
    if (tpcSeg.DeltaLYZ > cut) 
        return true;
    else
        return false;
}

//**************************************************
bool cutUtils::MuonPIDCut(const AnaTrackB& track, bool prod5Cut){
//**************************************************
   Float_t cut1 = ND::params().GetParameterD("psycheNDUPUtils.cututils.pidmuoncut");
   Float_t cut2 = ND::params().GetParameterD("psycheNDUPUtils.cututils.pidmipcut");

    Float_t pcut = 500;

    Float_t PIDLikelihood[4];
    anaUtils::GetPIDLikelihood(track, PIDLikelihood, prod5Cut);

    if (((PIDLikelihood[0]+PIDLikelihood[3])/(1-PIDLikelihood[2]) > cut1 || track.Momentum > pcut )){//; && (PIDLikelihood[0]>cut2)){
        return true; 
    }

    return false;
}



//**************************************************
bool cutUtils::MuonECALPIDCut(const AnaTrackB& track, bool prod5Cut, TFile* file_ECAL_PDF){
//**************************************************

  AnaTrueParticleB* trueParticle = track.GetTrueParticle();

  /*
  std::ifstream in(TString::Format("%s/data/binning.txt",getenv("PSYCHENDUPUTILSROOT")), std::ios_base::in);
  bool goto_binning_cos = false;
  std::vector<double> binning_mom, binning_cos;

  double number;
  while (in >> number){
    if (binning_mom.size() != 0 && number<binning_mom.back())
      goto_binning_cos = true;

    if (goto_binning_cos)
      binning_cos.push_back(number);
    else
      binning_mom.push_back(number);
  }
  */

  //TVector3 init_pos = anaUtils::ArrayToTVector3(trueParticle->Position);

  //TH1F *h_binning = (TH1F*)file_ECAL_PDF->Get("h_binning");

  for (int i=0; i<trueParticle->nDetCrossings; i++) {
    AnaDetCrossingB* cross = trueParticle->DetCrossings[i];
    int ECAL_number =  SubDetId::GetECal(cross->Detector);
    if (ECAL_number<=1 or !cross->InActive) continue;

    TVector3 P   = anaUtils::ArrayToTVector3(cross->EntranceMomentum);
    TVector3 pos = anaUtils::ArrayToTVector3(cross->EntrancePosition);

    pos.Print();
    return false;

    float mom = P.Mag();
    float cos = 0;
    bool isInside = ((pos.X() < 1000 and pos.X() > -1000) &&
		     (pos.Y() < 1000 and pos.Y() > -1000) &&
		     (pos.Z() < 1000 and pos.Z() > -1000));

    int mom_bin = 0;
    int cos_bin = 0;

    /*
    for (unsigned int bm=0; bm<binning_mom.size(); bm++)
      if (mom<binning_mom[bm]) { mom_bin = bm; break; }
    for (unsigned int bc=0; bc<binning_cos.size(); bc++)
      if (cos<binning_cos[bc]) { cos_bin = bc; break; }
    */

    /*
    mom_bin = h_binning->GetXaxis()->FindBin(mom);
    cos_bin = h_binning->GetYaxis()->FindBin(cos);

    TH1F *h_MipEM  = (TH1F*)file_ECAL_PDF->Get(TString::Format("pdf_MipEM_%i_%i_%i", mom_bin, cos_bin, isInside));
    TH1F *h_L_ECAL = (TH1F*)file_ECAL_PDF->Get(TString::Format("pdf_L_ECAL_%i_%i_%i", mom_bin, cos_bin, isInside));
    TH1F *h_EMene  = (TH1F*)file_ECAL_PDF->Get(TString::Format("pdf_EMene_%i_%i_%i", mom_bin, cos_bin, isInside));
   
    float MipEM  = h_MipEM->GetRandom();
    float L_ECAL = h_L_ECAL->GetRandom();
    float EMene  = h_EMene->GetRandom();
    */
    
  }
  

  return false;
}

//**************************************************
bool cutUtils::ProtonPIDCut(const AnaParticleB& part){
//**************************************************

  (void)part;
  return true;
/*
    if (anaUtils::GetPIDLikelihood(track,2)>0.9)
        return true;
    else    
        return false;
*/
}

//**************************************************
bool cutUtils::FiducialCut(const AnaParticleB& track, const SubDetId::SubDetEnum det){
//**************************************************
    if (!SubDetId::GetDetectorUsed(track.Detectors, det)) return false;
    return FiducialCut(track.PositionStart, det);
}

//**************************************************
bool cutUtils::FiducialCut(const Float_t* pos, const SubDetId::SubDetEnum det){
//**************************************************

    return anaUtils::InFiducialVolume(det,pos);
}

//**************************************************
bool cutUtils::CommonVertexCut(const AnaParticleB& track1, const AnaParticleB& track2, int cutx, int cuty, int cutz){
//**************************************************

    if (fabs(track1.PositionStart[0]-track2.PositionStart[0]) > cutx ) return false; 
    if (fabs(track1.PositionStart[1]-track2.PositionStart[1]) > cuty ) return false; 
    if (fabs(track1.PositionStart[2]-track2.PositionStart[2]) > cutz ) return false;   

    return true;
}



//**************************************************
bool cutUtils::DeltaLYZTargetCut( AnaTrackB& track,const SubDetId::SubDetEnum det) {
//**************************************************

  float targetLYZCut = ND::params().GetParameterD("psycheNDUPUtils.cututils.targetLYZCut");
  
    AnaTargetParticleB* TargetSegment = dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet( track, det));
    if (TargetSegment)
    { if (TargetSegment->DeltaLYZ > targetLYZCut) {
        return true;
      }
    
  }
//    std::cout<<"false"<<std::endl;

  return false;
}


//**************************************************
bool cutUtils::DeltaLYZTPCCut(AnaTrackB& track)  {
//**************************************************


  // This cut check the existence of track with position inside the Fiducial Volume and
  // with a minimum number of hits
   float tpcLYZCut = ND::params().GetParameterD("psycheNDUPUtils.cututils.tpcLYZCut");

  // Cast the ToyBox to the appropriate type

  for (int subdet = 0; subdet < 7; subdet++) {
    AnaTPCParticleB* TPCSegment = dynamic_cast<AnaTPCParticleB*>(anaUtils::GetSegmentInDet( track, static_cast<SubDetId::SubDetEnum >(subdet)));
    if (TPCSegment) {

      if (TPCSegment->DeltaLYZ > tpcLYZCut) {
        return true;
      }
    }
  }
  return false;
}


//**************************************************
bool cutUtils::DeltaLYZTargetORTPCCut(AnaTrackB& track,const SubDetId::SubDetEnum det)  {
//**************************************************


  // This cut check the existence of track with position inside the Fiducial Volume and
  // with a minimum number of hits
   float tpcLYZCut = ND::params().GetParameterD("psycheNDUPUtils.cututils.tpcLYZCut");

  // Cast the ToyBox to the appropriate type

  for (int subdet = 0; subdet < 7; subdet++) {
    AnaTPCParticleB* TPCSegment = dynamic_cast<AnaTPCParticleB*>(anaUtils::GetSegmentInDet( track, static_cast<SubDetId::SubDetEnum >(subdet)));
    if (TPCSegment) {

      if (TPCSegment->DeltaLYZ > tpcLYZCut) {
        return true;
      }
    }
  }
  float targetLYZCut = ND::params().GetParameterD("psycheNDUPUtils.cututils.targetLYZCut");
  
    AnaTargetParticleB* TargetSegment = dynamic_cast<AnaTargetParticleB*>(anaUtils::GetSegmentInDet( track, det));
    if (TargetSegment)
    { if (TargetSegment->DeltaLYZ > targetLYZCut) {
        return true;
      }
    }

  return false;
}


//___________________________________________________________________________ 
bool cutUtils::IsCCQE(int neut_reaction_mode)
{
  return neut_reaction_mode ==  1;
}
//___________________________________________________________________________ 
bool cutUtils::IsCC1pi(int neut_reaction_mode)
{
  bool is_cc1pi = (neut_reaction_mode == 11 ||
                   neut_reaction_mode == 12 ||
                   neut_reaction_mode == 13);
  return is_cc1pi;
}
//___________________________________________________________________________ 
bool cutUtils::IsCCcoh(int neut_reaction_mode)
{
  return neut_reaction_mode ==  16;
}
//___________________________________________________________________________ 
bool cutUtils::Is2p2h(int neut_reaction_mode)
{
  return neut_reaction_mode ==  2;
}
//___________________________________________________________________________
bool cutUtils::IsCCoth(int neut_reaction_mode)                               
{
  bool is_ccoth = (neut_reaction_mode == 17 ||
                     neut_reaction_mode == 21 ||
                     neut_reaction_mode == 22 ||
                     neut_reaction_mode == 23 ||
                     neut_reaction_mode == 26);
  return is_ccoth;
}
//___________________________________________________________________________ 
bool cutUtils::IsNC(int neut_reaction_mode)
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
int cutUtils::GetReacAll(int neut_reaction_mode){
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
