////////////////////////////////////////////////////////////
// $Id: ND280P0DulePlanksConstructor.cc,v 1.10 2011/11/22 21:24:53 mcgrew Exp $
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

#include "p0d/ND280P0DulePlanksConstructor.hh"
#include "ND280ExtrudedScintConstructor.hh"

// used to keep a list of SD logical volumes
#include "G4RegionStore.hh"
#include <G4Region.hh> 
//

class ND280P0DulePlanksMessenger: public ND280ConstructorMessenger {

private:
    ND280P0DulePlanksConstructor *fConstructor;

    G4UIcommand*               fSensitiveCMD;
    G4UIcmdWithADoubleAndUnit* fMaximumHitSagittaCMD;
    G4UIcmdWithADoubleAndUnit* fMaximumHitLengthCMD;

public:
    ND280P0DulePlanksMessenger(ND280P0DulePlanksConstructor* c) 
        : ND280ConstructorMessenger(c,"Control a scintilator layer."),
          fConstructor(c) {

        fSensitiveCMD = new G4UIcommand(CommandName("sensitive"),this);
        fSensitiveCMD->SetGuidance("Set the name of the sensitive detector");
        G4UIparameter* nameParam = new G4UIparameter('s');
        nameParam->SetParameterName("Name");
        fSensitiveCMD->SetParameter(nameParam);
        G4UIparameter* typeParam = new G4UIparameter('s');
        typeParam->SetParameterName("Type");
        fSensitiveCMD->SetParameter(typeParam);

        fMaximumHitSagittaCMD 
            = new G4UIcmdWithADoubleAndUnit(CommandName("maxHitSagitta"),this);
        fMaximumHitSagittaCMD->SetGuidance("Set the maximum sagitta for a hit.");
        fMaximumHitSagittaCMD->SetParameterName("Sagitta",false);
        fMaximumHitSagittaCMD->SetUnitCategory("Length");

        fMaximumHitLengthCMD 
            = new G4UIcmdWithADoubleAndUnit(CommandName("maxHitLength"),this);
        fMaximumHitLengthCMD->SetGuidance("Set the maximum length for a hit.");
        fMaximumHitLengthCMD->SetParameterName("HitLength",false);
        fMaximumHitLengthCMD->SetUnitCategory("Length");
    };

    ~ND280P0DulePlanksMessenger() {
        delete fSensitiveCMD;
        delete fMaximumHitSagittaCMD;
        delete fMaximumHitLengthCMD;
    };

  void SetNewValue(G4UIcommand *cmd, G4String val) {
    // if (cmd==fSensitiveCMD) {
    //     std::istringstream buf(val.c_str());
    //     std::string name, type;
    //     buf >> name;
    //     buf >> type;
    //     fConstructor->SetSensitiveDetector(name,type);
    // }
    // else if (cmd==fMaximumHitSagittaCMD) {
    //     fConstructor->
    //         SetMaximumHitSagitta(
    //             fMaximumHitSagittaCMD->GetNewDoubleValue(val));
    // }
    // else if (cmd==fMaximumHitLengthCMD) {
    //     fConstructor->
    //         SetMaximumHitLength(
    //             fMaximumHitLengthCMD->GetNewDoubleValue(val));
    // }
    // else {
    ND280ConstructorMessenger::SetNewValue(cmd,val);
    // }
  };

};

ND280P0DulePlanksConstructor::~ND280P0DulePlanksConstructor() {};
ND280P0DuleXPlanksConstructor::~ND280P0DuleXPlanksConstructor() {;};
ND280P0DuleYPlanksConstructor::~ND280P0DuleYPlanksConstructor() {;};

G4VisAttributes* ND280P0DulePlanksConstructor::GetVisual(void) const {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(0.8,0.8,0.8,1); // green
    return visual;
};

G4VisAttributes* ND280P0DuleXPlanksConstructor::GetVisual(void) const {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(0.5,1.0,0.5,1); // greenish
    return visual;
};

G4VisAttributes* ND280P0DuleYPlanksConstructor::GetVisual(void) const {
    G4VisAttributes *visual = new G4VisAttributes();
    visual->SetColor(1.0,0.5,0.5,1); // redish
    return visual;
};

void ND280P0DuleXPlanksConstructor::Init(void) {
    MakeXPlanks();
    SetNumberOfBars(126);
    SetBarLength(2272*mm);
    /// \todo Bar spacing averages over entire side.  In fact, the bars are
    /// first assembled into planks, and then assembled in to planes.  In a
    /// plank, the spacing between bar centers is about 16.77 mm.  The center
    /// to center space between bars at the edge of a plank is about 17.77 mm.
    SetBarSpacing(16.82*mm);
    // The measured average mass for the X layer is 82.91 kg (taken from mass
    // measurements at SBU during construction).  See the P0D mass summary
    // spreadsheet on docdb (808)
    SetMassCorrection(-0.007);
}

