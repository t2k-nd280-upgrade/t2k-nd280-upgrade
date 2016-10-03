#include "ndupAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  ndupAnalysis* ana = new ndupAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
