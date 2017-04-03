#ifndef RootVolume_h
#define RootVolume_h 1

#include <recpack/EGeo.h>
#include <recpack/EAlgebra.h>
#include <recpack/dictionary.h>
#include <recpack/Messenger.h>

using namespace Recpack;

namespace Recpack {

  //! base class for a surface
  class RootVolume {	
  protected:

    //! extra names of the surface
    key_vector _conditions;

    bool _has_surface;
    bool _has_volume;
    std::vector<EVector> _axes;

  public:

    //! default constructor
    RootVolume(const Key& name) 
      {_has_surface=false;_has_volume=true;
      _conditions.push_back(name);}
    RootVolume(const Key& name1,const Key& name2)
      {_has_surface=false;_has_volume=true;
      _conditions.push_back(name1);
      _conditions.push_back(name2);}
    RootVolume(const Key& name1,const Key& name2,const Key& name3 ) 
      {_has_surface=false;_has_volume=true;
      _conditions.push_back(name1);
      _conditions.push_back(name2);
      _conditions.push_back(name3);}

    //! default destructor
    virtual ~RootVolume() {}


    const key_vector& conditions() const {return _conditions;}

    bool is_in_name(const Key& vol);

    bool has_surface() const {return _has_surface;}
    void set_has_surface(bool surf){_has_surface = surf;}

    bool has_volume() const {return _has_volume;}
    void set_has_volume(bool vol){_has_volume = vol;}

    const std::vector<EVector>& axes() const {return _axes;}

    void add_axis(EVector axis) {_axes.push_back(axis);}

  };

  //! print the information of the surface
  //  std::ostream& operator<< (std::ostream& os, const RootVolume& surface){} 
}
#endif
