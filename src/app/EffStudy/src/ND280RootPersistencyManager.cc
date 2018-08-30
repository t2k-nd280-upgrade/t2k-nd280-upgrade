////////////////////////////////////////////////////////////
// $Id: ND280RootPersistencyManager.cc,v 1.93 2012/03/23 14:09:24 mcgrew Exp $
//
  
#include "ND280RootPersistencyManager.hh"
#include "ND280Trajectory.hh"
#include "ND280TrajectoryPoint.hh"
#include "ExN02VertexInfo.hh"
#include "ExN02Constants.hh"

#include "ExN02TrackerHit.hh"

#include <memory>
#include <cmath>
#include <algorithm>

#include <globals.hh>

#include <G4Event.hh>
#include <G4Run.hh>
#include <G4RunManager.hh>
#include <G4UnitsTable.hh>
#include <G4ParticleTable.hh>
#include <G4PrimaryVertex.hh>
#include <G4PrimaryParticle.hh>
#include <G4SDManager.hh>
#include <G4HCtable.hh>
#include <G4VisAttributes.hh>
#include <G4VSolid.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Sphere.hh>
#include <G4Polyhedra.hh>
#include <G4Trap.hh>
#include <G4StepStatus.hh>
#include <G4TransportationManager.hh>
#include <G4FieldManager.hh>

#include <TROOT.h>
#include <TPolyMarker3D.h>
#include <TFile.h>
#include <TTree.h>
#include <TGeoManager.h>
#include <TGeoVolume.h>
#include <TGeoMedium.h>
#include <TGeoPgon.h>

#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"

//#include "G4ProcessManager.hh"

TROOT root("ROOT","Root of Everything");

ND280RootPersistencyManager::ND280RootPersistencyManager() 
    : ND280PersistencyManager(), fOutput(NULL),
      fEventTree(NULL), fND280UpEvent(NULL), 
      fND280XMLInput(),fEventFirst(-99999),fNEvents(-99999),
      fEventsNotSaved(0),
      fNavigTarg1(NULL),fIsHistoMovedTarg1(false),fNavigDetExist(false),
      fMPPCProj2D_XY(NULL),fMPPCProj2D_XZ(NULL),fMPPCProj2D_YZ(NULL),
      fIsMPPCProjXY(false),fIsMPPCProjXZ(false),fIsMPPCProjYZ(false),
      fDetNameAlongX(""),fDetNameAlongY(""),fDetNameAlongZ(""),fModNum(0)
{}

ND280RootPersistencyManager* ND280RootPersistencyManager::GetInstance() {
    ND280RootPersistencyManager *current 
      = dynamic_cast<ND280RootPersistencyManager*>(G4VPersistencyManager::GetPersistencyManager());

    if (!current) current = new ND280RootPersistencyManager();
    return current;
}

ND280RootPersistencyManager::~ND280RootPersistencyManager() {
    if (fOutput) delete fOutput; fOutput = NULL;
    delete fND280XMLInput; fND280XMLInput = NULL;
}

bool ND280RootPersistencyManager::IsOpen() {
  if (fOutput && fOutput->IsOpen()) {
    fOutput->cd();
    return true;
  }
  return false;
}

void ND280RootPersistencyManager::OpenXML(G4String filename){
  fND280XMLInput = new ExN02ND280XML(filename); 
}

void ND280RootPersistencyManager::InitNavigator(G4VPhysicalVolume *logvolume,G4ThreeVector position){
  
  /*
    //
    // FROM GEANT4 GUIDE 4.10.1
    //
    G4Navigator methods:
    - SetWorldVolume()
      Sets the first volume in the hierarchy. It must be unrotated and untranslated from the origin.
    - LocateGlobalPointAndSetup()
      Locates the volume containing the specified global point. This involves a traverse of the hierarchy, 
      requiring the computation of compound transformations, testing replicated and parameterised volumes (etc). 
      To improve efficiency this search may be performed relative to the last, and this is the recommended way 
      of calling the function. A 'relative' search may be used for the first call of the function which will 
      result in the search defaulting to a search from the root node of the hierarchy. 
      Searches may also be performed using a G4TouchableHistory.
    - LocateGlobalPointAndUpdateTouchableHandle()
      First, search the geometrical hierarchy like the above method LocateGlobalPointAndSetup(). Then
      use the volume found and its navigation history to update the touchable.
    - ComputeStep()
      Computes the distance to the next boundary intersected along the specified unit 
      direction from a specified point. The point must be have been located prior to calling ComputeStep().
      When calling ComputeStep(), a proposed physics step is passed. If it can be determined that 
      the first intersection lies at or beyond that distance then kInfinity is returned. 
      In any case, if the returned step is greater than the physics step, the physics step must be taken.
    - SetGeometricallyLimitedStep()
      Informs the navigator that the last computed step was taken in its entirety. 
      This enables entering/exiting optimisation, and should be called prior to calling LocateGlobalPointAndSetup().
    - CreateTouchableHistory()
    Creates a G4TouchableHistory object, for which the caller has deletion responsibility. 
    The 'touchable' volume is the volume returned by the last Locate operation. 
    The object includes a copy of the current NavigationHistory, enabling the efficient relocation 
    of points in/close to the current volume in the hierarchy.  
  */
 
  if(GetNavigDetName_Targ1()==""){
    G4Exception("ND280RootPersistencyManager::InitNavigator",
    		"MyCode0002",FatalException,
    		"The volume for coordinate system has not been set yet!!!");
  }

  if( !fNavigTarg1 ){     
    fNavigTarg1 = new G4Navigator();
  }

  fNavigTarg1->SetWorldVolume(logvolume);
  fNavigTarg1->LocateGlobalPointAndSetup(position); // This chooses the local volume. Update it at each new hit!!!  
  
  G4TouchableHistoryHandle Touchable =  
    fNavigTarg1->CreateTouchableHistoryHandle(); 
  
  G4int depth = Touchable->GetHistory()->GetDepth();

  bool CoorSystVolExist = false;

  for (G4int i = 0; i<depth-1; ++i) {
    G4String touch_detname = Touchable->GetVolume()->GetLogicalVolume()->GetName();
    if(touch_detname == GetNavigDetName_Targ1()){
      CoorSystVolExist = true;
      break;            
    }
    Touchable->MoveUpHistory(1);
  }

  if(!CoorSystVolExist){
    G4Exception("ND280RootPersistencyManager::InitNavigator",
		"MyCode0002",FatalException,
		"The volume for coordinate system doesn't exist!!!");
  }

  //fNavigHistoTarg1 = Touchable->GetHistory(); // if I do this at the next hit it starts again from the most daughter volume
  fNavigHistoTarg1 = new G4NavigationHistory(*(Touchable->GetHistory()));
   
  SetHistoMovedTarg1(true); // define whether you can use the history or not


  /*
  G4String navig_namedet_0 = fNavigHistoTarg1->GetVolume(0)->GetName();
  G4String navig_namedet_2 = fNavigHistoTarg1->GetVolume(2)->GetName(); 
  G4cout << " "
	 << " - navig_namedet_0 = " << navig_namedet_0
	 << " - navig_namedet_2 = " << navig_namedet_2
	 << G4endl;

  G4String touch_namedet_0 = Touchable->GetVolume(0)->GetName();
  G4String touch_namedet_2 = Touchable->GetVolume(2)->GetName(); 
  G4cout << " "
	 << " - touch_namedet_0 = " << touch_namedet_0
	 << " - touch_namedet_2 = " << touch_namedet_2
	 << G4endl;
  exit(1);
  */

}


G4String ND280RootPersistencyManager::GetNavigHistoVolName(){ 
  int depth = fNavigHistoTarg1->GetDepth();
  return fNavigHistoTarg1->GetVolume(depth)->GetLogicalVolume()->GetName();
}

void ND280RootPersistencyManager::SetNavigDetName_Targ1(G4String name){
  fNavigDetName_Targ1=name;
  if(name=="") fNavigDetExist=false;
  else         fNavigDetExist=true;
  return;
}

G4ThreeVector ND280RootPersistencyManager::GetLocalPosition(G4ThreeVector WorldPos){
  //return GetNavigHistoTarg1()->GetTopTransform().TransformPoint(WorldPos);

  G4ThreeVector NotDefinedPoint(kBadNum,kBadNum,kBadNum);
  if(GetNavigHistoTarg1() &&
     doNavigDetExist() &&
     GetNavigTarg1()
     ){
    return GetNavigHistoTarg1()->GetTopTransform().TransformPoint(WorldPos);
  }
  else return NotDefinedPoint; 

}


