#ifndef ND280ECalScintYConstructor_hh_seen
#define ND280ECalScintYConstructor_hh_seen

#include <G4LogicalVolume.hh>
#include <G4String.hh>

#include "ND280ScintLayerConstructor.hh"
#include "ecal/ND280ECal.hh"

/// Constructs a layer of scintillator bars
class ND280ECalScintYConstructor : public ND280ScintYLayerConstructor {
public:
    ND280ECalScintYConstructor(G4String name, ND280Constructor* parent)
        : ND280ScintYLayerConstructor(name, parent) {Init();}
    
    virtual ~ND280ECalScintYConstructor(){};
    
    /// Set up must be called before GetPiece, in order to set the
    /// state of the scintillator layer required
    void SetUp(int n_bars, 
               double bar_length,
               ND280ECal::Detector detector);
    
private:
    void Init();

    /// The detector the layer is being constructed for. Allows for
    /// correct aquisition of the bar length
    ND280ECal::Detector fDetector;
};

#endif
