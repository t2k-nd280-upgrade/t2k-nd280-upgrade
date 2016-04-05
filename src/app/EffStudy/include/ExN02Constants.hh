#ifndef constants_h
#define constants_h

#include "G4SystemOfUnits.hh"

////////////////////////////
//                        //
//      Nomenclature      //
//                        //
////////////////////////////

// Detector names

const G4String cNameSolidWorld       = "world";
const G4String cNameLogicWorld       = "World";
const G4String cNamePhysiWorld       = "World";

const G4String cNameSolidTracker     = "tracker";
const G4String cNameLogicTracker     = "Tracker";
const G4String cNamePhysiTracker     = "Tracker";

const G4String cNameSolidTarget      = "target";
const G4String cNameLogicTarget      = "Target";
const G4String cNamePhysiTarget      = "Target";

const G4String cNameSolidChamberUp   = "tpcup";
const G4String cNameLogicChamberUp   = "TPCUp";
const G4String cNamePhysiChamberUp   = "TPCUp";

const G4String cNameSolidChamberDown = "tpcdown";
const G4String cNameLogicChamberDown = "TPCDown";
const G4String cNamePhysiChamberDown = "TPCDown";

// Material names (see DetectorConstruction::DefineMaterials())

const G4String cWorldMater   = "Air";
const G4String cTrackerMater = "Air";
const G4String cTargetMater  = "ActiveWater"; //fgd active water
//const G4String cTargetMater  = "FGDScintillator"; //fgd scintillator
const G4String cChamberMater = "GasMixtureTPC"; // gas mixture of ND280 TPCs


////////////////////////////
//                        //
// Dimension of detectors //
//                        //
////////////////////////////

// Water Target
const G4double cTargetLength = 240.0 * cm; 
const G4double cTargetWidth  = 230.0 * cm; 
const G4double cTargetHeight = 30.3  * cm;   

// Side TPCs
const G4double cChamberLength = cTargetLength;// cm; 
const G4double cChamberWidth  = cTargetWidth; // cm;
const G4double cChamberHeight = 100.0 * cm;  

// Tracker 
const G4double cTrackerLength = cTargetLength;                 // Full length of Tracker
const G4double cTrackerWidth  = cTargetWidth;                  // Full width of Tracker
const G4double cTrackerHeight = cTargetHeight + cChamberHeight*2; // Full height of Tracker

// World
const G4double cWorldLength = 1.2 *(cTrackerLength);
const G4double cWorldWidth  = 1.2 *(cTrackerWidth);
const G4double cWorldHeight = 1.2 *(cTrackerHeight);

////////////////////////////
//                        //
//       Step limits      //
//                        //
////////////////////////////

const G4double cMaxStep = 0.5*cChamberWidth;


#endif
