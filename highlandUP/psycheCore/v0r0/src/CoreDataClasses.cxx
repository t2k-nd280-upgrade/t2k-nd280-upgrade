#define CoreDataClasses_C

#include "CoreDataClasses.hxx"
#include "TMath.h"

//********************************************************************
AnaRecObjectC::AnaRecObjectC(){
//********************************************************************

  UniqueID   = -999;
  Status     = -999;
  Detector   = 0;  
  TrueObject = NULL;
}

//********************************************************************
AnaRecObjectC::~AnaRecObjectC(){
//********************************************************************

}

//********************************************************************
AnaRecObjectC::AnaRecObjectC(const AnaRecObjectC& recObj){
//********************************************************************

    Status            = recObj.Status;
    Detector          = recObj.Detector;
    UniqueID          = recObj.UniqueID;

    // The associated true recObj
    TrueObject = recObj.TrueObject;
}

//********************************************************************
void AnaRecObjectC::Print() const{
//********************************************************************

    std::cout << "-------- AnaRecObjectC --------- " << std::endl;

    std::cout << "UniqueID:             " << UniqueID           << std::endl;
}

//********************************************************************
AnaTrueObjectC::AnaTrueObjectC(){
//********************************************************************

  ID        = -999;
}

//********************************************************************
AnaTrueObjectC::~AnaTrueObjectC(){
//********************************************************************

}

//********************************************************************
AnaTrueObjectC::AnaTrueObjectC(const AnaTrueObjectC& trueObj){
//********************************************************************

    ID                = trueObj.ID;
}

//********************************************************************
void AnaTrueObjectC::Print() const{
//********************************************************************

    std::cout << "-------- AnaTrueObjectC --------- " << std::endl;

    std::cout << "ID:             " << ID           << std::endl;
}

//********************************************************************
EventBoxB::EventBoxB(){
//******************************************************************** 

    for (UInt_t i=0;i<NMAXRECOBJECTGROUPS;i++){
      nRecObjectsInGroup[i]=0;
      RecObjectsInGroup[i]=NULL;
    }

    for (UInt_t i=0;i<NMAXTRUEOBJECTGROUPS;i++){
      nTrueObjectsInGroup[i]=0;
      TrueObjectsInGroup[i]=NULL;
    }
}

//********************************************************************
EventBoxB::~EventBoxB(){
//********************************************************************

  for (UInt_t i=0;i<NMAXRECOBJECTGROUPS;i++){
    if (RecObjectsInGroup[i])
      delete [] RecObjectsInGroup[i];
    RecObjectsInGroup[i] = NULL;
  }

  for (UInt_t i=0;i<NMAXTRUEOBJECTGROUPS;i++){
    if (TrueObjectsInGroup[i])
      delete [] TrueObjectsInGroup[i];
    TrueObjectsInGroup[i] = NULL;
  }
} 

//********************************************************************
AnaBunchC::AnaBunchC(){
//********************************************************************

    Bunch = -999;
    // The initial weight of the bunch is 1;
    Weight=1;
}

//********************************************************************
AnaBunchC::~AnaBunchC(){
//********************************************************************

}

//********************************************************************
AnaBunchC::AnaBunchC(const AnaBunchC& bunch){
//********************************************************************

    Bunch  = bunch.Bunch;
    Weight = bunch.Weight;
}

//********************************************************************
void AnaBunchC::Print() const{
//********************************************************************

    std::cout << "Bunch:            " << Bunch  << std::endl;
    std::cout << "Weight:           " << Weight  << std::endl;
}


//********************************************************************
AnaSpillC::AnaSpillC(){
//********************************************************************

    isClone = false;

    Bunches.clear();    
}

//********************************************************************
AnaSpillC::~AnaSpillC(){
//********************************************************************

    for (UInt_t i=0;i<Bunches.size();i++)
        delete Bunches[i];    

    Bunches.clear();    
} 

//********************************************************************
AnaSpillC::AnaSpillC(const AnaSpillC& spill){
//********************************************************************

    Bunches.clear();
    for (UInt_t i=0;i<spill.Bunches.size();i++)
        Bunches.push_back(spill.Bunches[i]->Clone());  
}

//********************************************************************
void AnaSpillC::Print() const{
//********************************************************************

}

//********************************************************************
AnaEventC::AnaEventC(){
//********************************************************************

    UniqueID = 0;
    //    DataQuality = NULL;
    isClone = false;

    nEventBoxes=0;
    for (UInt_t i=0;i<NMAXEVENTBOXES;i++)
      EventBoxes[i]=NULL;

    // The initial weight of the Event is 1;
    Weight=1;

    // Must create a summary object when we create an event
    // This is initialised to NULL and SampleId::kUnassigned, so you know it has not passed a selection
    //    Summary = new AnaEventSummaryC();
}

//********************************************************************
AnaEventC::~AnaEventC(){
//********************************************************************
/*
    if (DataQuality){
        delete DataQuality;
        DataQuality = NULL;
    }
*/
    nEventBoxes=0;
    for (UInt_t i=0;i<NMAXEVENTBOXES;i++){
      if (EventBoxes[i]) delete EventBoxes[i];      
      EventBoxes[i]=NULL;
    }

    if(!isClone){
        if (Summary){
            delete Summary;
            Summary = NULL;
        }
    }
    else{
        Summary = NULL;
    }
}

//********************************************************************
void AnaEventC::Copy(const AnaEventC& event, bool copyBunchInfo, bool cloneTruth){
//********************************************************************

  (void) cloneTruth;

    if (copyBunchInfo){
        Weight = event.Weight;
        Summary = event.Summary;
    }

    nEventBoxes=0;
    for (UInt_t i=0;i<NMAXEVENTBOXES;i++)
      EventBoxes[i]=NULL;

    UniqueID = event.UniqueID;

}

//********************************************************************
AnaEventC::AnaEventC(const AnaEventC& event){
//********************************************************************

    // copy bunch info but don't clone truth
    Copy(event, true, false);
}

//********************************************************************
void AnaEventC::Print() const{
//********************************************************************

    std::cout << "-------- AnaEventC --------- " << std::endl;


    //EventInfo.Print();

    //    std::cout << "Good DQ:            " << DataQuality->GoodDaq << std::endl;
    //    std::cout << "Good Spill:         " << Beam->GoodSpill << std::endl;

}

//********************************************************************
AnaSuperEventB::AnaSuperEventB(){
//********************************************************************
    RawEvent = NULL;
    Event = NULL;
}

//********************************************************************
AnaSuperEventB::AnaSuperEventB(AnaEventC* event){
//********************************************************************
    RawEvent = event;
    Event = RawEvent->Clone();
    POTWeight = 1.0;
}

//********************************************************************
AnaSuperEventB::~AnaSuperEventB(){
//********************************************************************
    delete RawEvent;
    delete Event;
    RawEvent = NULL;
    Event = NULL;
}

//********************************************************************
AnaSuperEventB::AnaSuperEventB(const AnaSuperEventB& sevent){
//********************************************************************

    Event    = sevent.RawEvent->Clone();
    POTWeight = sevent.POTWeight;

}


