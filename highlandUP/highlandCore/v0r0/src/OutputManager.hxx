#ifndef OutputManager_h
#define OutputManager_h

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <stdexcept>

#include "BasicTypes.hxx"
#include "TreeManager.hxx"
#include "DocStringManager.hxx"

/// Maximum size for vector or matrices
const UInt_t MAXVECTORSIZE=10000;

const UInt_t NMAXTREEVARS = 1000;

/// Exception for if we try to fill outside the size of float_vector etc.
class OutOfBounds: public std::runtime_error {
  public:
    OutOfBounds() : std::runtime_error("OutOfBounds") {}
};

class float_vector{
  
public:
  
  float_vector(const int n1){
    _m = new float[n1];
    _n1 =n1;
  }
  ~float_vector() {
    if (_m) delete[] _m;
  }
  
  void Ini(Float_t val){
    for (UInt_t i = 0; i<num_rows(); i++) _m[i]=val;
  }
  
  void Fill(int i, Float_t val){
    if (i < 0 || i >= _n1) throw OutOfBounds();
    _m[i]=val;
    
  }

  Float_t GetValue(int i) const{
    if (i < 0 || i >= _n1) throw OutOfBounds();
    return _m[i];    
  }

  float* GetAddress(){return _m;}
  size_t num_rows(){return _n1;}

protected:
  Float_t *_m;
  int _n1;
};

class float_matrix{

public:
  
  float_matrix(const int n1, const int n2){
    _m = new float[n1*n2];
    _n1 =n1;
    _n2= n2;
  }
  ~float_matrix() {
    if (_m) delete[] _m;
  }

  void Ini(Float_t val){
    for (UInt_t i = 0; i<num_rows(); i++) 
      for (UInt_t j = 0; j<num_cols(); j++) 
	_m[i*_n2+j]=val;
  }

  void Fill(int i, int j, Float_t val){
    if (i < 0 || i >= _n1) throw OutOfBounds();
    if (j < 0 || j >= _n2) throw OutOfBounds();
    _m[i*_n2+j]=val;
  }

  Float_t GetValue(int i, int j) const{
    if (i < 0 || i >= _n1) throw OutOfBounds();
    if (j < 0 || j >= _n2) throw OutOfBounds();
    return _m[i*_n2+j];
  }

  float* GetAddress(){return _m;}
  size_t num_rows(){return _n1;}
  size_t num_cols(){return _n2;}

protected:
  Float_t *_m;
  int _n1;
  int _n2;

};


class float_3Dmatrix{

public:
  
  float_3Dmatrix(const int n1, const int n2, const int n3){
    _m = new float[n1*n2*n3];
    _n1 =n1;
    _n2= n2;
    _n3= n3;
  }
  ~float_3Dmatrix() {
    if (_m) delete[] _m;
  }
  
  void Ini(Float_t val){
    for (UInt_t i = 0; i<num_rows1(); i++) 
      for (UInt_t j = 0; j<num_rows2(); j++) 
	for (UInt_t k = 0; k<num_rows3(); k++) 
	  _m[i*_n3*_n2 + j*_n3 + k]=val;
  }

  void Fill(int i, int j, int k, Float_t val){
    if (i < 0 || i >= _n1) throw OutOfBounds();
    if (j < 0 || j >= _n2) throw OutOfBounds();
    if (k < 0 || k >= _n3) throw OutOfBounds();
    _m[i*_n3*_n2 + j*_n3 + k]=val;
  }
  Float_t GetValue(int i, int j, int k) const{
    if (i < 0 || i >= _n1) throw OutOfBounds();
    if (j < 0 || j >= _n2) throw OutOfBounds();
    if (k < 0 || k >= _n3) throw OutOfBounds();
    return _m[i*_n3*_n2 + j*_n3 + k];
  }
  float* GetAddress(){return _m;}
  size_t num_rows1(){return _n1;}
  size_t num_rows2(){return _n2;}
  size_t num_rows3(){return _n3;}

protected:
  Float_t *_m;
  int _n1;
  int _n2;
  int _n3;
};


class double_vector{

public:
  
  double_vector(const int n1){
    _m = new double[n1];
    _n1 =n1;
  }
  ~double_vector() {
    if (_m) delete[] _m;
  }

  void Ini(Double_t val){
    for (UInt_t i = 0; i<num_rows(); i++) _m[i]=val;
  }

  void Fill(int i, double val){
    if (i < 0 || i >= _n1) throw OutOfBounds();
    _m[i]=val;
  }
  double GetValue(int i) const{
    if (i < 0 || i >= _n1) throw OutOfBounds();
    return _m[i];
  }
  double* GetAddress(){return _m;}
  size_t num_rows(){return _n1;}

protected:
  double *_m;
  int _n1;
};

class double_matrix{

public:
  
  double_matrix(const int n1, const int n2){
    _m = new double[n1*n2];
    _n1 =n1;
    _n2= n2;
  }
  ~double_matrix() {
    if (_m) delete[] _m;
  }

