#include "SampleId.hxx"


//*********************************************************
std::string SampleId::ConvertSample(SampleEnum sample){
//*********************************************************

    std::string ssample="";

    if      (sample == kUnassigned) ssample      = "Unassigned";
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
