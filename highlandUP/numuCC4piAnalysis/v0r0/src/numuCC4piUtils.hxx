#ifndef numuCC4piUtils_h
#define numuCC4piUtils_h

#include "BaseDataClasses.hxx"
#include "DataClasses.hxx"
#include "ToyBoxB.hxx"

#include "TFile.h"

const Float_t LAFVmin[3] = {57.66, 57.66, 10.125};
const Float_t LAFVmax[3] = {57.66, 57.66, 0.};
const Float_t HAFVmin[3] = {57.66, 57.66, 10.125};
const Float_t HAFVmax[3] = {57.66, 57.66, 10.125};

const Float_t _FVdefminDsECal[3] = {40, 40, 25.};   // "z == 25." to reject peak near the DsECal beginning in both data and MC
const Float_t _FVdefmaxDsECal[3] = {40, 40, 18.17}; // reject last layer in both data and MC
const Float_t _FVdefminTarget2[3]   = {76.88, 38.44, 0.};     //9.61*8, 9.61*4
const Float_t _FVdefmaxTarget2[3]   = {76.88, 38.44, 10.125}; //9.61*8, 9.61*4
const Float_t _FVdefminFGD2[3]   = {76.88, 38.44, 0.};     //9.61*8, 9.61*4
const Float_t _FVdefmaxFGD2[3]   = {76.88, 38.44, 10.125}; //9.61*8, 9.61*4

namespace numuCC4pi_utils{

  int CC4piCategory(AnaTrack* candidate);
  int OOFVCategory(AnaTrack* candidate, int topo);
  //int GetFgdLayer(Float_t* pos);
  Float_t* GetFittedPos(Float_t* start, Float_t* end, double z);
  Float_t GetDeltaPos(Float_t* hit, Float_t* fit);
  Float_t GetDeltaPos2(Float_t* hit, Float_t* fit);
  bool HGlobalMomFirst(AnaTrackB* a, AnaTrackB* b);
  //bool HRangeMomFirst(AnaTrackB* a, AnaTrackB* b);
  bool isHMNT(const AnaTrackB* candidate, std::vector<AnaTrackB*> GroupTracks, bool lowangle);
	
  //bool VetoCut(int topo, const AnaEventB& event, AnaTrackB& candidate);
  //bool ExternalCut(int topo, const AnaEventC& event, AnaTrackB& candidate);
  bool MuonPIDCut(const AnaTrackB& candidate, bool ApplyMIP=true);
  /// Check whether a track is forward going
  bool IsForward(const AnaTrackB& track);

  void FindTPCPions(AnaEventC& event, ToyBoxB& boxB, SubDetId::SubDetEnum det);
  void FindIsoTargetPions(AnaEventC& event, ToyBoxB& boxB, SubDetId::SubDetEnum det);
  void FindMEPions(AnaEventC& event, ToyBoxB& boxB, SubDetId::SubDetEnum det);

  float GetToF(const AnaTrackB* track, AnaParticleB*& seg1, AnaParticleB*& seg2, TRandom3* gen);
  float ComputeToFMass(float mom, float ToF, float length);

}


#endif
