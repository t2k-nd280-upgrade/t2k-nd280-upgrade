////////////////////////////////////////////////////////////
// $Id: ND280P0DTopLadderConstructor.cc,v 1.3 2009/11/30 02:49:41 mcgrew Exp $
//

#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>

#include "p0d/ND280P0DTopLadderConstructor.hh"

/// Control the parameters of the P0D electronics ladders.
class ND280P0DTopLadderMessenger: public ND280ConstructorMessenger {
private:
    ND280P0DTopLadderConstructor *fConstructor;

public:
    ND280P0DTopLadderMessenger(ND280P0DTopLadderConstructor* c) 
        : ND280ConstructorMessenger(c,
                                    "Control construction of the electronics."),
          fConstructor(c) {};

    ~ND280P0DTopLadderMessenger() {};

    void SetNewValue(G4UIcommand *cmd, G4String val) {
        ND280ConstructorMessenger::SetNewValue(cmd,val);
    };
};

ND280P0DTopLadderConstructor::~ND280P0DTopLadderConstructor() {}

const G4VisAttributes* ND280P0DTopLadderConstructor::GetVisual(void) const {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(0.8,0.8,0.8,1); 
    return visual;
}

void ND280P0DTopLadderConstructor::Init() {
    SetMessenger(new ND280P0DTopLadderMessenger(this));
}

G4LogicalVolume *ND280P0DTopLadderConstructor::GetPiece(void) {

    // Create a box to contain the radiator
    G4VSolid *solid = new G4Box(GetName(),
				GetLadderLength()/2,
				GetLadderThickness()/2, 
                                GetLadderWidth()/2);

    // Create the volume around the radiator layer.
    G4LogicalVolume *logVolume
	= new G4LogicalVolume(solid,
                              FindMaterial("Air"),
                              GetName());
    logVolume->SetVisAttributes(G4VisAttributes::Invisible);

    /////////////////////////////////////////////
    // Totally bogus implementation.  This is NOT an accurate representation
    // of the actual electronics ladders.  I MADE UP the dimensions.  I
    // FABRICATED the shapes.  DO NOT use this in a physics analysis to
    // estimate any efficiencies or backgrounds.  This only exists as a place
    // holder.  Enough said...
    /////////////////////////////////////////////
    
    double diameter = 20*mm;
    G4LogicalVolume *logCooling
	= new G4LogicalVolume(new G4Box(GetName()+"/Cooling",
                                        GetLadderLength()/2, 
                                        diameter/2, diameter/2),
                              FindMaterial("Aluminum"),
                              GetName()+"/Cooling");

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(0,
                                    - GetLadderThickness()/2+diameter/2,
                                    -75*mm),// position 
                      logCooling, // logical volume
                      GetName()+"/Cooling", // name
                      logVolume, // mother  volume
                      false, // no boolean operations
                      0); // no specific field

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(0,
                                    - GetLadderThickness()/2+diameter/2 + 2*mm,
                                    +75*mm),// position 
                      logCooling, // logical volume
                      GetName()+"/Cooling", // name
                      logVolume, // mother  volume
                      false, // no boolean operations
                      0); // no specific field

    double thickness = 6.35*mm;
    G4LogicalVolume *logPlate
	= new G4LogicalVolume(new G4Box(GetName()+"/Plate",
                                        20*cm/2, 
                                        thickness/2,
                                        25*cm/2),
                              FindMaterial("Aluminum"),
                              GetName()+"/Plate");

    double step = (GetLadderLength() - 2*10*cm  - 20*cm)/6;
    for (int i=0; i<7; ++i) {
        double offset = - GetLadderLength()/2 + 10*cm + 20*cm/2 + i*step;
        new G4PVPlacement(0, // no rotation
                          G4ThreeVector(offset,
                                        - GetLadderThickness()/2
                                        +diameter
                                        +thickness/2 + 2*mm,
                                        0),// position 
                          logPlate, // logical volume
                          GetName()+"/Plate", // name
                          logVolume, // mother  volume
                          false, // no boolean operations
                          0); // no specific field
    }

    return logVolume;
}
