#include "ND280SFGDEvent.hh"

//********************************************************************
void ND280SFGDEvent::DrawTracks(){
//********************************************************************ç

    for(std::size_t itrk=0;itrk<fTracks.size();++itrk){
        if(fTracks.size() == 1) fTracks[0]->DrawVoxels(kTRUE,kFALSE,1,0);  //mode 0 == first and print
        else{
            if(itrk == 0) fTracks[itrk]->DrawVoxels(kTRUE,kFALSE,itrk+1,3);    //mode 3 == first but not print
            else if(itrk+1 == fTracks.size()) fTracks[itrk]->DrawVoxels(kTRUE,kFALSE,itrk+1,2);// mode 2 == not first and print
            else fTracks[itrk]->DrawVoxels(kTRUE,kFALSE,itrk+1,1);              // mode 1 == not first, not print
        }
    }
}


//********************************************************************
void ND280SFGDEvent::MergeHits(){
//********************************************************************ç

    std::vector <ND280SFGDHit*> listOfMergedHits;
    std::vector <int> analyzedHits(fHits.size(),0);

    std::cout << "Original # of hits: " << fHits.size() << std::endl;

    for(uint ihit=0; ihit<fHits.size(); ihit++){
        if(analyzedHits[ihit]) continue;
        listOfMergedHits.push_back(fHits[ihit]);
        analyzedHits[ihit]=1;
        for(uint jhit=0; jhit<fHits.size(); jhit++){
            if(analyzedHits[jhit]) continue;
            if(fHits[ihit]->GetView() == fHits[jhit]->GetView() && fHits[ihit]->GetX() == fHits[jhit]->GetX() && fHits[ihit]->GetY() == fHits[jhit]->GetY() && fHits[ihit]->GetZ() == fHits[jhit]->GetZ() ){
                analyzedHits[jhit] = 1;
                fHits[ihit]->SetCharge( fHits[ihit]->GetCharge() + fHits[jhit]->GetCharge() );
            }
        }
    }

    std::cout << "# of merged hits: " << listOfMergedHits.size() << std::endl;
    
    // we need to clean 0PE hits:
    std::vector <ND280SFGDHit*> listOfNonZeroHits;
    for(uint ihit=0; ihit<listOfMergedHits.size(); ihit++){
        if(listOfMergedHits[ihit]->GetCharge()>0) listOfNonZeroHits.push_back(listOfMergedHits[ihit]);
    }

    std::cout << "# of merged nonZero hits: " << listOfNonZeroHits.size() << std::endl;

    //clean old pointers:

    for(uint ihit=0; ihit<fHits.size(); ihit++){
        bool found = false;
        for(uint jhit=0; jhit<listOfNonZeroHits.size(); jhit++){
            if(fHits[ihit] == listOfNonZeroHits[jhit]) found = true;
        }
        if(!found) delete fHits[ihit];
    }

    fHits = listOfNonZeroHits;
}
