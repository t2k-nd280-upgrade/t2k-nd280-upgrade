#include "WeightType.hxx"

WeightType operator -(Float_t a, const WeightType& w){return WeightType(a-w.Correction, a-w.Systematic);}
WeightType operator *(Float_t a, const WeightType& w){return WeightType(a*w.Correction, a*w.Systematic);}

std::ostream& operator<<(std::ostream &os, const WeightType& w){
  os << "(" << w.Correction << ", " << w.Systematic << ")";
  return os;
}
