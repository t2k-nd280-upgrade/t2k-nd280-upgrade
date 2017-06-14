#ifndef ND280GapConstructor_hh_Seen
#define ND280GapConstructor_hh_Seen
// $Id: ND280GapConstructor.hh,v 1.11 2011/11/03 23:44:38 lindner Exp $
//

#include <G4ThreeVector.hh>
#include <G4VisAttributes.hh>
class G4LogicalVolume;

#include "ND280ComponentConstructor.hh"
#include "ND280ModuleConstructor.hh"

class G4VSensitiveDetector;

/// Construct an inactive layer material.
class ND280GapConstructor : public ND280ComponentConstructor {
public:
  //ND280GapConstructor(G4String n, ND280UserDetectorConstruction* c)
  ND280GapConstructor(G4String n, ExN02DetectorConstruction* c)
      : ND280ComponentConstructor(n,c) {Init();};
    ND280GapConstructor(G4String n, ND280Constructor* p)
        : ND280ComponentConstructor(n,p) {Init();};
    virtual ~ND280GapConstructor(); 

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume *GetPiece(void);

    /// Set the material that fills the gap.
    void SetMaterialName(const char* material) {
        fMaterialName = material;
    }

    /// Get the material the fills the gap
    G4String GetMaterialName(void) const {
        return fMaterialName;
    }

    /// Set the amount of unused space around the edges.
    void SetEdgeSpace(double d) {fEdgeSpace = d;}

    /// Get the amount of unused space around the edges.
    double GetEdgeSpace(void) {return fEdgeSpace;}

    virtual const G4VisAttributes* GetVisual(void) const {
        return &G4VisAttributes::Invisible;
    };

protected:
    /// The material for the gap.
    G4String fMaterialName;

    /// The amount of unused space around the edge.
    double fEdgeSpace;

    void Init(void);
};

/// Construct an inactive layer of air.
class ND280AirGapConstructor: public ND280GapConstructor {
public: 
  //ND280AirGapConstructor(G4String n, ND280UserDetectorConstruction* c)
  ND280AirGapConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280GapConstructor(n,c) {Init();};
    ND280AirGapConstructor(G4String n, ND280Constructor* p)
        : ND280GapConstructor(n,p) {Init();};
    virtual ~ND280AirGapConstructor();

protected:
    void Init(void) {
        SetMaterialName("Air");
    };

};    

///Construct an inactive layer of aluminum.
class ND280AluminumGapConstructor: public ND280GapConstructor {
public:
  //ND280AluminumGapConstructor(G4String n, ND280UserDetectorConstruction* c)
  ND280AluminumGapConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280GapConstructor(n,c) {Init();};
    ND280AluminumGapConstructor(G4String n, ND280Constructor* p)
      : ND280GapConstructor(n,p) {Init();};
  virtual ~ND280AluminumGapConstructor();

  virtual const G4VisAttributes* GetVisual(void) const;

protected:
    void Init(void) {
        SetMaterialName("Aluminum");
    };

};    


//this section is added for DUET and fgdmini
///Construct an inactive layer of Polyvinyltolulent.
class ND280PVTGapConstructor: public ND280GapConstructor {
public:
  //ND280PVTGapConstructor(G4String n, ND280UserDetectorConstruction* c)
  ND280PVTGapConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280GapConstructor(n,c) {Init();};
    ND280PVTGapConstructor(G4String n, ND280Constructor* p)
      : ND280GapConstructor(n,p) {Init();};
  virtual ~ND280PVTGapConstructor();

  virtual const G4VisAttributes* GetVisual(void) const;

protected:
    void Init(void) {
        SetMaterialName("PVT");
    };

};
///end///

/// Construct an inactive layer of lead.
class ND280LeadGapConstructor: public ND280GapConstructor {
public: 
  //ND280LeadGapConstructor(G4String n, ND280UserDetectorConstruction* c)
  ND280LeadGapConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280GapConstructor(n,c) {Init();};
    ND280LeadGapConstructor(G4String n, ND280Constructor* p)
        : ND280GapConstructor(n,p) {Init();};
    virtual ~ND280LeadGapConstructor();

