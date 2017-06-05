#ifndef ND280StructuralBeamConstructor_hh_seen
#define ND280StructuralBeamConstructor_hh_seen
//

class G4LogicalVolume;

#include "ND280BeamConstructor.hh"

/// Construct a structural (hollow rectangular beam to be used in constructing
/// the basket and other supports in the detector. The method get GetPiece()
/// constructs a new unplaced StructuralBeam that is facing along the Z axis.
/// The calling code should place the object into a large mother volume.

class ND280StructuralBeamConstructor : public ND280BeamConstructor {

public:

  //ND280StructuralBeamConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280StructuralBeamConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280BeamConstructor(n,c) {Init();};

    ND280StructuralBeamConstructor(G4String n, ND280Constructor* p)
        : ND280BeamConstructor(n,p) {Init();};

    virtual ~ND280StructuralBeamConstructor();

    /// Get the thickness of the beam wall.
    virtual double GetWallThickness(void) {return fWallThickness;}
    /// Set the thickness of the beam wall.
    virtual void SetWallThickness(double t) {fWallThickness = t;}

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume *GetPiece(void);

private:

    /// The beam wall thickness
    double fWallThickness;

    void Init(void);

};
#endif
