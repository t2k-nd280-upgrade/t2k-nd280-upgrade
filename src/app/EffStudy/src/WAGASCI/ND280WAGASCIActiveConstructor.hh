//##################################################################################
//##################################################################################
//################### Created by B. Quilain on 2017/05/17 ##########################
//################### Last modification by B. Quilain on 2017/05/17 ################
//##################################################################################
//##################################################################################
#ifndef ND280WAGASCIActiveConstructor_hh_seen
#define ND280WAGASCIActiveConstructor_hh_seen
// $Id: ND280WAGASCIActiveConstructor.hh,v 1.6 2011/06/07 18:37:40 mcgrew Exp $
//

class G4LogicalVolume;

#include "ND280Constructor.hh"
#include "G4SDManager.hh"

#include "ND280RootPersistencyManager.hh" // XML input file  
#include "ExN02ND280XML.hh" // XML input file

class ND280WAGASCIActiveConstructor : public ND280Constructor {
//class ND280WAGASCIActiveConstructor {
public:

  // D.S
  // Made this class more similar to the other ones
  // though it uses its own methods (e.g. GetPiece is not used)

  // D.S
  ND280WAGASCIActiveConstructor(G4String n, ExN02DetectorConstruction* c)
    : ND280Constructor(n,c) {Init();};
  ND280WAGASCIActiveConstructor(G4String n, ND280Constructor* p)
    : ND280Constructor(n,p) {Init();};
  virtual ~ND280WAGASCIActiveConstructor();
  /////
  
  /// Construct and return a G4 volume for the object.  This is a pure
  /// virtual function, which means it must be implemented by the inheriting
  /// classes.  This returns an unplaced logical volume which faces along
  /// the Z axis.
  virtual G4LogicalVolume* GetPiece(void); // D.S (not using it)

  void Construct(G4LogicalVolume * TargetLV, G4ThreeVector ModulePosition, G4ThreeVector ModuleSize, const G4String RepositoryName);

  /// Initialize the constructor.
  void Init(void);

  ExN02ND280XML * GetND280XML() const {return fND280XMLInput;};

private:

  ExN02ND280XML *fND280XMLInput;

};
#endif
