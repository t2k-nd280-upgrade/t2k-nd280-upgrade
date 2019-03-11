#ifndef _EVENT_H_
#define _EVENT_H_

#include <Hit.hh>
#include <TObject.h>
#include <TClonesArray.h>

class Event : public TObject {

private:

    TClonesArray*    fHits;                   //Collection of hits in the event
    Int_t            fNHits;                  //Number of hits in the event
    ULong_t          fEventID;                //Number of the event
    Double_t         fMaxCharge;              //Maximum energy in the event
    Int_t            fFEB12ch;                //FEB 12 channel
    Double_t         fFEB12LeadTime;          //lead time of FEB12 trigger
    Int_t            fOccupancyXZ[48];        //Occupancy per layer of Z in the XZ view
    Int_t            fOccupancyZY[48];        //Occupancy per layer of Z in the ZY view
    Double_t         fdEdzXZ[48];             //Energy loss along Z in XZ plane
    Double_t         fdEdzZY[48];             //Energy loss along Z in ZY plane
    Double_t         fdEdz[48];               //Energy loss along Z in both XZ and ZY planes (sum)
    Int_t            fRange;                  //last layer reached by the particle


public:

    //constructors
    Event()
    {
      fNHits = 0;
      fHits  = new TClonesArray("Hit", 10000);
    }

    //destructor
    virtual ~Event()
    {
      Reset(0);
    }

    //functions to store data
    void SetEventID(Int_t p_fEventID) { fEventID = p_fEventID; }
    void SetMaxCharge(Int_t p_MaxCharge) {fMaxCharge = p_MaxCharge;}
    void SetOccupancyXZ(Int_t p_Z, Int_t p_N) {fOccupancyXZ[p_Z] = p_N; }
    void SetOccupancyZY(Int_t p_Z, Int_t p_N) {fOccupancyZY[p_Z] = p_N; }
    void SetdEdzXZ(Int_t p_Z, Double_t p_E) {fdEdzXZ[p_Z] = p_E; }
    void SetdEdzZY(Int_t p_Z, Double_t p_E) {fdEdzZY[p_Z] = p_E; }
    void SetdEdz(Int_t p_Z, Double_t p_E) {fdEdz[p_Z] = p_E; }
    void SetRange(Int_t p_Z) {fRange = p_Z; }
    void SetFEB12ch(Double_t p_FEB12ch) {fFEB12ch = p_FEB12ch;}
    void SetFEB12LeadTime(Int_t p_FEB12LeadTime) {fFEB12LeadTime = p_FEB12LeadTime;}


    //functions to retrieve data
    TClonesArray *GetHits() const {return fHits;}
    Int_t GetNHits()  const { return fNHits; }
    ULong_t GetEventID() {return fEventID; }
    Double_t GetMaxCharge() {return fMaxCharge; }
    Int_t GetFEB12ch() {return fFEB12ch;}
    Double_t GetFEB12LeadTime() {return fFEB12LeadTime;}
    Int_t GetOccupancyXZ(Int_t p_Z) {return fOccupancyXZ[p_Z];}
    Int_t GetOccupancyZY(Int_t p_Z) {return fOccupancyZY[p_Z];}
    Double_t GetdEdzXZ(Int_t p_Z) {return fdEdzXZ[p_Z];}
    Double_t GetdEdzZY(Int_t p_Z) {return fdEdzZY[p_Z];}
    Double_t GetdEdz(Int_t p_Z) {return fdEdz[p_Z];}
    Int_t GetRange() {return fRange;}


    //methods
    void Clear(Option_t *option="");
    Hit* AddHit();
    Double_t FindMaxCharge();


    void Reset(Option_t* /*option*/="")
    {
      delete fHits;  fHits = 0;
    }

    ClassDef (Event,1);

};


#endif // _CLASSES_H_
