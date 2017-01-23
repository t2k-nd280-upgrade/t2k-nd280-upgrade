#include "numuCC4piAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  numuCC4piAnalysis* ana = new numuCC4piAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
