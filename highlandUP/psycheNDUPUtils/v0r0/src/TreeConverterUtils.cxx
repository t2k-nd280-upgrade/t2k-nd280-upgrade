#include "TreeConverterUtils.hxx"
#include <stdio.h>
#include <math.h>


namespace convUtils {}


//****************************************************************************
void convUtils::ConvertTPCDetEnumToBitField(unsigned long &det, int tpc){
//****************************************************************************
    SubDetId::SetDetectorUsed(det, SubDetId::kTPC);
    switch(tpc){
        case 0: SubDetId::SetDetectorUsed(det, SubDetId::kTPCUp1);
                break;
        case 1: SubDetId::SetDetectorUsed(det, SubDetId::kTPCUp2);
                break;
        case 2: SubDetId::SetDetectorUsed(det, SubDetId::kTPCDown1);
                break;
        case 3: SubDetId::SetDetectorUsed(det, SubDetId::kTPCDown2);
                break;
        case 4: SubDetId::SetDetectorUsed(det, SubDetId::kForwTPC1);
                break;
        case 5: SubDetId::SetDetectorUsed(det, SubDetId::kForwTPC2);
                break;
        case 6: SubDetId::SetDetectorUsed(det, SubDetId::kForwTPC3);
                break;

        default: SubDetId::SetDetectorUsed(det, SubDetId::kInvalid);
    }
}

//****************************************************************************
void convUtils::ConvertTargetDetEnumToBitField(unsigned long &det, int target){
//****************************************************************************
   SubDetId::SetDetectorUsed(det, SubDetId::kTarget);
    switch(target){
        case 0: SubDetId::SetDetectorUsed(det, SubDetId::kTarget1);
                break;
        case 1: SubDetId::SetDetectorUsed(det, SubDetId::kTarget2);
                break;
        default: SubDetId::SetDetectorUsed(det, SubDetId::kInvalid);
    }
}

//****************************************************************************
void convUtils::ConvertFGDDetEnumToBitField(unsigned long &det, int target){
//****************************************************************************
   SubDetId::SetDetectorUsed(det, SubDetId::kFGD);
    switch(target){
        case 0: SubDetId::SetDetectorUsed(det, SubDetId::kFGD1);
                break;
        case 1: SubDetId::SetDetectorUsed(det, SubDetId::kFGD2);
                break;
        default: SubDetId::SetDetectorUsed(det, SubDetId::kInvalid);
    }
}


//****************************************************************************
void convUtils::ConvertLocalDetEnumToBitField(unsigned long &det, int index, SubDetId::SubDetEnum subdet_enum){
//****************************************************************************

  switch(subdet_enum){
    case SubDetId::kTPC :
      convUtils::ConvertTPCDetEnumToBitField(det, index);    
      break;
    case SubDetId::kTarget :
      convUtils::ConvertTargetDetEnumToBitField(det, index);
      break;
    case SubDetId::kFGD :
      convUtils::ConvertFGDDetEnumToBitField(det, index);
      break;
    default :
      std::cout << "convUtils::ConvertLocalDetEnumToBitField(). Cannot get old local detector enumeration for subsystem: " << subdet_enum << ". Please define whether it is a TPC, FGD, ECAL, SMRD or P0D track." << std::endl;
      SubDetId::SetDetectorUsed(det, SubDetId::kInvalid);
  }
}

