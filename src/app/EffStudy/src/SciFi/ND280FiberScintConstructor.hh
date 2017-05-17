#ifndef ND280_FIBERSCINT_CONSTRUCTOR_HH_SEEN
#define ND280_FIBERSCINT_CONSTRUCTOR_HH_SEEN
// $Id: ND280FiberScintConstructor.hh, 2017/5/9  $
//

#include <G4ThreeVector.hh>

class G4LogicalVolume;

#include "ND280Constructor.hh"

// NEW ND280UPGRADE            
#include "ND280RootPersistencyManager.hh" // XML input file    
#include "ExN02ND280XML.hh" // XML input file
      
namespace conv {
  typedef enum EFiberDir
    {
      kUndefined = -999999,
      kAlongX,
      kAlongY,
      kAlongZ
    }
    FiberDir_t;
} // conv


using namespace conv;

class ND280FiberScintConstructor : public ND280Constructor {

public:

  //ND280FiberScintConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280FiberScintConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280Constructor(n,c) {Init();};
    ND280FiberScintConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();};
    virtual ~ND280FiberScintConstructor();

    /// Get a bar of scintillator.
    virtual G4LogicalVolume *GetPiece(void);

  // Set fiber direction (define the length axis)
  void SetFiberDir(FiberDir_t dir){fFiberDir=dir;}
  // Get fiber direction (define the length axis)
  FiberDir_t GetFiberDir(){return fFiberDir;}

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
  /// but assuming the axis along the fiber
  virtual double GetAxisLength(void) {return fAxisLength;};
  /// Set the axis length assuming it's along the axis   
  /// Be careful, this is not the length in the XYZ frame
  /// but assuming the axis along the fiber   
  virtual void SetAxisLength(double len) {fAxisLength = len;};

  /// Get the width of the top of the scintillator.  The top is along the X
  /// axis, and will be equal to zero for triangular bars.  For rectangular
  /// bars, top is equal to the base.  The size of the top is calculated
  /// internally so that it matches the current base and shape definitions.
  //
  //virtual double GetTop(void);
  //
 
  /// Set the scint fiber material
  virtual void SetFiberCoreMaterial(std::string mat){fFiberCoreMaterial = mat;};
  /// Get the scint fiber material
  std::string GetFiberCoreMaterial(){return fFiberCoreMaterial;};

  /// Set the scint fiber material
  virtual void SetFiberCladMaterial(std::string mat){fFiberCladMaterial = mat;};
  /// Get the scint fiber material
  std::string GetFiberCladMaterial(){return fFiberCladMaterial;};

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

  ExN02ND280XML * GetND280XML() const {return fND280XMLInput;};
  
private:

    void Init(void);
  
  /// Set the axis length assuming it's along the axis
  /// Be careful, this is not the length in the XYZ frame
  /// but assuming the axis along the fiber
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

  /// The sensitive detector for this extruded bar.
    G4VSensitiveDetector* fSensitiveDetector;

  /// The scint fiber material
  std::string fFiberCoreMaterial;
  std::string fFiberCladMaterial;

  ExN02ND280XML *fND280XMLInput;

  FiberDir_t fFiberDir;
};
#endif
