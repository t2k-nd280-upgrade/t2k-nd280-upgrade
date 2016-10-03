#include "CategoryClasses.hxx"

ClassImp(TrackCategoryDefinition)
ClassImp(TrackTypeDefinition)


//********************************************************************
std::string* TrackCategoryDefinition::GetNames(std::string names[]){
//********************************************************************

//  std::string* names = new std::string[20];
  for (UInt_t i=0;i<GetNTypes();i++){
    names[i] = _types[i]._name;
  }
  return names;
}


//********************************************************************
Int_t* TrackCategoryDefinition::GetCodes(Int_t codes[]){
//********************************************************************

//  Int_t* codes = new Int_t[20];
  for (UInt_t i=0;i<GetNTypes();i++){
    codes[i] = _types[i]._code;
  }
  return codes;
}


//********************************************************************
Int_t* TrackCategoryDefinition::GetColors(Int_t colors[]){
//********************************************************************

//  Int_t* colors = new Int_t[20];
  for (UInt_t i=0;i<GetNTypes();i++){
    colors[i] = _types[i]._color;
  }
  return colors;
}