//****************************************************************************
void convUtils::ConvertTrueParticleDetEnumToBitField(unsigned long &det, int DetUsed){
//****************************************************************************

/*   This is the enumeration used in oaAnalysis (ToaAnalysisUtils.hxx)
  kFGD1 = 0,
  kFGD2,
  kP0D,
  kDsECal,
  kBrlECal,
  kP0DECal,
  kTPC1,
  kTPC2,
  kTPC3,
  kMRD,
  kOffAxis,                                                         /// the rest of the off-axis detector
  kINGRID,
  kCavern,
  kNSubdetectors
*/

  switch(DetUsed){
    case 0: SubDetId::SetDetectorUsed(det, SubDetId::kTPCUp1);
            break;
    case 1: SubDetId::SetDetectorUsed(det, SubDetId::kTPCUp2);
            break;
    case 2: SubDetId::SetDetectorUsed(det, SubDetId::kTPCDown1);
            break;
    case 3: SubDetId::SetDetectorUsed(det, SubDetId::kTPCDown2);
            break;
    case 4: SubDetId::SetDetectorUsed(det, SubDetId::kForwTPC1);  // Temporary fix whilst SubDetId in truth is not available from oaAnalysis
            break;
    case 5: SubDetId::SetDetectorUsed(det, SubDetId::kForwTPC2);  // Temporary fix whilst SubDetId in truth is not available from oaAnalysis
            break;
    case 6: SubDetId::SetDetectorUsed(det, SubDetId::kForwTPC3);  // Temporary fix whilst SubDetId in truth is not available from oaAnalysis
            break;
    case 7: SubDetId::SetDetectorUsed(det, SubDetId::kTarget1);  // Temporary fix whilst SubDetId in truth is not available from oaAnalysis
            break;
    case 8: SubDetId::SetDetectorUsed(det, SubDetId::kTarget2);
            break;
    case 9: SubDetId::SetDetectorUsed(det, SubDetId::kFGD1);  // Temporary fix whilst SubDetId in truth is not available from oaAnalysis
            break;
    case 10: SubDetId::SetDetectorUsed(det, SubDetId::kFGD2);
            break;
    default: SubDetId::SetDetectorUsed(det, SubDetId::kInvalid);
  }
  SubDetId::SetDetectorSystemFields(det);
}


//****************************************************************************
void convUtils::ConvertTrackDetEnumToBitField(unsigned long &det, int DetUsed[]){
 //****************************************************************************
  for(UInt_t i = 0; i < NDETECTORS; ++i){
    if(DetUsed[i]==1){
     convUtils::SetDetectorUsedField(det, i);
    }
  }
  SubDetId::SetDetectorSystemFields(det);
}

//****************************************************************************
void convUtils::ConvertTrackDetEnumToBitField(unsigned long &det, bool DetUsed[]){
//****************************************************************************
  for(UInt_t i = 0; i < NDETECTORS; ++i){
    if(DetUsed[i]){
     convUtils::SetDetectorUsedField(det, i);
    }
  }
  SubDetId::SetDetectorSystemFields(det);
}


//****************************************************************************
void convUtils::SetDetectorUsedField(unsigned long &det, unsigned int i){
//****************************************************************************
  switch(i){
    case 0: SubDetId::SetDetectorUsed(det, SubDetId::kTPCUp1);
            break;
    case 1: SubDetId::SetDetectorUsed(det, SubDetId::kTPCUp2);
            break;
    case 5: SubDetId::SetDetectorUsed(det, SubDetId::kTPCDown1);
            break;
    case 6: SubDetId::SetDetectorUsed(det, SubDetId::kTPCDown2);
            break;
    case 2: SubDetId::SetDetectorUsed(det, SubDetId::kForwTPC1);  // Temporary fix whilst SubDetId in truth is not available from oaAnalysis
            break;
    case 3: SubDetId::SetDetectorUsed(det, SubDetId::kForwTPC2);  // Temporary fix whilst SubDetId in truth is not available from oaAnalysis
            break;
    case 4: SubDetId::SetDetectorUsed(det, SubDetId::kForwTPC3);  // Temporary fix whilst SubDetId in truth is not available from oaAnalysis
            break;
    case 7: SubDetId::SetDetectorUsed(det, SubDetId::kTarget1);  // Temporary fix whilst SubDetId in truth is not available from oaAnalysis
            break;
    case 8: SubDetId::SetDetectorUsed(det, SubDetId::kTarget2);
            break;
    case 9: SubDetId::SetDetectorUsed(det, SubDetId::kFGD1);  // Temporary fix whilst SubDetId in truth is not available from oaAnalysis
            break;
    case 10: SubDetId::SetDetectorUsed(det, SubDetId::kFGD2);
            break;

    default: SubDetId::SetDetectorUsed(det, SubDetId::kInvalid);
  }
  return;
}


