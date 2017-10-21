#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>

#include "MyND280BeamConstructor.hh"

//#include "G4SystemOfUnits.hh" // NEW GLOBAL
#include <CLHEP/Units/SystemOfUnits.h>

MyND280BeamConstructor::~MyND280BeamConstructor(){
  // delete
}

void MyND280BeamConstructor::Init(void) {
  SetVisible(true);
  SetWidth(10*CLHEP::cm);
  SetHeight(10*CLHEP::cm);
  SetLength(2.9*CLHEP::meter);
}

G4LogicalVolume *MyND280BeamConstructor::GetPiece(void) {

    G4LogicalVolume *logVolume
	= new G4LogicalVolume(new G4Box(GetName(),
                                        GetWidth()/2.0,
                                        GetHeight()/2.0,
                                        GetLength()/2.0),
                              //FindMaterial(fMaterialName),
                              fMaterial,
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
