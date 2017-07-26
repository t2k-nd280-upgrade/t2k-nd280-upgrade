#ifndef ND280UP_PID
#define ND280UP_PID

#include <TH2F.h>
#include <TSpline.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TF1.h>

using namespace std;

class ND280UpPID {
public:
  ND280UpPID();
  ~ND280UpPID();

  void SetPDF(TString hyp,TH1F *hpdf);
  TH1F *GetPDF(TString hyp); 
  TGraph *GetGraph(TString hyp); 
  TSpline3 *GetSpline(TString hyp);
  
  double GetIntegral(TString hyp);

  void CalcIntegral(TString hyp,const int npoints=100000);

  double CalcLogLikeRatio(double prob1,double prob2);
  double CalcLogLikeRatio(TString hyp1,TString hyp2,double input);
  
private:

  void Init();
  void Delete();

  double GetIntegral_Muon(){return fIntegral_Muon;}
  double GetIntegral_Pion(){return fIntegral_Pion;}
  double GetIntegral_Prot(){return fIntegral_Prot;}
  double GetIntegral_Elec(){return fIntegral_Elec;}

  void SetPDF_Muon(TH1F *hpdf);
  void SetPDF_Pion(TH1F *hpdf);
  void SetPDF_Prot(TH1F *hpdf);
  void SetPDF_Elec(TH1F *hpdf);

  TH1F *GetPDF_Muon(){return fPDF_Muon;}
  TH1F *GetPDF_Pion(){return fPDF_Pion;}
  TH1F *GetPDF_Prot(){return fPDF_Prot;}
  TH1F *GetPDF_Elec(){return fPDF_Elec;}

  TGraph *GetGraph_Muon(){return fGraph_Muon;}
  TGraph *GetGraph_Pion(){return fGraph_Pion;}
  TGraph *GetGraph_Prot(){return fGraph_Prot;}
  TGraph *GetGraph_Elec(){return fGraph_Elec;}

  TSpline3 *GetSpline_Muon(){return fSpline_Muon;}
  TSpline3 *GetSpline_Pion(){return fSpline_Pion;}
  TSpline3 *GetSpline_Prot(){return fSpline_Prot;}
  TSpline3 *GetSpline_Elec(){return fSpline_Elec;}

  TH1F *fPDF_Muon;
  TH1F *fPDF_Pion;
  TH1F *fPDF_Prot;
  TH1F *fPDF_Elec;

  TGraph *fGraph_Muon;
  TGraph *fGraph_Pion;
  TGraph *fGraph_Prot;
  TGraph *fGraph_Elec;
  
  TSpline3 *fSpline_Muon;
  TSpline3 *fSpline_Pion;
  TSpline3 *fSpline_Prot;
  TSpline3 *fSpline_Elec;

  double fIntegral_Muon;
  double fIntegral_Pion;
  double fIntegral_Prot;
  double fIntegral_Elec;
};

#endif
