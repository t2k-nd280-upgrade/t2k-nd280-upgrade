#ifndef ND280DsECalConstructor_hh_seen
#define ND280DsECalConstructor_hh_seen

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4String.hh>

//#include <TND280Log.hxx>

#include "ND280Constructor.hh"
#include "ecal/ND280ECalModuleConstructor.hh"
#include "ecal/ND280ECalConstructor.hh"
#include "ecal/ND280ECalMessenger.hh"

// NEW ND280UPGRADE
#include "ND280RootPersistencyManager.hh" // XML input file
#include "ExN02ND280XML.hh" // XML input file 
//

class ND280DsECalModuleConstructor : public ND280ECalModuleConstructor {
public:
    
  ND280DsECalModuleConstructor(G4String name, ND280Constructor* parent)
    : ND280ECalModuleConstructor(name, parent) {Init();}
  virtual ~ND280DsECalModuleConstructor(){}
    
    G4LogicalVolume* GetPiece();
    
    /// Set whether or not to build the ECal
    void SetBuild(const bool build)   { fBuild = build; }
    
    double GetWidth() { return GetLengthX(); }
    double GetHeight() { return GetWidthY(); }
    double GetLength() { return GetHeightZ(); }
   
  // NEW ND280UPGRADE
  ExN02ND280XML * GetND280XML() const {return fND280XMLInput;};

private:
    
    void Init();
    
    /// Flag as to whether or not to build the module
    bool fBuild;

  ExN02ND280XML *fND280XMLInput;
};

class ND280DsECalConstructor : public ND280ECalConstructor {
public:
  ND280DsECalConstructor(G4String name, ND280Constructor* parent)
    : ND280ECalConstructor(name, parent) {Init();}

    virtual ~ND280DsECalConstructor(){}
  
    virtual G4LogicalVolume* GetPiece() {
        ND280DsECalModuleConstructor& module 
            = Get<ND280DsECalModuleConstructor>("Module");
        
        SetWidth(module.GetLengthX());
        SetHeight(module.GetWidthY());
        SetLength(module.GetHeightZ());

        G4LogicalVolume* module_logical = module.GetPiece();
        //ND280Log("DS ECal:");
        //ND280Log("% Length (x) " << GetWidth() << "mm");
        //ND280Log("% Width  (y) " << GetHeight() << "mm");
        //ND280Log("% Height (z) " << GetLength() << "mm");
        
	G4cout << "DS ECal:" 
	       << " Length (x) " << GetWidth() << "mm"
	       << " Width  (y) " << GetHeight() << "mm"
	       << " Height (z) " << GetLength() << "mm"
	       << G4endl;

	return module_logical;
    }

private:
    void Init() {
        SetDesignOffset(ND280ECal::Offsets::Downstream);
        SetMessenger(new ND280ECalMessenger(this,""));
        AddConstructor(new ND280DsECalModuleConstructor("Module", this));
    }
  
};
#endif
