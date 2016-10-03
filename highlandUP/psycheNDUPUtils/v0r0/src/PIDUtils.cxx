#include "PIDUtils.hxx"
#include "CutUtils.hxx"
#include "VersioningUtils.hxx"
#include "TRandom.h"
#include "Parameters.hxx"
#include "KinematicsUtils.hxx"
//#include "Units.h"
const unsigned int NMAXPARTICLESWITHTPC = NMAXPARTICLES;

//********************************************************************
void anaUtils::RecomputeTPCPulls(AnaTPCParticleB &track) {
//********************************************************************

    // TODO
    (void)track;

    /*
       track.Pullmu  = ComputeTPCPull(track,"muon");
       track.Pullp   = ComputeTPCPull(track,"proton");
       track.Pullele = ComputeTPCPull(track,"electron");
       track.Pullpi  = ComputeTPCPull(track,"pion");
       track.Pullk   = ComputeTPCPull(track,"kaon");
       */
}

//********************************************************************
void anaUtils::ComputeTPCPull( AnaTPCParticleB &track, const AnaTrackB& track_) {
//********************************************************************
    track.dEdxMeas = RecondEdx(track, track_);
    track.dEdxexpMuon = ExpecteddEdx(track, ParticleId::kMuon);
    track.dEdxexpEle = ExpecteddEdx(track, ParticleId::kElectron);
    track.dEdxexpPion = ExpecteddEdx(track, ParticleId::kPionPlus);
    track.dEdxexpProton = ExpecteddEdx(track, ParticleId::kProton);
    track.dEdxSigmaMuon =TMath::Sqrt(pow( ComputedEdxSigma(track, track_.GetTrueParticle()->PDG),2)+pow(TrackMomError(track.Momentum,track.MomentumError,ParticleId::kMuon),2.0))*track.dEdxMeas;
    track.dEdxSigmaEle =TMath::Sqrt(pow( ComputedEdxSigma(track, track_.GetTrueParticle()->PDG),2)+pow(TrackMomError(track.Momentum,track.MomentumError,ParticleId::kElectron),2.0))*track.dEdxMeas;
    track.dEdxSigmaPion =TMath::Sqrt(pow( ComputedEdxSigma(track, track_.GetTrueParticle()->PDG),2)+pow(TrackMomError(track.Momentum,track.MomentumError,ParticleId::kPionPlus),2.0))*track.dEdxMeas;
    track.dEdxSigmaProton = TMath::Sqrt(pow( ComputedEdxSigma(track, track_.GetTrueParticle()->PDG),2)+pow(TrackMomError(track.Momentum,track.MomentumError,ParticleId::kProton),2.0))*track.dEdxMeas;

}


//********************************************************************
void anaUtils::ComputeTPCPull( AnaDetCrossingB &track, const AnaTrueParticleB& track_, Float_t* pulls) {
//********************************************************************
  double sigmap=0;
    double momentum =anaUtils::SmearMomentum( track, track_.PDG,sigmap);

    float dEdxMeas = RecondEdx(track, track_,momentum);
    float  dEdxexpMuon = ExpecteddEdx(momentum, ParticleId::kMuon);
    float dEdxexpEle = ExpecteddEdx(momentum, ParticleId::kElectron);
    float dEdxexpPion = ExpecteddEdx(momentum, ParticleId::kPionPlus);
    float dEdxexpProton = ExpecteddEdx(momentum, ParticleId::kProton);

    float dEdxSigmaMuon = TMath::Sqrt(pow( ComputedEdxSigma(track, track_.PDG), 2) + pow(TrackMomError(momentum, sigmap, ParticleId::kMuon), 2.0))*dEdxMeas;
    float dEdxSigmaEle  = TMath::Sqrt(pow( ComputedEdxSigma(track, track_.PDG), 2) + pow(TrackMomError(momentum, sigmap, ParticleId::kElectron), 2.0))*dEdxMeas;
    float dEdxSigmaPion = TMath::Sqrt(pow( ComputedEdxSigma(track, track_.PDG), 2) + pow(TrackMomError(momentum, sigmap, ParticleId::kPionPlus), 2.0))*dEdxMeas;
    float dEdxSigmaProton  = TMath::Sqrt(pow( ComputedEdxSigma(track, track_.PDG), 2) + pow(TrackMomError(momentum,sigmap, ParticleId::kProton), 2.0))*dEdxMeas;

    pulls[0] = (dEdxMeas - dEdxexpMuon) / dEdxSigmaMuon;
    pulls[1] = (dEdxMeas - dEdxexpEle) /dEdxSigmaEle;
    pulls[2] = (dEdxMeas - dEdxexpProton) / dEdxSigmaProton;
    pulls[3] = (dEdxMeas - dEdxexpPion) / dEdxSigmaPion;


}



