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
    kTarget1NuMuCC,
    kTarget1NuMuCCQE,
    kTarget1NuMuCC0Pi,
    kTarget1NuMuCC1Pi,
    kTarget1NuMuCCOther,
    kTarget2NuMuCC,
    kTarget2NuMuCC0Pi,
    kTarget2NuMuCC1Pi,
    kTarget2NuMuCCOther,
    kTarget1NuECC,
    kTarget1NuECC0Pi,
    kTarget1NuECCOther,
    kTarget2NuECC,
    kTarget2NuECC0Pi,
    kTarget2NuECCOther,
    kTarget1AntiNuMuCC,
    kTarget1AntiNuMuCC1Track,
    kTarget1AntiNuMuCCNTracks,
    kTarget2AntiNuMuCC,
    kTarget2AntiNuMuCC1Track,
    kTarget2AntiNuMuCCNTracks,
    kTarget1NuMuBkgInAntiNuModeCC,
    kTarget1NuMuBkgInAntiNuModeCC1Track,
    kTarget1NuMuBkgInAntiNuModeCCNTracks,
    kTarget2NuMuBkgInAntiNuModeCC,
    kTarget2NuMuBkgInAntiNuModeCC1Track,
    kTarget2NuMuBkgInAntiNuModeCCNTracks,
    kTarget1NuMuCCBwd,
    kTarget2NuMuCCBwd,
    kTarget1AntiNuECC,
    kTarget1AntiNuECC0Pi,
    kTarget1AntiNuECCOther,
    kTarget2AntiNuECC,
    kTarget2AntiNuECC0Pi,
    kTarget2AntiNuECCOther,
    kTarget1Gamma,
    kTarget2Gamma,
    
    kNSamples
  };

  std::string ConvertSample(SampleEnum sample);
  std::string ConvertSampleToSelection(SampleEnum sample);
  
}

#endif