/////////////////////////////////////
//                                 //
// MPPC stuffs for target responce //
//                                 //
/////////////////////////////////////


void ND280RootPersistencyManager::InitMPPCProj2D(double width, double height, double length, double numX, double numY, double numZ, bool IsProjXY=true, bool IsProjXZ=true, bool IsProjYZ=true, int modnum=1){
  
  SetIsMPPCProjXY(IsProjXY);
  SetIsMPPCProjXZ(IsProjXZ);
  SetIsMPPCProjYZ(IsProjYZ);
  
  double minX = -width/2.;
  double maxX = +width/2.;
  double minY = -height/2.;
  double maxY = +height/2.;
  double minZ = -length/2.;
  double maxZ = +length/2.;

  SetModNum(modnum);

  TString name;
  
  if(GetIsMPPCProjXY()){
    name = TString::Format("fMPPCProj2D_XY_mod_%02d",fModNum);
    fMPPCProj2D_XY 
      = new TH2F(name,name,numX,minX,maxX,numY,minY,maxY);
  }
  if(GetIsMPPCProjXZ()){
    name = TString::Format("fMPPCProj2D_XZ_mod_%02d",fModNum);
    fMPPCProj2D_XZ 
      = new TH2F(name,name,numX,minX,maxX,numZ,minZ,maxZ);
  }
  if(GetIsMPPCProjYZ()){
    name = TString::Format("fMPPCProj2D_YZ_mod_%02d",fModNum);
     fMPPCProj2D_YZ 
      = new TH2F(name,name,numY,minY,maxY,numZ,minZ,maxZ);
  }
}

void ND280RootPersistencyManager::GetHitPosXY(double lightX,double lightY, double &mppcX,double &mppcY){ 
  if(GetIsMPPCProjXY()){
    int binX = fMPPCProj2D_XY->GetXaxis()->FindBin(lightX);
    mppcX = fMPPCProj2D_XY->GetXaxis()->GetBinCenter(binX);
    int binY = fMPPCProj2D_XY->GetYaxis()->FindBin(lightY);
    mppcY = fMPPCProj2D_XY->GetYaxis()->GetBinCenter(binY);  
  }
  else{
    G4ExceptionDescription msg;
    msg << "The MPPC Projection XY doesn't exist in this detector!" << G4endl;
    G4Exception("ND280RootPersistencyManager::GetHitPosXY",
		"MyCode0002",FatalException, msg);
  }
}
								  
void ND280RootPersistencyManager::GetHitPosXZ(double lightX,double lightZ, double &mppcX,double &mppcZ){
  if(GetIsMPPCProjXZ()){
    int binX = fMPPCProj2D_XZ->GetXaxis()->FindBin(lightX);
    mppcX = fMPPCProj2D_XZ->GetXaxis()->GetBinCenter(binX);  
    int binZ = fMPPCProj2D_XZ->GetYaxis()->FindBin(lightZ);
    mppcZ = fMPPCProj2D_XZ->GetYaxis()->GetBinCenter(binZ);  
  }
  else{
    G4ExceptionDescription msg;
    msg << "The MPPC Projection XZ doesn't exist in this detector!" << G4endl;
    G4Exception("ND280RootPersistencyManager::GetHitPosXZ",
		"MyCode0002",FatalException, msg);
  }
}

void ND280RootPersistencyManager::GetHitPosYZ(double lightY,double lightZ, double &mppcY,double &mppcZ){
  if(GetIsMPPCProjYZ()){
    int binY = fMPPCProj2D_YZ->GetXaxis()->FindBin(lightY);
    mppcY = fMPPCProj2D_YZ->GetXaxis()->GetBinCenter(binY);  
    int binZ = fMPPCProj2D_YZ->GetYaxis()->FindBin(lightZ);
    mppcZ = fMPPCProj2D_YZ->GetYaxis()->GetBinCenter(binZ);  
  }
  else{
    G4ExceptionDescription msg;
    msg << "The MPPC Projection YZ doesn't exist in this detector!" << G4endl;
    G4Exception("ND280RootPersistencyManager::GetHitPosYZ",
		"MyCode0002",FatalException, msg);
  }
}

TH2F *ND280RootPersistencyManager::GetMPPCProj2D_XY(){ 
  if( !GetIsMPPCProjXY() ){
    G4ExceptionDescription msg;
    msg << "The MPPC Projection XY doesn't exist in this detector!" << G4endl;
    G4Exception("ND280RootPersistencyManager::GetMPPCProj2D_XY",
		"MyCode0002",FatalException, msg);
  }
  return fMPPCProj2D_XY;
}

TH2F *ND280RootPersistencyManager::GetMPPCProj2D_XZ(){ 
  if( !GetIsMPPCProjXZ() ){
    G4ExceptionDescription msg;
    msg << "The MPPC Projection XZ doesn't exist in this detector!" << G4endl;
    G4Exception("ND280RootPersistencyManager::GetMPPCProj2D_XZ",
		"MyCode0002",FatalException, msg);
  }
  return fMPPCProj2D_XZ;
}

TH2F *ND280RootPersistencyManager::GetMPPCProj2D_YZ(){ 
  if( !GetIsMPPCProjYZ() ){
    G4ExceptionDescription msg;
    msg << "The MPPC Projection YZ doesn't exist in this detector!" << G4endl;
    G4Exception("ND280RootPersistencyManager::GetMPPCProj2D_YZ",
		"MyCode0002",FatalException, msg);
  }

  return fMPPCProj2D_YZ;
}

// Assume the MPPC read-out plane is at x,y,z<0
// Assume at least 2 projections exist
G4double ND280RootPersistencyManager::GetMPPCPosX(){
  if( GetIsMPPCProjXY() ){
    return fMPPCProj2D_XY->GetXaxis()->GetBinLowEdge(1);
  }
  else if( GetIsMPPCProjXZ() ){
    return fMPPCProj2D_XZ->GetXaxis()->GetBinLowEdge(1);
  }
  else{
    G4ExceptionDescription msg;
    msg << "The projection for the MPPC X position is not available!" << G4endl;
    G4Exception("ND280RootPersistencyManager::GetMPPCPosX",
		"MyCode0002",FatalException, msg);
  }
  return 0;
}

G4double ND280RootPersistencyManager::GetMPPCPosY(){
  if( GetIsMPPCProjXY() ){
    return fMPPCProj2D_XY->GetYaxis()->GetBinLowEdge(1);
  }
  else if( GetIsMPPCProjYZ() ){
    return fMPPCProj2D_YZ->GetXaxis()->GetBinLowEdge(1);
  }
  else{
    G4ExceptionDescription msg;
    msg << "The projection for the MPPC Y position is not available!" << G4endl;
    G4Exception("ND280RootPersistencyManager::GetMPPCPosY",
		"MyCode0002",FatalException, msg);
  }
  return 0;
}

G4double ND280RootPersistencyManager::GetMPPCPosZ(){

  if( GetIsMPPCProjXZ() ){
    return fMPPCProj2D_XZ->GetYaxis()->GetBinLowEdge(1);
  }
  else if( GetIsMPPCProjYZ() ){
    return fMPPCProj2D_YZ->GetYaxis()->GetBinLowEdge(1);
  }
  else{
    G4ExceptionDescription msg;
    msg << "The projection for the MPPC Z position is not available!" << G4endl;
    G4Exception("ND280RootPersistencyManager::GetMPPCPosZ",
		"MyCode0002",FatalException, msg);
  }
  return 0;
}


// B.Q
void ND280RootPersistencyManager::AddPhysicalVolumeInformation(G4int dID0, G4String dName0, G4ThreeVector mppcPosition)
 {
   PVInfo = new ND280PhysicalVolumeInformation(dID0, dName0, mppcPosition);
   vPVInfo.push_back(*PVInfo);
   delete PVInfo;
}








/////////////////////////////////////
//                                 //
//             Output              //
//                                 //
/////////////////////////////////////



bool ND280RootPersistencyManager::Open(G4String filename) {
  
  if (fOutput) {
    G4cout << "ND280RootPersistencyManager::Open "
	   << "-- Delete current file pointer" 
	   << G4endl;
  }
  
  SetFilename(filename +".root");
  
  G4cout << "ND280RootPersistencyManager::Open " << GetFilename() << G4endl;
  
  //
  // TFile::Open() crashes!!!
  //
  //fOutput = TFile::Open(GetFilename(),"RECREATE","T2K-ND280 upgrade Root Output");
  //
  fOutput = new TFile(GetFilename().c_str(),"RECREATE");
  
  fEventTree = new TTree("ND280upEvents","ND280 upgrade Event Tree");
  
  //fND280UpEvent = new TND280UpEvent();    
  fEventTree->Branch("Event","TND280UpEvent",&fND280UpEvent,128000,0);
  
  fEventsNotSaved = 0;
  
  return true;
}

