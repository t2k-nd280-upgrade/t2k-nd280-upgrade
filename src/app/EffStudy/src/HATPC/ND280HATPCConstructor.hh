#ifndef ND280_HATPC_CONSTRUCTOR_HH_SEEN
#define ND280_HATPC_CONSTRUCTOR_HH_SEEN
// $Id: ND280HATPCConstructor.hh,v 1.0 2018/20/3 cjesus $
//

#include <G4ThreeVector.hh>
class G4LogicalVolume;

#include "ND280Constructor.hh"

#include "ND280RootPersistencyManager.hh"
#include "ExN02ND280XML.hh"

/// Create a bounding box to contain the off axis detector.  The method get
/// GetPiece() constructs a new unplaced magnet that is facing up along the Z
/// axis.  The calling code should then rotate the object into position and
/// and place the object into a large mother volume.
class ND280HATPCConstructor : public ND280Constructor {
public:
  ND280HATPCConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280Constructor(n,c) {Init();};
  ND280HATPCConstructor(G4String n, ND280Constructor* p)
    : ND280Constructor(n,p) {Init();};
  virtual ~ND280HATPCConstructor();
  
  /// Construct and return a G4 volume for the object.  This is a pure
  /// virtual function, which means it must be implemented by the inheriting
  /// classes.  This returns an unplaced logical volume which faces along
  /// the Z axis.
  virtual G4LogicalVolume* GetPiece(void);

  //Set the name that fixes the FC model
  void SetHATPCFCName(G4String w) {fHATPCFCName = w;}
  // Get the name of the FC model
  G4String GetHATPCFCName(void) {return fHATPCFCName;}

  /// Set the ParentName of the HATPC detector   
  void SetHATPCParentName(G4String w) {fHATPCParentName = w;}
  /// Get the Width of the HATPC detector
  G4String GetHATPCParentName(void) {return fHATPCParentName;}

  /// Set the Name of the HATPC detector   
  void SetHATPCName(G4String w) {fHATPCName = w;}
  /// Get the Name of the HATPC detector
  G4String GetHATPCName(void) {return fHATPCName;}

  /// Set the Width of the HATPC detector   
  void SetHATPCWidth(double w) {fHATPCWidth = w;}
  /// Get the Width of the HATPC detector
  double GetHATPCWidth(void) {return fHATPCWidth;}

  /// Set the Height of the HATPC detector   
  void SetHATPCHeight(double w) {fHATPCHeight = w;}
  /// Get the Height of the HATPC detector
  double GetHATPCHeight(void) {return fHATPCHeight;}

  /// Set the Length of the HATPC detector   
  void SetHATPCLength(double w) {fHATPCLength = w;}
  /// Get the Length of the HATPC detector
  double GetHATPCLength(void) {return fHATPCLength;}

  /// Set the DriftWidth of the HATPC detector   
  void SetHATPCDriftWidth(double w) {fHATPCDriftWidth = w;}
  /// Get the DriftWidth of the HATPC detector
  double GetHATPCDriftWidth(void) {return fHATPCDriftWidth;}

  /// Set the DriftHeight of the HATPC detector   
  void SetHATPCDriftHeight(double w) {fHATPCDriftHeight = w;}
  /// Get the DriftHeight of the HATPC detector
  double GetHATPCDriftHeight(void) {return fHATPCDriftHeight;}

  /// Set the DriftLength of the HATPC detector   
  void SetHATPCDriftLength(double w) {fHATPCDriftLength = w;}
  /// Get the DriftLength of the HATPC detector
  double GetHATPCDriftLength(void) {return fHATPCDriftLength;}

  /// Set the HATPCCathodeThickness of the HATPC detector
  void SetHATPCCathodeThickness(double w) {fHATPCCathodeThickness = w;}
  /// Get the HATPCCathodeThickness of the HATPC detector
  double GetHATPCCathodeThickness(void) {return fHATPCCathodeThickness;}

  /// Set the HATPCCO2Top of the HATPC detector
  void SetHATPCCO2Top(double w) {fHATPCCO2Top = w;}
  /// Get the HATPCCO2Top of the HATPC detector
  double GetHATPCCO2Top(void) {return fHATPCCO2Top;}

  /// Set the HATPCCO2Bottom of the HATPC detector
  void SetHATPCCO2Bottom(double w) {fHATPCCO2Bottom = w;}
  /// Get the HATPCCO2Bottom of the HATPC detector
  double GetHATPCCO2Bottom(void) {return fHATPCCO2Bottom;}

  /// Set the HATPCInnerBoxWall of the HATPC detector
  void SetHATPCInnerBoxWall(double w) {fHATPCInnerBoxWall = w;}
  /// Get the HATPCInnerBoxWall of the HATPC detector
  double GetHATPCInnerBoxWall(void) {return fHATPCInnerBoxWall;}

  /// Set the HATPCOuterBoxWall of the HATPC detector
  void SetHATPCOuterBoxWall(double w) {fHATPCOuterBoxWall = w;}
  /// Get the HATPCOuterBoxWall of the HATPC detector
  double GetHATPCOuterBoxWall(void) {return fHATPCOuterBoxWall;}

  /// Set the HATPCSteppingLimit of the HATPC detector
  void SetHATPCSteppingLimit(double w) {fHATPCSteppingLimit = w;}
  /// Get the HATPCSteppingLimit of the HATPC detector
  double GetHATPCSteppingLimit(void) {return fHATPCSteppingLimit;}

  /// Set the ActiveHATPCVerticalOffset of the HATPC detector
  void SetActiveHATPCVerticalOffset(double w) {fActiveHATPCVerticalOffset = w;}
  /// Get the ActiveHATPCVerticalOffset of the HATPC detector
  double GetActiveHATPCVerticalOffset(void) {return fActiveHATPCVerticalOffset;}

  /// Method to build the TPC Central Cathode.
  void BuildHATPCCentralCathode(G4LogicalVolume* v, double t);
  /// Method to build the HATPC Cages.
  void BuildHATPCCages(G4LogicalVolume* v);

private:

  G4String fHATPCParentName;
  G4String fHATPCName;
  G4String fHATPCFCName;

  //Size of the HATPC
  double fHATPCWidth;
  double fHATPCHeight;
  double fHATPCLength;

  //Size of the HATPC Drift
  double fHATPCDriftWidth;
  double fHATPCDriftHeight;
  double fHATPCDriftLength;

  double fHATPCCathodeThickness;
  double fHATPCCO2Top;
  double fHATPCCO2Bottom;
  double fHATPCInnerBoxWall;
  double fHATPCOuterBoxWall;

  double fHATPCSteppingLimit;
  double fActiveHATPCVerticalOffset;

  G4ThreeVector tpcMMTrans[2][12];
  G4RotationMatrix* tpcMMRot[2][12];
  G4ThreeVector tpcMMRotAxis;

//bool DebugHATPCMass;

  void Init(void);

//  ExN02ND280XML *fND280XMLInput;
};
#endif





