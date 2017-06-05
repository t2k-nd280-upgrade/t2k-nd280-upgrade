#ifndef ND280BrlECalActiveConstructor_hh_seen
#define ND280BrlECalActiveConstructor_hh_seen

#include <G4LogicalVolume.hh>
#include <G4String.hh>

#include "ecal/ND280ECal.hh"
#include "ND280Constructor.hh"

/// Constructs an "Active" region, containing layers of scintillator,
/// layers of lead and the aluminium frame which surrounds them.
class ND280BrlECalActiveConstructor : public ND280Constructor {
public:
    ND280BrlECalActiveConstructor(G4String name, ND280Constructor* parent)
        : ND280Constructor(name, parent) {Init();}
    virtual ~ND280BrlECalActiveConstructor() {}
    
    /// Must be called before GetPiece()
    void SetIsSide(const bool is_side) {fIsSide = is_side;}
    
    /// Returns the logical volume containing the "Active" volume
    G4LogicalVolume* GetPiece();
    
    /// Length of the volume along the x axis
    double GetLengthX() { return this->fLengthX; }
    
    /// Width of the volume along the y axis
    double GetWidthY() { return this->fWidthY; }
    
    /// Height of the volume is the size perpendicular to the plane
    /// of the layers (which lie in x-y) 
    double GetHeightZ() { return this->fHeightZ; }
    
private:
    void Init();

    /// Flag for if this is a side module
    bool fIsSide;
    
    /// Length of the volume along the x axis
    double fLengthX;
    
    /// Width of the volume along the y axis
    double fWidthY;
    
    /// Height of the volume is the size perpendicular to the plane
    /// of the layers (which lie in x-y)
    double fHeightZ;
};
#endif
