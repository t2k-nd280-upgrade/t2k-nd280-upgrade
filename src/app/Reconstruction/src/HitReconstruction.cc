#include <TH2F.h>
#include <TH1F.h>
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TMath.h>
#include <TGraph.h>
#include <TF1.h>
#include <TPolyMarker3D.h>

#include "TND280UpHit.hh"
#include "TND280UpTrackPoint.hh"
#include "TND280UpTrack.hh"
#include "TND280UpVertex.hh"
#include "TND280UpEvent.hh"

#include "ND280UpConst.hh"
#include <ND280UpApplyResponse.hh>
#include "ND280UpRecoTrack.hh"
#include "ND280UpPID.hh"

#include "HitReconstruction.hh"

#include "LightYield.hh"

#include <iostream>
#include <fstream>


inline int FindIndex(vector<int> vec,int val){
  vector<int>::iterator it;
  it=find(vec.begin(),vec.end(),val);
  int pos = distance(vec.begin(), it);
  if(it!=vec.end()){
    //    cout<<"FOUND  "<< *it<<"  at position: "<<pos<<endl;
  }
  else{
    cout << "NOT FOUND" << endl;
    pos = -1;
  }
  return pos;
}


inline bool IsTargetIn(double x,double y,double z,
                // Target 1
		double vtx_min_x_1,
                double vtx_max_x_1,
                double vtx_min_y_1,
		double vtx_max_y_1,
                double vtx_min_z_1,
                double vtx_max_z_1
		){
  if((x > vtx_min_x_1 && x < vtx_max_x_1) &&
     (y > vtx_min_y_1 && y < vtx_max_y_1) &&
     (z > vtx_min_z_1 && z < vtx_max_z_1)
     ){
    return true;
  }
  else{
    return false;
  }
}




