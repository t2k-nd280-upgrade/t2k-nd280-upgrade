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
// $Id: ExN02TrackerHit.cc,v 1.10 2006-06-29 17:48:24 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ExN02TrackerHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <fstream>
#include "Const.hh"

#define SideView 0 // resolution of vertical-direction

G4Allocator<ExN02TrackerHit> ExN02TrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerHit::ExN02TrackerHit(G4int dID0, G4int P0, G4int trkid, G4int parentid, G4double e, G4double eq, G4ThreeVector pos, G4double t) 
{
  fDetID = dID0;
  fTrackID = trkid;
  fParentID = parentid;
  fEdep = e;
  fEdep_q = eq;
  fParticle = P0;
  fPosition = pos;
  fTime = t;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerHit::ExN02TrackerHit(G4int dID0, G4double e,G4int P0) 
{
  fDetID = dID0;
  fEdep = e;
  fParticle = P0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerHit::ExN02TrackerHit(G4int dID0, G4double e) 
{
  fDetID = dID0;
  fEdep = e;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerHit::ExN02TrackerHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerHit::~ExN02TrackerHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerHit::ExN02TrackerHit(const ExN02TrackerHit& right)
  : G4VHit()
{
  fDetID = right.fDetID;
  fEdep     = right.fEdep;
  fEdep_q     = right.fEdep_q;
  fParticle = right.fParticle; 
  for(int i=0;i<3;i++) fPosition[i] = right.fPosition[i];
  fEventID = right.fEventID;
  // My new
  fTrackID = right.fTrackID;
  fParentID = right.fParentID;
  //

  /*
  // ND280 UPGRADE
  trackID      = right.trackID;
  parentID     = right.parentID;
  edep         = right.edep;
  prestepPos   = right.prestepPos;
  poststepPos  = right.poststepPos;
  namedet      = right.namedet;
  charge       = right.charge;
  mom          = right.mom; 
  trackpdg     = right.trackpdg;
  trackname    = right.trackname;
  tracklength  = right.tracklength;
  steplength   = right.steplength;
  stepdeltalyz = right.stepdeltalyz;
  nsteps       = right.nsteps;  
  trackcostheta= right.trackcostheta;
  ////
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const ExN02TrackerHit& ExN02TrackerHit::operator=(const ExN02TrackerHit& right)
{
  fDetID = right.fDetID;
  fEdep       = right.fEdep;
  fEdep_q      = right.fEdep_q;
  fParticle = right.fParticle; 
  for(int i=0;i<3;i++) fPosition[i] = right.fPosition[i];
  fEventID = right.fEventID;
  // My new
  fTrackID = right.fTrackID;
  fParentID = right.fParentID;
  //

  /*
  // ND280 UPGRADE
  trackID      = right.trackID;
  parentID     = right.parentID;
  chamberNb    = right.chamberNb;
  edep         = right.edep;
  prestepPos   = right.prestepPos;
  poststepPos  = right.poststepPos;
  namedet      = right.namedet;
  charge       = right.charge;
  mom          = right.mom; 
  trackpdg     = right.trackpdg;
  trackname    = right.trackname;
  tracklength  = right.tracklength;
  steplength   = right.steplength;
  stepdeltalyz = right.stepdeltalyz;
  nsteps       = right.nsteps;  
  trackcostheta= right.trackcostheta;
  ////
  */

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int ExN02TrackerHit::operator==(const ExN02TrackerHit& right) const
{
  return (this==&right) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int ExN02TrackerHit::CompareID(const ExN02TrackerHit right) 
{
  return (fDetID==right.fDetID) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int ExN02TrackerHit::CompareP(const ExN02TrackerHit right) 
{
  return (fParticle==right.fParticle) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int ExN02TrackerHit::isFaster(const ExN02TrackerHit right)
{
  return (fTime>right.fTime) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int ExN02TrackerHit::LargerEdep(const ExN02TrackerHit right)
{
  if(fParticle==11) return 1;
  else if(right.fParticle==11) return 0;
  else return (fEdep<right.fEdep) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02TrackerHit::Draw()
{
  double size = fEdep + 7.;
  if( fEdep>9 ) size = 16.;
  #if 1
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
    {
      G4ThreeVector pos; 
      for(int i=0;i<3;i++) pos[i] = fPosition[i]/**mm*/;
      G4Circle circle(pos);
      circle.SetScreenSize(size); 
      circle.SetFillStyle(G4Circle::filled);
      G4Colour colour(0.,1.,1.3); //cyan
      G4VisAttributes attribs(colour);
      circle.SetVisAttributes(attribs);
      pVVisManager->Draw(circle);
    }
  #endif

  // ND280 UPGRADE
  /*
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(prestepPos);
    circle.SetScreenSize(2.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02TrackerHit::Print()
{

  G4cout.precision(4);
  
  //G4cout << " Mod:" << mod 
  G4cout << " detID:" << fDetID
	 << " detName: " << fNameDet
    //<< " Pln:" << pln 
    //<< " Ch:" << ch 
    //<< " Time:" << time
    //<< "  Edep:" << G4BestUnit(edep,"Energy")
	 << " Edep:" << fEdep
    //<< " EdepQ:" << fEdep_q
    //<< " p.e.:" << pe
         << " PID:" << fParticle
    //		 << G4endl;
	 << " Trk:" << fTrackID
	 << " Parent:" << fParentID
         << " pos:{" << fPosition[0]/*/cm*/ << ", "
         << fPosition[1]/*/cm*/ << ", " << fPosition[2]/*/cm*/
         << "}" << G4endl;

  
  /*
  G4cout.precision(5);  
  //if(namedet=="Target" || namedet=="TPC Up"){
  G4cout << "  trackID: " << trackID 
	 << "  track length: " << G4BestUnit(tracklength,"Length") 
	 << "  energy deposit: " << G4BestUnit(edep,"Energy")
    //<< "  position: " << G4BestUnit(pos,"Length") 
    //<< "  momentum: " << G4BestUnit(mom,"Energy")
    //<< "  charge: " << G4BestUnit(charge,"Electric charge")
    //<< "  pdg: " << trackpdg 
	 << "  name: " << trackname
    //<< "  # of steps: " << nsteps
	 << "  detector: " << namedet 
	 << G4endl;
  //}
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

