#define DataClasses_C

#include "DataClasses.hxx"
#include "BasicUtils.hxx"
#include "TMath.h"

// define a constant value for uninitialised parameters
const Float_t  kFloatUnassigned = -999.;
const UInt_t   kUnassigned      = 0xDEADBEEF;
const short    kShortUnassigned = 0xFF;




//********************************************************************
AnaParticleE::AnaParticleE(){
//********************************************************************

  AveragedEdx   = -999;
  MomentumError = -999;

  NDOF          = -999;
  Chi2          = -999;
  Bunch         = -999;
  ReconPDG      = -999;

  MomentumEle         = -999;
  MomentumMuon        = -999;
  MomentumProton      = -999;
  MomentumErrorEle    = -999;
  MomentumErrorMuon   = -999;
  MomentumErrorProton = -999;


  ReconVertex = NULL;
  ReconVertices.clear();
  anaUtils::ReserveArray(DirectionAtVertex, 3);
  MomentumAtVertex    = -999;

}

//********************************************************************
AnaParticleE::~AnaParticleE(){
//********************************************************************

}

//********************************************************************
AnaParticleE::AnaParticleE(const AnaParticleE& track){
//********************************************************************

  AveragedEdx   = track.AveragedEdx;
  MomentumError = track.MomentumError;

  NDOF          = track.NDOF;
  Chi2          = track.Chi2;
  Bunch         = track.Bunch;
  ReconPDG      = track.ReconPDG;


  MomentumEle         = track.MomentumEle;
  MomentumMuon        = track.MomentumMuon;
  MomentumProton      = track.MomentumProton;
  MomentumErrorEle    = track.MomentumErrorEle;
  MomentumErrorMuon   = track.MomentumErrorMuon;
  MomentumErrorProton = track.MomentumErrorProton;


  // The most lower PrimaryIndex associated global vertex (if it exists).
  ReconVertex = NULL;//track.ReconVertex;

  ReconVertices.clear();
  anaUtils::CopyArray(track.DirectionAtVertex, DirectionAtVertex, 3);
  MomentumAtVertex = track.MomentumAtVertex;
}

//********************************************************************
void AnaParticleE::Print() const{
//********************************************************************

  std::cout << "-------- AnaParticleE --------- " << std::endl;

  std::cout << "Chi2:                    " << Chi2 << std::endl;
  std::cout << "MomentumMuon:            " << MomentumMuon << std::endl;
  std::cout << "MomentumEle:             " << MomentumEle << std::endl;
  std::cout << "MomentumProton:          " << MomentumProton << std::endl;

  std::cout << "Bunch:                   " << Bunch << std::endl;
  std::cout << "NReconVertices:          " << (int)ReconVertices.size() << std::endl;
  std::cout << "MomentumAtVertex:        " << MomentumAtVertex << std::endl;
  std::cout << "DirectionAtVertex:       " << DirectionAtVertex[0] << " " << DirectionAtVertex[1] << " " << DirectionAtVertex[2] << std::endl;
}

//********************************************************************
void AnaParticle::Print() const{
//********************************************************************

  std::cout << "-------- AnaParticle --------- " << std::endl;

  AnaParticleB::Print();
  AnaParticleE::Print();
}

//********************************************************************
AnaTrueVertex::AnaTrueVertex(): AnaTrueVertexB(){
//********************************************************************

  RooVtxIndex   = -999;
  RooVtxEntry   = -999;
  ReacCode      = -999;
  TargetPDG     = -999;
  anaUtils::ReserveArray(NuDir, 3);
  NuParentPDG   = -999;
  anaUtils::ReserveArray(NuParentDecPoint, 4);

  NBaryons      = -999;
  Q2            = -999;
  LeptonPDG     = -999;
  anaUtils::ReserveArray(TargetDir, 3);
  anaUtils::ReserveArray(LeptonDir, 3);
  anaUtils::ReserveArray(ProtonDir, 3);
  anaUtils::ReserveArray(PionDir, 3);
  TargetMom     = -999;
  LeptonMom     = -999;
  ProtonMom     = -999;
  PionMom       = -999;

  IsPauliBlocked = false;
  IsCohOnH = false;

  for (UInt_t i=0; i<(ParticleId::kLast+1); i++)
      NPrimaryParticles[i]=0;

  AccumLevel.clear();

  ReconParticles.clear();
  ReconVertices.clear();
}

