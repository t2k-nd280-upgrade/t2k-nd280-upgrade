
#include<iostream>

#include <TMath.h>

#include "ND280UpConst.hh"

#include "ND280UpRecoTrack.hh"
//#include "ND280UpApplyResponse.hh"
//#include "TND280UpEvent.hh"

using namespace std;


ND280UpRecoTrack::ND280UpRecoTrack()
{
  this->Init();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ND280UpRecoTrack::~ND280UpRecoTrack()
{
  this->Delete();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpRecoTrack::Init(){
  f2d_xy = 0;
  f2d_xz = 0;
  f2d_yz = 0;
  fTrkLength = 0;
  fTrkLengthX = 0;
  fTrkLengthY = 0;
  fTrkLengthZ = 0;
  fTrkLengthStraight = 0;
  fTrkLengthStraightX = 0;
  fTrkLengthStraightY = 0;
  fTrkLengthStraightZ = 0;
  fTrkLengthFit = 0;
  fTrkLengthFitX = 0;
  fTrkLengthFitY = 0;
  fTrkLengthFitZ = 0;
  fTrkEdep = 0;
  fUseView_xy = false;
  fUseView_xz = false;
  fUseView_yz = false;
  fNumView = 0;
  fIsOutFV = false;
  fIsReco = true;
  fNHitsXY = 0.;
  fNHitsYZ = 0.;
  fNHitsXZ = 0.;
  fNHits   = 0.;
  fRecoCosTheta = 0.;
  fRecoMom = 0.;
  fDetType = nd280upconv::kUndefined;
  
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpRecoTrack::Delete(){
  f2d_xy->Delete(); f2d_xy = 0;
  f2d_xz->Delete(); f2d_xz = 0;
  f2d_yz->Delete(); f2d_yz = 0;
  fTrkLength = 0;
  fTrkLengthX = 0;
  fTrkLengthY = 0;
  fTrkLengthZ = 0;
  fTrkLengthStraight = 0;
  fTrkLengthStraightX = 0;
  fTrkLengthStraightY = 0;
  fTrkLengthStraightZ = 0;
  fTrkLengthFit = 0;
  fTrkLengthFitX = 0;
  fTrkLengthFitY = 0;
  fTrkLengthFitZ = 0;
  fTrkEdep = 0;
  fUseView_xy = false;
  fUseView_xz = false;
  fUseView_yz = false;
  fNumView = 0;
  fIsOutFV = false;
  fIsReco = true;
  fNHitsXY = 0.;
  fNHitsXY = 0.;
  fNHitsXZ = 0.;
  fNHits   = 0.;
  fRecoCosTheta = 0.;
  fRecoMom = 0.;
  fDetType = nd280upconv::kUndefined;
  
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpRecoTrack::DoTracking(nd280upconv::TargetType_t dettype){

  fDetType = dettype;

  // Here compute the tracking and define if a track is reconstructed

  if(f2d_xy && f2d_xy->Integral()>0.){
    fUseView_xy = true;
    fNumView++;
  }
  if(f2d_xz && f2d_xz->Integral()>0.){
    fUseView_xz = true;
    fNumView++;
  }
  if(f2d_yz && f2d_yz->Integral()>0.){
    fUseView_yz = true;
    fNumView++;
  }
  if(fNumView<2){

    //cerr << endl;
    //cerr << "The number of available views is <2 --> can't do tracking" << endl;
    //cerr << "The only available view is: ";
    //if(fUseView_xy) cerr << "XY" << endl;
    //if(fUseView_xz) cerr << "XZ" << endl;
    //if(fUseView_yz) cerr << "YZ" << endl;
    //cerr << endl;
    
    SetIsReco(false);
    
    return;
  }

  
  CleanMPPCHits(); // Set to 0 MPPC hits with <X pe  
  CalcLength();
  CalcEdep();
  CalcNHits();
  CalcOutFV();  
  CalcRecoVar();

  DefineRecoTrack();
    
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpRecoTrack::CleanMPPCHits(){
  
  // Don't consider MPPC hits with # pe < CutValue
  // Then set it to 0
  
  const double MinPE = 2; // in FGD-official we set to 5 

  for(int iy=0;iy<f2d_xy->GetNbinsY();iy++){
    for(int ix=0;ix<f2d_xy->GetNbinsX();ix++){
      double bincont = f2d_xy->GetBinContent(ix+1,iy+1);
      if(bincont<MinPE) f2d_xy->SetBinContent(ix+1,iy+1,0.);
    }
  }
 
  for(int iy=0;iy<f2d_xz->GetNbinsY();iy++){
    for(int ix=0;ix<f2d_xz->GetNbinsX();ix++){
      double bincont = f2d_xz->GetBinContent(ix+1,iy+1);
      if(bincont<MinPE) f2d_xz->SetBinContent(ix+1,iy+1,0.);
    }
  }

  for(int iy=0;iy<f2d_yz->GetNbinsY();iy++){
    for(int ix=0;ix<f2d_yz->GetNbinsX();ix++){
      double bincont = f2d_yz->GetBinContent(ix+1,iy+1);
      if(bincont<MinPE) f2d_yz->SetBinContent(ix+1,iy+1,0.);
    }
  }

  return;
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpRecoTrack::DefineRecoTrack(){

  SetIsReco(true);
  
  if(fNumView<2){ 
    SetIsReco(false); // At least 2 views
    return;
  }
  
  //cout << "fNHitsXY = " << fNHitsXY << endl;

  // At least 2 views must have 3 hits
  int NHitViewMin;
  int NHitTotMin = 6; // 12 (FGD) // 6 (SuperFGD)

  if(fDetType == nd280upconv::kSuperFGD){
    NHitViewMin = 3;
    NHitTotMin = 6; 
  }
  else if(fDetType == nd280upconv::kFGDlike){
    
    // FGD cut on # of layers
    //   - 40 mm track length is equivalent to 4 layers
    
    double cut_length = 40; // mm
    double length_layer = 0;

    if     ( !fUseView_xy ){
      length_layer = fTrkLengthStraightZ;
    }
    else if( !fUseView_xz ){
      length_layer = fTrkLengthStraightY;
    }
    else if( !fUseView_yz ){
      length_layer = fTrkLengthStraightX;
    }

    if(length_layer < cut_length){ 
      SetIsReco(false);
      return;
    }
    
    // FGD cut on |costheta|<0.3
    //if(TMath::Abs(fRecoCosTheta) < 0.3){
    //SetIsReco(false);
    //return;
    //}

    NHitViewMin = 3;
    NHitTotMin = 6; 
  }
  else{
    cerr << endl;
    cerr << "Target ID " << fDetType << " is not supported" << endl; 
    cerr << endl;
    exit(1);
  }

  // Cut on # of MPPC hits per view
  if     (fNHitsXY >= NHitViewMin && fNHitsXZ >= NHitViewMin) SetIsReco(true);
  else if(fNHitsXY >= NHitViewMin && fNHitsYZ >= NHitViewMin) SetIsReco(true);
  else if(fNHitsXZ >= NHitViewMin && fNHitsYZ >= NHitViewMin) SetIsReco(true);
  else{
    SetIsReco(false);
    return;
  }
  
  // Cut on total # of MPPC hits
  if(fNHits < NHitTotMin){
    SetIsReco(false);
    return;
  }
  
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpRecoTrack::CalcRecoVar(){
  
  // Calculate the reconstructed CosTheta w/ straight track
  // CosTheta = Lenght_Z / Lenght
  fRecoCosTheta = fTrkLengthStraightZ / fTrkLengthStraight;
  
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpRecoTrack::CalcNHits(){
  
  // Calculate the # of hits by looking at the 
  //# of non-empty bins in each projection
      
  fNHitsXY = 0;
  for(int iy=0;iy<f2d_xy->GetNbinsY();iy++){
    for(int ix=0;ix<f2d_xy->GetNbinsX();ix++){
      double bincont = f2d_xy->GetBinContent(ix+1,iy+1);
      if(bincont>0.) fNHitsXY++;
    }
  }

  fNHitsXZ = 0;
  for(int iy=0;iy<f2d_xz->GetNbinsY();iy++){
    for(int ix=0;ix<f2d_xz->GetNbinsX();ix++){
      double bincont = f2d_xz->GetBinContent(ix+1,iy+1);
      if(bincont>0.) fNHitsXZ++;
    }
  }

  fNHitsYZ = 0;
  for(int iy=0;iy<f2d_yz->GetNbinsY();iy++){
    for(int ix=0;ix<f2d_yz->GetNbinsX();ix++){
      double bincont = f2d_yz->GetBinContent(ix+1,iy+1);
      if(bincont>0.) fNHitsYZ++;
    }
  }
  
  fNHits = fNHitsXY + fNHitsXZ + fNHitsYZ;
  
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpRecoTrack::CalcLength(){
  
  CalcLengthStraight();
  CalcLengthFit();

  SetLength( fTrkLengthStraight );
  SetLengthX(fTrkLengthStraightX);
  SetLengthY(fTrkLengthStraightY);
  SetLengthZ(fTrkLengthStraightZ);

  // TODO:
  // - Choose between straigh and fit length

  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpRecoTrack::CalcLengthStraight(){

  //
  // Calculate the track length as the max distance between
  // two points of the tracks (hp: straight track)
  // Follow the conditions:
  // - if 1 hit not possible to track --> length = 0!!!
  // 
  
  //
  // TODO:
  // - add case for 2 views (all other detectors)
  //

  double pos1 = nd280upconv::kBadNum; double pos2 = nd280upconv::kBadNum;
  bool istrkstart = false;
  TH1D *hX;
  double lengthX = 0.;
  double lengthY = 0.;
  double lengthZ = 0.;
  
  // length along X
  if(f2d_xy){
    hX = (TH1D*) f2d_xy->ProjectionX();
    pos1 = nd280upconv::kBadNum; pos2 = nd280upconv::kBadNum;
    istrkstart = false;
    for(int ix=0;ix<hX->GetNbinsX();ix++){
      double cont = hX->GetBinContent(ix+1);
      if(cont>0.){
	if(!istrkstart){ 
	  pos1 = hX->GetBinCenter(ix+1);
	  istrkstart = true;
	}
	pos2 = hX->GetBinCenter(ix+1);
      }
    }
    lengthX = TMath::Abs(pos2-pos1);
  }
  
  // length along Y
  if(f2d_xy){
    hX = (TH1D*) f2d_xy->ProjectionY();
    pos1 = nd280upconv::kBadNum; pos2 = nd280upconv::kBadNum;
    istrkstart = false;
    for(int ix=0;ix<hX->GetNbinsX();ix++){
      double cont = hX->GetBinContent(ix+1);
      if(cont>0.){
	if(!istrkstart){ 
	  pos1 = hX->GetBinCenter(ix+1);
	  istrkstart = true;
	}
	pos2 = hX->GetBinCenter(ix+1);
      }
    }
    lengthY = TMath::Abs(pos2-pos1);
  }
  
  // length along Z
  if(f2d_yz){
    hX = (TH1D*) f2d_yz->ProjectionY();
    pos1 = nd280upconv::kBadNum; pos2 = nd280upconv::kBadNum;
    istrkstart = false;
    for(int ix=0;ix<hX->GetNbinsX();ix++){
      double cont = hX->GetBinContent(ix+1);
      if(cont>0.){
	if(!istrkstart){ 
	  pos1 = hX->GetBinCenter(ix+1);
	  istrkstart = true;
	}
	pos2 = hX->GetBinCenter(ix+1);
      }
    }
    lengthZ = TMath::Abs(pos2-pos1);
  }

  fTrkLengthStraightX = lengthX;
  fTrkLengthStraightY = lengthY;
  fTrkLengthStraightZ = lengthZ;
  
  double lengthtot = TMath::Sqrt(lengthX*lengthX + lengthY*lengthY + lengthZ*lengthZ);
  fTrkLengthStraight = lengthtot;
    
  return;

  // //fRecoTrack_len.push_back(0);
  // //prevX = poshitX; currX = poshitX;
  //	//prevY = poshitY; currY = poshitY;
  //	//prevZ = poshitZ; currZ = poshitZ;	
  //   	//cout << "pex: " << pex << ", pey: " << pey << ", pez: " << pez << endl;
  //  	//fRecoTrack_ene.push_back(pex+pey+pez);
  // ....
  // //
  // // Calculate the track length as the max distance between
  // // two points of the tracks (hp: straight track)
  // //
  // //for(unsigned int ipt=0;ipt<fRecoTrack_hitXY.size();ipt++){	
  // //}     
  // // Take max distance from each 2D MPPC histo projection
  // currX = poshitX;
  // currY = poshitY;
  // currZ = poshitZ;
  // cout << "PrevLen: " << fRecoTrack_len[idx] << endl;
  // double len = TMath::Power(currX-prevX,2) + TMath::Power(currY-prevY,2) + TMath::Power(currZ-prevZ,2); 
  // len = TMath::Sqrt(len);
  // fRecoTrack_len[idx] = TMath::Max(fRecoTrack_len[idx],len);
  // cout << "dX:" << TMath::Abs(currX-prevX) 
  // 	   << ", dY:" << TMath::Abs(currY-prevY) 
  // 	   << ", dZ:" << TMath::Abs(currZ-prevZ)
  // 	   << " --> " << len
  // 	   << " --> NewLen: " << fRecoTrack_len[idx] << endl;
  // prevX = poshitX;
  // prevY = poshitY;
  // prevZ = poshitZ;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpRecoTrack::CalcLengthFit(){

  fTrkLengthFitX = 0;
  fTrkLengthFitY = 0;
  fTrkLengthFitZ = 0;
  fTrkLengthFit = 0;

  return;

  /*
    
  //
  // TODO: Calculate the track length precisely, from the fit
  //
  // - get the first and last point of the track (see CalcLengthStraight() from proj
  // - set fit ranges with init and end pt track in each direction
  // - fit with a pol2 in each view in the reco track range
  // - create a very fine 2d histo for each view and get the length as 
  //   the sum of the cells or fix xz in TF1 and get y length etc... (see below)  
  // - if the fit didn't converge use the straight length (maybe few hits...) 
  //
  
  TFitResultPtr fit_XY = fRecoTrack_MPPCHit_XY[itrk]->Fit("pol2","S"); 
  //double chi2   = fit_XY->Chi2(); // to retrieve the fit chi2
  //Double_t par0   = fit_XY->Parameter(0); // retrieve the value for the parameter 0
  //Double_t err0   = fit_XY->ParError(0); // retrieve the error for the parameter 0
  fit_XY->Print("V");     // print full information of fit including covariance matrix
  //fit_XY->Write();        // store the result in a file
  
  int fitstatus = fit_XY; // OK if 0
  bool IsFitGood = fit_XY->IsValid();
  cout << "Fit status = " << fitstatus << endl;
  if(IsFitGood) cout << "Fit is OK" << endl;
  
  TF1 *ffit_XY = fRecoTrack_MPPCHit_XY[itrk]->GetFunction("pol2");
  
  //fRecoTrack_MPPCHit_XZ[itrk]->Fit("pol2");
  //fRecoTrack_MPPCHit_YZ[itrk]->Fit("pol2");
  
  cout << "/////////////////" << endl;
  cout << endl;
  
  //
  // For lenght calculation of TF1 look at: 
  // https://root-forum.cern.ch/t/getting-the-y-value-from-a-tf2-for-a-given-x-and-z/20468
  // http://root.cern.ch/root/html/TF12.html#TF12:TF12@1
  // TF12(const char* name, TF2* f2, Double_t xy, Option_t* option = "x")
  // TF12 normal constructor.
  // Create a TF12 (special TF1) from a projection of a TF2
  // for a fix value of Y if option="X" or X if option="Y"
  // This value may be changed at any time via TF12::SetXY(xy)
  //
  
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpRecoTrack::CalcEdep(){

  //
  // TODO:
  // - apply the pe->MeV calibration (~ inverse response)
  //   using track length in each direction computed in
  //   CalcLengthFit() / CalcLengthStraight() for the 
  //   light attenuation in fiber
  //
  
  double integral_xy = 0;
  double integral_xz = 0;
  double integral_yz = 0;

  if(f2d_xy) integral_xy = f2d_xy->Integral();
  if(f2d_xz) integral_xz = f2d_xz->Integral();
  if(f2d_yz) integral_yz = f2d_yz->Integral();

  fTrkEdep = integral_xy + integral_yz + integral_xz;
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpRecoTrack::CalcOutFV(){

  // OutFV --> the track has 1 hit in the outer bar/cube

  fIsOutFV = false;
  
  TH1D *hX; 
  TH1D *hY; 
  TH1D *hZ; 

  if     (fUseView_xy) hX = (TH1D*) f2d_xy->ProjectionX();
  else if(fUseView_xz) hX = (TH1D*) f2d_xz->ProjectionX();

  if     (fUseView_xy) hY = (TH1D*) f2d_xy->ProjectionY();
  else if(fUseView_yz) hY = (TH1D*) f2d_yz->ProjectionX();

  if     (fUseView_xz) hZ = (TH1D*) f2d_xz->ProjectionY();
  else if(fUseView_yz) hZ = (TH1D*) f2d_yz->ProjectionY();

  double bincontX_first = hX->GetBinContent(1);
  double bincontX_last  = hX->GetBinContent(hX->GetNbinsX());
  if(bincontX_first>0. || bincontX_last>0.){
    fIsOutFV = true;
  }
  
  double bincontY_first = hY->GetBinContent(1);
  double bincontY_last  = hY->GetBinContent(hY->GetNbinsX());
  if(bincontY_first>0. || bincontY_last>0.){
    fIsOutFV = true;
  }

  double bincontZ_first = hZ->GetBinContent(1);
  double bincontZ_last  = hZ->GetBinContent(hZ->GetNbinsX());
  if(bincontZ_first>0. || bincontZ_last>0.){
    fIsOutFV = true;
  }
  
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......











