#include "numuCCMultiPiAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  numuCCMultiPiAnalysis* ana = new numuCCMultiPiAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
