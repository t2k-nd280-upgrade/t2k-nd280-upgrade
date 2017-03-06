#include <G4LogicalVolume.hh>

//#include <TND280Log.hxx>

#include "ecal/ND280DsECalConstructor.hh"
#include "ecal/ND280DsECalActiveConstructor.hh"
#include "ecal/ND280DsCarbonPanelConstructor.hh"
#include "ecal/ND280ECalComponents.hh"

void ND280DsECalModuleConstructor::Init() {

    ////////
    //
    // NEW ND280UPGRADE
    //    
    // Take XML inputs
    ND280RootPersistencyManager* InputPersistencyManager
      = ND280RootPersistencyManager::GetInstance();
    fND280XMLInput = InputPersistencyManager->GetXMLInput();  
    ////////

    SetLengthX(2300*mm);
    SetWidthY(2300*mm);
    SetHeightZ(485*mm);
    fBuild = true;

    AddConstructor(new ND280DsCarbonPanelConstructor("BasePanel",this));
    AddConstructor(new ND280DsCarbonPanelConstructor("LidPanel",this));
    AddConstructor(new ND280DsECalActiveConstructor("Active",this));
}

G4LogicalVolume* ND280DsECalModuleConstructor::GetPiece() {
  
  // // NEW ND280UPGRADE
  // G4VisAttributes *visual = new G4VisAttributes();
  // visual->SetColor(0.5,0.5,0.5,1); // light grey?

  ND280ECalComponents components(GetName());
    
    //
    // Module volume
    G4VSolid *solid = new G4Box(GetName(),
                                (GetLengthX()/2.0), (GetWidthY()/2.0), (GetHeightZ()/2.0));
    G4LogicalVolume* logical = new G4LogicalVolume(solid, 
                                                   FindMaterial("Air"), 
                                                   GetName());
    // // NEW ND280UPGRADE: no nd280mc
    // if( GetND280XML()->GetXMLInvisDsECal() ){
    //   logical->SetVisAttributes(G4VisAttributes::Invisible);
    // }
    // else{
    //   logical->SetVisAttributes(visual);
    // }


    if(!fBuild) return logical;



   
    // Carbon Base (Upstream)
    ND280DsCarbonPanelConstructor& basePanel 
        = Get<ND280DsCarbonPanelConstructor>("BasePanel");
    basePanel.SetLength(ND280ECal::DS::BaseThickness);
    double base_pos_z = (basePanel.GetLength() - GetHeightZ()) / 2.0;
            
    new G4PVPlacement(0,                     // rotation
    		      G4ThreeVector(0.0, 0.0, base_pos_z), // position
    		      basePanel.GetPiece(),      // logical volume
    		      basePanel.GetName(),       // name
    		      logical,               // mother volume
    		      false,                 // no boolean operations
    		      0);                    // copy number
  
    // // NEW ND280UPGRADE: define the logical volume to make it transparent
    // G4LogicalVolume *logbasePanel = basePanel.GetPiece();      // logical volume     
    // if( GetND280XML()->GetXMLInvisDsECal() ){
    //   logbasePanel->SetVisAttributes(G4VisAttributes::Invisible);
    // }
    // else{
    //   logbasePanel->SetVisAttributes(visual);
    // }
    // new G4PVPlacement(0,                     // rotation
    // 		      G4ThreeVector(0.0, 0.0, base_pos_z), // position
    // 		      logbasePanel,      // logical volume
    // 		      basePanel.GetName(),       // name
    // 		      logical,               // mother volume
    // 		      false,                 // no boolean operations
    // 		      0);                    // copy number

    
    //
    // Active Volume
    ND280DsECalActiveConstructor& active 
        = Get<ND280DsECalActiveConstructor>("Active");
    G4LogicalVolume* logActive = active.GetPiece();
    
    // // NEW ND280UPGRADE: no nd280mc
    // if( GetND280XML()->GetXMLInvisDsECal() ){
    //   logActive->SetVisAttributes(G4VisAttributes::Invisible);
    // }
    // else{
    //   logActive->SetVisAttributes(visual);
    // }

    // Centred in xy
    // Offset BaseHeight from -ve z face
    double active_pos_z 
        = base_pos_z + (basePanel.GetLength()/2.0) + (active.GetHeightZ()/2.0);
    
    new G4PVPlacement(0,                                     // rotation
                      G4ThreeVector(0.0, 0.0, active_pos_z), // position
                      logActive,                             // logical volume
                      active.GetName(),                      // name
                      logical,                               // mother volume
                      false,    // no boolean operations
                      0);       // copy number

     
    //
    // Carbon Lid (Downstream)
    ND280DsCarbonPanelConstructor& lidPanel 
        = Get<ND280DsCarbonPanelConstructor>("LidPanel");
    lidPanel.SetLength(ND280ECal::DS::LidThickness);
    double lid_pos_z = (GetHeightZ() - lidPanel.GetLength()) / 2.0;
    
    new G4PVPlacement(0,                                  // rotation
                      G4ThreeVector(0.0, 0.0, lid_pos_z), // position
                      lidPanel.GetPiece(),                // logical volume
                      lidPanel.GetName(),                 // name
                      logical,                            // mother volume
                      false,    // no boolean operations
                      0);       // lcopy number


    // // NEW ND280UPGRADE: define the logical volume to make it transparent
    // G4LogicalVolume *loglidPanel = lidPanel.GetPiece();      // logical volume     
    // if( GetND280XML()->GetXMLInvisDsECal() ){
    //   loglidPanel->SetVisAttributes(G4VisAttributes::Invisible);
    // }
    // else{
    //   loglidPanel->SetVisAttributes(visual);
    // }   
    // new G4PVPlacement(0,                                  // rotation
    //                   G4ThreeVector(0.0, 0.0, lid_pos_z), // position
    //                   loglidPanel,                // logical volume
    //                   lidPanel.GetName(),                 // name
    //                   logical,                            // mother volume
    //                   false,    // no boolean operations
    //                   0);       // lcopy number

    
    //
    // Primary Bulkheads
    //
    // The bulkheads which surround the active volume, on which the MPPCs sit.
    // Make the horizontal bulkeads the longer ones, to avoid gaps at corners.
    
    const double bulk_z = GetHeightZ() 
        - ND280ECal::DS::LidThickness - ND280ECal::DS::BaseThickness;
    const double bulk_pos_z = ((-GetHeightZ() + bulk_z) / 2.0) 
        + ND280ECal::DS::BaseThickness;
    const double bulk_thick = ND280ECal::BulkheadPrimaryThickness;
    
    // Horizontal
    double bulk_horiz_length = GetLengthX() 
        - (2.0 * ND280ECal::DS::BulkheadPrimaryInset);
    double bulk_horiz_pos_y = ((GetWidthY() - bulk_thick) / 2.0)
        - ND280ECal::DS::BulkheadPrimaryInset;
    
    // Vertical
    double bulk_vert_length = GetWidthY() 
        - (2.0 * (ND280ECal::DS::BulkheadPrimaryInset 
                  + ND280ECal::BulkheadPrimaryThickness));
    double bulk_vert_pos_x = ((GetLengthX() - bulk_thick) / 2.0) 
        - ND280ECal::DS::BulkheadPrimaryInset;
    
    G4LogicalVolume* bulk_horiz 
        = components.Bulkhead(bulk_horiz_length, bulk_thick, bulk_z);
    G4LogicalVolume* bulk_vert
        = components.Bulkhead(bulk_thick, bulk_vert_length, bulk_z);

    // // NEW ND280UPGRADE: no nd280mc
    // if( GetND280XML()->GetXMLInvisDsECal() ){
    //   bulk_horiz->SetVisAttributes(G4VisAttributes::Invisible);
    //   bulk_vert->SetVisAttributes(G4VisAttributes::Invisible);
    // }
    // else{
    //   bulk_horiz->SetVisAttributes(visual);
    //   bulk_vert->SetVisAttributes(visual);
    // }
	
    // horizontal top
    new G4PVPlacement(0,	                   // rotation
                      G4ThreeVector(0.0, 
                                    bulk_horiz_pos_y, 
                                    bulk_pos_z),   // position
                      bulk_horiz,                  // logical volume
                      GetName() + "/PrimaryBulkheadHoriz", // name
                      logical,                     // mother volume
                      false,                       // no boolean operations
                      0);                          // lcopy number
	
    // horizontal bottom
    new G4PVPlacement(0,	                   // rotation
                      G4ThreeVector(0.0, 
                                    -bulk_horiz_pos_y,
                                    bulk_pos_z),   // position
                      bulk_horiz,		   // logical volume
                      GetName() + "/PrimaryBulkheadHoriz", // name
                      logical,                     // mother volume
                      false,                       // no boolean operations
                      1);                          // copy number
	
    // horizontal left
    new G4PVPlacement(0,	                   // rotation
                      G4ThreeVector(bulk_vert_pos_x,
                                    0.0, bulk_pos_z),	// position
                      bulk_vert,                   // logical volume
                      GetName() + "/PrimaryBulkheadVert", // name
                      logical,                     // mother volume
                      false,                       // no boolean operations
                      0);                          // copy number
	
    // horizontal right
    new G4PVPlacement(0,                           // rotation
                      G4ThreeVector(-bulk_vert_pos_x,
                                    0.0, bulk_pos_z),	// position
                      bulk_vert,                   // logical volume
                      GetName() + "/PrimaryBulkheadVert", // name
                      logical,                     // mother volume
                      false,                       // no boolean operations
                      1);                          // copy number
	
    //
    // Aluminium Cooling Plates
    double cp_height = GetHeightZ() 
        - ND280ECal::DS::LidThickness - ND280ECal::DS::BaseThickness;
    double cp_length = GetLengthX() 
        - ( 2.0 * ND280ECal::CoolingPlateBaseWidth );
	
    G4LogicalVolume* cp_logical = components.CoolingPlate(cp_height, cp_length);

    // not in nd280mc
    //cp_logical->SetVisAttributes(G4VisAttributes::Invisible);

    double cp_pos_z = base_pos_z
        + (basePanel.GetLength()/2.0) + (cp_height/2.0);
	
    // Horizontal plate
    double cphoriz_pos_x = 0.0;
    double cphoriz_pos_y = (GetWidthY() - ND280ECal::CoolingPlateBaseWidth) / 2.0;
    G4RotationMatrix* cphoriz0_rotation = new G4RotationMatrix();
    cphoriz0_rotation->rotateZ(-90*degree);
    G4RotationMatrix* cphoriz1_rotation = new G4RotationMatrix();
    cphoriz1_rotation->rotateZ(90*degree);
	
    // Vertical plate
    double cpvert_pos_x = (GetLengthX() - ND280ECal::CoolingPlateBaseWidth) / 2.0;
    double cpvert_pos_y = 0.0;
    G4RotationMatrix* cpvert0_rotation = new G4RotationMatrix();
    G4RotationMatrix* cpvert1_rotation = new G4RotationMatrix();
    cpvert1_rotation->rotateZ(180*degree);
	
    // horizontal top
    new G4PVPlacement(cphoriz0_rotation,           // rotation
                      G4ThreeVector(cphoriz_pos_x,
                                    cphoriz_pos_y, cp_pos_z),	// position
                      cp_logical,                  // logical volume
                      GetName() + "/CoolingPlate", // name
                      logical,                     // mother volume
                      false,                       // no boolean operations
                      0);                          // copy number
	
    // horizontal bottom
    new G4PVPlacement(cphoriz1_rotation,	// rotation
                      G4ThreeVector(cphoriz_pos_x, 
                                    -cphoriz_pos_y, 
                                    cp_pos_z),	// position
                      cp_logical,               // logical volume
                      GetName() + "/CoolingPlate", // name
                      logical,                  // mother volume
                      false,                    // no boolean operations
                      1);                       // copy number
	
    // vertical left
    new G4PVPlacement(cpvert0_rotation,            // rotation
                      G4ThreeVector(cpvert_pos_x,
                                    cpvert_pos_y, cp_pos_z), // position
                      cp_logical,                  // logical volume
                      GetName() + "/CoolingPlate", // name
                      logical,                     // mother volume
                      false,                       // no boolean operations
                      2);                          // copy number
				
    // vertical right
    new G4PVPlacement(cpvert1_rotation,            // rotation
                      G4ThreeVector(-cpvert_pos_x, 
                                    cpvert_pos_y, cp_pos_z),	// position
                      cp_logical,                  // logical volume
                      GetName() + "/CoolingPlate", // name
                      logical,                     // mother volume
                      false,                       // no boolean operations
                      3);                          // copy number
	
    return logical;
}
