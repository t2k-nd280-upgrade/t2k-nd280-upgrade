#include <globals.hh>
#include <G4RotationMatrix.hh>
#include <G4VSolid.hh>
#include <G4ThreeVector.hh>
#include <G4Box.hh>
#include <G4PVPlacement.hh>

#include <ecal/ND280ECal.hh>
#include <ecal/ND280ECalComponents.hh>
#include <ecal/ND280ECalScintXConstructor.hh>
#include <ecal/ND280ECalScintYConstructor.hh>
#include <ecal/ND280BrlECalActiveConstructor.hh>

void ND280BrlECalActiveConstructor::Init() {
    fIsSide = false;
    fLengthX = 0;
    fWidthY = 0;
    fHeightZ = 0;

    AddConstructor(new ND280ECalScintYConstructor("ScintPara", this));
    AddConstructor(new ND280ECalScintXConstructor("ScintPerp", this));
}

G4LogicalVolume* ND280BrlECalActiveConstructor::GetPiece() {
    ND280ECalComponents components(GetName());
 
    // If bar number needs to increase in the negative direction of
    // the active volume, rotate them by 180 degrees
    G4RotationMatrix* rotation = new G4RotationMatrix();
 
    // Set number and length of bars based on whether this is a side module
    int  n_para_bars;
    double  para_bar_length = ND280ECal::Brl::ParaBarLength;
    int  n_perp_bars = ND280ECal::Brl::NPerpBars;
    double  perp_bar_length;
    if(fIsSide) {
        perp_bar_length = ND280ECal::Brl::PerpBarLengthSide;
        n_para_bars = ND280ECal::Brl::NParaBarsSide;
    }
    else {
        perp_bar_length = ND280ECal::Brl::PerpBarLengthTB;
        n_para_bars = ND280ECal::Brl::NParaBarsTB;
    }
 
    // Parallel Scintillator Layer
    ND280ECalScintYConstructor& layer_para 
        = Get<ND280ECalScintYConstructor>("ScintPara");
    layer_para.SetUp(n_para_bars, 
                     para_bar_length, 
                     ND280ECal::kBrl);
    G4LogicalVolume* layer_para_logical = layer_para.GetPiece();
 
    // Perpendicular Scintillator Layer
    ND280ECalScintXConstructor& layer_perp 
        = Get<ND280ECalScintXConstructor>("ScintPerp");
    layer_perp.SetUp(n_perp_bars, perp_bar_length, ND280ECal::kBrl);
    G4LogicalVolume* layer_perp_logical = layer_perp.GetPiece();
 
    // Lead Layer
    double lead_x = ( para_bar_length + (2.0 * ND280ECal::LeadOverhang) );
    double lead_y = ( perp_bar_length + ND280ECal::LeadOverhang );
    double lead_z = ND280ECal::Brl::LeadThickness;
 
    G4LogicalVolume* lead_logical = components.Lead(lead_x, lead_y, lead_z);
 
    // Layer Gap
    double gap_x = para_bar_length;
    double gap_y = perp_bar_length;
    double gap_z = ND280ECal::Brl::LayerGapThickness;
 
    G4LogicalVolume* gap_logical = components.LayerGap(gap_x, gap_y, gap_z);

    // ActiveVolume - Logical
    fLengthX = para_bar_length + (2.0 * ND280ECal::LayerFrameWidth);
    fWidthY = perp_bar_length + ND280ECal::LayerFrameWidth;
    fHeightZ = ND280ECal::Brl::NLayers * (ND280ECal::BarHeight+lead_z+gap_z);
 
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
 
    int n_y = 0;
    int n_x = 0;
 
    for(int i = 0; i != ND280ECal::Brl::NLayers; ++i) {
        // insert scintillator (more perp layers than parallel)
        if( (i % 2) == 0.0 ) {
            new G4PVPlacement(
                0,    // rotation
                G4ThreeVector(0.0, layer_y_pos, z_pos), // position
                layer_perp_logical, // logical volume
                layer_perp.GetName(), // name
                vol_logical, // mother volume
                false,   // no boolean operations
                n_y    // copy number
                );
            ++n_y;
        }
        else {
            new G4PVPlacement(
                rotation,  // rotation
                G4ThreeVector(0.0, layer_y_pos, z_pos), // position
                layer_para_logical, // logical volume
                layer_para.GetName(), // name
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
            G4ThreeVector(0.0, lead_y_pos, z_pos), // position
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
            G4ThreeVector(0.0, gap_y_pos, z_pos), // position
            gap_logical, // logical volume
            GetName()+"/LayerGap",  // name
            vol_logical, // mother volume
            false,   // no boolean operations
            i    // copy number
            );
  
        z_pos += delta_z3;
    }
 
    return vol_logical;
}
