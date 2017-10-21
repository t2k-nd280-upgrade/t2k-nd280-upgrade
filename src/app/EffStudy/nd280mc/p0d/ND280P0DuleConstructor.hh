#ifndef ND280P0DuleConstructor_hh_Seen
#define ND280P0DuleConstructor_hh_Seen
// $Id: ND280P0DuleConstructor.hh,v 1.7 2011/11/29 15:41:38 mcgrew Exp $
//

#include <G4ThreeVector.hh>
#include <G4VisAttributes.hh>
class G4LogicalVolume;

#include "ND280ComponentConstructor.hh"

class G4VSensitiveDetector;

/// Construct a single P0Dule including the LI system and the MPPCs.  This is
/// based on "as-built" information collected at SBU.
class ND280P0DuleConstructor : public ND280Constructor {
public:
  //ND280P0DuleConstructor(G4String n, ND280UserDetectorConstruction* c)
  ND280P0DuleConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280Constructor(n,c) {Init();};
    ND280P0DuleConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();};
    virtual ~ND280P0DuleConstructor(); 

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume *GetPiece(void);

    /// Calculate the P0Dule length.  This can vary a small amount from P0Dule
    /// to P0Dule, but is about 38.75mm.
    double GetLength(void);
    
    /// The P0Dule height including the MPPCs and LI covers.
    double GetHeight(void) const {
        return GetP0DuleHeight() + GetLICoverThickness() + GetMPPCSpace();
    }

    /// The P0Dule width including the MPPCs and LI covers.
    double GetWidth(void) const {
        return GetP0DuleWidth() + GetLICoverThickness() + GetMPPCSpace();
    }

    /// Get the height of the P0Dule not including the MPPCs or the LI.  This
    /// is the size of the glued P0Dule.
  double GetP0DuleHeight(void) const {return 2338*CLHEP::mm;}

    /// Get the width of the P0Dule not including the MPPCs or the LI.  This
    /// is the size of the glued P0Dule.
  double GetP0DuleWidth(void) const {return 2200*CLHEP::mm;}

    /// Get the thickness of the P0Dule lamination.  The p0dule lamination
    /// consists of a light tight skin, epoxy, the X bars, epoxy, the Y bars,
    /// epoxy, and a final light tight skin.  This isn't "const" since it uses
    /// the ND280Constructor::GetLength() method which may force the
    /// recalculation of sub-volume lengths.
    double GetLaminationThickness(void);

    /// Set the thickness of the epoxy.  The nominal epoxy thickness is 0.25
    /// mm, and but needs to be tuned to match the measured P0Dule thickness.
    /// There are three epoxy layers per P0Dule.
    void SetEpoxyThickness(double b) {fEpoxyThickness = b;}

    /// Get the thickness of the epoxy.
    double GetEpoxyThickness(void) const {return fEpoxyThickness;}

    /// Get the thickness of the Side MPPC frame.  Note that the frame pieces
    /// have insets for the light tight skins.  In the MC this is approximated
    /// by not extending the skin over the frame.  Note that while this is the
    /// actual thickness of the P0Dule, when P0Dules are stacked against a
    /// water target (or ECal radiators) they actually rest on the inset light
    /// tight skin so the entire assembly packs a bit more closely than this
    /// thickness implies.  This is approximated in the MC by adjusting the
    /// P0Dule thickness to equal this value minus the GetP0DuleFrameInset
    /// value which can be tuned.  Checked against parts.
  double GetSideMPPCFrameThickness() const {return 38.75*CLHEP::mm;}

    /// Get the amount that the light tight skin is inset into the frames.
    /// Twice this amount must be subtracted from the frame thickness to find
    /// the P0Dule stacking length.  This is consistent with the tolerance on
    /// measured values (0.5 +- 0.1 mm for the entire P0Dule), and is tuned to
    /// match the lamination thickness.
  double GetP0DuleFrameInset() const {return 0.28*CLHEP::mm;}

    /// Get the width of the Side MPPC frame.  Checked against parts.
  double GetSideMPPCFrameWidth() const {return 30*CLHEP::mm;}

    /// Get the length of the side MPPC frame.  This ignores the dovetail with
    /// the top and bottom frames.  The dovetail material is added to the
    /// top and bottom.  Checked against parts.
  double GetSideMPPCFrameLength() const {return 2278*CLHEP::mm;}

    /// Get the thickness of the Top MPPC frame.  Note that the frame pieces
    /// have insets for the light tight skins.  In the MC this is approximated
    /// by not extending the skin over the frame.  Checked against
    /// parts.  NOTE: See GetSideMPPCFrameThickness().
  double GetTopMPPCFrameThickness() const {return 38.75*CLHEP::mm;}

    /// Get the width of the Top MPPC frame.  Checked against parts.
  double GetTopMPPCFrameWidth() const {return 30*CLHEP::mm;}

    /// Get the length of the top MPPC frame.  This ignores the dovetail with
    /// the side frames.  The dovetail material is added to the
    /// top and bottom.  Checked against parts.
  double GetTopMPPCFrameLength() const {return 2200*CLHEP::mm;}

    /// Get the thickness of the Side LI frame.  Note that the frame pieces
    /// have insets for the light tight skins.  In the MC this is approximated
    /// by not exgtending the skin over the frame.  Checked against parts.
    /// NOTE: See GetSideMPPCFrameThickness().
  double GetSideLIFrameThickness() const {return 38.75*CLHEP::mm;}

    /// Get the width of the Side LI frame.  Checked against parts.
  double GetSideLIFrameWidth() const {return 30*CLHEP::mm;}

    /// Get the length of the side LI frame.  This ignores the dovetail with
    /// the top and bottom frames.  The dovetail material is added to the
    /// top and bottom.  Checked against parts.
  double GetSideLIFrameLength() const {return 2278*CLHEP::mm;}

    /// Get the thickness of the Top LI frame.  Note that the frame pieces
    /// have insets for the light tight skins.  In the MC this is approximated
    /// by not extending the skin over the frame.  Checked against parts.
    /// NOTE: See GetSideMPPCFrameThickness().
  double GetBottomLIFrameThickness() const {return 38.75*CLHEP::mm;}

    /// Get the width of the Top LI frame.  Checked against parts.
  double GetBottomLIFrameWidth() const {return 30*CLHEP::mm;}

    /// Get the length of the top LI frame.  This ignores the dovetail with
    /// the side frames.  The dovetail material is added to the
    /// top and bottom.  Checked against parts.
  double GetBottomLIFrameLength() const {return 2200*CLHEP::mm;}

    /// Get the LI cover thickness.
  double GetLICoverThickness() const {return 9.8*CLHEP::mm;}
    
    /// Get the thickness of the light tight sheet.
  double GetLightTightThickness(void) const {return 1.375*CLHEP::mm;}

    /// @{ Get the width of the light tight sheet.  There is a cut out in the
    /// frame pieces to make space for the light tight sheet, but the MC
    /// approximates this by leaving the frame as a box, and starting the
    /// sheet just inside of the frame.  For that reason, the sheet size is
    /// calculated.
    double GetLightTightHeight(void) const;
    double GetLightTightWidth(void) const;
    // @}

    /// Get the space required to hold the MPPCs and cables.  This is the
    /// amount that the MPPC shroud extends beyond the glued part of the
    /// P0Dule.
  double GetMPPCSpace(void) const {return 21*CLHEP::mm;}
    
    /// Get the diameter of the MPPC shrouds.
  double GetMPPCShroudDiameter(void) const {return 10*CLHEP::mm;}

    /// Get the number of X (vertical) bars.
    int GetNumberOfXBars() const {return 126;}

    /// Get the number of Y (horizontal) bars.
    int GetNumberOfYBars() const {return 134;}

    virtual G4VisAttributes* GetVisual(void) const;

private:

    double fEpoxyThickness;
    G4VisAttributes* fVisual;

    void Init(void);
};
    
#endif
