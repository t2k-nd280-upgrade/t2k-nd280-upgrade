#include "DetectorDefinition.hxx"
#include "Units.hxx"
#include <iomanip>

//dumping of array
#define dump_array3(par) std::cout << std::fixed    << \
  std::setw(10) << std::left   << #par   << ": \t"  << \
  std::setw(10) << std::right  << par[0] << " "     << \
  std::setw(10) << std::right  << par[1] << " "     << \
  std::setw(10) << std::right  << par[2] << std::endl;

#define dump_var(par)  std::cout << std::fixed << std::setw(20) << std::left << #par << ":\t" << \
  std::setw(10) << std::right << par << std::endl;

namespace DetDef {

  // Detector definitions
  // Can be overridden in yourAnalysis::Initialize()
  // with e.g. DetDef::Subdet1_1min[0] = x;
  // (note that DetDef::Subdet1_1min = {x,y,z} works only with old compilers)
  // Fiducial volume is defined by removing the edge volumes in FiducialVolumeDefinition.cxx

  Float_t TPCUp1min[3] = {-1150.00, -1150.0, -885.0};
  Float_t TPCUp1max[3] = { 1150.00,  1230.0,   89.0};
  Float_t TPCUp2min[3] = {-1150.00, -1170.0,  474.0};
  Float_t TPCUp2max[3] = { 1150.00,  1230.0, 1448.0};
  Float_t TPCDown1min[3] = {-1150.00, -1170.0, 1833.0};
  Float_t TPCDown1max[3] = { 1150.00,  1230.0, 2807.0};
  Float_t TPCDown2min[3] = {-1150.00, -1170.0, 1833.0};
  Float_t TPCDown2max[3] = { 1150.00,  1230.0, 2807.0};
  Float_t ForwTPC1min[3] = {-1150.00, -1170.0, -885.0};
  Float_t ForwTPC1max[3] = { 1150.00,  1230.0,   89.0};
  Float_t ForwTPC2min[3] = {-1150.00, -1170.0, -885.0};
  Float_t ForwTPC2max[3] = { 1150.00,  1230.0,   89.0};
  Float_t ForwTPC3min[3] = {-1150.00, -1170.0, -885.0};
  Float_t ForwTPC3max[3] = { 1150.00,  1230.0,   89.0};

  Float_t Target1min[3] = {-1150.00, -300.0, -2487.0};
  Float_t Target1max[3] = { 1150.00,  300.0,  -487.0};

  Float_t Target2min[3] = {-1150.00, -300.0, 487.0};
  Float_t Target2max[3] = { 1150.00,  300.0,   2487.0};


  //**********************************
  void DumpVolumes(){
  //**********************************
    std::streamsize ss = std::cout.precision();
    std::cout.precision(3);

    std::cout << "\n***** DetectorDefinition: available volumes list: ***** \n" << std::endl;

    dump_array3(TPCUp1min);
    dump_array3(TPCUp1max);

    dump_array3(TPCUp2min);
    dump_array3(TPCUp2max);

    dump_array3(TPCDown1min);
    dump_array3(TPCDown1max);

    dump_array3(TPCDown2min);
    dump_array3(TPCDown2max);

    dump_array3(ForwTPC1min);
    dump_array3(ForwTPC1max);

    dump_array3(ForwTPC2min);
    dump_array3(ForwTPC2max);

    dump_array3(ForwTPC3min);
    dump_array3(ForwTPC3max);

    dump_array3(Target1min);
    dump_array3(Target2max);


    std::cout << "\n******************************************************* \n" << std::endl;

    std::cout.precision(ss);
  }

} //namespace