//********************************************************************
AnaTrueVertex::AnaTrueVertex(const AnaTrueVertex& vertex):AnaTrueVertexB(vertex){
//********************************************************************

  RooVtxIndex   = vertex.RooVtxIndex;
  RooVtxEntry   = vertex.RooVtxEntry;
  ReacCode      = vertex.ReacCode;
  TargetPDG     = vertex.TargetPDG;
  anaUtils::CopyArray(vertex.NuDir, NuDir, 3);
  NuParentPDG   = vertex.NuParentPDG;
  anaUtils::CopyArray(vertex.NuParentDecPoint, NuParentDecPoint, 3);

  NBaryons      = vertex.NBaryons;
  Q2            = vertex.Q2;
  LeptonPDG     = vertex.LeptonPDG;
  anaUtils::CopyArray(vertex.TargetDir, TargetDir, 3);
  anaUtils::CopyArray(vertex.LeptonDir, LeptonDir, 3);
  anaUtils::CopyArray(vertex.ProtonDir, ProtonDir, 3);
  anaUtils::CopyArray(vertex.PionDir,   PionDir,   3);
  TargetMom     = vertex.TargetMom;
  LeptonMom     = vertex.LeptonMom;
  ProtonMom     = vertex.ProtonMom;
  PionMom       = vertex.PionMom;

  IsPauliBlocked = vertex.IsPauliBlocked;
  IsCohOnH       = vertex.IsCohOnH;

  for (UInt_t i=0; i<(ParticleId::kLast+1); i++)
      NPrimaryParticles[i]=vertex.NPrimaryParticles[i];
  AccumLevel.resize(vertex.AccumLevel.size());
  for (UInt_t i=0;i<vertex.AccumLevel.size();i++){
    AccumLevel[i].resize(vertex.AccumLevel[i].size());
    for (UInt_t j=0;j<vertex.AccumLevel[i].size();j++){
      AccumLevel[i][j]=vertex.AccumLevel[i][j];
    }
  }

  ReconParticles.clear();
  ReconVertices.clear();
  }

//********************************************************************
void AnaTrueVertex::Print() const{
//********************************************************************

  std::cout << "-------- AnaTrueVertex --------- " << std::endl;

  AnaTrueVertexB::Print();

  std::cout << "RooVtxIndex:      " << RooVtxIndex << std::endl;
  std::cout << "RooVtxEntry:      " << RooVtxEntry << std::endl;
  std::cout << "ReacCode:         " << ReacCode << std::endl;
  std::cout << "TargetPDG:        " << TargetPDG << std::endl;


  std::cout << "NuDir:            " << NuDir[0]  << " " << NuDir[1]  << " " << NuDir[2] << std::endl;

  std::cout << "NuParentPDG:      " << NuParentPDG << std::endl;
  std::cout << "NuParentDecPoint: " << NuParentDecPoint[0] << " " << NuParentDecPoint[1]  << " " << NuParentDecPoint[2] << " " << NuParentDecPoint[3] << std::endl;


  std::cout << "NBaryons:         " << NBaryons << std::endl;
  std::cout << "LeptonPDG:        " << LeptonPDG << std::endl;
  std::cout << "Q2:               " << Q2 << std::endl;

  std::cout << "TargetDir:        " << TargetDir[0]  << " " << TargetDir[1]  << " " << TargetDir[2] << std::endl;
  std::cout << "LeptonDir:        " << LeptonDir[0]  << " " << LeptonDir[1]  << " " << LeptonDir[2] << std::endl;
  std::cout << "ProtonDir:        " << ProtonDir[0]  << " " << ProtonDir[1]  << " " << ProtonDir[2] << std::endl;
  std::cout << "PionDir:          " << PionDir[0]    << " " << PionDir[1]    << " " << PionDir[2]   << std::endl;

  std::cout << "TargetMom:        " << TargetMom << std::endl;
  std::cout << "LeptonMom:        " << LeptonMom << std::endl;
  std::cout << "ProtonMom:        " << ProtonMom << std::endl;
  std::cout << "PionMom:          " << PionMom << std::endl;

  std::cout << "NReconParticles:  " << (int)ReconParticles.size() << std::endl;
  std::cout << "NReconVertices:   " << (int)ReconVertices.size() << std::endl;
}

