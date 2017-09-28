
#include <iomanip>

#include <TMath.h>
#include <TString.h>

#include "ND280UpConst.hh"

#include "ND280UpPID.hh"

#define DEBUG 0 // 0 --> disable debugging
                // 1 --> enable debugging

using namespace std;


ND280UpPID::ND280UpPID()
{
  this->Init();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ND280UpPID::~ND280UpPID()
{
  this->Delete();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpPID::Init(){
  fPDF_Muon=0;
  fPDF_Pion=0;
  fPDF_Prot=0;
  fPDF_Elec=0;

  fGraph_Muon=0;
  fGraph_Pion=0;
  fGraph_Prot=0;
  fGraph_Elec=0;

  fSpline_Muon=0;
  fSpline_Pion=0;
  fSpline_Prot=0;
  fSpline_Elec=0;

  fIntegral_Muon=0;
  fIntegral_Pion=0;
  fIntegral_Prot=0;
  fIntegral_Elec=0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpPID::Delete(){

  if(fPDF_Muon) fPDF_Muon->Delete(); fPDF_Muon=0;
  if(fPDF_Pion) fPDF_Pion->Delete(); fPDF_Pion=0;
  if(fPDF_Prot) fPDF_Prot->Delete(); fPDF_Prot=0;
  if(fPDF_Elec) fPDF_Elec->Delete(); fPDF_Elec=0;  

  if(fGraph_Muon) fGraph_Muon->Delete(); fGraph_Muon=0;
  if(fGraph_Pion) fGraph_Pion->Delete(); fGraph_Pion=0;
  if(fGraph_Prot) fGraph_Prot->Delete(); fGraph_Prot=0;
  if(fGraph_Elec) fGraph_Elec->Delete(); fGraph_Elec=0;

  if(fSpline_Muon) fSpline_Muon->Delete(); fSpline_Muon=0;
  if(fSpline_Pion) fSpline_Pion->Delete(); fSpline_Pion=0;
  if(fSpline_Prot) fSpline_Prot->Delete(); fSpline_Prot=0;
  if(fSpline_Elec) fSpline_Elec->Delete(); fSpline_Elec=0;

  fIntegral_Muon=0;
  fIntegral_Pion=0;
  fIntegral_Prot=0;
  fIntegral_Elec=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......    

double ND280UpPID::CalcLogLikeRatio(double prob1,double prob2){

  if     (prob1<0.) prob1 = 0.;
  else if(prob1>1.) prob1 = 1.;

  if     (prob2<0.) prob2 = 0.;
  else if(prob2>1.) prob2 = 1.;
  
  double loglike1 = 0.;
  double loglike2 = 0.;
  double ratio = 0.;

  if     (prob1<=0. && prob2>0.){
    ratio = -5.;
    return ratio;
  }  
  else if(prob1>0. && prob2<=0.){
    ratio = +5.;
    return ratio;
  }
  else{
    loglike1 = TMath::Log(prob1);
    loglike2 = TMath::Log(prob2);
    ratio = loglike1 - loglike2;
    return ratio;
  }
  return 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......    

double ND280UpPID::CalcLogLikeRatio(TString hyp1,TString hyp2,double input){
  
  //TSpline3 *spline1 = GetSpline(hyp1);
  //TSpline3 *spline2 = GetSpline(hyp2);
  
  //double prob1=spline1->Eval(input);  // / GetIntegral(hyp1);
  //double prob2=spline2->Eval(input); // / GetIntegral(hyp2);
  
  double prob1 = 0.;
  double prob2 = 0.;

  /*
  TH1F *histo1 = GetPDF(hyp1);
  TH1F *histo2 = GetPDF(hyp2);
  
  int bin1 = histo1->FindBin(input);
  double prob1_histo = histo1->GetBinContent(bin1);
  int bin2 = histo2->FindBin(input);
  double prob2_histo = histo2->GetBinContent(bin2);

  cout << endl;
  std::cout << std::setprecision(7)
	    << hyp1
	    << ", input = " << input << endl
	    << "Histo: prob1 = " << prob1_histo << " - "; //<< " - prob2 = " << prob2 << endl;
  */

  /*
  double prob1_graph = GetGraph(hyp1)->Eval(input);
  double prob2_graph = GetGraph(hyp2)->Eval(input);
  std::cout << std::setprecision(7)
	    << "Graph: prob1 = " << prob1_graph << " - "; //<< " - prob2 = " << prob2 << endl;
  */
  
  double prob1_spline = GetSpline(hyp1)->Eval(input);
  double prob2_spline = GetSpline(hyp2)->Eval(input);

  //std::cout << std::setprecision(7)
  //<< "Spline: prob1 = " << prob1_spline << " - "; //<< " - prob2 = " << prob2 << endl;
  
  prob1 = prob1_spline;
  prob2 = prob2_spline;

  if(prob1>1.){
    if(DEBUG){
      cout << "////////////////////////////" << endl;
      cout << "ND280UpPID::CalcLogLikeRatio" << endl;
      cout << "Hypothesis: " << hyp1 << endl;
      cout << "input: " << input << endl;
      cout << "WARNING: prob1 > 1, force it to be 1" << endl;
      cout << "////////////////////////////" << endl;
    }
    prob1 = 1.;
  }
  else if(prob1<=0.){
    if(DEBUG){
      cout << "////////////////////////////" << endl;
      cout << "ND280UpPID::CalcLogLikeRatio" << endl;
      cout << "Hypothesis: " << hyp1 << endl;
      cout << "input: " << input << endl;
      cout << "WARNING: prob1 < 0, force it to be 0" << endl;
      cout << "////////////////////////////" << endl;
    }
    prob1 = 0.;
  }

  if(prob2>1.){
    if(DEBUG){
      cout << "////////////////////////////" << endl;
      cout << "ND280UpPID::CalcLogLikeRatio" << endl;
      cout << "Hypothesis: " << hyp2 << endl;
      cout << "input: " << input << endl;
      cout << "WARNING: prob2 > 1, force it to be 1" << endl;
      cout << "////////////////////////////" << endl;
    }
    prob2 = 1.;
  }
  else if(prob2<=0.){
    if(DEBUG){
      cout << "////////////////////////////" << endl;
      cout << "ND280UpPID::CalcLogLikeRatio" << endl;
      cout << "Hypothesis: " << hyp2 << endl;
      cout << "input: " << input << endl;
      cout << "WARNING: prob2 < 0, force it to be 0" << endl;
      cout << "////////////////////////////" << endl;
    }
    prob2 = 0.;
  }

  //cout << "prob spline muon = " << prob1 << endl;
  //cout << "prob spline prot = " << prob2 << endl;
  
  double ratio = CalcLogLikeRatio(prob1,prob2);

  //cout << "prob1 = " << prob1 
  //<< " - prob2 = " << prob2 
  //<< " --> loglikeratio = " << ratio 
  //<< endl;

  return ratio;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TSpline3 *ND280UpPID::GetSpline(TString hyp){

  TSpline3 *spline;  
  if     (hyp=="Muon") spline = (TSpline3*)fSpline_Muon;
  else if(hyp=="Pion") spline = (TSpline3*)fSpline_Pion;
  else if(hyp=="Prot") spline = (TSpline3*)fSpline_Prot;
  else if(hyp=="Elec") spline = (TSpline3*)fSpline_Elec;
  else{
    cerr << "ND280UpPID::GetSpline" << endl;
    cerr << "The particle hypothesis " << hyp << " is not available for PID!" << endl;
    exit(1);
  }
  return spline;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

double ND280UpPID::GetIntegral(TString hyp){

  if     (hyp=="Muon") return fIntegral_Muon;
  else if(hyp=="Pion") return fIntegral_Pion;
  else if(hyp=="Prot") return fIntegral_Prot;
  else if(hyp=="Elec") return fIntegral_Elec;
  else{
    cerr << "ND280UpPID::CalcIntegral" << endl;
    cerr << "The particle hypothesis " << hyp << " is not available for PID!" << endl;
    exit(1);
  }
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TGraph *ND280UpPID::GetGraph(TString hyp){

  if     (hyp=="Muon") return fGraph_Muon;
  else if(hyp=="Pion") return fGraph_Pion;
  else if(hyp=="Prot") return fGraph_Prot;
  else if(hyp=="Elec") return fGraph_Elec;
  else{
    cerr << "ND280UpPID::GetGraph" << endl;
    cerr << "The particle hypothesis " << hyp << " is not available for PID!" << endl;
    exit(1);
  }
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TH1F *ND280UpPID::GetPDF(TString hyp){

  if(hyp=="Muon")      return fPDF_Muon;
  else if(hyp=="Pion") return fPDF_Pion;
  else if(hyp=="Prot") return fPDF_Prot;
  else if(hyp=="Elec") return fPDF_Elec;
  else{
    cerr << "ND280UpPID::GetPDF" << endl;
    cerr << "The particle hypothesis " << hyp << " is not available for PID!" << endl;
    exit(1);
  }
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpPID::SetPDF(TString hyp,TH1F *hpdf){

  if(!hpdf){
    cerr << "ND280UpPID::SetPDF" << endl;
    cerr << "The histogram of the PDF for hypothesis " << hyp << " is empty!" << endl;
    exit(1);
  }
  if(hpdf->Integral()<100){
    cerr << "ND280UpPID::SetPDF" << endl;
    cerr << "The histogram of the PDF for hypothesis " << hyp << " has less than 100 entries!" << endl;
    //exit(1);    
  }

  //cumulative distribution is estimated
  double Integral=hpdf->Integral();
  for(int ibinx=1;ibinx<=hpdf->GetNbinsX();ibinx++){
    double IntegralPart=hpdf->Integral(ibinx,hpdf->GetNbinsX());
    if(Integral!=0) IntegralPart /=Integral;
  }
  hpdf->Scale(1./hpdf->Integral());


  if     (hyp=="Muon") SetPDF_Muon(hpdf);
  else if(hyp=="Pion") SetPDF_Pion(hpdf);
  else if(hyp=="Prot") SetPDF_Prot(hpdf);
  else if(hyp=="Elec") SetPDF_Elec(hpdf);
  else{
    cerr << "ND280UpPID::SetPDF" << endl;
    cerr << "The particle hypothesis " << hyp << " is not available for PID!" << endl;
    exit(1);
  }
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

void ND280UpPID::SetPDF_Muon(TH1F *hpdf){
  fPDF_Muon = (TH1F*) hpdf->Clone("fPDF_Muon");
  fGraph_Muon = new TGraph(fPDF_Muon);
  fSpline_Muon = new TSpline3("fSpline_Muon",fGraph_Muon);
  CalcIntegral("Muon");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpPID::SetPDF_Pion(TH1F *hpdf){
  fPDF_Pion = (TH1F*) hpdf->Clone("fPDF_Pion");
  fGraph_Pion = new TGraph(fPDF_Pion);
  fSpline_Pion = new TSpline3("fSpline_Pion",fGraph_Pion);
  CalcIntegral("Pion");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpPID::SetPDF_Prot(TH1F *hpdf){
  fPDF_Prot = (TH1F*) hpdf->Clone("fPDF_Prot");
  fGraph_Prot = new TGraph(fPDF_Prot);
  fSpline_Prot = new TSpline3("fSpline_Prot",fGraph_Prot);
  CalcIntegral("Prot");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpPID::SetPDF_Elec(TH1F *hpdf){
  fPDF_Elec = (TH1F*) hpdf->Clone("fPDF_Elec");
  fGraph_Elec = new TGraph(fPDF_Elec);
  fSpline_Elec = new TSpline3("fSpline_Elec",fGraph_Elec);
  CalcIntegral("Elec");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpPID::CalcIntegral(TString hyp,const int npoints){

  TSpline3 *spline = (TSpline3*) GetSpline(hyp);  
  
  double min = spline->GetXmin();
  double max = spline->GetXmax();
  
  double step = (max - min) / (npoints-1);
  double integral = 0;

  //cout << "min = " << min 
  //<< ", max = " << max 
  //<< ", step = " << step 
  //<< endl << endl;
  
  // Calculate the trapezoidal area
  for(int ipt=0;ipt<(npoints-1);ipt++){

    double x = min + ipt*step; 
    double x_min = x;
    double x_max = x_min + step;
    double val_min = spline->Eval(x_min);
    double val_max = spline->Eval(x_max);    
    double trapezoid = step / 2 * (val_min + val_max); 
    
    if(trapezoid>0){
      integral += trapezoid;
      //cout << "ipt = " << ipt
      //<< ", x_min = " << x_min
      //<< ", x_max = " << x_max
      //<< ", val_min = " << val_min
      //<< ", val_max = " << val_max;
      //cout << ", integral = " << integral << ", ";
      //cout << endl;
    }
  }
  //cout << endl;

  if(hyp=="Muon"){
    fIntegral_Muon = integral;
    //cout << "fIntegral_Muon = " << fIntegral_Muon << endl;
  }
  else if(hyp=="Pion"){
    fIntegral_Pion = integral;
    //cout << "fIntegral_Pion = " << fIntegral_Pion << endl;
  }
  else if(hyp=="Prot"){
    fIntegral_Prot = integral;
    //cout << "fIntegral_Prot = " << fIntegral_Prot << endl;
  }
  else if(hyp=="Elec"){
    fIntegral_Elec = integral;
    //cout << "fIntegral_Elec = " << fIntegral_Elec << endl;
  }
  else{
    cerr << "ND280UpPID::CalcIntegral" << endl;
    cerr << "The particle hypothesis " << hyp << " is not available for PID!" << endl;
    exit(1);
  }

}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......







