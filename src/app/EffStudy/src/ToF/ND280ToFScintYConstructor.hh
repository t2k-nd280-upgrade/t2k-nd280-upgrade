#ifndef ND280ToFScintYConstructor_hh_seen
#define ND280ToFScintYConstructor_hh_seen

#include <G4LogicalVolume.hh>
#include <G4String.hh>

#include "ND280ScintLayerConstructor.hh"
//#include "ecal/ND280ECal.hh"

/// Constructs a layer of scintillator bars
class ND280ToFScintYConstructor : public ND280ScintYLayerConstructor {
public:
    ND280ToFScintYConstructor(G4String name, ND280Constructor* parent)
        : ND280ScintYLayerConstructor(name, parent) {Init();}
    
    virtual ~ND280ToFScintYConstructor(){};
    
    /// Set up must be called before GetPiece, in order to set the
    /// state of the scintillator layer required
    void SetUp(int n_bars, 
               double bar_length,
	           double bar_base,
               double bar_width);
        
    
private:
    void Init();

    /// The detector the layer is being constructed for. Allows for
    /// correct aquisition of the bar length
    //ND280ECal::Detector fDetector;
};

#endif
