#ifndef RecPackGeometry_hxx_seen
#define RecPackGeometry_hxx_seen

#include <vector>
#include <map>
#include <stdexcept>

//------ ROOT includes ---------

#include <TGeoVolume.h>

//------ RecPack includes -------

#include <recpack/Definitions.h>
#include <recpack/string_tools.h>
#include <recpack/stc_tools.h>

//------ Interface includes ---------

#include "RootSetup.h"
#include "DeDxMap.hxx"
#include "AnaBFieldMap.hxx"
#include "Parameters.hxx"


class RecPackGeometry {
public:
  virtual ~RecPackGeometry() {/*delete _root_setup;*/}

  /// Get a pointer to the singleton instance of the geometry information.
  static RecPackGeometry& Get(int verb=0);

  Recpack::RootSetup& GetSetup(void){return *_root_setup;}

  //  void SetPropertiesToVolume(const std::string& vol_name, std::vector<std::string> exclude_vol_name=std::vector<std::string>());
  void SetPropertiesToVolume(const std::string& vol_name, 
      const std::vector<std::string>& volumes_to_include1,
      const std::vector<std::string>& volumes_to_include2, bool include_exclude);

  void SetPropertiesToVirtualVolume(const std::string& father_name, const std::string& vol_name, 
      const std::vector<std::string>& volumes_to_include1,
      const std::vector<std::string>& volumes_to_include2, bool include_exclude);


  TGeoManager& root_geom_man(){return *fGeoManager;}

  const std::string& volume_name(int ID){return _root_setup->volume_name(ID);}
  const std::string& surface_name(int ID){return _root_setup->surface_name(ID);}

  void SetVerbosity(int verb){_verb=verb;} 
  int GetVerbosity() const {return _verb;} 

  void DumpVolumeProperties();

private:
  RecPackGeometry();
  RecPackGeometry(const RecPackGeometry& man);
  RecPackGeometry& operator=(const RecPackGeometry& man);


  /// Fill the internal tables.
  void FillInformation(void);

  /// Read names related to each sub-detector
  void FillNames(void);


  void RecurseGeometry(int depth, std::string name) {
    TGeoNode* node = fGeoManager->GetCurrentNode();

    name = name + "/" + node->GetName();

    if (!VisitNode(depth, name, node)) {
      fGeoManager->CdUp();
      return;
    }

    // Push all of the daughters onto the stack.
    for (int i=0; i<node->GetNdaughters(); ++i) {
      fGeoManager->CdDown(i);
      RecurseGeometry(depth + 1, name);
    }

    fGeoManager->CdUp();
  }

  void VisitGeometry() {
    
    
    fGeoManager->PushPath();
    fGeoManager->CdTop();
    
    RecurseGeometry(0,"");
    fGeoManager->PopPath();
  }; 


  bool VisitNode(int depth, const std::string& name, const TGeoNode* node);

  /// Clear the internal tables.
  void Clear(void);

  /// The static pointer to the singleton instance.
  static RecPackGeometry* fRecPackGeometry;

  /// The geometry manager that is currently described in this class.
  TGeoManager* fGeoManager;

  int _verb;

  Recpack::RootSetup* _root_setup;

  EVector _Bfield;
  AnaBFieldMap _BfieldMap;

  std::map<std::string, double> _X0_map;
  std::map<std::string, double> _nel_map;
  std::map<std::string, DeDxMap*> _de_dx_map;


  EVector _surfNormal;
  EVector _surfNormalECAL[6];

  EVector _xaxis;
  EVector _yaxis;
  EVector _zaxis;

  bool _initialised;


  //name of modules related to particular subdetectors
  //to be filled on initialization step
  std::vector<RP::Key>  fTPCMODULES1;
  std::vector<RP::Key>  fTPCMODULES2;
  std::vector<RP::Key>  fTPCMODULES3;
  std::vector<RP::Key>  fTPCMODULES4;
  std::vector<RP::Key>  fTPCMODULES5;
  std::vector<RP::Key>  fTPCMODULES6;
  std::vector<RP::Key>  fTPCMODULES7;



  std::vector<RP::Key>  fTOFMODULES1;
  std::vector<RP::Key>  fTOFMODULES2;
  std::vector<RP::Key>  fTOFMODULES3;
  std::vector<RP::Key>  fTOFMODULES4;
  std::vector<RP::Key>  fTOFMODULES5;
  std::vector<RP::Key>  fTOFMODULES6;
  std::vector<RP::Key>  fTOFMODULES7;
  std::vector<RP::Key>  fTOFMODULES8;
  std::vector<RP::Key>  fTOFMODULES9;
  std::vector<RP::Key>  fTOFMODULES10;
  std::vector<RP::Key>  fTOFMODULES11;
  std::vector<RP::Key>  fTOFMODULES12;

  std::vector<RP::Key>  fFGDMODULES1;
  std::vector<RP::Key>  fFGDMODULES2;
  std::vector<RP::Key>  fTARGET1;
  std::vector<RP::Key>  fTARGET2;

  std::vector<RP::Key>  fECALMODULES;
  std::vector<RP::Key>  fP0DMODULES;

  std::string _root_file_name;
  bool fExcludeTPCDeadMaterial;
  bool fSplitSMRDSideVolumes;
  bool fIncludeTPCMMVolumes;

};

namespace ND{

  RecPackGeometry& gman(int verb=0);

};


#endif
