
// 
// Class to define an ND280 upgrade vertex (not GEANT4 based)
// 
// 2/8/16 Davide Sgalaberna
//

#ifndef TND280UpVertex_h
#define TND280UpVertex_h 1

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

using namespace std;

class TND280UpVertex : public TObject
{
public :
     
  TND280UpVertex();
  ~TND280UpVertex();

  void AddTrack(TND280UpTrack *track);
  TND280UpTrack *GetTrack(int trkid);
  int GetNTracks(){return fNTracks;};

  void SetVertexID(int id){fVertexID=id;};
  void SetReacMode(string reac){fReacMode=reac;};
  
  int    GetVertexID()     {return fVertexID;};
  string GetReacMode()    {return fReacMode;};
  
  void PrintVertex();
  
private :

  TList *fListOfTracks;
  int fNTracks;  
  TList *GetListOfTracks(){return fListOfTracks;};

  int fVertexID;
  string fReacMode;
  
  ClassDef(TND280UpVertex,4)
};

#endif
