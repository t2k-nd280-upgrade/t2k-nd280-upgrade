
#include <TFile.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TTree.h>
#include <TVector3.h>

#include <iostream>
#include <fstream>

#include "ND280UpConst.hh"
#include "ND280UpApplyResponse.hh"
#include "ND280UpReco.hh"
#include "ND280UpPID.hh"

#include "TND280UpEvent.hh"

#include "TND280UpRecoEvent.hh"
#include "TND280UpRecoTrack.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline int FindIndex(vector<int> vec,int val){  
  vector<int>::iterator it;
  it=find(vec.begin(),vec.end(),val);
  int pos = distance(vec.begin(), it);  
  if(it!=vec.end()){
    //cout<<"FOUND  "<< *it<<"  at position: "<<pos<<endl;
  }
  else{
    cout<<"NOT FOUND"<<endl;
  }  
  return pos;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{   

  if (argc!=5){   // batch mode                                        
    cout << "You need to provide the following arguments:" << endl;
    cout << " 1) input ROOT file name (from GEANT4 simulation) " << endl;
    cout << " 2) first event number to run" << endl;
    cout << " 3) total number of events to run" << endl;
    cout << " 4) the tag for the output file name" << endl;
    //cout << " 5) maximum number of event displays" << endl;
    exit(1);
  }
 
  string rootfilename = argv[1];
  const int evtfirst = atoi(argv[2]);
  const int nevents = atoi(argv[3]);
  string tag = argv[4];
  
  const int NEvtDisplTot = 100000; //atoi(argv[5]); // max # of evt. displays 

  // Declare variable of reconstructed tracks

  vector<TH2F*> fRecoTrack_MPPCHit_XY;
  vector<TH2F*> fRecoTrack_MPPCHit_XZ;
  vector<TH2F*> fRecoTrack_MPPCHit_YZ;
  vector<int>   fRecoTrack_ID;
  vector<int>   fRecoTrack_PDG;
    
    
  // Declare histograms

  TH2F *hMPPCHits_XY[NEvtDisplTot]; 
  TH2F *hMPPCHits_XZ[NEvtDisplTot]; 
  TH2F *hMPPCHits_YZ[NEvtDisplTot];
  TH1F *hPEVsTime_x[NEvtDisplTot]; 
  TH1F *hPEVsTime_y[NEvtDisplTot]; 
  TH1F *hPEVsTime_z[NEvtDisplTot]; 

  TH1D * hMuon_Edep_X = new TH1D("hMuon_Edep_X","Muon energy deposit along X",20000,0,200);
  TH1D * hMuon_Edep_Y = new TH1D("hMuon_Edep_Y","Muon energy deposit along Y",20000,0,200);
  TH1D * hMuon_Edep_Z = new TH1D("hMuon_Edep_Z","Muon energy deposit along Z",20000,0,200);
 
  TH1D * hPion_Edep_X = new TH1D("hPion_Edep_X","Pion energy deposit along X",20000,0,200);
  TH1D * hPion_Edep_Y = new TH1D("hPion_Edep_Y","Pion energy deposit along Y",20000,0,200);
  TH1D * hPion_Edep_Z = new TH1D("hPion_Edep_Z","Pion energy deposit along Z",20000,0,200);

  TH1D * hProt_Edep_X = new TH1D("hProt_Edep_X","Proton energy deposit along X",20000,0,200);
  TH1D * hProt_Edep_Y = new TH1D("hProt_Edep_Y","Proton energy deposit along Y",20000,0,200);
  TH1D * hProt_Edep_Z = new TH1D("hProt_Edep_Z","Proton energy deposit along Z",20000,0,200);

  TH1D * hElec_Edep_X = new TH1D("hElec_Edep_X","Electron energy deposit along X",20000,0,200);
  TH1D * hElec_Edep_Y = new TH1D("hElec_Edep_Y","Electron energy deposit along Y",20000,0,200);
  TH1D * hElec_Edep_Z = new TH1D("hElec_Edep_Z","Electron energy deposit along Z",20000,0,200);

  TH1D * hMuon_Stopped_Edep_X = new TH1D("hMuon_Stopped_Edep_X","Stopping Muon energy deposit along X",20000,0,200);
  TH1D * hMuon_Stopped_Edep_Y = new TH1D("hMuon_Stopped_Edep_Y","Stopping Muon energy deposit along Y",20000,0,200);
  TH1D * hMuon_Stopped_Edep_Z = new TH1D("hMuon_Stopped_Edep_Z","Stopping Muon energy deposit along Z",20000,0,200);

  TH1D * hPion_Stopped_Edep_X = new TH1D("hPion_Stopped_Edep_X","Stopping Pion energy deposit along X",20000,0,200);
  TH1D * hPion_Stopped_Edep_Y = new TH1D("hPion_Stopped_Edep_Y","Stopping Pion energy deposit along Y",20000,0,200);
  TH1D * hPion_Stopped_Edep_Z = new TH1D("hPion_Stopped_Edep_Z","Stopping Pion energy deposit along Z",20000,0,200);

  TH1D * hProt_Stopped_Edep_X = new TH1D("hProt_Stopped_Edep_X","Stopping Proton energy deposit along X",20000,0,200);
  TH1D * hProt_Stopped_Edep_Y = new TH1D("hProt_Stopped_Edep_Y","Stopping Proton energy deposit along Y",20000,0,200);
  TH1D * hProt_Stopped_Edep_Z = new TH1D("hProt_Stopped_Edep_Z","Stopping Proton energy deposit along Z",20000,0,200);

  TH1D * hElec_Stopped_Edep_X = new TH1D("hElec_Stopped_Edep_X","Stopping Electron energy deposit along X",20000,0,200);
  TH1D * hElec_Stopped_Edep_Y = new TH1D("hElec_Stopped_Edep_Y","Stopping Electron energy deposit along Y",20000,0,200);
  TH1D * hElec_Stopped_Edep_Z = new TH1D("hElec_Stopped_Edep_Z","Stopping Electron energy deposit along Z",20000,0,200);



  // Set outputs

  TString outfilename = TString::Format("%s_Evt%d_NEvt%d.root",tag.c_str(),evtfirst,nevents);
  TFile *fileout = new TFile(outfilename.Data(),"RECREATE");
 
  TND280UpRecoEvent *fND280UpRecoEvent = new TND280UpRecoEvent();
  TTree *fOutEventTree = new TTree("ND280upRecoEvents","ND280 upgrade Reco Event Tree");
  fOutEventTree->Branch("RecoEvent","TND280UpRecoEvent",&fND280UpRecoEvent,128000,0);


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
    
  TTree *tinput = (TTree*) finput->Get("ND280upEvents");

  //
  // Take the MPPC 2D histogram
  //

  ND280UpApplyResponse ApplyResponse;    
  ApplyResponse.SetMPPCProj2D_XY(h2d_xy);
  ApplyResponse.SetMPPCProj2D_XZ(h2d_xz);
  ApplyResponse.SetMPPCProj2D_YZ(h2d_yz);

  // FGDlike
  ApplyResponse.SetDetNameAlongX("/BarScintHoriz");
  ApplyResponse.SetDetNameAlongY("");
  ApplyResponse.SetDetNameAlongZ("/BarScintVert");
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

  //
  // Loop over the events:
  // - apply response event by event
  // - apply reconstruction event by event
  //

  for(int ievt=evtfirst;ievt<=EntryLast;ievt++){ // get last entry
    tinput->GetEntry(ievt);

    int EvtID = nd280UpEvent->GetEventID();

    // Initialize/Reset variables of reconstructed tracks
    fRecoTrack_MPPCHit_XY.clear();
    fRecoTrack_MPPCHit_XZ.clear();
    fRecoTrack_MPPCHit_YZ.clear();
    fRecoTrack_ID.clear();
    fRecoTrack_PDG.clear();
    
    // Initialize histograms
    
    TString name;
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


    // Loop over the hits
    
    int NHits = nd280UpEvent->GetNHits();

    cout << endl;
    cout << "Event: " << ievt << endl;
    cout << "# of hits = " << nd280UpEvent->GetNHits() << endl;

    for(int ihit=0;ihit<NHits;ihit++){ // get last entry
      TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);
      
      int parentid = nd280UpHit->GetPrimaryId(); 
      //cout << parentid << endl;
      //int detid = (int)nd280upconv::kBadNum; 
      int pdg = nd280UpHit->GetPDG(); // only if MC hits of primary tracks are stored in TrackerSD
      //int pdg = (int)nd280upconv::kBadNum; 
      int trkid = nd280UpHit->fContributors.front(); 
      double edep = nd280UpHit->GetEnergyDeposit(); 
      double charge = 1.; // Apply alway Birks! 
      double time = (nd280UpHit->GetStartT() + nd280UpHit->GetStopT())/2.; // middle step time
      double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
      double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2.; // middle step Y 
      double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2.; // middle step Z
      TVector3 lightPos(posX,posY,posZ); // already in local position
      
      string detname = nd280UpHit->GetDetName();
      double steplength = nd280UpHit->GetTrackLength();

      //
      // Compute the detector response for each hit
      //

      ApplyResponse.SetTargetID(nd280upconv::kSuperFGD);
      //ApplyResponse.SetTargetID(nd280upconv::kFGDlike);
      ApplyResponse.CalcResponse(lightPos,trkid,parentid,charge,time,steplength,edep,detname);

      double pex = ApplyResponse.GetHitPE().x();
      double pey = ApplyResponse.GetHitPE().y();
      double pez = ApplyResponse.GetHitPE().z();
      
      double timepex = ApplyResponse.GetHitTime().x();
      double timepey = ApplyResponse.GetHitTime().y();
      double timepez = ApplyResponse.GetHitTime().z();
      
      double poshitX = ApplyResponse.GetHitPos().x();
      double poshitY = ApplyResponse.GetHitPos().y();
      double poshitZ = ApplyResponse.GetHitPos().z();
      
      trkid = ApplyResponse.GetHitTrkID();            

      //hMPPCHits_XY[ievt]->Fill(poshitX,poshitY,pez); // pe along Z
      //hMPPCHits_XZ[ievt]->Fill(poshitX,poshitZ,pey); // pe along Y
      //hMPPCHits_YZ[ievt]->Fill(poshitY,poshitZ,pex); // pe along X

      hPEVsTime_x[ievt]->Fill(timepex,pex);
      hPEVsTime_y[ievt]->Fill(timepey,pey);
      hPEVsTime_z[ievt]->Fill(timepez,pez);

      //cout << nd280UpHit->GetPEX() << ", " << pex << endl;
      //cout << nd280UpHit->GetPEY() << ", " << pey << endl;
      //cout << nd280UpHit->GetPEZ() << ", " << pez << endl;
      //cout << endl;      

      
      //////////////////////////////////////////////////
      //                                              //
      // True "pattern recognition" (100% efficiency) //
      //                                              //
      //////////////////////////////////////////////////

      // Fill vector of TH2F, each element for a different true track
      // Fill vector of PDG
      // Fill vector of TrackID
      
      bool used=false;
      for(unsigned int itrk=0;itrk<fRecoTrack_ID.size();itrk++){
	if(trkid==fRecoTrack_ID[itrk]) used=true; 
      }     
      if(!used){
	fRecoTrack_ID.push_back(trkid);
	fRecoTrack_PDG.push_back(pdg);	
	fRecoTrack_MPPCHit_XY.push_back(new TH2F(*h2d_xy));
	fRecoTrack_MPPCHit_XZ.push_back(new TH2F(*h2d_xz));
	fRecoTrack_MPPCHit_YZ.push_back(new TH2F(*h2d_yz));
      }
      
      // Find vector index
      int idx = FindIndex(fRecoTrack_ID,trkid);
      fRecoTrack_MPPCHit_XY[idx]->Fill(poshitX,poshitY,pez); // pe along Z
      fRecoTrack_MPPCHit_XZ[idx]->Fill(poshitX,poshitZ,pey); // pe along Y
      fRecoTrack_MPPCHit_YZ[idx]->Fill(poshitY,poshitZ,pex); // pe along X

      //////////////////////////////////////
      //                                  //
      // Fill the PDFs to use for the PID //	
      //                                  //
      //////////////////////////////////////
      
      if(abs(pdg)==13){
	hMuon_Edep_X->Fill(pex); hMuon_Edep_Y->Fill(pey); hMuon_Edep_Z->Fill(pez);
	hMuon_Stopped_Edep_X->Fill(pex); hMuon_Stopped_Edep_Y->Fill(pey); hMuon_Stopped_Edep_Z->Fill(pez);
      }
      else if(abs(pdg)==211){
	hPion_Edep_X->Fill(pex); hPion_Edep_Y->Fill(pey); hPion_Edep_Z->Fill(pez);
	hPion_Stopped_Edep_X->Fill(pex); hPion_Stopped_Edep_Y->Fill(pey); hPion_Stopped_Edep_Z->Fill(pez);
      }
      else if(abs(pdg)==2212){
	hProt_Edep_X->Fill(pex); hProt_Edep_Y->Fill(pey); hProt_Edep_Z->Fill(pez);
	hProt_Stopped_Edep_X->Fill(pex); hProt_Stopped_Edep_Y->Fill(pey); hProt_Stopped_Edep_Z->Fill(pez);
      }
      else if(abs(pdg)==11){
	hElec_Edep_X->Fill(pex); hElec_Edep_Y->Fill(pey); hElec_Edep_Z->Fill(pez);
	hElec_Stopped_Edep_X->Fill(pex); hElec_Stopped_Edep_Y->Fill(pey); hElec_Stopped_Edep_Z->Fill(pez);
      }
            
    } // end loop over the hits

    
    //
    // Fill the output Reco Event 
    // - add reconstructed tracks
    //
    // The informations for each "reco" track to look at are stored in:
    // - fRecoTrack_MPPCHit_XY: # pe Vs MPPC hit position (XY)
    // - fRecoTrack_MPPCHit_XZ: # pe Vs MPPC hit position (XZ)
    // - fRecoTrack_MPPCHit_YZ: # pe Vs MPPC hit position (YZ)
    // - fRecoTrack_ID: track ID
    // - fRecoTrack_PDG: PDG
    //
    //

    cout << "# of tracks: " << fRecoTrack_ID.size() << endl;

    for(unsigned int itrk=0;itrk<fRecoTrack_ID.size();itrk++){

      //if(fRecoTrack_PDG[itrk] == 22){
      hMPPCHits_XY[ievt]->Add(fRecoTrack_MPPCHit_XY[itrk]); // pe along Z 
      hMPPCHits_XZ[ievt]->Add(fRecoTrack_MPPCHit_XZ[itrk]); // pe along Y
      hMPPCHits_YZ[ievt]->Add(fRecoTrack_MPPCHit_YZ[itrk]); // pe along X
      //}

      //cout << fRecoTrack_PDG[itrk] << ", ";
      cout << fRecoTrack_ID[itrk] << ", ";
      
      TND280UpRecoTrack *fND280UpRecoTrack = new TND280UpRecoTrack();
      fND280UpRecoTrack->SetTrackID(fRecoTrack_ID[itrk]);
      fND280UpRecoTrack->SetPDG(fRecoTrack_PDG[itrk]);
      //fND280UpRecoTrack->SetParentID(); // TODO
      //fND280UpRecoTrack->SetInitMom(); // TODO
      //fND280UpRecoTrack->SetInitCosTheta(); // TODO
      //fND280UpRecoTrack->SetCharge(); // TODO
      //fND280UpRecoTrack->SetRange(); // TODO
      
      //fND280UpRecoTrack->SetMPPCHit_XY(fRecoTrack_MPPCHit_XY[itrk]);
      //fND280UpRecoTrack->SetMPPCHit_XZ(fRecoTrack_MPPCHit_XZ[itrk]);
      //fND280UpRecoTrack->SetMPPCHit_YZ(fRecoTrack_MPPCHit_YZ[itrk]);    

      //
      fND280UpRecoEvent->AddTrack(fND280UpRecoTrack);
    }
    cout << endl;

    fND280UpRecoEvent->SetEventID(EvtID);  
    
    // Fill the output tree with reco events

    fOutEventTree->Fill(); // This slows down a lot!!!

  } // end loop over events
  
  


  
  
  

  /*

  //////////////////////////
  //                      //
  // Apply reconstruction //
  //                      //
  //////////////////////////
  
  for(int ievt=evtfirst;ievt<=EntryLast;ievt++){ // get last entry
    //tinput->GetEntry(ievt);
    //int EvtID = nd280UpRecoEvent->GetEventID();
    
    
    // Apply tracking conditions (simplified pattern recognition)
    // - separation among tracks
    // - stopping particles
    // - get track length
    
    // Fill TND280UpRecoTrack after "simplified pattern recognition
    // - start / last track point
    // - track length
    // - true trkid
    // - true pdg
    // - all mppc hits (position,edep) 
    
    
    //ND280UpReco nd280UpReco; // class with reconstruction methods
    
    // Compute the Cumulative distribution for PID
    
    ND280UpPID  nd280UpPID; // class with PID methods
    nd280UpPID.SetMPPCHit_XY(hMPPCHits_XY[ievt]);
    nd280UpPID.SetMPPCHit_XZ(hMPPCHits_XZ[ievt]);
    nd280UpPID.SetMPPCHit_YZ(hMPPCHits_YZ[ievt]);
    //nd280UpPID.SetTrackID();
  
  }
  */

  
  
  
  // Calculate the track length
  
  // Calculate the reconstructed energy / momentum
  
  // Fill the TND280UpRecoTrack class w/ MPPC hits (see above)
  
  
  





  
  
  //
  // Write output file
  //
  
  fileout->cd();

  for(int ievtdispl=evtfirst;ievtdispl<=EntryLast;ievtdispl++){ // get last entry
    if(hMPPCHits_XY[ievtdispl]->Integral()>0 ||
       hMPPCHits_XZ[ievtdispl]->Integral()>0 ||
       hMPPCHits_YZ[ievtdispl]->Integral()>0 ){
      
      hMPPCHits_XY[ievtdispl]->Write();  
      hMPPCHits_XZ[ievtdispl]->Write();  
      hMPPCHits_YZ[ievtdispl]->Write();
      
      hPEVsTime_x[ievtdispl]->Write(); 
      hPEVsTime_y[ievtdispl]->Write(); 
      hPEVsTime_z[ievtdispl]->Write(); 
    }
  }
  //
  hMuon_Edep_X->Write(); hMuon_Edep_Y->Write(); hMuon_Edep_Z->Write();
  hPion_Edep_X->Write(); hPion_Edep_Y->Write(); hPion_Edep_Z->Write();
  hProt_Edep_X->Write(); hProt_Edep_Y->Write(); hProt_Edep_Z->Write();
  hElec_Edep_X->Write(); hElec_Edep_Y->Write(); hElec_Edep_Z->Write();
  hMuon_Stopped_Edep_X->Write(); hMuon_Stopped_Edep_Y->Write(); hMuon_Stopped_Edep_Z->Write();
  hPion_Stopped_Edep_X->Write(); hPion_Stopped_Edep_Y->Write(); hPion_Stopped_Edep_Z->Write();
  hProt_Stopped_Edep_X->Write(); hProt_Stopped_Edep_Y->Write(); hProt_Stopped_Edep_Z->Write();
  hElec_Stopped_Edep_X->Write(); hElec_Stopped_Edep_Y->Write(); hElec_Stopped_Edep_Z->Write();
  //
  fOutEventTree->Write();
  //
  fileout->Close();
  
  return 0;
}


