#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>

#include "ND280OffAxisConstructor.hh"
#include "ND280UA1MagnetConstructor.hh"

class ND280OffAxisMessenger: public ND280ConstructorMessenger {
private:
    ND280OffAxisConstructor* fConstructor;
    G4UIcmdWithADoubleAndUnit* fWidthCMD;
    G4UIcmdWithADoubleAndUnit* fHeightCMD;
    G4UIcmdWithADoubleAndUnit* fLengthCMD;
    
public:
    ND280OffAxisMessenger(ND280OffAxisConstructor* c) 
        : ND280ConstructorMessenger(c,"Control the off axis geometry."),
          fConstructor(c) {
        fWidthCMD = new G4UIcmdWithADoubleAndUnit(CommandName("width"),this);
        fWidthCMD->SetGuidance("Set the width of the off axis detector.");
        fWidthCMD->SetParameterName("Width",false);
        fWidthCMD->SetUnitCategory("Length");

        fHeightCMD = new G4UIcmdWithADoubleAndUnit(CommandName("height"),this);
        fHeightCMD->SetGuidance("Set the height of the off axis detector.");
        fHeightCMD->SetParameterName("Height",false);
        fHeightCMD->SetUnitCategory("Length");

        fLengthCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("length"),this);
        fLengthCMD->SetGuidance("Set the length of the off axis detector.");
        fLengthCMD->SetParameterName("Length",false);
        fLengthCMD->SetUnitCategory("Length");
    };

    virtual ~ND280OffAxisMessenger() {
        delete fWidthCMD;
        delete fHeightCMD;
        delete fLengthCMD;
    };

    void SetNewValue(G4UIcommand *cmd, G4String val) {
        if (cmd==fWidthCMD) {
            fConstructor->SetWidth(fWidthCMD->GetNewDoubleValue(val));
        }
        else if (cmd==fHeightCMD) {
            fConstructor->SetHeight(fHeightCMD->GetNewDoubleValue(val));
        }
        else if (cmd==fLengthCMD) {
            fConstructor->SetLength(fLengthCMD->GetNewDoubleValue(val));
        }
        else {
            ND280ConstructorMessenger::SetNewValue(cmd,val);
        }
    };
};

ND280OffAxisConstructor::~ND280OffAxisConstructor() {;}

void ND280OffAxisConstructor::Init(void) {
  fWidth = fHeight = fLength = 8.0*CLHEP::meter;

    AddConstructor(new ND280UA1MagnetConstructor("Magnet",this));
    
    SetMessenger(new ND280OffAxisMessenger(this));
}

// Get the width of the off-axis bounding box.
double ND280OffAxisConstructor::GetWidth(void) {
    ND280UA1MagnetConstructor& magnet
        = Get<ND280UA1MagnetConstructor>("Magnet");
    return std::max(fWidth,magnet.GetWidth());
}

G4LogicalVolume *ND280OffAxisConstructor::GetPiece(void) {
    G4LogicalVolume *logVolume
	= new G4LogicalVolume(new G4Box(GetName(),
                                        GetWidth()/2.0,
                                        GetHeight()/2.0,
                                        GetLength()/2.0),
                              FindMaterial("Air"),
                              GetName());

    if (GetVisible()) {
        G4VisAttributes *visual = new G4VisAttributes();
        visual->SetColor(0.5,0.5,0.5,1); // white
        visual->SetForceWireframe(true);
        logVolume->SetVisAttributes(visual);
    } 
    else {
        logVolume->SetVisAttributes(G4VisAttributes::Invisible);
    }
    
    ND280UA1MagnetConstructor& magnet
        = Get<ND280UA1MagnetConstructor>("Magnet");

    //------------------------------ 
    // OFF-AXIS FINE-GRAINED WITH THE MAGNET
    //------------------------------ 
    new G4PVPlacement(0,                   // rotation
                      G4ThreeVector(0,0,0), // position
                      magnet.GetPiece(), // logical volume
                      magnet.GetName(),  // name
                      logVolume,         // mother  volume
                      false,             // no boolean operations
                      0);                // not a copy.

    return logVolume;
}
