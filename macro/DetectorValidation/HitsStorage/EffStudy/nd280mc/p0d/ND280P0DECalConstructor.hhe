#ifndef ND280P0DECalConstructor_hh_seen
#define ND280P0DECalConstructor_hh_seen

#include <G4String.hh>
#include <G4LogicalVolume.hh>

#include "ND280Constructor.hh"
#include "ecal/ND280ECalConstructor.hh"
#include "ecal/ND280ECal.hh"

/// Construct a P0D ECal module.  The module to be constructed must be set
/// using the SetModule method.
class ND280P0DECalConstructor : public ND280ECalConstructor {
public:
    
    ND280P0DECalConstructor(G4String name, ND280Constructor* parent);
    virtual ~ND280P0DECalConstructor() {}
    
    /// Build a p0d ecal module (with a Top, Side or Bottom embedded.
    virtual G4LogicalVolume* GetPiece();
    
    /// Set the type of module to build.
    void SetModule(ND280ECal::Module mod);

private:
    
    ND280ECal::Module fModule;
};

#endif
