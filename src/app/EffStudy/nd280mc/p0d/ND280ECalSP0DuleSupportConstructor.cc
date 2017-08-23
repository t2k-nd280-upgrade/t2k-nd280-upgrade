#include <globals.hh>

#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>

//#include "ND280Log.hh"

#include "p0d/ND280ECalSP0DuleSupportConstructor.hh"
#include "ND280BeamConstructor.hh"

class ND280ECalSP0DuleSupportMessenger: public ND280ConstructorMessenger {
private:
    ND280ECalSP0DuleSupportConstructor* fConstructor;
    G4UIcmdWithADoubleAndUnit* fWidthCMD;
    G4UIcmdWithADoubleAndUnit* fHeightCMD;
    G4UIcmdWithADoubleAndUnit* fLengthCMD;

public:
    ND280ECalSP0DuleSupportMessenger(ND280ECalSP0DuleSupportConstructor* c)
        : ND280ConstructorMessenger(c,"Control the ECal SP0Dule Support geometry."),
          fConstructor(c) {
        fWidthCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("Width"),this);
        fWidthCMD->SetGuidance("Set the width of the support structure.");
        fWidthCMD->SetParameterName("Width",false);
        fWidthCMD->SetUnitCategory("Length");
        
        fHeightCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("Height"),this);
        fHeightCMD->SetGuidance("Set the height of the support structure.");
        fHeightCMD->SetParameterName("Height",false);
        fHeightCMD->SetUnitCategory("Length");
        
        fLengthCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("Length"),this);
        fLengthCMD->SetGuidance("Set length of the support structure.");
        fLengthCMD->SetParameterName("Length",false);
        fLengthCMD->SetUnitCategory("Length");
        
    };
    
    virtual ~ND280ECalSP0DuleSupportMessenger() {
        delete fWidthCMD;
        delete fHeightCMD;
        delete fLengthCMD;

    };
    
    void SetNewValue(G4UIcommand *cmd, G4String val) {
        if (cmd==fWidthCMD) {
            fConstructor->SetWidth(
                fWidthCMD->GetNewDoubleValue(val));
        }
        else if (cmd==fHeightCMD) {
            fConstructor->SetHeight(
                fHeightCMD->GetNewDoubleValue(val));
        }
        else if (cmd==fLengthCMD) {
            fConstructor->SetLength(
                fLengthCMD->GetNewDoubleValue(val));
        }
        else {
            ND280ConstructorMessenger::SetNewValue(cmd,val);
        }
    };
};

ND280ECalSP0DuleSupportConstructor::~ND280ECalSP0DuleSupportConstructor() {;}

void ND280ECalSP0DuleSupportConstructor::Init(void) {

  fHeight  = 146.05*CLHEP::mm;
  fWidth   = 304.8*CLHEP::mm;
  fLength  = 2320*CLHEP::mm;
    
  fTopSupportHeight = 19.05*CLHEP::mm;  //.75 in

  fSideThickness = 15.748*CLHEP::mm;   // .62 in
    
    /*
    fTopSupportWidth  = 76.2*mm;   // 3  in
    fTopSupportLength = fWidth;
    
    fTopThickness = 8.89*mm;   // .35 in
    fCChannelWidth = fWidth;
    fCChannelHeight = 127*mm;
    fCChannelLength = fLength;
    
    fRibWidth = 19.05*mm; // .75 in
    fRibHeight = 76.2*mm; // 3 in
    fRibLength = fWidth - 2*fSideThickness;
    
    fStripHeight  = 8*mm;
    fStripWidth   = 25.4*mm;
    fStripLength  = fLength;
    */
    
    fvisual = new G4VisAttributes();
    fvisual->SetColor(0.5,0.5,0.5,1); // Grey
    
    AddConstructor(new ND280BeamConstructor("Beam",this));

    SetMessenger(new ND280ECalSP0DuleSupportMessenger(this));
}

void ND280ECalSP0DuleSupportConstructor::SetVisAttributes(G4LogicalVolume* logVolume) {
    if (GetVisible()) {
        logVolume->SetVisAttributes(fvisual);
    } else {
        logVolume->SetVisAttributes(G4VisAttributes::Invisible);
    }
}

