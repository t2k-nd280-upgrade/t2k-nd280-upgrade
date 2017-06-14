#ifndef INGRID_RUN_ACTION_H
#define INGRID_RUN_ACTION_H

#include "TTree.h"
#include "TFile.h"

#include "G4UserRunAction.hh"
#include "INGRIDEVENTSUMMARY.h"

class G4Run;

class IngridRunAction : public G4UserRunAction {
public:
  //IngridRunAction(char* filename,float,float,double);
  IngridRunAction(char* filename);
  virtual ~IngridRunAction();

  virtual void BeginOfRunAction(const G4Run* aRun);
  virtual void EndOfRunAction(const G4Run* aRun);

  inline  TTree* GetTree(){ return tree; }
  inline  IngridEventSummary* GetEvtSum(){ return evt; }
  
  inline  TTree* GetSKTree(){ return h1; }//for t2kreweight

  void Clear() { evt->Clear("C"); }

  inline float GetX() {return x;}
  inline float GetY() {return y;}

  float vertex[3];

  Long64_t NotEntry;

  double HitEff() { return hiteff/100.; }	

  //variables for t2kreweight                                                                   
  Int_t           numnu;
  Int_t           mode;
  Int_t           ipnu[50];   //[numnu]                                                         
  Float_t         pnu[50];   //[numnu]  GeV/c                                                   
  Float_t         dirnu[50][3];   //[numnu]                                                     
  Float_t         Crsx;
  Float_t         Crsy;
  Float_t         Crsz;
  Float_t         Crsphi;
  Int_t           Nvert;
  Float_t         Posvert[150][3];   //[Nvert]                                                  
  Int_t           Iflgvert[150];   //[Nvert]                                                    
  Int_t           Nvcvert;
  Float_t         Dirvert[900][3];   //[Nvcvert]                                                
  Float_t         Abspvert[900];   //[Nvcvert]                                                  
  Float_t         Abstpvert[900];   //[Nvcvert]                                                 
  Int_t           Ipvert[900];   //[Nvcvert]                                                    
  Int_t           Iverti[900];   //[Nvcvert]                                                    
  Int_t           Ivertf[900];   //[Nvcvert]                                                    
  Float_t         Fsiprob;
  Int_t           Numbndn;
  Int_t           Numbndp;
  Int_t           Numfrep;
  Int_t           Numatom;
  Int_t           Ibound;

  Int_t           nring;
  Int_t           nev;
  UInt_t          nhitac;
  Float_t         wall;
  Float_t         evis;
  UInt_t          ip[10];   //[nring]                                                           
  Float_t         dir[10][3];   //[nring]                                                       
  Float_t         amom[10];   //[nring]                                                         
  Float_t         amome[10];   //[nring]                                                        
  Float_t         amomm[10];   //[nring]                                                        
  Float_t         pi0like[2];
  Float_t         pi0_e[2][2];
  Float_t         pi0_dir[2][2][3];
  Float_t         pi0mass[2];
  Int_t           nmue;
  Int_t           npar;
  Float_t         wallv;
  UInt_t          ipv[50];   //[npar]                                                           
  Float_t         posv[3];
  Float_t         dirv[50][3];   //[npar]                                                       
  Float_t         pmomv[50];   //[npar]  MeV/c                                                  
  Int_t           nscndprt;
  Int_t           itrkscnd[200];   //[nscndprt] 
  Float_t         vtxscnd[200][3];   //[nscndprt]                                               
  Float_t         pscnd[200][3];   //[nscndprt]                                                 
  Int_t           iprtscnd[200];   //[nscndprt]                                                 
  Float_t         tscnd[200];   //[nscndprt]                                                    
  Int_t           iprntprt[200];   //[nscndprt]                                                 
  Int_t           lmecscnd[200];   //[nscndprt]                                                 
  Int_t           iprnttrk[200];   //[nscndprt]                                                 
  Int_t           iorgprt[200];   //[nscndprt]                                                  
  Int_t           iflgscnd[200];   //[nscndprt]                                                 
  Float_t         pprnt[200][3];   //[nscndprt]                                                 
  Int_t           Npvc;
  Int_t           Ipvc[100];   //[Npvc]                                                         
  Int_t           Ichvc[100];   //[Npvc]                                                        
  Int_t           Iorgvc[100];   //[Npvc]                                                       
  Int_t           Iflvc[100];   //[Npvc]                                                        
  Float_t         Abspvc[100];   //[Npvc]                                                       
  Float_t         Pvc[100][3];   //[Npvc]  

private:
  float x;
  float y;
  char file[256];
  TFile* f;
  TTree* tree;
  TTree* h1;//for t2kreweight      

  IngridEventSummary *evt;

  long seed;

  double hiteff; //%


};

#endif
