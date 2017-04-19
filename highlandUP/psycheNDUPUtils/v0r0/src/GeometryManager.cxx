#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <memory>
#include <iomanip>
#include <typeinfo>
#include <cmath>
#include "GeometryManager.hxx"

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

#include <VersioningUtils.hxx>
#include <ND280UPAnalysisUtils.hxx>
#include <Parameters.hxx>

/// The static member pointer to the singleton.
GeometryManager* GeometryManager::fGeometryManager = NULL;

//*****************************************************************************
GeometryManager& ND::hgman(){
  //*****************************************************************************

  return GeometryManager::Get();
}

//*****************************************************************************
GeometryManager::GeometryManager(){
  //*****************************************************************************

  _GeoManagers.clear();
  _defaultFileName = "$(PSYCHENDUPUTILSROOT)/data/geometry.root";
  _useDefaultGeometry = ND::params().GetParameterD("psycheNDUPUtils.Geometry.UseDefaultGeometry");
  _currentGeomID=-1;
  
  
  }

//*****************************************************************************
GeometryManager& GeometryManager::Get() {
  //*****************************************************************************
  if (!fGeometryManager) {
    std::cout<<"Create a new GeometryManager object"<<std::endl;
    fGeometryManager = new GeometryManager;
  }
   
  return *fGeometryManager;
}

//*****************************************************************************
bool GeometryManager::LoadGeometry(const std::string& file_name0, Int_t geomID, const std::string& geomDir0) {
  //*****************************************************************************

  std::string file_name = file_name0;
  std::string geomDir = geomDir0;
  if (_useDefaultGeometry || file_name==""){
    file_name = fGeometryManager->_defaultFileName;
    geomDir = "";
    geomID = 0;
  }

  // If the requested geometry is already available just set its ID as current
  if (geomID>=0 && geomID<(Int_t)(_GeoManagers.size())){ 
    _currentGeomID = geomID;
    return false;
  }

  //get geometry from the input file
  TFile* file = TFile::Open(file_name.c_str());
  if(!file || !file->IsOpen())
    throw BadRootFileException();

  // Go to the appropriate directory in the root file
  file->cd(geomDir.c_str());

  //iterate through file contents
  TIter next(file->GetDirectory(geomDir.c_str())->GetListOfKeys());

  // Find the right key.
  TKey* defaultKey = NULL;
  TKey* key;

  while ((key = dynamic_cast<TKey*>(next()))) {
    if (!defaultKey && std::string(key->GetName()).find("ND280Geometry")!=std::string::npos) {
      defaultKey = key;
    }
  }

  key = defaultKey;

  if (!key)
    throw NoGeometryManagerException();

  // Don't load when the hash code already exists in the current geometry loaded: in this way just change the ID
  /*
  for (size_t i=0; i<_GeoManagers.size(); i++){
    if (_GeoManagers[i]->GetName() == key->GetName()){
      _currentGeomID = i;
      return false;
    }
  }
  */
  
  // Don't load when the hash code is the same
  if (_currentGeomID>=0 && _currentGeomID<(Int_t)(_GeoManagers.size()) && 
      _GeoManagers[_currentGeomID] && !std::strcmp(_GeoManagers[_currentGeomID]->GetName(), key->GetName())) return false;
  
  // Unprotect the geometry so we can load a new one.
  if (gGeoManager) gGeoManager->UnlockGeometry();

  TGeoManager *saveGeom = gGeoManager;
  TGeoManager *geom = dynamic_cast<TGeoManager*>(file->GetDirectory(geomDir.c_str())->Get(key->GetName()));
  if (!geom) {
    gGeoManager = saveGeom;
    throw NoGeometryManagerException();
  }

  if(!gGeoManager->GetCache()->HasIdArray()) 
    gGeoManager->GetCache()->BuildIdArray();

  //gGeoManager->GetCache()->BuildInfoBranch();

  // deal with  the top volume.
  TGeoVolume* top = gGeoManager->GetTopVolume();
  std::string topName(top->GetName());
  if (topName != "t2k") {
    std::cout<<"Geometry top volume has changed from t2k " << topName<<std::endl;
    top = gGeoManager->GetVolume("t2k");
    if (!top)
      throw NoGeometryManagerException();

    std::cout<<"Resetting top volume to " << top->GetName()<<std::endl;
    gGeoManager->SetTopVolume(top);
  }


  fGeometryManager->_GeoManagers.push_back(gGeoManager);

  gGeoManager->LockGeometry(); 

  file->Close();

  // increment the currentGeomID;
  _currentGeomID=fGeometryManager->_GeoManagers.size()-1;

  std::cout << "GeometryManager::LoadGeometry(). New Geometry with ID "<< _currentGeomID << " loaded. " << std::endl;
  std::cout << "GeometryManager::LoadGeometry(). Geometry Name = " << fGeometryManager->GeoManager()->GetName() << std::endl;

  if (ND::params().GetParameterI("psycheNDUPUtils.Geometry.DetectorDefinitionFromROOTGeometry"))
    InitializeDetectorDefinitionFromROOTGeometry();

  return true;
}

