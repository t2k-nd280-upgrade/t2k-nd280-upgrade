#include "OutputManager.hxx"
#include <TDirectory.h>
#include <TList.h>
#include <sstream>
#include <stdlib.h>
#include <algorithm>

//********************************************************************
OutputManager::OutputManager():TreeManager(){
//********************************************************************

  _default_docstring="No documentation was specified for this variable";
  _toy_index=0; 
  _RooVtxEntry=0;

  _tree_vars_counter_size.resize(NMAXTREES);
  _link_var_to_counter.resize(NMAXTREES);


  _tree_vars_counter.resize(NMAXTREES);

  _tree_vars_float.resize(NMAXTREES);
  _tree_vars_double.resize(NMAXTREES);
  _tree_vars_int.resize(NMAXTREES);
  _tree_vars_char.resize(NMAXTREES);

  _tree_vars_float_vector.resize(NMAXTREES);
  _tree_vars_double_vector.resize(NMAXTREES);
  _tree_vars_int_vector.resize(NMAXTREES);
  _tree_vars_char_vector.resize(NMAXTREES);

  _tree_vars_float_matrix.resize(NMAXTREES);
  _tree_vars_double_matrix.resize(NMAXTREES);
  _tree_vars_int_matrix.resize(NMAXTREES);
  _tree_vars_char_matrix.resize(NMAXTREES);

  _tree_vars_float_3Dmatrix.resize(NMAXTREES);
  _tree_vars_double_3Dmatrix.resize(NMAXTREES);
  _tree_vars_int_3Dmatrix.resize(NMAXTREES);

  _tree_vars_all_vars.resize(NMAXTREES);
  _tree_vars_all_counters.resize(NMAXTREES);


  _tree_vars_used_counter.resize(NMAXTREES);

  _tree_vars_used_float.resize(NMAXTREES);
  _tree_vars_used_double.resize(NMAXTREES);
  _tree_vars_used_int.resize(NMAXTREES);
  _tree_vars_used_char.resize(NMAXTREES);

  _tree_vars_used_float_vector.resize(NMAXTREES);
  _tree_vars_used_double_vector.resize(NMAXTREES);
  _tree_vars_used_int_vector.resize(NMAXTREES);
  _tree_vars_used_char_vector.resize(NMAXTREES);

  _tree_vars_used_float_matrix.resize(NMAXTREES);
  _tree_vars_used_double_matrix.resize(NMAXTREES);
  _tree_vars_used_int_matrix.resize(NMAXTREES);
  _tree_vars_used_char_matrix.resize(NMAXTREES);

  _tree_vars_used_float_3Dmatrix.resize(NMAXTREES);
  _tree_vars_used_double_3Dmatrix.resize(NMAXTREES);
  _tree_vars_used_int_3Dmatrix.resize(NMAXTREES);


  _single_tree_fill = -1;

}

//********************************************************************
OutputManager::~OutputManager() {
//********************************************************************
  UInt_t tree_index = 0;
  for (std::vector<TTree*>::iterator treeit = _trees.begin(); treeit != _trees.end(); treeit++, tree_index++) {

    std::vector<int_vector*>::iterator it_int_vector;
    for (it_int_vector = _tree_vars_int_vector[tree_index].begin(); it_int_vector != _tree_vars_int_vector[tree_index].end(); it_int_vector++)
      { delete *it_int_vector; }

    std::vector<double_vector*>::iterator it_double_vector;
    for (it_double_vector = _tree_vars_double_vector[tree_index].begin(); it_double_vector != _tree_vars_double_vector[tree_index].end(); it_double_vector++)
      { delete *it_double_vector; }

    std::vector<float_vector*>::iterator it_float_vector;
    for (it_float_vector = _tree_vars_float_vector[tree_index].begin(); it_float_vector != _tree_vars_float_vector[tree_index].end(); it_float_vector++)
      { delete *it_float_vector; }

    std::vector<char_vector*>::iterator it_char_vector;
    for (it_char_vector = _tree_vars_char_vector[tree_index].begin(); it_char_vector != _tree_vars_char_vector[tree_index].end(); it_char_vector++)
      { delete *it_char_vector; }


    std::vector<int_matrix*>::iterator it_int_matrix;
    for (it_int_matrix = _tree_vars_int_matrix[tree_index].begin(); it_int_matrix != _tree_vars_int_matrix[tree_index].end(); it_int_matrix++)
      { delete *it_int_matrix; }

    std::vector<double_matrix*>::iterator it_double_matrix;
    for (it_double_matrix = _tree_vars_double_matrix[tree_index].begin(); it_double_matrix != _tree_vars_double_matrix[tree_index].end(); it_double_matrix++)
      { delete *it_double_matrix; }

    std::vector<float_matrix*>::iterator it_float_matrix;
    for (it_float_matrix = _tree_vars_float_matrix[tree_index].begin(); it_float_matrix != _tree_vars_float_matrix[tree_index].end(); it_float_matrix++)
      { delete *it_float_matrix; }

    std::vector<char_matrix*>::iterator it_char_matrix;
    for (it_char_matrix = _tree_vars_char_matrix[tree_index].begin(); it_char_matrix != _tree_vars_char_matrix[tree_index].end(); it_char_matrix++)
      { delete *it_char_matrix; }


    std::vector<int_3Dmatrix*>::iterator it_int_3Dmatrix;
    for (it_int_3Dmatrix = _tree_vars_int_3Dmatrix[tree_index].begin(); it_int_3Dmatrix != _tree_vars_int_3Dmatrix[tree_index].end(); it_int_3Dmatrix++)
      { delete *it_int_3Dmatrix; }

    std::vector<double_3Dmatrix*>::iterator it_double_3Dmatrix;
    for (it_double_3Dmatrix = _tree_vars_double_3Dmatrix[tree_index].begin(); it_double_3Dmatrix != _tree_vars_double_3Dmatrix[tree_index].end(); it_double_3Dmatrix++)
      { delete *it_double_3Dmatrix; }

    std::vector<float_3Dmatrix*>::iterator it_float_3Dmatrix;
    for (it_float_3Dmatrix = _tree_vars_float_3Dmatrix[tree_index].begin(); it_float_3Dmatrix != _tree_vars_float_3Dmatrix[tree_index].end(); it_float_3Dmatrix++)
      { delete *it_float_3Dmatrix; }

   }
}

//**************************************************
void OutputManager::AddTreeWithName(Int_t tree_index, const std::string& name, TTree* tree){
//**************************************************

  // add the TTree with this name
  if (_file)
    _file->cd();
  
  if (tree){
    _trees[tree_index] = tree->CloneTree(0);
    _trees[tree_index]->SetTitle(GetString(tree_index).c_str());
  }
  else
    _trees[tree_index] = new TTree(name.c_str(),GetString(tree_index).c_str());

  _trees_indices.push_back(tree_index);
  _trees_nonull.push_back(_trees[tree_index]);

  if (!_file)
    _trees[tree_index]->SetDirectory(0);
  _trees[tree_index]->SetAutoSave(10000000);


  _tree_vars_counter_size[tree_index].resize(NMAXTREEVARS);
  _link_var_to_counter[tree_index].resize(NMAXTREEVARS);

  _tree_vars_counter[tree_index].resize(NMAXTREEVARS);

  _tree_vars_float[tree_index].resize(NMAXTREEVARS);
  _tree_vars_double[tree_index].resize(NMAXTREEVARS);
  _tree_vars_int[tree_index].resize(NMAXTREEVARS);
  _tree_vars_char[tree_index].resize(NMAXTREEVARS);

  _tree_vars_float_vector[tree_index].resize(NMAXTREEVARS);
  _tree_vars_double_vector[tree_index].resize(NMAXTREEVARS);
  _tree_vars_int_vector[tree_index].resize(NMAXTREEVARS);
  _tree_vars_char_vector[tree_index].resize(NMAXTREEVARS);

  _tree_vars_float_matrix[tree_index].resize(NMAXTREEVARS);
  _tree_vars_double_matrix[tree_index].resize(NMAXTREEVARS);
  _tree_vars_int_matrix[tree_index].resize(NMAXTREEVARS);
  _tree_vars_char_matrix[tree_index].resize(NMAXTREEVARS);

  _tree_vars_float_3Dmatrix[tree_index].resize(NMAXTREEVARS);
  _tree_vars_double_3Dmatrix[tree_index].resize(NMAXTREEVARS);
  _tree_vars_int_3Dmatrix[tree_index].resize(NMAXTREEVARS);


  for (UInt_t i=0;i<NMAXTREEVARS;i++){ 
    _link_var_to_counter[tree_index][i]=-1;
    _tree_vars_counter_size[tree_index][i]=-1;
  }

  _tree_vars_all_vars[tree_index].resize(NMAXTREEVARS);
  _tree_vars_all_counters[tree_index].resize(NMAXTREEVARS);


  for (UInt_t i=0;i<NMAXTREEVARS;i++){
    _tree_vars_all_vars[tree_index][i]="";
  }

  SetCurrentTree(tree_index);
}


