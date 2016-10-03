#include "EventWeightBase.hxx"
#include "MultiThread.hxx"


//********************************************************************
void EventWeightBase::InitializeEvent(const AnaEventC& event, const SelectionBase& sel, Int_t ibranch){
//********************************************************************

  Int_t uniqueID = 0;

#ifdef MULTITHREAD
  uniqueID = event.UniqueID;
#endif

  // Delete the SystBox when it exists and create a new one. 
  // TODO: It is probably faster to just reset the arrays

  // Get the selection index;
  Int_t isel=sel.GetEnabledIndex();  

  // Nothing to do when this box already created and filled
  if(_systBoxes[isel][ibranch][uniqueID]) return;

  // Create the box
  _systBoxes[isel][ibranch][uniqueID] = new SystBoxB();
  
  // Fill the SystBox with the relevant recObjects and true recObjects tor this systematic
  FillSystBox(event, sel, ibranch);
}

//********************************************************************
void EventWeightBase::FillSystBox(const AnaEventC& event, const SelectionBase& sel, Int_t ibranch){
//********************************************************************

  Int_t uniqueID = 0;

#ifdef MULTITHREAD
  uniqueID = event.UniqueID;
#endif

  // Get the selection index;
  Int_t isel=sel.GetEnabledIndex();  

  // Get the SystBox
  SystBoxB& box = *_systBoxes[isel][ibranch][uniqueID];

  // Get the EventBox for the input selection
  EventBoxB* EventBox = event.EventBoxes[sel.GetEventBoxId()];

  // Get the recObject groups relevant for this seystematic
  Int_t* groups;
  anaUtils::CreateArray(groups,10);
  Int_t nGroups = GetRelevantRecObjectGroups(sel, ibranch, groups);
  anaUtils::ResizeArray(groups,nGroups);

  if (nGroups>0){
    // compute the total number of objects in all groups
    Int_t nMaxRecObjects=0;
    for (Int_t g = 0; g<nGroups;g++)
      nMaxRecObjects += EventBox->nRecObjectsInGroup[groups[g]];

    // create the array of RelevantRecObjects
    if (box.RelevantRecObjects) delete [] box.RelevantRecObjects;
    anaUtils::CreateArray(box.RelevantRecObjects, nMaxRecObjects);
    box.nRelevantRecObjects=0;

    // Loop over groups and over recObjects in each group
    for (Int_t g = 0; g<nGroups;g++){
      for (Int_t i=0;i<EventBox->nRecObjectsInGroup[groups[g]];i++){
        AnaRecObjectC* recObject = EventBox->RecObjectsInGroup[groups[g]][i];

        // Is it relevant for this systematic ?
        if (IsRelevantRecObject(event, *recObject))    
          // Is it relevant for this selection and systematic ?    
          if (sel.IsRelevantRecObjectForSystematic(event,recObject,_index,ibranch))
            box.RelevantRecObjects[box.nRelevantRecObjects++]= recObject;
      }
    }
    // Resize the array once we know the number of objects
    if (box.nRelevantRecObjects != nMaxRecObjects)
      anaUtils::ResizeArray(box.RelevantRecObjects, box.nRelevantRecObjects);
  }

  // Delete the groups array
  if (groups) delete [] groups;

  // Fill the array of groups for true recObjects
  anaUtils::CreateArray(groups,10);
  Int_t nTrueGroups = GetRelevantTrueObjectGroups(sel, ibranch, groups);
  anaUtils::ResizeArray(groups,nTrueGroups);


  if (nTrueGroups>0){
    // compute the total number of objects in all groups
    Int_t nMaxRecObjects=0;
    for (Int_t g = 0; g<nTrueGroups;g++)
      nMaxRecObjects += EventBox->nTrueObjectsInGroup[groups[g]];

    // create the array of RelevantTrueObjects
    if (box.RelevantRecObjects) delete [] box.RelevantTrueObjects;
    anaUtils::CreateArray(box.RelevantTrueObjects, nMaxRecObjects);
    box.nRelevantTrueObjects=0;

    // Loop over groups and over true recObjects in each group
    for (Int_t g = 0; g<nTrueGroups;g++){
      for (Int_t i=0;i<EventBox->nTrueObjectsInGroup[groups[g]];i++){
        AnaTrueObjectC* trueRecObject = EventBox->TrueObjectsInGroup[groups[g]][i];

        // Is it relevant for this systematic ?        
        if (IsRelevantTrueObject(event, *trueRecObject))        
          // Is it relevant for this selection and systematic ?    
          if (sel.IsRelevantTrueObjectForSystematic(event,trueRecObject,_index,ibranch))        
            box.RelevantTrueObjects[box.nRelevantTrueObjects++]= trueRecObject;
      }
    }
    // Resize the array once we know the number of objects
    if (box.nRelevantTrueObjects != nMaxRecObjects)
      anaUtils::ResizeArray(box.RelevantTrueObjects, box.nRelevantTrueObjects);
  }

  // Delete the groups array
  if (groups) delete [] groups;


  // Fill the true-reco association. Only for some eff-like systematics
  FillTrueRecoAssociation(*_systBoxes[isel][ibranch][uniqueID]);

}

//********************************************************************
void EventWeightBase::FillTrueRecoAssociation(SystBoxB& box){
//********************************************************************
  if (box.RelevantTrueObjectsReco) delete [] box.RelevantTrueObjectsReco;
  
  // Create the box of true-rec association only for the first toy
  anaUtils::CreateArray(box.RelevantTrueObjectsReco, box.nRelevantTrueObjects);
  
  for (Int_t itrue=0;itrue< box.nRelevantTrueObjects; itrue++){      
    AnaTrueObjectC* trueRecObject = box.RelevantTrueObjects[itrue];
    
    box.RelevantTrueObjectsReco[itrue] = NULL;

    // Is there any reconstructed recObject associated to this true recObject ?
    for (Int_t irec=0;irec<(int) box.nRelevantRecObjects; irec++){
      AnaRecObjectC* recObject = box.RelevantRecObjects[irec];
      if (CheckTrueRecoAssociation(*trueRecObject, *recObject)){ 
        box.RelevantTrueObjectsReco[itrue] = recObject;
        break;
      }
    }
  }
}

//********************************************************************
bool EventWeightBase::CheckTrueRecoAssociation(const AnaTrueObjectC& trueObject, const AnaRecObjectC& recObject) const{
//********************************************************************

  // TrueObject must exist
  if (!recObject.TrueObject) return false;

  // This is the simplest true-reco association. This method can be overwritten by the derived classes
  if(trueObject.ID==recObject.TrueObject->ID) return true;
  return false;
}