  void Ini(Double_t val){
    for (UInt_t i = 0; i<num_rows(); i++) 
      for (UInt_t j = 0; j<num_cols(); j++) 
	_m[i*_n2+j]=val;
  }

  void Fill(int i, int j, double val){
    if (i < 0 || i >= _n1) throw OutOfBounds();
    if (j < 0 || j >= _n2) throw OutOfBounds();
    _m[i*_n2+j]=val;
  }
  double GetValue(int i, int j) const{
    if (i < 0 || i >= _n1) throw OutOfBounds();
    if (j < 0 || j >= _n2) throw OutOfBounds();
    return _m[i*_n2+j];
  }
  double* GetAddress(){return _m;}
  size_t num_rows(){return _n1;}
  size_t num_cols(){return _n2;}

protected:
  double *_m;
  int _n1;
  int _n2;

};


class double_3Dmatrix{
  
public:
  
  double_3Dmatrix(const int n1, const int n2, const int n3){
    _m = new double[n1*n2*n3];
    _n1 =n1;
    _n2= n2;
    _n3= n3;
  }
  ~double_3Dmatrix() {
    if (_m) delete[] _m;
  }

  void Ini(Double_t val){
    for (UInt_t i = 0; i<num_rows1(); i++) 
      for (UInt_t j = 0; j<num_rows2(); j++) 
	for (UInt_t k = 0; k<num_rows3(); k++) 
	  _m[i*_n3*_n2 + j*_n3 + k]=val;
  }

  void Fill(int i, int j, int k, double val){
    if (i < 0 || i >= _n1) throw OutOfBounds();
    if (j < 0 || j >= _n2) throw OutOfBounds();
    if (k < 0 || k >= _n3) throw OutOfBounds();
    _m[i*_n3*_n2 + j*_n3 + k]=val;
  }
  double GetValue(int i, int j, int k) const{
    if (i < 0 || i >= _n1) throw OutOfBounds();
    if (j < 0 || j >= _n2) throw OutOfBounds();
    if (k < 0 || k >= _n3) throw OutOfBounds();
    return _m[i*_n3*_n2 + j*_n3 + k];
  }
  double* GetAddress(){return _m;}
  size_t num_rows1(){return _n1;}
  size_t num_rows2(){return _n2;}
  size_t num_rows3(){return _n3;}

protected:
  double *_m;
  int _n1;
  int _n2;
  int _n3;
};


class int_vector{

public:
  
  int_vector(const int n1){
    _m = new int[n1];
    _n1 =n1;
  }
  ~int_vector() {
    if (_m) delete[] _m;
  }

  void Ini(Int_t val){
    for (UInt_t i = 0; i<num_rows(); i++) _m[i]=val;
  }

  void Fill(int i, int val){
    if (i < 0 || i >= _n1) throw OutOfBounds();
    _m[i]=val;
  }
  int GetValue(int i) const{
    if (i < 0 || i >= _n1) throw OutOfBounds();
    return _m[i];
  }
  int* GetAddress(){return _m;}
  size_t num_rows(){return _n1;}

protected:
  int *_m;
  int _n1;
};


class int_matrix{

public:
  
  int_matrix(const int n1, const int n2){
    _m = new int[n1*n2];
    _n1 =n1;
    _n2= n2;
  }
  ~int_matrix() {
    if (_m) delete[] _m;
  }

  void Ini(Int_t val){
    for (UInt_t i = 0; i<num_rows(); i++) 
      for (UInt_t j = 0; j<num_cols(); j++) 
	_m[i*_n2+j]=val;
  }

  void Fill(int i, int j, int val){
    if (i < 0 || i >= _n1) throw OutOfBounds();
    if (j < 0 || j >= _n2) throw OutOfBounds();
    _m[i*_n2+j]=val;
  }
  int GetValue(int i, int j) const{
    if (i < 0 || i >= _n1) throw OutOfBounds();
    if (j < 0 || j >= _n2) throw OutOfBounds();
    return _m[i*_n2+j];
  }
  int* GetAddress(){return _m;}
  size_t num_rows(){return _n1;}
  size_t num_cols(){return _n2;}

protected:
  int *_m;
  int _n1;
  int _n2;
};

class int_3Dmatrix{

public:
  
  int_3Dmatrix(const int n1, const int n2, const int n3){
    _m = new int[n1*n2*n3];
    _n1 =n1;
    _n2= n2;
    _n3= n3;
  }
  ~int_3Dmatrix() {
    if (_m) delete[] _m;
  }

  void Ini(Int_t val){
    for (UInt_t i = 0; i<num_rows1(); i++) 
      for (UInt_t j = 0; j<num_rows2(); j++) 
	for (UInt_t k = 0; k<num_rows3(); k++) 
	  _m[i*_n3*_n2 + j*_n3 + k]=val;
  }

