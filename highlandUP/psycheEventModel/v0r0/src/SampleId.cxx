#include "SampleId.hxx"


//*********************************************************
std::string SampleId::ConvertSample(SampleEnum sample){
//*********************************************************

    std::string ssample="";

    if      (sample == kUnassigned) ssample      = "Unassigned";
    else if (sample == kTarget1NuMuCC) ssample      = "Target1 numuCC inclusive";
    else if (sample == kTarget1NuMuCCQE) ssample    = "Target1 numuCC QE";
    else if (sample == kTarget1NuMuCC0Pi) ssample   = "Target1 numuCC 0pi";
    else if (sample == kTarget1NuMuCC1Pi) ssample   = "Target1 numuCC 1pi";
    else if (sample == kTarget1NuMuCCOther) ssample = "Target1 numuCC other";
    else if (sample == kTarget2NuMuCC) ssample      = "Target2 numuCC inclusive";
    else if (sample == kTarget2NuMuCC0Pi) ssample   = "Target2 numuCC 0pi";
    else if (sample == kTarget2NuMuCC1Pi) ssample   = "Target2 numuCC 1pi";
    else if (sample == kTarget2NuMuCCOther) ssample = "Target2 numuCC other";
    else if (sample == kTarget1NuECC) ssample       = "Target1 nueCC inclusive";
    else if (sample == kTarget1NuECC0Pi) ssample    = "Target1 nueCC 0pi";
    else if (sample == kTarget1NuECCOther) ssample  = "Target1 nueCC other";
    else if (sample == kTarget2NuECC) ssample       = "Target2 nueCC inclusive";
    else if (sample == kTarget2NuECC0Pi) ssample    = "Target2 nueCC 0pi";
    else if (sample == kTarget2NuECCOther) ssample  = "Target2 nueCC other";
    else if (sample == kTarget1AntiNuMuCC) ssample  = "Target1 anti-numuCC inclusive";
    else if (sample == kTarget1AntiNuMuCC1Track) ssample  = "Target1 anti-numuCC QE";
    else if (sample == kTarget1AntiNuMuCCNTracks) ssample  = "Target1 anti-numuCC nQE";
    else if (sample == kTarget2AntiNuMuCC) ssample         = "Target2 anti-numuCC inclusive";
    else if (sample == kTarget2AntiNuMuCC1Track) ssample   = "Target2 anti-numuCC 1 track";
    else if (sample == kTarget2AntiNuMuCCNTracks) ssample  = "Target2 anti-numuCC N tracks";
    else if (sample == kTarget1NuMuBkgInAntiNuModeCC) ssample  = "Target1 NuMuBkg CC inclusive in AntiNu Mode";
    else if (sample == kTarget1NuMuBkgInAntiNuModeCC1Track) ssample  = "Target1 NuMuBkg CCQE in AntiNu Mode ";
    else if (sample == kTarget1NuMuBkgInAntiNuModeCCNTracks) ssample  = "Target1 NuMuBkg CCnQE in AntiNu Mode";
    else if (sample == kTarget2NuMuBkgInAntiNuModeCC) ssample  = "Target2 NuMuBkg CC inclusive in AntiNu Mode";
    else if (sample == kTarget2NuMuBkgInAntiNuModeCC1Track) ssample  = "Target2 NuMuBkg CCQE in AntiNu Mode ";
    else if (sample == kTarget2NuMuBkgInAntiNuModeCCNTracks) ssample  = "Target2 NuMuBkg CCnQE in AntiNu Mode";
    else if (sample == kTarget1AntiNuECC) ssample       = "Target1 anti-nueCC inclusive";
    else if (sample == kTarget1AntiNuECC0Pi) ssample    = "Target1 anti-nueCC 0pi";
    else if (sample == kTarget1AntiNuECCOther) ssample  = "Target1 anti-nueCC other";
    else if (sample == kTarget2AntiNuECC) ssample       = "Target2 anti-nueCC inclusive";
    else if (sample == kTarget2AntiNuECC0Pi) ssample    = "Target2 anti-nueCC 0pi";
    else if (sample == kTarget2AntiNuECCOther) ssample  = "Target2 anti-nueCC other";
    else if (sample == kTarget1Gamma) ssample           = "Target1 gamma";
    else if (sample == kTarget2Gamma) ssample           = "Target2 gamma";

    else if (sample == kFGD1NuMuCC) ssample      = "FGD1 numuCC inclusive";
    else if (sample == kFGD1NuMuCCQE) ssample    = "FGD1 numuCC QE";
    else if (sample == kFGD1NuMuCC0Pi) ssample   = "FGD1 numuCC 0pi";
    else if (sample == kFGD1NuMuCC1Pi) ssample   = "FGD1 numuCC 1pi";
    else if (sample == kFGD1NuMuCCOther) ssample = "FGD1 numuCC other";
    else if (sample == kFGD2NuMuCC) ssample      = "FGD2 numuCC inclusive";
    else if (sample == kFGD2NuMuCC0Pi) ssample   = "FGD2 numuCC 0pi";
    else if (sample == kFGD2NuMuCC1Pi) ssample   = "FGD2 numuCC 1pi";
    else if (sample == kFGD2NuMuCCOther) ssample = "FGD2 numuCC other";
    else if (sample == kFGD1NuECC) ssample       = "FGD1 nueCC inclusive";
    else if (sample == kFGD1NuECC0Pi) ssample    = "FGD1 nueCC 0pi";
    else if (sample == kFGD1NuECCOther) ssample  = "FGD1 nueCC other";
    else if (sample == kFGD2NuECC) ssample       = "FGD2 nueCC inclusive";
    else if (sample == kFGD2NuECC0Pi) ssample    = "FGD2 nueCC 0pi";
    else if (sample == kFGD2NuECCOther) ssample  = "FGD2 nueCC other";
    else if (sample == kFGD1AntiNuMuCC) ssample  = "FGD1 anti-numuCC inclusive";
    else if (sample == kFGD1AntiNuMuCC1Track) ssample  = "FGD1 anti-numuCC QE";
    else if (sample == kFGD1AntiNuMuCCNTracks) ssample  = "FGD1 anti-numuCC nQE";
    else if (sample == kFGD2AntiNuMuCC) ssample         = "FGD2 anti-numuCC inclusive";
    else if (sample == kFGD2AntiNuMuCC1Track) ssample   = "FGD2 anti-numuCC 1 track";
    else if (sample == kFGD2AntiNuMuCCNTracks) ssample  = "FGD2 anti-numuCC N tracks";
    else if (sample == kFGD1NuMuBkgInAntiNuModeCC) ssample  = "FGD1 NuMuBkg CC inclusive in AntiNu Mode";
    else if (sample == kFGD1NuMuBkgInAntiNuModeCC1Track) ssample  = "FGD1 NuMuBkg CCQE in AntiNu Mode ";
    else if (sample == kFGD1NuMuBkgInAntiNuModeCCNTracks) ssample  = "FGD1 NuMuBkg CCnQE in AntiNu Mode";
    else if (sample == kFGD2NuMuBkgInAntiNuModeCC) ssample  = "FGD2 NuMuBkg CC inclusive in AntiNu Mode";
    else if (sample == kFGD2NuMuBkgInAntiNuModeCC1Track) ssample  = "FGD2 NuMuBkg CCQE in AntiNu Mode ";
    else if (sample == kFGD2NuMuBkgInAntiNuModeCCNTracks) ssample  = "FGD2 NuMuBkg CCnQE in AntiNu Mode";
    else if (sample == kFGD1AntiNuECC) ssample       = "FGD1 anti-nueCC inclusive";
    else if (sample == kFGD1AntiNuECC0Pi) ssample    = "FGD1 anti-nueCC 0pi";
    else if (sample == kFGD1AntiNuECCOther) ssample  = "FGD1 anti-nueCC other";
    else if (sample == kFGD2AntiNuECC) ssample       = "FGD2 anti-nueCC inclusive";
    else if (sample == kFGD2AntiNuECC0Pi) ssample    = "FGD2 anti-nueCC 0pi";
    else if (sample == kFGD2AntiNuECCOther) ssample  = "FGD2 anti-nueCC other";
    else if (sample == kFGD1Gamma) ssample           = "FGD1 gamma";
    else if (sample == kFGD2Gamma) ssample           = "FGD2 gamma";

    else ssample      = "Unassigned";

    return ssample;
}


