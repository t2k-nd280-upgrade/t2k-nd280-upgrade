
// 
// TODO:
// - check hard-coded numbers in GetP0DServiceLength
//   taken from nd280mc (methods and sub-methods...)
//

#include <globals.hh>

//#include <TND280Log.hxx>
//#include <G4TwoVector.hh>

#include <G4UnionSolid.hh>
#include <G4Box.hh>
#include <G4Trd.hh>
#include <G4Tubs.hh>
#include <G4Trap.hh>
#include <G4Polyhedra.hh>

#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4RotationMatrix.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>

#include <G4PhysicalConstants.hh>

//#include "ND280Log.hh"

#include "ND280BasketConstructor.hh"
#include "ND280IBeamConstructor.hh"
#include "ND280StructuralBeamConstructor.hh"
//#include "ND280FGDMiniCrateConstructor.hh"

//#include "p0d/ND280P0DConstructor.hh"
//#include "p0d/ND280P0DSupportConstructor.hh"
//#include "ND280TrackerConstructor.hh"
#include "ecal/ND280DsECalConstructor.hh"

class ND280BasketMessenger: public ND280ConstructorMessenger {
private:
    ND280BasketConstructor* fConstructor;
     
    /// Commands to transform downstream ECal
    /// These are deprecated. Use the DSECal's own commands instead.
    G4UIcmdWith3VectorAndUnit* fDsECalTransCMD;
    G4UIcmdWith3Vector* fDsECalRotAxisCMD;
    G4UIcmdWithADoubleAndUnit* fDsECalRotAngCMD;
    
public:

    ND280BasketMessenger(ND280BasketConstructor* c)
        : ND280ConstructorMessenger(c,"Control the basket geometry."),
          fConstructor(c) {
	
	fDsECalTransCMD = new G4UIcmdWith3VectorAndUnit(
            CommandName("SetDsECalTranslation"),this);
	fDsECalTransCMD->SetParameterName("dx", "dy", "dz", false);
	fDsECalTransCMD->SetUnitCategory("Length");
	fDsECalTransCMD->SetGuidance("DEPRECATED. Use /Basket/DsECal commands instead.");
	fDsECalTransCMD->SetGuidance(
            "Usage: /t2k/OA/Magnet/Basket/SetDsECalTranslation "
            "[dx] [dy] [dz] [unit]");
	fDsECalTransCMD->SetGuidance(
            "  Translate downstream ECal in local basket coordinates.  This ");
	fDsECalTransCMD->SetGuidance(
            " is intended to allow test of the alignment system.");
	
        fDsECalRotAxisCMD = new G4UIcmdWith3Vector(
            CommandName("SetDsECalRotationAxis"),this);
        fDsECalRotAxisCMD->SetParameterName("ux", "uy", "uz", false);
        fDsECalTransCMD->SetGuidance("DEPRECATED. Use /Basket/DsECal commands instead.");
	fDsECalRotAxisCMD->SetGuidance(
            "Usage: /t2k/OA/Magnet/Basket/SetDsECalRotationAxis "
            "[ux] [uy] [uz]");
        fDsECalRotAxisCMD->SetGuidance(
            "  Set rotation axis for the DsECal in local basket coordinates.");
        fDsECalRotAxisCMD->SetGuidance(
            "  The ux, uy and uz are the components of a unit vector ");
        fDsECalRotAxisCMD->SetGuidance(
            "  the DsECal will be rotated about.  This is intended to allow ");
	fDsECalRotAxisCMD->SetGuidance(
            "  test of the alignment system.");
	
	fDsECalRotAngCMD = new G4UIcmdWithADoubleAndUnit(
            CommandName("SetDsECalRotationAngle"),this);
	fDsECalRotAngCMD->SetParameterName("theta", false);
	fDsECalRotAngCMD->SetUnitCategory("Angle");
	fDsECalRotAngCMD->SetDefaultValue(0.0);
    fDsECalTransCMD->SetGuidance("DEPRECATED. Use /Basket/DsECal commands instead.");
	fDsECalRotAngCMD->SetGuidance(
            "Usage: /t2k/OA/Magnet/Basket/SetDsECalRotationAngle "
            "[theta] [unit]");
        fDsECalRotAngCMD->SetGuidance(
            "Set rotation angle around the axis for downstream ECal.");
    }

    virtual ~ND280BasketMessenger() {
	delete fDsECalTransCMD;
	delete fDsECalRotAxisCMD;
	delete fDsECalRotAngCMD;
    }
    
    void SetNewValue(G4UIcommand *cmd, G4String val) {
        if (cmd==fDsECalTransCMD) {
            fConstructor->SetDsECalTrans(
                fDsECalTransCMD->GetNew3VectorValue(val));
        }
        else if (cmd==fDsECalRotAxisCMD) {
            fConstructor->SetDsECalRotAxis(
                fDsECalRotAxisCMD->GetNew3VectorValue(val));
        }
        else if (cmd==fDsECalRotAngCMD) {
            fConstructor->SetDsECalRotAng(
                fDsECalRotAngCMD->GetNewDoubleValue(val));
        }
        else {
            ND280ConstructorMessenger::SetNewValue(cmd,val);
        }
    }
};

ND280BasketConstructor::~ND280BasketConstructor() { }

void ND280BasketConstructor::Init(void) {
  
    ////////
    //
    // NEW ND280UPGRADE
    //
    
    // Take XML inputs
    ND280RootPersistencyManager* InputPersistencyManager
      = ND280RootPersistencyManager::GetInstance();
    fND280XMLInput = InputPersistencyManager->GetXMLInput();  

    // Set visible / invisible Basket
    if( GetND280XML()->GetXMLInvisBasket() ) SetVisible(false);
    else SetVisible(true);

    ////////
    
    SetWidth(GetOuterWidth());
    SetHeight(GetOuterHeight());
    SetLength(GetOuterLength());
    
    // Calculated later from P0DConstructor.
    fP0DPosition = 990*meter;  
    
    // Offset taken from design measurements in unversioned copy of
    // ND280.org/integration/survey/measurements/"FGD and TPC Surveying-2.dwg"
    // provided by D. Karlen.  This is the center of TPC1.
    fTrackerPosition = -0.5*GetInnerLength() + 4286.0*mm;

    fDsECalPosition = 0.0;
    fDsECalVerticalPosition = 0.0;
    fDsECalRotAng = 0.0;
    fDsECalRotAxis = G4ThreeVector(0.0);


    SetFGD1Position(0.);
    SetFGD2Position(0.);

    fvisual = new G4VisAttributes();
    fvisual->SetColor(0.5,0.1,0.1,1); // dark red
    
    AddConstructor(new ND280StructuralBeamConstructor("Beam",this));
    AddConstructor(new ND280IBeamConstructor("Rail",this));
    // AddConstructor(new ND280P0DConstructor("P0D",this));
    // AddConstructor(new ND280P0DSupportConstructor("P0DSupport",this));

    if( GetND280XML()->GetXMLUseDsECal() ){        
      AddConstructor(new ND280DsECalConstructor("DsECal",this));
    }
    
    // AddConstructor(new ND280TrackerConstructor("Tracker",this));
    // AddConstructor(new ND280FGDMiniCrateConstructor("MiniCrate",this));
    SetMessenger(new ND280BasketMessenger(this));
}

void ND280BasketConstructor::SetVisAttributes(G4LogicalVolume* logVolume) {
  if (GetVisible()) {
    logVolume->SetVisAttributes(fvisual);
  } 
  else {
    logVolume->SetVisAttributes(G4VisAttributes::Invisible);
  }
}