//********************************************************************
AnaTrueParticle::AnaTrueParticle(): AnaTrueParticleB(){
//********************************************************************

  PrimaryID     = -999;
  Purity        = -999;
  Bunch         = -999;
  VertexIndex   = -999;

}

//********************************************************************
AnaTrueParticle::AnaTrueParticle(const AnaTrueParticle& truePart):AnaTrueParticleB(truePart){
//********************************************************************

  PrimaryID  = truePart.PrimaryID;
  Purity     = truePart.Purity;
  Bunch      = truePart.Bunch;
  VertexIndex = truePart.VertexIndex;


}

//********************************************************************
void AnaTrueParticle::Print() const{
//********************************************************************

  std::cout << "-------- AnaTrueParticle --------- " << std::endl;

  AnaTrueParticleB::Print();

  std::cout << "VertexIndex:     " << VertexIndex << std::endl;
}

//********************************************************************
AnaTrack::AnaTrack():AnaTrackB(){
//********************************************************************

  Length    = -999;

}

//********************************************************************
AnaTrack::~AnaTrack(){
//********************************************************************

}

//********************************************************************
AnaTrack::AnaTrack(const AnaTrack& track):AnaTrackB(track),AnaParticleE(track){
//********************************************************************

  Length        = track.Length;
  
}

//********************************************************************
void AnaTrack::Print() const{
//********************************************************************

  std::cout << "-------- AnaTrack --------- " << std::endl;

  AnaParticleE::Print();
  AnaTrackB::Print();

    std::cout << "Length:               " << Length                      << std::endl;
}

//********************************************************************
AnaBunch::AnaBunch(){
//********************************************************************

}

//********************************************************************
AnaBunch::~AnaBunch(){
//********************************************************************

}

//********************************************************************
AnaBunch::AnaBunch(const AnaBunch& bunch):AnaBunchB(bunch){
//********************************************************************

}

//********************************************************************
void AnaBunch::Print() const{
//********************************************************************

  AnaBunchB::Print();

}


//********************************************************************
AnaSpill::AnaSpill():AnaSpillB(){
//********************************************************************

}

//********************************************************************
AnaSpill::~AnaSpill(){
//********************************************************************
}

//********************************************************************
AnaSpill::AnaSpill(const AnaSpill& spill):AnaSpillB(spill){
//********************************************************************
}

//********************************************************************
void AnaSpill::RedoLinks(){
//********************************************************************

  // Reset the true->Reco links since truth is not cloned and the vectors of ReconParticles and ReconVertices are already filled
  for (UInt_t i=0;i<TrueVertices.size();i++){
    (static_cast<AnaTrueVertex*>(TrueVertices[i]))->ReconParticles.clear();
    (static_cast<AnaTrueVertex*>(TrueVertices[i]))->ReconVertices.clear();    
  }
  for (UInt_t i=0;i<TrueParticles.size();i++)
    (static_cast<AnaTrueParticle*>(TrueParticles[i]))->ReconParticles.clear();

  // Redo the links
  AnaSpillB::RedoLinks();
}

//********************************************************************
void AnaSpill::associateVertexToParticle(AnaParticleB* particle, AnaVertexB* vertex) const{
//********************************************************************  
  (static_cast<AnaTrack*>(particle))->ReconVertices.push_back(vertex);
}

