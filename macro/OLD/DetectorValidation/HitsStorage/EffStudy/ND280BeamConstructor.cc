#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>

#include "ND280BeamConstructor.hh"

class ND280BeamMessenger : public ND280ConstructorMessenger {

private:

    ND280BeamConstructor* fConstructor;

    G4UIcmdWithADoubleAndUnit* fWidthCMD;
    G4UIcmdWithADoubleAndUnit* fHeightCMD;
    G4UIcmdWithADoubleAndUnit* fLengthCMD;
    G4UIcmdWithAString*        fMaterialCMD;
    
public:

    ND280BeamMessenger(ND280BeamConstructor* c) 
        : ND280ConstructorMessenger(c,"Control the (support) beam geometry."),
          fConstructor(c) {

        fWidthCMD = new G4UIcmdWithADoubleAndUnit(CommandName("width"),this);
        fWidthCMD->SetGuidance("Set the width of the support beam.");
        fWidthCMD->SetParameterName("Width",false);
        fWidthCMD->SetUnitCategory("Length");

        fHeightCMD = new G4UIcmdWithADoubleAndUnit(CommandName("height"),this);
        fHeightCMD->SetGuidance("Set the height of the support beam.");
        fHeightCMD->SetParameterName("Height",false);
        fHeightCMD->SetUnitCategory("Length");

        fLengthCMD = new G4UIcmdWithADoubleAndUnit(CommandName("length"),this);
        fLengthCMD->SetGuidance("Set length of the support beam.");
        fLengthCMD->SetParameterName("Length",false);
        fLengthCMD->SetUnitCategory("Length");

        fMaterialCMD = new G4UIcmdWithAString(CommandName("material"),this);
        fMaterialCMD->SetGuidance("Set the material name of the support beam.");
        fMaterialCMD->SetParameterName("material",false);

    };

    virtual ~ND280BeamMessenger() {
        delete fWidthCMD;
        delete fHeightCMD;
        delete fLengthCMD;
        delete fMaterialCMD;
    };

    void SetNewValue(G4UIcommand *cmd, G4String val) {
        if (cmd==fWidthCMD) {
            fConstructor->SetWidth(fWidthCMD->GetNewDoubleValue(val));
        }
        else if (cmd==fHeightCMD) {
            fConstructor->SetHeight(fHeightCMD->GetNewDoubleValue(val));
        }
        else if (cmd==fLengthCMD) {
            fConstructor->SetLength(
                fLengthCMD->GetNewDoubleValue(val));
        }
        else if (cmd==fMaterialCMD) {
            fConstructor->SetMaterialName(val);
        }
        else {
            ND280ConstructorMessenger::SetNewValue(cmd,val);
        }
    };
};

ND280BeamConstructor::~ND280BeamConstructor() {;}

void ND280BeamConstructor::Init(void) {

    SetWidth(10*cm);
    SetHeight(10*cm);
    SetLength(2.9*meter);

    SetMessenger(new ND280BeamMessenger(this));
}

G4LogicalVolume *ND280BeamConstructor::GetPiece(void) {

    G4LogicalVolume *logVolume
	= new G4LogicalVolume(new G4Box(GetName(),
                                        GetWidth()/2.0,
                                        GetHeight()/2.0,
                                        GetLength()/2.0),
                              FindMaterial(fMaterialName),
                              GetName());

    if (GetVisible()) {
        G4VisAttributes *visual = new G4VisAttributes();
        visual->SetColor(0.6,0.4,0.4,1); 
        logVolume->SetVisAttributes(visual);
    } 
    else {
        logVolume->SetVisAttributes(G4VisAttributes::Invisible);
    }
    
    return logVolume;
}