//**************************************************
bool OutputManager::IsSpecialTree(Int_t tree_index) {
//**************************************************

  return (tree_index == header || tree_index == config || tree_index == truth || tree_index == NRooTrackerVtx || tree_index == GRooTrackerVtx || tree_index == RooTrackerVtx);
}


//********************************************************************
bool OutputManager::Initialize(){
//********************************************************************

  // this function is called before the event loop

  return true;
}

//********************************************************************
bool OutputManager::InitializeEntry(){
//********************************************************************

  _RooVtxEntrySaved=false;
  _saveRooVtxEntry=false;

  // this function is called at the beginning of each event
  /*
  std::map< std::string, std::map<std::string, int> >::iterator cit;
  std::map<std::string, int>::iterator cit2;
  for (cit= _tree_vars_int_var.begin();cit!=_tree_vars_int_var.end();cit++){    
    std::string name = cit->first;        
    for (cit2= _tree_vars_int_var[name].begin();cit2!=_tree_vars_int_var[name].end();cit2++){    
      cit2->second = 0;
    }
  }
  */
#if 0 
  /*

  std::map< std::string, std::map<std::string, std::vector<Int_t>* > >::iterator  vit;
  std::map<std::string, std::vector<Int_t>* >::iterator vit2;
  std::vector<Int_t>::iterator vit3;
  for (vit= _tree_vars_int_vector_var.begin();vit!=_tree_vars_int_vector_var.end();vit++){    
    std::string name = vit->first;        
    for (vit2= _tree_vars_int_vector_var[name].begin();vit2!=_tree_vars_int_vector_var[name].end();vit2++){    
      std::string var = vit2->first;        
      _tree_vars_int_vector_var[name][var]->clear();
     
    }
  }
  */
  /*

  std::map< std::string, std::map<std::string, std::vector<Double_t>* > >::iterator  dvit;
  std::map<std::string, std::vector<Double_t>* >::iterator dvit2;
  std::vector<Double_t>::iterator dvit3;
  for (dvit= _tree_vars_double_vector_var.begin();dvit!=_tree_vars_double_vector_var.end();dvit++){    
    std::string name = dvit->first;      
    for (dvit2= _tree_vars_double_vector_var[name].begin();dvit2!=_tree_vars_double_vector_var[name].end();dvit2++){    
      std::string var = dvit2->first;
      _tree_vars_double_vector_var[name][var]->clear();
    }
  }
  */
#endif 


  return true;
}

//********************************************************************
void OutputManager::InitializeTrees(bool iniVars){
//********************************************************************

  for (UInt_t i= 0; i<_trees_indices.size();i++){    
    InitializeTree(_trees_indices[i], iniVars);    
  }
}

//********************************************************************
void OutputManager::InitializeTree(Int_t tree_index, bool iniVars){
//********************************************************************

  // this function is called at the beginning of each tree

  _toyWeights.clear();

  std::vector<Int_t>::iterator it;
  for (it= _tree_vars_used_counter[tree_index].begin();it!=_tree_vars_used_counter[tree_index].end();it++){    
    // initalize to 0 the counters with no defined size
    InitializeCounter(tree_index,*it);
  }


  if (!iniVars) return;

  // this function is called at the beginning of each tree
  for (it= _tree_vars_used_int[tree_index].begin();it!=_tree_vars_used_int[tree_index].end();it++)
    _tree_vars_int[tree_index][*it]=-999;

  for (it= _tree_vars_used_double[tree_index].begin();it!=_tree_vars_used_double[tree_index].end();it++)    
    _tree_vars_double[tree_index][*it] = -999;

  for (it= _tree_vars_used_float[tree_index].begin();it!=_tree_vars_used_float[tree_index].end();it++)    
    _tree_vars_float[tree_index][*it] = -999;


  for (it= _tree_vars_used_int_vector[tree_index].begin();it!=_tree_vars_used_int_vector[tree_index].end();it++)    
    _tree_vars_int_vector[tree_index][*it]->Ini(-999);

  for (it= _tree_vars_used_float_vector[tree_index].begin();it!=_tree_vars_used_float_vector[tree_index].end();it++)    
    _tree_vars_float_vector[tree_index][*it]->Ini(-999);

  for (it= _tree_vars_used_double_vector[tree_index].begin();it!=_tree_vars_used_double_vector[tree_index].end();it++)    
    _tree_vars_double_vector[tree_index][*it]->Ini(-999);


  for (it= _tree_vars_used_int_matrix[tree_index].begin();it!=_tree_vars_used_int_matrix[tree_index].end();it++)    
    _tree_vars_int_matrix[tree_index][*it]->Ini(-999);

  for (it= _tree_vars_used_float_matrix[tree_index].begin();it!=_tree_vars_used_float_matrix[tree_index].end();it++)    
    _tree_vars_float_matrix[tree_index][*it]->Ini(-999);

  for (it= _tree_vars_used_double_matrix[tree_index].begin();it!=_tree_vars_used_double_matrix[tree_index].end();it++)    
    _tree_vars_double_matrix[tree_index][*it]->Ini(-999);


  for (it= _tree_vars_used_int_3Dmatrix[tree_index].begin();it!=_tree_vars_used_int_3Dmatrix[tree_index].end();it++)    
    _tree_vars_int_3Dmatrix[tree_index][*it]->Ini(-999);

  for (it= _tree_vars_used_float_3Dmatrix[tree_index].begin();it!=_tree_vars_used_float_3Dmatrix[tree_index].end();it++)    
    _tree_vars_float_3Dmatrix[tree_index][*it]->Ini(-999);

  for (it= _tree_vars_used_double_3Dmatrix[tree_index].begin();it!=_tree_vars_used_double_3Dmatrix[tree_index].end();it++)    
    _tree_vars_double_3Dmatrix[tree_index][*it]->Ini(-999);

}

//********************************************************************
void OutputManager::Finalize(){
//********************************************************************

  // this function is called at the end of each event

}

//********************************************************************
bool OutputManager::HasCounter(Int_t tree_index, Int_t counter_index){
//********************************************************************

  // check that counter exists
  return (_tree_vars_counter_size[tree_index][counter_index]!=-1);
}

//********************************************************************
bool OutputManager::CheckCounterType(Int_t counter_index, Int_t indx, Int_t var_index){
//********************************************************************

  // check whether the counter has a fix size or not
  bool correct=true;
  if (indx==-1){ 
    if(_tree_vars_counter_size[GetCurrentTree()][counter_index] > 0){
      std::cout << "Counter '" << GetCounterName(counter_index) << "' has a fix size = " 
		<<  _tree_vars_counter_size[GetCurrentTree()][counter_index]
		<< ". Index must be specified !!!" << std::endl;
      correct=false;
    }
  }
  else{
    if(_tree_vars_counter_size[GetCurrentTree()][counter_index] == 0){
      std::cout << "Counter '" << GetCounterName(counter_index) << "' does not have a fix size. Index cannot be specified !!!" << std::endl;
      correct=false;
    }
    if (indx > _tree_vars_counter_size[GetCurrentTree()][counter_index]){
      std::cout << "Requested index " << indx <<  " for variable '" << GetVarName(var_index) 
		<< "' bigger than fix counter size " << _tree_vars_counter_size[GetCurrentTree()][counter_index] << " !!!" << std::endl;
      correct =false;
    }    
  }

  return correct;
}