double ND280BasketConstructor::GetP0DServiceLength() {
  //
  // nd280mc CODE
  //
  //ND280P0DConstructor& p0d = Get<ND280P0DConstructor>("P0D");
  //return p0d.GetUpstreamSpace()+p0d.GetLength();
  //

  //
  // TODO: CHECK IF THE FOLLOWING HARD-CODE NUMBERS ARE CORRECT!!!
  //
  //
  // Use the code in the method as defined in /p0d/ND280P0DConstructor.cc
  //
  // double ND280P0DConstructor::GetLength(void) {
  // double length = fCECalDsLIPosition;
  // length += std::max(0.0,
  //     Get<ND280CECalSP0DuleConstructor>("CECal").GetECalBraceThickness());
  // length += std::max(0.0,std::abs(fCECalSkewAngle)*GetWidth());
  // length -= fUSECalDsLIPosition;
  // length += GetUSECalLength();
  // length += std::max(0.0,std::abs(fUSECalSkewAngle)*GetWidth());
  // length += GetLengthAdjustmentSpace();
  // return length;
  //
  // By looking to definitions of each length in the nd280mc code (hard-coded) I get the following:
  // (all the numbers are taken from /p0D/ND280P0DConstructor.cc except where explicitly writte)
  //

  double fCECalDsLIPosition = 2396*mm;
  
  // taken from p0d/ND280CECalSP0DuleConstructor.hh 
  double fECalBraceThickness = 0.75*25.4*mm; 
  
  double fCECalSkewAngle = 3.39*mrad;

  //
  // Taken from function below
  //
  // double ND280P0DConstructor::GetWidth(void) {
  //   double width = 0;
  //   width = std::max(
  //       width,Get<ND280USECalSP0DuleConstructor>("USECal").GetWidth());
  //   width = std::max(
  //       width,Get<ND280USTargetSP0DuleConstructor>("USTarget").GetWidth());
  //   width = std::max(
  //       width,Get<ND280CTargetSP0DuleConstructor>("CTarget").GetWidth());
  //   width = std::max(
  //       width,Get<ND280CECalSP0DuleConstructor>("CECal").GetWidth());
  //   // Allow a small amount of space for adjustment.
  //   width += GetWidthAdjustmentSpace(); 
  //   return width;
  // }
  //
  double width = 0.; 
  width = std::max(width,2300.8*mm); // from ND280USECalSP0DuleConstructor::
  width = std::max(width,2300.8*mm); // from ND280USTargetSP0DuleConstructor::
  width = std::max(width,2300.8*mm); // from ND280CTargetSP0DuleConstructor::
  width = std::max(width,2300.8*mm); // from ND280CECalSP0DuleConstructor::
  width += 10*mm;                    // from ND280P0DConstructor::SetWidthAdjustmentSpace()

  double fUSECalDsLIPosition = 323*mm;

  // Taken from ND280USECalSP0DuleConstructor.GetLength()
  double fUSECalLength = 179.97*mm; 
  
  double fUSECalSkewAngle = 2.71*mrad;
  double fLengthAdjustmentSpace = 3*mm;
  
  double length = fCECalDsLIPosition;
  length += std::max(0.0,fECalBraceThickness);
  length += std::max(0.0,std::abs(fCECalSkewAngle)*width);
  length -= fUSECalDsLIPosition;
  length += fUSECalLength;
  length += std::max(0.0,std::abs(fUSECalSkewAngle)*width);
  length += fLengthAdjustmentSpace;
  return length;
}


double ND280BasketConstructor::GetTrackerServiceLength() {
  //
  // nd280mc CODE
  //ND280TrackerConstructor& trk = Get<ND280TrackerConstructor>("Tracker");
  //return trk.GetLength();
  //

  //
  // Value taken from ND280TrackerConstructor.cc ( through SetLength() )
  //
  return 3702.0*mm;
}

