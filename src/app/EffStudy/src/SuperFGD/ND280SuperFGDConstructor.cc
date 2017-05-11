#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4PVReplica.hh>

#include <G4Tubs.hh>

#include "ND280SuperFGDConstructor.hh"
#include "ND280CubeScintConstructor.hh"

//
// Davide Sgalaberna 28/3/17
//
// Constructor of the SuperFGD detector
//

class ND280SuperFGDMessenger: public ND280ConstructorMessenger {
private:
  ND280SuperFGDConstructor* fConstructor;
  G4UIcmdWithADoubleAndUnit* fWidthCMD;
  G4UIcmdWithADoubleAndUnit* fHeightCMD;
  G4UIcmdWithADoubleAndUnit* fLengthCMD;
  
public:
    ND280SuperFGDMessenger(ND280SuperFGDConstructor* c) 
        : ND280ConstructorMessenger(c,"Control the off axis geometry."),
          fConstructor(c) {
        fWidthCMD = new G4UIcmdWithADoubleAndUnit(CommandName("width"),this);
        fWidthCMD->SetGuidance("Set the width of the off axis detector.");
        fWidthCMD->SetParameterName("Width",false);
        fWidthCMD->SetUnitCategory("Length");

        fHeightCMD = new G4UIcmdWithADoubleAndUnit(CommandName("height"),this);
        fHeightCMD->SetGuidance("Set the height of the off axis detector.");
        fHeightCMD->SetParameterName("Height",false);
        fHeightCMD->SetUnitCategory("Length");

        fLengthCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("length"),this);
        fLengthCMD->SetGuidance("Set the length of the off axis detector.");
        fLengthCMD->SetParameterName("Length",false);
        fLengthCMD->SetUnitCategory("Length");
    }

    virtual ~ND280SuperFGDMessenger() {
        delete fWidthCMD;
        delete fHeightCMD;
        delete fLengthCMD;
    }

    void SetNewValue(G4UIcommand *cmd, G4String val) {
      if (cmd==fWidthCMD) {
	fConstructor->SetWidth(fWidthCMD->GetNewDoubleValue(val));
      }
      else if (cmd==fHeightCMD) {
	fConstructor->SetHeight(fHeightCMD->GetNewDoubleValue(val));
      }
      else if (cmd==fLengthCMD) {
	fConstructor->SetLength(fLengthCMD->GetNewDoubleValue(val));
      }
      else {
	ND280ConstructorMessenger::SetNewValue(cmd,val);
      }
    }
};

ND280SuperFGDConstructor::~ND280SuperFGDConstructor() {;}

void ND280SuperFGDConstructor::Init(void) {
  
  /// The edge of a cube of the SuperFGD detector 
  fEdge = 1*cm;

  // Number of cubes along each axis 
  fCubeNumX = 1;
  fCubeNumY = 1;
  fCubeNumZ = 1;

  SetWidth( fCubeNumX * fEdge);
  SetHeight(fCubeNumY * fEdge);
  SetLength(fCubeNumZ * fEdge);

  //SetFiberRadius(0.5*mm);
  //SetFiberMaterial("FiberCore");

  // Position of the center of the SuperFGD detector
  fPosX = 0.;
  fPosY = 0.;
  fPosZ = 0.;

  //
  // Note:
  // GetName() is the SuperFGD name given in DetectorConstruction  
  // i.e. /t2k/OA/.../SuperFGD
  //

  G4String nameRepXYZ = "RepY"; // replica of single cube along Z
  G4String nameRepXZ  = nameRepXYZ+"/RepX"; // replica of single row  along X
  G4String nameRepZ   = nameRepXZ +"/RepZ"; // replica of single layer along Y
  G4String nameCube   = nameRepZ+"/CubeScint";
  
  SetNameRepXYZ(nameRepXYZ);
  SetNameRepXZ(nameRepXZ);
  SetNameRepZ(nameRepZ);
  SetNameCube(nameCube);

  AddConstructor(new ND280CubeScintConstructor(GetNameCube(), this)); 

  SetMessenger(new ND280SuperFGDMessenger(this));
}

