#ifndef WeightType_hxx
#define WeightType_hxx

#include "BasicTypes.hxx"

class WeightType{
public:

  WeightType(Float_t w=1)         {Correction=  Systematic=w;}
  WeightType(Float_t c, Float_t s){Correction=c;Systematic=s;}

  Float_t Correction;
  Float_t Systematic;

  WeightType operator *(const WeightType& w){return WeightType(Correction*w.Correction, Systematic*w.Systematic);}
  WeightType operator /(const WeightType& w){return WeightType(Correction/w.Correction, Systematic/w.Systematic);}
  WeightType operator +(const WeightType& w){return WeightType(Correction+w.Correction, Systematic+w.Systematic);}
  WeightType operator -(const WeightType& w){return WeightType(Correction-w.Correction, Systematic-w.Systematic);}

  WeightType operator *(Float_t a){return WeightType(Correction*a, Systematic*a);}
  WeightType operator /(Float_t a){return WeightType(Correction/a, Systematic/a);}
  WeightType operator -(Float_t a){return WeightType(Correction-a, Systematic-a);}
  WeightType operator +(Float_t a){return WeightType(Correction+a, Systematic+a);}

  WeightType& operator *=(const WeightType& w){Correction*=w.Correction; Systematic*=w.Systematic; return *this;}
  WeightType& operator /=(const WeightType& w){Correction/=w.Correction; Systematic/=w.Systematic; return *this;}
  WeightType& operator *=(Float_t a){Correction*=a; Systematic*=a; return *this;}
  WeightType& operator /=(Float_t a){Correction/=a; Systematic/=a; return *this;}

  bool operator ==(const WeightType& w){return (Correction==w.Correction && Systematic==w.Systematic);}
  bool operator !=(const WeightType& w){return (Correction!=w.Correction || Systematic!=w.Systematic);}

  bool operator ==(Float_t a){return (Systematic==a);}
  bool operator !=(Float_t a){return (Systematic!=a);}



};

WeightType operator -(Float_t a, const WeightType& w);
WeightType operator *(Float_t a, const WeightType& w);

std::ostream& operator<<(std::ostream &os, const WeightType& w);


//typedef Float_t Weight_h;
typedef WeightType Weight_h;


#endif
