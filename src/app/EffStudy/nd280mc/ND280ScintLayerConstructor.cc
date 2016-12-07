////////////////////////////////////////////////////////////
// $Id: ND280ScintLayerConstructor.cc,v 1.27 2011/11/22 21:23:20 mcgrew Exp $
//

#include <cmath>
#include <algorithm>

#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4PVPlacement.hh>

#include "ND280ScintLayerConstructor.hh"
#include "ND280ExtrudedScintConstructor.hh"

// used to keep a list of SD logical volumes
#include "G4RegionStore.hh"
#include <G4Region.hh> 
//

class ND280ScintLayerMessenger: public ND280ComponentConstructorMessenger {

private:
    ND280ScintLayerConstructor *fConstructor;

    G4UIcmdWithAnInteger* fBarNumberCMD;
    G4UIcmdWithADoubleAndUnit* fBarBaseCMD;
    G4UIcmdWithADoubleAndUnit* fBarHeightCMD;

public:
    ND280ScintLayerMessenger(ND280ScintLayerConstructor* c) 
        : ND280ComponentConstructorMessenger(c,
                                             "Control a scintilator layer."),
          fConstructor(c) {

        fBarBaseCMD=new G4UIcmdWithADoubleAndUnit(CommandName("barBase"),this);
        fBarBaseCMD->SetGuidance("Set the base of the bar used for layer.");
        fBarBaseCMD->SetParameterName("base",false);
        fBarBaseCMD->SetUnitCategory("Length");

        fBarHeightCMD = new G4UIcmdWithADoubleAndUnit(CommandName("barHeight"),
                                                    this);
        fBarHeightCMD->SetGuidance("Set the height of the "
                                   "bar used for layer.");
        fBarHeightCMD->SetParameterName("height",false);
        fBarHeightCMD->SetUnitCategory("Length");

        fBarNumberCMD = new G4UIcmdWithAnInteger(CommandName("barNumbers"),
                                                 this);
        fBarNumberCMD->SetGuidance("Set the number of bars used in a layer.");
        fBarNumberCMD->SetParameterName("number",false);
        fBarNumberCMD->SetRange("number>0");
        fBarNumberCMD->AvailableForStates(G4State_PreInit,G4State_Idle);
    };

    ~ND280ScintLayerMessenger() {

        delete fBarBaseCMD;
        delete fBarHeightCMD;
        delete fBarNumberCMD;
    };

    void SetNewValue(G4UIcommand *cmd, G4String val) {
        if (cmd == fBarBaseCMD) {
            fConstructor->SetBarBase(fBarBaseCMD->GetNewDoubleValue(val));
        }
        else if (cmd == fBarHeightCMD) {
            fConstructor->SetBarHeight(fBarHeightCMD->GetNewDoubleValue(val));
        }
        else if (cmd == fBarNumberCMD) {
            fConstructor->SetBarNumber(fBarNumberCMD->GetNewIntValue(val));
        } 
        else {
            ND280ComponentConstructorMessenger::SetNewValue(cmd,val);
        }
    };

};

ND280ScintLayerConstructor::~ND280ScintLayerConstructor() {};
ND280ScintXLayerConstructor::~ND280ScintXLayerConstructor() {;};
ND280ScintYLayerConstructor::~ND280ScintYLayerConstructor() {;};

G4VisAttributes* ND280ScintLayerConstructor::GetVisual(void) const {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(0.8,0.8,0.8,1); // green
    return visual;
};

G4VisAttributes* ND280ScintXLayerConstructor::GetVisual(void) const {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(0.5,1.0,0.5,1); // greenish
    return visual;
};

G4VisAttributes* ND280ScintYLayerConstructor::GetVisual(void) const {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(1.0,0.5,0.5,1); // redish
    return visual;
};

void ND280ScintLayerConstructor::Init() {
    
  AddConstructor(new ND280ExtrudedScintConstructor("Bar",this));

    SetBarNumber(0);
    fNumberOfBars = 0;

    fMakeXLayer = true;
    fAntiParallelBars = true;

    SetWidth(250*cm);
    SetHeight(250*cm);
    SetLength(GetBarHeight());

    SetEdgeSpace(0*cm);

    //SetSensitiveDetector("scint","segment");
    //SetMaximumHitLength(5*cm);
    //SetMaximumHitSagitta(1*cm);

    SetMessenger(new ND280ScintLayerMessenger(this));

};

