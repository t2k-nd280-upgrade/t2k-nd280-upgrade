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
