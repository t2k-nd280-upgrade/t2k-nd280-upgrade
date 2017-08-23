#include <globals.hh>
#include <G4Tubs.hh>
#include <G4Box.hh>
#include <G4UnionSolid.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4AssemblyVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4UnitsTable.hh>

//#include <TND280Log.hxx>

#include "ND280UA1CoilConstructor.hh"

class ND280UA1CoilMessenger: public ND280ConstructorMessenger {
private:
    ND280UA1CoilConstructor* fConstructor;

public:
    ND280UA1CoilMessenger(ND280UA1CoilConstructor* c) 
        : ND280ConstructorMessenger(c,"Control the magnet geometry."),
          fConstructor(c) { };

    virtual ~ND280UA1CoilMessenger() {  };

    void SetNewValue(G4UIcommand *cmd, G4String val) {
        ND280ConstructorMessenger::SetNewValue(cmd,val);
    };
};

ND280UA1CoilConstructor::~ND280UA1CoilConstructor() {;}

void ND280UA1CoilConstructor::Init(void) {
    
  ////////
  //
  // NEW ND280UPGRADE
  //
  
  // Take XML inputs
  ND280RootPersistencyManager* InputPersistencyManager
    = ND280RootPersistencyManager::GetInstance();
  fND280XMLInput = InputPersistencyManager->GetXMLInput();  
  
  // Set visible / invisible Magnet
  if( GetND280XML()->GetXMLInvisMagnet() ) SetVisible(false);
  else SetVisible(true);
  
  ////////
 

  fCoilInnerLength = 7009*CLHEP::mm;
    fCoilInnerHeight = 3540*CLHEP::mm;
    fCoilThickness = 4*5.5*CLHEP::cm;
    fCoilWidth = 785*CLHEP::mm;
    fCaseThickness = 1*CLHEP::cm;
    fBendInnerRadius = 1*CLHEP::cm;

    SetMessenger(new ND280UA1CoilMessenger(this));
}

G4LogicalVolume *ND280UA1CoilConstructor::GetPiece(void) {return NULL;}

const G4VisAttributes* ND280UA1CoilConstructor::GetVisual(void) const {
    if (GetVisible()) {
        G4VisAttributes* visual = new G4VisAttributes();
        visual->SetColor(0.1,0.6,0.1,1); // red
        return visual;
    }
    else {
        return &G4VisAttributes::Invisible;
    }
}

