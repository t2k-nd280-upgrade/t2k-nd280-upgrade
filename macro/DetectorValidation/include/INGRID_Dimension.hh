#ifndef _INGRID_Dimension_H
#define _INGRID_Dimension_H

#include<iostream>
#include<sstream>
#include<fstream>
using namespace std;

//const static double IronThick       =   6.5   ;     //cm
const static double IronThick       =   3.5   ;     //cm
const static double ScintiWidth     =   5.0   ;     //cm
const static double ScintiThick     =   1.0   ;     //cm
const static double PlnThick        =   4.2   ;     //cm
const static double VetoOffsetZX    =   -2.0  ;     //cm
const static double VetoStartZ      =   +1.0  ;     //cm
const static double VetoOffsetZY    =   -1.0  ;     //cm
const static double VetoOffsetRight = -10.575-2.5 ; //cm
const static double VetoOffsetLeft  = 130.9  -2.5 ; //cm
const static double VetoOffsetBottom=  -5.9   ;     //cm
const static double VetoOffsetUp    = 128.4   ;     //cm

const static double ScibarWidth     =   2.5   ;     //cm
const static double PlnThick_PM     =   4.6   ;     //cm
const static double PlnThick_front_PM=   5.0   ;     //cm
const static double PlnDist_PM      =   2.3   ;     //cm
const static double VetoOffsetZX_PM    =   -1.55  ;     //cm
const static double VetoOffsetZY_PM    =   -1.55  ;     //cm
const static double VetoOffsetRight_PM = -6 ; //cm
const static double VetoOffsetLeft_PM  = 125 ; //cm
const static double VetoOffsetBottom_PM=  -6   ;     //cm
const static double VetoOffsetUp_PM    = 125   ;     //cm
const static double VetoStartZ_PM      =   -0.55  ;     //cm

//B2///////////////////
const static double B2ScintiWidthfor = 5.0;
const static double B2ScintiThickfor = 1.0;
const static double B2PlnThickfor = 4.2;
const static double B2IronThickfor = 3.5;

const static double B2ScintiWidthside = 5.0;
const static double B2ScintiThickside = 1.0;
const static double B2PlnThickside = 4.2;
const static double B2IronThickside = 3.5;

const static double B2ScintiWidthback = 5.0;
const static double B2ScintiThickback = 1.0;
const static double B2PlnThickback = 5.0;
//const static double B2IronThickback = 3.5;

//const static double B2ScintiWidtharoundwater = 5.0;
//const static double B2ScintiThickaroundwater = 1.0;
//const static double B2PlnThickaroundwater = 0.0;

const static double B2ScintiWidthinwater = 5.0;
const static double B2ScintiThickinwater = 0.3;
const static double B2PlnThickinwater = 15.0;



class INGRID_Dimension{
private:

  double VETOOffsetZ;
public:
  INGRID_Dimension(){
  };

  ~INGRID_Dimension(){};

  bool get_pos(int mod, int pln, int ch, bool tpl, bool veto, double *posxy, double *posz);

  //########## For New Data Structure #############
  //###############################################
  bool get_posXY(int mod, int view, int pln, int ch,
		 double *posxy, double *posz);
  bool get_posVeto(int mod, int view, int pln, int ch, 
		   double *posxy, double *posz);//for new data structure (not complete)
  //###############################################


  bool get_expch(int mod, int pln, int *ch, bool tpl, bool veto, double a, double b);
  bool get_expchXY(int mod, int view, int pln, int *ch, double a, double b);


};
#endif

