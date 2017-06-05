#ifndef Neut_h
#define Neut_h 1

#define NVEC 100
#define NG 20

#include <stdio.h>
#include <iostream>

#define JNUBEAM10C

//
typedef struct {
  int NumParticle, ParticleID[NVEC];
  int ParentID[NVEC], TrackingFlag[NVEC], InteractionCode[NVEC];
  float AbsMomentum[NVEC], Momentum[NVEC][3], Vertex[3];
} SecondaryVector;

//
typedef struct {
    int gipart;
    float gpos0[3],gvec0[3],gamom0;
} PrimaryParticle;

typedef struct {//for t2kreweight
  float         Crsx;
  float         Crsy;
  float         Crsz;
  float         Crsphi;
  float         Crsq2;
} CrsInfo;

typedef struct {//for t2kreweight
  int           Nvert;
  float         Posvert[100][3];
  int           Iflgvert[100];
  int           Nvcvert;
  float         Dirvert[300][3];
  float         Abspvert[300];
  float         Abstpvert[300];
  int           Ipvert[300];
  int           Iverti[300];
  int           Ivertf[300];
  float         Fsiprob;
  int           Ibound;
} FsiHist;


//
typedef struct {
    int spid;
    int pgen;
    float psi0,xsi0[3],nsi0[3],cossi0bm;
    float xsi[3];
    int smech;
    int intgt;
    float prvtx[3];
    int smed;
    int gppid;
    float xgpi0[3],xgpi[3],pgpi0;
    int gpmech, gpmed;
    int prmech, prmed;
    int prdght, sdght, gpdght;
    int chain;
} OptionalVariables;

//
typedef struct {
	int ng;
	float gpx[NG],gpy[NG],gpz[NG],gcosbm[NG];
	float gvx[NG],gvy[NG],gvz[NG];
	int gpid[NG],gmec[NG];
} Ancestor;

//
typedef struct {
	float Enusk;
	float normsk;
	float anorm;
} SKInfo;

//
typedef struct {
	int Numbndn;
	int Numbndp;
	int Numfrep;
	int Numatom;
} TargetInfo;

//
typedef struct {
	float Crsenergy;
	float Totcrsne;		// total cross-section
	float Difcrsne[8];
} NeutCrs;

//
class Neut
{
  int NumberOfEvent;

    typedef class ParentVector{
      float AbsMomentum, Vertex[3], Direction[3], CosBeam;
    } ;

public:
  struct{

    int EventNumber;

    struct{
      int Mode;
      int NumParticle, ParticleID[NVEC];
      float AbsMomentum[NVEC], Momentum[NVEC][3];
    } Primary;
    

    FsiHist         Fsi;//for t2kreweight
    SecondaryVector Secondary;

    NeutCrs neutcrs;//for t2kreweight
    CrsInfo Crs;//for t2kreweight
        
    struct{
      float Energy;
      int ParentID, ProductionMode;
      ParentVector ParentDecay;
      float Norm;
      int nvtx0;
      ParentVector ParentProduction;
      float r, x, y, Direction[3];
      int FDID;
      PrimaryParticle primary_particle;
      /*
//removed before using t2kreweight
#ifdef JNBEAM10A
      OptionalVariables optional_variables;
#endif
#ifdef JNUBEAM10C
			Ancestor ancestor;
      SKInfo sk_info;
#endif
      */
    } Neutrino; 


    TargetInfo target_info;//for t2kreweight
    //NeutCrs neutcrs;//moved for t2kreweight


  } Vector;

	int ID;

public:

  int NtupleReadInit(char*);
  int NtupleReadEvent(SecondaryVector&,float*);

};

#endif
