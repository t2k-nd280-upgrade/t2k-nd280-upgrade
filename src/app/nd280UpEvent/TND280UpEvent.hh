
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

#include <iostream>

using namespace std;

class TND280UpEvent : public TObject
{
public :
  
  //using TObject::Read; // Suppress clang warnings  
  //using TObject::Draw; // Suppress clang warnings    
  TND280UpEvent();
  ~TND280UpEvent();
  
private :
  

  ClassDef(TND280UpEvent,1)
};

#endif
