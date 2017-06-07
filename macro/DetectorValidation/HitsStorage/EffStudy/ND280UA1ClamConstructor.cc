#include <globals.hh>
#include <G4Box.hh>
#include <G4SubtractionSolid.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4AssemblyVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4TransportationManager.hh>
#include <G4FieldManager.hh>
#include <G4UnitsTable.hh>

//#include <TND280Log.hxx>

//#include "ND280MagneticField.hh"
#include "ND280UA1ClamConstructor.hh"
#include "ND280UA1CoilConstructor.hh"
#include "ND280UA1FluxReturnConstructor.hh"

#include "ecal/ND280P0DECalConstructor.hh"
#include "ecal/ND280BrlECalConstructor.hh"

class ND280UA1ClamMessenger: public ND280ConstructorMessenger {
private:
    ND280UA1ClamConstructor* fConstructor;

    G4UIcmdWithABool* fBuildTopBrlECalCMD;
    G4UIcmdWithABool* fBuildSideBrlECalCMD;
    G4UIcmdWithABool* fBuildBottomBrlECalCMD;
    G4UIcmdWithABool* fBuildTopP0DECalCMD;
    G4UIcmdWithABool* fBuildSideP0DECalCMD;
    G4UIcmdWithABool* fBuildBottomP0DECalCMD;

public:
    ND280UA1ClamMessenger(ND280UA1ClamConstructor* c) 
        : ND280ConstructorMessenger(c,"Control the magnet geometry."),
          fConstructor(c)  { 

	fBuildTopBrlECalCMD 
            = new G4UIcmdWithABool(CommandName("buildTopBrlECal"), this);
	fBuildTopBrlECalCMD->SetParameterName("build", false);
	fBuildTopBrlECalCMD->SetGuidance(
            "Set true to build the ecal (default: true).");

	fBuildSideBrlECalCMD 
            = new G4UIcmdWithABool(CommandName("buildSideBrlECal"), this);
	fBuildSideBrlECalCMD->SetParameterName("build", false);
	fBuildSideBrlECalCMD->SetGuidance(
            "Set true to build the ecal (default: true).");

	fBuildBottomBrlECalCMD 
            = new G4UIcmdWithABool(CommandName("buildBottomBrlECal"), this);
	fBuildBottomBrlECalCMD->SetParameterName("build", false);
	fBuildBottomBrlECalCMD->SetGuidance(
            "Set true to build the ecal (default: true).");

	fBuildTopP0DECalCMD 
            = new G4UIcmdWithABool(CommandName("buildTopP0DECal"), this);
	fBuildTopP0DECalCMD->SetParameterName("build", false);
	fBuildTopP0DECalCMD->SetGuidance(
            "Set true to build the ecal (default: true).");

	fBuildSideP0DECalCMD 
            = new G4UIcmdWithABool(CommandName("buildSideP0DECal"), this);
	fBuildSideP0DECalCMD->SetParameterName("build", false);
	fBuildSideP0DECalCMD->SetGuidance(
            "Set true to build the ecal (default: true).");

	fBuildBottomP0DECalCMD 
            = new G4UIcmdWithABool(CommandName("buildBottomP0DECal"), this);
	fBuildBottomP0DECalCMD->SetParameterName("build", false);
	fBuildBottomP0DECalCMD->SetGuidance(
            "Set true to build the ecal (default: true).");

    }

    virtual ~ND280UA1ClamMessenger() {
        delete fBuildTopBrlECalCMD;
        delete fBuildSideBrlECalCMD;
        delete fBuildBottomBrlECalCMD;
        delete fBuildTopP0DECalCMD;
        delete fBuildSideP0DECalCMD;
        delete fBuildBottomP0DECalCMD;
    }

