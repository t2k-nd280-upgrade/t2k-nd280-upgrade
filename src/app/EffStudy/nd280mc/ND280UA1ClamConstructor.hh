#ifndef ND280UA1ClamConstructor_hh_seen
#define ND280UA1ClamConstructor_hh_seen

#include <G4ThreeVector.hh>
class G4LogicalVolume;

#include "ND280Constructor.hh"

// NEW ND280UPGRADE
#include "ND280RootPersistencyManager.hh" // XML input file
#include "ExN02ND280XML.hh" // XML input file
//

/// Create a bounding volume to contain one "clam shell" of the UA1 magnet.
/// The magnet clam is constructed so that when it's closed, the clam should
/// be installed at the center of the off-axis detector position which is
/// (0,0,0) in the local Magnet volume.  No rotations should be applied to the
/// magnet, or it won't align with the magnetic field.
///
/// The method get GetPiece() constructs a new unplaced magnet that is facing
/// up along the Z axis.  The calling code should then rotate the object into
/// position and and place the object into a large mother volume.
class ND280UA1ClamConstructor : public ND280Constructor {
public:
  //ND280UA1ClamConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280UA1ClamConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280Constructor(n,c) {
        Init();
    }
    
    ND280UA1ClamConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {
        Init();
    }

    virtual ~ND280UA1ClamConstructor();

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume *GetPiece(void);

    /// Get the total width of the magnet clamshell.
    double GetWidth() {return fWidth;}

    /// Get the total height of the magnet.
    double GetHeight() {return fHeight;}

    /// Get the total length of the magnet.
    double GetLength() {return fLength;}

    /// Make this constructor build a right hand clamshell.
    void SetRightClamshell() {fIsLeftClamshell=false;}

    /// Make this constructor build a right hand clamshell.
    void SetLeftClamshell() {fIsLeftClamshell=true;}

    /// Get the length magnet inner volume
    double GetInnerLength();

    /// Get the width of the void around the tracker.  This is calculated as
    /// half the width of the basket (e.g. 1260*mm) plus the design clearance
    /// around the tracker (e.g. 25*mm).  The tracker is designed to stay
    /// fully inside of the magnet.
  double GetTrackerVoidWidth() {return 1260*CLHEP::mm+25*CLHEP::mm;}

    /// Get the height of the void around the tracker.  This is calculated as
    /// the height of the basket (e.g. 2480*mm) plus the design clearance
    /// around the tracker (e.g. 25*mm).  The tracker is designed to stay
    /// fully inside of the magnet.
    double GetTrackerVoidHeight() {return 2480*CLHEP::mm+2*35*CLHEP::mm;}

    /// Get the width of the void around the P0D. This is calculated as the
    /// width of the basket (i.e. 1260*mm) plus the design clearance around
    /// the P0D (i.e. 248*mm.)  The side P0DECal approaches to within 2mm of
    /// it's envelope, so the void around the P0D has been reduced to 225mm.
    /// The P0D is designed to stay inside a 203 mm envelope outside of the
    /// top and bottom of the basket (i.e. 203mm < 248mm and finally 203mm <
    /// 225mm).
    double GetP0DVoidWidth() {return 1260*CLHEP::mm+225*CLHEP::mm;}

    /// Get the height of the void around the P0D.  This is calculated as the
    /// height of the basket (i.e. 2480mm) plus the design clearance around
    /// the P0D (i.e. 248mm).  The as-built dimension of the P0DECal reduces
    /// the clearance between the P0D and the P0DECal from 248mm to 230mm, so
    /// this dimension has been reduced to 225mm to allow alignment clearance
    /// for the P0DECal.  The P0D is designed to stay inside a 203mm envelope
    /// outside of the top and bottom of the basket (i.e. 203mm < 248mm, and
    /// finally 203mm < 225mm).
    double GetP0DVoidHeight() {return 2480*CLHEP::mm+2*225*CLHEP::mm;}

    /// The length of the void around the P0D.  This is the design envelope
    /// for the P0D (2436*mm), plus the upstream thickness of the
    /// basket. (e.g. 140*mm).
    double GetP0DVoidLength() {return 2436*CLHEP::mm + 140*CLHEP::mm;}

    /// The outer dimension of the basket.  This will be checked to make sure
    /// that it is less than the inner volume length.
    double GetBasketLength() {return 6930*CLHEP::mm;}

    /// @{Set or get flags to build the various ecal modules.
    void SetBuildTopBrlECal(bool build) {fBuildTopBrlECal = build;}
    void SetBuildSideBrlECal(bool build) {fBuildSideBrlECal = build;}
    void SetBuildBottomBrlECal(bool build) {fBuildBottomBrlECal = build;}
    void SetBuildTopP0DECal(bool build) {fBuildTopP0DECal = build;}
    void SetBuildSideP0DECal(bool build) {fBuildSideP0DECal = build;}
    void SetBuildBottomP0DECal(bool build) {fBuildBottomP0DECal = build;}
    bool GetBuildTopBrlECal() const {return fBuildTopBrlECal;}
    bool GetBuildSideBrlECal() const {return fBuildSideBrlECal;}
    bool GetBuildBottomBrlECal() const {return fBuildBottomBrlECal;}
    bool GetBuildTopP0DECal() const {return fBuildTopP0DECal;}
    bool GetBuildSideP0DECal() const {return fBuildSideP0DECal;}
    bool GetBuildBottomP0DECal() const {return fBuildBottomP0DECal;}
    /// @}

  // NEW ND280UPGRADE
  ExN02ND280XML * GetND280XML() const {return fND280XMLInput;};
  
private:

    /// The width of the magnet (horizontal size perpendicular to the beam
    /// axis)
    double fWidth;

    /// The height of the magnet (vertical size perpendicular to the beam
    /// axis.
    double fHeight;

    /// The thickness of the magnet (parallel to the beam axis).
    double fLength;

    /// The gap between the interior flux return modules.
    double fInteriorGap;

    /// The gap between the end flux return modules and the rest of the magnet.
    double fExteriorGap;

    /// The gap between the center coils.  Only half of this gap is in this
    /// volume. 
    double fCenterCoilGap;

    /// The gap between the edge and center coils.  
    double fEdgeCoilGap;

    /// Flag that this is a left hand clamshell.
    bool fIsLeftClamshell;

    /// The width of the space that is cut out of the Clamshell volume.
    double fInnerWidth;

    /// The height of the space that is cut out of the Clamshell volume.
    double fInnerHeight;

    /// Flags to build the various ECals.
    bool fBuildTopBrlECal;
    bool fBuildSideBrlECal;
    bool fBuildBottomBrlECal;
    bool fBuildTopP0DECal;
    bool fBuildSideP0DECal;
    bool fBuildBottomP0DECal;

    void Init(void);

  ExN02ND280XML *fND280XMLInput;
};
#endif
