#include "BeamBunching.hxx"
#include <TVector3.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

//*****************************************************************************
void BeamBunching::ReadBeamBunchStructure(const std::string& filename){
//*****************************************************************************                 
  
  FILE *pFile = fopen (filename.c_str(),"r");
  
  if( pFile == NULL ) {
    std::cout << "Cannot open file Bunch File '" <<  filename << "'" << std::endl;;
    exit(1);
  }
  
  // Read the numbers of bunches from the first line in the file
  fscanf(pFile,"%d",&_nbunches);

  Int_t runini; 
  Int_t runfin;
  std::vector<Float_t> bunch(_nbunches);
  
  while ( fscanf(pFile,"%d%d%f%f%f%f%f%f%f%f",&runini, &runfin, &bunch[0], &bunch[1], &bunch[2], &bunch[3], &bunch[4], &bunch[5], &bunch[6], &bunch[7]) == (Int_t)_nbunches+2 ){

    if( runini == -1 ){ // This is MC
      bunch_time_mc = bunch; 
    }
    else {
      bunchrunperiod.push_back(std::pair<int,int>(runini,runfin)); 
      std::vector<Float_t> local; 
      bunch_time_data.push_back(bunch); 
    }

  }
            
  fclose(pFile); 
}

//*****************************************************************************
Int_t  BeamBunching::GetBunchPeriod(const AnaEventB& event){
//*****************************************************************************

  //define start time as latest "edge" of bunch time window
  Int_t bunchperiod = -1; 
  
  if( ! event.GetIsMC() ) {
    for(UInt_t j = 0 ; j < bunchrunperiod.size(); j++ ) {
      if( event.EventInfo.Run <= bunchrunperiod[j].second && event.EventInfo.Run >= bunchrunperiod[j].first ){
	bunchperiod = j; 
	break; 
      }
    }
  }

  return bunchperiod;
}

//*****************************************************************************
UInt_t BeamBunching::GetNBunches(const AnaEventB& event){
//*****************************************************************************

  if( event.GetIsMC() ) 
    return bunch_time_mc.size();
  else {
    Int_t bunchperiod = GetBunchPeriod(event);
    if( bunchperiod > -1 ) {
      return bunch_time_data[bunchperiod].size();
    }
  }

  return 0;
}

//*****************************************************************************
Float_t BeamBunching::GetBunchCentralTime(const AnaEventB& event, Int_t ibunch){
//*****************************************************************************

  Float_t selTime = -99999.; 

  if(event.GetIsMC()) 
    selTime = bunch_time_mc[ibunch];
  else {
    Int_t bunchperiod = GetBunchPeriod(event);
    if( bunchperiod > -1 ) {
      selTime = bunch_time_data[bunchperiod][ibunch];
    }
  }

  return selTime;
}

//*****************************************************************************
Int_t BeamBunching::GetBunch(Float_t tTrack, Int_t run, bool isMC, bool cosmic_mode, Float_t sigma, Float_t NSigmas) const {
//*****************************************************************************

  if (cosmic_mode) {
    // In cosmics mode, everything gets put into one bunch.
    return 0;
  }

  Int_t bunch = -1;
  //  Float_t sigma;
  Float_t time;

  // Find Bunch Period 
  
  Int_t bunchperiod = -1; 

  if( !isMC ) {
    for(UInt_t j = 0 ; j < bunchrunperiod.size(); j++ ) {
      if( run <= bunchrunperiod[j].second && run >= bunchrunperiod[j].first ){
	bunchperiod = j; 
	break; 
      }
    }
    if( bunchperiod < 0 ) return bunch; 
  }

  for (UInt_t i=0;i<_nbunches;i++){
 

    if (isMC){
      time = bunch_time_mc[i];
    }
    else{
      time = bunch_time_data[bunchperiod][i];
    }

    if( time < 0 ) return bunch;
    if (fabs(tTrack-time)<sigma*NSigmas){
      bunch = i;
      break;
    }
  }

  return bunch;
}