  void Fill(int i, int j, int k, int val){
    if (i < 0 || i >= _n1) throw OutOfBounds();
    if (j < 0 || j >= _n2) throw OutOfBounds();
    if (k < 0 || k >= _n3) throw OutOfBounds();
    _m[i*_n3*_n2 + j*_n3 + k]=val;
  }
  int GetValue(int i, int j, int k) const{
    if (i < 0 || i >= _n1) throw OutOfBounds();
    if (j < 0 || j >= _n2) throw OutOfBounds();
    if (k < 0 || k >= _n3) throw OutOfBounds();
    return _m[i*_n3*_n2 + j*_n3 + k];
  }
  int* GetAddress(){return _m;}
  size_t num_rows1(){return _n1;}
  size_t num_rows2(){return _n2;}
  size_t num_rows3(){return _n3;}

protected:
  int *_m;
  int _n1;
  int _n2;
  int _n3;
};


class achar{

public:
  
  achar(){
  }
  ~achar() {
  }

  void Fill(const std::string& var){
    strcpy(_m, var.c_str());
  }
  char* GetAddress(){return _m;}

protected:
  char _m[100];
};

class char_vector{

public:

  // default constructor (required for char_matrix)
  char_vector(){
    _n1 = 0;
    _m.clear();
  } 
  char_vector(const int n1){
    _m.resize(n1);
    for (int i=0; i<n1; i++) _m[i] = "";
    _n1 = n1;
  }
  ~char_vector() {
    _m.clear();
  }

  void Fill(int i, const char* val){
    if (i < 0 || i >= _n1) throw OutOfBounds();
    _m[i]=val;
  }

  std::vector<std::string>* GetAddress(){
    return &_m;
  }
  size_t num_rows(){return _n1;}

protected:
  std::vector<std::string> _m;
  int _n1;
};

#ifdef __CINT__
#pragma link C++ class std::vector<char_vector>+;
#endif

class char_matrix{
  friend class char_vector;
public:

  char_matrix(const int n1, const int n2){
    _m.resize(n2);
    for (int i=0; i<n2; i++) 
      _m[i] = char_vector(n1);
    _n1= n1;
    _n2= n2;
  }
  ~char_matrix() {
    _m.clear();
  }

  void Fill(int i, int j, const char* val){
    if (i < 0 || i >= _n1) throw OutOfBounds();
    if (j < 0 || j >= _n2) throw OutOfBounds();
    _m[i].Fill(j,val);
  }

  std::vector<char_vector>* GetAddress(){return &_m;}
  size_t num_rows(){return _n1;}
  size_t num_cols(){return _n2;}

protected:
  std::vector<char_vector>  _m;
  int _n1;
  int _n2;
};

/// This class is an easy interfice ROOT's TTree functionality.
/// It allows adding single variables to a tree, vectors, matrices, etc.
///
/// TODO: This class needs to be documented properly.
class OutputManager: public TreeManager{
public :

  OutputManager();
  virtual ~OutputManager();
  
  virtual bool Initialize();
  virtual bool InitializeEntry();
  virtual void Finalize();

  void InitializeTrees(bool iniVars=true);
  void InitializeTree(Int_t tree_index, bool iniVars=true);
  void InitializeTree(){InitializeTree(GetCurrentTree());}

  std::string GetString(int a);
  std::string GetSize(const std::string& counter_name, unsigned int size);

  /// Returns the variable name
  const std::string& GetVarName(Int_t tree_index, Int_t var_index) const {return _tree_vars_all_vars[tree_index][var_index];}

  const std::string& GetVarName(Int_t var_index) const {return _tree_vars_all_vars[GetCurrentTree()][var_index];}

  /// Returns the counter name
  const std::string& GetCounterName(Int_t tree_index, Int_t counter_index) const {return _tree_vars_all_counters[tree_index][counter_index];}

  const std::string& GetCounterName(Int_t counter_index) const {return _tree_vars_all_counters[GetCurrentTree()][counter_index];}


  //--------- Operations with counters ------------


  void AddCounter(Int_t tree_index, Int_t index, Int_t counter_index, const std::string& counter_name, int size=-1);
  void AddCounter(Int_t tree_index,              Int_t counter_index, const std::string& counter_name, int size=-1);

  void InitializeCounter(Int_t tree_name, Int_t counter_index);
  void InitializeCounter(Int_t counter_index);

  void ResizeCounter(Int_t tree_index, Int_t counter_index, Int_t size);
  void ResizeCounter(Int_t counter_index, int size);

  bool HasCounter(Int_t tree_name, Int_t counter_index);

  bool CheckCounterType(Int_t counter_index, Int_t indx, Int_t var_index);

  void IncrementCounter(Int_t counter_index){_tree_vars_counter[_current_tree][counter_index]++;}

  void IncrementCounterForVar(Int_t index){IncrementCounter(GetCounterIndexForVar(index));}

  Int_t GetCounterIndexForVar(Int_t index){return _link_var_to_counter[_current_tree][index];}


  Int_t GetCounterValue(Int_t counter_index){return _tree_vars_counter[_current_tree][counter_index];}
  Int_t GetCounterValue(Int_t tree_index, Int_t counter_index){return _tree_vars_counter[tree_index][counter_index];}
  Int_t GetCounterValueForVar(Int_t index){return GetCounterValue(GetCounterIndexForVar(index));}
  Int_t GetCounterValueForVar(Int_t tree_index, Int_t index){return GetCounterValue(tree_index,GetCounterIndexForVar(index));}

