#include <string>
#include <sstream>
#include <cmath>

#include <G4VProcess.hh>
#include <G4ParticleTable.hh>
#include <G4AttDefStore.hh>
#include <G4AttDef.hh>
#include <G4AttValue.hh>
#include <G4UnitsTable.hh>

#include "ND280Trajectory.hh"
#include "ND280TrajectoryPoint.hh"
#include "ND280TrajectoryMap.hh"

//#include "G4SystemOfUnits.hh" // NEW GLOBAL
#include <CLHEP/Units/SystemOfUnits.h>

G4Allocator<ND280Trajectory> aND280TrajAllocator;

ND280Trajectory::ND280Trajectory()
    : fPositionRecord(0), fTrackID(0), fParentID(0),
      fPDGEncoding(0), fPDGCharge(0.0), fParticleName(""),
      fProcessName(""), fInitialMomentum(G4ThreeVector()),fInitialCosTheta(0),
      fSDEnergyDeposit(0), fSDTotalEnergyDeposit(0), fSDLength(0), 
      fSaveTrajectory(false) {;}

ND280Trajectory::ND280Trajectory(const G4Track* aTrack) {
    fPositionRecord = new TrajectoryPointContainer();
    // Following is for the first trajectory point
    fPositionRecord->push_back(new ND280TrajectoryPoint(aTrack));
    fTrackID = aTrack->GetTrackID();
    fParentID = aTrack->GetParentID();
    G4ParticleDefinition * fpParticleDefinition = aTrack->GetDefinition();
    fPDGEncoding = fpParticleDefinition->GetPDGEncoding();
    fPDGCharge = fpParticleDefinition->GetPDGCharge();
    fParticleName = fpParticleDefinition->GetParticleName();
    const G4VProcess* proc = aTrack->GetCreatorProcess();
    if (proc) fProcessName = proc->GetProcessName();
    else fProcessName = "primary";
    fInitialMomentum = aTrack->GetMomentum(); // Mom at preStep, checked!!!    
    fInitialCosTheta = aTrack->GetMomentumDirection().z(); // wrt nu direction (0,0,1) at preStep, checked!!! 
    fSDEnergyDeposit = 0.0;
    fSDTotalEnergyDeposit = 0.0;
    fSDLength = 0.0;
    fSaveTrajectory = false;
}

ND280Trajectory::ND280Trajectory(ND280Trajectory & right) : G4VTrajectory() {
    fTrackID = right.fTrackID;
    fParentID = right.fParentID;
    fPDGEncoding = right.fPDGEncoding;
    fPDGCharge = right.fPDGCharge;
    fParticleName = right.fParticleName;
    fProcessName = right.fProcessName;
    fInitialMomentum = right.fInitialMomentum;
    fInitialCosTheta = right.fInitialCosTheta;
    fSDEnergyDeposit = right.fSDEnergyDeposit;
    fSDTotalEnergyDeposit = right.fSDTotalEnergyDeposit;
    fSDLength = right.fSDLength;
    fSaveTrajectory = right.fSaveTrajectory;

    fPositionRecord = new TrajectoryPointContainer();
    for(size_t i=0;i<right.fPositionRecord->size();++i) {
        ND280TrajectoryPoint* rightPoint 
            = (ND280TrajectoryPoint*)((*(right.fPositionRecord))[i]);
        fPositionRecord->push_back(new ND280TrajectoryPoint(*rightPoint));
    }
}

ND280Trajectory::~ND280Trajectory() {
    for(size_t i=0;i<fPositionRecord->size();++i){
        delete  (*fPositionRecord)[i];
    }
    fPositionRecord->clear();
    
    delete fPositionRecord;
}

G4double ND280Trajectory::GetInitialKineticEnergy() const {
    const G4ParticleDefinition* p = GetParticleDefinition();
    double mom = GetInitialMomentum().mag();
    if (!p) return mom;
    double mass = p->GetPDGMass();
    double kin = std::sqrt(mom*mom + mass*mass) - mass;
    if (kin<0.0*CLHEP::MeV) return 0.0;
    return kin;
}

G4double ND280Trajectory::GetRange() const {
    if (GetPointEntries()<2) return 0.0;
    G4ThreeVector first 
        = dynamic_cast<ND280TrajectoryPoint*>(GetPoint(0))->GetPosition();
    G4ThreeVector last 
        = dynamic_cast<ND280TrajectoryPoint*>(GetPoint(GetPointEntries()-1))
            ->GetPosition();
    return (last - first).mag();
}

void ND280Trajectory::MarkTrajectory(bool save) {

    //G4cout << "ND280Trajectory::MarkTrajectory" << G4endl;

    fSaveTrajectory = true;
    
    if (!save) return;
    // Mark all parents to be saved as well.
    G4VTrajectory* g4Traj = ND280TrajectoryMap::Get(fParentID);
    if (!g4Traj) return;
    ND280Trajectory* traj = dynamic_cast<ND280Trajectory*>(g4Traj);
    if (!traj) return;
    if (this == traj) return;
    // Protect against infinite loops.
    traj->MarkTrajectory(save);
}

