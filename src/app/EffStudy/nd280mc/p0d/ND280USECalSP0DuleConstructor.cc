#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>

//#include <TND280Log.hxx>

#include "p0d/ND280USECalSP0DuleConstructor.hh"
#include "p0d/ND280P0DuleConstructor.hh"
#include "p0d/ND280P0DECalRadiatorConstructor.hh"
#include "p0d/ND280P0DTopLadderConstructor.hh"
#include "p0d/ND280P0DSideLadderConstructor.hh"

class ND280USECalMessenger : public ND280ConstructorMessenger {

private:

    ND280USECalSP0DuleConstructor* fConstructor;
    
    // Array of Commands to adjust P0Dules and SuperP0Dules in Monte Carlo
    std::vector<G4UIcmdWith3VectorAndUnit*> fP0DuleTransCMD;
    G4UIcmdWithADoubleAndUnit* fTopLadderOffsetCMD;
    G4UIcmdWithADoubleAndUnit* fSideLadderOffsetCMD;
    
public:
    ND280USECalMessenger(ND280USECalSP0DuleConstructor* c)
        : ND280ConstructorMessenger(c,"Control the USECal geometry."),
          fConstructor(c) {
        
        // Make commands for Translation of modules.
        for (int i = 0; i < 7; ++i) {
            std::ostringstream cmdName;
            std::ostringstream guidanceName;
            cmdName << "setP0Dule" << i << "Translation";
            guidanceName << "Set the translation parameters dx, dy, and dz"
                         << " for P0Dule " << i << ".";
            fP0DuleTransCMD.push_back(new G4UIcmdWith3VectorAndUnit(
                                          CommandName(cmdName.str().c_str()),
                                          this));
            fP0DuleTransCMD.back()->SetGuidance(guidanceName.str().c_str());
            fP0DuleTransCMD.back()->SetParameterName("transX", 
                                                     "transY", 
                                                     "transZ",
                                                     false);
            fP0DuleTransCMD.back()->SetUnitCategory("Length");
        }

        fTopLadderOffsetCMD 
            = new G4UIcmdWithADoubleAndUnit(
                CommandName("topLadderOffset"),this);
        fTopLadderOffsetCMD->SetGuidance(
            "Set the space between the P0Dule and the top electronics ladder.");
        fTopLadderOffsetCMD->SetParameterName("offset",false);
        fTopLadderOffsetCMD->SetUnitCategory("Length");

        fSideLadderOffsetCMD 
            = new G4UIcmdWithADoubleAndUnit(
                CommandName("sideLadderOffset"),this);
        fSideLadderOffsetCMD->SetGuidance(
            "Set the space between P0Dule and the side electronics ladder.");
        fSideLadderOffsetCMD->SetParameterName("offset",false);
        fSideLadderOffsetCMD->SetUnitCategory("Length");
        
    }
    
    virtual ~ND280USECalMessenger() {
        for (std::vector<G4UIcmdWith3VectorAndUnit*>::iterator p
                 = fP0DuleTransCMD.begin();
             p != fP0DuleTransCMD.end();
             ++p) {
            if (*p) delete *p;
            *p = NULL;
        }
        delete fTopLadderOffsetCMD;
        delete fSideLadderOffsetCMD;
    }
    
    void SetNewValue(G4UIcommand *cmd, G4String val) {
        for (std::vector<G4UIcmdWith3VectorAndUnit*>::iterator p
                 = fP0DuleTransCMD.begin();
             p != fP0DuleTransCMD.end();
             ++p) {
            if (cmd== *p) {
                fConstructor->SetP0DuleTrans(p - fP0DuleTransCMD.begin(),
                                             (*p)->GetNew3VectorValue(val));
                return;
            }
        }
        if (cmd == fTopLadderOffsetCMD) {
            fConstructor->SetTopLadderOffset(
                fTopLadderOffsetCMD->GetNewDoubleValue(val));
        }
        else if (cmd == fSideLadderOffsetCMD) {
            fConstructor->SetSideLadderOffset(
                fSideLadderOffsetCMD->GetNewDoubleValue(val));
        }
        else {
            ND280ConstructorMessenger::SetNewValue(cmd,val);
        }
    }
};

