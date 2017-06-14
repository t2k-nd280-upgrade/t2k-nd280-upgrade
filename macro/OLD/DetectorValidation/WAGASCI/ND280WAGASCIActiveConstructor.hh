#ifndef ND280WAGASCIActiveConstructor_hh_seen
#define ND280WAGASCIActiveConstructor_hh_seen
// $Id: ND280WAGASCIActiveConstructor.hh,v 1.6 2011/06/07 18:37:40 mcgrew Exp $
//

class G4LogicalVolume;

#include "ND280Constructor.hh"

/// Construct the active parts of the P0D (Pizero Detector).  This volume
/// incudes the super-P0Dules (and their electronics), but not the support
/// structure.  It can be positioned within the basket using
/// SetUpstreamClearance() which determines the distance from the upstream
/// end of the basket to the LI side of the upstream ecal,
/// SetSupportClearance() which determines the distance between the top of
/// the support to the bottom LI covers, and SetLIClearance() which
/// determines the distance from the side of the basket to the LI covers.
///
/// Note: The P0D actually rests on the supports, but the supports allow for
/// up and down movement throught the adjustment of the "jack screws."  Rather
/// than try to simulate the details of the jack screw adjustment, in the MC
/// the supports are simulated with the jack screws all the way down.  The
/// vertical position of the P0D is then set relative to the top of the
/// support structure (allowing an air gap the doesn't exist in reality).
///
/// The method get GetPiece() constructs a new unplaced P0D that is facing
/// along the Z axis.  The calling code should and place the object into a
/// large mother volume.  The calling code (e.g. ND280BasketConstructor)
/// should use GetUpstreamSpace() to get the distance from the upstream end of
/// the basket (the space at -Z), GetLISpace() to get the distance from the
/// side of the basket (the space at -X), and GetBottomSpace() to get the
/// distance from the P0D supports (the space at -Y).  The rotation of the P0D
/// is determined by GetXRotation(), GetYRotation(), and GetZRotation(), and
/// should be applied in that order.

//class ND280WAGASCIActiveConstructor : public ND280Constructor {
class ND280WAGASCIActiveConstructor {
public:
  //ND280WAGASCIActiveConstructor(G4String n, ND280UserDetectorConstruction* c)
  /*  ND280WAGASCIActiveConstructor(G4String n, ExN02DetectorConstruction* c)
      : ND280Constructor(n,c) {Init();};
  ND280WAGASCIActiveConstructor(G4String n, ND280Constructor* p)
  : ND280Constructor(n,p) {Init();};*/
  //ND280WAGASCIActiveConstructor() {Init();};

  //virtual ~ND280WAGASCIActiveConstructor();

