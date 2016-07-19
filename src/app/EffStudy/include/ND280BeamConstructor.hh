#ifndef ND280BeamConstructor_hh_seen
#define ND280BeamConstructor_hh_seen
//

class G4LogicalVolume;

//#include "ND280Constructor.hh"

//
// Taken from nd280mc. Cleaned a bit: it was a virtual class inherited from ND280Constructor.
// Here it's a stand-alone class to use in ExN02DetectorConstruction::BuildTPCCentralCathod
// In order to use exactly the same code as in nd280mc
// 

class ND280BeamConstructor {

public:

  ND280BeamConstructor(G4String n)
    : fMaterialName("Aluminum") {SetName(n); Init();};
  
  ~ND280BeamConstructor();
  
  /// Set the width of the beam.  This is the X dimension of the beam.
  void SetWidth(double w) {fWidth = w;}
  /// Get the width of the Beam.
  double GetWidth(void) {return fWidth;}

  /// Set the height of the beam.  This is the Y dimension of the beam.
  void SetHeight(double w) {fHeight = w;}
  /// Get the height of the beam.
  double GetHeight(void) {return fHeight;}
  
  /// Get the length of the beam.
  double GetLength(void) {return fLength;};
  /// Set the length of the beam.
  void SetLength(double t) {fLength = t;};
  
  /// Get the material name.
  G4String GetMaterialName(void) {return fMaterialName;};
  /// Set the material name.
  void SetMaterialName(G4String str) {fMaterialName = str;};

  // Set the material.
  G4Material * GetMaterial(void) {return fMaterial;};
  /// Set the material.
  void SetMaterial(G4Material *mat) {fMaterial = mat;};
    
  /// Get the detector name.
  G4String GetName(void) {return fDetectorName;};
  void SetName(G4String str) { fDetectorName = str;}; 
  
  // Set whether to draw the detector or not
  void SetVisible(bool isvisib=true){fIsVisible = isvisib;};
  bool GetVisible(){return fIsVisible;};
  
  /// Construct and return a G4 volume for the object.  This is a pure
  /// virtual function, which means it must be implemented by the inheriting
  /// classes.  This returns an unplaced logical volume which faces along
  /// the Z axis.
  G4LogicalVolume *GetPiece(void);
  
private:
  
  // The detector name
  G4String fDetectorName;

  // The detector visibility 
  bool fIsVisible;

  /// The width of the beam in the X direction.
  double fWidth;
  
  /// The height of the beam in the Y direction.
  double fHeight;
  
  /// The length of the beam along the Z axis.  
  double fLength;
  
  /// The name of the material from which the beam is made of.
  G4String fMaterialName;
  
  /// The material from which the beam is made of.
  G4Material* fMaterial;

  void Init(void);
  
};
#endif