bool ND280RootPersistencyManager::Close() {
  if (!fOutput) {
    G4ExceptionDescription msg;
    msg << "No Output File" << G4endl; 
    G4Exception("ND280RootPersistencyManager::Close",
		"ExN02Code001",FatalException, msg);
    return false;
  }
  
  fOutput->cd();
  fOutput->Write(); // store the tree (actually all the tobjects in memory)
  fOutput->Close();
  
  G4cout << "Output file " << GetFilename() << " closed." << G4endl;
  
  fEventTree = NULL;
  fND280UpEvent = NULL;

  delete fEventTree;
  delete fND280UpEvent;
  delete fND280XMLInput; fND280XMLInput=NULL;

  return true;
}

bool ND280RootPersistencyManager::Store(const G4Event* anEvent) {

  //G4ProcessManager* pmanager = particle->GetProcessManager();
  //pmanager->
  //GetProcessList;


  //
  // Get the stored hits
  //
  
  G4HCofThisEvent* hce = anEvent->GetHCofThisEvent();
  if (!hce) 
    {
      G4ExceptionDescription msg;
      msg << "No hits collection of this event found." << G4endl; 
      G4Exception("ExN02EventAction::EndOfEventAction()",
		  "ExN02Code001", JustWarning, msg);
      return false;
    }   
  
  if (!hce) return false;
  G4SDManager *sdM = G4SDManager::GetSDMpointer();
  G4HCtable *hcT = sdM->GetHCtable();

  if (!fOutput) {
    G4ExceptionDescription msg;
    msg << "No Output File" << G4endl; 
    G4Exception("ND280RootPersistencyManager::Store",
   		"ExN02Code001",FatalException, msg);
    return false;
  }  

  fND280UpEvent = new TND280UpEvent();
  // The event is constructed using an auto ptr since we must delete it
  // before leaving this method.                            
  //std::auto_ptr<TND280UpEvent> fND280UpEvent(new TND280UpEvent());  
  fND280UpEvent->SetEventID(anEvent->GetEventID());  
  




     
  //
  // Store the hits
  // 

  for (int i=0; i<hcT->entries(); ++i) {                                          
                                            
    G4String SDname = hcT->GetSDname(i);
    G4String HCname = hcT->GetHCname(i);
    
    // The collection name is given by <detector name>/<Primitive Scorer name>.    

    G4cout << "Sensitive detector: " << SDname+"/"+HCname << G4endl;
    
    int HCId = sdM->GetCollectionID(SDname+"/"+HCname);
    G4VHitsCollection* g4Hits = hce->GetHC(HCId);
    if (g4Hits->GetSize()<1) continue;
    
    G4int n_hit = g4Hits->GetSize();
    std::cout << "# of hits = " << n_hit << std::endl;
    
    for (unsigned int h=0; h<g4Hits->GetSize(); ++h) {                 

      ExN02TrackerHit* g4Hit = dynamic_cast<ExN02TrackerHit*>(g4Hits->GetHit(h));
      TND280UpHit *nd280Hit = dynamic_cast<TND280UpHit*>(g4Hit); // ExN02TrackerHit inherits from TND280UpHit
      
      // G4cout << "StartX=" << nd280Hit->GetStartX() << ", "
      // 	     << "StartY=" << nd280Hit->GetStartY() << ", "
      // 	     << "StartZ=" << nd280Hit->GetStartZ() << ", "
      // 	     << "StopX=" << nd280Hit->GetStopX() << ", "
      // 	     << "StopY=" << nd280Hit->GetStopY() << ", "
      // 	     << "StopZ=" << nd280Hit->GetStopZ() << ", "
      // 	     << "EnergyDeposit=" << nd280Hit->GetEnergyDeposit() << ", "
      // 	     << "TrackLength=" << nd280Hit->GetTrackLength() << ", "
      // 	     << "TrackID=" << nd280Hit->fContributors.front() << G4endl;
     
      G4int parentid = nd280Hit->GetPrimaryId(); 
      G4int detid = kBadNum; 
      G4int pdg = kBadNum; 
      G4int trkid = nd280Hit->fContributors.front(); 
      G4double edep = nd280Hit->GetEnergyDeposit(); 
      G4double charge = 1.; // Apply alway Birks! 
      G4double time = (nd280Hit->GetStartT() + nd280Hit->GetStopT())/2.; // middle step time
      G4double posX = (nd280Hit->GetStartX() + nd280Hit->GetStopX())/2.; // middle step X
      G4double posY = (nd280Hit->GetStartY() + nd280Hit->GetStopY())/2.; // middle step Y 
      G4double posZ = (nd280Hit->GetStartZ() + nd280Hit->GetStopZ())/2.; // middle step Z
      G4ThreeVector PMworldPosition(posX,posY,posZ);
      string detname = nd280Hit->GetDetName();
      G4double steplength = nd280Hit->GetTrackLength();

      //cout << "The tracks contributing to the hit (trkid=" << trkid << ", pdg=" << nd280Hit->GetPDG() << ") are " << endl;
      //for(int icont=0;icont<nd280Hit->fContributors.size();icont++){
      //cout << nd280Hit->fContributors[icont] << ", ";
      //}
      //cout << endl;


      // Calculate the position in the target reference system --> needed for response
      G4ThreeVector lightPos = GetLocalPosition(PMworldPosition); 
      
      /*

       // CHANGED

      ExN02ApplyResponse ApplyResponse;    
      ApplyResponse.CalcResponse(lightPos,trkid,parentid,charge,time,steplength,edep,detname);

      double pex = ApplyResponse.GetHitPE().x();
      double pey = ApplyResponse.GetHitPE().y();
      double pez = ApplyResponse.GetHitPE().z();
      
      double timepex = ApplyResponse.GetHitTime().x();
      double timepey = ApplyResponse.GetHitTime().y();
      double timepez = ApplyResponse.GetHitTime().z();
      
      double poshitX = ApplyResponse.GetHitPos().x();
      double poshitY = ApplyResponse.GetHitPos().y();
      double poshitZ = ApplyResponse.GetHitPos().z();

      trkid = ApplyResponse.GetHitTrkID();
      */


      // true
      nd280Hit->SetLocPosX(lightPos.x()); // now in target system
      nd280Hit->SetLocPosY(lightPos.y()); // now in target system
      nd280Hit->SetLocPosZ(lightPos.z()); // now in target system
      G4double startX = nd280Hit->GetStartX(); // world reference system
      G4double startY = nd280Hit->GetStartY(); // world reference system
      G4double startZ = nd280Hit->GetStartZ(); // world reference system
      G4ThreeVector start(startX,startY,startZ);
      G4ThreeVector startLoc = GetLocalPosition(start); 
      nd280Hit->SetStartX(startLoc.x()); // now in target system
      nd280Hit->SetStartY(startLoc.y()); // now in target system
      nd280Hit->SetStartZ(startLoc.z()); // now in target system
      G4double stopX = nd280Hit->GetStopX(); // world reference system
      G4double stopY = nd280Hit->GetStopY(); // world reference system
      G4double stopZ = nd280Hit->GetStopZ(); // world reference system
      G4ThreeVector stop(stopX,stopY,stopZ);
      G4ThreeVector stopLoc = GetLocalPosition(stop); 
      nd280Hit->SetStopX(stopLoc.x()); // now in target system
      nd280Hit->SetStopY(stopLoc.y()); // now in target system
      nd280Hit->SetStopZ(stopLoc.z()); // now in target system

      //cout << "edep" << edep << endl;

      //nd280Hit->SetTime(time);
      //nd280Hit->SetDetName(detname);      

      // reco
      /*

	// CHANGED
	
	nd280Hit->SetPEX(pex);
      nd280Hit->SetPEY(pey);
      nd280Hit->SetPEZ(pez);
      nd280Hit->SetMPPCPosX(poshitX);
      nd280Hit->SetMPPCPosY(poshitY);
      nd280Hit->SetMPPCPosZ(poshitZ);
      nd280Hit->SetTimePEX(timepex);
      nd280Hit->SetTimePEY(timepey);
      nd280Hit->SetTimePEZ(timepez);
      */
      //
      
      fND280UpEvent->AddHit(nd280Hit);
      
    } // end loop over hits    
  } // end loop over hit containers





  









  //
  // Store the primary Vertices
  //
  
  G4int vtxNumber=0;
  for (G4PrimaryVertex* vtx = anEvent->GetPrimaryVertex();vtx;vtx = vtx->GetNext()) {
    
    TND280UpVertex *nd280Vertex = new TND280UpVertex("PrimaryVertex");
    nd280Vertex->SetVertexID(vtxNumber);
      
    ExN02VertexInfo* vInfo 
      = dynamic_cast<ExN02VertexInfo*>(vtx->GetUserInformation());
    //= (ExN02VertexInfo*)vtx->GetUserInformation();
          
    // Loop over particles outgoing the vertex 
    for (G4int p=0; p<vtx->GetNumberOfParticle(); ++p) {
     
      // Define the vertex track
      TND280UpTrack *nd280VtxTrack = new TND280UpTrack();
      
      G4PrimaryParticle* prim = vtx->GetPrimary(p);
      G4ParticleDefinition* partDef = prim->GetG4code();
      G4ThreeVector dir = prim->GetMomentum().unit();
      
      double momX = prim->GetMomentum().x();
      double momY = prim->GetMomentum().y();
      double momZ = prim->GetMomentum().z();
      nd280VtxTrack->SetInitMom(momX,momY,momZ);

      int pdg = prim->GetPDGcode();
      nd280VtxTrack->SetPDG(pdg);
      
      // Fill the vertex with the outgoing track
      nd280Vertex->AddOutTrack(nd280VtxTrack);
   
      //delete nd280VtxTrack; 
      //nd280VtxTrack=NULL;
    }
    
    // Get the Incoming Vertex
    
    if(vInfo){
      
      const G4PrimaryVertex *incvtx = vInfo->GetInformationalVertex();
      
      // Loop over incoming particles
      for (G4int nu=0; nu<incvtx->GetNumberOfParticle(); ++nu) {
	
	// Define the vertex track
	TND280UpTrack *nd280VtxTrack = new TND280UpTrack();
	
	G4PrimaryParticle* prim = incvtx->GetPrimary(nu);
	G4ParticleDefinition* partDef = prim->GetG4code();
	G4ThreeVector dir = prim->GetMomentum().unit();
	
	double momX = prim->GetMomentum().x();
	double momY = prim->GetMomentum().y();
	double momZ = prim->GetMomentum().z();
	nd280VtxTrack->SetInitMom(momX,momY,momZ);
	
	int pdg = prim->GetPDGcode();    
	nd280VtxTrack->SetPDG(pdg);
	
	// Fill the vertex with the ingoing track
	nd280Vertex->AddInTrack(nd280VtxTrack);
	
	//delete nd280VtxTrack; 
	//nd280VtxTrack=NULL;
      }
      
      //
      // TODO: define reaction mode when NEUT is used
      //
      nd280Vertex->SetReacMode(vInfo->GetReactionNum());
      nd280Vertex->SetReacModeString(vInfo->GetReaction());
    }  
    nd280Vertex->SetPosition(vtx->GetX0()/CLHEP::mm,
      vtx->GetY0()/CLHEP::mm,
      vtx->GetZ0()/CLHEP::mm);
    nd280Vertex->SetTime(vtx->GetT0()/CLHEP::second);

    // Add the vertex to the event
    fND280UpEvent->AddVertex(nd280Vertex);

    
    
    ++vtxNumber;  
      
    //delete nd280Vertex; 
    //nd280Vertex=NULL;
  }
 



  //                                          
  // Store the track in the ND280 event 
  //                                          
  
  const G4TrajectoryContainer* trajectories = anEvent->GetTrajectoryContainer();
  if (!trajectories) {
    G4ExceptionDescription msg;
    msg << "No Trajectories" << G4endl; 
    G4Exception("ExN02EventAction::EndOfEventAction()",
   		"ExN02Code001", JustWarning, msg);
    return false;
  }


 
  cout << "ND280RootPersistencyManager::Store()" << endl;
  cout << "# of Tracks: " << trajectories->entries() << endl;



  
  // loop over the trajectories
  for (TrajectoryVector::iterator t = trajectories->GetVector()->begin();
       t != trajectories->GetVector()->end();
       ++t) { 
    
    ND280Trajectory* ndTraj = dynamic_cast<ND280Trajectory*>(*t);
    //   G4VTrajectory* g4Traj = dynamic_cast<G4VTrajectory*>(*t);
    
    G4String particleName = ndTraj->GetParticleName();
    G4int NptTraj = ndTraj->GetPointEntries();
    G4int TrajTrkId = ndTraj->GetTrackID(); 

    TND280UpTrack *nd280Track = new TND280UpTrack();
    nd280Track->SetTrackID(ndTraj->GetTrackID());
    nd280Track->SetParentID(ndTraj->GetParentID());
    nd280Track->SetPDG(ndTraj->GetPDGEncoding());
    nd280Track->SetProcessName(ndTraj->GetProcessName());
    nd280Track->SetInitKinEnergy(ndTraj->GetInitialKineticEnergy());
    
    double momX = ndTraj->GetInitialMomentum().x();
    double momY = ndTraj->GetInitialMomentum().y();
    double momZ = ndTraj->GetInitialMomentum().z();
    nd280Track->SetInitMom(momX,momY,momZ);
    
    nd280Track->SetInitCosTheta(ndTraj->GetInitialCosTheta());
    nd280Track->SetCharge(ndTraj->GetCharge());
    nd280Track->SetRange(ndTraj->GetRange());
    
    nd280Track->SetSDTotalEnergyDeposit(ndTraj->GetSDTotalEnergyDeposit());
    nd280Track->SetSDLength(ndTraj->GetSDLength());
    
    //cout << "ndTraj->GetSDLength() = " << ndTraj->GetSDLength() << endl;

    
    //
    // Store the points of the track 
    // and calculate other track variables
    //

    double LengthTarget1 = 0.;
    double LengthTarget2 = 0.;
    double LengthFGD1 = 0.;
    double LengthFGD2 = 0.;
    double LengthTPCUp1     = 0.;
    double LengthTPCUp2     = 0.;
    double LengthTPCDown1   = 0.;
    double LengthTPCDown2   = 0.;
    double LengthForwTPC1   = 0.;
    double LengthForwTPC2   = 0.;
    double LengthForwTPC3   = 0.;
    double LengthDsECal     = 0.;
    double LengthBrlECal    = 0.;
    double LengthP0DECal    = 0.;
    double LengthUSECalP0D  = 0.;

    double LyzTPCUp1     = 0.;
    double LyzTPCUp2     = 0.;
    double LyzTPCDown1   = 0.;
    double LyzTPCDown2   = 0.;
    double LyzForwTPC1   = 0.;
    double LyzForwTPC2   = 0.;
    double LyzForwTPC3   = 0.;

    double EdepTarget1 = 0.;
    double EdepTarget2 = 0.;
    double EdepFGD1 = 0.;
    double EdepFGD2 = 0.;
    double EdepTPCUp1     = 0.;
    double EdepTPCUp2     = 0.;
    double EdepTPCDown1   = 0.;
    double EdepTPCDown2   = 0.;
    double EdepForwTPC1   = 0.;
    double EdepForwTPC2   = 0.;
    double EdepForwTPC3   = 0.;
    double EdepDsECal     = 0.;
    double EdepBrlECal    = 0.;
    double EdepP0DECal    = 0.;
    double EdepUSECalP0D  = 0.;

    G4String detname_prev = "undefined";
    
    int NPoints = ndTraj->GetPointEntries();

    for(int itp=0;itp<NPoints;itp++){ // loop over all the points
      
      G4String detname_curr = "undefined";
      ND280TrajectoryPoint* ndPoint = dynamic_cast<ND280TrajectoryPoint*>(ndTraj->GetPoint(itp));
      //detname_curr = ndPoint->GetPhysVolName();
      detname_curr = ndPoint->GetLogVolName();
      
      //G4cout << detname_curr << G4endl;
      
      G4String detname_aft  = "undefined";
      ND280TrajectoryPoint* ndPointAfter; 
      if(itp<(NPoints-1)){ // not if last point
	ndPointAfter = dynamic_cast<ND280TrajectoryPoint*>(ndTraj->GetPoint(itp+1));
	//detname_aft  = ndPointAfter->GetPhysVolName();
	detname_aft  = ndPointAfter->GetLogVolName();
      }
      
      if (!ndPoint) {
	G4ExceptionDescription msg;
	msg << "No Points in the Trajectory" << G4endl; 
	G4Exception("ExN02EventAction::EndOfEventAction()",
		    "ExN02Code001", JustWarning, msg);
	return false;
      }
      
      double steplength = ndPoint->GetStepLength();
      double stepdeltalyz = ndPoint->GetStepDeltaLyz();  
      double stepedep = ndPoint->GetEdep();

      /////////////////////////////////////////
      //
      // Compare the Logical Volume name w/ hard-coded names 
      // of Logical volumes in SDRegion (printed out by ExN02DetectorConstruction)
      //
      /////////////////////////////////////////
      
      if(steplength>0.){
  	if     (detname_curr.contains("/t2k/OA/Magnet/Basket/Target1")){
	  LengthTarget1 += steplength;
	  EdepTarget1   += stepedep;
	}
	else if(detname_curr.contains("/t2k/OA/Magnet/Basket/Target2")){
	  LengthTarget2 += steplength;	
	  EdepTarget2   += stepedep;	
	}
 	else if(detname_curr.contains("/t2k/OA/Magnet/Basket/FGD1")){
	  LengthFGD1 += steplength;
	  EdepFGD1   += stepedep;
	}
	else if(detname_curr.contains("/t2k/OA/Magnet/Basket/FGD2")){
	  LengthFGD2 += steplength;	
	  EdepFGD2   += stepedep;	
	}
	else if(detname_curr.contains("/t2k/OA/Magnet/Basket/TPCUp1")){
	  LengthTPCUp1 += steplength;	
	  LyzTPCUp1    += stepdeltalyz;
	  EdepTPCUp1   += stepedep;
	}
	else if(detname_curr.contains("/t2k/OA/Magnet/Basket/TPCUp2")){
	  LengthTPCUp2 += steplength;	
	  LyzTPCUp2    += stepdeltalyz;
	  EdepTPCUp2   += stepedep;
	}
	else if(detname_curr.contains("/t2k/OA/Magnet/Basket/TPCDown1")){
	  LengthTPCDown1 += steplength;	
	  LyzTPCDown1    += stepdeltalyz;
	  EdepTPCDown1   += stepedep;
	}
	else if(detname_curr.contains("/t2k/OA/Magnet/Basket/TPCDown2")){
	  LengthTPCDown2 += steplength;	
	  LyzTPCDown2    += stepdeltalyz;
	  EdepTPCDown2   += stepedep;
	}
	else if( detname_curr.contains("/t2k/OA/Magnet/Basket/ForwTPC1/Half") ||
		 //detname_curr.contains("/t2k/OA/Magnet/Basket/ForwTPC1/Drift") ||
		 detname_curr.contains("/t2k/OA/Magnet/Basket/ForwTPC1/MM")
		 ){
	  LengthForwTPC1 += steplength;	
	  LyzForwTPC1    += stepdeltalyz;
	  EdepForwTPC1   += stepedep;
	}
	else if( detname_curr.contains("/t2k/OA/Magnet/Basket/ForwTPC2/Half") ||
		 //detname_curr.contains("/t2k/OA/Magnet/Basket/ForwTPC2/Drift") ||
		 detname_curr.contains("/t2k/OA/Magnet/Basket/ForwTPC2/MM")
		 ){
	  LengthForwTPC2 += steplength;	
	  LyzForwTPC2    += stepdeltalyz;
	  EdepForwTPC2   += stepedep;
	}
	else if( detname_curr.contains("/t2k/OA/Magnet/Basket/ForwTPC3/Half") ||
		 //detname_curr.contains("/t2k/OA/Magnet/Basket/ForwTPC3/Drift") ||
		 detname_curr.contains("/t2k/OA/Magnet/Basket/ForwTPC3/MM")
		 ){
	  LengthForwTPC3 += steplength;	
	  LyzForwTPC3    += stepdeltalyz;
	  EdepForwTPC3   += stepedep;
	}
	
	else if(detname_curr.contains("/t2k/OA/Magnet/Basket/DsECal")){
	  
	  LengthDsECal += steplength;

	  if( detname_curr.contains("/t2k/OA/Magnet/Basket/DsECal/Module/Active/ScintHoriz/Bar") ||
	      detname_curr.contains("/t2k/OA/Magnet/Basket/DsECal/Module/Active/ScintVert/Bar")
		 ){
	    EdepDsECal   += stepedep;
	  }
	}
	else if(detname_curr.contains("/t2k/OA/Magnet/LeftClam/P0DECal")){
	  
	  LengthP0DECal += steplength;
	  
	  if( detname_curr.contains("/t2k/OA/Magnet/LeftClam/P0DECal/TopLeftBotRight/Active/ScintPara/Bar") ||
	      detname_curr.contains("/t2k/OA/Magnet/LeftClam/P0DECal/LeftSide/Active/ScintPara/Bar")        ||
	      detname_curr.contains("/t2k/OA/Magnet/LeftClam/P0DECal/BotLeftTopRight/Active/ScintPara/Bar") ||
	      detname_curr.contains("/t2k/OA/Magnet/RightClam/P0DECal/TopLeftBotRight/Active/ScintPara/Bar")||       
	      detname_curr.contains("/t2k/OA/Magnet/RightClam/P0DECal/RightSide/Active/ScintPara/Bar")      || 
	      detname_curr.contains("/t2k/OA/Magnet/RightClam/P0DECal/BotLeftTopRight/Active/ScintPara/Bar")
	      ){
	    EdepP0DECal   += stepedep;
	  }
	}
	else if(detname_curr.contains("/t2k/OA/Magnet/LeftClam/BrlECal")){
	  
	  LengthBrlECal += steplength;
	  
	  if( detname_curr.contains("/t2k/OA/Magnet/LeftClam/BrlECal/TopLeftBotRight/Active/ScintPara/Bar") ||
	      detname_curr.contains("/t2k/OA/Magnet/LeftClam/BrlECal/TopLeftBotRight/Active/ScintPerp/Bar") ||
	      detname_curr.contains("/t2k/OA/Magnet/LeftClam/BrlECal/LeftSide/Active/ScintPara/Bar")        ||
	      detname_curr.contains("/t2k/OA/Magnet/LeftClam/BrlECal/LeftSide/Active/ScintPerp/Bar")        ||       
	      detname_curr.contains("/t2k/OA/Magnet/LeftClam/BrlECal/BotLeftTopRight/Active/ScintPara/Bar") || 
	      detname_curr.contains("/t2k/OA/Magnet/LeftClam/BrlECal/BotLeftTopRight/Active/ScintPerp/Bar") ||
	      detname_curr.contains("/t2k/OA/Magnet/RightClam/BrlECal/BotLeftTopRight/Active/ScintPara/Bar")|| 
	      detname_curr.contains("/t2k/OA/Magnet/RightClam/BrlECal/BotLeftTopRight/Active/ScintPerp/Bar")|| 
	      detname_curr.contains("/t2k/OA/Magnet/RightClam/BrlECal/RightSide/Active/ScintPara/Bar")      ||
	      detname_curr.contains("/t2k/OA/Magnet/RightClam/BrlECal/RightSide/Active/ScintPerp/Bar")      ||
	      detname_curr.contains("/t2k/OA/Magnet/RightClam/BrlECal/TopLeftBotRight/Active/ScintPara/Bar")||
	      detname_curr.contains("/t2k/OA/Magnet/RightClam/BrlECal/TopLeftBotRight/Active/ScintPerp/Bar")
	      ){
	    EdepBrlECal   += stepedep;
	  }
	}
	else if(detname_curr.contains("/t2k/OA/Magnet/Basket/P0D/USECal")){
	  
	  LengthUSECalP0D += steplength;
	  
	  if( detname_curr.contains("/t2k/OA/Magnet/Basket/P0D/USECal/P0Dule/X/Bar") ||
	      detname_curr.contains("/t2k/OA/Magnet/Basket/P0D/USECal/P0Dule/Y/Bar") 
	      ){
	    EdepUSECalP0D += stepedep;
	  }
	}

      }
            
      //Select points if first/last of the track or
      //if first/last in a SD
      if( detname_curr != detname_prev ||
      	  detname_curr != detname_aft  ||
      	  itp == 0                     ||
      	  itp == (NPoints-1)
      	  ){
      
	//G4cout << "TrajTrkId = " << TrajTrkId << " : " 
	//<< detname_prev << " " << detname_curr << " " << detname_aft << G4endl;
	
	TND280UpTrackPoint *nd280TrackPoint = new TND280UpTrackPoint();
	//std::auto_ptr<TND280UpTrackPoint> nd280TrackPoint(new TND280UpTrackPoint());
	
	nd280TrackPoint->SetPointID(itp);
	nd280TrackPoint->SetTime(ndPoint->GetTime());
	
	// momentum 
	double momPtX = ndPoint->GetMomentum().x();
	double momPtY = ndPoint->GetMomentum().y();
	double momPtZ = ndPoint->GetMomentum().z();
	nd280TrackPoint->SetMomentum(momPtX,momPtY,momPtZ);
	
	nd280TrackPoint->SetEdep(ndPoint->GetEdep());
	nd280TrackPoint->SetStepLength(ndPoint->GetStepLength());
	nd280TrackPoint->SetStepDeltaLyz(ndPoint->GetStepDeltaLyz());
	nd280TrackPoint->SetStepStatus(ndPoint->GetStepStatus());
	nd280TrackPoint->SetPhysVolName(ndPoint->GetPhysVolName());
	nd280TrackPoint->SetLogVolName(ndPoint->GetLogVolName());
	
	// preStep position 
	double prevX = ndPoint->GetPrevPosition().x();
	double prevY = ndPoint->GetPrevPosition().y();
	double prevZ = ndPoint->GetPrevPosition().z();
	nd280TrackPoint->SetPrePosition(prevX,prevY,prevZ);
	
	// postStep position
	double postX = ndPoint->GetPostPosition().x();
	double postY = ndPoint->GetPostPosition().y();
	double postZ = ndPoint->GetPostPosition().z();
	nd280TrackPoint->SetPostPosition(postX,postY,postZ);
	
	nd280TrackPoint->SetIsOnBoundary(ndPoint->IsOnBoundary());

	
	//
	// Store points if first/last point of the track
	// or first/last of a SD
	// (same condition already defined above)
	//
		
	// G4cout << "- Track PDG: " << nd280Track->GetPDG()
	//        << " --> TrkID = " << nd280Track->GetTrackID() 
	//        << " --> ParID = " << nd280Track->GetParentID() 
	//        << G4endl;
	  
	// Mark the points
	MarkPoint(ndPoint); // Store if in a SD 
	if(ndPoint->SavePoint()){    
	//if(1){ // TEST PARTICLE GUN
	  nd280Track->AddPoint(nd280TrackPoint);
	}	
	else if(itp == 0           || 
		itp == (NPoints-1)
		){
	  nd280Track->AddPoint(nd280TrackPoint);
	}

	//delete nd280TrackPoint; 
	//nd280TrackPoint=NULL;
      }
      
      detname_prev = detname_curr;
      
    } // end loop over the points   




    // Store the track length    
    //G4cout << "Target track length: " 
    //<< LengthTarget1 << ", "
    //<< LengthTarget2 
    //<< G4endl;

    nd280Track->SetLengthTarget1(LengthTarget1);
    nd280Track->SetLengthTarget2(LengthTarget2); 
    nd280Track->SetLengthFGD1(LengthFGD1);
    nd280Track->SetLengthFGD2(LengthFGD2); 
    nd280Track->SetLengthTPCUp1(LengthTPCUp1);
    nd280Track->SetLengthTPCUp2(LengthTPCUp2);
    nd280Track->SetLengthTPCDown1(LengthTPCDown1);
    nd280Track->SetLengthTPCDown2(LengthTPCDown2);
    nd280Track->SetLengthForwTPC1(LengthForwTPC1);
    nd280Track->SetLengthForwTPC2(LengthForwTPC2);
    nd280Track->SetLengthForwTPC3(LengthForwTPC3);
    nd280Track->SetLengthDsECal(LengthDsECal);
    nd280Track->SetLengthBrlECal(LengthBrlECal);
    nd280Track->SetLengthP0DECal(LengthP0DECal);
    nd280Track->SetLengthUSECalP0D(LengthUSECalP0D);
    
    nd280Track->SetLyzTPCUp1(LyzTPCUp1);
    nd280Track->SetLyzTPCUp2(LyzTPCUp2);
    nd280Track->SetLyzTPCDown1(LyzTPCDown1);
    nd280Track->SetLyzTPCDown2(LyzTPCDown2);
    nd280Track->SetLyzForwTPC1(LyzForwTPC1);
    nd280Track->SetLyzForwTPC2(LyzForwTPC2);
    nd280Track->SetLyzForwTPC3(LyzForwTPC3);
    
    nd280Track->SetEdepTarget1(EdepTarget1);
    nd280Track->SetEdepTarget2(EdepTarget2);    
    nd280Track->SetEdepFGD1(EdepFGD1);
    nd280Track->SetEdepFGD2(EdepFGD2);    
    nd280Track->SetEdepTPCUp1(EdepTPCUp1);
    nd280Track->SetEdepTPCUp2(EdepTPCUp2);
    nd280Track->SetEdepTPCDown1(EdepTPCDown1);
    nd280Track->SetEdepTPCDown2(EdepTPCDown2);
    nd280Track->SetEdepForwTPC1(EdepForwTPC1);
    nd280Track->SetEdepForwTPC2(EdepForwTPC2);
    nd280Track->SetEdepForwTPC3(EdepForwTPC3);
    nd280Track->SetEdepDsECal(EdepDsECal);
    nd280Track->SetEdepBrlECal(EdepBrlECal);
    nd280Track->SetEdepP0DECal(EdepP0DECal);
    nd280Track->SetEdepUSECalP0D(EdepUSECalP0D);
    
    // Mark the trajectories to save.
    // // MarkTrajectories(anEvent); // loop over all the tracks again... --> don't use it!!!
    MarkTrajectory(ndTraj,anEvent);
    if(ndTraj->SaveTrajectory()){
      nd280Track->SaveIt(true); // useless     
      fND280UpEvent->AddTrack(nd280Track);

      G4cout << "TrajTrkId = " << TrajTrkId 
	     << ", ParID = " << ndTraj->GetParentID() 
	     << ", pdg = " << ndTraj->GetPDGEncoding()
	     << ", name = " << ndTraj->GetParticleName()
	     << ", mom = " << ndTraj->GetInitialMomentum().mag()
	//<< ", ekin = " << ndTraj->GetInitialKineticEnergy()
	     << ", sdedep = " << ndTraj->GetSDTotalEnergyDeposit()
	     << ", process = " << ndTraj->GetProcessName()
	     << G4endl;
    }
  
    // 
    // ND280Trajectory::ShowTrajectory()
    // and 
    // G4VTrajectory::ShowTrajectory(os)
    // don't work:
    //
    //  G4BestUnit: the category Momentum does not exist.
    //
    // -------- EEEE ------- G4Exception-START -------- EEEE -------
    // *** G4Exception : InvalidCall
    //       issued by : G4BestUnit::G4BestUnit()
    // Missing unit category !
    // *** Fatal Exception *** core dump ***
    // -------- EEEE -------- G4Exception-END --------- EEEE -------
    //
    //ndTraj->ShowTrajectory(G4cout);
    //
    
    //delete nd280Track; 
    //nd280Track=NULL;

  } // end loop over Trajectories


  cout << "Tot # of Tracks stored in output event: " << fND280UpEvent->GetNTracks() << endl;

    
  
  fOutput->cd();
  
  // Get the address of the event from the auto_ptr to the fEventTree branch
  // pointer,, and  then fill the event  tree.  This copies the  data to the
  // output  file.  After the  data is  saved, I  reset the  the fND280Event
  // pointer (the fEventTree branch pointer).
  //fND280Event = event.get(); 
  
  
  //G4cout << "fEventTree->Fill();" << G4endl;
  
  fEventTree->Fill();
    
  //G4cout << "delete fND280UpEvent;" << G4endl;




  //delete nd280Track; 
  //nd280Track=NULL;
 
  delete fND280UpEvent;
  fND280UpEvent = NULL;




  // ++fEventsNotSaved;
  // if (fEventsNotSaved>100) {
  //   ND280Info("AutoSave Event Tree");
  //   fEventTree->AutoSave("SaveSelf");
  //   fEventsNotSaved = 0;
  // }  

  return true;
}