  bool GetFirstIndexFromCounter(Int_t index, Int_t& indx1);

  //--------- Adding variables to tree ------------

  void DeleteVar(Int_t tree_index, Int_t index);


  /// Add a single variable to all trees
  void AddVar(                  Int_t index, const std::string& name, const std::string& type, const std::string& doc, double ini=-9999);

  /// Add a single variable to a specific tree
  void AddVar(Int_t tree_index, Int_t index, const std::string& name, const std::string& type, const std::string& doc, double ini=-9999);

  /// Add a vector variable to all trees
  void AddVectorVar(                  Int_t index, const std::string& name, const std::string& type, const std::string& doc, Int_t counter_index, const std::string& counter_name, Int_t size= -MAXVECTORSIZE);

  /// Add a vector variable to a specific tree
  void AddVectorVar(Int_t tree_index, Int_t index, const std::string& name, const std::string& type, const std::string& doc, Int_t counter_index, const std::string& counter_name, Int_t size= -MAXVECTORSIZE);

  /// Add a vector variable to all trees
  void AddVectorVar(                  Int_t index, const std::string& name, const std::string& type, const std::string& doc, const int size);

  /// Add a vector variable to a specific tree
  void AddVectorVar(Int_t tree_index, Int_t index, const std::string& name, const std::string& type, const std::string& doc, const int size);

  /// Add a matrix variable to all trees
  void AddMatrixVar(                  Int_t index, const std::string& name, const std::string& type, const std::string& doc, Int_t counter_index, const std::string& counter_name, int size1=-MAXVECTORSIZE, int size2=-1);

  /// Add a matrix variable to a specific tree
  void AddMatrixVar(Int_t tree_index, Int_t index, const std::string& name, const std::string& type, const std::string& doc, Int_t counter_index, const std::string& counter_name, int size1=-MAXVECTORSIZE, int size2=-1);

  /// Add a matrix variable to all trees
  void AddMatrixVar(                  Int_t index, const std::string& name, const std::string& type, const std::string& doc, int size1=-MAXVECTORSIZE, int size2=-1);

  /// Add a matrix variable to a specific tree
  void AddMatrixVar(Int_t tree_index, Int_t index, const std::string& name, const std::string& type, const std::string& doc, int size1=-MAXVECTORSIZE, int size2=-1);

  /// Add a 3D matrix variable to all trees
  void Add3DMatrixVar(                  Int_t index, const std::string& name, const std::string& type, const std::string& doc, Int_t counter_index, const std::string& counter_name, 
					int size1=-MAXVECTORSIZE, int size2=-1, int size3=-1);
  /// Add a 3D matrix variable to a specific tree
  void Add3DMatrixVar(Int_t tree_index, Int_t index, const std::string& name, const std::string& type, const std::string& doc, Int_t counter_index, const std::string& counter_name, 
		      int size1=-MAXVECTORSIZE, int size2=-1, int size3=-1);

  /// Add a single analysis variable to all trees
  void AddToyVar(                        Int_t index, const std::string& name, const std::string& type, const std::string& docstring);

  /// Add a single analysis variable to a specific tree
  void AddToyVar(      Int_t tree_index, Int_t index, const std::string& name, const std::string& type, const std::string& docstring);

  /// Add a vector analysis variable to all trees
  void AddToyVectorVar(                  Int_t index, const std::string& name, const std::string& type, const std::string& docstring, int ncomp);

  /// Add a vector analysis variable to a specific tree
  void AddToyVectorVar(Int_t tree_index, Int_t index, const std::string& name, const std::string& type, const std::string& docstring, int ncomp);


  /// Add a matrix analysis variable to all trees
  void AddToyMatrixVar(                  Int_t index, const std::string& name, const std::string& type, const std::string& docstring, int ncomp1, int ncomp2);

  /// Add a matrix analysis variable to a specific tree
  void AddToyMatrixVar(Int_t tree_index, Int_t index, const std::string& name, const std::string& type, const std::string& docstring, int ncomp1, int ncomp2);



  //---------- Filling variables --------------

  void InitializeVar(Int_t index, Double_t ini);
  void InitializeVectorVar(Int_t index, Double_t ini);
  void InitializeMatrixVar(Int_t index, Double_t ini);

  /// Fill a single variable
  void FillVar(Int_t index, Float_t var);
  void FillVar(Int_t index, Double_t var);
  void FillVar(Int_t index, Int_t var);
  void FillVar(Int_t index, const std::string& var);

  /// Get the value of a var already filled (so to be used in another package)
  Float_t  GetVarValueF(Int_t index);
  Double_t GetVarValueD(Int_t index);
  Int_t    GetVarValueI(Int_t index);

  /// Fill a vector variable
  void FillVectorVar(Int_t index, Float_t var,            Int_t indx=-1);
  void FillVectorVar(Int_t index, Int_t var,              Int_t indx=-1);
  void FillVectorVar(Int_t index, Double_t var,           Int_t indx=-1);
  void FillVectorVar(Int_t index, const std::string& var, Int_t indx=-1);