int HitReconstruction (int argc, char** argv){
 if (argc!=6) {
   
   cout << "1) input ROOT file name" << endl;
   cout << "2) first event number" << endl;
   cout << "3) total number of events" << endl;
   cout << "4) tag ([tag].root)" << endl;
   cout << "5) Use SFGD (1) or FGDXZ (0)" << endl;
  
   exit(1);

 }

 string infilename = argv[1];
 const int evtfirst = atoi(argv[2]);
 const int nevents = atoi(argv[3]);
 string tag = argv[4];

 // Set the inputs

 nd280upconv::TargetType_t DetType;

 const bool useSFGD = atoi(argv[5]);


 if (useSFGD) DetType = nd280upconv::kSuperFGD;
 else DetType = nd280upconv::kFGDlike;


  if(nevents>1000){
    cout << "You cannot use more than 1000 events!!!" << endl;
    exit(1);
  }

  const int NEvtDisplTot = 1000; 

  

  // Declare variable of reconstructed tracks
  vector<TH2F*> fRecoTrack_MPPCHit_XY;
  vector<TH2F*> fRecoTrack_MPPCHit_XZ;
  vector<TH2F*> fRecoTrack_MPPCHit_YZ;
  vector<int>   fRecoTrack_ID;
  vector<int>   fRecoTrack_PDG;
  vector<int>   fRecoTrack_ParentID;
  vector<bool> fRecoTrack_outfv;
  vector<bool> fRecoTrack_isreco;
  vector<double> fRecoTrack_reco_costh;
  vector<double> fRecoTrack_reco_mom;
  vector<double> fRecoTrack_reco_length;
  vector<double> fRecoTrack_true_length;
  vector<double> fRecoTrack_true_edep;

  vector< vector< vector<double> > > fRecoTrack_hitXY; // track< hit< (trkid,pdg,pos,pe) > >
  vector< vector< vector<double> > > fRecoTrack_hitXZ;
  vector< vector< vector<double> > > fRecoTrack_hitYZ;

  TH2F *hMPPCHits_XY[NEvtDisplTot];
  TH2F *hMPPCHits_XZ[NEvtDisplTot];
  TH2F *hMPPCHits_YZ[NEvtDisplTot];
  
  ////

  TFile *finput = new TFile(infilename.c_str(),"READ");



  // Take 2D projection histograms with MPPC hits 

  TH2F* h2d_xy; TH2F* h2d_xz; TH2F* h2d_yz;
  h2d_xy = (TH2F*)finput->Get("OutMPPCProj2D_XY");
  h2d_xz = (TH2F*)finput->Get("OutMPPCProj2D_XZ");
  h2d_yz = (TH2F*)finput->Get("OutMPPCProj2D_YZ");  
  //

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


  cout << "getting TTree" << endl;
  TTree *tinput = (TTree*) finput->Get("ND280upEvents");


  //
  // Take the MPPC 2D histogram
  //

  cout << "Apply response" << endl;
  ND280UpApplyResponse ApplyResponse;
  ApplyResponse.SetMPPCProj2D_XY(h2d_xy);
  ApplyResponse.SetMPPCProj2D_XZ(h2d_xz);
  ApplyResponse.SetMPPCProj2D_YZ(h2d_yz);
  //

  if (!useSFGD) {
    ApplyResponse.SetDetNameAlongX("/BarScintHoriz");
    ApplyResponse.SetDetNameAlongY("");
    ApplyResponse.SetDetNameAlongZ("/BarScintVert");
  }

  cout << "Apply response done" << endl;
  
  TND280UpEvent *nd280UpEvent = new TND280UpEvent();  
  tinput->SetBranchAddress("Event",&nd280UpEvent);
  
  int NTreeEntries = tinput->GetEntries();  
  int evtlasttree = evtfirst+NTreeEntries-1;
  int Nentries = nevents;
  // int Nentries = 1000;

  cout << endl;
  cout << "# of entries in the Tree: " << NTreeEntries << endl;
  cout << "last tree event ID: " << evtlasttree << endl;
  cout << "first event ID: " << evtfirst << endl;
  cout << endl;

  // int EntryLast = evtfirst+Nentries-1;


  vector <int> passed_evt;
  passed_evt.clear();



  // int nccqe =0;

  

  // BRANCH
  ///

  const int max = 100;

  int NEnt;
  int NTracks;
  int NRecoTracks;
  int neutmode;
  double vertex[3];
  int particle_id[max];
  int particle_id_true[max];
  double pmom[max];
  double pcos[max];
  

  ////

  TTree *outtree = new TTree("outtree","hit reconstruction output");
  
  //outtree->Branch("ntruetrk",        &NTracks,          "ntruetrk/I");
  outtree->Branch("Entry",           &NEnt,             "Entry/I");
  outtree->Branch("nrecotrk",        &NRecoTracks,      "nrecotrk/I");
  outtree->Branch("mode",            &neutmode,         "mode/I");
  outtree->Branch("vertex",          &vertex,           "vertex[3]/D");
  outtree->Branch("pdg",             &particle_id,      "pdg[nrecotrk]/I");
  //  outtree->Branch("pdg_true",        &particle_id_true, "pdg_true[ntruetrk]/I");
  //  outtree->Branch("pmom_true",       &pmom,             "pmom_true[ntruetrk]/D");
  // outtree->Branch("pcos_true",       &pcos,             "pcos_true[ntruetrk]/D");
  
  ////    

  for(int ievt=0;ievt<Nentries;ievt++){ // get last entry
    
    tinput->GetEntry(ievt);
    NEnt = ievt+evtfirst;

    for (int i=0;i<3;i++) {
      vertex[i]=0.;
    }
    for (int i=0;i<max;i++){
      pmom[i] = 0.; 
      pcos[i] =0.;
      particle_id[i] = 0;
      particle_id_true[i] = 0;
    }
  
    
    TString name;

    /////

    // Initialize/Reset variables of reconstructed tracks
    fRecoTrack_MPPCHit_XY.clear();
    fRecoTrack_MPPCHit_XZ.clear();
    fRecoTrack_MPPCHit_YZ.clear();
    fRecoTrack_ID.clear();
    fRecoTrack_ParentID.clear();
    fRecoTrack_PDG.clear();
    fRecoTrack_outfv.clear();
    fRecoTrack_isreco.clear();
    fRecoTrack_reco_costh.clear();
    fRecoTrack_reco_mom.clear();
    fRecoTrack_reco_length.clear();
    fRecoTrack_true_length.clear();
    fRecoTrack_true_edep.clear();
    fRecoTrack_hitXY.clear();
    fRecoTrack_hitXZ.clear();
    fRecoTrack_hitYZ.clear();

    for(unsigned int i=0;i<fRecoTrack_hitXY.size();i++){
      fRecoTrack_hitXY[i].clear();
      for(unsigned int j=0;j<fRecoTrack_hitXY[i].size();j++){
        fRecoTrack_hitXY[i][j].clear();
      }
    }

    for(unsigned int i=0;i<fRecoTrack_hitXZ.size();i++){
      fRecoTrack_hitXZ[i].clear();
      for(unsigned int j=0;j<fRecoTrack_hitXZ[i].size();j++){
        fRecoTrack_hitXZ[i][j].clear();
      }
    }


    for(unsigned int i=0;i<fRecoTrack_hitYZ.size();i++){
      fRecoTrack_hitYZ[i].clear();
      for(unsigned int j=0;j<fRecoTrack_hitYZ[i].size();j++){
        fRecoTrack_hitYZ[i][j].clear();
      }
    }

      name = TString::Format("hMPPCHits_XY_%d",ievt+evtfirst);
      hMPPCHits_XY[ievt] = (TH2F*)h2d_xy->Clone(name);
      hMPPCHits_XY[ievt]->SetTitle("");
      hMPPCHits_XY[ievt]->GetXaxis()->SetTitle("Distance from Target Center in X axis (mm)");
      hMPPCHits_XY[ievt]->GetYaxis()->SetTitle("Distance from Target Center in Y axis (mm)");

      name = TString::Format("hMPPCHits_XZ_%d",ievt+evtfirst);
      hMPPCHits_XZ[ievt] = (TH2F*)h2d_xz->Clone(name);
      hMPPCHits_XZ[ievt]->SetTitle("");
      hMPPCHits_XZ[ievt]->GetXaxis()->SetTitle("Distance from Target Center in X axis (mm)");
      hMPPCHits_XZ[ievt]->GetYaxis()->SetTitle("Distance from Target Center in Z axis (mm)");
      
      name = TString::Format("hMPPCHits_YZ_%d",ievt+evtfirst);
      hMPPCHits_YZ[ievt] = (TH2F*)h2d_yz->Clone(name);
      hMPPCHits_YZ[ievt]->SetTitle("");
      hMPPCHits_YZ[ievt]->GetXaxis()->SetTitle("Distance from Target Center in Y axis (mm)");
      hMPPCHits_YZ[ievt]->GetYaxis()->SetTitle("Distance from Target Center in Z axis (mm)");


    /////
      
    // Loop over the tracks
  
    NTracks = nd280UpEvent->GetNTracks();
  
    bool inthetarget = true;
  
    int NVertices = nd280UpEvent->GetNVertices();
    if(NVertices>1){
      cerr << endl;
      cerr << "More than 1 vertex is not yet supported!!!" << endl;
      cerr << endl;
      exit(1);
    }


    TND280UpVertex *nd280UpVertex;

    
      for (int ivtx=0;ivtx<NVertices;ivtx++)
	{
	  nd280UpVertex = nd280UpEvent->GetVertex(ivtx);
	  // nd280UpVertex->PrintVertex();
	  // TND280UpTrack *nd280UpVtxTrkIn1 = nd280UpVertex->GetInTrack(0);
	  // int pdg_vtxtrk1 = nd280UpVtxTrkIn1->GetPDG();
	  int reacmode = abs(nd280UpVertex->GetReacMode());
	  neutmode = reacmode;
	} 
    
      
      
    cout << "EVT #" << ievt << endl;
    /*
    cout << "NEUT MODE: " << neutmode << endl;
    cout << "NTracks: " << NTracks << endl;
      */

    
    for(int itrk=0;itrk<NTracks;itrk++){
      
      TND280UpTrack *nd280UpTrack = nd280UpEvent->GetTrack(itrk);
      // nd280UpTrack->PrintTrack();      
      // int trkID = nd280UpTrack->GetTrackID();
      int parentID = nd280UpTrack->GetParentID();
      int PDG = nd280UpTrack->GetPDG();
      double costheta = nd280UpTrack->GetInitCosTheta();
      double mom = nd280UpTrack->GetInitMom().Mag(); 
      // double range = nd280UpTrack->GetRange(); 
      // string procName = nd280UpTrack->GetProcessName();
      // double charge = nd280UpTrack->GetCharge();
      // double ekin = nd280UpTrack->GetInitKinEnergy();
      // int NPoints = nd280UpTrack->GetNPoints();

      particle_id_true[itrk] = PDG;
      pmom[itrk] = mom;
      pcos[itrk] = costheta;
      
      TND280UpTrackPoint *nd280UpTrackPoint = nd280UpTrack->GetPoint(0); 
      // double length = nd280UpTrackPoint->GetStepLength();
      // double PtMom = nd280UpTrackPoint->GetMomentum().Mag();
      // string volname = nd280UpTrackPoint->GetLogVolName();


      if (parentID==0){
	double PtX = nd280UpTrackPoint->GetPostPosition().X();
	double PtY = nd280UpTrackPoint->GetPostPosition().Y();
	double PtZ = nd280UpTrackPoint->GetPostPosition().Z();

	vertex[0] = PtX;
	vertex[1] = PtY;
	vertex[2] = PtZ;
	//	for (int a=0;a<3;a++) cout << vertex[a] <<endl;
	//inthetarget = IsTargetIn(PtX,PtY,PtZ,-1020.,1020.,-260.,260.,-940.,940.);
	inthetarget = IsTargetIn(PtX,PtY,PtZ,-960.,960.,-280.,280.,-2720.412,-800.412);
      }
    }
    
  if (!inthetarget){
	cout << "Out of Target; rejected."<<endl;
        continue;
  }  
  
  
    /////
  
    cout << " >>> Passed preliminary track cuts  <<< " << endl;
    cout << "NEUT MODE: " << neutmode << endl;
    cout << "NTracks: " << NTracks << endl;
   
    

      
    int NHits = nd280UpEvent->GetNHits();

    cout << "NHits:" << NHits << endl;

    double posX_prev = 0.;
    double posY_prev = 0.;
    double posZ_prev = 0.;
    int trkid_prev = 0.;
  
    for(int ihit=0;ihit<NHits;ihit++){ // get last entry


      TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);

      int parentid = nd280UpHit->GetPrimaryId();     
      double charge = 1.; // Apply alway Birks!
      int trkid = nd280UpHit->fContributors.front();
      double edep = nd280UpHit->GetEnergyDeposit();
      double time = (nd280UpHit->GetStartT() + nd280UpHit->GetStopT())/2.; // middle step time
      double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
      double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2.; // middle step Y
      double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2.; // middle step Z
      
      // cout << "getting pdg..." << endl;
      int pdg = nd280UpHit->GetPDG(); // only if MC hits of primary tracks are stored in TrackerSD
      // cout << "pdg = " << pdg << endl;
      TString detname = nd280UpHit->GetDetName();
      // cout << "detname = " << detname << endl;
      
      TVector3 lightPos(posX,posY,posZ); // already in local position
      double length_true_mchit = 0.;
      
      //
      // compute the mc hit length
      //
      
      // If the track is different the first segment has length null
      if(trkid_prev != trkid){
	posX_prev = posX;
        posY_prev = posY;
        posZ_prev = posZ;
      }
      double deltaX = posX - posX_prev;
      double deltaY = posY - posY_prev;
      double deltaZ = posZ - posZ_prev;

      length_true_mchit = sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ*deltaZ);

      double steplength = nd280UpHit->GetTrackLength(); // check how it's calculated in geant4


      ApplyResponse.SetTargetID(DetType);
      /*
      cout << "calculating response..." << endl;
      cout << "lightpos = " << lightPos[0] << ", " <<lightPos[1] << ", "<< lightPos[2] << endl;
      cout << "trkid = " << trkid << endl; 
      cout << "parentid = " << parentid << endl; 
      cout << "charge = " << charge << endl; 
      cout << "time = " << time << endl; 
      cout << "steplength = " << steplength << endl; 
      cout << "edep = " << edep << endl; 
      cout << "detname = " << detname << endl; 
      */
      ApplyResponse.CalcResponse(lightPos,trkid,parentid,charge,time,steplength,edep,detname);
      //  cout << "calculated response..." << endl;


      double pex = ApplyResponse.GetHitPE().x();
      double pey = ApplyResponse.GetHitPE().y();
      double pez = ApplyResponse.GetHitPE().z();

      double poshitX = ApplyResponse.GetHitPos().x();
      double poshitY = ApplyResponse.GetHitPos().y();
      double poshitZ = ApplyResponse.GetHitPos().z();

      trkid = ApplyResponse.GetHitTrkID();

      //                
      // int particleID = pdg;
  

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
	//cout << trkid << ", "<< pdg<<  endl;
        fRecoTrack_ID.push_back(trkid);
        fRecoTrack_PDG.push_back(pdg);
        fRecoTrack_ParentID.push_back(parentid);

        fRecoTrack_true_length.push_back(0.); // initialized. Sum of hit lenght later
        fRecoTrack_true_edep.push_back(0.); // initialized. Sum of hit lenght later

        fRecoTrack_MPPCHit_XY.push_back((TH2F*)h2d_xy->Clone("h2d_xy"));
        fRecoTrack_MPPCHit_XZ.push_back((TH2F*)h2d_xz->Clone("h2d_xz"));
        fRecoTrack_MPPCHit_YZ.push_back((TH2F*)h2d_yz->Clone("h2d_yz"));

        vector<double> singleHitXY; // get hit (x,y,pez)
	singleHitXY.push_back(trkid);
	singleHitXY.push_back(pdg);
        singleHitXY.push_back(poshitX);
        singleHitXY.push_back(poshitY);
        singleHitXY.push_back(pez);
        vector< vector<double> > hitsXY; // add hit to hit list
        hitsXY.push_back(singleHitXY);
        fRecoTrack_hitXY.push_back(hitsXY); // add hit list to trk (1 for each trk)

	vector<double> singleHitXZ; // get hit (x,z,pey)
	singleHitXZ.push_back(trkid);
	singleHitXZ.push_back(pdg);
	singleHitXZ.push_back(poshitX);
        singleHitXZ.push_back(poshitZ);
        singleHitXZ.push_back(pey);
        vector< vector<double> > hitsXZ; // add hit to hit list
        hitsXZ.push_back(singleHitXZ);
        fRecoTrack_hitXZ.push_back(hitsXZ); // add hit list to trk (1 for each trk)

	vector<double> singleHitYZ; // get hit (y,z,pex)
	singleHitYZ.push_back(trkid);
	singleHitYZ.push_back(pdg);
        singleHitYZ.push_back(poshitY);
        singleHitYZ.push_back(poshitZ);
        singleHitYZ.push_back(pex);
        vector< vector<double> > hitsYZ; // add hit to hit list
        hitsYZ.push_back(singleHitYZ);
        fRecoTrack_hitYZ.push_back(hitsYZ); // add hit list to trk (1 for each trk)
      }

      // cout << "find index..." <<endl;
      // Find vector index of the track
      int idx = FindIndex(fRecoTrack_ID,trkid);
      // cout << "idx = " << idx << endl;
      // Add hits to the track vector
      if(used){ // the first hit is already added
        vector<double> infoXY; // get info of hit (x,y,pe)
	infoXY.push_back(trkid);
	infoXY.push_back(pdg);
        infoXY.push_back(poshitX);
        infoXY.push_back(poshitY);
        infoXY.push_back(pez);
        fRecoTrack_hitXY[idx].push_back(infoXY);

        vector<double> infoXZ; // get info of hit (x,z,pe)
	infoXZ.push_back(trkid);
	infoXZ.push_back(pdg);
        infoXZ.push_back(poshitX);
        infoXZ.push_back(poshitZ);
        infoXZ.push_back(pey);
        fRecoTrack_hitXZ[idx].push_back(infoXZ);

        vector<double> infoYZ; // get info of hit (y,z,pe)
	infoYZ.push_back(trkid);
	infoYZ.push_back(pdg);
        infoYZ.push_back(poshitY);
        infoYZ.push_back(poshitZ);
        infoYZ.push_back(pex);
        fRecoTrack_hitYZ[idx].push_back(infoYZ);
      }
      
      fRecoTrack_true_length[idx] += length_true_mchit; // initialized to 0 at first mc hit ( if(!used) )
      fRecoTrack_true_edep[idx]   += edep; // initialized to 0 at first mc hit ( if(!used) )

      // Fill the event display
      fRecoTrack_MPPCHit_XY[idx]->Fill(poshitX,poshitY,pez); // pe along Z
      fRecoTrack_MPPCHit_XZ[idx]->Fill(poshitX,poshitZ,pey); // pe along Y
      fRecoTrack_MPPCHit_YZ[idx]->Fill(poshitY,poshitZ,pex); // pe along X

      // Update the previous position
      posX_prev  = posX;
      posY_prev  = posY;
      posZ_prev  = posZ;
      trkid_prev = trkid;
    } // end loop over the hits
  


    // FILL HITS MAP
    TH2F* hits_map_XY = (TH2F*)h2d_xy->Clone("hits_map_XY");
    TH2F* hits_map_XZ = (TH2F*)h2d_xz->Clone("hits_map_XZ");
    TH2F* hits_map_YZ = (TH2F*)h2d_yz->Clone("hits_map_YZ");

    cout << "# of RecoTrack: " << fRecoTrack_ID.size()<<endl;
    NRecoTracks = fRecoTrack_ID.size();

    for(unsigned int itrk=0;itrk<fRecoTrack_ID.size();itrk++){
      particle_id[itrk] = fRecoTrack_PDG[itrk];
      /*cout << fRecoTrack_ID[itrk] <<endl;
	cout << fRecoTrack_PDG[itrk] << endl;
	cout << fRecoTrack_ParentID[itrk] << endl;
      */
      }

  
    if (fRecoTrack_ID.size() <= 0) {
      cout << "No RecoTrack!" << endl;
      continue;
    }      
    for(unsigned int itrk=0;itrk<fRecoTrack_ID.size();itrk++){
      hits_map_XY->Add(fRecoTrack_MPPCHit_XY[itrk]); // pe along Z
      hits_map_XZ->Add(fRecoTrack_MPPCHit_XZ[itrk]); // pe along Y
      hits_map_YZ->Add(fRecoTrack_MPPCHit_YZ[itrk]); // pe along X
      // cout << "adding hit map: " << itrk << endl;
    }
    hMPPCHits_XY[ievt]->Add(hits_map_XY); // pe along Z
    hMPPCHits_XZ[ievt]->Add(hits_map_XZ); // pe along Y
    hMPPCHits_YZ[ievt]->Add(hits_map_YZ); // pe along X

    passed_evt.push_back(ievt);

    outtree->Fill();     
    delete nd280UpEvent;    
    nd280UpEvent = 0;
} // end loop over events

// Write output file
TString outfilename = TString::Format("%s_Evt%d_NEvt%d.root",tag.c_str(),evtfirst,nevents);
TFile *out = new TFile(outfilename.Data(),"RECREATE");

outtree->Write();

 for (int i=0;i<passed_evt.size();i++){
  hMPPCHits_XY[passed_evt[i]]->Write();
  hMPPCHits_XZ[passed_evt[i]]->Write();
  hMPPCHits_YZ[passed_evt[i]]->Write();	  
 }


out->Close();

return 0;
}