//********************************************************************
//void OutputManager::AddCounter(const std::string& tree_name, const std::string& var_name, const std::string& counter_name, int size){
//********************************************************************
/*

  if (counter_name!=""){
    // Add an integer variable for the counter and initialize it to 0
    AddCounter(tree_name,counter_name,0);
    
    // Associate the counter to the variable
    _tree_vars_vector_counter[tree_name][var_name]=counter_name;  
    
    // Give a fix value to the counter if requested
    _tree_vars_counter_size[tree_name][counter_name]=0;        
    if (size>0)
      _tree_vars_counter_size[tree_name][counter_name]=size;         
  }
  else{
    // Associate the counter to the variable
    _tree_vars_vector_counter[tree_name][var_name]=var_name;  
    
    // Give a fix value to the counter if requested
    _tree_vars_counter_size[tree_name][var_name]=0;        
    if (size>0)
      _tree_vars_counter_size[tree_name][var_name]=size;         
  }
}
*/
//********************************************************************
void OutputManager::ResizeCounter(Int_t counter_index, Int_t size){
//********************************************************************

  for (UInt_t i= 0; i<_trees_indices.size();i++){    
    ResizeCounter(_trees_indices[i],counter_index,size);
  }
}

//********************************************************************
void OutputManager::ResizeCounter(Int_t tree_index, Int_t counter_index, Int_t size){
//********************************************************************

  // check that the variable exists
  if(!HasCounter(tree_index, counter_index)){
    std::cout << "Counter '" << counter_index << "' does not exist !!!" << std::endl;
    return;
  }

  // Change the size of the counter
  _tree_vars_counter_size[tree_index][counter_index]=size;

  _tree_vars_counter[tree_index][counter_index]=size;	

  // Initialize all variables using this counter
  InitializeCounter(tree_index, counter_index);
}

//********************************************************************
void OutputManager::DeleteVar(Int_t tree_index, Int_t index){
//********************************************************************

  (void)tree_index;
  (void)index;

/*
  if (_tree_vars_int_var[tree_name].find(name) != _tree_vars_int_var[tree_name].end()){
    _tree_vars_int_var[tree_name].erase(_tree_vars_int_var[tree_name].find(name));
  }
  else if (_tree_vars_double_var[tree_name].find(name) != _tree_vars_double_var[tree_name].end()){
    _tree_vars_double_var[tree_name].erase(_tree_vars_double_var[tree_name].find(name));
  }
  else if (_tree_vars_float_var[tree_name].find(name) != _tree_vars_float_var[tree_name].end()){
    _tree_vars_float_var[tree_name].erase(_tree_vars_float_var[tree_name].find(name));
  }
  else if (_tree_vars_char_var[tree_name].find(name) != _tree_vars_char_var[tree_name].end()){
    _tree_vars_char_var[tree_name].erase(_tree_vars_char_var[tree_name].find(name));
  }
  else if (_tree_vars_int_vector_var[tree_name].find(name) != _tree_vars_int_vector_var[tree_name].end()){
    delete _tree_vars_int_vector_var[tree_name][name];
    _tree_vars_int_vector_var[tree_name].erase(_tree_vars_int_vector_var[tree_name].find(name));
  }
  else if (_tree_vars_double_vector_var[tree_name].find(name) != _tree_vars_double_vector_var[tree_name].end()){
    delete _tree_vars_double_vector_var[tree_name][name];
    _tree_vars_double_vector_var[tree_name].erase(_tree_vars_double_vector_var[tree_name].find(name));
  }
  else if (_tree_vars_float_vector_var[tree_name].find(name) != _tree_vars_float_vector_var[tree_name].end()){
    delete _tree_vars_float_vector_var[tree_name][name];
    _tree_vars_float_vector_var[tree_name].erase(_tree_vars_float_vector_var[tree_name].find(name));
  }
  else if (_tree_vars_char_vector_var[tree_name].find(name) != _tree_vars_char_vector_var[tree_name].end()){
    delete _tree_vars_char_vector_var[tree_name][name];
    _tree_vars_char_vector_var[tree_name].erase(_tree_vars_char_vector_var[tree_name].find(name));
  }
  else if (_tree_vars_int_matrix_var[tree_name].find(name) != _tree_vars_int_matrix_var[tree_name].end()){
    delete _tree_vars_int_matrix_var[tree_name][name];
    _tree_vars_int_matrix_var[tree_name].erase(_tree_vars_int_matrix_var[tree_name].find(name));
  }
  else if (_tree_vars_double_matrix_var[tree_name].find(name) != _tree_vars_double_matrix_var[tree_name].end()){
    delete _tree_vars_double_matrix_var[tree_name][name];
    _tree_vars_double_matrix_var[tree_name].erase(_tree_vars_double_matrix_var[tree_name].find(name));
  }
  else if (_tree_vars_float_matrix_var[tree_name].find(name) != _tree_vars_float_matrix_var[tree_name].end()){
    delete _tree_vars_float_matrix_var[tree_name][name];
    _tree_vars_float_matrix_var[tree_name].erase(_tree_vars_float_matrix_var[tree_name].find(name));
  }
  else if (_tree_vars_char_matrix_var[tree_name].find(name) != _tree_vars_char_matrix_var[tree_name].end()){
    delete _tree_vars_char_matrix_var[tree_name][name];
    _tree_vars_char_matrix_var[tree_name].erase(_tree_vars_char_matrix_var[tree_name].find(name));
  }
  else if (_tree_vars_int_3Dmatrix_var[tree_name].find(name) != _tree_vars_int_3Dmatrix_var[tree_name].end()){
    delete _tree_vars_int_3Dmatrix_var[tree_name][name];
    _tree_vars_int_3Dmatrix_var[tree_name].erase(_tree_vars_int_3Dmatrix_var[tree_name].find(name));
  }
  else if (_tree_vars_double_3Dmatrix_var[tree_name].find(name) != _tree_vars_double_3Dmatrix_var[tree_name].end()){
    delete _tree_vars_double_3Dmatrix_var[tree_name][name];
    _tree_vars_double_3Dmatrix_var[tree_name].erase(_tree_vars_double_3Dmatrix_var[tree_name].find(name));
  }
  else if (_tree_vars_float_3Dmatrix_var[tree_name].find(name) != _tree_vars_float_3Dmatrix_var[tree_name].end()){
    delete _tree_vars_float_3Dmatrix_var[tree_name][name];
    _tree_vars_float_3Dmatrix_var[tree_name].erase(_tree_vars_float_3Dmatrix_var[tree_name].find(name));
  }

*/
  /*
  if (std::find(_tree_vars_all_vars[tree_name].begin(), _tree_vars_all_vars[tree_name].end(), name) != _tree_vars_all_vars[tree_name].end()) {
    _tree_vars_all_vars[tree_name].erase(std::find(_tree_vars_all_vars[tree_name].begin(), _tree_vars_all_vars[tree_name].end(), name));
  }
  */
}


