#ifndef ND280P0DTargetRadiatorConstructor_hh_Seen
#define ND280P0DTargetRadiatorConstructor_hh_Seen
// $Id: ND280P0DTargetRadiatorConstructor.hh,v 1.1 2009/11/19 21:23:59 mcgrew Exp $
//

#include <G4ThreeVector.hh>
#include <G4VisAttributes.hh>
#include <G4LogicalVolume.hh>

#include "ND280Constructor.hh"

/// Construct a radiator used to construct the P0D upstream and central water
/// target super-p0dules.  This is based on the "as-built" information for the
/// radiators manufactured at SBU.  The target radiator is made of two sheets
/// of brass joined in the middle by a strip of tape.  It is simulated as a
/// single sheet of brass.
class ND280P0DTargetRadiatorConstructor : public ND280Constructor {
public:
    ND280P0DTargetRadiatorConstructor(G4String n,
				      //ND280UserDetectorConstruction* c)
                                    ExN02DetectorConstruction* c)
        : ND280Constructor(n,c) {Init();};
    ND280P0DTargetRadiatorConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();};
    virtual ~ND280P0DTargetRadiatorConstructor(); 

    /// Get the total length of the ecal radiator.
    double GetLength() const {
        return fLength;
    }

    /// Get the height of the ecal radiator.  The value of the height is fixed
    /// by the "As-Built" drawings.  Checked against the physical parts.
    double GetHeight() const {return 2350*mm;}
    
    /// Get the width of the ecal radiator.  The value of the width is fixed
    /// by the "As-Built" drawings.  Checked against the physical parts.
    double GetWidth() const {return 2200*mm;}

    /// Set the thickness of the radiator.
    void SetThickness(double s) {fLength=s;}
    
    /// Get the thickness of the radiator.
    double GetThickness() const {
        return fLength;
    }

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume *GetPiece(void);

    /// Return the visual attributes for this constructor.
    virtual const G4VisAttributes* GetVisual(void) const;

protected:
    void Init(void);

    /// The thickness of the brass radiator.
    double fLength;
};
#endif
