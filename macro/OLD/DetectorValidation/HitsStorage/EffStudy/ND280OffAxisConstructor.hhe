#ifndef ND280_OFFAXIS_CONSTRUCTOR_HH_SEEN
#define ND280_OFFAXIS_CONSTRUCTOR_HH_SEEN
// $Id: ND280OffAxisConstructor.hh,v 1.9 2010/10/06 19:17:44 mcgrew Exp $
//

#include <G4ThreeVector.hh>
class G4LogicalVolume;

#include "ND280Constructor.hh"

/// Create a bounding box to contain the off axis detector.  The method get
/// GetPiece() constructs a new unplaced magnet that is facing up along the Z
/// axis.  The calling code should then rotate the object into position and
/// and place the object into a large mother volume.
class ND280OffAxisConstructor : public ND280Constructor {
public:
  //ND280OffAxisConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280OffAxisConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280Constructor(n,c) {Init();};
    ND280OffAxisConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();};
    virtual ~ND280OffAxisConstructor();

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume* GetPiece(void);

    /// Set the width of the off-axis bounding box.  This is the size with the
    /// magnet closed.  If the magnet is open, this will be increased in the
    /// GetWidth() method.
    void SetWidth(double w) {fWidth = w;}

    /// Get the width of the off-axis bounding box.
    double GetWidth(void);

    /// Set the height of the off-axis bounding box.
    void SetHeight(double w) {fHeight = w;}

    /// Get the height of the off-axis bounding box.
    double GetHeight(void) {return fHeight;}

    /// Set the length of the off-axis bounding box "along" the beam axis (the
    /// Z axis).  Note that the beam goes down at at a few degrees, but the Z
    /// axis is horizontal
    void SetLength(double w) {fLength = w;}

    /// Get the length of the off-axis bounding box.
    double GetLength(void) {return fLength;}

private:
    /// The width of the off-axis bounding box (horizontal size perpendicular
    /// to the beam axis)
    double fWidth;

    /// The height  of the off-axis bounding box  (vertical size perpendicular
    /// to the beam axis.
    double fHeight;

    /// The length of the off-axis bounding box (parallel to the beam
    /// axis).
    double fLength;

    void Init(void);

};
#endif
