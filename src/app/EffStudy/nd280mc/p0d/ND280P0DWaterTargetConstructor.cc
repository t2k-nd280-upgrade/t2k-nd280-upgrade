////////////////////////////////////////////////////////////
// $Id: ND280P0DWaterTargetConstructor.cc,v 1.10 2011/11/29 16:03:18 mcgrew Exp $
//

#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>

//#include <TND280Log.hxx>

#include "p0d/ND280P0DWaterTargetConstructor.hh"

/// Control the parameters of the P0D radiator layers.
class ND280P0DWaterTargetMessenger: public ND280ConstructorMessenger {
private:
    ND280P0DWaterTargetConstructor *fConstructor;
    G4UIcmdWithADoubleAndUnit* fLeftDepthCMD;
    G4UIcmdWithADoubleAndUnit* fRightDepthCMD;
    G4UIcmdWithADoubleAndUnit* fGasketThicknessCMD;

public:
    ND280P0DWaterTargetMessenger(ND280P0DWaterTargetConstructor* c) 
        : ND280ConstructorMessenger(c, "Control a water targetlayer."),
          fConstructor(c) {

        fLeftDepthCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("leftDepth"),this);
        fLeftDepthCMD->SetGuidance("Set water depth in +X target.");
        fLeftDepthCMD->SetParameterName("Depth",false);
        fLeftDepthCMD->SetUnitCategory("Length");

        fRightDepthCMD
            = new G4UIcmdWithADoubleAndUnit(CommandName("rightDepth"),this);
        fRightDepthCMD->SetGuidance("Set water depth in -X target.");
        fRightDepthCMD->SetParameterName("Depth",false);
        fRightDepthCMD->SetUnitCategory("Length");

        fGasketThicknessCMD
            = new G4UIcmdWithADoubleAndUnit(
                CommandName("GasketThickness"),this);
        fGasketThicknessCMD->SetGuidance("Set thickness of gasket in frame.");
        fGasketThicknessCMD->SetParameterName("Thickness",false);
        fGasketThicknessCMD->SetUnitCategory("Length");
    };

    ~ND280P0DWaterTargetMessenger() {
        delete fLeftDepthCMD;
        delete fRightDepthCMD;
        delete fGasketThicknessCMD;
    };

    void SetNewValue(G4UIcommand *cmd, G4String val) {
        if (fLeftDepthCMD == cmd) {
            fConstructor->
                SetLeftWaterDepth(fLeftDepthCMD->GetNewDoubleValue(val));
        }
        else if (fRightDepthCMD == cmd) {
            fConstructor->
                SetRightWaterDepth(fRightDepthCMD->GetNewDoubleValue(val));
        }
        else if (fGasketThicknessCMD == cmd) {
            fConstructor->
                SetGasketThickness(
                    fGasketThicknessCMD->GetNewDoubleValue(val));
        }
        else {
            ND280ConstructorMessenger::SetNewValue(cmd,val);
        }
    };
};

ND280P0DWaterTargetConstructor::~ND280P0DWaterTargetConstructor() {;};

const G4VisAttributes* ND280P0DWaterTargetConstructor::GetVisual(void) const {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(0.8,0.8,0.8,1); 
    return visual;
};

void ND280P0DWaterTargetConstructor::Init() {
    NoBackCover();

    // This gasket thickness is tuned to take all of the "slop" space from the
    // P0Dule, radiator and water target and put it into the water volume.
    SetGasketThickness(0.21*mm);

    // This is the effective thickness of the water bag to account for all of
    // the folds and overlaps in the fiducial volume.  The actual thickness is
    // 0.1524 mm.
    SetBagThickness(0.161*mm);

    SetLeftWaterDepth(GetSideFrameLength()
                      -GetBottomFrameWidth()
                      -GetTopFrameWidth()
                      -75*mm);
    SetRightWaterDepth(GetLeftWaterDepth());

    // Account for the volume that would be taken up by the missing water
    // sensor tubes.
    SetMassCorrection(-0.0158);

    SetMessenger(new ND280P0DWaterTargetMessenger(this));
};

double ND280P0DWaterTargetConstructor::GetLeftWaterDepth() const {
    const double minDepth = 10*mm;
    if (fLeftDepth<minDepth) {
        // ND280Warn("Depth is below level of drain tubes: " 
        //              << fLeftDepth/mm << " mm reset to " 
        //              << minDepth << " mm");
      G4ExceptionDescription msg;
      msg << "Depth is below level of drain tubes: " 
	  << fLeftDepth/mm << " mm reset to " 
	  << minDepth << " mm" << G4endl;
      G4Exception("ND280P0DWaterTargetConstructor::GetLeftWaterDepth",
		  "MyCode0002",JustWarning, msg);
      
        return minDepth;
    }
    return fLeftDepth;
}