//********************************************************************
void anaUtils::ComputeTPCPull( AnaTPCParticleB &track, Float_t * pulls) {
//********************************************************************

    pulls[0] = (track.dEdxMeas - track.dEdxexpMuon)/ track.dEdxSigmaMuon;
    pulls[1] = (track.dEdxMeas - track.dEdxexpEle) / track.dEdxSigmaEle;
    pulls[2] = (track.dEdxMeas - track.dEdxexpProton)/ track.dEdxSigmaProton;
    pulls[3] = (track.dEdxMeas - track.dEdxexpPion) / track.dEdxSigmaPion;
}

Float_t anaUtils::ComputedEdxSigma(AnaTPCParticleB &track, int PDG) {

    return  0.08 *sqrt(720/track.DeltaLYZ);
}

Float_t anaUtils::ComputedEdxSigma(AnaDetCrossingB &track, int PDG) {
    return  0.08* sqrt(720/track.DeltaLYZ);
}
//********************************************************************
Float_t anaUtils::RecondEdx( AnaTPCParticleB &track,  const AnaTrackB& track_) {
//********************************************************************

    Int_t PDG = abs(track_.GetTrueParticle()->PDG);
    double dedxexp=0.;
    if (PDG==13) 

        dedxexp =  ExpecteddEdx(track.SmearedMomentum, ParticleId::kMuon);
    else if (PDG==211)
        dedxexp =  ExpecteddEdx(track.SmearedMomentum, ParticleId::kPiPos);
    else if (PDG==-211)
        dedxexp =  ExpecteddEdx(track.SmearedMomentum, ParticleId::kPiNeg);
    else if (PDG==11)
        dedxexp =  ExpecteddEdx(track.SmearedMomentum, ParticleId::kElectron);
    else if (PDG==2212)
        dedxexp =  ExpecteddEdx(track.SmearedMomentum, ParticleId::kProton);

    

    double sigmadedx = ComputedEdxSigma(track, PDG)*dedxexp;
    double dedxsmeared = gRandom->Gaus(dedxexp, sigmadedx);

    return dedxsmeared;
}

//********************************************************************
Float_t anaUtils::RecondEdx( AnaDetCrossingB &track, const AnaTrueParticleB& track_,double momentum) {
//********************************************************************

    Int_t PDG = abs(track_.PDG);

 double dedxexp=0.;
    if (PDG==13) 

        dedxexp =  ExpecteddEdx(momentum, ParticleId::kMuon);
    else if (PDG==211)
        dedxexp =  ExpecteddEdx(momentum, ParticleId::kPiPos);
    else if (PDG==-211)
        dedxexp =  ExpecteddEdx(momentum, ParticleId::kPiNeg);

    else if (PDG==11)
        dedxexp =  ExpecteddEdx(momentum, ParticleId::kElectron);
    else if (PDG==2212)
        dedxexp =  ExpecteddEdx(momentum, ParticleId::kProton);

    
    double sigmadedx = ComputedEdxSigma(track, PDG)*dedxexp;

    double dedxsmeared = gRandom->Gaus(dedxexp, sigmadedx);

    return dedxsmeared;

}



//************************************************************
Double_t anaUtils::TrackMomError(Double_t momentum, Double_t momerr, ParticleId::ParticleEnum partID) {
//************************************************************
    double bgless, bgmore, dedxless, dedxmore;
    double epsilon = 2;
    double Mparticle =anaUtils::GetParticleMass(partID);
    if (momentum < 6)
        bgless = 1; ///protect bgless from being negative
    else
        bgless = (momentum - epsilon) / Mparticle;

    bgmore = (momentum + epsilon) / Mparticle;

    dedxless = ExpecteddEdx(momentum - epsilon, partID);
    dedxmore = ExpecteddEdx(momentum + epsilon, partID);

    return TMath::Abs((dedxless - dedxmore) / (bgless - bgmore) * momerr / Mparticle);

}

//********************************************************************
Float_t anaUtils::ExpecteddEdx(const AnaParticleMomB& part, ParticleId::ParticleEnum partID) {
//********************************************************************

    return ExpecteddEdx(part.SmearedMomentum, partID);
}

