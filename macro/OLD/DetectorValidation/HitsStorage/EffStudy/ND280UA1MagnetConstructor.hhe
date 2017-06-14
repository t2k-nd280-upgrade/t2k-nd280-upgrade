#ifndef ND280_UA1MAGNET_CONSTRUCTOR_HH_SEEN
#define ND280_UA1MAGNET_CONSTRUCTOR_HH_SEEN

#include <cmath>
#include <G4ThreeVector.hh>
class G4LogicalVolume;

#include "ND280Constructor.hh"

// NEW ND280UPGRADE
#include "ND280RootPersistencyManager.hh" // XML input file
#include "ExN02ND280XML.hh" // XML input file 
//

/// Create a bounding box to contain the UA1 magnet.  The method get
/// GetPiece() constructs a new unplaced magnet that is facing up along the Z
/// axis.  The calling code should then rotate the object into position and
/// and place the object into a large mother volume.  The magnet must be
/// centered at the global origin, or the magnetic field will not align with
/// the yoke.
class ND280UA1MagnetConstructor : public ND280Constructor {
public:
  //ND280UA1MagnetConstructor(G4String n, ND280UserDetectorConstruction* c)
  ND280UA1MagnetConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280Constructor(n,c) {Init();};
  ND280UA1MagnetConstructor(G4String n, ND280Constructor* p)
    : ND280Constructor(n,p) {Init();};
  virtual ~ND280UA1MagnetConstructor();

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume *GetPiece(void);

    /// Get the total width of the magnet.
    double GetWidth() {return GetMagnetWidth() 
            + 2*std::max(GetRightClamSplit(),GetLeftClamSplit());}

    /// Get the total height of the magnet.
    double GetHeight() {return GetMagnetHeight();}

    /// Get the total length of the magnet.
    double GetLength() {return GetMagnetLength();}

    /// Get the width of the UA1 magnet.
    double GetMagnetWidth(void) {return fMagnetWidth;}

    /// Get the height of the UA1 magnet.  
    double GetMagnetHeight(void) {return fMagnetHeight;}

    /// Get the length of the UA1 magnet.
    double GetMagnetLength(void) {return fMagnetLength;}

    /// Get the current split of the left hand UA1 magnet clamshell.
    double GetLeftClamSplit(void) {return fLeftClamSplit;}

    /// Set the current split of the left hand UA1 magnet clamshell.  This
    /// must be set before the magnet is constructed.
    void SetLeftClamSplit(double s) {fLeftClamSplit = s;}
    
    /// Get the current split of the right hand UA1 magnet clamshell.
    double GetRightClamSplit(void) {return fRightClamSplit;}

    /// Set the current split of the right hand UA1 magnet clamshell.  This
    /// must be set before the magnet is constructed.
    void SetRightClamSplit(double s) {fRightClamSplit = s;}

  // 
  
  // NEW ND280UPGRADE
  ExN02ND280XML * GetND280XML() const {return fND280XMLInput;};

private:
    /// The width of the UA1 magnet (horizontal size perpendicular to the beam
    /// axis)
    double fMagnetWidth;

    /// The height of the UA1 magnet (vertical size perpendicular to the beam
    /// axis).
    double fMagnetHeight;

    /// The length of the UA1 magnet volume (parallel to the beam axis).
    double fMagnetLength;

    /// The current split of the left hand UA1 clamshell.
    double fLeftClamSplit;

    /// The current split of the left hand UA1 clamshell.
    double fRightClamSplit;

    void Init(void);

  ExN02ND280XML *fND280XMLInput;
};
#endif
