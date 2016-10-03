#ifndef DocStringManager_h
#define DocStringManager_h

#include <iostream>
#include <algorithm>
#include "DocString.hxx"
#include <HLClonesArray.hxx>

/// This class is used to store and access documentation for the variables stored
/// in the micro-trees.
///
/// This singleton class will store a TClonesArray of DocString objects into the
/// "config" tree. When using the DrawingTools, the array will be read in, and the
/// use can then use the DrawingTools functions to interrogate the information.
///
/// When adding a variable to the micro-tree, the DocumentVar function is called
/// automatically. The user does not need to call this function themselves.
class DocStringManager: public HLClonesArray{
 public :


  DocStringManager();
  
  virtual ~DocStringManager(){}

  /// Create the DocString object for this variable, and add it to the array
  /// that will be saved. See the DocString class documentation for the
  /// meaning of the parameters passed in here.
  void DocumentVar(const std::string& tree, const std::string& name, const std::string& doc, const std::string& type, int size1, const std::string& counter1, int size2, const std::string& counter2, int size3, const std::string& counter3) {
    DocString* newdoc = new((*_objects)[_NObjects++]) DocString(tree, name, doc, type, size1, counter1, size2, counter2, size3, counter3);
    _docStrings.push_back(newdoc);
  }

  /// Read the "config" tree from the given file, to read the docstrings that were
  /// used when running the analysis. This is used so we can access the info in
  /// the DrawingTools.
  void ReadDocStrings(const std::string& file);

  /// Print the details of the specified variable in the given tree.
  void ExplainVar(const std::string& name, const std::string& tree_name);

  /// List all the variables that were stored in this tree.
  void ListVars(const std::string& tree_name);

protected:

  std::vector<DocString*> _docStrings;

};


#endif
