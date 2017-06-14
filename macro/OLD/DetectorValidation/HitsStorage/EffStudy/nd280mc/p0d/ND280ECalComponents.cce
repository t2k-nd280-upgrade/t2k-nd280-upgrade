#include <globals.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VSolid.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4PVPlacement.hh>
#include <G4VisExtent.hh>
#include <G4VisAttributes.hh>

#include "ecal/ND280ECal.hh"
#include "ecal/ND280ECalComponents.hh"

#include "G4SystemOfUnits.hh"

G4LogicalVolume* ND280ECalComponents::Lead(double x, double y, double z) const {
    G4VSolid *lead_solid = new G4Box(fPrefix+"/Absorber",
                                     (x/2.0), (y/2.0), (z/2.0));
    G4Material* material = G4Material::GetMaterial("Lead",true);
    G4LogicalVolume* lead_logical = new G4LogicalVolume(lead_solid,
                                                        material, 
                                                        fPrefix+"/Absorber");
    //
    // not commented in nd280mc
    //
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(0.3,0.3,0.5,1); // blue/grey?    
    lead_logical->SetVisAttributes(visual);
    //lead_logical->SetVisAttributes(G4VisAttributes::Invisible);
    
    return lead_logical;
}

G4LogicalVolume* 
ND280ECalComponents::Bulkhead(double x, double y, double z) const {
    G4VSolid *solid = new G4Box(fPrefix+"/Bulkhead", (x/2.0), (y/2.0), (z/2.0));
    G4Material* material = G4Material::GetMaterial("Aluminum",true);
    G4LogicalVolume* logical = new G4LogicalVolume(solid, 
                                                   material, 
                                                   fPrefix+"/Bulkhead");
    //
    // not commented in nd280mc
    //
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(0.5,0.5,0.5,1); // light grey?
    logical->SetVisAttributes(visual);
    //logical->SetVisAttributes(G4VisAttributes::Invisible);
       
    return logical;
}

G4LogicalVolume* 
ND280ECalComponents::LayerGap(double x, double y, double z) const {
    G4VSolid *solid = new G4Box(fPrefix+"/Bulkhead", (x/2.0), (y/2.0), (z/2.0));
    G4Material* material = G4Material::GetMaterial("Air",true);
    G4LogicalVolume* logical = new G4LogicalVolume(solid,
                                                   material,
                                                   fPrefix+"/Bulkhead");
    
    //
    // not commented in nd280mc
    //
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(0.0,0.0,0.5,1); // blue?
    logical->SetVisAttributes(visual);
    //logical->SetVisAttributes(G4VisAttributes::Invisible);
    
    return logical;
}

G4LogicalVolume* 
ND280ECalComponents::CarbonFibre(double x, double y, double z) const {
    G4VSolid *solid = new G4Box(fPrefix+"/CarbonFibre", 
                                (x/2.0), (y/2.0), (z/2.0));
    G4Material* material = G4Material::GetMaterial("CarbonFibre",true);
    G4LogicalVolume* logical = new G4LogicalVolume(solid, 
                                                   material, 
                                                   fPrefix+"/CarbonFibre");
    // not in nd280mc
    //logical->SetVisAttributes(G4VisAttributes::Invisible);

    return logical;
}

