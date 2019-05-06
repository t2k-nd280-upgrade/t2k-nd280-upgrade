#ifndef CHit3D_hxx_seen
#define CHit3D_hxx_seen

#include "TROOT.h"
#include "TObject.h"
#include "TMath.h"
#include <iostream>
#include <vector>
#include "TVector3.h"
#include "CHit2D.hxx"


/// The base class for a hit detector element.

class CHit3D : public TObject {
public:
    CHit3D() {}
    virtual ~CHit3D() {}
    
    void SetId(int id){fID=id;}
    
   void SetTime(double time){
       fTime=time;
    }
    
   void SetPosition(TVector3 position){
        fPosition.SetX(position.X());
        fPosition.SetY(position.Y());
        fPosition.SetZ(position.Z());
    }
    
   void SetCharge(double charge){
       fCharge=charge;
    }
    
    void AddConstituent(int hitsId, int plane){
        switch (plane) {
            case 0:
                fConstituentsYZ.push_back(hitsId);
                break;
            case 1:
                fConstituentsXZ.push_back(hitsId);
                break;
            case 2:
                fConstituentsXY.push_back(hitsId);
                break;
            default:
                std::cout<<"Wrong plane, no constituent added"<<std::endl;
                break;
        }
    }
    
    void SetFiberCharge(double charge, int plane){
        switch (plane) {
            case 0:
                fChargeYZ=charge;
                break;
            case 1:
                fChargeXZ=charge;
                break;
            case 2:
                fChargeXY=charge;
                break;
            default:
                std::cout<<"Wrong plane, no charge set"<<std::endl;
                break;
        }
    }
    
    std::vector<int> Get2DConstituents(int plane=0) const{
        if(plane==0){
            if(fConstituentsYZ.size()>0){
                return fConstituentsYZ;
            }else{
                std::cout<<"NO YZ Hits"<<std::endl;
                return {};
            }
        }
        if(plane==1){
            if(fConstituentsXZ.size()>0){
                return fConstituentsXZ;
            }else{
                std::cout<<"NO XZ Hits"<<std::endl;
                return {};
            }
        }
        if(plane==2){
            if(fConstituentsXY.size()>0){
                return fConstituentsXY;
            }else{
                std::cout<<"NO XY Hits"<<std::endl;
                return {};
            }
        }
        return {};
    }

    int GetId() const{return fID;}
    
    double GetTime() const {return fTime;}
    
    double GetCharge() const{return fCharge;}
    
   TVector3 GetPosition() const {return fPosition;}
    
    double GetFiberCharge(int plane) const{
        if(plane==0)return fChargeYZ;
        if(plane==1)return fChargeXZ;
        if(plane==2)return fChargeXY;
        return -1;
    }
    
    
    bool operator==(const CHit3D& rhs) const { return this->GetId() == rhs.GetId();}
    
    bool operator<(const CHit3D& rhs) const{
        
        if(this->GetPosition().X()!=rhs.GetPosition().X()){
            return this->GetPosition().X()<rhs.GetPosition().X();
        }else if(this->GetPosition().X()==rhs.GetPosition().X()){
            if(this->GetPosition().Z()!=rhs.GetPosition().Z()){
                return this->GetPosition().Z()<rhs.GetPosition().Z();
            }else if(this->GetPosition().Z()==rhs.GetPosition().Z()){
                return this->GetPosition().Y()<rhs.GetPosition().Y();
            }
        }
        return false;
    }
    
   
private:
    
    int fID;
    
    double fTime;
    
    TVector3 fPosition;
    
    double fCharge;
    
    double fChargeXY;
    double fChargeXZ;
    double fChargeYZ;
    
    //0=YZ, 1=XZ, 2=XY
    double fPlane;
    
    std::vector<int> fConstituentsXY;
    std::vector<int> fConstituentsXZ;
    std::vector<int> fConstituentsYZ;
    
ClassDef(CHit3D,1);

};
#endif