//********************************************************************
int convUtils::GetLocalDetEnum(SubDetId::SubDetEnum det, SubDetId::SubDetEnum idet){
//********************************************************************

    switch(det){
        case SubDetId::kTPC :
            if(SubDetId::IsTPC(idet)) return (idet - 2);
	          else std::cout << "Error: Should not be here, detector " << idet << " is not in the TPC" << std::endl;
            break;
        case SubDetId::kTarget :
            if(SubDetId::IsTarget(idet)) return (idet);
	          else std::cout << "Error: Should not be here, detector " << idet << " is not in the Target" << std::endl;
            break;
        case SubDetId::kFGD :
            if(SubDetId::IsFGD(idet)) return (idet);
	          else std::cout << "Error: Should not be here, detector " << idet << " is not in the FGD" << std::endl;
            break;
        default :
            std::cout << "convUtils::GetLocalDetEnum(). Cannot get old local detector enumeration for subsystem: " << det << ". Please define whether it is a TPC, FGD, ECAL, P0D or SMRD track." << std::endl;
            return -1;
    }
    std::cout << "convUtils::GetLocalDetEnum(). Cannot get old local detector enumeration for subsystem: " << det << ". Please define whether it is a TPC, FGD, ECAL, P0D or SMRD track." << std::endl;
    return -1;
}



//****************************************************************************
void convUtils::ConvertBitFieldToTrueParticleDetEnum(unsigned long det, int &trueDet){
//****************************************************************************

  //Note! special treatment since oaAnalysis true info does not allow to split between 
  //TECal/PECal/SMRD sub-detectors and hence now sub-detector bits are set
  
  trueDet=-1;

  if ( SubDetId::GetDetectorUsed(det, SubDetId::kTPCUp1))           trueDet = 0;
  else if ( SubDetId::GetDetectorUsed(det, SubDetId::kTPCUp2))      trueDet = 1;
  else if ( SubDetId::GetDetectorUsed(det, SubDetId::kTPCDown1))    trueDet = 5;
  else if ( SubDetId::GetDetectorUsed(det, SubDetId::kTPCDown2))    trueDet = 6;
  else if ( SubDetId::GetDetectorUsed(det, SubDetId::kForwTPC1))    trueDet = 2;
  else if ( SubDetId::GetDetectorUsed(det, SubDetId::kForwTPC2))    trueDet = 3;
  else if ( SubDetId::GetDetectorUsed(det, SubDetId::kForwTPC3))    trueDet = 4;
  else if ( SubDetId::GetDetectorUsed(det, SubDetId::kTarget1))     trueDet = 7;
  else if ( SubDetId::GetDetectorUsed(det, SubDetId::kTarget2))     trueDet = 8;
  else if ( SubDetId::GetDetectorUsed(det, SubDetId::kFGD1))        trueDet = 9;
  else if ( SubDetId::GetDetectorUsed(det, SubDetId::kFGD2))        trueDet = 10;
  else if ( SubDetId::GetDetectorUsed(det, SubDetId::kDsECal))      trueDet = 11;
  else if ((SubDetId::GetDetectorUsed(det, SubDetId::kBrlECal)) ||
	   (     det & (1<<SubDetId::kBrlECal)))                    trueDet = 13;
  else if ((SubDetId::GetDetectorUsed(det, SubDetId::kP0DECal)) ||
	   (     det & (1<<SubDetId::kP0DECal)))                    trueDet = 12;

}

