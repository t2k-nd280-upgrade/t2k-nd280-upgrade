#include "DocStringManager.hxx"


//********************************************************************
DocStringManager::DocStringManager(): HLClonesArray("config","DocString","DocString",1000){
//********************************************************************
  
}

//********************************************************************
void DocStringManager::ReadDocStrings(const std::string& file){
//********************************************************************

  ReadClonesArray(file);
}

//********************************************************************
void DocStringManager::ExplainVar(const std::string& name, const std::string& tree_name) {
//********************************************************************
  for (int i = 0; i < _NObjects; i++) {
    DocString* doc = (DocString*) _objects->At(i);
    if (!doc) {
      std::cout << "Unable to read docstring " << i << std::endl;
      continue;
    }
    if (doc->Name() == name && doc->Tree() == tree_name) {
      doc->Explain();
      return;
    }
  }

  std::cout << "Unable to find requested variable" << std::endl;
}

//********************************************************************
void DocStringManager::ListVars(const std::string& tree_name) {
//********************************************************************
  std::vector<std::string> vars;

  for (int i = 0; i < _NObjects; i++) {
    DocString* doc = (DocString*) _objects->At(i);
    if (!doc) {
      std::cout << "Unable to read docstring " << i << std::endl;
      continue;
    }
    if (doc->Tree() == tree_name) {
      vars.push_back(doc->Name());
    }
  }

  std::sort(vars.begin(), vars.end());

  for (unsigned int i = 0; i < vars.size(); i++) {
    std::cout << vars[i] << std::endl;
  }
}
