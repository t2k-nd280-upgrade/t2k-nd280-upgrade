#ifndef ND280_SUPERFGD_CONSTRUCTOR_HH_SEEN
#define ND280_SUPERFGD_CONSTRUCTOR_HH_SEEN
// $Id: ND280SuperFGDConstructor.hh,v 1.9 2017/28/3 dsgalabe $
//

#include <G4ThreeVector.hh>
class G4LogicalVolume;

#include "ND280Constructor.hh"

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

  // Set and Get the detector name
  //  void SetName(string name) {fName = name;}
  //string GetName() {return fName;}
  
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
  
  // Detector name
  //string fName;
  
  void Init(void);
};
#endif
