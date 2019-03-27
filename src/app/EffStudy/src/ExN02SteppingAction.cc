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
// $Id: ExN02SteppingAction.cc,v 1.9 2006-06-29 17:48:18 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//#include "G4SteppingManager.hh"

#include "ExN02SteppingAction.hh"
#include "ExN02EventAction.hh"
#include "ExN02DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02SteppingAction::ExN02SteppingAction(
 					 const ExN02DetectorConstruction* detectorConstruction,
					 ExN02EventAction* eventAction)
  : G4UserSteppingAction(),
    fDetConstruction(detectorConstruction),
    fEventAction(eventAction)
{
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02SteppingAction::~ExN02SteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02SteppingAction::UserSteppingAction(const G4Step* step)
{ 
(void)step;
#ifdef NEUTRON_FILE  
  // Collect energy and track length step by step

  // energy deposit
  G4double edep = step->GetTotalEnergyDeposit();
  //if(edep==0.) return;


  //G4cout << "edep = " << edep << G4endl;
  // G4cout << "ParentID = " << step->GetTrack()->GetParentID() << G4endl;
  // return;



  
  G4StepPoint* prestep  = step->GetPreStepPoint();
  G4StepPoint* poststep = step->GetPostStepPoint();
  G4Track *track = step->GetTrack(); // it's PostStepPoint!!!
  
  // get volume of the current step
  //G4VPhysicalVolume* volume = prestep->GetTouchableHandle()->GetVolume();

  // step length 
  G4double stepLength = 0.;

  G4String process_name_post;
  G4String process_name_pre;
  G4String particle_name = track->GetParticleDefinition()->GetParticleName();

  if (particle_name != "neutron")
    return;

  if(step->GetPostStepPoint()->GetProcessDefinedStep() != NULL)
    process_name_post = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

  if (process_name_post == "Transportation" || process_name_post == "")
    return;

  if(step->GetPreStepPoint()->GetProcessDefinedStep() != NULL)
    process_name_pre = step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName();

  G4double energy_pre = step->GetPreStepPoint()->GetKineticEnergy();
  G4double energy_post = step->GetPostStepPoint()->GetKineticEnergy();

    //cout << "Particle  " << particle_name << "   Process pre" <<  process_name_pre << "   post " << process_name_post << endl;
    //cout << "Particle  " << particle_name << "   Process " << process_name_post  << "   pre e " << energy_pre << " post " << energy_post << endl;
  //}

  // USE PRE ENERGY!!!

  //const G4TrackVector* fSecondary = step->GetSecondary();
  const std::vector<const G4Track*>* fSecondary = step->GetSecondaryInCurrentStep();
  //size_t tN2ndariesTot = (*fSecondary).size();
  G4int tN2ndariesTot = (*fSecondary).size();

  G4int Nproton = 0;
  G4int Nalpha  = 0;

  if (process_name_post == "hadElastic" && tN2ndariesTot == 1) {
    if ((*fSecondary)[0]->GetDefinition()->GetParticleName() == "proton") {
      fEventAction->FH(1, energy_pre);
      fEventAction->FH(12, energy_pre);
    }
    if ((*fSecondary)[0]->GetDefinition()->GetParticleName() == "C12") {
      fEventAction->FH(2, energy_pre);
      fEventAction->FH(8, energy_pre);
    }
  } 

  if (process_name_post == "neutronInelastic") {
    fEventAction->FH(3, energy_pre);

    bool B = false;
    bool Li = false;
    G4int threealpha = 0;
    bool Be = false;


    for(size_t lp1=(*fSecondary).size()-tN2ndariesTot; lp1<(*fSecondary).size(); lp1++){
      cout << (*fSecondary)[lp1]->GetDefinition()->GetParticleName() << "    " << (*fSecondary)[lp1]->GetKineticEnergy() << endl;
      G4String particle = (*fSecondary)[lp1]->GetDefinition()->GetParticleName();
      if (particle.contains("Li")) Li = true;
      else if (particle.contains("B") && !particle.contains("Be")) B = true;
      else if (particle.contains("Be")) Be = true;
      else if (particle.contains("alpha")) {
        ++threealpha;
        ++Nalpha;
      } else if (particle.contains("proton"))
        ++Nproton;
    }

    if (Li)
      fEventAction->FH(5, energy_pre);
    if (B)
      fEventAction->FH(4, energy_pre);
    if (Be)
      fEventAction->FH(7, energy_pre);
    if (threealpha == 3)
      fEventAction->FH(6, energy_pre);

    if (!Nalpha && !Nproton)
      fEventAction->FH(9, energy_pre);
    else if (Nalpha == 1 && Nproton == 0)
      fEventAction->FH(10, energy_pre);
    else if (Nalpha > 1 && Nproton == 0)
      fEventAction->FH(11, energy_pre);
    else if (Nalpha == 0 && Nproton == 1)
      fEventAction->FH(12, energy_pre);
    else if (Nalpha == 1 && Nproton == 1)
      fEventAction->FH(13, energy_pre);
    else if (Nalpha > 1 && Nproton == 1)
      fEventAction->FH(14, energy_pre);
    else if (Nalpha > 1 && Nproton > 1)
      fEventAction->FH(15, energy_pre);
  }
#endif


 /* if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
    stepLength = step->GetStepLength();

    
  //G4String namedet = prestep->GetTouchableHandle()->GetVolume()->GetName();        
  //fEventAction->AddAbsTot(edep,stepLength);    

  //fEventAction->SetTrack(step); // IT CRASHES

    
  // // Take track informations from its first step
  // G4int newtrkid      = track->GetTrackID();
  // G4int newparentid   = track->GetParentID();
  // G4int newtrkpdg     = track->GetDefinition()->GetPDGEncoding();
  
  // // Get the total initial energy of the track
  // G4double newtrkmass        = track->GetDefinition()->GetPDGMass();
  // G4double newtrkEkinVtx     = track->GetVertexKineticEnergy();  
  // G4double newtrkEtotVtx     = newtrkEkinVtx + newtrkmass;             
  
  // // Get the total initial momentum
  // //G4ThreeVector newVecMomVtx = track->GetMomentum(); // it's PostStep not PreStep!!!  
  // G4double newtrkMomX   = prestep->GetMomentum().x();
  // G4double newtrkMomY   = prestep->GetMomentum().y();
  // G4double newtrkMomZ   = prestep->GetMomentum().z();
  // G4double newtrkMomMag = prestep->GetMomentum().mag();
  
  //std::cout << "Track ID: = " << newtrkid 
  //<< " - Parent ID: = " << newparentid
  //	    << " - Track PDG: = " << newtrkpdg   
  //	    << " - Track E: = " << newtrkEtotVtx  
  //	    << " - Track Mom: = " << newtrkMomMag   
    //<< " - PreStep Mom: = " << prestep->GetMomentum().mag()
    //<< " - PostStep Mom: = " << poststep->GetMomentum().mag()
  //	    << std::endl;  


  //G4ThreeVector Pos = prestep->GetPosition();
  //G4ThreeVector PosPost = poststep->GetPosition();
  //G4String namedet = theTouchable->GetVolume()->GetLogicalVolume()->GetName(); 
  //if(namedet.contains("FGDlike1")){
  //G4cout << namedet << ": " << G4endl 
  //	   << "PreStep: " << Pos.x() << ", " << Pos.y() << ", " << Pos.z() << G4endl
  //	   << "PostStep: " << PosPost.x() << ", " << PosPost.y() << ", " << PosPost.z() << G4endl;
  //G4cout << endl;
  //G4cout << "..................." << G4endl; 
  //}


  if(track->GetParentID()==0){
    
    //G4cout << " Edep:" << step->GetTotalEnergyDeposit()
    //<< " PID:" << track->GetDefinition()->GetPDGEncoding()
    //<< " Trk:" << track->GetTrackID()
    //<< " Parent:" << track->GetParentID()
    //<< G4endl;
    
    G4StepPoint* preStepPoint = step->GetPreStepPoint();

    G4int trackid        = track->GetTrackID();
    G4int parentid       = track->GetParentID();
    G4double particle    = track->GetDefinition()->GetPDGEncoding();
    G4double prestepTime = preStepPoint->GetGlobalTime();

    G4TouchableHandle theTouchable = preStepPoint->GetTouchableHandle();
    G4String namedet = theTouchable->GetVolume()->GetLogicalVolume()->GetName();
    G4int detID = theTouchable->GetVolume(0)->GetCopyNo();

    if(namedet=="/t2k/OA/Magnet/Basket/target1/SuperFGD1/RepY/RepX/RepZ/CubeScint/Extrusion/Core"){
      G4cout << " detID:" << detID
	     << " detName: " << namedet
	     << " Edep:" << edep
	     << " PID:" << particle
	     << " Trk:" << trackid
	     << " Parent:" << parentid
	     << "}" << G4endl;
    }
  }
  */


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......      
