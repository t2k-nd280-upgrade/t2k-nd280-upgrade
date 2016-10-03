#ifndef SampleId_hxx
#define SampleId_hxx

#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iostream>


/// Event sample - whether it passes the NuMu-CCQE selection, or CC1Pi selection for example
namespace SampleId{

  
  enum SampleEnum {
    kUnassigned,
    kFGD1NuMuCC,
    kFGD1NuMuCCQE,
    kFGD1NuMuCC0Pi,
    kFGD1NuMuCC1Pi,
    kFGD1NuMuCCOther,
    kFGD2NuMuCC,
    kFGD2NuMuCC0Pi,
    kFGD2NuMuCC1Pi,
    kFGD2NuMuCCOther,
    kFGD1NuECC,
    kFGD1NuECC0Pi,
    kFGD1NuECCOther,
    kFGD2NuECC,
    kFGD2NuECC0Pi,
    kFGD2NuECCOther,
    kFGD1AntiNuMuCC,
    kFGD1AntiNuMuCC1Track,
    kFGD1AntiNuMuCCNTracks,
    kFGD2AntiNuMuCC,
    kFGD2AntiNuMuCC1Track,
    kFGD2AntiNuMuCCNTracks,
    kFGD1NuMuBkgInAntiNuModeCC,
    kFGD1NuMuBkgInAntiNuModeCC1Track,
    kFGD1NuMuBkgInAntiNuModeCCNTracks,
    kFGD2NuMuBkgInAntiNuModeCC,
    kFGD2NuMuBkgInAntiNuModeCC1Track,
    kFGD2NuMuBkgInAntiNuModeCCNTracks,
    kFGD1NuMuCCBwd,
    kFGD2NuMuCCBwd,
    kFGD1AntiNuECC,
    kFGD1AntiNuECC0Pi,
    kFGD1AntiNuECCOther,
    kFGD2AntiNuECC,
    kFGD2AntiNuECC0Pi,
    kFGD2AntiNuECCOther,
    kFGD1Gamma,
    kFGD2Gamma,
    
    kNSamples
  };

  std::string ConvertSample(SampleEnum sample);
  std::string ConvertSampleToSelection(SampleEnum sample);
  
}

#endif
