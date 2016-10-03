#include <FiducialVolumeDefinition.hxx>
#include <DetectorDefinition.hxx>
#include "Units.hxx"
#include <iomanip>

//dumping of array
#define dump_array3(par) std::cout << std::fixed    << \
  std::setw(10) << std::left   << #par   << ": \t"  << \
  std::setw(10) << std::right  << par[0] << " "     << \
  std::setw(10) << std::right  << par[1] << " "     << \
  std::setw(10) << std::right  << par[2] << std::endl;

namespace FVDef {

  // FV definitions: edges to be removed from the detector definitions in DetectorDefinition.cxx
  // Can be overridden in yourAnalysis::Initialize()
  // with e.g. FVDef::FVdefminSubdet1_1[0] = x;
  // (note that FVDef::FVdefminSubdet1_1 = {x,y,z} works only with old compilers)

  Float_t FVdefminTPCUp1[3] = {20, 20, 20};
  Float_t FVdefmaxTPCUp1[3] = {20, 20, 20};

  Float_t FVdefminTPCUp2[3] = {20, 20, 20};
  Float_t FVdefmaxTPCUp2[3] = {20, 20, 20};

  Float_t FVdefminTPCDown1[3] = {20, 20, 20};
  Float_t FVdefmaxTPCDown1[3] = {20, 20, 20};

  Float_t FVdefminTPCDown2[3] = {20, 20, 20};
  Float_t FVdefmaxTPCDown2[3] = {20, 20, 20};

  Float_t FVdefminForwTPC1[3] = {20, 20, 20};
  Float_t FVdefmaxForwTPC1[3] = {20, 20, 20};

  Float_t FVdefminForwTPC2[3] = {20, 20, 20};
  Float_t FVdefmaxForwTPC2[3] = {20, 20, 20};

  Float_t FVdefminForwTPC3[3] = {20, 20, 20};
  Float_t FVdefmaxForwTPC3[3] = {20, 20, 20};


  Float_t FVdefminTarget1[3] = {0*units::cm,0*units::cm,0*units::cm};
  Float_t FVdefmaxTarget2[3] = {0*units::cm,0*units::cm,0*units::cm};


  //**********************************
  void DumpFV(){
  //**********************************
    std::streamsize ss = std::cout.precision();
    std::cout.precision(3);

    std::cout << "\n***** FiducialVolume: available volumes list: ***** \n" << std::endl;

    dump_array3(FVdefminTPCUp1);
    dump_array3(FVdefmaxTPCUp1);

    dump_array3(FVdefminTPCUp2);
    dump_array3(FVdefmaxTPCUp2);

    dump_array3(FVdefminTPCDown1);
    dump_array3(FVdefmaxTPCDown1);

    dump_array3(FVdefminTPCDown2);
    dump_array3(FVdefmaxTPCDown2);

    dump_array3(FVdefminForwTPC1);
    dump_array3(FVdefmaxForwTPC1);

    dump_array3(FVdefminForwTPC2);
    dump_array3(FVdefmaxForwTPC2);

    dump_array3(FVdefminForwTPC3);
    dump_array3(FVdefmaxForwTPC3);

    dump_array3(FVdefminTarget1);
    dump_array3(FVdefmaxTarget2);

    std::cout << "\n******************************************************* \n" << std::endl;

    std::cout.precision(ss);
  }

}
