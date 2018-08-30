#include <globals.hh>
#include <G4Box.hh>
#include <G4GenericTrap.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4PVReplica.hh>
#include <G4SubtractionSolid.hh>
#include "ND280FlatCableConstructor.hh"



//
// Konosuke Iwamoto 8/9/2018
//
// Constructor of the SuperFGD Flat Cable
//

ND280FlatCableConstructor::~ND280FlatCableConstructor() {;}

void ND280FlatCableConstructor::Init(void) {
  
  fFlatCableWidth = 1*CLHEP::cm;
  fFlatCableHeight = 1*CLHEP::cm;
  fFlatCableLength = 1*CLHEP::cm;

  fCableRegionWidth = 1*CLHEP::cm;
  fCableRegionHeight = 1*CLHEP::cm;
  fCableRegionLength = 1*CLHEP::cm;

  fFlatCableX = 1;
  fFlatCableZ = 1;
  fFlatCableThickness = 1*CLHEP::cm;

  G4String nameFlatCableParent = "FlatCableParent";
  G4String nameFlatCable = "FlatCable";
  
  SetFlatCableParentName(nameFlatCableParent);
  SetFlatCableName(nameFlatCable);
}

G4LogicalVolume *ND280FlatCableConstructor::GetPiece(void) {

  G4String parentname = GetFlatCableParentName();
  G4String name = GetFlatCableName();
  
  //double cablethickness = 0.117 *CLHEP::mm; //Thickness of the FlatCable
  double cablethickness = GetFlatCableThickness(); //Thickness of the FlatCable
  
  //int ncableX = 18;
  //int ncableZ = 32;

  int ncableX = GetFlatCableX();
  int ncableZ = GetFlatCableZ();

  //  double FlatCableWidth = 27.85 *CLHEP::mm;
  double FlatCableWidth = GetFlatCableWidth();;
  double FlatCableHeight = cablethickness * ncableX;
  double FlatCableLength = GetFlatCableLength();

  double CableRegionWidth = GetCableRegionWidth();
  double CableRegionHeight = FlatCableHeight;
  double CableRegionLength = FlatCableLength;


  SetFlatCableWidth(FlatCableWidth);
  SetFlatCableHeight(FlatCableHeight);
  SetFlatCableLength(FlatCableLength);

  SetCableRegionWidth(CableRegionWidth);
  SetCableRegionHeight(CableRegionHeight);
  SetCableRegionLength(CableRegionLength);


   SetFlatCableX(ncableX);
   SetFlatCableZ(ncableZ);


   cout << "Flat Cable Thickness: " << cablethickness  << endl;
   cout << "Flat Cable Width: " << FlatCableWidth  << endl;
   cout << "Flat Cable nX: " << ncableX  << endl;
   cout << "Flat Cable Height: " << FlatCableHeight  << endl;
   cout << "Flat Cable nZ: " << ncableZ  << endl;

  // 0) PCB logical volume

   //  G4LogicalVolume *FlatCable_logical = new G4LogicalVolume(new G4Box(parentname+"/"+name,FlatCableWidth/2., FlatCableHeight/2., FlatCableLength/2.), FindMaterial("Air"), parentname+"/"+name);

   //  G4LogicalVolume *FlatCable_logical = new G4LogicalVolume(new G4Box(parentname+"/"+name,CableRegionWidth/2., CableRegionHeight/2., CableRegionLength/2.), FindMaterial("Air"), parentname+"/"+name);

  G4LogicalVolume *FlatCable_logical = new G4LogicalVolume(new G4Box(parentname+"/"+name,CableRegionWidth/2., CableRegionLength/2., CableRegionHeight/2.), FindMaterial("Air"), parentname+"/"+name);



  //  cout << FlatCableWidth/2. << ", " << FlatCableHeight/2. << ", " << FlatCableLength/2. << endl;


  vector<G4TwoVector> vertices; 
  vertices.push_back( G4TwoVector( -FlatCableWidth/2.,-FlatCableLength/2.) );
  vertices.push_back( G4TwoVector( -FlatCableWidth/2., FlatCableLength/2.) );
  vertices.push_back( G4TwoVector(  FlatCableWidth/2., FlatCableLength/2.) );
  vertices.push_back( G4TwoVector(  FlatCableWidth/2.,-FlatCableLength/2.) );
 
  vertices.push_back( G4TwoVector( -FlatCableWidth/2.,-FlatCableLength/2.) );
  vertices.push_back( G4TwoVector(  FlatCableWidth/2.,-FlatCableLength/2.) );
  vertices.push_back( G4TwoVector( -FlatCableWidth/2.,-FlatCableLength/2.) );
  vertices.push_back( G4TwoVector(  FlatCableWidth/2.,-FlatCableLength/2.) );

  G4VSolid *FlatCableSolid = new G4GenericTrap(
					       "FlatCable_onPCB",
					       FlatCableHeight/2., 
					       vertices
					       );

  G4LogicalVolume *FC_vol = new G4LogicalVolume(FlatCableSolid, FindMaterial("Aluminum"), parentname+"/"+"FlatCable_onPCB");
  FC_vol->SetVisAttributes(G4Colour(1.0,1.0,1.0));

  //ncableZ = 32
  for (int i=0; i<ncableZ; i++)
    {
      //     cout<<"Cable Region Length:" << CableRegionLength/2. << endl;
      //     cout <<i << "th: " << -CableRegionLength/2.+i*(CableRegionLength/ncableZ) + FlatCableWidth/2. << endl;
      new G4PVPlacement(0, G4ThreeVector(-CableRegionWidth/2.+i*(CableRegionWidth/ncableZ) + FlatCableWidth/2.,0,0), FC_vol, parentname+"/"+"FlatCable", FlatCable_logical, false, i);
    }

  G4cout << "FlatCable Mass: " << FC_vol->GetMass()/CLHEP::g << "g" << G4endl;

  return FlatCable_logical;
}
