#include "CategoryManager.hxx"
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <cstdlib>

namespace anaUtils{
  CategoryManager* _categ=NULL;
}

//********************************************************************
CategoryManager::CategoryManager(): HLClonesArray("config","CATEG","TrackCategoryDefinition",NMAXCATEG){
//********************************************************************

  // is it ready ? Are the actual track properties saved ?
  _ready = false;
}


//********************************************************************
void CategoryManager::AddCategory(const std::string& categ_name, int ntypes, std::string* names, int* codes, int* colors, bool multi, bool noWarning, bool addNOTRUTH, bool addSAND){
//********************************************************************

  // Fill this track category definition

  TrackCategoryDefinition* categ;
  if (HasCategory(categ_name)){
    if ( ! noWarning)
        std::cerr << "WARNING: category '" << categ_name << "' already exists !!! Replacing properties..." << std::endl;
    categ = _track_category_map[categ_name];
    categ->Reset();
  }
  else{
    categ = new((*_objects)[_NObjects++]) TrackCategoryDefinition(multi);
    categ->_name = categ_name;
    // Add it to the map of categories
    _track_category_map[categ_name] = categ;
  }

  for (int i=0;i<ntypes;i++) {
    if ( ! noWarning && addSAND && codes[i]==CATSAND) {
      std::cerr << "ERROR in CategoryManager::AddCategory: for category '" << categ_name
                << "' user type " << i << " '" << names[i] << "' has same code " << codes[i]
                << " as the automatic SAND muon type. Please use a different code !!!" << std::endl;
    }
    if ( ! noWarning && addNOTRUTH && codes[i]==CATNOTRUTH) {
      std::cerr << "ERROR in CategoryManager::AddCategory: for category '" << categ_name
                << "' user type " << i << " '" << names[i] << "' has same code " << codes[i]
                << " as the automatic NO TRUTH type. Please use a different code !!!" << std::endl;
    }

    TrackTypeDefinition type;
    type._name  = names[i];
    type._code  = codes[i];
    type._color = colors[i];
    type._order = i;
    type._enabled=true;
    categ->AddType(type);
  }

  // add "notruth" and "sand #mu" type to all categories unless specified otherwise with the input arguments

  if (addNOTRUTH){
    // first no truth (since often sand has no truth)
    TrackTypeDefinition type;
    type._name  = NAMENOTRUTH;
    type._code  = CATNOTRUTH;
    type._color = COLNOTRUTH;
    type._order = categ->GetNTypes();
    type._enabled = true;
    categ->AddType(type);
  }
  
  if (addSAND){
    // sand as last type
    TrackTypeDefinition typeSand;
    typeSand._name  = NAMESAND;
    typeSand._code  = CATSAND;
    typeSand._color = COLSAND;
    typeSand._order = categ->GetNTypes();
    typeSand._enabled = true;
    categ->AddType(typeSand);
  }
}

//********************************************************************
void CategoryManager::CopyCategory(const std::string& categ_name, const std::string& categ_name2){
//********************************************************************

  if (!HasCategory(categ_name)){
    std::cout << "cannot copy category '" << categ_name << "' because it does not exist !!!" << std::endl;
    return;
  }

  // Get properties of the existing category
  const UInt_t NMAXTYPES=50;
  TrackCategoryDefinition* categ = _track_category_map[categ_name];
  Int_t ntypes = categ->GetNTypes();
  bool multi = categ->IsMultiType();
  std::string names[NMAXTYPES];
  Int_t codes[NMAXTYPES];
  Int_t colors[NMAXTYPES];

  for (Int_t i=0;i<ntypes;i++){
    names[i]  = categ->GetCategoryTypes()[i]._name;
    codes[i]  = categ->GetCategoryTypes()[i]._code;
    colors[i] = categ->GetCategoryTypes()[i]._color;
  }

  // Add the new category
  AddCategory(categ_name2, ntypes, names, codes, colors, multi);

}

//********************************************************************
void CategoryManager::ReadCategories(const std::string& file){
//********************************************************************

  // Reset the vectors
  _track_category_map.clear();

  ReadClonesArray(file);

  // Add them to the map of categories
  for (int i=0;i<_NObjects;i++){
    TrackCategoryDefinition* categ = (TrackCategoryDefinition*)(*_objects)[i];
    _track_category_map[categ->_name] = categ;
  }
}

//********************************************************************
bool CategoryManager::HasCategory(const std::string& categ) {
//********************************************************************

  if (_track_category_map.find(categ)==_track_category_map.end()){
    return false;
  }
  return true;
}

//********************************************************************
void CategoryManager::DumpCategories() {
//********************************************************************

  std::cout << "-------- Available track categories -------" << std::endl;
  std::map< std::string, TrackCategoryDefinition* >::iterator it;
  for (it=GetCategories().begin();it!=GetCategories().end();it++ ){
    std::string categ_name = it->first;
    std::cout << " - " << categ_name << std::endl;
  }
  std::cout << "-------------------------------------------" << std::endl;

}

//********************************************************************
void CategoryManager::DumpCategory(const std::string& categ) {
//********************************************************************

  if (!HasCategory(categ)) {
    std::cout << "Category " << categ << " hasn't been set!" << std::endl;
    return;
  }
  std::cout << " --------------------------------------------------------" << std::endl;
  std::cout << " Types for '" << categ << "' category" << std::endl;
  TrackCategoryDefinition* def = _track_category_map[categ];
  std::vector<TrackTypeDefinition>& types = def->GetCategoryTypes();

  for (unsigned int i = 0; i < types.size(); i++) {
    TrackTypeDefinition type = types[i];
    std::cout << std::setw(25) << type._name << std::setw(15) << "code " << type._code << std::setw(15) << "color " << type._color << std::endl;
  }

  std::cout << "-------------------------------------------" << std::endl;
  std::cout << std::endl;

}

//********************************************************************
void CategoryManager::ResetCurrentCategories(){
//********************************************************************

  std::map< std::string, TrackCategoryDefinition* >::iterator it;
  for (it=GetCategories().begin();it!=GetCategories().end();it++ ){
    TrackCategoryDefinition& categ = *(it->second);
    if (categ.IsMultiType()){
      for (unsigned int i=0;i<categ.GetNTypes();i++)
          categ.SetCategoryType(i,false);
    }
    else{
      categ.SetCode(-999,-999);
    }
  }

  _ready = false;
}

//********************************************************************
int CategoryManager::GetCode(const std::string& categ) {
//********************************************************************

  if (!HasCategory(categ)) return -1;

  if (!GetCategory(categ).IsMultiType())
    return GetCategory(categ).GetCode();
  else{
    std::cout << "Category '" << categ << "' is a multi-type category" << std::endl;
    return -1;
  }
}

//********************************************************************
bool CategoryManager::CheckCategoryType(const std::string& categ, int index){
//********************************************************************

  if (!HasCategory(categ)) return -1;

  if (GetCategory(categ).IsMultiType())
    return GetCategory(categ).CheckCategoryType(index);
  else{
    std::cout << "Category '" << categ << "' is a single-type category" << std::endl;
    return false;
  }
}

