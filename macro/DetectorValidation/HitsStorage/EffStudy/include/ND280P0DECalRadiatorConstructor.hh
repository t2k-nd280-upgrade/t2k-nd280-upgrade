#ifndef ND280P0DECalRadiatorConstructor_hh_Seen
#define ND280P0DECalRadiatorConstructor_hh_Seen
// $Id: ND280P0DECalRadiatorConstructor.hh,v 1.1 2009/11/20 19:55:38 mcgrew Exp $
//

#include <G4ThreeVector.hh>
#include <G4VisAttributes.hh>
#include <G4LogicalVolume.hh>

#include "ND280Constructor.hh"

/// Construct an radiator used to construct the P0D upstream and central ecal
/// super-p0dules.  This is based on the "as-built" information for the
/// radiators manufactured at SBU.
class ND280P0DECalRadiatorConstructor : public ND280Constructor {
public:
    ND280P0DECalRadiatorConstructor(G4String n,
                                    //ND280UserDetectorConstruction* c)
                                    ExN02DetectorConstruction* c)
        : ND280Constructor(n,c) {Init();};
    ND280P0DECalRadiatorConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();};
    virtual ~ND280P0DECalRadiatorConstructor(); 

    /// Get the total length of the ecal radiator.
    double GetLength() const {
        return 2*GetSteelThickness()
            + GetLeadThickness() 
            + 2*GetEpoxyThickness();
    }

    /// Get the height of the ecal radiator.  The value of the height is fixed
    /// by the "As-Built" drawings.  Checked against the physical parts.
    double GetHeight() const {return 2350*mm;}
    
    /// Get the width of the ecal radiator.  The value of the width is fixed
    /// by the "As-Built" drawings.  Checked against the physical parts.
    double GetWidth() const {return 2200*mm;}

    /// Get the height of the steel skins. Checked against the physical parts.
    double GetSteelHeight() const {return 2340*mm;}

    /// Get the width of the steel skins.  Checked agains the physical parts.
    double GetSteelWidth() const {return 2190*mm;}

    /// Get the thickness of the steel skins for the P0D radiator.
    double GetSteelThickness() const {return 0.46*mm;}

    /// Set the average thickness of the lead.  The actual lead thickness will
    /// varry from plate to plate.
    void SetLeadThickness(double s) {fLeadThickness=s;}
    
    /// Get the average thickness of the lead.  The actual lead thickness will
    /// varry from plate to plate.
    double GetLeadThickness() const {return fLeadThickness;}

    /// Set the average thickness of the epoxy.  The actual epoxy thickness
    /// will be the amount of space remaining around the lead sheets.
    void SetEpoxyThickness(double s) {fEpoxyThickness=s;}
    
    /// Get the average thickness of the epoxy.  The actual epoxy thickness
    /// will be the amount of space remaining around the lead sheets.
    double GetEpoxyThickness() const {return fEpoxyThickness;}

    /// Get the "width" of the bottom aluminum frame.  This frame stretchs
    /// the full width of the radiator.
    double GetBottomFrame() const {return 98.43*mm;}

    /// Get the "width" of the side aluminum frame.
    double GetSideFrame() const {return 88.9*mm;}

    /// Get the "width of the top aluminum frame.  This frame stretchs the
    /// full width of the radiator.
    double GetTopFrame() const {return 88.9*mm;}

    /// Get the "length" of the side radiator.
    double GetSideFrameLength() const {return 2248.6*mm;}

    /// Get the "length" of the top radiator.
    double GetTopFrameLength() const {return 2019.7*mm;}

    /// Get the height of a lead plate.  This is the nominal size manufactured
    /// by Sullivan Metals, Inc.  The lead plates come in two varieties.  Most
    /// plates have the normal plate height, but four plates have half size
    /// (GetPlateHeight()/2).  The nominal full size plate is 15.875 inches by
    /// 17 inches.
    /// 
    /// Looking from upstream the plates are arranged 
    ///
    ///    f h f h f
    ///    f f f f f
    ///    f f f f f
    ///    f f f f f
    ///    f f f f f
    ///      h   h
    ///
    /// The weights have been measured for each plate.  This matches the
    /// layout of the plates recorded on the radiator assembly traveler.
    double GetPlateHeight() const {return 431.8*mm;}

    /// Get the width of a lead plate.  This is the nominal size manufactured
    /// by Sullivan Metals, Inc.
    double GetPlateWidth() const {return 403.23*mm;}

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume *GetPiece(void);

    /// Return the visual attributes for this constructor.
    virtual const G4VisAttributes* GetVisual(void) const;

protected:
    void Init(void);

    /// The thickness of the steel skins on the P0D radiator.
    double fSteelThickness;

    /// The average thickness of the lead plates.
    double fLeadThickness;

    /// The average thickness of the epoxy in the radiator.
    double fEpoxyThickness;
};
#endif
