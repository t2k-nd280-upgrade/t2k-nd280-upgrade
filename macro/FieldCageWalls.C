#include <TH1D.h>
#include <TH2D.h>
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TMath.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TCanvas.h>
#include <TPaveText.h>

#include "/home/cjesus/Desktop/PhD/T2K/ND280/simulations/t2k-edited/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.hh"
#include "/home/cjesus/Desktop/PhD/T2K/ND280/simulations/t2k-edited/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.hh"
#include "/home/cjesus/Desktop/PhD/T2K/ND280/simulations/t2k-edited/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.hh"
#include "/home/cjesus/Desktop/PhD/T2K/ND280/simulations/t2k-edited/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.hh"

void FieldCageWalls
(
 const int evtfirst = 0,
 const int nevents = 1,
 
 string tag = "",

 string infilename = ""


)
{

   TCanvas *c1 = new TCanvas("c1","c1",1200,600);
   gStyle->SetOptStat(0000);
   gStyle->SetPadLeftMargin(0.15);
   gStyle->SetPadRightMargin(0.11);
   gStyle->SetPadTopMargin(0.08);
   gStyle->SetPadBottomMargin(0.13);
   gStyle->SetTitleX(0.57f);
   gStyle->SetTitleW(0.8f);
   gStyle->SetTextFont(2);
   gStyle->SetLabelFont(22,"xyz");
   gStyle->SetTitleFont(22,"xyz");
   gStyle->SetTitleFont(22);
   gStyle->SetOptTitle(0);

   c1->Divide(4,2);

// Read the TTree in infilename.

  TH1D *hAng = new TH1D("hAng","hAng",200,-75,75);

  TH1D *hAngDevMu = new TH1D("hAngDevMu","hAngDevMu",200,-3,3);
  TH1D *hAngDevEl = new TH1D("hAngDevEl","hAngDevEl",200,-3,3);
  TH1D *hAngDevPi = new TH1D("hAngDevPi","hAngDevPi",200,-3,3);
  TH1D *hAngDevPr = new TH1D("hAngDevPr","hAngDevPr",200,-3,3);

  string FCname = "";

  int ElIni = 0;
  int ElFin = 300;
  int ElY = 50;
  int PixHeight = (ElFin-ElIni)/ElY;

  int MuIni = 0;
  int MuFin = 500;
  int MuY = (MuFin-MuIni)/PixHeight;

  int PiIni = 0;
  int PiFin = 500;
  int PiY = (PiFin-PiIni)/PixHeight;

  int PrIni = 200;
  int PrFin = 500;
  int PrY = (PrFin-PrIni)/PixHeight;


/*
  TH2D *hAngDev2DEl = new TH2D("hAngDev2DEl","hAngDev2DEl",30,-75,75,50,0,250);
  TH2D *hAngDevCntEl = new TH2D("hAngDevCntEl","hAngDevCntEl",30,-75,75,50,0,250);
  TH2D *hAngDev2DMu = new TH2D("hAngDev2DMu","hAngDev2DMu",30,-75,75,50,50,300);
  TH2D *hAngDevCntMu = new TH2D("hAngDevCntMu","hAngDevCntMu",30,-75,75,50,50,300);
  TH2D *hAngDev2DPi = new TH2D("hAngDev2DPi","hAngDev2DPi",30,-75,75,50,50,300);
  TH2D *hAngDevCntPi = new TH2D("hAngDevCntPi","hAngDevCntPi",30,-75,75,50,50,300);
  TH2D *hAngDev2DPr = new TH2D("hAngDev2DPr","hAngDev2DPr",30,-75,75,50,50,300);
  TH2D *hAngDevCntPr = new TH2D("hAngDevCntPr","hAngDevCntPr",30,-75,75,50,50,300);

  TH2D *hMomEl = new TH2D("hMomEl","hMomEl",30,-75,75,50,0,250);
  TH2D *hMomCntEl = new TH2D("hMomCntEl","hMomCntEl",30,-75,75,50,0,250);
  TH2D *hMomMu = new TH2D("hMomMu","hMomMu",30,-75,75,50,50,300);
  TH2D *hMomCntMu = new TH2D("hMomCntMu","hMomCntMu",30,-75,75,50,50,300);
  TH2D *hMomPi = new TH2D("hMomPi","hMomPi",30,-75,75,50,50,300);
  TH2D *hMomCntPi = new TH2D("hMomCntPi","hMomCntPi",30,-75,75,50,50,300);
  TH2D *hMomPr = new TH2D("hMomPr","hMomPr",30,-75,75,50,50,300);
  TH2D *hMomCntPr = new TH2D("hMomCntPr","hMomCntPr",30,-75,75,50,50,300);
*/

  TH2D *hAngDev2DEl = new TH2D("hAngDev2DEl","hAngDev2DEl",30,-75,75,ElY,ElIni,ElFin);
  TH2D *hAngDevCntEl = new TH2D("hAngDevCntEl","hAngDevCntEl",30,-75,75,ElY,ElIni,ElFin);
  TH2D *hAngDev2DMu = new TH2D("hAngDev2DMu","hAngDev2DMu",30,-75,75,MuY,MuIni,MuFin);
  TH2D *hAngDevCntMu = new TH2D("hAngDevCntMu","hAngDevCntMu",30,-75,75,MuY,MuIni,MuFin);
  TH2D *hAngDev2DPi = new TH2D("hAngDev2DPi","hAngDev2DPi",30,-75,75,PiY,PiIni,PiFin);
  TH2D *hAngDevCntPi = new TH2D("hAngDevCntPi","hAngDevCntPi",30,-75,75,PiY,PiIni,PiFin);
  TH2D *hAngDev2DPr = new TH2D("hAngDev2DPr","hAngDev2DPr",30,-75,75,PrY,PrIni,PrFin);
  TH2D *hAngDevCntPr = new TH2D("hAngDevCntPr","hAngDevCntPr",30,-75,75,PrY,PrIni,PrFin);

  TH2D *hMomEl = new TH2D("hMomEl","hMomEl",30,-75,75,ElY,ElIni,ElFin);
  TH2D *hMomCntEl = new TH2D("hMomCntEl","hMomCntEl",30,-75,75,ElY,ElIni,ElFin);
  TH2D *hMomMu = new TH2D("hMomMu","hMomMu",30,-75,75,MuY,MuIni,MuFin);
  TH2D *hMomCntMu = new TH2D("hMomCntMu","hMomCntMu",30,-75,75,MuY,MuIni,MuFin);
  TH2D *hMomPi = new TH2D("hMomPi","hMomPi",30,-75,75,PiY,PiIni,PiFin);
  TH2D *hMomCntPi = new TH2D("hMomCntPi","hMomCntPi",30,-75,75,PiY,PiIni,PiFin);
  TH2D *hMomPr = new TH2D("hMomPr","hMomPr",30,-75,75,PrY,PrIni,PrFin);
  TH2D *hMomCntPr = new TH2D("hMomCntPr","hMomCntPr",30,-75,75,PrY,PrIni,PrFin);

  TFile *finput = new TFile(infilename.c_str(),"READ");
  TTree *tinput = (TTree*) finput->Get("ND280upEvents");
  
  TND280UpEvent *nd280UpEvent = new TND280UpEvent();  
  tinput->SetBranchAddress("Event",&nd280UpEvent);
  
  int NTreeEntries = tinput->GetEntries();  
  int evtlasttree = NTreeEntries-1;
  int Nentries = -999;

// Message summarizing the TTree content.

  cout << endl;
  cout << "# of entries in the Tree: " << NTreeEntries << endl;
  cout << "last tree event ID: " << evtlasttree << endl;
  cout << "first event ID: " << evtfirst << endl;
  cout << endl;
  
// Warning message in case the analysis starts with an event number larger than the number of events in the TTree.
  if(evtfirst > evtlasttree){
    cout << endl;
    cout << "first selection evt ID > first evt ID of tree:" << endl;
    cout << " - # of tree events = " << NTreeEntries << endl;
    cout << " - last tree event = " << evtlasttree << endl;
    cout << " - first event = " << evtfirst << endl;
    cout << "Exit!!!" << endl;
    cout << endl;
    exit(1);
  }
// Warning message in case the analysis will look for an event number larger than the total number of events in the TTree.
  else if(NTreeEntries < (evtfirst+nevents)){
    
    Nentries = NTreeEntries - evtfirst;
    
    cout << endl;
    cout << "WARNING:" << endl;
    cout << "first evt ID + # of events > tot # of tree events:" << endl;
    cout << " - # of tree events = " << NTreeEntries << endl;
    cout << " - # of events to run = " << nevents << endl;
    cout << " - first event = " << evtfirst << endl;
    cout << "Set # of events to run to " << Nentries;
    cout << endl;
  }
// If the analysis makes sense: loop it Nentries' times.
  else{
    Nentries = nevents;    
  }

// Message explaining the subset of events analysed by the macro.

  int EntryLast = evtfirst+Nentries-1;
  
  cout << endl;
  cout << "First event = " << evtfirst << endl;
  cout << "Last event = " << EntryLast << endl;
  cout << "# of events = " << Nentries << endl;
  cout << endl;


//Loop over events.
  for(int ievt=evtfirst;ievt<=EntryLast;ievt++)
{ 
    tinput->GetEntry(ievt); 
    if(!(ievt%1000)){
      cout << "Event " << ievt << endl; // Message for large files: Print the analysis status every 1000 analyzed events.    
    }

//Loop over Tracks (per each events).
    int NTracks = nd280UpEvent->GetNTracks();
    for(int itrk=0;itrk<NTracks;itrk++)
    {

      TND280UpTrack *nd280UpTrack = nd280UpEvent->GetTrack(itrk);
      //nd280UpTrack->PrintTrack();
      
      int trkID = nd280UpTrack->GetTrackID();
      int parentID = nd280UpTrack->GetParentID();
      int pdg = nd280UpTrack->GetPDG();
      double charge = nd280UpTrack->GetCharge();
      double ekin = nd280UpTrack->GetInitKinEnergy();
      double mom = nd280UpTrack->GetInitMom().Mag(); 
    
      double dirX = nd280UpTrack->GetInitMom().X() / mom;
      double dirY = nd280UpTrack->GetInitMom().Y() / mom;
      double dirZ = nd280UpTrack->GetInitMom().Z() / mom;
      double phi = atan2(dirZ,dirY);
      double cosphi = cos(phi);

      hAng->Fill(phi*360/(2*3.141592));
    
//      cout << "pdg is:  " << pdg << endl;

// pdg: Electron (11) // Muon (13) // Pion- (-211) // proton (2212) 

//Loop over points (per each track in each event).
      int NPoints = nd280UpTrack->GetNPoints();
      int cnt =0;
      for(int ipt=0;ipt<NPoints;ipt++)
      {	
        TND280UpTrackPoint *nd280UpTrackPoint = nd280UpTrack->GetPoint(ipt);	
	double length = nd280UpTrackPoint->GetStepLength();
       	double PtX = nd280UpTrackPoint->GetPostPosition().X();
       	double PtY = nd280UpTrackPoint->GetPostPosition().Y();
       	double PtZ = nd280UpTrackPoint->GetPostPosition().Z();
       	double MomX = nd280UpTrackPoint->GetMomentum().X();
       	double MomY = nd280UpTrackPoint->GetMomentum().Y();
       	double MomZ = nd280UpTrackPoint->GetMomentum().Z();       	
	double MomMod = nd280UpTrackPoint->GetMomentum().Mag();
        double MomFin;
        double PosVec[] = {PtX,PtY,PtZ};
        double MomVec[] = {MomX/MomMod,MomY/MomMod,MomZ/MomMod};
        double phi2=atan2(MomVec[2],MomVec[1]);
	string volname = nd280UpTrackPoint->GetLogVolName();

//        cout << "volname is: " << volname << endl;

//        cout << "Volume is: " << volname << endl;
//        cout << "Momentum Vector is: (" << MomX/MomMod <<","<<MomY/MomMod<<","<<MomZ/MomMod<<")" << endl;
//        cout << "Momentum Mod: " << PtMom << endl;
//        cout << "Mom Module: " << MomMod << endl;

        if(volname == "/t2k/OA/Magnet/Basket/HATPCUp/FC/layer8" || volname == "/t2k/OA/Magnet/Basket/HATPCUp/FCNexus/layer8" || volname == "/t2k/OA/Magnet/Basket/HATPCUp/FCGF/layer7" || volname == "/t2k/OA/Magnet/Basket/HATPCUp/FCSolid/layer5")

{

if(volname== "/t2k/OA/Magnet/Basket/HATPCUp/FC/layer8") FCname = "old";
if(volname== "/t2k/OA/Magnet/Basket/HATPCUp/FCNexus/layer8") FCname = "Nexus";
if(volname== "/t2k/OA/Magnet/Basket/HATPCUp/FCGF/layer7") FCname = "GF";
if(volname== "/t2k/OA/Magnet/Basket/HATPCUp/FCSolid/layer5") FCname = "Solid";
        
        MomFin = MomMod;
        if(cnt == 0)
          {

if(pdg ==11)
{
hAngDevCntEl->Fill(phi*360/(2*3.141592),mom,1);
hAngDev2DEl->Fill(phi*360/(2*3.141592),mom,abs((phi2-phi)*360/(2*3.141592))); 
hMomCntEl->Fill(phi*360/(2*3.141592),mom,1);
hMomEl->Fill(phi*360/(2*3.141592),mom,100*MomFin/mom); 
}
if(pdg ==13)
{
hAngDevCntMu->Fill(phi*360/(2*3.141592),mom,1);
hAngDev2DMu->Fill(phi*360/(2*3.141592),mom,abs((phi2-phi)*360/(2*3.141592))); 
hMomCntMu->Fill(phi*360/(2*3.141592),mom,1);
hMomMu->Fill(phi*360/(2*3.141592),mom,100*MomFin/mom); 
}
if(pdg == -211)
{
hAngDevCntPi->Fill(phi*360/(2*3.141592),mom,1);
hAngDev2DPi->Fill(phi*360/(2*3.141592),mom,abs((phi2-phi)*360/(2*3.141592))); 
hMomCntPi->Fill(phi*360/(2*3.141592),mom,1);
hMomPi->Fill(phi*360/(2*3.141592),mom,100*MomFin/mom); 
}
if(pdg ==2212)
{
hAngDevCntPr->Fill(phi*360/(2*3.141592),mom,1);
hAngDev2DPr->Fill(phi*360/(2*3.141592),mom,abs((phi2-phi)*360/(2*3.141592))); 
hMomCntPr->Fill(phi*360/(2*3.141592),mom,1);
hMomPr->Fill(phi*360/(2*3.141592),mom,100*MomFin/mom); 
}

          }      
        cnt++;     
        }

      }
    }
}


double BCEl[31][51];
double BCMu[31][51];
double BCPi[31][51];
double BCPr[31][51];

for(int i = 0; i<31; i++){
for(int j = 0; j<51; j++){
BCEl[i][j]= hMomCntEl->GetBinContent(i,j);
//cout << "ij and BC: " << i << "," << j << "," << BCEl[i][j] << endl;
}
}

for(int i = 0; i<31; i++){
for(int j = 0; j<51; j++){
BCMu[i][j]= hMomCntMu->GetBinContent(i,j);
//cout << "ij and BC: " << i << "," << j << "," << BCMu[i][j] << endl;
}
}

for(int i = 0; i<31; i++){
for(int j = 0; j<51; j++){
BCPi[i][j]= hMomCntPi->GetBinContent(i,j);
//cout << "ij and BC: " << i << "," << j << "," << BCPi[i][j] << endl;
}
}

for(int i = 0; i<31; i++){
for(int j = 0; j<51; j++){
BCPr[i][j]= hMomCntPr->GetBinContent(i,j);
//cout << "ij and BC: " << i << "," << j << "," << BCPr[i][j] << endl;
}
}


int minEntries = 15;

for(int i = 0; i<31; i++){
for(int j = 0; j<51; j++){
if( BCEl[i][j] <=minEntries)
{
hMomEl-> SetBinContent(i,j,0);
hAngDev2DEl-> SetBinContent(i,j,0);
}
}
}


for(int i = 1; i<31; i++){
for(int j = 1; j<51; j++){
if( BCMu[i][j] <=minEntries)
{
hMomMu-> SetBinContent(i,j,0);
hAngDev2DMu-> SetBinContent(i,j,0);
}
}
}

for(int i = 1; i<31; i++){
for(int j = 1; j<51; j++){
if( BCPi[i][j] <=minEntries)
{
hMomPi-> SetBinContent(i,j,0);
hAngDev2DPi-> SetBinContent(i,j,0);
}
}
}

for(int i = 1; i<31; i++){
for(int j = 1; j<51; j++){
if( BCPr[i][j] <=minEntries)
{
hMomPr-> SetBinContent(i,j,0);
hAngDev2DPr-> SetBinContent(i,j,0);
}
}
}


hMomEl->Divide(hMomCntEl);
hAngDev2DEl->Divide(hAngDevCntEl);

hMomMu->Divide(hMomCntMu);
hAngDev2DMu->Divide(hAngDevCntMu);

hMomPi->Divide(hMomCntPi);
hAngDev2DPi->Divide(hAngDevCntPi);

hMomPr->Divide(hMomCntPr);
hAngDev2DPr->Divide(hAngDevCntPr);


c1->cd(1);
  hMomEl->GetZaxis()->SetRangeUser(80, 100);
  hMomEl->GetYaxis()->SetTitle("Initial True Momentum [MeV]");
  hMomEl->GetXaxis()->SetTitle("Initial True Angle [deg]");
  hMomEl->SetTitle("");
  hMomEl->GetXaxis()->CenterTitle(true);
  hMomEl->GetYaxis()->CenterTitle(true);
  hMomEl->GetXaxis()->SetTitleSize(0.050);
  hMomEl->GetYaxis()->SetTitleSize(0.050);
  hMomEl->GetXaxis()->SetLabelSize(0.050);
  hMomEl->GetYaxis()->SetLabelSize(0.050);
  hMomEl->GetXaxis()->SetTitleOffset(1.1);
  hMomEl->Draw("COLZ");
  TPaveText *t1 = new TPaveText(0.32, 0.94, 0.82, 1.0, "bINDC");
  t1->AddText("Electron Final True Momentum [%]");
  t1->SetTextFont(22);
  t1->SetTextSize(0.050);
  t1->SetFillColor(kWhite);
  t1->Draw();
c1->cd(5);
  hAngDev2DEl->GetZaxis()->SetRangeUser(0, 10);
  hAngDev2DEl->GetYaxis()->SetTitle("Initial True Momentum [MeV]");
  hAngDev2DEl->GetXaxis()->SetTitle("Initial True Angle [deg]");
  hAngDev2DEl->SetTitle("");
  hAngDev2DEl->GetXaxis()->CenterTitle(true);
  hAngDev2DEl->GetYaxis()->CenterTitle(true);
  hAngDev2DEl->GetXaxis()->SetTitleSize(0.050);
  hAngDev2DEl->GetYaxis()->SetTitleSize(0.050);
  hAngDev2DEl->GetXaxis()->SetLabelSize(0.050);
  hAngDev2DEl->GetYaxis()->SetLabelSize(0.050);
  hAngDev2DEl->GetXaxis()->SetTitleOffset(1.1);
  hAngDev2DEl->Draw("COLZ");
  TPaveText *t2 = new TPaveText(0.3, 0.94, 0.81, 1.0, "bINDC");
  t2->AddText("Electron Final True Angle [deg]");
  t2->SetTextFont(22);
  t2->SetTextSize(0.050);
  t2->SetFillColor(kWhite);
  t2->Draw();
c1->cd(2);
  hMomMu->GetZaxis()->SetRangeUser(80, 100);
  hMomMu->GetYaxis()->SetTitle("Initial True Momentum [MeV]");
  hMomMu->GetXaxis()->SetTitle("Initial True Angle [deg]");
  hMomMu->SetTitle("");
  hMomMu->GetXaxis()->CenterTitle(true);
  hMomMu->GetYaxis()->CenterTitle(true);
  hMomMu->GetXaxis()->SetTitleSize(0.050);
  hMomMu->GetYaxis()->SetTitleSize(0.050);
  hMomMu->GetXaxis()->SetLabelSize(0.050);
  hMomMu->GetYaxis()->SetLabelSize(0.050);
  hMomMu->GetXaxis()->SetTitleOffset(1.1);
  hMomMu->Draw("COLZ");
  TPaveText *t3 = new TPaveText(0.32, 0.94, 0.82, 1.0, "bINDC");
  t3->AddText("Muon Final True Momentum [%]");
  t3->SetTextFont(22);
  t3->SetTextSize(0.050);
  t3->SetFillColor(kWhite);
  t3->Draw();
c1->cd(6);
  hAngDev2DMu->GetZaxis()->SetRangeUser(0, 5);
  hAngDev2DMu->GetYaxis()->SetTitle("Initial True Momentum [MeV]");
  hAngDev2DMu->GetXaxis()->SetTitle("Initial True Angle [deg]");
  hAngDev2DMu->SetTitle("");
  hAngDev2DMu->GetXaxis()->CenterTitle(true);
  hAngDev2DMu->GetYaxis()->CenterTitle(true);
  hAngDev2DMu->GetXaxis()->SetTitleSize(0.050);
  hAngDev2DMu->GetYaxis()->SetTitleSize(0.050);
  hAngDev2DMu->GetXaxis()->SetLabelSize(0.050);
  hAngDev2DMu->GetYaxis()->SetLabelSize(0.050);
  hAngDev2DMu->GetXaxis()->SetTitleOffset(1.1);
  hAngDev2DMu->Draw("COLZ");
  TPaveText *t4 = new TPaveText(0.3, 0.94, 0.81, 1.0, "bINDC");
  t4->AddText("Muon Final True Angle [deg]");
  t4->SetTextFont(22);
  t4->SetTextSize(0.050);
  t4->SetFillColor(kWhite);
  t4->Draw();
c1->cd(3);
  hMomPi->GetZaxis()->SetRangeUser(80, 100);
  hMomPi->GetYaxis()->SetTitle("Initial True Momentum [MeV]");
  hMomPi->GetXaxis()->SetTitle("Initial True Angle [degree]");
  hMomPi->SetTitle("");
  hMomPi->GetXaxis()->CenterTitle(true);
  hMomPi->GetYaxis()->CenterTitle(true);
  hMomPi->GetXaxis()->SetTitleSize(0.050);
  hMomPi->GetYaxis()->SetTitleSize(0.050);
  hMomPi->GetXaxis()->SetLabelSize(0.050);
  hMomPi->GetYaxis()->SetLabelSize(0.050);
  hMomPi->GetXaxis()->SetTitleOffset(1.1);
  hMomPi->Draw("COLZ");
  TPaveText *t5 = new TPaveText(0.32, 0.94, 0.82, 1.0, "bINDC");
  t5->AddText("Pion Final True Momentum [%]");
  t5->SetTextFont(22);
  t5->SetTextSize(0.050);
  t5->SetFillColor(kWhite);
  t5->Draw();
c1->cd(7);
  hAngDev2DPi->GetZaxis()->SetRangeUser(0, 5);
  hAngDev2DPi->GetYaxis()->SetTitle("Initial True Momentum [MeV]");
  hAngDev2DPi->GetXaxis()->SetTitle("Initial True Angle [deg]");
  hAngDev2DPi->SetTitle("");
  hAngDev2DPi->GetXaxis()->CenterTitle(true);
  hAngDev2DPi->GetYaxis()->CenterTitle(true);
  hAngDev2DPi->GetXaxis()->SetTitleSize(0.050);
  hAngDev2DPi->GetYaxis()->SetTitleSize(0.050);
  hAngDev2DPi->GetXaxis()->SetLabelSize(0.050);
  hAngDev2DPi->GetYaxis()->SetLabelSize(0.050);
  hAngDev2DPi->GetXaxis()->SetTitleOffset(1.1);
  hAngDev2DPi->Draw("COLZ");
  TPaveText *t6 = new TPaveText(0.3, 0.94, 0.81, 1.0, "bINDC");
  t6->AddText("Pion Final True Angle [deg]");
  t6->SetTextFont(22);
  t6->SetTextSize(0.050);
  t6->SetFillColor(kWhite);
  t6->Draw();
c1->cd(4);
  hMomPr->GetZaxis()->SetRangeUser(80, 100);
  hMomPr->GetYaxis()->SetTitle("Initial True Momentum [MeV]");
  hMomPr->GetXaxis()->SetTitle("Initial True Angle [deg]");
  hMomPr->SetTitle("");
  hMomPr->GetXaxis()->CenterTitle(true);
  hMomPr->GetYaxis()->CenterTitle(true);
  hMomPr->GetXaxis()->SetTitleSize(0.050);
  hMomPr->GetYaxis()->SetTitleSize(0.050);
  hMomPr->GetXaxis()->SetLabelSize(0.050);
  hMomPr->GetYaxis()->SetLabelSize(0.050);
  hMomPr->GetXaxis()->SetTitleOffset(1.1);
  hMomPr->Draw("COLZ");
  TPaveText *t7 = new TPaveText(0.32, 0.94, 0.82, 1.0, "bINDC");
  t7->AddText("Proton Final True Momentum [%]");
  t7->SetTextFont(22);
  t7->SetTextSize(0.050);
  t7->SetFillColor(kWhite);
  t7->Draw();

  c1->cd(8);
  hAngDev2DPr->GetZaxis()->SetRangeUser(0, 5);
  hAngDev2DPr->GetYaxis()->SetTitle("Initial True Momentum [MeV]");
  hAngDev2DPr->GetXaxis()->SetTitle("Initial True Angle [deg]");
  hAngDev2DPr->SetTitle("");
  hAngDev2DPr->GetXaxis()->CenterTitle(true);
  hAngDev2DPr->GetYaxis()->CenterTitle(true);
  hAngDev2DPr->GetXaxis()->SetTitleSize(0.050);
  hAngDev2DPr->GetYaxis()->SetTitleSize(0.050);
  hAngDev2DPr->GetXaxis()->SetLabelSize(0.050);
  hAngDev2DPr->GetYaxis()->SetLabelSize(0.050);
  hAngDev2DPr->GetXaxis()->SetTitleOffset(1.1);
  hAngDev2DPr->Draw("COLZ");
  TPaveText *t8 = new TPaveText(0.3, 0.94, 0.81, 1.0, "bINDC");
  t8->AddText("Proton Final True Angle [deg]");
  t8->SetTextFont(22);
  t8->SetTextSize(0.050);
  t8->SetFillColor(kWhite);
  t8->Draw();

/*
c1->cd(1);
  hMomCntEl->Draw("COLZ");
c1->cd(2);
  hMomCntMu->Draw("COLZ");
c1->cd(3);
  hMomCntPi->Draw("COLZ");
c1->cd(4);
  hMomCntPr->Draw("COLZ");
*/

// Save in an output file.

TString outfilename = "";

if(FCname == "Nexus") outfilename = TString::Format("%s_Nexus_Evt%d_NEvt%d.root",tag.c_str(),evtfirst,nevents);
if(FCname == "GF")    outfilename = TString::Format("%s_GF_Evt%d_NEvt%d.root",tag.c_str(),evtfirst,nevents);
if(FCname == "Solid") outfilename = TString::Format("%s_Solid_Evt%d_NEvt%d.root",tag.c_str(),evtfirst,nevents);


  TFile *out = new TFile(outfilename.Data(),"RECREATE");
  
  hAngDev2DMu->Write();
  hAngDev2DEl->Write();
  hAngDev2DPi->Write();
  hAngDev2DPr->Write();

  hMomEl->Write();
  hMomMu->Write();
  hMomPi->Write();
  hMomPr->Write();

  out->Close();

  cout << outfilename << endl;

if(FCname == "old") c1->Print("~/Desktop/FCStudies/Plots/old.pdf");
if(FCname == "Nexus") c1->Print("~/Desktop/FCStudies/Plots/Nexus.pdf");
if(FCname == "GF") c1->Print("~/Desktop/FCStudies/Plots/GF.pdf");
if(FCname == "Solid") c1->Print("~/Desktop/FCStudies/Plots/Solid.pdf");


}