  void Construct(G4LogicalVolume * TargetLV, G4ThreeVector ModulePosition, G4ThreeVector ModuleSize, const G4String RepositoryName);
  /*
    /// Get the width of the P0D volume.  The P0D volume width allows a small
    /// amount of space (GetWidthAdjustmentSpace()) to account for the relative
    /// alignment of the super-P0Dules.
    virtual double GetWidth(void);

    /// Get the height of the P0D volume.  The P0D volume height allows a
    /// small amount of space (GetHeightAdjustmentSpace()) to account for the
    /// relative alignment of the super-P0Dules.
    virtual double GetHeight(void);

    /// Get the length of the P0D volume.  The P0D volume length allows a
    /// small amount of space (GetLengthAdjustmentSpace()) to account for the
    /// relative alignment of the super-P0Dules.
    virtual double GetLength(void);

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume *GetPiece(void);

    /// Set the distance from the front of the basket to the downstream edge
    /// of the USECal on the LI side.  This is a simple value to measure in
    /// the basket and is used to calculate the relative position of the
    /// upstream ecal in the P0D volume.
    void SetUSECalDsLIPosition(double d) {
        fUSECalDsLIPosition = d;
    }

    /// Set the skew angle for the USECal.
    void SetUSECalSkewAngle(double d) {
        fUSECalSkewAngle = d;
    }

    /// Set the distance from the front of the basket to the downstream end of
    /// the USWT on the LI side.  This is a simple value to measure in the
    /// basket and is used to calculate the relative position of the upstream
    /// water target in the P0D volume.
    void SetUSWTDsLIPosition(double d) {
        fUSWTDsLIPosition = d;
    }

    /// Set the skew angle for the USWT.
    void SetUSWTSkewAngle(double d) {
        fUSWTSkewAngle = d;
    }

    /// Set the distance from the front of the basket to the downstream end of
    /// the CWT on the LI side.  This is a simple value to measure in the
    /// basket and is used to calculate the relative position of the central
    /// water target in the P0D volume.
    void SetCWTDsLIPosition(double d) {
        fCWTDsLIPosition = d;
    }

    /// Set the skew angle for the CWT.
    void SetCWTSkewAngle(double d) {
        fCWTSkewAngle = d;
    }

    /// Set the distance from the front of the basket to the downstream end of
    /// the CECal on the LI side.  This is a simple value to measure in the
    /// basket and is then used to calculate the relative position of the
    /// central ecal in the P0D volume.
    void SetCECalDsLIPosition(double d) {
        fCECalDsLIPosition = d;
    }

    /// Set the skew angle for the CECal.
    void SetCECalSkewAngle(double d) {
        fCECalSkewAngle = d;
    }

    /// Get the central position of the USECal.
    G4ThreeVector GetUSECalPosition();

    /// Get the rotation matrix for the USECal.
    G4RotationMatrix* GetUSECalRotation() const;

    /// Get the central position of the USWT.
    G4ThreeVector GetUSWTPosition();
    
    /// Get the rotation matrix for the USWT.
    G4RotationMatrix* GetUSWTRotation() const;

    /// Get the central position of the CWT.
    G4ThreeVector GetCWTPosition();
    
    /// Get the rotation matrix for the CWT.
    G4RotationMatrix* GetCWTRotation() const;

    /// Get the central position of the CECal.
    G4ThreeVector GetCECalPosition();
    
    /// Get the rotation matrix for the CECal.
    G4RotationMatrix* GetCECalRotation() const;

    /// @{ Get the length, width and Height of the USECal 
    double GetUSECalLength(void);
    double GetUSECalWidth(void);
    double GetUSECalHeight(void);
    // @}

    /// @{ Get the length, width and Height of the USTarget 
    double GetUSTargetLength(void);
    double GetUSTargetWidth(void);
    double GetUSTargetHeight(void);
    // @}

    /// @{ Get the length, width and Height of the CTarget 
    double GetCTargetLength(void);
    double GetCTargetWidth(void);
    double GetCTargetHeight(void);
    // @}

    /// @{ Get the length, width and Height of the CECal 
    double GetCECalLength(void);
    double GetCECalWidth(void);
    double GetCECalHeight(void);
    // @}

    /// Get the space allowed around the Super-P0Dules to allow for fine
    /// adjustment of the relative super-P0Dule positions.
    double GetHeightAdjustmentSpace(void) const {return fHeightAdjustmentSpace;}
    double GetWidthAdjustmentSpace(void) const {return fWidthAdjustmentSpace;}
    double GetLengthAdjustmentSpace(void) const {return fLengthAdjustmentSpace;}

    /// Set the amount of space available to adjust the relative super-P0Dule
    /// positions.
    void SetHeightAdjustmentSpace(double s) {fHeightAdjustmentSpace = s;}
    void SetWidthAdjustmentSpace(double s) {fWidthAdjustmentSpace = s;}
    void SetLengthAdjustmentSpace(double s) {fLengthAdjustmentSpace = s;}

    /// @{Set or get the space between the LI covers and the inner side of the
    /// basket.
    void SetLIClearance(double c) {fLIClearance = c;}
    double GetLIClearance(void) const {return fLIClearance;}
    /// @}

    /// @{Set or get the space between the upstream end of the LI covers on
    /// the USECal and the upstream end of the basket (inner side of the
    /// beam).
    void SetBottomClearance(double c) {fBottomClearance = c;}
    double GetBottomClearance(void) const {return fBottomClearance;}
    /// @}

    /// @{Set or get the space between the upstream end of the LI covers on
    /// the USECal and the upstream end of the basket.
    void SetUpstreamClearance(double c) {fUpstreamClearance = c;}
    double GetUpstreamClearance(void) const {return fUpstreamClearance;}
    /// @}

    /// @{Return the space between the edge of the P0D and the fiducial
    /// positions in the basket.  These values are calculated from
    /// GetUpstreamClearance(), GetLIClearance(), GetBottomClearance(), and
    /// Get*AdjustmentSpace().  The GetUpstreamSpace() is the amount added
    /// to the P0D Z position to account for the distance between the upstream
    /// end of the basket and the USECal.  The GetBottomSpace() is the amount
    /// added to Y to account for the position of the jack screws (vertical
    /// position of the P0D).  The GetLISpace is the amount added to X to
    /// account for the space between the basket and the LI.
    double GetUpstreamSpace(void) const;
    double GetLISpace(void) const;
    double GetBottomSpace(void) const; 
    /// @}

    /// @{Set or Get the P0D rotation angles about the X, Y, and Z axis.
    /// These should be small angles, and align the P0D based on survey
    /// results.  In the calling routine, the rotation should be constructed
    ///
    /// \code
    /// G4Rotation* p0dRot = new G4Rotation;
    /// p0dRot->rotateX(p0d.GetXRotation());
    /// p0dRot->rotateY(p0d.GetYRotation());
    /// p0dRot->rotateZ(p0d.GetZRotation());
    /// \endcode
    ///
    /// The call order matters.
    void SetXRotation(double r) {fXRotation=r;}
    void SetYRotation(double r) {fYRotation=r;}
    void SetZRotation(double r) {fZRotation=r;}
    double GetXRotation() const {return fXRotation;}
    double GetYRotation() const {return fYRotation;}
    double GetZRotation() const {return fZRotation;}
    /// @}

    /// @{Set the translation parameters for a super P0Dule.
    void SetUSECalTrans(const G4ThreeVector& t) {fUSECalTrans = t;}
    void SetUSTargetTrans(const G4ThreeVector& t) {fUSTargetTrans = t;}
    void SetCTargetTrans(const G4ThreeVector& t) {fCTargetTrans = t;}
    void SetCECalTrans(const G4ThreeVector& t) {fCECalTrans = t;}
    ///@}

    /// @{Get the translation parameters for a super P0Dule.
    const G4ThreeVector& GetUSECalTrans() const {return fUSECalTrans;}
    const G4ThreeVector& GetUSTargetTrans() const {return fUSTargetTrans;}
    const G4ThreeVector& GetCTargetTrans() const {return fCTargetTrans;}
    const G4ThreeVector& GetCECalTrans() const {return fCECalTrans;}
    /// @}
private:

    /// The width of the P0D in the X direction.
    double fWidth;

    /// The height of the P0D in the Y direction.
    double fHeight;

    /// @{ The distance from the basket to the downstream LI side of the
    /// USECal, USWT, CWT, and CECal.
    double fUSECalDsLIPosition;
    double fUSWTDsLIPosition;
    double fCWTDsLIPosition;
    double fCECalDsLIPosition;
    /// @}

    /// @{ The skew angle of the super P0Dules in the basket.
    double fUSECalSkewAngle;
    double fUSWTSkewAngle;
    double fCWTSkewAngle;
    double fCECalSkewAngle;
    /// @}

    /// The space available in the volume to allow for the relative
    /// positioning of the super-P0Dules.  This should be enough to allow for
    /// the relative super-P0Dule shifts, and rotations.  Any overall shift
    /// should be accommodated by moving the entire P0D.
    double fHeightAdjustmentSpace;
    double fWidthAdjustmentSpace;
    double fLengthAdjustmentSpace;

    /// @{See documentation for the getters for definitions.
    double fLIClearance;
    double fBottomClearance;
    double fUpstreamClearance;
    /// @}

    /// @{The rotation angles about the X, Y and Z axes.  
    double fXRotation;
    double fYRotation;
    double fZRotation;
    /// @}

    /// The alignment translation for the USECal.
    G4ThreeVector fUSECalTrans;

    /// The alignment translation for the USTarget.
    G4ThreeVector fUSTargetTrans;

    /// The alignment translation for the CTarget.
    G4ThreeVector fCTargetTrans;

    /// The alignment translation for the CECal.
    G4ThreeVector fCECalTrans;
  */
    /// Initialize the constructor.
    void Init(void);
  
};
#endif