//*****************************************************************************
void GeometryManager::InitializeGeometry(bool IsMC) const {
  //*****************************************************************************
  // ECal volumes has significant data/MC differences,  up to ~3-4 cm for prod 6 (alignment applied but as-built geometry
  // has not yet been changed)
  // so set the appropriate values (substitute MC with data) when needed 

 // if (IsMC)
//    return;

  // only prod6 should be affected
 // if (!versionUtils::prod6_corrections)
   // return;

//  anaUtils::CopyArray(DetDef::tecalLmin_p6_data,   DetDef::tecalLmin,  3);

  //anaUtils::CopyArray(DetDef::tecalLmax_p6_data,   DetDef::tecalLmax,  3);

  //anaUtils::CopyArray(DetDef::tecalRmin_p6_data,   DetDef::tecalRmin,  3);

  //anaUtils::CopyArray(DetDef::tecalRmax_p6_data,   DetDef::tecalRmax,  3);
/*
  anaUtils::CopyArray(DetDef::tecalTLmin_p6_data,  DetDef::tecalTLmin, 3);

  anaUtils::CopyArray(DetDef::tecalTLmax_p6_data,  DetDef::tecalTLmax, 3);

  anaUtils::CopyArray(DetDef::tecalTRmin_p6_data,  DetDef::tecalTRmin, 3);

  anaUtils::CopyArray(DetDef::tecalTRmax_p6_data,  DetDef::tecalTRmax, 3);

  anaUtils::CopyArray(DetDef::tecalBLmin_p6_data,  DetDef::tecalBLmin, 3);

  anaUtils::CopyArray(DetDef::tecalBLmax_p6_data,  DetDef::tecalBLmax, 3);

  anaUtils::CopyArray(DetDef::tecalBRmin_p6_data,  DetDef::tecalBRmin, 3);

  anaUtils::CopyArray(DetDef::tecalBRmax_p6_data,  DetDef::tecalBRmax, 3);
*/
  return;

} 


