#ifndef ND280_BARSCINT_CONSTRUCTOR_HH_SEEN
#define ND280_BARSCINT_CONSTRUCTOR_HH_SEEN
// $Id: ND280BarScintConstructor.hh, 2017/5/9  $
//

#include <G4ThreeVector.hh>

class G4LogicalVolume;

#include "ND280Constructor.hh"

// NEW ND280UPGRADE            
#include "ND280RootPersistencyManager.hh" // XML input file    
#include "ExN02ND280XML.hh" // XML input file
      
namespace conv {
  typedef enum EBarDir
    {
      kUndefined = -999999,
      kAlongX,
      kAlongY,
      kAlongZ
    }
    BarDir_t;
} // conv


using namespace conv;

class ND280BarScintConstructor : public ND280Constructor {

public:

  //ND280BarScintConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280BarScintConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280Constructor(n,c) {Init();};
    ND280BarScintConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();};
    virtual ~ND280BarScintConstructor();

    /// Get a bar of scintillator.
    virtual G4LogicalVolume *GetPiece(void);

  // Set bar direction (define the length axis)
  void SetBarDir(BarDir_t dir){fBarDir=dir;}
  // Get bar direction (define the length axis)
  BarDir_t GetBarDir(){return fBarDir;}

  /// Get the base of the bar to be constructed.
  virtual double GetBase(void) {return fBase;};
  /// Set the base of the bar to be constructed.
  virtual void SetBase(double len) {fBase = len;};

  /// Get the width of the bar to be constructed.
  virtual double GetWidth(void) {return fWidth;};
  /// Set the width of the bar to be constructed.
  virtual void SetWidth(double len) {fWidth = len;};
  
  /// Get the height of the bar to be constructed.
  virtual double GetHeight(void) {return fHeight;};
  /// Set the height of the bar to be constructed.
  virtual void SetHeight(double len) {fHeight = len;};
  
  /// Get the length of the bar to be constructed.
  virtual double GetLength(void) {return fLength;};
  /// Set the length of the bar to be constructed.
  virtual void SetLength(double len) {fLength = len;};
  
  /// Get the axis length assuming it's along the axis
  /// Be careful, this is not the length in the XYZ frame   
  /// but assuming the axis along the bar
  virtual double GetAxisLength(void) {return fAxisLength;};
  /// Set the axis length assuming it's along the axis   
  /// Be careful, this is not the length in the XYZ frame
  /// but assuming the axis along the bar   
  virtual void SetAxisLength(double len) {fAxisLength = len;};

  /// Get the width of the top of the scintillator.  The top is along the X
  /// axis, and will be equal to zero for triangular bars.  For rectangular
  /// bars, top is equal to the base.  The size of the top is calculated
  /// internally so that it matches the current base and shape definitions.
  //
  //virtual double GetTop(void);
  //
 
  /// Get the thickness of the reflective coating.
  virtual double GetCoatingThickness(void) {return fCoatingThickness;}  
  /// Set the thickness of the reflective coating.
    virtual void SetCoatingThickness(double t) {fCoatingThickness=t;}

  /// Set the gap between the bars which corresponds to the space taken by
  /// whatever material is around the extrusion.  E.G. This is the space
  /// taken by the glue that is holding the bars together.
  virtual void SetGap(double g) {fGap = g;};
  
  /// Get the gap between the bars which corresponds to the space taken by
  /// whatever material is around the extrusion.  
  virtual double GetGap(void) {return fGap;};

  /// Get the center to center spacing for bars which depends on the shape
  /// of the bar.
  //
  //virtual double GetCenterToCenter(void);
  //
  
  /// @{ The name of the material to use for the scintillator coating.
  /// [Default: ScintillatorCoating]
  virtual void SetCoatingMaterial(const char* mat) {fCoatingMaterial = mat;}
  const char* GetCoatingMaterial() const {return fCoatingMaterial.c_str();}
  /// @}
  
  /// @{ The name of the material to use for the scintillator. 
  /// [Default: Scintillator]
  virtual void SetScintillatorMaterial(const char* mat) {
    fScintillatorMaterial = mat;}
  const char* GetScintillatorMaterial() const {
    return fScintillatorMaterial.c_str();}
  /// @}
  
  virtual void SetVisibility(bool vis) {fIsVisible = vis;}
  
  virtual bool GetVisibility(void) {return fIsVisible;}
  
  /// Set the sensitive detector for this component.
  //virtual void SetSensitiveDetector(G4VSensitiveDetector* s);
  
  /// Get the sensitive detector for this component.
  //virtual G4VSensitiveDetector* GetSensitiveDetector(void) const {
  //  return fSensitiveDetector;
  //}

  // Set the radius of the hole.
  virtual void SetHoleRadius(double radius) {fHoleRadius = radius;};
  
  // Get the radius of the hole.
  virtual double GetHoleRadius(void) {return fHoleRadius;};

  // Set the Fiber Hole position along X axis
  virtual void SetHolePosition_X(G4ThreeVector pos) {fHolePositionX = pos;};
  // Get the Fiber Hole position along X axis
  virtual G4ThreeVector GetHolePosition_X(void) {return fHolePositionX;};

  // Set the Fiber Hole position along Y axis
  virtual void SetHolePosition_Y(G4ThreeVector pos) {fHolePositionY = pos;};
  // Get the Fiber Hole position along Y axis
  virtual G4ThreeVector GetHolePosition_Y(void) {return fHolePositionY;};

  // Set the Fiber Hole position along Z axis
  virtual void SetHolePosition_Z(G4ThreeVector pos) {fHolePositionZ = pos;};
  // Get the Fiber Hole position along Z axis
  virtual G4ThreeVector GetHolePosition_Z(void) {return fHolePositionZ;};

  /// Set the WLS fiber radius
  virtual void SetFiberRadius(double rad){fFiberRadius = rad;};
  /// Get the WLS fiber radius
  double GetFiberRadius(){return fFiberRadius;};

  /// Set the WLS fiber material
  virtual void SetFiberMaterial(std::string mat){fFiberMaterial = mat;};
  /// Get the WLS fiber material
  std::string GetFiberMaterial(){return fFiberMaterial;};

  ExN02ND280XML * GetND280XML() const {return fND280XMLInput;};
  