G4LogicalVolume *ND280ECalSP0DuleSupportConstructor::GetPiece(void) {

    G4LogicalVolume *logVolume
        = new G4LogicalVolume(new G4Box(GetName(),
                                        GetWidth()/2,
                                        GetHeight()/2,
                                        GetLength()/2),
                              FindMaterial("Air"),
                              GetName());

    SetVisAttributes(logVolume);
    
    //////////////////////////////////////////
    // CONSTRUCT THE SUPPORT STRUCTURE
    //////////////////////////////////////////

    ND280BeamConstructor& beam 
      = Get<ND280BeamConstructor>("Beam"); // Default aluminum

    // Construct the side beams in the C-Channel (1)

    G4double beamWidth  = GetSideThickness();
    G4double beamHeight = GetHeight()-GetTopSupportHeight();
    G4double beamLength = GetLength();
    beam.SetWidth(beamWidth);
    beam.SetHeight(beamHeight);
    beam.SetLength(beamLength);

    G4RotationMatrix* rotP90Y = new G4RotationMatrix();
    rotP90Y->rotateY(90*CLHEP::degree);
    
    new G4PVPlacement(0,          //rotation
		      G4ThreeVector( -(GetWidth()/2-beamWidth/2),
				     -(GetHeight()/2-beamHeight/2),
				     0),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #
    
    new G4PVPlacement(0,          //rotation
		      G4ThreeVector( (GetWidth()/2-beamWidth/2),
				     -(GetHeight()/2-beamHeight/2),
				     0),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    // Construct the 8 support ribs inside the C-Channel

    beamWidth = GetTopSupportHeight();
    beamHeight = 110.5*CLHEP::mm;
    beamLength = GetWidth()-2*GetSideThickness();

    beam.SetWidth(beamWidth);
    beam.SetHeight(beamHeight);
    beam.SetLength(beamLength);

    const G4double ribSpacing = (GetLength() - 8*beamWidth)/7;

    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-beamHeight/2-GetTopSupportHeight()),
				     -(7*ribSpacing/2 +7*beamWidth/2)),
		      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #


    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-beamHeight/2-GetTopSupportHeight()),
				     -(5*ribSpacing/2 + 5*beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #


    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-beamHeight/2-GetTopSupportHeight()),
				     -(3*ribSpacing/2 + 3*beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #


    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-beamHeight/2-GetTopSupportHeight()),
				     -(ribSpacing/2 + beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #


    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-beamHeight/2-GetTopSupportHeight()),
				     (ribSpacing/2 + beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #


    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-beamHeight/2-GetTopSupportHeight()),
				     (3*ribSpacing/2 + 3*beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #


    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-beamHeight/2-GetTopSupportHeight()),
				     (5*ribSpacing/2 + 5*beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #


    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-beamHeight/2-GetTopSupportHeight()),
				     (7*ribSpacing/2 + 7*beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #
		      
    
    //Construct the aluminum strips that connect the ribs

    beamWidth  = 25.4*CLHEP::mm;
    beamHeight = 6.35*CLHEP::mm;
    beamLength = GetLength();

    beam.SetWidth(beamWidth);
    beam.SetHeight(beamHeight);
    beam.SetLength(beamLength);

    const G4double stripSpacing = (GetWidth()-2*GetSideThickness()-3*beamWidth)/5;

    new G4PVPlacement(0,          //rotation
		      G4ThreeVector( -(stripSpacing + beamWidth),
				     (GetHeight()/2-beamHeight/2-GetTopSupportHeight()-110.5*CLHEP::mm),
				     0),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #
		      

    new G4PVPlacement(0,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-beamHeight/2-GetTopSupportHeight()-110.5*CLHEP::mm),
				     0),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #
    

    new G4PVPlacement(0,          //rotation
		      G4ThreeVector( (stripSpacing + beamWidth),
				     (GetHeight()/2-beamHeight/2-GetTopSupportHeight()-110.5*CLHEP::mm),
				     0),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #
		      
    //Construct the top supports that the SP0Dule rests on

    beamWidth = 76.2*CLHEP::mm;
    beamHeight = GetTopSupportHeight();
    beamLength = GetWidth();

    beam.SetWidth(beamWidth);
    beam.SetHeight(beamHeight);
    beam.SetLength(beamLength);

    const G4double supportSpacing = (GetLength() - 8*beamWidth)/7;

    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-beamHeight/2),
				     -(7*supportSpacing/2 +7*beamWidth/2)),
		      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #


    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-beamHeight/2),
				     -(5*supportSpacing/2 + 5*beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #


    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-beamHeight/2),
				     -(3*supportSpacing/2 + 3*beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #


    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-beamHeight/2),
				     -(supportSpacing/2 + beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #


    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-beamHeight/2),
				     (supportSpacing/2 + beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #


    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-beamHeight/2),
				     (3*supportSpacing/2 + 3*beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #


    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-beamHeight/2),
				     (5*supportSpacing/2 + 5*beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #


    new G4PVPlacement(rotP90Y,          //rotation
		      G4ThreeVector( 0,
				     (GetHeight()/2-beamHeight/2),
				     (7*supportSpacing/2 + 7*beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #
		      
    return logVolume;
}
