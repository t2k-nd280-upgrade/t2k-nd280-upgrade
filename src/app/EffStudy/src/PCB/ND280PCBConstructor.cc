#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4PVReplica.hh>
#include <G4SubtractionSolid.hh>
#include "ND280PCBConstructor.hh"



//
// Konosuke Iwamoto 8/9/2018
//
// Constructor of the SuperFGD PCB
//

ND280PCBConstructor::~ND280PCBConstructor() {;}

void ND280PCBConstructor::Init(void) {
  
  fPCBWidth = 1*CLHEP::cm;
  fPCBHeight = 1*CLHEP::cm;
  fPCBLength = 1*CLHEP::cm;

  G4String namePCBParent = "PCBParent";
  G4String namePCB = "PCB";
  
  SetPCBParentName(namePCBParent);
  SetPCBName(namePCB);
}

G4LogicalVolume *ND280PCBConstructor::GetPiece(void) {

  G4String parentname = GetPCBParentName();
  G4String name = GetPCBName();
  
  double layer1thickness = 0.050 *CLHEP::mm; //Copper
  double layer2thickness = 0.200 *CLHEP::mm; //FR4
  double layer3thickness = 0.035 *CLHEP::mm; //Copper
  double layer4thickness = 1.000 *CLHEP::mm; //FR4
  double layer5thickness = 0.035 *CLHEP::mm; //Copper
  double layer6thickness = 0.200 *CLHEP::mm; //FR4
  double layer7thickness = 0.050 *CLHEP::mm; //Copper

  double totalthickness = 1.57*CLHEP::mm; 


  double PCBWidth = GetPCBWidth();
  double PCBHeight = GetPCBHeight();
  // double PCBHeight = totalthickness; //for now height should be always 1.57 mm
  double PCBLength = GetPCBLength();

  SetPCBWidth(PCBWidth);
  SetPCBHeight(PCBHeight);
  SetPCBLength(PCBLength);


  
  double level=totalthickness/2.;


  // 0) PCB logical volume

  G4LogicalVolume *PCB_logical = new G4LogicalVolume(new G4Box(parentname+"/"+name,PCBWidth/2., PCBHeight/2., PCBLength/2.), FindMaterial("Air"), parentname+"/"+name);

  
  // 1) Layer1
  G4VSolid *PCB_layer1 = new G4Box("PCB_layer1", PCBWidth/2., layer1thickness/2., PCBLength/2.);
  G4LogicalVolume *PCB_l1_logical = new G4LogicalVolume(PCB_layer1, FindMaterial("Copper"), parentname+"/"+"PCBlayer1");
  PCB_l1_logical->SetVisAttributes(G4Colour(1.0,1.0,0.0));
  level -= layer1thickness/2.;
  new G4PVPlacement(0, G4ThreeVector(0,level,0), PCB_l1_logical, parentname+"/"+"PCBlayer1", PCB_logical, false, 0);

  // 2) Layer2
  G4VSolid *PCB_layer2 = new G4Box("PCB_layer2", PCBWidth/2., layer2thickness/2., PCBLength/2.);
  G4LogicalVolume *PCB_l2_logical = new G4LogicalVolume(PCB_layer2, FindMaterial("G10"), parentname+"/"+"PCBlayer2");
  PCB_l2_logical->SetVisAttributes(G4Colour(0.0,1.0,0.0));
  level -= layer1thickness/2. + layer2thickness/2. ;
  new G4PVPlacement(0, G4ThreeVector(0,level,0), PCB_l2_logical, parentname+"/"+"PCBlayer2", PCB_logical, false, 0);

  // 3) Layer3
  G4VSolid *PCB_layer3 = new G4Box("PCB_layer3", PCBWidth/2., layer3thickness/2., PCBLength/2.);
  G4LogicalVolume *PCB_l3_logical = new G4LogicalVolume(PCB_layer3, FindMaterial("Copper"), parentname+"/"+"PCBlayer3");
  PCB_l3_logical->SetVisAttributes(G4Colour(1.0,1.0,0.0));
  level -= layer2thickness/2. + layer3thickness/2. ;
  new G4PVPlacement(0, G4ThreeVector(0,level,0), PCB_l3_logical, parentname+"/"+"PCBlayer3", PCB_logical, false, 0);

  // 4) Layer4
  G4VSolid *PCB_layer4 = new G4Box("PCB_layer4", PCBWidth/2., layer4thickness/2., PCBLength/2.);
  G4LogicalVolume *PCB_l4_logical = new G4LogicalVolume(PCB_layer4, FindMaterial("G10"), parentname+"/"+"PCBlayer4");
  PCB_l4_logical->SetVisAttributes(G4Colour(0.0,1.0,0.0));
  level -= layer3thickness/2. + layer4thickness/2. ;
  new G4PVPlacement(0, G4ThreeVector(0,level,0), PCB_l4_logical, parentname+"/"+"PCBlayer4", PCB_logical, false, 0);

  // 5) Layer5
  G4VSolid *PCB_layer5 = new G4Box("PCB_layer5", PCBWidth/2., layer5thickness/2., PCBLength/2.);
  G4LogicalVolume *PCB_l5_logical = new G4LogicalVolume(PCB_layer5, FindMaterial("Copper"), parentname+"/"+"PCBlayer5");
  PCB_l3_logical->SetVisAttributes(G4Colour(1.0,1.0,0.0));
  level -= layer4thickness/2. + layer5thickness/2. ;
  new G4PVPlacement(0, G4ThreeVector(0,level,0), PCB_l5_logical, parentname+"/"+"PCBlayer5", PCB_logical, false, 0);

  // 6) Layer6
  G4VSolid *PCB_layer6 = new G4Box("PCB_layer6", PCBWidth/2., layer6thickness/2., PCBLength/2.);
  G4LogicalVolume *PCB_l6_logical = new G4LogicalVolume(PCB_layer6, FindMaterial("G10"), parentname+"/"+"PCBlayer6");
  PCB_l3_logical->SetVisAttributes(G4Colour(0.0,1.0,0.0));
  level -= layer5thickness/2. + layer6thickness/2. ;
  new G4PVPlacement(0, G4ThreeVector(0,level,0), PCB_l6_logical, parentname+"/"+"PCBlayer6", PCB_logical, false, 0);

  // 7) Layer7
  G4VSolid *PCB_layer7 = new G4Box("PCB_layer7", PCBWidth/2., layer7thickness/2., PCBLength/2.);
  G4LogicalVolume *PCB_l7_logical = new G4LogicalVolume(PCB_layer7, FindMaterial("Copper"), parentname+"/"+"PCBlayer7");
  PCB_l7_logical->SetVisAttributes(G4Colour(1.0,1.0,0.0));
  level -= layer6thickness/2. + layer7thickness/2. ;
  new G4PVPlacement(0, G4ThreeVector(0,level,0), PCB_l7_logical, parentname+"/"+"PCBlayer7", PCB_logical, false, 0);

  /*
  G4cout << "CFBox_Outer Mass: " << CFBox_Out_logical->GetMass()/CLHEP::g << "g" << G4endl;
  G4cout << "AIREX Mass: " << AIREX_logical->GetMass()/CLHEP::g << "g" << G4endl;
  G4cout << "CFBox_Inner Mass: " << CFBox_In_logical->GetMass()/CLHEP::g << "g" << G4endl;
  */


  // 4) Electronics on 3 sides
  /*
  G4VSolid *OuterPCBX = new G4Box("PCBX", PCBXWidth/2., PCBHeight/2., PCBXLength/2.);
  G4LogicalVolume* logPCBX = new G4LogicalVolume(OuterPCBX, FindMaterial("Copper"),parentname+"/"+name+"/PCBX");
  logPCBX->SetVisAttributes(G4Colour(0.0,1.0,0.0));

  G4VSolid *OuterPCBY = new G4Box("PCBY", PCBYWidth/2., PCBHeight/2., PCBYLength/2.);
  G4LogicalVolume* logPCBY = new G4LogicalVolume(OuterPCBY, FindMaterial("Copper"),parentname+"/"+name+"/PCBY");
  logPCBY->SetVisAttributes(G4Colour(0.0,1.0,0.0));

  G4VSolid *OuterPCBZ = new G4Box("PCBZ", PCBZWidth/2., PCBHeight/2., PCBZLength/2.);
  G4LogicalVolume* logPCBZ = new G4LogicalVolume(OuterPCBZ, FindMaterial("Copper"),parentname+"/"+name+"/PCBZ");
  logPCBZ->SetVisAttributes(G4Colour(0.0,1.0,0.0));


  double xPCB[3] = {CFBoxWidth/2. + PCBHeight/2., 0., 0.};
  double yPCB[3] = {0., CFBoxHeight/2. + PCBHeight/2., 0.};
  double zPCB[3] = {0., 0. ,CFBoxLength/2. + PCBHeight/2.};

  G4RotationMatrix rot[3];  

  rot[0].rotateZ(90.0*CLHEP::deg);
  rot[1].rotateY(0.0*CLHEP::deg);
  rot[2].rotateX(90.0*CLHEP::deg);
  
    new G4PVPlacement(G4Transform3D(rot[0], G4ThreeVector(xPCB[0],yPCB[0],zPCB[0])), logPCBX,  parentname+"/"+name+"/PCBX", CFBox_logical, false,  0);

    new G4PVPlacement(G4Transform3D(rot[1], G4ThreeVector(xPCB[1],yPCB[1],zPCB[1])), logPCBY,  parentname+"/"+name+"/PCBY", CFBox_logical, false,  1);

    new G4PVPlacement(G4Transform3D(rot[2], G4ThreeVector(xPCB[2],yPCB[2],zPCB[2])), logPCBZ,  parentname+"/"+name+"/PCBZ", CFBox_logical, false,  2);

  G4cout << "PCBX Mass: " << logPCBX->GetMass()/CLHEP::g << "g" << G4endl;
  G4cout << "PCBY Mass: " << logPCBY->GetMass()/CLHEP::g << "g" << G4endl;
  G4cout << "PCBZ Mass: " << logPCBZ->GetMass()/CLHEP::g << "g" << G4endl;

  */
  return PCB_logical;
}
