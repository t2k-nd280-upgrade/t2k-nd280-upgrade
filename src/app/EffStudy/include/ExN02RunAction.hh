//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: ExN02RunAction.hh,v 1.8 2006-06-29 17:47:45 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ExN02RunAction_h
#define ExN02RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

#include "Randomize.hh"

#include <CLHEP/Random/Random.h>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;
class ExN02EventAction;

/// Run action class              
///                                   
/// It accumulates statistic and computes dispersion of the energy deposit       
/// and track lengths of charged particles with use of analysis tools: 
/// H1D histograms are created in BeginOfRunAction() for the following  
/// physics quantities:                
/// - Edep in absorber                
/// - Edep in gap                    
/// - Track length in absorber     
/// - Track length in gap             
/// The same values are also saved in the ntuple. 
/// The histograms and ntuple are saved in the output file in a format       
/// accoring to a selected technology in B4Analysis.hh. 
///                                                   
/// In EndOfRunAction(), the accumulated statistic and computed    
/// dispersion is printed.              
///                                   


class ExN02RunAction : public G4UserRunAction
{
public:
  ExN02RunAction(ExN02EventAction* eventAction);
  virtual ~ExN02RunAction();
  
  //virtual G4Run* GenerateRun();
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);
 
  // Set the seed to a new value.  This takes a long since the low-level
  // random generate expects a long seed.
  void SetSeed();
  void SetSeed(long);
  
  // Get the seed that started the low level random generator.
  long GetSeed(void) const;
 
private:
  ExN02EventAction* fEventAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif





