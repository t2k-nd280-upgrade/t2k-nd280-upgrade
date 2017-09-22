#include <globals.hh>
#include <G4VSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4Box.hh>

#include "ecal/ND280ECal.hh"
#include "ecal/ND280ECalComponents.hh"
#include "ecal/ND280P0DECalActiveConstructor.hh"
#include "ecal/ND280P0DECalRightConstructor.hh"

void ND280P0DECalRightConstructor::Init() {
  SetLengthX(2454*CLHEP::mm);
  SetWidthY(2894*CLHEP::mm);
  fModuleHeightZ = 147.0*CLHEP::mm;
    SetHeightZ(ND280ECal::P0D::BulkheadReadoutHeight 
               + ND280ECal::P0D::BaseThickness);
    AddConstructor(new ND280P0DECalActiveConstructor("Active",this));
    Get<ND280P0DECalActiveConstructor>("Active").SetIsSide(true);
}

G4LogicalVolume* ND280P0DECalRightConstructor::GetPiece() {
    ND280ECalComponents components(GetName());
 
    // Module volume
    G4VSolid *solid = new G4Box(GetName(),
                                (GetLengthX()/2.0), 
                                (GetWidthY()/2.0), 
                                (GetHeightZ()/2.0));
    G4LogicalVolume* logical = new G4LogicalVolume(solid,
                                                   FindMaterial("Air"), 
                                                   GetName());
 
    // Thin Aluminium Bulkhead Plate
 
    // Spans entire detector in x-z.
    const double plate_y = ND280ECal::BulkheadPlateThickness;
    const double plate_z = fModuleHeightZ;
    const double plate_pos_y = (-GetWidthY() + plate_y) / 2.0;
    const double plate_pos_z = (-GetHeightZ() + plate_z) / 2.0;
 
    G4LogicalVolume* plate_logical 
        = components.Bulkhead(GetLengthX(), plate_y, plate_z);
 
    new G4PVPlacement(0,    // rotation
                      G4ThreeVector(0.0, plate_pos_y, plate_pos_z), // position
                      plate_logical, // logical volume
                      GetName()+"/BulkheadPlate",// name
                      logical,  // mother volume
                      false,   // no boolean operations
                      0);    // copy number

    // Base - Carbon Panel
    const double base_y = GetWidthY() - plate_y;
    const double base_z = ND280ECal::P0D::BaseThickness;
    const double base_pos_y = plate_y / 2.0;
    const double base_pos_z = (-GetHeightZ() + base_z) / 2.0;
 
    G4LogicalVolume* base_logical
        = components.CarbonPanel(GetLengthX(), base_y, base_z,
                                 ND280ECal::P0D::CarbonFrameX,
                                 ND280ECal::P0D::CarbonFrameYSide);
 
    new G4PVPlacement(0,    // rotation
                      G4ThreeVector(0.0, base_pos_y, base_pos_z), // position
                      base_logical, // logical volume
                      GetName()+"/Base",   // name
                      logical,  // mother volume
                      false,   // no boolean operations
                      0);    // copy number

    // Active Volume
    ND280P0DECalActiveConstructor& active
        = Get<ND280P0DECalActiveConstructor>("Active");
    G4LogicalVolume* active_logical = active.GetPiece();
 
    // Centred in x
    // Offset 4mm from -ve y face
    // Offset BaseHeight from -ve z face
    const double active_pos_y = -(GetWidthY()/2.0) + (active.GetWidthY()/2.0)
        + plate_y;
    const double active_pos_z = base_pos_z + (base_z/2.0)
        + (active.GetHeightZ()/2.0);
 
    new G4PVPlacement(0,    // rotation
                      G4ThreeVector(0.0, 
                                    active_pos_y, 
                                    active_pos_z), // position
                      active_logical, // logical volume
                      active.GetName(),   // name
                      logical,  // mother volume
                      false,   // no boolean operations
                      0);    // copy number
 
    // Aluminium Lid
    const double lid_x = GetLengthX() - ND280ECal::BulkheadPrimaryThickness;
    const double lid_y = GetWidthY() - plate_y;
    const double lid_z = ND280ECal::P0D::LidThickness;
    const double lid_pos_x = (-GetLengthX() + lid_x) / 2.0;
    const double lid_pos_y = plate_y / 2.0;
    const double lid_pos_z = ((-GetHeightZ() - lid_z) / 2.0) + fModuleHeightZ;
 
    G4LogicalVolume* lid_logical = components.Bulkhead(lid_x, lid_y, lid_z);
 
    new G4PVPlacement(0,    // rotation
                      G4ThreeVector(lid_pos_x,
                                    lid_pos_y,
                                    lid_pos_z), // position
                      lid_logical, // logical volume
                      GetName()+"/Lid", // name
                      logical,  // mother volume
                      false,   // no boolean operations
                      0);    // copy number
 
    // Primary Bulkheads
 
    // Upstream (readout) bulkhead
    const double bulk_up_x = ND280ECal::BulkheadPrimaryThickness;
    const double bulk_up_y = GetWidthY() - ND280ECal::BulkheadPlateThickness;
    const double bulk_up_z = ND280ECal::P0D::BulkheadReadoutHeight;
    const double bulk_up_pos_x = (GetLengthX() - bulk_up_x) / 2.0;
    const double bulk_up_pos_y = (GetWidthY() - bulk_up_y) / 2.0;
    const double bulk_up_pos_z = (GetHeightZ() - bulk_up_z) / 2.0;
 
    G4LogicalVolume* bulk_up 
        = components.Bulkhead(bulk_up_x, bulk_up_y, bulk_up_z);
 
    new G4PVPlacement(0,    // rotation
                      G4ThreeVector(bulk_up_pos_x, 
                                    bulk_up_pos_y,
                                    bulk_up_pos_z), // position
                      bulk_up, // logical volume
                      GetName()+"/PrimaryBulkheadUpstream", // name
                      logical,  // mother volume
                      false,   // no boolean operations
                      0);    // copy number
 
 
    // Downstream Bulkheads
 
    const double bulk_down_x = ND280ECal::BulkheadPrimaryThickness;
    const double bulk_down_y = GetWidthY() - ND280ECal::BulkheadPlateThickness;
    const double bulk_down_z = fModuleHeightZ - ND280ECal::P0D::LidThickness 
        - ND280ECal::P0D::BaseThickness;
    const double bulk_down_pos_x = (-GetLengthX() + bulk_down_x) / 2.0;
    const double bulk_down_pos_y = (GetWidthY() - bulk_down_y) / 2.0;
    const double bulk_down_pos_z = ((-GetHeightZ() + bulk_down_z) / 2.0) 
        + ND280ECal::P0D::BaseThickness;
 
    G4LogicalVolume* bulk_down 
        = components.Bulkhead(bulk_down_x, bulk_down_y, bulk_down_z);
 
    new G4PVPlacement(0,    // rotation
                      G4ThreeVector(bulk_down_pos_x, 
                                    bulk_down_pos_y, 
                                    bulk_down_pos_z), // position
                      bulk_down, // logical volume
                      GetName()+"/PrimaryBulkheadDownstream", // name
                      logical,  // mother volume
                      false,   // no boolean operations
                      0);    // copy number
 
 
    // Parallel Bulkhead
 
    const double bulk_para_x = GetLengthX() 
        - (2.0 * ND280ECal::BulkheadPrimaryThickness);
    const double bulk_para_y = ND280ECal::BulkheadPrimaryThickness;
    const double bulk_para_z = fModuleHeightZ - ND280ECal::P0D::LidThickness 
        - ND280ECal::P0D::BaseThickness;
    const double bulk_para_pos_y = ((GetWidthY() - bulk_para_y) / 2.0) 
        - ND280ECal::P0D::BulkheadPrimaryInsetSide;
    const double bulk_para_pos_z = ((-GetHeightZ() + bulk_para_z) / 2.0)
        + ND280ECal::P0D::BaseThickness;
 
    G4LogicalVolume* bulk_para 
        = components.Bulkhead(bulk_para_x, bulk_para_y, bulk_para_z);
 
    new G4PVPlacement(0,    // rotation
                      G4ThreeVector(0.0,
                                    bulk_para_pos_y,
                                    bulk_para_pos_z), // position
                      bulk_para, // logical volume
                      GetName()+"/PrimaryBulkheadPara", // name
                      logical,  // mother volume
                      false,   // no boolean operations
                      0);    // copy number
 
    return logical;
}