bool ND280RootPersistencyManager::Store(const G4Run* aRun) {
  
  // Not used in nd280mc

  if (!fOutput) {
    G4ExceptionDescription msg;
    msg << "No Output File" << G4endl; 
    G4Exception("ND280RootPersistencyManager::Store",
   		"ExN02Code001",FatalException, msg);
    return false;
  }  

  fOutput->cd();
  
  // Store histograms with MPPC 2D readout binning

  TH2F *OutMPPCProj2D_XY;
  TH2F *OutMPPCProj2D_XZ;
  TH2F *OutMPPCProj2D_YZ;
  
  //  TString name;

  if( GetIsMPPCProjXY() ){
    OutMPPCProj2D_XY = new TH2F(*fMPPCProj2D_XY);
    // name = TString::Format("OutMPPCProj2D_XY_mod_%02d",fModNum);
    // OutMPPCProj2D_XY->SetName(name);
    // OutMPPCProj2D_XY->SetTitle(name);
    OutMPPCProj2D_XY->Write();
  }
  if( GetIsMPPCProjXZ() ){
    OutMPPCProj2D_XZ = new TH2F(*fMPPCProj2D_XZ);
    // name = TString::Format("OutMPPCProj2D_XZ_mod_%02d",fModNum);
    // OutMPPCProj2D_XZ->SetName(name);
    // OutMPPCProj2D_XZ->SetTitle(name);  
    OutMPPCProj2D_XZ->Write();
  }
  if( GetIsMPPCProjYZ() ){  
    OutMPPCProj2D_YZ = new TH2F(*fMPPCProj2D_YZ);
    // name = TString::Format("OutMPPCProj2D_YZ_mod_%02d",fModNum);
    // OutMPPCProj2D_YZ->SetName(name);
    // OutMPPCProj2D_YZ->SetTitle(name);
    OutMPPCProj2D_YZ->Write();
  }
  
  // Store translation local-world reference system

  const G4NavigationHistory *pmHistory = this->GetNavigHistoTarg1();
  G4ThreeVector PMworldPosition(0,0,0);
  G4ThreeVector PMlocalPosition = GetLocalPosition(PMworldPosition);

  TPolyMarker3D *pm3d = new TPolyMarker3D(1);
  pm3d->SetName("WorldOrigInLocal");
  double x = PMlocalPosition.x();
  double y = PMlocalPosition.y();
  double z = PMlocalPosition.z();
  pm3d->SetPoint(0,x,y,z);
  pm3d->Write();

  return true;
}

