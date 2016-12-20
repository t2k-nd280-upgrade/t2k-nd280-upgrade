#include "numuCCAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  numuCCAnalysis* ana = new numuCCAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
