#ifndef WeightTools_h
#define WeightTools_h

#include "CoreDataClasses.hxx"
#include "TTree.h"


namespace weightTools{

  std::string ApplyWeightsOld(TTree* tree, const std::string& opt);
  std::string ApplyWeights(TTree* tree, const std::string& cut,const std::string& uopt);
}

#endif