//
// IT DOESN'T WORK BECAUSE NOT ALL THE UNIT (momentum) ARE AVAILABLE:
//
//  G4BestUnit: the category Momentum does not exist.
//
// -------- EEEE ------- G4Exception-START -------- EEEE -------
// *** G4Exception : InvalidCall
//       issued by : G4BestUnit::G4BestUnit()
// Missing unit category !
// *** Fatal Exception *** core dump ***
// -------- EEEE -------- G4Exception-END --------- EEEE -------
void ND280Trajectory::ShowTrajectory(std::ostream& os) const {
    // Invoke the default implementation in G4VTrajectory...
    G4VTrajectory::ShowTrajectory(os);
    // ... or override with your own code here.
}

//
// IT DOESN'T COMPILE
//
// void ND280Trajectory::DrawTrajectory(G4int i_mode) const {
//     // Invoke the default implementation in G4VTrajectory...
//     G4VTrajectory::DrawTrajectory(i_mode);
//     // ... or override with your own code here.
// }

const std::map<G4String,G4AttDef>* ND280Trajectory::GetAttDefs() const {
    G4bool isNew;
    std::map<G4String,G4AttDef>* store
        = G4AttDefStore::GetInstance("ND280Trajectory",isNew);
    
    if (isNew) {
        G4String ID("ID");
        (*store)[ID] = G4AttDef(ID,
                                "Track ID",
                                "Bookkeeping","","G4int");
        
        G4String PID("PID");
        (*store)[PID] = G4AttDef(PID,
                                 "Parent ID",
                                 "Bookkeeping","","G4int");
        
        G4String PN("PN");
        (*store)[PN] = G4AttDef(PN,
                                "Particle Name",
                                "Physics","","G4String");
        
        G4String Ch("Ch");
        (*store)[Ch] = G4AttDef(Ch,
                                "Charge",
                                "Physics","","G4double");
        
        G4String PDG("PDG");
        (*store)[PDG] = G4AttDef(PDG,
                                 "PDG Encoding",
                                 "Physics","","G4int");
        
        G4String IMom("IMom");
        (*store)[IMom] = G4AttDef(IMom, 
                                  "Momentum of track at start of trajectory",
                                  "Physics","","G4ThreeVector");

        G4String ICosTheta("ICosTheta");
        (*store)[ICosTheta] = G4AttDef(ICosTheta, 
                                  "CosTheta of track at start of trajectory",
                                  "Physics","","G4double");
        
        G4String NTP("NTP");
        (*store)[NTP] = G4AttDef(NTP,
                                 "No. of points",
                                 "Physics","","G4int");
        
    }
    return store;
}

std::vector<G4AttValue>* ND280Trajectory::CreateAttValues() const {
    std::string c;
    std::ostringstream s(c);
    
    std::vector<G4AttValue>* values = new std::vector<G4AttValue>;
    
    s.seekp(std::ios::beg);
    s << fTrackID << std::ends;
    values->push_back(G4AttValue("ID",c.c_str(),""));
    
    s.seekp(std::ios::beg);
    s << fParentID << std::ends;
    values->push_back(G4AttValue("PID",c.c_str(),""));
    
    values->push_back(G4AttValue("PN",fParticleName,""));
    
    s.seekp(std::ios::beg);
    s << fPDGCharge << std::ends;
    values->push_back(G4AttValue("Ch",c.c_str(),""));
    
    s.seekp(std::ios::beg);
    s << fPDGEncoding << std::ends;
    values->push_back(G4AttValue("PDG",c.c_str(),""));
    
    s.seekp(std::ios::beg);
    s << G4BestUnit(fInitialMomentum,"Energy") << std::ends;
    values->push_back(G4AttValue("IMom",c.c_str(),""));

    s.seekp(std::ios::beg);
    //s << G4BestUnit(fInitialCosTheta,"CosTheta") << std::ends;
    values->push_back(G4AttValue("ICosTheta",c.c_str(),""));
    
    s.seekp(std::ios::beg);
    s << GetPointEntries() << std::ends;
    values->push_back(G4AttValue("NTP",c.c_str(),""));
    
    return values;
}

void ND280Trajectory::AppendStep(const G4Step* aStep) {

  //G4cout << "ND280Trajectory::AppendStep" << G4endl;

    ND280TrajectoryPoint* point = new ND280TrajectoryPoint(aStep);
    fPositionRecord->push_back(point);
}
  
G4ParticleDefinition* ND280Trajectory::GetParticleDefinition() const {
    return (G4ParticleTable::GetParticleTable()->FindParticle(fParticleName));
}

void ND280Trajectory::MergeTrajectory(G4VTrajectory* secondTrajectory) {

  //G4cout << "ND280Trajectory::MergeTrajectory" << G4endl;

    if(!secondTrajectory) return;
    ND280Trajectory* seco = (ND280Trajectory*)secondTrajectory;
    G4int ent = seco->GetPointEntries();
    // initial point of the second trajectory should not be merged
    for(G4int i=1; i<ent; ++i) { 
        fPositionRecord->push_back((*(seco->fPositionRecord))[i]);
    }
    delete (*seco->fPositionRecord)[0];
    seco->fPositionRecord->clear();
}


