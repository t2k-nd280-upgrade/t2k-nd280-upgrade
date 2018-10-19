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

  fFlatCableThickness = 1*CLHEP::cm;
  fFlatCableN = 6;
  
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
  int nbundles = GetFlatCableNBundles();
 
  //  double FlatCableWidth = 27.85 *CLHEP::mm;
  double FlatCableWidth = GetFlatCableWidth();;
  double FlatCableHeight = cablethickness;
  double FlatCableTotalHeight = cablethickness * nbundles; 
  double FlatCableBundleHeight = cablethickness * (nbundles-1);
  double FlatCableLength = GetFlatCableLength();

  double CableRegionWidth = FlatCableWidth;
  double CableRegionHeight = FlatCableTotalHeight;
  double CableRegionLength = FlatCableLength;


  SetFlatCableWidth(FlatCableWidth);
  SetFlatCableHeight(FlatCableTotalHeight);
  SetFlatCableLength(FlatCableLength);

  SetCableRegionWidth(CableRegionWidth);
  SetCableRegionHeight(CableRegionHeight);
  SetCableRegionLength(CableRegionLength);


  SetFlatCableNBundles(nbundles);

   cout << "Flat Cable Thickness: " << cablethickness << endl;
   cout << "Flat Cable Width: " << FlatCableWidth  << endl;
   cout << "Flat Cable Bundle Thickness: " << FlatCableBundleHeight  << endl;
   cout << "Flat Cable Total Height: " << FlatCableTotalHeight  << endl;
   cout << "# of Bundles: " << nbundles  << endl;

  // 0) PCB logical volume

  G4LogicalVolume *FlatCable_logical = new G4LogicalVolume(new G4Box(parentname+"/"+name,CableRegionWidth/2., CableRegionLength/2., CableRegionHeight/2.), FindMaterial("Air"), parentname+"/"+name);



  double level = CableRegionHeight/2.;

  //  cout << FlatCableWidth/2. << ", " << FlatCableHeight/2. << ", " << FlatCableLength/2. << endl;

  G4VSolid *FlatCableSingleSolid = new G4Box("FlatCableSingle", CableRegionWidth/2.,CableRegionLength/2.,FlatCableHeight/2.);

  G4LogicalVolume *FC_single_vol = new G4LogicalVolume(FlatCableSingleSolid, FindMaterial("Aluminum"), parentname+"/"+"FlatCableSingle");
  FC_single_vol->SetVisAttributes(G4Colour(2.,.0,.0)); 

  cout << level << endl;
  level -= FlatCableHeight/2.;
  cout << level << endl;
  new G4PVPlacement(0, G4ThreeVector(0,0,level), FC_single_vol, parentname+"/"+"FlatCableSingle", FlatCable_logical, false, 0);

  //bundles
  vector<G4TwoVector> vertices;  
  vertices.push_back( G4TwoVector( -FlatCableWidth/2.,-FlatCableLength/2.) );
  vertices.push_back( G4TwoVector(  FlatCableWidth/2.,-FlatCableLength/2.) );
  vertices.push_back( G4TwoVector( -FlatCableWidth/2.,-FlatCableLength/2.) );
  vertices.push_back( G4TwoVector(  FlatCableWidth/2.,-FlatCableLength/2.) );

  vertices.push_back( G4TwoVector( -FlatCableWidth/2.,-FlatCableLength/2.) );
  vertices.push_back( G4TwoVector( -FlatCableWidth/2., FlatCableLength/2.) );
  vertices.push_back( G4TwoVector(  FlatCableWidth/2., FlatCableLength/2.) );
  vertices.push_back( G4TwoVector(  FlatCableWidth/2.,-FlatCableLength/2.) );

  
  G4VSolid *FlatCableBundleSolid = new G4GenericTrap(
					       "FlatCableBundle",
					       FlatCableBundleHeight/2., 
					       vertices
					       );
  
  G4LogicalVolume *FC_bundle_vol = new G4LogicalVolume(FlatCableBundleSolid, FindMaterial("Aluminum"), parentname+"/"+"FlatCable_onPCB");
  FC_bundle_vol->SetVisAttributes(G4Colour(2.,.0,.0));
  level -= FlatCableHeight/2. + FlatCableBundleHeight/2.;
  new G4PVPlacement(0, G4ThreeVector(0,0,level), FC_bundle_vol, parentname+"/"+"FlatCableBundle", FlatCable_logical, false, 0);

    cout << level << endl;
  G4cout << "FlatCable Mass: " <<FC_single_vol->GetMass()/CLHEP::g + FC_bundle_vol->GetMass()/CLHEP::g << "g" << G4endl;

  return FlatCable_logical;
}
