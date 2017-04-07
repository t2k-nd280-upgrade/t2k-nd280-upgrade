#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <memory>
#include <iomanip>
#include <typeinfo>
#include "RecPackGeometry.hxx"

//------ ROOT includes ---------

#include <TGeoManager.h>
#include <TGeoNode.h>
#include <TGeoVolume.h>
#include <TGeoBBox.h>
#include <TVector3.h>
#include <TKey.h>
#include <TFile.h>
#include <TString.h>
#include <TPRegexp.h>


//------ nd280 includes ---------

#include <Units.hxx>
#include "Parameters.hxx"
#include "GeometryManager.hxx"

/* Info about FGD volumes
   "Beam"       // these are tubes
   "SidePlate"  // 11 mm thick on each side
   "Foot"       // these are small pieces
   "EndPlate"   // 6 mm thick at front and back
   "BusBoard"   // 48 mm thick on each side
   "MiniCrate"  // 100 mm thick at the bottom (6 modules)

   Info about ECAL modules

   "PrimaryBulk"    // These are all surounding the sides of the modules  (not essential)
   "CoolingPlate"   // These are all surounding the sides of the modules  (not essential)

   "BasePanel"      // Dead material upstream of DsECAL (Only CarbonFibre is relevant, rest are tube like objects)
   "LidPanel"       // Dead material downstream of DsECAL   (not needed)

   "Base"           // Dead material between BrlEcal/P0DEcal and Tracker (Only CarbonFibre is relevant, rest are tube like objects)
   "Lid"            // Dead material between BrlEcal/P0DEcal and SMRD


*/


/// The static member pointer to the singleton.
RecPackGeometry* RecPackGeometry::fRecPackGeometry = NULL;

//*****************************************************************************
RecPackGeometry& ND::gman(int verb) {
  //*****************************************************************************

  return RecPackGeometry::Get(verb);

}

//*****************************************************************************
RecPackGeometry::RecPackGeometry() {
  //*****************************************************************************

  _verb = 0;
  _root_setup = NULL;
  fGeoManager = NULL;
  _initialised = false;

  _root_file_name = ND::params().GetParameterS("highlandRecPack.RootGeometryFile");

  fExcludeTPCDeadMaterial = (bool)ND::params().GetParameterI("highlandRecPack.ExcludeTPCDeadMaterial");


  //Whether to add a volume for each TPC MM
  fIncludeTPCMMVolumes = (bool)ND::params().GetParameterI("highlandRecPack.IncludeTPCMMVolumes");

  //try to open the file
  //std::ifstream geom_file(_root_file_name.c_str(), std::ios::in);
  //if(!geom_file)
  //throw BadRootFileException();

  Clear();


}

//*****************************************************************************
RecPackGeometry& RecPackGeometry::Get(int verb) {
  //*****************************************************************************
  if (!fRecPackGeometry) {
    std::cout << "Create a new RecPackGeometry object" << std::endl;
    fRecPackGeometry = new RecPackGeometry;
  }
  else
    return *fRecPackGeometry;

  // Get the geometry from the GeometryManager in highlandTools
  fRecPackGeometry->fGeoManager = ND::hgman().GeoManager();

  fRecPackGeometry->SetVerbosity(verb);
  fRecPackGeometry->FillInformation();

  return *fRecPackGeometry;
}

//*****************************************************************************
void RecPackGeometry::Clear() {
  //*****************************************************************************

  //  if (_root_setup) delete _root_setup;

}