bool ND280RootPersistencyManager::Store(const G4VPhysicalVolume* aWorld) {
  return false;
  
  // if (!gGeoManager) {
  //   ND280Error("ND280RootPersistencyManage::Store(world) run before /t2k/update");
  //   ND280RootGeometryManager::Get()->Update(aWorld,true);
  // }
  // if (!fOutput) {
  //   ND280Error("ND280RootPersistencyManager::Store "
  // 	       << "-- No Output File");
  //   return false;
  // }
  // fOutput->cd();
  // gGeoManager->Write();
  // return true;
}



// std::vector<int>::iterator ND280RootPersistencyManager::CleanHitContributors(
//     std::map<int,int>& parentMap,
//     const ND::TG4TrajectoryContainer& trajCon,
//     std::vector<int>::iterator start, 
//     std::vector<int>::iterator finish) {

//     for (std::vector<int>::iterator c=start; c != finish; ++c) {
//         // Check each contributor to make sure that it is a valid
//         // trajectory.  If it isn't in the trajectory map, then set it
//         // to a parent that is.
//         int loopTrap = 100000;
//         while (!trajCon.GetTrajectory(*c)) {
//             std::map<int,int>::iterator t = parentMap.find(*c);
//             if (t == parentMap.end()) {
//                 ND280Warn("% Contributing trajectory without parent");
//                 break;
//             }
//             *c = t->second;
//             if ( 0 > --loopTrap) {
//                 ND280Warn("Break loop in "
//                           << "ND280RootPersistencyManager::"
//                           << "CleanHitContributors");
//                 break;
//             }
//         }
//     }