//*****************************************************************************
void GeometryManager::InitializeDetectorDefinitionFromROOTGeometry() const {
  //*****************************************************************************

  std::cout << "GeometryManager::InitializeDetectorDefinitionFromROOTGeometry(). Overwrite DetectorDefinition values with ROOT geometry !!!!" << std::endl;

  FillVolumesWithGeometryLoop();

  //if (ND::params().GetParameterI("psycheNDUPUtils.Geometry.DetectorDefinitionFromROOTGeometry.FillFGDInfo"))
   // FillFGDInfoFromROOTGeometry(); 


  /* 
  // Get the FGD1 boundaries from the ROOT geometry
  SetDetectorBoundaries("FGD1",DetDef::fgd1min, DetDef::fgd1max, 
  "/t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/FGD1_0/ScintX_0",
  "/t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/FGD1_0/ScintY_14");

  // Get the FGD2 boundaries from the ROOT geometry
  SetDetectorBoundaries("FGD2", DetDef::fgd2min, DetDef::fgd2max, 
  "/t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/FGD2_0/ScintX_0",
  "/t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/FGD2_0/ScintY_6");

  // Get the DSECAL boundaries from the ROOT geometry
  SetDetectorBoundaries("DsECAL", DetDef::dsecalmin, DetDef::dsecalmax, "/t2k_1/OA_0/Magnet_0/Basket_0/DsECal_0/Active_0");

  // Get the TrECAL Right boundaries from the ROOT geometry
  SetDetectorBoundaries("TrECAL R", DetDef::tecalRmin, DetDef::tecalRmax, "/t2k_1/OA_0/Magnet_0/RightClam_0/BrlECal_1/Side_0/Active_0");

  // Get the TrECAL Left boundaries from the ROOT geometry
  SetDetectorBoundaries("TrECAL L", DetDef::tecalLmin, DetDef::tecalLmax, "/t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_1/Side_0/Active_0");

  // Get the TrECAL Top Right boundaries from the ROOT geometry
  SetDetectorBoundaries("TrECAL TR", DetDef::tecalRmin, DetDef::tecalRmax, "/t2k_1/OA_0/Magnet_0/RightClam_0/BrlECal_0/Top_0/Active_0");

  // Get the TrECAL Top Left boundaries from the ROOT geometry
  SetDetectorBoundaries("TrECAL TL", DetDef::tecalTLmin, DetDef::tecalTLmax, "/t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_0/Top_0/Active_0");

  // Get the TrECAL Bottom Right boundaries from the ROOT geometry
  SetDetectorBoundaries("TrECAL BR", DetDef::tecalRmin, DetDef::tecalRmax, "/t2k_1/OA_0/Magnet_0/RightClam_0/BrlECal_2/Bottom_0/Active_0");

  // Get the TrECAL Bottom Left boundaries from the ROOT geometry
  SetDetectorBoundaries("TrECAL BL", DetDef::tecalBLmin, DetDef::tecalBLmax, "/t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_2/Bottom_0/Active_0");

  std::cout << "---------------------------------------------------------------------------------------" << std::endl;
  */
}

