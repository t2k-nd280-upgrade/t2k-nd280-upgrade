#include "HighlandAnalysisUtils.hxx"
#include <stdio.h>
#include <math.h>

const unsigned int NMAXPARTICLESWITHTPC = NMAXPARTICLES;
/*
//********************************************************************
void anaUtils::ComputeTPCPullIncludingKaon(const AnaTPCParticleB &trackB, Float_t* pulls) {
//********************************************************************

  // cast the track
  const AnaTPCParticleB& track = *static_cast<const AnaTPCParticleB*>(&trackB);

  // compute pulls for the standard hypotheses using the method in psyche utils
  anaUtils::ComputeTPCPull(track, pulls);
  
  // compute pull for kaon hypothesis
//  pulls[4] = ((track.dEdxMeas - track.dEdxexpKaon) / track.dEdxSigmaKaon);
}
/*
//********************************************************************
void anaUtils::GetPIDLikelihoodIncludingKaon(const AnaTrackB& track, Float_t* hypo) {
//********************************************************************

    UInt_t itrk = track.Index;

    if( itrk >= NMAXPARTICLESWITHTPC ) return; // Protection against values out of the vector. 

    Double_t prob[5]={1,1,1,1,1};
    Double_t tmp_prob[3][5];
    Double_t total_prob=0;
    bool found=false;

    AnaTPCParticleB* segmentsInTPC[3];
    for(int i = 0; i < 3; ++i){
        segmentsInTPC[i] = NULL;
        for (Int_t j=0;j<5;j++){
            hypo[j]=-1;
            tmp_prob[i][j] = 1;
        }
    }
    // Get the closest TPC. We should make sure that at least the segment in that TPC has the proper PID info
    SubDetId::SubDetEnum closesttpc = anaUtils::GetClosestTPC(track);

    // Loop over TPC segments
    for (int j = 0; j < track.nTPCSegments; ++j){
        AnaTPCParticleB* TPCSegment = track.TPCSegments[j];
        if (!TPCSegment) continue;

        // Only segments passing the TPC track quality cut will contribute to the likelihood
        // Was not applied for production 5 BANFF analysis
        //        if(!prod5Cut) if (!cutUtils::TPCTrackQualityCut(*TPCSegment)) continue;
        
        // Require valid values for all quantities involved
        if( TPCSegment->dEdxexpMuon==-0xABCDEF || TPCSegment->dEdxexpEle==-0xABCDEF || TPCSegment->dEdxexpPion==-0xABCDEF || TPCSegment->dEdxexpProton==-0xABCDEF) continue;
        if( TPCSegment->dEdxMeas ==-0xABCDEF ) continue; 
        if( TPCSegment->dEdxexpMuon==-99999 || TPCSegment->dEdxexpEle==-99999 || TPCSegment->dEdxexpPion==-99999 || TPCSegment->dEdxexpProton==-99999) continue;
        if( TPCSegment->dEdxMeas ==-99999 ) continue; 
        
        // Require valid values for kaon quantities (these are not yet in psyche)
        AnaTPCParticleB* TPCSegmentK = (AnaTPCParticleB*)TPCSegment;
        if( TPCSegmentK->dEdxexpKaon==-0xABCDEF || TPCSegmentK->dEdxMeas ==-0xABCDEF ) continue;
        if( TPCSegmentK->dEdxexpKaon==-99999    || TPCSegmentK->dEdxMeas ==-99999    ) continue;
        
        Float_t pulls[5];
        // Pulls in order: Muon, Electron, Proton, Pion, Kaon
        anaUtils::ComputeTPCPullIncludingKaon(*TPCSegment,pulls);
        Float_t pullmu  = pulls[0];
        Float_t pullele = pulls[1];
        Float_t pullp   = pulls[2];
        Float_t pullpi  = pulls[3];
        Float_t pullka  = pulls[4];

        if (!TMath::Finite(pullmu) || !TMath::Finite(pullele) || !TMath::Finite(pullp) || !TMath::Finite(pullpi) || !TMath::Finite(pullka)) continue;
        if (pullmu != pullmu || pullele != pullele || pullp != pullp || pullpi != pullpi || pullka != pullka) continue;

        SubDetId::SubDetEnum det = SubDetId::GetSubdetectorEnum(TPCSegment->Detector);
        
        // To avoid mismatching between FlatTree and oaAnalysis we allow only one segment per TPC to be included in the likelihood, the one with more nodes
        if (segmentsInTPC[det-2]){
            if (TPCSegment->NHits > segmentsInTPC[det-2]->NHits){
                segmentsInTPC[det-2] = TPCSegment;
                tmp_prob[det-2][0] = exp(-(pullmu*pullmu)/2);
                tmp_prob[det-2][1] = exp(-(pullele*pullele)/2);
                tmp_prob[det-2][2] = exp(-(pullp*pullp)/2);
                tmp_prob[det-2][3] = exp(-(pullpi*pullpi)/2);
                tmp_prob[det-2][4] = exp(-(pullka*pullka)/2);
            }            
        }
        else{
            segmentsInTPC[det-2] = TPCSegment;      
            tmp_prob[det-2][0] = exp(-(pullmu*pullmu)/2);
            tmp_prob[det-2][1] = exp(-(pullele*pullele)/2);
            tmp_prob[det-2][2] = exp(-(pullp*pullp)/2);
            tmp_prob[det-2][3] = exp(-(pullpi*pullpi)/2);
            tmp_prob[det-2][4] = exp(-(pullka*pullka)/2);
        }
    }

    // Loop over all segments contributing to the likelihood and compute it
    for (int tpc=0;tpc<3;tpc++){
        if (segmentsInTPC[tpc]){
            // The pull should be already corrected by all corrections (CT and CT expected)
            prob[0] *= tmp_prob[tpc][0]; 
            prob[1] *= tmp_prob[tpc][1];
            prob[2] *= tmp_prob[tpc][2];
            prob[3] *= tmp_prob[tpc][3];
            prob[4] *= tmp_prob[tpc][4];

            if (SubDetId::GetDetectorUsed(segmentsInTPC[tpc]->Detector, closesttpc)) found = true;
        }
    }

    // If at least the segment in the closest TPC has a  valid PID info
    if (found){
        for (int h=0;h<5;h++){
            total_prob += prob[h];
        }

        if (total_prob>0){
            for (int h=0;h<5;h++){
                hypo[h] = prob[h]/total_prob;
            }
        }
    }
    return;
}

//********************************************************************
Float_t anaUtils::GetPIDLikelihoodIncludingKaon(const AnaTrackB& track, Int_t hypo) {
//********************************************************************

    if( hypo >= 5 ) return -1.e+6; 

    Float_t Likelihood[5];
    GetPIDLikelihoodIncludingKaon(track,Likelihood);
    return Likelihood[hypo];
}

*/