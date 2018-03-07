#define BaseDataClasses_C

#include "BaseDataClasses.hxx"
#include "TMath.h"
#include "BasicUtils.hxx"

//********************************************************************
AnaTargetParticleB::AnaTargetParticleB(){
//********************************************************************
    DeltaLYZ=-9999;
    IsReconstructed=false;
    IdAsProton=true;
}

//********************************************************************
AnaTargetParticleB::AnaTargetParticleB(const AnaTargetParticleB& seg):AnaParticleB(seg){
//********************************************************************
    SegLength=seg.SegLength;
    IsReconstructed=seg.IsReconstructed;
    IdAsProton=seg.IdAsProton;
}

//********************************************************************
void AnaTargetParticleB::Print() const{
//********************************************************************

    std::cout << "-------- AnaTargetParticleB --------- " << std::endl;

    AnaParticleB::Print();

    std::cout << "DeltaLYZ:          " << DeltaLYZ        << std::endl;
    std::cout << "IsReconstructed:          " <<  IsReconstructed        << std::endl;

}


//********************************************************************
AnaTargetParticleB::~AnaTargetParticleB(){
//********************************************************************

}

//********************************************************************
AnaFGDParticleB::AnaFGDParticleB(){
//********************************************************************
    DeltaLYZ=-9999;
    IsReconstructed=false;

}

//********************************************************************
AnaFGDParticleB::AnaFGDParticleB(const AnaFGDParticleB& seg):AnaParticleB(seg){
//********************************************************************
    SegLength=seg.SegLength;
    IsReconstructed=seg.IsReconstructed;

}

//********************************************************************
void AnaFGDParticleB::Print() const{
//********************************************************************

    std::cout << "-------- AnaFGDParticleB --------- " << std::endl;

    AnaParticleB::Print();

    std::cout << "DeltaLYZ:          " << DeltaLYZ        << std::endl;
    std::cout << "IsReconstructed:          " <<  IsReconstructed        << std::endl;

}

//********************************************************************
AnaFGDParticleB::~AnaFGDParticleB(){
//********************************************************************

}

//********************************************************************
AnaToFParticleB::AnaToFParticleB(){
//********************************************************************
    DeltaLYZ=-9999;
    IsReconstructed=false;
    Detector_name="";
}

//********************************************************************
AnaToFParticleB::AnaToFParticleB(const AnaToFParticleB& seg):AnaParticleB(seg){
//********************************************************************
    SegLength=seg.SegLength;
    IsReconstructed=seg.IsReconstructed;
    Detector_name=seg.Detector_name;
}

//********************************************************************
void AnaToFParticleB::Print() const{
//********************************************************************

    std::cout << "-------- AnaToFParticleB --------- " << std::endl;

    AnaParticleB::Print();

    std::cout << "DeltaLYZ:          " << DeltaLYZ        << std::endl;
    std::cout << "IsReconstructed:          " <<  IsReconstructed        << std::endl;

}

//********************************************************************
AnaToFParticleB::~AnaToFParticleB(){
//********************************************************************

}

//********************************************************************
AnaECalParticleB::AnaECalParticleB(){
//********************************************************************
    DeltaLYZ=-9999;
        IsReconstructed=false;

}

//********************************************************************
AnaECalParticleB::AnaECalParticleB(const AnaECalParticleB& seg):AnaParticleB(seg){
//********************************************************************
    SegLength=seg.SegLength;
    IsReconstructed=seg.IsReconstructed;
    IsReconstructedForTiming=seg.IsReconstructedForTiming;
}

//********************************************************************
void AnaECalParticleB::Print() const{
//********************************************************************

    std::cout << "-------- AnaECalParticleB --------- " << std::endl;

    AnaParticleB::Print();

    std::cout << "DeltaLYZ:          " << DeltaLYZ        << std::endl;
    std::cout << "IsReconstructed:          " <<  IsReconstructed        << std::endl;

}

//********************************************************************
AnaECalParticleB::~AnaECalParticleB(){
//********************************************************************

}

//********************************************************************
AnaTPCParticleB::AnaTPCParticleB():AnaParticleMomB() {
//********************************************************************

    EFieldRefitMomentum  = -999;

    dEdxMeas        = -999;
    dEdxexpMuon     = -999;
    dEdxexpEle      = -999;
    dEdxexpPion     = -999;
    dEdxexpProton   = -999;
    dEdxSigmaMuon   = -999;
    dEdxSigmaEle    = -999;
    dEdxSigmaPion   = -999;
    dEdxSigmaProton = -999;
    DeltaLYZ=-999;
    SegLength=-999;

}

//********************************************************************
AnaTPCParticleB::AnaTPCParticleB(const AnaTPCParticleB& seg):AnaParticleMomB(seg){
//********************************************************************

    EFieldRefitMomentum       = seg.EFieldRefitMomentum;

    dEdxMeas      = seg.dEdxMeas; 
    dEdxexpMuon   = seg.dEdxexpMuon; 
    dEdxexpEle    = seg.dEdxexpEle; 
    dEdxexpPion   = seg.dEdxexpPion;  
    dEdxexpProton = seg.dEdxexpProton; 

    dEdxSigmaMuon   = seg.dEdxSigmaMuon; 
    dEdxSigmaEle    = seg.dEdxSigmaEle; 
    dEdxSigmaPion   = seg.dEdxSigmaPion; 
    dEdxSigmaProton = seg.dEdxSigmaProton;
    DeltaLYZ=seg.DeltaLYZ;
    SegLength=seg.SegLength;
}

//********************************************************************
void AnaTPCParticleB::Print() const{
//********************************************************************

    std::cout << "-------- AnaTPCParticleB --------- " << std::endl;

    AnaParticleMomB::Print();

    std::cout << "DeltaLYZ:          " << DeltaLYZ        << std::endl;
    std::cout << "SegLength:          " << SegLength        << std::endl;


}

//********************************************************************
AnaTPCParticleB::~AnaTPCParticleB(){
//********************************************************************
}

//********************************************************************
AnaParticleMomE::AnaParticleMomE(){
//********************************************************************
    SmearedMomentum  = -999;
    MomentumError  = -999;

    Charge            = -999;
    Momentum          = -999;
    MomentumEnd       = -999;
}

//********************************************************************
AnaParticleMomE::~AnaParticleMomE(){
//********************************************************************

}

//********************************************************************
AnaParticleMomE::AnaParticleMomE(const AnaParticleMomE& part){
//********************************************************************
    SmearedMomentum  = part.SmearedMomentum;
    MomentumError  = part.MomentumError;

    Charge            = part.Charge;
    Momentum          = part.Momentum;
    MomentumEnd       = part.MomentumEnd;
}

//********************************************************************
void AnaParticleMomE::Print() const{
//********************************************************************
    std::cout << "SmearedMomentum       " <<SmearedMomentum     <<std::endl; 
    std::cout << "MomentumError       " <<MomentumError     <<std::endl; 
 
    std::cout << "Charge:               " << Charge             << std::endl;
    std::cout << "Momentum:             " << Momentum           << std::endl;
    std::cout << "MomentumEnd:          " << MomentumEnd        << std::endl;
 
}

