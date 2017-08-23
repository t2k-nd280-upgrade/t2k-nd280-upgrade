
#include<iostream>

#include <TMath.h>
#include <TCanvas.h>
#include <TVector3.h>
#include <TVector2.h>

#include "ND280UpConst.hh"
#include "ND280UpRecoTrack.hh"

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

  f2d_xy_other = 0;
  f2d_xz_other = 0;
  f2d_yz_other = 0;

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
  fIsSeparated = true;
  fMinPE = 0;

  for(int i=0;i<2;i++){
    fPt1_XY[i] = nd280upconv::kBadNum; 
    fPt2_XY[i] = nd280upconv::kBadNum; 
    fPt1_XZ[i] = nd280upconv::kBadNum; 
    fPt2_XZ[i] = nd280upconv::kBadNum; 
    fPt1_YZ[i] = nd280upconv::kBadNum; 
    fPt2_YZ[i] = nd280upconv::kBadNum; 
  }

  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpRecoTrack::Delete(){

  // Histograms are only casted and deleted externally
  
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
  fIsSeparated = true;
  fMinPE = 0; 

  for(int i=0;i<2;i++){
    fPt1_XY[i] = nd280upconv::kBadNum; 
    fPt2_XY[i] = nd280upconv::kBadNum; 
    fPt1_XZ[i] = nd280upconv::kBadNum; 
    fPt2_XZ[i] = nd280upconv::kBadNum; 
    fPt1_YZ[i] = nd280upconv::kBadNum; 
    fPt2_YZ[i] = nd280upconv::kBadNum; 
  }
  
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
    SetIsReco(false);    
    return;
  }
  
  //CleanMPPCHits(); // Set to 0 MPPC hits with <X pe  
  CleanMPPCHits(f2d_yz);
  CleanMPPCHits(f2d_xz);
  CleanMPPCHits(f2d_xy);
  
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
  
  double MinPE = fMinPE; // in FGD-official we set to 5 

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