//********************************************************************
bool OutputManager::ValidateVarNameAndIndex(Int_t tree_index, Int_t var_index, const std::string& var_name) {
//********************************************************************

  if (std::find(_tree_vars_all_vars[tree_index].begin(), _tree_vars_all_vars[tree_index].end(), var_name) != _tree_vars_all_vars[tree_index].end()) {
    std::cerr << "ERROR: The tree '" << _trees[tree_index]->GetName() << "' already contains a variable called '" << var_name << "' - please call your variable something else." << std::endl;
    std::cerr << "Program will now exit, to save you from having to solve a nasty bug later..." << std::endl;
    exit(1);
  }

  if (_tree_vars_all_vars[tree_index][var_index]!="") {
    std::cerr << "ERROR: The tree '" << _trees[tree_index]->GetName() << "' already contains a variable with index " << var_index 
              << " and a different name '" << GetVarName(tree_index, var_index)   
              << "' - please use a different index for variable '" << var_name << "'" << std::endl;
    std::cerr << "Program will now exit, to save you from having to solve a nasty bug later..." << std::endl;
    exit(1);
  }

  _tree_vars_all_vars[tree_index][var_index]=var_name;

  return true;
}

//********************************************************************
void OutputManager::InitializeCounter(Int_t counter_index){
//********************************************************************

  for (UInt_t i= 0; i<_trees_indices.size();i++){    
    InitializeCounter(_trees_indices[i], counter_index);
  }    
}

//********************************************************************
void OutputManager::InitializeCounter(Int_t tree_index, Int_t counter_index){
//********************************************************************
  
  if (_tree_vars_counter_size[tree_index][counter_index] == -1) return;

  // check if the counter has a fixed value
  if (_tree_vars_counter_size[tree_index][counter_index] > 0) return;
  
  // Reset the counter to 0
  _tree_vars_counter[tree_index][counter_index]=0;	

}

//********************************************************************
void OutputManager::InitializeVar(Int_t index, Double_t ini){
//********************************************************************

  // Initialise int and double variables to the value ini

  for (UInt_t i= 0; i<_trees_indices.size();i++){    
    Int_t tree_index = _trees_indices[i];
    if (_tree_vars_int[tree_index][index]!=-1){
      _tree_vars_int[tree_index][index]=(Int_t)ini;	
    }
    else if (_tree_vars_float[tree_index][index]!=-1){
      _tree_vars_float[tree_index][index]=(Float_t)ini;
    }
    else if (_tree_vars_double[tree_index][index]!=-1){
      _tree_vars_double[tree_index][index]=(Double_t)ini;
    }
  }
}

//********************************************************************
void OutputManager::InitializeVectorVar(Int_t index, Double_t ini){
//********************************************************************

  (void)index;
  (void)ini;

  // Initialise int and Double_t analysis variables to the value ini
  /*
  std::map< std::string, TTree* >::iterator cit;
  for (cit= GetTrees().begin();cit!=GetTrees().end();cit++){    
    std::string tree_name = cit->first;

    // Get the counter corresponding to this vector variable
    std::string counter_name = _tree_vars_vector_counter[GetCurrentTree()][name];

    if (_tree_vars_counter_size[GetCurrentTree()][name]>0){
      if (_tree_vars_int_vector_var[tree_name].find(name)!=_tree_vars_int_vector_var[tree_name].end()){
	for (int i=0;i<_tree_vars_counter_size[GetCurrentTree()][name];i++){
	  _tree_vars_int_vector_var[tree_name][name]->Fill(i,(Int_t)ini);  
	}
      }      
      else if (_tree_vars_float_vector_var[tree_name].find(name)!=_tree_vars_float_vector_var[tree_name].end()){
	for (int i=0;i<_tree_vars_counter_size[GetCurrentTree()][name];i++){
	  _tree_vars_float_vector_var[tree_name][name]->Fill(i,(Float_t)ini);	  
	}
      }
      else if (_tree_vars_double_vector_var[tree_name].find(name)!=_tree_vars_double_vector_var[tree_name].end()){
	for (int i=0;i<_tree_vars_counter_size[GetCurrentTree()][name];i++){
	  _tree_vars_double_vector_var[tree_name][name]->Fill(i,(Double_t)ini);	  
	}
      }
    }
  }
  */
}

//********************************************************************
void OutputManager::InitializeMatrixVar(Int_t index, Double_t ini){
//********************************************************************
  (void)index;
  (void)ini;
  // Initialise int and double analysis variables to the value ini
  /*
  std::map< std::string, TTree* >::iterator cit;
  for (cit= GetTrees().begin();cit!=GetTrees().end();cit++){    
    std::string tree_name = cit->first;
    if (_tree_vars_int_matrix_var[tree_name].find(name)!=_tree_vars_int_matrix_var[tree_name].end()){
      for (unsigned int i=0;i<_tree_vars_int_matrix_var[tree_name][name]->size();i++){
	for (unsigned int j=0;j<(*_tree_vars_int_matrix_var[tree_name][name])[i].size();j++){
	  (*(_tree_vars_int_matrix_var[tree_name][name]))[i][j]=(Float_t)ini; 
	}
      }
    }

    else if (_tree_vars_double_matrix_var[tree_name].find(name)!=_tree_vars_double_matrix_var[tree_name].end()){
      for (unsigned int i=0;i<_tree_vars_double_matrix_var[tree_name][name]->size();i++){
	for (unsigned int j=0;j<(*_tree_vars_double_matrix_var[tree_name][name])[i].size();j++){
	  (*(_tree_vars_double_matrix_var[tree_name][name]))[i][j]=(Double_t)ini; 
	}    	
      }
    }

  }
    */
}

//********************************************************************
void OutputManager::InitializeAnalysisVar(Int_t index, Double_t ini){
//********************************************************************

  InitializeVectorVar(index,ini);
}

//********************************************************************
void OutputManager::InitializeAnalysisVectorVar(Int_t index, Double_t ini){
//********************************************************************

  InitializeMatrixVar(index,ini);
}


//********************************************************************
void OutputManager::FillTree(Int_t tree_index){
//********************************************************************  

  if(_file)
    _file->cd();
  GetTree(tree_index)->Fill();  
  
}

//********************************************************************
bool OutputManager::OpenOutputFile(const std::string& file){
//********************************************************************

  if (_file) delete _file;

  _file = new TFile(file.c_str(),"NEW");

  return !_file->IsZombie();
}

//********************************************************************
void OutputManager::CloseOutputFile(){
//********************************************************************

  // Write a tree to a file
  _file->Close();
}

//********************************************************************
void OutputManager::WriteTree(const std::string& file, const std::string& tree_name){
//********************************************************************

  // Write a tree to a file  
  _file = new TFile(file.c_str(),"NEW");
  GetTree(tree_name.c_str())->Write();
  _file->Close();
}

//********************************************************************
void OutputManager::WriteTrees(const std::string& file){
//********************************************************************

  // Write all trees to a file
  _file = new TFile(file.c_str(),"NEW");

  for (UInt_t i= 0; i<_trees_indices.size();i++){    
    GetTree(_trees_indices[i])->Write();
  }
  _file->Close();  
}

//********************************************************************
std::string OutputManager::GetString(int a){
//********************************************************************

  std::stringstream st;
  st << a;
  return st.str();
}


//********************************************************************
std::string OutputManager::GetSize(const std::string& counter_name, unsigned int size){
//********************************************************************

  std::string ssize=counter_name;  
  if (ssize=="")
    ssize = GetString(size);

  return ssize;
}

//********************************************************************
void OutputManager::FillMicroTrees(){
//********************************************************************

  // Number of toy experiments
  FillVar(NTOYS, (Int_t)GetNToys());
  
  /*
  Float_t total=0;
  for (UInt_t i=0;i<GetNToys();i++){
    total += _toyWeights[i];
  }

  if (total!=0){
    for (UInt_t i=0;i<GetNToys();i++){
      FillVectorVar(toy_weight,   (Float_t)_toyWeights[i]/total, i);
    }
  }
  */
  Float_t w = 1./GetNToys();
  for (UInt_t i=0;i<GetNToys();i++)
    FillVectorVar(toy_weight, w, i);

  // Fill the tree
  FillTree();  
}