  /// Fill a vector variable from array
  void FillVectorVarFromArray(Int_t index, const Double_t    var[], UInt_t size) {for (UInt_t i=0;i<size;i++) FillVectorVar(index,(Double_t)var[i]         ,i);}
  void FillVectorVarFromArray(Int_t index, const Float_t     var[], UInt_t size) {for (UInt_t i=0;i<size;i++) FillVectorVar(index,(Float_t)var[i]          ,i);}
  void FillVectorVarFromArray(Int_t index, const Int_t       var[], UInt_t size) {for (UInt_t i=0;i<size;i++) FillVectorVar(index,(Int_t)var[i]            ,i);}
  void FillVectorVarFromArray(Int_t index, const std::string var[], UInt_t size) {for (UInt_t i=0;i<size;i++) FillVectorVar(index,(const std::string)var[i],i);}

  /// Get the value of a var already filled (so to be used in another package)
  Float_t  GetVectorVarValueF(Int_t index, Int_t i1){return _tree_vars_float_vector [GetCurrentTree()][index]->GetValue(i1);}
  Double_t GetVectorVarValueD(Int_t index, Int_t i1){return _tree_vars_double_vector[GetCurrentTree()][index]->GetValue(i1);}
  Int_t    GetVectorVarValueI(Int_t index, Int_t i1){return _tree_vars_int_vector   [GetCurrentTree()][index]->GetValue(i1);}

  /// Fill a matrix variable
  void FillMatrixVar(Int_t index, Float_t var,  Int_t indx1, Int_t indx2);
  void FillMatrixVar(Int_t index, Int_t var,    Int_t indx1, Int_t indx2);
  void FillMatrixVar(Int_t index, Double_t var, Int_t indx1, Int_t indx2);

  /// Fill a matrix variable from array
  void FillMatrixVarFromArray(Int_t index, const Double_t var[], Int_t indx1, UInt_t size) {for (UInt_t i=0;i<size;i++) FillMatrixVar(index,(Double_t)var[i],indx1,i);}
  void FillMatrixVarFromArray(Int_t index, const Float_t  var[], Int_t indx1, UInt_t size) {for (UInt_t i=0;i<size;i++) FillMatrixVar(index,(Float_t)var[i] ,indx1,i);}
  void FillMatrixVarFromArray(Int_t index, const Int_t    var[], Int_t indx1, UInt_t size) {for (UInt_t i=0;i<size;i++) FillMatrixVar(index,(Int_t)var[i]   ,indx1,i);}

   /// Fill a matrix variable from array
  void FillMatrixVarFromArray(Int_t index, const Double_t var[], UInt_t size) {for (UInt_t i=0;i<size;i++) FillMatrixVar(index,(Double_t)var[i],-1,i);}
  void FillMatrixVarFromArray(Int_t index, const Float_t  var[], UInt_t size) {for (UInt_t i=0;i<size;i++) FillMatrixVar(index,(Float_t)var[i] ,-1,i);}
  void FillMatrixVarFromArray(Int_t index, const Int_t    var[], UInt_t size) {for (UInt_t i=0;i<size;i++) FillMatrixVar(index,(Int_t)var[i]   ,-1,i);}

  /// Get the value of a var already filled (so to be used in another package)
  Float_t  GetMatrixVarValueF(Int_t index, Int_t i1, Int_t i2){return _tree_vars_float_matrix [GetCurrentTree()][index]->GetValue(i1,i2);}
  Double_t GetMatrixVarValueD(Int_t index, Int_t i1, Int_t i2){return _tree_vars_double_matrix[GetCurrentTree()][index]->GetValue(i1,i2);}
  Int_t    GetMatrixVarValueI(Int_t index, Int_t i1, Int_t i2){return _tree_vars_int_matrix   [GetCurrentTree()][index]->GetValue(i1,i2);}

  /// Fill a 3D matrix variable
  void Fill3DMatrixVar(Int_t index, Float_t var,  Int_t indx1, Int_t indx2, Int_t indx3);
  void Fill3DMatrixVar(Int_t index, Int_t var,    Int_t indx1, Int_t indx2, Int_t indx3);
  void Fill3DMatrixVar(Int_t index, Double_t var, Int_t indx1, Int_t indx2, Int_t indx3);

  /// Fill a 3D matrix variable from array
  void Fill3DMatrixVarFromArray(Int_t index, const Double_t var[], Int_t indx1, Int_t indx2, UInt_t size) {for (UInt_t i=0;i<size;i++) Fill3DMatrixVar(index,(Double_t)var[i],indx1,indx2,i);}
  void Fill3DMatrixVarFromArray(Int_t index, const Float_t  var[], Int_t indx1, Int_t indx2, UInt_t size) {for (UInt_t i=0;i<size;i++) Fill3DMatrixVar(index,(Float_t)var[i] ,indx1,indx2,i);}
  void Fill3DMatrixVarFromArray(Int_t index, const Int_t    var[], Int_t indx1, Int_t indx2, UInt_t size) {for (UInt_t i=0;i<size;i++) Fill3DMatrixVar(index,(Int_t)var[i]   ,indx1,indx2,i);}

