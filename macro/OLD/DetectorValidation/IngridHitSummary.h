#ifndef INGRIDHITSUMMARY_H
#define INGRIDHITSUMMARY_H
#include <iostream>
#include "TObject.h"

#include "TRefArray.h"
#include "TRef.h"

#include "IngridSimHitSummary.h"

#define INGRIDHIT_MAXSIMHITS 1

//......................................................................

class IngridHitSummary : public TObject
{
public:
    IngridHitSummary();
    IngridHitSummary(const IngridHitSummary& hit);
    virtual ~IngridHitSummary();
    
    void Clear   (Option_t* option="");
    void Print();

public:

    int mod;                  // Module ID
                              // 0~6:Horizontal, 7~13:Vertical
                              // 14, 15: Off-axis INGRID module
                              // 16: proton module??
    int cyc;                  // Integration number 0~22
                              // usually 1st beam bunch <-> 4 cycle

    int view;                 // xz view (0), yz view (1)
    int pln;                  // plane number
    int ch;                   // strip number(0~23)
    int adc;                  // high gain ADC value
    int loadc;                // low  gain ADC value
    float pe;                 // number of photoelectrons, without correction
                              // for light attenuation in fiber,
                              //     MPPC crosstalk&afterpulse,
                              //     MPPC liniarity
                              //     Elec. linearity
    float lope;               // number of photoelectrons, without correction
                              // for light attenuation in fiber,
                              //     MPPC crosstalk&afterpulse,
                              //     MPPC liniarity
                              //     Elec. linearity
    float Pe(){ if(pe<50)return pe; else return lope;}
    float pecorr;             // number of photoelectrons, with correction
                              // after correction
                              // (needs 3D reconstruction)
    float vise;               // visible energy (MeV), without correction
                              // before correction
    float visecorr;           // visible energy (MeV), with correction
                              // after  correction
    long   tdc;                // raw TDC value
    float time;               // hit time (ns).
                              // before correction 
                              // for light propagation in fiber
    float tnearhit;           // minumum value of hit time difference (ns).
                              // comparing other hit

    float timecorr;           // hit time (ns). Only the first hit inside 
                              // after  correction
    float xy;                 // transverse position (cm), x or y
                              // depending on view
    float z;                  // position (cm) along beam direction
    int   addbasicrecon;      // This hit is member of basic recon or not
    int   dummy;              // this is dummy(study for MPPC noise)

    bool  gocosmic;           // For efficiency study with cosmic, it is denominator for efficiency
    bool  hitcosmic;          // For efficiency study with cosmic, it is numerator for efficiency
    bool  isohit;

    int NSimHits() const {return nsimhits;}
    // number of sim hits associated to this reco hit

    void AddIngridSimHit(IngridSimHitSummary* hit);
    IngridSimHitSummary* GetIngridSimHit(int i) const;

private:

    int nsimhits;
    TRef fIngridSimHit[INGRIDHIT_MAXSIMHITS];

    ClassDef(IngridHitSummary, 6) // Ingrid Hit Summary
};

#endif // INGRIDHITSUMMARY_H
////////////////////////////////////////////////////////////////////////