bool RecPackGeometry::VisitNode(int depth, const std::string& name, const TGeoNode* node) {

  if (!node)
    return false;

  // Check if we have found FGD1 top volume
  if ( name.find("/FGD1_0" ) != std::string::npos  && fFGDMODULES1.size() < 1) {
    fFGDMODULES1.push_back(name);
    return false;
  }

  // Check if we have found FGD2 top volume
  if ( name.find("/FGD2_0" ) != std::string::npos  && fFGDMODULES2.size() < 1) {
    fFGDMODULES2.push_back(name);
    return false;
  }
  // Check if we have found FGD2 top volume
  if ( name.find("/Target1_0" ) != std::string::npos  && fTARGET1.size() < 1 ) {
    fTARGET1.push_back(name);
    return false;
  }
  // Check if we have found FGD2 top volume
  if ( name.find("/Target2_0" ) != std::string::npos && fTARGET2.size() < 1 ) {
    fTARGET2.push_back(name);
    return false;
  }

  // Check if we have found P0D top volume
  if ( name.find("/P0D_0" ) != std::string::npos
       && fP0DMODULES.size() < 1) {
    fP0DMODULES.push_back(name);
    return false;
  }

  if ( name.find("/ForwTPC1_0" ) != std::string::npos &&  name.find("/ForwTPC1_0/" ) == std::string::npos) {
    fTPCMODULES1.push_back(name);
    return false;

  }

  if ( name.find("/ForwTPC2_0" ) != std::string::npos && fTPCMODULES2.size() < 1 ) {
    fTPCMODULES2.push_back(name);
    return false;

  }

  if ( name.find("/ForwTPC3_0" ) != std::string::npos && fTPCMODULES3.size() < 1 ) {
    fTPCMODULES3.push_back(name);
    return false;

  }
  if ( name.find("/TPCUp1_0" ) != std::string::npos  && fTPCMODULES4.size() < 1) {
    fTPCMODULES4.push_back(name);
    return false;

  }
  if ( name.find("/TPCUp2_0" ) != std::string::npos  && fTPCMODULES5.size() < 1) {
    fTPCMODULES5.push_back(name);
    return false;

  }
  if ( name.find("/TPCDown1_0" ) != std::string::npos  && fTPCMODULES6.size() < 1) {
    fTPCMODULES6.push_back(name);
    return false;

  }
  if ( name.find("/TPCDown2_0" ) != std::string::npos  && fTPCMODULES7.size() < 1) {
    fTPCMODULES7.push_back(name);
    return false;

  }

  //oh!...ECals
  if (name.find("DsECal") != std::string::npos) {
    fECALMODULES.push_back(name);
    return false;
  }

  if (name.find("P0DECal") != std::string::npos && name.find("Top") != std::string::npos) {
    fECALMODULES.push_back(name);
    return false;
  }

  if (name.find("P0DECal") != std::string::npos && name.find("Bottom") != std::string::npos) {
    fECALMODULES.push_back(name);
    return false;
  }

  if (name.find("P0DECal") != std::string::npos && name.find("Side") != std::string::npos) {
    fECALMODULES.push_back(name);
    return false;
  }



  if (name.find("BrlECal") != std::string::npos && name.find("Top") != std::string::npos) {
    fECALMODULES.push_back(name);
    return false;
  }

  if (name.find("BrlECal") != std::string::npos && name.find("Bottom") != std::string::npos) {
    fECALMODULES.push_back(name);
    return false;
  }
  if (name.find("BrlECal") != std::string::npos && name.find("Side") != std::string::npos) {
    fECALMODULES.push_back(name);
    return false;
  }


  return true;

}

