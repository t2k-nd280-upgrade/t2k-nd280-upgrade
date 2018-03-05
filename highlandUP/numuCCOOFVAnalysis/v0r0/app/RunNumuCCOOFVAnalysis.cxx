#include "numuCCOOFVAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  numuCCOOFVAnalysis* ana = new numuCCOOFVAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
