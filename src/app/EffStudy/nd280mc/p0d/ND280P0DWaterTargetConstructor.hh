#ifndef ND280P0DWaterTargetConstructor_hh_Seen
#define ND280P0DWaterTargetConstructor_hh_Seen
// $Id: ND280P0DWaterTargetConstructor.hh,v 1.6 2011/11/23 22:13:13 mcgrew Exp $
//

#include <G4ThreeVector.hh>
#include <G4VisAttributes.hh>
#include <G4LogicalVolume.hh>

#include "ND280Constructor.hh"

/// Construct a target layer used to construct the P0D upstream and central
/// water target super-p0dules.  This is based on the "as-built" information
/// for the targets manufactured at UW and CSU.
class ND280P0DWaterTargetConstructor : public ND280Constructor {
public:
    ND280P0DWaterTargetConstructor(G4String n,
				   //ND280UserDetectorConstruction* c)
                                    ExN02DetectorConstruction* c)
        : ND280Constructor(n,c) {Init();};
    ND280P0DWaterTargetConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();};
    virtual ~ND280P0DWaterTargetConstructor(); 

    /// Get the total length of the water target module.  
    double GetLength() const {
        double length = GetFrameThickness() + 2*GetGasketThickness();
        if (fMakeBackCover) length += GetBackCoverThickness();
        return length;
    }

    /// Get the height of the target volume.  This matchs the height of the
    /// P0Dule.
  double GetHeight() const {return 2348*CLHEP::mm;}
    
    /// Get the width of the target volume.  The value of the width is fixed
    /// by the size of the P0Dule.
  double GetWidth() const {return 2200*CLHEP::mm;}

    /// Get the water depth in the left-hand target.  Control the water depth
    /// in the left-hand target (looking towards SK from the neutrino target,
    /// or positive X side).
    double GetLeftWaterDepth() const;

    /// Set the water depth in the left-hand target.
    void SetLeftWaterDepth(double d) {fLeftDepth=d;}

    /// Get the water depth in the right-hand target.  Control the water depth
    /// in the right-hand target (looking towards SK from the neutrino target,
    /// or negative X side).
    double GetRightWaterDepth() const;

    /// Set the water depth in the right-hand target.
    void SetRightWaterDepth(double d) {fRightDepth=d;}

    /// Get the "width" of the bottom HDPE frame.  The bottom frame includes
    /// both the actual frame, and the footer installed on the HDPE bag.  The
    /// frame is actually 75mm wide, and the footer is 88mm wide. Checked
    /// against the physical parts.
  double GetBottomFrameWidth() const {return 163*CLHEP::mm;}

    /// Get the length of the bottom frame.  Checked against the physical
    /// parts.  This includes the dovetail with the side frame at each end.
  double GetBottomFrameLength() const {return 2168*CLHEP::mm;}

    /// Get the "width" of the side HDPE frames.  Checked against the
    /// physical parts.
  double GetSideFrameWidth() const {return 77.5*CLHEP::mm;}

    /// Get the "length" of the side HDPE frames.  Checked against the
    /// physical parts.  This includes the dovetail with the bottom frame at
    /// each end.
  double GetSideFrameLength() const {return 2334*CLHEP::mm;}

    /// Get the "width" of the top HDPE Frame.  This frame is actually the
    /// header of the water target bags.  Checked against the physical parts.
  double GetTopFrameWidth() const {return 75*CLHEP::mm;}

    /// Get the "Length" of the top HDPE Frame.  This approximates the frame
    /// as a single piece across the top of the water bag.
  double GetTopFrameLength() const {return 2168*CLHEP::mm;}

    /// Get the "width" of the center frame.  This stretchs from the bottom to
    /// the top down the center of the water target.  Checked against the
    /// physical parts.
  double GetCenterFrameWidth() const {return 18*CLHEP::mm;}

    /// Get the "length" of the center frame.  This stretchs from the bottom
    /// to the top down the center of the water target.  This is the same as
    /// the side frame length.
    double GetCenterFrameLength() const {return GetSideFrameLength();}

    /// Get the thickness of the frame pieces.  This thickness doesn't include
    /// the gasket between the frame and the adjacent P0Dule (or radiator).
    /// Checked against the physical parts.
  double GetFrameThickness() const {return 28*CLHEP::mm;}

    /// Get the back cover thickness.
  double GetBackCoverThickness() const {return 6.35*CLHEP::mm;}

    /// Get the thickness of the (compressed) gasket.  The nominal gasket
    /// thickness is zero since the gasket should be fully compressed into
    /// it's groove.  The maximum thickness should be less than 1 mm since
    /// that is the total amount of thickness outside the groove (before
    /// compression).
    double GetGasketThickness() const {return fGasketThickness;}

    /// Set the thickness of the (compressed) gasket.  The nominal gasket
    /// thickness is zero since the gasket should be fully compressed into
    /// it's groove.  The maximum thickness should be less than 1 mm since
    /// that is the total amount of thickness outside the groove (before
    /// compression).
    void SetGasketThickness(double t) {fGasketThickness=t;}

    /// Get the thickness of the water.  This the sum of the frame thickness,
    /// and the gasket thickness.
    double GetWaterThickness(void) const {
        return GetFrameThickness()
            + 2*GetGasketThickness() 
            - 2*GetBagThickness();
    }

    /// Set the thickness of the P0D water bag material.
    void SetBagThickness(double t) {fBagThickness = t;}

    /// Get the thickness of the water bag material
    double GetBagThickness() const {return fBagThickness;}

    /// Set the correction to the water mass.  This is applied to allow the
    /// water mass in the MC match the measured mass.  Conceptually, it is
    /// accounting for the P0Dules buldging into the water.  A correction of
    /// zero leaves the mass unchanged.
    void SetMassCorrection(double s) {fMassCorrection = s;}

    /// Get the mass correction factor.
    double GetMassCorrection() const {return fMassCorrection;}

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume *GetPiece(void);

    /// Return the visual attributes for this constructor.
    virtual const G4VisAttributes* GetVisual(void) const;

    /// Setup to build the back cover.
    void MakeBackCover() {fMakeBackCover = true;}

    /// Setup to skip the back cover.
    void NoBackCover() {fMakeBackCover = false;}

protected:
    void Init(void);

    /// True if the back cover should be constructed.
    bool fMakeBackCover;

    /// The thickness of the compressed gasket.
    double fGasketThickness;

    /// The thickness of the water bag material.
    double fBagThickness;

    /// The water depth in the positive X target.
    double fLeftDepth;

    /// The water depth in the negative X target.
    double fRightDepth;

    /// The mass correction factor.
    double fMassCorrection;
};
#endif
