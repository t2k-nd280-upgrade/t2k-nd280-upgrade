////////////////////////////////////////////////////////////
// $Id: ND280XMLPersistencyManager.hh,v 1.31 2011/09/06 18:58:35 mcgrew Exp $
// 
#ifndef ND280InputPersistencyManager_hh_seen
#define ND280InputPersistencyManager_hh_seen
 
#include <string>
#include <vector>

class TFile;

#include "ND280PersistencyManager.hh"
#include "ExN02ND280XML.hh"

/// Provide inputs for the geant 4.
class ND280InputPersistencyManager : public ND280PersistencyManager {
public:
  virtual ~ND280InputPersistencyManager();
  
  /// If a persistency manager has not been created, create one.
  static ND280InputPersistencyManager* GetInstance(void);
  
  /// Interface with PersistencyMessenger (open and close the
  /// database).
  virtual G4bool Open(G4String dbname);
  virtual G4bool Close(void);
  
  ExN02ND280XML *GetXMLInput(){return fND280XMLInput;};
  
  void SetEventFirst(int first){fEventFirst=first;};
  void SetNEvents(int num){fNEvents=num;};

  int GetEventFirst(){return fEventFirst;};
  int GetNEvents(){return fNEvents;};

protected:
  /// use GetPersistencyManager() instead
  ND280InputPersistencyManager();

private:
  /// The XML input file.
  ExN02ND280XML *fND280XMLInput;

  G4int fEventFirst;
  G4int fNEvents;
};
#endif