//*********************************************************
std::string SampleId::ConvertSampleToSelection(SampleEnum sample){
//*********************************************************

    //These should be the same as what is used to initialize selection names in 
    //AnalysisManager. Some of them are made up right now. -ACK

    std::string ssample="";

    if      (sample == kUnassigned) ssample      = "Unassigned";
    else if (sample == kTarget1NuMuCC)      ssample = "kTrackerNumuCC";
    else if (sample == kTarget1NuMuCCQE)    ssample = "kTrackerNumuCCQE";
    else if (sample == kTarget1NuMuCC0Pi)   ssample = "kTrackerNumuCCMultiPi";
    else if (sample == kTarget1NuMuCC1Pi)   ssample = "kTrackerNumuCCMultiPi";
    else if (sample == kTarget1NuMuCCOther) ssample = "kTrackerNumuCCMultiPi";
    else if (sample == kTarget2NuMuCC)      ssample = "kTrackerNumuCCTarget2";
    else if (sample == kTarget2NuMuCC0Pi)   ssample = "kTrackerNumuCCMultiPiTarget2";
    else if (sample == kTarget2NuMuCC1Pi)   ssample = "kTrackerNumuCCMultiPiTarget2";
    else if (sample == kTarget2NuMuCCOther) ssample = "kTrackerNumuCCMultiPiTarget2";
    else if (sample == kTarget1NuECC)       ssample = "kTrackerNueCC";
    else if (sample == kTarget1NuECC0Pi)    ssample = "kTrackerNueCCMultiPi";
    else if (sample == kTarget1NuECCOther)  ssample = "kTrackerNueCCMultiPi";
    else if (sample == kTarget2NuECC)       ssample = "kTrackerNueCCTarget2";
    else if (sample == kTarget2NuECC0Pi)    ssample = "kTrackerNueCCMultiPiTarget2";
    else if (sample == kTarget2NuECCOther)  ssample = "kTrackerNueCCMultiPiTarget2";
    else if (sample == kTarget1AntiNuMuCC)        ssample  = "kTrackerAntiNumuCC";
    else if (sample == kTarget1AntiNuMuCC1Track)  ssample  = "kTrackerAntiNumuCCMultiPi";
    else if (sample == kTarget1AntiNuMuCCNTracks) ssample  = "kTrackerAntiNumuCCMultiPi";
    else if (sample == kTarget2AntiNuMuCC)        ssample  = "kTrackerAntiNumuCCTarget2";
    else if (sample == kTarget2AntiNuMuCC1Track)  ssample  = "kTrackerAntiNumuCCMultiPiTarget2";
    else if (sample == kTarget2AntiNuMuCCNTracks) ssample  = "kTrackerAntiNumuCCMultiPiTarget2";
    else if (sample == kTarget1NuMuBkgInAntiNuModeCC)        ssample   = "kTrackerNumuInAntiNuModeCC";
    else if (sample == kTarget1NuMuBkgInAntiNuModeCC1Track)  ssample   = "kTrackerNumuInAntiNuModeCCMultiPi";
    else if (sample == kTarget1NuMuBkgInAntiNuModeCCNTracks) ssample   = "kTrackerNumuInAntiNuModeCCMultiPi";
    else if (sample == kTarget2NuMuBkgInAntiNuModeCC)        ssample   = "kTrackerNumuInAntiNuModeCCTarget2";
    else if (sample == kTarget2NuMuBkgInAntiNuModeCC1Track)  ssample   = "kTrackerNumuInAntiNuModeCCMultiPiTarget2";
    else if (sample == kTarget2NuMuBkgInAntiNuModeCCNTracks) ssample   = "kTrackerNumuInAntiNuModeCCMultiPiTarget2";
    else if (sample == kTarget1AntiNuECC)      ssample  = "kTrackerAntiNueCC";
    else if (sample == kTarget1AntiNuECC0Pi)   ssample  = "kTrackerAntiNueCCMultiPi";
    else if (sample == kTarget1AntiNuECCOther) ssample  = "kTrackerAntiNueCCMultiPi";
    else if (sample == kTarget2AntiNuECC)      ssample  = "kTrackerAntiNueCCTarget2";
    else if (sample == kTarget2AntiNuECC0Pi)   ssample  = "kTrackerAntiNueCCMultiPiTarget2";
    else if (sample == kTarget2AntiNuECCOther) ssample  = "kTrackerAntiNueCCMultiPiTarget2";
    else if (sample == kTarget1Gamma)          ssample  = "kTrackerGamma";
    else if (sample == kTarget2Gamma)          ssample  = "kTrackerGammaTarget2";

    else if (sample == kFGD1NuMuCC)      ssample = "kTrackerNumuCC";
    else if (sample == kFGD1NuMuCCQE)    ssample = "kTrackerNumuCCQE";
    else if (sample == kFGD1NuMuCC0Pi)   ssample = "kTrackerNumuCCMultiPi";
    else if (sample == kFGD1NuMuCC1Pi)   ssample = "kTrackerNumuCCMultiPi";
    else if (sample == kFGD1NuMuCCOther) ssample = "kTrackerNumuCCMultiPi";
    else if (sample == kFGD2NuMuCC)      ssample = "kTrackerNumuCCFGD2";
    else if (sample == kFGD2NuMuCC0Pi)   ssample = "kTrackerNumuCCMultiPiFGD2";
    else if (sample == kFGD2NuMuCC1Pi)   ssample = "kTrackerNumuCCMultiPiFGD2";
    else if (sample == kFGD2NuMuCCOther) ssample = "kTrackerNumuCCMultiPiFGD2";
    else if (sample == kFGD1NuECC)       ssample = "kTrackerNueCC";
    else if (sample == kFGD1NuECC0Pi)    ssample = "kTrackerNueCCMultiPi";
    else if (sample == kFGD1NuECCOther)  ssample = "kTrackerNueCCMultiPi";
    else if (sample == kFGD2NuECC)       ssample = "kTrackerNueCCFGD2";
    else if (sample == kFGD2NuECC0Pi)    ssample = "kTrackerNueCCMultiPiFGD2";
    else if (sample == kFGD2NuECCOther)  ssample = "kTrackerNueCCMultiPiFGD2";
    else if (sample == kFGD1AntiNuMuCC)        ssample  = "kTrackerAntiNumuCC";
    else if (sample == kFGD1AntiNuMuCC1Track)  ssample  = "kTrackerAntiNumuCCMultiPi";
    else if (sample == kFGD1AntiNuMuCCNTracks) ssample  = "kTrackerAntiNumuCCMultiPi";
    else if (sample == kFGD2AntiNuMuCC)        ssample  = "kTrackerAntiNumuCCFGD2";
    else if (sample == kFGD2AntiNuMuCC1Track)  ssample  = "kTrackerAntiNumuCCMultiPiFGD2";
    else if (sample == kFGD2AntiNuMuCCNTracks) ssample  = "kTrackerAntiNumuCCMultiPiFGD2";
    else if (sample == kFGD1NuMuBkgInAntiNuModeCC)        ssample   = "kTrackerNumuInAntiNuModeCC";
    else if (sample == kFGD1NuMuBkgInAntiNuModeCC1Track)  ssample   = "kTrackerNumuInAntiNuModeCCMultiPi";
    else if (sample == kFGD1NuMuBkgInAntiNuModeCCNTracks) ssample   = "kTrackerNumuInAntiNuModeCCMultiPi";
    else if (sample == kFGD2NuMuBkgInAntiNuModeCC)        ssample   = "kTrackerNumuInAntiNuModeCCFGD2";
    else if (sample == kFGD2NuMuBkgInAntiNuModeCC1Track)  ssample   = "kTrackerNumuInAntiNuModeCCMultiPiFGD2";
    else if (sample == kFGD2NuMuBkgInAntiNuModeCCNTracks) ssample   = "kTrackerNumuInAntiNuModeCCMultiPiFGD2";
    else if (sample == kFGD1AntiNuECC)      ssample  = "kTrackerAntiNueCC";
    else if (sample == kFGD1AntiNuECC0Pi)   ssample  = "kTrackerAntiNueCCMultiPi";
    else if (sample == kFGD1AntiNuECCOther) ssample  = "kTrackerAntiNueCCMultiPi";
    else if (sample == kFGD2AntiNuECC)      ssample  = "kTrackerAntiNueCCFGD2";
    else if (sample == kFGD2AntiNuECC0Pi)   ssample  = "kTrackerAntiNueCCMultiPiFGD2";
    else if (sample == kFGD2AntiNuECCOther) ssample  = "kTrackerAntiNueCCMultiPiFGD2";
    else if (sample == kFGD1Gamma)          ssample  = "kTrackerGamma";
    else if (sample == kFGD2Gamma)          ssample  = "kTrackerGammaFGD2";

    else ssample      = "Unassigned";

    return ssample;
}
