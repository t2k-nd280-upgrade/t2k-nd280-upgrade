//#include "cfortran/cfortran.h"
//#include "cfortran/hbook.h"
// comment out by akira 090918

#include "G4VPhysicalVolume.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"

#include "IngridVLayerSD.hh"

#define DEBUG 0

IngridVLayerSD::IngridVLayerSD(const G4String& name)
  : G4VSensitiveDetector(name)
{
  collectionName.insert("vlayerHitCollection");
  ingresp = new IngridResponse();
}


IngridVLayerSD::~IngridVLayerSD()
{
    if(ingresp!=NULL) delete ingresp;
}


void IngridVLayerSD::Initialize(G4HCofThisEvent* HCTE)
{
    //G4cout << "Initialization of vlayerSD" << G4endl;

  vlayerHitCollection = 
    new IngridVLayerHitsCollection(SensitiveDetectorName,collectionName[0]);
  TotalvlayerDep = 0.;
  
  static int HCID = -1;
  if(HCID<0) HCID = GetCollectionID(0); 
  HCTE->AddHitsCollection(HCID,vlayerHitCollection);
}



G4bool IngridVLayerSD::ProcessHits(G4Step* aStep, 
				G4TouchableHistory* ROhist)
{
  // only when there is energy deposit in a sensitive detector,
  // create a new hit.
   
  G4Track* track = aStep->GetTrack();
  G4double edep = aStep->GetTotalEnergyDeposit();
  G4double length = aStep->GetStepLength();

  //  G4cout << "length : " << length << G4endl;

  if(edep==0.) return false;
 

  TotalvlayerDep += edep;
  // volume information must be extracted from Touchable of "PreStepPoint"
  // e.g.
  const G4VTouchable* Touchable = aStep->GetPreStepPoint()->GetTouchable();
  G4int detID = Touchable->GetVolume(0)->GetCopyNo();
    

    G4int trackID = track->GetTrackID();
#if DEBUG
    G4cout << "vlayer-Track ID ; " << trackID << G4endl;
#endif

  G4int PDG = track->GetDefinition()->GetPDGEncoding();
  G4ThreeVector hitPos = aStep->GetPreStepPoint()->GetPosition();
  G4double hittime = aStep->GetPreStepPoint()->GetGlobalTime();
  
  //
  G4double edep_q = edep;
  ingresp->ApplyScintiResponse(&edep_q,track);

  //
  IngridVLayerHit* aHit 
    = new IngridVLayerHit(detID,PDG,trackID,edep,edep_q,hitPos,hittime);
    
  IngridVLayerHit* bHit;
 
  for(int k=0;k<vlayerHitCollection->entries();k++){
    bHit = (*vlayerHitCollection)[k];

    if(bHit->CompareID(*aHit)){
      bHit->AddEdep(edep,edep_q);

			if(bHit->isFaster(*aHit)) { 
			  bHit->SetTime( aHit->GetTime() );
			}
			if(bHit->LargerEdep(*aHit)) { 
			  bHit->SetParticle(aHit->GetParticle()); 
			}
      return true;
    }
  }

  vlayerHitCollection->insert( aHit );

  return true;
}

void IngridVLayerSD::EndOfEvent(G4HCofThisEvent* HCTE)
{
	IngridVLayerHit *cHit;

	G4double edep_tmp;
	G4double time_tmp;
	G4ThreeVector posinmod;
	G4int mod;
	G4int view;
	G4int adc;
	G4int loadc;
	G4double pe;
	G4double lope;

        G4int pln; //added for B2 2014/1/27

	//
	// apply ingrid response
	for(G4int k=0;k<vlayerHitCollection->entries();k++) {
		cHit = (*vlayerHitCollection)[k];
		edep_tmp = cHit->GetEdepQ();
		time_tmp = cHit->GetTime();
		posinmod = cHit->GetPosInMod();
		mod = cHit->GetMod();
		view = cHit->GetView();
                pln = cHit->GetPln();

		//apply light collection
		//ingresp->ApplyLightCollection(&edep_tmp,mod,view,posinmod);
		ingresp->ApplyLightCollection(&edep_tmp,mod,view,posinmod,pln);//B2

		//apply fiber attenuation
		//ingresp->ApplyFiberResponse(&edep_tmp,&time_tmp,view,posinmod);
		ingresp->ApplyFiberResponse(&edep_tmp,mod,&time_tmp,view,posinmod,pln);//B2

		//convert edep -> p.e. & cross & after pulse
		ingresp->ApplyMPPCResponse(edep_tmp,&pe);


		//apply ADC responce
		ingresp->ApplyADCResponse(&pe,&lope,&adc,&loadc);

		//fill variable to hitcollection
		cHit->SetPE(pe);
		cHit->SetLOPE(lope);
		cHit->SetDelayTime(time_tmp);
	}
}

void IngridVLayerSD::DrawAll()
{
  for(G4int k=0; k < vlayerHitCollection->entries(); k++)
   (*vlayerHitCollection)[k]->Draw(); 
}

void IngridVLayerSD::PrintAll()
{
   for(G4int k=0; k < vlayerHitCollection->entries(); k++)
     (*vlayerHitCollection)[k]->Print(); 
   //vlayerHitCollection-> PrintAllHits();
}

