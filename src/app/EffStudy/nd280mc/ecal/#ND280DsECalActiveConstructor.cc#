#include <globals.hh>
#include <G4RotationMatrix.hh>
#include <G4VSolid.hh>
#include <G4ThreeVector.hh>
#include <G4Box.hh>
#include <G4PVPlacement.hh>

#include "ecal/ND280ECal.hh"
#include "ecal/ND280ECalComponents.hh"
#include "ecal/ND280ECalScintXConstructor.hh"
#include "ecal/ND280ECalScintYConstructor.hh"
#include "ecal/ND280DsECalActiveConstructor.hh"

void ND280DsECalActiveConstructor::Init() {
    fLengthX = 0;
    fWidthY = 0;
    fHeightZ = 0;

    AddConstructor(new ND280ECalScintYConstructor("ScintHoriz", this));
    AddConstructor(new ND280ECalScintXConstructor("ScintVert", this));
}

G4LogicalVolume* ND280DsECalActiveConstructor::GetPiece() {

  ND280ECalComponents components(GetName());
    
    // If bar number needs to increase in the negative direction of
    // the active volume, rotate them by 180 degrees
    G4RotationMatrix* rotation = new G4RotationMatrix();
    
    // Set number and length of bars based on whether this is a side module
    int  n_bars = ND280ECal::DS::NBars;
    double  bar_length = ND280ECal::DS::BarLength;

    G4cout << "ND280DsECalActiveConstructor::GetPiece" << G4endl;
    G4cout << "n_bars = " << n_bars << G4endl;
    G4cout << "bar_length = " << bar_length << G4endl;
    //exit(1);

    // Horizontal Scintillator Layer
    ND280ECalScintYConstructor& layer_horiz 
        = Get<ND280ECalScintYConstructor>("ScintHoriz");
    layer_horiz.SetUp(n_bars,
                      bar_length,
                      ND280ECal::kDS); // nBars, barLength, isX, detector
    G4LogicalVolume* layer_horiz_logical = layer_horiz.GetPiece();
 
     // Perpendicular fScintillator Layer
    ND280ECalScintXConstructor& layer_vert 
        = Get<ND280ECalScintXConstructor>("ScintVert");
    layer_vert.SetUp(n_bars, 
                     bar_length, 
                     ND280ECal::kDS); // nBars, barLength, isX, detector
    G4LogicalVolume* layer_vert_logical = layer_vert.GetPiece();
 
    // Lead Layer
    double lead_x = ( bar_length + (2.0 * ND280ECal::LeadOverhang) );
    double lead_y = ( bar_length + (2.0 * ND280ECal::LeadOverhang) );
    double lead_z = ND280ECal::DS::LeadThickness;
    
    G4LogicalVolume* lead_logical = components.Lead(lead_x, lead_y, lead_z);
    
    // Layer Gap
    double gap_x = bar_length;
    double gap_y = bar_length;
    double gap_z = ND280ECal::DS::LayerGapThickness;
    
    G4LogicalVolume* gap_logical = components.LayerGap(gap_x, gap_y, gap_z);
    
    // ActiveVolume - Logical
    fLengthX = bar_length + (2.0 * ND280ECal::LayerFrameWidth);
    fWidthY = bar_length + (2.0 * ND280ECal::LayerFrameWidth);
    fHeightZ 
        = (ND280ECal::BarHeight +  lead_z + gap_z) * ND280ECal::DS::NLayers;
    
    G4VSolid* vol_solid = new G4Box(GetName(),
                                    (fLengthX/2.0),
                                    (fWidthY/2.0), 
                                    (fHeightZ/2.0));
    G4LogicalVolume* vol_logical
        = new G4LogicalVolume(vol_solid, 
                              FindMaterial("Aluminum"), 
                              GetName());
    
    // Positioning
    // Lead, Gap and Layers are centred in xy
    double delta_z1 = (ND280ECal::BarHeight + lead_z) / 2.0;
    double delta_z2 = (lead_z + gap_z) / 2.0;
    double delta_z3 = (gap_z + ND280ECal::BarHeight) / 2.0;
    
    double z_pos = (-fHeightZ / 2.0) + (ND280ECal::BarHeight / 2.0);
 
    int n_y = 0;
    int n_x = 0;
 
    for(int i = 0; i != ND280ECal::DS::NLayers; ++i) {
        // insert scintillator (horiz layers first)
        if( (i % 2) == 0.0 ) {
            new G4PVPlacement(
                0,    // rotation
                G4ThreeVector(0.0, 0.0, z_pos), // position
                layer_horiz_logical, // logical volume
                layer_horiz.GetName(), // name
                vol_logical, // mother volume
                false,   // no boolean operations
                n_y    // copy number
                );
            ++n_y;
        }
        else {
            new G4PVPlacement(
                rotation,  // rotation
                G4ThreeVector(0.0, 0.0, z_pos), // position
                layer_vert_logical, // logical volume
                layer_vert.GetName(), // name
                vol_logical, // mother volume
                false,   // no boolean operations
                n_x    // copy number
                );
            ++n_x;
        }
  
        z_pos += delta_z1;
  



        // insert lead
        new G4PVPlacement(
            0,    // rotation
            G4ThreeVector(0.0, 0.0, z_pos), // position
            lead_logical, // logical volume
            GetName() + "/Absorber",  // name
            vol_logical, // mother volume
            false,   // no boolean operations
            i    // copy number
            );
  
        z_pos += delta_z2;
   
        // insert gap
        new G4PVPlacement(
            0,    // rotation
            G4ThreeVector(0.0, 0.0, z_pos), // position
            gap_logical, // logical volume
            GetName() + "/LayerGap",  // name
            vol_logical, // mother volume
            false,   // no boolean operations
            i    // copy number
            );
  
        z_pos += delta_z3;

    } 

    return vol_logical;
}
