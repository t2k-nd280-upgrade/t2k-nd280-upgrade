// G4
#include <globals.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VSolid.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4PVPlacement.hh>

#include "ecal/ND280ECal.hh"
#include "ecal/ND280ECalComponents.hh"
#include "ecal/ND280DsCarbonPanelConstructor.hh"

ND280DsCarbonPanelConstructor::~ND280DsCarbonPanelConstructor() {}

void ND280DsCarbonPanelConstructor::Init() {
    SetWidth(2300*mm);
    SetHeight(2300*mm);
    SetLength(ND280ECal::CarbonSheetThickness);
}

G4LogicalVolume* ND280DsCarbonPanelConstructor::GetPiece() {
    ND280ECalComponents components(GetName());
    
    // Module volume
    G4LogicalVolume* logical 
        = new G4LogicalVolume(new G4Box(GetName(), 
                                        (GetWidth()/2.0), 
                                        (GetHeight()/2.0), 
                                        (GetLength()/2.0)), 
                              FindMaterial("Air"), 
                              GetName());
    
    const double al_thickness = ND280ECal::DS::CarbonFrameWidth;
    
    // Carbon Fibre Sheets
    double cf_x = GetWidth() - ( 2 * al_thickness );
    double cf_y = GetHeight() - ( 2 * al_thickness );
    double cf_z = ND280ECal::CarbonSheetThickness;
    G4LogicalVolume* cf_logical = components.CarbonFibre(cf_x, cf_y, cf_z);
    
    // Top: +z
    new G4PVPlacement(0,                        // rotation
                      G4ThreeVector(0.0, 0.0,
                                    ((GetLength()-cf_z)/2.0)), // position
                      cf_logical,               // logical volume
                      GetName()+"/CarbonFibre", // name
                      logical,                  // mother volume
                      false,                    // no boolean operations
                      0);                       // copy number
    
    // Bottom: -z
    new G4PVPlacement(0,                        // rotation
                      G4ThreeVector(0.0, 0.0, 
                                    ((-GetLength() + cf_z)/2.0)), // position
                      cf_logical,               // logical volume
                      GetName()+"/CarbonFibre", // name
                      logical,                  // mother volume
                      false,                    // no boolean operations
                      1);                       // copy number
    
    // Y Aluminium
    // Span full width of the panel in x.
    G4LogicalVolume* aly_logical 
        = components.Bulkhead(GetWidth(), al_thickness, GetLength());
    
    // +y
    new G4PVPlacement(0,                       // rotation
                      G4ThreeVector( 0.0, 
                                     ((GetHeight() - al_thickness)/2.0),
                                     0.0),     // position
                      aly_logical,             // logical volume
                      GetName()+"/AluminiumY", // name
                      logical,                 // mother volume
                      false,                   // no boolean operations
                      0                        // copy number
        );
    
    // -y		
    new G4PVPlacement(0,                       // rotation
                      G4ThreeVector( 0.0,
                                     ((-GetHeight() + al_thickness)/2.0), 
                                     0.0),     // position
                      aly_logical,             // logical volume
                      GetName()+"/AluminiumY", // name
                      logical,                 // mother volume
                      false,                   // no boolean operations
                      1);                      // copy number
    
    // X Aluminium
    // Spans between the two AluminiumY
    G4LogicalVolume* alx_logical 
        = components.Bulkhead(al_thickness, 
                              (GetHeight() - (2*al_thickness)),
                              GetLength());
    
    // +x
    new G4PVPlacement(0,                       // rotation
                      G4ThreeVector( ((GetWidth() - al_thickness)/2.0),
                                     0.0, 
                                     0.0),     // position
                      alx_logical,             // logical volume
                      GetName()+"/AluminiumX", // name
                      logical,                 // mother volume
                      false,                   // no boolean operations
                      0);                      // copy number
    
    // -x
    new G4PVPlacement(0,                       // rotation
                      G4ThreeVector( ((-GetWidth() + al_thickness)/2.0),
                                     0.0,
                                     0.0),     // position
                      alx_logical,             // logical volume
                      GetName()+"/AluminiumX", // name
                      logical,                 // mother volume
                      false,                   // no boolean operations
                      1);                      // copy number
    
    return logical;
}

