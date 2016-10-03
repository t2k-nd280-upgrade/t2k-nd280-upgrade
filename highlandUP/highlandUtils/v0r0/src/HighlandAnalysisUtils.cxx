#include "HighlandAnalysisUtils.hxx"
#include <TMath.h>
#include <math.h>



//*********************************************************
AnaVertexB* anaUtils::GetGlobalVertex(const AnaEventB& event, AnaTrackB** Tracks, int nTracks){
//*********************************************************
  AnaVertexB* vertex = NULL;
  
  // Loop over vertices available in the event
  for (int i = 0; i < event.nVertices; i++){
    
    if (!event.Vertices[i]) continue;
    
    int nMatch_tmp  = 0;
    int nTracks_tmp = 0;
    bool tracks_loop = false;
    
    // Loop through vertex constituents
    for (int j = 0; j < event.Vertices[i]->nParticles; j++){
      
      if (!event.Vertices[i]->Particles[j]) continue;
      
      // Loop through input tracks
      for (int k = 0; k < nTracks; k++){
        if (!Tracks[k]) continue;
        
        // Count valid tracks
        if (!tracks_loop) nTracks_tmp++; 
        
        // Check whether have the same objects, check UniqueID just in case some copying took place 
        // (probably redundunt)
        if (event.Vertices[i]->Particles[j]->UniqueID == Tracks[k]->UniqueID)
          nMatch_tmp++;
      }
      
      tracks_loop = true; 
    }
    
    // Check whether all valid tracks provided belong to the vertex
    if (nTracks_tmp == nMatch_tmp){ 
      vertex = event.Vertices[i];
      break;
    }
  }
  
  return vertex;

}
