#include "testtimeAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  testtimeAnalysis* ana = new testtimeAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
