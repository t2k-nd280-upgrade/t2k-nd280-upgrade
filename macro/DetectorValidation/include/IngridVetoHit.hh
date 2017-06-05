#ifndef IngridVetoHit_h
#define IngridVetoHit_h 1
 
#include "G4ThreeVector.hh"
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class IngridVetoHit : public G4VHit 
{
public:
  //IngridVetoHit(G4int, G4int, G4int, G4double, const G4ThreeVector&, G4double);
  IngridVetoHit(G4int, G4int, G4int, G4double, G4double, G4ThreeVector, G4double);
  IngridVetoHit(G4int, G4double, G4int);
  IngridVetoHit(G4int, G4double);

  ~IngridVetoHit();
  //copy constructor & assignment operator
  IngridVetoHit(const IngridVetoHit &right);
  const IngridVetoHit& operator=(const IngridVetoHit &right);
  G4int operator==(const IngridVetoHit &right) const;
  G4int CompareID(const IngridVetoHit right) ;
  G4int CompareP(const IngridVetoHit right);
  G4int isFaster(const IngridVetoHit right);

  // new/delete operators
  inline void *operator new(size_t);
  inline void operator delete(void  *aHit);

  void Draw();
  void Print();
  void Print_WeakEdep(); // Edep < 0.5 GeV
  void Print_Hit();
  
private:
    G4int detID;
    G4int trackID;
    G4double edep;
    G4double edep_q;
    G4double pe;
    G4double lope;
    G4int Particle;     // PDG encoding of the particle
    G4ThreeVector position;
    G4ThreeVector posinmod;
    G4double time;
    G4double delay_time;
    G4int eventID;

    G4int mod;
    G4int view;
    G4int pln;
    G4int ch;

  //set/get functions 
public:
	inline void AddEdep(G4double e, G4double eq) { edep += e; edep_q += eq; }
	inline void SetEventID(G4int event_ID){ eventID = event_ID;}
	inline void SetPE(G4double p) { pe = p; }
	inline void SetLOPE(G4double p) { lope = p; }
	inline void SetParticle(G4int p) { Particle = p; }
	inline void SetTime(G4double t) { time = t; }
	inline void SetDelayTime(G4double dt) { delay_time = dt; }
  
	inline G4int GetDetID() { return detID; }
	inline G4int GetTrackID() { return trackID; }
	inline G4int GetParticle() { return Particle; }
	inline G4double GetEdep() { return edep; }
	inline G4double GetEdepQ() { return edep_q; }
	inline G4double GetTime() { return time; }
	inline G4double GetDelayTime() { return delay_time; }
	inline G4ThreeVector GetPosition() { return position; }
	inline G4ThreeVector GetPosInMod() { return posinmod; }
	inline G4int GetEventID() { return eventID; }
	inline G4int GetMod() { return mod; }
	inline G4int GetView() { return view; }
	inline G4int GetPln() { return pln; }
	inline G4int GetCh() { return ch; }
	inline G4double GetPE() { return pe; }
	inline G4double GetLOPE() { return lope; }

};


// ====================================================================
// inline functions
// ====================================================================

// externally instanciated.
typedef G4THitsCollection<IngridVetoHit> IngridVetoHitsCollection;

extern G4Allocator<IngridVetoHit> IngridVetoHitAllocator; 

inline void* IngridVetoHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) IngridVetoHitAllocator.MallocSingle();
  return aHit;
}

inline void IngridVetoHit::operator delete(void *aHit)
{
  IngridVetoHitAllocator.FreeSingle((IngridVetoHit*) aHit);
}

#endif
