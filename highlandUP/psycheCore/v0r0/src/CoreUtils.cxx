#include "CoreUtils.hxx"
#include <TVector3.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <typeinfo>


//********************************************************************
void anaUtils::ConfigureTreeBranch(TTree* tree, const char* bname, void* add, TBranch** ptr){
//********************************************************************
  
  if (!tree->FindLeaf(bname)) return;
  tree->SetBranchAddress(bname,add,ptr);  
}

//********************************************************************
void anaUtils::ClearArray(AnaRecObjectC** objArr, int nObj){
//********************************************************************

    for(int i = 0; i < nObj; ++i){
        objArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::ClearArray(AnaTrueObjectC** objArr, int nObj){
//********************************************************************

    for(int i = 0; i < nObj; ++i){
        objArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::ReserveArray(Int_t* arr, int n, Int_t ini){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = ini;
    }
    return;
}

//********************************************************************
void anaUtils::ReserveArray(Float_t* arr, int n, Float_t ini){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = ini;
    }
    return;
}

//********************************************************************
void anaUtils::CopyArray(const Int_t* src, Int_t* dest, int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        dest[i] = src[i];
    }
    return;
}


//********************************************************************
void anaUtils::CopyArray(const Float_t* src, Float_t* dest, int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        dest[i] = src[i];
    }
    return;
}

//********************************************************************
void anaUtils::CopyArray(const Double_t* src, Float_t* dest, int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        dest[i] = (Float_t) src[i];
    }
    return;
}

//********************************************************************
void anaUtils::CopyArray(AnaRecObjectC** tgtArr, AnaRecObjectC** srcArr, int nObj){
//********************************************************************

    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = srcArr[i];
    }
}

//********************************************************************
void anaUtils::CopyArray(AnaTrueObjectC** tgtArr, AnaTrueObjectC** srcArr, int nObj){
//********************************************************************

    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = srcArr[i];
    }
}

//********************************************************************
void anaUtils::VectorToArray(const TLorentzVector& v, Float_t* array){
//********************************************************************
    for (UInt_t i=0; i<4;i++) array[i]=v[i];
}

//********************************************************************
void anaUtils::VectorToArray(const TVector3& v, Float_t* array){
//********************************************************************
    for (UInt_t i=0; i<3;i++) array[i]=v[i];
}

//********************************************************************
TVector3 anaUtils::ArrayToTVector3(const Float_t* array){
//********************************************************************

    TVector3 v;
    for (UInt_t i=0; i<3;i++) v[i]=array[i];
    return v;
}

//********************************************************************
TLorentzVector anaUtils::ArrayToTLorentzVector(const Float_t* array){
//********************************************************************

    TLorentzVector v;
    for (UInt_t i=0; i<4;i++) v[i]=array[i];
    return v;
}

//********************************************************************
void anaUtils::CreateArray(Weight_h* &tgtArr, int nObj, Weight_h ini){
//********************************************************************

    tgtArr = new Weight_h[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = ini;
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaRecObjectC** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaRecObjectC*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaTrueObjectC** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaTrueObjectC*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::CreateArray(Float_t* &tgtArr, int nObj, Float_t ini){
//********************************************************************

    tgtArr = new Float_t[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = ini;
    }
}

//********************************************************************
void anaUtils::CreateArray(Int_t* &tgtArr, int nObj, Int_t ini){
//********************************************************************

    tgtArr = new Int_t[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = ini;
    }
}

//********************************************************************
void anaUtils::CreateArray(bool* &tgtArr, int nObj, bool ini){
//********************************************************************

    tgtArr = new bool[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = ini;
    }
}

//********************************************************************
void anaUtils::ResizeArray(AnaRecObjectC** &tgtArr, int nObj){
//********************************************************************

  tgtArr = (AnaRecObjectC**) realloc (tgtArr, nObj*sizeof(AnaRecObjectC*));
}

//********************************************************************
void anaUtils::ResizeArray(AnaTrueObjectC** &tgtArr, int nObj){
//********************************************************************

  tgtArr = (AnaTrueObjectC**) realloc (tgtArr, nObj*sizeof(AnaTrueObjectC*));
}

//********************************************************************
void anaUtils::ResizeArray(Int_t* &tgtArr, int nObj){
//********************************************************************

  tgtArr = (Int_t*) realloc (tgtArr, nObj*sizeof(Int_t));
}


//********************************************************************
Float_t anaUtils::ScalarProduct(Float_t* array1, Float_t* array2, Int_t size){
//********************************************************************

    Float_t product=0;
    for(int i = 0; i < size; ++i){
      product += array1[i]*array2[i];
    }
    return product;
}
