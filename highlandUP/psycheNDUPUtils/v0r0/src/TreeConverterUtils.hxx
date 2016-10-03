#ifndef TreeConverterUtils_h
#define TreeConverterUtils_h

#include "BaseDataClasses.hxx"
#include "ND280UPAnalysisUtils.hxx"
#include "SubDetId.hxx"

/// This namespace contains useful functions for analyses. 
/// This is in addition to those defined in psycheUtils
namespace convUtils{

    
    /// Convert the detector used array to the bit field used by psyche for TPC track segments
    void ConvertTPCDetEnumToBitField(unsigned long &det, int tpc);
    /// Convert the detector used array to the bit field used by psyche for FGD track segments
    void ConvertTargetDetEnumToBitField(unsigned long &det, int target);
    
    /// Convert the detector used array to the bit field used by psyche given a SubDetEnum
    void ConvertLocalDetEnumToBitField(unsigned long &det, int index, SubDetId::SubDetEnum subdet_enum); 

    /// Convert the detector used array to the bit field used by psyche for truth trueParts
    void ConvertTrueParticleDetEnumToBitField(unsigned long &det, int DetUsed);
    
   
    /// Convert the detector used array to the bit field used by psyche for ana tracks
    void ConvertTrackDetEnumToBitField(unsigned long &det, int DetUsed[]);
    /// Convert the detector used array to the bit field used by psyche for ana tracks
    void ConvertTrackDetEnumToBitField(unsigned long &det, bool DetUsed[]);
    /// Set the bit field for a used detector given oaAnalysis detector enumeration (TGlobalPID::DetectorUsed)
    void SetDetectorUsedField(unsigned long &det, unsigned int i);
    /// Get old local detector enumeration to find array index of Flat tree  
    int GetLocalDetEnum(SubDetId::SubDetEnum det, SubDetId::SubDetEnum idet);

   
    /// Get highland style numbering for true objects
    void ConvertBitFieldToTrueParticleDetEnum(unsigned long det, int &trueDet);

}
#endif