//*****************************************************************************
void RecPackGeometry::FillInformation(void) {
  //*****************************************************************************

  Clear();

  VisitGeometry();


  // axes
  _xaxis = EVector(3, 0);
  _yaxis = EVector(3, 0);
  _zaxis = EVector(3, 0);

  _xaxis[0] = 1.; _yaxis[1] = 1.; _zaxis[2] = 1.;

  // left, right, top, bottom, downstream, upstream
  const int ECALORIENTATIONS = 6;
  EVector normal[ECALORIENTATIONS] = {_xaxis, _xaxis, _yaxis, _yaxis, _zaxis, _zaxis};
  for (int i = 0; i < ECALORIENTATIONS; i++) _surfNormalECAL[i] = normal[i];


  // default surface normal
  _surfNormal = _zaxis;

  // creates a new setup called "main"
  _root_setup = new RootSetup(fGeoManager, "main");

  // Set the verbosity level to the RootSetup
  GetSetup().SetVerbosity(_verb);

  // Set the verbosity level to the RP::Setup
  RP::Messenger::Level levels[4] = {Messenger::NORMAL, Messenger::DETAILED, Messenger::VERBOSE, Messenger::VVERBOSE};
  int ilevel = _verb;
  if (_verb > 3) ilevel = 3;
  GetSetup().set_verbosity(levels[ilevel]);

  // the name of the mother volume
  std::string mother_name = "/t2k_1";

  std::string oa_name = "/OA_0";
  std::string magnet_name = "/Magnet_0";

  std::string basket_name = "/Basket_0";

  // sets the mother volume
  GetSetup().SetMother(mother_name);

  std::vector<std::string> volumes_to_exclude;
  std::vector<std::string> volumes_to_include1;
  std::vector<std::string> volumes_to_include2;



  // ------ Add all volumes containing the specified patterns

  GetSetup().AddVolume("Magnet_0");
  std::string TRACKERBASE = mother_name + oa_name + magnet_name + basket_name;
  // Farther volume
// GetSetup().AddVolume(TRACKERBASE);

  //  GetSetup().AddVolume("Coil");
  //  GetSetup().AddVolume("FluxReturn");
  //  GetSetup().AddVolume("MRD");

    GetSetup().AddVolume("Basket_0");




  // Add FGD volumes
  //NEED TO REVIEW
//  unsigned int NFGDVOLUMES = fFGDMODULES.size();
  volumes_to_exclude.clear();
/*
  if (fFGDMODULES1.size() > 0) {
    // The FGD parent volume
    GetSetup().AddVolume(TRACKERBASE, "FGD1_0");

    SetPropertiesToVolume(TRACKERBASE + "/FGD1_0",       volumes_to_exclude, volumes_to_exclude, false);

  }
  if (fFGDMODULES2.size() > 0) {
    // The FGD parent volume
    GetSetup().AddVolume(TRACKERBASE, "FGD2_0");
    SetPropertiesToVolume(TRACKERBASE + "/FGD2_0",       volumes_to_exclude, volumes_to_exclude, false);

  }

*/
  //if (fTARGET1.size() > 0) {
    // The FGD parent volume
    GetSetup().AddVolume(TRACKERBASE, "Target1_0");

    SetPropertiesToVolume(TRACKERBASE + "/Target1_0",       volumes_to_exclude, volumes_to_exclude, false);

 // }
 // if (fTARGET2.size() > 0) {
    // The FGD parent volume
    GetSetup().AddVolume(TRACKERBASE, "Target2_0");
    SetPropertiesToVolume(TRACKERBASE + "/Target2_0",       volumes_to_exclude, volumes_to_exclude, false);

  //}
  unsigned int NTPCMODULES1 = fTPCMODULES1.size();
  for (unsigned int i = 0; i < NTPCMODULES1; i++) { // The FGD parent volume

    GetSetup().AddVolume(fTPCMODULES1[i]);
    GetSetup().AddVolume(fTPCMODULES1[i], "Drift_0");
    GetSetup().AddVolume(fTPCMODULES1[i], "Drift_0/CentralCathode_0");
  }

  unsigned int NTPCMODULES2 = fTPCMODULES2.size();
  for (unsigned int i = 0; i < NTPCMODULES2; i++) { // The FGD parent volume

    GetSetup().AddVolume(fTPCMODULES2[i]);
    GetSetup().AddVolume(fTPCMODULES2[i], "Drift_0");
    GetSetup().AddVolume(fTPCMODULES2[i], "Drift_0/CentralCathode_0");
  }
  unsigned int NTPCMODULES3 = fTPCMODULES3.size();
  for (unsigned int i = 0; i < NTPCMODULES3; i++) { // The FGD parent volume

    GetSetup().AddVolume(fTPCMODULES3[i]);
    GetSetup().AddVolume(fTPCMODULES3[i], "Drift_0");
    GetSetup().AddVolume(fTPCMODULES3[i], "Drift_0/CentralCathode_0");
  }

  if (fTPCMODULES4.size() > 0) {
    // The FGD parent volume
    GetSetup().AddVolume(TRACKERBASE, "TPCUp1_0");
    SetPropertiesToVolume(TRACKERBASE + "/TPCUp1_0",       volumes_to_exclude, volumes_to_exclude, false);

  }
  if (fTPCMODULES5.size() > 0) {
    // The FGD parent volume
    GetSetup().AddVolume(TRACKERBASE, "TPCUp2_0");
    SetPropertiesToVolume(TRACKERBASE + "/TPCUp2_0",       volumes_to_exclude, volumes_to_exclude, false);

  }

  if (fTPCMODULES6.size() > 0) {
    // The FGD parent volume
    GetSetup().AddVolume(TRACKERBASE, "TPCDown1_0");
    SetPropertiesToVolume(TRACKERBASE + "/TPCDown1_0",       volumes_to_exclude, volumes_to_exclude, false);

  }
  if (fTPCMODULES7.size() > 0) {
    // The FGD parent volume
    GetSetup().AddVolume(TRACKERBASE, "TPCDown2_0");
    SetPropertiesToVolume(TRACKERBASE + "/TPCDown2_0",       volumes_to_exclude, volumes_to_exclude, false);

  }



  unsigned int NP0DVOLUMES = fP0DMODULES.size();
  const unsigned int NP0DSUBVOLUMES = 1;

  std::string p0d_subvolume[NP0DSUBVOLUMES] = {"USECal_0"};
  for (unsigned int i = 0; i < NP0DVOLUMES; i++) {

    GetSetup().AddVolume(fP0DMODULES[i]);
    for (unsigned int j = 0; j < NP0DSUBVOLUMES; j++) {

      // P0D module father volume
      if (j < 2) {
        // CECal_0 and USECal_0
        GetSetup().AddVolume(fP0DMODULES[i], p0d_subvolume[j]);
      }
      else {
        // CTarget_0 and USTarget_0   grouped into a virtual volume Target_0 to avoid overlap between real volumes
        volumes_to_include1.clear();
        volumes_to_exclude.clear();
        GetSetup().AddVirtualVolume(fP0DMODULES[i], p0d_subvolume[j], volumes_to_include1, volumes_to_exclude, true, false);
      }


      volumes_to_include2.clear();
      volumes_to_include2.push_back(p0d_subvolume[j]);

      volumes_to_include1.clear();
      volumes_to_include1.push_back("BigPlate");          // inside radiator for USECal and CECal
      volumes_to_include1.push_back("SmallPlate");        //  "
      volumes_to_include1.push_back("Epoxy");             // Inside P0Dule
      GetSetup().AddVirtualVolume(fP0DMODULES[i] + "/" + p0d_subvolume[j], "Active_0", volumes_to_include1, volumes_to_include2, true, true);



      volumes_to_include1.clear();
      volumes_to_include1.push_back("BottomLICover");
      volumes_to_include1.push_back("BottomLIFrame");
      GetSetup().AddVirtualVolume(fP0DMODULES[i] + "/" + p0d_subvolume[j], "BottomLI_0", volumes_to_include1, volumes_to_include2, true, true);

      volumes_to_include1.clear();
      volumes_to_include1.push_back("SideLIFrame");
      GetSetup().AddVirtualVolume(fP0DMODULES[i] + "/" + p0d_subvolume[j], "RightLI_0", volumes_to_include1, volumes_to_include2, true, true);



      volumes_to_include1.clear();
      volumes_to_include1.push_back("TopMPPCFrame");
      GetSetup().AddVirtualVolume(fP0DMODULES[i] + "/" + p0d_subvolume[j], "VirtTopMPPCFrame_0", volumes_to_include1, volumes_to_include2, true, true);

      volumes_to_include1.clear();
      volumes_to_include1.push_back("SideMPPCFrame");
      GetSetup().AddVirtualVolume(fP0DMODULES[i] + "/" + p0d_subvolume[j], "VirtLeftMPPCFrame_0", volumes_to_include1, volumes_to_include2, true, true);

      volumes_to_include1.clear();
      volumes_to_include1.push_back("TopLadder_0/Plate");   // cooling not considered
      volumes_to_include1.push_back("TopLadder_1/Plate");
      GetSetup().AddVirtualVolume(fP0DMODULES[i] + "/" + p0d_subvolume[j], "VirtTopLadder_0", volumes_to_include1, volumes_to_include2, true, true);

      volumes_to_include1.clear();
      volumes_to_include1.push_back("SideLadder_0/Plate");   // cooling not considered
      volumes_to_include1.push_back("SideLadder_1/Plate");
      volumes_to_include1.push_back("SideLadder_0/Small");
      volumes_to_include1.push_back("SideLadder_1/Small");
      GetSetup().AddVirtualVolume(fP0DMODULES[i] + "/" + p0d_subvolume[j], "VirtLeftLadder_0", volumes_to_include1, volumes_to_include2, true, true);

      // Brace not considered
    }
  }

  // Add ECAL volumes
  for (unsigned int i = 0; i < NTPCMODULES1; i++) {
    std::string tpc_name = fTPCMODULES1[i];

    // surface normal
    //    GetSetup().set_volume_property_to_sons(tpc_name,RP::SurfNormal,_surfNormal);

    // central cathode
    volumes_to_exclude.clear();
    SetPropertiesToVolume(tpc_name + "/GasGap_0/Drift_0/CentralCathode_0", volumes_to_exclude, volumes_to_exclude, false);
    SetPropertiesToVolume(tpc_name + "/GasGap_0/Drift_0", volumes_to_exclude, volumes_to_exclude, false);

  }
  // Add ECAL volumes
  for (unsigned int i = 0; i < NTPCMODULES2; i++) {
    std::string tpc_name = fTPCMODULES2[i];

    // surface normal
    //    GetSetup().set_volume_property_to_sons(tpc_name,RP::SurfNormal,_surfNormal);

    // central cathode
    volumes_to_exclude.clear();
    SetPropertiesToVolume(tpc_name + "/GasGap_0/Drift_0/CentralCathode_0", volumes_to_exclude, volumes_to_exclude, false);
    SetPropertiesToVolume(tpc_name + "/GasGap_0/Drift_0", volumes_to_exclude, volumes_to_exclude, false);

  }
  // Add ECAL volumes
  for (unsigned int i = 0; i < NTPCMODULES3; i++) {
    std::string tpc_name = fTPCMODULES3[i];

    // surface normal
    //    GetSetup().set_volume_property_to_sons(tpc_name,RP::SurfNormal,_surfNormal);

    // central cathode
    volumes_to_exclude.clear();
    SetPropertiesToVolume(tpc_name + "/GasGap_0/Drift_0/CentralCathode_0", volumes_to_exclude, volumes_to_exclude, false);
    SetPropertiesToVolume(tpc_name + "/GasGap_0/Drift_0", volumes_to_exclude, volumes_to_exclude, false);

  }

  unsigned int NECALVOLUMES = fECALMODULES.size();
  for (unsigned int i = 0; i < NECALVOLUMES; i++) {

    // ECAL ECAL father volume
    GetSetup().AddVolume(fECALMODULES[i]);

    // ECAL Active volume
    GetSetup().AddVolume(fECALMODULES[i], "Active_0");


    if (fECALMODULES[i].find("DsECal") != std::string::npos) {
      // DsECAL dead material
      GetSetup().AddVolume(fECALMODULES[i] + "/BasePanel_0");
    }
    else {
      // BrECAL and P0DECAL dead material
      GetSetup().AddVolume(fECALMODULES[i] + "/Lid_0");
      GetSetup().AddVolume(fECALMODULES[i] + "/Base_0");
    }
  }
  // exclude all volumes containing the pattern "/bar_"
  //use /Bar so not to exclude the coils
  GetSetup().ExcludeVolume("/Bar_");

  // Get the path of the magnet
  GetSetup().AddVolumeToGetPath("Magnet_0");


  // Build the RecPack setup following the previous instructions
  GetSetup().BuildSetup();

  // Check and solve overlaps smaller than 10 mm and leaving a tolerance of 0.01 mm
  GetSetup().solve_overlaps(10 * mm, 0.01 * mm);
  std::string magnet_name_ = GetSetup().GetVolumePath("Magnet_0");

  if (magnet_name_ == "")
    magnet_name_ = mother_name + "/Basket_0";


  //*************************** Set Volume and surface properties **************************************

  //---- Set the magnetic field
  _Bfield = EVector(3, 0);

  if ( magnet_name_ == mother_name + "/Basket_0" ) {
    _Bfield[0] = 0.0001 * tesla;  _Bfield[1] = 0.0 * tesla;  _Bfield[2] = 0.0 * tesla;
    GetSetup().set_volume_property_to_sons(magnet_name_, RP::BField, _Bfield);
  }
  else {
    // set this volume property to all sons including the volume itself
    GetSetup().set_volume_property_to_sons(magnet_name_, RP::BFieldMap, _BfieldMap);
    EVector pos(3, 0);
    EVector bv2 = _BfieldMap.compute_vector(pos);

    std::cout << "RecPackGeometry: used B field at pos 0: " << print(bv2) << std::endl;
  }


  for (unsigned int i = 0; i < NECALVOLUMES; i++) {
    std::string active_name = fECALMODULES[i] + "/Active_0";
    std::string ecal_name = fECALMODULES[i];

    volumes_to_include2.clear();



    if (ecal_name.find("DsECal") != std::string::npos) {
      // DsECAL dead material
      volumes_to_include1.clear();
      volumes_to_include1.push_back("/BasePanel_0/CarbonFibre");
      SetPropertiesToVolume(ecal_name + "/BasePanel_0", volumes_to_include1, volumes_to_include2, true);
    }
    else {
      // BrECAL and P0DECAL dead material
      volumes_to_include1.clear();
      volumes_to_include1.push_back("/Lid_0");
      SetPropertiesToVolume(ecal_name + "/Lid_0", volumes_to_include1, volumes_to_include2, true);
      volumes_to_include1.clear();
      volumes_to_include1.push_back("/Base_0/CarbonFibre");
      SetPropertiesToVolume(ecal_name + "/Base_0", volumes_to_include1, volumes_to_include2, true);
    }


    // ECAL active volume
    volumes_to_include1.clear();
    volumes_to_include2.clear();
    volumes_to_include1.push_back("Active_0");
    SetPropertiesToVolume(active_name, volumes_to_include1, volumes_to_include2, true);
  }



  for (unsigned int i = 0; i < NP0DVOLUMES; i++) {
    for (unsigned int j = 0; j < NP0DSUBVOLUMES; j++) {
      std::string active_name = fP0DMODULES[i] + "/" + p0d_subvolume[j] + "/Active_0";
      std::string p0d_name = fP0DMODULES[i] + "/" + p0d_subvolume[j];

      std::string father_name = p0d_name;
      // the father has to be a real volume (it cannot be P0D_0/Target_0)
      if (j == 2) father_name = fP0DMODULES[i];

      // surface normal
      //      GetSetup().set_volume_property_to_sons(fP0DMODULES[i],RP::SurfNormal,_surfNormal);


      volumes_to_include2.clear();
      volumes_to_include2.push_back(p0d_subvolume[j]);

      volumes_to_include1.clear();
      volumes_to_include1.push_back("BigPlate");          // inside radiator for USECal and CECal
      volumes_to_include1.push_back("SmallPlate");        //  "
      volumes_to_include1.push_back("SteelSkin");         //  "
      volumes_to_include1.push_back("Epoxy");             // Inside P0Dule
      volumes_to_include1.push_back("RightTarget");       // Inside CTarget/Target and USTarget/Target
      volumes_to_include1.push_back("LeftTarget");        //  "
      volumes_to_include1.push_back("Target_0/Radiator"); // This is the radiator inside CTarget_0 and USTarget_0
      SetPropertiesToVirtualVolume(father_name, active_name, volumes_to_include1, volumes_to_include2, true);


      volumes_to_include1.clear();
      volumes_to_include1.push_back("BottomLICover");
      volumes_to_include1.push_back("BottomLIFrame");
      SetPropertiesToVirtualVolume(father_name, p0d_name + "/BottomLI_0", volumes_to_include1, volumes_to_include2, true);

      volumes_to_include1.clear();
      volumes_to_include1.push_back("SideLICover");
      volumes_to_include1.push_back("SideLIFrame");
      SetPropertiesToVirtualVolume(father_name, p0d_name + "/RightLI_0", volumes_to_include1, volumes_to_include2, true);


      volumes_to_include1.clear();
      volumes_to_include1.push_back("TopMPPCFrame");
      SetPropertiesToVirtualVolume(father_name, p0d_name + "/VirtTopMPPCFrame_0", volumes_to_include1, volumes_to_include2, true);

      volumes_to_include1.clear();
      volumes_to_include1.push_back("SideMPPCFrame");
      SetPropertiesToVirtualVolume(father_name, p0d_name + "/VirtLeftMPPCFrame_0", volumes_to_include1, volumes_to_include2, true);

      volumes_to_include1.clear();
      volumes_to_include1.push_back("TopLadder_0/Plate");   // cooling not considered
      volumes_to_include1.push_back("TopLadder_1/Plate");
      SetPropertiesToVirtualVolume(father_name, p0d_name + "/VirtTopLadder_0", volumes_to_include1, volumes_to_include2, true);

      volumes_to_include1.clear();
      volumes_to_include1.push_back("SideLadder_0/Plate");   // cooling not considered
      volumes_to_include1.push_back("SideLadder_1/Plate");
      volumes_to_include1.push_back("SideLadder_0/Small");
      volumes_to_include1.push_back("SideLadder_1/Small");
      SetPropertiesToVirtualVolume(father_name, p0d_name + "/VirtLeftLadder_0", volumes_to_include1, volumes_to_include2, true);

      // Brace not considered
    }
  }


  if (!_initialised) {
    _initialised = true;
  }

  return;
}


