#ifndef nueCCUtils_h
#define nueCCUtils_h

#include "BaseDataClasses.hxx"
#include "DataClasses.hxx"
#include "ToyBoxB.hxx"

#include <TFile.h>
#include <TRandom3.h>
#include <TLorentzVector.h>

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

namespace nueCCUtils{

  void AddCategories(bool AntiNu = false);
  int nueCCCategory(AnaEventB* event, AnaTrack* track, const SubDetId::SubDetEnum det, bool AntiNu);
  bool IsNuESignal(AnaTrueVertex* vtx, const SubDetId::SubDetEnum det, bool AntiNu);
  bool IsNuECC0pi(AnaTrueVertex* vtx, const SubDetId::SubDetEnum det, bool AntiNu);
  bool IsGammaSignal(AnaTrack* track, const SubDetId::SubDetEnum det, int parent);
  bool IsGammaSignalOOFGD(AnaTrack* track, const SubDetId::SubDetEnum det, int parent);

  TLorentzVector GetMomLorentzVector(const AnaTrackB& track, Float_t mass);
  
   /// Find the best e-e+ pair
  AnaTrackB* FindPairTrack(const AnaEventB& event, const AnaTrackB& track, Float_t delta_pos_max);

  Float_t* GetFittedPos(Float_t* start, Float_t* end, double z);
  Float_t GetDeltaPos(Float_t* hit, Float_t* fit);
  Float_t GetDeltaPos2(Float_t* hit, Float_t* fit);
  bool HGlobalMomFirst(AnaTrackB* a, AnaTrackB* b);
  bool isHMNT(const AnaTrackB* candidate, std::vector<AnaTrackB*> GroupTracks, bool lowangle);
  
  bool TPCElectronPIDCut(const AnaTrackB& candidate, Float_t Lelec);
  bool TPCMuonPIDCut(const AnaTrackB& candidate, Float_t Lmu);
  /// Check whether a track is forward going
  bool IsForward(const AnaTrackB& track);
}


#endif