void ND280P0DuleYPlanksConstructor::Init(void) {
    MakeYPlanks();
    SetNumberOfBars(134);
    SetBarLength(2133*mm);
    /// \todo Bar spacing averages over entire side.  In fact, the bars are
    /// first assembled into planks, and then assembled in to planes.  In a
    /// plank, the spacing between bar centers is about 16.77 mm.  The center
    /// to center space between bars at the edge of a plank is about 17.77 mm.
    SetBarSpacing(16.83*mm);
    // The measured average mass for the Y layer is 83.05 kg (taken from mass
    // measurements at SBU during construction).  See the P0D mass summary
    // spreadsheet on docdb (808)
    SetMassCorrection(-0.003);
}

void ND280P0DulePlanksConstructor::Init() {
    AddConstructor(new ND280ExtrudedScintConstructor("Bar",this));
    SetScintillatorMaterial("P0DScintillator");

    fNumberOfBars = 1;

    fMakeXPlanks = true;

    // The thickness of the planks was measured at SBU.
    SetPlankThickness(17.34*mm);

    // Design Specification (based on enclosing triangle).  The measured base
    // length of the bar is 32.5 mm, but the enclosing triangle is designed to
    // be 33 mm.  The enclosing triangle is 17 mm high.  
    SetBarBase(33.0*mm);
    SetBarHeight(17.0*mm);

    // The design spec from Kuraray.
    SetBarFiberRadius(0.6*mm);
    // The designed hold diameter.  The actual hole diameter varied
    // significantly.
    SetBarHoleRadius(1.3*mm);  
    // The coating thickness is set to give the right fractional area to the
    // coating.  This is based on measurements of the bar.
    SetBarCoatingThickness(0.21*mm);

    //SetSensitiveDetector("p0d","segment");
    //SetMaximumHitLength(5*cm);
    //SetMaximumHitSagitta(1*cm);

    SetMessenger(new ND280P0DulePlanksMessenger(this));
};

double ND280P0DulePlanksConstructor::GetLength() {
    if (GetPlankThickness() < GetBarHeight()) {
      //G4Exception("Plank not thick enough");
      G4Exception("ND280P0DulePlanksConstructor::GetLength",
		  "MyCode0002",FatalException,
		  "Plank not thick enough");
    }
    return GetPlankThickness();
}

double ND280P0DulePlanksConstructor::GetWidth() {
    if (fMakeXPlanks) return (fNumberOfBars+1)*GetBarSpacing();
    return GetBarLength();
}

double ND280P0DulePlanksConstructor::GetHeight() {
    if (fMakeXPlanks) return GetBarLength();
    return (fNumberOfBars+1)*GetBarSpacing();
}

void ND280P0DulePlanksConstructor::SetBarLength(double r) {
    fBarLength = r;
}

double ND280P0DulePlanksConstructor::GetBarLength() {
    return fBarLength;
}

void ND280P0DulePlanksConstructor::SetBarSpacing(double s) {
    fBarSpacing = s;
}

double ND280P0DulePlanksConstructor::GetBarSpacing() {
    return fBarSpacing;
}

void ND280P0DulePlanksConstructor::SetNumberOfBars(G4int n) {
    fNumberOfBars = n;
}

G4int ND280P0DulePlanksConstructor::GetNumberOfBars(void) const {
    return fNumberOfBars;
}

void ND280P0DulePlanksConstructor::SetBarCoatingThickness(double b) {
    ND280ExtrudedScintConstructor& bar
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    bar.SetCoatingThickness(b);
}

double ND280P0DulePlanksConstructor::GetBarCoatingThickness(void) {
    ND280ExtrudedScintConstructor& bar
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    return bar.GetCoatingThickness();
}

void ND280P0DulePlanksConstructor::SetBarCoatingRadius(double b) {
    ND280ExtrudedScintConstructor& bar
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    bar.SetCoatingRadius(b);
}

double ND280P0DulePlanksConstructor::GetBarCoatingRadius(void) {
    ND280ExtrudedScintConstructor& bar
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    return bar.GetCoatingRadius();
}

void ND280P0DulePlanksConstructor::SetBarHoleRadius(double b) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    bar.SetHoleRadius(b);
}

double ND280P0DulePlanksConstructor::GetBarHoleRadius(void) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    return bar.GetHoleRadius();
}

void ND280P0DulePlanksConstructor::SetBarFiberRadius(double b) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    bar.SetFiberRadius(b);
}

double ND280P0DulePlanksConstructor::GetBarFiberRadius(void) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    return bar.GetFiberRadius();
}

void ND280P0DulePlanksConstructor::SetCoatingMaterial(const char* mat) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    bar.SetCoatingMaterial(mat);
}

void ND280P0DulePlanksConstructor::SetScintillatorMaterial(const char* mat) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    bar.SetScintillatorMaterial(mat);
}