    virtual const G4VisAttributes* GetVisual(void) const;

protected:
    void Init(void) {
        SetMaterialName("Lead");
    };

};    

/// Construct an inactive layer of water.
class ND280WaterGapConstructor: public ND280GapConstructor {
public: 
  //ND280WaterGapConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280WaterGapConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280GapConstructor(n,c) {Init();};
    ND280WaterGapConstructor(G4String n, ND280Constructor* p)
        : ND280GapConstructor(n,p) {Init();};
    virtual ~ND280WaterGapConstructor();

    virtual const G4VisAttributes* GetVisual(void) const;

protected:
    void Init(void) {
        SetMaterialName("Water");
    };
};    

/// Construct an active layer of water.
class ND280ActiveWaterGapConstructor: public ND280GapConstructor {
public:
  //ND280ActiveWaterGapConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280ActiveWaterGapConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280GapConstructor(n,c) {Init();};
    ND280ActiveWaterGapConstructor(G4String n, ND280Constructor* p)
        : ND280GapConstructor(n,p) {Init();};
    virtual ~ND280ActiveWaterGapConstructor();

    virtual const G4VisAttributes* GetVisual(void) const;

protected:
    void Init(void) {
        SetMaterialName("ActiveWater");
    };
};

/// Build an inactive layer of G10.
class ND280G10GapConstructor: public ND280GapConstructor {
public:
    
  //ND280G10GapConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280G10GapConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280GapConstructor(n,c) {Init();};
    
    ND280G10GapConstructor(G4String n, ND280Constructor* p)
        : ND280GapConstructor(n,p) {Init();};
    
    virtual ~ND280G10GapConstructor();
    
    virtual const G4VisAttributes* GetVisual(void) const;
    
protected:
    
    void Init(void) {
        SetMaterialName("G10");
    }
};

/// Build an inactive layer of Glue.
class ND280GlueGapConstructor: public ND280GapConstructor {
public:
    
  //ND280GlueGapConstructor(G4String n, ND280UserDetectorConstruction* c)
  ND280GlueGapConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280GapConstructor(n,c) {Init();};
    
    ND280GlueGapConstructor(G4String n, ND280Constructor* p)
        : ND280GapConstructor(n,p) {Init();};
    
    virtual ~ND280GlueGapConstructor();
    
    virtual const G4VisAttributes* GetVisual(void) const;
    
protected:
    
    void Init(void) {
        SetMaterialName("FGDGlue");
    }
};

/// Build an inactive layer of FGD Water Module Epoxy.
class ND280WaterModuleEpoxyGapConstructor: public ND280GapConstructor {
public:
    
  //ND280WaterModuleEpoxyGapConstructor(G4String n, ND280UserDetectorConstruction* c)
    ND280WaterModuleEpoxyGapConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280GapConstructor(n,c) {Init();};
    
    ND280WaterModuleEpoxyGapConstructor(G4String n, ND280Constructor* p)
        : ND280GapConstructor(n,p) {Init();};
    
    virtual ~ND280WaterModuleEpoxyGapConstructor();
    
    virtual const G4VisAttributes* GetVisual(void) const;
    
protected:
    
    void Init(void) {
        SetMaterialName("FGDWaterModuleEpoxy");
    }
};

/// Build an inactive layer of Polypropylene.
class ND280PropyleneGapConstructor: public ND280GapConstructor {
public:

  //ND280PropyleneGapConstructor(G4String n, ND280UserDetectorConstruction* c)
  ND280PropyleneGapConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280GapConstructor(n,c) {Init();};

    ND280PropyleneGapConstructor(G4String n, ND280Constructor* p)
        : ND280GapConstructor(n,p) {Init();};

    virtual ~ND280PropyleneGapConstructor();

    virtual const G4VisAttributes* GetVisual(void) const;

protected:

    void Init(void) {
        SetMaterialName("Polypropylene");
    }
};

#endif