//********************************************************************
AnaParticleMomB::AnaParticleMomB():AnaParticleB(),AnaParticleMomE(){
//********************************************************************

}

//********************************************************************
AnaParticleMomB::~AnaParticleMomB(){
//********************************************************************

}

//********************************************************************
AnaParticleMomB::AnaParticleMomB(const AnaParticleMomB& part):AnaParticleB(part),AnaParticleMomE(part){
//********************************************************************

}

//********************************************************************
bool AnaParticleMomB::CompareMomentum(const AnaParticleMomB* t1, const AnaParticleMomB* t2){
//********************************************************************

    // function to sort particles in decreasing momentum order

    // set null pointer to be sorted the very last.
    Float_t m1 = -3e6;
    Float_t m2 = -3e6;
    if (t1 != NULL) m1 = t1->Momentum;
    if (t2 != NULL) m2 = t2->Momentum;

    // Also send inf to the end
    if (! TMath::Finite(m1)) m1 = -2e6;
    if (! TMath::Finite(m2)) m2 = -2e6;

    // Set nan to be -1e6 so they are sorted last. Leaving them as nan can cause
    // bad things to happen...
    if (m1 != m1) m1 = -1e6;
    if (m2 != m2) m2 = -1e6;

    return m1 > m2;
}


//********************************************************************
void AnaParticleMomB::Print() const{
//********************************************************************

    std::cout << "-------- AnaParticleMomB --------- " << std::endl;

    AnaParticleB::Print();
    AnaParticleMomE::Print();
}

//********************************************************************
AnaParticleB::AnaParticleB(): AnaRecObjectC(){
//********************************************************************

    NHits             = -999;
    Detectors             = 0;


    anaUtils::ReserveArray(DirectionStart, 3);
    anaUtils::ReserveArray(PositionStart,  4);
    anaUtils::ReserveArray(PositionEnd,    4);
    anaUtils::ReserveArray(DirectionEnd,   3);
    EDeposit=-999;
    CosTheta=-999;
    Original  = NULL;
}

//********************************************************************
AnaParticleB::~AnaParticleB(){
//********************************************************************

}

//********************************************************************
AnaParticleB::AnaParticleB(const AnaParticleB& part): AnaRecObjectC(part){
//********************************************************************

    NHits             = part.NHits;

    anaUtils::CopyArray(part.DirectionStart,  DirectionStart, 3);
    anaUtils::CopyArray(part.DirectionEnd,    DirectionEnd,   3);
    anaUtils::CopyArray(part.PositionStart,   PositionStart,  4);
    anaUtils::CopyArray(part.PositionEnd,     PositionEnd,    4);
    Detectors =part.Detectors;
    EDeposit=part.EDeposit;
    CosTheta=part.CosTheta;
    // A pointer to the original particle
    Original = &part;
}

//**************************************************
bool AnaParticleB::CompareNHits(const AnaParticleB* t1, const AnaParticleB* t2){
//**************************************************
    return t1->NHits > t2->NHits;
}

//********************************************************************
void AnaParticleB::Print() const{
//********************************************************************

    std::cout << "-------- AnaParticleB --------- " << std::endl;

    AnaRecObjectC::Print();

    std::cout << "NHits:                " << NHits              << std::endl;
    std::cout << "Detecotrs:                " << Detectors             << std::endl;
    std::cout << "EDeposit:                " << EDeposit             << std::endl;


    dump_array4(PositionStart);
    dump_array4(PositionEnd);

    dump_array3(DirectionStart);
    dump_array3(DirectionEnd);
}

//********************************************************************
AnaTrackB::AnaTrackB(): AnaParticleMomB(){
//********************************************************************

    Index               = -999;
    MomentumFlip        = -999;
    anaUtils::ReserveArray(DirectionStartFlip, 3);

    TPCQualityCut     = -999;

    nTargetSegments = 0;
    nFGDSegments = 0;
    nTPCSegments = 0;
    nECalSegments = 0;
    nToFSegments = 0;
    
    EKin=-999;
    TargetSegmentsVect.clear();
    FGDSegmentsVect.clear();
    TPCSegmentsVect.clear();
    ECalSegmentsVect.clear();
    ToFSegmentsVect.clear();  

    ToF_reco_time = -999; 
    ToF_true_time = -999; 
    ToF_sigma_time = -999;

    ToF_firstDet = SubDetId::kInvalid;
    ToF_secondDet = SubDetId::kInvalid;

    ToF_reco_length = -999;
    ToF_true_length = -999;
    ToF_sigma_length = -999;

    ToF_reco_mass = -999;
    ToF_sigma_mass = -999;

    HasFlipped=false;

}

//********************************************************************
AnaTrackB::~AnaTrackB(){
//********************************************************************

    for (Int_t i=0;i<nTargetSegments;i++){
        delete TargetSegments[i];    
        TargetSegments[i] = NULL;
    }
    for (Int_t i=0;i<nFGDSegments;i++){
        delete FGDSegments[i];    
        FGDSegments[i] = NULL;
    }

    for (Int_t i=0;i<nTPCSegments;i++){
        delete TPCSegments[i];    
        TPCSegments[i] = NULL;
    }
    for (Int_t i=0;i<nECalSegments;i++){
        delete ECalSegments[i];    
        ECalSegments[i] = NULL;
    }
    for (Int_t i=0;i<nToFSegments;i++){
        delete ToFSegments[i];    
        ToFSegments[i] = NULL;
    }    

    if (TargetSegmentsVect.size()>0  && nTargetSegments ==0) for (UInt_t i=0;i<TargetSegmentsVect.size(); i++) delete TargetSegmentsVect[i];
    if (FGDSegmentsVect.size()>0  && nFGDSegments ==0) for (UInt_t i=0;i<FGDSegmentsVect.size(); i++) delete FGDSegmentsVect[i];
    if (TPCSegmentsVect.size()>0  && nTPCSegments ==0) for (UInt_t i=0;i<TPCSegmentsVect.size(); i++) delete TPCSegmentsVect[i];
    if (ECalSegmentsVect.size()>0  && nECalSegments ==0) for (UInt_t i=0;i<ECalSegmentsVect.size(); i++) delete ECalSegmentsVect[i];
        if (ToFSegmentsVect.size()>0  && nToFSegments ==0) for (UInt_t i=0;i<ToFSegmentsVect.size(); i++) delete ToFSegmentsVect[i];

    nTargetSegments = 0;
    nFGDSegments    = 0;
    nTPCSegments    = 0;
    nECalSegments   = 0;
    nToFSegments    = 0;

    TargetSegmentsVect.clear();
    FGDSegmentsVect.clear();
    TPCSegmentsVect.clear();
    ECalSegmentsVect.clear();
    ToFSegmentsVect.clear();

}

