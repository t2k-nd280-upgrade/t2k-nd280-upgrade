
// 
// Class to define an ND280 upgrade event
// 
// 27/7/16 Davide Sgalaberna
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

#include "TND280UpEvent.hh"

using namespace std;

ClassImp(TND280UpEvent)

TND280UpEvent::TND280UpEvent(){
  fEventID = -9999999;
  //fReacMode = "";
  
  fNTracks = 0;
  fListOfTracks = new TList;

  fNVertices = 0;
  fListOfVertices = new TList;

  fNHits = 0;
  fListOfHits = new TList;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TND280UpEvent::~TND280UpEvent(){
  //delete ...;

  fEventID = -9999999;

  //fListOfTracks->SetOwner(true);
  fListOfTracks->Delete();
  delete fListOfTracks;
  //fListOfTracks = 0;

  //fListOfVertices->SetOwner(true);
  fListOfVertices->Delete();
  delete fListOfVertices;
  //fListOfVertices = 0;

  //fListOfHits->SetOwner(true);
  fListOfHits->Delete();
  delete fListOfHits;
  //fListOfHits = 0;
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


void TND280UpEvent::AddVertex(TND280UpVertex *vtx)
{
  fListOfVertices->Add(vtx);
  fNVertices = fListOfVertices->GetEntries(); // Update # of vertices in the event
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

TND280UpVertex * TND280UpEvent::GetVertex(int vtxid)
{
  TObject *objvtx = fListOfVertices->At(vtxid);
  if(!objvtx){
    cerr << endl;
    cerr << "TND280UpEvent::GetVertex" << endl;
    cerr << "The index " << vtxid << " is out of range" << endl;
    cerr << endl;
    exit(1);
  }
  TND280UpVertex *vtx = dynamic_cast<TND280UpVertex*>(objvtx); 
  return vtx;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TND280UpEvent::AddHit(TND280UpHit *hit)
{
  fListOfHits->Add(hit);
  fNHits = fListOfHits->GetEntries(); // Update # of hits in the event
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

TND280UpHit * TND280UpEvent::GetHit(int hitid)
{
  TObject *objhit = fListOfHits->At(hitid);
  if(!objhit){
    cerr << endl;
    cerr << "TND280UpEvent::GetHit" << endl;
    cerr << "The index " << hitid << " is out of range" << endl;
    cerr << endl;
    exit(1);
  }
  TND280UpHit *hit = dynamic_cast<TND280UpHit*>(objhit); 
  return hit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void TND280UpEvent::PrintEvent()
{
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
  cout << "Event ID: " << fEventID << endl;
  cout << "# of tracks: " << fNTracks << endl;
  cout << "# of vertices: " << fNVertices << endl;
  for(int ivtx=0;ivtx<fNVertices;ivtx++){
    cout << " - " << GetVertex(ivtx)->GetVertexName() << endl;
  }
  cout << "# of hits: " << fNHits << endl;
  cout << endl;
  cout << "oooooooooooooooooooooooooooooooo" << endl;
  cout << endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
