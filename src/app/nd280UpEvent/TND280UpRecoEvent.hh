
// 
// Class to define an ND280 upgrade reconstructed event (not GEANT4 based)
// 
// 4/7/17 Davide Sgalaberna
//

#ifndef TND280UpRecoEvent_h
#define TND280UpRecoEvent_h 1

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

#include "TND280UpRecoTrack.hh"

using namespace std;

class TND280UpRecoEvent : public TObject
{
public :
     
  TND280UpRecoEvent();
  ~TND280UpRecoEvent();

  void AddTrack(TND280UpRecoTrack *track);
  TND280UpRecoTrack *GetTrack(int trkid);
  int GetNTracks(){return fNTracks;};

  void SetEventID(int id){fEventID=id;};  
  int  GetEventID(){return fEventID;};
  
  void PrintEvent();
  
private :

  TList *fListOfTracks;
  int fNTracks;  
  TList *GetListOfTracks(){return fListOfTracks;};

  int fEventID;
  
  ClassDef(TND280UpRecoEvent,1)
};

#endif