    void SetNewValue(G4UIcommand *cmd, G4String val) {
        if (cmd == fBuildTopBrlECalCMD ) {
            fConstructor->SetBuildTopBrlECal(
                fBuildTopBrlECalCMD->GetNewBoolValue(val));
        }
        else if (cmd == fBuildSideBrlECalCMD ) {
            fConstructor->SetBuildSideBrlECal(
                fBuildSideBrlECalCMD->GetNewBoolValue(val));
        }
        else if (cmd == fBuildBottomBrlECalCMD ) {
            fConstructor->SetBuildBottomBrlECal(
                fBuildBottomBrlECalCMD->GetNewBoolValue(val));
        }
        else if (cmd == fBuildTopP0DECalCMD ) {
            fConstructor->SetBuildTopP0DECal(
                fBuildTopP0DECalCMD->GetNewBoolValue(val));
        }
        else if (cmd == fBuildSideP0DECalCMD ) {
            fConstructor->SetBuildSideP0DECal(
                fBuildSideP0DECalCMD->GetNewBoolValue(val));
        }
        else if (cmd == fBuildBottomP0DECalCMD ) {
            fConstructor->SetBuildBottomP0DECal(
                fBuildBottomP0DECalCMD->GetNewBoolValue(val));
        }
        else {
            ND280ConstructorMessenger::SetNewValue(cmd,val);
        }
    }
};

ND280UA1ClamConstructor::~ND280UA1ClamConstructor() {}

void ND280UA1ClamConstructor::Init(void) {
    
  ////////
  //
  // NEW ND280UPGRADE
  //
  // Take XML inputs
  ND280RootPersistencyManager* InputPersistencyManager
    = ND280RootPersistencyManager::GetInstance();
  fND280XMLInput = InputPersistencyManager->GetXMLInput();  

  // Set visible / invisible Magnet
  if( GetND280XML()->GetXMLInvisMagnet() ) SetVisible(false);
  else SetVisible(true);

  ////////


  fWidth = 5710*mm/2;
  fHeight = 6150*mm;
  fLength = 7723*mm;

    fInteriorGap = 80*mm;
    fExteriorGap = 110*mm;

    fCenterCoilGap = 200*mm;
    fEdgeCoilGap = 94*mm;

    fIsLeftClamshell = true;

    SetBuildTopBrlECal(true);
    SetBuildSideBrlECal(true);
    SetBuildBottomBrlECal(true);
    SetBuildTopP0DECal(true);
    SetBuildSideP0DECal(true);
    SetBuildBottomP0DECal(true);
    
    AddConstructor(new ND280UA1CoilConstructor("Coil",this));
    
    AddConstructor(new ND280UA1FluxReturnConstructor("FluxReturn",this));
    ND280UA1FluxReturnConstructor& fluxReturn = 
        Get<ND280UA1FluxReturnConstructor>("FluxReturn");
    fluxReturn.SetIsLefthandReturn(true);
    
    if( GetND280XML()->GetXMLUseBrlECal() ){    
      AddConstructor(new ND280BrlECalConstructor("BrlECal", this));
    }
    if( GetND280XML()->GetXMLUseP0DECal() ){    
      AddConstructor(new ND280P0DECalConstructor("P0DECal", this));
    }
    
    SetMessenger(new ND280UA1ClamMessenger(this));
}

double ND280UA1ClamConstructor::GetInnerLength() {
    ND280UA1CoilConstructor& coil = 
        Get<ND280UA1CoilConstructor>("Coil");
    return coil.GetCoilInnerLength();
}

