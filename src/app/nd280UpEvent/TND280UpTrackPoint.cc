
// 
// Class to define an ND280 upgrade track point
// 
// 27/7/16 Davide Sgalaberna
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "TND280UpTrackPoint.hh"

using namespace std;

ClassImp(TND280UpTrackPoint)

TND280UpTrackPoint::TND280UpTrackPoint(){
  fPointID = -999;
}

TND280UpTrackPoint::~TND280UpTrackPoint(){
  //delete ...;
}

