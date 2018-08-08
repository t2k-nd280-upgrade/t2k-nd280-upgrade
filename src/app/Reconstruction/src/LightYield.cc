#include <TFile.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TH3F.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TTree.h>
#include <TVector3.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TMath.h>
#include <TStopwatch.h>
#include <TPolyMarker3D.h>
#include <TMarker.h>

#include <iostream>
#include <fstream>

#include <TRandom3.h>

#include "ND280UpConst.hh"
#include "ND280UpApplyResponse.hh"
#include "ND280UpRecoTrack.hh"
#include "ND280UpPID.hh"

#include "TND280UpEvent.hh"
#include "TND280UpVertex.hh"
#include "TND280UpRecoEvent.hh"
#include "TND280UpRecoTrack.hh"

#include "LightYield.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int LightYield(int argc,char** argv)
{   
  
  if (argc!=14){   // batch mode                                        
    cout << "You need to provide the following arguments:" << endl;
    cout << " 1) input ROOT file name (from GEANT4 simulation) " << endl;
    cout << " 2) first event number to run" << endl;
    cout << " 3) total number of events to run" << endl;
    cout << " 4) the tag for the output file name" << endl;
    cout << " 5) detector ID: 0-->SuperFGD, 1-->FGD-like, 2-->WAGASCI" << endl;
    cout << " 6) set debug option" << endl;
    cout << " 7) set debug plot option" << endl;
    cout << " 8) use view XY (0 or 1)" << endl;
    cout << " 9) use view XZ (0 or 1)" << endl;
    cout << " 10) use view YZ (0 or 1)" << endl;
    cout << " 11) Minimum track distance (if <=0 use default)" << endl;
    cout << " 12) Look only at tracks inside the Truth FV (use MC hits)" << endl;
    cout << " 13) Output file name. In iteractive run it can be first event, but for parallel need separate param" << endl;
    exit(1);
  }

  std::string prefix = "_ly";
  
  string rootfilename = argv[1];
  const int evtfirst = atoi(argv[2]);
  const int nevents = atoi(argv[3]);
  string tag = argv[4];
  tag += prefix;
  const int detectorID = atoi(argv[5]);
  const int DEBUG = atoi(argv[6]);
  const int UseViewXY = atoi(argv[8]);
  const int UseViewXZ = atoi(argv[9]);
  const int UseViewYZ = atoi(argv[10]);


  // Set the inputs

  nd280upconv::TargetType_t DetType;
  if     (detectorID == 0) DetType = nd280upconv::kSuperFGD;
  else if(detectorID == 1) DetType = nd280upconv::kFGDlike;
  
  const int NEvtDisplTot = 50; //atoi(argv[5]); // max # of evt. displays

  std::cout << "Run for prototype " << PROTO << std::endl;
  std::cout << "0 - Full size SuperFGD 1 - 5x5x5 2 - 24x8x48" << std::endl;
#ifdef ELECSIM
  std::cout << "ELECSIM is on" << std::endl;
#else
  std::cout << "ELECSIM is off. Work with effective reflection" << std::endl;
#endif
  std::cout << "Crosstalk algorithm is " << CROSSTALK << std::endl;

  //
  // Declare histograms
  //

  // Event displays

  TH2F *hMPPCHits_XY[NEvtDisplTot]; 
  TH2F *hMPPCHits_XZ[NEvtDisplTot]; 
  TH2F *hMPPCHits_YZ[NEvtDisplTot];

  TH1F *hPEVsTime_x[NEvtDisplTot];
  TH1F *hPEVsTime_y[NEvtDisplTot];
  TH1F *hPEVsTime_z[NEvtDisplTot];

  // L.Y. histoes
  TRandom3* fRndm = new TRandom3(0);

  Int_t   ly_nbins  = 300;
  Float_t ly_first  = 0.;
  Float_t ly_last   = 300.;

  Int_t   ly_nbins_short  = 100;
  Float_t ly_first_short  = 0.;
  Float_t ly_last_short   = 100.;

  Int_t   ly_nbins_ext1  = 1000;
  Float_t ly_first_ext1  = 0.;
  Float_t ly_last_ext1   = 1000.;

  Int_t   ly_nbins_ext2  = 5000;
  Float_t ly_first_ext2  = 0.;
  Float_t ly_last_ext2   = 5000.;

  TH1F* LY_ch[1728];
#if PROTO == 1
    for (Int_t i = 0; i < 12; ++i)
      LY_ch[i]    = new TH1F(Form("LY_ch%i", i),  "", ly_nbins_short,ly_first_short, ly_last_short);
#elif PROTO == 2
    for (Int_t i = 0; i < 192; ++i)
      LY_ch[i] = new TH1F(Form("LY_ch_XY_%i_%i", int(i/8+1), int(i%8+1)),  "", ly_nbins_ext2,ly_first_ext2, ly_last_ext2);

    for (Int_t i = 192; i < 1344; ++i)
      LY_ch[i] = new TH1F(Form("LY_ch_XZ_%i_%i", int((i-192)%8+1), int((i-192)/8+1)),  "", ly_nbins_ext1,ly_first_ext1, ly_last_ext1);

    for (Int_t i = 1344; i < 1728; ++i)
      LY_ch[i] = new TH1F(Form("LY_ch_YZ_%i_%i", int((i-1344)%24+1), int((i-1344)/24+1)),  "", ly_nbins_ext1,ly_first_ext1, ly_last_ext1);
#endif


  // Set outputs
  
  TString outfilename = TString::Format("%s_Evt%d_NEvt%d.root",tag.c_str(),evtfirst,nevents);
  TFile *fileout = new TFile(outfilename.Data(),"RECREATE"); 

  /////////////////////////////
  //                         //
  // Apply detector response //
  //                         //
  /////////////////////////////

  // Take inputs
  TFile *finput = new TFile(rootfilename.c_str(),"READ");
  cout << "Reading the input ROOT file: " << rootfilename << endl;

  TH2F* h2d_xy; TH2F* h2d_xz; TH2F* h2d_yz;
  h2d_xy = (TH2F*)finput->Get("OutMPPCProj2D_XY");
  h2d_xz = (TH2F*)finput->Get("OutMPPCProj2D_XZ");
  h2d_yz = (TH2F*)finput->Get("OutMPPCProj2D_YZ");

  // Take the World origin position in the target (local) reference system
  TPolyMarker3D *WorldOrigInLocal = (TPolyMarker3D*)finput->Get("WorldOrigInLocal");
  double OrigInLocX=0.;
  double OrigInLocY=0.;
  double OrigInLocZ=0.;
  WorldOrigInLocal->GetPoint(0,OrigInLocX,OrigInLocY,OrigInLocZ);
  
  cout << "The World origin in local coordinates is:" << endl; 
  cout << OrigInLocX << ", " << OrigInLocY << ", " << OrigInLocZ << endl;
  cout << endl;

  TGraph *mygraph = new TGraph();
  mygraph->SetName("mygraph");
  mygraph->SetTitle("mygraph");
  
  TTree *tinput = (TTree*) finput->Get("ND280upEvents");

  //
  // Take the MPPC 2D histogram
  //

  ND280UpApplyResponse ApplyResponse;    
  ApplyResponse.SetMPPCProj2D_XY(h2d_xy);
  ApplyResponse.SetMPPCProj2D_XZ(h2d_xz);
  ApplyResponse.SetMPPCProj2D_YZ(h2d_yz);
  // FGDlike
  if(DetType == nd280upconv::kFGDlike){
    ApplyResponse.SetDetNameAlongX("/BarScintHoriz");
    ApplyResponse.SetDetNameAlongY("");
    ApplyResponse.SetDetNameAlongZ("/BarScintVert");
  }
  // SciFi
  //ApplyResponse.SetDetNameAlongX("/FiberScintHoriz");
  //ApplyResponse.SetDetNameAlongY("");
  //ApplyResponse.SetDetNameAlongZ("/FiberScintVert");

  
  // Declare the input true event object
  TND280UpEvent *nd280UpEvent = new TND280UpEvent();  
  tinput->SetBranchAddress("Event",&nd280UpEvent);
  
  int NTreeEntries = tinput->GetEntries();  
  int evtlasttree = NTreeEntries-1;
  int Nentries = -999;

  cout << endl;
  cout << "# of entries in the Tree: " << NTreeEntries << endl;
  cout << "last tree event ID: " << evtlasttree << endl;
  cout << "first event ID: " << evtfirst << endl;
  cout << endl;
  
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
  else{
    Nentries = nevents;    
  }
  
  int EntryLast = evtfirst+Nentries-1;
  
  cout << endl;
  cout << "First event = " << evtfirst << endl;
  cout << "Last event = " << EntryLast << endl;
  cout << "# of events = " << Nentries << endl;
  cout << endl;

  TStopwatch sw_event;

  for(int ievt=evtfirst;ievt<=EntryLast;ievt++){ // get last entry
    tinput->GetEntry(ievt);

    if(!(ievt%500)){
      cout << "Event " << ievt;
      sw_event.Stop();
      double CPUtime = sw_event.CpuTime();
      sw_event.Reset();
      sw_event.Start(0);
      cout << " --> CPU time = " << CPUtime << endl;
    }
    
    TString name;

    if(ievt<NEvtDisplTot){
      name = TString::Format("hMPPCHits_XY_%d",ievt);
      hMPPCHits_XY[ievt] = (TH2F*)h2d_xy->Clone(name);
      name = TString::Format("hMPPCHits_XZ_%ыd",ievt);
      hMPPCHits_XZ[ievt] = (TH2F*)h2d_xz->Clone(name);
      name = TString::Format("hMPPCHits_YZ_%d",ievt);
      hMPPCHits_YZ[ievt] = (TH2F*)h2d_yz->Clone(name);
      
      name = TString::Format("hPEVsTime_x_%d",ievt);
      hPEVsTime_x[ievt] = new TH1F(name,name,100,0,10000);
      name = TString::Format("hPEVsTime_y_%d",ievt);
      hPEVsTime_y[ievt] = new TH1F(name,name,100,0,10000);
      name = TString::Format("hPEVsTime_z_%d",ievt);
      hPEVsTime_z[ievt] = new TH1F(name,name,100,0,10000);
    }
    
    // Loop over the hits
    
    int NHits = nd280UpEvent->GetNHits();

    if(DEBUG){
      cout << endl;
      cout << "//////////////////////" << endl;
      cout << "Event: " << ievt << endl;
      cout << "# of hits = " << nd280UpEvent->GetNHits() << endl;
    }

    Int_t binsX = h2d_xy->GetXaxis()->GetNbins();
    Int_t binsY = h2d_xy->GetYaxis()->GetNbins();
    Int_t binsZ = h2d_xz->GetYaxis()->GetNbins();

    TH3F* event_histo = new TH3F("ev_h", "", binsX, 1., binsX*1. + 1., binsY, 1., binsY*1. + 1., binsZ, 1., binsZ*1. + 1.);

    // DEFINE HITS MAP
    TH2F* hits_map_XY = (TH2F*)h2d_xy->Clone("hits_map_XY");
    TH2F* hits_map_XZ = (TH2F*)h2d_xz->Clone("hits_map_XZ");
    TH2F* hits_map_YZ = (TH2F*)h2d_yz->Clone("hits_map_YZ");

    for(int ihit=0;ihit<NHits;ihit++){ // get last entry
      TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);

      double charge = 1.; // Apply alway Birks! 
      double time = (nd280UpHit->GetStartT() + nd280UpHit->GetStopT())/2.; // middle step time
      double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
      double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2.; // middle step Y 
      double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2.; // middle step Z
      TVector3 lightPos(posX,posY,posZ); // already in local position

      double edep = nd280UpHit->GetEnergyDeposit(); 

      double steplength = nd280UpHit->GetTrackLength(); // check how it's calculated in geant4

      string detname = nd280UpHit->GetDetName();

      //
      // Compute the detector response for each hit
      //
     	
      ApplyResponse.SetTargetID(DetType);
      ApplyResponse.CalcResponse(lightPos,1,0,charge,time,steplength,edep,detname);
      
      double pex = ApplyResponse.GetHitPE().x();
      double pey = ApplyResponse.GetHitPE().y();
      double pez = ApplyResponse.GetHitPE().z();
      
      if(!UseViewXY) pez = 0;
      if(!UseViewYZ) pex = 0;
      if(!UseViewXZ) pey = 0;

      double timepex = ApplyResponse.GetHitTime().x();
      double timepey = ApplyResponse.GetHitTime().y();
      double timepez = ApplyResponse.GetHitTime().z();
      
      double poshitX = ApplyResponse.GetHitPos().x();
      double poshitY = ApplyResponse.GetHitPos().y();
      double poshitZ = ApplyResponse.GetHitPos().z();
            
      if(ievt<NEvtDisplTot){
        hPEVsTime_x[ievt]->Fill(timepex,pex);
        hPEVsTime_y[ievt]->Fill(timepey,pey);
        hPEVsTime_z[ievt]->Fill(timepez,pez);
      }

      // store light yield
      Int_t MPPCx = h2d_xz->GetXaxis()->FindBin(poshitX);
      Int_t MPPCy = h2d_yz->GetXaxis()->FindBin(poshitY);
      Int_t MPPCz = h2d_yz->GetYaxis()->FindBin(poshitZ);
#if CROSSTALK > 0
      event_histo->Fill(MPPCx, MPPCy, MPPCz, (pex+pey+pez) / 3.);
#endif    

      hits_map_XY->Fill(poshitX,poshitY,pez);  // pe along Z
      hits_map_XZ->Fill(poshitX,poshitZ,pey); // pe along Y
      hits_map_YZ->Fill(poshitY,poshitZ,pex); // pe along X
    } // end loop over the hits

    // channel - to - channel crosstalk
#if CROSSTALK == 1
    TH2F* hits_map_temp[3];
    hits_map_temp[0] = (TH2F*)hits_map_XY->Clone("hits_map_XY_temp");
    hits_map_temp[1] = (TH2F*)hits_map_XZ->Clone("hits_map_XZ_temp");
    hits_map_temp[2] = (TH2F*)hits_map_YZ->Clone("hits_map_YZ_temp");

    TH2F* hits_map_final[3];
    hits_map_final[0] = hits_map_XY;
    hits_map_final[1] = hits_map_XZ;
    hits_map_final[2] = hits_map_YZ;

    double prob = 1. - 1./1.148;

    double leave_prob = 0.;
    double dir_prob = 5.;
    // for each of 3 projections
    for (int proj = 0; proj < 3; ++proj) {
      // 2D scan over channel map
      for (int i = 1; i <= hits_map_temp[proj]->GetXaxis()->GetNbins(); ++i) {
        for (int j = 1; j <= hits_map_temp[proj]->GetYaxis()->GetNbins(); ++j) {

          if (hits_map_temp[proj]->GetBinContent(i, j) < 1.)
            continue;

          // set number of photons
          hits_map_final[proj]->SetBinContent(i, j, hits_map_temp[proj]->GetBinContent(i, j));

          int Nphot = int(hits_map_temp[proj]->GetBinContent(i, j) + 0.5);
          for (int ph = 0; ph < Nphot; ++ph) {
            leave_prob = fRndm->Uniform();
            if (leave_prob > prob)
              continue;
            // subtract photon
            hits_map_final[proj]->SetBinContent(i, j, hits_map_final[proj]->GetBinContent(i, j) - 1);

            // add photon to neigbour channel
            dir_prob = fRndm->Uniform(4.);
            if (dir_prob < 1 && i > 1)
              hits_map_final[proj]->Fill(hits_map_final[proj]->GetXaxis()->GetBinCenter(i-1), 
                                         hits_map_final[proj]->GetXaxis()->GetBinCenter(j));
            else if (dir_prob < 2 && j < hits_map_final[proj]->GetYaxis()->GetNbins())
              hits_map_final[proj]->Fill(hits_map_final[proj]->GetXaxis()->GetBinCenter(i),   
                                         hits_map_final[proj]->GetXaxis()->GetBinCenter(j+1));
            else if (dir_prob < 3 && i < hits_map_final[proj]->GetXaxis()->GetNbins())
              hits_map_final[proj]->Fill(hits_map_final[proj]->GetXaxis()->GetBinCenter(i+1), 
                                         hits_map_final[proj]->GetXaxis()->GetBinCenter(j));
            else if (dir_prob < 4 && j > 1)
              hits_map_final[proj]->Fill(hits_map_final[proj]->GetXaxis()->GetBinCenter(i),   
                                         hits_map_final[proj]->GetXaxis()->GetBinCenter(j-1));
          }
        }
      }
    }
    delete hits_map_temp[0];
    delete hits_map_temp[1];
    delete hits_map_temp[2];
#endif

    // artifitially add cross talk to neighbour cubes at known level.
#if CROSSTALK == 2
    for (Int_t i = 1; i <= hits_map_XY->GetXaxis()->GetNbins(); ++i) {
      for (Int_t j = 1; j <= hits_map_XY->GetYaxis()->GetNbins(); ++j) {
        for (Int_t k = 1; k <= hits_map_XZ->GetYaxis()->GetNbins(); ++k) {
          if (event_histo->GetBinContent(i, j, k) < 0.5)
            continue;

          Int_t phot = (int)(event_histo->GetBinContent(i, j, k) + 0.5);
          for (Int_t ph = 0; ph < phot; ++ph) {
            double rndunif = 1.;
              // left
            if (i > 1) {
              rndunif = fRndm->Uniform();
              if (rndunif < 0.037) {
                hits_map_XY->Fill(hits_map_XY->GetXaxis()->GetBinCenter(i-1), hits_map_XY->GetYaxis()->GetBinCenter(j));
                hits_map_XZ->Fill(hits_map_XY->GetXaxis()->GetBinCenter(i-1), hits_map_YZ->GetYaxis()->GetBinCenter(k));
                hits_map_YZ->Fill(hits_map_YZ->GetXaxis()->GetBinCenter(j), hits_map_YZ->GetYaxis()->GetBinCenter(k));
              }
            }
              // right
            if (i < hits_map_XY->GetXaxis()->GetNbins()) {
              rndunif = fRndm->Uniform();
              if (rndunif < 0.037) {
                hits_map_XY->Fill(hits_map_XY->GetXaxis()->GetBinCenter(i+1), hits_map_XY->GetYaxis()->GetBinCenter(j));
                hits_map_XZ->Fill(hits_map_XY->GetXaxis()->GetBinCenter(i+1), hits_map_YZ->GetYaxis()->GetBinCenter(k));
                hits_map_YZ->Fill(hits_map_YZ->GetXaxis()->GetBinCenter(j), hits_map_YZ->GetYaxis()->GetBinCenter(k));
              }
            }
              // top
            if (j < hits_map_XY->GetYaxis()->GetNbins()) {
              rndunif = fRndm->Uniform();
              if (rndunif < 0.037) {
                hits_map_XY->Fill(hits_map_XY->GetXaxis()->GetBinCenter(i), hits_map_XY->GetYaxis()->GetBinCenter(j+1));
                hits_map_XZ->Fill(hits_map_XY->GetXaxis()->GetBinCenter(i), hits_map_YZ->GetYaxis()->GetBinCenter(k));
                hits_map_YZ->Fill(hits_map_YZ->GetXaxis()->GetBinCenter(j+1), hits_map_YZ->GetYaxis()->GetBinCenter(k));
              }
            }
              // bottom
            if (j > 1) {
              rndunif = fRndm->Uniform();
              if (rndunif < 0.037) {
                hits_map_XY->Fill(hits_map_XY->GetXaxis()->GetBinCenter(i), hits_map_XY->GetYaxis()->GetBinCenter(j-1));
                hits_map_XZ->Fill(hits_map_XY->GetXaxis()->GetBinCenter(i), hits_map_YZ->GetYaxis()->GetBinCenter(k));
                hits_map_YZ->Fill(hits_map_YZ->GetXaxis()->GetBinCenter(j-1), hits_map_YZ->GetYaxis()->GetBinCenter(k));
              }
            }
              // forward
            if (k < hits_map_XZ->GetYaxis()->GetNbins()) {
              rndunif = fRndm->Uniform();
              if (rndunif < 0.037) {
                hits_map_XY->Fill(hits_map_XY->GetXaxis()->GetBinCenter(i), hits_map_XY->GetYaxis()->GetBinCenter(j));
                hits_map_XZ->Fill(hits_map_XY->GetXaxis()->GetBinCenter(i), hits_map_YZ->GetYaxis()->GetBinCenter(k+1));
                hits_map_YZ->Fill(hits_map_YZ->GetXaxis()->GetBinCenter(j), hits_map_YZ->GetYaxis()->GetBinCenter(k+1));
              }
            }
              // backward
            if (k > 1) {
              rndunif = fRndm->Uniform();
              if (rndunif < 0.037) {
                hits_map_XY->Fill(hits_map_XY->GetXaxis()->GetBinCenter(i), hits_map_XY->GetYaxis()->GetBinCenter(j));
                hits_map_XZ->Fill(hits_map_XY->GetXaxis()->GetBinCenter(i), hits_map_YZ->GetYaxis()->GetBinCenter(k-1));
                hits_map_YZ->Fill(hits_map_YZ->GetXaxis()->GetBinCenter(j), hits_map_YZ->GetYaxis()->GetBinCenter(k-1));
              }
            }

          }
        }
      }
    }
#endif
#if CROSSTALK == 3
    // the most physical cross talk simulation
    // from random fiber to random fiber

    for (Int_t i = 1; i <= hits_map_XY->GetXaxis()->GetNbins(); ++i) {
      for (Int_t j = 1; j <= hits_map_XY->GetYaxis()->GetNbins(); ++j) {
        for (Int_t k = 1; k <= hits_map_XZ->GetYaxis()->GetNbins(); ++k) {
          if (event_histo->GetBinContent(i, j, k) > 0) {
              // add cross talk
            Int_t phot = (int)(event_histo->GetBinContent(i, j, k)*1.222 + 0.5);

            hits_map_YZ->SetBinContent(j, k, hits_map_YZ->GetBinContent(j, k) * 1.222);
            hits_map_XZ->SetBinContent(i, k, hits_map_XZ->GetBinContent(i, k) * 1.222);
            hits_map_XY->SetBinContent(i, j, hits_map_XY->GetBinContent(i, j) * 1.222);

            for (Int_t ph = 0; ph < phot; ++ph) {
              double leave_prob = 1.;
              double which_leave = 3.;
              double which_surface = 6.;
              double which_fiber = 3.;
              leave_prob    = fRndm->Uniform();
              which_leave   = fRndm->Uniform(3.);
              which_surface = fRndm->Uniform(6.);
              which_fiber   = fRndm->Uniform(3.);

              Float_t prob =  1. - 1./1.222;

              if (leave_prob < prob) {
                  // substract photon
                if (which_leave < 1) {
                  hits_map_YZ->SetBinContent(j, k, hits_map_YZ->GetBinContent(j, k) - 1);
                } else if (which_leave < 2) {
                  hits_map_XZ->SetBinContent(i, k, hits_map_XZ->GetBinContent(i, k) - 1);
                } else {
                  hits_map_XY->SetBinContent(i, j, hits_map_XY->GetBinContent(i, j) - 1);
                }
                  // add photon
                if (which_surface < 1 && i > 1) {
                  if (which_fiber < 1) {
                    hits_map_YZ->SetBinContent(j, k, hits_map_YZ->GetBinContent(j, k) + 1);
                  } else if (which_fiber < 2) {
                    hits_map_XZ->SetBinContent(i-1, k, hits_map_XZ->GetBinContent(i-1, k) + 1);
                  } else if (which_fiber < 3) {
                    hits_map_XY->SetBinContent(i-1, j, hits_map_XY->GetBinContent(i-1, j) + 1);
                  }
                } else if (which_surface < 2 && i < hits_map_XY->GetXaxis()->GetNbins()) {
                  if (which_fiber < 1) {
                    hits_map_YZ->SetBinContent(j, k, hits_map_YZ->GetBinContent(j, k) + 1);
                  } else if (which_fiber < 2) {
                    hits_map_XZ->SetBinContent(i+1, k, hits_map_XZ->GetBinContent(i+1, k) + 1);
                  } else if (which_fiber < 3) {
                    hits_map_XY->SetBinContent(i+1, j, hits_map_XY->GetBinContent(i+1, j) + 1);
                  }
                } else if (which_surface < 3 && j > 1) {
                  if (which_fiber < 1) {
                    hits_map_YZ->SetBinContent(j-1, k, hits_map_YZ->GetBinContent(j-1, k) + 1);
                  } else if (which_fiber < 2) {
                    hits_map_XZ->SetBinContent(i, k, hits_map_XZ->GetBinContent(i, k) + 1);
                  } else if (which_fiber < 3) {
                    hits_map_XY->SetBinContent(i, j-1, hits_map_XY->GetBinContent(i, j-1) + 1);
                  }
                } else if (which_surface < 4 && j < hits_map_XY->GetYaxis()->GetNbins()) {
                  if (which_fiber < 1) {
                    hits_map_YZ->SetBinContent(j+1, k, hits_map_YZ->GetBinContent(j+1, k) + 1);
                  } else if (which_fiber < 2) {
                    hits_map_XZ->SetBinContent(i, k, hits_map_XZ->GetBinContent(i, k) + 1);
                  } else if (which_fiber < 3) {
                    hits_map_XY->SetBinContent(i, j+1, hits_map_XY->GetBinContent(i, j+1) + 1);
                  }
                } else if (which_surface < 5 && k > 1) {
                  if (which_fiber < 1) {
                    hits_map_YZ->SetBinContent(j, k-1, hits_map_YZ->GetBinContent(j, k-1) + 1);
                  } else if (which_fiber < 2) {
                    hits_map_XZ->SetBinContent(i, k-1, hits_map_XZ->GetBinContent(i, k-1) + 1);
                  } else if (which_fiber < 3) {
                    hits_map_XY->SetBinContent(i, j, hits_map_XY->GetBinContent(i, j) + 1);
                  }
                } else if (which_surface < 6 && k < hits_map_XZ->GetYaxis()->GetNbins()) {
                  if (which_fiber < 1) {
                    hits_map_YZ->SetBinContent(j, k-1, hits_map_YZ->GetBinContent(j, k-1) + 1);
                  } else if (which_fiber < 2) {
                    hits_map_XZ->SetBinContent(i, k-1, hits_map_XZ->GetBinContent(i, k-1) + 1);
                  } else if (which_fiber < 3) {
                    hits_map_XY->SetBinContent(i, j, hits_map_XY->GetBinContent(i, j) + 1);
                  }
                }
              }
            }
          }
        }
      }
    }
#endif

    delete event_histo;

    // store several channels:
#if PROTO == 1
      if (hits_map_XZ->GetBinContent(1, 1) > 0);
        LY_ch[0]->Fill(hits_map_XZ->GetBinContent(1, 1));
      if (hits_map_XZ->GetBinContent(2, 1) > 0);
        LY_ch[1]->Fill(hits_map_XZ->GetBinContent(2, 1));
      if (hits_map_XZ->GetBinContent(3, 1) > 0);
        LY_ch[2]->Fill(hits_map_XZ->GetBinContent(3, 1));

      if (hits_map_YZ->GetBinContent(1, 1) > 0)
        LY_ch[3]->Fill(hits_map_YZ->GetBinContent(1, 1));
      if (hits_map_YZ->GetBinContent(2, 1) > 0)
        LY_ch[4]->Fill(hits_map_YZ->GetBinContent(2, 1));
      if (hits_map_YZ->GetBinContent(3, 1) > 0)
        LY_ch[5]->Fill(hits_map_YZ->GetBinContent(3, 1));

      if (hits_map_XZ->GetBinContent(1, 5) > 0)
        LY_ch[6]->Fill(hits_map_XZ->GetBinContent(1, 5));
      if (hits_map_XZ->GetBinContent(2, 5) > 0)
        LY_ch[7]->Fill(hits_map_XZ->GetBinContent(2, 5));
      if (hits_map_XZ->GetBinContent(3, 5) > 0)
        LY_ch[8]->Fill(hits_map_XZ->GetBinContent(3, 5));

      if (hits_map_YZ->GetBinContent(1, 5) > 0)
        LY_ch[9]->Fill(hits_map_YZ->GetBinContent(1, 5));
      if (hits_map_YZ->GetBinContent(2, 5) > 0)
        LY_ch[10]->Fill(hits_map_YZ->GetBinContent(2, 5));
      if (hits_map_YZ->GetBinContent(3, 5) > 0)
        LY_ch[11]->Fill(hits_map_YZ->GetBinContent(3, 5));

#elif PROTO == 2
    // Fill Z fibers
      Int_t counter = 0;
      for (Int_t i = 1; i < 25; ++i) {
        for (Int_t j = 1; j < 9; ++j) {
          if (hits_map_XY->GetBinContent(i , j) > 0) {
            LY_ch[counter]->Fill(hits_map_XY->GetBinContent(i , j));
          }
          ++counter;
        }
      }

    // Fill Y fibers
      for (Int_t i = 1; i < 49; ++i) {
        for (Int_t j = 1; j < 25; ++j) {
          if (hits_map_XZ->GetBinContent(j , i) > 0)
            LY_ch[counter]->Fill(hits_map_XZ->GetBinContent(j , i));
          ++counter;

        }
      }


    // Fill X fibers
      for (Int_t i = 1; i < 49; ++i) {
        for (Int_t j = 1; j < 9; ++j) {
          if (hits_map_YZ->GetBinContent(j , i) > 0)
            LY_ch[counter]->Fill(hits_map_YZ->GetBinContent(j , i));
          ++counter;
        }
      }
#endif


    // END OF FILLING HITS MAP

    if(ievt<NEvtDisplTot){
      hMPPCHits_XY[ievt]->Add(hits_map_XY); // pe along Z
      hMPPCHits_XZ[ievt]->Add(hits_map_XZ); // pe along Y
      hMPPCHits_YZ[ievt]->Add(hits_map_YZ); // pe along X
    }
  } // end loop over events   
  
  //
  // Write output file
  //
  
  fileout->cd();

  int last = evtfirst+Nentries-1;
  for(int ievtdispl=evtfirst;ievtdispl<=last ;ievtdispl++){  
    // Response
    if(ievtdispl<NEvtDisplTot){
            
      if(hMPPCHits_XY[ievtdispl]->Integral()>0 ||
	       hMPPCHits_XZ[ievtdispl]->Integral()>0 ||
	       hMPPCHits_YZ[ievtdispl]->Integral()>0 ){
	
        hMPPCHits_XY[ievtdispl]->Write();  
        hMPPCHits_XZ[ievtdispl]->Write();  
        hMPPCHits_YZ[ievtdispl]->Write();
      } 
    }
  }

#if PROTO == 1
    for (Int_t i = 0; i < 12; ++i)
      LY_ch[i]->Write();
#elif PROTO == 2
    for (Int_t i = 0; i < 1728; ++i)
      LY_ch[i]->Write();
#endif

  fileout->Close();
  
  return 0;
}
