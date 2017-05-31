#ifndef ND280_SCIFI_CONSTRUCTOR_HH_SEEN
#define ND280_SCIFI_CONSTRUCTOR_HH_SEEN
// $Id: ND280SciFiConstructor.hh,v 1.9 2017/28/3 dsgalabe $
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
class ND280SciFiConstructor : public ND280Constructor {
public:
  ND280SciFiConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280Constructor(n,c) {Init();};
  ND280SciFiConstructor(G4String n, ND280Constructor* p)
    : ND280Constructor(n,p) {Init();};
  virtual ~ND280SciFiConstructor();
  
  /// Construct and return a G4 volume for the object.  This is a pure
  /// virtual function, which means it must be implemented by the inheriting
  /// classes.  This returns an unplaced logical volume which faces along
  /// the Z axis.
  virtual G4LogicalVolume* GetPiece(void);
  
  /// Set the width of the SciFi detector bounding box "along" the X axis
  void SetWidth(double w) {fWidth = w;}
  /// Get the width of the SciFi detector
  double GetWidth(void) {return fWidth;}

  /// Set the height of the SciFi detector bounding box "along" the Y axis  
  void SetHeight(double w) {fHeight = w;}
  /// Get the height of the SciFi detector
  double GetHeight(void) {return fHeight;}
  
  /// Set the length of the SciFi detector bounding box "along" the Y axis  
  void SetLength(double w) {fLength = w;}
  /// Get the length of the SciFi detector
  double GetLength(void) {return fLength;}

  /// Set the length of the scint fibers along X 
  void SetFiberHorizLength(double w) {fFiberHorizLength = w;}
  /// Get the length of the scint fibers along X
  double GetFiberHorizLength(void) {return fFiberHorizLength;}

  /// Set the length of the scint fibers along Y 
  void SetFiberVertLength(double w) {fFiberVertLength = w;}
  /// Get the length of the scint fibers along Y
  double GetFiberVertLength(void) {return fFiberVertLength;}

  /// Set the # of XY layers 
  void SetLayerNum(double w) {fLayerNum = w;}
  /// Get the # of XY layers 
  double GetLayerNum(void) {return fLayerNum;}

  // Set size of fiber edges  
  void SetEdge(double w) {fEdge = w;}
  // Get size of fiber edge 
  double GetEdge(void) {return fEdge;}

  // Set number of fibers along X
  void SetFiberHorizNum(int num) {fFiberHorizNum = num;}
  int GetFiberHorizNum() {return fFiberHorizNum;}

  // Set number of fibers along Y
  void SetFiberVertNum(int num) {fFiberVertNum = num;}
  int GetFiberVertNum() {return fFiberVertNum;}

  /// Set the X position of the SciFi detector     
  void SetPosX(double pos) {fPosX = pos;}
  /// Get the X position of the SciFi detector  
  double GetPosX(void) {return fPosX;}
  
  /// Set the Y position of the SciFi detector       
  void SetPosY(double pos) {fPosX = pos;}
  /// Get the Y position of the SciFi detector   
  double GetPosY(void) {return fPosY;}
  
  /// Set the Z position of the SciFi detector 
  void SetPosZ(double pos) {fPosZ = pos;}
  // Get the Z position of the SciFi detector 
  double GetPosZ(void) {return fPosZ;}

  // Set and Get the detectors name

  virtual void SetNameRepLayer(G4String name){fNameRepLayer=name;};
  virtual void SetNameRepFiberHoriz(G4String name){fNameRepFiberHoriz=name;};
  virtual void SetNameRepFiberVert(G4String name){fNameRepFiberVert=name;};
  virtual void SetNameFiberHoriz (G4String name){fNameFiberHoriz =name;};
  virtual void SetNameFiberVert (G4String name){fNameFiberVert =name;};

  virtual G4String GetNameRepLayer(){return fNameRepLayer;};
  virtual G4String GetNameRepFiberHoriz(){return fNameRepFiberHoriz;};
  virtual G4String GetNameRepFiberVert(){return fNameRepFiberVert;};
  virtual G4String GetNameFiberHoriz (){return fNameFiberHoriz;};
  virtual G4String GetNameFiberVert (){return fNameFiberVert;};

  //

  ExN02ND280XML * GetND280XML() const {return fND280XMLInput;};

private:
  // Size of the SciFi
  double fWidth;
  double fHeight;
  double fLength;

  // Length of the fibers
  double fFiberHorizLength;
  double fFiberVertLength;
  
  // # of XY layers
  double fLayerNum;

  /// The edge of a fiber of the SciFi detector 
  double fEdge;

  // Number of fibers along each axis 
  int fFiberHorizNum;
  int fFiberVertNum;
  
  // Position of the center of the SciFi detector
  double fPosX;
  double fPosY;
  double fPosZ;

  /// The name of the material to use for the fiber.
  std::string fFiberCoreMaterial;
  std::string fFiberCladMaterial;
  
  // Detector names
  G4String fNameRepLayer; //
  G4String fNameRepFiberHoriz;  //
  G4String fNameRepFiberVert;   // 
  G4String fNameFiberHoriz;  // Single fiber
  G4String fNameFiberVert;  // Single fiber

  void Init(void);

  ExN02ND280XML *fND280XMLInput;
};
#endif