private:

    void Init(void);
  
  /// Set the axis length assuming it's along the axis
  /// Be careful, this is not the length in the XYZ frame
  /// but assuming the axis along the bar
  double fAxisLength;
  double fBase;

    /// The width in the XYZ frame (depends on the rotation)
    double fWidth;
  
    /// The length in the XYZ frame (depends on the rotation)
    double fLength;
    
    /// The height in the XYZ frame (depends on the rotation)
    double fHeight;

    /// The thickness of the material that is wrapping the scintillator (this
    /// may be extruded).
    double fCoatingThickness;

    /// Space outside of the bar to allow for glueing and mounting.
    double fGap;

    /// Flag if the created bar should be set to visible for G4 visualization. 
    bool fIsVisible;

    /// The name of the material to use for the coating.
    std::string fCoatingMaterial;

    /// The name of the material to use for the scintillator.
    std::string fScintillatorMaterial;

    /// The radius of the hole.
    double fHoleRadius;

  // Fiber Hole position along X, Y and Z axes
  G4ThreeVector fHolePositionX;
  G4ThreeVector fHolePositionY;
  G4ThreeVector fHolePositionZ;

  // Rotation for the Fiber Hole along X, Y and Z axes
  G4RotationMatrix fHoleRotX;
  G4RotationMatrix fHoleRotY;
  G4RotationMatrix fHoleRotZ;

  /// The WLS fiber material
  std::string fFiberMaterial;

  /// The WLS fiber radius
  double fFiberRadius;

  /// The sensitive detector for this extruded bar.
    G4VSensitiveDetector* fSensitiveDetector;

  ExN02ND280XML *fND280XMLInput;

  BarDir_t fBarDir;
};
#endif