//     // Remove the duplicate entries.
//     std::sort(start,finish);
//     finish = std::unique(start,finish);
        
//     return finish;
// }

// void ND280RootPersistencyManager::FillParentMap(std::map<int,int>& parentMap,
//                                                     const G4Event* src) {
//     parentMap.clear();

//     const G4TrajectoryContainer* g4Traj = src->GetTrajectoryContainer();
//     if (g4Traj) {
//         for (TrajectoryVector::iterator t = g4Traj->GetVector()->begin();
//              t != g4Traj->GetVector()->end();
//              ++t) {
//             parentMap[(*t)->GetTrackID()] = (*t)->GetParentID();
//         }
//     }
//     else {
//         ND280Warn("%%% Missing trajectories for Hit Contributors");
//     }
// }
                                                       
// ND::TG4HitContainer* ND280RootPersistencyManager::CreateHitSegmentContainer(
//     std::map<int,int>& parentMap,
//     const ND::TG4TrajectoryContainer& trajCon,
//     G4VHitsCollection* g4Hits,
//     std::string hitsName,
//     ND::TND280Event& dest) {

//     if (!g4Hits) return NULL;
//     if (g4Hits->GetSize()<1) return NULL;
//     // Check to make sure that this hit collection contains ND280HitSegment
//     // objects.
//     ND280HitSegment* g4Hit = dynamic_cast<ND280HitSegment*>(g4Hits->GetHit(0));
//     if (!g4Hit) return NULL;

