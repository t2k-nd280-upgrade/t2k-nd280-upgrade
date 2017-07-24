#include "DetectorDefinition.hxx"
#include "Units.hxx"
#include "Parameters.hxx"
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

  Float_t TPCUp1min[3]   = {-1100.00, 280.0, -1760.0};
  Float_t TPCUp1max[3]   = { 1100.00,  -1140.0,   -520.0};
  Float_t TPCUp2min[3]   = {-1100.00, 280.0,  520.0};
  Float_t TPCUp2max[3]   = { 1100.00,  -1140.0, 1760.0};
  Float_t TPCDown1min[3] = {-1100.00, -1160.0, -1760.0};
  Float_t TPCDown1max[3] = { 1100.00,  -300.0, -520.0};
  Float_t TPCDown2min[3] = {-1100.00, -1160.0, 520.0};
  Float_t TPCDown2max[3] = { 1100.00,  -300.0, 1760.0};
  Float_t ForwTPC1min[3] = {-1130.00, -920.0, -2680.0};
  Float_t ForwTPC1max[3] = { 1130.00,  920.0,   -1880.0};
  Float_t ForwTPC2min[3] = {-1130.00, -920.0, -400.0};
  Float_t ForwTPC2max[3] = { 1130.00,  920.0,   400.0};
  Float_t ForwTPC3min[3] = {-1130.00, -920.0, 1880.0};
  Float_t ForwTPC3max[3] = { 1130.00,  920.0,   2690.0};

  Float_t Target1min[3] = {-932.00, -316.0, -1787.0};
  Float_t Target1max[3] = { 932.00,  284.0,  -487.0};
  Float_t Target2min[3] = {-932.00, -316.0,   487.0};
  Float_t Target2max[3] = { 932.00,  283.0,  1787.0};

 Float_t FGD1min[3] = {-932.15, -948.15,  276.0};
 Float_t FGD1max[3] = { 932.15,  916.15,  579.0};

 Float_t FGD2min[3] = {-932.15, -948.15, 1553.0};
 Float_t FGD2max[3] = { 932.15,  916.15, 1856.0};

  //from RECPACK simplified geometry: active DsECal volume
  Float_t dsecalmin[3] = {-1019.97,-1009.97,2855.03};
  Float_t dsecalmax[3] = {1019.97,1029.97,3273.17};

  //active Barrel ECal from RECPACK simplified geomtry
  Float_t tecalLmin[3] = {1385.03,-1063.47,-663.97};
  Float_t tecalLmax[3] = {1766.27,1236.47,3215.97};
  Float_t tecalRmin[3] = {-1766.27,-1063.47,-663.97};
  Float_t tecalRmax[3] = {-1385.03,1237.47,3215.97};

  Float_t tecalTLmin[3] = {36.03,1310.53,-663.97};
  Float_t tecalTLmax[3] = {1575.97,1691.77,3215.97};
  Float_t tecalTRmin[3] = {-1575.97,1310.53,-663.97};
  Float_t tecalTRmax[3] = {-36.03,1691.77,3215.97};

  Float_t tecalBLmin[3] = {136.03,-1711.77,-663.97};
  Float_t tecalBLmax[3] = {1675.97,-1330.53,3215.97};
  Float_t tecalBRmin[3] = {-1675.03,-1711.77,-663.97};
  Float_t tecalBRmax[3] = {-136.03,-1330.53,3215.97};

  //active P0D ECal from RECPACK simplified geomtry
  Float_t pecalLmin[3] = {1570.00,  -1344.00,   -3270.00};
  Float_t pecalLmax[3] = {1657.00,  1436.00,    -890.00};
  Float_t pecalRmin[3] = {-1657.00, -1344.00,   -3270.00};
  Float_t pecalRmax[3] = {-1570.00, 1436.00,    -890.00};


  Float_t pecalTLmin[3] = {36.00,     1490.00,  -3270.00};
  Float_t pecalTLmax[3] = {1576.00,   1577.00,  -890.00};
  Float_t pecalTRmin[3] = {-1576.00,  1490.03,  -3269.97};
  Float_t pecalTRmax[3] = {-36.00,    1577.97,  -890.03};

  Float_t pecalBLmin[3] = {136.00,    -1597.00, -3270.00};
  Float_t pecalBLmax[3] = {1676.00,   -1510.00, -890.00};
  Float_t pecalBRmin[3] = {-1676.00,  -1597.00, -3270.00};
  Float_t pecalBRmax[3] = {136.00,    -1510.00, -890.00};

  Float_t usecalpmin[3] = {-1858.05,    -1174.1, 3308.50};
  Float_t usecalpmax[3] = {1846.85,   1264.7, 2997.87};

  Float_t trackerAllmin[3] = {-1160.00, -1256.0,  -885.0};
  Float_t trackerAllmax[3] = { 1160.00,  1230.0,  2807.0};


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
    dump_array3(FGD1min);
    dump_array3(FGD2max);

    dump_array3(dsecalmin);
    dump_array3(dsecalmax);

    dump_array3(tecalLmin);
    dump_array3(tecalLmax);

    dump_array3(tecalRmin);
    dump_array3(tecalRmax);

    dump_array3(tecalTLmin);
    dump_array3(tecalTLmax);
    dump_array3(tecalTRmin);
    dump_array3(tecalTRmax);

    dump_array3(tecalBLmin);
    dump_array3(tecalBLmax);
    dump_array3(tecalBRmin);
    dump_array3(tecalBRmax);

    dump_array3(pecalLmin);
    dump_array3(pecalLmax);

    dump_array3(pecalRmin);
    dump_array3(pecalRmax);

    dump_array3(pecalTLmin);
    dump_array3(pecalTLmax);
    dump_array3(pecalTRmin);
    dump_array3(pecalTRmax);


    dump_array3(pecalBLmin);
    dump_array3(pecalBLmax);
    dump_array3(pecalBRmin);
    dump_array3(pecalBRmax);

    dump_array3(usecalpmin);
    dump_array3(usecalpmax);

    std::cout << "\n******************************************************* \n" << std::endl;

    std::cout.precision(ss);
  }

  //**********************************
  bool Initialize(Int_t Configuration, Int_t ToF) {
  //**********************************
    if (Configuration == -1) {
      Configuration = (Int_t)ND::params().GetParameterI("psycheNDUPUtils.Geometry.nd280upConfiguration");
      ToF           = (Int_t)ND::params().GetParameterI("psycheNDUPUtils.Geometry.AddToF");
    }

    if (Configuration == 1) {
      if (ToF) {
        DetDef::Target1min[2] =  -1737.00;
        DetDef::Target1max[2] =   -537.00;
        DetDef::Target2min[2] =    537.00;
        DetDef::Target2max[2] =   1737.00;
      }
    }
  
    if (Configuration >= 2) {
      
      DetDef::FGD1min[0] = -932.15;
      DetDef::FGD1max[0] =  932.15; 
      DetDef::FGD1min[1] = -948.15;
      DetDef::FGD1max[1] =  916.15; 
      DetDef::FGD1min[2] =  276.00;
      DetDef::FGD1max[2] =  579.00;
  
      DetDef::FGD2min[0] = -932.15;
      DetDef::FGD2max[0] =  932.15; 
      DetDef::FGD2min[1] = -948.15;
      DetDef::FGD2max[1] =  916.15; 
      DetDef::FGD2min[2] = 1553.00;
      DetDef::FGD2max[2] = 1856.00;
    
    }
  
    if (Configuration == 2) {
      if (ToF) {
        DetDef::Target1min[2] =  -2642.00;
        DetDef::Target1max[2] =   -748.00;
      }
      else {
        DetDef::Target1min[2] =  -2692.00;
        DetDef::Target1max[2] =   -698.00;
      }
    }
    if (Configuration == 3) {
      if (ToF) {
        DetDef::Target1min[2] =  -1668.00;
        DetDef::Target1max[2] =    226.00;
      }
      else {
        DetDef::Target1min[2] =  -1718.00;
        DetDef::Target1max[2] =    276.00;
      }
    }
    return true;
  }
  
} //namespace


