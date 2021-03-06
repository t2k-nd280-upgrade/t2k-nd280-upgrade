////////////////////////////////////////////////////////////
// $Id: ND280P0DTargetRadiatorConstructor.cc,v 1.4 2011/09/15 14:21:27 mcgrew Exp $
//

#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>

#include "p0d/ND280P0DTargetRadiatorConstructor.hh"

/// Control the parameters of the P0D radiator layers.
class ND280P0DTargetRadiatorMessenger: public ND280ConstructorMessenger {
private:
    ND280P0DTargetRadiatorConstructor *fConstructor;
    G4UIcmdWithADoubleAndUnit* fThicknessCMD;

public:
    ND280P0DTargetRadiatorMessenger(ND280P0DTargetRadiatorConstructor* c) 
        : ND280ConstructorMessenger(c,
                                             "Control a gap layer."),
          fConstructor(c) {

        fThicknessCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("brassThickness"),this);
        fThicknessCMD->SetGuidance("Set thickness of brass radiator.");
        fThicknessCMD->SetParameterName("Thickness",false);
        fThicknessCMD->SetUnitCategory("Length");

    };

    ~ND280P0DTargetRadiatorMessenger() {
        delete fThicknessCMD;
    };

    void SetNewValue(G4UIcommand *cmd, G4String val) {
        if (fThicknessCMD == cmd) {
            fConstructor->
                SetThickness(fThicknessCMD->GetNewDoubleValue(val));
        }
        else {
            ND280ConstructorMessenger::SetNewValue(cmd,val);
        }
    };
};

ND280P0DTargetRadiatorConstructor::~ND280P0DTargetRadiatorConstructor() {;}

const G4VisAttributes* ND280P0DTargetRadiatorConstructor::GetVisual(void) const {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(0.8,0.8,0.0,1); 
    return visual;
}

void ND280P0DTargetRadiatorConstructor::Init() {
#ifdef OLD_MEASUREMENT
  SetThickness(1.56*CLHEP::mm); // Measured 3/30/09
#else
    // Measured by Gilje 6/11 and report in T2K-TN-73 (Alignment and Mass of
    // the P0D Detector)
  SetThickness(1.278*CLHEP::mm); 
#endif
    SetMessenger(new ND280P0DTargetRadiatorMessenger(this));
}

G4LogicalVolume *ND280P0DTargetRadiatorConstructor::GetPiece(void) {

    // Create a box to contain the radiator
    G4VSolid *solid = new G4Box(GetName(),
				GetWidth()/2, 
				GetHeight()/2,
                                GetLength()/2);

    // Create the volume around the radiator layer.
    G4LogicalVolume *logVolume
	= new G4LogicalVolume(solid,
                              FindMaterial("Brass"),
                              GetName());

    if (GetVisible()) {
        logVolume->SetVisAttributes(GetVisual());
    } 
    else {
        logVolume->SetVisAttributes(G4VisAttributes::Invisible);
    }

    return logVolume;
}
