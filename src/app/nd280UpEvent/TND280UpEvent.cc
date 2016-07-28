
// 
// Class to define an ND280 upgrade event
// 
// 27/7/16 Davide Sgalaberna
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "TND280UpEvent.hh"

using namespace std;

ClassImp(TND280UpEvent)

TND280UpEvent::TND280UpEvent(){
  fEventID = -999;
  fReacMode = "";
  
  fNTracks = 0;
  fListOfTracks = new TList;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TND280UpEvent::~TND280UpEvent(){
  //delete ...;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......   

void TND280UpEvent::AddTrack(TND280UpTrack *track)
{
  fListOfTracks->Add(track);
  fNTracks = fListOfTracks->GetEntries(); // Update # of tracks in the event
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

TND280UpTrack * TND280UpEvent::GetTrack(int trkid)
{
  TObject *objtrack = fListOfTracks->At(trkid);
  if(!objtrack){
    cerr << endl;
    cerr << "TND280UpEvent::GetTrack" << endl;
    cerr << "The index " << trkid << " is out of range" << endl;
    cerr << endl;
    exit(1);
  }
  TND280UpTrack *track = dynamic_cast<TND280UpTrack*>(objtrack); 
  return track;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......   

void TND280UpEvent::PrintEvent()
{
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
  cout << "Event ID: " << fEventID << endl;
  cout << "Reaction mode: " << fReacMode << endl;
  cout << "# of tracks: " << fNTracks << endl;
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
