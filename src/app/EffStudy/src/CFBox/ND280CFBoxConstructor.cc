#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4PVReplica.hh>
#include <G4SubtractionSolid.hh>
#include "ND280CFBoxConstructor.hh"



//
// Konosuke Iwamoto 7/10/2018
//
// Constructor of the SuperFGD CF Box
//

ND280CFBoxConstructor::~ND280CFBoxConstructor() {;}

void ND280CFBoxConstructor::Init(void) {
  
  fCFBoxWidth = 1*CLHEP::cm;
  fCFBoxHeight = 1*CLHEP::cm;
  fCFBoxLength = 1*CLHEP::cm;

  fCFBoxCFRPThickness = 2*CLHEP::mm;
  fCFBoxAIREXThickness = 16*CLHEP::mm;
  fCFBoxPlasticThickness = 4*CLHEP::mm;
  fCFBoxInnerClearanceThickness = 5*CLHEP::mm;
  fCFBoxOuterClearanceThickness = 5*CLHEP::mm;

  G4String nameCFBoxParent = "CFBoxParent";
  G4String nameCFBox = "CFBox";
  
  SetCFBoxParentName(nameCFBoxParent);
  SetCFBoxName(nameCFBox);
}

G4LogicalVolume *ND280CFBoxConstructor::GetPiece(void) {

  G4String parentname = GetCFBoxParentName();
  G4String name = GetCFBoxName();
  
  //  double CFRPThickness = 2*CLHEP::mm;
  //  double AIREXThickness = 30*CLHEP::mm;

  double InnerClearanceThickness = GetCFBoxInnerClearanceThickness(); //5mm
  double CFRPThickness = GetCFBoxCFRPThickness(); // 2mm x2
  double AIREXThickness = GetCFBoxAIREXThickness(); // 16mm 
  double PlasticThickness = GetCFBoxPlasticThickness(); //4mm
  double OuterClearanceThickness = GetCFBoxOuterClearanceThickness(); //5mm
  
  double CFBoxWidth = GetCFBoxWidth();
  double CFBoxHeight = GetCFBoxHeight();
  double CFBoxLength = GetCFBoxLength();

  //double SuperFGDWidth = CFBoxWidth;
  //double SuperFGDHeight = CFBoxHeight;
  //double SuperFGDLength = CFBoxLength;

  CFBoxWidth  +=  (2. * CFRPThickness) + AIREXThickness + (2.*PlasticThickness) + InnerClearanceThickness + (2.*OuterClearanceThickness); 
  CFBoxHeight +=  (2. * CFRPThickness) + AIREXThickness + (2.*PlasticThickness) + InnerClearanceThickness + (2.*OuterClearanceThickness); 
  CFBoxLength +=  (2. * CFRPThickness) + AIREXThickness + (2.*PlasticThickness) + InnerClearanceThickness + (2.*OuterClearanceThickness); 


  double thickness = 0.;
  double x = 0.;
  double y = 0.;
  double z = 0.;
  




  // 0) CFBox logical volume

  G4LogicalVolume *CFBox_logical = new G4LogicalVolume(new G4Box(parentname+"/"+name,CFBoxWidth/2., CFBoxHeight/2., CFBoxLength/2.), FindMaterial("Air"), parentname+"/"+name);
  CFBox_logical->SetVisAttributes(G4VisAttributes::Invisible);
  
  //Material Ordering:
  //OuterC
  //Plastic

  //CFRP
  //AIREX
  //CFRP
  //InnerC

  


  // Set x,y,z to place Outer clearance and outer plastic layers
  x = CFBoxWidth/2.;
  y = CFBoxHeight/2.;
  z = CFBoxLength/2.;
  // Set the CFRP+AIREX+CFRP+InnerC box thickness
  thickness =  2.*(OuterClearanceThickness + PlasticThickness);

  SetCFBoxWidth(CFBoxWidth-thickness);
  SetCFBoxLength(CFBoxLength-thickness);
  SetCFBoxHeight(CFBoxHeight-thickness);
  

  G4RotationMatrix rot[6];

  rot[0].rotateZ(90.0*CLHEP::deg);
  rot[1].rotateY(0.0*CLHEP::deg);
  rot[2].rotateX(90.0*CLHEP::deg);
  
  rot[3].rotateZ(-90.0*CLHEP::deg);
  rot[4].rotateY(-0.0*CLHEP::deg);
  rot[5].rotateX(-90.0*CLHEP::deg);

  // 1) //OuterClearance



  

  G4VSolid *CFBox_OuterC_X = new G4Box("CFBox_OuterC_X", CFBoxHeight/2.-thickness/2., OuterClearanceThickness/2., CFBoxLength/2.-thickness/2.);
  G4LogicalVolume *CFBox_OuterC_X_logical = new G4LogicalVolume(CFBox_OuterC_X, FindMaterial("Air"), parentname+"/"+"OuterClearance_X");
  CFBox_OuterC_X_logical->SetVisAttributes(G4Colour(1.0,1.0,1.0));
  x -= OuterClearanceThickness/2.;
  new G4PVPlacement(G4Transform3D( rot[0], G4ThreeVector( x,0,0)), CFBox_OuterC_X_logical, parentname+"/"+"OuterClearance_X0", CFBox_logical, false, 0);
  new G4PVPlacement(G4Transform3D( rot[3], G4ThreeVector(-x,0,0)), CFBox_OuterC_X_logical, parentname+"/"+"OuterClearance_X1", CFBox_logical, false, 1);

  G4VSolid *CFBox_OuterC_Y = new G4Box("CFBox_OuterC_Y", CFBoxWidth/2.-thickness/2., OuterClearanceThickness/2., CFBoxLength/2.-thickness/2.);
  G4LogicalVolume *CFBox_OuterC_Y_logical = new G4LogicalVolume(CFBox_OuterC_Y, FindMaterial("Air"), parentname+"/"+"OuterClearance_Y");
  CFBox_OuterC_Y_logical->SetVisAttributes(G4Colour(1.0,1.0,1.0));
  y -= OuterClearanceThickness/2.;
  new G4PVPlacement(G4Transform3D( rot[1], G4ThreeVector(0, y,0)), CFBox_OuterC_Y_logical, parentname+"/"+"OuterClearance_Y0", CFBox_logical, false, 0);
  new G4PVPlacement(G4Transform3D( rot[4], G4ThreeVector(0,-y,0)), CFBox_OuterC_Y_logical, parentname+"/"+"OuterClearance_Y1", CFBox_logical, false, 1);
  
  G4VSolid *CFBox_OuterC_Z = new G4Box("CFBox_OuterC_Z", CFBoxWidth/2.-thickness/2., OuterClearanceThickness/2., CFBoxHeight/2.-thickness/2.);
  G4LogicalVolume *CFBox_OuterC_Z_logical = new G4LogicalVolume(CFBox_OuterC_Z, FindMaterial("Air"), parentname+"/"+"OuterClearance_Z");
  CFBox_OuterC_Z_logical->SetVisAttributes(G4Colour(1.0,1.0,1.0));
  z -= OuterClearanceThickness/2.;
  new G4PVPlacement(G4Transform3D( rot[2], G4ThreeVector(0,0, z)), CFBox_OuterC_Z_logical, parentname+"/"+"OuterClearance_Z0", CFBox_logical, false, 0);
  new G4PVPlacement(G4Transform3D( rot[5], G4ThreeVector(0,0,-z)), CFBox_OuterC_Z_logical, parentname+"/"+"OuterClearance_Z1", CFBox_logical, false, 1);

 
  // 2) //Plastic

  G4VSolid *CFBox_Plastic_X = new G4Box("CFBox_Plastic_X", CFBoxHeight/2.-thickness/2., PlasticThickness/2., CFBoxLength/2.-thickness/2.);
  G4LogicalVolume *CFBox_Plastic_X_logical = new G4LogicalVolume(CFBox_Plastic_X, FindMaterial("Polystyrene"), parentname+"/"+"OuterPlastic_X");
  CFBox_Plastic_X_logical->SetVisAttributes(G4Colour(1.0,1.0,0.0));
  x -= OuterClearanceThickness/2.+ PlasticThickness/2.;
  new G4PVPlacement(G4Transform3D( rot[0],G4ThreeVector( x,0,0)), CFBox_Plastic_X_logical, parentname+"/"+"OuterPlastic_X0", CFBox_logical, false, 0);
  new G4PVPlacement(G4Transform3D( rot[3],G4ThreeVector(-x,0,0)), CFBox_Plastic_X_logical, parentname+"/"+"OuterPlastic_X1", CFBox_logical, false, 1);

  G4VSolid *CFBox_Plastic_Y = new G4Box("CFBox_Plastic_Y", CFBoxWidth/2.-thickness/2., PlasticThickness/2., CFBoxLength/2.-thickness/2.);
  G4LogicalVolume *CFBox_Plastic_Y_logical = new G4LogicalVolume(CFBox_Plastic_Y, FindMaterial("Polystyrene"), parentname+"/"+"OuterPlastic_Y");
  CFBox_Plastic_Y_logical->SetVisAttributes(G4Colour(1.0,1.0,0.0));
  y -= OuterClearanceThickness/2.+ PlasticThickness/2.;
  new G4PVPlacement(G4Transform3D( rot[1], G4ThreeVector(0, y,0)), CFBox_Plastic_Y_logical, parentname+"/"+"OuterPlastic_Y0", CFBox_logical, false, 0);
  new G4PVPlacement(G4Transform3D( rot[4], G4ThreeVector(0,-y,0)), CFBox_Plastic_Y_logical, parentname+"/"+"OuterPlastic_Y1", CFBox_logical, false, 1);

  G4VSolid *CFBox_Plastic_Z = new G4Box("CFBox_Plastic_Z", CFBoxWidth/2.-thickness/2., PlasticThickness/2., CFBoxHeight/2.-thickness/2.);
  G4LogicalVolume *CFBox_Plastic_Z_logical = new G4LogicalVolume(CFBox_Plastic_Z, FindMaterial("Polystyrene"), parentname+"/"+"OuterPlastic_Z");
  CFBox_Plastic_Z_logical->SetVisAttributes(G4Colour(1.0,1.0,0.0));
  z -= OuterClearanceThickness/2.+ PlasticThickness/2.;
  new G4PVPlacement(G4Transform3D( rot[2], G4ThreeVector(0,0, z)), CFBox_Plastic_Z_logical, parentname+"/"+"OuterPlastic_Z0", CFBox_logical, false, 0);
  new G4PVPlacement(G4Transform3D( rot[5], G4ThreeVector(0,0,-z)), CFBox_Plastic_Z_logical, parentname+"/"+"OuterPlastic_Z1", CFBox_logical, false, 1);
  
  // 3) Outer CFRP skin layer
  G4VSolid *CFBox_Out_outer = new G4Box("CFBox_Out_outer", CFBoxWidth/2.-thickness/2., CFBoxHeight/2.-thickness/2., CFBoxLength/2.-thickness/2.);
  thickness += CFRPThickness;
  G4VSolid *CFBox_CFRP_outer = new G4Box("CFBox_CFRP_inner", CFBoxWidth/2.-thickness/2., CFBoxHeight/2.-thickness/2., CFBoxLength/2.-thickness/2.);
  G4SubtractionSolid *CFBox_CFRPOut = new G4SubtractionSolid("CFBox_CFRPOut",CFBox_Out_outer,CFBox_CFRP_outer);
  G4LogicalVolume *CFBox_CFRPOut_logical = new G4LogicalVolume(CFBox_CFRPOut, FindMaterial("CarbonFibre"), parentname+"/"+"CFOuter");
  CFBox_CFRPOut_logical->SetVisAttributes(G4Colour(1.0,0.0,0.0));
  //  CFBox_CFRPOut_logical->SetVisAttributes(G4VisAttributes::Invisible);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), CFBox_CFRPOut_logical, parentname+"/"+"CFRPOuter", CFBox_logical, false, 0);

  // 4) AIREX layer between two CFRP skin layers
  thickness += AIREXThickness;
  G4VSolid *CFBox_Airex_inner = new G4Box("CFAIREX", CFBoxWidth/2. - thickness/2., CFBoxHeight/2. - thickness/2., CFBoxLength/2. - thickness/2.);
  G4SubtractionSolid *CFBox_Airex = new G4SubtractionSolid("CFBox_Airex",CFBox_CFRP_outer,CFBox_Airex_inner);
  G4LogicalVolume *AIREX_logical = new G4LogicalVolume(CFBox_Airex, FindMaterial("AIREX"), parentname+"/"+"CFAIREX");
  AIREX_logical->SetVisAttributes(G4Colour(1.0,1.0,0.0));
  new G4PVPlacement(0, G4ThreeVector(0,0,0), AIREX_logical, parentname+"/"+"CFAIREX", CFBox_logical, false, 0);

  // 5) Inner CFRP layer
  thickness += CFRPThickness;
  G4VSolid *CFBox_CFRP_inner = new G4Box("CFBox_CFRP_inner", CFBoxWidth/2. - thickness/2., CFBoxHeight/2. - thickness/2., CFBoxLength/2. - thickness/2.);
  G4SubtractionSolid *CFBox_CFRPIn = new G4SubtractionSolid("CFBox_CFRPIn",CFBox_Airex_inner,CFBox_CFRP_inner);
  G4LogicalVolume *CFBox_CFRPIn_logical = new G4LogicalVolume(CFBox_CFRPIn, FindMaterial("CarbonFibre"), parentname+"/"+"CFInner");
  CFBox_CFRPIn_logical->SetVisAttributes(G4Colour(1.0,0.0,0.0));
  new G4PVPlacement(0, G4ThreeVector(0,0,0), CFBox_CFRPIn_logical, parentname+"/"+"CFRPInner", CFBox_logical, false, 0);

  // 6) Inner Clearance
  thickness += InnerClearanceThickness;
  G4VSolid *CFBox_Inner = new G4Box("CFBox_Inner", CFBoxWidth/2. - thickness/2., CFBoxHeight/2. - thickness/2., CFBoxLength/2. - thickness/2.);
  G4SubtractionSolid *CFBox_InnerC = new G4SubtractionSolid("CFBox_InnerC",CFBox_CFRP_inner,CFBox_Inner);
  G4LogicalVolume *CFBox_InnerC_logical = new G4LogicalVolume(CFBox_InnerC, FindMaterial("Air"), parentname+"/"+"CFInnerClearance");
  CFBox_InnerC_logical->SetVisAttributes(G4Colour(1.0,1.0,1.0));
  new G4PVPlacement(0, G4ThreeVector(0,0,0), CFBox_InnerC_logical, parentname+"/"+"CFInnerClearance", CFBox_logical, false, 0); 

  
  G4cout << "Outer Plastic X Mass: " << CFBox_Plastic_X_logical->GetMass()/CLHEP::g << "g" << G4endl;
  G4cout << "Outer Plastic Y Mass: " << CFBox_Plastic_Y_logical->GetMass()/CLHEP::g << "g" << G4endl;
  G4cout << "Outer Plastic Z Mass: " << CFBox_Plastic_Z_logical->GetMass()/CLHEP::g << "g" << G4endl;
  G4cout << "CFBox_Outer Mass: " << CFBox_CFRPOut_logical->GetMass()/CLHEP::g << "g" << G4endl;
  G4cout << "AIREX Mass: " << AIREX_logical->GetMass()/CLHEP::g << "g" << G4endl;
  G4cout << "CFBox_Inner Mass: " << CFBox_CFRPIn_logical->GetMass()/CLHEP::g << "g" << G4endl;

  return CFBox_logical;
}
