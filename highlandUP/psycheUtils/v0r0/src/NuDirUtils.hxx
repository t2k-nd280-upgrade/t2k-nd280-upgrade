#ifndef NuDirUtils_h
#define NuDirUtils_h

#include <TMath.h>
#include <TVector3.h>

// code from xianguo. method for calculating the reconstructed neutrino direction

namespace anaUtils{

  //in m
  extern Float_t BeamPipePosition[3];
  extern Float_t NeutrinoParentDecay[3];

  extern bool NeutrinoBeamLineParametersInitialized;

  void InitializeNeutrinoBeamLineParameters();

  void SetNeutrinoParentDecPointRec(TVector3 &vec);
  TVector3 CalcNuDir(TVector3 &nup0Global, TVector3 &nup1Local);
  TVector3 GetNuDirRec(const Float_t vertex_pos[]);
}

#endif
