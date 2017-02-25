#ifndef FiducialVolumeDefinition_h
#define FiducialVolumeDefinition_h

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <typeinfo>
#include "TVector3.h"

namespace FVDef {

  //----- FiducialVolume volume definitions ---

  /// Fiducial volume cut for Subdet1_1. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd1min is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefminTPCUp1[3];

  /// Fiducial volume cut for Subdet1_1. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd1max is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefmaxTPCUp1[3];

  /// Fiducial volume cut for Subdet1_2. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd2min is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefminTPCUp2[3];

  /// Fiducial volume cut for Subdet1_2. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd2max is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefmaxTPCUp2[3];

  /// Fiducial volume cut for Subdet1_2. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd2min is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefminTPCDown1[3];

  /// Fiducial volume cut for Subdet1_2. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd2max is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefmaxTPCDown1[3];

  /// Fiducial volume cut for Subdet1_2. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd2min is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefminTPCDown2[3];

  /// Fiducial volume cut for Subdet1_2. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd2max is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefmaxTPCDown2[3];

    /// Fiducial volume cut for Subdet1_2. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd2min is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefminForwTPC1[3];

  /// Fiducial volume cut for Subdet1_2. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd2max is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefmaxForwTPC1[3];

    /// Fiducial volume cut for Subdet1_2. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd2min is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefminForwTPC2[3];

  /// Fiducial volume cut for Subdet1_2. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd2max is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefmaxForwTPC2[3];

    /// Fiducial volume cut for Subdet1_2. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd2min is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefminForwTPC3[3];

  /// Fiducial volume cut for Subdet1_2. See definition in StandardActions.cxx for
  /// default value. This is the amount by which DetDef::fgd2max is shrunk
  /// in the FiducialCut function.
  extern Float_t FVdefmaxForwTPC3[3];

  extern Float_t FVdefminTarget1[3];
  extern Float_t FVdefmaxTarget1[3];
  extern Float_t FVdefminTarget2[3];
  extern Float_t FVdefmaxTarget2[3];

  extern Float_t FVdefminFGD1[3];
  extern Float_t FVdefmaxFGD1[3];
  extern Float_t FVdefminFGD2[3];
  extern Float_t FVdefmaxFGD2[3];

  /// Dump Fiducial Volume definitions
  void DumpFV();
}

#endif
