#include "DataSample.hxx"

#include <sstream>

//********************************************************************
DataSample::DataSample():TreeManager(),Header(){
//********************************************************************

  _norm=0;
  _cut="";
  _filePath="";
  _systGroup="default";
  _systOption="";
}

//********************************************************************
DataSample::DataSample(const std::string& file, double norm, const std::string& cut):TreeManager(),Header(){
//********************************************************************

  _cut = cut;
  _norm = norm;
  _filePath = file;
  _systGroup="default";
  _systOption="";

  // Load all POT and Spill values from the header tree in the file
  ReadHeader(file);

  // Load all trees from the file
  ReadFile(_filePath);
}
