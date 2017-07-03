#ifndef ND280UP_CONV
#define ND280UP_CONV


namespace nd280upconv {
  
  const double kBadNum = 999999999999.;

  typedef enum ETargetType
    {
      kUndefined = -999999,
      kSuperFGD = 0,
      kFGD,
      kFGD3D,
      kWAGASCI,
      kSciFi,
      kFGDlike
    }
    TargetType_t;
  
} // conv

#endif
