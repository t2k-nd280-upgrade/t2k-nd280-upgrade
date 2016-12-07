#ifndef ND280ECalModuleConstructor_hh_seen
#define ND280ECalModuleConstructor_hh_seen

#include <string>

//#include <ND280Constructor.hh>

#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4AssemblyVolume.hh>

/// Constructor of an ECal module. A base class for all of the ECal modules.
//class ND280ECalModuleConstructor : public ND280Constructor {
class ND280ECalModuleConstructor {
public:
  
  //ND280ECalModuleConstructor(G4String name, ND280Constructor* parent)
  //    : ND280Constructor(name, parent) {Init();}
  ND280ECalModuleConstructor(G4String name){Init();}
  
    virtual ~ND280ECalModuleConstructor();
  
    /// Return a G4LogicalVolume containing the ECal module
    virtual G4LogicalVolume* GetPiece() = 0;
  
    /// Return the length of the ECal module volume (length is parallel
    /// to the beam direction)
    double GetLengthX() const { return fLengthX; }
    void SetLengthX(double v) {fLengthX=v;}

    /// Return the width of the ECal module volume (width is perpendicular
    /// to the beam direction)
    double GetWidthY() const { return fWidthY; }
    void SetWidthY(double v) {fWidthY=v;}
  
    /// Return the height of the ECal module volume (height is perpendicular
    /// to the plane of the layers)
    double GetHeightZ() const { return fHeightZ; }
    void SetHeightZ(double v) {fHeightZ = v;}
    
private:
  
    /// Virtual function in which the module should initialise variables
    /// which control is construction
    void Init();
  
    /// Length of the ECal module volume (length is parallel
    /// to the beam direction)
    double fLengthX;
  
    /// Width of the ECal module volume (width is perpendicular
    /// to the beam direction)
    double fWidthY;
  
    /// Height of the ECal module volume (height is perpendicular
    /// to the plane of the layers)
    double fHeightZ;
};

#endif
