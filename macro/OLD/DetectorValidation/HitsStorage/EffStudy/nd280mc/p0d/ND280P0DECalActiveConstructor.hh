#ifndef ND280P0DECalActiveConstructor_hh_seen
#define ND280P0DECalActiveConstructor_hh_seen

#include <G4LogicalVolume.hh>
#include <G4String.hh>

#include "ND280Constructor.hh"
#include "ecal/ND280ECal.hh"

/// Constructs an "Active" region, containing layers of scintillator,
/// layers of lead and the aluminium frame which surrounds them.
class ND280P0DECalActiveConstructor : public ND280Constructor {
public:
    ND280P0DECalActiveConstructor(G4String name, ND280Constructor* parent)
        : ND280Constructor(name, parent) {Init();}
    virtual ~ND280P0DECalActiveConstructor(){};
    
    /// Must be called before GetPiece()
    void SetIsSide(bool is_side) {fIsSide = is_side;}
    
    /// Returns the logical volume containing the "Active" volume
    G4LogicalVolume* GetPiece();
    
    /// Length of the volume along the x axis
    double GetLengthX() { return fLengthX; }
    
    /// Width of the volume along the y axis
    double GetWidthY() { return fWidthY; }
    
    /// Height of the volume is the size perpendicular to the plane
    /// of the layers (which lie in x-y) 
    double GetHeightZ() { return fHeightZ; }
    
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
