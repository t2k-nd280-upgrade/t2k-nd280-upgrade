#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4PVReplica.hh>

#include <G4Tubs.hh>

#include "ND280WAGASCIConstructor.hh"
#include "ND280CellConstructor.hh"

//
// Davide Sgalaberna 28/3/17
//
// Constructor of the WAGASCI detector
//

class ND280WAGASCIMessenger: public ND280ConstructorMessenger {
private:
  ND280WAGASCIConstructor* fConstructor;
  G4UIcmdWithADoubleAndUnit* fWidthCMD;
  G4UIcmdWithADoubleAndUnit* fHeightCMD;
  G4UIcmdWithADoubleAndUnit* fLengthCMD;
  
public:
    ND280WAGASCIMessenger(ND280WAGASCIConstructor* c) 
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

    virtual ~ND280WAGASCIMessenger() {
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

ND280WAGASCIConstructor::~ND280WAGASCIConstructor() {;}

void ND280WAGASCIConstructor::Init(void) {
  
  /// The edge of a cube of the WAGASCI detector 
  fEdge = 25*mm;

  // Number of cubes along each axis 
  fCubeNumX = 1;
  fCubeNumY = 1;
  fCubeNumZ = 1;

  SetWidth( fCubeNumX * fEdge);
  SetHeight(fCubeNumY * fEdge);
  SetLength(fCubeNumZ * fEdge);

  //SetFiberRadius(0.5*mm);
  //SetFiberMaterial("FiberCore");

  // Position of the center of the WAGASCI detector
  fPosX = 0.;
  fPosY = 0.;
  fPosZ = 0.;
  
  AddConstructor(new ND280CellConstructor("/cellDirXYZ/cellDirXZ/cellDirZ/Cell", this)); 
  
  SetMessenger(new ND280WAGASCIMessenger(this));
}

G4LogicalVolume *ND280WAGASCIConstructor::GetPiece(void) {

  ND280RootPersistencyManager* InputPersistencyManager
    = ND280RootPersistencyManager::GetInstance();
  fND280XMLInput = InputPersistencyManager->GetXMLInput();

  //
  
  G4LogicalVolume *logVolume
    = new G4LogicalVolume(new G4Box(GetName(),
				    GetWidth()/2.0,  
				    GetHeight()/2.0,
				    GetLength()/2.0),
			  FindMaterial("Air"),
			  GetName());
  
  // Build the plastic scintillator cell

  ND280CellConstructor& cell
    = Get<ND280CellConstructor>("/cellDirXYZ/cellDirXZ/cellDirZ/Cell");
  
  cell.SetHoleRadius(0.7*mm);
  cell.SetFiberRadius(0.5*mm);
  cell.SetCoatingThickness(0.25*mm);
  cell.SetScintThickness(3.0*mm);
  cell.SetGap(0.0*mm);
  cell.SetSpaceLayerY(1.*mm);

  cell.SetBase(fEdge);
  cell.SetLength(fEdge);
  cell.SetHeight(fEdge+
   		 cell.GetScintThickness()+
   		 cell.GetSpaceLayerY()); // height of 1 bar + horiz bar thick
  
  double shift = cell.GetHoleRadius();
  G4ThreeVector HolePosAlongX = G4ThreeVector(0.    , shift , shift); // hole along X
  G4ThreeVector HolePosAlongY = G4ThreeVector(shift , 0.    , -shift); // hole along Y
  G4ThreeVector HolePosAlongZ = G4ThreeVector(-shift, -shift, 0.    ); // hole along Z
  cell.SetHolePosition_X(HolePosAlongX); 
  cell.SetHolePosition_Y(HolePosAlongY); 
  cell.SetHolePosition_Z(HolePosAlongZ); 
    
  G4LogicalVolume* cell_logical = cell.GetPiece();


  // Set total size of the Super-FGD

  double TotWidth  = fCubeNumX * cell.GetBase(); 
  double TotHeight = fCubeNumY * cell.GetHeight(); 
  double TotLength = fCubeNumZ * cell.GetLength(); 
  SetWidth(TotWidth);
  SetLength(TotLength);
  SetHeight(TotHeight);

  
  //G4cout << "Cell base = " << cell.GetBase() << G4endl;
  //G4cout << " mass="<<cell_logical->GetMass()/kg   <<" kg" << G4endl;


  
  // Build bar Z of cells
  
  G4VSolid *cellDirZ_solid 
    = new G4Box(GetName()+"/cellDirZ_solid", 
		cell.GetBase()/2, 
		cell.GetHeight()/2, 
		GetLength()/2); 
  
  G4LogicalVolume *cellDirZ_logical 
    = new G4LogicalVolume(cellDirZ_solid,
			  FindMaterial("Air"),
			  GetName()+"/cellDirZ");
  
  cellDirZ_logical->SetVisAttributes(G4VisAttributes::Invisible);  
  
  G4PVReplica *cellDirZ
    = new G4PVReplica(GetName()+"/cellDirZ",    // its name
		      cell_logical, // its logical volume
		      cellDirZ_logical,          // its mother volume
		      kZAxis,             // axis along replication applied
		      fCubeNumZ,          // number of replicated volumes
		      cell.GetLength()    // width of single replica along axis 
		      );
  
  
  // Build layer XZ of cells
  
  G4VSolid *cellDirXZ_solid 
    = new G4Box(GetName()+"/cellDirXZ_solid", 
		GetWidth()/2, 
		cell.GetHeight()/2, 
		GetLength()/2); 
  
  G4LogicalVolume *cellDirXZ_logical 
    = new G4LogicalVolume(cellDirXZ_solid,
			  FindMaterial("Air"),
			  GetName()+"/cellDirXZ");

  cellDirXZ_logical->SetVisAttributes(G4VisAttributes::Invisible);  
  
  G4PVReplica *cellDirXZ
    = new G4PVReplica(GetName()+"/cellDirXZ",    // its name
		      cellDirZ_logical, // its logical volume
		      cellDirXZ_logical,          // its mother volume
		      kXAxis,             // axis along replication applied
		      fCubeNumX,          // number of replicated volumes
		      cell.GetBase()    // width of single replica along axis 
		      );
  
  
  // Build box XYZ of cells
  
  G4VSolid *cellDirXYZ_solid 
    = new G4Box(GetName()+"/cellDirXYZ_solid", 
		GetWidth()/2, 
		GetHeight()/2, 
		GetLength()/2); 
  
  G4LogicalVolume *cellDirXYZ_logical 
    = new G4LogicalVolume(cellDirXYZ_solid,
			  FindMaterial("Air"),
			  GetName()+"/cellDirXYZ");

  cellDirXYZ_logical->SetVisAttributes(G4VisAttributes::Invisible);  
  
  G4PVReplica *cellDirXYZ
    = new G4PVReplica(GetName()+"/cellDirXYZ",    // its name
		      cellDirXZ_logical, // its logical volume
		      cellDirXYZ_logical,          // its mother volume
		      kYAxis,             // axis along replication applied
		      fCubeNumY,          // number of replicated volumes
		      cell.GetHeight()    // width of single replica along axis 
		      );

  // new G4PVPlacement(
  // 		    0, // rotation
  // 		    G4ThreeVector(0.0,0.0,0.0), // position       
  // 		    cellDirXYZ_logical, // logical volume
  // 		    GetName(), // name       
  // 		    logVolume, // mother volume
  // 		    false,   // no boolean operations
  // 		    CubeIdx    // copy number
  // 		    );

 



  
  return cellDirXYZ_logical;
    




























  //

  /*
    

  */
  
  /*

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

    
    if( GetND280XML()->GetXMLInvisWAGASCI() ){
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


  */



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
