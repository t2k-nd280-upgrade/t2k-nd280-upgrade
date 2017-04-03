#ifndef RootSetup_h
#define RootSetup_h 1

//---------- RecPack includes -----

#include <recpack/EGeo.h>
#include <recpack/EAlgebra.h>
#include <recpack/dictionary.h>
#include <recpack/Messenger.h>
#include <recpack/Setup.h>


//---------- ROOT includes -----

#include <TGeoManager.h>
#include <TGeoNode.h>
#include <TGeoMatrix.h>
#include <TGeoVolume.h>
#include <TGeoBBox.h>

//--------- interface class ------

#include "RootVolume.h"
#include "RootVirtualVolume.h"

using namespace Recpack;

namespace Recpack {

  //! base class for a surface
  class RootSetup: public Setup {	
  protected:

    //! vector of volume name patterns to be included
    std::vector<RootVirtualVolume*> _virtual_volumes;

    //! vector of volume name patterns to be included
    std::vector<RootVolume*> _volumes;

    //! vector of volumes to be excluded
    std::vector<RootVolume*> _excluded_volumes;

    //! the mother volume
    RootVolume* _root_mother;

    //! The geometry manager that is currently described in this class.
    TGeoManager* _GeoManager;

    std::map<int,std::string> _nodeIDs;
    std::vector<std::string> _surfaces_nodeIDs;

    std::string _mother_name;

    double _tolerance;

    std::map<Key,Key> _saved_volumes;

    int _verb;

  public:

    //! default constructor the setup name
    RootSetup(const Key& name ) 
      {_GeoManager = NULL; set_name(name); _nodeIDs.clear();_surfaces_nodeIDs.resize(150000);_tolerance=0.01*mm;}

    //! Constructor provided the ROOT geometry manager and the setup name
    RootSetup(TGeoManager* man, const Key& name) 
      {_GeoManager = man; set_name(name); _nodeIDs.clear();_surfaces_nodeIDs.resize(150000);_tolerance=0.01*mm;}

    //! default destructor
    virtual ~RootSetup() {}


    void SetMother(const Key& name){
      _root_mother = new RootVolume(name);
      _mother_name = name;
      }

    // add a volume with 1 key
    void AddVolume(const Key& name){
        _volumes.push_back(new RootVolume(name));}

    // add a volume with 2 keys
    void AddVolume(const Key& n1,const Key& n2)
      {_volumes.push_back(new RootVolume(n1,n2));}

    // add a volume with 3 key
    void AddVolume(const Key& n1, const Key& n2,const Key& n3)
      {_volumes.push_back(new RootVolume(n1,n2,n3));}

    // exclude a volume with 1 key
    void ExcludeVolume(const Key& name)
      {_excluded_volumes.push_back(new RootVolume(name));}

    // exclude a volume with 2 keys
    void ExcludeVolume(const Key& n1, const Key& n2 )
      {_excluded_volumes.push_back(new RootVolume(n1, n2));}

    // exclude a volume with 3 keys
    void ExcludeVolume(const Key& n1, const Key& n2, const Key& n3  )
      {_excluded_volumes.push_back(new RootVolume(n1, n2, n3));}

    // add a volume with 1 key and a surface inside with given axes
    void AddVolumeSurface(const Key& name, const EVector& axis1, const EVector& axis2);

    // add a volume with 2 keys and a surface inside with given axes
    void AddVolumeSurface(const Key& n1,const Key& n2, const EVector& axis1, 
			    const EVector& axis2);

    // add a volume with 3 keys and a surface inside with given axes
    void AddVolumeSurface(const Key& n1,const Key& n2,const Key& n3, const EVector& axis1, 
			    const EVector& axis2);

    // add a surface with one key
    void AddSurface(const Key& name, const EVector& axis1, const EVector& axis2);

    // add a surface with 2 keys
    void AddSurface(const Key& n1,const Key& n2, const EVector& axis1, 
		    const EVector& axis2);

   // add a surface with 3 keys
    void AddSurface(const Key& n1,const Key& n2,const Key& n3, const EVector& axis1, 
		    const EVector& axis2);

    // add a volume to look for its name
    void AddVolumeToGetPath(const Key& n1){_saved_volumes[n1]="";}

    // Get the path of a volume
    std::string GetVolumePath(const Key& n1){return _saved_volumes[n1];}

    // Add a virtual volume surounding the ones specified
    void AddVirtualVolume(const std::string& father_name, const std::string& virt_name, 
			  const std::vector<std::string>& volumes_to_include1, 
			  bool include_exclude);

    void AddVirtualVolume(const std::string& father_name, const std::string& virt_name, 
			  const std::vector<std::string>& volumes_to_include1, 
			  const std::vector<std::string>& volumes_to_include2, 
			  bool include_exclude1, bool include_exclude2);

    bool HasVirtualVolume(const std::string& name);

    //computes the average dE/dx and X0 of a volume
    bool GetAverageProperties(const std::string& father_name, 
			      std::vector<std::string> include_vol_name1, 
			      std::vector<std::string> include_vol_name2, 
			      bool include_exclude, bool include_father, double& X0, double& dedx, double& nel, double& cap_considered);

    bool GetMixtureProperties(TGeoMaterial* mat, double& dedx, double& nel);
    bool GetElementProperties(double A, double Z, double rho, double p, double& dedx, double& nel);


    // set the tolerance for subvolumes
    void SetTolerance(double tol){_tolerance=tol;}

    void BuildSetup();

    void SetVerbosity(int verb){_verb=verb;} 
    int GetVerbosity() const {return _verb;} 

    const std::string& volume_name(int ID){return _nodeIDs[ID];}

    const std::string& surface_name(int ID){return _surfaces_nodeIDs[ID];}

    
  private:

    void set_geo_manager(TGeoManager* man)
      {_GeoManager = man;}

    Volume* BuildRecPackVolume(TGeoNode* node, const Key& name, const double tolerance = 0.);

    Surface* BuildRecPackSurface(const Volume& vol, const EVector& axis1, 
				 const EVector& axis2);


    const Key  get_father_name(const Key& son_name); 

    const double get_tolerance(const Key& father_name); 
    

  };

  //! print the information of the surface
//  std::ostream& operator<< (std::ostream& os, const RootSetup& surface){} 

}
#endif
