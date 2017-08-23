#include <globals.hh>
#include <G4VSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4Box.hh>

#include "ecal/ND280ECal.hh"
#include "ecal/ND280ECalComponents.hh"
#include "ecal/ND280BrlECalActiveConstructor.hh"
#include "ecal/ND280BrlECalTLBRConstructor.hh"

void ND280BrlECalTLBRConstructor::Init() {
  SetLengthX(4140*CLHEP::mm);
  SetWidthY(1676*CLHEP::mm);
  SetHeightZ(462.5*CLHEP::mm);
    AddConstructor(new ND280BrlECalActiveConstructor("Active",this));
    Get<ND280BrlECalActiveConstructor>("Active").SetIsSide(false);
}

G4LogicalVolume* ND280BrlECalTLBRConstructor::GetPiece() {
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
    // Centred in x-z. Sits along -y face (magnet centre-line)
    double plate_y = ND280ECal::BulkheadPlateThickness;
    double plate_pos_y = (-GetWidthY() + plate_y) / 2.0;
 
    G4LogicalVolume* plate_logical 
        = components.Bulkhead(GetLengthX(), plate_y, GetHeightZ());
 
    new G4PVPlacement(0,    // rotation
                      G4ThreeVector(0.0, plate_pos_y, 0.0), // position
                      plate_logical, // logical volume
                      GetName()+"/BulkheadPlate",// name
                      logical,  // mother volume
                      false,   // no boolean operations
                      0);    // copy number

    // Base - Carbon Panel
    double base_y = GetWidthY() - plate_y;
    double base_z = ND280ECal::Brl::BaseThicknessTB;
    double base_pos_y = plate_y / 2.0;
    double base_pos_z = (base_z - GetHeightZ()) / 2.0;
 
    G4LogicalVolume* base_logical 
        = components.CarbonPanel(GetLengthX(), base_y, base_z,
                                 ND280ECal::Brl::CarbonFrameX,
                                 ND280ECal::Brl::CarbonFrameYTB);
 
    new G4PVPlacement(0,    // rotation
                      G4ThreeVector(0.0, base_pos_y, base_pos_z), // position
                      base_logical, // logical volume
                      GetName()+"/Base",   // name
                      logical,  // mother volume
                      false,   // no boolean operations
                      0);    // copy number

    // Active Volume
    ND280BrlECalActiveConstructor& active
        = Get<ND280BrlECalActiveConstructor>("Active");
    G4LogicalVolume* active_logical = active.GetPiece();
 
    // Centred in x
    // Offset 4mm from -ve y face
    // Offset BaseHeight from -ve z face
    double active_pos_y = -(GetWidthY()/2.0) + (active.GetWidthY()/2.0) + plate_y;
    double active_pos_z = base_pos_z + (base_z/2.0) + (active.GetHeightZ()/2.0);
 
    new G4PVPlacement(0,    // rotation
                      G4ThreeVector(0.0, 
                                    active_pos_y,
                                    active_pos_z), // position
                      active_logical, // logical volume
                      GetName()+"/Active",   // name
                      logical,  // mother volume
                      false,   // no boolean operations
                      0);    // copy number

    // Aluminium Lid
    double lid_y = GetWidthY() - plate_y;
    double lid_z = ND280ECal::Brl::LidThicknessTB;
    double lid_pos_y = plate_y / 2.0;
    double lid_pos_z = (GetHeightZ() - lid_z) / 2.0;
 
    G4LogicalVolume* lid_logical = components.Bulkhead(GetLengthX(),
                                                       lid_y, lid_z);
 
    new G4PVPlacement(0,    // rotation
                      G4ThreeVector(0.0, 
                                    lid_pos_y, 
                                    lid_pos_z), // position
                      lid_logical,              // logical volume
                      GetName() + "/Lid",   // name
                      logical,  // mother volume
                      false,   // no boolean operations
                      0);    // copy number
 
    // Primary Bulkheads
    const double bulk_z = GetHeightZ()
        - ND280ECal::Brl::LidThicknessTB 
        - ND280ECal::Brl::BaseThicknessTB;
    const double bulk_pos_z = ((-GetHeightZ() + bulk_z) / 2.0)
        + ND280ECal::Brl::BaseThicknessTB;
    const double bulk_thick = ND280ECal::BulkheadPrimaryThickness;
 
    double bulk_para_x = GetLengthX()
        - (2.0 * ND280ECal::Brl::BulkheadPrimaryInset);
    double bulk_para_pos_y = ((GetWidthY() - bulk_thick) / 2.0) 
        - ND280ECal::Brl::BulkheadPrimaryInset;
 
    double bulk_perp_y = GetWidthY() - ND280ECal::BulkheadPlateThickness 
        - bulk_thick - ND280ECal::Brl::BulkheadPrimaryInset;
    double bulk_perp_pos_x = ((GetLengthX() - bulk_thick) / 2.0) 
        - ND280ECal::Brl::BulkheadPrimaryInset;
    double bulk_perp_pos_y = ((-GetWidthY() + bulk_perp_y) / 2.0)
        + ND280ECal::BulkheadPlateThickness;
 
    G4LogicalVolume* bulk_para 
        = components.Bulkhead(bulk_para_x, bulk_thick, bulk_z);
    G4LogicalVolume* bulk_perp 
        = components.Bulkhead(bulk_thick, bulk_perp_y, bulk_z);
 
    // Parallel Bulkhead
    new G4PVPlacement(0,      // rotation
                      G4ThreeVector(0.0,
                                    bulk_para_pos_y,
                                    bulk_pos_z), // position
                      bulk_para,    // logical volume
                      GetName()+"/PrimaryBulkheadPara", // name
                      logical,    // mother volume
                      false,     // no boolean operations
                      0);      // copy number
    
    // Perpendicular Downstream Bulkhead
    new G4PVPlacement(0,      // rotation
                      G4ThreeVector(bulk_perp_pos_x, 
                                    bulk_perp_pos_y,
                                    bulk_pos_z), // position
                      bulk_perp,    // logical volume
                      GetName()+"/PrimaryBulkheadPerp", // name
                      logical,    // mother volume
                      false,     // no boolean operations
                      0);      // copy number
    
    // Perpendicular Upstream Bulkhead
    new G4PVPlacement(0,      // rotation
                      G4ThreeVector(-bulk_perp_pos_x,
                                    bulk_perp_pos_y,
                                    bulk_pos_z), // position
                      bulk_perp,    // logical volume
                      GetName()+"/PrimaryBulkheadPerp", // name
                      logical,    // mother volume
                      false,     // no boolean operations
                      1);      // copy number
    
    // Aluminium Cooling Plates
    double cp_height = GetHeightZ() - ND280ECal::Brl::LidThicknessTB 
        - ND280ECal::Brl::BaseThicknessTB;
    double cp_pos_z = base_pos_z + (base_z/2.0) + (cp_height/2.0);
 
    // Parallel plate
    double cppara_length = GetLengthX()
        - ( 2.0 * ND280ECal::CoolingPlateBaseWidth );
    double cppara_pos_x = 0.0;
    double cppara_pos_y = (GetWidthY() - ND280ECal::CoolingPlateBaseWidth) / 2.0;
    G4RotationMatrix* cppara_rotation = new G4RotationMatrix();
    cppara_rotation->rotateZ(-90*CLHEP::degree);
    
    // Perpendicular plates
    double cpperp_length = GetWidthY() - ND280ECal::BulkheadPlateThickness 
        - ND280ECal::CoolingPlateBaseWidth;
    double cpperp_pos_x = (GetLengthX() - ND280ECal::CoolingPlateBaseWidth) / 2.0;
    double cpperp_pos_y = ((-GetWidthY() + cpperp_length) / 2.0)
        + ND280ECal::BulkheadPlateThickness;
    G4RotationMatrix* cpperp1_rotation = new G4RotationMatrix();
    cpperp1_rotation->rotateZ(180*CLHEP::degree);
 
    G4LogicalVolume* cpperp_logical 
        = components.CoolingPlate(cp_height, cpperp_length);
    G4LogicalVolume* cppara_logical 
        = components.CoolingPlate(cp_height, cppara_length);
 
    new G4PVPlacement(cppara_rotation, // rotation
                      G4ThreeVector(cppara_pos_x,
                                    cppara_pos_y,
                                    cp_pos_z), // position
                      cppara_logical,  // logical volume
                      GetName()+"/CoolingPlatePara", // name
                      logical,   // mother volume
                      false,    // no boolean operations
                      0);     // copy number
    
    new G4PVPlacement(0,    // rotation
                      G4ThreeVector(cpperp_pos_x,
                                    cpperp_pos_y,
                                    cp_pos_z), // position
                      cpperp_logical, // logical volume
                      GetName()+"/CoolingPlatePerp", // name
                      logical,  // mother volume
                      false,   // no boolean operations
                      0);    // copy number
    
    new G4PVPlacement(cpperp1_rotation, // rotation
                      G4ThreeVector(-cpperp_pos_x,
                                    cpperp_pos_y, 
                                    cp_pos_z), // position
                      cpperp_logical,  // logical volume
                      GetName()+"/CoolingPlatePerp", // name
                      logical,   // mother volume
                      false,    // no boolean operations
                      1);     // copy number
 
    return logical;
}
