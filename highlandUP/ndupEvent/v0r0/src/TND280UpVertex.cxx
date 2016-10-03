
// 
// Class to define an ND280 upgrade vertex
// 
// 2/8/16 Davide Sgalaberna
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <cstdlib>

#include "TND280UpVertex.hxx"

using namespace std;

ClassImp(TND280UpVertex)

TND280UpVertex::TND280UpVertex(string name){
  fVertexID = kUndefVertex;
  fReacMode = kUndefVertex;
  fReacModeString = "";
  fPosition.SetXYZ(kUndefVertex,
		   kUndefVertex,
		   kUndefVertex);
  
  fNInTracks = 0;
  fListOfInTracks = new TList;

  fNOutTracks = 0;
  fListOfOutTracks = new TList;

  SetVertexName(name);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TND280UpVertex::~TND280UpVertex(){
  //delete ...;

  fVertexID = kUndefVertex;
  fReacMode = kUndefVertex;
  fReacModeString = "";
  fPosition.SetXYZ(kUndefVertex,
                   kUndefVertex,
                   kUndefVertex);

  SetVertexName("");

  fListOfInTracks->Delete();
  delete fListOfInTracks;

  fListOfOutTracks->Delete();
  delete fListOfOutTracks;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......   

void TND280UpVertex::AddInTrack(TND280UpTrack *track)
{
  fListOfInTracks->Add(track);
  fNInTracks = fListOfInTracks->GetEntries(); // Update # of in tracks in the vertex
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

TND280UpTrack * TND280UpVertex::GetInTrack(int trkid)
{
  TObject *objtrack = fListOfInTracks->At(trkid);
  if(!objtrack){
    cerr << endl;
    cerr << "TND280UpVertex::GetInTrack" << endl;
    cerr << "The index " << trkid << " is out of range" << endl;
    cerr << endl;
    exit(1);
  }
  TND280UpTrack *track = dynamic_cast<TND280UpTrack*>(objtrack); 
  return track;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

void TND280UpVertex::AddOutTrack(TND280UpTrack *track)
{
  fListOfOutTracks->Add(track);
  fNOutTracks = fListOfOutTracks->GetEntries(); // Update # of out tracks in the vertex
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

TND280UpTrack * TND280UpVertex::GetOutTrack(int trkid)
{
  TObject *objtrack = fListOfOutTracks->At(trkid);
  if(!objtrack){
    cerr << endl;
    cerr << "TND280UpVertex::GetOutTrack" << endl;
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
  cout << "Reaction mode (string): " << fReacModeString << endl;
  cout << "# of input tracks: " << fNInTracks << endl;
  for(int itrk=0;itrk<fNInTracks;itrk++){
    cout << " - pdg: " << GetInTrack(itrk)->GetPDG() << endl;
  }
  cout << "# of output tracks: " << fNOutTracks << endl;
  for(int itrk=0;itrk<fNOutTracks;itrk++){
    cout << " - pdg: " << GetOutTrack(itrk)->GetPDG() << endl;
  }
  cout << "Time: " << fTime << endl;
  cout << "Position: (" 
       << fPosition.X() << ", "
       << fPosition.Y() << ", "
       << fPosition.Z() << ") "
       << endl;
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
