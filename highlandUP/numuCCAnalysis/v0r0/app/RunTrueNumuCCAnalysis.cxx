#include "truenumuCCAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  truenumuCCAnalysis* ana = new truenumuCCAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
