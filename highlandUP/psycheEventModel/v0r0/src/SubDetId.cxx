#include "SubDetId.hxx"
#include <iostream>
#include <bitset>

const unsigned long SubDetId::DetMask[] = {
    1<<SubDetId::kTPCUp1,
    1<<SubDetId::kTPCUp2,
    1<<SubDetId::kForwTPC1,
    1<<SubDetId::kForwTPC2,
    1<<SubDetId::kForwTPC3,
    1<<SubDetId::kTPCDown1,
    1<<SubDetId::kTPCDown2,
    1<<SubDetId::kTarget1,
    1<<SubDetId::kTarget2,
    1<<SubDetId::kFGD1,
    1<<SubDetId::kFGD2,
    1<<SubDetId::kP0D,
    1<<SubDetId::kDsECal,
    1<<SubDetId::kP0DECal,
    1<<SubDetId::kBrlECal,
    1<<SubDetId::kToFTopDown,
    1<<SubDetId::kToFTopUp,
    1<<SubDetId::kToFBotDown,
    1<<SubDetId::kToFBotUp,
    1<<SubDetId::kToFLeftDown,
    1<<SubDetId::kToFLeftUp,
    1<<SubDetId::kToFRightDown,
    1<<SubDetId::kToFRightUp,
    1<<SubDetId::kToFBackDown,
    1<<SubDetId::kToFBackUp,
    1<<SubDetId::kToFFrontDown,
    1<<SubDetId::kToFFrontUp,
    1<<SubDetId::kToFECalP0D,
    1<<SubDetId::kInvalidSubdetector,
    1<<SubDetId::kTPC     | SubDetId::MakeMask(SubDetId::kTPCUp1,     SubDetId::kTPCDown2),
    1<<SubDetId::kTarget  | SubDetId::MakeMask(SubDetId::kTarget1,    SubDetId::kTarget2),
    1<<SubDetId::kFGD     | SubDetId::MakeMask(SubDetId::kFGD1,       SubDetId::kFGD2),
    1<<SubDetId::kECAL    | SubDetId::MakeMask(SubDetId::kDsECal,     SubDetId::kBrlECal),
    1<<SubDetId::kToF     | SubDetId::MakeMask(SubDetId::kToFTopDown, SubDetId::kToFFrontUp),
    1<<SubDetId::kInvalid
};

void SubDetId::SetDetectorUsed(unsigned long &BitField, SubDetId::SubDetEnum det){
    BitField = BitField | 1<<det;
}

bool SubDetId::GetDetectorUsed(unsigned long BitField, SubDetId::SubDetEnum det){
    return BitField & DetMask[det];
}

bool SubDetId::GetDetectorUsed(unsigned long BitField, SubDetId_h det){
    return BitField & DetMask[det];
}


bool SubDetId::GetDetectorArrayUsed(unsigned long BitField, SubDetId::SubDetEnum dets[], int nDet){

    bool passed = 1;

    for(int i = 0; i < nDet; ++i){
        if(!(BitField & DetMask[dets[i]])){
            passed = 0;
        }
    }
    return passed;
}

bool SubDetId::GetDetectorArrayUsed(unsigned long BitField, SubDetId_h dets[], int nDet){

    bool passed = 1;

    for(int i = 0; i < nDet; ++i){
        if(!(BitField & DetMask[dets[i]])){
            passed = 0;
        }
    }
    return passed;
}


int SubDetId::GetTPC(unsigned long BitField){
    if      (BitField & DetMask[SubDetId::kTPCUp1]) return 1;
    else if (BitField & DetMask[SubDetId::kTPCUp2]) return 2;
    else if (BitField & DetMask[SubDetId::kForwTPC1]) return 3;
    else if (BitField & DetMask[SubDetId::kForwTPC2]) return 4;
    else if (BitField & DetMask[SubDetId::kForwTPC3]) return 5;
    else if (BitField & DetMask[SubDetId::kTPCDown1]) return 6;
    else if (BitField & DetMask[SubDetId::kTPCDown2]) return 7;

    else return -1;
}

int SubDetId::GetTarget(unsigned long BitField){
    if      (BitField & DetMask[SubDetId::kTarget1]) return 1;
    else if (BitField & DetMask[SubDetId::kTarget2]) return 2;
    else return -1;
}

int SubDetId::GetFGD(unsigned long BitField){
    if      (BitField & DetMask[SubDetId::kFGD1]) return 1;
    else if (BitField & DetMask[SubDetId::kFGD2]) return 2;
    else return -1;
}

int SubDetId::GetECal(unsigned long BitField){
    if      (BitField & DetMask[SubDetId::kDsECal])  return 1;
    else if (BitField & DetMask[SubDetId::kP0DECal]) return 2;
    else if (BitField & DetMask[SubDetId::kBrlECal]) return 3;
    else return -1;
}