G4LogicalVolume *ND280SuperFGDConstructor::GetPiece(void) {

  ND280RootPersistencyManager* InputPersistencyManager
    = ND280RootPersistencyManager::GetInstance();
  fND280XMLInput = InputPersistencyManager->GetXMLInput();

  // Set total size of the Super-FGD

  double TotWidth  = fCubeNumX * fEdge; 
  double TotHeight = fCubeNumY * fEdge; 
  double TotLength = fCubeNumZ * fEdge; 

  SetWidth(TotWidth);
  SetLength(TotLength);
  SetHeight(TotHeight);

  // Build the plastic scintillator cube

  ND280CubeScintConstructor& cube
    = Get<ND280CubeScintConstructor>(GetNameCube());
  
  cube.SetBase(fEdge);
  cube.SetLength(fEdge);
  cube.SetHeight(fEdge);  
  cube.SetHoleRadius(0.7*mm);
  cube.SetFiberRadius(0.5*mm);
  cube.SetCoatingThickness(0.25*mm);
  cube.SetGap(0.0*mm);
  
  double shift = 1.5*mm; //cube.GetHoleRadius();
  G4ThreeVector HolePosAlongX = G4ThreeVector(0.    , shift , shift); // hole along X
  G4ThreeVector HolePosAlongY = G4ThreeVector(shift , 0.    , -shift); // hole along Y
  G4ThreeVector HolePosAlongZ = G4ThreeVector(-shift, -shift, 0.    ); // hole along Z
  cube.SetHolePosition_X(HolePosAlongX); 
  cube.SetHolePosition_Y(HolePosAlongY); 
  cube.SetHolePosition_Z(HolePosAlongZ); 

  G4LogicalVolume* cube_logical = cube.GetPiece();

  //
  // Make the logical volumes for the replicas
  //
 
  // Build row of cubes along Z (replica of single cube along Z)

  G4VSolid *repZ_solid 
    = new G4Box(GetName()+"/"+GetNameRepZ(), 
		cube.GetBase()/2, 
		cube.GetHeight()/2, 
		GetLength()/2); 
  G4LogicalVolume *repZ_logical 
    = new G4LogicalVolume(repZ_solid,
			  FindMaterial("Air"),
			  GetName()+"/"+GetNameRepZ());
  repZ_logical->SetVisAttributes(G4VisAttributes::Invisible);  
     
  // Build layer of cubes XZ (replica of single row of cubes along X)  

  G4VSolid *repXZ_solid 
    = new G4Box(GetName()+"/"+GetNameRepXZ(), 
		GetWidth()/2, 
		cube.GetHeight()/2, 
		GetLength()/2);   
  G4LogicalVolume *repXZ_logical 
    = new G4LogicalVolume(repXZ_solid,
			  FindMaterial("Air"),
			  GetName()+"/"+GetNameRepXZ());
  repXZ_logical->SetVisAttributes(G4VisAttributes::Invisible);  

  // Build box XYZ of cubes

  G4VSolid *repXYZ_solid 
    = new G4Box(GetName()+"/"+GetNameRepXYZ(), 
		GetWidth()/2, 
		GetHeight()/2, 
		GetLength()/2); 
  G4LogicalVolume *repXYZ_logical 
    = new G4LogicalVolume(repXYZ_solid,
			  FindMaterial("Air"),
			  GetName()+"/"+GetNameRepXYZ());
  repXYZ_logical->SetVisAttributes(G4VisAttributes::Invisible);  

  //
  // Place the cubes:
  // 1) replica of cubes along Z --> single row
  // 2) replica of rows along X --> single layer
  // 3) replica of layers along Y --> SuperFGD
  //
  
  new G4PVReplica(GetName()+"/"+GetNameRepZ(),        // its name
		  cube_logical,    // its logical volume
		  repZ_logical, // its mother volume
		  kZAxis,          // axis along replication applied
		  fCubeNumZ,       // number of replicated volumes
		  cube.GetLength() // width of single replica along axis 
		  );

  new G4PVReplica(GetName()+"/"+GetNameRepXZ(),        // its name
		  repZ_logical,  // its logical volume
		  repXZ_logical, // its mother volume
		  kXAxis,           // axis along replication applied
		  fCubeNumX,        // number of replicated volumes
		  cube.GetBase()    // width of single replica along axis 
		  );

  new G4PVReplica(GetName()+"/"+GetNameRepXYZ(),        // its name
		  repXZ_logical,  // its logical volume
		  repXYZ_logical, // its mother volume
		  kYAxis,            // axis along replication applied
		  fCubeNumY,         // number of replicated volumes
		  cube.GetHeight()   // width of single replica along axis 
		  );

  return repXYZ_logical;
    

























  /*    
  // Place the plastic scintillator cubes    
  int CubeIdx = 0;
  for(int icubeX=0;icubeX<fCubeNumX;icubeX++){
  for(int icubeY=0;icubeY<fCubeNumY;icubeY++){
  for(int icubeZ=0;icubeZ<fCubeNumZ;icubeZ++){	
  double Xcube = GetPosX() -GetWidth()/2 + fEdge/2. + fEdge*icubeX;
  double Ycube = GetPosY() -GetHeight()/2 + fEdge/2. + fEdge*icubeY;
  double Zcube = GetPosZ() -GetLength()/2 + fEdge/2. + fEdge*icubeZ;	  	  
  new G4PVPlacement(
  0, // rotation
  //G4ThreeVector(0.0,0.0,Zcube), // position       
  G4ThreeVector(Xcube,Ycube,Zcube), // position       
  cube_logical, // logical volume
  cube.GetName(), // name       
  logVolume, // mother volume
  false,   // no boolean operations
  CubeIdx    // copy number
  );
  CubeIdx++;	  
  }
  }
  }
  //} // if fiber radius > hole radius
  logVolume->SetVisAttributes(G4VisAttributes::Invisible);  
  return logVolume;
  */    

}
