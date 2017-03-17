#include "BasicUtils.hxx"
#include <TVector3.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <typeinfo>

//********************************************************************
void anaUtils::ReclaimArray(AnaTargetParticleB* arr[], int n, int size){
//********************************************************************
    if(arr[0] == NULL) return;
    if(size < 1) return;
    for(int i = size; i < n; ++i){
        if(arr[i] == NULL ) continue;
        delete [] arr[i];
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ReclaimArray(AnaFGDParticleB* arr[], int n, int size){
//********************************************************************
    if(arr[0] == NULL) return;
    if(size < 1) return;
    for(int i = size; i < n; ++i){
        if(arr[i] == NULL ) continue;
        delete [] arr[i];
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ReclaimArray(AnaToFParticleB* arr[], int n, int size){
//********************************************************************
    if(arr[0] == NULL) return;
    if(size < 1) return;
    for(int i = size; i < n; ++i){
        if(arr[i] == NULL ) continue;
        delete [] arr[i];
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ReclaimArray(AnaTPCParticleB* arr[], int n, int size){
//********************************************************************
    if(arr[0] == NULL) return;
    if(size < 1) return;
    for(int i = size; i < n; ++i){
        if(arr[i] == NULL ) continue;
        delete [] arr[i];
        arr[i] = NULL;
    }
    return;
}


//********************************************************************
void anaUtils::ReclaimArray(AnaParticleB* arr[], int n, int size){
//********************************************************************
    if(arr[0] == NULL) return;
    if(size < 1) return;
    for(int i = size; i < n; ++i){
        if(arr[i] == NULL)continue;
        delete [] arr[i];
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ReclaimArray(AnaTrueParticleB* arr[], int n, int size){
//********************************************************************
    if(arr[0] == NULL) return;
    if(size < 1) return;
    for(int i = size; i < n; ++i){
        if(arr[i] == NULL)continue;
        delete [] arr[i];
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ReclaimArray(AnaTrackB* arr[], int n, int size){
//********************************************************************
    if(arr[0] == NULL) return;
    if(size < 1) return;
    for(int i = size; i < n; ++i){
        if(arr[i] == NULL)continue;
        delete [] arr[i];
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ReserveArray(AnaTargetParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = new AnaTargetParticleB();
    }
    return;
}

//********************************************************************
void anaUtils::ReserveArray(AnaFGDParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = new AnaFGDParticleB();
    }
    return;
}

//********************************************************************
void anaUtils::ReserveArray(AnaToFParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = new AnaToFParticleB();
    }
    return;
}


//********************************************************************
void anaUtils::ReserveArray(AnaTPCParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = new AnaTPCParticleB();
    }
    return;
}

//********************************************************************
void anaUtils::ReserveArray(AnaParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = new AnaParticleB;
    }
    return;
}

//********************************************************************
void anaUtils::ReserveArray(AnaTrueParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = new AnaTrueParticleB();
    }
    return;
}

//********************************************************************
void anaUtils::ReserveArray(AnaTrackB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = new AnaTrackB();
    }
    return;
}

//********************************************************************
void anaUtils::ClearArray(AnaTargetParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ClearArray(AnaFGDParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ClearArray(AnaToFParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ClearArray(AnaTPCParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ClearArray(AnaParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ClearArray(AnaTrueParticleB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = NULL;
    }
    return;
}

//********************************************************************
void anaUtils::ClearArray(AnaTrackB* arr[], int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        arr[i] = NULL;
    }
    return;
}
//********************************************************************
void anaUtils::CopyArray(AnaTrackB** const &src, AnaTrackB** &dest, int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        dest[i] = src[i];
    }
    return;
}

//********************************************************************
void anaUtils::CopyArray(AnaTrueParticleB** const &src, AnaTrueParticleB** &dest, int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        dest[i] = src[i];
    }
    return;
}

//********************************************************************
void anaUtils::CopyArray(AnaParticleB** const &src, AnaParticleB** &dest, int n){
//********************************************************************

    for(int i = 0; i < n; ++i){
        dest[i] = src[i];
    }
    return;
}

//********************************************************************
void anaUtils::CopyArray(AnaRecObjectC** tgtArr, AnaParticleB** srcArr, int nObj){
//********************************************************************

    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = srcArr[i];
    }
}

//********************************************************************
void anaUtils::CopyArray(AnaRecObjectC** tgtArr, AnaTrackB** srcArr, int nObj){
//********************************************************************

    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = srcArr[i];
    }
}

//********************************************************************
void anaUtils::CopyArray(AnaTrueObjectC** tgtArr, AnaTrueParticleB** srcArr, int nObj){
//********************************************************************

    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = srcArr[i];
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaTrackB** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaTrackB*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaVertexB** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaVertexB*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaTrueVertexB** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaTrueVertexB*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaParticleB** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaParticleB*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaTargetParticleB** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaTargetParticleB*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaFGDParticleB** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaFGDParticleB*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaToFParticleB** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaToFParticleB*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaTPCParticleB** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaTPCParticleB*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaDetCrossingB** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaDetCrossingB*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::CreateArray(SubDetId::SubDetEnum* &tgtArr, int nObj, SubDetId::SubDetEnum ini){
//********************************************************************

    tgtArr = new SubDetId::SubDetEnum[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = ini;
    }
}

//********************************************************************
void anaUtils::CreateArray(AnaTrueParticleB** &tgtArr, int nObj){
//********************************************************************

    tgtArr = new AnaTrueParticleB*[nObj];
    for(int i = 0; i < nObj; ++i){
        tgtArr[i] = NULL;
    }
}

//********************************************************************
void anaUtils::ResizeArray(AnaTrueParticleB** &tgtArr, int nObj){
//********************************************************************

  tgtArr = (AnaTrueParticleB**) realloc (tgtArr, nObj*sizeof(AnaTrueParticleB*));
}


//********************************************************************
void anaUtils::ResizeArray(AnaTrackB** &tgtArr, int nObj){
//********************************************************************

  tgtArr = (AnaTrackB**) realloc (tgtArr, nObj*sizeof(AnaTrackB*));
}
