#include "EventVariationBase.hxx"
#include "MultiThread.hxx"

//********************************************************************
void EventVariationBase::InitializeEvent(const AnaEventC& event, const SelectionBase& sel){
//********************************************************************

  Int_t uniqueID = 0;

#ifdef MULTITHREAD
  uniqueID = event.UniqueID;
#endif

  // Delete the SystBox when it exists and create a new one. 
  // TODO: It is probably faster to just reset the arrays

  // Create the box
  if(!_systBoxes[0][0][uniqueID])
    _systBoxes[0][0][uniqueID] = new SystBoxB();
  
  // Fill the SystBox with the relevant tracks and true tracks tor this systematic
  FillSystBox(event, sel, *_systBoxes[0][0][uniqueID]);
}

//********************************************************************
void EventVariationBase::InitializeEvent(const AnaEventC& event){
//********************************************************************

  Int_t uniqueID = 0;

#ifdef MULTITHREAD
  uniqueID = event.UniqueID;
#else 
  (void)event;
#endif

  SystBoxB& box =  *(_systBoxes[0][0][uniqueID]);

  if (box.RelevantRecObjects) delete [] box.RelevantRecObjects;
  anaUtils::CreateArray(box.RelevantRecObjects, box.RelevantRecObjectsSet.size());
  box.nRelevantRecObjects=0;  
  for (std::set<AnaRecObjectC*>::iterator it = box.RelevantRecObjectsSet.begin();it!=box.RelevantRecObjectsSet.end();it++){
    box.RelevantRecObjects[box.nRelevantRecObjects++] = *it;
  }

}

//********************************************************************
void EventVariationBase::FillSystBox(const AnaEventC& event, const SelectionBase& sel, SystBoxB& box){
//********************************************************************

  Int_t* groups;
  anaUtils::CreateArray(groups,10);
  Int_t nGroups = GetRelevantRecObjectGroups(sel, groups);
  anaUtils::ResizeArray(groups,nGroups);

  // Get the EventBox for the input selection
  EventBoxB* EventBox = event.EventBoxes[sel.GetEventBoxId()];
  
  if (nGroups>0){
    Int_t nMaxRecObjects=0;
    for (Int_t g = 0; g<nGroups;g++)
      nMaxRecObjects += EventBox->nRecObjectsInGroup[groups[g]];
    
    for (Int_t g = 0; g<nGroups;g++){
      for (Int_t i=0;i<EventBox->nRecObjectsInGroup[groups[g]];i++){
        AnaRecObjectC* recObject = EventBox->RecObjectsInGroup[groups[g]][i];
        if (IsRelevantRecObject(event, *recObject))        
          if (sel.IsRelevantRecObjectForSystematic(event,recObject,_index))
            box.RelevantRecObjectsSet.insert(recObject);
      }
    }
  }
    
  if (groups) delete [] groups;
}

