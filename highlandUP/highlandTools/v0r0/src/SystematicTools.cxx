#include "SystematicTools.hxx"
#include "WeightTools.hxx"
#include <TGraphAsymmErrors.h>
#include <math.h>
#include <TMath.h>

#include <sstream>


//********************************************************************
SystematicsTools::SystematicsTools(){
  //********************************************************************
  errdebug=false;

  _softwareVersion="";
}

//*********************************************************
void SystematicsTools::FillSystematicHistos(TTree* tree, const std::string& var, int nx, double* xbins, const std::string& cut, int NTOYS, const std::string& uopt, 
    TH2F* h2, TH2F* h1w){
  //*********************************************************
  if(errdebug)    std::cout<<"FillSystematicHistos \n============================= "<<std::endl;

  /* 
     This function fills all necessary histos to compute the systematics:
     - h2:   toy_index vs var
     - h1w:  toy_index vs var, weighted by weight_syst
     */

  (void)nx;
  (void)xbins;
  (void)NTOYS;
  (void)uopt;

  std::string cut2=cut;
  if (cut2=="") cut2="1==1";

  // This is the weighted number of events for each toy experiment t and each bin i: N_i^t
  // In the microtree: t=toy_index, and i is given by the histogram binning
  tree->Project("h2",("toy_index:"+var).c_str(),("("+cut2+")").c_str());

  /* TODO. To review. In principle we should just compare the average of the toys with each of the toys (with the weights applied in both cases)
            The cut cames already with the weights applied
  if (uopt.find("SSYS")==std::string::npos && uopt.find("SYS")!=std::string::npos){
    // Project the number of events weighted by systematic weights
    cut2 = weightTools::ApplyWeights(tree,cut2,uopt);    
    tree->Project("h1w",("toy_index:"+var).c_str(),("("+cut2+")").c_str());
  }
  else{
  */
    // just copy h2 into h1w. TODO: since now h2 and h1w are the same thing we could probably get rid of one of them
    for (int i=0;i<nx;i++)
      for (int j=0;j<NTOYS;j++)
        h1w->SetBinContent(i+1,j+1,h2->GetBinContent(i+1,j+1));
    //  }
}

//*********************************************************
void SystematicsTools::UpdateSystematicCov(HistoStack* hs, TTree* tree, const std::string& var, int nx, double* xbins, const std::string& cut, int NTOYS, const std::string& uopt){
  //*********************************************************

  /* 
     This method allows the use of several trees (in general using Experiment) for computing the systematic covariance. The results for each tree are updated inside a HistoStack. 
     */

  // Create 2Ds histo with the specified variable in the x axis and the toy experiment index in the y axis.
  TH2F h2("h2","h2",nx,xbins,NTOYS,0,NTOYS);
  TH2F h1w("h1w","h1w",nx,xbins,NTOYS,0,NTOYS);

  // Unused histos
  TH2F h1("h1","h1",nx,xbins,NTOYS,0,NTOYS);

  // Fill the histograms that are necessary to compute the systematics
  FillSystematicHistos(tree,var,nx,xbins,cut,NTOYS,uopt,&h2,&h1w);

  // Add them to the HistoStack
  hs->AddSystHistos(&h1,&h2,&h1w);
}

//*********************************************************
const TMatrixD& SystematicsTools::GetSystematicCovBase(HistoStack* hs1, HistoStack* hs2, const std::string& uopt, const std::string& group){
  //*********************************************************

  // 2D histos 
  TH2F* h2, *h1w;

  // Compute the covariance matrix

  if (group!=""){
    GetSystematicHistos(group,hs1, hs2,h2,h1w);
    return GetSystematicCovBase(*h2,*h1w,uopt);
  }

  std::vector<std::string> syst_groups = hs2->GetSystHistosGroups();
  std::vector<std::string>::iterator it;
  for (it=syst_groups.begin();it!=syst_groups.end();it++){
    GetSystematicHistos(*it,hs1, hs2,h2,h1w);
    const TMatrixD& C = GetSystematicCovBase(*h2,*h1w,uopt);
    if (it==syst_groups.begin()){
      int nx = h2->GetNbinsX();
      _covTotal.ResizeTo(nx, nx);
      _covTotal = C;
    }
    else
      _covTotal = _covTotal + C;
  }

  return _covTotal;
}