//********************************************************************
AnaTrackB::AnaTrackB(const AnaTrackB& track):AnaParticleMomB(track){
//********************************************************************

  Index             = track.Index;
  MomentumFlip      = track.MomentumFlip;
  anaUtils::CopyArray(track.DirectionStartFlip,  DirectionStartFlip, 3);

  TPCQualityCut     = track.TPCQualityCut;

  nTargetSegments  = track.nTargetSegments;
  nFGDSegments     = track.nFGDSegments;
  nTPCSegments     = track.nTPCSegments;
  nECalSegments    = track.nECalSegments;
  nToFSegments     = track.nToFSegments;

  EKin=track.EKin;
  for (Int_t i=0;i<track.nTargetSegments;i++)
    TargetSegments[i] = track.TargetSegments[i]->Clone();

  for (Int_t i=0;i<track.nFGDSegments;i++)
    FGDSegments[i] = track.FGDSegments[i]->Clone();
  for (Int_t i=0;i<track.nTPCSegments;i++)
    TPCSegments[i] = track.TPCSegments[i]->Clone();

  for (Int_t i=0;i<track.nECalSegments;i++)
    ECalSegments[i] = track.ECalSegments[i]->Clone();

  for (Int_t i=0;i<track.nToFSegments;i++)
    ToFSegments[i] = track.ToFSegments[i]->Clone();


  TargetSegmentsVect.clear();
  FGDSegmentsVect.clear();
  TPCSegmentsVect.clear();
  ECalSegmentsVect.clear();
  ToFSegmentsVect.clear();

  ToF_reco_time = track.ToF_reco_time; 
  ToF_true_time = track.ToF_true_time; 
  ToF_sigma_time = track.ToF_sigma_time;

  ToF_firstDet = track.ToF_firstDet;
  ToF_secondDet = track.ToF_secondDet;

  ToF_reco_length = track.ToF_reco_length;
  ToF_true_length = track.ToF_true_length;
  ToF_sigma_length = track.ToF_sigma_length;

  ToF_reco_mass = track.ToF_reco_mass;
  ToF_sigma_mass = track.ToF_sigma_mass;

  HasFlipped = track.HasFlipped;

}

//********************************************************************
void AnaTrackB::Print() const{
//********************************************************************

    std::cout << "-------- AnaTrackB --------- " << std::endl;

    AnaParticleMomB::Print();

    std::cout << "MomentumFlip:         " << MomentumFlip       << std::endl;
    std::cout << "EKin:                " << EKin       << std::endl;

    dump_array3(DirectionStartFlip);

    std::cout << "NTargets:                " << nTargetSegments       << std::endl;
    std::cout << "NFGDs:                " << nFGDSegments       << std::endl;
    std::cout << "NTPCs:                " << nTPCSegments       << std::endl;

}

//********************************************************************
AnaDetCrossingB::AnaDetCrossingB(){
//********************************************************************

    InActive = 0;
    Detector = 0;

    anaUtils::ReserveArray(EntrancePosition, 4);
    anaUtils::ReserveArray(ExitPosition,     4);
  
    anaUtils::ReserveArray(EntranceMomentum, 3);
    anaUtils::ReserveArray(ExitMomentum,     3);

    SegLength = -999;

    DeltaLYZ = -999;

    EDeposit= -999;


}
//********************************************************************
void AnaDetCrossingB::Print() const{
//********************************************************************

    std::cout << "-------- AnaDetCrossingB:: --------- " << std::endl;

    std::cout << "InActive:                " << InActive              << std::endl;
    std::cout << "Detector:                " << Detector              << std::endl;   
    std::cout << "Detector full name:      " << Detector_name         << std::endl;   
    std::cout << "DeltaLYZ:                " << DeltaLYZ              << std::endl;
    std::cout << "SegLength:               " << SegLength             << std::endl;
    std::cout << "EDeposit:                " << EDeposit              << std::endl;

    dump_array4(EntrancePosition);
    dump_array4(ExitPosition);
    
    dump_array3(EntranceMomentum);
    dump_array3(ExitMomentum);
}
//********************************************************************
AnaDetCrossingB::~AnaDetCrossingB(){
//********************************************************************

};

//********************************************************************
AnaDetCrossingB::AnaDetCrossingB(const AnaDetCrossingB& cross){
//********************************************************************

    Detector = cross.Detector;
    Detector_name = cross.Detector_name;
    InActive = cross.InActive;

    anaUtils::CopyArray(cross.EntrancePosition,  EntrancePosition,   4);
    anaUtils::CopyArray(cross.ExitPosition,      ExitPosition,       4);

    anaUtils::CopyArray(cross.EntranceMomentum,  EntranceMomentum,   3);
    anaUtils::CopyArray(cross.ExitMomentum,      ExitMomentum,       3);

    SegLength = cross.SegLength;

    DeltaLYZ = cross.DeltaLYZ;

    EDeposit= cross.EDeposit;


}

//********************************************************************
AnaTrueParticleB::AnaTrueParticleB():AnaTrueObjectC(){
//********************************************************************

    PDG           = -999;
    ParentID      = 0; // These are 0 as it has a special meaning
    GParentID     = 0; // These are 0 as it has a special meaning
    ParentPDG     = 0; // These are 0 as it has a special meaning
    GParentPDG    = 0;
    stopped =-999;
    anaUtils::ReserveArray(Position, 4);
    anaUtils::ReserveArray(PositionEnd, 4);
    anaUtils::ReserveArray(Direction, 3);

    Momentum      = -999;
    Charge        = -999;
    VertexID      = -999;
    Length        = -999;
    EDeposit      = -999;
    EKin          = -999;
    CosTheta      = -999;
    TrueVertex    = NULL;
    nDetCrossings = 0;
    DetCrossings  = NULL;
    DetCrossingsVect.clear();

    Merged=false;
}

//********************************************************************
AnaTrueParticleB::~AnaTrueParticleB(){
//********************************************************************

  for (Int_t i=0;i<nDetCrossings;i++)
    DetCrossings[i] = NULL;  
  
  for (UInt_t i = 0; i < DetCrossingsVect.size(); i++)  
    delete DetCrossingsVect[i];
  DetCrossingsVect.clear();
  
  nDetCrossings = 0;
  DetCrossings = NULL;
  
}

//********************************************************************
AnaTrueParticleB::AnaTrueParticleB(const AnaTrueParticleB& truePart):AnaTrueObjectC(truePart){
  //********************************************************************

    PDG        = truePart.PDG;
    ParentID   = truePart.ParentID;
    GParentID  = truePart.GParentID;
    ParentPDG  = truePart.ParentPDG;
    GParentPDG = truePart.GParentPDG;
    stopped=truePart.stopped;
    anaUtils::CopyArray(truePart.Position, Position, 4);
    anaUtils::CopyArray(truePart.PositionEnd, PositionEnd, 4);
    anaUtils::CopyArray(truePart.Direction, Direction, 3);

    Momentum   = truePart.Momentum;
    Charge     = truePart.Charge;
    Length   = truePart.Length;
    EDeposit     = truePart.EDeposit;
    EKin        = truePart.EKin;
    VertexID   = truePart.VertexID;
    TrueVertex = truePart.TrueVertex;
    CosTheta = truePart.CosTheta;
    /// store for each subdetector
    /// if the true truePart enter the active volume of the subdetector i
    /// what is the entrance position of the subdetector i
    ///  "    "  "   exit       "               "        

    anaUtils::CreateArray(DetCrossings, truePart.nDetCrossings);
    for (Int_t i=0;i<truePart.nDetCrossings;i++){
      DetCrossings[i] = truePart.DetCrossings[i]->Clone();
    }
    nDetCrossings = truePart.nDetCrossings;
    
    DetCrossingsVect.clear();
    for (Int_t i=0; i<truePart.DetCrossingsVect.size(); i++)
      DetCrossingsVect.push_back(truePart.DetCrossingsVect[i]->Clone());

    Merged=truePart.Merged;

}

