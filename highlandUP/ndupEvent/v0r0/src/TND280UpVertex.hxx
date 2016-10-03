
// 
// Class to define an ND280 upgrade vertex (not GEANT4 based)
// 
// 2/8/16 Davide Sgalaberna
//

#ifndef TND280UpVertex_hxx
#define TND280UpVertex_hxx 1

#include <TTree.h>
#include <TFile.h>
#include <TBits.h>
#include <TObjString.h>
#include <vector>
#include <TObject.h>
#include <TVector3.h>
#include <TList.h>
#include <TCollection.h>
#include <TIterator.h>

#include <iostream>
#include <string>

#include "TND280UpTrack.hxx"

using namespace std;

const double kUndefVertex = -999999999;

class TND280UpVertex : public TObject
{
public :
     
  TND280UpVertex(string name="vertex");
  ~TND280UpVertex();

  // Tracks incoming the vertex (neutrino and nucleus)
  void AddInTrack(TND280UpTrack *track);
  TND280UpTrack *GetInTrack(int trkid);
  int GetNInTracks(){return fNInTracks;};

  // Tracks outgoing the vertex (propagated through the detector)
  void AddOutTrack(TND280UpTrack *track);
  TND280UpTrack *GetOutTrack(int trkid);
  int GetNOutTracks(){return fNOutTracks;};

  void SetVertexID(int id){fVertexID=id;};
  void SetReacMode(int num){fReacMode=num;};
  void SetReacModeString(string mode){fReacModeString=mode;};
  void SetPosition(double x,double y,double z){fPosition.SetXYZ(x,y,z);};
  void SetVertexName(string name){fVertexName=name;};
  void SetTime(double time){fTime=time;};
  
  int    GetVertexID()   {return fVertexID;};
  int    GetReacMode()   {return fReacMode;};
  string GetReacModeString() {return fReacModeString;};
  TVector3 GetPosition() {return fPosition;};
  string GetVertexName() {return fVertexName;};
  double GetTime()       {return fTime;};

  void PrintVertex();
  
private :

  TList *fListOfInTracks;
  int fNInTracks;  
  TList *GetListOfInTracks(){return fListOfInTracks;};

  TList *fListOfOutTracks;
  int fNOutTracks;  
  TList *GetListOfOutTracks(){return fListOfOutTracks;};
  
  int fVertexID;
  int fReacMode;
  string fReacModeString;
  TVector3 fPosition;
  string fVertexName;
  double fTime;

  ClassDef(TND280UpVertex,4)
};

#endif
