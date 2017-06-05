#ifndef ND280DsECalActiveConstructor_hh_seen
#define ND280DsECalActiveConstructor_hh_seen

#include <G4LogicalVolume.hh>
#include <G4String.hh>

#include "ecal/ND280ECal.hh"
#include "ND280Constructor.hh"

/// Constructs an "Active" region for the DsECal, containing layers of
/// scintillator, layers of lead and the aluminium frame which surrounds them.
class ND280DsECalActiveConstructor : public ND280Constructor {
public:
    ND280DsECalActiveConstructor(G4String name, ND280Constructor* parent)
        : ND280Constructor(name, parent) {Init();}
    virtual ~ND280DsECalActiveConstructor(){};
    
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

    /// Length of the volume along the x axis
    double fLengthX;
    
    /// Width of the volume along the y axis
    double fWidthY;
    
    /// Height of the volume is the size perpendicular to the plane
    /// of the layers (which lie in x-y)
    double fHeightZ;
};
#endif
