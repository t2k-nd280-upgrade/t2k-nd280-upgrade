#include "../../src/app/nd280UpEvent/ND280SFGDVoxelSet.hh"
#include "../../src/app/nd280UpEvent/ND280SFGDVoxel.hh"
#include "../../src/app/nd280UpEvent/ND280SFGDHit.hh"
#include "../../src/app/nd280UpEvent/ND280SFGDTrack.hh"
#include "../../src/app/nd280UpEvent/ND280SFGDEvent.hh"

#include "../../src/app/nd280UpEvent/Event.hh"
#include "../../src/app/nd280UpEvent/Hit.hh"

#include "../src/extraClasses/CHit2D.hxx"
#include "../src/extraClasses/CHit3D.hxx"
#include "../src/extraClasses/CBond3D.hxx"
#include "../src/extraClasses/CCluster3D.hxx"

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

//Unpacking Classes
#pragma link C++ class Event+;
#pragma link C++ class Hit+;

//Extra Classes
#pragma link C++ class CHit3D+;
#pragma link C++ class CHit2D+;
#pragma link C++ class vector<CHit3D>+;
#pragma link C++ class vector<CHit2D>+;

#pragma link C++ class CBond3D+;
#pragma link C++ class vector<CBond3D>+;
#pragma link C++ class CHit3D+;

#pragma link C++ class CCluster3D+;
#pragma link C++ class vector<CCluster3D>+;


#endif // __CINT__
