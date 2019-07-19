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
  fHitPE = TVector3(nd280upconv::kBadNum,nd280upconv::kBadNum,nd280upconv::kBadNum);
  fHitTime = TVector3(nd280upconv::kBadNum,nd280upconv::kBadNum,nd280upconv::kBadNum);
  fHitTrkID = nd280upconv::kBadNum;

  fDetectorXsize = fDetectorYsize = fDetectorZsize = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpApplyResponse::Delete(){
  fHitPos = TVector3(nd280upconv::kBadNum,nd280upconv::kBadNum,nd280upconv::kBadNum);
  fHitPE = TVector3(nd280upconv::kBadNum,nd280upconv::kBadNum,nd280upconv::kBadNum);
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
    
  double photX=0.; double distX=0.; // photons along fiber X; distance to MPPC from photons' source along X
  double photY=0.; double distY=0.; // photons along fiber Y; distance to MPPC from photons' source along Y
  double photZ=0.; double distZ=0.; // photons along fiber Z; distance to MPPC from photons' source along Z 
  double peX=0.; double peY=0.; double peZ=0.;// photoelectons produced in X, Y and Z MPPCs
  double timepeX=0.; double timepeY=0.; double timepeZ=0.;// time photoelectrons were produced in X, Y and Z MPPCs
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
    distX = abs(lightX - GetMPPCPosX(hitZ)); 
    distY = abs(lightY - GetMPPCPosY(hitZ)); 
    distZ = abs(lightZ - GetMPPCPosZ(hitX));     
  }
  
  //////////////////////////////////////////////
  //
  // TODO: Add more checks on the hit position
  //
  //////////////////////////////////////////////
  
  if( (distX>240 && distX!=nd280upconv::kUndefined) || 
      (distY>80 && distY!=nd280upconv::kUndefined) || 
      (distZ>480 && distZ!=nd280upconv::kUndefined) ){
    cout << "distX = " << distX 
	 << ", distY = " << distY 
	 << ", distZ = " << distZ 
	 << endl;
    cout << "ND280UpApplyResponse::CalcResponse" << endl;
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
    
    timepeX = prestepTime;
    timepeY = prestepTime;
    timepeZ = prestepTime;
    
    // light collection + attenuation
    TargetReadOut.ApplyFiberResponse(photX,timepeX,distX, fDetectorXsize); // along X fiber
    TargetReadOut.ApplyFiberResponse(photY,timepeY,distY, fDetectorYsize); // along Y fiber
    TargetReadOut.ApplyFiberResponse(photZ,timepeZ,distZ, fDetectorZsize); // along Z fiber
    
    // MPPC efficiency. Converts photons to PE
    peX = TargetReadOut.ApplyMPPCResponse(photX, 1);
    peY = TargetReadOut.ApplyMPPCResponse(photY, GetMPPCType(hitZ));
    peZ = TargetReadOut.ApplyMPPCResponse(photZ, 1);
    
  }
  else if( ftargetid==nd280upconv::kSciFi ||
	   ftargetid==nd280upconv::kFGDlike ){ // SciFi_XZ or FGDlike_XZ
    
    // SciFi_XZ or FGDlike_XZ (i.e. built in XY and rotated by 90deg on X axis)

    // Take the detector name along X and Z
    TString DetNameAlongX = GetDetNameAlongX(); 
    TString DetNameAlongZ = GetDetNameAlongZ();
    
    // Propagation only along X
    if(touch_namedet.Contains(DetNameAlongX)){ // "FiberScintHoriz"
      peX = photons; timepeX = prestepTime;
      peY = 0.; timepeY = 0.;
      peZ = 0.; timepeZ = 0.;	
      // light collection + attenuation
      TargetReadOut.ApplyFiberResponse(photX,timepeX,distX);	
      // MPPC efficiency. Converts photons to PE
      peX = TargetReadOut.ApplyMPPCResponse(photX);
    }
    // Propagation only along Z
    else if(touch_namedet.Contains(DetNameAlongZ)){ //"FiberScintVert"
      peX = 0.; timepeX = 0.;
      peY = 0.; timepeY = 0.;
      photZ = photons; timepeZ = prestepTime;	
      // light collection + attenuation
      TargetReadOut.ApplyFiberResponse(photZ,timepeZ,distZ);	
      // MPPC efficiency
      peZ = TargetReadOut.ApplyMPPCResponse(photZ);
    }
    else{
      cout << "ND280UpApplyResponse::ProcessHits" << endl;
      cout << "The hit is outside both direction fibers!!!" << endl;
      exit(1);    
    }            
  } // if SuperFGD || SciFi ...
  
  int trackid = trkid; 
  
  fHitPos = TVector3(hitX,hitY,hitZ); 
  fHitPE = TVector3(peX,peY,peZ);
  fHitTime = TVector3(timepeX,timepeY,timepeZ);
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
    if (Z < -80.)
      return 2;
    else if (Z < 0.)
      return 3;
    else if (Z < 240.)
      return 1;
    else{
      cout << "ND280UpApplyResponse::GetMPPCType" << endl;
      cout << "Z value outside of possible range" << endl;
      exit(1);
    }
  }
  else
    return 1;
}

///////////////////////////
