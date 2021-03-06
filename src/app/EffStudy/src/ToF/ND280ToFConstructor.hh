#ifndef ND280_TOF_CONSTRUCTOR_HH_SEEN
#define ND280_TOF_CONSTRUCTOR_HH_SEEN
// $Id: ND280ToFConstructor.hh,v 1.9 2017/24/02 dsgalabe $
//

#include <G4ThreeVector.hh>
class G4LogicalVolume;

#include "ND280Constructor.hh"

/// Create a bounding box to contain the off axis detector.  The method get
/// GetPiece() constructs a new unplaced magnet that is facing up along the Z
/// axis.  The calling code should then rotate the object into position and
/// and place the object into a large mother volume.
class ND280ToFConstructor : public ND280Constructor {
public:
  ND280ToFConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280Constructor(n,c) {Init();};
    ND280ToFConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();};
    virtual ~ND280ToFConstructor();

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume* GetPiece(void);

    /// Set the width of the ToF detector
    void SetWidth(double w) {fWidth = w;}
    /// Get the width of the ToF detector
    double GetWidth(void) {return fWidth;}

    /// Set the height of the ToF detector
    void SetHeight(double w) {fHeight = w;}
    /// Get the height of the ToF detector   
    double GetHeight(void) {return fHeight;}

    /// Set the length of the ToF detector bounding box "along" the beam axis (the
    /// Z axis).  
    void SetLength(double w) {fLength = w;}
    /// Get the length of the ToF detector
    double GetLength(void) {return fLength;}

  // Set number of planes XY, i.e. 1 layer X + 1 layer Y
  void SetPlaneXYNum(int num) {fPlaneXYNum = num;}
  int GetPlaneXYNum() {return fPlaneXYNum;}

  // Set number of scintillator bars in one layer
  void SetLayerHorizNBar(int num) {fLayerHorizNBar = num;}
  int GetLayerHorizNBar() {return fLayerHorizNBar;}
  void SetLayerVertNBar(int num) {fLayerVertNBar = num;}
  int GetLayerVertNBar() {return fLayerVertNBar;}

  // Set size of each scintillator bar
  void SetBarWidth(double w)  {fBarWidth = w;}
  void SetBarHeight(double w) {fBarHeight = w;}
  void SetBarLength(double w) {fBarLength = w;}
  void SetBarHorizLength(double w) {fBarHorizLength = w;}
  void SetBarVertLength(double w)  {fBarVertLength = w;}
  
  // Set rotation of the detector 
  void SetRotationX(double rot) {fRotationX = rot;}
  void SetRotationY(double rot) {fRotationY = rot;}
  void SetRotationZ(double rot) {fRotationZ = rot;}
  double GetRotationX() {return fRotationX;}
  double GetRotationY() {return fRotationY;}
  double GetRotationZ() {return fRotationZ;}

  // Get size of each scintillator bar
  double GetBarWidth()  {return fBarWidth;}
  double GetBarHeight() {return fBarHeight;}
  double GetBarLength() {return fBarLength;}
  double GetBarHorizLength() {return fBarHorizLength;}
  double GetBarVertLength()  {return fBarVertLength;}

    /// Set the X position of the ToF detector     
    void SetPosX(double pos) {fPosX = pos;}
    /// Get the X position of the ToF detector  
    double GetPosX(void) {return fPosX;}

    /// Set the Y position of the ToF detector       
    void SetPosY(double pos) {fPosX = pos;}
    /// Get the Y position of the ToF detector   
    double GetPosY(void) {return fPosY;}

    /// Set the Z position of the ToF detector 
    void SetPosZ(double pos) {fPosZ = pos;}
    // Get the Z position of the ToF detector 
    double GetPosZ(void) {return fPosZ;}

  // Set and Get the detector name
  //  void SetName(string name) {fName = name;}
  //string GetName() {return fName;}

private:
    /// The width of the ToF detector bounding box (horizontal size perpendicular
    /// to the beam axis)
    double fWidth;

    /// The height  of the ToF detector bounding box  (vertical size perpendicular
    /// to the beam axis.
    double fHeight;

    /// The length of the ToF detector bounding box (parallel to the beam
    /// axis).
    double fLength;

  // Number of scintillator bars in one layer
  int fLayerHorizNBar;
  int fLayerVertNBar;

  // Rotation of the detector along the X, Y and Z axes 
  double fRotationX;
  double fRotationY;
  double fRotationZ;

  // Number of layers XY (1 layer X + 1 layer Y)
  double fPlaneXYNum;

  // Single bar size
  double fBarWidth;
  double fBarHeight;
  double fBarLength;
  double fBarHorizLength;
  double fBarVertLength;

    // Position of the center of the ToF detector
    double fPosX;
    double fPosY;
    double fPosZ;

    // Detector name
    //string fName;

    void Init(void);

};
#endif
