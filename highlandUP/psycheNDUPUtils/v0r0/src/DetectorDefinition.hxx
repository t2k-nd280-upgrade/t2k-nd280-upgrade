#ifndef DetectorDefinition_h
#define DetectorDefinition_h

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <typeinfo>
#include "TVector3.h"

namespace DetDef {

  //----- Detector volume definitions ---

  /// Minimum of the SUBDET1_1 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t TPCUp1min[3];

  /// Maximum of the SUBDET1_1 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t TPCUp1max[3];

  /// Minimum of the SUBDET1_2 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t TPCUp2min[3];

  /// Maximum of the SUBDET1_2 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t TPCUp2max[3];



  /// Minimum of the SUBDET2_1 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t TPCDown1min[3];

  /// Maximum of the SUBDET2_1 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t TPCDown1max[3];

  /// Minimum of the SUBDET2_2 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t TPCDown2min[3];

  /// Maximum of the SUBDET2_2 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t TPCDown2max[3];

  /// Minimum of the SUBDET2_3 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t ForwTPC1min[3];

  /// Maximum of the SUBDET2_3 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t ForwTPC1max[3];
  
  /// Minimum of the SUBDET2_3 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t ForwTPC2min[3];

  /// Maximum of the SUBDET2_3 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t ForwTPC2max[3];
  
    /// Minimum of the SUBDET2_3 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t ForwTPC3min[3];

  /// Maximum of the SUBDET2_3 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t ForwTPC3max[3];

    /// Minimum of the SUBDET2_3 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t Target1min[3];

  /// Maximum of the SUBDET2_3 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t Target1max[3];

    /// Minimum of the SUBDET2_3 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t Target2min[3];

  /// Maximum of the SUBDET2_3 det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t Target2max[3];

  /// Dump volume definitions
  void DumpVolumes();  
}

#endif
