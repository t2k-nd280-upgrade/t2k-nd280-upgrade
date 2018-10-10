#ifndef ND280BasketConstructor_hh_seen
#define ND280BasketConstructor_hh_seen

//#include <TND280Log.hxx>
#include <G4ThreeVector.hh>

#include "G4VisAttributes.hh"

class G4LogicalVolume;

#include "ND280Constructor.hh"

// NEW ND280UPGRADE
#include "ND280RootPersistencyManager.hh" // XML input file
#include "ExN02ND280XML.hh" // XML input file 
//

/// Construct the basket that holds all of the inner detectors.  The method
/// get GetPiece() constructs a new unplaced basket that is facing up along
/// the Z axis.  The calling code should then rotate the object into position
/// and and place the object into a large mother volume.
class ND280BasketConstructor : public ND280Constructor {
public:
  //    ND280BasketConstructor(G4String n, ND280UserDetectorConstruction* c)
  //    : ND280Constructor(n,c) {Init();};
    ND280BasketConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280Constructor(n,c) {Init();};
    ND280BasketConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();};
    virtual ~ND280BasketConstructor();

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume *GetPiece(void);

    /// Get the width of the basket MC volume.
    G4double GetWidth(void) {return fWidth;}

    /// Get the height of the basket MC volume.
    G4double GetHeight(void) {return fHeight;}

    /// Get the length of the basket MC volume.
    G4double GetLength(void) {return fLength;}

    /// Set the width of the basket MC volume.
    void SetWidth(G4double len) {fWidth = len;}

    /// Set the height of the basket MC volume.
    void SetHeight(G4double len) {fHeight = len;}

    /// Set the length of the basket MC volume.
    void SetLength(G4double len) {fLength = len;}

    /// Get the outer width of the basket.  This is the outer width of the SS
    /// frame, and not the outer width of the MC volume.
    double GetOuterWidth(void) {return GetInnerWidth() + 2*GetSideThickness();}

    /// Get the outer height of the basket.  This is the outer height of the
    /// SS frame, and not the outer height of the MC volume.
    double GetOuterHeight(void) {
      return 2462*CLHEP::mm;
    }

    /// Get the outer length of the basket.  This is the outer length of the
    /// SS frame, and not the outer length of the MC volume.
    double GetOuterLength(void) {
        return GetInnerLength() 
            + GetUpstreamThickness() 
            + GetDownstreamThickness();
    }

    /// Get the internal width of the basket.
  G4double GetInnerWidth(void) {return 2320*CLHEP::mm;}

    /// Get the internal height of the basket.
    G4double GetInnerHeight(void) {return GetOuterHeight();}

    /// Get the internal length of the basket.
    G4double GetInnerLength(void) {return 6650*CLHEP::mm;}

    /// Get the thickness of the side structure.  This is the horizontal
    /// dimension of the side beams (top and bottom).
    G4double GetSideThickness(void) {return 100*CLHEP::mm;}

    /// Get the thickness of the bottom structure.  This is the vertical
    /// dimension of the bottom beam.
    G4double GetBottomThickness(void) {return 300*CLHEP::mm;}

    /// Get the thickness of the top structure.  This is the vertical
    /// dimension of the top beam.
    G4double GetTopThickness(void) {return 200*CLHEP::mm;}

    /// @{Get the dimensions of the beam used to contruct the staves.  These
    /// are three square tube located along the sides of the basket which
    /// maintain the separation between the top and bottom beams.  This is
    /// taken from the design drawings.
    double GetStaveWidth(void) {return 80*CLHEP::mm;}
    double GetStaveHeight(void) {return 120*CLHEP::mm;}
    /// @}

    /// @{ Get the location of the staves as measured from the upstream,
    /// outside end of the basket.  This is taken from the design drawings.
    double GetUpstreamStave() {return 2557*CLHEP::mm;}
    double GetCenterStave() {return 3916*CLHEP::mm;}
    double GetDownstreamStave() {return 5275*CLHEP::mm;}
    /// @}

    /// Get the thickness of the downstream structure.  This is the dimension
    /// along the beam axis.
    G4double GetDownstreamThickness(void) {return 140*CLHEP::mm;}

    /// Get the thickness of the upstream structure.  This is the dimension
    /// along the beam axis.
    G4double GetUpstreamThickness(void) {return 140*CLHEP::mm;}

    /// Get the width of the top and bottom on the ends of the basket.  This
    /// is the vertical dimension of the end beams.
    double GetEndWidth(void) {return 180*CLHEP::mm;}

    /// Get the "diameter" of the central hub.  This is the distance between
    /// the flats of the hub, and not the distance between the corners.
    double GetHubDiameter() {return 500*CLHEP::mm;}

    /// Get the "axle" diameter.  This is the OD of the pipe that supports the
    /// basket.
    double GetAxleDiameter() {return 250*CLHEP::mm;}

    /// Get the wall thickness for the "axle" tube.
    double GetAxleWallThickness() {return 10*CLHEP::mm;}

    /// The position of the center of the basket in the global coordinate
    /// system.  This is the center of the loaded basket.
    G4ThreeVector GetDeflectedCenter() const {
        return G4ThreeVector(0,-16*CLHEP::mm,0);
    }

    /// Get the space available for services that are located outside of the
    /// basket frame.  The space for the services is located between the
    /// basket and the P0DECal.  The P0DEcal and TrackerECal allow for a
    /// different amount of space for the services.  The value of the P0D
    /// Service space is taken P0D design envelop (i.e. 203 mm outside of the
    /// basket).  The ECal envelop begins at 248 mm outside of the basket.
    double GetP0DServiceSpace(void) {return 203*CLHEP::mm;}

    /// Get the length around the P0D that can be used for services.  This is
    /// the space covered by the P0DECal.
    double GetP0DServiceLength(void);

    /// Get the length around the Tracker that can be used for services.
    double GetTrackerServiceLength(void);

    /// Get the space available for services that are located outside the
    /// basket frame.  The space for the services is located between the
    /// basket and the Barrel ECal.  The P0DEcal and Barrel ECal allow for a
    /// different amount of space for the services.  The current value is
    /// chosen to allow small extrusions of the tracker outside of the basket.
    /// The ECal as-built envelope begins 40 mm outside of the basket.  This
    /// service height (13mm) allows for alignment of the ECal and the basket
    /// with respect to the magnet.
    /// In production 6 the service height was reduced from 25mm to 13mm
    /// to allow barrel ECal alignment.
    double GetTrackerServiceHeight(void) const {return 13*CLHEP::mm;}
    double GetTrackerServiceWidth(void) const {return 5*CLHEP::mm;}

    /// Get the position of the P0D along the Z axis.
    G4double GetP0DPosition(void) const {return fP0DPosition;}

    /// Get the distance from the bottom edge of the basket to the top of the
    /// P0D support structure.  This position determines the lowest possible
    /// position of the P0D within the basket simulation after all adjustments
    /// are applied (although values below about 79mm from the bottom are
    /// unphysical).  The value is taken from the solidworks drawings (7/6/11)
    /// and actually represents the top of the ECal C-Channels.
    double GetP0DSupportStructureHeight(void) const {return 59*CLHEP::mm;}

    /// Get the distance from the bottom edge of the basket to the lowest
    /// physical position of the P0Dules in the basket.  This represents the
    /// top of the P0D supports that the P0Dules rest on, however since the
    /// actual cross pieces are not simulated, this is a "fictitious"
    /// position in the MC and the P0D can have a negative offset from this
    /// height (i.e. be below this height).  Taken from the solidworks drawing
    /// (7/6/11) and represents the bottom surface of the P0Dule LI covers in
    /// the drawing.
    double GetP0DSupportHeight(void) const {return 78.9*CLHEP::mm;} 

    /// Get the position of the Tracker along the Z axis.
    G4double GetTrackerPosition(void) {return fTrackerPosition;}

    /// Get the position of FGD1 along the Z axis.
    G4double GetFGD1Position(void) {return fFGD1Position;}
    /// Get the position of FGD2 along the Z axis.
    G4double GetFGD2Position(void) {return fFGD2Position;}

    /// Set the downstream ecal Z position 
    void SetDsECalPosition(double z) {
        fDsECalPosition = z;
    }

    /// Get the downstream ecal Z position.
    G4double GetDsECalPosition(void) const {return fDsECalPosition;}

    /// Set the downstream ecal vertical position 
    void SetDsECalVerticalPosition(double y) {
        fDsECalVerticalPosition = y;
    }

    /// Get the vertical position of the downstream ecal.
    G4double GetDsECalVerticalPosition(void) const {
        return fDsECalVerticalPosition;
    }
    
  /// Set and get transformation parameters for DsECal.
  virtual void SetDsECalTrans(G4ThreeVector vec)
  {
    //ND280Warn("DS ECal translation being set by Basket.");
    //ND280Warn("- This has been deprecated.");
    //ND280Warn("- Set translation in DS ECal instead");    
    G4cout << "DS ECal translation being set by Basket." << G4endl;
    G4cout << "- This has been deprecated." << G4endl;
    G4cout << "- Set translation in DS ECal instead" << G4endl;

    fDsECalTrans = vec;
  }
  virtual G4ThreeVector GetDsECalTrans(void){return fDsECalTrans;}

  /// Only accept unit vectors as rotation axis.
  virtual void SetDsECalRotAxis(G4ThreeVector axis)
  {
    // ND280Warn("DS ECal rotation axis being set by Basket.");
    // ND280Warn("- This has been deprecated.");
    // ND280Warn("- Set rotation axis in DS ECal instead");
    G4cout << "DS ECal rotation axis being set by Basket." << G4endl;
    G4cout << "- This has been deprecated." << G4endl;
    G4cout << "- Set rotation axis in DS ECal instead" << G4endl;
    fDsECalRotAxis = axis.unit();
  }
  virtual G4ThreeVector GetDsECalRotAxis(void){return fDsECalRotAxis;}
  virtual void SetDsECalRotAng(double ang)
  {
    // ND280Warn("DS ECal rotation angle being set by Basket.");
    // ND280Warn("- This has been deprecated.");
    // ND280Warn("- Set rotation angle in DS ECal instead");
    G4cout << "DS ECal rotation angle being set by Basket." << G4endl;
    G4cout << "- This has been deprecated." << G4endl;
    G4cout << "- Set rotation angle in DS ECal instead" << G4endl;
    fDsECalRotAng = ang;
  }
  virtual double GetDsECalRotAng(void){return fDsECalRotAng;}
  
  // NEW ND280UPGRADE
  ExN02ND280XML * GetND280XML() const {return fND280XMLInput;};

