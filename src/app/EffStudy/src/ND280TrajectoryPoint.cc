//#include "ND280RootGeometryManager.hh"
#include "ND280TrajectoryPoint.hh"
#include "ExN02Constants.hh"

#include <G4Track.hh>
#include <G4Step.hh>
#include <G4VProcess.hh>
#include <G4StepStatus.hh>

#include <G4AttDefStore.hh>
#include <G4AttDef.hh>
#include <G4AttValue.hh>
#include <G4UnitsTable.hh>

//#include <TGeoManager.h>

//#include <TND280Log.hxx>

G4Allocator<ND280TrajectoryPoint> aND280TrajPointAllocator;

ND280TrajectoryPoint::ND280TrajectoryPoint()
  : fTime(0.), fMomentum(0.,0.,0.), fEdep(0), fStepLength(0),
    fStepDeltaLyz(0), fStepStatus(fUndefined), 
    fPhysVolName("OutofWorld"), fPrevPosition(0,0,0),
    fPostPosition(0,0,0) { }

// This is the one used --> See ND280Trajectory::AppendStep(G4Step)
ND280TrajectoryPoint::ND280TrajectoryPoint(const G4Step* aStep)
  : G4TrajectoryPoint(aStep->GetPostStepPoint()->GetPosition()) {
  fTime = aStep->GetPostStepPoint()->GetGlobalTime();
  
  // Need momentum at postStep because the first point 
  // of the trajectory is taken w/ ND280TrajectoryPoint(G4Track)
  // when only the preStep is available!!!
  
  fMomentum = aStep->GetPostStepPoint()->GetMomentum();
  fEdep = aStep->GetTotalEnergyDeposit();
  fStepLength = aStep->GetStepLength();
  
  G4ThreeVector prestepPos = aStep->GetPreStepPoint()->GetPosition();
  G4ThreeVector poststepPos = aStep->GetPostStepPoint()->GetPosition();
  G4double prestep_x = prestepPos.x() / mm;
  G4double prestep_y = prestepPos.y() / mm;
  G4double prestep_z = prestepPos.z() / mm;
  G4double poststep_x = poststepPos.x() / mm;
  G4double poststep_y = poststepPos.y() / mm;
  G4double poststep_z = poststepPos.z() / mm;  
  G4double stepdeltalyz = sqrt( (prestep_y - poststep_y)*(prestep_y - poststep_y) +
				(prestep_z - poststep_z)*(prestep_z - poststep_z) );
  fStepDeltaLyz = stepdeltalyz;

  fStepStatus = aStep->GetPostStepPoint()->GetStepStatus();
  if (aStep->GetPostStepPoint()->GetPhysicalVolume()) {
    fPhysVolName = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();
  }
  else {
    fPhysVolName == "OutOfWorld";
  }
  fPrevPosition = aStep->GetPreStepPoint()->GetPosition();
  
  fPostPosition = aStep->GetPostStepPoint()->GetPosition();
}

// Used for the first point 
ND280TrajectoryPoint::ND280TrajectoryPoint(const G4Track* aTrack)
    : G4TrajectoryPoint(aTrack->GetPosition()) {
    fTime = aTrack->GetGlobalTime();
    fMomentum = aTrack->GetMomentum(); // It takes the preStep
    
    // FIRST POINT OF THE TRACK --> BEFORE ANY STEP (ONLY PRE-STEP)!!! 
    fEdep = 0.;         
    fStepLength = 0.;       
    fStepDeltaLyz = 0.;       
    fStepStatus = fUndefined;  
    //
    
    if (aTrack->GetVolume()) {
        fPhysVolName = aTrack->GetVolume()->GetName();
    }
    else {
        fPhysVolName == "OutOfWorld";
    }
    fPrevPosition = aTrack->GetPosition();

    fPostPosition = aTrack->GetPosition(); // in the first point pre=post
}

ND280TrajectoryPoint::ND280TrajectoryPoint(const ND280TrajectoryPoint &right)
    : G4TrajectoryPoint(right) {
    fTime = right.fTime;
    fMomentum = right.fMomentum;
    fEdep = right.fEdep;
    fStepLength = right.fStepLength;
    fStepDeltaLyz = right.fStepDeltaLyz;
    fStepStatus = right.fStepStatus;
    fPhysVolName = right.fPhysVolName;
    fPrevPosition = right.fPrevPosition;
    fPostPosition = right.fPostPosition;
}

ND280TrajectoryPoint::~ND280TrajectoryPoint() { }

const std::map<G4String,G4AttDef>* ND280TrajectoryPoint::GetAttDefs() const {
    G4bool isNew;

    std::map<G4String,G4AttDef>* store
        = G4AttDefStore::GetInstance("ND280TrajectoryPoint",isNew);

    if (isNew) {

        G4String Time("Time");
        (*store)[Time] =
            G4AttDef(Time, "Time", "Physics","G4BestUnit","G4double");

        G4String Momentum("Momentum");
        (*store)[Momentum] =
            G4AttDef(Momentum, "Momentum", "Physics",
                     "G4BestUnit","G4ThreeVector");

        G4String Edep("Edep");
        (*store)[Edep] =
            G4AttDef(Edep, "Edep", "Physics",
                     "G4BestUnit","G4double");

        G4String StepLength("StepLength");
        (*store)[StepLength] =
            G4AttDef(StepLength, "StepLength", "Physics",
                     "G4BestUnit","G4double");

        G4String StepDeltaLyz("StepDeltaLyz");
        (*store)[StepDeltaLyz] =
            G4AttDef(StepDeltaLyz, "StepDeltaLyz", "Physics",
                     "G4BestUnit","G4double");

        G4String StepStatus("StepStatus");
        (*store)[StepStatus] =
            G4AttDef(StepStatus, "StepStatus", "Physics", "", "G4StepStatus");

        G4String VolumeName("VolumeName");
        (*store)[VolumeName] =
            G4AttDef(VolumeName, "VolumeName", "Physics", "", "G4String");

        G4String VolumeNode("VolumeNode");
        (*store)[VolumeNode] =
            G4AttDef(VolumeNode, "VolumeNode", "Physics", "", "G4int");

    }
    return store;
}

std::vector<G4AttValue>* ND280TrajectoryPoint::CreateAttValues() const {
    std::vector<G4AttValue>* values = new std::vector<G4AttValue>;

    values->push_back(G4AttValue("Time",G4BestUnit(fTime,"Time"),""));

    values->push_back(G4AttValue("Momentum",
                                 G4BestUnit(fMomentum,"Momentum"),""));

    values->push_back(G4AttValue("Edep",
                                 G4BestUnit(fEdep,"Energy"),""));

    values->push_back(G4AttValue("StepLength",
                                 G4BestUnit(fStepLength,"Length"),""));

    values->push_back(G4AttValue("StepDeltaLyz",
                                 G4BestUnit(fStepDeltaLyz,"Length"),""));

    values->push_back(G4AttValue("StepStatus",fStepStatus,""));

    values->push_back(G4AttValue("PhysVolName",fPhysVolName,""));

    //#ifdef G4ATTDEBUG
    //ND280Info(G4AttCheck(values,GetAttDefs()));
    //#endif

    return values;
}

// int ND280TrajectoryPoint::GetVolumeNode() const {
//     gGeoManager->PushPath();
//     int node
//         = ND280RootGeometryManager::Get()->GetNodeId(0.5*(GetPosition()
//                                                           +fPrevPosition));
//     gGeoManager->PopPath();
//     return node;
// }
