#ifndef ND280ComponentConstructor_hh_seen
#define ND280ComponentConstructor_hh_seen
// $Id: ND280ComponentConstructor.hh,v 1.7 2009/02/12 17:16:40 mcgrew Exp $
//

#include <G4ThreeVector.hh>
class G4LogicalVolume;

#include "ND280Constructor.hh"
////
// update nd280mc
//#include "ND280SensitiveDetectorFactory.hh"
//#include "ND280SegmentSD.hh"
//////

class G4VSensitiveDetector;

/// Construct a component to be used by a ND280ModuleConstructor to build a
/// component.
class ND280ComponentConstructor : public ND280Constructor {
public:
  //ND280ComponentConstructor(G4String n, ND280UserDetectorConstruction* c) // update nd280mc
    ND280ComponentConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280Constructor(n,c), fLength(0.0), 
          fMaximumWidth(0.0), fMaximumHeight(0.0),
          fSensitiveDetector(NULL), 
          fMaximumHitLength(5*CLHEP::mm), fMaximumHitSagitta(1*CLHEP::mm) {}
    ND280ComponentConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p), fLength(0.0),
          fMaximumWidth(0.0), fMaximumHeight(0.0),
          fSensitiveDetector(NULL), 
          fMaximumHitLength(5*CLHEP::mm), fMaximumHitSagitta(1*CLHEP::mm) {}
    virtual ~ND280ComponentConstructor() {;};

    /// Set the width of the component.  This is the X dimension of the space
    /// available for the component and may be relative to the outside geometry.
    /// The width is set by the ND280ModuleConstructor object which will
    /// contain this component, so it should not be directly set in user code.
    /// The actual constructed component width must be less than the width set
    /// by the owning ND280ModuleConstructor.
    virtual void SetWidth(double w) {fWidth = w;}

    /// Get the width of the component.  This is the X dimension of the space
    /// available for the component and may be relative to the outside geometry.
    /// The width is set by the ND280ModuleConstructor object which will
    /// contain this component.  The actual constructed component width must
    /// be less than the width set by the owning ND280ModuleConstructor.
    virtual double GetWidth(void) {return fWidth;}

    /// Set the physical width of the component, this may be smaller than the
    /// available width set in SetWidth.  This is the X dimension of the
    /// component.
    virtual void SetMaximumWidth(double w) {fMaximumWidth = w;}

    /// Set the height of the component.  This is the Y dimension of the space
    /// available for the component and may be relative to the outside
    /// geometry.  The height is set by the ND280ModuleConstructor object
    /// which will contain this component, so it should not be directly set in
    /// user code.  The actual constructed component height must be less than
    /// the height set by the owning ND280ModuleConstructor.
    virtual void SetHeight(double w) {fHeight = w;}

    /// Get the height of the component.  This is the Y dimension of the space
    /// available for the component and may be relative to the outside
    /// geometry.  The actual constructed component height must be less than
    /// the height set by the owning ND280ModuleConstructor.
    virtual double GetHeight(void) {return fHeight;}

    /// Get the length of the component.
    virtual double GetLength(void) {return fLength;}

    /// Set the length of the component.
    virtual void SetLength(double t) {fLength = t;}

    /// Set the physical height of the component, this must be less than or
    /// equal to the available height set in SetHeight (by the parent
    /// ND280ModuleConstructor object). This is the Y dimension of the
    /// component.
    virtual void SetMaximumHeight(double h) {fMaximumHeight = h;}

    /// This is the width of the component that will be actually constructed.
    /// This can be used in GetPiece() to find the size of the bounding box.
    virtual double GetPhysicalWidth() {
        double w = fWidth;
        if (fMaximumWidth>0 && fMaximumWidth<w) w = fMaximumWidth;
        return w;
    }

    /// This is the height of the component that will be actually constructed.
    /// This can be used in GetPiece() to find the size of the bounding box.
    virtual double GetPhysicalHeight() {
        double h = fHeight;
        if (fMaximumHeight>0 && fMaximumHeight<h) h = fMaximumHeight;
        return h;
    }
  
    /// Set the sensitive detector for this component.
    virtual void SetSensitiveDetector(G4VSensitiveDetector* s) {
        fSensitiveDetector = s;
    }

    // /// Set the sensitive detector for this component by name.
    // virtual void SetSensitiveDetector(G4String name, G4String type) {
    //   //ND280SensitiveDetectorFactory factory(type);
    //   SetSensitiveDetector(factory.MakeSD(name));
    //   SetMaximumHitSagitta(fMaximumHitSagitta);
    //   SetMaximumHitLength(fMaximumHitLength);
    // }

    /// Get the sensitive detector for this component.
    virtual G4VSensitiveDetector* GetSensitiveDetector(void) {
        return fSensitiveDetector;
    }

    // virtual void SetMaximumHitSagitta(double sagitta) {
    //     fMaximumHitSagitta = sagitta;
    //     ND280SegmentSD *segSD 
    //         = dynamic_cast<ND280SegmentSD*>(fSensitiveDetector);
    //     if (segSD) {
    //         segSD->SetMaximumHitSagitta(fMaximumHitSagitta);
    //     }
    // }

    // virtual void SetMaximumHitLength(double length) {
    //     fMaximumHitLength = length;
    //     ND280SegmentSD *segSD 
    //         = dynamic_cast<ND280SegmentSD*>(fSensitiveDetector);
    //     if (segSD) {
    //         segSD->SetMaximumHitLength(fMaximumHitLength);
    //     }
    // }

