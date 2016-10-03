#include "TreeManager.hxx"
#include <TDirectory.h>
#include <TList.h>
#include <sstream>
#include <stdlib.h>
#include <algorithm>

//********************************************************************
TreeManager::TreeManager(){
//********************************************************************

  _current_tree=-1;
  _file=NULL;

  // Put all trees to NULL
  _trees.resize(NMAXTREES);
  for (UInt_t i=0;i<_trees.size();i++)
    _trees[i]= NULL;

  _trees_indices.clear();
}

//********************************************************************
TreeManager::~TreeManager() {
//********************************************************************

  if (_file) delete _file;
}

//**************************************************
TTree* TreeManager::GetTree(const std::string& name){
//**************************************************

  Int_t index = GetTreeIndex(name);
  if (index!=-1) return GetTree(index);
  else return NULL;  
}

//**************************************************
Int_t TreeManager::GetTreeIndex(const std::string& name){
//**************************************************

  for (UInt_t i=0;i<_trees_indices.size();i++){
    if (_trees[_trees_indices[i]]->GetName() == name) return _trees_indices[i];
  }

  return -1;
}

//**************************************************
std::string TreeManager::GetTreeName(Int_t index){
//**************************************************

  if (!HasTree(index)) return "";
  return _trees[index]->GetName();
}

//**************************************************
bool TreeManager::HasTree(Int_t index){
//**************************************************

  if (index<0 || index>(Int_t)_trees.size()-1) return false;
  return (_trees[index]!=NULL);
}

//**************************************************
bool TreeManager::HasTree(const std::string& name){
//**************************************************

  return (GetTreeIndex(name)!=-1);
}

//********************************************************************
void TreeManager::ReadTree(const std::string& file, Int_t index){
//********************************************************************

  // Load a tree from a file
  if (_file) delete _file;
  _file = new TFile(file.c_str(),"READ");

  std::string tree_name = GetTreeName(index);

  TTree *t = (TTree*)(_file->Get(tree_name.c_str()));
  Int_t tree_index = std::atoi(t->GetTitle());
  _trees[tree_index]= t;
  
  _trees_indices.push_back(tree_index);
  _trees_nonull.push_back(_trees[tree_index]);
  
  // Set as current tree
  SetCurrentTree(index);  
}


//********************************************************************
void TreeManager::ReadTree(const std::string& file, const std::string& tree_name){
//********************************************************************

  Int_t index = GetTreeIndex(tree_name);
  if (index!=-1) ReadTree(file,index);
}

//********************************************************************
void TreeManager::ReadFile(const std::string& file){
//********************************************************************

  for (UInt_t i=0;i<_trees.size();i++)
    _trees[i]= NULL;

  _trees_indices.clear();


  // Load all trees from a file
  _file = new TFile(file.c_str(),"READ");

  TList *keys = _file->GetListOfKeys();
  for (int i=0;i<keys->GetEntries();i++){
    std::string tree_name = keys->At(i)->GetName();
    TTree *t = (TTree*)(_file->Get(tree_name.c_str()));
    Int_t tree_index = std::atoi(t->GetTitle());
    if(_trees[tree_index]) continue;
    _trees[tree_index]= t;

    _trees_indices.push_back(tree_index);
    _trees_nonull.push_back(_trees[tree_index]);
  }

  // Set as current tree the "default"
  SetCurrentTree("default");
}


//**************************************************
std::string TreeManager::GetCurrentTreeName(){
//**************************************************

  if (GetCurrentTree()==-1) return "";
  return GetTreeName(GetCurrentTree());
}


//**************************************************
void TreeManager::SetCurrentTree(const std::string& tree_name){
//**************************************************

  SetCurrentTree(GetTreeIndex(tree_name));
}
