#include "tpcECalSystematics.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  tpcECalSystematics* ana = new tpcECalSystematics();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