protected:
    /// Increment the length of the component.  This is the distance the
    /// component stretchs along the Z axis of the detector.
    virtual void AddLength(double t) {fLength += t;};

private:
    /// The available width of the component in the X direction.
    double fWidth;

    /// The available height of the component in the Y direction.
    double fHeight;

    /// The length of the component along the Z axis.  
    double fLength;

    /// The maximum physical width of the constructed component.  This must be
    /// less than the available width.
    double fMaximumWidth;

    /// The maximum physical height of the constructed component.  This must be
    /// less than the available height.
    double fMaximumHeight;

    /// The sensitive detector for this tracking component.
    G4VSensitiveDetector* fSensitiveDetector;

    /// Control the total length of track that can be added to a single hit.
    double fMaximumHitLength;

    /// Control the maximum sagitta of a single hit.
    double fMaximumHitSagitta;

};

class ND280ComponentConstructorMessenger: public ND280ConstructorMessenger {
private:
    ND280ComponentConstructor* fConstructor;
    G4UIcmdWithADoubleAndUnit* fWidthCMD;
    G4UIcmdWithADoubleAndUnit* fHeightCMD;
    G4UIcmdWithADoubleAndUnit* fMaxWidthCMD;
    G4UIcmdWithADoubleAndUnit* fMaxHeightCMD;
    G4UIcommand*               fSensitiveCMD;
    G4UIcmdWithADoubleAndUnit* fMaximumHitSagittaCMD;
    G4UIcmdWithADoubleAndUnit* fMaximumHitLengthCMD;

public:
    ND280ComponentConstructorMessenger(ND280ComponentConstructor* c,
                                       const char* guide) 
        : ND280ConstructorMessenger(c,guide),
          fConstructor(c) {

      fWidthCMD = new G4UIcmdWithADoubleAndUnit(CommandName("width"),this);
        fWidthCMD->SetGuidance("Set the width of the component.");
        fWidthCMD->SetParameterName("Width",false);
        fWidthCMD->SetUnitCategory("Length");

        fHeightCMD = new G4UIcmdWithADoubleAndUnit(CommandName("height"),this);
        fHeightCMD->SetGuidance("Set the height of the component.");
        fHeightCMD->SetParameterName("Height",false);
        fHeightCMD->SetUnitCategory("Length");

        fMaxWidthCMD 
            = new G4UIcmdWithADoubleAndUnit(CommandName("maxWidth"),this);
        fMaxWidthCMD->SetGuidance("Set the maximum width of the component.");
        fMaxWidthCMD->SetParameterName("Width",false);
        fMaxWidthCMD->SetUnitCategory("Length");

        fMaxHeightCMD 
            = new G4UIcmdWithADoubleAndUnit(CommandName("maxHeight"),this);
        fMaxHeightCMD->SetGuidance("Set the maximum height of the component.");
        fMaxHeightCMD->SetParameterName("Height",false);
        fMaxHeightCMD->SetUnitCategory("Length");

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

    virtual ~ND280ComponentConstructorMessenger() {
        delete fWidthCMD;
        delete fHeightCMD;
        delete fMaxWidthCMD;
        delete fMaxHeightCMD;
        delete fSensitiveCMD;
        delete fMaximumHitSagittaCMD;
        delete fMaximumHitLengthCMD;
    };

    void SetNewValue(G4UIcommand *cmd, G4String val) {
        if (cmd==fWidthCMD) {
            fConstructor->SetWidth(fWidthCMD->GetNewDoubleValue(val));
        }
        else if (cmd==fHeightCMD) {
            fConstructor->SetHeight(fHeightCMD->GetNewDoubleValue(val));
        }
        else if (cmd==fMaxWidthCMD) {
            fConstructor->
                SetMaximumWidth(fMaxWidthCMD->GetNewDoubleValue(val));
        }
        else if (cmd==fMaxHeightCMD) {
            fConstructor->
                SetMaximumHeight(fMaxHeightCMD->GetNewDoubleValue(val));
        }
        //else if (cmd==fSensitiveCMD) {
	//  std::istringstream buf(val.c_str());
	//  std::string name, type;
	//  buf >> name;
	//  buf >> type;
	//  fConstructor->SetSensitiveDetector(name,type);
        //}
        //else if (cmd==fMaximumHitSagittaCMD) {
	//  fConstructor->
	//      SetMaximumHitSagitta(
	//          fMaximumHitSagittaCMD->GetNewDoubleValue(val));
        //}
        //else if (cmd==fMaximumHitLengthCMD) {
	//  fConstructor->
	//      SetMaximumHitLength(
	//          fMaximumHitLengthCMD->GetNewDoubleValue(val));
        //}
        else {
            ND280ConstructorMessenger::SetNewValue(cmd,val);
        }
    };
};
#endif
