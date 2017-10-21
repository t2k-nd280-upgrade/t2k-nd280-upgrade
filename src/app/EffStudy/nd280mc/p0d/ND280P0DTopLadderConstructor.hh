#ifndef ND280P0DTopLadderConstructor_hh_Seen
#define ND280P0DTopLadderConstructor_hh_Seen
// $Id: ND280P0DTopLadderConstructor.hh,v 1.1 2009/11/21 14:39:22 mcgrew Exp $
//

#include <G4ThreeVector.hh>
#include <G4VisAttributes.hh>
#include <G4LogicalVolume.hh>

#include "ND280Constructor.hh"

/// Construct a single side electronics ladder.
class ND280P0DTopLadderConstructor : public ND280Constructor {
public:
    ND280P0DTopLadderConstructor(G4String n,
				 //ND280UserDetectorConstruction* c)
				 ExN02DetectorConstruction* c)   
    : ND280Constructor(n,c) {Init();};
    ND280P0DTopLadderConstructor(G4String n, ND280Constructor* p)
        : ND280Constructor(n,p) {Init();};
    virtual ~ND280P0DTopLadderConstructor(); 

    /// Get the total length ladder along the Z axis.  This is the same as the
    /// ladder width.
    double GetLength() const {
        return GetLadderWidth();;
    }
    
    /// Get the total thickness of the ladder.
  double GetLadderThickness() const {return 40*CLHEP::mm;}

    /// Get the total length of the ladder.
  double GetLadderLength() const {return 2000*CLHEP::mm;}

    /// Get the total width of the ladder.
  double GetLadderWidth() const {return 300*CLHEP::mm;}

    /// Construct and return a G4 volume for the object.  This is a pure
    /// virtual function, which means it must be implemented by the inheriting
    /// classes.  This returns an unplaced logical volume which faces along
    /// the Z axis.
    virtual G4LogicalVolume *GetPiece(void);

    /// Return the visual attributes for this constructor.
    virtual const G4VisAttributes* GetVisual(void) const;

protected:
    void Init(void);
};
#endif
