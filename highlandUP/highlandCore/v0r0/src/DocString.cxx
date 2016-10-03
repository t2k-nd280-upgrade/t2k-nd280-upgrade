#include "DocString.hxx"
#include <iostream>

ClassImp(DocString)


DocString::DocString(const std::string& tree,
                     const std::string& name,
                     const std::string& doc,
                     const std::string& type,
                     int size1,
                     const std::string& counter1,
                     int size2,
                     const std::string& counter2,
                     int size3,
                     const std::string& counter3) {
  _tree  = tree;
  _name  = name;
  _doc   = doc;
  _type  = type;
  _counter1 = counter1;
  _counter2 = counter2;
  _counter3 = counter3;
  _size1 = size1;
  _size2 = size2;
  _size3 = size3;
}

void DocString::Explain() {
  std::string dim = "Unknown";
  std::string nicetype = "Unknown";

  if (_size3 != 0) {
    dim = "3D matrix of ";
  } else if (_size2 != 0) {
    dim = "Matrix of ";
  } else if (_size1 != 0) {
    dim = "Vector of ";
  } else {
    dim = "";
  }

  if (_type == "F") {
    nicetype = "float";
  } else if (_type == "D") {
    nicetype = "double";
  } else if (_type == "I") {
    nicetype = "integer";
  } else if (_type == "C") {
    nicetype = "char";
  }

  std::cout << "Variable name..... " << _name << std::endl;
  std::cout << "Stored in tree.... " << _tree << std::endl;
  std::cout << "Type of variable.. " << dim << nicetype << std::endl;
  if (_size1 != 0) {
    std::cout << "Size of variable: " << std::endl;
    std::cout << "  1st dimension... ";
    if (_size1>0) {
      std::cout << abs(_size1) << " (fixed size)" << std::endl;
    } else {
      std::cout << _counter1 << " (variable size, up to a maximum of " << abs(_size1) << ")" << std::endl;
    }
    if (_size2 != 0) {
      std::cout << "  2nd dimension... ";
      if (_size2>0) {
        std::cout << abs(_size2) << " (fixed size)" << std::endl;
      } else {
        std::cout << _counter2 << " (variable size, up to a maximum of " << abs(_size2) << ")" << std::endl;
      }
      if (_size3 != 0) {
        std::cout << "  3rd dimension... ";
        if (_size3>0) {
          std::cout << abs(_size3) << " (fixed size)" << std::endl;
        } else {
          std::cout << _counter3 << " (variable size, up to a maximum of " << abs(_size3) << ")" << std::endl;
        }
      }
    }
  }
  std::cout << "Documentation..... " << _doc << std::endl;

  //  if (ND::categ().HasCategory(_name)) {
  //    ND::categ().DumpTrackCategory(_name);
  //  }
}