//********************************************************************
void AnaSpill::associateVertexToTrueVertex(AnaVertexB* vertex) const{
//********************************************************************
  
  if (vertex->TrueVertex)
    (static_cast<AnaTrueVertex*>(vertex->TrueVertex))->ReconVertices.push_back(vertex);
}

//********************************************************************
void AnaSpill::associateParticleToTrueParticle(AnaParticleB* particle) const{
//********************************************************************

  if (particle->TrueObject){
    (static_cast<AnaTrueParticle*>(particle->TrueObject))->ReconParticles.push_back(particle);

    if (particle->GetTrueParticle()->TrueVertex)
      (static_cast<AnaTrueVertex*>(particle->GetTrueParticle()->TrueVertex))->ReconParticles.push_back(particle);
  }
}

//********************************************************************
void AnaSpill::Print() const{
//********************************************************************

  std::cout << "-------- AnaSpill --------- " << std::endl;

  AnaSpillB::Print();
}


//********************************************************************
AnaEvent::AnaEvent():AnaEventB(){
//********************************************************************

}

//********************************************************************
AnaEvent::~AnaEvent(){
//********************************************************************

}

//********************************************************************
AnaEvent::AnaEvent(const AnaEvent& event):AnaEventB(event){
//********************************************************************

}

//*****************************************************************************
AnaEvent::AnaEvent(const AnaSpill& spill, const AnaBunch& bunch):AnaEventB(spill,bunch) {
//*****************************************************************************
}

//********************************************************************
void AnaEvent::Print() const{
//********************************************************************

  std::cout << "-------- AnaEvent --------- " << std::endl;

  AnaEventB::Print();
}

//********************************************************************
AnaVertex::AnaVertex():AnaVertexB(){
//********************************************************************

  Bunch        = -999;
  anaUtils::ReserveArray(Variance, 4);
  NDOF         = -999;
  Chi2         = -999;
}

//********************************************************************
AnaVertex::AnaVertex(const AnaVertex& vertex):AnaVertexB(vertex){
//********************************************************************

  Bunch        = vertex.Bunch;
  anaUtils::CopyArray(vertex.Variance, Variance, 4);
  NDOF         = vertex.NDOF;
  Chi2         = vertex.Chi2;
}

//********************************************************************
void AnaVertex::Print() const{
//********************************************************************

  std::cout << "-------- AnaVertex --------- " << std::endl;

  AnaVertexB::Print();

  std::cout << "Bunch:               " << Bunch << std::endl;
  std::cout << "Variance:            " << Variance[0] << " " << Variance[1] << " " << Variance[2] << " " << Variance[3] << std::endl;
  std::cout << "NDOF:                " << NDOF << std::endl;
  std::cout << "Chi2:                " << Chi2 << std::endl;
}

//********************************************************************
AnaEventInfo::AnaEventInfo():AnaEventInfoB(){
//********************************************************************

  EventTime = -999;
}

//********************************************************************
AnaEventInfo::AnaEventInfo(const AnaEventInfo& spill):AnaEventInfoB(spill){
//********************************************************************

  EventTime = spill.EventTime;
}

//********************************************************************
void AnaEventInfo::Print() const{
//********************************************************************

  std::cout << "-------- AnaEventInfo --------- " << std::endl;

  AnaEventInfoB::Print();

  std::cout << "EventTime:          " << EventTime << std::endl;
}



//********************************************************************
AnaBeam::AnaBeam(){
//********************************************************************

  POT           = -999;
  Spill         = -999;
  SpillNumber   = -999;
  BeamRunNumber = -999;
}

//********************************************************************
AnaBeam::AnaBeam(const AnaBeam& beam):AnaBeamB(beam){
//********************************************************************

  POT           = beam.POT;
  Spill         = beam.Spill;
  SpillNumber   = beam.SpillNumber;
  BeamRunNumber = beam.BeamRunNumber;
}
