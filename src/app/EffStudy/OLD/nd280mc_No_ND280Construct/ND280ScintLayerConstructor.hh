#ifndef ND280ScintLayerConstructor_hh_Seen
#define ND280ScintLayerConstructor_hh_Seen
// $Id: ND280ScintLayerConstructor.hh,v 1.16 2011/11/22 21:23:21 mcgrew Exp $
//

#include <G4RotationMatrix.hh>

#include "ND280ComponentConstructor.hh"
#include "ND280ExtrudedScintConstructor.hh"

class G4VisAttributes;
class G4LogicalVolume;

class G4VSensitiveDetector;

/// Construct a layer of scintillator for tracking.  A scintillator layer is
/// either X or Y.
class ND280ScintLayerConstructor : public ND280ComponentConstructor {
public:
    // ND280ScintLayerConstructor(G4String n, ND280UserDetectorConstruction* c)
    //     : ND280ComponentConstructor(n,c) {Init();};
    // ND280ScintLayerConstructor(G4String n, ND280Constructor* p)
    //     : ND280ComponentConstructor(n,p) {
    //     Init();};

  ND280ScintLayerConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280ComponentConstructor(n,c) {Init();};
  ND280ScintLayerConstructor(G4String n)
    : ND280ComponentConstructor(n) {Init();};


    virtual ~ND280ScintLayerConstructor(); 

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.    
    virtual G4LogicalVolume *GetPiece(void);

    /// Flag that an X layer should be constructed.
    void MakeXLayer(void) {fMakeXLayer=true;};

    /// Flag that an X layer should be constructed.
    void MakeYLayer(void) {fMakeXLayer=false;};
    
    /// Flag that local +z direction of bars in layer should all
    /// point the same way
    void MakeParallelBars(void) {fAntiParallelBars=false;};

    /// Flag that local +z direction of bars in layer should point
    /// opposite to neighbouring bars
  void MakeAntiParallelBars(void) {fAntiParallelBars=true;};

  /// Set the number of bars used in a layer.
    void SetBarNumber(G4int n);

    /// Get the number of bars used in a layer.
    G4int GetBarNumber(void);

    /// Set the base width of the extruded bars that will be used to construct
    /// this layer. 
    void SetBarBase(double b);

    /// Get the base width of the extruded bars used to construct this layer.
    double GetBarBase(void);

    /// This is the thickness of the bar along the beam axis.  For a
    /// triangular bar, this is the height of the triangle.  By default, this
    /// is set to the base width of the extruded bars that will be used to
    /// construct this layer.
    void SetBarHeight(double t);

    /// Get the base width of the extruded bars used to construct this layer.
    double GetBarHeight(void);

    /// Set the extruded bar's coating thickness
    void SetBarCoatingThickness(double t);

    /// Get the extruded bar's coating thickness
    double GetBarCoatingThickness(void);

    /// Set the extruded bar's coating corner radius
    void SetBarCoatingRadius(double r);

    /// Get the extruded bar's coating corner radius
    double GetBarCoatingRadius(void);

    /// Set the amount of glue between each extruded bars.
    void SetBarGap(double t);

    /// Get the amount of glue between each extruded bar.
    double GetBarGap();

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

    /// Set the amount of unused space around the edges.
    void SetEdgeSpace(double d) {fEdgeSpace = d;}

    /// Get the amount of unused space around the edges.
    double GetEdgeSpace(void) {return fEdgeSpace;}

    /// Make a square bar.
    void SquareBar(void);

    /// Make a triangular bar.
    void TriangularBar(void);

    /// Set the material to use for the scintillator coating.
    void SetCoatingMaterial(const char* mat);

    /// Set the material to use for the scintillator.
    void SetScintillatorMaterial(const char* mat);
    
    /// Set the material to use for the fiber.
    void SetFiberMaterial(const char* mat);

    virtual G4VisAttributes* GetVisual(void) const;

private:

    void Init(void);

  //
  /// To replace AddConstructor (inherited from ND280Constructor)
  //
  ND280ExtrudedScintConstructor *fExtrBarScintConstr;

    /// Flag to build a X layer.
    bool fMakeXLayer;
    
    /// Flag to rotate bars 180 degrees with respect
    /// to neighbouring bars in layer.
    bool fAntiParallelBars;

    /// The amount of unused space around the edge.
    double fEdgeSpace;

    /// The number of bars in the layer as specified by the
    /// command 'barNumbers'.
    G4int fNumBars;

    /// The number of bars actually used in a layer.
    G4int fNumberOfBars;
};

class ND280ScintXLayerConstructor: public ND280ScintLayerConstructor {
public: 
    // ND280ScintXLayerConstructor(G4String n, ND280UserDetectorConstruction* c)
    //     : ND280ScintLayerConstructor(n,c) {Init();}
    // ND280ScintXLayerConstructor(G4String n, ND280Constructor* p)
    //     : ND280ScintLayerConstructor(n,p) {Init();}
    ND280ScintXLayerConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280ScintLayerConstructor(n,c) {Init();}
    ND280ScintXLayerConstructor(G4String n)
        : ND280ScintLayerConstructor(n) {Init();}
    virtual ~ND280ScintXLayerConstructor();

    virtual G4VisAttributes* GetVisual(void) const;

protected:
    void Init(void) {
        MakeXLayer();
    };

};    

class ND280ScintYLayerConstructor: public ND280ScintLayerConstructor {
public: 
    // ND280ScintYLayerConstructor(G4String n, ND280UserDetectorConstruction* c)
    //     : ND280ScintLayerConstructor(n,c) {Init();}
    // ND280ScintYLayerConstructor(G4String n, ND280Constructor* p)
    //     : ND280ScintLayerConstructor(n,p) {Init();}

    ND280ScintYLayerConstructor(G4String n, ExN02DetectorConstruction* c)
        : ND280ScintLayerConstructor(n,c) {Init();}
  ND280ScintYLayerConstructor(G4String n)
        : ND280ScintLayerConstructor(n) {Init();}

    virtual ~ND280ScintYLayerConstructor();

    virtual G4VisAttributes* GetVisual(void) const;

protected:
    void Init(void) {
        MakeYLayer();
    };

};    
#endif
