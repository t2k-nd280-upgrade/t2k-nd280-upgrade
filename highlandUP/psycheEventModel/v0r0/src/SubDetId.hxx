#ifndef SubDetId_hxx_seen
#define SubDetId_hxx_seen

#include "BasicTypes.hxx"

class SubDetId {

private:
    /// Bit mask array to select different subdetectors and systems
    const static unsigned long DetMask[28];

    /// Method to make detector bit masks
    static unsigned long MakeMask(int msb, int lsb) {return (((1 << (((msb) - (lsb)) + 1)) - 1) << (lsb));}

    /// Method to count the number of set bits in a given bit field
    static int NumberOfSetBits(unsigned long v);


public:

    SubDetId() {};
    ~SubDetId() {};

    /// Enumeration of all detector systems and subdetectors (example of one subdetector with 2 modules and another one with 3
    enum SubDetEnum {
        kTPCUp1 = 0,
        kTPCUp2,
        kForwTPC1,
        kForwTPC2,
        kForwTPC3,       
        kTPCDown1,
        kTPCDown2,
        kTarget1,
        kTarget2,
        kDsECal,
        kP0DECal,
        kBrlECal,
        kInvalidSubdetector,
        kTPC,
        kTarget,
        kECAL,
        kInvalid
    };

    /// Method to set a certain subdetector or subdetector system to used used
    static void SetDetectorUsed(unsigned long &BitField, SubDetId::SubDetEnum det);

    /// Method to see if a certain subdetector or subdetector system is used
    static bool GetDetectorUsed(unsigned long BitField, SubDetId::SubDetEnum det);

    static bool GetDetectorArrayUsed(unsigned long BitField, SubDetId::SubDetEnum dets[], int nDet);

    static bool GetDetectorUsed(unsigned long BitField, SubDetId_h det);
    static bool GetDetectorArrayUsed(unsigned long BitField, SubDetId_h dets[], int nDet);


    /// Returns the TPCUp number (TPCUp1 = 1, TPCUp2 = 2 etc.) of the track
    /// Does not check whether track passed through more than one TPCUp module, just returns the first TPCUp it finds 
    static int GetTPC(unsigned long BitField);
    /// Returns the Target number (Target1 = 1, Target2 = 2 etc.) of the track
    /// Does not check whether track passed through more than one Target module, just returns the first Target it finds 
    static int GetTarget(unsigned long BitField);
    /// Returns the Target number (DsECal = 1, P0DECal = 2 etc.) of the track
    /// Does not check whether track passed through more than one Target module, just returns the first Target it finds 
    static int GetECal(unsigned long BitField);
 
    /// Method to set the detector system bits using the individual subdetector bits.
    /// e.g. If SetDetectorUsed(BitField, SubDetId::kTPC1) had been called, this function
    /// would call SetDetectorUsed(BitField, SubDetId::kTPC)
    static void SetDetectorSystemFields(unsigned long &BitField);

    /// Method to count the number of Segments a given track has in a detector system
    /// The DetMask bitfield masks are used to select the specified detector system
    static int GetNSegmentsInDet(unsigned long BitFIeld, SubDetId::SubDetEnum det);

    /// Check whether a track only uses a specified subdetector or detector system
    static bool TrackUsesOnlyDet(unsigned long BitFIeld, SubDetId::SubDetEnum det);

    /// Check if a detector enumeration refers to a TPC or not
    static bool IsTarget(SubDetId::SubDetEnum det);

    /// Check if a detector enumeration refers to a FGD or not
    static bool IsTPC(SubDetId::SubDetEnum det);
    /// Check if a detector enumeration refers to a FGD or not
    static bool IsECal(SubDetId::SubDetEnum det);
   
    /// Get the single subdetector that this track is from
    static SubDetId::SubDetEnum GetSubdetectorEnum(unsigned long BitField);

};
#endif


