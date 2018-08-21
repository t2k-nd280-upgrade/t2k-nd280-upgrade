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
  fCFBoxAIREXThickness = 30*CLHEP::mm;

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

  double CFRPThickness = GetCFBoxCFRPThickness();
  double AIREXThickness = GetCFBoxAIREXThickness();

  double CFBoxWidth = GetCFBoxWidth();
  double CFBoxHeight = GetCFBoxHeight();
  double CFBoxLength = GetCFBoxLength();

  double SuperFGDWidth = CFBoxWidth;
  double SuperFGDHeight = CFBoxHeight;
  double SuperFGDLength = CFBoxLength;

  CFBoxWidth  +=  (2. * CFRPThickness) + AIREXThickness; 
  CFBoxHeight +=  (2. * CFRPThickness) + AIREXThickness; 
  CFBoxLength +=  (2. * CFRPThickness) + AIREXThickness; 


  double thickness = 0;

  SetCFBoxWidth(CFBoxWidth);
  SetCFBoxLength(CFBoxLength);
  SetCFBoxHeight(CFBoxHeight);



  // 0) CFBox logical volume

  G4LogicalVolume *CFBox_logical = new G4LogicalVolume(new G4Box(parentname+"/"+name,CFBoxWidth/2., CFBoxHeight/2., CFBoxLength/2.), FindMaterial("Air"), parentname+"/"+name);


  // 1) Outer CFRP skin layer 
  G4VSolid *CFBox_Out_outer = new G4Box("CFBox_Out_outer", CFBoxWidth/2., CFBoxHeight/2., CFBoxLength/2.);
  thickness = CFRPThickness;
  G4VSolid *CFBox_Out_inner = new G4Box("CFBox_Out_inner", CFBoxWidth/2.-thickness/2., CFBoxHeight/2.-thickness/2., CFBoxLength/2.-thickness/2.);

  G4SubtractionSolid *CFBox_Out = new G4SubtractionSolid("CFBox_Out",CFBox_Out_outer,CFBox_Out_inner);
  G4LogicalVolume *CFBox_Out_logical = new G4LogicalVolume(CFBox_Out, FindMaterial("CarbonFibre"), parentname+"/"+"CFOuter");
  CFBox_Out_logical->SetVisAttributes(G4Colour(1.0,0.0,0.0));
  //  CFBox_Out_logical->SetVisAttributes(G4VisAttributes::Invisible);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), CFBox_Out_logical, parentname+"/"+"CFOuter", CFBox_logical, false, 0);

  //2 ) AIREX layer between two CFRP skin layers
  thickness += AIREXThickness;
  G4VSolid *CFBox_Airex_inner = new G4Box("CFAIREX", CFBoxWidth/2. - thickness/2., CFBoxHeight/2. - thickness/2., CFBoxLength/2. - thickness/2.);
  G4SubtractionSolid *CFBox_Airex = new G4SubtractionSolid("CFBox_Airex",CFBox_Out_inner,CFBox_Airex_inner);
  G4LogicalVolume *AIREX_logical = new G4LogicalVolume(CFBox_Airex, FindMaterial("AIREX"), parentname+"/"+"CFAIREX");
   AIREX_logical->SetVisAttributes(G4Colour(1.0,1.0,0.0));
  new G4PVPlacement(0, G4ThreeVector(0,0,0), AIREX_logical, parentname+"/"+"CFAIREX", CFBox_logical, false, 0);


  // 3) Inner CFRP layer
  thickness += CFRPThickness;
  G4VSolid *CFBox_In_inner = new G4Box("CFBox_In_inner", CFBoxWidth/2. - thickness/2., CFBoxHeight/2. - thickness/2., CFBoxLength/2. - thickness/2.);
  G4SubtractionSolid *CFBox_In = new G4SubtractionSolid("CFBox_In",CFBox_Airex_inner,CFBox_In_inner);
  G4LogicalVolume *CFBox_In_logical = new G4LogicalVolume(CFBox_In, FindMaterial("CarbonFibre"), parentname+"/"+"CFInner");
  CFBox_In_logical->SetVisAttributes(G4Colour(1.0,0.0,0.0));
  new G4PVPlacement(0, G4ThreeVector(0,0,0), CFBox_In_logical, parentname+"/"+"CFInner", CFBox_logical, false, 0); 
  

  G4cout << "CFBox_Outer Mass: " << CFBox_Out_logical->GetMass()/CLHEP::g << "g" << G4endl;
  G4cout << "AIREX Mass: " << AIREX_logical->GetMass()/CLHEP::g << "g" << G4endl;
  G4cout << "CFBox_Inner Mass: " << CFBox_In_logical->GetMass()/CLHEP::g << "g" << G4endl;

  return CFBox_logical;
}