G4LogicalVolume* 
ND280ECalComponents::CarbonPanel(
    double x, double y, double z, 
    double al_thickness_x, double al_thickness_y) const {
    ND280ECalComponents components(fPrefix+"/CarbonPanel");

    G4VSolid *solid = new G4Box(fPrefix+"/CarbonPanel", 
                                (x/2.0), (y/2.0), (z/2.0));
    G4Material* material = G4Material::GetMaterial("Air",true);
    G4LogicalVolume* logical = new G4LogicalVolume(solid,
                                                   material, 
                                                   fPrefix+"/CarbonPanel");

    // not in nd280mc
    //logical->SetVisAttributes(G4VisAttributes::Invisible);

    // Carbon Fibre Sheets
    double cf_x = x - ( 2 * al_thickness_x );
    double cf_y = y - al_thickness_y;
    double cf_z = ND280ECal::CarbonSheetThickness;
    G4LogicalVolume* cf_logical = components.CarbonFibre(cf_x, cf_y, cf_z);

    // not in nd280mc
    //cf_logical->SetVisAttributes(G4VisAttributes::Invisible);

    // Top: +z
    new G4PVPlacement(
        0,    // rotation
        G4ThreeVector( 0.0, (-al_thickness_y/2.0), ((z - cf_z)/2.0) ), // position
        cf_logical,  // logical volume
        fPrefix+"/CarbonFibre", // name
        logical,  // mother volume
        false,   // no boolean operations
        0    // copy number
        );
 
    // Bottom: -z
    new G4PVPlacement(
        0,    // rotation
        G4ThreeVector( 0.0, (-al_thickness_y/2.0), ((-z + cf_z)/2.0) ), // position
        cf_logical,  // logical volume
        fPrefix+"/CarbonFibre", // name
        logical,  // mother volume
        false,   // no boolean operations
        1    // copy number
        );
    
    
    //
    // Y Aluminium (single thick end)
    // Placed at the positive y end.
    // Spans full width of the panel in x.
    
    G4LogicalVolume* aly_logical = components.Bulkhead(x, al_thickness_y, z);
    
    // not in nd280mc
    //aly_logical->SetVisAttributes(G4VisAttributes::Invisible);

    new G4PVPlacement(
        0,    // rotation
        G4ThreeVector( 0.0, ((y - al_thickness_y)/2.0), 0.0), // position
        aly_logical, // logical volume
        fPrefix+"/AluminiumY", // name
        logical,  // mother volume
        false,   // no boolean operations
        0    // copy number
        );
 
 
    //
    // X Aluminium (two thinner upstream/downstream ends)
    // Spans from edge of AlumiumY to end of pannel
    
    G4LogicalVolume* alx_logical = components.Bulkhead(al_thickness_x, (y - al_thickness_y), z);
    
    // not in nd280mc
    //alx_logical->SetVisAttributes(G4VisAttributes::Invisible);
    
    // +x
    new G4PVPlacement(
        0,    // rotation
        G4ThreeVector( ((x - al_thickness_x)/2.0), (-al_thickness_y/2.0), 0.0), // position
        alx_logical, // logical volume
        fPrefix+"/AluminiumX", // name
        logical,  // mother volume
        false,   // no boolean operations
        0    // copy number
        );
 
    // -x
    new G4PVPlacement(
        0,    // rotation
        G4ThreeVector( ((-x + al_thickness_x)/2.0), (-al_thickness_y/2.0), 0.0), // position
        alx_logical, // logical volume
        fPrefix+"/AluminiumX", // name
        logical,  // mother volume
        false,   // no boolean operations
        1    // copy number
        );
    
    
    //
    // Aluminium Spacer
    // Placed between carbon sheets at -y end
    // Spans the gap between the two AluminiumX
    
    double bar_y = ND280ECal::CarbonSpacerWidth;
    G4LogicalVolume* bar_logical = components.Bulkhead(cf_x, bar_y, (z - (2* ND280ECal::CarbonSheetThickness)));
    
    // not in nd280mc
    //bar_logical->SetVisAttributes(G4VisAttributes::Invisible);

    new G4PVPlacement(
        0,    // rotation
        G4ThreeVector( 0.0, ((-y + bar_y)/2.0), 0.0), // position
        bar_logical, // logical volume
        fPrefix+"/AluminiumSpacer", // name
        logical,  // mother volume
        false,   // no boolean operations
        0    // copy number
        );

    return logical;
}

G4LogicalVolume*
ND280ECalComponents::CoolingPlate(double height, double length) const {
    ND280ECalComponents components(fPrefix+"/CoolingPlate");

    // Z is the same as the ECal Z
    // Y is the longest dimension of the cooling plate (its length)
    // X is the shortest dimension of the cooling plate (its base-width)
    // - the plate is placed at the -ve x side
    
    const double x = ND280ECal::CoolingPlateBaseWidth;
    const double y = length;
    const double z = height;
 
    G4VSolid *solid = new G4Box(fPrefix+"/CoolingPlate",
                                (x/2.0), (y/2.0), (z/2.0));
    G4Material* material = G4Material::GetMaterial("Air",true);
    G4LogicalVolume* logical = new G4LogicalVolume(solid, 
                                                   material,
                                                   fPrefix+"/CoolingPlate");
    
    // not in nd280mc
    //logical->SetVisAttributes(G4VisAttributes::Invisible);

    // Base (top and bottom)
    G4LogicalVolume* base_logical 
        = components.Bulkhead(x, y, ND280ECal::CoolingPlateBaseThickness);

    // not in nd280mc
    //base_logical->SetVisAttributes(G4VisAttributes::Invisible);
    
    const double base_pos_z = (z - ND280ECal::CoolingPlateBaseThickness) / 2.0;
    
    // top
    new G4PVPlacement(
        0,    // rotation
        G4ThreeVector( 0.0, 0.0, base_pos_z), // position
        base_logical, // logical volume
        fPrefix+"/CoolingPlate/Base",   // name
        logical,  // mother volume
        false,   // no boolean operations
        0    // copy number
        );
 
    // bottom
    new G4PVPlacement(
        0,    // rotation
        G4ThreeVector( 0.0, 0.0, -base_pos_z), // position
        base_logical, // logical volume
        fPrefix+"/CoolingPlate/Base",   // name
        logical,  // mother volume
        false,   // no boolean operations
        1    // copy number
        );
 
    // Plate
    double plate_z = z - (2.0 * ND280ECal::CoolingPlateBaseThickness);
    
    G4LogicalVolume* plate_logical 
        = components.Bulkhead(ND280ECal::CoolingPlateThickness, y, plate_z);
    
    // not in nd280mc
    //plate_logical->SetVisAttributes(G4VisAttributes::Invisible);

    double plate_pos_x = (-x + ND280ECal::CoolingPlateThickness) / 2.0;
    
    new G4PVPlacement(
        0,    // rotation
        G4ThreeVector(plate_pos_x, 0.0, 0.0), // position
        plate_logical, // logical volume
        fPrefix+"/CoolingPlate/Plate",  // name
        logical,  // mother volume
        false,   // no boolean operations
        0    // copy number
        );

    return logical;
}
