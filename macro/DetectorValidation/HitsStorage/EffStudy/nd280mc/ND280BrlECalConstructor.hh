#ifndef ND280BrlECalConstructor_hh_seen
#define ND280BrlECalConstructor_hh_seen

// nd280mc
#include <ND280Constructor.hh>
#include <ecal/ND280ECalConstructor.hh>
#include <ecal/ND280ECal.hh>

// G4
#include <G4String.hh>
#include <G4LogicalVolume.hh>

/// Construct a Barrel ECal module.  The module to be constructed must be set
/// using the SetModule method.
class ND280BrlECalConstructor : public ND280ECalConstructor {
public:
    
    ND280BrlECalConstructor(G4String name, ND280Constructor* parent);
    
    virtual ~ND280BrlECalConstructor(){}
    
    /// Build a piece of the barrel ecal.
    virtual G4LogicalVolume* GetPiece();

    /// Set the type of module to build.
    void SetModule(ND280ECal::Module mod);
    
private:
    
    ND280ECal::Module fModule;
};

#endif