//*********************************************************
void OutputManager::SetNToys(Int_t tree_index, Int_t nana){
//*********************************************************

  if(HasCounter(tree_index, NTOYS))
    ResizeCounter(tree_index, NTOYS,nana);
  else
    AddCounter(tree_index,NTOYS,"NTOYS",nana);	

  // Delete the variables that depend on the number of toy experiments
  DeleteVar(tree_index,toy_index);
  DeleteVar(tree_index,toy_weight);

  // Add the variables again with the correct size
  AddToyVar(tree_index,toy_index, "toy_index" ,"I", "Toy experiment index");
  AddToyVar(tree_index,toy_weight,"toy_weight","F", "Total toy experiment weight");

}

//********************************************************************
void OutputManager::AddToyVar(Int_t index, const std::string& name, const std::string& type, const std::string& doc){
//********************************************************************

  if (_single_tree_fill==-1){
    // Add a variable to all trees
    for (UInt_t i= 0; i<_trees_indices.size();i++){    
      Int_t tree_index = _trees_indices[i];
      if (IsSpecialTree(tree_index)) continue;
      AddVectorVar(tree_index, index, name, type,doc, NTOYS,"NTOYS", GetNToys(tree_index));
    }
  }
  else{  
    AddVectorVar(_single_tree_fill, index, name, type,doc, NTOYS,"NTOYS", GetNToys(_single_tree_fill));
  }
}

//********************************************************************
void OutputManager::AddToyVar(Int_t tree_index, Int_t index, const std::string& name, const std::string& type, const std::string& docstring){
//********************************************************************

  AddVectorVar(tree_index,index, name, type,docstring, NTOYS, "NTOYS",GetNToys(tree_index));
}

//********************************************************************
void OutputManager::AddToyVectorVar(Int_t index, const std::string& name, const std::string& type, const std::string& doc, Int_t size2){
//********************************************************************

  if (_single_tree_fill==-1){
    // Add a variable to all trees
    for (UInt_t i= 0; i<_trees_indices.size();i++){    
      Int_t tree_index = _trees_indices[i];
      if (IsSpecialTree(tree_index)) continue;
      AddMatrixVar(tree_index, index, name, type,doc, NTOYS, "NTOYS",GetNToys(tree_index), size2);
    }
  }
  else{  
    AddMatrixVar(_single_tree_fill, index, name, type,doc, NTOYS, "NTOYS",GetNToys(_single_tree_fill), size2);
  }
}

//********************************************************************
void OutputManager::AddToyVectorVar(Int_t tree_index, Int_t index, const std::string& name, const std::string& type, const std::string& docstring, Int_t size){
//********************************************************************

  AddMatrixVar(tree_index, index, name, type,docstring, NTOYS, "NTOYS",GetNToys(tree_index),size);
}

//********************************************************************
void OutputManager::AddToyMatrixVar(Int_t index, const std::string& name, const std::string& type, const std::string& doc, Int_t size2, Int_t size3){
//********************************************************************

  if (_single_tree_fill==-1){
    // Add a variable to all trees
    for (UInt_t i= 0; i<_trees_indices.size();i++){    
      Int_t tree_index = _trees_indices[i];
      if (IsSpecialTree(tree_index)) continue;
      Add3DMatrixVar(tree_index, index, name, type,doc, NTOYS, "NTOYS",GetNToys(tree_index), size2, size3);
    }
  }
  else{  
    Add3DMatrixVar(_single_tree_fill, index, name, type,doc, NTOYS, "NTOYS",GetNToys(_single_tree_fill), size2, size3);
  }
}

//********************************************************************
void OutputManager::AddToyMatrixVar(Int_t tree_index, Int_t index, const std::string& name, const std::string& type, const std::string& docstring, Int_t size2, Int_t size3){
//********************************************************************

  Add3DMatrixVar(tree_index, index, name, type,docstring, NTOYS, "NTOYS",GetNToys(tree_index),size2, size3);
}


//********************************************************************
void OutputManager::AddVar(Int_t index, const std::string& name, const std::string& type, const std::string& doc, double ini){
//********************************************************************

  if (_single_tree_fill==-1){
    // Add a variable to all trees
    for (UInt_t i= 0; i<_trees_indices.size();i++){    
      Int_t tree_index = _trees_indices[i];
      if (IsSpecialTree(tree_index)) continue;
      AddVar(tree_index, index, name, type,doc, ini);
    }
  }
  else{  
    AddVar(_single_tree_fill, index, name, type,doc, ini);
  }
}

//********************************************************************
void OutputManager::AddVar(Int_t tree_index, Int_t index, const std::string& name, const std::string& type, const std::string& doc, double ini){
//********************************************************************  

  ValidateVarNameAndIndex(tree_index,index,name);

  if (type=="I"){
    _tree_vars_int[tree_index][index]=ini;
    _tree_vars_used_int[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), &_tree_vars_int[tree_index][index], (name+"/"+type).c_str());
    _trees[tree_index]->SetBranchAddress(name.c_str(), &_tree_vars_int[tree_index][index]);
  }
  else if (type=="F"){
    _tree_vars_float[tree_index][index]=ini;
    _tree_vars_used_float[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), &_tree_vars_float[tree_index][index], (name+"/"+type).c_str());
    _trees[tree_index]->SetBranchAddress(name.c_str(), &_tree_vars_float[tree_index][index]);
  }
  else if (type=="D"){
    _tree_vars_double[tree_index][index]=ini;
    _tree_vars_used_double[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), &_tree_vars_double[tree_index][index], (name+"/"+type).c_str());
    _trees[tree_index]->SetBranchAddress(name.c_str(), &_tree_vars_double[tree_index][index]);
  }
  else if (type=="C"){
    _tree_vars_char[tree_index][index].Fill("");
    _tree_vars_used_char[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), &_tree_vars_char[tree_index][index], (name+"/"+type).c_str());
    _trees[tree_index]->SetBranchAddress(name.c_str(), &_tree_vars_char[tree_index][index]);
  }

  docstrings().DocumentVar(GetTreeName(tree_index), name, doc, type, 0, "", 0, "", 0, "");
}

//********************************************************************
void OutputManager::AddVectorVar(Int_t index, const std::string& name, const std::string& type, const std::string& doc, Int_t counter_index, const std::string& counter_name, Int_t size){
//********************************************************************

  if (_single_tree_fill==-1){
    // Add a variable to all trees
    for (UInt_t i= 0; i<_trees_indices.size();i++){    
      Int_t tree_index = _trees_indices[i];
      if (IsSpecialTree(tree_index)) continue;
      AddVectorVar(tree_index, index, name, type,doc, counter_index, counter_name, size);
    }
  }
  else{  
    AddVectorVar(_single_tree_fill, index, name, type,doc, counter_index, counter_name, size);
  }
}


//********************************************************************
void OutputManager::AddVectorVar(Int_t tree_index, Int_t index, const std::string& name, const std::string& type, const std::string& doc, Int_t counter_index, const std::string& counter_name, Int_t size){
//********************************************************************

  ValidateVarNameAndIndex(tree_index,index,name);

  std::string ssize= GetSize(counter_name,size);  

  AddCounter(tree_index,index,counter_index,counter_name, size);	

  if (type=="I"){
    _tree_vars_int_vector[tree_index][index]= new int_vector(abs(size));          
    _tree_vars_used_int_vector[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), _tree_vars_int_vector[tree_index][index]->GetAddress(), (name+"["+ssize+"]/I").c_str());
    _trees[tree_index]->SetBranchAddress(name.c_str(), _tree_vars_int_vector[tree_index][index]->GetAddress());
  }
  else if (type=="F"){
    _tree_vars_float_vector[tree_index][index]= new float_vector(abs(size));          
    _tree_vars_used_float_vector[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), _tree_vars_float_vector[tree_index][index]->GetAddress(), (name+"["+ssize+"]/F").c_str());
    _trees[tree_index]->SetBranchAddress(name.c_str(), _tree_vars_float_vector[tree_index][index]->GetAddress());
  }
  else if (type=="D"){
    _tree_vars_double_vector[tree_index][index]= new double_vector(abs(size));          
    _tree_vars_used_double_vector[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), _tree_vars_double_vector[tree_index][index]->GetAddress(), (name+"["+ssize+"]/D").c_str());
    _trees[tree_index]->SetBranchAddress(name.c_str(), _tree_vars_double_vector[tree_index][index]->GetAddress());
  }
  else if (type=="C"){
    _tree_vars_char_vector[tree_index][index]= new char_vector(abs(size));          
    _tree_vars_used_char_vector[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), _tree_vars_char_vector[tree_index][index]->GetAddress());
    // _trees[tree_index]->SetBranchAddress(name.c_str(), _tree_vars_char_vector[tree_index][index]->GetAddress());
  }

  docstrings().DocumentVar(GetTreeName(tree_index), name, doc, type, size, counter_name, 0, "", 0, "");
}

