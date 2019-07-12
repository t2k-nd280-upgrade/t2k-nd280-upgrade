#include <algorithm>
#include <TRandom.h>
#include <globals.hh>

#include <CLHEP/Random/Randomize.h>

#include "ND280UpApplyResponse.hh"
#include "ND280UpTargReadOut.hh"
  
using namespace std;

ND280UpApplyResponse::ND280UpApplyResponse()
{
  this->Init();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ND280UpApplyResponse::~ND280UpApplyResponse()
{
  this->Delete();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpApplyResponse::Init(){

  //persistencyManager = ND280RootPersistencyManager::GetInstance();
 
  ftargetid = nd280upconv::kUndefined;

  fHitPos = TVector3(nd280upconv::kBadNum,nd280upconv::kBadNum,nd280upconv::kBadNum);
  fHitPhot = TVector3(nd280upconv::kBadNum,nd280upconv::kBadNum,nd280upconv::kBadNum);
  fHitTime = TVector3(nd280upconv::kBadNum,nd280upconv::kBadNum,nd280upconv::kBadNum);
  fHitTrkID = nd280upconv::kBadNum;

  fDetectorXsize = fDetectorYsize = fDetectorZsize = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpApplyResponse::Delete(){
  fHitPos = TVector3(nd280upconv::kBadNum,nd280upconv::kBadNum,nd280upconv::kBadNum);
  fHitPhot = TVector3(nd280upconv::kBadNum,nd280upconv::kBadNum,nd280upconv::kBadNum);
  fHitTime = TVector3(nd280upconv::kBadNum,nd280upconv::kBadNum,nd280upconv::kBadNum);
  fHitTrkID = nd280upconv::kBadNum;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpApplyResponse::CalcResponse(TVector3 lightPos,int trkid,int parid,double charge,double prestepTime,double steplength, double edep,TString touch_namedet){

  (void)parid;

  if(ftargetid==nd280upconv::kUndefined){
    cout << "The Target ID is not defined!" << endl;
    exit(1);
  }

  // Set the target type
  ND280UpTargReadOut TargetReadOut;
  TargetReadOut.SetTargType(ftargetid);

  // find the light position in the local frame
  double lightX = lightPos.x();
  double lightY = lightPos.y();
  double lightZ = lightPos.z();
  
  // use the light position in the local frame to find the bin centers
  // of the hit in X, Y and Z
  double hitX = -9999999999;
  double hitY = -9999999999;
  double hitZ = -9999999999;
  
  if( ftargetid==nd280upconv::kSuperFGD ||
      ftargetid==nd280upconv::kProto ){ // SuperFGD
    //persistencyManager->GetHitPosXY(lightX,lightY,hitX,hitY);
    //persistencyManager->GetHitPosYZ(lightY,lightZ,hitY,hitZ);
    GetHitPosXY(lightX,lightY,hitX,hitY);
    GetHitPosYZ(lightY,lightZ,hitY,hitZ);
  }
  else if( ftargetid==nd280upconv::kSciFi ||
	   ftargetid==nd280upconv::kFGDlike ){ // SciFi_XZ or FGDlike_XZ
    //persistencyManager->GetHitPosXY(lightX,lightY,hitX,hitY);
    //persistencyManager->GetHitPosYZ(lightY,lightZ,hitY,hitZ);
    GetHitPosXY(lightX,lightY,hitX,hitY);
    GetHitPosYZ(lightY,lightZ,hitY,hitZ);
  }

  double edep_q = edep;
  
  // Calculate the # of photons that reach the MPPCs (X,Y,Z)
    
  double photX=0.; double distX=0.; double timephotX=0.;// photons along fiber X; distance to MPPC from photons' source along X; time photons reach the MPPC
  double photY=0.; double distY=0.; double timephotY=0.;// photons along fiber Y; distance to MPPC from photons' source along Y; time photons reach the MPPC 
  double photZ=0.; double distZ=0.; double timephotZ=0.;// photons along fiber Z; distance to MPPC from photons' source along Z; time photons reach the MPPC  

  // Calculate the distance from the photons' origin to the MPPCs 
  if( ftargetid==nd280upconv::kSuperFGD ||
      ftargetid==nd280upconv::kProto ){ // SuperFGD
    //distX = lightX - persistencyManager->GetMPPCPosX(); 
    //distY = lightY - persistencyManager->GetMPPCPosY(); 
    //distZ = lightZ - persistencyManager->GetMPPCPosZ(); 
    distX = lightX - GetMPPCPosX(hitZ); 
    distY = lightY - GetMPPCPosY(hitZ); 
    distZ = lightZ - GetMPPCPosZ(hitX); 
  }
  else if( ftargetid==nd280upconv::kSciFi ||
	   ftargetid==nd280upconv::kFGDlike ){ // SciFi_XZ or FGDlike_XZ    
    //distX = lightX - persistencyManager->GetMPPCPosX(); 
    //distY = lightY - persistencyManager->GetMPPCPosY(); 
    //distZ = lightZ - persistencyManager->GetMPPCPosZ();     
    distX = lightX - GetMPPCPosX(hitZ); 
    distY = lightY - GetMPPCPosY(hitZ); 
    distZ = lightZ - GetMPPCPosZ(hitX);     
  }
  
  //////////////////////////////////////////////
  //
  // TODO: Add more checks on the hit position
  //
  //////////////////////////////////////////////
  
  if( (distX<0 && distX!=nd280upconv::kUndefined) || 
      (distY<0 && distY!=nd280upconv::kUndefined) || 
      (distZ<0 && distZ!=nd280upconv::kUndefined) ){
    cout << "distX = " << distX 
	 << ", distY = " << distY 
	 << ", distZ = " << distZ 
	 << endl;
    cout << "ND280UpApplyResponse::ProcessHits" << endl;
    cout << "The hit is outside the target!!!" << endl;
    exit(1);
  }
  
  // light attenuation (Birks' formula) and conversion of edep to photons 
  double photons = TargetReadOut.ApplyScintiResponse(edep_q,steplength,charge); 
  if(steplength == -1){
    photons = edep;
  }

  if( ftargetid==nd280upconv::kSuperFGD ||
      ftargetid==nd280upconv::kProto ){ // SuperFGD
    photX = photons;
    photY = photons;
    photZ = photons;
    
    timephotX = prestepTime;
    timephotY = prestepTime;
    timephotZ = prestepTime;
    
    // light collection + attenuation
    TargetReadOut.ApplyFiberResponse(photX,timephotX,distX, fDetectorXsize); // along X fiber
    TargetReadOut.ApplyFiberResponse(photY,timephotY,distY, fDetectorYsize); // along Y fiber
    TargetReadOut.ApplyFiberResponse(photZ,timephotZ,distZ, fDetectorZsize); // along Z fiber
    
    // MPPC efficiency
    TargetReadOut.ApplyMPPCResponse(photX, 1);
    TargetReadOut.ApplyMPPCResponse(photY, GetMPPCType(hitZ));
    TargetReadOut.ApplyMPPCResponse(photZ, 1);
    
  }
  else if( ftargetid==nd280upconv::kSciFi ||
	   ftargetid==nd280upconv::kFGDlike ){ // SciFi_XZ or FGDlike_XZ
    
    // SciFi_XZ or FGDlike_XZ (i.e. built in XY and rotated by 90deg on X axis)

    // Take the detector name along X and Z
    TString DetNameAlongX = GetDetNameAlongX(); 
    TString DetNameAlongZ = GetDetNameAlongZ();
    
    // Propagation only along X
    if(touch_namedet.Contains(DetNameAlongX)){ // "FiberScintHoriz"
      photX = photons; timephotX = prestepTime;
      photY = 0.; timephotY = 0.;
      photZ = 0.; timephotZ = 0.;	
      // light collection + attenuation
      TargetReadOut.ApplyFiberResponse(photX,timephotX,distX);	
      // MPPC efficiency
      TargetReadOut.ApplyMPPCResponse(photX);
    }
    // Propagation only along Z
    else if(touch_namedet.Contains(DetNameAlongZ)){ //"FiberScintVert"
      photX = 0.; timephotX = 0.;
      photY = 0.; timephotY = 0.;
      photZ = photons; timephotZ = prestepTime;	
      // light collection + attenuation
      TargetReadOut.ApplyFiberResponse(photZ,timephotZ,distZ);	
      // MPPC efficiency
      TargetReadOut.ApplyMPPCResponse(photZ);
    }
    else{
      cout << "ND280UpApplyResponse::ProcessHits" << endl;
      cout << "The hit is outside both direction fibers!!!" << endl;
      exit(1);    
    }            
  } // if SuperFGD || SciFi ...
  
  int trackid = trkid; 
  
  fHitPos = TVector3(hitX,hitY,hitZ); 
  fHitPhot = TVector3(photX,photY,photZ);
  fHitTime = TVector3(timephotX,timephotY,timephotZ);
  fHitTrkID = trackid;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/////////////////////////////////
//                             //
// ND280RootPersistencyManager //
//                             //
/////////////////////////////////

void ND280UpApplyResponse::SetMPPCProj2D_XY(TH2F *h2d){
  fMPPCProj2D_XY = new TH2F(*h2d);
  if (!fDetectorXsize)
    fDetectorXsize = fMPPCProj2D_XY->GetXaxis()->GetBinUpEdge(fMPPCProj2D_XY->GetXaxis()->GetLast());
  if (!fDetectorYsize)
    fDetectorYsize = fMPPCProj2D_XY->GetYaxis()->GetBinUpEdge(fMPPCProj2D_XY->GetYaxis()->GetLast());
}
void ND280UpApplyResponse::SetMPPCProj2D_XZ(TH2F *h2d){
  fMPPCProj2D_XZ = new TH2F(*h2d);
  if (!fDetectorXsize)
    fDetectorXsize = fMPPCProj2D_XZ->GetXaxis()->GetBinUpEdge(fMPPCProj2D_XZ->GetXaxis()->GetLast());
  if (!fDetectorZsize)
    fDetectorZsize = fMPPCProj2D_XZ->GetYaxis()->GetBinUpEdge(fMPPCProj2D_XZ->GetYaxis()->GetLast());
}
void ND280UpApplyResponse::SetMPPCProj2D_YZ(TH2F *h2d){
  fMPPCProj2D_YZ = new TH2F(*h2d);
  if (!fDetectorYsize)
    fDetectorYsize = fMPPCProj2D_YZ->GetXaxis()->GetBinUpEdge(fMPPCProj2D_YZ->GetXaxis()->GetLast());
  if (!fDetectorZsize)
    fDetectorZsize = fMPPCProj2D_YZ->GetYaxis()->GetBinUpEdge(fMPPCProj2D_YZ->GetYaxis()->GetLast());
}

// Takes the point light was produced at and returns the centers of the X and Y bins the point is in
void ND280UpApplyResponse::GetHitPosXY(double lightX,double lightY, double &mppcX,double &mppcY){ 
  if(fMPPCProj2D_XY){
    int binX = fMPPCProj2D_XY->GetXaxis()->FindBin(lightX);
    mppcX = fMPPCProj2D_XY->GetXaxis()->GetBinCenter(binX);
    int binY = fMPPCProj2D_XY->GetYaxis()->FindBin(lightY);
    mppcY = fMPPCProj2D_XY->GetYaxis()->GetBinCenter(binY);  
  }
  else{
    cout << "The MPPC Projection XY doesn't exist in this detector!" << endl;
    cout << "ND280UpApplyResponse::GetHitPosXY" << endl;
    exit(1);
  }
}
								  
// Takes the point light was produced at and returns the centers of the X and Z bins the point is in
void ND280UpApplyResponse::GetHitPosXZ(double lightX,double lightZ, double &mppcX,double &mppcZ){
  if(fMPPCProj2D_XZ){
    int binX = fMPPCProj2D_XZ->GetXaxis()->FindBin(lightX);
    mppcX = fMPPCProj2D_XZ->GetXaxis()->GetBinCenter(binX);  
    int binZ = fMPPCProj2D_XZ->GetYaxis()->FindBin(lightZ);
    mppcZ = fMPPCProj2D_XZ->GetYaxis()->GetBinCenter(binZ);  
  }
  else{
    cout << "The MPPC Projection XZ doesn't exist in this detector!" << endl;
    cout << "ND280UpApplyResponse::GetHitPosXZ" << endl;
    exit(1);
  }
}

// Takes the point light was produced at and returns the centers of the Y and Z bins the point is in
void ND280UpApplyResponse::GetHitPosYZ(double lightY,double lightZ, double &mppcY,double &mppcZ){
  if(fMPPCProj2D_YZ){
    int binY = fMPPCProj2D_YZ->GetXaxis()->FindBin(lightY);
    mppcY = fMPPCProj2D_YZ->GetXaxis()->GetBinCenter(binY);  
    int binZ = fMPPCProj2D_YZ->GetYaxis()->FindBin(lightZ);
    mppcZ = fMPPCProj2D_YZ->GetYaxis()->GetBinCenter(binZ);  
  }
  else{
    cout << "The MPPC Projection YZ doesn't exist in this detector!" << endl;
    cout << "ND280UpApplyResponse::GetHitPosYZ" << endl;
    exit(1);
  }
}

TH2F *ND280UpApplyResponse::GetMPPCProj2D_XY(){ 
  if(!fMPPCProj2D_XY){
    cout << "The MPPC Projection XY doesn't exist in this detector!" << endl;
    cout << "ND280UpApplyResponse::GetMPPCProj2D_XY" << endl;
    exit(1);
  }
  return fMPPCProj2D_XY;
}

TH2F *ND280UpApplyResponse::GetMPPCProj2D_XZ(){ 
  if(!fMPPCProj2D_XZ){
    cout << "The MPPC Projection XZ doesn't exist in this detector!" << endl;
    cout << "ND280UpApplyResponse::GetMPPCProj2D_XZ" << endl;
    exit(1);
  }
  return fMPPCProj2D_XZ;
}

TH2F *ND280UpApplyResponse::GetMPPCProj2D_YZ(){ 
  if(fMPPCProj2D_YZ){
    cout << "The MPPC Projection YZ doesn't exist in this detector!" << endl;
    cout << "ND280UpApplyResponse::GetMPPCProj2D_YZ" << endl;
    exit(1);
  }

  return fMPPCProj2D_YZ;
}

// Assume the MPPC read-out plane is at x,y,z<0
// Assume at least 2 projections exist
double ND280UpApplyResponse::GetMPPCPosX(double Zpos){
  if(fMPPCProj2D_XY){
    if ( ftargetid==nd280upconv::kProto ){
      // account for staggering of MPPCs either side of the detector
      if ((int)floor(Zpos)%2 == 1)
        return fMPPCProj2D_XY->GetXaxis()->GetBinLowEdge(1);
      else
        return fMPPCProj2D_XY->GetXaxis()->GetBinUpEdge(fMPPCProj2D_XY->GetXaxis()->GetLast());
    }
    else
      return fMPPCProj2D_XY->GetXaxis()->GetBinLowEdge(1);
  }
  else if(fMPPCProj2D_XZ){
    if ( ftargetid==nd280upconv::kProto ){
      if ((int)floor(Zpos)%2 == 1)
        return fMPPCProj2D_XZ->GetXaxis()->GetBinLowEdge(1);
      else
        return fMPPCProj2D_XZ->GetXaxis()->GetBinUpEdge(fMPPCProj2D_XZ->GetXaxis()->GetLast());
    }
    else
      return fMPPCProj2D_XZ->GetXaxis()->GetBinLowEdge(1);
  }
  else{
    cout << "The projection for the MPPC X position is not available!" << endl;
    cout << "ND280UpApplyResponse::GetMPPCPosX" << endl;
    exit(1);
  }
  return 0;
}

double ND280UpApplyResponse::GetMPPCPosY(double Zpos){
  if(fMPPCProj2D_XY){
    if ( ftargetid==nd280upconv::kProto ){
      // account for staggering of MPPCs either side of the detector
      if ((int)floor(Zpos)%2 == 0)
        return fMPPCProj2D_XY->GetYaxis()->GetBinLowEdge(1);
      else
        return fMPPCProj2D_XY->GetYaxis()->GetBinUpEdge(fMPPCProj2D_XY->GetYaxis()->GetLast());
    }
    else
      return fMPPCProj2D_XY->GetYaxis()->GetBinLowEdge(1);
  }
  else if(fMPPCProj2D_YZ){
    if ( ftargetid==nd280upconv::kProto ){
      if ((int)floor(Zpos)%2 == 0)
        return fMPPCProj2D_YZ->GetYaxis()->GetBinLowEdge(1);
      else
        return fMPPCProj2D_YZ->GetYaxis()->GetBinUpEdge(fMPPCProj2D_YZ->GetYaxis()->GetLast());
    }
    else
      return fMPPCProj2D_YZ->GetYaxis()->GetBinLowEdge(1);
  }
  else{
    cout << "The projection for the MPPC Y position is not available!" << endl;
    cout << "ND280UpApplyResponse::GetMPPCPosY" << endl;
    exit(1);
  }
  return 0;
}

double ND280UpApplyResponse::GetMPPCPosZ(double Xpos){
  if(fMPPCProj2D_XZ){
    if ( ftargetid==nd280upconv::kProto ){
      // account for staggering of MPPCs either side of the prototype
      if ((int)floor(Xpos)%2 == 0)
        return fMPPCProj2D_XZ->GetZaxis()->GetBinLowEdge(1);
      else
        return fMPPCProj2D_XZ->GetZaxis()->GetBinUpEdge(fMPPCProj2D_XZ->GetZaxis()->GetLast());
    }
    else
      return fMPPCProj2D_XZ->GetZaxis()->GetBinLowEdge(1);
  }
  else if(fMPPCProj2D_YZ){
    if ( ftargetid==nd280upconv::kProto ){
      if ((int)floor(Xpos)%2 == 0)
        return fMPPCProj2D_YZ->GetZaxis()->GetBinLowEdge(1);
      else
        return fMPPCProj2D_YZ->GetZaxis()->GetBinUpEdge(fMPPCProj2D_YZ->GetZaxis()->GetLast());
    }
    else
      return fMPPCProj2D_XZ->GetZaxis()->GetBinLowEdge(1);
  }
  else{
    cout << "The projection for the MPPC Z position is not available!" << endl;
    cout << "ND280UpApplyResponse::GetMPPCPosZ" << endl;
    exit(1);
  }
  return 0;
}

// return the MPPC type in the XZ plane using the Z position of the MPPC
int ND280UpApplyResponse::GetMPPCType(double Z){
  if (ftargetid==nd280upconv::kProto){
    if (Z < 16.)
      return 2;
    else if (Z < 24.)
      return 3;
    else
      return 1;
  }
  else
    return 1;
}

///////////////////////////
