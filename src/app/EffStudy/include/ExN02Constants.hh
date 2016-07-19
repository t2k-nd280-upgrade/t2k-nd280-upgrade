#ifndef constants_h
#define constants_h

#include "G4SystemOfUnits.hh"

/////////////////////////////////                 
//                             //                    
//      Flags for compiler     //                         
//                             //          
/////////////////////////////////                                       

//#define USE_PAI

//#define USE_BLINETRACER // Use the BlineTracer class w/ bline_vis.mac
                          // Plot geantino along magnetic field direction
 
/////////////////////////////////                 
//                             //                    
//      General constants      //                         
//                             //          
/////////////////////////////////                                       

const G4double kBadNum = -9999999.;


////////////////////////////
//                        //
// Default Magnetic field //
//                        //
////////////////////////////

const G4double cMagFieldX   = 0.18 * tesla; // Default ND280 magnetic field (NIM A 659 (2011) 106–135)
const G4double cMinStep     = 1.0 * mm;    // Minimal step of 1 mm is default
const G4int    cStepperType = 4;           // ClassicalRK4 is default stepper

////////////////////////////
//                        //
//       Step limits      //
//                        //
////////////////////////////

const G4double cStepLimit = 1.0 * mm; // used in the TPC drift region for charge particles


#endif