G4LogicalVolume *ND280BasketConstructor::GetPiece(void) {
  
  // Make the main basket space allowing for the tracker service space which
  // is outside of the basket.
  G4VSolid* mainBasket = new G4Box(GetName()+"/mainbasket",
				   GetOuterWidth()/2
				   +GetTrackerServiceWidth(),
				   GetOuterHeight()/2
				   +GetTrackerServiceHeight(),
				   GetOuterLength()/2);
  
  // Make space for the P0D services which are outside of the basket.
  G4VSolid* p0dServices = new G4Box(GetName()+"/p0dServices",
  				    GetOuterWidth()/2 + GetP0DServiceSpace(),
  				    GetOuterHeight()/2 + GetP0DServiceSpace(),
  				    GetP0DServiceLength()/2);  
  G4VSolid *envSolid 
    = new G4UnionSolid(GetName(),
  		       mainBasket, p0dServices, NULL,
  		       G4ThreeVector(0,0,
  				     -GetInnerLength()/2
  				     + GetP0DServiceLength()/2));
  G4LogicalVolume *logVolume
    = new G4LogicalVolume(envSolid,
  			  FindMaterial("Air"),
  			  GetName());
  
  // // NEW ND280UPGRADE: w/o P0D services (don't use it...)
  // G4LogicalVolume *logVolume
  //   = new G4LogicalVolume(mainBasket,
  // 			  FindMaterial("Air"),
  // 			  GetName());
  
  
  SetVisAttributes(logVolume);
    
  //////////////////////////////////////////
  // CONSTRUCT THE BASKET
  //////////////////////////////////////////
  
  /// The center of the basket to construct.  The coordinate system is
  /// defined by the position of the *UNLOADED* basket.  The constructed
  /// basket is centered at the position of the *UNLOADED* basket.

  ND280StructuralBeamConstructor& beam 
  = Get<ND280StructuralBeamConstructor>("Beam");
  
  if( GetND280XML()->GetXMLUseBasket() ){    
    
    // Construct the Lower Long Beam (1)
      
    G4double beamWidth  = GetSideThickness();
    G4double beamHeight = GetBottomThickness();
    G4double beamLength = GetInnerLength();

    beam.SetWidth(beamWidth);
    beam.SetHeight(beamHeight);
    beam.SetLength(beamLength);

    beam.SetLocalName("beamBottomSouth");
    new G4PVPlacement(0,                // rotation
                      G4ThreeVector((GetInnerWidth()/2+beamWidth/2),
                                    -(GetOuterHeight()/2-beamHeight/2),
                                    0), // position
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #
    beam.SetLocalName("beamBottomNorth");
    new G4PVPlacement(0,                // rotation
                      G4ThreeVector(-(GetInnerWidth()/2+beamWidth/2),
                                    -(GetOuterHeight()/2-beamHeight/2),
                                    0), // position
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      1);               // copy #

    // Construct the Top Long Beam (2)

    beamHeight = GetTopThickness();
    beam.SetHeight(beamHeight);
    beam.SetLocalName("beamTopSouth");

    new G4PVPlacement(0,                // rotation
                      G4ThreeVector((GetInnerWidth()/2+beamWidth/2),
                                    (GetOuterHeight()/2-beamHeight/2),
                                    0), // position
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      2);               // copy #
    beam.SetLocalName("beamTopNorth");
    new G4PVPlacement(0,                // rotation
                      G4ThreeVector(-(GetInnerWidth()/2+beamWidth/2),
                                    (GetOuterHeight()/2-beamHeight/2),
                                    0), // position
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      3);               // copy #

    // Construct the End Horizontal Beam (5)

    beamWidth = GetUpstreamThickness();
    beamHeight = GetEndWidth();
    beamLength = GetInnerWidth();
    beam.SetWidth(beamWidth);
    beam.SetHeight(beamHeight);
    beam.SetLength(beamLength);

    G4RotationMatrix* rotP90Y = new G4RotationMatrix();
    rotP90Y->rotateY(90*degree);
    beam.SetLocalName("beamTopDownstream");
    new G4PVPlacement(rotP90Y,          // rotation
                      G4ThreeVector(0,
                                     (GetOuterHeight()/2-beamHeight/2),
                                     (GetOuterLength()/2-beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // not a copy.
    beam.SetLocalName("beamBottomDownstream");
    new G4PVPlacement(rotP90Y,          // rotation
                      G4ThreeVector(0,
                                    -(GetOuterHeight()/2-beamHeight/2),
                                     (GetOuterLength()/2-beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      1);               // not a copy.
    beam.SetLocalName("beamTopUpstream");
    new G4PVPlacement(rotP90Y,          // rotation
                      G4ThreeVector(0,
                                    (GetOuterHeight()/2-beamHeight/2),
                                    -(GetOuterLength()/2-beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      2);               // copy #
    beam.SetLocalName("beamBottomUpstream");
    new G4PVPlacement(rotP90Y,          // rotation
                      G4ThreeVector(0,
                                    -(GetOuterHeight()/2-beamHeight/2),
                                    -(GetOuterLength()/2-beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      3);               // copy #

    // Construct the Corner Vertical Support (4) and (22)

    beamWidth = GetSideThickness();
    beamHeight = GetUpstreamThickness();
    beamLength = GetOuterHeight();
    beam.SetWidth(beamWidth);
    beam.SetHeight(beamHeight);
    beam.SetLength(beamLength);

    G4RotationMatrix* rotP90X = new G4RotationMatrix();
    rotP90X->rotateX(90*degree);
    beam.SetLocalName("beamDownstreamSouth");
    new G4PVPlacement(rotP90X,          // rotation
                      G4ThreeVector((GetInnerWidth()/2+beamWidth/2),
                                    0,
                                    (GetOuterLength()/2-beamHeight/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #
    beam.SetLocalName("beamDownstreamNorth");
    new G4PVPlacement(rotP90X,          // rotation
                      G4ThreeVector(-(GetInnerWidth()/2+beamWidth/2),
                                    0,
                                    (GetOuterLength()/2-beamHeight/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      1);               // copy #
    beam.SetLocalName("beamUpstreamSouth");
    new G4PVPlacement(rotP90X,          // rotation
                      G4ThreeVector((GetInnerWidth()/2+beamWidth/2),
                                    0,
                                    -(GetOuterLength()/2-beamHeight/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      2);               // copy #
    beam.SetLocalName("beamUpstreamNorth");
    new G4PVPlacement(rotP90X,          // rotation
                      G4ThreeVector(-(GetInnerWidth()/2+beamWidth/2),
                                    0,
                                    -(GetOuterLength()/2-beamHeight/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      3);               // copy #

    // Construct the Vertical Support Stave (3)

    beamWidth =   GetStaveWidth();
    beamHeight =  GetStaveHeight();
    beamLength = GetOuterHeight()-GetBottomThickness()-GetTopThickness();
    beam.SetWidth(beamWidth);
    beam.SetHeight(beamHeight);
    beam.SetLength(beamLength);

    beam.SetLocalName("staveDownstreamSouth");
    new G4PVPlacement(rotP90X,          // rotation
                      G4ThreeVector((GetInnerWidth()/2+beamWidth/2),
                                    0.5*(GetBottomThickness()
                                         - GetTopThickness()),
                                    -GetOuterLength()/2+GetUpstreamStave()),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #
    beam.SetLocalName("staveCenterSouth");
    new G4PVPlacement(rotP90X,          // rotation
                      G4ThreeVector((GetInnerWidth()/2+beamWidth/2),
                                    0.5*(GetBottomThickness()
                                         - GetTopThickness()),
                                    -GetOuterLength()/2+GetCenterStave()),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      1);               // copy #
    beam.SetLocalName("staveUpstreamSouth");
    new G4PVPlacement(rotP90X,          // rotation
                      G4ThreeVector((GetInnerWidth()/2+beamWidth/2),
                                    0.5*(GetBottomThickness()
                                         - GetTopThickness()),
                                    -GetOuterLength()/2+GetDownstreamStave()),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      2);               // copy #
    beam.SetLocalName("staveDownstreamNorth");
    new G4PVPlacement(rotP90X,          // rotation
                      G4ThreeVector(-(GetInnerWidth()/2+beamWidth/2),
                                    0.5*(GetBottomThickness()
                                         - GetTopThickness()),
                                    -GetOuterLength()/2+GetUpstreamStave()),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      3);               // copy #
    beam.SetLocalName("staveCenterNorth");
    new G4PVPlacement(rotP90X,          // rotation
                      G4ThreeVector(-(GetInnerWidth()/2+beamWidth/2),
                                    0.5*(GetBottomThickness()
                                         - GetTopThickness()),
                                    -GetOuterLength()/2+GetCenterStave()),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      4);               // copy #
    beam.SetLocalName("staveUpstreamNorth");
    new G4PVPlacement(rotP90X,          // rotation
                      G4ThreeVector(-(GetInnerWidth()/2+beamWidth/2),
                                    0.5*(GetBottomThickness()
                                         - GetTopThickness()),
                                    -GetOuterLength()/2+GetDownstreamStave()),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      5);               // copy #

    // Construct the Horizontal Hub Beam (6)
    beamWidth =  GetDownstreamThickness();
    beamHeight = GetEndWidth();
    beamLength = (GetInnerWidth()-GetHubDiameter())/2;
    beam.SetWidth(beamWidth);
    beam.SetHeight(beamHeight);
    beam.SetLength(beamLength);

    beam.SetLocalName("hubDownstreamSouth");
    new G4PVPlacement(rotP90Y,          // rotation
                      G4ThreeVector( (GetHubDiameter()/2+beamLength/2),
                                     0,
                                     (GetOuterLength()/2-beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #
    beam.SetLocalName("hubDownstreamNorth");
    new G4PVPlacement(rotP90Y,          // rotation
                      G4ThreeVector(-(GetHubDiameter()/2+beamLength/2),
                                    0,
                                     (GetOuterLength()/2-beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      1);               // copy #
    beam.SetLocalName("hubUpstreamSouth");
    new G4PVPlacement(rotP90Y,          // rotation
                      G4ThreeVector( (GetHubDiameter()/2+beamLength/2),
                                    0,
                                    -(GetOuterLength()/2-beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      2);               // copy #
    beam.SetLocalName("hubUpstreamNorth");
    new G4PVPlacement(rotP90Y,          // rotation
                      G4ThreeVector(-(GetHubDiameter()/2+beamLength/2),
                                    0,
                                    -(GetOuterLength()/2-beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      3);               // copy #

    // Construct the Vertical Hub Beam (7)

    beamWidth =  GetDownstreamThickness();
    beamHeight = GetEndWidth();
    beamLength = (GetOuterHeight()-2.*GetEndWidth()-GetHubDiameter())/2;
    beam.SetWidth(beamWidth);
    beam.SetHeight(beamHeight);
    beam.SetLength(beamLength);

    G4RotationMatrix* rotP90YP90X = new G4RotationMatrix();
    rotP90YP90X->rotateY(90*degree);
    rotP90YP90X->rotateX(90*degree);

    beam.SetLocalName("hubDownstreamTop");
    new G4PVPlacement(rotP90YP90X,      // rotation
                      G4ThreeVector(0, 
                                    (GetHubDiameter()/2+beamLength/2),
                                    (GetOuterLength()/2-beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #
    beam.SetLocalName("hubDownstreamBottom");
    new G4PVPlacement(rotP90YP90X,      // rotation
                      G4ThreeVector(0,
                                    -(GetHubDiameter()/2+beamLength/2),
                                     (GetOuterLength()/2-beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      1);               // copy #
    beam.SetLocalName("hubUpstreamTop");
    new G4PVPlacement(rotP90YP90X,      // rotation
                      G4ThreeVector(0,
                                    (GetHubDiameter()/2+beamLength/2),
                                    -(GetOuterLength()/2-beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      2);               // copy #
    beam.SetLocalName("hubUpstreamBottom");
    new G4PVPlacement(rotP90YP90X,      // rotation
                      G4ThreeVector(0,
                                    -(GetHubDiameter()/2+beamLength/2),
                                    -(GetOuterLength()/2-beamWidth/2)),
                      beam.GetPiece(),  // logical volume
                      beam.GetName(),   // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      3);               // copy #

    ////////////////////////////////////////////////////////////
    // Construct the Central Hub.  The radius used is the radius to the flats,
    // not the radius to the corners.
    ///////////////////////////////////////////////////////////
    G4double rOut = 0.5*GetHubDiameter();
    G4double rIn = rOut - beam.GetWallThickness();
    G4double zPlane[] = {-GetUpstreamThickness()/2, 
                         GetUpstreamThickness()/2};
    G4double rInner[] = {0, 0};
    G4double rOuter[] = {rOut, rOut};

    G4LogicalVolume *logHub
        = new G4LogicalVolume(new G4Polyhedra(GetName()+"/CentreHub",
                                              0*degree, 360*degree, 8,
                                              2, zPlane, rInner, rOuter),
                                              FindMaterial("Steel"),
                                              GetName()+"/CentreHub");

    SetVisAttributes(logHub);
 
    // Build the void in the central hubs.
    G4double zPlaneAir[] = {zPlane[0]+beam.GetWallThickness(), 
                            zPlane[1]-beam.GetWallThickness()};
    G4double rInnerAir[] = {0,0};
    G4double rOuterAir[] = {rIn,rIn};

    G4LogicalVolume *logHubAir
        = new G4LogicalVolume(new G4Polyhedra(GetName()+"/CentreHubAir",
                                              0*degree, 360*degree, 8,
                                              2,zPlaneAir,rInnerAir,rOuterAir),
                                              FindMaterial("Air"),
                                              GetName()+"/CentreHubAir");
    SetVisAttributes(logHubAir);

    new G4PVPlacement(0,         // rotation
                      G4ThreeVector(0,0,0), // position
                      logHubAir,        // logical volume
                      GetName()+"/CentreHubAir", // name
                      logHub,           // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    // Build the hole into the central hub.
    G4LogicalVolume *logHubAirDisk
        = new G4LogicalVolume(new G4Tubs(GetName()+"/CentreHubAirDisk",
                                         0, (GetAxleDiameter()/2 
                                             - GetAxleWallThickness()),
                                         beam.GetWallThickness()/2,
                                         0*degree, 360*degree),
                                         FindMaterial("Air"),
                                         GetName()+"/CentreHubAirDisk");

    SetVisAttributes(logHubAirDisk);
    
    new G4PVPlacement(0,                // rotation
                      G4ThreeVector(0, 0,
                                    -0.5*(GetUpstreamThickness()
                                          - beam.GetWallThickness())),
                      logHubAirDisk,    // logical volume
                      GetName()+"/UpstreamCentreHubAirDisk", // name
                      logHub,           // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    new G4PVPlacement(0,                // rotation
                      G4ThreeVector(0, 0,
                                    0.5*(GetUpstreamThickness()
                                         -beam.GetWallThickness())),
                      logHubAirDisk,    // logical volume
                      GetName()+"/DownstreamCentreHubAirDisk", // name
                      logHub,           // mother  volume
                      false,            // no boolean operations
                      1);               // copy #

    G4LogicalVolume *logHubSteelTube
        = new G4LogicalVolume(new G4Tubs(GetName()+"/CentreHubSteelTube",
                                         GetAxleDiameter()/2
                                         -GetAxleWallThickness(),
                                         GetAxleDiameter()/2,
                                         GetUpstreamThickness()/2-
                                         beam.GetWallThickness(),
                                         0*degree, 360*degree),
                                         FindMaterial("Steel"),
                                         GetName()+"/CentreHubSteelTube");

    SetVisAttributes(logHubSteelTube);
    
    new G4PVPlacement(0,                // rotation
                      G4ThreeVector(0,0,0), // position
                      logHubSteelTube,  // logical volume
                      GetName()+"/CentreHubSteelTube", // name
                      logHubAir,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    G4RotationMatrix* rotP45b2Z = new G4RotationMatrix();
    rotP45b2Z->rotateZ(0.5*45.0*degree);

    new G4PVPlacement(rotP45b2Z,        // rotation
                      G4ThreeVector(0, 0,
                                    (GetOuterLength()
                                     -GetUpstreamThickness())/2),
                      logHub,           // logical volume
                      GetName()+"/DownstreamCentralHub",     // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #
    new G4PVPlacement(rotP45b2Z,         // rotation
                      G4ThreeVector(0, 0,
                                    -(GetOuterLength()
                                      -GetUpstreamThickness())/2),
                      logHub,           // logical volume
                      GetName()+"/UpstreamCentralHub",     // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      1);               // copy #

#ifdef EXTERNAL_SUPPORT_STRUCTURE
    /// \todo The external support structure are located outside of the magnet
    /// volume on the up and down stream ends of the basket.  The magnet and
    /// basket model will need to be significantly modified to support this
    /// geometry.

    // Construct the Support Tube (9)

    G4LogicalVolume *logSupportTube
        = new G4LogicalVolume(new G4Tubs(GetName()+"/SupportTube",
                                         250*mm/2-10*mm,
                                         250*mm/2,
                                         1250*mm/2,
                                         0*degree, 360*degree),
                                         FindMaterial("Steel"),
                                         GetName()+"/SupportTube");

    SetVisAttributes(logSupportTube);
    
    new G4PVPlacement(0,                // rotation
                      G4ThreeVector(0,
                                    0,
                                    -(GetOuterLength()+1250*mm)/2), // position
                      logSupportTube,  // logical volume
                      GetName()+"/SupportTube", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #
    new G4PVPlacement(0,                // rotation
                      G4ThreeVector(0,
                                    0,
                                    (GetOuterLength()+1250*mm)/2), // position
                      logSupportTube,  // logical volume
                      GetName()+"/SupportTube", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      1);               // copy #

    // Construct the Upper Stiffening Plate (10)

    G4LogicalVolume *logUStiffPlate
         = new G4LogicalVolume(new G4Box(GetName()+"/UStiffPlate",
                                         640*mm/2,
                                         765*mm/2,
                                         15*mm/2),
                                         FindMaterial("Steel"),
                                         GetName()+"/UStiffPlate");

    SetVisAttributes(logUStiffPlate);
    
    G4LogicalVolume *logUStiffPlateAir
        = new G4LogicalVolume(new G4Trap(GetName()+"/UStiffPlateAir",
                                         15*mm,
                                         (765*mm-165*mm),
                                         (640*mm-250*mm),
                                         1*mm),
                                         FindMaterial("Air"),
                                         GetName()+"/UStiffPlateAir");

    SetVisAttributes(logUStiffPlateAir);
    
    G4RotationMatrix* rotP180Z = new G4RotationMatrix();
    rotP180Z->rotateZ(180*degree);

    new G4PVPlacement(rotP180Z,         // rotation
                      G4ThreeVector(640*mm/2-(640*mm-250*mm)/4,
                                    765*mm/2-(765*mm-165*mm)/2,
                                    0), // position
                      logUStiffPlateAir,  // logical volume
                      GetName()+"/UStiffPlateAir", // name
                      logUStiffPlate,   // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    new G4PVPlacement(rotP90Y,          // rotation
                      G4ThreeVector( 0,
                                     (250*mm/2+765*mm/2),
                                     (GetOuterLength()/2+640*mm/2)), // position
                      logUStiffPlate,   // logical volume
                      GetName()+"/UStiffPlate", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    G4RotationMatrix* rotM90Y = new G4RotationMatrix();
    rotM90Y->rotateY(-90*degree);

    new G4PVPlacement(rotM90Y,          // rotation
                      G4ThreeVector( 0,
                                     (250*mm/2+765*mm/2),
                                    -(GetOuterLength()/2+640*mm/2)), // position
                      logUStiffPlate,   // logical volume
                      GetName()+"/UStiffPlate", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      1);               // copy #

    // Construct the Lower Stiffening Plate (11)

    G4LogicalVolume *logLStiffPlate
         = new G4LogicalVolume(new G4Box(GetName()+"/LStiffPlate",
                                         640*mm/2,
                                         765*mm/2,
                                         15*mm/2),
                                         FindMaterial("Steel"),
                                         GetName()+"/LStiffPlate");

    SetVisAttributes(logLStiffPlate);
    
    new G4PVPlacement(rotP90Y,          // rotation
                      G4ThreeVector( 0,
                                    -(250*mm/2+765*mm/2),
                                     (GetOuterLength()/2+640*mm/2)), // position
                      logLStiffPlate,   // logical volume
                      GetName()+"/LStiffPlate", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #
    new G4PVPlacement(rotM90Y,          // rotation
                      G4ThreeVector( 0,
                                    -(250*mm/2+765*mm/2),
                                    -(GetOuterLength()/2+640*mm/2)), // position
                      logLStiffPlate,   // logical volume
                      GetName()+"/LStiffPlate", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      1);               // copy #
#endif

    // Construct the Diagonal Lower Bracing P0D Side (13)

    G4double dx1 = (GetOuterHeight()/2-300*mm) * std::sin(pi/4);
    G4double dx2 = dx1 - std::tan(pi/4)*200*mm;

    G4LogicalVolume *logDiagLBracing
         = new G4LogicalVolume(new G4Trd(GetName()+"/DiagLBracing",
                                         dx1,
                                         dx2,
                                         100/2*mm,
                                         100/2*mm,
                                         200/2*mm),
                                         FindMaterial("Steel"),
                                         GetName()+"/DiagLBracing");

    SetVisAttributes(logDiagLBracing);
    
    G4LogicalVolume *logDiagLBracingAir
         = new G4LogicalVolume(new G4Trd(GetName()+"/DiagLBracingAir",
                                         dx1-6*mm*std::tan(pi/4),
                                         dx2+6*mm*std::tan(pi/4),
                                         100/2*mm-6*mm,
                                         100/2*mm-6*mm,
                                         200/2*mm-6*mm),
                                         FindMaterial("Air"),
                                         GetName()+"/DiagLBracingAir");

    SetVisAttributes(logDiagLBracingAir);
    
    new G4PVPlacement(0,                // rotation
                      G4ThreeVector(0,
                                    0,
                                    0), // position
                      logDiagLBracingAir,// logical volume
                      GetName()+"/DiagLBracingAir", // name
                      logDiagLBracing,   // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    G4double s1 = (GetOuterHeight()/2-300*mm)*std::cos(pi/4);

    G4RotationMatrix* rotP90ZP225Y = new G4RotationMatrix();
    rotP90ZP225Y->rotateZ(90*degree);
    rotP90ZP225Y->rotateY(225*degree);

    new G4PVPlacement(rotP90ZP225Y,      // rotation
                      G4ThreeVector( (GetOuterWidth()/2-100*mm/2),
                                    -(GetOuterHeight()/2-300*mm-
                                      (s1-200/2*mm)*std::sin(pi/4)),
                                    -(GetInnerLength()/2-
                                      (s1-200/2*mm)*std::cos(pi/4))),// position
                      logDiagLBracing,   // logical volume
                      GetName()+"/DiagLBracing", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #
    new G4PVPlacement(rotP90ZP225Y,     // rotation
                      G4ThreeVector(-(GetOuterWidth()/2-100*mm/2),
                                    -(GetOuterHeight()/2-300*mm-
                                      (s1-200/2*mm)*std::sin(pi/4)),
                                    -(GetInnerLength()/2-
                                      (s1-200/2*mm)*std::cos(pi/4))),// position
                      logDiagLBracing,   // logical volume
                      GetName()+"/DiagLBracing", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      1);               // copy #

    // Construct the Diagonal Upper Bracing P0D Side (12)

    dx1 = (GetOuterHeight()/2-200*mm) * std::sin(pi/4);
    dx2 = dx1 - std::tan(pi/4)*200*mm;

    G4LogicalVolume *logDiagUBracing
         = new G4LogicalVolume(new G4Trd(GetName()+"/DiagUBracing",
                                         dx1,
                                         dx2,
                                         100/2*mm,
                                         100/2*mm,
                                         200/2*mm),
                                         FindMaterial("Steel"),
                                         GetName()+"/DiagUBracing");

    SetVisAttributes(logDiagUBracing);
    
    G4LogicalVolume *logDiagUBracingAir
         = new G4LogicalVolume(new G4Trd(GetName()+"/DiagUBracingAir",
                                         dx1-6*mm*std::tan(pi/4),
                                         dx2+6*mm*std::tan(pi/4),
                                         100/2*mm-6*mm,
                                         100/2*mm-6*mm,
                                         200/2*mm-6*mm),
                                         FindMaterial("Air"),
                                         GetName()+"/DiagUBracingAir");

    SetVisAttributes(logDiagUBracingAir);
    
    new G4PVPlacement(0,                // rotation
                      G4ThreeVector(0,
                                    0,
                                    0), // position
                      logDiagUBracingAir,// logical volume
                      GetName()+"/DiagUBracingAir", // name
                      logDiagUBracing,   // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    s1 = (GetOuterHeight()/2-200*mm)*std::cos(pi/4);

    G4RotationMatrix* rotP90ZP135Y = new G4RotationMatrix();
    rotP90ZP135Y->rotateZ(90*degree);
    rotP90ZP135Y->rotateY(135*degree);

    new G4PVPlacement(rotP90ZP135Y,      // rotation
                      G4ThreeVector( (GetOuterWidth()/2-100*mm/2),
                                     (GetOuterHeight()/2-200*mm-
                                      (s1-200/2*mm)*std::sin(pi/4)),
                                    -(GetInnerLength()/2-
                                      (s1-200/2*mm)*std::cos(pi/4))),// position
                      logDiagUBracing,   // logical volume
                      GetName()+"/DiagUBracing", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #
    new G4PVPlacement(rotP90ZP135Y,     // rotation
                      G4ThreeVector(-(GetOuterWidth()/2-100*mm/2),
                                     (GetOuterHeight()/2-200*mm-
                                      (s1-200/2*mm)*std::sin(pi/4)),
                                    -(GetInnerLength()/2-
                                      (s1-200/2*mm)*std::cos(pi/4))),// position
                      logDiagUBracing,   // logical volume
                      GetName()+"/DiagUBracing", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      1);               // copy #

    // Construct the Diagonal Bracing ECAL Side (14)

    dx1 = 500.63*mm * std::sin(pi/4);
    dx2 = dx1 - std::tan(pi/4)*200*mm;

    G4LogicalVolume *logDiagBracing
         = new G4LogicalVolume(new G4Trd(GetName()+"/DiagBracing",
                                         dx1,
                                         dx2,
                                         100/2*mm,
                                         100/2*mm,
                                         200/2*mm),
                                         FindMaterial("Steel"),
                                         GetName()+"/DiagBracing");

    SetVisAttributes(logDiagBracing);
    
    G4LogicalVolume *logDiagBracingAir
         = new G4LogicalVolume(new G4Trd(GetName()+"/DiagBracingAir",
                                         dx1-6*mm*std::tan(pi/4),
                                         dx2+6*mm*std::tan(pi/4),
                                         100/2*mm-6*mm,
                                         100/2*mm-6*mm,
                                         200/2*mm-6*mm),
                                         FindMaterial("Air"),
                                         GetName()+"/DiagBracingAir");

    SetVisAttributes(logDiagBracingAir);
    
    new G4PVPlacement(0,                // rotation
                      G4ThreeVector(0,
                                    0,
                                    0), // position
                      logDiagBracingAir,// logical volume
                      GetName()+"/DiagBracingAir", // name
                      logDiagBracing,   // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    s1 = 500.63*mm*std::cos(pi/4);

    G4RotationMatrix* rotP90ZM45Y = new G4RotationMatrix();
    rotP90ZM45Y->rotateZ(90*degree);
    rotP90ZM45Y->rotateY(-45*degree);

    new G4PVPlacement(rotP90ZM45Y,      // rotation
                      G4ThreeVector( (GetOuterWidth()/2-100*mm/2),
                                    -(GetOuterHeight()/2-300*mm-
                                      (s1-200/2*mm)*std::sin(pi/4)),
                                     (GetInnerLength()/2-
                                      (s1-200/2*mm)*std::cos(pi/4))),// position
                      logDiagBracing,   // logical volume
                      GetName()+"/DiagBracing", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #
    new G4PVPlacement(rotP90ZM45Y,      // rotation
                      G4ThreeVector(-(GetOuterWidth()/2-100*mm/2),
                                    -(GetOuterHeight()/2-300*mm-
                                      (s1-200/2*mm)*std::sin(pi/4)),
                                     (GetInnerLength()/2-
                                      (s1-200/2*mm)*std::cos(pi/4))),// position
                      logDiagBracing,   // logical volume
                      GetName()+"/DiagBracing", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      1);               // copy #

    G4RotationMatrix* rotP90ZP45Y = new G4RotationMatrix();
    rotP90ZP45Y->rotateZ(90*degree);
    rotP90ZP45Y->rotateY(45*degree);

    new G4PVPlacement(rotP90ZP45Y,      // rotation
                      G4ThreeVector( (GetOuterWidth()/2-100*mm/2),
                                     (GetOuterHeight()/2-200*mm-
                                      (s1-200/2*mm)*std::sin(pi/4)),
                                     (GetInnerLength()/2-
                                      (s1-200/2*mm)*std::cos(pi/4))),// position
                      logDiagBracing,   // logical volume
                      GetName()+"/DiagBracing", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      2);               // copy #
    new G4PVPlacement(rotP90ZP45Y,      // rotation
                      G4ThreeVector(-(GetOuterWidth()/2-100*mm/2),
                                     (GetOuterHeight()/2-200*mm-
                                      (s1-200/2*mm)*std::sin(pi/4)),
                                     (GetInnerLength()/2-
                                      (s1-200/2*mm)*std::cos(pi/4))),// position
                      logDiagBracing,   // logical volume
                      GetName()+"/DiagBracing", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      3);               // copy #

    // Construct the ECAL Support (18)

    dx1 = 715*mm - 300*mm - (500.63*mm+140*mm-492.5*mm)*std::tan(pi/4);
    dx2 = dx1 - 200*mm/std::tan(pi/4);

    G4LogicalVolume *logEcalSupport
         = new G4LogicalVolume(new G4Trap(GetName()+"/EcalSupport",
                                         100*mm,
                                         200*mm,
                                         dx1,
                                         dx2),
                                         FindMaterial("Steel"),
                                         GetName()+"/EcalSupport");

    SetVisAttributes(logEcalSupport);
    
    G4LogicalVolume *logEcalSupportAir
         = new G4LogicalVolume(new G4Trap(GetName()+"/EcalSupportAir",
                                         100*mm-2*6*mm,
                                         200*mm-2*6*mm,
                                         dx1-6*mm*std::tan(pi/4)-25*mm,
                                         dx2+6*mm*std::tan(pi/4)-25*mm),
                                         FindMaterial("Air"),
                                         GetName()+"/EcalSupportAir");

    SetVisAttributes(logEcalSupportAir);

    new G4PVPlacement(0,                // rotation
                      G4ThreeVector(25*mm/2,
                                    0,
                                    0), // position
                      logEcalSupportAir,// logical volume
                      GetName()+"/EcalSupportAir", // name
                      logEcalSupport,   // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    G4RotationMatrix* rotP90YP90Z = new G4RotationMatrix();
    rotP90YP90Z->rotateY(90*degree);
    rotP90YP90Z->rotateZ(90*degree);

    s1 = (dx1 - 200*mm/2*std::tan(pi/4))/2;

    // Don't build because of overlaps.
    new G4PVPlacement(rotP90YP90Z,      // rotation
                      G4ThreeVector( (GetOuterWidth()/2-100*mm/2),
                                    -(GetOuterHeight()/2 - 715*mm + s1),
                                     (GetOuterLength()/2-492.5*mm+200*mm/2)),
                      logEcalSupport,   // logical volume
                      GetName()+"/EcalSupport", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #

    new G4PVPlacement(rotP90YP90Z,      // rotation
                      G4ThreeVector(-(GetOuterWidth()/2-100*mm/2),
                                    -(GetOuterHeight()/2 - 715*mm + s1),
                                     (GetOuterLength()/2-492.5*mm+200*mm/2)),
                      logEcalSupport,   // logical volume
                      GetName()+"/EcalSupport", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      1);               // copy #





    /*
      
    //
    // ND280UPGRADE: comment the following block where the rails 
    // for the ForwTPCs are implemented. Since we move the ForwTPCs
    // can be moved depending on the detector configuration
    // this can create overlaps and fail.
    // Don't use the rails for now.
    //
    
    // Construct the various Rails
    ND280IBeamConstructor& rail
        = Get<ND280IBeamConstructor>("Rail");

    rail.SetIBeamCutOutNumber(1);
    rail.SetMaterialName("Steel");
    rail.SetStemThickness(6.35*mm);
    rail.SetBaseThickness(6.35*mm);

    rail.SetLength(GetInnerWidth());
    rail.SetWidth(50.8*mm);
    rail.SetHeight(101.6*mm);

    new G4PVPlacement(rotP90YP90Z,      // rotation
                      G4ThreeVector( 0,
                                     -(GetOuterHeight()/2 - rail.GetWidth()/2),
                                     -(GetInnerLength()/2-2582*mm)),
                      rail.GetPiece(),  // logical volume
                      GetName()+"/TPCRail", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #
    new G4PVPlacement(rotP90YP90Z,      // rotation
                      G4ThreeVector( 0,
                                     -(GetOuterHeight()/2 - rail.GetWidth()/2),
                                     -(GetInnerLength()/2-3272*mm)),
                      rail.GetPiece(),  // logical volume
                      GetName()+"/TPCRail", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      1);               // copy #
    new G4PVPlacement(rotP90YP90Z,      // rotation
                      G4ThreeVector( 0,
                                     -(GetOuterHeight()/2 - rail.GetWidth()/2),
                                     -(GetInnerLength()/2-3941*mm)),
                      rail.GetPiece(),  // logical volume
                      GetName()+"/TPCRail", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      2);               // copy #
    new G4PVPlacement(rotP90YP90Z,      // rotation
                      G4ThreeVector( 0,
                                     -(GetOuterHeight()/2 - rail.GetWidth()/2),
                                     -(GetInnerLength()/2-4631*mm)),
                      rail.GetPiece(),  // logical volume
                      GetName()+"/TPCRail", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      3);               // copy #
    new G4PVPlacement(rotP90YP90Z,      // rotation
                      G4ThreeVector( 0,
                                     -(GetOuterHeight()/2 - rail.GetWidth()/2),
                                     -(GetInnerLength()/2-5300*mm)),
                      rail.GetPiece(),  // logical volume
                      GetName()+"/TPCRail", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      4);               // copy #
    new G4PVPlacement(rotP90YP90Z,      // rotation
                      G4ThreeVector( 0,
                                     -(GetOuterHeight()/2 - rail.GetWidth()/2),
                                     -(GetInnerLength()/2-5990*mm)),
                      rail.GetPiece(),  // logical volume
                      GetName()+"/TPCRail", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      5);               // copy #
   		      
    // FGD Mounting Block (15)

    //  To avoid Overlap make them narrower (20mm->10mm)
    //  e.g. SetLength((GetInnerWidth()-trk.GetWidth())/2)
    //    rail.SetLength(20*mm);
    //    rail.SetIBeamLength(20*mm);
    rail.SetLength(10*mm);
    rail.SetIBeamLength(10*mm);
    rail.SetWidth(100*mm);
    rail.SetHeight(120*mm+2*6*mm);

#ifdef BUILD_FGD_RAILS
    new G4PVPlacement(rotP90YP90Z,      // rotation
                      G4ThreeVector( -(GetInnerWidth()/2-rail.GetLength()/2),
                                     -(GetOuterHeight()/2 - rail.GetWidth()/2),
                                     -(GetInnerLength()/2-3480.35*mm)),
                      rail.GetPiece(),  // logical volume
                      GetName()+"/FGDRail", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      0);               // copy #
    new G4PVPlacement(rotP90YP90Z,      // rotation
                      G4ThreeVector(  (GetInnerWidth()/2-rail.GetLength()/2),
                                     -(GetOuterHeight()/2 - rail.GetWidth()/2),
                                     -(GetInnerLength()/2-3480.35*mm)),
                      rail.GetPiece(),  // logical volume
                      GetName()+"/FGDRail", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      1);               // copy #
    new G4PVPlacement(rotP90YP90Z,      // rotation
                      G4ThreeVector( -(GetInnerWidth()/2-rail.GetLength()/2),
                                     -(GetOuterHeight()/2 - rail.GetWidth()/2),
                                     -(GetInnerLength()/2-3732.65*mm)),
                      rail.GetPiece(),  // logical volume
                      GetName()+"/FGDRail", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      2);               // copy #
    new G4PVPlacement(rotP90YP90Z,      // rotation
                      G4ThreeVector(  (GetInnerWidth()/2-rail.GetLength()/2),
                                     -(GetOuterHeight()/2 - rail.GetWidth()/2),
                                     -(GetInnerLength()/2-3732.65*mm)),
                      rail.GetPiece(),  // logical volume
                      GetName()+"/FGDRail", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      3);               // copy #
    new G4PVPlacement(rotP90YP90Z,      // rotation
                      G4ThreeVector( -(GetInnerWidth()/2-rail.GetLength()/2),
                                     -(GetOuterHeight()/2 - rail.GetWidth()/2),
                                     -(GetInnerLength()/2-4839.35*mm)),
                      rail.GetPiece(),  // logical volume
                      GetName()+"/FGDRail", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      4);               // copy #
    new G4PVPlacement(rotP90YP90Z,      // rotation
                      G4ThreeVector(  (GetInnerWidth()/2-rail.GetLength()/2),
                                     -(GetOuterHeight()/2 - rail.GetWidth()/2),
                                     -(GetInnerLength()/2-4839.35*mm)),
                      rail.GetPiece(),  // logical volume
                      GetName()+"/FGDRail", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      5);               // copy #
    new G4PVPlacement(rotP90YP90Z,      // rotation
                      G4ThreeVector( -(GetInnerWidth()/2-rail.GetLength()/2),
                                     -(GetOuterHeight()/2 - rail.GetWidth()/2),
                                     -(GetInnerLength()/2-5091.65*mm)),
                      rail.GetPiece(),  // logical volume
                      GetName()+"/FGDRail", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      6);               // copy #
    new G4PVPlacement(rotP90YP90Z,      // rotation
                      G4ThreeVector(  (GetInnerWidth()/2-rail.GetLength()/2),
                                     -(GetOuterHeight()/2 - rail.GetWidth()/2),
                                     -(GetInnerLength()/2-5091.65*mm)),
                      rail.GetPiece(),  // logical volume
                      GetName()+"/FGDRail", // name
                      logVolume,        // mother  volume
                      false,            // no boolean operations
                      7);               // copy #
#endif

    */






    // ND280Log("Basket Inner Width: " << GetInnerWidth()/mm << " mm"
    //          << "   Total Width: " << GetOuterWidth()/mm << " mm");
    // ND280Log("Basket Inner Height: " << GetInnerHeight()/mm << " mm"
    //          << "   Total Height: " << GetOuterHeight()/mm << " mm");
    // ND280Log("Basket Inner Length: " << GetInnerLength()
    //          << "   Total Length: " << GetOuterLength()/mm << " mm");

    G4cout << "Basket Inner Width: " << GetInnerWidth()/mm << " mm"
	   << "   Total Width: " << GetOuterWidth()/mm << " mm" << G4endl;
    G4cout << "Basket Inner Height: " << GetInnerHeight()/mm << " mm"
	   << "   Total Height: " << GetOuterHeight()/mm << " mm" << G4endl;
    G4cout << "Basket Inner Length: " << GetInnerLength()
	   << "   Total Length: " << GetOuterLength()/mm << " mm" << G4endl;

  }
  
  



    /*
    
    //////////////////////////////////////////
    // CONSTRUCT THE PI-ZERO DETECTOR
    //////////////////////////////////////////
    ND280P0DConstructor& p0d = Get<ND280P0DConstructor>("P0D");
    G4LogicalVolume* p0dVolume = p0d.GetPiece();

    if (fP0DPosition>10*meter) {
        fP0DPosition = -GetInnerLength()/2;
        fP0DPosition += p0d.GetUpstreamSpace();
        fP0DPosition += p0d.GetLength()/2;
    }

    ND280Log("P0D Envelope Length: "
                 << p0d.GetLength()/mm << " mm");
    ND280Log("P0D Z Center: " << fP0DPosition / mm << " mm"
             << " from "
             << (fP0DPosition - p0d.GetLength()/2)/mm << " mm"
             << " to " 
             << (fP0DPosition + p0d.GetLength()/2)/mm << " mm");
    
    double p0dDownstreamEnd = fP0DPosition + p0d.GetLength()/2;

    //////////////////////////////////////////
    // CONSTRUCT THE PI-ZERO DETECTOR SUPPORT
    //////////////////////////////////////////
    ND280P0DSupportConstructor& p0dSupport 
      = Get<ND280P0DSupportConstructor>("P0DSupport");

    p0dSupport.SetWidth(GetInnerWidth());
    p0dSupport.SetUSECalSupportLength(p0d.GetUSECalLength());
    p0dSupport.SetUSTargetSupportLength(p0d.GetUSTargetLength());
    p0dSupport.SetCTargetSupportLength(p0d.GetCTargetLength());
    p0dSupport.SetCECalSupportLength(p0d.GetCECalLength());

    G4LogicalVolume* p0dSupportVolume = p0dSupport.GetPiece();
    double supportTop = -GetOuterHeight()/2 + GetP0DSupportStructureHeight();
    new G4PVPlacement(0,        // rotation
                      G4ThreeVector(0,
                                    supportTop - p0dSupport.GetHeight()/2,
                                    fP0DPosition),
                      p0dSupportVolume, // logical volume
                      p0dSupport.GetName(), // name
                      logVolume, // mother  volume
                      false,    // no boolean operations
                      0);       // not a copy.

    double p0dXPosition = (-GetInnerWidth()/2 
                           +p0d.GetLISpace()
                           +p0d.GetWidth()/2);
    double p0dXLISpace = GetInnerWidth()/2 + (p0dXPosition - p0d.GetWidth()/2);
    double p0dXElecSpace = GetInnerWidth()/2 - (p0dXPosition+p0d.GetWidth()/2);

    double p0dYPosition = (-GetOuterHeight()/2
                           +GetP0DSupportHeight() 
                           +p0d.GetBottomSpace()
                           +p0d.GetHeight()/2);

    ND280Log("P0D X Position: " << p0dXPosition << " mm"
             << " LI space: " << p0dXLISpace
             << " Electronics space: " << p0dXElecSpace);
    ND280Log("P0D Y Position: " << p0dYPosition << " mm");
    
    // Put the P0D on top of the support.
    new G4PVPlacement(0,        // rotation
                      G4ThreeVector(p0dXPosition,
                                    p0dYPosition,
                                    fP0DPosition),
                      p0dVolume, // logical volume
                      p0d.GetName(), // name
                      logVolume, // mother  volume
                      false,    // no boolean operations
                      0);       // not a copy.
  */











 
  
  ///////////////////////////////////////////
  /// Set the centerline position tracker.  The centerline position is
  /// adjusted so that as the basket is moved in the global coordinate
  /// system, the tracker remains at a fixed global coordinate.
  ///////////////////////////////////////////
  const G4double centerLineX = -GetDeflectedCenter().x();
  const G4double centerLineY = 30*mm - GetDeflectedCenter().y();
  
  //////////////////////////////////////////
  // CONSTRUCT THE DOWNSTREAM ECAL INSIDE BASKET
  //////////////////////////////////////////
  
  if( GetND280XML()->GetXMLUseDsECal() ){    
    ND280DsECalConstructor& dsECal 
      = Get<ND280DsECalConstructor>("DsECal");
    
    G4LogicalVolume* dsECalVolume = dsECal.GetPiece();
    
    dsECalVolume->SetVisAttributes(G4VisAttributes::Invisible);
    
    if (dsECal.GetWidth() > GetInnerWidth()) {
      // ND280Error(dsECal.GetName());
      // ND280Error(" is "
      //           << dsECal.GetWidth()/cm
      //           << " cm wide with "
      //           << GetInnerWidth()/cm << " cm"
      //           << " available");
      // G4Exception("ND280BasketConstructor:: DSECal volume too wide");
      
      G4ExceptionDescription msg;
      msg << dsECal.GetName()
	  << " is "
	  << dsECal.GetWidth()/cm
	  << " cm wide with "
	  << GetInnerWidth()/cm << " cm"
	  << " available" << G4endl
	  << "ND280BasketConstructor:: DSECal volume too wide"
	  << G4endl;
      G4Exception("ND280BasketConstructor::GetPiece",
		  "MyCode0002",FatalException, msg);

    }

    if (dsECal.GetHeight() > GetInnerHeight()) {
      // ND280Error(dsECal.GetName());
      // ND280Error(" is "
      //           << dsECal.GetHeight()/cm
      //           << " cm height with "
      //           << GetInnerHeight()/cm << " cm"
      //           << " available");
      // G4Exception("ND280BasketConstructor:: DSECal volume too high");
      
      G4ExceptionDescription msg;
      msg << dsECal.GetName()
	  << " is "
	  << dsECal.GetHeight()/cm
	  << " cm height with "
	  << GetInnerWidth()/cm << " cm"
	  << " available" << G4endl
	  << "ND280BasketConstructor:: DSECal volume too high"
	  << G4endl;
      G4Exception("ND280BasketConstructor::GetPiece",
		  "MyCode0002",FatalException, msg);
    }
    
    //
    // Rotation
    //

    G4RotationMatrix DsECalRot;
    if( dsECal.GetRotationAngle() != 0.0 ) {
		DsECalRot = G4RotationMatrix(dsECal.GetRotationAxis(), 
                                             dsECal.GetRotationAngle());
	}
	else if( fDsECalRotAng != 0.0 ) {
	  // ND280Warn("DS ECal rotation being set by Basket.");
	  // ND280Warn("- This has been deprecated.");
	  // ND280Warn("- Set rotation in DS ECal instead");
	  G4cout << "DS ECal rotation being set by Basket." << G4endl;
	  G4cout << "- This has been deprecated." << G4endl;
	  G4cout << "- Set rotation in DS ECal instead" << G4endl;
	  DsECalRot = G4RotationMatrix(GetDsECalRotAxis(), 
				       GetDsECalRotAng());
	}
	else {
	  DsECalRot = G4RotationMatrix(G4ThreeVector(0.0), 0.0);
	}
    
    //
    // Position
    //
    
    G4ThreeVector DsECalPos(centerLineX, centerLineY-30.0*mm, 0.0);
    if( dsECal.GetExtraOffset().mag() != 0.0 ) {
        DsECalPos += dsECal.GetOffset();
    }
    else if ((fDsECalPosition != 0.0)||(fDsECalVerticalPosition != 0.0)) {
      // ND280Warn("DS ECal translation being set by Basket.");
      // ND280Warn("- This has been deprecated.");
      // ND280Warn("- Set translation in DS ECal instead");
      G4cout << "DS ECal translation being set by Basket." << G4endl;
      G4cout << "- This has been deprecated." << G4endl;
      G4cout << "- Set translation in DS ECal instead" << G4endl;
      DsECalPos.setY( DsECalPos.y() + fDsECalVerticalPosition );
      DsECalPos.setZ( DsECalPos.z() + fDsECalPosition );
    }
    else {
        DsECalPos += dsECal.GetOffset();
    }
	
    const double dsECalUpstreamEnd   
        = DsECalPos.z() - (dsECal.GetLength() / 2.0);
    const double dsECalDownstreamEnd 
        = DsECalPos.z() + (dsECal.GetLength() / 2.0);
    
    if (dsECalVolume) {
    
      new G4PVPlacement(G4Transform3D(DsECalRot,DsECalPos), 
			// rotation + position
			dsECalVolume, 		// logical volume
			dsECal.GetName(),	// name
			logVolume, 			// mother  volume
			false,    			// no boolean operations
			0);       			// not a copy.

        // ND280Log("Downstream ECal Envelope Length: " 
        //          << dsECal.GetLength()/mm << " mm");
        // ND280Log("Downstream ECal Z Center: " 
        //          << DsECalPos.z() / mm << " mm"
        //          << " from "
        //          << dsECalUpstreamEnd/mm << " mm"
        //          << " to " 
        //          << dsECalDownstreamEnd/mm << " mm");
        // ND280Log("Downstream ECal X Position: " << DsECalPos.x()/mm << " mm");
        // ND280Log("Downstream ECal Y Position: " << DsECalPos.y()/mm << " mm");
      
        G4cout << "Downstream ECal Envelope Length: " 
	       << dsECal.GetLength()/mm << " mm"
	       << G4endl
	       << "Downstream ECal Z Center: " 
	       << DsECalPos.z() / mm << " mm"
	       << " from "
	       << dsECalUpstreamEnd/mm << " mm"
	       << " to " 
	       << dsECalDownstreamEnd/mm << " mm" 
	       << G4endl
	       << "Downstream ECal X Position: " << DsECalPos.x()/mm << " mm"
	       << G4endl
	       << "Downstream ECal Y Position: " << DsECalPos.y()/mm << " mm"
	       << G4endl;

    }
    else {
        // ND280Log("**************************************");
        // ND280Log("*** Downstream ECal not constructed");
        // ND280Log("**************************************");
      G4cout << "**************************************"
	     << "*** Downstream ECal not constructed"
	     << "**************************************"
	     << G4endl;
    }

    if (dsECalDownstreamEnd > GetInnerLength()/2) {
        // ND280Error(dsECal.GetName());
        // ND280Error(" downstream end is at "
        //            << dsECalDownstreamEnd/mm
        //            << " mm, but inner basket ends at "
        //            << GetInnerWidth()/mm << " mm");
        // G4Exception("ND280BasketConstructor:: volume out of basket");

      G4ExceptionDescription msg;
      msg << dsECal.GetName()
	  << " downstream end is at "
	  << dsECalDownstreamEnd/mm
	  << " mm, but inner basket ends at " 
	  << GetInnerWidth()/mm << " mm"
	  << "ND280BasketConstructor:: volume out of basket"
	  << G4endl;
      G4Exception("ND280BasketConstructor::GetPiece",
		  "MyCode0002",FatalException, msg);
    }

    // if (dsECalUpstreamEnd < p0dDownstreamEnd) {
    //   // ND280Error(dsECal.GetName());
    //   // ND280Error(" upstream  end is at "
    //   //            << dsECalUpstreamEnd/mm
    //   //            << " mm, but collides with P0D at "
    //   //            << p0dDownstreamEnd/mm << " mm");
    //   // G4Exception("ND280BasketConstructor:: volume collision");
    //   G4ExceptionDescription msg;
    //   msg << dsECal.GetName()
    // 	  << " upstream end is at "
    // 	  << dsECalUpstreamEnd/mm
    // 	  << " mm, but collides with P0D at "
    // 	  << p0dDownstreamEnd/mm << " mm"
    // 	  << "ND280BasketConstructor:: volume collision"
    // 	  << G4endl;
    //   G4Exception("ND280BasketConstructor::GetPiece",
    // 		  "MyCode0002",FatalException, msg);
    // }

    if (dsECalUpstreamEnd > GetInnerLength()/2) {
      // ND280Error(dsECal.GetName());
      // ND280Error(" upstream  end is at "
      // 		 << dsECalUpstreamEnd/mm
      // 		 << " mm, but collides with basket at "
      // 		 << GetInnerLength()/mm << " mm");
      // G4Exception("ND280BasketConstructor:: volume collision");
      
      G4ExceptionDescription msg;
      msg << dsECal.GetName()
	  << " downstream end is at "
	  << dsECalUpstreamEnd/mm
	  << " mm, but collides basket at " 
	  << GetInnerLength()/mm << " mm"
	  << "ND280BasketConstructor:: volume collision"
	  << G4endl;
      G4Exception("ND280BasketConstructor::GetPiece",
		  "MyCode0002",FatalException, msg); 
    }
   
  } // if( GetND280XML()->GetXMLUseDsECal() )







    /*

    //////////////////////////////////////////
    // CONSTRUCT THE TRACKING DETECTOR
    //////////////////////////////////////////

    ND280TrackerConstructor& trk = Get<ND280TrackerConstructor>("Tracker");
    G4LogicalVolume* trkVolume = trk.GetPiece();

    if (trk.GetWidth() > GetInnerWidth()) {
        ND280Error(trk.GetName());
        ND280Error(" is "
                  << trk.GetWidth()/mm
                  << " mm wide with "
                  << GetInnerWidth()/mm << " mm"
                  << " available");
        G4Exception("ND280BasketConstructor:: tracker too wide");
    }

    if (trk.GetHeight() > GetInnerHeight()) {
        ND280Error(trk.GetName());
        ND280Error(" is "
                  << trk.GetHeight()/mm
                  << " mm high with "
                  << GetInnerHeight()/mm << " mm"
                  << " available");
        G4Exception("ND280BasketConstructor:: tracker too high");
    }

    //////////////////////////////////////////
    // FIND POSITION OF THE TRACKER.
    //////////////////////////////////////////

    const double trkAvailableLength = dsECalUpstreamEnd - p0dDownstreamEnd;

    if (trk.GetLength() > trkAvailableLength) {
        ND280Error(trk.GetName());
        ND280Error(" is "
                  << trk.GetLength()/mm
                  << " mm long with "
                   << trkAvailableLength/mm << " mm"
                  << " available");
        G4Exception("ND280BasketConstructor:: tracker too long");
    }

    if (trkVolume) {
        if (fTrackerPosition>10*meter) {
            fTrackerPosition = (p0dDownstreamEnd + dsECalUpstreamEnd)/2;
        }

        new G4PVPlacement(0,        // rotation
                          G4ThreeVector(centerLineX,
                                        centerLineY,
                                        fTrackerPosition), // position
                          trkVolume, // logical volume
                          trk.GetName(), // name
                          logVolume, // mother  volume
                          false,    // no boolean operations
                          0);       // not a copy.

        ND280Log("Tracker Envelope Length: "
                 << trk.GetLength()/mm << " mm");
        ND280Log("Tracker Z Center: " 
                 << fTrackerPosition / mm << " mm"
                 << " from "
                 << (fTrackerPosition - trk.GetLength()/2)/mm << " mm"
                 << " to " 
                 << (fTrackerPosition + trk.GetLength()/2)/mm << " mm");
        ND280Log("Tracker X Position: " << centerLineX/mm << " mm");
        ND280Log("Tracker Y Position: " << centerLineY/mm << " mm");

        if (trk.GetFGD1Position()!=0.)
           SetFGD1Position(fTrackerPosition+trk.GetFGD1Position());
        if (trk.GetFGD2Position()!=0.)
           SetFGD2Position(fTrackerPosition+trk.GetFGD2Position());

    }
    else {
        ND280Log("******************************");
        ND280Log("*** Tracker not constructed");
        ND280Log("******************************");
    }

    */
  
  return logVolume;
}
