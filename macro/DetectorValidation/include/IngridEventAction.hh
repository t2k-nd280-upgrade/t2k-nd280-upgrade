#ifndef IngridEventAction_h
#define IngridEventAction_h 1

#include "G4UserEventAction.hh"
#include "IngridRunAction.hh"
#include <vector>

class G4Event;

//
class IngridEventAction : public G4UserEventAction
{
public:

  IngridEventAction(IngridRunAction*); 
  ~IngridEventAction();

public:
  void BeginOfEventAction(const G4Event* anEvent);
  void EndOfEventAction(const G4Event* anEvent);

  inline void SetTrackID(int i) { TrackID = i; }
  int GetTrackID() { return TrackID; }
  inline void SetWriteFlag(int j) { Flag = j; }

private:
  IngridRunAction* runaction;
  int TrackID;
  int Flag;
    
    IngridSimHitSummary *ingsimhit;
    IngridHitSummary *inghit;

};

#endif
