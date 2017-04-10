#ifndef ND280WAGASCIActiveConstructor_hh_seen
#define ND280WAGASCIActiveConstructor_hh_seen

class G4LogicalVolume;

#include "ND280Constructor.hh"

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
  
  /// Set Rotation Matrix
  void SetXRotation(double r) {fXRotation=r;}
  void SetYRotation(double r) {fYRotation=r;}
  void SetZRotation(double r) {fZRotation=r;}

  /// Get Rotation Matrix
  double GetXRotation() const {return fXRotation;}
  double GetYRotation() const {return fYRotation;}
  double GetZRotation() const {return fZRotation;}
  
    private:

    /// The width of the P0D in the X direction.
    double fWidth;

    /// The height of the P0D in the Y direction.
    double fHeight;

    /// The rotation angles about the X, Y and Z axes.  
    double fXRotation;
    double fYRotation;
    double fZRotation;

  */
  
    /// Initialize the constructor.
    void Init(void);
  
};
#endif
