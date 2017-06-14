#ifndef ND280UA1MRDSideConstructor_hh_seen
#define ND280UA1MRDSideConstructor_hh_seen
// $Id: ND280UA1MRDSideConstructor.hh,v 1.3 2009/05/05 14:25:35 mcgrew Exp $
//

#include <G4ThreeVector.hh>
class G4LogicalVolume;

#include "ND280Constructor.hh"
//#include "ND280SensitiveDetectorFactory.hh"
//#include "ND280SegmentSD.hh"

// NEW ND280UPGRADE
#include "ND280RootPersistencyManager.hh" // XML input file
#include "ExN02ND280XML.hh" // XML input file 
//

/// Create a UA1 MRD volume will contain one "slot" worth of MRD for the sides
/// of the UA1 Magnet.  The method GetPiece() constructs a new unplaced slab
/// with the normal along the Y axis, and the "photosensor" axis along the Z.
/// The calling code must rotate the object into position and and place the
/// object into a large mother volume.
class ND280UA1MRDSideConstructor : public ND280Constructor {
public:
  //ND280UA1MRDSideConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280UA1MRDSideConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280Constructor(n,c) {Init();};
    ND280UA1MRDSideConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();};
    virtual ~ND280UA1MRDSideConstructor();

    /// Set the yoke number for this MRD.  
    void SetYokeIndex(int i) {fYokeIndex = i;}

    /// Set the layer number for this MRD.  The layers are counted from the
    /// inner most slot toward the outside.  The number of layers that are
    /// filled vary depending on the yoke.
    void SetSlotLayer(int i) {fSlotLayer = i;}

    /// Set the slot index for this MRD.  The slot zero is in the center, and
    /// then the odd slots count up, and the even slots count down.
    void SetSlotIndex(int i) {fSlotIndex = i;}

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.
    virtual G4LogicalVolume *GetPiece(void);

    /// Set the width of the MRD side modules.
    void SetWidth(double w) {fWidth = w;}

    /// Get the width of the MRDS side modules.
    double GetWidth(void) {return fWidth;}

    /// Set the height of the MRD side modules.  This must be less than the
    /// thickness of the slot in the UA1 flux return coils.
    void SetHeight(double w) {fHeight = w;}

    /// Get the height of the MRD side modules.  This is less than the thickness
    /// of the slot in the UA1 flux return coils.
    double GetHeight(void) {return fHeight;}

    /// Set the length of the MRD side modules along the beam axis.  This is
    /// nominally the thickness of the UA1 flux return coil.
    void SetLength(double w) {fLength = w;}

    /// Get the length of the MRD side modules along the beam axis.  This is
    /// nominally the thickness of the UA1 flux return coil.
    double GetLength(void) {return fLength;}

    // /// Set the sensitive detector for this component.
    // virtual void SetSensitiveDetector(G4VSensitiveDetector* s) {
    //     fSensitiveDetector = s;
    // }

    // /// Set the sensitive detector for this component by name.
    // virtual void SetSensitiveDetector(G4String name, G4String type) {
    //     ND280SensitiveDetectorFactory factory(type);
    //     SetSensitiveDetector(factory.MakeSD(name));
    //     SetMaximumHitSagitta(fMaximumHitSagitta);
    //     SetMaximumHitLength(fMaximumHitLength);
    // }

    // /// Get the sensitive detector for this component.
    // virtual G4VSensitiveDetector* GetSensitiveDetector(void) {
    //     return fSensitiveDetector;
    // }

    // virtual void SetMaximumHitSagitta(double sagitta) {
    //     fMaximumHitSagitta = sagitta;
    //     ND280SegmentSD *segSD 
    //         = dynamic_cast<ND280SegmentSD*>(fSensitiveDetector);
    //     if (segSD) {
    //         segSD->SetMaximumHitSagitta(fMaximumHitSagitta);
    //     }
    // }

    // virtual void SetMaximumHitLength(double length) {
    //     fMaximumHitLength = length;
    //     ND280SegmentSD *segSD 
    //         = dynamic_cast<ND280SegmentSD*>(fSensitiveDetector);
    //     if (segSD) {
    //         segSD->SetMaximumHitLength(fMaximumHitLength);
    //     }
    // }
  
  // NEW ND280UPGRADE
  ExN02ND280XML * GetND280XML() const {return fND280XMLInput;};

private:

    /// The width of the MRD side modules (horizontal size
    /// perpendicular to the beam axis)
    double fWidth;

    /// The height of the MRD side modules (vertical size perpendicular to the
    /// beam axis).
    double fHeight;

    /// The length of the MRD side modules (parallel to the beam axis).
    double fLength;

    /// The sensitive detector for this tracking component.
    G4VSensitiveDetector* fSensitiveDetector;

    /// Control the total length of track that can be added to a single hit.
    double fMaximumHitLength;

    /// Control the maximum sagitta of a single hit.
    double fMaximumHitSagitta;
   
    /// The yoke of this MRD.  The yoke numbers are counted from upstream to
    /// down.  
    int fYokeIndex;

    /// The layer of this MRD.  The slot layers are counted from inner most to
    /// outermost.
    int fSlotLayer;

    /// The index of this MRD in the layer.  The zero index is in the
    /// center, then the odd layers are above, and the even layers below the
    /// center.
    int fSlotIndex;

    void Init(void);

  ExN02ND280XML *fND280XMLInput;
};
#endif
