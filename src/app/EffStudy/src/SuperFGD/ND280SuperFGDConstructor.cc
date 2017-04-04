#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>

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

  SetFiberRadius(0.5*mm);
  SetFiberMaterial("FiberCore");

  // Position of the center of the SuperFGD detector
  fPosX = 0.;
  fPosY = 0.;
  fPosZ = 0.;
  
  AddConstructor(new ND280CubeScintConstructor("CubeScint", this)); 
  
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

  //
  
  G4LogicalVolume *logVolume
    = new G4LogicalVolume(new G4Box(GetName(),
				    GetWidth()/2.0,  
				    GetHeight()/2.0,
				    GetLength()/2.0),
			  FindMaterial("Air"),
			  GetName());

  // Build the plastic scintillator cube

  ND280CubeScintConstructor& cube
    = Get<ND280CubeScintConstructor>("CubeScint");
  
  cube.SetBase(fEdge);
  cube.SetLength(fEdge);
  cube.SetHeight(fEdge);  
  cube.SetHoleRadius(0.7*mm);
  cube.SetCoatingThickness(0.25*mm);
  cube.SetGap(0.0*mm);
  
  double shift = cube.GetHoleRadius();
  G4ThreeVector HolePosAlongX = G4ThreeVector(0.    , shift , shift); // hole along X
  G4ThreeVector HolePosAlongY = G4ThreeVector(shift , 0.    , -shift); // hole along Y
  G4ThreeVector HolePosAlongZ = G4ThreeVector(-shift, -shift, 0.    ); // hole along Z
  cube.SetHolePosition_X(HolePosAlongX); 
  cube.SetHolePosition_Y(HolePosAlongY); 
  cube.SetHolePosition_Z(HolePosAlongZ); 

  G4LogicalVolume* cube_logical = cube.GetPiece();
  
  //G4cout << "Cube base = " << cube.GetBase() << G4endl;
  //G4cout << cube_logical->GetMaterial()->GetName() << G4endl; 
  //G4cout << " mass="<<cube_logical->GetMass()/kg   <<" kg" << G4endl;
    
  // Build the fibers

  G4VisAttributes *visAtt_Fiber = new G4VisAttributes();
  visAtt_Fiber->SetColor(0.0,1.0,0.0); // green
  visAtt_Fiber->SetForceSolid(true);

  if (GetFiberRadius()>cube.GetHoleRadius()) { 
    G4Exception("ND280CubeScintConstructor::GetPiece",
  		"MyCode0002",FatalException,
  		"Fiber radius > Hole radius !");
  }
  else{
    
    G4VSolid* fiberAlongX = new G4Tubs(GetName()+"/FiberX",
				       0.0*cm,
				       GetFiberRadius(),
				       GetWidth()/2,
				       0.*deg,
				       360.*deg);

    G4VSolid* fiberAlongY = new G4Tubs(GetName()+"/FiberY",
				       0.0*cm,
				       GetFiberRadius(),
				       GetHeight()/2,
				       0.*deg,
				       360.*deg);

    G4VSolid* fiberAlongZ = new G4Tubs(GetName()+"/FiberZ",
				       0.0*cm,
				       GetFiberRadius(),
				       GetLength()/2,
				       0.*deg,
				       360.*deg);
    
    G4LogicalVolume *fiberAlongXVolume
      = new G4LogicalVolume(fiberAlongX,
			    FindMaterial(GetFiberMaterial()),
			    GetName()+"/FiberX");

    G4LogicalVolume *fiberAlongYVolume
      = new G4LogicalVolume(fiberAlongY,
			    FindMaterial(GetFiberMaterial()),
			    GetName()+"/FiberY");

    G4LogicalVolume *fiberAlongZVolume
      = new G4LogicalVolume(fiberAlongZ,
			    FindMaterial(GetFiberMaterial()),
			    GetName()+"/FiberZ");

    
    if( GetND280XML()->GetXMLInvisSuperFGD() ){
      fiberAlongXVolume->SetVisAttributes(G4VisAttributes::Invisible);
      fiberAlongYVolume->SetVisAttributes(G4VisAttributes::Invisible);
      fiberAlongZVolume->SetVisAttributes(G4VisAttributes::Invisible);
    }
    else{
      fiberAlongXVolume->SetVisAttributes(visAtt_Fiber);
      fiberAlongYVolume->SetVisAttributes(visAtt_Fiber);
      fiberAlongZVolume->SetVisAttributes(visAtt_Fiber);
    }

    // Place the fibers along Z --> loop in the XY plane   
    
    int FiberIdxZ=0;

    for(int iFiberY=0;iFiberY<fCubeNumY;iFiberY++){
      for(int iFiberX=0;iFiberX<fCubeNumX;iFiberX++){
	
	// The fiber position is shifted with respect to the cube center (along Z)

	double Xcube = GetPosX() -GetWidth()/2. + fEdge/2. + fEdge*iFiberX;
	double Ycube = GetPosY() -GetHeight()/2. + fEdge/2. + fEdge*iFiberY;
	double Zcube = GetPosZ();
	
	double Xfiber = Xcube + HolePosAlongZ.x();
	double Yfiber = Ycube + HolePosAlongZ.y();
	double Zfiber = Zcube + HolePosAlongZ.z();
	
	new G4PVPlacement(0,                  // no rotation         
			  G4ThreeVector(Xfiber,Yfiber,Zfiber), 
			  fiberAlongZVolume,         // its logical volume
			  GetName()+"/FiberZ",  // its name
			  logVolume,          // its mother  volume
			  false,               // no boolean operations
			  FiberIdxZ);           // copy number
	
	FiberIdxZ++;
	
      } // end loop over X fibers
    } // end loop over Y fibers

	
    // Place the fibers along X --> loop in the YZ plane   

    int FiberIdxX=0;

    for(int iFiberY=0;iFiberY<fCubeNumY;iFiberY++){
      for(int iFiberZ=0;iFiberZ<fCubeNumZ;iFiberZ++){
	
	G4RotationMatrix* rotationAlongX = new G4RotationMatrix();
	rotationAlongX->rotateX(0  *degree); 
	rotationAlongX->rotateY(90 *degree); 
	rotationAlongX->rotateZ(0  *degree);      

	// The fiber position is shifted with respect to the cube center (along X)

	double Xcube = GetPosX();
	double Ycube = GetPosY() -GetHeight()/2. + fEdge/2. + fEdge*iFiberY;
	double Zcube = GetPosZ() -GetLength()/2. + fEdge/2. + fEdge*iFiberZ;
	
	double Xfiber = Xcube + HolePosAlongX.x();
	double Yfiber = Ycube + HolePosAlongX.y();
	double Zfiber = Zcube + HolePosAlongX.z();
	
	new G4PVPlacement(rotationAlongX,                           
			  G4ThreeVector(Xfiber,Yfiber,Zfiber), 
			  fiberAlongXVolume,         // its logical volume
			  GetName()+"/FiberX",  // its name
			  logVolume,          // its mother  volume
			  false,               // no boolean operations
			  FiberIdxX);                  // copy number
	
	FiberIdxX++;

      } // end loop over Y fibers
    } // end loop over Z fibers


    // Place the fibers along Y --> loop in the XZ plane   

    int FiberIdxY=0;

    for(int iFiberX=0;iFiberX<fCubeNumX;iFiberX++){
      for(int iFiberZ=0;iFiberZ<fCubeNumZ;iFiberZ++){
	
	G4RotationMatrix* rotationAlongY = new G4RotationMatrix();
	rotationAlongY->rotateX(90 *degree); 
	rotationAlongY->rotateY(0  *degree); 
	rotationAlongY->rotateZ(0  *degree);      

	// The fiber position is shifted with respect to the cube center (along X)

	double Xcube = GetPosX() -GetWidth()/2. + fEdge/2. + fEdge*iFiberX;
	double Ycube = GetPosY();
	double Zcube = GetPosZ() -GetLength()/2. + fEdge/2. + fEdge*iFiberZ;
	
	double Xfiber = Xcube + HolePosAlongY.x();
	double Yfiber = Ycube + HolePosAlongY.y();
	double Zfiber = Zcube + HolePosAlongY.z();
	
     	new G4PVPlacement(rotationAlongY,                           
			  G4ThreeVector(Xfiber,Yfiber,Zfiber), 
			  fiberAlongYVolume,         // its logical volume
			  GetName()+"/FiberY",  // its name
			  logVolume,          // its mother  volume
			  false,               // no boolean operations
			  FiberIdxY);          // copy number

	FiberIdxY++;
	
      } // end loop over X fibers
    } // end loop over Z fibers


    
    
    
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
  
  }

  logVolume->SetVisAttributes(G4VisAttributes::Invisible);  
  
  return logVolume;
}