//********************************************************************
void AnaTrueParticleB::Print() const{
//********************************************************************

    std::cout << "-------- AnaTrueParticleB --------- " << std::endl;

    AnaTrueObjectC::Print();

    std::cout << "PDG:        " << PDG << std::endl;
    std::cout << "ParentPDG:  " << ParentPDG << std::endl;
    std::cout << "GParentPDG: " << GParentPDG << std::endl;
    std::cout << "Momentum:   " << Momentum << std::endl;
    std::cout << "Position:   " << Position[0]  << " " << Position[1]  << " " << Position[2] << " " << Position[3] << std::endl;
    std::cout << "Direction:  " << Direction[0] << " " << Direction[1] << " " << Direction[2] << std::endl;
    std::cout << "VertexID:   " << VertexID << std::endl;
    std::cout << "Length:   " << Length << std::endl;
    std::cout << "EDeposit:   " << EDeposit << std::endl;
    std::cout << "EKin:   " << EKin << std::endl;

}

//********************************************************************
AnaTrueVertexB::AnaTrueVertexB():AnaTrueObjectC(){
//********************************************************************

    Detector = 0;

    NuEnergy      = -999;
    NuMom         = -999;

    NuPDG         = -999;
    TargetPDG     = -999;
    Bunch         = -999;

    anaUtils::ReserveArray(Position, 4);

    TrueParticles = NULL;
    nTrueParticles = 0;

    TrueParticlesVect.clear();
}

//********************************************************************
AnaTrueVertexB::~AnaTrueVertexB(){
//********************************************************************

  // Must delete array of pointers, since we re-create this every time we apply a selection
  if(TrueParticles != NULL) delete [] TrueParticles;
  TrueParticles = NULL;      

  TrueParticlesVect.clear();
}

//********************************************************************
AnaTrueVertexB::AnaTrueVertexB(const AnaTrueVertexB& vertex):AnaTrueObjectC(vertex){
//********************************************************************

    NuEnergy      = vertex.NuEnergy;
    NuMom         = vertex.NuMom;
    NuPDG         = vertex.NuPDG;
    TargetPDG     = vertex.TargetPDG;
    Bunch         = vertex.Bunch;
    Detector      = vertex.Detector;

    anaUtils::CopyArray(vertex.Position, Position, 4);


    TrueParticlesVect.clear();
    for (UInt_t i = 0; i < vertex.TrueParticlesVect.size(); i++) {
        TrueParticlesVect.push_back(vertex.TrueParticlesVect[i]);
    }
    anaUtils::CreateArray(TrueParticles, vertex.nTrueParticles);
    for (Int_t i = 0; i < vertex.nTrueParticles; i++) {
        TrueParticles[i]=vertex.TrueParticles[i];
    }

    nTrueParticles = vertex.nTrueParticles;

//    TrueParticlesVect.clear();
}

//********************************************************************
void AnaTrueVertexB::Print() const{
//********************************************************************

    std::cout << "-------- AnaTrueVertexB --------- " << std::endl;

    AnaTrueObjectC::Print();

    std::cout << "NuPDG:            " << NuPDG << std::endl;
    std::cout << "TargetPDG:        " << TargetPDG << std::endl;
    std::cout << "NuEnergy:         " << NuEnergy << std::endl;
    std::cout << "NuMom:            " << NuMom << std::endl;

    std::cout << "Bunch:            " << Bunch << std::endl;
    std::cout << "Detector:         " << Detector << std::endl;
    std::cout << "Position:         " << Position[0]  << " " << Position[1]  << " " << Position[2] << " " << Position[3] << std::endl;
    std::cout << "NTrueParticles:      " << nTrueParticles << std::endl;
}

//********************************************************************
AnaDelayedClustersB::AnaDelayedClustersB(){
//********************************************************************
    MinTime = -999;
    MaxTime = -999;

    NHits = -999;
    RawChargeSum = -999;
}

//********************************************************************
AnaDelayedClustersB::AnaDelayedClustersB(const AnaDelayedClustersB& clusters){
//********************************************************************

    MinTime = clusters.MinTime;
    MinTime = clusters.MaxTime;
    NHits   = clusters.NHits;
    RawChargeSum = clusters.RawChargeSum;
}

//********************************************************************
AnaBunchB::AnaBunchB():AnaBunchC(){
//********************************************************************

    Vertices.clear();
    Particles.clear();

    DelayedClusters=NULL;
    nDelayedClusters=0;
}

//********************************************************************
AnaBunchB::~AnaBunchB(){
//********************************************************************

    for (UInt_t i=0;i<Vertices.size();i++)
        delete Vertices[i];

    Vertices.clear();

    for (UInt_t i=0;i<Particles.size();i++)
        delete Particles[i];

    Particles.clear();

    for (Int_t i=0;i<nDelayedClusters;i++){
      delete DelayedClusters[i];
      DelayedClusters[i] = NULL;
    }
    nDelayedClusters = 0;
    if (DelayedClusters) delete [] DelayedClusters;
    DelayedClusters = NULL;    
}

//********************************************************************
AnaBunchB::AnaBunchB(const AnaBunchB& bunch):AnaBunchC(bunch){
//********************************************************************

    Vertices.clear();
    for (UInt_t i=0;i<bunch.Vertices.size();i++)
        Vertices.push_back(bunch.Vertices[i]->Clone());

    Particles.clear();
    for (UInt_t i=0;i<bunch.Particles.size();i++){
      //  if (bunch.Particles[i]) {
      //      bunch.Particles[i]->Print();
            Particles.push_back(bunch.Particles[i]->Clone());
        //}
    }
    nDelayedClusters = 0;

    //  anaUtils::CreateArray(DelayedClusters, bunch.nDelayedClusters);
    
    DelayedClusters = new AnaDelayedClustersB*[bunch.nDelayedClusters];
    for(int i = 0; i < bunch.nDelayedClusters; ++i){
      DelayedClusters[i] = NULL;
    }
        
    for (Int_t i=0;i<bunch.nDelayedClusters;i++){
      DelayedClusters[nDelayedClusters] = bunch.DelayedClusters[i]->Clone();
      nDelayedClusters++;
    }
    
}

//********************************************************************
void AnaBunchB::Print() const{
//********************************************************************

    std::cout << "-------- AnaBunchB --------- " << std::endl;

    AnaBunchC::Print();

    std::cout << "NVertices:        " << (int)Vertices.size()  << std::endl;
    std::cout << "NParticles:       " << (int)Particles.size()  << std::endl;
    std::cout << "NDelayedClusters: " << nDelayedClusters  << std::endl;
}

