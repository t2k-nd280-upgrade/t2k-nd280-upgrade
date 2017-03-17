#ifndef CoreUtils_h
#define CoreUtils_h

#include "BasicTypes.hxx"
#include "WeightType.hxx"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "CoreDataClasses.hxx"
#include "TTree.h"

/// This namespace contains useful functions for analyses
/// SHOULD CONSIDER TEMPLATING THE METHODS!
namespace anaUtils{


    void VectorToArray(const TLorentzVector& v, Float_t* array);  
    void VectorToArray(const TVector3& v, Float_t* array);

    TVector3 ArrayToTVector3(const Float_t* array);
    TLorentzVector ArrayToTLorentzVector(const Float_t* array);

    /// Scalar product of two Float_t arrays. Provided their size
    Float_t ScalarProduct(Float_t* array1, Float_t* array2, Int_t size=3);

    void ConfigureTreeBranch(TTree* tree, const char* bname, void* add, TBranch** ptr = 0);


    void ClearArray(AnaRecObjectC** objArr, int nObj);
    void ClearArray(AnaTrueObjectC** objArr, int nObj);

    void ReserveArray(Int_t*   arr, int n, Int_t   ini=-999.);
    void ReserveArray(Float_t* arr, int n, Float_t ini=-999.);
    
    void CopyArray( const Int_t*          src,      Int_t*          dest,   int n);
    void CopyArray( const Float_t*        src,      Float_t*        dest,   int n);
    void CopyArray( const Double_t*       src,      Float_t*        dest,   int n);

    void CopyArray(AnaRecObjectC**  tgtArr, AnaRecObjectC**    srcArr, int nObj);
    void CopyArray(AnaTrueObjectC** tgtArr, AnaTrueObjectC**   srcArr, int nObj);
   /// Copying of an array 
  
  template <typename T1, typename T2> void CopyArray(const T1* srcArr, T2* tgtArr, size_t nObj){
    for (size_t i = 0; i < nObj; i++) 
      tgtArr[i] = srcArr[i];
  }

 /// Resize an array, basic case for int(s), float(s) etc 
  template <typename T> void ResizeArray(T* &tgtArr, size_t nObjNew, size_t nObjOrig){
    T* tmp = new T[nObjNew];
    for (size_t i = 0; i < nObjNew; i++) 
      tmp[i] = (i < nObjOrig ? tgtArr[i] : 0);

    delete [] tgtArr;
    tgtArr = tmp;
  }


    /// Create variable sized arrays of pointers
    void CreateArray(Weight_h*            &tgtArr,  int nObj, Weight_h ini=1);
    void CreateArray(AnaRecObjectC**      &tgtArr,  int nObj);
    void CreateArray(AnaTrueObjectC**     &tgtArr,  int nObj);
    void CreateArray(Float_t*             &tgtArr,  int nObj, Float_t ini=0);
    void CreateArray(Int_t*               &tgtArr,  int nObj, Int_t ini=0);
    void CreateArray(bool*                &tgtArr,  int nObj, bool ini=true);

    void ResizeArray( AnaRecObjectC**   &tgtArr,    int nObj);
    void ResizeArray( AnaTrueObjectC**  &tgtArr,    int nObj);
    void ResizeArray( Int_t*            &tgtArr,    int nObj);
}
#endif
