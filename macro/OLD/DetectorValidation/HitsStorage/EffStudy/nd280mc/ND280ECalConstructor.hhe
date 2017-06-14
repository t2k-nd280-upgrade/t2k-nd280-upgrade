#ifndef ND280ECalConstructor_hh_seen
#define ND280ECalConstructor_hh_seen

#include <string>

#include <G4String.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4AssemblyVolume.hh>

#include "ND280Constructor.hh"
#include "ecal/ND280ECal.hh"

class ND280ECalConstructor : public ND280Constructor {
public:
    
    ND280ECalConstructor(G4String name, ND280Constructor* parent);
    
    virtual ~ND280ECalConstructor() {}
    
    virtual G4LogicalVolume* GetPiece() = 0;
    
    /// Get the volume's width - extent along the x axis
    double GetWidth() const  { return fWidthX;  }

    /// Set the volume width 
    void SetWidth(double v) { fWidthX = v; }

    /// Get the volume's height - extent along the y axis
    double GetHeight() const { return fHeightY; }

    /// Set the volume height.
    void SetHeight(double v) { fHeightY = v; }

    /// Get the volume's length - extent along the z axis
    double GetLength() const { return fLengthZ; }
    
    /// Set the volume length
    void SetLength(double v) { fLengthZ = v; }

    /// Get the total offset being applied to the module w.r.t.
    /// it's reference position
    G4ThreeVector GetOffset() const;
    
    /// Get the design offset of the module w.r.t. it's reference
    /// position
    G4ThreeVector GetDesignOffset() const;
    
    /// Set the design offset of the module w.r.t. it's reference position. 
    void SetDesignOffset(const G4ThreeVector& v) { fDesignOffset = v; }

    /// Get the additional offset being applied to the module w.r.t.
    /// it's design position
    G4ThreeVector GetExtraOffset() const { return fExtraOffset; }
    
    /// Get the axis about which any rotation will be applied
    G4ThreeVector GetRotationAxis() const { return fRotationAxis; }
    
    /// Get the angle through which the module will be rotated
    double GetRotationAngle() const { return fRotationAngle; }
    
    /// Set whether this should be built or not
    void SetBuild(const bool build) { fBuild = build; }
    
    /// Set the offset of the module w.r.t. its design position
    void SetExtraOffset(const G4ThreeVector offset);
    
    /// Apply an additional offset of the module w.r.t its current
    /// offset position
    void ApplyExtraOffset(const G4ThreeVector offset);
    
    /// Set the axis about which any rotation will be applied
    void SetRotationAxis(const G4ThreeVector axis);
    
    /// Set the angle through which the module will be rotated
    void SetRotationAngle(const double angle);

    /// Check if this a left clamshell is being built.
    bool GetIsLeftClamshell() const {return fIsLeftClamshell;}

    /// Call to specify that the ECal is destined for the left clamshell
    void SetLeftClamshell();
    
    /// Call to specify that the ECal is destined for the right clamshell
    void SetRightClamshell();
    
private:
    
    /// Flag as to whether this is the left (south) side of the magnet
    bool fIsLeftClamshell;
    
    bool fBuild;
    
    double fHeightY;
    double fWidthX;
    double fLengthZ;
    
    G4ThreeVector fDesignOffset;
    G4ThreeVector fExtraOffset;
    G4ThreeVector fRotationAxis;
    double fRotationAngle;
};
#endif
