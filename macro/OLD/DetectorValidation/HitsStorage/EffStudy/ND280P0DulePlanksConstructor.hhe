#ifndef ND280P0DulePlanksConstructor_hh_Seen
#define ND280P0DulePlanksConstructor_hh_Seen
// $Id: ND280P0DulePlanksConstructor.hh,v 1.5 2011/11/22 21:24:53 mcgrew Exp $
//

#include <G4RotationMatrix.hh>

#include "ND280Constructor.hh"
//#include "ND280SensitiveDetectorFactory.hh"
//#include "ND280SegmentSD.hh"

class G4VisAttributes;
class G4LogicalVolume;

class G4VSensitiveDetector;

/// Construct a layer of scintillator for tracking.  A scintillator layer is
/// either X or Y.
class ND280P0DulePlanksConstructor : public ND280Constructor {
public:
  //ND280P0DulePlanksConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280P0DulePlanksConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280Constructor(n,c), 
          fMakeXPlanks(false), fNumberOfBars(0), fPlankThickness(0),
          fBarLength(0), fBarSpacing(0), fMassCorrection(0), 
          fSensitiveDetector(0), fMaximumHitLength(0), fMaximumHitSagitta(0) 
        {Init();};
    ND280P0DulePlanksConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {
        Init();};
    virtual ~ND280P0DulePlanksConstructor(); 

    /// Get the length of the plank (along the beam axis).
    double GetLength();

    /// Get the width of the planks when they are mounted in the detector (the
    /// horizontal direction across the beam axis).
    double GetWidth();
   
    /// Get the height of the planks when they are mounted in the detector
    /// (the vertical direction across the beam axis).
    double GetHeight();

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume *GetPiece(void);

    /// Flag that an X layer should be constructed.  The X layer measures the
    /// X position, so the bars run along the Y axis (vertically)
    void MakeXPlanks(void) {fMakeXPlanks=true;};

    /// Flag that an Y layer should be constructed.  The Y layer measures the
    /// Y position, so the bars run along the X axis (horizontally).
    void MakeYPlanks(void) {fMakeXPlanks=false;};
    
    /// Set the thickness of the plank.
    void SetPlankThickness(double s) {fPlankThickness = s;}

    /// Get the plank thickness.
    double GetPlankThickness(void) const {return fPlankThickness;}

    /// Set the length of the bars used to construct a layer.
    void SetBarLength(double r);

    /// Get the bar length.
    double GetBarLength();

    /// Set the center to center spacing for the bars.
    void SetBarSpacing(double s);

    /// Get the bar spacing.
    double GetBarSpacing();

    /// Set the number of bars used in a layer.
    void SetNumberOfBars(G4int n);

    /// Get the number of bars used in a layer.
    G4int GetNumberOfBars(void) const;

    /// Set the base width of the extruded bars that will be used to construct
    /// this layer. 
    void SetBarBase(double b) {fBarBase = b;}

    /// Get the base width of the extruded bars used to construct this layer.
    double GetBarBase(void) {return fBarBase;}

    /// This is the thickness of the bar along the beam axis.  For a
    /// triangular bar, this is the height of the triangle.  By default, this
    /// is set to the base width of the extruded bars that will be used to
    /// construct this layer.
    void SetBarHeight(double t) {fBarHeight = t;}

    /// Get the base width of the extruded bars used to construct this layer.
    double GetBarHeight(void) {return fBarHeight;}

    /// Set the extruded bar's coating thickness
    void SetBarCoatingThickness(double t);

    /// Get the extruded bar's coating thickness
    double GetBarCoatingThickness(void);

    /// Set the extruded bar's coating corner radius
    void SetBarCoatingRadius(double r);

    /// Get the extruded bar's coating corner radius
    double GetBarCoatingRadius(void);
    
    /// Set the radius of the hole down the center of the scintillator.  If
    /// the hole radius is less than the fiber radius, then neither the hole,
    /// nor the fiber will be constructed.
    void SetBarHoleRadius(double b);