ND280USECalSP0DuleConstructor::~ND280USECalSP0DuleConstructor() {}

void ND280USECalSP0DuleConstructor::Init(void) {
    // Set default translation values.
    for (int i=0; i<7; ++i) SetP0DuleTrans(i,G4ThreeVector(0,0,0));

    // Set the ladder offsets.
    SetTopLadderOffset(10*CLHEP::mm);
    SetSideLadderOffset(10*CLHEP::mm);

    AddConstructor(new ND280P0DuleConstructor("P0Dule",this));
    AddConstructor(new ND280P0DECalRadiatorConstructor("Radiator",this));
    AddConstructor(new ND280P0DSideLadderConstructor("SideLadder",this));
    AddConstructor(new ND280P0DTopLadderConstructor("TopLadder",this));

    SetMessenger(new ND280USECalMessenger(this));
}

double ND280USECalSP0DuleConstructor::GetLength() {
    ND280P0DuleConstructor& p0dule 
        =  Get<ND280P0DuleConstructor>("P0Dule");
    ND280P0DECalRadiatorConstructor& radiator
        = Get<ND280P0DECalRadiatorConstructor>("Radiator");

    double length = 7*p0dule.GetLength() + 7*radiator.GetLength();

    return length;
}

double ND280USECalSP0DuleConstructor::GetWidth() {
    ND280P0DuleConstructor& p0dule 
        =  Get<ND280P0DuleConstructor>("P0Dule");
    ND280P0DSideLadderConstructor &sideLadder 
        = Get<ND280P0DSideLadderConstructor>("SideLadder");

    return p0dule.GetWidth() + GetSideLadderOffset() 
        + sideLadder.GetLadderThickness();
}

double ND280USECalSP0DuleConstructor::GetHeight() {
    ND280P0DuleConstructor& p0dule 
        =  Get<ND280P0DuleConstructor>("P0Dule");
    ND280P0DTopLadderConstructor &topLadder 
        = Get<ND280P0DTopLadderConstructor>("TopLadder");

    return p0dule.GetHeight() + GetTopLadderOffset()
        + topLadder.GetLadderThickness();
}

/// Method to set translation parameters in a P0Dule
void ND280USECalSP0DuleConstructor::SetP0DuleTrans(int mod, 
						   const G4ThreeVector& trans) {
  if (mod < 0){
    //G4Exception("P0Dule number out of range");
    G4Exception("ND280USECalSP0DuleConstructor::SetP0DuleTrans",
		"MyCode0002",FatalException,
		"P0Dule number out of range");
  }
  if (mod > 7){
    //G4Exception("P0Dule number out of range");
    G4Exception("ND280USECalSP0DuleConstructor::SetP0DuleTrans",
		"MyCode0002",FatalException,
		"P0Dule number out of range");
  }
  fP0DuleTrans[mod] = trans;
}
    
/// Method to get translation parameters in a module
G4ThreeVector ND280USECalSP0DuleConstructor::GetP0DuleTrans(int mod) const {
  if (mod < 0){
    //G4Exception("P0Dule number out of range");
    G4Exception("ND280USECalSP0DuleConstructor::GetP0DuleTrans",
		"MyCode0002",FatalException,
		"P0Dule number out of range");
  }
  if (mod > 7){
    //G4Exception("P0Dule number out of range");
    G4Exception("ND280USECalSP0DuleConstructor::GetP0DuleTrans",
		"MyCode0002",FatalException,
		"P0Dule number out of range");
  }
  return fP0DuleTrans[mod];
}
    
