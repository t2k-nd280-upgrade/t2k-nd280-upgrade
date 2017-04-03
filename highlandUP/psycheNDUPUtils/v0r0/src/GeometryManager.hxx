#ifndef GeometryManager_hxx_seen
#define GeometryManager_hxx_seen

#include <vector>
#include <map>
#include <stdexcept>

//------ ROOT includes ---------

#include <TGeoVolume.h>
#include <TGeoManager.h>
#include <TGeoNode.h>
#include <TGeoMatrix.h>
#include "BasicUtils.hxx"


/// This exception is thrown  when fail to open a file with geometry
class BadRootFileException: public std::runtime_error {
  public:
    BadRootFileException() :
      std::runtime_error("Error reading file with ROOT geometry") {
    }
};

/// This exception is thrown when no geometry is found in a file
class NoGeometryManagerException: public std::runtime_error {
  public:
    NoGeometryManagerException() :
      std::runtime_error("No ROOT geometry found in file") {
    }
};

/// This exception is thrown when the geometry is not a proper one (not with t2k volume)
class BadGeometryManagerException: public std::runtime_error {
  public:
    BadGeometryManagerException() :
      std::runtime_error("No t2k volume found in the geometry") {
    }
};


class GeometryManager {
public:
  virtual ~GeometryManager() {}

  /// Get a pointer to the singleton instance of the geometry information.
  static GeometryManager& Get();

  // Returns the ROOT TGeoManager
  TGeoManager* GeoManager(){
    
    TGeoManager* man = NULL;
    
    if (_currentGeomID>=0 && _currentGeomID<(Int_t)(_GeoManagers.size())) 
      man = _GeoManagers[_currentGeomID];
    else{
      //geometry is not loaded yet,  load default one
      LoadGeometry();
      if (_currentGeomID>=0 && _currentGeomID<(Int_t)(_GeoManagers.size())) 
        man = _GeoManagers[_currentGeomID];
    }
    
    if (man) return man;
    
    std::cout << "ERROR. GeometryManager::GeoManager() failed to retrieve valid manager" << std::endl;
    exit(1);
  }

  /// General function to initialize the geometry
  /// For the moment used to set appropriate ECal volume definitions depending on Data/MC status
  void InitializeGeometry(bool IsMC = true) const;

  /// Overrride the values in DetectorDefinion with the ones extracted from the ROOT geometry
  void InitializeDetectorDefinitionFromROOTGeometry() const;
  
  /// Fill FGD info
  void FillFGDInfoFromROOTGeometry() const;

  /// Get the volume position and size from the ROOT geometry
  void GetVolumeProperties(const std::string& name, Double_t* pos, Double_t* size) const;

  /// Overrride the values in DetectorDefinion with the ones extracted from the ROOT geometry  
  void SetDetectorBoundaries(const std::string& det_name, Float_t* det_min, Float_t* det_max, 
                             const std::string& name1, const std::string& name2="") const;
  

  /// Load the TGeoManager from the input root file. Returns true when the new geometry was loaded
  bool LoadGeometry(const std::string& file="",Int_t geomID=-1, const std::string& geomDir="");
  
  /// Set detector boundaries
  void SetDetectorBoundaries(Float_t* detMin, Float_t* detMax, const Float_t* min, const Float_t* max) const{
    anaUtils::CopyArray(min,  detMin, 3);
    anaUtils::CopyArray(max,  detMax, 3);
  }
  
  /// Retrieve current geometry ID
  Int_t GetCurrentGeomID() const{
    return _currentGeomID;
  }
   
private:
  GeometryManager();
  GeometryManager(const GeometryManager& man);
  GeometryManager& operator=(const GeometryManager& man);

  /// The static pointer to the singleton instance.
  static GeometryManager* fGeometryManager;

  /// The geometry manager that is currently described in this class.
  mutable std::vector<TGeoManager*> _GeoManagers;

  /// default geometry file name
  std::string _defaultFileName;

  /// Use the default geometry file under psycheUtils/data
  bool _useDefaultGeometry;

  /// The current geometry ID
  Int_t _currentGeomID;
  
  /// Loop through the ROOT geometry and retrieve the info for detetor volume envelopes
  void FillVolumesWithGeometryLoop() const;
 
  
  /// Process GeoNode including it into particular detector volume
  /// returns boolean which defines whether to go deeper into the ROOT volume hierarchy: saves time
  bool VisitNode(const std::string& name, const TGeoNode* node) const;
  
  /// Update particular volume bound: if the provided low/high values exceed the current limits
  void UpdateVolumeBound(Float_t* detMin, Float_t* detMax, const Float_t* low, const Float_t* high) const;
 
  /// Navigate geometry processing the nodes
  void RecurseGeometry(std::string name) const;
  
  /// Reset volume definitions
  void ResetVolumeDefinitions() const;
  
};

namespace ND{
  GeometryManager& hgman();
}


#endif
