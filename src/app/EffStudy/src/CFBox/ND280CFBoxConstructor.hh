#ifndef ND280_CFBOX_CONSTRUCTOR_HH_SEEN
#define ND280_CFBOX_CONSTRUCTOR_HH_SEEN
// $Id: ND280CFBoxConstructor.hh,v 1.9 2017/28/3 dsgalabe $
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
class ND280CFBoxConstructor : public ND280Constructor {
public:
  ND280CFBoxConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280Constructor(n,c) {Init();};
  ND280CFBoxConstructor(G4String n, ND280Constructor* p)
    : ND280Constructor(n,p) {Init();};
  virtual ~ND280CFBoxConstructor();
  
  /// Construct and return a G4 volume for the object.  This is a pure
  /// virtual function, which means it must be implemented by the inheriting
  /// classes.  This returns an unplaced logical volume which faces along
  /// the Z axis.
  virtual G4LogicalVolume* GetPiece(void);
  
  /// Set the width of the SuperFGD CF Box
  void SetCFBoxWidth(double w) {fCFBoxWidth = w;}
  /// Get the width of the SuperFGD CF Box
  double GetCFBoxWidth(void) {return fCFBoxWidth;}

  /// Set the height of the SuperFGD CF Box
  void SetCFBoxHeight(double w) {fCFBoxHeight = w;}
  /// Get the height of the SuperFGD CF Box
  double GetCFBoxHeight(void) {return fCFBoxHeight;}

  /// Set the length of the SuperFGD CF Box
  void SetCFBoxLength(double w) {fCFBoxLength = w;}
  /// Get the length of the SuperFGD CF Box
  double GetCFBoxLength(void) {return fCFBoxLength;}

  /// Set the thickness of the CFRP
  void SetCFBoxCFRPThickness(double w) {fCFBoxCFRPThickness = w;}
  /// Get the thickness of the CFRP
  double GetCFBoxCFRPThickness(void) {return fCFBoxCFRPThickness;}

  /// Set the thickness of the AIREX
  void SetCFBoxAIREXThickness(double w) {fCFBoxAIREXThickness = w;}
  /// Get the thickness of the AIREX
  double GetCFBoxAIREXThickness(void) {return fCFBoxAIREXThickness;}

  // Set and get the names
  virtual void SetCFBoxParentName  (G4String name){fCFBoxParentName = name;};
  virtual G4String GetCFBoxParentName  (){return fCFBoxParentName;};

  virtual void SetCFBoxName  (G4String name){fCFBoxName = name;};
  virtual G4String GetCFBoxName  (){return fCFBoxName;};


private:
  // Size of the CF Box
  double fCFBoxWidth;
  double fCFBoxHeight;
  double fCFBoxLength;

  // CFBox Thickness

  double fCFBoxCFRPThickness;
  double fCFBoxAIREXThickness;

  // CF Box Names
  G4String fCFBoxParentName;
  G4String fCFBoxName;

  void Init(void);

};
#endif