//*****************************************************************************
void GeometryManager::FillFGDInfoFromROOTGeometry() const {
  //*****************************************************************************
 
 /* std::string full_path1 = "/t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/FGD1_0/";
  std::string full_path2 = "/t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/FGD2_0/";

  Double_t pos[3];
  Double_t size[3];

  int index = 0;
  
  // FGD1  
  // loop through the layers,  fill bars
  for (int i=0; i<15; i++){
    std::stringstream sstream;
    sstream << i;

    // get two positions
    // X layer
    GetVolumeProperties(full_path1 + "ScintX_" + sstream.str(), pos, size);
    DetDef::fgd1BarCenter[index++] = pos[2];

    // Y layer
    GetVolumeProperties(full_path1 + "ScintY_" + sstream.str(), pos, size);
    DetDef::fgd1BarCenter[index++] = pos[2];  
  }
  
  // FGD2  
  // loop through the layers,  fill bars
  for (int i=0; i<7; i++){
    std::stringstream sstream;
    sstream << i;

    // get two positions
    // X layer
    GetVolumeProperties(full_path2 + "ScintX_" + sstream.str(), pos, size);
    DetDef::fgd2BarCenter[index++] = pos[2];

    // Y layer
    GetVolumeProperties(full_path2 + "ScintY_" + sstream.str(), pos, size);
    DetDef::fgd2BarCenter[index++] = pos[2];  
  }

  // water bags
  for (int i=0; i<6; i++){
    std::stringstream sstream;
    sstream << i;

    GetVolumeProperties(full_path2 + "WaterGap_" + sstream.str(), pos, size);
    DetDef::fgd2WaterCenter[i] = pos[2];

  }
  
  //various module vars
  
  // bar width in Z
  GetVolumeProperties(full_path2 + "ScintX_0/Bar_0", pos, size);
  DetDef::fgdXYBarWidth = 2*fabs(size[2]);
  
  // glue between X and Y layers
  GetVolumeProperties(full_path2 + "GlueGap1_0", pos, size);
  DetDef::fgdXYGlueWidth = 2*fabs(size[2]);
  
  // glue before/after X/Y layers
  GetVolumeProperties(full_path2 + "GlueGap2_0", pos, size);
  DetDef::fgdXYMiddleGlueWidth = 2*fabs(size[2]);
  
  // glue for water bags
  GetVolumeProperties(full_path2 + "GlueGap3_0", pos, size);
  DetDef::fgdWaterGlueWidth = 2*fabs(size[2]);
  
  // air before/after X/Y layers
  GetVolumeProperties(full_path2 + "AirGap1_0", pos, size);
  DetDef::fgdXYAirWidth = 2*fabs(size[2]);
 
  // air before/after water bags
  GetVolumeProperties(full_path2 + "AirGap2_0", pos, size);
  DetDef::fgdWaterAirWidth = 2*fabs(size[2]);
  
  // G10 skin before/after X/Y layers 
  GetVolumeProperties(full_path2 + "G10Gap_0", pos, size);
  DetDef::fgdXYSkinWidth = 2*fabs(size[2]);
  
  // polypropelene skin
  GetVolumeProperties(full_path2 + "PropyleneGap_0", pos, size);
  DetDef::fgdWaterSkinWidth = 2*fabs(size[2]);
  
  // water bag
  GetVolumeProperties(full_path2 + "WaterGap_0", pos, size);
  DetDef::fgdWaterPCWidth = 2*fabs(size[2]);
  
  // XY module = G10_sheets + glue  + X layer + glue  + Y layer + glue  + G10_sheets
  DetDef::fgdXYModuleWidth = DetDef::fgdXYSkinWidth  +  DetDef::fgdXYGlueWidth + DetDef::fgdXYBarWidth +
    DetDef::fgdXYMiddleGlueWidth + DetDef::fgdXYBarWidth +   DetDef::fgdXYGlueWidth +  DetDef::fgdXYSkinWidth; 
  
  // water module = polypropylene + glue   + water/PC(polycarbonate) + glue   + polypropylene
  DetDef::fgdWaterModuleWidth = DetDef::fgdWaterSkinWidth + DetDef::fgdWaterGlueWidth + DetDef::fgdWaterPCWidth + 
    DetDef::fgdWaterGlueWidth + DetDef::fgdWaterSkinWidth; 

  
  //dump the results
  DetDef::DumpFGDInfo();*/
}

//*****************************************************************************
void GeometryManager::GetVolumeProperties(const std::string& name, Double_t* pos, Double_t* size) const {
  //*****************************************************************************

  TGeoManager* geom = fGeometryManager->GeoManager();
  int ini_node = geom->GetCurrentNodeId();

  geom->cd(name.c_str());
  TGeoNode* node = geom->GetCurrentNode();
  TGeoVolume* volume = node->GetVolume();
  TGeoBBox *shape =  static_cast<TGeoBBox*>(volume->GetShape());  

  double local[3] = {0,0,0};
  geom->LocalToMaster(local,pos);        

  Double_t size0[3];
  size0[0] = shape->GetDX();
  size0[1] = shape->GetDY();
  size0[2] = shape->GetDZ();


  while (geom->GetCurrentNodeId() > 1){

    node = geom->GetCurrentNode();

    const Double_t *rot =  node->GetMatrix()->GetRotationMatrix();
    for (int i=0;i<3;i++){
      size[i]=0;
      for (int j=0;j<3;j++){
        size[i] += rot[i*3+j]*size0[j];
      }
    }

    for (int i=0;i<3;i++)
      size0[i]=size[i];

    geom->CdUp();        

  }

  geom->CdNode(ini_node);

}

