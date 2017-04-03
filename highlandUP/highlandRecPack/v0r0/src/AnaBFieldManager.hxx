#ifndef AnaBFieldManager_hxx
#define AnaBFieldManager_hxx 1
#include <Units.hxx>
#include "TVector3.h"
#include "Parameters.hxx"

//! A Class that defines the magnetic field map in UA1 magnet
//! Currently MC map is used
class AnaBFieldManager{

  
public:
  
  //!default destructor
  virtual ~AnaBFieldManager() {};
  
  
  //! Get a pointer to the singleton instance
  static AnaBFieldManager& Get(void);
   
  //! get magnetic field for particular point,  for now it is just MC one
  TVector3 GetFieldValue(const TVector3& point){return GetMCFieldValue(point);};

private:
 
  AnaBFieldManager();
  
  //FROM ND280MC
  TVector3 GetMCFieldValue(const TVector3& pos);
  
  double fFieldStrengthX;
  double fFieldStrengthY;
  double fFieldStrengthZ;
  
  double fYokeOuterWidth; 
  double fYokeOuterHeight; 
  double fYokeOuterLength;
  double fYokeInnerWidth; 
  double fYokeInnerHeight; 
  double fYokeInnerLength;
  double fCoilOuterWidth; 
  double fCoilOuterHeight; 
  double fCoilOuterLength;
  double fCoilInnerWidth; 
  double fCoilInnerHeight; 
  double fCoilInnerLength;
  
  //! The static pointer to the singleton instance.
  static AnaBFieldManager* fAnaBFieldManager;

  //! debug
  bool fDebug;
};

namespace ND{
  
  AnaBFieldManager& bman(); 
};
#endif

