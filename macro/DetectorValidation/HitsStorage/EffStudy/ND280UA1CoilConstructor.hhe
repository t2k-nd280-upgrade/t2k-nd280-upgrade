#ifndef ND280_UA1COIL_CONSTRUCTOR_HH_SEEN
#define ND280_UA1COIL_CONSTRUCTOR_HH_SEEN
// $Id: ND280UA1CoilConstructor.hh,v 1.5 2007/07/23 13:20:02 mcgrew Exp $
//

#include <G4ThreeVector.hh>
class G4LogicalVolume;
class G4AssemblyVolume;

#include "ND280Constructor.hh"

// NEW ND280UPGRADE
#include "ND280RootPersistencyManager.hh" // XML input file
#include "ExN02ND280XML.hh" // XML input file 
//

/// Create single coil for the UA1 Magnet.  This can be used to create any of
/// the coils in the UA1 magnet, but the dimensions must be hard coded.  There
/// is not flexibility since the coil is the coil, and is specified by the
/// physical specs of the existing coils.  The method get GetPiece()
/// constructs a new unplaced coil that can be placed without rotation.
class ND280UA1CoilConstructor : public ND280Constructor {
public:
  //ND280UA1CoilConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280UA1CoilConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280Constructor(n,c) {Init();};
    ND280UA1CoilConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();};
    virtual ~ND280UA1CoilConstructor();

    /// This is a pure virtual function, which means it must be implemented by
    /// the inheriting classes.  This returns an unplaced logical volume which
    /// faces along the Z axis.  For a UA1 Coil, this always returns NULL.  
    virtual G4LogicalVolume *GetPiece(void);

    /// Construct an assembly for the coil.
    G4AssemblyVolume *GetAssembly(void);

    /// Set the length available inside of the coil.
    void SetCoilInnerLength(double len) {
        fCoilInnerLength = len;
    }

    /// Get the total length available inside of the coil.
    double GetCoilInnerLength(void) const {
        return fCoilInnerLength;
    }

    /// Set the total height available inside of the coil.
    void SetCoilInnerHeight(double h) {
        fCoilInnerHeight = h;
    }

    /// Get the total height available inside of the coil.
    double GetCoilInnerHeight(void) const {
        return fCoilInnerHeight;
    }

    /// Set the thickness of the coil.
    void SetCoilThickness(double t) {
        fCoilThickness = t;
    }

    /// Get the thickness of the coil.  
    double GetCoilThickness(void) const {return fCoilThickness;}

    /// Set the thickness of the case around the coil
    void SetCaseThickness(double t) {
        fCaseThickness = t;
    }
        
    /// Get the thickness case around the coil.
    double GetCaseThickness(void) const {return fCaseThickness;}

    /// Set the width of the coil.
    void SetCoilWidth(double w) {
        fCoilWidth = w;
    }

    /// Get the width of each coil.
    double GetCoilWidth(void) const {return fCoilWidth;}

    /// Set the inner radius of the coil corner bends.
    void SetBendInnerRadius(double r) {
        fBendInnerRadius = r;
    }

    /// Get the inner radius of the coil corner bends.
    double GetBendInnerRadius() const {
        return fBendInnerRadius;
    }

    // The remaining methods are calculated relative to the set dimensions
    // above.  

    /// Get the outer length of the coil.  The measured value is 7723mm.
    double GetCoilLength(void) const {
        return GetCoilInnerLength() 
            + 2*(GetCoilThickness() + 2*GetCaseThickness());
    }

    /// Get the outer height of the coil.
    double GetCoilHeight(void) {
        return GetCoilInnerHeight() 
            + 2*(GetCoilThickness() + 2*GetCaseThickness());
    }

    /// Get the outer radius of the coil corner bends.
    double GetBendOuterRadius() const {
        return GetBendInnerRadius() 
            + GetCoilThickness() + 2*GetCaseThickness();
    }

    /// Get the length of the top coil bar.
    double GetTopBarLength() const {
        return GetCoilInnerLength()
            - 2*GetBendInnerRadius() - 2*GetCaseThickness();
    }
    
    /// Get the length of the bottom coil bar.
    double GetBottomBarLength() const {
        return GetTopBarLength();
    }

    /// Get the length of the upstream coil bar.
    double GetFrontBarLength() const {
        return GetCoilInnerHeight() 
            - 2*GetBendInnerRadius() - 2*GetCaseThickness();
    }

    /// Get the length of the downstream coil bar.
    double GetBackBarLength() const {
        return GetFrontBarLength();
    }

  // NEW ND280UPGRADE
  ExN02ND280XML * GetND280XML() const {return fND280XMLInput;};

private:

    /// The visual to use for this flux return object.
    const G4VisAttributes* GetVisual(void) const;
    
    /// The length available inside of the coil.
    double fCoilInnerLength;

    /// The height available inside of the coil.
    double fCoilInnerHeight;

    /// The thickness of the coil.
    double fCoilThickness;

    /// The width of the coil.
    double fCoilWidth; 

    /// The thickness of the case around the coil.
    double fCaseThickness;

    /// The radius of curvature for the center of the coil.
    double fBendInnerRadius;

    void Init(void);

  ExN02ND280XML *fND280XMLInput;
};
#endif