//*****************************************************************************
void GeometryManager::SetDetectorBoundaries(const std::string& det_name, Float_t* det_min, Float_t* det_max, 
    const std::string& name1, const std::string& name2) const {
  //*****************************************************************************

  double pos1[3];
  double size1[3];

  double pos2[3];
  double size2[3];


  for (Int_t i=0;i<3;i++){
    det_min[i]= 1e10;
    det_max[i]=-1e10;
  }  

  double temp[4][3];

  fGeometryManager->GetVolumeProperties(name1,pos1,size1);
  if (name2!="")
    fGeometryManager->GetVolumeProperties(name2,pos2,size2);

  for (Int_t i=0;i<3;i++){
    temp[0][i] = pos1[i]+size1[i];
    temp[1][i] = pos1[i]-size1[i];

    for (Int_t j=0;j<2;j++){
      if (temp[j][i]<det_min[i]) det_min[i]=temp[j][i];
      if (temp[j][i]>det_max[i]) det_max[i]=temp[j][i];
    }

    if (name2!=""){
      temp[2][i] = pos2[i]+size2[i];
      temp[3][i] = pos2[i]-size2[i];

      for (Int_t j=2;j<4;j++){
        if (temp[j][i]<det_min[i]) det_min[i]=temp[j][i];
        if (temp[j][i]>det_max[i]) det_max[i]=temp[j][i];
      }
    }


  }

  char out[256];
  sprintf(out," %-13s %10.3f %10.3f    %10.3f %10.3f    %10.3f %10.3f", det_name.c_str(), det_min[0], det_max[0], det_min[1], det_max[1], det_min[2], det_max[2]);
  std::cout << out << std::endl;


}


//*****************************************************************************
void GeometryManager::FillVolumesWithGeometryLoop() const {
  //*****************************************************************************

  //reset the geometries
  ResetVolumeDefinitions();

  TGeoManager* geom = fGeometryManager->GeoManager();

  //push the current state into the stack
  geom->PushPath();
  //go to top volume
  geom->CdTop();

  //navigate over geometry and build volumes 
  RecurseGeometry("");

  //return back the last pushed state
  gGeoManager->PopPath();

  DetDef::DumpVolumes();


}

//*****************************************************************************
void GeometryManager::RecurseGeometry(std::string name) const {
  //*****************************************************************************
  TGeoManager* geom = fGeometryManager->GeoManager();

  TGeoNode* node = geom->GetCurrentNode();
  name = name + "/" + node->GetName();

  if (!VisitNode(name, node)) {
    geom->CdUp();
    return;
  }

  // navigate through daughters
  for (int i=0; i<node->GetNdaughters(); ++i) {
    geom->CdDown(i);
    RecurseGeometry(name);
  }

  geom->CdUp();
}

