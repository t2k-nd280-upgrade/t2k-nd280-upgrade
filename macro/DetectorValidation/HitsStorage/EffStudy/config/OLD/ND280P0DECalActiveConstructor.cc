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
#include "ecal/ND280P0DECalActiveConstructor.hh"

void ND280P0DECalActiveConstructor::Init() {
    fIsSide = false;
    fLengthX = 0;
    fWidthY = 0;
    fHeightZ = 0;

    AddConstructor(new ND280ECalScintYConstructor("ScintPara", this));
}

G4LogicalVolume* ND280P0DECalActiveConstructor::GetPiece() {
    ND280ECalComponents components(GetName());
 
    // If bar number needs to increase in the negative direction of
    // the active volume, rotate them by 180 degrees
    G4RotationMatrix* rotation = new G4RotationMatrix();
 
    // Set number and length of bars based on whether this is a side module
    int n_bars;
    double bar_length = ND280ECal::P0D::BarLength;
    if(fIsSide) {
        n_bars = ND280ECal::P0D::NBarsSide;
    }
    else {
        n_bars = ND280ECal::P0D::NBarsTB;
    }
 
    // Scintillator Layer
    ND280ECalScintYConstructor& layer 
        = Get<ND280ECalScintYConstructor>("ScintPara");
    layer.SetUp(n_bars, bar_length, ND280ECal::kP0D);
    G4LogicalVolume* layer_logical = layer.GetPiece();
 
    // Lead Layer
    double lead_x 
        = ( ND280ECal::P0D::BarLength + (2.0 * ND280ECal::LeadOverhang) );
    double lead_y = layer.GetHeight() + ND280ECal::LeadOverhang;
    double lead_z = ND280ECal::P0D::LeadThickness;
 
    G4LogicalVolume* lead_logical = components.Lead(lead_x, lead_y, lead_z);
 
    // Layer Gap
    double gap_x = lead_x;
    double gap_y = lead_y;
    double gap_z = ND280ECal::P0D::LayerGapThickness;
 
    G4LogicalVolume* gap_logical = components.LayerGap(gap_x, gap_y, gap_z);
 
    // ActiveVolume - Logical
    fLengthX 
        = ( ND280ECal::P0D::BarLength + (2.0 * ND280ECal::LayerFrameWidth) );
    fWidthY = lead_y - ND280ECal::LeadOverhang + ND280ECal::LayerFrameWidth;
    fHeightZ =  ND280ECal::P0D::NLayers * (ND280ECal::BarHeight 
                                           + ND280ECal::P0D::LeadThickness 
                                           + ND280ECal::P0D::LayerGapThickness);
  
    G4VSolid* vol_solid = new G4Box(GetName(), 
                                    (fLengthX/2.0),
                                    (fWidthY/2.0),
                                    (fHeightZ/2.0));
 
    G4LogicalVolume* vol_logical 
        = new G4LogicalVolume(vol_solid, 
                              FindMaterial("Aluminum"), 
                              GetName());
    
    // Positioning
    // Lead and layers are offset (-y) due to the aluminium frame only being
    // down one side along x
    double lead_y_pos
        = (ND280ECal::LeadOverhang - ND280ECal::LayerFrameWidth) / 2.0;
    double layer_y_pos = -(ND280ECal::LayerFrameWidth / 2.0);
    double gap_y_pos = lead_y_pos;
 
    double delta_z1 = (ND280ECal::BarHeight + lead_z) / 2.0;
    double delta_z2 = (lead_z + gap_z) / 2.0;
    double delta_z3 = (gap_z + ND280ECal::BarHeight) / 2.0;
 
    double z_pos = (-fHeightZ / 2.0) + (ND280ECal::BarHeight / 2.0);
 
    for(int i = 0; i != ND280ECal::P0D::NLayers; ++i) {
        // insert scintillator
        new G4PVPlacement(
            rotation,  // rotation
            G4ThreeVector(0.0, layer_y_pos, z_pos), // position
            layer_logical, // logical volume
            layer.GetName(), // name
            vol_logical, // mother volume
            false,   // no boolean operations
            i    // copy number
            );
  
        z_pos += delta_z1;
  
        // insert lead
        new G4PVPlacement(
            0,    // rotation
            G4ThreeVector(0.0, lead_y_pos, z_pos), // position
            lead_logical, // logical volume
            GetName()+"/Absorber",  // name
            vol_logical, // mother volume
            false,   // no boolean operations
            i    // copy number
            );
  
        z_pos += delta_z2;
  
        // insert air gap
        new G4PVPlacement(
            0,    // rotation
            G4ThreeVector(0.0, gap_y_pos, z_pos), // position
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