G4LogicalVolume *ND280UA1ClamConstructor::GetPiece(void) {
    // Construct the logical volumes first, and in the right order so that the
    // object dimensions can be calculated.
    
    //////////////////////////////////////////////////////////////
    // Construct the coil assemblies.
    ND280UA1CoilConstructor& coil
        = Get<ND280UA1CoilConstructor>("Coil");
    G4AssemblyVolume* coilAssembly;
    if( GetND280XML()->GetXMLUseMagnet() ){    
      coilAssembly = coil.GetAssembly();
    }
    
    //////////////////////////////////////////////////////////////
    // Construct assemblies for the flux return yokes.
    ND280UA1FluxReturnConstructor& interiorReturn
        = Get<ND280UA1FluxReturnConstructor>("FluxReturn");
    ND280UA1FluxReturnConstructor& exteriorReturn
        = Get<ND280UA1FluxReturnConstructor>("FluxReturn");



    ////////////////////////////////////////////////////////////
    // Construct the bounding volume for the flux return C's, the coils and
    // the barrel ecals (around tracker and P0D).
    ////////////////////////////////////////////////////////////
    double trackerVoidXOffset = GetWidth()/2 - GetTrackerVoidWidth()/2;
    double p0dVoidXOffset = GetWidth()/2 - GetP0DVoidWidth()/2;
    double p0dVoidZOffset = -GetBasketLength()/2 + GetP0DVoidLength()/2;
    
    G4ThreeVector trackerVoidOffset;
    G4ThreeVector p0dVoidOffset;
    if (fIsLeftClamshell) {
        trackerVoidOffset.setX(-trackerVoidXOffset);
        p0dVoidOffset.setX(-p0dVoidXOffset);
        p0dVoidOffset.setZ(p0dVoidZOffset+1*mm);
    }
    else {
        trackerVoidOffset.setX(trackerVoidXOffset);
        p0dVoidOffset.setX(p0dVoidXOffset);
        p0dVoidOffset.setZ(p0dVoidZOffset+1*mm);
    }

    // Build the inner void for the basket allowing enough room for the
    // tracker services.  This is subtracted from the outer envelope The space
    // for the P0D services is subtracted separatedly.
    // The -15mm for the y void is to make room for barrel ECal alignment
    G4VSolid *trackerVoid 
        = new G4SubtractionSolid(GetName(),
                                 new G4Box("OuterEnvelop",GetWidth()/2.0,
                                           GetHeight()/2.0,
                                           GetLength()/2.0),
                                 new G4Box("TrackerVoid",
                                           GetTrackerVoidWidth()/2.0+1*mm,
                                           GetTrackerVoidHeight()/2.0-15.*mm,
                                           GetInnerLength()/2.0),
                                 NULL,
                                 trackerVoidOffset);
    
    // Build the void for the P0D and its services.  This is subtracted from
    // the trackerVoid which already allows space for the basket.
    G4VSolid *clamSolid 
        = new G4SubtractionSolid(GetName(),
                                 trackerVoid, 
                                 new G4Box("P0DVoid",
                                           GetP0DVoidWidth()/2.0+1*mm,
                                           GetP0DVoidHeight()/2.0,
                                           GetP0DVoidLength()/2.0),
                                 NULL,
                                 p0dVoidOffset);
    G4LogicalVolume *logVolume
        = new G4LogicalVolume(clamSolid,
                              FindMaterial("Air"),
                              GetName());
    if (GetVisible()) {
        G4VisAttributes *visual = new G4VisAttributes();
        visual->SetColor(0.1,0.1,0.1,1); 
        visual->SetForceWireframe(true);
        logVolume->SetVisAttributes(visual);
    } 
    else {
        logVolume->SetVisAttributes(G4VisAttributes::Invisible);
    }

  
    /////////////////////////////////////////////
    // Construct the coils for the clamshell.
    /////////////////////////////////////////////
    G4ThreeVector coilPosition;;
    G4RotationMatrix *rotCoil;

    rotCoil = new G4RotationMatrix();
    rotCoil->rotateY(90*degree);

    G4ThreeVector centerCoilPosition;
    G4ThreeVector edgeCoilPosition;
    if (fIsLeftClamshell) {
        centerCoilPosition.set(-GetWidth()/2 
                               + fCenterCoilGap/2
                               + coil.GetCoilWidth()/2,
                               0,0);
        edgeCoilPosition.set(-GetWidth()/2 
                             + fCenterCoilGap/2
                             + coil.GetCoilWidth()
                             + fEdgeCoilGap
                             + coil.GetCoilWidth()/2,
                             0,0);
    }
    else {
        centerCoilPosition.set(GetWidth()/2 
                               - fCenterCoilGap/2
                               - coil.GetCoilWidth()/2,
                               0,0);
        edgeCoilPosition.set(GetWidth()/2 
                             - fCenterCoilGap/2
                             - coil.GetCoilWidth()
                             - fEdgeCoilGap
                             - coil.GetCoilWidth()/2,
                             0,0);
    }

    if( GetND280XML()->GetXMLUseMagnet() ){    
      coilAssembly->MakeImprint(logVolume,centerCoilPosition,rotCoil);
      coilAssembly->MakeImprint(logVolume,edgeCoilPosition,rotCoil);
    }

    /////////////////////////////////////////
    // Construct the flux return C's for the magnet.
    /////////////////////////////////////////

    G4RotationMatrix *fluxReturnRotation = new G4RotationMatrix();
    G4ThreeVector fluxReturnPosition;

    if (fIsLeftClamshell) {
        fluxReturnPosition.setX(GetWidth()/2
                                -interiorReturn.GetFluxReturnThickness());
        fluxReturnRotation->rotateZ(90*degree);
    }
    else {
        fluxReturnPosition.setX(-GetWidth()/2
                                +interiorReturn.GetFluxReturnThickness());
        fluxReturnRotation->rotateZ(-90*degree);
    }

    // Do the flux return modules from upstream to downstream
    double fluxReturnOffset = 
        -0.5*(2*exteriorReturn.GetWidth() + 2*fExteriorGap
              + 6*interiorReturn.GetWidth() + 5*fInteriorGap);
    double magnetLength = -2.0 * fluxReturnOffset;

    fluxReturnOffset += 0.5*exteriorReturn.GetWidth();
    fluxReturnPosition.setZ(fluxReturnOffset);

    exteriorReturn.SetYokeIndex(0);
    exteriorReturn.SetIsLefthandReturn(fIsLeftClamshell);
    if( GetND280XML()->GetXMLUseMagnet() ){    
      exteriorReturn.GetAssembly()->MakeImprint(logVolume,
						fluxReturnPosition,
						fluxReturnRotation);
    }
    
    fluxReturnOffset += 0.5*exteriorReturn.GetWidth();
    fluxReturnOffset += 0.5*interiorReturn.GetWidth();
    fluxReturnOffset += fExteriorGap;
    fluxReturnPosition.setZ(fluxReturnOffset);
    interiorReturn.SetYokeIndex(1);
    interiorReturn.SetIsLefthandReturn(fIsLeftClamshell);
    if( GetND280XML()->GetXMLUseMagnet() ){    
      interiorReturn.GetAssembly()->MakeImprint(logVolume,
						fluxReturnPosition,
						fluxReturnRotation);
    }
    
    fluxReturnOffset += interiorReturn.GetWidth();
    fluxReturnOffset += fInteriorGap;
    fluxReturnPosition.setZ(fluxReturnOffset);
    interiorReturn.SetYokeIndex(2);
    interiorReturn.SetIsLefthandReturn(fIsLeftClamshell);
    if( GetND280XML()->GetXMLUseMagnet() ){    
      interiorReturn.GetAssembly()->MakeImprint(logVolume,
						fluxReturnPosition,
						fluxReturnRotation);
    }

    fluxReturnOffset += interiorReturn.GetWidth();
    fluxReturnOffset += fInteriorGap;
    fluxReturnPosition.setZ(fluxReturnOffset);
    interiorReturn.SetYokeIndex(3);
    interiorReturn.SetIsLefthandReturn(fIsLeftClamshell);
    if( GetND280XML()->GetXMLUseMagnet() ){    
      interiorReturn.GetAssembly()->MakeImprint(logVolume,
						fluxReturnPosition,
						fluxReturnRotation);
    }
    
    fluxReturnOffset += interiorReturn.GetWidth();
    fluxReturnOffset += fInteriorGap;
    fluxReturnPosition.setZ(fluxReturnOffset);
    interiorReturn.SetYokeIndex(4);
    interiorReturn.SetIsLefthandReturn(fIsLeftClamshell);
    if( GetND280XML()->GetXMLUseMagnet() ){    
      interiorReturn.GetAssembly()->MakeImprint(logVolume,
						fluxReturnPosition,
						fluxReturnRotation);
    }

    fluxReturnOffset += interiorReturn.GetWidth();
    fluxReturnOffset += fInteriorGap;
    fluxReturnPosition.setZ(fluxReturnOffset);
    interiorReturn.SetYokeIndex(5);
    interiorReturn.SetIsLefthandReturn(fIsLeftClamshell);
    if( GetND280XML()->GetXMLUseMagnet() ){    
      interiorReturn.GetAssembly()->MakeImprint(logVolume,
						fluxReturnPosition,
						fluxReturnRotation);
    }

    fluxReturnOffset += interiorReturn.GetWidth();
    fluxReturnOffset += fInteriorGap;
    fluxReturnPosition.setZ(fluxReturnOffset);
    interiorReturn.SetYokeIndex(6);
    interiorReturn.SetIsLefthandReturn(fIsLeftClamshell);
    if( GetND280XML()->GetXMLUseMagnet() ){    
      interiorReturn.GetAssembly()->MakeImprint(logVolume,
						fluxReturnPosition,
						fluxReturnRotation);
    }

    fluxReturnOffset += 0.5*interiorReturn.GetWidth();
    fluxReturnOffset += 0.5*exteriorReturn.GetWidth();
    fluxReturnOffset += fExteriorGap;
    fluxReturnPosition.setZ(fluxReturnOffset);
    exteriorReturn.SetYokeIndex(7);
    exteriorReturn.SetIsLefthandReturn(fIsLeftClamshell);
    if( GetND280XML()->GetXMLUseMagnet() ){    
      exteriorReturn.GetAssembly()->MakeImprint(logVolume,
						fluxReturnPosition,
						fluxReturnRotation);
    }
    
    ////////////////////////////////////////////////////////////////
    // ECALS
    ////////////////////////////////////////////////////////////////
    // NOTE: Offsets are returned with their signs already set
    // appropriately, so they should always be added to the position.


    ////////////////////////////////////////////////////////////
    // Position the P0D ECAL MODULES
    ////////////////////////////////////////////////////////////
 
    // Measure of the width left available to the P0D
    double pecalVoidWidth = interiorReturn.GetInnerArmLength();
    
    // Measure of the height left available to the P0D
    double pecalVoidHeight = interiorReturn.GetInnerHeight();

    if( GetND280XML()->GetXMLUseP0DECal() ){    
      
      // P0D TOP ECal
      ND280P0DECalConstructor& pECalTop =
        Get<ND280P0DECalConstructor>("P0DECal");
      pECalTop.SetModule(ND280ECal::kTop);
      if(fIsLeftClamshell) pECalTop.SetLeftClamshell();
      else pECalTop.SetRightClamshell();
      G4LogicalVolume* pECalTopVolume = pECalTop.GetPiece();
      
      // X position is measured from the magnet wall
      double pECalTopX = (GetWidth() / 2.0)
        - interiorReturn.GetFluxReturnThickness()
        - (pECalTop.GetWidth() / 2.0);
      if(!fIsLeftClamshell) pECalTopX = -pECalTopX;
      
      // Y position is measured from the top magnet arm
      double pECalTopY = (interiorReturn.GetInnerHeight() / 2.0)
        - (pECalTop.GetHeight() / 2.0);
      
      // Z position is measured from the upstream end of the magent
      double pECalTopZ = - (magnetLength / 2.0)
        + (pECalTop.GetLength() / 2.0);
      
      G4ThreeVector pECalTopPos(pECalTopX, pECalTopY, pECalTopZ);
      pECalTopPos += pECalTop.GetOffset();
      
      if (GetBuildTopP0DECal()) {
	new G4PVPlacement(0,
			  pECalTopPos,
			  pECalTopVolume,
			  pECalTop.GetName(),
			  logVolume,
			  false,
			  0);
      }
      
      pecalVoidHeight -= ( pECalTop.GetHeight() + pECalTop.GetOffset().y() );
      
      // P0D SIDE ECal
      ND280P0DECalConstructor& pECalSide =
        Get<ND280P0DECalConstructor>("P0DECal");
      pECalSide.SetModule(ND280ECal::kSide);
      if(fIsLeftClamshell) pECalSide.SetLeftClamshell();
      else pECalSide.SetRightClamshell();
      G4LogicalVolume* pECalSideVolume = pECalSide.GetPiece();
      
      // X position is measured from the magnet wall
      double pECalSideX = (GetWidth() / 2.0)
        - interiorReturn.GetFluxReturnThickness()
        - (pECalSide.GetWidth() / 2.0);
      if(!fIsLeftClamshell) pECalSideX = -pECalSideX;
      
      // Y position is measured from the top magnet arm
      double pECalSideY = (interiorReturn.GetInnerHeight() / 2.0)
        - (pECalSide.GetHeight() / 2.0);
      
      // Z position is measured from the upstream end of the magent
      double pECalSideZ = - (magnetLength / 2.0)
        + (pECalSide.GetLength() / 2.0);
      
      G4ThreeVector pECalSidePos(pECalSideX, pECalSideY, pECalSideZ);
      pECalSidePos += pECalSide.GetOffset();
      
      if (GetBuildSideP0DECal()) {
	new G4PVPlacement(0,
			  pECalSidePos,
			  pECalSideVolume,
			  pECalSide.GetName(),
			  logVolume,
			  false,
			  0);
      }
      
      pecalVoidWidth -= ( pECalSide.GetWidth() + pECalSide.GetOffset().x() );
      
      // P0D BOTTOM ECal
      ND280P0DECalConstructor& pECalBottom =
        Get<ND280P0DECalConstructor>("P0DECal");
      pECalBottom.SetModule(ND280ECal::kBottom);
      if(fIsLeftClamshell) pECalBottom.SetLeftClamshell();
      else pECalBottom.SetRightClamshell();
      G4LogicalVolume* pECalBottomVolume = pECalBottom.GetPiece();
      
      // X position is measured from the magnet wall
      double pECalBottomX = (GetWidth() / 2.0)
        - interiorReturn.GetFluxReturnThickness()
        - (pECalBottom.GetWidth() / 2.0);
      if(!fIsLeftClamshell) pECalBottomX = -pECalBottomX;
      
      // Y position is measured from the bottom magnet arm
      double pECalBottomY = - (interiorReturn.GetInnerHeight() / 2.0)
        + (pECalBottom.GetHeight() / 2.0);
      
      // Z position is measured from the upstream end of the magnet
      double pECalBottomZ = - (magnetLength / 2.0)
        + (pECalBottom.GetLength() / 2.0);
      
      G4ThreeVector pECalBottomPos(pECalBottomX, pECalBottomY, pECalBottomZ);
      pECalBottomPos += pECalBottom.GetOffset();
      
      if (GetBuildBottomP0DECal()) {
	new G4PVPlacement(0,
			  pECalBottomPos,
			  pECalBottomVolume,
			  pECalBottom.GetName(),
			  logVolume,
			  false,
			  0);
      }
      
      pecalVoidHeight 
        -= ( pECalBottom.GetHeight() + pECalBottom.GetOffset().y() );
      
      if (pecalVoidWidth < GetP0DVoidWidth()) {
	//ND280Error("P0D Inner width: " 
	//<< pecalVoidWidth
	//<< " < " << GetP0DVoidWidth());
	//G4Exception("P0D ECal outside of envelope");
	
	G4cout << "P0D Inner width: " 
	       << pecalVoidWidth
	       << " < " << GetP0DVoidWidth() << G4endl;
	G4Exception("ND280UA1ClamConstructor::GetPiece",
		    "MyCode0002",FatalException,"P0D ECal outside of envelope");
      }
      if (pecalVoidHeight < GetP0DVoidHeight()) {
	// ND280Error("P0D Inner height: " 
	//            << pecalVoidHeight
	//            << " < " << GetP0DVoidHeight());
	// G4Exception("P0D ECal outside of envelope");
	
	G4cout << "P0D Inner height: " 
	       << pecalVoidHeight
	       << " < " << GetP0DVoidHeight() << G4endl;
	G4Exception("ND280UA1ClamConstructor::GetPiece",
		    "MyCode0002",FatalException,"P0D ECal outside of envelope");
      }
    
    } // if( GetND280XML()->GetXMLUseP0DECal() ){   



    ////////////////////////////////////////////////////////////
    // Position the BARREL ECAL MODULES
    ////////////////////////////////////////////////////////////
 
    // Measure of the width left available to the Tracker
    double becalVoidWidth = interiorReturn.GetInnerArmLength();
    
    // Measure of the height left available to the Tracker
    double becalVoidHeight = interiorReturn.GetInnerHeight();
    
    if( GetND280XML()->GetXMLUseBrlECal() ){   
      
      // Barrel TOP ECal
      ND280BrlECalConstructor& bECalTop =
        Get<ND280BrlECalConstructor>("BrlECal");
      bECalTop.SetModule(ND280ECal::kTop);
      if(fIsLeftClamshell) bECalTop.SetLeftClamshell();
      else bECalTop.SetRightClamshell();
      G4LogicalVolume* bECalTopVolume = bECalTop.GetPiece();
      
      // X position measured from the magnet wall
      double bECalTopX = (GetWidth() / 2.0)
        - interiorReturn.GetFluxReturnThickness()
        - (bECalTop.GetWidth() / 2.0);
      if(!fIsLeftClamshell) bECalTopX = -bECalTopX;
      
      // Y position measured from the top magnet arm
      double bECalTopY = (interiorReturn.GetInnerHeight() / 2.0)
        - (bECalTop.GetHeight() / 2.0);
      
      // Z position measured from the downstream end of the magent
      double bECalTopZ = (magnetLength / 2.0)
        - (bECalTop.GetLength() / 2.0);
      
      G4ThreeVector bECalTopPos(bECalTopX, bECalTopY, bECalTopZ);
      bECalTopPos += bECalTop.GetOffset();
      
      if (GetBuildTopBrlECal()) {
	new G4PVPlacement(0,
			  bECalTopPos,
                          bECalTopVolume,
                          bECalTop.GetName(),
                          logVolume,
                          false,
                          0);
      }
      
      becalVoidHeight -= ( bECalTop.GetHeight() + bECalTop.GetOffset().y() );
      
      // Barrel SIDE ECal
      ND280BrlECalConstructor& bECalSide =
        Get<ND280BrlECalConstructor>("BrlECal");
      bECalSide.SetModule(ND280ECal::kSide);
      if(fIsLeftClamshell) bECalSide.SetLeftClamshell();
      else bECalSide.SetRightClamshell();
      G4LogicalVolume* bECalSideVolume = bECalSide.GetPiece();
      
      // X position measured from the magnet wall
      double bECalSideX = (GetWidth() / 2.0)
        - interiorReturn.GetFluxReturnThickness()
        - (bECalSide.GetWidth() / 2.0);
      if(!fIsLeftClamshell) bECalSideX = -bECalSideX;
      
      // Y position measured from the top magnet arm
      double bECalSideY = (interiorReturn.GetInnerHeight() / 2.0)
        - (bECalSide.GetHeight() / 2.0);
      
      // Z position measured fromt the dowstream end of the magnet
      double bECalSideZ = (magnetLength / 2.0)
        - (bECalSide.GetLength() / 2.0);
      
      G4ThreeVector bECalSidePos(bECalSideX, bECalSideY, bECalSideZ);
      bECalSidePos += bECalSide.GetOffset();
      
      if (GetBuildSideBrlECal()) {
	new G4PVPlacement(0,
                          bECalSidePos,
                          bECalSideVolume,
                          bECalSide.GetName(),
                          logVolume,
                          false,
                          0);
      }
      
      becalVoidWidth -= ( bECalSide.GetWidth() + bECalSide.GetOffset().x() );
      
      // Barrel BOTTOM ECal
      ND280BrlECalConstructor& bECalBottom =
	Get<ND280BrlECalConstructor>("BrlECal");
      bECalBottom.SetModule(ND280ECal::kBottom);
      if(fIsLeftClamshell) bECalBottom.SetLeftClamshell();
      else bECalBottom.SetRightClamshell();
      G4LogicalVolume* bECalBottomVolume = bECalBottom.GetPiece();
      
      // X position measured from the magnet wall
      double bECalBottomX = (GetWidth() / 2.0)
	- interiorReturn.GetFluxReturnThickness()
	- (bECalBottom.GetWidth() / 2.0);
      if(!fIsLeftClamshell) bECalBottomX = -bECalBottomX;
      
      // Y position measured from the bottom magnet arm
      double bECalBottomY = - (interiorReturn.GetInnerHeight() / 2.0)
	+ (bECalBottom.GetHeight() / 2.0);
      
      // Z position measured fron the downstream end of the magnet
      double bECalBottomZ = (magnetLength / 2.0)
	- (bECalBottom.GetLength() / 2.0);
      
      G4ThreeVector bECalBottomPos(bECalBottomX, bECalBottomY, bECalBottomZ);
      bECalBottomPos += bECalBottom.GetOffset();
      
      if (GetBuildBottomBrlECal()) {
	new G4PVPlacement(0,
			  bECalBottomPos,
			  bECalBottomVolume,
			  bECalBottom.GetName(),
			  logVolume,
			  false,
			  0);
      }
      
      becalVoidHeight 
	-= ( bECalBottom.GetHeight() + bECalBottom.GetOffset().y() );
      
      if (becalVoidWidth < GetTrackerVoidWidth()) {
	//ND280Error("Tracker Inner width: " 
	//<< becalVoidWidth
	//<< " < " << GetTrackerVoidWidth());
	//G4Exception("Barrel ECal outside of envelope");
	
	G4cout << "Tracker Inner width: " 
	       << becalVoidWidth
	       << " < " << GetTrackerVoidWidth() << G4endl;
	G4Exception("ND280UA1ClamConstructor::GetPiece",
		    "MyCode0002",FatalException,"Barrel ECal outside of envelope");
      }
      if (becalVoidHeight < GetTrackerVoidHeight()) {
	// ND280Error("Tracker Inner height: " 
	//            << becalVoidHeight
	//            << " < " << GetTrackerVoidHeight());
	// G4Exception("Barrel ECal outside of envelope");
	
	G4cout << "Tracker Inner height: " 
	       << becalVoidHeight
	       << " < " << GetTrackerVoidHeight() << G4endl;
	G4Exception("ND280UA1ClamConstructor::GetPiece",
		    "MyCode0002",FatalException,"Barrel ECal outside of envelope");
      }
      
    } // if( GetND280XML()->GetXMLUseBrlECal() ){   

    
    if (fIsLeftClamshell) {
      //ND280Log("%%%% Construct left magnet clam");
      G4cout << "%%%% Construct left magnet clam" << G4endl;
    }
    else {
      //ND280Log("%%%% Construct right magnet clam");
      G4cout << "%%%% Construct right magnet clam" << G4endl;
    }
    // ND280Log("%%   Magnet full length (along beam direction): "
    //          << magnetLength);
    // ND280Log("%%   Arm length (half available width in Magnet): "
    //          << interiorReturn.GetInnerArmLength());
    // ND280Log("%%    Height inside of coil: "
    //          << coil.GetCoilInnerHeight());
    // ND280Log("%%    Length inside of coil: "
    //          << coil.GetCoilInnerLength());
    // ND280Log("%%    Length available for basket: "
    //          << GetInnerLength());
    // ND280Log("%%    Envelope around tracker region: "
    //          << GetTrackerVoidWidth() << " x " << GetTrackerVoidHeight());
    // ND280Log("%%    Envelope around P0D region: "
    //          << GetP0DVoidWidth() 
    //          << " x " << GetP0DVoidHeight() 
    //          << " x " << GetP0DVoidLength());
    
    G4cout << "%%   Magnet full length (along beam direction): "
	   << magnetLength << G4endl;
    G4cout << "%%   Arm length (half available width in Magnet): "
	   << interiorReturn.GetInnerArmLength() << G4endl;
    G4cout << "%%    Height inside of coil: "
	   << coil.GetCoilInnerHeight() << G4endl;
    G4cout << "%%    Length inside of coil: "
	   << coil.GetCoilInnerLength() << G4endl;
    G4cout << "%%    Length available for basket: "
	   << GetInnerLength() << G4endl;
    G4cout << "%%    Envelope around tracker region: "
	   << GetTrackerVoidWidth() << " x " << GetTrackerVoidHeight() << G4endl;
    G4cout << "%%    Envelope around P0D region: "
	   << GetP0DVoidWidth() 
	   << " x " << GetP0DVoidHeight() 
	   << " x " << GetP0DVoidLength() << G4endl;
    
    if (GetInnerLength() <= GetBasketLength()) {
      //G4Exception("Not enough space for basket");
      
      G4Exception("ND280UA1ClamConstructor::GetPiece",
		  "MyCode0002",FatalException,"Not enough space for basket");
    }
    
    
    
    /*
	
      // NEW ND280UPGRADE
    
    // Pass the magnet size information to the field calculation.
    G4FieldManager* fieldMgr 
        = G4TransportationManager::GetTransportationManager()
        ->GetFieldManager();
    
    const ND280MagneticField* constMag
        = dynamic_cast<const ND280MagneticField*>(fieldMgr->GetDetectorField());
    
    if (!constMag) {
        ND280Warn("### Undefined magnetic field.");
    }
    else {
        ND280MagneticField* mag = const_cast<ND280MagneticField*>(constMag);

        // This needs to be filled with the geometry values.
        double innerWidth = 2*interiorReturn.GetInnerArmLength() 
            + 2*fInteriorGap;
        double outerWidth = 2*interiorReturn.GetOuterArmLength()
            + 2*fInteriorGap;
        double outerHeight = interiorReturn.GetInnerHeight() 
            + 2*interiorReturn.GetFluxReturnThickness();
        mag->SetYokeOuter(outerWidth,
                          outerHeight,
                          coil.GetCoilLength());
        mag->SetYokeInner(innerWidth,
                          interiorReturn.GetInnerHeight(),
                          coil.GetCoilLength());
        mag->SetCoilOuter(innerWidth,
                          coil.GetCoilHeight(),
                          coil.GetCoilLength());
        mag->SetCoilInner(innerWidth,
                          coil.GetCoilInnerHeight(),
                          coil.GetCoilInnerLength());
    }
    */
    
    return logVolume;
}
