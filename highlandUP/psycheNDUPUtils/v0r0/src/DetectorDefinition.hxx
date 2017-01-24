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
  /// Minimum of the DsECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t dsecalmin[3];

  /// Maximum of the DsECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t dsecalmax[3];

  /// Minimum of the Left BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalLmin[3];

  /// Maximum of the Left BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalLmax[3];

  /// Minimum of the Right BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalRmin[3];

  /// Maximum of the Right BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalRmax[3];

  /// Minimum of the Top BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalTLmin[3];

  /// Maximum of the Top BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalTLmax[3];

  /// Minimum of the Top BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalTRmin[3];

  /// Maximum of the Top BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalTRmax[3];

  /// Minimum of the Bottom BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalBLmin[3];

  /// Maximum of the Bottom BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalBLmax[3];

  /// Minimum of the Bottom BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalBRmin[3];

  /// Maximum of the Bottom BarrelECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t tecalBRmax[3];

  /// Minimum of the Left P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalLmin[3];

  /// Maximum of the Left P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalLmax[3];

  /// Minimum of the Right P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalRmin[3];

  /// Maximum of the Right P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalRmax[3];

  /// Minimum of the Top P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalTLmin[3];

  /// Maximum of the Top P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalTLmax[3];
  
  /// Minimum of the Top P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalTRmin[3];

  /// Maximum of the Top P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalTRmax[3];

  /// Minimum of the Bottom P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalBLmin[3];

  /// Maximum of the Bottom P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalBLmax[3];

  /// Minimum of the Bottom P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalBRmin[3];

  /// Maximum of the Bottom P0DECal det. See definition in AnalysisUtils.cxx for default value.
  extern Float_t pecalBRmax[3];

  /// Dump volume definitions
  void DumpVolumes();  
}

#endif
