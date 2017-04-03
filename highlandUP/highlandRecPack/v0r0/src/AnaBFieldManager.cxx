#include "AnaBFieldManager.hxx"
#include "Units.hxx"
#include "math.h"
//// The static member pointer to the singleton.
AnaBFieldManager* AnaBFieldManager::fAnaBFieldManager = NULL;

//*****************************************************************************
AnaBFieldManager& ND::bman(){
//*****************************************************************************

  return AnaBFieldManager::Get();

}
//*****************************************************************************
AnaBFieldManager::AnaBFieldManager() {
//*****************************************************************************

  fFieldStrengthX = ND::params().GetParameterD("highlandRecPack.BField.FieldStrengthX") * units::tesla;

  fFieldStrengthY = 0;
  fFieldStrengthZ = 0;

  //Initiaze values for the field map used so far by the ND280MC
  fYokeOuterWidth = 5870 * units::mm;
  fYokeOuterHeight = 6086 * units::mm;
  fYokeOuterLength = 7489 * units::mm;
  fYokeInnerWidth = 3824 * units::mm;
  fYokeInnerHeight = 4040 * units::mm;
  fYokeInnerLength = 7489 * units::mm;
  fCoilOuterWidth = 3824 * units::mm;
  fCoilOuterHeight = 4020 * units::mm;
  fCoilOuterLength = 7489 * units::mm;
  fCoilInnerWidth = 3824 * units::mm;
  fCoilInnerHeight = 3540 * units::mm;
  fCoilInnerLength = 7009 * units::mm;
  
  fDebug = (bool)ND::params().GetParameterI("highlandRecPack.Debug");
  
}

//*****************************************************************************
AnaBFieldManager& AnaBFieldManager::Get(void) {
//*****************************************************************************

  if (!fAnaBFieldManager) {
  	fAnaBFieldManager = new AnaBFieldManager;
  }
 
  return *fAnaBFieldManager;
}



//***********************************************************************
//****************** MC *************************************************

//********************************************************
TVector3 AnaBFieldManager::GetMCFieldValue(const TVector3& pos) {
//********************************************************
  
  TVector3 BField(0,0,0);

   if (fabs(pos.X()) > fYokeOuterWidth/2.) return BField;
   if (fabs(pos.Y()) > fYokeOuterHeight/2.) return BField;
   if (fabs(pos.Z()) > fYokeOuterLength/2.) return BField;

   // This is the field strength we use for MC.
   // As backup we use the value from parameters file...
   double mcFieldStrengthX = fFieldStrengthX;
  
   if (fabs(pos.X()) < fCoilInnerWidth/2.
      && fabs(pos.Y()) < fCoilInnerHeight/2. 
      && fabs(pos.Z()) < fCoilInnerLength/2.) {
    // Inside the inner volume of the magnet.  
    // This will eventually come from a measured field map.
    BField.SetX(mcFieldStrengthX);
  } else if (fabs(pos.X()) < fCoilOuterWidth/2.
	     && fabs(pos.Y()) < fCoilOuterHeight/2. 
	     && fabs(pos.Z()) < fCoilOuterLength/2.) {
    // Inside the coil volume of the magnet.
    double Y = (fabs(pos.Y()) - fCoilInnerHeight/2.)
      / (fCoilOuterHeight/2. - fCoilInnerHeight/2.);
    double Z = (fabs(pos.z()) - fCoilInnerLength/2.)
      / (fCoilOuterLength/2. - fCoilInnerLength/2.);
    double depth = std::max(Y,Z);
    if (depth<0.0) depth = 0.0;
    if (depth>1.0) depth = 1.0;
    BField.SetX(mcFieldStrengthX * (1.0 - depth));
  }

  return BField;
}