G4LogicalVolume *ND280USECalSP0DuleConstructor::GetPiece(void) {
    ND280P0DuleConstructor& p0dule 
        =  Get<ND280P0DuleConstructor>("P0Dule");

    ND280P0DECalRadiatorConstructor& radiator
        = Get<ND280P0DECalRadiatorConstructor>("Radiator");

    G4VSolid *solid = new G4Box(GetName(),
                                GetWidth()/2, 
                                GetHeight()/2,
                                GetLength()/2);
    G4LogicalVolume *logVolume
        = new G4LogicalVolume(solid,
                              FindMaterial("Air"),
                              GetName());
    logVolume->SetVisAttributes(G4VisAttributes::Invisible);

    //////////////////////////////////////
    // Create the seven ecal layers.
    //////////////////////////////////////
    double xCenter = -GetWidth()/2 + p0dule.GetWidth()/2;
    double yCenter = -GetHeight()/2 + p0dule.GetHeight()/2;
    double xRadiator = -GetWidth()/2 + radiator.GetWidth()/2;
    double yRadiator = -GetHeight()/2 + radiator.GetHeight()/2;
    double zCenter = -GetLength()/2;
    for (int i=0; i<7; ++i) {
        double xP0Dule = xCenter + GetP0DuleTrans(i).x();
        double yP0Dule = yCenter + GetP0DuleTrans(i).y();
        zCenter += p0dule.GetLength()/2;
        new G4PVPlacement(0,                  // no rotation
                          G4ThreeVector(xP0Dule, yP0Dule, zCenter),// position 
                          p0dule.GetPiece(),  // logical volume
                          p0dule.GetName(),   // name
                          logVolume,          // mother  volume
                          false,              // no boolean operations
                          0);                 // no specific field
        zCenter += p0dule.GetLength()/2;

        zCenter += radiator.GetLength()/2;
        new G4PVPlacement(0,                    // no rotation
                          G4ThreeVector(xRadiator,yRadiator,zCenter),
                          radiator.GetPiece(),  // logical volume
                          radiator.GetName(),   // name
                          logVolume,            // mother  volume
                          false,                // no boolean operations
                          0);                   // no specific field
        zCenter += radiator.GetLength()/2;
    }

    ////////////////////////////////////
    // Build the electronics ladders. 
    ////////////////////////////////////

    ND280P0DTopLadderConstructor &topLadder 
        = Get<ND280P0DTopLadderConstructor>("TopLadder");
    new G4PVPlacement(0,                    // no rotation
                      G4ThreeVector(0,
                                    - GetHeight()/2
                                    + p0dule.GetHeight()
                                    + GetTopLadderOffset()
                                    + topLadder.GetLadderThickness()/2,
                                    0),
                      topLadder.GetPiece(),  // logical volume
                      topLadder.GetName(),   // name
                      logVolume,            // mother  volume
                      false,                // no boolean operations
                      0);                   // no specific field

    ND280P0DSideLadderConstructor &sideLadder 
        = Get<ND280P0DSideLadderConstructor>("SideLadder");
    new G4PVPlacement(0,                    // no rotation
                      G4ThreeVector(- GetWidth()/2
                                    + p0dule.GetWidth()
                                    + GetSideLadderOffset()
                                    + sideLadder.GetLadderThickness()/2,
                                    0,
                                    0),
                      sideLadder.GetPiece(),  // logical volume
                      sideLadder.GetName(),   // name
                      logVolume,            // mother  volume
                      false,                // no boolean operations
                      0);                   // no specific field

    // ND280Log("P0D USECal Length: " << GetLength()/mm << " mm");
    // ND280Log("   USECal P0Dule Thickness:   " << p0dule.GetLength()/mm <<" mm");
    // ND280Log("   USECal Radiator Thickness: " <<radiator.GetLength()/mm<<" mm");
    G4cout << "P0D USECal Length: " << GetLength()/CLHEP::mm << " mm" << G4endl;
    G4cout << "   USECal P0Dule Thickness:   " << p0dule.GetLength()/CLHEP::mm <<" mm" << G4endl;
    G4cout << "   USECal Radiator Thickness: " <<radiator.GetLength()/CLHEP::mm<<" mm" << G4endl;
    
    return logVolume;
}
