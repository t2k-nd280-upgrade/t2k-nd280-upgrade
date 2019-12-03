#ifndef CCluster3D_hxx_seen
#define CCluster3D_hxx_seen

#include "TROOT.h"
#include "TObject.h"
#include "TMath.h"
#include <iostream>
#include <vector>
#include "TVector3.h"
#include "CHit3D.hxx"
#include "CBond3D.hxx"

/// The base class for a clustered hits

class CCluster3D : public TObject {
public:
    CCluster3D() {}
    virtual ~CCluster3D() {}
    
    void SetId(int id){fID=id;}
    
    void AddConstituent(int hit){
        fConstituents.push_back(hit);
    }
    
    void SetStartPoint(int hit){
        fStart = hit;
    }
    
    void SetEndPoint(int hit){
        fEnd = hit;
    }
   
    void AddBond(int bond){
        fBonds.push_back(bond);
    }
   
    int GetId() const{return fID;}
    
    std::vector<int> GetConstituents() const{
        return fConstituents;
    }
    
    int GetStartPoint() const{
        return fStart;
    }
    
    int GetEndPoint() const{
        return fEnd;
    }
    
    std::vector<int> GetBonds() const{
        return fBonds;
    }
    
    bool operator==(const CCluster3D& rhs) const { return this->GetId() == rhs.GetId();}
    
   
private:
    
    int fID;
    
    std::vector<int> fConstituents;
    
    int fStart;
    
    int fEnd;
    
    std::vector<int> fBonds;

    ClassDef(CCluster3D,1);

};
#endif
