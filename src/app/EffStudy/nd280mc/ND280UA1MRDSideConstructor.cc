#include <string>
#include <sstream>

#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>

#include "ND280UA1MRDSideConstructor.hh"

class ND280UA1MRDSideMessenger: public ND280ConstructorMessenger {
private:
    ND280UA1MRDSideConstructor* fConstructor;
    G4UIcmdWithADoubleAndUnit* fWidthCMD;
    G4UIcmdWithADoubleAndUnit* fHeightCMD;
    G4UIcmdWithADoubleAndUnit* fLengthCMD;
    G4UIcommand*               fSensitiveCMD;
    G4UIcmdWithADoubleAndUnit* fMaximumHitSagittaCMD;
    G4UIcmdWithADoubleAndUnit* fMaximumHitLengthCMD;
    
public:
    ND280UA1MRDSideMessenger(ND280UA1MRDSideConstructor* c) 
        : ND280ConstructorMessenger(c,"Control the MRD side module geometry."),
          fConstructor(c) {
        fWidthCMD = new G4UIcmdWithADoubleAndUnit(CommandName("width"),this);
        fWidthCMD->SetGuidance("Set the width of the MRD side modules.");
        fWidthCMD->SetParameterName("Width",false);
        fWidthCMD->SetUnitCategory("Length");

        fHeightCMD = new G4UIcmdWithADoubleAndUnit(CommandName("height"),this);
        fHeightCMD->SetGuidance("Set the height of the MRD side modules.");
        fHeightCMD->SetParameterName("Height",false);
        fHeightCMD->SetUnitCategory("Length");

        fLengthCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("length"),this);
        fLengthCMD->SetGuidance("Set length of the MRD side modules.");
        fLengthCMD->SetParameterName("Length",false);
        fLengthCMD->SetUnitCategory("Length");

        // fSensitiveCMD = new G4UIcommand(CommandName("sensitive"),this);
        // fSensitiveCMD->SetGuidance("Set the name of the sensitive detector");
        // G4UIparameter* nameParam = new G4UIparameter('s');
        // nameParam->SetParameterName("Name");
        // fSensitiveCMD->SetParameter(nameParam);
        // G4UIparameter* typeParam = new G4UIparameter('s');
        // typeParam->SetParameterName("Type");
        // fSensitiveCMD->SetParameter(typeParam);

        // fMaximumHitSagittaCMD 
        //     = new G4UIcmdWithADoubleAndUnit(CommandName("maxHitSagitta"),this);
        // fMaximumHitSagittaCMD->SetGuidance(
        //     "Set the maximum sagitta for a hit.");
        // fMaximumHitSagittaCMD->SetParameterName("Sagitta",false);
        // fMaximumHitSagittaCMD->SetUnitCategory("Length");

        // fMaximumHitLengthCMD 
        //     = new G4UIcmdWithADoubleAndUnit(CommandName("maxHitLength"),this);
        // fMaximumHitLengthCMD->SetGuidance("Set the maximum length for a hit.");
        // fMaximumHitLengthCMD->SetParameterName("HitLength",false);
        // fMaximumHitLengthCMD->SetUnitCategory("Length");

    };

    virtual ~ND280UA1MRDSideMessenger() {
        delete fWidthCMD;
        delete fHeightCMD;
        delete fLengthCMD;
        delete fSensitiveCMD;
        delete fMaximumHitSagittaCMD;
        delete fMaximumHitLengthCMD;
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
        // else if (cmd==fSensitiveCMD) {
        //     std::istringstream buf(val.c_str());
        //     std::string name, type;
        //     buf >> name;
        //     buf >> type;
        //     fConstructor->SetSensitiveDetector(name,type);
        // }
        // else if (cmd==fMaximumHitSagittaCMD) {
        //     fConstructor->
        //         SetMaximumHitSagitta(
        //             fMaximumHitSagittaCMD->GetNewDoubleValue(val));
        // }
        // else if (cmd==fMaximumHitLengthCMD) {
        //     fConstructor->
        //         SetMaximumHitLength(
        //             fMaximumHitLengthCMD->GetNewDoubleValue(val));
        // }
        else {
            ND280ConstructorMessenger::SetNewValue(cmd,val);
        }
    };
};

ND280UA1MRDSideConstructor::~ND280UA1MRDSideConstructor() {;}

void ND280UA1MRDSideConstructor::Init(void) {
  
  ////////
  //
  // NEW ND280UPGRADE
  //
  
  // Take XML inputs
  ND280RootPersistencyManager* InputPersistencyManager
    = ND280RootPersistencyManager::GetInstance();
  fND280XMLInput = InputPersistencyManager->GetXMLInput();  
  
  // Set visible / invisible Magnet
  if( GetND280XML()->GetXMLInvisSMRD() ) SetVisible(false);
  else SetVisible(true);
  
  ////////

  fWidth = (5.0 * (177.5 + 0.5))*CLHEP::mm; // plateCount * plateOffset
    fHeight = 7*CLHEP::mm;
    fLength = 875*CLHEP::mm;
    //SetSensitiveDetector("mrd","segment");
    //SetMaximumHitLength(5*CLHEP::cm);
    //SetMaximumHitSagitta(1*CLHEP::cm);
    fYokeIndex = 0;
    fSlotLayer = 0;
    fSlotIndex = 0;
    SetMessenger(new ND280UA1MRDSideMessenger(this));
}

G4LogicalVolume *ND280UA1MRDSideConstructor::GetPiece(void) {
    std::ostringstream mrdString;
    mrdString << GetName() << ":" << fYokeIndex << fSlotLayer << fSlotIndex;
    std::string mrdName = mrdString.str();
    G4LogicalVolume *logVolume
	= new G4LogicalVolume(new G4Box(mrdName,
                                        GetWidth()/2.0,
                                        GetHeight()/2.0,
                                        GetLength()/2.0),
                              FindMaterial("Air"),
                              mrdName);
    logVolume->SetVisAttributes(G4VisAttributes::Invisible);

    int plateCount = 5;
    double plateWidth = 175.0*CLHEP::mm;
    double plateOffset = GetWidth() / plateCount;

    for (int i = 0; i<plateCount; ++i) {
        G4LogicalVolume* plateVolume 
            = new G4LogicalVolume(new G4Box(mrdName+"/Bar",
                                            plateWidth/2.0,
                                            GetHeight()/2.0,
                                            GetLength()/2.0),
                                  FindMaterial("Scintillator"),
                                  mrdName+"/Bar");

        double currentOffset = -0.5 * GetWidth() + (0.5 + i) * plateOffset;
        new G4PVPlacement(0,                  // no rotation
                          G4ThreeVector(currentOffset,0,0), // position 
                          plateVolume,        // logical volume
                          mrdName+"/Bar",     // name
                          logVolume,          // mother  volume
                          false,              // no boolean operations
                          0);                 // no specific field

        // if (GetSensitiveDetector()){
        //     plateVolume->SetSensitiveDetector(GetSensitiveDetector());
        // }
        
        if (GetVisible()) {
            G4VisAttributes *visual = new G4VisAttributes();
            visual->SetColor(0.2,0.5,0.2,1); // green
            plateVolume->SetVisAttributes(visual);
        }
        else {
            plateVolume->SetVisAttributes(G4VisAttributes::Invisible);
        }
    }

    return logVolume;
}
