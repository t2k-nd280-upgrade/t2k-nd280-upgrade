#ifndef ND280_PCB_CONSTRUCTOR_HH_SEEN
#define ND280_PCB_CONSTRUCTOR_HH_SEEN
// $Id: ND280PCBConstructor.hh,v 1.9 2017/28/3 dsgalabe $
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
class ND280PCBConstructor : public ND280Constructor {
public:
  ND280PCBConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280Constructor(n,c) {Init();};
  ND280PCBConstructor(G4String n, ND280Constructor* p)
    : ND280Constructor(n,p) {Init();};
  virtual ~ND280PCBConstructor();
  
  /// Construct and return a G4 volume for the object.  This is a pure
  /// virtual function, which means it must be implemented by the inheriting
  /// classes.  This returns an unplaced logical volume which faces along
  /// the Z axis.
  virtual G4LogicalVolume* GetPiece(void);
  
  /// Set the width of the SuperFGD PCB
  void SetPCBWidth(double w) {fPCBWidth = w;}
  /// Get the width of the SuperFGD PCB
  double GetPCBWidth(void) {return fPCBWidth;}

  /// Set the height of the SuperFGD PCB
  void SetPCBHeight(double w) {fPCBHeight = w;}
  /// Get the height of the SuperFGD PCB
  double GetPCBHeight(void) {return fPCBHeight;}

  /// Set the length of the SuperFGD PCB
  void SetPCBLength(double w) {fPCBLength = w;}
  /// Get the length of the SuperFGD PCB
  double GetPCBLength(void) {return fPCBLength;}

  // Set and get the names
  virtual void SetPCBParentName  (G4String name){fPCBParentName = name;};
  virtual G4String GetPCBParentName  (){return fPCBParentName;};

  virtual void SetPCBName  (G4String name){fPCBName = name;};
  virtual G4String GetPCBName  (){return fPCBName;};


private:
  // Size of the PCB
  double fPCBWidth;
  double fPCBHeight;
  double fPCBLength;

  // CF Box Names
  G4String fPCBParentName;
  G4String fPCBName;

  void Init(void);

};
#endif
