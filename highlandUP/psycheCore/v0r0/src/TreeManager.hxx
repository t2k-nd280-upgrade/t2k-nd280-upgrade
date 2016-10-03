#ifndef TreeManager_h
#define TreeManager_h

#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

/// This class handles several trees, one of which is selected

const UInt_t NMAXTREES = 50;

class TreeManager{
public :

  TreeManager();
  virtual ~TreeManager();

  //--------- Tree operations ------------

  /// Returns the map of trees
  std::vector< TTree* >& GetTrees(){return _trees_nonull;}

  /// Returns the a tree with a given index
  TTree* GetTree(Int_t index){return _trees[index];}

  /// Returns the a tree with a given name
  TTree* GetTree(const std::string& name);

  /// Returns the a tree set as current
  TTree* GetTree(){return _trees[GetCurrentTree()];}

  /// Read a tree from a file provided the index
  void ReadTree(const std::string& file, Int_t index);

  /// Read a tree from a file provided the name
  void ReadTree(const std::string& file, const std::string& name);

  /// Read all trees from a file
  void ReadFile(const std::string& file);

  /// Retuns the tree index provided the name
  Int_t GetTreeIndex(const std::string& tree_name);

  /// Retuns the tree name provided the index
  std::string GetTreeName(Int_t index);

  /// Retuns the current tree index
  Int_t GetCurrentTree() const {return _current_tree;}

  /// Retuns the current tree name
  std::string GetCurrentTreeName();

  /// Sets the current tree provided the index
  void SetCurrentTree(Int_t index){_current_tree=index;}

  /// Sets the current tree provided the name
  void SetCurrentTree(const std::string& tree_name);

  /// Check the existence of a tree provided the index
  bool HasTree(Int_t index);

  /// Check the existence of a tree provided the name
  bool HasTree(const std::string& tree_name);

 protected:

  /// Root input or output file 
  TFile *_file;

  /// Vector of trees
  std::vector< TTree* > _trees;

  /// Vector of non NULL trees
  std::vector< TTree* > _trees_nonull;

  /// Vector of indices for the non NULL TTree in previous vector
  std::vector< Int_t > _trees_indices;

  /// The current tree
  Int_t _current_tree;
};

#endif


