
// 
// Class to define an ND280 upgrade vertex
// 
// 2/8/16 Davide Sgalaberna
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "TND280UpVertex.hh"

using namespace std;

ClassImp(TND280UpVertex)

TND280UpVertex::TND280UpVertex(){
  fVertexID = -9999999;
  fReacMode = "";
  
  fNTracks = 0;
  fListOfTracks = new TList;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TND280UpVertex::~TND280UpVertex(){
  //delete ...;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......   

void TND280UpVertex::AddTrack(TND280UpTrack *track)
{
  fListOfTracks->Add(track);
  fNTracks = fListOfTracks->GetEntries(); // Update # of tracks in the vertex
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

TND280UpTrack * TND280UpVertex::GetTrack(int trkid)
{
  TObject *objtrack = fListOfTracks->At(trkid);
  if(!objtrack){
    cerr << endl;
    cerr << "TND280UpVertex::GetTrack" << endl;
    cerr << "The index " << trkid << " is out of range" << endl;
    cerr << endl;
    exit(1);
  }
  TND280UpTrack *track = dynamic_cast<TND280UpTrack*>(objtrack); 
  return track;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......   

void TND280UpVertex::PrintVertex()
{
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
  cout << "Vertex ID: " << fVertexID << endl;
  cout << "Reaction mode: " << fReacMode << endl;
  cout << "# of tracks: " << fNTracks << endl;
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