//*********************************************************
void SystematicsTools::GetSystematicHistos(const std::string& group, HistoStack* hs1, HistoStack* hs2, TH2F*& h2, TH2F*& h1w){
  //*********************************************************

  // Unused histos
  TH2F* h1;
  // 2D histos
  TH2F* h2_1, *h1w_1;
  TH2F* h2_2, *h1w_2;

  if (!hs1){
    hs2->GetSystHistos(group,h1,h2,h1w);  
    return;
  }

  // Get the systematic histos from the stack
  hs1->GetSystHistos(group,h1,h2_1,h1w_1);
  hs2->GetSystHistos(group,h1,h2_2,h1w_2);

  TH2F h2_1p(*h2_1);
  TH2F h1w_1p(*h1w_1);

  if (h2_2->GetNbinsY() > h2_1->GetNbinsY()){
    h2_1p = TH2F(*h2_2);
    h1w_1p = TH2F(*h1w_2);
    for (Int_t i=0;i<h2_2->GetNbinsX();i++){
      for (Int_t j=0;j<h2_2->GetNbinsY();j++){
        h2_1p.SetBinContent(i+1,j+1,h2_1->GetBinContent(i+1,1));
        h1w_1p.SetBinContent(i+1,j+1,h1w_1->GetBinContent(i+1,1));
      }
    }
  }


  // compute the ratio  
  h2 = new TH2F(*h2_2);
  h2->Divide(&h2_1p, h2_2);

  h1w = new TH2F(*h1w_2);
  h1w->Divide(&h1w_1p, h1w_2);

}

//*********************************************************
const TMatrixD& SystematicsTools::GetSystematicCov(HistoStack* hs, const std::string& uopt, const std::string& group){
  //*********************************************************
  if(errdebug)    std::cout<<"GetSystematicCov \n============================= "<<std::endl;


  if (uopt.find("DUMMY")!=std::string::npos){
    _cov.ResizeTo(1, 1);
    return _cov;
  }

  /* 
     Compute the covariance matrix from the histos stored in the HistoStack
     */

  // Compute the covariance matrix
  return GetSystematicCovBase(NULL,hs,uopt,group);
}

//*********************************************************
const TMatrixD& SystematicsTools::GetRatioSystematicCov(HistoStack* hs1, HistoStack* hs2, const std::string& uopt, const std::string& group){
  //*********************************************************
  if(errdebug)    std::cout<<"GetSystematicCov \n============================= "<<std::endl;

  /* 
     Compute the covariance matrix from the histos stored in the HistoStack
     */

  // Compute the covariance matrix
  return GetSystematicCovBase(hs1,hs2,uopt+" RATIO",group);


  // 2D histos 
  TH2F* h1, * h2_1, *h1w_1;

  TH2F* h2_2, *h1w_2;

  // Get the systematic histos from the stack
  hs1->GetSystHistos("default",h1,h2_1,h1w_1);
  hs2->GetSystHistos("default",h1,h2_2,h1w_2);

  TH2F h2_1p(*h2_1);
  TH2F h1w_1p(*h1w_1);

  if (h2_2->GetNbinsY() > h2_1->GetNbinsY()){
    h2_1p = TH2F(*h2_2);
    h1w_1p = TH2F(*h1w_2);
    for (Int_t i=0;i<h2_2->GetNbinsX();i++){
      for (Int_t j=0;j<h2_2->GetNbinsY();j++){
        h2_1p.SetBinContent(i+1,j+1,h2_1->GetBinContent(i+1,1));
        h1w_1p.SetBinContent(i+1,j+1,h1w_1->GetBinContent(i+1,1));
      }
    }
  }


  // compute the ratio
  TH2F h2_ratio(*h2_2);
  h2_ratio.Divide(&h2_1p, h2_2);

  TH2F h1w_ratio(*h1w_2);
  h1w_ratio.Divide(&h1w_1p, h1w_2);

  // Compute the covariance matrix
  return GetSystematicCovBase(h2_ratio,h1w_ratio,uopt+" RATIO");

}