//*****************************************************************************
bool GeometryManager::VisitNode(const std::string& name, const TGeoNode* node) const {
  //*****************************************************************************       
  TGeoManager* geom   = fGeometryManager->GeoManager();
  TGeoVolume *volume  = node->GetVolume();
  TGeoBBox* shape     = dynamic_cast<TGeoBBox*>(volume->GetShape());

  Double_t high_local[3]  = {shape->GetDX(),   shape->GetDY(), shape->GetDZ()};
  Double_t low_local[3]   = {-high_local[0],   -high_local[1], -high_local[2]};

  Double_t high_tmp[3];
  Double_t low_tmp[3];

  geom->LocalToMaster(low_local,  low_tmp);
  geom->LocalToMaster(high_local, high_tmp);

  Float_t high[3];
  Float_t low[3];

  std::copy(low_tmp,  low_tmp   + 3, low);
  std::copy(high_tmp, high_tmp  + 3, high);
  
  //Tracker overall envelop
  if (name.find("FGD")    != std::string::npos 
      || name.find("TPC") != std::string::npos){
    UpdateVolumeBound(DetDef::trackerAllmin,  DetDef::trackerAllmax,  low, high);
  }

  //P0D 
  //ToDo: review
  if (name.find("P0D_") != std::string::npos
      && name.find("Bar_") != std::string::npos) {
    UpdateVolumeBound(DetDef::usecalpmin, DetDef::usecalpmax, low, high);
    return false;
  }

  //TPC
  //Should be split into left and right across the cathode!!!
  if (name.find("ForwTPC1")     != std::string::npos
    //  && name.find("Drift") != std::string::npos) {
    ){
    UpdateVolumeBound(DetDef::ForwTPC1min, DetDef::ForwTPC1max, low, high);
    return false;
  }
  if (name.find("ForwTPC2")     != std::string::npos
     // && name.find("Drift") != std::string::npos) {
    ){
    UpdateVolumeBound(DetDef::ForwTPC2min, DetDef::ForwTPC2max, low, high);
    return false;
  }
  if (name.find("ForwTPC3")     != std::string::npos
   //   && name.find("Drift") != std::string::npos) {
  ){
    UpdateVolumeBound(DetDef::ForwTPC3min, DetDef::ForwTPC3max, low, high);
    return false;
  }
  if (name.find("TPCUp1")     != std::string::npos
   //   && name.find("Drift") != std::string::npos) {
  ){
    UpdateVolumeBound(DetDef::TPCUp1min, DetDef::TPCUp1max, low, high);
    return false;
  }
  if (name.find("TPCUp2")     != std::string::npos
   //   && name.find("Drift") != std::string::npos) {
  ){
    UpdateVolumeBound(DetDef::TPCUp1min, DetDef::TPCUp2max, low, high);
    return false;
  }
  if (name.find("TPCDown2")     != std::string::npos
   //   && name.find("Drift") != std::string::npos) {
  ){
    UpdateVolumeBound(DetDef::TPCDown2min, DetDef::TPCDown2max, low, high);
    return false;
  }
  if (name.find("TPCDown1")     != std::string::npos
   //   && name.find("Drift") != std::string::npos) {
  ){
    UpdateVolumeBound(DetDef::TPCDown1min, DetDef::TPCDown1max, low, high);
    return false;
  }

  //FGD1 
  if (name.find("FGD1")       != std::string::npos
      /*&& name.find("/Scint")  != std::string::npos*/) {
    UpdateVolumeBound(DetDef::FGD1min, DetDef::FGD1max, low, high);
    return false;
  }

  //FGD2
  if (name.find("FGD2")       != std::string::npos
     /* && name.find("/Scint")  != std::string::npos*/) {
    UpdateVolumeBound(DetDef::FGD2min, DetDef::FGD2max, low, high);
    return false;
  }


  //FGD1 
  if (name.find("Target1")       != std::string::npos
     /* && name.find("/Scint")  != std::string::npos*/) {
    UpdateVolumeBound(DetDef::Target1min, DetDef::Target1max, low, high);
    return false;
  }

  //FGD2
  if (name.find("Target2")       != std::string::npos
     /* && name.find("/Scint")  != std::string::npos*/) {
    UpdateVolumeBound(DetDef::Target2min, DetDef::Target2max, low, high);
    return false;
  }
   
  //DsECal
  if (name.find("DsECal")     != std::string::npos
      && name.find("/Active") != std::string::npos) {
    UpdateVolumeBound(DetDef::dsecalmin, DetDef::dsecalmax, low, high);
    return false;
  }

  //P0DECal
  if (name.find("/LeftClam_")   != std::string::npos
      && name.find("/P0DECal_") != std::string::npos
      && name.find("/Top_")     != std::string::npos
      && name.find("/Active")   != std::string::npos) {
    UpdateVolumeBound(DetDef::pecalTLmin, DetDef::pecalTLmax, low, high);
    return false;
  }
  if (name.find("/LeftClam_")   != std::string::npos
      && name.find("/P0DECal_") != std::string::npos
      && name.find("/Side_")    != std::string::npos
      && name.find("/Active")   != std::string::npos) {
    UpdateVolumeBound(DetDef::pecalLmin, DetDef::pecalLmax, low, high);
    return false;
  }
  if (name.find("/LeftClam_")   != std::string::npos
      && name.find("/P0DECal_") != std::string::npos
      && name.find("/Bottom_")  != std::string::npos
      && name.find("/Active_")  != std::string::npos) {
    UpdateVolumeBound(DetDef::pecalBLmin, DetDef::pecalBLmax, low, high);
    return false;
  }
  if (name.find("/RightClam_")  != std::string::npos
      && name.find("/P0DECal_") != std::string::npos
      && name.find("/Top_")     != std::string::npos
      && name.find("/Active")   != std::string::npos) {
    UpdateVolumeBound(DetDef::pecalTRmin, DetDef::pecalTRmax, low, high);
    return false;
  }
  if (name.find("/RightClam_")  != std::string::npos
      && name.find("/P0DECal_") != std::string::npos
      && name.find("/Side_")    != std::string::npos
      && name.find("/Active")   != std::string::npos) {
    UpdateVolumeBound(DetDef::pecalRmin, DetDef::pecalRmax, low, high);
    return false;
  }
  if (name.find("/RightClam_")  != std::string::npos
      && name.find("/P0DECal_") != std::string::npos
      && name.find("/Bottom_")  != std::string::npos
      && name.find("/Active_")  != std::string::npos) {
    UpdateVolumeBound(DetDef::pecalBRmin, DetDef::pecalBRmax, low, high);
    return false;
  }

  //BarrelECal
  if (name.find("/LeftClam_")   != std::string::npos
      && name.find("/BrlECal_") != std::string::npos
      && name.find("/Top_")     != std::string::npos
      && name.find("/Active")   != std::string::npos) {
    UpdateVolumeBound(DetDef::tecalTLmin, DetDef::tecalTLmax, low, high);
    return false;
  }
  if (name.find("/LeftClam_")   != std::string::npos
      && name.find("/BrlECal_") != std::string::npos
      && name.find("/Side_")    != std::string::npos
      && name.find("/Active")   != std::string::npos) {
    UpdateVolumeBound(DetDef::tecalLmin, DetDef::tecalLmax, low, high);
    return false;
  }
  if (name.find("/LeftClam_")   != std::string::npos
      && name.find("/BrlECal_") != std::string::npos
      && name.find("/Bottom_")  != std::string::npos
      && name.find("/Active_")  != std::string::npos) {
    UpdateVolumeBound(DetDef::tecalBLmin, DetDef::tecalBLmax, low, high);
    return false;
  }
  if (name.find("/RightClam_")  != std::string::npos
      && name.find("/BrlECal_") != std::string::npos
      && name.find("/Top_")     != std::string::npos
      && name.find("/Active")   != std::string::npos) {
    UpdateVolumeBound(DetDef::tecalTRmin, DetDef::tecalTRmax, low, high);
    return false;
  }
  if (name.find("/RightClam_")  != std::string::npos
      && name.find("/BrlECal_") != std::string::npos
      && name.find("/Side_")    != std::string::npos
      && name.find("/Active")   != std::string::npos) {
    UpdateVolumeBound(DetDef::tecalRmin, DetDef::tecalRmax, low, high);
    return false;
  }
  if (name.find("/RightClam_")  != std::string::npos
      && name.find("/BrlECal_") != std::string::npos
      && name.find("/Bottom_")  != std::string::npos
      && name.find("/Active_")  != std::string::npos) {
    UpdateVolumeBound(DetDef::tecalBRmin, DetDef::tecalBRmax, low, high);
    return false;
  }

  

  //keeping digging the geometry down the hierarchy
  return true;
}