//********************************************************************
AnaVertexB* AnaBunchB::GetPrimaryVertex(){
//********************************************************************

  if ((int)Vertices.size()==0) return NULL;

  AnaVertexB* vertex0 = Vertices.front();
  if (vertex0->PrimaryIndex == -1) {
    if (Bunch != vertex0->Bunch)
        std::cout << "WARNING: this vertex is not in this bunch " << Bunch << std::endl;
    return (Vertices.front());
  }

  std::vector<AnaVertexB*>::const_iterator it;
  for (it = Vertices.begin(); it != Vertices.end(); it++) {
    if ( (*it)->PrimaryIndex ==-1 ) {
      std::cout << "WARNING: here you are the PrimaryVertex with PrimaryIndex -1, but they are not properly sorted!" << std::endl;
      AnaVertexB* thisvertex= *it;
      if (Bunch != thisvertex->Bunch)
          std::cout << "WARNING: this vertex is not in this bunch " << Bunch << std::endl;
      return (*it);
    }
  }

  std::cout << "WARNING: Primary Vertex not found, here you are the vertex with PrimaryIndex: " << Vertices.front()->PrimaryIndex << std::endl;
  if (Bunch != vertex0->Bunch)
      std::cout << "WARNING: this vertex is not in this bunch " << Bunch << std::endl;
  return (Vertices.front());
}

//********************************************************************
bool AnaBunchB::IsEmpty() const{
//********************************************************************
  return (Particles.empty() && Vertices.empty());
}


//********************************************************************
AnaSpillB::AnaSpillB():AnaSpillC(){
//********************************************************************

    GeomID=999;
    NTotalTrueVertices = 999;
    NTotalTrueParticles   = 999;
    TrueVertices.clear();    
    TrueParticles.clear();    
    OutOfBunch = NULL;
    EventInfo = NULL;
    Beam = NULL;
//    DataQuality = NULL;
    //    isClone = false;
}

//********************************************************************
AnaSpillB::~AnaSpillB(){
//********************************************************************

    // TrueVertex's are not cloned. Only delete them in the raw spill
    if (!isClone){
        for (UInt_t i=0;i<TrueVertices.size();i++)
            delete TrueVertices[i];    
    }

    TrueVertices.clear();    

    // TrueParticles are not cloned.  Only delete them in the raw spill
    if (!isClone){
      for (UInt_t i=0;i<TrueParticles.size();i++)
            delete TrueParticles[i];    
    }

    TrueParticles.clear();    


    if (OutOfBunch)
        delete OutOfBunch;

    if (EventInfo)
        delete EventInfo;

    if (Beam)
        delete Beam;

}

//********************************************************************
AnaSpillB::AnaSpillB(const AnaSpillB& spill):AnaSpillC(spill){
//********************************************************************

    GeomID=spill.GeomID;

    NTotalTrueVertices = spill.NTotalTrueVertices;
    NTotalTrueParticles   = spill.NTotalTrueParticles;
    
    // Don't clone truth
    TrueVertices.clear();
    for (UInt_t i=0;i<spill.TrueVertices.size();i++)
        TrueVertices.push_back(spill.TrueVertices[i]);

    // Don't clone truth
    TrueParticles.clear();
    for (UInt_t i=0;i<spill.TrueParticles.size();i++)
        TrueParticles.push_back(spill.TrueParticles[i]);


    EventInfo = NULL;
    if (spill.EventInfo)
        EventInfo = spill.EventInfo->Clone();

    Beam = NULL;
    if (spill.Beam)
        Beam = spill.Beam->Clone();

    OutOfBunch = NULL;
    if (spill.OutOfBunch)
        OutOfBunch = spill.OutOfBunch->Clone();
}

//********************************************************************
void AnaSpillB::RedoLinks(){
//********************************************************************

    //--------- This codes redo all links between objects after cloning or reading MiniTree -------------

    std::vector<AnaBunchC*> allBunches = Bunches;
    if (OutOfBunch) allBunches.push_back(OutOfBunch);

    for (UInt_t i=0;i<allBunches.size();i++){
      AnaBunchB* bunch = static_cast<AnaBunchB*>(allBunches[i]);

      for (UInt_t j=0;j<bunch->Particles.size();j++){
        AnaParticleB* part = bunch->Particles[j];
        // AnaTrueParticle::ReconParticles
        // AnaTrueVertex::ReconParticles
        associateParticleToTrueParticle(part);
      }

      for (UInt_t j=0;j<bunch->Vertices.size();j++){
        AnaVertexB* vertex = bunch->Vertices[j];
        // AnaTrueVertex::ReconVertices
        associateVertexToTrueVertex(vertex);
        for (Int_t k=0;k<vertex->nParticles;k++){
          bool found=false;
          if (!vertex->Particles[k]) continue; // Particles in a vertex can be NULL in some cases (see oaAnalysisTreeConverter::FindParticle)
          for (UInt_t l=0;l<bunch->Particles.size();l++){
            if (vertex->Particles[k]->UniqueID == bunch->Particles[l]->UniqueID){
              // AnaVertexB::Particles
              vertex->Particles[k] = bunch->Particles[l];
              // AnaParticle::ReconVertices
              associateVertexToParticle(vertex->Particles[k], vertex);
              found=true;
              break;
            }
          }
          if (!found){
            for (UInt_t i2=0;i2<allBunches.size();i2++){
              if ((Int_t)i2==bunch->Bunch) continue;
              AnaBunchB* bunch2 = static_cast<AnaBunchB*>(allBunches[i2]);
              for (UInt_t l=0;l<bunch2->Particles.size();l++){
                if (vertex->Particles[k]->UniqueID == bunch2->Particles[l]->UniqueID){
                  // AnaVertexB::Particles
                  vertex->Particles[k] = bunch2->Particles[l];
                  // AnaParticle::ReconVertices
                  associateVertexToParticle(vertex->Particles[k], vertex);
                  found=true;
                  break;
                }                
              }
              if (found) break;
            }
          }
        }
      }
    }
}

//********************************************************************
void AnaSpillB::Print() const{
//********************************************************************

    std::cout << "-------- AnaSpillB --------- " << std::endl;

    if (EventInfo)
      EventInfo->Print();
    else
      std::cout << "NO Event Info available !!!" << std::endl;
    std::cout << "GeomID:              " << GeomID << std::endl;
    std::cout << "NBunches:            " << (int)Bunches.size() << std::endl;
    std::cout << "NTotalTrueVertices:  " << NTotalTrueVertices << std::endl;
    std::cout << "NSavedTrueVertices:  " << (int)TrueVertices.size() << std::endl;
    std::cout << "NTotalTrueParticles:    " << NTotalTrueParticles << std::endl;
    std::cout << "NSavedTrueParticles:    " << (int)TrueParticles.size() << std::endl;
    if (Beam){
      std::cout << "Good Spill:          " << Beam->GoodSpill << std::endl;
      std::cout << "POT since last Spill:" << Beam->POTSincePreviousSavedSpill << std::endl;
    }
    else
      std::cout << "NO Beam Info available !!!" << std::endl;
}