G4AssemblyVolume *ND280UA1CoilConstructor::GetAssembly(void) {
    G4LogicalVolume *logVolume = NULL;
    G4AssemblyVolume* assembly = new G4AssemblyVolume();

    G4ThreeVector volCenter(+ GetTopBarLength()/2,
                            + (GetCoilInnerHeight()/2
                               - GetBendInnerRadius()
                               - GetCaseThickness()),
                            0);
    logVolume 
        = new G4LogicalVolume(new G4Tubs(GetName()+"TopFrontBend",
                                         GetBendInnerRadius(),
                                         GetBendOuterRadius(),
                                         GetCoilWidth()/2,
                                         0.0*CLHEP::deg, 90.0*CLHEP::deg),
                              FindMaterial("Aluminum"),
                              GetName()+"TopFrontBend");
    assembly->AddPlacedVolume(logVolume,volCenter,NULL);
    
    logVolume->SetVisAttributes(GetVisual());
    
    volCenter.set(- GetTopBarLength()/2,
                  + (GetCoilInnerHeight()/2
                     - GetBendInnerRadius()
                     - GetCaseThickness()),
                  0);
    logVolume =
        new G4LogicalVolume(new G4Tubs(GetName()+"TopBackBend",
                                       GetBendInnerRadius(),
                                       GetBendOuterRadius(), 
                                       GetCoilWidth()/2,
                                       90.0*CLHEP::deg, 90.0*CLHEP::deg),
                            FindMaterial("Aluminum"),
                            GetName()+"TopFrontBend");
    assembly->AddPlacedVolume(logVolume,volCenter,NULL);
    logVolume->SetVisAttributes(GetVisual());

    volCenter.set(- GetTopBarLength()/2,
                  - (GetCoilInnerHeight()/2
                     - GetBendInnerRadius()
                     - GetCaseThickness()),
                  0);
    logVolume =
        new G4LogicalVolume(new G4Tubs(GetName()+"BotBackBend",
                                       GetBendInnerRadius(), 
                                       GetBendOuterRadius(),
                                       GetCoilWidth()/2,
                                       180.0*CLHEP::deg, 90.0*CLHEP::deg),
                            FindMaterial("Aluminum"),
                            GetName()+"TopFrontBend");
    assembly->AddPlacedVolume(logVolume,volCenter,NULL);
    logVolume->SetVisAttributes(GetVisual());

    volCenter.set(+ GetTopBarLength()/2,
                  - (GetCoilInnerHeight()/2
                     - GetBendInnerRadius()
                     - GetCaseThickness()),
                  0);
    logVolume =
        new G4LogicalVolume(new G4Tubs(GetName()+"BotFrontBend",
                                       GetBendInnerRadius(),
                                       GetBendOuterRadius(),
                                       GetCoilWidth()/2,
                                       270.0*CLHEP::deg, 90.0*CLHEP::deg),
                            FindMaterial("Aluminum"),
                            GetName()+"TopFrontBend");
    assembly->AddPlacedVolume(logVolume,volCenter,NULL);
    logVolume->SetVisAttributes(GetVisual());

    volCenter.set(0,(GetCoilInnerHeight()/2+GetCoilThickness()/2),0);
    logVolume =
        new G4LogicalVolume(new G4Box(GetName()+"TopBar",
                                      GetTopBarLength()/2,
                                      GetCoilThickness()/2,
                                      GetCoilWidth()/2),
                            FindMaterial("Aluminum"),
                            GetName()+"TopBar");
    assembly->AddPlacedVolume(logVolume,volCenter,NULL);
    logVolume->SetVisAttributes(GetVisual());

    volCenter.set(0,-(GetCoilInnerHeight()/2+GetCoilThickness()/2),0);
    logVolume =
        new G4LogicalVolume(new G4Box(GetName()+"BotBar",
                                      GetBottomBarLength()/2,
                                      GetCoilThickness()/2,
                                      GetCoilWidth()/2),
                            FindMaterial("Aluminum"),
                            GetName()+"BotBar");
    assembly->AddPlacedVolume(logVolume,volCenter,NULL);
    logVolume->SetVisAttributes(GetVisual());

    volCenter.set(+ (GetCoilInnerLength()/2
                     +GetCoilThickness()/2
                     +GetCaseThickness()),0,0);
    logVolume =
        new G4LogicalVolume(new G4Box(GetName()+"FrontBar",
                                      GetCoilThickness()/2,
                                      GetFrontBarLength()/2,
                                      GetCoilWidth()/2),
                            FindMaterial("Aluminum"),
                            GetName()+"FrontBar");
    assembly->AddPlacedVolume(logVolume,volCenter,NULL);
    logVolume->SetVisAttributes(GetVisual());

    volCenter.set(- (GetCoilInnerLength()/2
                     +GetCoilThickness()/2
                     +GetCaseThickness()),0,0);
    logVolume =
        new G4LogicalVolume(new G4Box(GetName()+"BackBar",
                                      GetCoilThickness()/2,
                                      GetBackBarLength()/2,
                                      GetCoilWidth()/2),
                            FindMaterial("Aluminum"),
                            GetName()+"BackBar");
    assembly->AddPlacedVolume(logVolume,volCenter,NULL);
    logVolume->SetVisAttributes(GetVisual());

    // ND280Log("%%%% Total Coil Length: " << GetCoilLength());
    // ND280Log("%%%% Total Coil Height: " <<  GetCoilHeight());
    // ND280Log("%%%% Total Coil Width:  " << GetCoilWidth());
    // ND280Log("%%   Coil Inner Length:   " 
    //          << GetCoilInnerLength());
    // ND280Log("%%   Coil Inner Height:   " 
    //          << GetCoilInnerHeight());
    // ND280Log("%%   Coil Thickness:      "
    //          << GetCoilThickness());
    // ND280Log("%%   Coil Case Thickness: "
    //          << GetCaseThickness());

    G4cout << "%%%% Total Coil Length: " << GetCoilLength() << G4endl;
    G4cout << "%%%% Total Coil Height: " <<  GetCoilHeight() << G4endl;
    G4cout << "%%%% Total Coil Width:  " << GetCoilWidth() << G4endl;
    G4cout << "%%   Coil Inner Length:   " 
	   << GetCoilInnerLength() << G4endl;
    G4cout << "%%   Coil Inner Height:   " 
	   << GetCoilInnerHeight() << G4endl;
    G4cout << "%%   Coil Thickness:      "
	   << GetCoilThickness() << G4endl;
    G4cout << "%%   Coil Case Thickness: "
	   << GetCaseThickness() << G4endl;

    return assembly;
}