//********************************************************************
void OutputManager::AddVectorVar(Int_t index, const std::string& name, const std::string& type, const std::string& doc, Int_t size){
//********************************************************************

  if (_single_tree_fill==-1){
    // Add a variable to all trees
    for (UInt_t i= 0; i<_trees_indices.size();i++){    
      Int_t tree_index = _trees_indices[i];
      if (IsSpecialTree(tree_index)) continue;
      AddVectorVar(tree_index, index, name, type,doc, size);
    }
  }
  else{  
    AddVectorVar(_single_tree_fill, index, name, type,doc, size);
  }
}

//********************************************************************
void OutputManager::AddVectorVar(Int_t tree_index, Int_t index, const std::string& name, const std::string& type, const std::string& doc, const int size){
//********************************************************************

  ValidateVarNameAndIndex(tree_index,index,name);

  std::string ssize= GetSize("",size);  

  AddCounter(tree_index,index,-1,"", size);	

  if (type=="I"){
    _tree_vars_int_vector[tree_index][index]= new int_vector(abs(size));          
    _tree_vars_used_int_vector[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), _tree_vars_int_vector[tree_index][index]->GetAddress(), (name+"["+ssize+"]/I").c_str());
    _trees[tree_index]->SetBranchAddress(name.c_str(), _tree_vars_int_vector[tree_index][index]->GetAddress());
  }
  else if (type=="F"){
    _tree_vars_float_vector[tree_index][index]= new float_vector(abs(size));          
    _tree_vars_used_float_vector[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), _tree_vars_float_vector[tree_index][index]->GetAddress(), (name+"["+ssize+"]/F").c_str());
    _trees[tree_index]->SetBranchAddress(name.c_str(), _tree_vars_float_vector[tree_index][index]->GetAddress());
  }
  else if (type=="D"){
    _tree_vars_double_vector[tree_index][index]= new double_vector(abs(size));          
    _tree_vars_used_double_vector[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), _tree_vars_double_vector[tree_index][index]->GetAddress(), (name+"["+ssize+"]/D").c_str());
    _trees[tree_index]->SetBranchAddress(name.c_str(), _tree_vars_double_vector[tree_index][index]->GetAddress());
  }
  else if (type=="C"){
    _tree_vars_char_vector[tree_index][index]= new char_vector(abs(size));          
    _tree_vars_used_char_vector[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), _tree_vars_char_vector[tree_index][index]->GetAddress());
    // _trees[tree_index]->SetBranchAddress(name.c_str(), _tree_vars_char_vector[tree_index][index]->GetAddress());
  }

  docstrings().DocumentVar(GetTreeName(tree_index), name, doc, type, size, "", 0, "", 0, "");
}

//********************************************************************
void OutputManager::AddMatrixVar(Int_t index, const std::string& name, const std::string& type, const std::string& doc, Int_t counter_index, const std::string& counter_name, Int_t size1, Int_t size2){
//********************************************************************

  if (_single_tree_fill==-1){
    // Add a variable to all trees
    for (UInt_t i= 0; i<_trees_indices.size();i++){    
      Int_t tree_index = _trees_indices[i];
      if (IsSpecialTree(tree_index)) continue;
      AddMatrixVar(tree_index, index, name, type,doc, counter_index, counter_name, size1, size2);
    }
  }
  else{  
    AddMatrixVar(_single_tree_fill, index, name, type,doc, counter_index, counter_name, size1, size2);
  }
}

//********************************************************************
void OutputManager::AddMatrixVar(Int_t tree_index, Int_t index, const std::string& name, const std::string& type, const std::string& docstring, Int_t counter_index, const std::string& counter_name, int size1, int size2){
//********************************************************************

  ValidateVarNameAndIndex(tree_index,index,name);

  std::string ssize1= GetSize(counter_name,size1);  
  std::string ssize2= GetSize("",size2);  
  AddCounter(tree_index,index,counter_index,counter_name, size1);	
  //  AddCounter(tree_index,index,-1,"", size2);	
  if (type=="I"){
    _tree_vars_int_matrix[tree_index][index]= new int_matrix(abs(size1),size2);  
    _tree_vars_used_int_matrix[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), _tree_vars_int_matrix[tree_index][index]->GetAddress(), (name+"["+ssize1+"]["+ssize2+"]/I").c_str());
    _trees[tree_index]->SetBranchAddress(name.c_str(), _tree_vars_int_matrix[tree_index][index]->GetAddress());
  }
  else if (type=="D"){
    _tree_vars_double_matrix[tree_index][index]= new double_matrix(abs(size1),size2);  
    _tree_vars_used_double_matrix[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), _tree_vars_double_matrix[tree_index][index]->GetAddress(), (name+"["+ssize1+"]["+ssize2+"]/D").c_str());
    _trees[tree_index]->SetBranchAddress(name.c_str(), _tree_vars_double_matrix[tree_index][index]->GetAddress());
  } 
  else if (type=="F"){
    _tree_vars_float_matrix[tree_index][index]= new float_matrix(abs(size1),size2);  
    _tree_vars_used_float_matrix[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), _tree_vars_float_matrix[tree_index][index]->GetAddress(), (name+"["+ssize1+"]["+ssize2+"]/F").c_str());
    _trees[tree_index]->SetBranchAddress(name.c_str(), _tree_vars_float_matrix[tree_index][index]->GetAddress());
  } 
  else if (type=="C"){
    _tree_vars_char_matrix[tree_index][index]= new char_matrix(abs(size1),size2);  
    _tree_vars_used_char_matrix[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), _tree_vars_char_matrix[tree_index][index]->GetAddress());
    _trees[tree_index]->SetBranchAddress(name.c_str(), _tree_vars_char_matrix[tree_index][index]->GetAddress());
  }

  docstrings().DocumentVar(GetTreeName(tree_index), name, docstring, type, size1, counter_name, size2, "", 0, "");
}

//********************************************************************
void OutputManager::AddMatrixVar(Int_t index, const std::string& name, const std::string& type, const std::string& doc, Int_t size1, Int_t size2){
//********************************************************************

  if (_single_tree_fill==-1){
    // Add a variable to all trees
    for (UInt_t i= 0; i<_trees_indices.size();i++){    
      Int_t tree_index = _trees_indices[i];
      if (IsSpecialTree(tree_index)) continue;
      AddMatrixVar(tree_index, index, name, type,doc, size1, size2);
    }
  }
  else{  
    AddMatrixVar(_single_tree_fill, index, name, type,doc, size1, size2);
  }
}

