#ifndef ND280WAGASCIElements_hh_seen
#define ND280WAGASCIElements_hh_seen
#include <stdio.h>
#include <iostream>
#include <vector>

#include "G4Element.hh"
#include "G4Material.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

/// Elements used in WAGASCI construction
/// TO DO: Merge them with the elements of the other upgrade parts: one defintion of scinti material, water material etc...
namespace ND280WAGASCIElements
{
  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  G4String name, symbol;
  G4int nel;
  G4double iz;

  G4Element* elO;
  G4Element* elH;
  G4Element* elC;

  G4Material * Air;
  G4Material * Water;
  G4Material * Scinti;

}


#endif