  /// Fill a single analysis variable
  void FillToyVar(Int_t index, Int_t var);
  void FillToyVar(Int_t index, Float_t var);
  void FillToyVar(Int_t index, Double_t var);

  /// Get the value of a var already filled (so to be used in another package)
  Float_t  GetToyVarValueF(Int_t index){return GetVectorVarValueF(index, GetToyIndex());}
  Double_t GetToyVarValueD(Int_t index){return GetVectorVarValueD(index, GetToyIndex());}
  Int_t    GetToyVarValueI(Int_t index){return GetVectorVarValueI(index, GetToyIndex());}

  /// Fill a vector analysis variable
  void FillToyVectorVar(Int_t index, Int_t var,    Int_t comp);
  void FillToyVectorVar(Int_t index, Float_t var,  Int_t comp);
  void FillToyVectorVar(Int_t index, Double_t var, Int_t comp);

  /// Get the value of a var already filled (so to be used in another package)
  Float_t  GetToyVectorVarValueF(Int_t index, Int_t i1){return GetMatrixVarValueF(index, GetToyIndex(), i1);}
  Double_t GetToyVectorVarValueD(Int_t index, Int_t i1){return GetMatrixVarValueD(index, GetToyIndex(), i1);}
  Int_t    GetToyVectorVarValueI(Int_t index, Int_t i1){return GetMatrixVarValueI(index, GetToyIndex(), i1);}

  /// Fill a matrix analysis variable
  void FillToyMatrixVar(Int_t index, Int_t var,    Int_t comp1, Int_t comp2);
  void FillToyMatrixVar(Int_t index, Float_t var,  Int_t comp1, Int_t comp2);
  void FillToyMatrixVar(Int_t index, Double_t var, Int_t comp1, Int_t comp2);


  //--------- Manage analysis dependent variables --------------


  // Initialize the analysis variable
  void InitializeAnalysisVar(Int_t index, Double_t ini);
  void InitializeAnalysisVectorVar(Int_t index, Double_t ini);

  //--------- Tree operations ------------

  /// Fill a specific tree
  void FillTree(Int_t tree_index);

  /// Fill the current tree
  void FillTree(){FillTree(GetCurrentTree());}

  /// Write a specific tree into a file
  void WriteTree(const std::string& file, const std::string& conf);

  /// Write all trees into a file
  void WriteTrees(const std::string& file);

  /// Add a tree provided its index and name
  void AddTreeWithName(Int_t tree_index, const std::string& tree_name, TTree* tree=NULL);

  /// open the output file
  bool OpenOutputFile(const std::string& file);

  /// close the output file
  void CloseOutputFile();

  /// Ensure that the variable 'name' hasn't been added to the tree with index tree_index
  /// already. Keeps a record of names that it's seen in _tree_vars_all_vars.
  bool ValidateVarNameAndIndex(Int_t tree_index, Int_t var_index, const std::string& var_name);

  /// Whether the given tree name (conf) is "special" or not. Special trees aren't
  /// the normal micro-trees, and contain meta information, like the header or
  /// config info, or RooTracker information.
  bool IsSpecialTree(Int_t tree_index);
  
  //--------- functions to control different toy experiments ----------

  /// Set and gets the index of the current toy experiment
  void SetToyIndex(Int_t index){_toy_index=index;}
  Int_t GetToyIndex(){return _toy_index;}

  /// Sets and gets the number of toy experiments for a given configuration
  void SetNToys(Int_t tree_index, int ntoys);

  /// Get the number of toys for a given tree
  UInt_t GetNToys(Int_t tree_index){return GetCounterValue(tree_index,NTOYS);}

  /// Get the number of toys for the current tree
  UInt_t GetNToys(){return GetCounterValue(NTOYS);}


  void AddToyWeight(Double_t w){_toyWeights.push_back(w);}

  void FillMicroTrees();


  void SetFillSingleTree(Int_t tree_index){_single_tree_fill=tree_index;}

  void SetFillAllTrees(){_single_tree_fill=-1;}

  /// Methods to set and get the current entry in the RooTrackerVtx tree
  void SetRooVtxEntry(Int_t entry){_RooVtxEntry=entry;}
  Int_t GetRooVtxEntry() const {return _RooVtxEntry;}

  /// Methods to check whether the current RooTrackerVtxEntry has been already saved.
  void SetRooVtxEntrySaved(bool saved){_RooVtxEntrySaved=saved;}
  bool GetRooVtxEntrySaved() const {return _RooVtxEntrySaved;}

  /// Methods to check whether the current RooTrackerVtxEntry has been already scheduled for saving
  void SetSaveRooVtxEntry(bool save){_saveRooVtxEntry=save;}
  bool GetSaveRooVtxEntry() const {return _saveRooVtxEntry;}