//********************************************************************
void OutputManager::AddMatrixVar(Int_t tree_index, Int_t index, const std::string& name, const std::string& type, const std::string& docstring, int size1, int size2){
//********************************************************************

  ValidateVarNameAndIndex(tree_index,index,name);

  std::string ssize1= GetSize("",size1);  
  std::string ssize2= GetSize("",size2);  
  AddCounter(tree_index,index,-1,"", size1);	

  if (type=="I"){
    _tree_vars_int_matrix[tree_index][index]= new int_matrix(abs(size1),size2);  
    _tree_vars_used_int_matrix[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), _tree_vars_int_matrix[tree_index][index]->GetAddress(), (name+"["+ssize1+"]["+ssize2+"]/I").c_str());
    _trees[tree_index]->SetBranchAddress(name.c_str(), _tree_vars_int_matrix[tree_index][index]->GetAddress());
  }
  else if (type=="D"){
    _tree_vars_double_matrix[tree_index][index]= new double_matrix(abs(size1),size2);  
    _tree_vars_used_double_matrix[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), _tree_vars_double_matrix[tree_index][index]->GetAddress(), (name+"["+ssize1+"]["+ssize2+"]/D").c_str());
    _trees[tree_index]->SetBranchAddress(name.c_str(), _tree_vars_double_matrix[tree_index][index]->GetAddress());
  } 
  else if (type=="F"){
    _tree_vars_float_matrix[tree_index][index]= new float_matrix(abs(size1),size2);  
    _tree_vars_used_float_matrix[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), _tree_vars_float_matrix[tree_index][index]->GetAddress(), (name+"["+ssize1+"]["+ssize2+"]/F").c_str());
    _trees[tree_index]->SetBranchAddress(name.c_str(), _tree_vars_float_matrix[tree_index][index]->GetAddress());
  } 
  else if (type=="C"){
    _tree_vars_char_matrix[tree_index][index]= new char_matrix(abs(size1),size2);  
    _tree_vars_used_char_matrix[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), _tree_vars_char_matrix[tree_index][index]->GetAddress());
    _trees[tree_index]->SetBranchAddress(name.c_str(), _tree_vars_char_matrix[tree_index][index]->GetAddress());
  }

  docstrings().DocumentVar(GetTreeName(tree_index), name, docstring, type, size1, "", size2, "", 0, "");
}


//********************************************************************
void OutputManager::Add3DMatrixVar(Int_t index, const std::string& name, const std::string& type, const std::string& doc, Int_t counter_index, const std::string& counter_name, Int_t size1, Int_t size2, Int_t size3){
//********************************************************************

  if (_single_tree_fill==-1){
    // Add a variable to all trees
    for (UInt_t i= 0; i<_trees_indices.size();i++){    
      Int_t tree_index = _trees_indices[i];
      if (IsSpecialTree(tree_index)) continue;
      Add3DMatrixVar(tree_index, index, name, type,doc, counter_index, counter_name, size1, size2, size3);
    }
  }
  else{  
    Add3DMatrixVar(_single_tree_fill, index, name, type,doc, counter_index, counter_name, size1, size2, size3);
  }
}

//********************************************************************
void OutputManager::Add3DMatrixVar(Int_t tree_index, Int_t index, const std::string& name, const std::string& type, const std::string& docstring, Int_t counter_index, const std::string& counter_name, int size1, int size2, int size3){
//********************************************************************

  ValidateVarNameAndIndex(tree_index,index,name);

  std::string ssize1= GetSize(counter_name,size1);  
  std::string ssize2= GetSize("",size2);  
  std::string ssize3= GetSize("",size3);  

  AddCounter(tree_index,index,counter_index,counter_name, size1);	
  
  if (type=="I"){
    _tree_vars_int_3Dmatrix[tree_index][index]= new int_3Dmatrix(abs(size1),size2,size3);  
    _tree_vars_used_int_3Dmatrix[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), _tree_vars_int_3Dmatrix[tree_index][index]->GetAddress(), (name+"["+ssize1+"]["+ssize2+"]["+ssize3+"]/I").c_str());
    _trees[tree_index]->SetBranchAddress(name.c_str(), _tree_vars_int_3Dmatrix[tree_index][index]->GetAddress());
  }
  else if (type=="D"){
    _tree_vars_double_3Dmatrix[tree_index][index]= new double_3Dmatrix(abs(size1),size2,size3);  
    _tree_vars_used_double_3Dmatrix[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), _tree_vars_double_3Dmatrix[tree_index][index]->GetAddress(), (name+"["+ssize1+"]["+ssize2+"]["+ssize3+"]/D").c_str());
    _trees[tree_index]->SetBranchAddress(name.c_str(), _tree_vars_double_3Dmatrix[tree_index][index]->GetAddress());
  } 
  else if (type=="F"){
    _tree_vars_float_3Dmatrix[tree_index][index]= new float_3Dmatrix(abs(size1),size2,size3);  
    _tree_vars_used_float_3Dmatrix[tree_index].push_back(index);
    _trees[tree_index]->Branch(name.c_str(), _tree_vars_float_3Dmatrix[tree_index][index]->GetAddress(), (name+"["+ssize1+"]["+ssize2+"]["+ssize3+"]/F").c_str());
    _trees[tree_index]->SetBranchAddress(name.c_str(), _tree_vars_float_3Dmatrix[tree_index][index]->GetAddress());
  } 

  docstrings().DocumentVar(GetTreeName(tree_index), name, docstring, type, size1, counter_name, size2, "", size3, "");
}

//********************************************************************
void OutputManager::AddCounter(Int_t tree_index, Int_t counter_index, const std::string& counter_name, int size){
//********************************************************************

  if (HasCounter(tree_index, counter_index)) return;


  _tree_vars_counter[tree_index][counter_index]=0;
  _tree_vars_counter_size[tree_index][counter_index]=0;        

  // Give a fix value to the counter if requested
  if (size>0){

    _tree_vars_counter[tree_index][counter_index]=size;
    _tree_vars_counter_size[tree_index][counter_index]=size;         
  }

  _trees[tree_index]->Branch(counter_name.c_str(), &_tree_vars_counter[tree_index][counter_index], (counter_name+"/I").c_str());
  _trees[tree_index]->SetBranchAddress(counter_name.c_str(), &_tree_vars_counter[tree_index][counter_index]);

  //add the name to the counters list
  _tree_vars_all_counters[tree_index][counter_index]=counter_name;

  _tree_vars_used_counter[tree_index].push_back(counter_index);

}

//********************************************************************
void OutputManager::AddCounter(Int_t tree_index, Int_t index, Int_t counter_index, const std::string& counter_name, int size){
//********************************************************************
  if (counter_index==-1){

    _tree_vars_counter[tree_index][index]=0;
    _tree_vars_counter_size[tree_index][index]=0;        
    
    // Give a fix value to the counter if requested
    if (size>0){
      _tree_vars_counter[tree_index][index]=size;
      _tree_vars_counter_size[tree_index][index]=size;         
    }    
    _link_var_to_counter[tree_index][index]= index;
  }
  else{
    _link_var_to_counter[tree_index][index]= counter_index;
    AddCounter(tree_index,counter_index,counter_name,size);
  }

}

//********************************************************************
bool OutputManager::GetFirstIndexFromCounter(Int_t index, Int_t& indx1){
//********************************************************************

  // Get the counter corresponding to this variable
  Int_t counter_index = GetCounterIndexForVar(index);
  // check the counter type
  if (!CheckCounterType(counter_index, indx1, index)) return false;

  // Take the index from the counter
  if (indx1==-1)
    indx1 = _tree_vars_counter[GetCurrentTree()][counter_index];

  return true;
}

//********************************************************************
void OutputManager::FillVar(Int_t index,  Float_t var){
//********************************************************************

  _tree_vars_float[GetCurrentTree()][index]= var;
}

//********************************************************************
void OutputManager::FillVar(Int_t index,  Double_t var){
//********************************************************************

  _tree_vars_double[GetCurrentTree()][index]= var;
}

//********************************************************************
void OutputManager::FillVar(Int_t index,  Int_t var){
//********************************************************************

  _tree_vars_int[GetCurrentTree()][index]= var;
}

//********************************************************************
void OutputManager::FillVar(Int_t index,  const std::string& var){
//********************************************************************

  _tree_vars_char[GetCurrentTree()][index].Fill(var);
}

//********************************************************************
Float_t OutputManager::GetVarValueF(Int_t index){
//********************************************************************

  return _tree_vars_float[GetCurrentTree()][index];
}

