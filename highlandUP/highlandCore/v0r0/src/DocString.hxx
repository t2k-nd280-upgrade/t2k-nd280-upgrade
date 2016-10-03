#ifndef DocString_h
#define DocString_h

#include <cstdlib>
#include <TObject.h>

/// This class is used to store information about the variables stored in the
/// micro-trees. The DocStringManager is used to create, store and access
/// DocString objects.
///
/// Each variable stored in the micro-trees has an associated DocString object,
/// which records the name, type, size and other information about the variable.
///
/// This DocString class should not need to be accessed by users. All the
/// functionality is handled automatically.
class DocString: public TObject {
  public:

    /// Empty constructor to keep ROOT happy when reading in from TClonesArray.
    DocString(){};

    /// Constructor that actually sets the information.
    DocString(const std::string& tree, const std::string& name, const std::string& doc, const std::string& type, int size1, const std::string& counter1, int size2, const std::string& counter2, int size3, const std::string& counter3);

    /// Destructor.
    virtual ~DocString() {}

    /// Print out the details of this variable. If it is a track category variable,
    /// also print out the track categories.
    void Explain();

    /// Get the name of this variable.
    std::string Name() { return _name; }

    /// Get the name of the tree this variable was stored in.
    std::string Tree() { return _tree; }

    ClassDef(DocString, 1);

  protected:

    /// The name of the tree this variable was stored in.
    std::string _tree;

    /// The name of this variable.
    std::string _name;

    /// The documentation string provided by the user.
    std::string _doc;

    /// The type of this variable (I/F/C/D)
    std::string _type;

    /// If a vector, the counter used for the 1st dimension.
    std::string _counter1;

    /// If a matrix, the counter used for the 2nd dimension.
    std::string _counter2;

    /// If a 3D matrix, the counter used for the 3rd dimension.
    std::string _counter3;

    /// If a vector, the size of the 1st dimension.
    int _size1;

    /// If a matrix, the size of the 2nd dimension.
    int _size2;

    /// If a 3D matrix, the size of the 3rd dimension.
    int _size3;

};

#endif
