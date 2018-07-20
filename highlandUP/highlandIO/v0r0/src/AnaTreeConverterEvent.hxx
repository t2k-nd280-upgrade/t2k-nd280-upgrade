#ifndef AnaTreeConverterEvent_h
#define AnaTreeConverterEvent_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TClonesArray.h>
#include <TRefArray.h>
#include <TRef.h>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <set>
#include "InputConverter.hxx"
#include "DataClasses.hxx"
#include "TND280UpTrackPoint.hxx"
#include "TND280UpTrack.hxx"
#include "TND280UpVertex.hxx"
#include "TH2D.h"
#include "TND280UpEvent.hxx"
#include "BaseDataClasses.hxx"

#include "BinnedParams.hxx"

#include <TRandom.h>
#include "TRandom3.h"
#include "TSpline.h"

class AnaTreeConverterEvent: public InputConverter{

 public:

  AnaTreeConverterEvent();
  virtual ~AnaTreeConverterEvent();

  virtual bool Initialize();
  virtual Int_t GetSpill(Long64_t& entry, AnaSpillC*& spill);
  Int_t GetEvent(Long64_t& entry, AnaEventC*& event){(void)entry;(void)event; return 0;}

  /// Record the POT for the current spill, based on information in the AnaBeam
  /// member of the current AnaSpill.
  void IncrementPOTBySpill();

  virtual Int_t ReadEntries(Long64_t& entry);
  virtual bool AddFileToTChain(const std::string& inputString);

  //----------------
  virtual AnaSpillB*        MakeSpill()       { return new AnaSpill(); }
  virtual AnaBunch*         MakeBunch()       { return new AnaBunch(); }
  virtual AnaBeamB*         MakeBeam()        { return new AnaBeam(); }
  virtual AnaEventInfo*     MakeEventInfo()   { return new AnaEventInfo(); }
 
  virtual AnaTrueParticle*  MakeTrueParticle(){ return new AnaTrueParticle(); }
  virtual AnaTrackB*        MakeTrack()       { return new AnaTrack(); }
  virtual AnaParticleMomB*  MakeParticleMomB(){ return new AnaParticleMomB(); }

  virtual AnaTrueVertex*    MakeTrueVertex()  { return new AnaTrueVertex(); }
  virtual AnaParticle*      MakeParticle()    { return new AnaParticle(); }
  virtual AnaVertexB*       MakeVertex()      { return new AnaVertexB(); }
  virtual AnaParticleB*     MakeTPCTrack()    { return new AnaTPCParticleB(); }
  virtual AnaParticleB*     MakeTargetTrack() { return new AnaTargetParticleB(); }
  virtual AnaParticleB*     MakeFGDTrack()    { return new AnaFGDParticleB(); }
  virtual AnaParticleB*     MakeECalTrack()   { return new AnaECalParticleB(); }
  virtual AnaParticleB*     MakeToFTrack()    { return new AnaToFParticleB(); }

  virtual AnaDetCrossingB*        MakeAnaDetCrossing()      { return new AnaDetCrossingB();}

  // ----------------------------

  virtual void FillInfo(AnaSpill * spill, Int_t entry);
  virtual void FillBeamInfo(AnaBeam * beam);
  virtual void FillTrueInfo(AnaSpill * spill);
  virtual void FillBunchInfo(std::vector<AnaTrueVertexB*>& TrueVertices, AnaBunch * bunch);
  virtual void FillTrueParticleInfo(TND280UpVertex * trueVertex, TND280UpTrack * upTrack, AnaTrueParticleB * truePart);
  virtual void Fill_Tracks_Recon_From_True(AnaTrueParticleB* trueParticles, AnaTrack* reconParticle);

  virtual void FillVertexInfo(AnaTrueVertexB * truevertex, AnaVertexB* vertex, int bunch);
  virtual void FillTrueVertexInfo(TND280UpVertex * trueVertex, AnaTrueVertex * vertex);
  virtual void FindSegments(TND280UpTrack * upTrack, AnaTrueParticleB * truePart);
  AnaTrueObjectC* FindTrueParticle(Int_t g4id, std::vector<AnaTrueParticleB*>& trueParticles);

  virtual bool IsReconstructedFGD(float* pos_start, float* pos_end, double length, double theta);
  virtual bool IsReconstructedTarget(int pdg, double mom, double cos, double length);
  virtual bool IsReconstructedECal(TVector3 P, int PDG, TString det, bool useTPC, bool useFGD);

  virtual bool IdAsProton(int pdg, double mom);
  virtual bool ChargeConfused(AnaTrueParticleB* trueParticle);
  virtual void Merge(TND280UpTrack* t1, TND280UpTrack* t2);
  
protected:
  
  AnaSpill* _spill;
  
  std::string _previousFile;
  Int_t _previousRunID;
  Int_t _previousSubrunID;
  Int_t _previousRefEventID;

 protected:

  // TChains   
  TChain *ND280upEvents;
  TChain *FileIndexTree;
  TND280UpEvent *nd280UpEvent;
  Int_t Entries; 
  Int_t Counter;
  Bool_t _isMC;
  std::string _softwareVersion;

  std::vector<Int_t> _nhits_trk;
  std::vector<Float_t> _dedx_trk;
  std::vector<Int_t> _nhits_shw;
  std::vector<Float_t> _dedx_shw;

  TList *fListOfTracks;
  
  TFile* fefficiency_target;
  TH2D*  hefficiency_target_muon;
  TH2D*  hefficiency_target_pion;
  TH2D*  hefficiency_target_proton;

  TFile* fmisid_target;
  TH1F*  hmisid_muon_as_proton;
  TH1F*  hmisid_proton_as_muon;
  TH1F*  hmisid_pion_as_proton;
  TH1F*  hmisid_proton_as_pion;

  int TargetType;

  TFile *_file_charge_confusion;
  TSpline3 *elecspline;
  TSpline3 *muonspline;
  TSpline3 *pionspline;
  TSpline3 *protspline;

  BinnedParams *_ECal_FGDmatch_eff, *_ECal_reco_eff, *_TPC_ECal_eff;
  
  // Header's
  Int_t EventTime;
  Int_t TriggerWord;
  Float_t POTPerSpill;
};

#endif

