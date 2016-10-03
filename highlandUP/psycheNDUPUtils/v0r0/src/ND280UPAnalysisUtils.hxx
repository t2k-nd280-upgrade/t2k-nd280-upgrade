#ifndef ND280UPAnalysisUtils_h
#define ND280UPAnalysisUtils_h

#include "AnalysisUtils.hxx"

#include "ConstituentsUtils.hxx"
#include "PIDUtils.hxx"
#include "TruthUtils.hxx"
#include "VersioningUtils.hxx"
#include "Header.hxx"
#include "DataClasses.hxx"

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
}
#endif

//  LocalWords:  ifndef
