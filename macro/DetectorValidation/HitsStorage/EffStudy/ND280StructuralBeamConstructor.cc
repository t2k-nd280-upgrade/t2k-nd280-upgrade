#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include "G4SystemOfUnits.hh"

#include "ND280StructuralBeamConstructor.hh"

class ND280StructuralBeamMessenger : public ND280ConstructorMessenger {

private:

    ND280StructuralBeamConstructor* fConstructor;
    G4UIcmdWithADoubleAndUnit* fWallThicknessCMD;
    
public:

    ND280StructuralBeamMessenger(ND280StructuralBeamConstructor* c) 
        : ND280ConstructorMessenger(c,"Control the structural beam geometry."),
          fConstructor(c) {

        fWallThicknessCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("wallThickness"),this);
        fWallThicknessCMD->SetGuidance("Set thickness of the beam wall.");
        fWallThicknessCMD->SetParameterName("Length",false);
        fWallThicknessCMD->SetUnitCategory("Length");

    };

    virtual ~ND280StructuralBeamMessenger() {
        delete fWallThicknessCMD;
    };

    void SetNewValue(G4UIcommand *cmd, G4String val) {
        if (cmd==fWallThicknessCMD) {
            fConstructor->SetWallThickness(
                fWallThicknessCMD->GetNewDoubleValue(val));
        }
        else {
            ND280ConstructorMessenger::SetNewValue(cmd,val);
        }
    };
};

ND280StructuralBeamConstructor::~ND280StructuralBeamConstructor() {}

void ND280StructuralBeamConstructor::Init(void) {
    SetWidth(100*mm);
    SetHeight(300*mm);
    SetLength(6650*mm);
    SetWallThickness(6*mm);

    SetMessenger(new ND280StructuralBeamMessenger(this));
}

G4LogicalVolume *ND280StructuralBeamConstructor::GetPiece(void) {

    G4LogicalVolume *logVolume
	= new G4LogicalVolume(new G4Box(GetName(),
                                        GetWidth()/2.0,
                                        GetHeight()/2.0,
                                        GetLength()/2.0),
                              FindMaterial("Steel"),
                              GetName());

    if (GetVisible()) {
        G4VisAttributes *visual = new G4VisAttributes();
        visual->SetColor(0.6,0.4,0.4,1); 
        logVolume->SetVisAttributes(visual);
    } 
    else {
        logVolume->SetVisAttributes(G4VisAttributes::Invisible);
    }
    
    G4LogicalVolume *logAir
	= new G4LogicalVolume(new G4Box(GetName()+"/Air",
                                        (GetWidth()-2*GetWallThickness())/2.0,
                                        (GetHeight()-2*GetWallThickness())/2.0,
                                        GetLength()/2.0),
                              FindMaterial("Air"),
                              GetName());

    logAir->SetVisAttributes(G4VisAttributes::Invisible);

    new G4PVPlacement(0,
                      G4ThreeVector(0,0,0),
                      logAir,
                      GetName()+"/Air",
                      logVolume,
                      false,
                      0);

    return logVolume;
}
