#ifndef ND280_FGDLIKE_CONSTRUCTOR_HH_SEEN
#define ND280_FGDLIKE_CONSTRUCTOR_HH_SEEN
// $Id: ND280FGDlikeConstructor.hh,v 1.9 2017/28/3 dsgalabe $
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
class ND280FGDlikeConstructor : public ND280Constructor {
public:
  ND280FGDlikeConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280Constructor(n,c) {Init();};
  ND280FGDlikeConstructor(G4String n, ND280Constructor* p)
    : ND280Constructor(n,p) {Init();};
  virtual ~ND280FGDlikeConstructor();
  
  /// Construct and return a G4 volume for the object.  This is a pure
  /// virtual function, which means it must be implemented by the inheriting
  /// classes.  This returns an unplaced logical volume which faces along
  /// the Z axis.
  virtual G4LogicalVolume* GetPiece(void);
  
  /// Set the width of the FGDlike detector bounding box "along" the X axis
  void SetWidth(double w) {fWidth = w;}
  /// Get the width of the FGDlike detector
  double GetWidth(void) {return fWidth;}

  /// Set the height of the FGDlike detector bounding box "along" the Y axis  
  void SetHeight(double w) {fHeight = w;}
  /// Get the height of the FGDlike detector
  double GetHeight(void) {return fHeight;}
  
  /// Set the length of the FGDlike detector bounding box "along" the Y axis  
  void SetLength(double w) {fLength = w;}
  /// Get the length of the FGDlike detector
  double GetLength(void) {return fLength;}

  /// Set the length of the scint bars along X 
  void SetBarHorizLength(double w) {fBarHorizLength = w;}
  /// Get the length of the scint bars along X
  double GetBarHorizLength(void) {return fBarHorizLength;}

  /// Set the length of the scint bars along Y 
  void SetBarVertLength(double w) {fBarVertLength = w;}
  /// Get the length of the scint bars along Y
  double GetBarVertLength(void) {return fBarVertLength;}

  /// Set the # of XY layers 
  void SetLayerNum(double w) {fLayerNum = w;}
  /// Get the # of XY layers 
  double GetLayerNum(void) {return fLayerNum;}

  // Set size of bar edges  
  void SetEdge(double w) {fEdge = w;}
  // Get size of bar edge 
  double GetEdge(void) {return fEdge;}

  // Set number of bars along X
  void SetBarHorizNum(int num) {fBarHorizNum = num;}
  int GetBarHorizNum() {return fBarHorizNum;}

  // Set number of bars along Y
  void SetBarVertNum(int num) {fBarVertNum = num;}
  int GetBarVertNum() {return fBarVertNum;}

  /// Set the X position of the FGDlike detector     
  void SetPosX(double pos) {fPosX = pos;}
  /// Get the X position of the FGDlike detector  
  double GetPosX(void) {return fPosX;}
  
  /// Set the Y position of the FGDlike detector       
  void SetPosY(double pos) {fPosX = pos;}
  /// Get the Y position of the FGDlike detector   
  double GetPosY(void) {return fPosY;}
  
  /// Set the Z position of the FGDlike detector 
  void SetPosZ(double pos) {fPosZ = pos;}
  // Get the Z position of the FGDlike detector 
  double GetPosZ(void) {return fPosZ;}

  // Set and Get the detectors name

  virtual void SetNameRepLayer(G4String name){fNameRepLayer=name;};
  virtual void SetNameRepBarHoriz(G4String name){fNameRepBarHoriz=name;};
  virtual void SetNameRepBarVert(G4String name){fNameRepBarVert=name;};
  virtual void SetNameBarHoriz (G4String name){fNameBarHoriz =name;};
  virtual void SetNameBarVert (G4String name){fNameBarVert =name;};

  virtual G4String GetNameRepLayer(){return fNameRepLayer;};
  virtual G4String GetNameRepBarHoriz(){return fNameRepBarHoriz;};
  virtual G4String GetNameRepBarVert(){return fNameRepBarVert;};
  virtual G4String GetNameBarHoriz (){return fNameBarHoriz;};
  virtual G4String GetNameBarVert (){return fNameBarVert;};

  //

  ExN02ND280XML * GetND280XML() const {return fND280XMLInput;};

private:
  // Size of the FGDlike
  double fWidth;
  double fHeight;
  double fLength;

  // Length of the bars
  double fBarHorizLength;
  double fBarVertLength;
  
  // # of XY layers
  double fLayerNum;

  /// The edge of a bar of the FGDlike detector 
  double fEdge;

  // Number of bars along each axis 
  int fBarHorizNum;
  int fBarVertNum;
  
  // Position of the center of the FGDlike detector
  double fPosX;
  double fPosY;
  double fPosZ;

  /// The name of the material to use for the bar.
  std::string fBarCoreMaterial;
  std::string fBarCladMaterial;
  
  // Detector names
  G4String fNameRepLayer; //
  G4String fNameRepBarHoriz;  //
  G4String fNameRepBarVert;   // 
  G4String fNameBarHoriz;  // Single bar
  G4String fNameBarVert;  // Single bar

  void Init(void);

  ExN02ND280XML *fND280XMLInput;
};
#endif
