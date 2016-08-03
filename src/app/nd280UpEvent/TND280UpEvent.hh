
// 
// Class to define an ND280 upgrade event (not GEANT4 based)
// 
// 27/7/16 Davide Sgalaberna
//

#ifndef TND280UpEvent_h
#define TND280UpEvent_h 1

#include <TTree.h>
#include <TFile.h>
#include <TBits.h>
#include <TObjString.h>
#include <vector>
#include <TObject.h>
#include <TList.h>
#include <TCollection.h>
#include <TIterator.h>

#include <iostream>

#include "TND280UpTrack.hh"
#include "TND280UpVertex.hh"

using namespace std;

class TND280UpEvent : public TObject
{
public :
     
  TND280UpEvent();
  ~TND280UpEvent();

  void AddVertex(TND280UpVertex *vtx);
  TND280UpVertex *GetVertex(int vtxid);
  int GetNVertices(){return fNVertices;};
  
  void AddTrack(TND280UpTrack *track);
  TND280UpTrack *GetTrack(int trkid);
  int GetNTracks(){return fNTracks;};
  
  void SetEventID(int id){fEventID=id;};  
  int  GetEventID(){return fEventID;};
  
  void PrintEvent();
  
private :

  TList *fListOfTracks;
  int fNTracks;  
  TList *GetListOfTracks(){return fListOfTracks;};

  TList *fListOfVertices;
  int fNVertices;  
  TList *GetListOfVertices(){return fListOfVertices;};
  
  int fEventID;
  
  ClassDef(TND280UpEvent,1)
};

#endif
