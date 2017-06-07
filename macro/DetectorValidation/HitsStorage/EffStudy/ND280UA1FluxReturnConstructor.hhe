#ifndef ND280_UA1FLUXRETURN_CONSTRUCTOR_HH_SEEN
#define ND280_UA1FLUXRETURN_CONSTRUCTOR_HH_SEEN
// $Id: ND280UA1FluxReturnConstructor.hh,v 1.11 2010/06/02 19:55:56 holeczek Exp $
//

#include <G4ThreeVector.hh>
class G4LogicalVolume;
class G4AssemblyVolume;

#include "ND280Constructor.hh"

// NEW ND280UPGRADE
#include "ND280RootPersistencyManager.hh" // XML input file
#include "ExN02ND280XML.hh" // XML input file 
//

/// Create single flux return C for the UA1 Magnet.  This can be used to
/// create any of the flux return objects in the UA1 magnet.  The method get
/// GetPiece() constructs a new unplaced flux return object which is "laying
/// on it's back" and will need to be rotated about the Z axis before it is
/// placed in the UA1 magnet.
class ND280UA1FluxReturnConstructor : public ND280Constructor {
public:
  //ND280UA1FluxReturnConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280UA1FluxReturnConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280Constructor(n,c) {Init();};
    ND280UA1FluxReturnConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();};
    virtual ~ND280UA1FluxReturnConstructor();

    /// This is a pure virtual function, which means it must be implemented by
    /// the inheriting classes.  This returns an unplaced logical volume which
    /// faces along the Z axis.  For a UA1 flux return object, this always
    /// returns NULL.
    virtual G4LogicalVolume *GetPiece(void);

    /// Construct an assembly for the flux return object..
    G4AssemblyVolume *GetAssembly(void);

    /// Set if this is going to be a left or right flux return.
    void SetIsLefthandReturn(bool left) {fIsLefthandReturn = left;}

    /// Get the width of the flux return C (about 90cm)..
    double GetWidth(void) {return fPlateWidth;}

    /// Get the flux return thickness.
    double GetFluxReturnThickness(void) {
        return fOuterArmLength - fInnerArmLength;
    }

    /// Get the inner dimension flux return coil.
    double GetInnerHeight(void) {
        return fInnerHeight;
    }

    /// Get the outer dimension of the flux return coil arms.
    double GetOuterArmLength(void) {
        return fOuterArmLength;
    }

    /// Get the inner dimension of the flux return coil arms.
    double GetInnerArmLength(void) {
        return fInnerArmLength;
    }

    /// Set that a particular layer has a scintillator installed.
    void SetScintillator(int i, bool installed);

    /// Get if a particular layer has a scintillator installed.
    bool GetScintilator(int i);

    /// Set the index of the yoke being generated.
    void SetYokeIndex(int i) {fYokeIndex = i;}

  // NEW ND280UPGRADE
  ExN02ND280XML * GetND280XML() const {return fND280XMLInput;};

private:

    /// The visual to use for this flux return object.
    const G4VisAttributes* GetVisual(void) const;
    
    /// Create a new piece of steel to go into the C.
    G4LogicalVolume* MakeIronPlate(double fullLength);

    /// Create a spacer to go between the plates.
    G4LogicalVolume* MakeSpacer(double diameter);

    /// Create a new piece of scintillator to go into the C.
    G4LogicalVolume* MakeScintPlate(double fullLength);

    /// The gap between the plates of the flux return.
    double fGapThickness;

    /// The diameter of the spacers in the side.
    double fSideSpacerDiameter;

    /// The diameter of the spacers in the arms.
    double fArmSpacerDiameter;

    /// The size of the gap in the side
    double fSideGapLength;
    
    /// The size of the gap in the arms
    double fArmGapLength;

    /// The offset of the first spacer from the end of the arm
    double fArmSpacerOffset;

    /// The number of plates in the flux return.
    unsigned int fPlateCount;

    /// The overall thickness of the iron plates.
    double fPlateThickness;

    /// The width of the plate.
    double fPlateWidth;

    /// The spacing between successive plates.  This includes the gap
    /// thickness and is equal to fPlateThickness+fGapThickness.
    double fPlateSpacing;

    /// The scintillator layers installed in the flux return.
    std::vector<int> fScintInstalled;

    /// The height of the inner volume inside of the magnet.
    double fInnerHeight;

    /// The outer length of the magnet arm.
    double fOuterArmLength;

    /// The inner length of the magnet arm.
    double fInnerArmLength;

    /// Flag that is true if this will be a left hand return.  The assembly
    /// will still need to be rotated, but this controls stuff like the
    /// visualization color, and any other "handedness" issues.
    bool fIsLefthandReturn;

    /// The index of the magnet C being generated.
    int fYokeIndex;

    void Init(void);

  ExN02ND280XML *fND280XMLInput;
};
#endif
