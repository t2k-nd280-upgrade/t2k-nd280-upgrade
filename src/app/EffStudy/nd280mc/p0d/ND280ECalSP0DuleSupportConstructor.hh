#ifndef ND280ECalSP0DuleSupportConstructor_hh_seen
#define ND280ECalSP0DuleSupportConstructor_hh_seen
// $Id: ND280ECalSP0DuleSupportConstructor.hh,v 1.1 2009/11/19 21:23:59 mcgrew Exp $
//


#include "G4VisAttributes.hh"
#include "ND280Constructor.hh"

class G4LogicalVolume;

/// Construct the support structure for an ECal SuperP0Dule.  The method
/// get GetPiece() constructs a new unplaced support structure that is facing 
/// up along the Z axis.  The calling code should then rotate the object into 
/// position and and place the object into a large mother volume.
class ND280ECalSP0DuleSupportConstructor : public ND280Constructor {
public:
  //ND280ECalSP0DuleSupportConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280ECalSP0DuleSupportConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280Constructor(n,c) {Init();};
    ND280ECalSP0DuleSupportConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();};
    virtual ~ND280ECalSP0DuleSupportConstructor();

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume *GetPiece(void);

    /// Get the external width of the support structure.
    void SetWidth(G4double w) {fWidth = w;}

    /// Get the external width of the support structure.
    G4double GetWidth(void) {return fWidth;}

    /// Get the external height of the support structure.
    void SetHeight(G4double w) {fHeight = w;}

    /// Get the external height of the support structure.
    G4double GetHeight(void) {return fHeight;}

    /// Get the external length of the support structure.
    void SetLength(G4double t) {fLength = t;}

    /// Get the external length of the support structure.
    G4double GetLength(void) {return fLength;}

    /// Get the height of the top support beams
    G4double GetTopSupportHeight(void) {return fTopSupportHeight;}

    /// Get the side thickness of the C-Channel
    G4double GetSideThickness(void) {return fSideThickness;}


private:

    /// The width of the support structure bounding box (parallel to the
    /// beam axis)
    G4double fWidth;

    /// The height of the support structure bounding box (vertical size
    /// perpendicular to the beam axis.
    G4double fHeight;

    /// The length of the support structure bounding box (horizontal size
    /// perpendicular to the beam axis).
    G4double fLength;

    /// The height of the beams that support the SuperP0Dule
    G4double fTopSupportHeight;

    /// The thickness of the C-Channel side beams
    G4double fSideThickness;

    void Init(void);

    G4VisAttributes *fvisual;

    void SetVisAttributes(G4LogicalVolume* logVolume);

};
#endif
