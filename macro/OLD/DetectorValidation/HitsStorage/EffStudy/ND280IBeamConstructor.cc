#include <cmath>

#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include "G4SystemOfUnits.hh"

#include "ND280IBeamConstructor.hh"

class ND280IBeamMessenger : public ND280ConstructorMessenger {

private:

    ND280IBeamConstructor* fConstructor;
    G4UIcmdWithADoubleAndUnit* fStemThicknessCMD;
    G4UIcmdWithADoubleAndUnit* fBaseThicknessCMD;
    G4UIcmdWithADoubleAndUnit* fIBeamLengthCMD;
    
public:

    ND280IBeamMessenger(ND280IBeamConstructor* c) 
        : ND280ConstructorMessenger(c,"Control the I-Beam geometry."),
          fConstructor(c) {

        fStemThicknessCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("stemThickness"),this);
        fStemThicknessCMD->SetGuidance("Set thickness of the I-beam stem.");
        fStemThicknessCMD->SetParameterName("Length",false);
        fStemThicknessCMD->SetUnitCategory("Length");

        fBaseThicknessCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("baseThickness"),this);
        fBaseThicknessCMD->SetGuidance("Set thickness of the I-beam base.");
        fBaseThicknessCMD->SetParameterName("Length",false);
        fBaseThicknessCMD->SetUnitCategory("Length");

        fIBeamLengthCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("ibeamLength"),this);
        fIBeamLengthCMD->SetGuidance("Set the length of the I-beam.");
        fIBeamLengthCMD->SetParameterName("Length",false);
        fIBeamLengthCMD->SetUnitCategory("Length");

    };

    virtual ~ND280IBeamMessenger() {
        delete fStemThicknessCMD;
        delete fBaseThicknessCMD;
        delete fIBeamLengthCMD;
    };

    void SetNewValue(G4UIcommand *cmd, G4String val) {
        if (cmd==fStemThicknessCMD) {
            fConstructor->SetStemThickness(
                fStemThicknessCMD->GetNewDoubleValue(val));
        }
        else if (cmd==fBaseThicknessCMD) {
            fConstructor->SetBaseThickness(
                fBaseThicknessCMD->GetNewDoubleValue(val));
        }
        else if (cmd==fIBeamLengthCMD) {
            fConstructor->SetIBeamLength(
                fIBeamLengthCMD->GetNewDoubleValue(val));
        }
        else {
            ND280ConstructorMessenger::SetNewValue(cmd,val);
        }
    };
};

ND280IBeamConstructor::~ND280IBeamConstructor() {}

void ND280IBeamConstructor::Init(void) {

    SetWidth(   50*mm);
    SetHeight( 104*mm);
    SetLength(2300*mm);

    SetStemThickness(4.75*mm);
    SetBaseThickness(9.50*mm);

    SetIBeamLength(100*m);

    SetMessenger(new ND280IBeamMessenger(this));
}


double ND280IBeamConstructor::GetIBeamLength() {
    return std::min(fIBeamLength,GetLength());
}

G4LogicalVolume *ND280IBeamConstructor::GetPiece(void) {

    G4LogicalVolume *logVolume
	= new G4LogicalVolume(new G4Box(GetName(),
                                        GetWidth()/2.0,
                                        GetHeight()/2.0,
                                        GetLength()/2.0),
                              FindMaterial(GetMaterialName()),
                              GetName());

    if (GetVisible()) {
        G4VisAttributes *visual = new G4VisAttributes();
        visual->SetColor(0.6,0.4,0.4,1); 
        logVolume->SetVisAttributes(visual);
    } 
    else {
        logVolume->SetVisAttributes(G4VisAttributes::Invisible);
    }

    double width  = (GetWidth() - GetStemThickness())/2.;
    double height =  GetHeight()-2.*GetBaseThickness();

    if ( fNumber == 1 ) width *= 2.;

    G4LogicalVolume *logAir
	= new G4LogicalVolume(new G4Box(GetName()+"/Air",
                                        width/2.,
                                        height/2.,
                                        GetIBeamLength()/2.),
                              FindMaterial("Air"),
                              GetName()+"/Air");

    logAir->SetVisAttributes(G4VisAttributes::Invisible);

    new G4PVPlacement(0,
                      G4ThreeVector(GetWidth()/2.-width/2.,0,0),
                      logAir,
                      GetName()+"/Air",
                      logVolume,
                      false,
                      0);

    if ( fNumber!=1 ) {
       new G4PVPlacement(0,
                         G4ThreeVector(-GetWidth()/2.+width/2.,0,0),
                         logAir,
                         GetName()+"/Air",
                         logVolume,
                         false,
                         1);
    }

    return logVolume;
}