//*****************************************************************************
void RecPackGeometry::SetPropertiesToVolume(const std::string& vol_name,
    const std::vector<std::string>& volumes_to_include1,
    const std::vector<std::string>& volumes_to_include2, bool include_exclude) {
  //*****************************************************************************


  bool father_considered = true;

  if (GetVerbosity() > 1) {
    std::cout << "Computing Avg properties of volume: " << vol_name << std::endl;
    std::cout << "   Recursive. Father included: " << father_considered << std::endl;
    if (include_exclude)
      std::cout << "  Include daugthers containing: " << std::endl;
    else
      std::cout << "  Exclude daugthers containing: " << std::endl;
    for (unsigned int j = 0; j < volumes_to_include1.size(); j++)
      std::cout << "   - " << volumes_to_include1[j] << std::endl;

    if (volumes_to_include2.size() > 0) {
      std::cout << "  and also: " << std::endl;

      for (unsigned int j = 0; j < volumes_to_include2.size(); j++)
        std::cout << "   - " << volumes_to_include2[j] << std::endl;
    }
  }


  // Compute and set the average X0 and dE/dx
  double dedx;
  double X0;
  double nel;
  double cap_considered;

  bool ok = GetSetup().GetAverageProperties(vol_name, volumes_to_include1, volumes_to_include2, include_exclude, father_considered, X0, dedx, nel, cap_considered);
  if (ok) {
    _de_dx_map[vol_name] = new DeDxMap(dedx * units::MeV / units::mm);
    if (GetVerbosity() > 2)
      _de_dx_map[vol_name]->set_verbosity(Messenger::VVERBOSE);
    _X0_map[vol_name] = X0;
    _nel_map[vol_name] = nel;
    GetSetup().set_volume_property(vol_name, RP::de_dx_map, *(_de_dx_map[vol_name]));
    GetSetup().set_volume_property(vol_name, RP::X0, _X0_map[vol_name]);
    GetSetup().set_volume_property(vol_name, RP::Nel, _nel_map[vol_name]);
  }

}


