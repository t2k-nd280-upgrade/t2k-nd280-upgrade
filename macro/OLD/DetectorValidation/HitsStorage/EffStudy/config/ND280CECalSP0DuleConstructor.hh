#ifndef ND280CECalSP0DuleConstructor_hh_seen
#define ND280CECalSP0DuleConstructor_hh_seen
// $Id: ND280CECalSP0DuleConstructor.hh,v 1.5 2011/06/07 15:01:51 mcgrew Exp $
//

#include "ND280Constructor.hh"

/// Create the Central ECal SuperP0Dule.  This volume includes the
/// electronics mounted on the side of the superP0Dule.
class ND280CECalSP0DuleConstructor: public ND280Constructor {
public:
  //ND280CECalSP0DuleConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280CECalSP0DuleConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280Constructor(n,c) {Init();};
    ND280CECalSP0DuleConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();};
    virtual ~ND280CECalSP0DuleConstructor();

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

    /// Get the thickness of the central ECal brace.
    double GetECalBraceThickness() const {return 0.75*25.4*mm;}

    /// Get the width of the central ECal brace bars.
    double GetECalBraceWidth() const {return 3*25.4*mm;}

    /// Method to set translation parameters in a P0Dule.  The third component
    /// of the vector (z) is ignored since the P0Dules are closely packed.
    void SetP0DuleTrans(int mod, const G4ThreeVector& trans);
    
    /// Method to get translation parameters in a P0Dule.  The third component
    /// of the vector (z) is ignored since the P0Dules are closely packed.
    G4ThreeVector GetP0DuleTrans(int mod) const;
    
    /// @{Get and Set the offset between the MPPC shrouds and the electronics
    /// ladder.  This is the space required to install the MPPCs
    void SetTopLadderOffset(double v) {fTopLadderOffset = v;}
    double GetTopLadderOffset() const {return fTopLadderOffset;}
    void SetSideLadderOffset(double v) {fSideLadderOffset = v;}
    double GetSideLadderOffset() const {return fSideLadderOffset;}
    /// @}

private: 
    /// Array of G4ThreeVectors to store translation parameters.  The third
    /// component (dz) is ignored.
    G4ThreeVector fP0DuleTrans[7];
    
    /// The space between the top of the P0Dules and the bottom of the top
    /// electronics ladders.
    double fTopLadderOffset;

    /// The space between the side of the P0Dules and the "bottom" of the side
    /// electronics ladders.
    double fSideLadderOffset;

    void Init();
};
#endif