//********************************************************************
void AnaSpillB::CopyArraysIntoVectors(){
//********************************************************************

    // Add OutOfBunch to the vector of bunches
    std::vector<AnaBunchC*> bunches = Bunches;
    if (OutOfBunch) bunches.push_back(OutOfBunch);

    for (std::vector<AnaBunchC*>::iterator it=bunches.begin();it!=bunches.end();it++){
      AnaBunchB* bunch = static_cast<AnaBunchB*>(*it);      
      for (UInt_t i=0;i<bunch->Particles.size();i++){
        AnaTrackB* track = dynamic_cast<AnaTrackB*>(bunch->Particles[i]);
        if (!track)  continue;

        track->TargetSegmentsVect.clear();
        for (Int_t j=0;j<track->nTargetSegments;j++)
          track->TargetSegmentsVect.push_back(track->TargetSegments[j]);   

	track->FGDSegmentsVect.clear();
        for (Int_t j=0;j<track->nFGDSegments;j++)
          track->FGDSegmentsVect.push_back(track->FGDSegments[j]);   

        track->TPCSegmentsVect.clear();
        for (Int_t j=0;j<track->nTPCSegments;j++)
          track->TPCSegmentsVect.push_back(track->TPCSegments[j]);

	track->ToFSegmentsVect.clear();
        for (Int_t j=0;j<track->nToFSegments;j++)
          track->ToFSegmentsVect.push_back(track->ToFSegments[j]);
      }      
      for (UInt_t i=0;i<bunch->Vertices.size();i++){
        AnaVertexB* vertex = bunch->Vertices[i];

        vertex->ParticlesVect.clear();
        for (Int_t j=0;j<vertex->nParticles;j++){
          vertex->ParticlesVect.push_back(vertex->Particles[j]);
        }
      }

    }


    for (std::vector<AnaTrueVertexB*>::iterator it=TrueVertices.begin();it!=TrueVertices.end();it++){
      AnaTrueVertexB* vertex = *it;
      vertex->TrueParticlesVect.clear();
      for (Int_t j=0;j<vertex->nTrueParticles;j++)
        vertex->TrueParticlesVect.push_back(vertex->TrueParticles[j]);
    }
    for (std::vector<AnaTrueParticleB*>::iterator it=TrueParticles.begin();it!=TrueParticles.end();it++){
      AnaTrueParticleB* truePart = *it;
      truePart->DetCrossingsVect.clear();
      for (Int_t j=0;j<truePart->nDetCrossings;j++)
        truePart->DetCrossingsVect.push_back(truePart->DetCrossings[j]);
    }

}

//*****************************************************************************
void AnaSpillB::CopyVectorsIntoArrays(){
//*****************************************************************************

  // Add OutOfBunch to the vector of bunches
  std::vector<AnaBunchC*> bunches = Bunches;
  if (OutOfBunch) bunches.push_back(OutOfBunch);

  // Copy the std::vectors into the arrays for AnaTrackB::XXXSegments
  for (std::vector<AnaBunchC*>::iterator it=bunches.begin();it!=bunches.end();it++){

    AnaBunchB* bunch = static_cast<AnaBunchB*>(*it);
    for (UInt_t i=0;i<bunch->Particles.size();i++){
      AnaTrackB* track = dynamic_cast<AnaTrackB*>(bunch->Particles[i]);
      if (!track)  continue;

      track->nTargetSegments=0;
      for (UInt_t j=0;j<track->TargetSegmentsVect.size();j++)
        track->TargetSegments[track->nTargetSegments++]=track->TargetSegmentsVect[j];

      track->nFGDSegments=0;
      for (UInt_t j=0;j<track->FGDSegmentsVect.size();j++)
        track->FGDSegments[track->nFGDSegments++]=track->FGDSegmentsVect[j];

      track->nTPCSegments=0;
      for (UInt_t j=0;j<track->TPCSegmentsVect.size();j++)
        track->TPCSegments[track->nTPCSegments++]=track->TPCSegmentsVect[j];

      track->nToFSegments=0;
      for (UInt_t j=0;j<track->ToFSegmentsVect.size();j++)
        track->ToFSegments[track->nToFSegments++]=track->ToFSegmentsVect[j];     
    }

    for (UInt_t i=0;i<bunch->Vertices.size();i++){
      AnaVertexB* vertex = bunch->Vertices[i];

      vertex->nParticles=0;
      anaUtils::CreateArray(vertex->Particles, (Int_t)vertex->ParticlesVect.size());
      for (UInt_t j=0;j<vertex->ParticlesVect.size();j++)
        vertex->Particles[vertex->nParticles++] = vertex->ParticlesVect[j];
    }
  }

  // Copy the std::vectors into the arrays for AnaTrueVertexB::TrueParticles
  for (std::vector<AnaTrueVertexB*>::iterator it=TrueVertices.begin();it!=TrueVertices.end();it++){
    AnaTrueVertexB* vertex = *it;
    vertex->nTrueParticles=0;
    anaUtils::CreateArray(vertex->TrueParticles, (Int_t)vertex->TrueParticlesVect.size());
    for (UInt_t j=0;j<vertex->TrueParticlesVect.size();j++)
      vertex->TrueParticles[vertex->nTrueParticles++] = vertex->TrueParticlesVect[j];
  }

  // Copy the std::vectors into the arrays for AnaTrueParticleB::DetCrossings
  for (std::vector<AnaTrueParticleB*>::iterator it=TrueParticles.begin();it!=TrueParticles.end();it++){
    AnaTrueParticleB* truePart = *it;
    truePart->nDetCrossings = 0;
    anaUtils::CreateArray(truePart->DetCrossings, (Int_t)truePart->DetCrossingsVect.size());
    for (UInt_t j=0;j<truePart->DetCrossingsVect.size();j++)
      truePart->DetCrossings[truePart->nDetCrossings++] = truePart->DetCrossingsVect[j];
  }

}

//********************************************************************
AnaEventInfoB::AnaEventInfoB(){
//********************************************************************

    Run       = -999;
    SubRun    = -999;
    Event     = -999;
    IsMC      = false;
    IsSand    = false;
}

//********************************************************************
AnaEventInfoB::~AnaEventInfoB(){
//********************************************************************


}

//********************************************************************
AnaEventInfoB::AnaEventInfoB(const AnaEventInfoB& spill){
//********************************************************************
    Run       = spill.Run;
    SubRun    = spill.SubRun;
    Event     = spill.Event;
    IsMC      = spill.IsMC;
    IsSand    = spill.IsSand;
}

//********************************************************************
void AnaEventInfoB::Print() const{
//********************************************************************

    std::cout << "-------- AnaEventInfoB --------- " << std::endl;

    std::cout << "IsMC:               " << IsMC << std::endl;
    std::cout << "IsSand:             " << IsSand << std::endl;
    std::cout << "SubRun:             " << SubRun << std::endl;
    std::cout << "Evt:                " << Event << std::endl;
}

//********************************************************************
void AnaEventInfoB::SetIsSandMC(){
//********************************************************************
    std::stringstream sRun;
    sRun << Run;
    IsSand = ((sRun.str())[4]=='7');
}