//*****************************************************************************
void RecPackGeometry::SetPropertiesToVirtualVolume(const std::string& father_name, const std::string& vol_name,
    const std::vector<std::string>& volumes_to_include1,
    const std::vector<std::string>& volumes_to_include2, bool include_exclude) {
  //*****************************************************************************

  bool father_considered = false;

  // the father name should be a real volume

  if (GetVerbosity() > 1) {
    std::cout << "Computing Avg properties of virtual volume: " << vol_name << std::endl;
    std::cout << "   Recursive. Father included: " << father_considered << std::endl;
    if (include_exclude)
      std::cout << "  Include daugthers containing: " << std::endl;
    else
      std::cout << "  Exclude daugthers containing: " << std::endl;
    for (unsigned int j = 0; j < volumes_to_include1.size(); j++)
      std::cout << "   - " << volumes_to_include1[j] << std::endl;

    if (volumes_to_include2.size() > 0) {
      std::cout << "  and also: " << std::endl;
      for (unsigned int j = 0; j < volumes_to_include2.size(); j++)
        std::cout << "   - " << volumes_to_include2[j] << std::endl;
    }
  }

  // Compute and set the average X0 and dE/dx
  double dedx;
  double X0;
  double nel;
  double cap_considered;

  bool ok = GetSetup().GetAverageProperties(father_name, volumes_to_include1, volumes_to_include2, include_exclude, father_considered, X0, dedx, nel, cap_considered);
  if (ok) {
    double cap_virt = GetSetup().volume(vol_name).capacity();
    double dedx_virt = dedx * cap_considered / cap_virt;
    double X0_virt   = X0 / (cap_considered / cap_virt);
    double nel_virt  = nel * cap_considered / cap_virt;

    if (GetVerbosity() > 1) {
      std::cout << "Final Avg Properties of virtual volume: " << vol_name << std::endl;
      std::cout << "  cap virtual (cm3): " << cap_virt / cm3 << std::endl;
      std::cout << "  cap_considered  :  " << cap_considered / cm3 << std::endl;
      std::cout << "  DeDx (MeV/mm):     " << dedx << std::endl;
      std::cout << "  DeDx virtual:      " << dedx_virt << std::endl;
      std::cout << "  X0 (mm):           " << X0 << std::endl;
      std::cout << "  X0 virtual:        " << X0_virt << std::endl;
      std::cout << "  Nel (e-/mm3)       " << nel << std::endl;
      std::cout << "  Nel virtual:       " << nel_virt << std::endl;
    }

    _de_dx_map[vol_name] = new DeDxMap(dedx_virt * units::MeV / units::mm);
    if (GetVerbosity() > 2)
      _de_dx_map[vol_name]->set_verbosity(Messenger::VVERBOSE);
    _X0_map[vol_name] = X0_virt;
    _nel_map[vol_name] = nel_virt;
    GetSetup().set_volume_property(vol_name, RP::de_dx_map, *(_de_dx_map[vol_name]));
    GetSetup().set_volume_property(vol_name, RP::X0, _X0_map[vol_name]);
    GetSetup().set_volume_property(vol_name, RP::Nel, _nel_map[vol_name]);
  }

}


