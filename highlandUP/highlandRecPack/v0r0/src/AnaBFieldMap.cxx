#include "AnaBFieldMap.hxx"


//*******************************************************
AnaBFieldMap::AnaBFieldMap():EVectorMap(3) {
//*******************************************************

// dimension 3 for the default vector

}

//*******************************************************
EVector AnaBFieldMap::compute_vector(const EVector& pos) const{
//*******************************************************

  // Here is where we should implement our field map
  EVector Bv(3,0);

  TVector3 bv = ND::bman().GetFieldValue(TVector3(pos[0],pos[1],pos[2]));

  Bv[0]=bv.X();
  Bv[1]=0;
  Bv[2]=0;

  return Bv;
  
}
