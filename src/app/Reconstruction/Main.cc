#include "TrackReconstruction.hh"
#include "LightYield.hh"
#include "NeutronAnalysis.hh"
#include "StoppingProton.hh"
#include "EGammaSeparation.hh"

int main(int argc,char** argv) {
  // 1) leave one or several analysis uncommented
  // 2) recompile
  // 3) run

  TrackReconstruction(argc, argv);
  EGammaSeparation(argc, argv);
  
  //LightYield(argc, argv);
  // NeutronAnalysis(argc, argv);
  //StoppingProton(argc, argv);
  
  return 1;
}