void ND280ScintLayerConstructor::SetBarNumber(G4int n) {
    fNumBars = n;
}

G4int ND280ScintLayerConstructor::GetBarNumber(void) {
    return fNumberOfBars;
}

void ND280ScintLayerConstructor::SetBarBase(double b) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    bar.SetBase(b);
}

double ND280ScintLayerConstructor::GetBarBase(void) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    return bar.GetBase();
}

void ND280ScintLayerConstructor::SetBarHeight(double b) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    bar.SetHeight(b);
    SetLength(GetBarHeight());
}

double ND280ScintLayerConstructor::GetBarHeight(void) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    return bar.GetHeight();
}

void ND280ScintLayerConstructor::SetBarCoatingThickness(double b) {
    ND280ExtrudedScintConstructor& bar
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    bar.SetCoatingThickness(b);
}

double ND280ScintLayerConstructor::GetBarCoatingThickness(void) {
    ND280ExtrudedScintConstructor& bar
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    return bar.GetCoatingThickness();
}

void ND280ScintLayerConstructor::SetBarCoatingRadius(double b) {
    ND280ExtrudedScintConstructor& bar
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    bar.SetCoatingRadius(b);
}

double ND280ScintLayerConstructor::GetBarCoatingRadius(void) {
    ND280ExtrudedScintConstructor& bar
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    return bar.GetCoatingRadius();
}

void ND280ScintLayerConstructor::SetBarGap(double b) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    bar.SetGap(b);
}

double ND280ScintLayerConstructor::GetBarGap(void) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    return bar.GetGap();
}

void ND280ScintLayerConstructor::SetBarHoleRadius(double b) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    bar.SetHoleRadius(b);
}

double ND280ScintLayerConstructor::GetBarHoleRadius(void) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    return bar.GetHoleRadius();
}

void ND280ScintLayerConstructor::SetBarFiberRadius(double b) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    bar.SetFiberRadius(b);
}

double ND280ScintLayerConstructor::GetBarFiberRadius(void) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    return bar.GetFiberRadius();
}

void ND280ScintLayerConstructor::SquareBar(void) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    return bar.SetShape(ND280ExtrudedScintConstructor::eRectangle);
}

void ND280ScintLayerConstructor::TriangularBar(void) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    return bar.SetShape(ND280ExtrudedScintConstructor::eTriangle);
}


void ND280ScintLayerConstructor::SetCoatingMaterial(const char* mat) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    bar.SetCoatingMaterial(mat);
}

void ND280ScintLayerConstructor::SetScintillatorMaterial(const char* mat) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    bar.SetScintillatorMaterial(mat);
}

void ND280ScintLayerConstructor::SetFiberMaterial(const char* mat) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    bar.SetFiberMaterial(mat);
}

