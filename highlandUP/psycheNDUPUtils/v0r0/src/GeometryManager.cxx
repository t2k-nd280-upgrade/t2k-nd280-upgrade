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
  _defaultFileName = "$(PSYCHEUTILSROOT)/data/v5r1p3-baseline-2010-11_geom-aa7ba9d3-c9bce01d-1965fe02-bb7afdbf-6c35814b.root";
  _useDefaultGeometry = ND::params().GetParameterD("psycheDUNEUtils.Geometry.UseDefaultGeometry");
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
  if (topName != "World") {
    std::cout<<"Geometry top volume has changed from t2k " << topName<<std::endl;
    top = gGeoManager->GetVolume("World_1");
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

  if (IsMC)
    return;

  // only prod6 should be affected
  if (!versionUtils::prod6_corrections)
    return;


  return;

} 


//*****************************************************************************
void GeometryManager::InitializeDetectorDefinitionFromROOTGeometry() const {
  //*****************************************************************************

  std::cout << "GeometryManager::InitializeDetectorDefinitionFromROOTGeometry(). Overwrite DetectorDefinition values with ROOT geometry !!!!" << std::endl;

  FillVolumesWithGeometryLoop();

  //  if (ND::params().GetParameterI("psycheDUNEUtils.Geometry.DetectorDefinitionFromROOTGeometry.FillSubdet1Info"))
  //    FillSubdet1InfoFromROOTGeometry(); 


  /* 
  // Get the SUBDET1_1 boundaries from the ROOT geometry
  SetDetectorBoundaries("SUBDET1_1",DetDef::Subdet1_1min, DetDef::Subdet1_1max, 
  "/t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/SUBDET1_1_0/ScintX_0",
  "/t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/SUBDET1_1_0/ScintY_14");

  // Get the SUBDET1_2 boundaries from the ROOT geometry
  SetDetectorBoundaries("SUBDET1_2", DetDef::Subdet1_2min, DetDef::Subdet1_2max, 
  "/t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/SUBDET1_2_0/ScintX_0",
  "/t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/SUBDET1_2_0/ScintY_6");

  */
}

//*****************************************************************************
void GeometryManager::FillTargetInfoFromROOTGeometry() const {
  //*****************************************************************************
 
  /*
  std::string full_path1 = "/t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/SUBDET1_1_0/";
  std::string full_path2 = "/t2k_1/OA_0/Magnet_0/Basket_0/Tracker_0/SUBDET1_2_0/";

  Double_t pos[3];
  Double_t size[3];

  int index = 0;
  
  // SUBDET1_1  
  // loop through the layers,  fill bars
  for (int i=0; i<15; i++){
    std::stringstream sstream;
    sstream << i;

    // get two positions
    // X layer
    //    GetVolumeProperties(full_path1 + "ScintX_" + sstream.str(), pos, size);
    //    DetDef::Subdet1_1BarCenter[index++] = pos[2];

    // Y layer
    //    GetVolumeProperties(full_path1 + "ScintY_" + sstream.str(), pos, size);
    //    DetDef::Subdet1_1BarCenter[index++] = pos[2];  
  }
  
  // SUBDET1_2  
  // loop through the layers,  fill bars
  for (int i=0; i<7; i++){
    std::stringstream sstream;
    sstream << i;

    // get two positions
    // X layer
    //    GetVolumeProperties(full_path2 + "ScintX_" + sstream.str(), pos, size);
    //    DetDef::Subdet1_2BarCenter[index++] = pos[2];

    // Y layer
    //    GetVolumeProperties(full_path2 + "ScintY_" + sstream.str(), pos, size);
    //    DetDef::Subdet1_2BarCenter[index++] = pos[2];  
  }

  //various module vars
  */  
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
  
  //Subdet2
  //Should be split into left and right across the cathode!!!
  if (name.find("ForwTPC1_0")     != std::string::npos
    //  && name.find("Drift") != std::string::npos) {
    ){
    UpdateVolumeBound(DetDef::ForwTPC1min, DetDef::ForwTPC1max, low, high);
    return false;
  }
  if (name.find("ForwTPC2_0")     != std::string::npos
     // && name.find("Drift") != std::string::npos) {
    ){
    UpdateVolumeBound(DetDef::ForwTPC2min, DetDef::ForwTPC2max, low, high);
    return false;
  }
  if (name.find("ForwTPC3_0")     != std::string::npos
   //   && name.find("Drift") != std::string::npos) {
  ){
    UpdateVolumeBound(DetDef::ForwTPC3min, DetDef::ForwTPC3max, low, high);
    return false;
  }
 if (name.find("TPCUp1_0")     != std::string::npos
   //   && name.find("Drift") != std::string::npos) {
  ){
    UpdateVolumeBound(DetDef::TPCUp1min, DetDef::TPCUp1max, low, high);
    return false;
  }
 if (name.find("TPCUp2_0")     != std::string::npos
   //   && name.find("Drift") != std::string::npos) {
  ){
    UpdateVolumeBound(DetDef::TPCUp2min, DetDef::TPCUp2max, low, high);
    return false;
  }
 if (name.find("TPCDown1_0")     != std::string::npos
   //   && name.find("Drift") != std::string::npos) {
  ){
    UpdateVolumeBound(DetDef::TPCDown1min, DetDef::TPCDown1max, low, high);
    return false;
  }
 if (name.find("TPCDown2_0")     != std::string::npos
   //   && name.find("Drift") != std::string::npos) {
  ){
    UpdateVolumeBound(DetDef::TPCDown2min, DetDef::TPCDown2max, low, high);
    return false;
  }
 if (name.find("Target1_0")     != std::string::npos
   //   && name.find("Drift") != std::string::npos) {
  ){
    UpdateVolumeBound(DetDef::Target1min, DetDef::Target1max, low, high);
    return false;
  }
 if (name.find("Target2_0")     != std::string::npos
   //   && name.find("Drift") != std::string::npos) {
  ){
    UpdateVolumeBound(DetDef::Target2min, DetDef::Target2max, low, high);
    return false;
  }

   


  //keeping digging the geometry down the hierarchy
  return true;
}

//*****************************************************************************
void GeometryManager::UpdateVolumeBound(Float_t* detMin, Float_t* detMax, const Float_t* low, const Float_t* high) const{
  //*****************************************************************************

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

  Float_t min[3] = {1e10, 1e10, 1e10};
  Float_t max[3] = {-1e10, -1e10, -1e10};

  //SUBDET1_1
  SetDetectorBoundaries(DetDef::TPCUp1min,      DetDef::TPCUp1max,    min, max); 

  //SUBDET1_2
  SetDetectorBoundaries(DetDef::TPCUp2min,      DetDef::TPCUp2max,    min, max); 

  //Subdet2_1
  SetDetectorBoundaries(DetDef::TPCDown1min,      DetDef::TPCDown1max,    min, max); 

  //Subdet2_2
  SetDetectorBoundaries(DetDef::TPCDown2min,      DetDef::TPCDown2max,    min, max); 

  //Subdet2_3
  SetDetectorBoundaries(DetDef::ForwTPC1min,      DetDef::ForwTPC1max,    min, max);

  SetDetectorBoundaries(DetDef::ForwTPC2min,      DetDef::ForwTPC2max,    min, max);

  SetDetectorBoundaries(DetDef::ForwTPC3min,      DetDef::ForwTPC3max,    min, max);

  SetDetectorBoundaries(DetDef::Target1min,      DetDef::Target1max,    min, max);

  SetDetectorBoundaries(DetDef::Target2min,      DetDef::Target2max,    min, max);

}
