#include "nueCCAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  nueCCAnalysis* ana = new nueCCAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