void ND280UpRecoTrack::CleanMPPCHits(TH2F *h2d){
  
  // Don't consider MPPC hits with # pe < CutValue
  // Then set it to 0
  
  double MinPE = fMinPE; // in FGD-official we set to 5 

  for(int iy=0;iy<h2d->GetNbinsY();iy++){
    for(int ix=0;ix<h2d->GetNbinsX();ix++){
      double bincont = h2d->GetBinContent(ix+1,iy+1);
      if(bincont<MinPE) h2d->SetBinContent(ix+1,iy+1,0.);
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
  int NHitViewMin = 3;
  int NHitTotMin = 6; // 12 (FGD) // 6 (SuperFGD)

  if(fDetType == nd280upconv::kSuperFGD){
    NHitViewMin = 3;
    NHitTotMin = 6; 
  }
  else if(fDetType == nd280upconv::kFGDlike){
    
    // FGD cut on # of layers
    //   - 40 mm track length is equivalent to AT LEAST 4 layers (???)
    
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

    double recosintheta = sqrt( 1. - fRecoCosTheta*fRecoCosTheta );

    // FGD cut on direction
    // if no XY view --> |costheta|<0.3    
    if(!fUseView_xy && (TMath::Abs(fRecoCosTheta) < 0.3)){
      SetIsReco(false);
      return;
    }
    // if no XZ or YZ view --> |sintheta|<0.3
    else if( ( !fUseView_xz || !fUseView_yz )
	     && TMath::Abs(recosintheta) < 0.3){

       SetIsReco(false);
      return;
    }
    
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
  // CosTheta = Lenght_Z / Lenght_tot
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

void ND280UpRecoTrack::CalcVtxReco(){
  
  //fVtxReco;
  
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
  // - Choose between straight and fit length

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
  
  double pos1 = nd280upconv::kBadNum; double pos2 = nd280upconv::kBadNum;
  bool istrkstart = false;
  TH1F *hX;
  double lengthX = 0.;
  double lengthY = 0.;
  double lengthZ = 0.;
  
  double len1 = 0.;
  double len2 = 0.;

  // length along X
  len1 = 0.;
  len2 = 0.;
  if(f2d_xy) len1 = CalcLengthStraight1D(f2d_xy,true);
  if(f2d_xz) len2 = CalcLengthStraight1D(f2d_xz,true);
  lengthX = TMath::Max(len1,len2);

  //cout << "X: len1 = " << len1 << ", len2 = " << len2 << ", max = " << lengthX << endl;

  // length along Y
  len1 = 0.;
  len2 = 0.;
  if(f2d_xy) len1 = CalcLengthStraight1D(f2d_xy,false);
  if(f2d_yz) len2 = CalcLengthStraight1D(f2d_yz,true);
  lengthY = TMath::Max(len1,len2);

  //cout << "Y: len1 = " << len1 << ", len2 = " << len2 << ", max = " << lengthY << endl;

  // length along Z
  len1 = 0.;
  len2 = 0.;
  if(f2d_xz) len1 = CalcLengthStraight1D(f2d_xz,false);
  if(f2d_yz) len2 = CalcLengthStraight1D(f2d_yz,false);
  lengthZ = TMath::Max(len1,len2);

  //cout << "Z: len1 = " << len1 << ", len2 = " << len2 << ", max = " << lengthZ << endl;
  //cout << endl;

  fTrkLengthStraightX = lengthX;
  fTrkLengthStraightY = lengthY;
  fTrkLengthStraightZ = lengthZ;
  
  double lengthtot = TMath::Sqrt(lengthX*lengthX + lengthY*lengthY + lengthZ*lengthZ);
  fTrkLengthStraight = lengthtot;
  
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

double ND280UpRecoTrack::CalcLengthStraight1D(TH2F *h2D,bool axisX){
  double length = 0;  
  double pos1 = nd280upconv::kBadNum; double pos2 = nd280upconv::kBadNum;
  bool istrkstart = false;
  TH1F *h1D;
  
  if(axisX) h1D = (TH1F*) h2D->ProjectionX(); // take X projection
  else      h1D = (TH1F*) h2D->ProjectionY(); // take Y projection
  
  pos1 = nd280upconv::kBadNum; pos2 = nd280upconv::kBadNum;
  istrkstart = false;
  for(int ix=0;ix<h1D->GetNbinsX();ix++){
    double cont = h1D->GetBinContent(ix+1);
    if(cont>0.){
      if(!istrkstart){ 
	pos1 = h1D->GetBinCenter(ix+1);
	istrkstart = true;
      }
      pos2 = h1D->GetBinCenter(ix+1);
    }
  }
  length = TMath::Abs(pos2-pos1);
  
  return length;
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
  
  fIsOutFV = CalcOutFVView(f2d_xy);
  if(fIsOutFV) return;

  fIsOutFV = CalcOutFVView(f2d_xz);
  if(fIsOutFV) return;
  
  fIsOutFV = CalcOutFVView(f2d_yz);
  if(fIsOutFV) return;

  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool ND280UpRecoTrack::CalcOutFVTrue(TH2F *h2dmppchit,TGraph *gmchit){
  
  //
  // Take the graph of the MC hits in each view and fill
  // the MPPC hit histogram --> treat same manner as SuperFGD also for FGDlike
  //

  TH2F *myh2d = (TH2F*)h2dmppchit->Clone("myh2d");
 
  for(int ipt=0;ipt<gmchit->GetN();ipt++){
    double x=0;
    double y=0;
    gmchit->GetPoint(ipt,x,y);
    myh2d->Fill(x,y);
  }

  bool TruthOutFV = CalcOutFVView(myh2d);

  delete myh2d;

  return TruthOutFV;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool ND280UpRecoTrack::CalcOutFVView(TH2F *h2D){

  TH1F *hX; 
  TH1F *hY; 

  hX = (TH1F*) h2D->ProjectionX();
  hY = (TH1F*) h2D->ProjectionY();
  
  double bincontX_first = hX->GetBinContent(1);
  double bincontX_last  = hX->GetBinContent(hX->GetNbinsX());
  if(bincontX_first>0. || bincontX_last>0.){
    return true;
  }

  double bincontY_first = hY->GetBinContent(1);
  double bincontY_last  = hY->GetBinContent(hY->GetNbinsX());
  if(bincontY_first>0. || bincontY_last>0.){
    return true;
  }
  
  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpRecoTrack::DoTrackSeparation(){

  //cout << endl;
  //cout << "Begin ND280UpRecoTrack::DoTrackSeparation" << endl;

  // In a view a track is separated if there is an empty
  // MPPC hit between all the other tracks and one-end hit
  
  fIsSeparated = false;

  // N.B. The track must be separated in at least 2 views

  // Find the first / last point of the "current" track in each view

  FindTrackExtremes(f2d_xy, fPt1_XY, fPt2_XY,fUseView_xy);
  FindTrackExtremes(f2d_xz, fPt1_XZ, fPt2_XZ,fUseView_xz);
  FindTrackExtremes(f2d_yz, fPt1_YZ, fPt2_YZ,fUseView_yz);
  
  // cout << "Current track: " << endl;
  // cout << "XY view: " << endl;
  // cout << " - Pt1: " << fPt1_XY[0] << ", " << fPt1_XY[1] << endl;
  // cout << " - Pt2: " << fPt2_XY[0] << ", " << fPt2_XY[1] << endl;
  // cout << "XZ view: " << endl;
  // cout << " - Pt1: " << fPt1_XZ[0] << ", " << fPt1_XZ[1] << endl;
  // cout << " - Pt2: " << fPt2_XZ[0] << ", " << fPt2_XZ[1] << endl;
  // cout << "YZ view: " << endl;
  // cout << " - Pt1: " << fPt1_YZ[0] << ", " << fPt1_YZ[1] << endl;
  // cout << " - Pt2: " << fPt2_YZ[0] << ", " << fPt2_YZ[1] << endl;
  // cout << endl;

  // Find the first / last point of the "other" track in each view

  double Pt1_XY_oth[2]; // initialized inside the function
  double Pt2_XY_oth[2];
  FindTrackExtremes(f2d_xy_other,Pt1_XY_oth,Pt2_XY_oth,fUseView_xy);
  
  double Pt1_XZ_oth[2]; // initialized inside the function
  double Pt2_XZ_oth[2];
  FindTrackExtremes(f2d_xz_other,Pt1_XZ_oth,Pt2_XZ_oth,fUseView_xz);

  double Pt1_YZ_oth[2];
  double Pt2_YZ_oth[2];
  FindTrackExtremes(f2d_yz_other,Pt1_YZ_oth,Pt2_YZ_oth,fUseView_yz);
  
  // cout << "Other track: " << endl;
  // cout << "XY view: " << endl;
  // cout << " - Pt1: " << Pt1_XY_oth[0] << ", " << Pt1_XY_oth[1] << endl;
  // cout << " - Pt2: " << Pt2_XY_oth[0] << ", " << Pt2_XY_oth[1] << endl;
  // cout << "XZ view: " << endl;
  // cout << " - Pt1: " << Pt1_XZ_oth[0] << ", " << Pt1_XZ_oth[1] << endl;
  // cout << " - Pt2: " << Pt2_XZ_oth[0] << ", " << Pt2_XZ_oth[1] << endl;
  // cout << "YZ view: " << endl;
  // cout << " - Pt1: " << Pt1_YZ_oth[0] << ", " << Pt1_YZ_oth[1] << endl;
  // cout << " - Pt2: " << Pt2_YZ_oth[0] << ", " << Pt2_YZ_oth[1] << endl;
  // cout << endl;
  //
  // cout << "Current distance 2D XY = " << CalcPtDistance2D(fPt1_XY,fPt2_XY) << endl;
  // cout << endl;

  // Now find the vertex between two tracks to define the track direction in each view
  // It is found by looking at the closest "current" and "other" points
  // For each view:
  // - pass the two extreme points of each track ("current" and "other")
  // - compute the distance between all the points
  // - the vertex in each track corresponds to the couple of closest points

  double posdif_XY_cur[2] = {0.};
  double posdif_XZ_cur[2] = {0.};
  double posdif_YZ_cur[2] = {0.};
  double posdif_XY_oth[2] = {0.};
  double posdif_XZ_oth[2] = {0.};
  double posdif_YZ_oth[2] = {0.};


  if(fUseView_xy){

    //cout << "XY view" << endl;
    
    GetPositionDiffView(fPt1_XY, fPt2_XY, Pt1_XY_oth, Pt2_XY_oth,posdif_XY_cur,posdif_XY_oth); 

    // cout << "posdif_XY_cur_X = " << posdif_XY_cur[0] << endl;
    // cout << "posdif_XY_cur_Y = " << posdif_XY_cur[1] << endl;
    // cout << "posdif_XY_oth_X = " << posdif_XY_oth[0] << endl;
    // cout << "posdif_XY_oth_Y = " << posdif_XY_oth[1] << endl;

    // TVector3 vec_cur(posdif_XY_cur[0],posdif_XY_cur[1],0.);
    // TVector3 vec_oth(posdif_XY_oth[0],posdif_XY_oth[1],0.);
    // cout << "Angle is " << vec_cur.Angle(vec_oth) / TMath::Pi() * 180. << endl;
  }

  if(fUseView_xz){

    //cout << "XZ view" << endl;
    
    GetPositionDiffView(fPt1_XZ, fPt2_XZ, Pt1_XZ_oth, Pt2_XZ_oth,posdif_XZ_cur,posdif_XZ_oth); 

    // cout << "posdif_XZ_cur_X = " << posdif_XZ_cur[0] << endl;
    // cout << "posdif_XZ_cur_Z = " << posdif_XZ_cur[1] << endl;
    // cout << "posdif_XZ_oth_X = " << posdif_XZ_oth[0] << endl;
    // cout << "posdif_XZ_oth_Z = " << posdif_XZ_oth[1] << endl;

    // TVector3 vec_cur(posdif_XZ_cur[0],posdif_XZ_cur[1],0.);
    // TVector3 vec_oth(posdif_XZ_oth[0],posdif_XZ_oth[1],0.);
    // cout << "Angle is " << vec_cur.Angle(vec_oth) / TMath::Pi() * 180. << endl;
  }


  if(fUseView_yz){

    // cout << "YZ view" << endl;
    
    GetPositionDiffView(fPt1_YZ, fPt2_YZ, Pt1_YZ_oth, Pt2_YZ_oth,posdif_YZ_cur,posdif_YZ_oth); 
    
    // cout << "posdif_YZ_cur_Y = " << posdif_YZ_cur[0] << endl;
    // cout << "posdif_YZ_cur_Z = " << posdif_YZ_cur[1] << endl;
    // cout << "posdif_YZ_oth_Y = " << posdif_YZ_oth[0] << endl;
    // cout << "posdif_YZ_oth_Z = " << posdif_YZ_oth[1] << endl;

    // TVector3 vec_cur(posdif_YZ_cur[0],posdif_YZ_cur[1],0.);
    // TVector3 vec_oth(posdif_YZ_oth[0],posdif_YZ_oth[1],0.);
    // cout << "Angle is " << vec_cur.Angle(vec_oth) / TMath::Pi() * 180. << endl;
  }  


 

  // Now check if in the other tracks there is a MPPC hit 1 
  
  int NViewSeparated = 0;

  //cout << " - Check XY separation" << endl;
  double isSeparXY = false;
  if(fUseView_xy) isSeparXY = GetTrackSeparationInView(posdif_XY_cur,posdif_XY_oth);
  //if(isSeparXY) cout << "  Separated in XY" << endl;
  //else cout << "  NOT separated in XY" << endl;

  //cout << " - Check XZ separation" << endl;
  double isSeparXZ = false;
  if(fUseView_xz) isSeparXZ = GetTrackSeparationInView(posdif_XZ_cur,posdif_XZ_oth);
  //if(isSeparXZ) cout << "  Separated in XZ" << endl;
  //else cout << "  NOT separated in XZ" << endl;

  //cout << " - Check YZ separation" << endl;
  double isSeparYZ = false; 
  if(fUseView_yz) isSeparYZ = GetTrackSeparationInView(posdif_YZ_cur,posdif_YZ_oth);
  //if(isSeparYZ) cout << "  Separated in YZ" << endl;
  //else cout << "  NOT separated in YZ" << endl;
  
  if(isSeparXY) NViewSeparated++;
  if(isSeparXZ) NViewSeparated++;
  if(isSeparYZ) NViewSeparated++;

  // Need the track to be separated in at least 2 views
  if(NViewSeparated>=2) fIsSeparated = true;
  else                  fIsSeparated = false;
  
  //cout << "len_cur = " << len_cur << endl;
  //cout << "len_oth = " << len_oth << endl;
  
  //cout << "End ND280UpRecoTrack::DoTrackSeparation" << endl;
  //cout << endl;

  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

bool ND280UpRecoTrack::GetTrackSeparationInView(double *posdif_cur,double *posdif_oth){

  // 0 --> X, 1 --> Y
  
  //cout << "Begin ND280UpRecoTrack::GetTrackSeparationInView" << endl;

  // The following piece of code is adapted from AnalysisT2HKFullUpgrade.cc (Benjamin)

  double dist_cur = 0;
  double dist_oth = 0;
  
  dist_cur = TMath::Sqrt( posdif_cur[0]*posdif_cur[0] + posdif_cur[1]*posdif_cur[1] );
  dist_oth = TMath::Sqrt( posdif_oth[0]*posdif_oth[0] + posdif_oth[1]*posdif_oth[1] );
    
  if(dist_cur > dist_oth) return true; // always separated from any shorter track
  else{
    
    double scalar_prod = posdif_cur[0]*posdif_oth[0] + posdif_cur[1]*posdif_oth[1];
    double costh = scalar_prod / (dist_cur*dist_oth);
    
    if(std::abs(costh)>1){
      cout << endl;
      cout << "WARNING" << endl;
      cout << "ND280UpRecoTrack::GetTrackSeparationInView" << endl;
      cout << "costh = " << costh << " --> |costh| > 1 !!!" << endl;
      
      if(costh>1)      costh = 1;
      else if(costh<1) costh = -1;

      cout << "Force it to be " << costh << " !!!" << endl;
      cout << endl;
    }

    double distance=std::min(dist_cur,dist_oth)*TMath::Sin(TMath::ACos(costh));

    // cout << "scalar_prod = " << scalar_prod << endl;
    // cout << "dist_cur = " << dist_cur << endl;
    // cout << "dist_oth = " << dist_oth << endl;
    // cout << "costh = " << costh << endl;
    // cout << "angle = " << TMath::ACos(costh) / TMath::Pi() * 180. << endl;
    // cout << "distance = " << distance << endl;
    
    if(distance < fTrackSeparationMin) return false;
    else return true;
  }
  
  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpRecoTrack::FindTrackExtremes(TH2F *h2d,double *extreme,bool SelProjX){

  // 0 --> X, 1 --> Y

  //
  // N.B. I get the same track length as in CalcLengthStraight() --> OK!!!
  //

  //
  // Returns the difference of the extreme point positions along each direction
  //

  double pos1 = nd280upconv::kBadNum; double pos2 = nd280upconv::kBadNum;
  TH1F *hProj;
  bool istrkstart = false;
  
  if(h2d && h2d->Integral()>0.){
    if(SelProjX) hProj = (TH1F*) h2d->ProjectionX(); // X projection
    else         hProj = (TH1F*) h2d->ProjectionY(); // Y projection
    pos1 = nd280upconv::kBadNum; pos2 = nd280upconv::kBadNum;
    istrkstart = false;
    
    for(int i=0;i<hProj->GetNbinsX();i++){
      double cont = hProj->GetBinContent(i+1);
      if(cont>0.){
	if(!istrkstart){ 
	  pos1 = hProj->GetBinCenter(i+1);
	  istrkstart = true;
	}
	pos2 = hProj->GetBinCenter(i+1);
      }
    }
  } 
  
  extreme[0] = pos1;
  extreme[1] = pos2;
  
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpRecoTrack::FindTrackExtremes(TH2F *h2d,double *Pt1,double *Pt2,bool IsViewUsed){

  // 0 --> X, 1 --> Y
  
  //
  // Find the extreme positions in each projection (X and Y independently)
  // then find the combination (among 4 in total) that corresponds to the 
  // extreme points using h2d->FindBin --> avoid looping over all bins!!!
  //

  // Pt1 --> first  track extreme point {X1,Y1}
  // Pt2 --> second track extreme point {X2,Y2}

  Pt1[0] = nd280upconv::kBadNum;
  Pt1[1] = nd280upconv::kBadNum;
  Pt2[0] = nd280upconv::kBadNum;
  Pt2[1] = nd280upconv::kBadNum;
  
  if(!IsViewUsed) return; // if the view is not used values are not defined

  double extremeX[2]; 
  double extremeY[2];  
  extremeX[0] = nd280upconv::kBadNum;
  extremeX[1] = nd280upconv::kBadNum;
  extremeY[0] = nd280upconv::kBadNum;
  extremeY[1] = nd280upconv::kBadNum;
  
  FindTrackExtremes(h2d,extremeX,true);  // extremes of the track in X projection
  FindTrackExtremes(h2d,extremeY,false); // extremes of the track in Y projection
  
  // - it should provide the point (x,y) not just a single coordinate
  // - to avoid looking at all the bins in the th2 (time consuming)
  //   try all the x[0],x[1],y[0],y[1] combinations and check if it  
  //   corresponds to a bin with content > 0 in the th2

  int bin = 0;
  double bincont = 0;
  double x = 0;
  double y = 0;
  bool FirstPtFound = false;
  
  // { x[0],y[0] }
  x = extremeX[0];
  y = extremeY[0];
  bin = h2d->FindBin(x,y); 
  bincont = h2d->GetBinContent(bin);
  if(bincont>0.){
    if(!FirstPtFound){
      Pt1[0] = x; Pt1[1] = y;
      FirstPtFound = true;
    }
    else{
      Pt2[0] = x; Pt2[1] = y;
    }
  }
  
  // { x[0],y[1] }
  x = extremeX[0];
  y = extremeY[1];
  bin = h2d->FindBin(x,y); 
  bincont = h2d->GetBinContent(bin);
  if(bincont>0.){
    if(!FirstPtFound){
      Pt1[0] = x; Pt1[1] = y;
      FirstPtFound = true;
    }
    else{
      Pt2[0] = x; Pt2[1] = y;
    }
  }

  // { x[1],y[0] }
  x = extremeX[1];
  y = extremeY[0];
  bin = h2d->FindBin(x,y); 
  bincont = h2d->GetBinContent(bin);
  if(bincont>0.){
    if(!FirstPtFound){
      Pt1[0] = x; Pt1[1] = y;
      FirstPtFound = true;
    }
    else{
      Pt2[0] = x; Pt2[1] = y;
    }
  }

  // { x[1],y[1] }
  x = extremeX[1];
  y = extremeY[1];
  bin = h2d->FindBin(x,y); 
  bincont = h2d->GetBinContent(bin);
  if(bincont>0.){
    if(!FirstPtFound){
      Pt1[0] = x; Pt1[1] = y;
      FirstPtFound = true;
    }
    else{
      Pt2[0] = x; Pt2[1] = y;
    }
  }
  
  return;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

double ND280UpRecoTrack::CalcPtDistance2D(double *pt1,double *pt2){

  // 0 --> X, 1 --> Y

  double dist = nd280upconv::kBadNum;
  double dX = pt1[0] - pt2[0];
  double dY = pt1[1] - pt2[1];
  dist = sqrt(dX*dX + dY*dY);  
  return dist;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

double ND280UpRecoTrack::CalcPtDistance3D(double *pt1,double *pt2){

  // 0 --> X, 1 --> Y

  double dist = nd280upconv::kBadNum;
  double dX = pt1[0] - pt2[0];
  double dY = pt1[1] - pt2[1];
  double dZ = pt1[2] - pt2[2];
  dist = sqrt(dX*dX + dY*dY + dZ*dZ);
  return dist;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ND280UpRecoTrack::GetPositionDiffView(double *Pt1_cur,double *Pt2_cur,double *Pt1_oth,double *Pt2_oth,double *posdif_cur,double *posdif_oth){
  
  // 0 --> X, 1 --> Y
  
  double Vtx_cur[2];
  Vtx_cur[0] = nd280upconv::kBadNum;
  Vtx_cur[1] = nd280upconv::kBadNum;
  double Vtx_oth[2];
  Vtx_oth[0] = nd280upconv::kBadNum;
  Vtx_oth[1] = nd280upconv::kBadNum;
  
  double dist_min = 9999999999;

  // || Pt1 curr - Pt1_oth ||
  
  double dist_11 = CalcPtDistance2D(Pt1_cur,Pt1_oth); 

  if(dist_11 < dist_min){
    dist_min = dist_11;
    Vtx_cur[0] = Pt1_cur[0];
    Vtx_cur[1] = Pt1_cur[1];
    Vtx_oth[0] = Pt1_oth[0];
    Vtx_oth[1] = Pt1_oth[1];
  }

  // || Pt1 curr - Pt2_oth ||
  
  double dist_12 = CalcPtDistance2D(Pt1_cur,Pt2_oth);

  if(dist_12 < dist_min){
    dist_min = dist_12;
    Vtx_cur[0] = Pt1_cur[0];
    Vtx_cur[1] = Pt1_cur[1];
    Vtx_oth[0] = Pt2_oth[0];
    Vtx_oth[1] = Pt2_oth[1];
  }

  // || Pt2 curr - Pt1_oth ||
  
  double dist_21 = CalcPtDistance2D(Pt2_cur,Pt1_oth);

  if(dist_21 < dist_min){
    dist_min = dist_21;
    Vtx_cur[0] = Pt2_cur[0];
    Vtx_cur[1] = Pt2_cur[1];
    Vtx_oth[0] = Pt1_oth[0];
    Vtx_oth[1] = Pt1_oth[1];
  }

  // || Pt2 curr - Pt2_oth ||
  
  double dist_22 = CalcPtDistance2D(Pt2_cur,Pt2_oth);

  if(dist_22 < dist_min){
    dist_min = dist_22;
    Vtx_cur[0] = Pt2_cur[0];
    Vtx_cur[1] = Pt2_cur[1];
    Vtx_oth[0] = Pt2_oth[0];
    Vtx_oth[1] = Pt2_oth[1];
  }

  // cout << "Track vertex" << endl;
  // cout << " - current: " << Vtx_cur[0] << ", " << Vtx_cur[1] << endl;
  // cout << " - other:   " << Vtx_oth[0] << ", " << Vtx_oth[1] << endl;

  // Calculate the position difference (same as length but with sign)
  // for each projection based on the found view vertex track point
  
  posdif_cur[0] = 0.; 
  posdif_cur[1] = 0.; 
  posdif_oth[0] = 0.; 
  posdif_oth[1] = 0.; 

  if(Pt1_cur[0] != Vtx_cur[0]) posdif_cur[0] = Pt1_cur[0] - Vtx_cur[0]; 
  else                         posdif_cur[0] = Pt2_cur[0] - Vtx_cur[0]; 
  
  if(Pt1_cur[1] != Vtx_cur[1]) posdif_cur[1] = Pt1_cur[1] - Vtx_cur[1]; 
  else                         posdif_cur[1] = Pt2_cur[1] - Vtx_cur[1]; 
  
  if(Pt1_oth[0] != Vtx_oth[0]) posdif_oth[0] = Pt1_oth[0] - Vtx_oth[0]; 
  else                         posdif_oth[0] = Pt2_oth[0] - Vtx_oth[0]; 
  
  if(Pt1_oth[1] != Vtx_oth[1]) posdif_oth[1] = Pt1_oth[1] - Vtx_oth[1]; 
  else                         posdif_oth[1] = Pt2_oth[1] - Vtx_oth[1]; 
  
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......











