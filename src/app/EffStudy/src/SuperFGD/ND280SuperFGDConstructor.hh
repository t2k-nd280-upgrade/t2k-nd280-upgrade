#ifndef ND280_SUPERFGD_CONSTRUCTOR_HH_SEEN
#define ND280_SUPERFGD_CONSTRUCTOR_HH_SEEN
// $Id: ND280SuperFGDConstructor.hh,v 1.9 2017/28/3 dsgalabe $
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
class ND280SuperFGDConstructor : public ND280Constructor {
public:
  ND280SuperFGDConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280Constructor(n,c) {Init();};
  ND280SuperFGDConstructor(G4String n, ND280Constructor* p)
    : ND280Constructor(n,p) {Init();};
  virtual ~ND280SuperFGDConstructor();
  
  /// Construct and return a G4 volume for the object.  This is a pure
  /// virtual function, which means it must be implemented by the inheriting
  /// classes.  This returns an unplaced logical volume which faces along
  /// the Z axis.
  virtual G4LogicalVolume* GetPiece(void);
  
  // Set size of cube edges  
  void SetEdge(double w) {fEdge = w;}
  // Get size of cube edge 
  double GetEdge(void) {return fEdge;}

  /// Set the width of the SuperFGD detector bounding box "along" the X axis
  void SetWidth(double w) {fWidth = w;}
  /// Get the width of the SuperFGD detector
  double GetWidth(void) {return fWidth;}

  /// Set the height of the SuperFGD detector bounding box "along" the Y axis  
  void SetHeight(double w) {fHeight = w;}
  /// Get the height of the SuperFGD detector
  double GetHeight(void) {return fHeight;}
  
  /// Set the length of the SuperFGD detector bounding box "along" the beam axis (the
  /// Z axis).  
  void SetLength(double w) {fLength = w;}
  /// Get the length of the SuperFGD detector
  double GetLength(void) {return fLength;}

  // Set number of cubes along X
  void SetCubeNumX(int num) {fCubeNumX = num;}
  int GetCubeNumX() {return fCubeNumX;}

  // Set number of cubes along Y
  void SetCubeNumY(int num) {fCubeNumY = num;}
  int GetCubeNumY() {return fCubeNumY;}

  // Set number of cubes along Z
  void SetCubeNumZ(int num) {fCubeNumZ = num;}
  int GetCubeNumZ() {return fCubeNumZ;}

  /// Set the X position of the SuperFGD detector     
  void SetPosX(double pos) {fPosX = pos;}
  /// Get the X position of the SuperFGD detector  
  double GetPosX(void) {return fPosX;}
  
  /// Set the Y position of the SuperFGD detector       
  void SetPosY(double pos) {fPosX = pos;}
  /// Get the Y position of the SuperFGD detector   
  double GetPosY(void) {return fPosY;}
  
  /// Set the Z position of the SuperFGD detector 
  void SetPosZ(double pos) {fPosZ = pos;}
  // Get the Z position of the SuperFGD detector 
  double GetPosZ(void) {return fPosZ;}

  // Set the radius of the WLS fiber.
  virtual void SetFiberRadius(double radius) {fFiberRadius = radius;};
  
  // Get the radius of the WLS fiber.
  virtual double GetFiberRadius(void) {return fFiberRadius;};

  
  /// @{ The name of the material to use for the fiber.
  /// [Default: FiberCore]
  virtual void SetFiberMaterial(const char* mat) {
    fFiberMaterial = mat;}
  const char* GetFiberMaterial() const {
    return fFiberMaterial.c_str();}
  /// @}

  // Set and Get the detectors name
  virtual void SetNameRepXYZ(G4String name){fNameRepXYZ=name;};
  virtual void SetNameRepXZ (G4String name){fNameRepXZ =name;};
  virtual void SetNameRepZ  (G4String name){fNameRepZ  =name;};
  virtual void SetNameCube  (G4String name){fNameCube  =name;};

  virtual G4String GetNameRepXYZ(){return fNameRepXYZ;};
  virtual G4String GetNameRepXZ (){return fNameRepXZ;};
  virtual G4String GetNameRepZ  (){return fNameRepZ;};
  virtual G4String GetNameCube  (){return fNameCube;};

  //

  ExN02ND280XML * GetND280XML() const {return fND280XMLInput;};

private:
  // Size of the SuperFGD
  double fWidth;
  double fHeight;
  double fLength;

  /// The edge of a cube of the SuperFGD detector 
  double fEdge;

  // Number of cubes along each axis 
  int fCubeNumX;
  int fCubeNumY;
  int fCubeNumZ;
  
  // Position of the center of the SuperFGD detector
  double fPosX;
  double fPosY;
  double fPosZ;

  /// The radius of the WLS fiber.
  double fFiberRadius;
  
  /// The name of the material to use for the fiber.
  std::string fFiberMaterial;
  
  // Detector names
  G4String fNameRepXYZ; // Replica of layers along Y
  G4String fNameRepXZ;  // Replica of rows along X
  G4String fNameRepZ;   // Replica of cubes along Z
  G4String fNameCube;   // Single cube

  void Init(void);

  ExN02ND280XML *fND280XMLInput;
};
#endif
