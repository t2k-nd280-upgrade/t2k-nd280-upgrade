#ifndef RootVirtualVolume_h
#define RootVirtualVolume_h 1

#include <recpack/EGeo.h>
#include <recpack/EAlgebra.h>
#include <recpack/dictionary.h>
#include <recpack/Messenger.h>
#include <recpack/Volume.h>

using namespace Recpack;

namespace Recpack {

  //! base class for a surface
  class RootVirtualVolume {	
  protected:

    //! extra names of the surface
    key_vector _conditions;


    bool _virtual_father;
    std::string _father_name;
    std::string _virt_name;
    std::string _full_name;
    std::vector<std::string> _included_volumes1;
    std::vector<std::string> _included_volumes2;
    bool _include1;
    bool _include2;


    Volume* _volume;

  public:

    //! default constructor
    RootVirtualVolume(const std::string& father_name, bool virtual_father, const std::string& virt_name, 
		      const std::vector<std::string>& volumes_to_include1,
		      const std::vector<std::string>& volumes_to_include2, bool include_exclude1, bool include_exclude2);
    RootVirtualVolume(const std::string& father_name, bool virtual_father, const std::string& virt_name, 
		      const std::vector<std::string>& volumes_to_include1, bool include_exclude);

    //! default destructor
    virtual ~RootVirtualVolume() {delete _volume;}


    const key_vector& conditions() const {return _conditions;}

    bool is_in_name(const Key& vol);

    void apply_tolerance(double tol);

    void update(Volume* vol);
    Volume* get_RecPack_volume(){return _volume;}

    const std::string& name(){return _full_name;}
    const std::string& father_name(){return _father_name;}
    const std::string& virt_name(){return _virt_name;}

  };

  //! print the information of the surface
  //  std::ostream& operator<< (std::ostream& os, const RootVirtualVolume& surface){} 
}
#endif
