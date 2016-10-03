#include "NuDirUtils.hxx"
#include "Parameters.hxx"

namespace anaUtils{
  bool NeutrinoBeamLineParametersInitialized=false;
  Float_t BeamPipePosition[3]={0,0,0};
  Float_t NeutrinoParentDecay[3]={0,0,0};
}

//***************************************************************
void anaUtils::InitializeNeutrinoBeamLineParameters(){ 
//***************************************************************

  if (NeutrinoBeamLineParametersInitialized) return;

  BeamPipePosition[0] = ND::params().GetParameterD("psycheUtils.NeutrinoBeamLine.BeamPipePositionX");
  BeamPipePosition[1] = ND::params().GetParameterD("psycheUtils.NeutrinoBeamLine.BeamPipePositionY");
  BeamPipePosition[2] = ND::params().GetParameterD("psycheUtils.NeutrinoBeamLine.BeamPipePositionZ");


  NeutrinoParentDecay[0] = ND::params().GetParameterD("psycheUtils.NeutrinoBeamLine.AvgNuParentDecayPointX"); 
  NeutrinoParentDecay[1] = ND::params().GetParameterD("psycheUtils.NeutrinoBeamLine.AvgNuParentDecayPointY"); 
  NeutrinoParentDecay[2] = ND::params().GetParameterD("psycheUtils.NeutrinoBeamLine.AvgNuParentDecayPointZ"); 
  
  NeutrinoBeamLineParametersInitialized=true;
}

//***************************************************************
void anaUtils::SetNeutrinoParentDecPointRec(TVector3 &vec){ 
//***************************************************************

  InitializeNeutrinoBeamLineParameters();
  vec = TVector3(NeutrinoParentDecay); 
}

//***************************************************************
TVector3 anaUtils::CalcNuDir(TVector3 &nup0Global, TVector3 &nup1Local){
//***************************************************************

  InitializeNeutrinoBeamLineParameters();
  const TVector3 nd280Global(BeamPipePosition);

  //in m 
  const TVector3 nup0Local = nup0Global - nd280Global;

  TVector3 nuDirCalc = TVector3(nup1Local - nup0Local);
  nuDirCalc *= 1./nuDirCalc.Mag();

  return nuDirCalc;
}

//***************************************************************
TVector3 anaUtils::GetNuDirRec(const Float_t vertex_pos[]){
//***************************************************************

  //-------- nup0 
  //in m, mean of the distribution, 6B neutrino flux
  TVector3 nup0Global;
  anaUtils::SetNeutrinoParentDecPointRec(nup0Global);
  
  //------ nup1
  //in m
  TVector3 nup1Local(vertex_pos);
  nup1Local *= 0.001; //default mm

  return anaUtils::CalcNuDir(nup0Global, nup1Local);
}

