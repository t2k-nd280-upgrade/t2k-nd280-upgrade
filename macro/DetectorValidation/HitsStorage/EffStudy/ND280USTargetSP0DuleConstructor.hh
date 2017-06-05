#ifndef ND280USTargetSP0DuleConstructor_hh_seen
#define ND280USTargetSP0DuleConstructor_hh_seen
// $Id: ND280USTargetSP0DuleConstructor.hh,v 1.6 2011/09/08 21:36:13 mcgrew Exp $
//

#include "ND280Constructor.hh"

/// Create the Upstream Water Target SuperP0Dule.  This volume includes the
/// electronics mounted on the side of the superP0Dule.
class ND280USTargetSP0DuleConstructor: public ND280Constructor {
public:
    ND280USTargetSP0DuleConstructor(G4String n, 
                                    //ND280UserDetectorConstruction* c)
                                    ExN02DetectorConstruction* c)
        : ND280Constructor(n,c) {Init();};
    ND280USTargetSP0DuleConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();};
    virtual ~ND280USTargetSP0DuleConstructor();

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume *GetPiece(void);

    /// Get the length of the SuperP0Dule volume.
    double GetLength(void);
    
    /// Get the height of the SuperP0Dule volume.
    double GetHeight(void);

    /// Get the width of the SuperP0Dule volume.
    double GetWidth(void);

    /// Method to set translation parameters in a P0Dule.  The third component
    /// of the vector (z) is ignored since the P0Dules are closely packed.
    void SetP0DuleTrans(int mod, const G4ThreeVector& trans);
    
    /// Method to get translation parameters in a P0Dule.  The third component
    /// of the vector (z) is ignored since the P0Dules are closely packed.
    G4ThreeVector GetP0DuleTrans(int mod) const;

    /// @{Get and Set the amount of wasted space between the P0Dule and the
    /// brass layer.  The P0Dule is not exactly flat, and the brass is not
    /// perfectly flexible so this interface takes up a small amount of space
    /// that is essentially airfilled.  The space is fraction of a mm.  Based
    /// on external measurements after construction, the airspace is about 0.3
    /// mm.
    void SetWastedSpace(double s) {fWastedSpace = s;}
    double GetWastedSpace() const {return fWastedSpace;}
    /// @}

    /// @{Get and Set the offset between the top of the MPPC shrouds and the
    /// electronics ladder.  This is the space required to install the MPPCs
    void SetTopLadderOffset(double v) {fTopLadderOffset = v;}
    double GetTopLadderOffset() const {return fTopLadderOffset;}
    void SetSideLadderOffset(double v) {fSideLadderOffset = v;}
    double GetSideLadderOffset() const {return fSideLadderOffset;}
    /// @}

private: 
    /// Array of G4ThreeVectors to store translation parameters.  The third
    /// component (dz) is ignored.
    G4ThreeVector fP0DuleTrans[13];
    
    /// The space between the radiator and the P0Dule.
    double fWastedSpace;

    /// The space between the top of the P0Dules and the bottom of the top
    /// electronics ladders.
    double fTopLadderOffset;

    /// The space between the side of the P0Dules and the "bottom" of the side
    /// electronics ladders.
    double fSideLadderOffset;

    void Init();
};
#endif
