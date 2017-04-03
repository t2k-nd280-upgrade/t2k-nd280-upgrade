#ifndef DeDxMap_hxx
#define DeDxMap_hxx 1

#include <recpack/DynamicProperty.h>
#include <Units.hxx>

using namespace Recpack;

#define NPBINS1 38
const double  _de_dx_min_def1 = 0.195*units::MeV/units::mm;

namespace Recpack {

  //! A EVector depending on the position 
  class DeDxMap: public DynamicProperty {
  protected:

    double  _de_dx[NPBINS1];
    double  _de_dx_elec[NPBINS1];
    double  _pbins[NPBINS1];
      
    double _melectron;
    double _mproton;
    double _mmuon;
    double _mpion;
    double _mkaon;

    double _de_dx_min;
  
  public:
    
    //! constructor with dimensions, null vector
    DeDxMap(double de_dx_min = _de_dx_min_def1);
    
    //! default destructor
    virtual ~DeDxMap(){};

    //! return non-const vector 
    double property(const State& state) const;

    double de_dx_min(){return _de_dx_min;}
    
  };
    
} 
#endif

