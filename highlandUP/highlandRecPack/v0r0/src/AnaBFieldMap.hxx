#ifndef AnaBFieldMap_hxx
#define AnaBFieldMap_hxx 1

#include <recpack/EVectorMap.h>
#include <Units.hxx>
#include <AnaBFieldManager.hxx>

//! A EVector depending on the position 
class AnaBFieldMap: public EVectorMap {
protected:
  
public:
  
  //! constructor
  AnaBFieldMap();
  
  //! default destructor
  virtual ~AnaBFieldMap(){};
  
  //! return vector property depending on position
  EVector compute_vector(const EVector& pos) const;
  
};

#endif

