#ifndef ND280_FLATCABLE_CONSTRUCTOR_HH_SEEN
#define ND280_FLATCABLE_CONSTRUCTOR_HH_SEEN
// $Id: ND280FlatCableConstructor.hh,v 1.9 2017/28/3 dsgalabe $
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
class ND280FlatCableConstructor : public ND280Constructor {
public:
  ND280FlatCableConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280Constructor(n,c) {Init();};
  ND280FlatCableConstructor(G4String n, ND280Constructor* p)
    : ND280Constructor(n,p) {Init();};
  virtual ~ND280FlatCableConstructor();
  
  /// Construct and return a G4 volume for the object.  This is a pure
  /// virtual function, which means it must be implemented by the inheriting
  /// classes.  This returns an unplaced logical volume which faces along
  /// the Z axis.
  virtual G4LogicalVolume* GetPiece(void);
  
  /// Set the width of the SuperFGD FlatCable
  void SetFlatCableWidth(double w) {fFlatCableWidth = w;}
  /// Get the width of the SuperFGD FlatCable
  double GetFlatCableWidth(void) {return fFlatCableWidth;}

  /// Set the height of the SuperFGD FlatCable
  void SetFlatCableHeight(double w) {fFlatCableHeight = w;}
  /// Get the height of the SuperFGD FlatCable
  double GetFlatCableHeight(void) {return fFlatCableHeight;}

  /// Set the length of the SuperFGD FlatCable
  void SetFlatCableLength(double w) {fFlatCableLength = w;}
  /// Get the length of the SuperFGD FlatCable
  double GetFlatCableLength(void) {return fFlatCableLength;}


  /// Set the thickness of the individual SuperFGD FlatCable
  void SetFlatCableThickness(double w) {fFlatCableThickness = w;}
  /// Get the thickness of the individual SuperFGD FlatCable
  double GetFlatCableThickness(void) {return fFlatCableThickness;}



  /// Set the width of the SuperFGD FlatCable Region
  void SetCableRegionWidth(double w) {fCableRegionWidth = w;}
  /// Get the width of the SuperFGD FlatCable Region
  double GetCableRegionWidth(void) {return fCableRegionWidth;}

  /// Set the height of the SuperFGD FlatCable Region
  void SetCableRegionHeight(double w) {fCableRegionHeight = w;}
  /// Get the height of the SuperFGD FlatCable Region
  double GetCableRegionHeight(void) {return fCableRegionHeight;}

  /// Set the length of the SuperFGD FlatCable Region
  void SetCableRegionLength(double w) {fCableRegionLength = w;}
  /// Get the length of the SuperFGD FlatCable Region
  double GetCableRegionLength(void) {return fCableRegionLength;}

  // Set and get the names
  virtual void SetFlatCableParentName  (G4String name){fFlatCableParentName = name;};
  virtual G4String GetFlatCableParentName  (){return fFlatCableParentName;};

  virtual void SetFlatCableName  (G4String name){fFlatCableName = name;};
  virtual G4String GetFlatCableName  (){return fFlatCableName;};

  void SetFlatCableNBundles (int w){fFlatCableN = w;}
  int GetFlatCableNBundles (){return fFlatCableN;}


private:
  // Size of the FlatCable
  double fFlatCableWidth;
  double fFlatCableHeight;
  double fFlatCableLength;
  double fFlatCableThickness;


  // Size of the FlatCable
  double fCableRegionWidth;
  double fCableRegionHeight;
  double fCableRegionLength;

  // Quantity of the FlatCable
  int fFlatCableN;


  // FlatCable Names
  G4String fFlatCableParentName;
  G4String fFlatCableName;

  void Init(void);

};
#endif