  /// Increment by one the current entry in the RooTrackerVtx tree
  void IncrementRooVtxEntry(){_RooVtxEntry++;}

  void SetDocStringManager(DocStringManager* doc){_doc=doc;}

  DocStringManager& docstrings(){return *_doc;}
  
 protected:

  // Map of trees
  std::string _default_docstring;

  DocStringManager* _doc;

  std::vector<Double_t> _toyWeights;

  /// current toy experiment index
  int _toy_index; 

  /// current RooTrackerVtx entry
  Int_t _RooVtxEntry;

  /// has the current RooTrackerVtx entry been saved ?
  bool _RooVtxEntrySaved;

  /// has the current RooTrackerVtx entry scheduled to be saved?
  bool _saveRooVtxEntry;

  Int_t _single_tree_fill;


  // ----- Branches in the tree  ---------

  /// The names of all variables added
  std::vector< std::vector< std::string > >      _tree_vars_all_vars;

   /// The names of all counters added
  std::vector< std::vector< std::string > >      _tree_vars_all_counters;
  
  /// Correspondece between a variable index and the counter index  
  std::vector< std::vector< Int_t > >            _link_var_to_counter;

  /// The size of the counter: 0 for variable size vectors, >0 for fix size vectors
  std::vector< std::vector< Int_t > >            _tree_vars_counter_size;

  /// The counter it self
  std::vector< std::vector< Int_t > >            _tree_vars_counter;

  /// Single variables
  std::vector< std::vector< Int_t > >            _tree_vars_int;
  std::vector< std::vector< Float_t > >          _tree_vars_float;
  std::vector< std::vector< Double_t > >         _tree_vars_double;
  std::vector< std::vector< achar > >            _tree_vars_char;

  /// Vector variables (1 index)
  std::vector< std::vector< int_vector* > >      _tree_vars_int_vector;
  std::vector< std::vector< float_vector* > >    _tree_vars_float_vector;
  std::vector< std::vector< double_vector* > >   _tree_vars_double_vector;
  std::vector< std::vector< char_vector* > >     _tree_vars_char_vector;

  /// Matrix variables (2 indices)
  std::vector< std::vector< int_matrix* > >      _tree_vars_int_matrix;
  std::vector< std::vector< float_matrix* > >    _tree_vars_float_matrix;
  std::vector< std::vector< double_matrix* > >   _tree_vars_double_matrix;
  std::vector< std::vector< char_matrix* > >     _tree_vars_char_matrix;

  /// 3D matrix variables (3 indices)
  std::vector< std::vector< int_3Dmatrix* > >    _tree_vars_int_3Dmatrix;
  std::vector< std::vector< float_3Dmatrix* > >  _tree_vars_float_3Dmatrix;
  std::vector< std::vector< double_3Dmatrix* > > _tree_vars_double_3Dmatrix;


  /// The counter it self
  std::vector< std::vector< Int_t > >            _tree_vars_used_counter;

  /// Single variables
  std::vector< std::vector< Int_t > >            _tree_vars_used_int;
  std::vector< std::vector< Int_t > >            _tree_vars_used_float;
  std::vector< std::vector< Int_t > >            _tree_vars_used_double;
  std::vector< std::vector< Int_t > >            _tree_vars_used_char;

  /// Vector variables (1 index)
  std::vector< std::vector< Int_t > >            _tree_vars_used_int_vector;
  std::vector< std::vector< Int_t > >            _tree_vars_used_float_vector;
  std::vector< std::vector< Int_t > >            _tree_vars_used_double_vector;
  std::vector< std::vector< Int_t > >            _tree_vars_used_char_vector;

  /// Matrix variables (2 indices)
  std::vector< std::vector< Int_t > >            _tree_vars_used_int_matrix;
  std::vector< std::vector< Int_t > >            _tree_vars_used_float_matrix;
  std::vector< std::vector< Int_t > >            _tree_vars_used_double_matrix;
  std::vector< std::vector< Int_t > >            _tree_vars_used_char_matrix;

  /// 3D matrix variables (3 indices)
  std::vector< std::vector< Int_t > >            _tree_vars_used_int_3Dmatrix;
  std::vector< std::vector< Int_t > >            _tree_vars_used_float_3Dmatrix;
  std::vector< std::vector< Int_t > >            _tree_vars_used_double_3Dmatrix;
  
public:

  enum enumStandardMicroTrees_OutputManager{
    NTOYS=0,
    toy_weight,
    toy_index,    
    enumStandardMicroTreesLast_OutputManager
  };

  enum enumSpecialTrees{
    config=0,
    header,
    truth,
    NRooTrackerVtx, 
    GRooTrackerVtx,
    RooTrackerVtx,
    enumSpecialTreesLast
  };
  
};


#define AddVarF(out, name, doc)  out.AddVar(name,#name,"F",doc,-9999);
#define AddVarI(out, name, doc)  out.AddVar(name,#name,"I",doc,-9999);
#define AddVarC(out, name, doc)  out.AddVar(name,#name,"C",doc,-9999);
#define AddVarD(out, name, doc)  out.AddVar(name,#name,"D",doc,-9999);