int SubDetId::GetTOF(unsigned long BitField){
    if      (BitField & DetMask[SubDetId::kToFTopDown]) return 1;
    else if (BitField & DetMask[SubDetId::kToFTopUp]) return 2;
    else if (BitField & DetMask[SubDetId::kToFBotDown]) return 3;
    else if (BitField & DetMask[SubDetId::kToFBotUp]) return 4;
    else if (BitField & DetMask[SubDetId::kToFLeftDown]) return 5;
    else if (BitField & DetMask[SubDetId::kToFLeftUp]) return 6;
    else if (BitField & DetMask[SubDetId::kToFRightDown]) return 6;
    else if (BitField & DetMask[SubDetId::kToFRightUp]) return 7;
    else if (BitField & DetMask[SubDetId::kToFBackDown]) return 8;
    else if (BitField & DetMask[SubDetId::kToFBackUp]) return 9;
    else if (BitField & DetMask[SubDetId::kToFFrontDown]) return 10;
    else if (BitField & DetMask[SubDetId::kToFFrontUp]) return 11;
    else if (BitField & DetMask[SubDetId::kToFECalP0D]) return 12;

    else return -1;
}

void SubDetId::SetDetectorSystemFields(unsigned long &BitField){
  for(int i = SubDetId::kTPCUp1; i < SubDetId::kInvalidSubdetector; ++i){ //loop through sub-detectors list
    if (GetDetectorUsed(BitField, static_cast<SubDetId::SubDetEnum>(i))){
      // Subdet1
    }else if (i == SubDetId::kTPCUp1 || i == SubDetId::kTPCUp1 || i == SubDetId::kForwTPC1 || i == SubDetId::kForwTPC2 || i == SubDetId::kForwTPC3 || i == SubDetId::kTPCDown2 || i == SubDetId::kTPCDown1){
      SetDetectorUsed(BitField, SubDetId::kTPC);
      // Subdet2
    }else if (i == SubDetId::kTarget1 || i == SubDetId::kTarget2){
      SetDetectorUsed(BitField, SubDetId::kTarget);
    }else if (i == SubDetId::kFGD1 || i == SubDetId::kFGD2){
      SetDetectorUsed(BitField, SubDetId::kFGD);
    }else if (i == SubDetId::kP0D){
      return;
    }else if (i == SubDetId::kDsECal || i == SubDetId::kBrlECal || i == SubDetId::kP0DECal ){
      SetDetectorUsed(BitField, SubDetId::kECAL);
    }else if (i == SubDetId::kToFTopUp   || i == SubDetId::kToFTopDown || 
	      i == SubDetId::kToFBotUp   ||  i == SubDetId::kToFBotDown || 
	      i == SubDetId::kToFLeftUp  || i == SubDetId::kToFLeftDown ||
              i == SubDetId::kToFRightUp || i == SubDetId::kToFRightDown ||
	      i == SubDetId::kToFBackUp  || i == SubDetId::kToFBackDown || 
	      i == SubDetId::kToFFrontUp || i == SubDetId::kToFFrontDown ||
	      i == SubDetId::kToFECalP0D){
      SetDetectorUsed(BitField, SubDetId::kToF);

    }
  }
}
int SubDetId::NumberOfSetBits(unsigned long v){
    int c; // c accumulates the total bits set in v
    for (c = 0; v; c++){
        v &= v - 1; // clear the least significant bit set
    }
    return c;
}

int SubDetId::GetNSegmentsInDet(unsigned long BitField, SubDetId::SubDetEnum det){
    return NumberOfSetBits(BitField & DetMask[det]);
}

bool SubDetId::TrackUsesOnlyDet(unsigned long BitField, SubDetId::SubDetEnum det){
    BitField = BitField & MakeMask(SubDetId::kToFFrontUp, SubDetId::kTPCUp1);
    return !(BitField & ~DetMask[det]);
}

bool SubDetId::IsTarget(SubDetId::SubDetEnum det){
  return ((det <= SubDetId::kTarget2 && det >= SubDetId::kTarget1 )|| det == SubDetId::kTarget);
}

bool SubDetId::IsTPC(SubDetId::SubDetEnum det){
  return ((det <= SubDetId::kTPCDown2 && det >= SubDetId::kTPCUp1) || det == SubDetId::kTPC);
}

bool SubDetId::IsFGD(SubDetId::SubDetEnum det){
  return ((det <= SubDetId::kFGD2 && det >= SubDetId::kFGD1) || det == SubDetId::kFGD);
}

bool SubDetId::IsECal(SubDetId::SubDetEnum det){
  return ((det <= SubDetId::kBrlECal  && det >= SubDetId::kDsECal) || det == SubDetId::kECAL);
}
    
bool SubDetId::IsTOF(SubDetId::SubDetEnum det){
  return ((det <= SubDetId::kToFECalP0D && det >= SubDetId::kToFTopDown) || det == SubDetId::kToF);
}

bool SubDetId::IsP0D(SubDetId::SubDetEnum det){
  return (det == kP0D);
}

SubDetId::SubDetEnum SubDetId::GetSubdetectorEnum(unsigned long BitField){
 
    BitField = BitField & MakeMask(SubDetId::kToFECalP0D, SubDetId::kTPCUp1);
    int nBits = NumberOfSetBits(BitField);
    if(nBits!=1){
        std::cout << " Error: Track contains multiple subdetectors, cannot find a single subdetector enum to return." << std::endl;
        return SubDetId::kInvalid;
    }
    BitField = BitField | (BitField-1);
    nBits = NumberOfSetBits(BitField);
    return static_cast<SubDetId::SubDetEnum>(nBits-1);
}

