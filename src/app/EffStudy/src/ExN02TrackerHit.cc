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
#include <G4Step.hh>

#include "ND280TrajectoryMap.hh"

#include <fstream>
#include "Const.hh"

//#include "G4SystemOfUnits.hh" // NEW GLOBAL
#include <CLHEP/Units/SystemOfUnits.h>

#define SideView 0 // resolution of vertical-direction

G4Allocator<ExN02TrackerHit> ExN02TrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerHit::ExN02TrackerHit(double maxSagitta, double maxLength, G4int dID0, G4int P0, G4int trkid, G4int parentid, G4double e, G4double charge, G4ThreeVector pos, G4double t) 
{
  // NEW ND280 SEGMENT
  fMaxSagitta = maxSagitta;
  fMaxLength = maxLength;
  fPath.reserve(500);
  /////
  fDetID = dID0;
  fTrackID = trkid;
  fParentID = parentid;
  fEdep = e;
  fCharge = charge;
  fParticle = P0;
  fPosition = pos;
  fTime = t;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......   

// NEW ND280 HitSegment 

ExN02TrackerHit::ExN02TrackerHit(double maxSagitta, double maxLength)
{
  fMaxSagitta = maxSagitta;
  fMaxLength = maxLength;
  fPath.reserve(500);
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
  fCharge     = right.fCharge;
  fParticle = right.fParticle; 
  for(int i=0;i<3;i++) fPosition[i] = right.fPosition[i];
  fTrackID = right.fTrackID;
  fParentID = right.fParentID;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const ExN02TrackerHit& ExN02TrackerHit::operator=(const ExN02TrackerHit& right)
{
  fDetID = right.fDetID;
  fEdep       = right.fEdep;
  fCharge      = right.fCharge;
  fParticle = right.fParticle; 
  for(int i=0;i<3;i++) fPosition[i] = right.fPosition[i];
  fTrackID = right.fTrackID;
  fParentID = right.fParentID;
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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02TrackerHit::Print()
{
  G4cout.precision(4);
  G4cout << " detID:" << fDetID
	 << " detName: " << fNameDet
	 << " Edep:" << fEdep
	 << " Charge:" << fCharge
         << " Particle:" << fParticle
	 << " Trk:" << fTrackID
	 << " Parent:" << fParentID
         << " pos:{" << fPosition[0] << ", "
         << fPosition[1] << ", " << fPosition[2]
         << "}" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......





//
// NEW ND280 SEGMENT
//

// The following code up to the end is added to use
// the ND280HitSegment class in this framework

bool ExN02TrackerHit::SameHit(G4Step* theStep) {

  //G4cout << "ExN02TrackerHit::SameHit" << G4endl;

    // Check that the hit and new step are in the same volume
    G4TouchableHandle touchable 
        = theStep->GetPreStepPoint()->GetTouchableHandle();
    if (fHitVolume != touchable) return false;

    // Check that the hit and new step are close together.
    double endToEnd
        = (theStep->GetPostStepPoint()->GetPosition() - fPath.front()).mag();
    if (endToEnd > fMaxLength) return false;

    double deltaT = std::abs(theStep->GetPostStepPoint()->GetGlobalTime()
                             - fStopT);
    if (deltaT>1*CLHEP::ns) return false;

    int trackId = theStep->GetTrack()->GetTrackID();
    if (fContributors.front() == trackId) {
        // This is still the same track that initially created this hit.
        // Check to see if the hit should be extended, or if we should start a
        // new segment.
        double sagitta = FindSagitta(theStep);
        if (sagitta > fMaxSagitta) return false;
    }
    else {
        // This is not the same track that started this hit, but check to see
        // if it is a delta-ray that should be added to this segment.
        double separation = FindSeparation(theStep);
        if (separation > fMaxSagitta) return false;
    }

    return true;
}

int ExN02TrackerHit::FindPrimaryId(G4Track *theTrack) {

  //G4cout << "ExN02TrackerHit::FindPrimaryId" << G4endl;
    return ND280TrajectoryMap::FindPrimaryId(theTrack->GetTrackID());
}

void ExN02TrackerHit::AddStep(G4Step* theStep, double start, double end) {
  (void)start;
  (void)end;
  
  //G4cout << "ExN02TrackerHit::AddStep" << G4endl;

  G4TouchableHandle touchable 
        = theStep->GetPreStepPoint()->GetTouchableHandle();
    G4ThreeVector prePos = theStep->GetPreStepPoint()->GetPosition();
    G4ThreeVector postPos = theStep->GetPostStepPoint()->GetPosition();
    G4StepStatus stepStatus = theStep->GetPostStepPoint()->GetStepStatus();
    G4ParticleDefinition* particle =  theStep->GetTrack()->GetDefinition();
    double energyDeposit = theStep->GetTotalEnergyDeposit();
    double stepLength = (prePos-postPos).mag();
    double trackLength = theStep->GetStepLength();

    if (trackLength < 0.75*stepLength || trackLength < stepLength - 1*CLHEP::mm) {
        // ND280Warn("Track length shorter than step: " 
        //           << trackLength/CLHEP::mm << " CLHEP::mm "
        //           << "<" << stepLength/CLHEP::mm << " CLHEP::mm");
        // ND280Warn("    Volume: "
        //           << theStep->GetTrack()->GetVolume()->GetName());
        // ND280Warn("    Particle: " << particle->GetParticleName()
        //           << " Depositing " << energyDeposit/MeV << " MeV");
        // ND280Warn("    Total Energy: " 
        //           << theStep->GetTrack()->GetTotalEnergy());
    }

    trackLength = std::max(trackLength,stepLength);

    if (energyDeposit <= 0.) {
      //ND280Warn("ND280HitSegment:: No energy deposited: " << energyDeposit);
    }

    if (trackLength <= 0.) {
      //ND280Warn("ND280HitSegment:: No track length: " << trackLength);
    }

    // Occasionally, a neutral particle will produce a particle below
    // threshold, and it will be recorded as generating the hit.  All of the
    // energy should be deposited at the stopping point of the track.
    if (stepStatus == fPostStepDoItProc
        && std::abs(particle->GetPDGCharge()) < 0.1) {
        double origStep = stepLength;
        G4ThreeVector dir = (postPos - prePos).unit();
        stepLength = trackLength = std::min(0.5*CLHEP::mm,0.8*origStep);
        prePos = postPos - stepLength*CLHEP::mm*dir;
        // ND280Debug("ND280HitSegment:: " << particle->GetParticleName()
        //            << " Deposited " << energyDeposit/MeV << " MeV");
        // ND280Debug("    Original step: " << origStep/mm << " mm");
        // ND280Debug("    New step: " << stepLength/mm << " mm");
    }

    if (stepLength>fMaxLength || trackLength>fMaxLength) {
      //G4Track* trk = theStep->GetTrack(); // not used
      
      // ND280Warn("ND280HitSegment:: Long step in "
        //           << trk->GetVolume()->GetName());
        // ND280Warn("  Step Length is " 
        //           << stepLength/mm 
        //           << " mm and track length is " 
        //           << trackLength/mm << " mm");

        // ND280Warn("  PID: " << particle->GetParticleName()
        //           << " E: " << trk->GetTotalEnergy()/MeV << " MeV"
        //           << " (kin: " << trk->GetKineticEnergy()/MeV << " MeV"
        //           << " Deposit: " 
        //           << energyDeposit/MeV << "MeV"
        //           << " Status: " << theStep->GetPreStepPoint()->GetStepStatus()
        //           << " -> " << theStep->GetPostStepPoint()->GetStepStatus());
        
        const G4VProcess* proc = theStep->GetPostStepPoint()
            ->GetProcessDefinedStep();
        if (!proc) {
	  //ND280Warn("    Step Limit Reached");
        }
        else {
	  //ND280Warn("    Process: " << proc->GetProcessName()
	  //<<"/"<< proc->GetProcessTypeName(proc->GetProcessType()));
        }
    }

    /// First make sure the step has been initialized.
    if (!fHitVolume) {
        fHitVolume = touchable;

	// NEW ND280 HitSegment (not in nd280mc)
	string detname = touchable->GetVolume()->GetLogicalVolume()->GetName();
	SetDetName(detname);
	SetPDG(theStep->GetTrack()->GetDefinition()->GetPDGEncoding());
	//
	
        fPrimaryId = FindPrimaryId(theStep->GetTrack());
        fStartX = prePos.x();
        fStartY = prePos.y();
        fStartZ = prePos.z();
        fStartT = theStep->GetPreStepPoint()->GetGlobalTime();
        fPath.push_back(G4ThreeVector(fStartX,fStartY,fStartZ));
        fStopX = postPos.x();
        fStopY = postPos.y();
        fStopZ = postPos.z();
        fStopT = theStep->GetPostStepPoint()->GetGlobalTime();
        fPath.push_back(G4ThreeVector(fStopX,fStopY,fStopZ));
        fContributors.push_back(theStep->GetTrack()->GetTrackID());
    }
    else {
        // Record the tracks that contribute to this hit.
        int trackId = theStep->GetTrack()->GetTrackID();
        fContributors.push_back(trackId);

        // Check to see if we have a new stopping point.
        if (trackId == fContributors.front()
            && (fPath.back()-prePos).mag()<0.1*CLHEP::mm) {
            fStopX = postPos.x();
            fStopY = postPos.y();
            fStopZ = postPos.z();
            fStopT = theStep->GetPostStepPoint()->GetGlobalTime();
            fPath.push_back(G4ThreeVector(fStopX,fStopY,fStopZ));
        }
    }

    fEnergyDeposit += energyDeposit;
    fTrackLength += trackLength;
    
    //G4cout << "fEnergyDeposit = " << fEnergyDeposit << G4endl;
}

double ExN02TrackerHit::FindSagitta(G4Step* theStep) {
    const G4ThreeVector& point = fPath.back();
    const G4ThreeVector& preStep = theStep->GetPreStepPoint()->GetPosition();

    // Make sure that the step began at the end of the current segment.  If
    // not, return a ridiculously large sagitta.
    if ((point-preStep).mag()>0.01*CLHEP::mm) return 10*CLHEP::m;

    const G4ThreeVector& postStep = theStep->GetPostStepPoint()->GetPosition();
    // The proposed new segment direction; 
    G4ThreeVector newDir = (postStep-point).unit();
    
    // Initialize the maximum sagitta found.
    double maxSagitta = 0.0;

    G4ThreeVector& front = fPath.front();

    // Loop over the existing path points and see if any would fall outside of
    // the tolerance.
    for (std::vector<G4ThreeVector>::iterator p = fPath.begin();
         p != fPath.end();
         ++p) {
        G4ThreeVector delta = ((*p)-front);
        double s = (delta*newDir);
        maxSagitta = std::max(maxSagitta,(delta - s*newDir).mag());
        if (maxSagitta > fMaxSagitta) break;
    }

    return maxSagitta;
}

double ExN02TrackerHit::FindSeparation(G4Step* theStep) {
    G4ThreeVector& front = fPath.front();
    const G4ThreeVector& back = fPath.back();
    const G4ThreeVector& preStep = theStep->GetPreStepPoint()->GetPosition();
    const G4ThreeVector& postStep = theStep->GetPostStepPoint()->GetPosition();
    G4ThreeVector dir = (back-front).unit();

    // Check to make sure the beginning of the new step isn't after the
    // end of this segment.
    if ((preStep-back)*dir>0.0) return 10*CLHEP::m;

    // Check to make sure the beginning of the new step isn't before the
    // beginning of this segment.
    G4ThreeVector frontDelta = preStep-front;
    double cosDelta = frontDelta*dir;
    if (cosDelta<0.0) return 10*CLHEP::m;

    // Find the distance from the segment center line to the initial point of
    // the new step.
    double s1 = (frontDelta - cosDelta*dir).mag();


    // Find the distance from the segment center line to the final point of
    // the new step.
    G4ThreeVector rearDelta = postStep-front;
    cosDelta = rearDelta*dir;
    double s2 = (rearDelta - cosDelta*dir).mag();
    
    return std::max(s1,s2);
}

double ExN02TrackerHit::GetLength() const {
    return (G4Point3D(fStartX, fStartY, fStartZ)
            - G4Point3D(fStopX, fStopY, fStopZ)).mag();
}

////////