//*********************************************************
const TMatrixD& SystematicsTools::GetSystematicCov(TTree* tree, const std::string& var, int nx, double* xbins, const std::string& cut, int NTOYS, const std::string& uopt){
  //*********************************************************
  if(errdebug)    std::cout<<"GetSystematicCov \n============================= "<<std::endl;

  /* 
     Compute the covariance matrix for a single tree
TODO: Use the method for HistoStack
*/


  // If the covariance has bin already computed for the same binning and cut just return it
  if (CheckSystComputed(tree,NULL,var,nx,xbins,cut,"",1,NTOYS,uopt)){
    return _cov;
  }

  // Create 2Ds histo with the specified variable in the x axis and the toy experiment index in the y axis.
  TH2F h2("h2","h2",nx,xbins,NTOYS,0,NTOYS);
  TH2F h1w("h1w","h1w",nx,xbins,NTOYS,0,NTOYS);

  // Unused histos
  TH2F h1("h1","h1",nx,xbins,NTOYS,0,NTOYS);

  // Fill the histograms that are necessary to compute the systematics
  FillSystematicHistos(tree,var,nx,xbins,cut,NTOYS,uopt,&h2,&h1w);

  // Compute the covariance matrix
  return GetSystematicCovBase(h2,h1w, uopt);
}

//*********************************************************
const TMatrixD& SystematicsTools::GetSystematicCovBase(TH2F& h2,TH2F& h1w,const std::string& uopt){
  //*********************************************************
  //     Compute the covariance matrix from the 1D and 2D histograms 
  /*
     Computes the standard (or variation) systematic covariance matrix for both the method with random throws and the method with binned PDF.
     In fact the only difference between both is the number of toys (larger for throws) and the fact that for throws the weight is the same for all toys 
     while it is different for binned PDF    
     */

  int nx    = h2.GetNbinsX();
  int NTOYS = h2.GetNbinsY();
  //  int toy_ref=0;  //TODO

  std::vector< std::vector<double> > na;
  std::vector< std::vector<double> > wa;
  std::vector<double> avg;

  std::vector<double> wpdf;
  std::vector<double> na_tot;
  std::vector<double> norm;
  double avg_tot=0;

  na.resize(nx);
  wa.resize(nx);
  avg.resize(nx);

  na_tot.resize(NTOYS);
  norm.resize(NTOYS);
  wpdf.resize(NTOYS);

  for (int itoy=0;itoy<NTOYS;itoy++){      
    na_tot[itoy]=0;
    norm[itoy]=1.;
    wpdf[itoy]=1./NTOYS;
  }

  for (int i=0;i<nx;i++){
    avg[i]=0;
    na[i].resize(NTOYS);
    wa[i].resize(NTOYS);

    for (int itoy=0;itoy<NTOYS;itoy++){      
      // The reference toy must be skipped for new trees since there is an extra toy for the reference toy
      // if (itoy == toy_ref && _softwareVersion!="") continue;
      // this is the number of events with systematic applied in each bin and toy experiment (N_i^t)_w
      na[i][itoy]=h1w.GetBinContent(i+1,itoy+1);
      // This is the average number of events (with no systematic weights): N_avg_i = sum_t N_i^t*w^t
      // for weight systematics, for each toy h2.GetBinContent is the same
      // for variation system. it is different.
      avg[i]+=h2.GetBinContent(i+1,itoy+1)*wpdf[itoy];     
      //for variation systematic wa=1
      if (h2.GetBinContent(i+1,itoy+1)>0)
        wa[i][itoy]=na[i][itoy]/h2.GetBinContent(i+1,itoy+1);
      else
        wa[i][itoy]=0;

      // Total number of events for this toy experiment
      if (IsValidWeight(wa[i][itoy]))
        na_tot[itoy]+=na[i][itoy];
    }
    // Average of the total number of events
    avg_tot +=avg[i]; 
  }


  // compute normalization factor for shapeonly systematics
  if (uopt.find("SHAPEONLY")!=std::string::npos){    
    for (int itoy=0;itoy<NTOYS;itoy++){      	
      if (na_tot[itoy]!=0)
        norm[itoy] = avg_tot/na_tot[itoy];
      else
        norm[itoy] = 0.;
    }
  }

  // Give the appropriate dimensions to the output cov matrix
  _cov.ResizeTo(nx, nx);
  for (int i=0;i<nx;i++){
    for (int j=0;j<nx;j++){
      _cov(i,j)=0;      
      for (int itoy=0;itoy<NTOYS;itoy++){	  
        // The reference toy must be skipped for new trees since there is an extra toy for the reference toy
        // if (itoy == toy_ref && _softwareVersion!="") continue;
        if (!IsValidWeight(wa[i][itoy])) continue;
        if (!IsValidWeight(wa[j][itoy])) continue;
        if (na[i][itoy]< 0 || na[j][itoy]<0) continue;
        _cov(i,j) += (norm[itoy]*na[i][itoy]- avg[i])*(norm[itoy]*na[j][itoy]- avg[j])*wpdf[itoy];
        //	std::cout<<itoy<<" wpdf "<<wpdf[itoy]<<std::endl;
        //_cov(i,j) += (na[i][itoy]- avg[i])*(na[j][itoy]- avg[j])*wpdf[itoy];

      }

      // compute relative errors when requested
      if (uopt.find("RELATIVE")!=std::string::npos){
        if (avg[i]*avg[j]!=0 && ((avg[i]>1 && avg[j]>1) || uopt.find("RATIO")!=std::string::npos)){
          _cov(i,j) /= (avg[i]*avg[j]);	 
        }
        else
          _cov(i,j) = 0;
      }	
    }
  }

  return _cov;
}