//********************************************************************
Double_t OutputManager::GetVarValueD(Int_t index){
//********************************************************************

  return _tree_vars_double[GetCurrentTree()][index];
}

//********************************************************************
Int_t OutputManager::GetVarValueI(Int_t index){
//********************************************************************

  return _tree_vars_int[GetCurrentTree()][index];
}

//********************************************************************
void OutputManager::FillVectorVar(Int_t index, Int_t var, Int_t indx){
//********************************************************************

  if (!GetFirstIndexFromCounter(index,indx)) return;

  // fill the variable
  try {
    (_tree_vars_int_vector[GetCurrentTree()][index])->Fill(indx,var);
  } catch (OutOfBounds) {
    std::cout << "WARNING: Tried to fill variable " << GetVarName(index) << " outside vector size. Index was: " << indx << std::endl;
  }
}

//********************************************************************
void OutputManager::FillVectorVar(Int_t index, Float_t var, Int_t indx){
//********************************************************************

  if (!GetFirstIndexFromCounter(index,indx)) return;

  // fill the variable
  try {
    (_tree_vars_float_vector[GetCurrentTree()][index])->Fill(indx,var);
  } catch (OutOfBounds) {
    std::cout << "WARNING: Tried to fill variable " << GetVarName(index) << " outside vector size. Index was: " << indx << std::endl;
  }
}

//********************************************************************
void OutputManager::FillVectorVar(Int_t index, Double_t var, Int_t indx){
//********************************************************************

  if (!GetFirstIndexFromCounter(index,indx)) return;

  // fill the variable
  try {
    (_tree_vars_double_vector[GetCurrentTree()][index])->Fill(indx,var);
  } catch (OutOfBounds) {
    std::cout << "WARNING: Tried to fill variable " << GetVarName(index) << " outside vector size. Index was: " << indx << std::endl;
  }
}

//********************************************************************
void OutputManager::FillVectorVar(Int_t index, const std::string& var, Int_t indx){
//********************************************************************

  if (!GetFirstIndexFromCounter(index,indx)) return;

  // fill the variable
  try {
    (_tree_vars_char_vector[GetCurrentTree()][index])->Fill(indx,var.c_str());
  } catch (OutOfBounds) {
    std::cout << "WARNING: Tried to fill variable " << GetVarName(index) << " outside vector size. Index was: " << indx << std::endl;
  }
}

//********************************************************************
void OutputManager::FillMatrixVar(Int_t index, Int_t var, Int_t indx1, Int_t indx2){
//********************************************************************

  
  if (!GetFirstIndexFromCounter(index,indx1)) return;

  // fill the variable    
  try {
    (_tree_vars_int_matrix[GetCurrentTree()][index])->Fill(indx1,indx2,var);
  } catch (OutOfBounds) {
    std::cout << "WARNING: Tried to fill variable " << GetVarName(index) << " outside matrix size. Indices were: " << indx1 << ", " << indx2 << std::endl;
  }
}

//********************************************************************
void OutputManager::FillMatrixVar(Int_t index, Float_t var, Int_t indx1, Int_t indx2){
//********************************************************************

  if (!GetFirstIndexFromCounter(index,indx1)) return;

  // fill the variable    
  try {
    (_tree_vars_float_matrix[GetCurrentTree()][index])->Fill(indx1,indx2,var);
  } catch (OutOfBounds) {
    std::cout << "WARNING: Tried to fill variable " << GetVarName(index) << " outside matrix size. Indices were: " << indx1 << ", " << indx2 << std::endl;
  }
}

//********************************************************************
void OutputManager::FillMatrixVar(Int_t index, Double_t var, Int_t indx1, Int_t indx2){
//********************************************************************

  if (!GetFirstIndexFromCounter(index,indx1)) return;

  // fill the variable    
  try {
    (_tree_vars_double_matrix[GetCurrentTree()][index])->Fill(indx1,indx2,var);
  } catch (OutOfBounds) {
    std::cout << "WARNING: Tried to fill variable " << GetVarName(index) << " outside matrix size. Indices were: " << indx1 << ", " << indx2 << std::endl;
  }
}

//********************************************************************
void OutputManager::Fill3DMatrixVar(Int_t index, Int_t var, Int_t indx1, Int_t indx2, Int_t indx3){
//********************************************************************

  if (!GetFirstIndexFromCounter(index,indx1)) return;

  // fill the variable    
  try {
    (_tree_vars_int_3Dmatrix[GetCurrentTree()][index])->Fill(indx1,indx2,indx3,var);
  } catch (OutOfBounds) {
    std::cout << "WARNING: Tried to fill variable " << GetVarName(index) << " outside 3D matrix size. Indices were: " << indx1 << ", " << indx2 << ", " << indx3 << std::endl;
  }
}

//********************************************************************
void OutputManager::Fill3DMatrixVar(Int_t index, Float_t var, Int_t indx1, Int_t indx2, Int_t indx3){
//********************************************************************

  if (!GetFirstIndexFromCounter(index,indx1)) return;

  // fill the variable    
  try {
    (_tree_vars_float_3Dmatrix[GetCurrentTree()][index])->Fill(indx1,indx2,indx3,var);
  } catch (OutOfBounds) {
    std::cout << "WARNING: Tried to fill variable " << GetVarName(index) << " outside 3D matrix size. Indices were: " << indx1 << ", " << indx2 << ", " << indx3 << std::endl;
  }
}

//********************************************************************
void OutputManager::Fill3DMatrixVar(Int_t index, Double_t var, Int_t indx1, Int_t indx2, Int_t indx3){
//********************************************************************

  if (!GetFirstIndexFromCounter(index,indx1)) return;

  // fill the variable    
  try {
    (_tree_vars_double_3Dmatrix[GetCurrentTree()][index])->Fill(indx1,indx2,indx3,var);
  } catch (OutOfBounds) {
    std::cout << "WARNING: Tried to fill variable " << GetVarName(index) << " outside 3D matrix size. Indices were: " << indx1 << ", " << indx2 << ", " << indx3 << std::endl;
  }
}

//********************************************************************
void OutputManager::FillToyVar(Int_t index, Int_t var){
//********************************************************************

  FillVectorVar(index, var, GetToyIndex());  
}

//********************************************************************
void OutputManager::FillToyVar(Int_t index, Float_t var){
//********************************************************************
  FillVectorVar(index, var, GetToyIndex());  
}

//********************************************************************
void OutputManager::FillToyVar(Int_t index, Double_t var){
//********************************************************************
  FillVectorVar(index, var, GetToyIndex());  
}

//********************************************************************
void OutputManager::FillToyVectorVar(Int_t index, Int_t var, const Int_t comp){
//******************************************************************** 
  FillMatrixVar(index,var, GetToyIndex(),comp);  
}

//********************************************************************
void OutputManager::FillToyVectorVar(Int_t index, Float_t var, const Int_t comp){
//********************************************************************

  FillMatrixVar(index,var, GetToyIndex(),comp);  
}

//********************************************************************
void OutputManager::FillToyVectorVar(Int_t index, Double_t var, const Int_t comp){
//******************************************************************** 
  FillMatrixVar(index,var, GetToyIndex(),comp);  
}

//********************************************************************
void OutputManager::FillToyMatrixVar(Int_t index, Int_t var, const Int_t comp1, const Int_t comp2){
//******************************************************************** 
  Fill3DMatrixVar(index,var, GetToyIndex(),comp1,comp2);  
}

//********************************************************************
void OutputManager::FillToyMatrixVar(Int_t index, Float_t var, const Int_t comp1, const Int_t comp2){
//********************************************************************

  Fill3DMatrixVar(index,var, GetToyIndex(),comp1,comp2);  
}

//********************************************************************
void OutputManager::FillToyMatrixVar(Int_t index, Double_t var, const Int_t comp1, const Int_t comp2){
//******************************************************************** 
  Fill3DMatrixVar(index,var, GetToyIndex(),comp1,comp2);  
}