G4LogicalVolume *ND280ScintLayerConstructor::GetPiece(void) {

  // NEW ND280UPGRADE
  G4Region* SDRegion = G4RegionStore::GetInstance()->
    GetRegion("SDRegion",false);
  
  //
  
    G4RotationMatrix* rot1 = new G4RotationMatrix(); 
    G4RotationMatrix* rot2 = new G4RotationMatrix(); 
    
    SetLength(GetBarHeight());

    G4VSolid *solid = new G4Box(GetName(),
                                GetPhysicalWidth()/2-GetEdgeSpace(),
                                GetPhysicalHeight()/2-GetEdgeSpace(),
                                GetLength()/2);
    
    // Create the volume around the tracking layer.
    G4LogicalVolume* logVolume
        = new G4LogicalVolume(solid,
                              FindMaterial("Air"),
                              GetName());

    if (GetVisible()) {
      G4VisAttributes *visual = GetVisual();
      logVolume->SetVisAttributes(visual);
    }
    else {
      logVolume->SetVisAttributes(G4VisAttributes::Invisible);
    }
    
    // Finish up making the bars.
    G4double barLength;
    if (fMakeXLayer) {
        barLength = GetPhysicalHeight();
    } 
    else {
        barLength = GetPhysicalWidth();
    }
    barLength -= 2*GetEdgeSpace();
    
    /// The extruded bars to be added to the tracking layer.
    ND280ExtrudedScintConstructor& bar 
        = Get<ND280ExtrudedScintConstructor>("Bar");
    
    //bar.SetSensitiveDetector(GetSensitiveDetector());
    //bar.SetLength(barLength);
    
    ND280ExtrudedScintConstructor::ExtrudedShape barShape = bar.GetShape();
    
    //
    // N.B.: barLog is the logical volume that contains 
    // fNumberOfBars extruded scintillating bars
    //
    G4LogicalVolume* barLog = bar.GetPiece();

    if(SDRegion) SDRegion->AddRootLogicalVolume(barLog);
    
    //

    G4double orientation;
    G4double maxBound;

    if (fMakeXLayer) {
       orientation = 90*degree;
       if (barShape == ND280ExtrudedScintConstructor::eTriangle) {
          rot1->rotateX( 90*degree);
          rot1->rotateZ(180*degree);
          rot2->rotateX( 90*degree);
       }
       else {
          rot1->rotateX( 90*degree);
          rot2->rotateX(-90*degree);
       }

       maxBound = GetPhysicalWidth()/2;

    }
    else {
       orientation = 0*degree;

       if ( barShape == ND280ExtrudedScintConstructor::eTriangle) {
          rot1->rotateY(-90*degree);
          rot1->rotateZ(-90*degree);
          rot2->rotateY(-90*degree);
          rot2->rotateZ( 90*degree);
       }
       else {
          rot1->rotateY(-90*degree);
          rot1->rotateZ(-90*degree);
          rot2->rotateY( 90*degree);
          rot2->rotateZ(-90*degree);
       }

       maxBound = GetPhysicalHeight()/2;
    }

    G4int maxCount;

    if (barShape == ND280ExtrudedScintConstructor::eTriangle) {
        maxBound -= GetEdgeSpace();   // Allow for making the plane smaller.
        double planeSize = 2*maxBound;
        double spaceForBars = (planeSize+bar.GetCenterToCenter()/2);
        maxCount = (G4int) (spaceForBars/bar.GetCenterToCenter());
        maxCount = maxCount - 1;
    } 
    else {
       maxBound -= GetEdgeSpace();
       maxCount = (G4int) ((2*maxBound)/bar.GetCenterToCenter());
    }

    // Limit the maximum number of bars.
    if (fNumBars > 0) {
       if (fNumBars*bar.GetCenterToCenter()<=2*maxBound) maxCount = fNumBars;
    }

    fNumberOfBars = maxCount;

    G4double offset = 0.0;
    if (barShape == ND280ExtrudedScintConstructor::eTriangle) {
       if (maxCount%2 == 0) offset = bar.GetCenterToCenter()/2;
       maxCount = maxCount/2;
       offset -= (maxCount * bar.GetCenterToCenter());
       if (maxCount%2 == 1) std::swap(rot1,rot2);
    } 
    else {
       if (maxCount%2 == 0) offset = bar.GetCenterToCenter()/2;
       maxCount = maxCount/2;
       offset -= maxCount * bar.GetCenterToCenter();
    }

    maxBound = offset + fNumberOfBars*bar.GetCenterToCenter();

    G4int barCount = 1;

    // This can suffer from round-off since a "small" number is being added to
    // an increasingly large number.  Make sure that a bar isn't created just
    // inside of the volume boundary.
    for (int count = 0; count < fNumberOfBars; ++ count) {
        G4RotationMatrix *rot;
        
        // Position the bar in the layer.
        if (barCount%2 == 1 || !fAntiParallelBars) {
            rot = rot1;
        } 
        else {
            rot = rot2;
        }
        
        // Place a bar at positive offset.
        G4double xPos=sin(orientation)*(count*bar.GetCenterToCenter()+offset);
        G4double yPos=cos(orientation)*(count*bar.GetCenterToCenter()+offset);
        
        new G4PVPlacement(rot,               // rotation
                          G4ThreeVector(xPos,
                                        yPos,
                                        0),  // position
                          barLog,            // logical volume
                          bar.GetName(),     // name
                          logVolume,         // mother  volume
                          false,             // no boolean operations
                          barCount);         // copy number
    
	//G4cout << barLog->GetName() << " --> " 
	//<< bar.GetName() << G4endl;
       
        ++barCount;
    }

    // not used in nd280mc
    //barLog->SetVisAttributes(G4Color(0.,0.7,0.));
    //barLog->SetVisAttributes(G4VisAttributes::Invisible);
        
    return logVolume;
}
