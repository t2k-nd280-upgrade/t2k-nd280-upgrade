#include <globals.hh>
#include <G4Box.hh>
#include <G4String.hh>
#include <G4VSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4RotationMatrix.hh>

//#include <TND280Log.hxx>

#include "ND280Constructor.hh"
#include "ecal/ND280ECal.hh"
#include "ecal/ND280BrlECalLeftConstructor.hh"
#include "ecal/ND280BrlECalRightConstructor.hh"
#include "ecal/ND280BrlECalBLTRConstructor.hh"
#include "ecal/ND280BrlECalTLBRConstructor.hh"

#include "ecal/ND280BrlECalConstructor.hh"

ND280BrlECalConstructor::ND280BrlECalConstructor(G4String name, 
                                                 ND280Constructor* parent)
    : ND280ECalConstructor(name, parent) {
    AddConstructor(new ND280BrlECalLeftConstructor("LeftSide", this));
    AddConstructor(new ND280BrlECalRightConstructor("RightSide", this));
    AddConstructor(new ND280BrlECalTLBRConstructor("TopLeftBotRight", this));
    AddConstructor(new ND280BrlECalBLTRConstructor("BotLeftTopRight", this));
}

void ND280BrlECalConstructor::SetModule(ND280ECal::Module mod) {
    fModule = mod;
    switch (fModule) {
    case ND280ECal::kTop:
        SetDesignOffset(ND280ECal::Offsets::BrlTop);
        break;
    case ND280ECal::kSide:
        SetDesignOffset(ND280ECal::Offsets::BrlSide);
        break;
    case ND280ECal::kBottom:
        SetDesignOffset(ND280ECal::Offsets::BrlBottom);
        break;
    default:
      //G4Exception("Invalid BrlECal Module Set");
      G4ExceptionDescription msg;
      msg << "Invalid BrlECal Module Set" << G4endl;
      G4Exception("ND280BrlECalConstructor::SetModule",
		  "MyCode0002",FatalException, msg);
    }
}

G4LogicalVolume* ND280BrlECalConstructor::GetPiece() {
    G4RotationMatrix* module_rotation = new G4RotationMatrix();
    G4LogicalVolume* module_logical = NULL;
    G4String module_name;

    if( fModule == ND280ECal::kSide ) {
        module_name = "Side";
        if(GetIsLeftClamshell()) {
            ND280ECalModuleConstructor& module
                = Get<ND280ECalModuleConstructor>("LeftSide");
            module_logical = module.GetPiece();
            module_rotation->rotateX(-180*CLHEP::degree);
            module_rotation->rotateY(-90*CLHEP::degree);
            SetWidth(module.GetHeightZ());
            SetHeight(module.GetWidthY());
            SetLength(module.GetLengthX());
        }
        else {
            ND280ECalModuleConstructor& module
                = Get<ND280ECalModuleConstructor>("RightSide");
            module_logical = module.GetPiece();
            module_rotation->rotateX(-180*CLHEP::degree);
            module_rotation->rotateY(90*CLHEP::degree);
            SetWidth(module.GetHeightZ());
            SetHeight(module.GetWidthY());
            SetLength(module.GetLengthX());
        }
    }
    else if( fModule == ND280ECal::kTop ) {
        module_name = "Top";
        if(GetIsLeftClamshell()) {
            ND280ECalModuleConstructor& module
                = Get<ND280ECalModuleConstructor>("TopLeftBotRight");
            module_logical = module.GetPiece();
            module_rotation->rotateY(90*CLHEP::degree);
            module_rotation->rotateX(90*CLHEP::degree);
            SetWidth(module.GetWidthY());
            SetHeight(module.GetHeightZ());
            SetLength(module.GetLengthX());
        }
        else {
            ND280ECalModuleConstructor& module
                = Get<ND280ECalModuleConstructor>("BotLeftTopRight");
            module_logical = module.GetPiece();
            module_rotation->rotateY(-90*CLHEP::degree);
            module_rotation->rotateX(90*CLHEP::degree);
            SetWidth(module.GetWidthY());
            SetHeight(module.GetHeightZ());
            SetLength(module.GetLengthX());
        }
    }
    else if( fModule == ND280ECal::kBottom ) {
        module_name = "Bottom";
        if(GetIsLeftClamshell()) {
            ND280ECalModuleConstructor& module
                = Get<ND280ECalModuleConstructor>("BotLeftTopRight");
            module_logical = module.GetPiece();
            module_rotation->rotateY(-90*CLHEP::degree);
            module_rotation->rotateX(-90*CLHEP::degree);
            SetWidth(module.GetWidthY());
            SetHeight(module.GetHeightZ());
            SetLength(module.GetLengthX());
        }
        else {
            ND280ECalModuleConstructor& module
                = Get<ND280ECalModuleConstructor>("TopLeftBotRight");
            module_logical = module.GetPiece();
            module_rotation->rotateY(90*CLHEP::degree);
            module_rotation->rotateX(-90*CLHEP::degree);
            SetWidth(module.GetWidthY());
            SetHeight(module.GetHeightZ());
            SetLength(module.GetLengthX());
        }
    }
    else {
      //G4Exception("Invalid Brl ECal Module Built");
      G4ExceptionDescription msg;
      msg << "Invalid BrlECal Module Built" << G4endl;
      G4Exception("ND280BrlECalConstructor::GetPiece",
		  "MyCode0002",FatalException, msg);
    }
	
	
    // Volume
	
    G4LogicalVolume* logical 
        = new G4LogicalVolume(new G4Box(GetName(), 
                                        (GetWidth()/2.0), 
                                        (GetHeight()/2.0), 
                                        (GetLength()/2.0)),
                              FindMaterial("Air"), 
                              GetName());

    new G4PVPlacement(module_rotation,              // rotation
                      G4ThreeVector(0.0, 0.0, 0.0), // position
                      module_logical,               // logical volume
                      GetName()+"/"+module_name,    // name
                      logical,                      // mother volume
                      false,                        // no boolean operations
                      0);                           // copy number
	
    std::string name(module_name);

    if(GetIsLeftClamshell()) {
        name.append(" Left");
    }
    else {
        name.append(" Right");
    }
    
    //ND280Log("Brl ECal " << name << " module:");
    //ND280Log("% Width  (x) " << GetWidth() << "mm");
    //ND280Log("% Height (y) " << GetHeight() << "mm");
    //ND280Log("% Length (z) " << GetLength() << "mm");

    G4cout << "Brl ECal " << name << " module:" << G4endl;
    G4cout << "% Width  (x) " << GetWidth() << "mm" << G4endl;
    G4cout << "% Height (y) " << GetHeight() << "mm" << G4endl;
    G4cout << "% Length (z) " << GetLength() << "mm" << G4endl;
    
    return logical;
}