//*********************************************************
bool SystematicsTools::CheckSystComputed(TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins, const std::string& cut1, const std::string& cut2, double norm, int NTOYS, const std::string& uopt){
  //*********************************************************

  /* 
     Check whether systematics were already computed for this particular conditions
     */


  if (_syst_computed && _tree1_syst==tree1 && _tree2_syst==tree2 && _var_syst==var && _nbins_syst==nx && _cut_syst1==cut1 && _cut_syst2==cut2 && _norm_syst==norm && _NTOYS_syst==NTOYS && _uopt_syst == uopt){

    bool diff=false;
    for (int i=0;i<=nx;i++){
      if (xbins[i] != _xbins_syst[i]) diff=true;
    }
    if (!diff)
      return true;
  }

  _tree1_syst = tree1;
  _tree2_syst = tree2;
  _var_syst=var;
  _nbins_syst=nx;
  _cut_syst1=cut1;
  _cut_syst2=cut2;
  _norm_syst = norm;
  _NTOYS_syst = NTOYS;
  _uopt_syst=uopt;
  _syst_computed=true;

  for (int i=0;i<=nx;i++){
    _xbins_syst[i] = xbins[i];
  }

  return false;
}



//*********************************************************
bool SystematicsTools::TreeHasVar(TTree* tree, const std::string& var){
  //*********************************************************

  if (tree->FindLeaf(var.c_str()))
    return true;
  else
    return false;
}


//*********************************************************
int SystematicsTools::GetVarFromTree(TTree* tree, const std::string& var){
  //*********************************************************

  if (!TreeHasVar(tree,var)) return 0;

  TH1F h0("v","v",1,0,1);
  tree->Project("v","0.5",("(1==1)*"+var).c_str(),"",1);
  return (int)(h0.GetBinContent(1));
}


//*********************************************************
std::string SystematicsTools::GetString(int code){
  //*********************************************************

  std::stringstream scode;
  scode << code;

  return scode.str();
}

//*********************************************************
bool SystematicsTools::IsValidWeight(Float_t weight){
  //*********************************************************


  if (!TMath::Finite(weight)) return false;
  if (weight!=weight) return false;
  if (weight<0 || weight>10) return false;

  return true;
}