double ND280P0DWaterTargetConstructor::GetRightWaterDepth() const {
    const double minDepth = 10*mm;
    if (fRightDepth<minDepth) {
        // ND280Warn("Depth is below level of drain tubes: " 
        //              << fRightDepth/mm << " mm reset to " 
        //              << minDepth << " mm");
        
      G4ExceptionDescription msg;
      msg << "Depth is below level of drain tubes: " 
	 << fRightDepth/mm << " mm reset to " 
	  << minDepth << " mm" << G4endl;
      G4Exception("ND280P0DWaterTargetConstructor::GetRightWaterDepth",
		  "MyCode0002",JustWarning, msg);
      
	return minDepth;
    }
    return fRightDepth;
}

G4LogicalVolume *ND280P0DWaterTargetConstructor::GetPiece(void) {

    // Create a box to contain the radiator
    G4VSolid *solid = new G4Box(GetName(),
				GetWidth()/2, 
				GetHeight()/2,
                                GetLength()/2);

    // Create the volume around the water target layer.
    G4LogicalVolume *logVolume
	= new G4LogicalVolume(solid,
                              FindMaterial("Air"),
                              GetName());

    // Create the HDPE frames
    G4VSolid *bottomFrame = new G4Box(GetName()+"/BottomFrame",
                                      GetBottomFrameLength()/2,
                                      GetBottomFrameWidth()/2,
                                      GetFrameThickness()/2);
    G4LogicalVolume *logBottomFrame 
        = new G4LogicalVolume(bottomFrame,
                              FindMaterial("HDPE"),
                              GetName()+"/BottomFrame");

    // Note that the frame length is complicated because the side frame length
    // is measured from the part, and doesn't account for the dovetailed
    // construction.  In the MC, the frames are not dovetailed and the side
    // frame is shortened to keep it from overlapping with the top and
    // bottom.
    G4VSolid *sideFrame = new G4Box(GetName()+"/SideFrame",
                                    GetSideFrameWidth()/2,
                                    0.5*(GetSideFrameLength()
                                         -GetBottomFrameWidth()
                                         -GetTopFrameWidth()),
                                    GetFrameThickness()/2);
    G4LogicalVolume *logSideFrame 
        = new G4LogicalVolume(sideFrame,
                              FindMaterial("HDPE"),
                              GetName()+"/SideFrame");

    G4VSolid *topFrame = new G4Box(GetName()+"/TopFrame",
                                      GetTopFrameLength()/2,
                                      GetTopFrameWidth()/2,
                                      GetFrameThickness()/2);
    G4LogicalVolume *logTopFrame 
        = new G4LogicalVolume(topFrame,
                              FindMaterial("HDPE"),
                              GetName()+"/TopFrame");
    
    G4VSolid *centerFrame = new G4Box(GetName()+"/CenterFrame",
                                      GetCenterFrameWidth()/2,
                                      0.5*(GetSideFrameLength()
                                           -GetBottomFrameWidth()
                                           -GetTopFrameWidth()),
                                      GetFrameThickness()/2);
    G4LogicalVolume *logCenterFrame 
        = new G4LogicalVolume(centerFrame,
                              FindMaterial("HDPE"),
                              GetName()+"/CenterFrame");
    
    double bagWidth = GetBottomFrameLength()/2 
        - GetSideFrameWidth() - GetCenterFrameWidth()/2;
    double bagHeight = GetSideFrameLength()
        - GetBottomFrameWidth()
        - GetTopFrameWidth();

    G4VSolid *leftTarget = new G4Box(GetName()+"/LeftTarget",
                                     bagWidth/2,
                                     GetLeftWaterDepth()/2,
                                     ((1.0+GetMassCorrection())
                                      *GetWaterThickness()/2));

    G4LogicalVolume *logLeftTarget 
        = new G4LogicalVolume(leftTarget,
                              FindMaterial("Water"),
                              GetName()+"/LeftTarget");

    G4VSolid *rightTarget = new G4Box(GetName()+"/RightTarget",
                                      bagWidth/2,
                                      GetRightWaterDepth()/2,
                                      ((1.0+GetMassCorrection())
                                       *GetWaterThickness()/2));
    
    G4LogicalVolume *logRightTarget 
        = new G4LogicalVolume(rightTarget,
                              FindMaterial("Water"),
                              GetName()+"/RightTarget");


    G4VSolid *waterBag = new G4Box(GetName()+"/Bag",
                                   bagWidth/2,
                                   bagHeight/2,
                                   GetBagThickness()/2);
    
    G4LogicalVolume *logWaterBag
        = new G4LogicalVolume(waterBag,
                              FindMaterial("HDPE"),
                              GetName()+"/Bag");


    double zTarget = -GetLength()/2 + GetBagThickness() + GetWaterThickness()/2;

    //////////////////////////////////////////////////////////////
    // Place the frame around the water target.
    //////////////////////////////////////////////////////////////

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(0,
                                    -GetHeight()/2 + GetBottomFrameWidth()/2,
                                    zTarget),
                      logBottomFrame,
                      GetName()+"/BottomFrame",
                      logVolume,
                      false,
                      0);

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(0,
                                    GetSideFrameLength()/2-GetTopFrameWidth()/2,
                                    zTarget),
                      logTopFrame,
                      GetName()+"/TopFrame",
                      logVolume,
                      false,
                      0);

    // Position the Left water target frame.  Note that the Y position is
    // complicated because the side frame length doesn't account for the
    // dovetailed construction.  In the MC, the frames are not dovetailed and
    // the side frame is shortened to keep it from overlapping with the top
    // and bottom.
    new G4PVPlacement(0, // no rotation
                      G4ThreeVector((GetBottomFrameLength()/2
                                     -GetSideFrameWidth()/2), 
                                    (-GetHeight()/2 
                                     + GetBottomFrameWidth()
                                     + (GetSideFrameLength()
                                        - GetBottomFrameWidth()
                                        - GetTopFrameWidth())/2),
                                    zTarget),
                      logSideFrame,
                      GetName()+"/LeftFrame",
                      logVolume,
                      false,
                      0);

    // See note on placement of the left side frame.
    new G4PVPlacement(0, // no rotation
                      G4ThreeVector((-GetBottomFrameLength()/2
                                     +GetSideFrameWidth()/2),
                                    (-GetHeight()/2 
                                     + GetBottomFrameWidth()
                                     + (GetSideFrameLength()
                                        - GetBottomFrameWidth()
                                        - GetTopFrameWidth())/2),
                                    zTarget),
                      logSideFrame,
                      GetName()+"/RightFrame",
                      logVolume,
                      false,
                      0);

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(0,
                                    (-GetHeight()/2 
                                     + GetBottomFrameWidth()
                                     + (GetCenterFrameLength()
                                        - GetBottomFrameWidth()
                                        - GetTopFrameWidth())/2),
                                    zTarget),
                      logCenterFrame,
                      GetName()+"/CenterFrame",
                      logVolume,
                      false,
                      0);

    ////////////////////////////////////////////////////////////////
    // Place the water bags into the frame.
    ////////////////////////////////////////////////////////////////

    double yBag = -GetHeight()/2 + GetBottomFrameWidth() + bagHeight/2;
    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(bagWidth/2+GetCenterFrameWidth()/2,
                                    yBag, 
                                    (zTarget 
                                     - GetWaterThickness()/2 
                                     - GetBagThickness()/2)),
                      logWaterBag,
                      GetName()+"/LeftBag",
                      logVolume,
                      false,
                      0);

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(bagWidth/2+GetCenterFrameWidth()/2,
                                    yBag,
                                    (zTarget 
                                     + GetWaterThickness()/2 
                                     + GetBagThickness()/2)),
                      logWaterBag,
                      GetName()+"/LeftBag",
                      logVolume,
                      false,
                      0);

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(-bagWidth/2 - GetCenterFrameWidth()/2,
                                    yBag,
                                    (zTarget 
                                     - GetWaterThickness()/2 
                                     - GetBagThickness()/2)),
                      logWaterBag,
                      GetName()+"/RightBag",
                      logVolume,
                      false,
                      0);

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(-bagWidth/2 - GetCenterFrameWidth()/2,
                                    yBag, 
                                    (zTarget 
                                     + GetWaterThickness()/2 
                                     + GetBagThickness()/2)),
                      logWaterBag,
                      GetName()+"/RightBag",
                      logVolume,
                      false,
                      0);

    ////////////////////////////////////////////////////////////////
    // Place the water volumes into the frame.
    ////////////////////////////////////////////////////////////////

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(bagWidth/2+GetCenterFrameWidth()/2,
                                    (-GetHeight()/2
                                     + GetBottomFrameWidth()
                                     + GetLeftWaterDepth()/2),
                                    zTarget),
                      logLeftTarget,
                      GetName()+"/LeftTarget",
                      logVolume,
                      false,
                      0);

    new G4PVPlacement(0, // no rotation
                      G4ThreeVector(-bagWidth/2 - GetCenterFrameWidth()/2,
                                    (-GetHeight()/2 
                                     + GetBottomFrameWidth()
                                     + GetRightWaterDepth()/2),
                                    zTarget),
                      logRightTarget,
                      GetName()+"/RightTarget",
                      logVolume,
                      false,
                      0);

    /////////////////////////////////////////////////////////////////////////
    /// Place the back cover on the water target.  This only affects the 13th
    /// water target at the back of the Upstream Target SuperP0Dule.
    if (fMakeBackCover) {
        G4VSolid *cover = new G4Box(GetName()+"/Cover",
                                    GetBottomFrameLength()/2,
                                    GetSideFrameLength()/2,
                                    GetBackCoverThickness()/2);
        G4LogicalVolume *logCover 
            = new G4LogicalVolume(cover,
                                  FindMaterial("HDPE"),
                                  GetName()+"/Cover");
        new G4PVPlacement(0, // no rotation
                          G4ThreeVector(0,
                                        -GetHeight()/2+GetSideFrameLength()/2,
                                        GetLength()/2
                                        -GetBackCoverThickness()/2),
                          logCover,
                          GetName()+"/Cover",
                          logVolume,
                          false,
                          0);
    }

    if (GetVisible()) {
        logVolume->SetVisAttributes(GetVisual());
    } 
    else {
        logVolume->SetVisAttributes(G4VisAttributes::Invisible);
    }

    return logVolume;
}