//     ND::TG4HitContainer* hits = new ND::TG4HitContainer(hitsName.c_str());
//     for (unsigned int h=0; h<g4Hits->GetSize(); ++h) {
//         ND280HitSegment* g4Hit
//             = dynamic_cast<ND280HitSegment*>(g4Hits->GetHit(h));
//         std::vector<int>::iterator e = 
//             CleanHitContributors(parentMap,trajCon,
//                                  g4Hit->GetContributors().begin(),
//                                  g4Hit->GetContributors().end());
//         g4Hit->GetContributors().erase(e,g4Hit->GetContributors().end());
//         ND::TG4HitSegment* hit = dynamic_cast<ND::TG4HitSegment*>(g4Hit);
//         hits->push_back(new ND::TG4HitSegment(*hit));
//     }
    
//     return hits;
// }
                                                       
// void ND280RootPersistencyManager::CopyHitCollections(ND::TND280Event& dest, 
//                                                      const G4Event* src) {
//     G4HCofThisEvent* HCofEvent = src->GetHCofThisEvent();
//     G4SDManager *sdM = G4SDManager::GetSDMpointer();
//     G4HCtable *hcT = sdM->GetHCtable();
//     if (!HCofEvent) return;

//     // Make sure that the directory for G4 hits exists in the output event.
//     if (!dest.FindDatum("truth/g4Hits")) {
//         if (!dest.FindDatum("truth")) {
//             dest.push_back(new ND::TDataVector("truth"));
//         }
//         dest.Get<ND::TDataVector>("truth")->push_back(
//             new ND::TDataVector("g4Hits"));
//     }

//     //Keep track of all trajectories and which ones are saved.
//     std::map<int,int> parentMap;
//     FillParentMap(parentMap,src);

//     // Get the trajectories that have been saved in the event.
//     ND::THandle<ND::TG4TrajectoryContainer> trajCon 
//         = dest.Get<ND::TG4TrajectoryContainer>("truth/G4Trajectories");

//     // Copy each of the hit categories into the output event.
//     for (int i=0; i<hcT->entries(); ++i) {
//         G4String SDname = hcT->GetSDname(i);
//         G4String HCname = hcT->GetHCname(i);
//         int HCId = sdM->GetCollectionID(SDname+"/"+HCname);
//         G4VHitsCollection* g4Hits = HCofEvent->GetHC(HCId);
//         if (g4Hits->GetSize()<1) continue;
//         std::string hitsName = SDname;
//         ND::TDatum* hits = NULL;
//         if (!hits) hits = CreateHitSegmentContainer(parentMap,*trajCon,
//                                                     g4Hits, hitsName, dest);
//         if (hits) dest.Get<ND::TDataVector>("truth/g4Hits")->push_back(hits);
//     }
// }

// ND::TG4PrimaryVertexContainer* ND280RootPersistencyManager::MakePrimary(
//     const char* name, const char* title, const G4PrimaryVertex* src) {
//     if (!src) return NULL;

//     ND::TG4PrimaryVertexContainer* vertexContainer = NULL;
//     while (src) {
//         ND::TG4PrimaryVertex vtx;
        
//         vtx.GetPosition().SetX(src->GetX0());
//         vtx.GetPosition().SetY(src->GetY0());
//         vtx.GetPosition().SetZ(src->GetZ0());
//         vtx.GetPosition().SetT(src->GetT0());
        
//         for (int i=0; i< src->GetNumberOfParticle(); ++i) {
//             ND::TG4PrimaryParticle prim;
//             G4PrimaryParticle *g4Prim = src->GetPrimary(i);
//             prim.SetTrackId(g4Prim->GetTrackID());
//             prim.SetPDGCode(g4Prim->GetPDGcode());
//             prim.SetTitle(g4Prim->GetG4code()->GetParticleName());
//             prim.GetMomentum().SetPx(g4Prim->GetPx());
//             prim.GetMomentum().SetPy(g4Prim->GetPy());
//             prim.GetMomentum().SetPz(g4Prim->GetPz());
//             double E = pow(prim.GetMomentum().P(),2) 
//                 + pow(g4Prim->GetG4code()->GetPDGMass(),2);
//             if (E>0) E = sqrt(E);
//             else E = 0;
//             prim.GetMomentum().SetE(E);
//             vtx.GetPrimaryParticles().push_back(prim);
//             ND280Verbose("   "
//                       << " id " << g4Prim->GetTrackID()
//                       << " " << g4Prim->GetG4code()->GetParticleName()
//                       << " with " << G4BestUnit(prim.GetMomentum().E(),"Energy")
//                       << " (from " << G4BestUnit(g4Prim->GetMomentum().mag(), 
//                                                  "Energy") << "/c" 
//                       << " + " << G4BestUnit(prim.GetMomentum().M(), 
//                                              "Energy") << "/c^2"
//                       << ")");
//         }

