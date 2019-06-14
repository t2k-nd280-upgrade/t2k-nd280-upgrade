#ifndef CBond3D_hxx_seen
#define CBond3D_hxx_seen

#include "TROOT.h"
#include "TObject.h"
#include "TMath.h"
#include <iostream>
#include <vector>
#include "TVector3.h"
#include "CHit3D.hxx"

/// The base class for a clustered hits

class CBond3D : public TObject {
public:
    CBond3D() {}
    virtual ~CBond3D() {}
    
    void SetId(int id){fID=id;}
    
    void AddConstituent(int clusterId){
        fConstituents.push_back(clusterId);
    }
    
    void SetPoint(int hit){
        fPoint = hit;
    }
   
    int GetId() const{return fID;}
    
    std::vector<int> GetConstituents() const{
        return fConstituents;
    }
    
    int GetPoint() const{
        return fPoint;
    }
    
    bool operator==(const CBond3D& rhs) const { return this->GetId() == rhs.GetId();}
    
   
private:
    
    int fID;
    
    std::vector<int> fConstituents;
    
    int fPoint; //CHit3D

    ClassDef(CBond3D,1);

};
#endif
