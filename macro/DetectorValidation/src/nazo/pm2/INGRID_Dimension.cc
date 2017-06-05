#ifndef _INGRID_Dimension_C
#define _INGRID_Dimension_C

#include<iostream>
#include<sstream>
#include<fstream>
#include"INGRID_Dimension.hh"

using namespace std;

bool INGRID_Dimension::get_pos(int mod, int pln, int ch, bool tpl, bool veto, double *posxy, double *posz){
  if(tpl){
    *posxy = (ScintiWidth)*ch;
    *posz = (PlnThick+IronThick)*pln;
  }
  if(veto){
    /*
    if(pln==11)*posxy =;
    if(pln==12)*posxy =;
    if(pln==13)*posxy =;
    if(pln==14)*posxy =;
    */
    *posxy=0;
    *posz = (ScintiWidth)*ch;
  }
  return true;
}

bool INGRID_Dimension::get_posXY(int mod, int view, int pln, int ch, double *posxy, double *posz){
  *posxy   = (ScintiWidth)*ch;
  if( pln <= 10 ){
    if(view == 1) 
      *posz    = ( PlnThick + IronThick ) * pln ;
    else if(view == 0)
      *posz    = ( PlnThick + IronThick ) * pln + ScintiThick;
    return true;
  }
  else if(pln >= 11){
    this -> get_posVeto( mod, view, pln, ch, posxy, posz );
  }
}

bool INGRID_Dimension::get_posVeto(int mod, int view, int pln, int ch, double *posxy, double *posz){

  if(pln<=10||pln>=15) return false;

  *posz    = VetoStartZ     + ScintiWidth * ch;
  if(pln==11){//############ Right  VETO ################
    //*posz  = VetoOffsetZX + ScintiWidth*ch;
    *posxy = VetoOffsetRight;
  }
  if(pln==12){//############ Left   VETO ################
    //*posz  = VetoOffsetZX + ScintiWidth*ch;
    *posxy = VetoOffsetLeft;
  }
  if(pln==13){//############ Bottom VETO ################
    //*posz  = VetoOffsetZY + ScintiWidth*ch;
    *posxy = VetoOffsetBottom;
  }
  if(pln==14){//############ Up VETO     ################
    //*posz  = VetoOffsetZY + ScintiWidth*ch;
    *posxy = VetoOffsetUp;
  }
  return true;

}

double  Wi = 0.5;

bool INGRID_Dimension::get_expch(int mod, int pln, int *ch, bool tpl, bool veto, double a, double b)
{
  if(tpl){
    double expz=pln*(PlnThick+IronThick);
    double expxy=expz*a+b;
    for(int numch=0;numch<48;numch++){
      double diffxy=expxy-numch*ScintiWidth;
      if(-Wi*ScintiWidth<=diffxy&&diffxy<Wi*ScintiWidth){
	*ch=numch;
	return true;
      }
    }
    return false;
  }

  if(veto){
    if(pln==0){//Right VETO
      double expz=(VetoOffsetRight-b)/a;
      for(int numch=0;numch<22;numch++){
	double diffxy=expz-numch*ScintiWidth+VetoOffsetZY;
	if(-Wi*ScintiWidth<=diffxy&&diffxy<Wi*ScintiWidth){
	  *ch=numch;
	  return true;
	}
      }
    }
    else if(pln==1){//LEFT VETO
      double expz=(VetoOffsetLeft-b)/a;
      for(int numch=0;numch<22;numch++){
	double diffxy=expz-numch*ScintiWidth+VetoOffsetZY;
	if(-Wi*ScintiWidth<=diffxy&&diffxy<Wi*ScintiWidth){
	  *ch=numch;
	  return true;
	}
      }
    }
    else if(pln==2){//Bottom VETO
      double expz=(VetoOffsetBottom-b)/a;
      for(int numch=0;numch<22;numch++){
	double diffxy=expz-numch*ScintiWidth+VetoOffsetZX;
	if(-Wi*ScintiWidth<=diffxy&&diffxy<Wi*ScintiWidth){
	  *ch=numch;
	  return true;
	}
      }
    }
    else if(pln==3){//UP VETO
      double expz=(VetoOffsetUp-b)/a;
      for(int numch=0;numch<22;numch++){
	double diffxy=expz-numch*ScintiWidth+VetoOffsetZX;
	if(-Wi*ScintiWidth<=diffxy&&diffxy<Wi*ScintiWidth){
	  *ch=numch;
	  return true;
	}
      }
    }

    return false;
  }

}

bool INGRID_Dimension::get_expchXY(int mod, int view, int pln, int *ch, double a, double b){
  int temp=-777;
  if(pln>=11){//VETO plane
    int veto = pln - 11;
    bool flag = this->get_expch(mod, veto, &temp, 0, 1, a, b);
    *ch = temp;
    return flag;
  }
  else {//Tracking plane
    bool flag = this->get_expch(mod, pln   , &temp, 1, 0, a, b);
    if(temp>23)return false;
    if(temp<0)return false;
    *ch = temp;
    return flag;
  }
}


#endif