//********************************************************************
Float_t anaUtils::ExpecteddEdx(Float_t mom, ParticleId::ParticleEnum partID) {
//********************************************************************

    // for production 5
    Float_t ExpecteddEP0 = 149.4;
    Float_t ExpecteddEP1 = 2.765;
    Float_t ExpecteddEP2 = 0.103;
    Float_t ExpecteddEP3 = 2.052;
    Float_t ExpecteddEP4 = 0.5104;

  /* Float_t ExpecteddEP0 = 2.57616;
    Float_t ExpecteddEP1 = 0.226844;
    Float_t ExpecteddEP2 = -0.765909;
    Float_t ExpecteddEP3 = 0.857942;
    Float_t ExpecteddEP4 = 0.0138356;*/
/*    fa1->SetParameter(0, 2.51547);
    fa1->SetParameter(1, 0.149169);
    fa1->SetParameter(2, -0.746853);
    fa1->SetParameter(3, 0.820637);
    fa1->SetParameter(4, 0.018912);
*/
   /* if (versionUtils::prod6_corrections) {
        // for production 6
        ExpecteddEP0 = 53.87;
        ExpecteddEP1 = 5.551;
        ExpecteddEP2 = 0.001913;
        ExpecteddEP3 = 2.283;
        ExpecteddEP4 = 1.249;
    }*/
    Float_t mass = anaUtils::GetParticleMass(partID);
    if (mass < 0) {
        std::cout << "Tried to compute dEdx for invalid particle hypothesis" << std::endl;
        return -10000;
    }
    Float_t bg = mom / mass;
    Float_t beta = bg / sqrt(1. + bg * bg);
    Float_t func = ExpecteddEP1 - pow(beta, ExpecteddEP3) - log(ExpecteddEP2 + 1. / pow(bg, ExpecteddEP4));
    func = func * ExpecteddEP0 / pow(beta, ExpecteddEP3);

    return func;
}

double anaUtils::Sigma( AnaDetCrossingB &track,int pdg){
double LYZ=track.DeltaLYZ*units::mm;
double N=track.DeltaLYZ/ units::mm;
double mass=0.;

  if     (pdg==211)  mass = 139.570; // pi+-
  else if(pdg==13)   mass = 105.658; // muon
  else if(pdg==2212) mass = 938.272; // proton
  else if(pdg==11)   mass = 0.511;   // electron

double bg=anaUtils::GetEntranceMomentum(track)/mass;
double betta=bg / sqrt(1. + bg * bg);
double sigma_x= ND::params().GetParameterD("psycheNDUPUtils.mom_corrections.sigma_x");
double B= ND::params().GetParameterD("psycheNDUPUtils.mom_corrections.B");
double X0= ND::params().GetParameterD("psycheNDUPUtils.mom_corrections.x0");
double p_iv=anaUtils::ComputeInversePT(track)*units::GeV;
double sigma_inv_p=sqrt(TMath::Power(1./(0.3*B*LYZ*LYZ)*sigma_x/p_iv*sqrt(720/(N+4)),2)+TMath::Power(0.2/(B*sqrt(LYZ*X0)*betta),2));
return sigma_inv_p;
}

//********************************************************************
double anaUtils::SmearMomentum(  AnaDetCrossingB &track, int pdg,double sigma){
//********************************************************************
  sigma=Sigma( track,pdg)*anaUtils::GetEntranceMomentum(track);

  double prob=gRandom->Gaus(anaUtils::GetEntranceMomentum(track),sigma);
  return prob;
}