private:

    /// The width of the basket bounding box (horizontal size
    /// perpendicular to the beam axis)
    G4double fWidth;

    /// The height of the basket bounding box (vertical size
    /// perpendicular to the beam axis.
    G4double fHeight;

    /// The length of the basket bounding box (parallel to the
    /// beam axis).
    G4double fLength;

    /// The position of the pizero detector (P0D) along the Z axis of the
    /// interior part of the UA1 Magnet.
    G4double fP0DPosition;

    /// The position of the tracking detector (Tracker) along the Z axis of
    /// the interior part of the UA1 Magnet.
    G4double fTrackerPosition;

    /// The position of FGD1 along the Z axis of the interior part of the
    /// UA1 Magnet.
    G4double fFGD1Position;

    /// The position of FGD2 along the Z axis of the interior part of the
    /// UA1 Magnet.
    G4double fFGD2Position;

    /// The position of the DSECal along the Z axis of the interior part of
    /// the UA1 Magnet.
    G4double fDsECalPosition;

    /// The position of the DSECal relative to the centerline of the basket.
    G4double fDsECalVerticalPosition;
    
     /// DsECal transformation variables
    G4ThreeVector fDsECalTrans;
    G4ThreeVector fDsECalRotAxis;
    double fDsECalRotAng;

    G4VisAttributes *fvisual;

    void Init(void);

    void SetVisAttributes(G4LogicalVolume* logVolume);

    /// Set the position of FGD1 along the Z axis.
    void SetFGD1Position(double z) {fFGD1Position = z;}
    /// Set the position of FGD2 along the Z axis.
    void SetFGD2Position(double z) {fFGD2Position = z; }

  // NEW ND280UPGRADE
  ExN02ND280XML *fND280XMLInput;
};
#endif