    /// Get the radius of the hole down the center of the scintillator.  
    double GetBarHoleRadius(void);

    /// Set the radius of the WLS fiber in the scintillator.  If
    /// the hole radius is less than the fiber radius, then neither the hole,
    /// nor the fiber will be constructed.
    void SetBarFiberRadius(double b);

    /// Get the radius of the WLS fiber.
    double GetBarFiberRadius(void);

    /// Set the material to use for the scintillator coating.
    void SetCoatingMaterial(const char* mat);

    /// Set the material to use for the scintillator.
    void SetScintillatorMaterial(const char* mat);
    
    /// Set the material to use for the fiber.
    void SetFiberMaterial(const char* mat);

    /// Set the mass correction factor.  This is applied to the actual bar
    /// size to make sure that the mass of the simulated plank matchs the
    /// measured masses.  The correction factor should be small (less than a
    /// percent or two).  The mass is corrected by changing the height and
    /// base of the bar.  The actual bar dimensions vary by a few percent, and
    /// the simulation uses the design value.  The mass correction is used to
    /// change the bar dimensions within the allowed range so that the overall
    /// plank mass matchs the mass measurements taken at SBU.
    void SetMassCorrection(double m) {fMassCorrection = m;}

    /// Get the mass correction factor.
    double GetMassCorrection() const { return fMassCorrection;}

    // /// Set the sensitive detector for this component.
    // virtual void SetSensitiveDetector(G4VSensitiveDetector* s);
  
    // /// Set the sensitive detector for this component by name.
    // virtual void SetSensitiveDetector(G4String name, G4String type);
    
    // /// Get the sensitive detector for this component.
    // virtual G4VSensitiveDetector* GetSensitiveDetector(void);
    
    // virtual void SetMaximumHitSagitta(double sagitta);

    // virtual void SetMaximumHitLength(double length);

    virtual G4VisAttributes* GetVisual(void) const;

private:

    void Init(void);

    /// Flag to build a X layer.
    bool fMakeXPlanks;
    
    /// The number of bars actually used in a layer.
    G4int fNumberOfBars;

    /// The thickness of the plank
    double fPlankThickness;

    /// The base of the scintillator bar.
    double fBarBase;

    /// The height of the scintillator bar.
    double fBarHeight;

    /// The length of the bars.
    double fBarLength;

    /// The center to center spacing of the scintillator bars.
    double fBarSpacing;

    /// The mass correction factor.
    double fMassCorrection;

    /// The sensitive detector for this tracking component.
    G4VSensitiveDetector* fSensitiveDetector;

    /// Control the total length of track that can be added to a single hit.
    double fMaximumHitLength;

    /// Control the maximum sagitta of a single hit.
    double fMaximumHitSagitta;

};

class ND280P0DuleXPlanksConstructor: public ND280P0DulePlanksConstructor {
public: 
  //ND280P0DuleXPlanksConstructor(G4String n, ND280UserDetectorConstruction* c)
  ND280P0DuleXPlanksConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280P0DulePlanksConstructor(n,c) {Init();}
    ND280P0DuleXPlanksConstructor(G4String n, ND280Constructor* p)
        : ND280P0DulePlanksConstructor(n,p) {Init();}
    virtual ~ND280P0DuleXPlanksConstructor();

    virtual G4VisAttributes* GetVisual(void) const;

protected:
    void Init(void);
};    

class ND280P0DuleYPlanksConstructor: public ND280P0DulePlanksConstructor {
public: 
  //ND280P0DuleYPlanksConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280P0DuleYPlanksConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280P0DulePlanksConstructor(n,c) {Init();}
    ND280P0DuleYPlanksConstructor(G4String n, ND280Constructor* p)
        : ND280P0DulePlanksConstructor(n,p) {Init();}
    virtual ~ND280P0DuleYPlanksConstructor();

    virtual G4VisAttributes* GetVisual(void) const;

protected:
    void Init(void);
};    
#endif