void ND280P0DulePlanksConstructor::SetFiberMaterial(const char* mat) {
    ND280ExtrudedScintConstructor& bar 
        =  Get<ND280ExtrudedScintConstructor>("Bar");
    bar.SetFiberMaterial(mat);
}

G4LogicalVolume *ND280P0DulePlanksConstructor::GetPiece(void) {

  // NEW ND280 UPGRADE
  G4Region* SDRegion = G4RegionStore::GetInstance()->
    GetRegion("SDRegion",false);
  //

    // This rotates the bar into position with the apex pointing upstream
    // (toward the target [-Z]).
    G4RotationMatrix* rotUpstream = new G4RotationMatrix(); 
    // This rotates the bar into position with the apex pointing downstream
    // (toward Super-Kamiokande [+Z]).
    G4RotationMatrix* rotDownstream = new G4RotationMatrix(); 

    G4VSolid *solid = new G4Box(GetName(),
                                GetWidth()/2,
                                GetHeight()/2,
                                GetLength()/2);

    // Create the volume around the tracking layer.  The enclosing volume
    // material is set to air since very little epoxy was actually used in the
    // construction of the planks.  Close inspection (and breaking apart a
    // plank) shows that most of the area between bars is air filled.
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

    /// The extruded bars to be added to the tracking layer.
    ND280ExtrudedScintConstructor& bar 
        = Get<ND280ExtrudedScintConstructor>("Bar");

    //bar.SetSensitiveDetector(GetSensitiveDetector());

    
    bar.SetLength(GetBarLength());
    bar.SetBase(GetBarBase()*std::sqrt(1.0+GetMassCorrection()));
    bar.SetHeight(GetBarHeight()*std::sqrt(1.0+GetMassCorrection()));

    G4LogicalVolume* barLog = bar.GetPiece();
    
    //
    // N.B.: barLog is the logical volume that contains only 1 scintillating bar 
    // (different from ECal in ND280ScintLayerConstructor where there's a single volume for all the bars)
    //
    if(SDRegion) SDRegion->AddRootLogicalVolume(barLog); // NEW ND280 UPGRADE
    //

    G4double SinOrientation;
    G4double CosOrientation;
    
    if (fMakeXPlanks) {
        SinOrientation = 0;
        CosOrientation = 1;
        rotDownstream->rotateX( 90*degree);
        rotDownstream->rotateZ(180*degree);
        rotUpstream->rotateX( 90*degree);
    }
    else {
        SinOrientation = 1;
        CosOrientation = 0;
        rotDownstream->rotateY(-90*degree);
        rotDownstream->rotateZ(-90*degree);
        rotUpstream->rotateY(-90*degree);
        rotUpstream->rotateZ( 90*degree);
    }
    
    double offset = ((fNumberOfBars-1) * GetBarSpacing())/2;
    
    for (int count = 0; count < fNumberOfBars; ++count) {
        G4RotationMatrix *rot;
        
        // Rotate the bar into the layer.  The first bar in each layer is at
        // the most negative coordinate, and has the apex facing upstream.
        if (count%2 == 0) rot = rotUpstream; else rot = rotDownstream;
        
        // Place a bar at positive offset.
        double xPos=CosOrientation*(count*GetBarSpacing()-offset);
        double yPos=SinOrientation*(count*GetBarSpacing()-offset);
        
        new G4PVPlacement(rot,               // rotation
                          G4ThreeVector(xPos, yPos, 0),  // position
                          barLog,            // logical volume
                          bar.GetName(),     // name
                          logVolume,         // mother  volume
                          false,             // no boolean operations
                          count);            // copy number
    }
    
    return logVolume;
}

//void ND280P0DulePlanksConstructor::SetSensitiveDetector(
//G4VSensitiveDetector* s) {
//fSensitiveDetector = s;
//}

//void ND280P0DulePlanksConstructor::SetSensitiveDetector(G4String name,
//G4String type) {
//ND280SensitiveDetectorFactory factory(type);
//SetSensitiveDetector(factory.MakeSD(name));
//SetMaximumHitSagitta(fMaximumHitSagitta);
//SetMaximumHitLength(fMaximumHitLength);
//}

//G4VSensitiveDetector* ND280P0DulePlanksConstructor::GetSensitiveDetector(void) {
//return fSensitiveDetector;
//}

//void ND280P0DulePlanksConstructor::SetMaximumHitSagitta(double sagitta) {
//fMaximumHitSagitta = sagitta;
//ND280SegmentSD *segSD 
//= dynamic_cast<ND280SegmentSD*>(fSensitiveDetector);
//if (segSD) {
//segSD->SetMaximumHitSagitta(fMaximumHitSagitta);
//}
//}

//void ND280P0DulePlanksConstructor::SetMaximumHitLength(double length) {
//fMaximumHitLength = length;
//ND280SegmentSD *segSD 
//= dynamic_cast<ND280SegmentSD*>(fSensitiveDetector);
//if (segSD) {
//segSD->SetMaximumHitLength(fMaximumHitLength);
//}
//}
