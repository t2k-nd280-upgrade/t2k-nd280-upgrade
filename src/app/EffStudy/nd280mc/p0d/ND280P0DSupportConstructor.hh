#ifndef ND280P0DSupportConstructor_hh_seen
#define ND280P0DSupportConstructor_hh_seen
// $Id: ND280P0DSupportConstructor.hh,v 1.2 2009/12/01 14:21:03 mcgrew Exp $
//


#include "G4VisAttributes.hh"
#include "ND280Constructor.hh"

class G4LogicalVolume;

/// Construct the support structure for a Target SuperP0Dule.  The method
/// get GetPiece() constructs a new unplaced support structure that is facing 
/// up along the Z axis.  The calling code should then rotate the object into 
/// position and and place the object into a large mother volume.
class ND280P0DSupportConstructor : public ND280Constructor {
public:
  //ND280P0DSupportConstructor(G4String n, ND280UserDetectorConstruction* c)
  ND280P0DSupportConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280Constructor(n,c) {Init();};
    ND280P0DSupportConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();};
    virtual ~ND280P0DSupportConstructor();

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume *GetPiece(void);

    /// Set the length of the support structure.
    void  SetUSECalSupportLength(double w) {fUSECalSupportLength = w;}

    /// Get the length of the support structure.
    G4double GetUSECalSupportLength(void) {return fUSECalSupportLength;}

    /// Set the length of the support structure.
    void  SetUSTargetSupportLength(double w) {fUSTargetSupportLength = w;}

    /// Get the length of the support structure.
    G4double GetUSTargetSupportLength(void) {return fUSTargetSupportLength;}

    /// Set the length of the support structure.
    void  SetCTargetSupportLength(double w) {fCTargetSupportLength = w;}

    /// Get the length of the support structure.
    G4double GetCTargetSupportLength(void) {return fCTargetSupportLength;}

    /// Set the length of the support structure.
    void  SetCECalSupportLength(double w) {fCECalSupportLength = w;}

    /// Get the length of the support structure.
    G4double GetCECalSupportLength(void) {return fCECalSupportLength;}

    /// Set the length of the space between support structures.
    void  SetSupportSpace(double w) {fSupportSpace = w;}

    /// Get the length of the space between support structures.
    G4double GetSupportSpace(void) {return fSupportSpace;}

    /// Get the width of the support structure.
    G4double GetWidth(void) {return fWidth;}

    /// Set the width of the support structure.
    void SetWidth(double w) {fWidth = w;}

    /// Get the height of the support structure.
    G4double GetHeight(void) {return 146*mm;}

    /// Get the length of the support structure.
    G4double GetLength(void);

private:

    /// The length of the USECal support structure bounding box 
    /// parallel to the beam axis).
    G4double fUSECalSupportLength;

    /// The length of the USTarget support structure bounding box 
    /// parallel to the beam axis).
    G4double fUSTargetSupportLength;

    /// The length of the CTarget support structure bounding box 
    /// parallel to the beam axis).
    G4double fCTargetSupportLength;

    /// The length of the CECal support structure bounding box 
    /// parallel to the beam axis).
    G4double fCECalSupportLength;

    /// The length of the support structure spacing 
    /// parallel to the beam axis).
    G4double fSupportSpace;

    /// The width of the support structure bounding box (perpendicular to the
    /// beam axis)
    G4double fWidth;

    /// The height of the support structure bounding box (vertical size
    /// perpendicular to the beam axis.
    G4double fHeight;

    void Init(void);

    G4VisAttributes *fvisual;

    void SetVisAttributes(G4LogicalVolume* logVolume);

};
#endif