//*****************************************************************************
void RecPackGeometry::DumpVolumeProperties() {
  //*****************************************************************************


  std::map<std::string, double>::const_iterator it;
  unsigned int lmax = 0;
  for (it = _X0_map.begin(); it != _X0_map.end(); it++)
    if (it->first.length() > lmax) lmax = it->first.length();

  std::cout << std::endl;
  std::cout << "------ List of volume properties: X0 ---------" << std::endl;
  std::cout << std::endl;
  for (it = _X0_map.begin(); it != _X0_map.end(); it++) {
    std::string sp = ""; for (unsigned int i = 0; i < lmax + 5 - it->first.length(); i++) sp += ".";
    std::cout << it->first << sp << " " << it->second << " mm" << std::endl;
  }

  std::map<std::string, DeDxMap*>::const_iterator it2;
  std::cout << std::endl;
  std::cout << "------ List of volume properties: dE/dx minimum ----------" << std::endl;
  std::cout << std::endl;
  for (it2 = _de_dx_map.begin(); it2 != _de_dx_map.end(); it2++) {
    std::string sp = ""; for (unsigned int i = 0; i < lmax + 5 - it2->first.length(); i++) sp += ".";
    std::cout << it2->first << sp << " " << it2->second->de_dx_min() << " MeV/mm" << std::endl;
  }

  std::cout << std::endl;
  std::cout << "------ List of volume properties: Nel (electron density) ---------" << std::endl;
  std::cout << std::endl;
  for (it = _nel_map.begin(); it != _nel_map.end(); it++) {
    std::string sp = ""; for (unsigned int i = 0; i < lmax + 5 - it->first.length(); i++) sp += ".";
    std::cout << it->first << sp << " " << it->second << " e-/mm3" << std::endl;
  }


}
