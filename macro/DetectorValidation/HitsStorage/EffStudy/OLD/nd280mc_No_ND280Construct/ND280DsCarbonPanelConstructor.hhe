#ifndef ND280DsCarbonPanelConstructor_hh_seen
#define ND280DsCarbonPanelConstructor_hh_seen

#include <G4LogicalVolume.hh>

#include "ND280Constructor.hh"

/// Construct a logical volume containing dead-material for the DS ECal 
class ND280DsCarbonPanelConstructor: public ND280Constructor {
public:
  //ND280DsCarbonPanelConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280DsCarbonPanelConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280Constructor(n,c) {Init();}
    ND280DsCarbonPanelConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();}
    virtual ~ND280DsCarbonPanelConstructor(); 
		
    /// Create a carbon panel for the DsECal.
    virtual G4LogicalVolume *GetPiece();

    /// @{ Get or set the length of the panel.
    double GetLength() const {return fLength;}
    void SetLength(double v) {fLength = v;}
    /// @}
    
    /// @{ Get or set the width of the panel.
    double GetWidth() const {return fWidth;}
    void SetWidth(double v) {fWidth = v;}
    /// @}
    
    /// @{ Get or set the height of the panel.
    double GetHeight() const {return fHeight;}
    void SetHeight(double v) {fHeight = v;}
    /// @}
    
private:
    /// The length of the panel.
    double fLength;

    /// The width of the panel.
    double fWidth;

    /// The height of the panel.
    double fHeight;

    void Init(void);
};
#endif
