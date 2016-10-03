#include <HLClonesArray.hxx>
#include <stdio.h>
#include <iostream>


//********************************************************************
HLClonesArray::HLClonesArray(const std::string& tree_name, const std::string& array_name, const std::string& class_name, const UInt_t size){
//********************************************************************

  // Create a TChain to save the steps into the config tree
  //  _chain = new TChain(tree_name.c_str());

  _fileName = "";
  
  _treeName = tree_name;
  
  _NObjects =0;
  // The ClonesArray to save
  _objects = new TClonesArray(class_name.c_str(), size);  

  // Name of the TClonesArray in the tree
  _tcArrayName = array_name;

  _chain = NULL;
}  

//********************************************************************
HLClonesArray::~HLClonesArray(){
//********************************************************************

//  delete _chain;
}

//********************************************************************
void HLClonesArray::ReadClonesArray(const std::string& file){
//********************************************************************

  // Nothing to do if we read the same file
  if (file == _fileName) return;

  // the current file
  _fileName = file;

  // Reset the vectors
  _objects->Clear();

  // Reset to 0, just in case the clones array is not found
  _NObjects = 0;

  // Add the file to the chain
  if (_chain) delete _chain;
  _chain = new TChain(_treeName.c_str());
  _chain->AddFile(file.c_str());

  const std::string& name = GetClonesArrayName();

  // Set the branch addresses to read the tree
  _chain->SetBranchAddress(("N"+name).c_str(),   &_NObjects);
  _chain->SetBranchAddress(name.c_str(),         &_objects);

  // Read one entry
  Long64_t centry = _chain->LoadTree(0);
  if (centry<0){
    std::cout << "HLClonesArray::ReadClonesArray(). failed in reading '" << name << "' clones array !!!" << std::endl;
  }

  Int_t nb = _chain->GetEntry(0);
  if (nb==0){
    std::cout << "HLClonesArray::ReadClonesArray(). failed in reading '" << name << "' clones array !!!" << std::endl;
  }

  delete _chain; _chain=NULL;
}


//********************************************************************
void HLClonesArray::WriteClonesArray(TTree& tree){
//********************************************************************

  const std::string& name = GetClonesArrayName();

  tree.Branch(("N"+name).c_str(), &(GetNObjects()), ("N"+name+"/I").c_str(), 32000);
  tree.Branch(name.c_str(),"TClonesArray", GetClonesArray(), 32000, 99);
}
