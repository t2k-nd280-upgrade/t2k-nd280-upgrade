#ifndef HLClonesArray_h
#define HLClonesArray_h

#include <TClonesArray.h>
#include <TChain.h>

class HLClonesArray{
 public :

  HLClonesArray(const std::string& tree_name, const std::string& tcarray_name, const std::string& class_name, const UInt_t size);
  virtual ~HLClonesArray();
    
  // Read the clones array of a given name from a file
  void ReadClonesArray(const std::string& file);

  // Write the clones array of a given name into a tree
  void WriteClonesArray(TTree& tree);
  
  /// Return the number of steps that have been added.
  Int_t& GetNObjects(){return _NObjects;}

  /// Return a TClonesArray 
  /// This is used when saving the objects to the output file, so we can access
  /// them in the DrawingTools.
  TClonesArray* GetClonesArray(){return _objects;}

  /// Return the name of the TClonesArray.
  const std::string& GetClonesArrayName() const{return _tcArrayName;}

  /// Return the name of the tree
  const std::string& GetTreeName() const{return _treeName;}

    
protected:

  /// TChain used to read the "config" tree from the output file.
  TChain* _chain;

  /// The number of steps that were added.
  Int_t _NObjects;

  /// A TClonesArray with the top level steps, for reading/writing to the
  /// "config" tree in the output file.
  TClonesArray* _objects;

  /// Name of the TClonesArray in the tree
  std::string _tcArrayName;

  /// Name of the Tree
  std::string _treeName;

  /// Name of the last file read
  std::string _fileName;

};

#endif
