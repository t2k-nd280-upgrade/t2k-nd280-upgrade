#ifndef ND280BeamConstructor_hh_seen
#define ND280BeamConstructor_hh_seen
//

class G4LogicalVolume;

#include "ND280Constructor.hh"

/// Construct a simple rectangular bar to be used in constructing detector
/// elements.  This is the base class for ND280StructuralBeamConstructor
/// (builds a square tube beam) and ND280IBeamConstructor.  The method get
/// GetPiece() constructs a new unplaced beam that is facing along the Z axis.
/// The calling code should place the object into a large mother volume.

class ND280BeamConstructor : public ND280Constructor {

public:

  //ND280BeamConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280BeamConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280Constructor(n,c), fMaterialName("Aluminum") {Init();};

    ND280BeamConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p), fMaterialName("Aluminum") {Init();};

    virtual ~ND280BeamConstructor();

    /// Set the width of the beam.  This is the X dimension of the beam.
    virtual void SetWidth(double w) {fWidth = w;}
    /// Get the width of the Beam.
    virtual double GetWidth(void) {return fWidth;}

    /// Set the height of the beam.  This is the Y dimension of the beam.
    virtual void SetHeight(double w) {fHeight = w;}
    /// Get the height of the beam.
    virtual double GetHeight(void) {return fHeight;}

    /// Get the length of the beam.
    virtual double GetLength(void) {return fLength;};
    /// Set the length of the beam.
    virtual void SetLength(double t) {fLength = t;};

    /// Get the material name.
    virtual G4String GetMaterialName(void) {return fMaterialName;};
    /// Set the material name.
    virtual void SetMaterialName(G4String s) {fMaterialName = s;};

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume *GetPiece(void);

private:

    /// The width of the beam in the X direction.
    double fWidth;

    /// The height of the beam in the Y direction.
    double fHeight;

    /// The length of the beam along the Z axis.  
    double fLength;

    /// The name of the material from which the beam is made of.
    G4String fMaterialName;

    void Init(void);

};
#endif
