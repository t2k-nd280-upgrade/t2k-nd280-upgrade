#ifndef ND280IBeamConstructor_hh_seen
#define ND280IBeamConstructor_hh_seen
//

class G4LogicalVolume;

#include "ND280BeamConstructor.hh"



/// Construct an I beam (or a C-Channel) to be used in constructing the basket
/// and other supports in the detector. The method get GetPiece() constructs a
/// new unplaced StructuralBeam that is facing along the Z axis. The calling
/// code should place the object into a large mother volume.
/// 
/// The constructed I beam can have caps on the ends by using SetIBeamLength
/// to be less than the beam length.  This will result in symmetric caps on
/// the beam.  If the value of SetIBeamLength (default is 100*m) is larger
/// than the beam length, the beam is not capped.
///
/// A C-Channel can be constructed by using SetIBeamCutOutNumber to set the
/// number of cutouts to one.
class ND280IBeamConstructor : public ND280BeamConstructor {

public:

  //ND280IBeamConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280IBeamConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280BeamConstructor(n,c), fNumber(2) {Init();};

    ND280IBeamConstructor(G4String n, ND280Constructor* p)
        : ND280BeamConstructor(n,p), fNumber(2) {Init();};

    virtual ~ND280IBeamConstructor();

    /// Get the thickness of the I-Beam vertical stem.
    virtual double GetStemThickness(void) const { return fStemThickness; }
    /// Set the thickness of the I-Beam vertical stem.
    virtual void SetStemThickness(double t) { fStemThickness = t; }

    /// Get the thickness of the I-Beam horizontal base/top.
    virtual double GetBaseThickness(void) const { return fBaseThickness; }
    /// Set the thickness of the I-Beam horizontal base/top.
    virtual void SetBaseThickness(double t) { fBaseThickness = t; }

    /// Get the length of the I-Beam section.
    virtual double GetIBeamLength(void);

    /// Set the length of the I-Beam section.  If this is less than the length
    /// of the beam, this results in a beam with caps on the end.  If this is
    /// longer than the length of the beam, then a standard I-beam is
    /// constructed.
    virtual void SetIBeamLength(double t) {fIBeamLength = t;}

    /// Get the number of I-beam cut-outs [default = 2].
    virtual int GetIBeamCutOutNumber(void) const { return fNumber; }

    /// Set the number of I-Beam cut-outs.  If the number of cutouts is equal
    /// to 1, then a C channel is constructed with the cutout on the +X side
    /// and the stem on the -X side of the beam.  If the number of cut outs
    /// has any other value (including zero), then a standard I-Beam is
    /// constructed.
    virtual void SetIBeamCutOutNumber(int n) { fNumber = n; }

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume *GetPiece(void);

private:

    /// The beam stem thickness
    double fStemThickness;

    /// The beam base thickness
    double fBaseThickness;

    /// The length of the I-Beam section
    double fIBeamLength;

    /// Number of I-Beam cut-outs
    int fNumber;

    void Init(void);

};
#endif
