#ifndef CHit2D_hxx_seen
#define CHit2D_hxx_seen

#include "TROOT.h"
#include "TObject.h"
#include "TMath.h"
#include <iostream>
#include <vector>


/// The base class for a hit detector element 2D. Works like fiber map.

class CHit2D : public TObject {
public:
    CHit2D() {}
    virtual ~CHit2D() {}
    
    void SetId(int id){fId=id;}
    
   void SetConstituents(const std::vector<int>& id){
        for(std::size_t i=0;i<id.size();++i){
            fConstituents.push_back(id[i]);
        }
   }
    
    void SetRow(double r){fRow=r;}
    
    void SetColumn(double c){fColumn=c;}
    
    void SetTime(double time){fTime=time;}
    
    void SetCharge(double charge){fCharge=charge;}
    
    void SetPlane(double plane){fPlane=plane;}
    
    int GetId() const {return fId;}
    
    std::vector<int> GetConstituents() const {return fConstituents;}
    
    double GetRow() const {return fRow;}
    
    double GetColumn() const {return fColumn;}
    
    double GetTime() const {return fTime;}
    
    double GetCharge()const {return fCharge;}
    
    int GetPlane() const {return fPlane;}
    
    bool operator==(const CHit2D& rhs) const { return this->GetId() == rhs.GetId() && this->GetPlane() == rhs.GetPlane();}
    
    
   
private:
    
    int fId;
    
    std::vector<int> fConstituents;
    
   
    double fTime;
    
    double fRow;
    
    double fColumn;
    
    double fCharge;
    
    //0=YZ, 1=XZ, 2=XY
    double fPlane;

   ClassDef(CHit2D,1);
};


#endif
