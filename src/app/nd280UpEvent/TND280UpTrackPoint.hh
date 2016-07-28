
// 
// Class to define an ND280 upgrade track point (not GEANT4 based)
// 
// 27/7/16 Davide Sgalaberna
//

#ifndef TND280UpTrackPoint_h
#define TND280UpTrackPoint_h 1

#include <TTree.h>
#include <TFile.h>
#include <TBits.h>
#include <TObjString.h>
#include <vector>
#include <TObject.h>

#include <iostream>

using namespace std;

class TND280UpTrackPoint : public TObject
{
public :
     
  TND280UpTrackPoint();
  ~TND280UpTrackPoint();
  
  void SetPointID(int id){fPointID=id;};
  //void SetReacMode(string reac){fReacMode=reac;};
  
  int    GetPointID() {return fPointID;};
  //string GetReacMode(){return fReacMode;};
  
  
private :

  int fPointID;
  //string fReacMode;

  ClassDef(TND280UpTrackPoint,3)
};

#endif