#define AddVarAndIniF(out, name, doc, ini)  out.AddVar(name,#name,"F",doc,ini);
#define AddVarAndIniI(out, name, doc, ini)  out.AddVar(name,#name,"I",doc,ini);
#define AddVarAnaIniC(out, name, doc, ini)  out.AddVar(name,#name,"C",doc,ini);
#define AddVarAndIniD(out, name, doc, ini)  out.AddVar(name,#name,"D",doc,ini);

#define AddVarVF(out, name, doc, count)  out.AddVectorVar(name,#name,"F",doc,count,#count);
#define AddVarVI(out, name, doc, count)  out.AddVectorVar(name,#name,"I",doc,count,#count);
#define AddVarVC(out, name, doc, count)  out.AddVectorVar(name,#name,"C",doc,count,#count);
#define AddVarVD(out, name, doc, count)  out.AddVectorVar(name,#name,"D",doc,count,#count);

#define AddVarMaxSizeVF(out, name, doc, count,size)  out.AddVectorVar(name,#name,"F",doc,count,#count,-size);
#define AddVarMaxSizeVI(out, name, doc, count,size)  out.AddVectorVar(name,#name,"I",doc,count,#count,-size);
#define AddVarMaxSizeVC(out, name, doc, count,size)  out.AddVectorVar(name,#name,"C",doc,count,#count,-size);
#define AddVarMaxSizeVD(out, name, doc, count,size)  out.AddVectorVar(name,#name,"D",doc,count,#count,-size);

#define AddVarFixVF(out, name, doc, size)  out.AddVectorVar(name,#name,"F",doc,size);
#define AddVarFixVI(out, name, doc, size)  out.AddVectorVar(name,#name,"I",doc,size);
#define AddVarFixVC(out, name, doc, size)  out.AddVectorVar(name,#name,"C",doc,size);
#define AddVarFixVD(out, name, doc, size)  out.AddVectorVar(name,#name,"D",doc,size);

#define AddVarMF(out, name, doc, count,size1,size2)  out.AddMatrixVar(name,#name,"F",doc,count,#count,size1,size2);
#define AddVarMI(out, name, doc, count,size1,size2)  out.AddMatrixVar(name,#name,"I",doc,count,#count,size1,size2);
#define AddVarMC(out, name, doc, count,size1,size2)  out.AddMatrixVar(name,#name,"C",doc,count,#count,size1,size2);
#define AddVarMD(out, name, doc, count,size1,size2)  out.AddMatrixVar(name,#name,"D",doc,count,#count,size1,size2);


#define AddVarFixMF(out, name, doc, size1,size2)  out.AddMatrixVar(name,#name,"F",doc,size1,size2);
#define AddVarFixMI(out, name, doc, size1,size2)  out.AddMatrixVar(name,#name,"I",doc,size1,size2);
#define AddVarFixMC(out, name, doc, size1,size2)  out.AddMatrixVar(name,#name,"C",doc,size1,size2);
#define AddVarFixMD(out, name, doc, size1,size2)  out.AddMatrixVar(name,#name,"D",doc,size1,size2);


#define AddVar3VF(out, name, doc)  out.AddVectorVar(name,#name,"F",doc,3);
#define AddVar3VD(out, name, doc)  out.AddVectorVar(name,#name,"D",doc,3);

#define AddVar4VF(out, name, doc)  out.AddVectorVar(name,#name,"F",doc,4);
#define AddVar4VD(out, name, doc)  out.AddVectorVar(name,#name,"D",doc,4);

#define AddVar3MF(out, name, doc, count)  out.AddMatrixVar(name,#name,"F",doc,count,#count,-MAXVECTORSIZE,3);
#define AddVar3MD(out, name, doc, count)  out.AddMatrixVar(name,#name,"D",doc,count,#count,-MAXVECTORSIZE,3);

#define AddVar4MF(out, name, doc, count)  out.AddMatrixVar(name,#name,"F",doc,count,#count,-MAXVECTORSIZE,4);
#define AddVar4MD(out, name, doc, count)  out.AddMatrixVar(name,#name,"D",doc,count,#count,-MAXVECTORSIZE,4);


#define AddToyVarF(out, name, doc)  out.AddToyVar(name,#name,"F",doc);
#define AddToyVarI(out, name, doc)  out.AddToyVar(name,#name,"I",doc);
#define AddToyVarC(out, name, doc)  out.AddToyVar(name,#name,"C",doc);
#define AddToyVarD(out, name, doc)  out.AddToyVar(name,#name,"D",doc);

#define AddToyVarVF(out, name, doc, size)  out.AddToyVectorVar(name,#name,"F",doc,size);
#define AddToyVarVI(out, name, doc, size)  out.AddToyVectorVar(name,#name,"I",doc,size);
#define AddToyVarVD(out, name, doc, size)  out.AddToyVectorVar(name,#name,"D",doc,size);


#define AddTree(out, name, tree)  out.AddTreeWithName(name,#name,tree);


#endif