//********************************************************************
AnaBeamB::AnaBeamB(){
//********************************************************************

    GoodSpill                     = -999;
    POTSincePreviousSavedSpill    = -999;
    SpillsSincePreviousSavedSpill = -999;
}

//********************************************************************
AnaBeamB::AnaBeamB(const AnaBeamB& beam){
//********************************************************************

    GoodSpill                     = beam.GoodSpill;
    POTSincePreviousSavedSpill    = beam.POTSincePreviousSavedSpill;
    SpillsSincePreviousSavedSpill = beam.SpillsSincePreviousSavedSpill;
}

//********************************************************************
AnaVertexB::AnaVertexB():AnaRecObjectC(){
//********************************************************************

    PrimaryIndex = -999;

    anaUtils::ReserveArray(Position, 4);

    Original     = NULL;
    TrueVertex   = NULL;
    Particles = NULL;
    nParticles = 0;

    ParticlesVect.clear();
}

//********************************************************************
AnaVertexB::~AnaVertexB(){
//********************************************************************
    // Must delete array of pointers, since we re-create this every time we apply a selection
    if(Particles != NULL) delete [] Particles;
    Particles = NULL;

    ParticlesVect.clear();
}


//********************************************************************
AnaVertexB::AnaVertexB(const AnaVertexB& vertex):AnaRecObjectC(vertex){
//********************************************************************

    PrimaryIndex = vertex.PrimaryIndex;

    anaUtils::CopyArray(vertex.Position, Position, 4);

    anaUtils::CreateArray(Particles, vertex.nParticles);   
    anaUtils::CopyArray(vertex.Particles, Particles, vertex.nParticles);

    nParticles = vertex.nParticles;

    TrueVertex   = vertex.TrueVertex;


    // A ponter to the original vertex
    Original = &vertex;
    for (UInt_t i = 0; i < vertex.ParticlesVect.size(); i++) {
        AnaParticleB* particle_tmp = vertex.ParticlesVect[i];

        ParticlesVect.push_back(vertex.ParticlesVect[i]);
    }

   // ParticlesVect.clear();
}

//********************************************************************
bool AnaVertexB::ComparePrimaryIndex(const AnaVertexB* t1, const AnaVertexB* t2){
//********************************************************************

    // function to sort global vertices in increasing PrimaryIndex order

    // set null pointer to be 999 so they are sorted last
    Int_t m1 = 999;
    Int_t m2 = 999;
    if (t1 != NULL) m1 = t1->PrimaryIndex;
    if (t2 != NULL) m2 = t2->PrimaryIndex;

    // set nan to be 999 so they are sorted last
    if (m1 != m1) m1 = 999;
    if (m2 != m2) m2 = 999;

    // sort by PrimaryIndex
    if (m1 != m2 || ! t1 || ! t2) return m1 < m2;

    // else if same PrimaryIndex, sort by higher momentum constituent (->Particles should be already sorted by momentum)
    // TODO: we assume Particles are global tracks here, because not all particles have momentum
    if (t1->nParticles > 0 && t2->nParticles > 0) {
        if (t1->Particles[0] && t2->Particles[0] && t1->Particles[0] != t2->Particles[0])
          return static_cast<AnaTrackB*>(t1->Particles[0])->Momentum > static_cast<AnaTrackB*>(t2->Particles[0])->Momentum;
    }

    // else if same higher momentum particle, sort by number of constituent particles
    if (t1->nParticles != t2->nParticles) return t1->nParticles > t2->nParticles;

    // else if same constituents, sort by position.Z
    return t1->Position[2] < t2->Position[2];
}

//********************************************************************
void AnaVertexB::Print() const{
//********************************************************************

    std::cout << "-------- AnaVertexB --------- " << std::endl;

    AnaRecObjectC::Print();

    std::cout << "PrimaryIndex:        " << PrimaryIndex << std::endl;
    std::cout << "Position:            " << Position[0] << " " << Position[1] << " " << Position[2] << " " << Position[3] << std::endl;
    std::cout << "NReconParticles:     " << nParticles << std::endl;
}

//********************************************************************
AnaEventSummaryB::AnaEventSummaryB(): AnaEventSummaryC() {
//********************************************************************
      for(int i = 0; i < SampleId::kNSamples; ++i){
          LeptonCandidate[i]=NULL;
          RooVertexIndex[i] = -1;
          TrueVertex[i] = NULL;
          for(int j = 0; j < 4; ++j){
              VertexPosition[i][j] = -9999;
          }
      }
      EventSample = SampleId::kUnassigned;
}


//********************************************************************
void AnaEventSummaryB::ResetSummary(){
//********************************************************************

      for(int i = 0; i < SampleId::kNSamples; ++i){
        LeptonCandidate[i]=NULL;
          RooVertexIndex[i] = -1;
          TrueVertex[i] = NULL;
          for(int j = 0; j < 4; ++j){
              VertexPosition[i][j] = -9999;
          }
      }
      EventSample = SampleId::kUnassigned;
}

//********************************************************************
AnaEventB::AnaEventB():AnaEventC(){
//********************************************************************

    Particles = NULL;
    Vertices = NULL;
    TrueParticles = NULL;
    TrueVertices = NULL;
    nParticles = 0;
    nVertices = 0;
    nTrueParticles = 0;
    nTrueVertices = 0;
    Beam = NULL;

    DelayedClusters=NULL;
    nDelayedClusters=0;

    Summary = new AnaEventSummaryB();
}

//********************************************************************
AnaEventB::~AnaEventB(){
//********************************************************************

    for (Int_t i=0;i<nParticles;i++){
        delete Particles[i];
        Particles[i] = NULL;
    }
    nParticles = 0;    
    if (Particles) delete [] Particles;
    Particles = NULL;

    for (Int_t i=0;i<nVertices;i++){
        delete Vertices[i];
        Vertices[i] = NULL;
    }
    nVertices = 0;
    if (Vertices) delete [] Vertices;
    Vertices = NULL;    

    // TrueParticles are not cloned.  Only delete them in the raw spill
    if (!isClone){
        for (Int_t i=0;i<nTrueParticles;i++){
            delete TrueParticles[i];
            TrueParticles[i] = NULL;
        }
    }
    nTrueParticles = 0;
    if (TrueParticles) delete [] TrueParticles;
    TrueParticles = NULL;

    // TrueVertex's are not cloned. Only delete them in the raw spill
    if (!isClone){
        for (Int_t i=0;i<nTrueVertices;i++){
            delete TrueVertices[i];
            TrueVertices[i] = NULL;
        }
    }

    nTrueVertices = 0;
    if (TrueVertices) delete [] TrueVertices;
    TrueVertices = NULL;

    if (Beam){
        delete Beam;
        Beam = NULL;
    }
    for (Int_t i=0;i<nDelayedClusters;i++){
      delete DelayedClusters[i];
      DelayedClusters[i] = NULL;
    }
    nDelayedClusters = 0;
    if (DelayedClusters) delete [] DelayedClusters;
    DelayedClusters = NULL;

}

