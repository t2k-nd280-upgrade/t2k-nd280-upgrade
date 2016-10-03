/*
   converter for DUNE NueTree input format

   A. Cervera AprilMay 2016
*/

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
//#include "LArSoftReaderHeaders.h"
#include <TRandom.h>

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
  virtual AnaEventInfo*    MakeEventInfo()   { return new AnaEventInfo(); }
 
  virtual AnaTrueParticle*  MakeTrueParticle(){ return new AnaTrueParticle(); }
  virtual AnaTrackB*  MakeTrack(){ return new AnaTrack(); }
  virtual AnaParticleMomB*  MakeParticleMomB(){ return new AnaParticleMomB(); }

  virtual AnaTrueVertex*    MakeTrueVertex()  { return new AnaTrueVertex(); }
  virtual AnaParticle*      MakeParticle()    { return new AnaParticle(); }
  virtual AnaVertexB*        MakeVertex()      { return new AnaVertexB(); }
  virtual AnaParticleB*        MakeTPCTrack()      { return new AnaTPCParticleB();}
  virtual AnaParticleB*        MakeTargetTrack()      { return new AnaTargetParticleB();}
  virtual AnaDetCrossingB*        MakeAnaDetCrossing()      { return new AnaDetCrossingB();}

  // ----------------------------

  virtual void FillInfo(AnaSpill * spill, Int_t entry);
  virtual void FillBeamInfo(AnaBeam * beam);
  virtual void FillTrueInfo(AnaSpill * spill);
  virtual void FillBunchInfo(std::vector<AnaTrueVertexB*>& TrueVertices, AnaBunch * bunch);
//  virtual void FillParticleInfo(std::vector<AnaTrueParticleB*>& trueParticles,           Int_t itrk, AnaParticle* part);
  virtual void FillTrueParticleInfo(TND280UpVertex * trueVertex, TND280UpTrack * upTrack, AnaTrueParticleB * truePart);
  virtual void Fill_Tracks_Recon_From_True(AnaTrueParticleB* trueParticles, AnaTrack* reconParticle);

 virtual void FillVertexInfo(AnaTrueVertexB * truevertex, AnaVertexB* vertex, int bunch);
  virtual void FillTrueVertexInfo(TND280UpVertex * trueVertex, AnaTrueVertex * vertex);
  virtual void FindSegments(TND280UpTrack * upTrack, AnaTrueParticleB * truePart);
 AnaTrueObjectC* FindTrueParticle(Int_t g4id, std::vector<AnaTrueParticleB*>& trueParticles);
 virtual bool GetEfficiency(double length,double theta);
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
  TH2D *hefficiency_target;
  Bool_t _isMC;
  std::string _softwareVersion;

  std::vector<Int_t> _nhits_trk;
  std::vector<Float_t> _dedx_trk;
  std::vector<Int_t> _nhits_shw;
  std::vector<Float_t> _dedx_shw;

  TList *fListOfTracks;
  TFile* fefficiency_target;
  // Header's
  Int_t EventTime;
  Int_t TriggerWord;
  Float_t POTPerSpill;
};



#endif