/*s
//********************************************************************
void anaUtils::GetPIDLikelihood(const AnaTrackB& track, Float_t* hypo) {
//********************************************************************

    UInt_t itrk = track.Index;

    if ( itrk >= NMAXPARTICLESWITHTPC ) return; // Protection against values out of the vector.

    Double_t prob[4] = {1, 1, 1, 1};
    Double_t tmp_prob[3][4];
    Double_t total_prob = 0;
    bool found = false;

    AnaTPCParticleB* segmentsInTPC[3];
    for (int i = 0; i < 3; ++i) {
        segmentsInTPC[i] = NULL;
        for (Int_t j = 0; j < 4; j++) {
            hypo[j] = -1;
            tmp_prob[i][j] = 1;
        }
    }
    // Get the closest Subdet2. We should make sure that at least the segment in that Subdet2 has the proper PID info
    SubDetId::SubDetEnum closesttpc = anaUtils::GetClosestTPC(track);

    // Loop over Subdet2 segments
    for (int j = 0; j < track.nTPCSegments; ++j) {
        AnaTPCParticleB* TPCSegment = track.TPCSegments[j];
        if (!TPCSegment) continue;

        // Only segments passing the Subdet2 track quality cut will contribute to the likelihood
        // Was not applied for production 5 BANFF analysis
        //        if(!prod5Cut) if (!cutUtils::Subdet2TrackQualityCut(*Subdet2Segment)) continue;

        // Require valid values for all quantities involved
        if ( TPCSegment->dEdxexpMuon == -0xABCDEF || TPCSegment->dEdxexpEle == -0xABCDEF || TPCSegment->dEdxexpPion == -0xABCDEF || TPCSegment->dEdxexpProton == -0xABCDEF) continue;
        if ( TPCSegment->dEdxMeas == -0xABCDEF ) continue;
        if ( TPCSegment->dEdxexpMuon == -99999 || TPCSegment->dEdxexpEle == -99999 || TPCSegment->dEdxexpPion == -99999 || TPCSegment->dEdxexpProton == -99999) continue;
        if ( TPCSegment->dEdxMeas == -99999 ) continue;

        Float_t pulls[4];
        // Pulls in order: Muon, Electron, Proton, Pion
        anaUtils::ComputeTPCPull(*TPCSegment, pulls);
        Float_t pullmu  = pulls[0];
        Float_t pullp   = pulls[2];
        Float_t pullele = pulls[1];
        Float_t pullpi  = pulls[3];

        if (!TMath::Finite(pullmu) || !TMath::Finite(pullele) || !TMath::Finite(pullp) || !TMath::Finite(pullpi)) continue;
        if (pullmu  != pullmu || pullele != pullele || pullp   != pullp || pullpi  != pullpi) continue;

        SubDetId::SubDetEnum det = SubDetId::GetSubdetectorEnum(TPCSegment->Detector);

        /*  // To avoid mismatching between FlatTree and oaAnalysis we allow only one segment per TPC to be included in the likelihood, the one with more nodes
          if (segmentsInTPC[det-2]){
              if (TPCSegment->NHits > segmentsInTPC[det-2]->NHits){
                  segmentsInTPC[det-2] = TPCSegment;
                  tmp_prob[det-2][0] = exp(-(pullmu*pullmu)/2);
                  tmp_prob[det-2][1] = exp(-(pullele*pullele)/2);
                  tmp_prob[det-2][2] = exp(-(pullp*pullp)/2);
                  tmp_prob[det-2][3] = exp(-(pullpi*pullpi)/2);
              }
          }
          else{
              segmentsInTPC[det-2] = TPCSegment;
              tmp_prob[det-2][0] = exp(-(pullmu*pullmu)/2);
              tmp_prob[det-2][1] = exp(-(pullele*pullele)/2);
              tmp_prob[det-2][2] = exp(-(pullp*pullp)/2);
              tmp_prob[det-2][3] = exp(-(pullpi*pullpi)/2);
          }*/
  /*  }

    // Loop over all segments contributing to the likelihood and compute it
    for (int tpc = 0; tpc < 3; tpc++) {
        if (segmentsInTPC[tpc]) {
            // The pull should be already corrected by all corrections (CT and CT expected)
            prob[0] *= tmp_prob[tpc][0];
            prob[1] *= tmp_prob[tpc][1];
            prob[2] *= tmp_prob[tpc][2];
            prob[3] *= tmp_prob[tpc][3];

            if (SubDetId::GetDetectorUsed(segmentsInTPC[tpc]->Detector, closesttpc)) found = true;
        }
    }

    // If at least the segment in the closest TPC has a  valid PID info
    if (found) {
        for (int h = 0; h < 4; h++) {
            total_prob += prob[h] ;
        }

        if (total_prob > 0) {
            for (int h = 0; h < 4; h++) {
                hypo[h] = prob[h] / total_prob ;
            }
        }
    }
    return;
}

//********************************************************************
Float_t anaUtils::GetPIDLikelihood(const AnaTrackB& track, Int_t hypo) {
//********************************************************************

    if ( hypo >= 4 ) return -1.e+6;

    Float_t Likelihood[4];
    GetPIDLikelihood(track, Likelihood);
    return Likelihood[hypo];
}

//********************************************************************
Float_t anaUtils::GetPIDLikelihoodMIP(const AnaTrackB &track) {
//********************************************************************

    Float_t Likelihood[4];
    GetPIDLikelihood(track, Likelihood);

    Float_t likemu = Likelihood[0];
    Float_t likepi = Likelihood[3];
    Float_t likep  = Likelihood[2];

    return (likemu + likepi) / (1 - likep);
}

//********************************************************************
Float_t anaUtils::GetPIDPrior(const AnaTrackB& track, Int_t hypo) {
//********************************************************************

    // This function is not used yet

    Float_t xbins[18] = {0., 100., 200., 300., 400., 500., 600., 700, 800, 1000., 1200., 1400., 1700, 2000., 2500., 3000., 4000., 5000.};

    Float_t eprior[17] = {800., 250., 100,  40,    30,  25,   10,    5,    0,   0,     0,     0,     0,     0,     0,     0,     0};

    for (Int_t i = 0; i < 17; i++) {
        eprior[i] /= 400.;
    }

    Int_t pbin = 16;
    for (Int_t i = 0; i < 17; i++) {
        pbin = i - 1;
        if (track.Momentum > 0 && track.Momentum < xbins[i]) break;
    }


    if (hypo == 1)  return eprior[pbin];
    else return 1.;

}
*/
