#include "truendupAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  truendupAnalysis* ana = new truendupAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
