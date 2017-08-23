////////////////////////////////////////////////////////////
// $Id: ND280GapConstructor.cc,v 1.16 2011/11/03 23:44:38 lindner Exp $
//

#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>


#include "ND280GapConstructor.hh"

class ND280GapMessenger: public ND280ComponentConstructorMessenger {
private:
    ND280GapConstructor *fConstructor;
    G4UIcmdWithAString* fMaterialCMD;
    G4UIcmdWithADoubleAndUnit* fLengthCMD;

public:
    ND280GapMessenger(ND280GapConstructor* c) 
        : ND280ComponentConstructorMessenger(c,
                                             "Control a gap layer."),
          fConstructor(c) {
        fMaterialCMD=new G4UIcmdWithAString(CommandName("material"),this);
        fMaterialCMD->SetGuidance("Set the material in the gap.");
        fMaterialCMD->SetParameterName("material",false);

        fLengthCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("length"),this);
        fLengthCMD->SetGuidance("Set length of the inactive gap.");
        fLengthCMD->SetParameterName("Length",false);
        fLengthCMD->SetUnitCategory("Length");
    };

    ~ND280GapMessenger() {
        delete fMaterialCMD;
        delete fLengthCMD;
    };

    void SetNewValue(G4UIcommand *cmd, G4String val) {
        if (fMaterialCMD == cmd) {
            fConstructor->SetMaterialName(val);
        }
        else if (fLengthCMD == cmd) {
            fConstructor->SetLength(fLengthCMD->GetNewDoubleValue(val));
        }
        else {
            ND280ComponentConstructorMessenger::SetNewValue(cmd,val);
        }
    };
};

ND280GapConstructor::~ND280GapConstructor() {;}
ND280AirGapConstructor::~ND280AirGapConstructor() {;}
ND280G10GapConstructor::~ND280G10GapConstructor() {;}
ND280GlueGapConstructor::~ND280GlueGapConstructor() {;}
ND280WaterModuleEpoxyGapConstructor::~ND280WaterModuleEpoxyGapConstructor() {;}
ND280LeadGapConstructor::~ND280LeadGapConstructor() {;}
ND280WaterGapConstructor::~ND280WaterGapConstructor() {;}
ND280ActiveWaterGapConstructor::~ND280ActiveWaterGapConstructor() {;}
ND280PropyleneGapConstructor::~ND280PropyleneGapConstructor() {;}

//the following section is added specifically for DUET and FGDmini
ND280AluminumGapConstructor::~ND280AluminumGapConstructor() {;}
//Polyvinyltolulene Gap Constructor for the S0, S1, and other scint things
ND280PVTGapConstructor::~ND280PVTGapConstructor(){;}

const G4VisAttributes* ND280AluminumGapConstructor::GetVisual(void) const {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(0.5,0.5,0.5,1); //grey 
    return visual;
}

const G4VisAttributes* ND280PVTGapConstructor::GetVisual(void) const {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(1.0,0.8,0.2,1); //orange? 
    return visual;
}
////end////

const G4VisAttributes* ND280LeadGapConstructor::GetVisual(void) const {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(0.1,0.1,0.8,1); 
    return visual;
}

const G4VisAttributes* ND280WaterGapConstructor::GetVisual(void) const {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(0.0,0.0,1.0,1); 
    return visual;
}

const G4VisAttributes* ND280ActiveWaterGapConstructor::GetVisual(void) const {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(0.0,0.0,1.0,1);
    return visual;
}

const G4VisAttributes* ND280G10GapConstructor::GetVisual(void) const {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(0.1,0.5,0.5,1);
    return visual;
}

const G4VisAttributes* ND280GlueGapConstructor::GetVisual(void) const {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(0.1,0.5,0.5,1);
    return visual;
}

const G4VisAttributes* ND280WaterModuleEpoxyGapConstructor::GetVisual(void) const {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(0.1,0.5,0.5,1);
    return visual;
}

const G4VisAttributes* ND280PropyleneGapConstructor::GetVisual(void) const {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(0.1,0.2,0.7,1);
    return visual;
}

void ND280GapConstructor::Init() {
    SetMaterialName("Air");
    SetLength(3*CLHEP::cm);
    SetEdgeSpace(0*CLHEP::cm);
    SetMessenger(new ND280GapMessenger(this));
}

G4LogicalVolume *ND280GapConstructor::GetPiece(void) {
    if (GetLength() < 0.01*CLHEP::mm) return NULL;

    G4VSolid *solid = new G4Box(GetName(),
				GetPhysicalWidth()/2 - GetEdgeSpace(), 
				GetPhysicalHeight()/2 - GetEdgeSpace(),
                                GetLength()/2);

    /// Create the volume around the tracking layer.
    G4LogicalVolume *logVolume
	= new G4LogicalVolume(solid,
                              FindMaterial(GetMaterialName()),
                              GetName());
    if (GetSensitiveDetector()){
        logVolume->SetSensitiveDetector(GetSensitiveDetector());
    }
    if (GetVisible()) {
        logVolume->SetVisAttributes(GetVisual());
    } 
    else {
        logVolume->SetVisAttributes(G4VisAttributes::Invisible);
    }

    return logVolume;
}