//*****************************************************************************
void GeometryManager::UpdateVolumeBound(Float_t* detMin, Float_t* detMax, const Float_t* low, const Float_t* high) const{
  //*****************************************************************************
std::cout<<"update geometry"<<std::endl;

  for (int i=0; i<3; i++){
    //min
    detMin[i] = std::min(detMin[i], low[i]);
    detMin[i] = std::min(detMin[i], high[i]);
    //max
    detMax[i] = std::max(detMax[i], low[i]);
    detMax[i] = std::max(detMax[i], high[i]);
  }

}

//*****************************************************************************
void GeometryManager::ResetVolumeDefinitions() const {
  //*****************************************************************************     
std::cout<<"reset geometry"<<std::endl;
  Float_t min[3] = {1e10, 1e10, 1e10};
  Float_t max[3] = {-1e10, -1e10, -1e10};

  //FGD1
  SetDetectorBoundaries(DetDef::FGD1min,      DetDef::FGD1max,    min, max); 

  //FGD2
  SetDetectorBoundaries(DetDef::FGD2min,      DetDef::FGD2max,    min, max); 

  //Target1
  SetDetectorBoundaries(DetDef::Target1min,      DetDef::Target1max,    min, max); 

  //Target2
  SetDetectorBoundaries(DetDef::Target2min,      DetDef::Target2max,    min, max); 


  //TPC1
  SetDetectorBoundaries(DetDef::ForwTPC1min,      DetDef::ForwTPC1max,    min, max); 

  //TPC2
  SetDetectorBoundaries(DetDef::ForwTPC2min,      DetDef::ForwTPC2max,    min, max); 

  //TPC3
  SetDetectorBoundaries(DetDef::ForwTPC3min,      DetDef::ForwTPC3max,    min, max);


  //TPC1
  SetDetectorBoundaries(DetDef::TPCUp1min,      DetDef::TPCUp1max,    min, max); 

  //TPC2
  SetDetectorBoundaries(DetDef::TPCUp2min,      DetDef::TPCUp2max,    min, max); 

  //TPC3
  SetDetectorBoundaries(DetDef::TPCDown1min,      DetDef::TPCDown1max,    min, max);
  //TPC3
  SetDetectorBoundaries(DetDef::TPCDown2min,      DetDef::TPCDown2max,    min, max);


  //P0D
  SetDetectorBoundaries(DetDef::usecalpmin,       DetDef::usecalpmin,     min, max);

  //DsECal
  SetDetectorBoundaries(DetDef::dsecalmin,    DetDef::dsecalmax,  min, max);

  //BarrelECal
  SetDetectorBoundaries(DetDef::tecalLmin,    DetDef::tecalLmax,  min, max);
  SetDetectorBoundaries(DetDef::tecalRmin,    DetDef::tecalRmax,  min, max);

  SetDetectorBoundaries(DetDef::tecalTLmin,   DetDef::tecalTLmax,  min, max);
  SetDetectorBoundaries(DetDef::tecalTRmin,   DetDef::tecalTRmax,  min, max);

  SetDetectorBoundaries(DetDef::tecalBLmin,   DetDef::tecalBLmax,  min, max);
  SetDetectorBoundaries(DetDef::tecalBRmin,   DetDef::tecalBRmax,  min, max);

  //P0DECal
  SetDetectorBoundaries(DetDef::pecalLmin,    DetDef::pecalLmax,  min, max);
  SetDetectorBoundaries(DetDef::pecalRmin,    DetDef::pecalRmax,  min, max);

  SetDetectorBoundaries(DetDef::pecalTLmin,   DetDef::pecalTLmax,  min, max);
  SetDetectorBoundaries(DetDef::pecalTRmin,   DetDef::pecalTRmax,  min, max);

  SetDetectorBoundaries(DetDef::pecalBLmin,   DetDef::pecalBLmax,  min, max);
  SetDetectorBoundaries(DetDef::pecalBRmin,   DetDef::pecalBRmax,  min, max);

  
  //Tracker overall envelop
  SetDetectorBoundaries(DetDef::trackerAllmin,  DetDef::trackerAllmax,  min, max);
  

}
