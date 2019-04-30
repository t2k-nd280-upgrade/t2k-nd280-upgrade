#include "../../src/app/nd280UpEvent/ND280SFGDVoxelSet.hh"
#include "../../src/app/nd280UpEvent/ND280SFGDVoxel.hh"
#include "../../src/app/nd280UpEvent/ND280SFGDHit.hh"
#include "../../src/app/nd280UpEvent/ND280SFGDTrack.hh"
#include "../../src/app/nd280UpEvent/ND280SFGDEvent.hh"

#include <vector>

#ifdef __MAKECINT__

#pragma link C++ class vector<int>;
#pragma link C++ class vector<vector<int> > ;
#pragma link C++ class vector<short>;
#pragma link C++ class vector<vector<short> > ;
#pragma link C++ class vector<vector<double> >;

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ nestedclasses;

// Reconstruction Classes
#pragma link C++ class ND280SFGDVoxel+;
#pragma link C++ class ND280SFGDVoxelSet+;
#pragma link C++ class ND280SFGDHit++;
#pragma link C++ class ND280SFGDEvent+;
#pragma link C++ class ND280SFGDTrack+;

#endif // __CINT__
