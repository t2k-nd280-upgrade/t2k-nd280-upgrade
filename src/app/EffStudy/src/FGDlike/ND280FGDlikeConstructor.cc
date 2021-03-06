#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4PVReplica.hh>

#include <G4Tubs.hh>

#include "ND280FGDlikeConstructor.hh"
#include "ND280BarScintConstructor.hh"

//
// Davide Sgalaberna 16/5/17
//
// Constructor of the FGDlike detector
//

using namespace conv;

class ND280FGDlikeMessenger: public ND280ConstructorMessenger {
private:
  ND280FGDlikeConstructor* fConstructor;
  G4UIcmdWithADoubleAndUnit* fWidthCMD;
  G4UIcmdWithADoubleAndUnit* fHeightCMD;
  G4UIcmdWithADoubleAndUnit* fLengthCMD;
  
public:
    ND280FGDlikeMessenger(ND280FGDlikeConstructor* c) 
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

    virtual ~ND280FGDlikeMessenger() {
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

ND280FGDlikeConstructor::~ND280FGDlikeConstructor() {;}

void ND280FGDlikeConstructor::Init(void) {
  
  /// The edge of a bar of the FGDlike detector 
  fEdge = 2.*CLHEP::mm;

  // Number of bars along each axis 
  fBarHorizNum = 1;
  fBarVertNum = 1;

  SetWidth( 100*CLHEP::mm);
  SetHeight(100*CLHEP::mm);  
  SetLength(1000*CLHEP::mm);

  // Position of the center of the FGDlike detector
  fPosX = 0.;
  fPosY = 0.;
  fPosZ = 0.;

  //
  // Note:
  // GetName() is the FGDlike name given in DetectorConstruction  
  // i.e. /t2k/OA/.../FGDlike
  //

  G4String nameRepLayer = "Layer"; // replica of layers along Z
  G4String nameRepBarVert = nameRepLayer +"/RepY"; // rep bar vert along Y
  G4String nameRepBarHoriz = nameRepLayer +"/RepX"; // rep bar horiz along X
  G4String nameBarHoriz = nameRepBarHoriz+"/FGDlikeHoriz";
  G4String nameBarVert = nameRepBarVert+"/FGDlikeVert";
  
  SetNameRepLayer(nameRepLayer);
  SetNameRepBarHoriz(nameRepBarHoriz);
  SetNameRepBarVert(nameRepBarVert);
  SetNameBarHoriz(nameBarHoriz);
  SetNameBarVert(nameBarHoriz);

  AddConstructor(new ND280BarScintConstructor(GetNameBarHoriz()+"/BarScintHoriz", this)); 
  AddConstructor(new ND280BarScintConstructor(GetNameBarVert()+"/BarScintVert", this)); 

  SetMessenger(new ND280FGDlikeMessenger(this));
}

G4LogicalVolume *ND280FGDlikeConstructor::GetPiece(void) {

  ND280RootPersistencyManager* InputPersistencyManager
    = ND280RootPersistencyManager::GetInstance();
  fND280XMLInput = InputPersistencyManager->GetXMLInput();

  // Set total size of the FGDlike

  double TotWidth  = fBarVertNum * fEdge; 
  SetBarHorizLength(TotWidth);
  SetWidth(TotWidth);

  double TotHeight = fBarHorizNum * fEdge;   
  SetBarVertLength(TotHeight);
  SetHeight(TotHeight);

  //G4cout << "width = " << GetWidth() << G4endl;
  //G4cout << "height = " << GetHeight() << G4endl;
  //G4cout << "edge = " << fEdge << G4endl;

  // Build the plastic scintillator bar Vertical

  //G4cout << "Vertical Bar" << G4endl;

  ND280BarScintConstructor& barVert
    = Get<ND280BarScintConstructor>(GetNameBarVert()+"/BarScintVert");
  barVert.SetBarDir(conv::kAlongY);
  barVert.SetBase( fEdge); // 
  barVert.SetAxisLength(TotHeight); // lengthVert based on # of bars along X 
  barVert.SetHoleRadius(0.7*CLHEP::mm);
  barVert.SetFiberRadius(0.5*CLHEP::mm);
  barVert.SetCoatingThickness(0.25*CLHEP::mm);
  barVert.SetGap(0.0*CLHEP::mm);
  G4LogicalVolume* barVert_logical = barVert.GetPiece();


  // Build the plastic scintillator bar Horizontal

  //G4cout << "Horizontal Bar" << G4endl;

  ND280BarScintConstructor& barHoriz
    = Get<ND280BarScintConstructor>(GetNameBarHoriz()+"/BarScintHoriz");
  barHoriz.SetBarDir(conv::kAlongX);
  barHoriz.SetBase( fEdge); // 
  barHoriz.SetAxisLength(TotWidth); // lengthVert based on # of bars along Y 
  barHoriz.SetHoleRadius(0.7*CLHEP::mm);
  barHoriz.SetFiberRadius(0.5*CLHEP::mm);
  barHoriz.SetCoatingThickness(0.25*CLHEP::mm);
  barHoriz.SetGap(0.0*CLHEP::mm);
  G4LogicalVolume* barHoriz_logical = barHoriz.GetPiece();

  //
  
  double LayerLength = barHoriz.GetLength() + barVert.GetLength();
  double NLayers = GetLayerNum();
  double TotLength = NLayers * LayerLength;
  SetLength(TotLength); // along Z


  // Build box N XY layers

  G4VSolid *repLayer_solid 
    = new G4Box(GetName()+"/"+GetNameRepLayer(), 
		GetWidth()/2, 
		GetHeight()/2, 
		GetLength()/2); 
  G4LogicalVolume *repLayer_logical 
    = new G4LogicalVolume(repLayer_solid,
			  FindMaterial("Air"),
			  GetName()+"/"+GetNameRepLayer());
  repLayer_logical->SetVisAttributes(G4VisAttributes::Invisible);  
  //repLayer_logical->SetVisAttributes(G4Colour(0.0,1.0,1.0)); //green
    

  // Plane of vert bars --> replica vert bars along X
  
  G4VSolid *repVert_solid 
    = new G4Box(GetName()+"/"+GetNameRepBarVert(), 
		GetWidth()/2, 
		GetHeight()/2, 
		barVert.GetBase()/2); 
  G4LogicalVolume *repVert_logical 
    = new G4LogicalVolume(repVert_solid,
			  FindMaterial("Air"),
			  GetName()+"/"+GetNameRepBarVert());
  repVert_logical->SetVisAttributes(G4VisAttributes::Invisible);  
  
  new G4PVReplica(GetName()+"/"+GetNameRepBarVert(),        // its name
		  barVert_logical,    // its logical volume
		  repVert_logical, // its mother volume
		  kXAxis,          // axis along replication applied
		  fBarVertNum,       // number of replicated volumes
		  barVert.GetBase() // width of single replica along axis 
		  );

  // Plane of horiz bars --> replica horiz bars along Y
  
  G4VSolid *repHoriz_solid 
    = new G4Box(GetName()+"/"+GetNameRepBarHoriz(), 
		GetWidth()/2, 
		//fBarHorizNum*barHoriz.GetBase()/2.,
		GetHeight()/2, 
		barHoriz.GetBase()/2); 
  G4LogicalVolume *repHoriz_logical 
    = new G4LogicalVolume(repHoriz_solid,
			  FindMaterial("Air"),
			  GetName()+"/"+GetNameRepBarHoriz());
  repHoriz_logical->SetVisAttributes(G4VisAttributes::Invisible);  
  
  new G4PVReplica(GetName()+"/"+GetNameRepBarHoriz(),        // its name
		  barHoriz_logical,    // its logical volume
		  repHoriz_logical, // its mother volume
		  kYAxis,          // axis along replication applied
		  fBarHorizNum,       // number of replicated volumes
		  barHoriz.GetBase() // width of single replica along axis 
		  );  

  // make a layer volume (X + Y plane) --> vert+horiz
  
  G4VSolid *layer_solid
    = new G4Box(GetName()+"/"+GetNameRepLayer(),
		GetWidth()/2,
		GetHeight()/2,
		LayerLength/2 
		);
  
  G4LogicalVolume *layer_logical 
    = new G4LogicalVolume(layer_solid,
			  FindMaterial("Air"),
			  GetName()+"/"+GetNameRepLayer());
  layer_logical->SetVisAttributes(G4VisAttributes::Invisible);
  //layer_logical->SetVisAttributes(G4Colour(1.0,1.0,0.0)); //yellow

  
  
  // place the horiz and vert planes inside the layer

  double posZ_horiz = -barHoriz.GetBase()/2.;
  double posZ_vert  = +barVert.GetBase()/2.;

  new G4PVPlacement(0,                   // no rotation
		    G4ThreeVector(0,0,posZ_vert),     // position
		    repVert_logical,         // its logical volume
		    GetName()+"/"+GetNameRepBarVert(),   // its name
		    layer_logical,     // its mother  volume
		    false,               // no boolean operations
		    0);                  // copy number
 
  new G4PVPlacement(0,                   // no rotation
		    G4ThreeVector(0,0,posZ_horiz),     // position
		    repHoriz_logical,         // its logical volume
		    GetName()+"/"+GetNameRepBarHoriz(),   // its name
		    layer_logical,     // its mother  volume
		    false,               // no boolean operations
		    0);                  // copy number
  

  //G4cout << "GetLength() = " << GetLength() << G4endl;
  //G4cout << "LayerLength = " << LayerLength << G4endl;
  //G4cout << "NLayers = " << NLayers << G4endl;

  
  // make a target volume (all layers) --> replica of layers
  
  new G4PVReplica(GetName()+"/"+GetNameRepLayer(),        // its name
		  layer_logical,    // its logical volume
		  repLayer_logical, // its mother volume
		  kZAxis,          // axis along replication applied
		  NLayers,       // number of replicated volumes
		  LayerLength // width of single replica along axis 
		  );
 
  return repLayer_logical;
}
