#ifndef ND280UPAnalysisUtils_h
#define ND280UPAnalysisUtils_h

#include "AnalysisUtils.hxx"

#include "ConstituentsUtils.hxx"
#include "PIDUtils.hxx"
#include "TruthUtils.hxx"
#include "VersioningUtils.hxx"
#include "Header.hxx"
#include "DataClasses.hxx"

#include <TRandom3.h>

/// This namespace contains useful functions for analyses. Functions include
/// those related to fiducial volumes, accessing tracks with specific
/// characteristics, and more.
namespace anaUtils{

  //----- Utility functions -------------

  /// Return a scale factor for the magnet intensity, based on the ND280 run number.
  /// THIS IS CURRENTLY DISABLED AND WILL ALWAYS RETURN 1!
  Float_t MagnetIntensityCorrection(Int_t Run);
  
  /// Whether the ECal object is "contained" within the ECal volume. This is
  /// intended for deciding whether to trust the ECal EM energy or not.
  /// At the moment this is based on the start position of the shower. It will
  /// be updated to a more rigorous definition when production 5F files have been
  /// studied more closely.
  //  bool IsEcalContained(AnaEcalTrackEcalPID* EcalSegment);
  
  /// Returns the run period (sequentially: 0,1,2,3,4,5 ...)
  int GetRunPeriod(int run);
  
  /// Returns the sans muon modes (sequentially: 0,1,2)
  int GetSandMode(int run);
    
  /// Method to load external ipon cross section data into arrays
  template<int nIntBins, int nMomBins>
  void LoadPionCrossSections(char* inputFileName, Float_t (&xsec_array)[nIntBins][nMomBins], Float_t (&err_array)[nIntBins][nMomBins]){
    ifstream inputFile(inputFileName, std::ios::in);
    
    if (!inputFile) {
      std::cerr << "Cannot open input file '" << inputFileName << "'. Exit!" << std::endl;
      exit(0);
    }
    
    std::string ctype, cintBin, cvalue;
    char* pEnd;
    while (inputFile >> ctype >> cintBin) {
      int intBin = strtod(cintBin.c_str(),&pEnd);
      int type = strtod(ctype.c_str(),&pEnd);
      
      if(intBin >= nIntBins) std::cout << "Error - interaction bin larger than input array size" << std::endl;
      
      for (int momBin = 0;momBin<nMomBins;momBin++){
        inputFile >> cvalue;
        Float_t value = strtod(cvalue.c_str(),&pEnd);
        if (type==0){
          xsec_array[intBin][momBin] =  value;
        }
        else
          err_array[intBin][momBin] =  value;
      }
    }
    
    inputFile.close();
  }
  Float_t ComputeInversePT( AnaDetCrossingB &cross, bool entrance = true);
  Float_t ComputeInversePT(AnaTPCParticleB& track);

  void IncrementPOTBySpill(const AnaSpillB& spill, Header& header);
 // void Fill_Tracks_Recon_From_True(AnaTrueParticleB* trueParticles, AnaTrack* reconParticle);

  /// Get ToF based on the maximum t/dt
  Float_t GetToF(const AnaTrackB* track, AnaParticleB*& seg1, AnaParticleB*& seg2, Float_t& sigma, bool UseSmearing = true);

  /// Get the length between two segments for ToF calculations
  Float_t GetLength(AnaTrackB* track, AnaParticleB*& p1, AnaParticleB*& p2, Float_t& sigma_length, bool UseSmearing = true);

  /// Calculate ToF mass based on ToF and length
  Float_t ComputeToFMass(Float_t mom, Float_t ToF, Float_t length);

  /// Calculate ToF based on mass and length
  Float_t ComputeToFTime(Float_t mom, Float_t mass, Float_t length); 

  /// Get detectors used for ToF calculations
  bool GetToFdetectors(AnaParticleB*& seg1, AnaParticleB*& seg2, SubDetId::SubDetEnum& det1, SubDetId::SubDetEnum& det2);

  /// Calculate the detectors used for ToF
  bool GetToFdetectors(AnaTrackB* track, SubDetId::SubDetEnum& det1, SubDetId::SubDetEnum& det2, bool UseSmearing = true);

  /// Calculate ToF Llh for the track
  bool GetToFLikelihood(AnaTrackB* track, Float_t* ToF_lkl, bool UseSmearing = true);

  /// Get Llh for given ToF and length
  bool GetToFLikelihood(Float_t mom, Float_t mom_err, Float_t ToF, Float_t sigma, Float_t length, Float_t sigma_length, Float_t* ToF_lkl, bool UseSmearing = true);

  /// Calculate Llh from pulls
  bool CalculateToFLikelihood(Float_t* ToF_pull, Float_t* ToF_lkl);

  /// Get Llh for track for certain hypo
  Float_t GetToFLikelihood(AnaTrackB* track, Int_t hypo, bool UseSmearing = true);

  /// Get Llh  for track for certain hypo from ToF and length
  Float_t GetToFLikelihood(Float_t mom, Float_t mom_err, Float_t ToF, Float_t sigma, Float_t length, Float_t sigma_length, Int_t hypo, bool UseSmearing = true);

  /// Compute pulls based on the track or ToF&length
  bool ComputeToFpulls(AnaTrackB* track, Float_t* ToF_pull, bool UseSmearing = true);
  bool ComputeToFpulls(Float_t mom, Float_t mom_err, Float_t ToF, Float_t sigma, Float_t length, Float_t sigma_length, Float_t* ToF_pull, bool UseSmearing = true);

  void FlipTrack(AnaTrack* track);
  void FlipTrack(AnaParticleB* track);

}
#endif

//  LocalWords:  ifndef
