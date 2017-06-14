#ifndef ND280_CELL_CONSTRUCTOR_HH_SEEN
#define ND280_CELL_CONSTRUCTOR_HH_SEEN
// $Id: ND280CellConstructor.hh, 2017/4/7  $
//

#include <G4ThreeVector.hh>

class G4LogicalVolume;

#include "ND280Constructor.hh"

// NEW ND280UPGRADE                                                      
#include "ND280RootPersistencyManager.hh" // XML input file                 
#include "ExN02ND280XML.hh" // XML input file                              
//    


///
/// This class is taken and adapted from /SuperFGD/ND280CubeScintConstructor
///
/// Construct a cell of "Air" or "Water" and add the plastic scintillator bars as sides.  
/// Build the WAGASCI cell already with the rotation consistent with an horizontal target:
///  - make the grid in the XZ plane
///  - add the slits (if any) close to the intersections (TODO)
///  - add the horizontal bar along Z axis (to be placed on top of the XZ grid)
///  - put a space between the XZ grid and the horizontal bar along Z
/// All the cells are of the same size.
/// A cell contains only 3 out of 6 sides. Then when 3 sides will be uncovered and 3 layers will be added 
/// 

class ND280CellConstructor : public ND280Constructor {

public:

  //typedef enum {eTriangle, eRectangle} ExtrudedShape;

  //ND280CellConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280CellConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280Constructor(n,c) {Init();};
    ND280CellConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();};
    virtual ~ND280CellConstructor();

    /// Get a bar of scintillator.
    virtual G4LogicalVolume *GetPiece(void);

    /// Get the length of the bar to be constructed.
    virtual double GetLength(void) {return fLength;};

    /// Set the length of the bar to be constructed.
    virtual void SetLength(double len) {fLength = len;};

    /// Get the base width of the extruded bar.
    virtual double GetBase(void) {return fBase;};

    /// Set the base width of the extruded bar.
    virtual void SetBase(double base);
  
  /// Set the space between the XZ grid and the horizontal bar along Z
  virtual void SetSpaceLayerY(double space) {fSpaceLayerY = space;};
  /// Get the space between the XZ grid and the horizontal bar along Z
  double GetSpaceLayerY() {return fSpaceLayerY;};
  
    /// Get the width of the top of the scintillator.  The top is along the X
    /// axis, and will be equal to zero for triangular bars.  For rectangular
    /// bars, top is equal to the base.  The size of the top is calculated
    /// internally so that it matches the current base and shape definitions.
    //
    //virtual double GetTop(void);
    //
  
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



  // Set rotation for the Fiber Hole along X axis
  virtual void SetHoleRot_X(G4RotationMatrix rot) {fHoleRotX = rot;};
  // Get rotation for the Fiber Hole along X axis
  virtual G4RotationMatrix GetHoleRot_X(void) {return fHoleRotX;};

  // Set rotation for the Fiber Hole along Y axis
  virtual void SetHoleRot_Y(G4RotationMatrix rot) {fHoleRotY = rot;};
  // Get rotation for the Fiber Hole along Y axis
  virtual G4RotationMatrix GetHoleRot_Y(void) {return fHoleRotY;};

  // Set rotation for the Fiber Hole along Z axis
  virtual void SetHoleRot_Z(G4RotationMatrix rot) {fHoleRotZ = rot;};
  // Get rotation for the Fiber Hole along Z axis
  virtual G4RotationMatrix GetHoleRot_Z(void) {return fHoleRotZ;};





  
  /// Get the thickness of the reflective coating.
  virtual double GetCoatingThickness(void) {return fCoatingThickness;}
  
  /// Set the thickness of the reflective coating.
    virtual void SetCoatingThickness(double t) {fCoatingThickness=t;}

  ///// Get the inner radius of the corner reflective coating for FGD scints.
  //virtual double GetCoatingRadius(void) {return fCoatingRadius;}

  ///// Set the inner radius of the corner reflective coating for FGD scints.
  //virtual void SetCoatingRadius(double r) {fCoatingRadius=r;} 

    /// Set the gap between the bars which corresponds to the space taken by
    /// whatever material is around the extrusion.  E.G. This is the space
    /// taken by the glue that is holding the bars together.
  virtual void SetGap(double g) {fGap = g;};

    /// Get the gap between the bars which corresponds to the space taken by
    /// whatever material is around the extrusion.  
    virtual double GetGap(void) {return fGap;};

  /// Set the shape of the extrusion.
  //virtual void SetShape(ExtrudedShape s) {fShape = s;};

  /// Get the shape of the extrusion.
  //virtual ExtrudedShape GetShape(void) {return fShape;};

    /// Get the height of the extruded bar.  The default value will be by a
    /// call to SetBase().
    virtual double GetHeight(void) {return fHeight;};

    /// Set the height of the extruded bar.
    virtual void SetHeight(double height);

  /// Set the thickness of the WAGASCI scintillator bar (face of the cube).  
  virtual void SetScintThickness(double thick){ fScintThickness = thick;};
  /// Get the thickness of the WAGASCI scintillator bar (face of the cube).     
  double GetScintThickness(){return fScintThickness;};

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

  ExN02ND280XML * GetND280XML() const {return fND280XMLInput;};
  
private:

    void Init(void);

    /// The length of the scintillator bar.
    double fLength;
    
    /// The shape of the bar.
    //ExtrudedShape fShape;

    /// The width of the base of the scintillator.  The base is along the X
    /// axis.
    double fBase;
    
    /// The height of the scintillator.  If the bar is triangular this will be
    /// calculated when the base is set, but may be overridden.  
    double fHeight;

  /// Space between the XZ grid and the horizontal bar along Z
  double fSpaceLayerY;
  
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
  
    /// The thickness of the material that is wrapping the scintillator (this
    /// may be extruded).
    double fCoatingThickness;

    /// The inner corner radius of the extruded wrapping for square FGD
    /// scintillators
    //double fCoatingRadius;

    /// Space outside of the bar to allow for glueing and mounting.
    double fGap;

    /// Flag if the created bar should be set to visible for G4 visualization. 
    bool fIsVisible;

    /// The name of the material to use for the coating.
    std::string fCoatingMaterial;

    /// The name of the material to use for the scintillator.
    std::string fScintillatorMaterial;

    /// Flag if we want to use FGD scintillator material for this bar.
    bool fUseFGDScint;

    /// The sensitive detector for this extruded bar.
    G4VSensitiveDetector* fSensitiveDetector;

  /// The WLS fiber material
  std::string fFiberMaterial;

  /// The WLS fiber radius
  double fFiberRadius;

  /// Get the thickness of the WAGASCI scintillator bar (face of the cube). 
  double fScintThickness;
  
  ExN02ND280XML *fND280XMLInput;
};
#endif
