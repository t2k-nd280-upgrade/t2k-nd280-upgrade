#ifndef BasicUtils_h
#define BasicUtils_h

#include "BasicTypes.hxx"
#include "CoreUtils.hxx"
#include "BaseDataClasses.hxx"
#include "SubDetId.hxx"

/// This namespace contains useful functions for analyses
/// SHOULD CONSIDER TEMPLATING THE METHODS!
namespace anaUtils{

  void ClearArray(AnaTrackB* arr[], int n);
  void ClearArray(AnaParticleB* arr[], int n);
  void ClearArray(AnaTrueParticleB* arr[], int n);
  void ClearArray(AnaTargetParticleB* arr[], int n);
  void ClearArray(AnaFGDParticleB* arr[], int n);
  void ClearArray(AnaToFParticleB* arr[], int n);
  void ClearArray(AnaTPCParticleB* arr[], int n);

  void ReserveArray(AnaTrackB* arr[], int n);
  void ReserveArray(AnaParticleB* arr[], int n);
  void ReserveArray(AnaTrueParticleB* arr[], int n);
  void ReserveArray(AnaTargetParticleB* arr[], int n);
  void ReserveArray(AnaFGDParticleB* arr[], int n);
  void ReserveArray(AnaToFParticleB* arr[], int n);
  void ReserveArray(AnaTPCParticleB* arr[], int n);

  void ReclaimArray(AnaTrackB* arr[], int n, int size);
  void ReclaimArray(AnaParticleB* arr[], int n, int size);
  void ReclaimArray(AnaTrueParticleB* arr[], int n, int size);
  void ReclaimArray(AnaTargetParticleB* arr[], int n, int size);
  void ReclaimArray(AnaFGDParticleB* arr[], int n, int size);
  void ReclaimArray(AnaToFParticleB* arr[], int n, int size);
  void ReclaimArray(AnaTPCParticleB* arr[], int n, int size);

  void CopyArray( AnaTrackB** const     &src,     AnaTrackB**     &dest,  int n);


  void CopyArray(AnaRecObjectC**  tgtArr, AnaParticleB**     srcArr, int nObj);
  void CopyArray(AnaRecObjectC**  tgtArr, AnaTrackB**        srcArr, int nObj);
  void CopyArray(AnaTrueObjectC** tgtArr, AnaTrueParticleB** srcArr, int nObj);


  /// Create varaible sized arrays of pointers
  void CreateArray(AnaTrackB**          &tgtArr,  int nObj);
  void CreateArray(AnaTrueParticleB**      &tgtArr,  int nObj);
  void CreateArray(AnaVertexB**         &tgtArr,  int nObj);
  void CreateArray(AnaTrueVertexB**     &tgtArr,  int nObj);
  void CreateArray(AnaParticleB**       &tgtArr,  int nObj);
  void CreateArray(AnaTargetParticleB**       &tgtArr,  int nObj);
  void CreateArray(AnaFGDParticleB**       &tgtArr,  int nObj);
  void CreateArray(AnaToFParticleB**       &tgtArr,  int nObj);
  void CreateArray(AnaTPCParticleB**       &tgtArr,  int nObj);
  void CreateArray(AnaDetCrossingB**    &tgtArr,  int nObj);
  void CreateArray(SubDetId::SubDetEnum*  &tgtArr,  int nObj, SubDetId::SubDetEnum ini=SubDetId::kInvalid);

  void CopyArray(   AnaTrueParticleB** const &src, AnaTrueParticleB** &dest, int n);
  void CopyArray(   AnaParticleB**     const &src, AnaParticleB**     &dest, int n);
    
  void ResizeArray( AnaTrueParticleB**   &tgtArr,    int nObj);
  void ResizeArray( AnaTrackB**       &tgtArr,    int nObj);
    
}
#endif