//********************************************************************
void AnaEventB::Copy(const AnaEventC& eventC, bool copyBunchInfo, bool cloneTruth){
//********************************************************************

//    AnaEventC::Copy(event,copyBunchInfo,cloneTruth);

    const AnaEventB& event = *static_cast<const AnaEventB*>(&eventC);

    if (copyBunchInfo){
        nParticles = 0;
        anaUtils::CreateArray(Particles, event.nParticles);
        for (Int_t i=0;i<event.nParticles;i++){
            Particles[nParticles] = event.Particles[i]->Clone();
            nParticles++;
        }
        nVertices = 0;
        anaUtils::CreateArray(Vertices, event.nVertices);
        for (Int_t i=0;i<event.nVertices;i++){
            Vertices[nVertices] = event.Vertices[i]->Clone();
            nVertices++;
        }
        //        Weight = event.Weight;
        Bunch = event.Bunch;
        //        Summary = event.Summary;
    }

    nTrueVertices = 0;
    anaUtils::CreateArray(TrueVertices, event.nTrueVertices);
    for (Int_t i=0;i<event.nTrueVertices;i++){
        if (cloneTruth){
            TrueVertices[nTrueVertices] = event.TrueVertices[i]->Clone();
            // must clear particles since we need to redo the links below
            TrueVertices[nTrueVertices]->nTrueParticles = 0;
            nTrueVertices++;
        }
        else{
            TrueVertices[nTrueVertices] = event.TrueVertices[i];
            nTrueVertices++;
        }
    }

    nTrueParticles = 0;
    anaUtils::CreateArray(TrueParticles, event.nTrueParticles);
    for (Int_t i=0;i<event.nTrueParticles;i++){
        if (cloneTruth){
            TrueParticles[nTrueParticles] = event.TrueParticles[i]->Clone();
            for(int j = 0; j < nTrueVertices; ++j){
                if(TrueParticles[nTrueParticles]->VertexID == TrueVertices[j]->ID){
                    // Associate the vertex to this particle
                    TrueParticles[nTrueParticles]->TrueVertex = TrueVertices[j];

                    // add this particle to the list of true particles in the true vertex
                    if(TrueVertices[j]->nTrueParticles == 0){
                        anaUtils::CreateArray(TrueVertices[j]->TrueParticles, event.nTrueParticles);
                    }
                    TrueVertices[j]->TrueParticles[TrueVertices[j]->nTrueParticles] = TrueParticles[nTrueParticles];	
                    TrueVertices[j]->nTrueParticles++;
                    break;
                }
            } 
            nTrueParticles++;
        }
        else{
            TrueParticles[nTrueParticles] = event.TrueParticles[i];
            nTrueParticles++;
        }
    }

    // Fill the FGD time bins vector
    nDelayedClusters = 0;
    //  anaUtils::CreateArray(DelayedClusters, event.nDelayedClusters);
    
    DelayedClusters = new AnaDelayedClustersB*[event.nDelayedClusters];
    for(int i = 0; i < event.nDelayedClusters; ++i){
    DelayedClusters[i] = NULL;
    }
    for (Int_t i=0;i<event.nDelayedClusters;i++){
      DelayedClusters[nDelayedClusters] = event.DelayedClusters[i]->Clone();
      nDelayedClusters++;
    }


    EventInfo = event.EventInfo;

    Beam = NULL;
    if (event.Beam)
        Beam = event.Beam->Clone();


}

//********************************************************************
AnaEventB::AnaEventB(const AnaEventB& event):AnaEventC(event){
//********************************************************************

    // copy bunch info but don't clone truth
    Copy(event, true, false);
}

//********************************************************************
void AnaEventB::Print() const{
//********************************************************************

    std::cout << "-------- AnaEventB --------- " << std::endl;

    EventInfo.Print();

    std::cout << "Bunch:              " << Bunch << std::endl;
    std::cout << "NParticles:         " << nParticles  << std::endl;
    std::cout << "NVertices:          " << nVertices  << std::endl;
    std::cout << "NTrueParticles:     " << nTrueParticles << std::endl;
    std::cout << "NTrueVertices:      " << nTrueVertices << std::endl;
    std::cout << "Good Spill:         " << Beam->GoodSpill << std::endl;
}


//*****************************************************************************
AnaEventB::AnaEventB(const AnaSpillB& spill, const AnaBunchB& bunch){
//*****************************************************************************

  nEventBoxes=0;
  for (UInt_t i=0;i<NMAXEVENTBOXES;i++)
    EventBoxes[i]=NULL;
  
  // The initial weight of the Event is 1;
  Weight=1;
  
  // Must create a summary object when we create an event
  // This is initialised to NULL and SampleId::kUnassigned, so you know it has not passed a selection
  Summary = new AnaEventSummaryB();


  UniqueID = 0;
  isClone = false;  
  Particles = NULL;
  Vertices = NULL;
  TrueParticles = NULL;
  TrueVertices = NULL;
  nParticles = 0;
  nVertices = 0;
  nTrueParticles = 0;
  nTrueVertices = 0;
  Beam = NULL;

  
  //------ Copy from Spill and Bunch ----------------

  Weight      = bunch.Weight;
  Bunch       = bunch.Bunch;
  EventInfo   = *spill.EventInfo;
  Beam        = spill.Beam->Clone();
 
  // Fill the recon particles vector
  nParticles = 0;
  anaUtils::CreateArray(Particles, bunch.Particles.size());
  for (UInt_t i=0;i<bunch.Particles.size();i++){
    Particles[nParticles] = bunch.Particles[i]->Clone();
    nParticles++;
  }

  // Fill the recon vertices vector
  nVertices = 0;
  anaUtils::CreateArray(Vertices, bunch.Vertices.size());
  for (UInt_t i=0;i<bunch.Vertices.size();i++){
    Vertices[nVertices] = bunch.Vertices[i]->Clone();
    nVertices++;
  }

  // Fill the true particles vector
  nTrueParticles = 0;
  anaUtils::CreateArray(TrueParticles, spill.TrueParticles.size());
  for (UInt_t i=0;i<spill.TrueParticles.size();i++){
    TrueParticles[nTrueParticles] = spill.TrueParticles[i]->Clone();
    nTrueParticles++;
  }

  // Fill the true vertices vector
  nTrueVertices = 0;
  anaUtils::CreateArray(TrueVertices, spill.TrueVertices.size());
  for (UInt_t i=0;i<spill.TrueVertices.size();i++){
    TrueVertices[nTrueVertices] = spill.TrueVertices[i]->Clone();
    nTrueVertices++;
  }

  // Fill the FGD time bins vector
  nDelayedClusters = 0;
  //  anaUtils::CreateArray(DelayedClusters, bunch.nDelayedClusters);

  DelayedClusters = new AnaDelayedClustersB*[bunch.nDelayedClusters];
  for(int i = 0; i < bunch.nDelayedClusters; ++i){
    DelayedClusters[i] = NULL;
  }

  for (Int_t i=0;i<bunch.nDelayedClusters;i++){
    DelayedClusters[nDelayedClusters] = bunch.DelayedClusters[i]->Clone();
    nDelayedClusters++;
  }


}