//         ND280VertexInfo* srcInfo 
//             = dynamic_cast<ND280VertexInfo*>(src->GetUserInformation());
//         if (srcInfo) {
//             vtx.SetGeneratorName(srcInfo->GetName());
//             vtx.SetReaction(srcInfo->GetReaction());
// #if oaEvent_MAJOR_VERSION>7 ||				
//     (oaEvent_MAJOR_VERSION==7 && oaEvent_MINOR_VERSION>1) || 
//     (oaEvent_MAJOR_VERSION==6 &&				
//      oaEvent_MINOR_VERSION>2 && 
//      oaEvent_PATCH_VERSION>5)
//             vtx.SetFilename(srcInfo->GetFilename());
// #else
// #warning Not saving input file name, tree, or entry.
// #endif
//             vtx.SetInteractionNumber(srcInfo->GetInteractionNumber());
//             vtx.SetCrossSection(srcInfo->GetCrossSection());
//             vtx.SetDiffCrossSection(srcInfo->GetDiffCrossSection());
//             vtx.SetWeight(srcInfo->GetWeight());
//             vtx.SetProbability(srcInfo->GetProbability());
//             ND::TG4PrimaryVertexContainer *vtxInfo
//                 = MakePrimary("Informational","Informational Vertex",
//                               srcInfo->GetInformationalVertex());
//             if (vtxInfo) {
//                 vtx.SetInfoVertex(*vtxInfo);
//                 delete vtxInfo;
//             }
//         }

//         if (!vertexContainer) {
//             vertexContainer = new ND::TG4PrimaryVertexContainer(name,title);
//         }
//         vertexContainer->push_back(vtx);
//         src = src->GetNext();
//     }
    
//     return vertexContainer;
// }


// void ND280RootPersistencyManager::CopyTrajectoryPoints(ND::TG4Trajectory& traj, 
//                                                        G4VTrajectory* g4Traj) {
//     std::vector<int> selected;

//     // Choose the trajectory points that are going to be saved.
//     SelectTrajectoryPoints(selected, g4Traj);

//     // Make sure the selected trajectory points are in order and unique.
//     std::sort(selected.begin(),selected.end());
//     selected.erase(std::unique(selected.begin(), selected.end()),
//                    selected.end());

//     ////////////////////////////////////
//     // Save the trajectories.
//     ////////////////////////////////////
//     for (std::vector<int>::iterator tp = selected.begin(); 
//          tp != selected.end(); ++tp) {
//         ND280TrajectoryPoint* nd280Point 
//             = dynamic_cast<ND280TrajectoryPoint*>(g4Traj->GetPoint(*tp));
//         ND::TG4TrajectoryPoint point;
//         point.SetPosition(nd280Point->GetPosition().x(),
//                           nd280Point->GetPosition().y(),
//                           nd280Point->GetPosition().z(),
//                           nd280Point->GetTime());
//         point.SetMomentum(nd280Point->GetMomentum().x(),
//                           nd280Point->GetMomentum().y(),
//                           nd280Point->GetMomentum().z());
//         point.SetVolumeNode(nd280Point->GetVolumeNode());
//         traj.GetTrajectoryPoints().push_back(point);
//     }

// }

// void ND280RootPersistencyManager::CopyTrajectories(
//     ND::TND280Event& dest,
//     const G4TrajectoryContainer* src) {
//     if (!src) {
//         ND280Verbose("No Trajectories ");
//         return;
//     }

//     ND::TG4TrajectoryContainer* traj = new ND::TG4TrajectoryContainer();

//     // Keep track of all trajectories and which ones are saved.  The first
//     // field is the track id, and the second is whether it is saved.
//     std::map<int,std::pair<int,bool> > parentMap;
//     for (TrajectoryVector::iterator t = src->GetVector()->begin();
//          t != src->GetVector()->end();
//          ++t) {
//         ND280Trajectory* ndTraj = dynamic_cast<ND280Trajectory*>(*t);

//         parentMap[ndTraj->GetTrackID()] 
//             = std::pair<int,bool>(ndTraj->GetParentID(),false);

//         // Check if the trajectory should be saved.
//         if (!ndTraj->SaveTrajectory()) continue;
//         parentMap[(*t)->GetTrackID()].second = true;

//         std::string particleName = ndTraj->GetParticleName();

//         ND::TG4Trajectory copy;
//         copy.SetTrackId((*t)->GetTrackID());
//         // Set the particle type information.
//         G4ParticleDefinition* part 
//             = G4ParticleTable::GetParticleTable()->FindParticle(particleName);
//         if (!part) {
//             G4Exception(std::string("ND280RootPersistencyManager::")
//                         + "No particle information for "
//                         + (*t)->GetParticleName());
//         }
//         copy.SetParticleName(particleName);
//         copy.SetPDGEncoding((*t)->GetPDGEncoding());
//         // Check through the list of saved trajectories to find the first
//         // parent that is written to the output file.
//         int parentId = (*t)->GetTrackID();
//         do {
//             parentId = parentMap[parentId].first;
//         } while (parentId != 0 && !parentMap[parentId].second);
//         copy.SetParentId(parentId);
//         // Set the particle momentum.
//         TLorentzVector mom;
//         mom.SetXYZM((*t)->GetInitialMomentum().x(),
//                     (*t)->GetInitialMomentum().y(),
//                     (*t)->GetInitialMomentum().z(),
//                     part->GetPDGMass());
//         copy.SetInitialMomentum(mom);
//         CopyTrajectoryPoints(copy, (*t));
//         (*traj)[copy.GetTrackId()] = copy;
//     }

//     ND::THandle<ND::TDataVector> vect = dest.Get<ND::TDataVector>("truth");
//     vect->push_back(traj);
// }

// void ND280RootPersistencyManager::MakeMCHeader(ND::TND280Event& dest,
//                                                const G4Event* src) {
// #if oaEvent_MAJOR_VERSION>7 ||                                  
//     (oaEvent_MAJOR_VERSION==7 && oaEvent_MINOR_VERSION>26)
//     // create the MC Header.
//     ND::TND280MCHeader* mcHeader = new ND::TND280MCHeader();
//     ND::THandle<ND::TDataVector> vect = dest.Get<ND::TDataVector>("truth");
//     vect->AddDatum(mcHeader);

//     // Add the off-axis detector magnetic field.
//     G4FieldManager* fieldMgr 
//         = G4TransportationManager::GetTransportationManager()
//         ->GetFieldManager();
//     const ND280MagneticField* field
//         = dynamic_cast<const ND280MagneticField*>(fieldMgr->GetDetectorField());
//     if (field) mcHeader->SetOffAxisField(field->GetFieldStrength());

// #if oaEvent_MAJOR_VERSION>7
//     // Save the hash code of the geometry for this event.
//     mcHeader->SetGeometryHash(ND::TOADatabase::Get().GeomId().GetHash());
// #else
// #warning Not saving geometry hash in MC Header.
// #endif

//     const G4RunManager* runMan = G4RunManager::GetRunManager();
//     const ND280UserPrimaryGeneratorAction* userGenerator
//         = dynamic_cast<const ND280UserPrimaryGeneratorAction*>(
//             runMan->GetUserPrimaryGeneratorAction());
//     if (!userGenerator) G4Exception("Missing ND280UserPrimaryGeneratorAction");
//     const ND280PrimaryGenerator* primGenerator 
//         = dynamic_cast<const ND280PrimaryGenerator*>(
//             userGenerator->GetGenerator(0));
//     if (!primGenerator) G4Exception("Missing ND280UserPrimaryGeneratorAction");
//     double intensity = primGenerator->GetCountGenerator()->GetIntensity();

// #if not BEFORE_oaEvent(8,6,0)
//     mcHeader->SetIntensity(intensity);
// #else
// #warning Not saving intensity in MC Header.
// #endif

// #else
//     return;
// #warning Not creating MC Header.
// #endif
// }
