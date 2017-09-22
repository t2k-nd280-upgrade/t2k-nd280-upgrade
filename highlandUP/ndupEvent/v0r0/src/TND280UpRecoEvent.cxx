
// 
// Class to define an ND280 upgrade reconstructed event
// 
// 4/7/17 Davide Sgalaberna
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

#include "TND280UpRecoEvent.hxx"

using namespace std;

ClassImp(TND280UpRecoEvent)

TND280UpRecoEvent::TND280UpRecoEvent(){
  fEventID = -9999999;
  //fReacMode = "";
  
  fNTracks = 0;
  fListOfTracks = new TList;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TND280UpRecoEvent::~TND280UpRecoEvent(){
  //delete ...;

  fEventID = -9999999;

  //fListOfTracks->SetOwner(true);
  fListOfTracks->Delete();
  delete fListOfTracks;
  //fListOfTracks = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......   

void TND280UpRecoEvent::AddTrack(TND280UpRecoTrack *track)
{
  fListOfTracks->Add(track);
  fNTracks = fListOfTracks->GetEntries(); // Update # of tracks in the event
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

TND280UpRecoTrack * TND280UpRecoEvent::GetTrack(int trkid)
{
  TObject *objtrack = fListOfTracks->At(trkid);
  if(!objtrack){
    cerr << endl;
    cerr << "TND280UpRecoEvent::GetTrack" << endl;
    cerr << "The index " << trkid << " is out of range" << endl;
    cerr << endl;
    exit(1);
  }
  TND280UpRecoTrack *track = dynamic_cast<TND280UpRecoTrack*>(objtrack); 
  return track;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
void TND280UpRecoEvent::PrintEvent()
{
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
  cout << "Event ID: " << fEventID << endl;
  cout << "# of tracks: " << fNTracks << endl;
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
