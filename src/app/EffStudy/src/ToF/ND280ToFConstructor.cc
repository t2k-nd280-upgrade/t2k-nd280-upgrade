#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>

#include "ND280ToFConstructor.hh"
#include "ND280ToFScintXConstructor.hh"
#include "ND280ToFScintYConstructor.hh"

//
// Davide Sgalaberna 26/2/17
//
// Constructor of the ToF detector
// By defaul is constructed on the X-Y plane
//

class ND280ToFMessenger: public ND280ConstructorMessenger {
private:
  ND280ToFConstructor* fConstructor;
  G4UIcmdWithADoubleAndUnit* fWidthCMD;
  G4UIcmdWithADoubleAndUnit* fHeightCMD;
  G4UIcmdWithADoubleAndUnit* fLengthCMD;
  
public:
    ND280ToFMessenger(ND280ToFConstructor* c) 
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

    virtual ~ND280ToFMessenger() {
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

ND280ToFConstructor::~ND280ToFConstructor() {;}

void ND280ToFConstructor::Init(void) {
  fWidth = 1.0*CLHEP::meter;
  fHeight = 1.0*CLHEP::cm;
  fLength = 1.0*CLHEP::meter;

  fPlaneXYNum = 1;

  fBarWidth = 1.0*CLHEP::cm;
  fBarHeight = 1.0*CLHEP::cm;
  fBarHorizLength = fWidth;
  fBarVertLength = fHeight;

  fPosX = 0.;
  fPosY = 0.;
  fPosZ = 0.;
  
  AddConstructor(new ND280ToFScintYConstructor("ScintHoriz", this));
  AddConstructor(new ND280ToFScintXConstructor("ScintVert", this)); 
    
  SetMessenger(new ND280ToFMessenger(this));
}

G4LogicalVolume *ND280ToFConstructor::GetPiece(void) {

  double bar_width  = GetBarWidth();
  double bar_height = GetBarHeight();
  double bar_length = GetBarLength();
  
  //double barvert_length = n_bars_horiz * bar_height;
  //SetBarVertLength(barvert_length);

  //double barhoriz_length = n_bars_vert * bar_width;
  //SetBarHorizLength(barhoriz_length);

  int NBars = GetPlaneXYNum();
  
  double TotWidth = bar_width; 
  double TotHeight = bar_height * NBars;
  double TotLength = bar_length;

  SetWidth(TotWidth);
  SetLength(TotLength);
  SetHeight(TotHeight);

  G4LogicalVolume *logVolume
	= new G4LogicalVolume(new G4Box(GetName(),
                                        GetWidth()/2.0,  
                                        GetHeight()/2.0,
                                        GetLength()/2.0),
                              FindMaterial("Air"),
                              GetName());
 
  // Horizontal Scintillator Layer (nd280mc convention)
  ND280ToFScintYConstructor& layer_horiz
    = Get<ND280ToFScintYConstructor>("ScintHoriz");
  layer_horiz.SetUp(NBars,
  		    bar_length,
  		    bar_width); // nBars, barLength, barWidth       

  G4LogicalVolume* layer_horiz_logical = layer_horiz.GetPiece();

  /*
  // Vertical Scintillator Layer (nd280mc convention)
  ND280ToFScintXConstructor& layer_vert
    = Get<ND280ToFScintXConstructor>("ScintVert");

  layer_vert.SetUp(n_bars_vert,
                    barvert_length,
                    bar_width); // nBars, barLength, barWidth
  G4LogicalVolume* layer_vert_logical = layer_vert.GetPiece();
  */
  /*
  G4cout << "logVolume: " << GetWidth() << ", " << GetHeight() << ", " << GetLength() << G4endl;
  G4cout << "layer_vert.GetWidth() = " << layer_vert.GetWidth() << G4endl;
  G4cout << "layer_vert.GetHeight() = " << layer_vert.GetHeight() << G4endl;
  G4cout << "layer_vert.GetLength() = " << layer_vert.GetLength() << G4endl;
  G4cout << "layer_horiz.GetWidth() = " << layer_horiz.GetWidth() << G4endl;
  G4cout << "layer_horiz.GetHeight() = " << layer_horiz.GetHeight() << G4endl;
  G4cout << "layer_horiz.GetLength() = " << layer_horiz.GetLength() << G4endl;
  G4cout << "bar_width = " << bar_width << G4endl;
  G4cout << "barhoriz_length = " << barhoriz_length << G4endl;
  G4cout << "barvert_length = " << barvert_length << G4endl;
  G4cout << "n_bars_horiz = " << n_bars_horiz << G4endl;
  G4cout << "n_bars_vert = " << n_bars_vert << G4endl;
  */

  
  // Placements

  new G4PVPlacement(
    0, // rotation
    G4ThreeVector(0.0,0.0,0.0), // position
    layer_vert_logical, // logical volume      
    layer_vert.GetName(), // name
    logVolume, // mother volume         
    false,   // no boolean operations
    0    // copy number
  );
  
  //double z_pos = -(GetLength() / 2.0) + (bar_width / 2.); // bars are in XY plane, i.e. width is along Z
  //double z_pos = -fPlaneXYNum * bar_width / 2.;
  //double delta_z1 = bar_height; 
  
  //int n_x = 0;
  //int n_y = 0;
  /*
  for(int i = 0; i != fPlaneXYNum*2.; ++i) { // loop over the # of layers (i.e. # of planes / 2)
    
    G4cout << "z_pos = " << z_pos << G4endl;
    
    if( (i % 2) == 0.0 ) {
      
      new G4PVPlacement(
			0, // rotation
			G4ThreeVector(0.0,0.0,z_pos), // position
			layer_vert_logical, // logical volume      
			layer_vert.GetName(), // name
			logVolume, // mother volume         
			false,   // no boolean operations
			n_y    // copy number
			);
      ++n_y;
      z_pos += bar_height;   
    }        
    else {
      
      new G4PVPlacement(
			0, // rotation
			G4ThreeVector(0.0,0.0,z_pos), // position       
			layer_horiz_logical, // logical volume
			layer_horiz.GetName(), // name       
			logVolume, // mother volume
			false,   // no boolean operations
			n_x    // copy number
			);
      ++n_x;      
      z_pos += bar_width;
    }
  }
  */
  
  /*
    if (GetVisible()) {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(1.0,0.,0.); // red
    visual->SetForceWireframe(true);
    logVolume->SetVisAttributes(visual);
    } 
    else {
    logVolume->SetVisAttributes(G4VisAttributes::Invisible);
    }
  */

  logVolume->SetVisAttributes(G4VisAttributes::Invisible);  
  
  return logVolume;
}
