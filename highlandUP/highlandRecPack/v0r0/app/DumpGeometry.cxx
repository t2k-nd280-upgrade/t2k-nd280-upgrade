
#include <iostream>
#include <stdlib.h>

#include <TROOT.h>

#include <TFile.h>
#include <TTree.h>

#include <stdio.h>
#include <TApplication.h>
#include <memory>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TBox.h>
#include <TGraph.h>
#include <TLine.h>
#include <TText.h>
#include <TMarker.h>
#include <TStyle.h>
#include <TFile.h>


#include <AnaRecPackManager.hxx>
#include <RecPackGeometry.hxx>


TApplication *theApp;

TH2F* YZHisto;
TH2F* XZHisto;
TH2F* XYHisto;

TCanvas *YZ;
TCanvas *XZ;
TCanvas *XY;


std::vector<TBox*> box_XZ;
std::vector<TBox*> box_YZ;
std::vector<TBox*> box_XY;

int _verb = 0;

//**************************************************************  
void DrawDetector() {
//**************************************************************  


  
  YZ = new TCanvas ("YZ","  YZ  ",100,0,600,400);
  XZ = new TCanvas ("XZ","  XZ  ",720,0,600,400);
  XY = new TCanvas ("XY","  XY  ",100,420,400,400);
  
  YZHisto = new TH2F("YZHisto"," YZ Projection ",100,-4100.,4100.,100,-3200.,3200.);
  XZHisto = new TH2F("XZHisto"," XZ Projection ",100,-4100.,4100.,100,-3000.,3000.);
  XYHisto = new TH2F("XYHisto"," XY Projection ",100,-3000.,3000.,100,-3200.,3200.);
  

  const std::vector<dict::Key>&  volumes = ND::gman().GetSetup().volumes().keys(); 
  box_XZ.resize(volumes.size());
  box_XY.resize(volumes.size());
  box_YZ.resize(volumes.size());

  for (unsigned int i=0;i<volumes.size();i++){

    if (volumes[i]=="mother") continue;

    const Volume& vol = ND::gman().GetSetup().volume(volumes[i]);
    const EVector size = vol.size();
    const EVector pos = vol.position();
    TVector3 smin(pos[0]-size[0],pos[1]-size[1],pos[2]-size[2]);
    TVector3 smax(pos[0]+size[0],pos[1]+size[1],pos[2]+size[2]);
    box_XZ[i] = new TBox(smin.Z(),smin.X(), smax.Z(), smax.X());
    box_YZ[i] = new TBox(smin.Z(),smin.Y(), smax.Z(), smax.Y());
    box_XY[i] = new TBox(smin.X(),smin.Y(), smax.X(), smax.Y());    

    int color=1;
    int fcolor=0;
    int fcolor2=0;
    int fstyle=0;
    int fstyle2=0;

    if (volumes[i].find("Tracker") !=std::string::npos ) color=1;
    if (volumes[i].find("Target") !=std::string::npos ) color=2;
    if (volumes[i].find("TPCUP") !=std::string::npos ) color=3;
    if (volumes[i].find("TPCDown") !=std::string::npos ) color=4;
  
    fstyle=0;
    fcolor=color;

    fstyle2=fstyle;
    fcolor2=fcolor;

    /*
    if (volumes[i].find("Active") !=std::string::npos ||
	( volumes[i].find("Drift") !=std::string::npos && volumes[i].find("Cathode") ==std::string::npos)  ){
	fcolor2=18;
	fstyle2=1001;
      }
    */
    box_XZ[i]->SetFillStyle(fstyle2);
    box_YZ[i]->SetFillStyle(fstyle2);
    box_XY[i]->SetFillStyle(fstyle);

    box_XZ[i]->SetFillColor(fcolor2);
    box_YZ[i]->SetFillColor(fcolor2);
    box_XY[i]->SetFillColor(fcolor);

    box_XZ[i]->SetLineColor(color);
    box_YZ[i]->SetLineColor(color);
    box_XY[i]->SetLineColor(color);

    box_XZ[i]->SetLineStyle(1);
    box_YZ[i]->SetLineStyle(1);
    box_XY[i]->SetLineStyle(1);


  }


  YZ->cd();
  YZHisto->Draw();
  for (unsigned int i=0;i<volumes.size();i++){
    if (volumes[i].find("Side") != std::string::npos ) continue;
    if (box_YZ[i]) box_YZ[i]->Draw("same");
  }

  YZ->Update();

  XZ->cd();
  XZHisto->Draw();
  for (unsigned int i=0;i<volumes.size();i++){
    if (volumes[i].find("Top") !=std::string::npos ) continue;
    if (volumes[i].find("Bottom") !=std::string::npos ) continue;
    if (box_XZ[i]) box_XZ[i]->Draw("same");
  }
  XZ->Update();

  XY->cd();
  XYHisto->Draw();
  for (unsigned int i=0;i<volumes.size();i++)
    if (box_XY[i]) box_XY[i]->Draw("same");

  XY->Update();

  return;


  
}



//**************************************************
int main(int argc, char **argv) {
//**************************************************
 static char optstring[] = "v:";
 int c;
 while ((c=getopt(argc, argv, optstring)) != -1){
  switch(c){
    case 'v':
      {
       int verb = atoi(optarg);
       if (verb<0) verb = 0;
       if (verb>3) verb = 3;
       _verb = verb;
       break;
      }
    case '?' :
      {
        std::cout <<  "Usage:"  <<  std::endl;
        std::cout <<  "DumpGeometry.exe <options>" <<  std::endl;
        std::cout <<  "where options are \n"
                  <<" - v <verbosity (0-3) \n";
        exit(1);
      }
  }
 } 
  
  std::cout << RecPackGeometry::Get(_verb).GetSetup() << std::endl;
  std::cout<<"DumpVolumeProperties"<<std::endl;
  RecPackGeometry::Get().DumpVolumeProperties();
double prange=0;
TVector3 point_start(-827.343, 249, -1737);
TVector3 point_end(-878.903, 378.169, -2033.4);

//ND::tman().CorrectEnergyLossLinear( point_start,point_end, prange, ParticleId::kMuon, AnaRecPackManager::kBwd);
  int argc2 = 0;
  char **argv2 = NULL;
    
  theApp = new TApplication("App",&argc2,argv2);
    
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

//  DrawDetector();


  theApp->Run(kTRUE);
  
}
