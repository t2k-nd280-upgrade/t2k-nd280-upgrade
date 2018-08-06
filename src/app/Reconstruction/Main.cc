#include "TrackReconstruction.hh"
#include "LightYield.hh"
#include "NeutronAnalysis.hh"

int main(int argc,char** argv) {
  // 1) leave one or several analysis uncommented
  // 2) recompile
  // 3) run

  TrackReconstruction(argc, argv);
  LightYield(argc, argv);
  NeutronAnalysis(argc, argv);
  
  return 1;
}