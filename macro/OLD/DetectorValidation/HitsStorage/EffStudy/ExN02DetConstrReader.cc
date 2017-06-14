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
/// \file persistency/P02/src/ExP02DetConstrReader.cc
/// \brief Implementation of the ExP02DetConstrReader class
//
// $Id: ExP02DetConstrReader.cc 71727 2013-06-21 07:55:45Z gcosmo $
//
//ROOT
#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "Cintex/Cintex.h"

//G4
#include "G4Element.hh"
#include "G4PVPlacement.hh"
#include "ExN02GeoTree.hh"

// local
#include "ExN02DetConstrReader.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02DetConstrReader::ExN02DetConstrReader()
 : G4VUserDetectorConstruction()
{  
  // initialize ROOT
  TSystem ts;
  gSystem->Load("libExN02ClassesDict");

  //  ROOT::Cintex::Cintex::SetDebug(2);
  ROOT::Cintex::Cintex::Enable();
  //  gDebug = 1;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02DetConstrReader::~ExN02DetConstrReader()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* ExN02DetConstrReader::Construct()
{
  ExN02GeoTree* geotree;

  TFile fo("geo.root");
  fo.GetObject("my_geo", geotree);

  return geotree->TopVol();
}
