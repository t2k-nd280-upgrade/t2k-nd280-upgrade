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
#include <ND280UpApplyResponse.hh>
#include "ND280UpRecoTrack.hh"
#include "ND280UpPID.hh"

#include "TND280UpEvent.hh"
#include "TND280UpVertex.hh"
#include "TND280UpRecoEvent.hh"
#include "TND280UpRecoTrack.hh"

#include "LightYield.hh"

const int VERTEX_ACTIVITY = 3;

#define NEvtDisplTot 50
bool PLOT = false;

bool CloseHit(int i, int j, TH2F* h_ini);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int NeutronAnalysis(int argc,char** argv) {
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

  std::string prefix = "_na";

  
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
  const int outname = atoi (argv[13]);


  // Set the inputs

  nd280upconv::TargetType_t DetType = nd280upconv::kUndefined;
  if     (detectorID == 0) DetType = nd280upconv::kSuperFGD;
  else if(detectorID == 1) DetType = nd280upconv::kFGDlike;
  

  // Declare canvases
#ifdef NEvtDisplTot
  TCanvas *cMPPCHits_XY[NEvtDisplTot]; 
  TCanvas *cMPPCHits_XZ[NEvtDisplTot]; 
  TCanvas *cMPPCHits_YZ[NEvtDisplTot];
#endif

  TRandom3* gen = new TRandom3();

  //
  // Declare histograms
  //

  // number of hits
  TH1F* hits_number = new TH1F("hitsN", "Number of hits", 50, 0., 50.);
  // hits versus energy
  TH2F* hits_energy = new TH2F("hitsE", "Hits vs Energy", 250, 0., 500., 50, 0., 50.);

  // initial energy versus cos theta
  TH2F* init_e_cos  = new TH2F("ini_ET", "Initial", 10, -1., 1., 250, 0., 500);
  TH2F* eff_e_cos   = new TH2F("eff_ET", "Efficiency", 10, -1., 1., 250, 0., 500);
  // p.e. vs theta and energy
  TH2F* pe_e_cos    = new TH2F("pe_ET", "PE from 1st hit", 10, -1., 1., 250, 0., 500);
  // p.e. vs energy
  TH2F* pe_e        = new TH2F("pe_E", "PE from 1st hit", 250., 0., 500, 250, 0., 500.);
  // distance from born position towards 1st hit
  TH2F* e_dist      = new TH2F("e_dist", "Energy vs distance", 300, 0., 1500, 250, 0., 500.);
  // momentum for forward going
  TH1F* mom_forward = new TH1F("mom",   "Momentum", 500, 0., 1000.);
  TH1F* mom_norm    = new TH1F("mom1",   "Momentum1", 500, 0., 1000.);

  TH2F* vertexXZ    = new TH2F("vertexA", "Average vertex activity", 5, -2., 3., 5, -2., 3.);

  TH2F* beta_ekin     = new TH2F("beta_ekin", "energy resolution", 250, 0., 500, 250, 0., 1.);
  TH2F* beta_ekin_sm  = new TH2F("beta_ekin_sm", "energy resolution", 250, 0., 500, 250, 0., 1.);

  TH1F* hits_cl_XY = new TH1F("hitsN_XY", "Number of hits XY", 50, 0., 50.);
  TH1F* hits_cl_XZ = new TH1F("hitsN_XZ", "Number of hits XZ", 50, 0., 50.);
  TH1F* hits_cl_YZ = new TH1F("hitsN_YZ", "Number of hits YZ", 50, 0., 50.);

  //TH1F* diff = new TH1F("diff", "", 400, -20., 20);

  int vertex_norm = 0;

  // Event displays
#ifdef NEvtDisplTot
  TH2F *hMPPCHits_XY[NEvtDisplTot]; 
  TH2F *hMPPCHits_XZ[NEvtDisplTot]; 
  TH2F *hMPPCHits_YZ[NEvtDisplTot];

  TGraph *gMCHits_XY[NEvtDisplTot]; 
  TGraph *gMCHits_XZ[NEvtDisplTot]; 
  TGraph *gMCHits_YZ[NEvtDisplTot];

  TH1F *hPEVsTime_x[NEvtDisplTot];
  TH1F *hPEVsTime_y[NEvtDisplTot];
  TH1F *hPEVsTime_z[NEvtDisplTot];
#endif
   // Set outputs
  
  TString outfilename = TString::Format("%s_Evt%d_NEvt%d.root",tag.c_str(),outname,nevents);
  TFile *fileout = new TFile(outfilename.Data(),"RECREATE"); 
  TTree* outtree = new TTree("neutron", "");

  Double_t ekin, costheta, light, dist_cubes, dist, first_hit_time, neutron_time, neutron_dist_true;
  Double_t N_hits_XY, N_hits_XZ, N_hits_YZ;
  Double_t dir_true[3], dir_reco[3];
  ekin = costheta = light = dist_cubes = dist = first_hit_time = neutron_time = neutron_dist_true = -1.;
  outtree->Branch("KinEnergy_true",     &ekin);
  outtree->Branch("CosTheta_true",      &costheta);
  outtree->Branch("Dir_True",           dir_true, "dir_true[3]/D");
  outtree->Branch("Dir_Reco",           dir_reco, "dir_reco[3]/D");
  outtree->Branch("Light",              &light);
  outtree->Branch("Distance_cubes",     &dist_cubes);
  outtree->Branch("Distance_hit_true",  &dist);
  outtree->Branch("First_hit_time",     &first_hit_time);
  outtree->Branch("Neutron_time",       &neutron_time);
  outtree->Branch("Neutron_dist",       &neutron_dist_true);

  outtree->Branch("N_hits_XY",          &N_hits_XY);
  outtree->Branch("N_hits_XZ",          &N_hits_XZ);
  outtree->Branch("N_hits_YZ",          &N_hits_YZ);

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
  /*
  h2d_xy = (TH2F*)finput->Get("fMPPCProj2D_XY_mod_01");
  h2d_xz = (TH2F*)finput->Get("fMPPCProj2D_XZ_mod_01");
  h2d_yz = (TH2F*)finput->Get("fMPPCProj2D_YZ_mod_01");
  */

  Int_t binX = h2d_xy->GetXaxis()->GetNbins();
  Int_t binY = h2d_xy->GetYaxis()->GetNbins();
  Int_t binZ = h2d_xz->GetYaxis()->GetNbins();

  Float_t X_min = h2d_xy->GetXaxis()->GetBinLowEdge(0);
  Float_t X_max = h2d_xy->GetXaxis()->GetBinUpEdge(binX);
  Float_t Y_min = h2d_xy->GetYaxis()->GetBinLowEdge(0);
  Float_t Y_max = h2d_xy->GetYaxis()->GetBinUpEdge(binX);
  Float_t Z_min = h2d_xz->GetYaxis()->GetBinLowEdge(0);
  Float_t Z_max = h2d_xz->GetYaxis()->GetBinUpEdge(binX);

  TH2F* first_bin_XY = (TH2F*)h2d_xy->Clone("fb_XY");
  TH2F* first_bin_XZ = (TH2F*)h2d_xz->Clone("fb_XZ");
  TH2F* first_bin_YZ = (TH2F*)h2d_yz->Clone("fb_YZ");

  TH3F* h3d = new TH3F("3d", "", binX, X_min, X_max, binY, Y_min, Y_max, binZ, Z_min, Z_max);

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

    h3d->Reset();
    
   // Initialize histograms
    
    TString name;

    if(ievt<NEvtDisplTot){

      name = TString::Format("gMCHits_XY_%d",ievt);
      gMCHits_XY[ievt] = new TGraph(); 
      gMCHits_XY[ievt]->SetName(name);
      gMCHits_XY[ievt]->SetTitle(name);
      gMCHits_XY[ievt]->SetMarkerSize(1);
      gMCHits_XY[ievt]->SetMarkerStyle(20);
      name = TString::Format("gMCHits_XZ_%d",ievt);
      gMCHits_XZ[ievt] = new TGraph(); 
      gMCHits_XZ[ievt]->SetName(name);
      gMCHits_XZ[ievt]->SetTitle(name);
      gMCHits_XZ[ievt]->SetMarkerSize(1);
      gMCHits_XZ[ievt]->SetMarkerStyle(20);
      name = TString::Format("gMCHits_YZ_%d",ievt);
      gMCHits_YZ[ievt] = new TGraph();
      gMCHits_YZ[ievt]->SetName(name);
      gMCHits_YZ[ievt]->SetTitle(name);
      gMCHits_YZ[ievt]->SetMarkerSize(1);
      gMCHits_YZ[ievt]->SetMarkerStyle(20);

      name = TString::Format("hMPPCHits_XY_%d",ievt);
      hMPPCHits_XY[ievt] = (TH2F*)h2d_xy->Clone(name);
      name = TString::Format("hMPPCHits_XZ_%d",ievt);
      hMPPCHits_XZ[ievt] = (TH2F*)h2d_xz->Clone(name);
      name = TString::Format("hMPPCHits_YZ_%d",ievt);
      hMPPCHits_YZ[ievt] = (TH2F*)h2d_yz->Clone(name);
      
      name = TString::Format("hPEVsTime_x_%d",ievt);
      hPEVsTime_x[ievt] = new TH1F(name,name,100,0,10000);
      name = TString::Format("hPEVsTime_y_%d",ievt);
      hPEVsTime_y[ievt] = new TH1F(name,name,100,0,10000);
      name = TString::Format("hPEVsTime_z_%d",ievt);
      hPEVsTime_z[ievt] = new TH1F(name,name,100,0,10000);    
      
      // Initialize canvases
      name = TString::Format("cMPPCHits_XY_%d",ievt);
      cMPPCHits_XY[ievt] = new TCanvas(name,name);
      name = TString::Format("cMPPCHits_XZ_%d",ievt);
      cMPPCHits_XZ[ievt] = new TCanvas(name,name);
      name = TString::Format("cMPPCHits_YZ_%d",ievt);
      cMPPCHits_YZ[ievt] = new TCanvas(name,name);
    }

    // take the vertex point
    if (nd280UpEvent->GetNVertices() < 1) {
      std::cout << "WARNING! no vertices in the event! The event will be skipped" << std::endl;
      continue;
    }

    Int_t vertex_binX = h2d_xz->GetXaxis()->FindBin(nd280UpEvent->GetVertex(0)->GetPosition().X());
    Int_t vertex_binY = h2d_yz->GetXaxis()->FindBin(nd280UpEvent->GetVertex(0)->GetPosition().Y());
    //Int_t vertex_binZ = h2d_yz->GetYaxis()->FindBin(nd280UpEvent->GetVertex(0)->GetPosition().Z() + 1695);
    Int_t vertex_binZ = h2d_yz->GetYaxis()->FindBin(nd280UpEvent->GetVertex(0)->GetPosition().Z());

    Double_t vertex_x = nd280UpEvent->GetVertex(0)->GetPosition().X();
    Double_t vertex_y = nd280UpEvent->GetVertex(0)->GetPosition().Y();
    Double_t vertex_z = nd280UpEvent->GetVertex(0)->GetPosition().Z();
    Double_t vertex_time = nd280UpEvent->GetVertex(0)->GetTime();


    // take for the neutron vars
    // TODO loop over trajs looking for neutron
    if (nd280UpEvent->GetNTracks() < 1) {
      std::cout << "WARNING! no tracks in the event! The event will be skipped" << std::endl;
      continue;
    }
    TND280UpTrack* track = nd280UpEvent->GetTrack(0);
    if (track->GetPDG() != 2112) {
      std::cout << "WARNING! First track is not neutron! The event will be skipped. PDG = " << track->GetPDG() << std::endl;
      continue;
    }

    ekin      = track->GetInitKinEnergy();
    costheta  = track->GetInitCosTheta();
    TVector3 mom_vec = track->GetInitMom();
    mom_vec = mom_vec.Unit();
    dir_true[0]  = mom_vec.X();
    dir_true[1]  = mom_vec.Y();
    dir_true[2]  = mom_vec.Z();
    
    // Loop over the hits
    
    int NHits = nd280UpEvent->GetNHits();

    if(DEBUG){
      cout << endl;
      cout << "//////////////////////" << endl;
      cout << "Event: " << ievt << endl;
      cout << "Vertex placed in cubes " << vertex_binX << "\t" << vertex_binY << "\t" << vertex_binZ << endl;
      cout << "# of hits = " << nd280UpEvent->GetNHits() << endl;
      cout << "neutron E   = " << ekin << endl;
      cout << "neutron cos = " << costheta << endl;
    }

    first_hit_time = 1.e9;
    dist = -2.;
    dist_cubes = -2.;

    Int_t first_binX, first_binY, first_binZ;

    // DEFINE HITS MAP
    TH2F* hits_map_XY = (TH2F*)h2d_xy->Clone("hits_map_XY");
    TH2F* hits_map_XZ = (TH2F*)h2d_xz->Clone("hits_map_XZ");
    TH2F* hits_map_YZ = (TH2F*)h2d_yz->Clone("hits_map_YZ");

    /*TND280UpTrackPoint* point = track->GetPoint(track->GetNPoints()-1);
    Int_t Last_true_binX = hits_map_XY->GetXaxis()->FindBin(point->GetPostPosition().X());
    Int_t Last_true_binY = hits_map_XY->GetYaxis()->FindBin(point->GetPostPosition().Y());
    Int_t Last_true_binZ = hits_map_YZ->GetYaxis()->FindBin(point->GetPostPosition().Z());
    Float_t Last_true_t  = point->GetTime();
    */

    bool far_away = false;
    if (vertex_binX < VERTEX_ACTIVITY || vertex_binX > binX - VERTEX_ACTIVITY ||
        vertex_binY < VERTEX_ACTIVITY || vertex_binY > binY - VERTEX_ACTIVITY ||
        vertex_binZ < VERTEX_ACTIVITY || vertex_binZ > binZ - VERTEX_ACTIVITY)
      far_away = true;

    // artificially fill the vertex activity region considering the worst case
    for (Int_t i = -VERTEX_ACTIVITY/2; i < VERTEX_ACTIVITY/2; ++i) {
      for (Int_t j = -VERTEX_ACTIVITY/2; j < VERTEX_ACTIVITY/2; ++j) {
        hits_map_XY->Fill(vertex_binX + i, vertex_binY + j, 100.);
        hits_map_XZ->Fill(vertex_binX + i, vertex_binZ + j, 100.);
        hits_map_YZ->Fill(vertex_binY + i, vertex_binZ + j, 100.);
      }
    }

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

      Int_t hit_binX = h2d_xz->GetXaxis()->FindBin(posX);
      Int_t hit_binY = h2d_yz->GetXaxis()->FindBin(posY);
      Int_t hit_binZ = h2d_yz->GetYaxis()->FindBin(posZ);

      //
      // Compute the detector response for each hit
      //
      
      ApplyResponse.SetTargetID(DetType);
      ApplyResponse.CalcResponse(lightPos,1,0,charge,time,steplength,edep,detname);
      
      double pex = ApplyResponse.GetHitPE().x();
      double pey = ApplyResponse.GetHitPE().y();
      double pez = ApplyResponse.GetHitPE().z();


      // ommit the vertex activity in VERTEX_ACTIVITY
      if (abs(hit_binX - vertex_binX) < VERTEX_ACTIVITY / 2 + 1 &&
          abs(hit_binY - vertex_binY) < VERTEX_ACTIVITY / 2 + 1 &&
          abs(hit_binZ - vertex_binZ) < VERTEX_ACTIVITY / 2 + 1) {
        /*if (!far_away){
          vertexXZ->Fill(hit_binX - vertex_binX, hit_binZ - vertex_binZ, pey);
        }*/
        continue;
      }
      
      if(!UseViewXY) pez = 0;
      if(!UseViewYZ) pex = 0;
      if(!UseViewXZ) pey = 0;

      double timepex = ApplyResponse.GetHitTime().x();
      double timepey = ApplyResponse.GetHitTime().y();
      double timepez = ApplyResponse.GetHitTime().z();
      
      double poshitX = ApplyResponse.GetHitPos().x();
      double poshitY = ApplyResponse.GetHitPos().y();
      double poshitZ = ApplyResponse.GetHitPos().z();

      hits_map_XY->Fill(poshitX,poshitY,pez);  // pe along Z
      hits_map_XZ->Fill(poshitX,poshitZ,pey); // pe along Y
      hits_map_YZ->Fill(poshitY,poshitZ,pex); // pe along X

      Int_t binX = hits_map_XY->GetXaxis()->FindBin(poshitX);
      Int_t binY = hits_map_XY->GetYaxis()->FindBin(poshitY);
      Int_t binZ = hits_map_YZ->GetYaxis()->FindBin(poshitZ);

      if (DEBUG) 
        cout << "   # hit " << ihit << "  pos "<< binX << "\t" << binY << "\t" << binZ  << "\t" << time << " ly " << pex + pey + pez << endl;

      if (pex + pey + pez > 0 && time < first_hit_time) {
        first_binX = hits_map_XY->GetXaxis()->FindBin(poshitX);
        first_binY = hits_map_XY->GetYaxis()->FindBin(poshitY);
        first_binZ = hits_map_YZ->GetYaxis()->FindBin(poshitZ);
        first_hit_time = time;
        dist = sqrt(  (poshitX - vertex_x) * (poshitX - vertex_x) + 
                      (poshitY - vertex_y) * (poshitY - vertex_y) +
                      (poshitZ - vertex_z) * (poshitZ - vertex_z));
        dist_cubes = sqrt(  (first_binX - vertex_binX) * (first_binX - vertex_binX) +
                            (first_binY - vertex_binY) * (first_binY - vertex_binY) +
                            (first_binZ - vertex_binZ) * (first_binZ - vertex_binZ) );
    
        /*dist_cubes_true = sqrt( (Last_true_binX - vertex_binX) * (Last_true_binX - vertex_binX) +
                                      (Last_true_binY - vertex_binY) * (Last_true_binY - vertex_binY) +
                                      (Last_true_binZ - vertex_binZ) * (Last_true_binZ - vertex_binZ) );*/
      }

      Int_t bin = h3d->FindBin(poshitX, poshitY, poshitZ);
      if (!h3d->GetBinContent(bin)) {
        h3d->Fill(poshitX, poshitY, poshitZ);
      }

      if(ievt<NEvtDisplTot){
        gMCHits_XY[ievt]->SetPoint(gMCHits_XY[ievt]->GetN(), poshitX, poshitY);
        gMCHits_XZ[ievt]->SetPoint(gMCHits_XZ[ievt]->GetN(), poshitX, poshitZ);
        gMCHits_YZ[ievt]->SetPoint(gMCHits_YZ[ievt]->GetN(), poshitY, poshitZ);

        hMPPCHits_XY[ievt]->Add(hits_map_XY);
        hMPPCHits_XZ[ievt]->Add(hits_map_XZ);
        hMPPCHits_YZ[ievt]->Add(hits_map_YZ);

        hPEVsTime_x[ievt]->Fill(timepex,pex);
        hPEVsTime_y[ievt]->Fill(timepey,pey);
        hPEVsTime_z[ievt]->Fill(timepez,pez);
      }
    } // end loop over the hits

    if (!far_away)
      ++vertex_norm;

    hits_number->Fill(h3d->Integral());

    hits_energy->Fill(ekin, h3d->Integral());
    init_e_cos->Fill(costheta, ekin);
    e_dist->Fill(dist, ekin);
    float mom = sqrt((939.565379 + ekin)*(939.565379 + ekin) - 939.565379*939.565379);
    if (costheta > 0.9)
      mom_norm->Fill(mom);

    // END OF FILLING HITS MAP

    // Draw the Canvas
    if(ievt<NEvtDisplTot){
      cMPPCHits_XY[ievt]->cd();
      hMPPCHits_XY[ievt]->Draw("colz");
      gMCHits_XY[ievt]->Draw("p same");
      //TargVtx_XY->Draw();
    
      cMPPCHits_YZ[ievt]->cd();
      hMPPCHits_YZ[ievt]->Draw("colz");
      gMCHits_YZ[ievt]->Draw("p same");
      //TargVtx_YZ->Draw();
    
      cMPPCHits_XZ[ievt]->cd();
      hMPPCHits_XZ[ievt]->Draw("colz");
      gMCHits_XZ[ievt]->Draw("p same");
      //TargVtx_XZ->Draw();
    }

    // The analysis

    // at least one hit should exists
    if (first_hit_time == 1.e9) 
      continue;

    TH2F* hits_map_XY_cluster = (TH2F*)h2d_xy->Clone("hits_map_XY_cluster");
    TH2F* hits_map_XZ_cluster = (TH2F*)h2d_xz->Clone("hits_map_XZ_cluster");
    TH2F* hits_map_YZ_cluster = (TH2F*)h2d_yz->Clone("hits_map_YZ_cluster");

    hits_map_XY_cluster->SetBinContent(first_binX, first_binY, 1.);
    hits_map_XZ_cluster->SetBinContent(first_binX, first_binZ, 1.);
    hits_map_YZ_cluster->SetBinContent(first_binY, first_binZ, 1.);

    if (DEBUG)
      cout  << "Clustering hits 0 " << hits_map_XY->Integral() 
                          << "\t" << hits_map_XZ->Integral()
                          << "\t" << hits_map_YZ->Integral() << endl;

    if (DEBUG)
      cout  << "Clustering hits 1 " << hits_map_XY_cluster->Integral() 
                          << "\t" << hits_map_XZ_cluster->Integral()
                          << "\t" << hits_map_YZ_cluster->Integral() << endl;

    int N_prev = 0;

    while (hits_map_XY_cluster->Integral() != N_prev) {
      N_prev = hits_map_XY_cluster->Integral();
      for (Int_t i = 1; i <= hits_map_XY_cluster->GetXaxis()->GetNbins(); ++i) {
        for (Int_t j = 1; j <= hits_map_XY_cluster->GetYaxis()->GetNbins(); ++j) {
          if (CloseHit(i, j, hits_map_XY) && !hits_map_XY_cluster->GetBinContent(i, j))
            hits_map_XY_cluster->SetBinContent(i, j, 1);
        }
      }
    }

    N_prev = 0;
    while (hits_map_XZ_cluster->Integral() != N_prev) {
      N_prev = hits_map_XZ_cluster->Integral();
      for (Int_t i = 1; i <= hits_map_XZ_cluster->GetXaxis()->GetNbins(); ++i) {
        for (Int_t j = 1; j <= hits_map_XZ_cluster->GetYaxis()->GetNbins(); ++j) {
          if (CloseHit(i, j, hits_map_XZ) && !hits_map_XZ_cluster->GetBinContent(i, j))
            hits_map_XZ_cluster->SetBinContent(i, j, 1);
        }
      }
    }

    N_prev = 0;
    while (hits_map_YZ_cluster->Integral() != N_prev) {
      N_prev = hits_map_YZ_cluster->Integral();
      for (Int_t i = 1; i <= hits_map_YZ_cluster->GetXaxis()->GetNbins(); ++i) {
        for (Int_t j = 1; j <= hits_map_YZ_cluster->GetYaxis()->GetNbins(); ++j) {
          if (CloseHit(i, j, hits_map_YZ) && !hits_map_YZ_cluster->GetBinContent(i, j))
            hits_map_YZ_cluster->SetBinContent(i, j, 1);
        }
      }
    }

    if (DEBUG)
      cout  << "Clustering hits 2 " << hits_map_XY_cluster->Integral() 
                          << "\t" << hits_map_XZ_cluster->Integral()
                          << "\t" << hits_map_YZ_cluster->Integral() << endl;


    // map the first hit
    Double_t x = hits_map_XY->GetXaxis()->GetBinCenter(first_binX);
    Double_t y = hits_map_XY->GetYaxis()->GetBinCenter(first_binY);
    Double_t z = hits_map_XZ->GetYaxis()->GetBinCenter(first_binZ);

    first_bin_XY->Fill(x, y);
    first_bin_YZ->Fill(y, z);
    first_bin_XZ->Fill(x, z);

    // separated  at least with a 1 hit from the vertex activity
    if (abs(first_binX - vertex_binX) < VERTEX_ACTIVITY + 1 &&
        abs(first_binY - vertex_binY) < VERTEX_ACTIVITY + 1 &&
        abs(first_binZ - vertex_binZ) < VERTEX_ACTIVITY + 1 )
      continue;

    hits_cl_XY->Fill(hits_map_XY_cluster->Integral());
    hits_cl_XZ->Fill(hits_map_XZ_cluster->Integral());
    hits_cl_YZ->Fill(hits_map_YZ_cluster->Integral());

    N_hits_XY = hits_map_XY_cluster->Integral();
    N_hits_XZ = hits_map_XZ_cluster->Integral();
    N_hits_YZ = hits_map_YZ_cluster->Integral();

    TVector3 reco_vec(first_binX - vertex_binX, first_binY - vertex_binY, first_binZ - vertex_binZ);
    reco_vec    = reco_vec.Unit();
    dir_reco[0] = reco_vec.X();
    dir_reco[1] = reco_vec.Y();
    dir_reco[2] = reco_vec.Z();

    // SELECTION IS DONE 
    // filling histoes
    Int_t NPoints = track->GetNPoints();
    neutron_time = -1.;
    neutron_dist_true = -1.;
    Int_t pre_X = -1; Int_t pre_Y = -1; Int_t pre_Z = -1;
    for (Int_t pointID = 0; pointID < NPoints; ++pointID) {
      TND280UpTrackPoint* point = track->GetPoint(pointID);
      if (!point)
        continue;

      Float_t pointX = point->GetPrePosition().X();
      Float_t pointY = point->GetPrePosition().Y();
      Float_t pointZ = point->GetPrePosition().Z();
      Float_t pointT = point->GetTime();

      Int_t point_binX = hits_map_XY->GetXaxis()->FindBin(pointX);
      Int_t point_binY = hits_map_XY->GetYaxis()->FindBin(pointY);
      Int_t point_binZ = hits_map_XZ->GetYaxis()->FindBin(pointZ);

      if (point_binX == pre_X && pre_Y == point_binY && point_binZ == pre_Z)
        continue;
      else {
        pre_X = point_binX;
        pre_Y = point_binY;
        pre_Z = point_binZ;
      }

      if (DEBUG)
        cout << "       # point " << pointID << "  pos " <<  point_binX << "\t" << point_binY << "\t" << point_binZ << "\t\t" << pointT <<endl;

      if (point_binX == first_binX && point_binY == first_binY && point_binZ == first_binZ) {
        neutron_time = pointT;
        neutron_dist_true = sqrt( (first_binX - vertex_binX) * (first_binX - vertex_binX) + 
                                  (point_binY - vertex_binY) * (point_binY - vertex_binY) +
                                  (point_binZ - vertex_binZ) * (point_binZ - vertex_binZ));
      }
    }

    if (DEBUG) {
      cout << "First light in time at " <<  first_binX << "\t" << 
                                            first_binY << "\t" << 
                                            first_binZ << "\t" <<
                                            first_hit_time << endl;
      cout << "Neutron was there at " << neutron_time << endl;
      /*cout << "Neutron end at " <<  Last_true_binX << "\t" << 
                                    Last_true_binY << "\t" << 
                                    Last_true_binZ << "\t" << 
                                    Last_true_t << std::endl;
      cout << "Light in this cube " <<  hits_map_XY->GetBinContent(Last_true_binX, Last_true_binY) << "\t" <<
                                        hits_map_XZ->GetBinContent(Last_true_binX, Last_true_binZ) << "\t" <<
                                        hits_map_YZ->GetBinContent(Last_true_binY, Last_true_binZ) << endl;*/
    }

    light = hits_map_XY->GetBinContent(first_binX, first_binY) +
                    hits_map_XZ->GetBinContent(first_binX, first_binZ) +
                    hits_map_YZ->GetBinContent(first_binY, first_binZ);

    outtree->Fill();

    eff_e_cos->Fill(costheta, ekin);
    
    pe_e_cos->Fill( costheta, ekin, light);

    pe_e->Fill(ekin, light);
    if (costheta > 0.9)
      mom_forward->Fill(mom);

    float beta = dist_cubes / (first_hit_time * 30.);
    beta_ekin->Fill(ekin, beta);

  } // loop over events

  fileout->cd();

  outtree->Write("", TObject::kOverwrite);

  hits_number->Write();
  hits_energy->Write();
  e_dist->Write();

  //diff->Write();

  first_bin_XY->Write();
  first_bin_YZ->Write();
  first_bin_XZ->Write();

  pe_e_cos->Write();
  pe_e->Write();

  init_e_cos->Write();
  eff_e_cos->Write();

  vertexXZ->Scale(1./vertex_norm);
  vertexXZ->Write();

  beta_ekin->Write();
  beta_ekin_sm->Write();

  mom_forward->Write();
  mom_norm->Write();

  hits_cl_XY->Write();
  hits_cl_XZ->Write();
  hits_cl_YZ->Write();

  int last = evtfirst+Nentries-1;
  for(int ievtdispl=evtfirst;ievtdispl<=last ;ievtdispl++){ 
    // Response
    if(ievtdispl<NEvtDisplTot && PLOT){
      //cout << "ievtdispl = " << ievtdispl << endl;
      if(hMPPCHits_XY[ievtdispl]->Integral()>0 ||
         hMPPCHits_XZ[ievtdispl]->Integral()>0 ||
         hMPPCHits_YZ[ievtdispl]->Integral()>0 ){
  
        hMPPCHits_XY[ievtdispl]->Write();  
        hMPPCHits_XZ[ievtdispl]->Write();  
        hMPPCHits_YZ[ievtdispl]->Write();
      
        gMCHits_XY[ievtdispl]->Write();
        gMCHits_XZ[ievtdispl]->Write();
        gMCHits_YZ[ievtdispl]->Write();
      
        cMPPCHits_XY[ievtdispl]->Write();  
        cMPPCHits_XZ[ievtdispl]->Write();  
        cMPPCHits_YZ[ievtdispl]->Write();
      }
    }
  }

  fileout->Close();
  
  return 0;
}

bool CloseHit(int i, int j, TH2F* h_ini) {
  if (!h_ini->GetBinContent(i, j))
    return false;

  if (i > 1 && h_ini->GetBinContent(i - 1, j))
    return true;
  if (i < h_ini->GetXaxis()->GetNbins() && h_ini->GetBinContent(i + 1, j))
    return true;
  if (j > 1 && h_ini->GetBinContent(i, j - 1))
    return true;
  if (j < h_ini->GetYaxis()->GetNbins() && h_ini->GetBinContent(i, j + 1))
    return true;

  if (i < h_ini->GetXaxis()->GetNbins() && j < h_ini->GetYaxis()->GetNbins() && h_ini->GetBinContent(i + 1, j + 1))
    return true;
  if (i < h_ini->GetXaxis()->GetNbins() && j > 1 && h_ini->GetBinContent(i + 1, j - 1))
    return true;
  if (i > 2 && j < h_ini->GetYaxis()->GetNbins() && h_ini->GetBinContent(i - 1, j + 1))
    return true;
  if (i > 2 && j > 2 && h_ini->GetBinContent(i - 1, j - 1))
    return true;

  return false;
}