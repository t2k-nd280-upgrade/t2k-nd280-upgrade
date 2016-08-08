
// Vertex cut (inside the Target)
const bool doCut_Vtx = true;
const double vtx_min_x_1 = -1150;  //mm
const double vtx_max_x_1 = +1150;  //mm
const double vtx_min_y_1 = -300; //mm
const double vtx_max_y_1 = +300; //mm
const double vtx_min_z_1 = +487;  //mm
const double vtx_max_z_1 = +2487;  //mm

const double vtx_min_x_2 = -1150;  //mm
const double vtx_max_x_2 = +1150;  //mm
const double vtx_min_y_2 = -300; //mm
const double vtx_max_y_2 = +300; //mm
const double vtx_min_z_2 = -2487;  //mm
const double vtx_max_z_2 = -487;  //mm

void TestND280UpEvent(){

  gROOT->ProcessLine(".L /Users/davidesgalaberna/Desktop/GENEVA_postdoc/CODE/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.cc+");
  gROOT->ProcessLine(".L /Users/davidesgalaberna/Desktop/GENEVA_postdoc/CODE/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.cc+");
  gROOT->ProcessLine(".L /Users/davidesgalaberna/Desktop/GENEVA_postdoc/CODE/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.cc+");
  gROOT->ProcessLine(".L /Users/davidesgalaberna/Desktop/GENEVA_postdoc/CODE/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.cc+");

  // gROOT->ProcessLine(".L /afs/cern.ch/user/d/dsgalabe/T2K/work/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.cc+");
  // gROOT->ProcessLine(".L /afs/cern.ch/user/d/dsgalabe/T2K/work/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.cc+");
  // gROOT->ProcessLine(".L /afs/cern.ch/user/d/dsgalabe/T2K/work/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.cc+");
  // gROOT->ProcessLine(".L /afs/cern.ch/user/d/dsgalabe/T2K/work/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.cc+");

  // Vertex
  TH2D *hVtx_XY = new TH2D("hVtx_XY","hVtx_XY",60,-3000,3000,60,-3000,3000); // mm
  //TH2D *hVtx_XZ = new TH2D("hVtx_XZ","hVtx_XZ",600,-6000,6000,600,-300,300); // mm
  //TH2D *hVtx_YZ = new TH2D("hVtx_YZ","hVtx_YZ",600,-6000,6000,600,-300,300); // mm
  TH1D *hVtx_X = new TH1D("hVtx_X","hVtx_X",600,-3000,3000); // mm
  TH1D *hVtx_Y = new TH1D("hVtx_Y","hVtx_Y",600,-300,300); // mm
  TH1D *hVtx_Z = new TH1D("hVtx_Z","hVtx_Z",600,-3000,3000); // mm
  
  //

  TFile *finput = new TFile("../../../G4FILES/ND280upgrade.root","READ");
  TTree *tinput = (TTree*) finput->Get("ND280upEvents");
  
  TND280UpEvent *nd280UpEvent = new TND280UpEvent();
  
  tinput->SetBranchAddress("Event",&nd280UpEvent);

  int Nentries = tinput->GetEntries();
  cout << "# of events = " << Nentries << endl;
  
  for(int ievt=0;ievt<Nentries;ievt++){
    
    tinput->GetEntry(ievt);
    
    //nd280UpEvent->PrintEvent();
    
    int NVertices = nd280UpEvent->GetNVertices();
    for(int ivtx=0;ivtx<NVertices;ivtx++){
      TND280UpVertex *nd280UpVertex = dynamic_cast<TND280UpVertex*>(nd280UpEvent->GetVertex(ivtx));
      
      double x = nd280UpVertex->GetPosition().X();
      double y = nd280UpVertex->GetPosition().Y();
      double z = nd280UpVertex->GetPosition().Z();

      if(doCut_Vtx){
	if((x < vtx_min_x_1 || x > vtx_max_x_1) && (x < vtx_min_x_2 || x > vtx_max_x_2)){
	  hVtx_XY->Fill(x,y);
	  hVtx_X->Fill(x);
	  hVtx_Y->Fill(y);
	  hVtx_Z->Fill(z);
       	  continue;
	}
	if((y < vtx_min_y_1 || y > vtx_max_y_1) && (y < vtx_min_y_2 || y > vtx_max_y_2)){
	  hVtx_XY->Fill(x,y);
	  hVtx_X->Fill(x);
	  hVtx_Y->Fill(y);
	  hVtx_Z->Fill(z);
	  continue;
	}
	if((z < vtx_min_z_1 || z > vtx_max_z_1) && (z < vtx_min_z_2 || z > vtx_max_z_2)){
	  hVtx_XY->Fill(x,y);
	  hVtx_X->Fill(x);
	  hVtx_Y->Fill(y);
	  hVtx_Z->Fill(z);
	  continue;
	}
      }

      //nd280UpVertex->PrintVertex();
    }

    int NTracks = nd280UpEvent->GetNTracks();

    for(int itrk=0;itrk<NTracks;itrk++){
      TND280UpTrack *nd280UpTrack = dynamic_cast<TND280UpTrack*>(nd280UpEvent->GetTrack(itrk));

      //if(nd280UpTrack->GetTrackID()!=5) continue;
      
      //nd280UpTrack->PrintTrack();
      
      double sdedep = 0.;
      double sdlength = 0.;
      
      int NPoints = nd280UpTrack->GetNPoints();
      for(int ipt=0;ipt<NPoints;ipt++){
	
	TND280UpTrackPoint *nd280UpTrackPoint = 
	  dynamic_cast<TND280UpTrackPoint*>(nd280UpTrack->GetPoint(ipt));	  
	
	//nd280UpTrackPoint->PrintTrackPoint();
	
	string detname = nd280UpTrackPoint->GetPhysVolName();
	double stepedep = nd280UpTrackPoint->GetEdep();
	double steplength = nd280UpTrackPoint->GetStepLength();
	
	if(1){
	  //if(detname=="ForwTPC1/Half" ||
	   // detname=="ForwTPC2/Half" ||
	   // detname=="ForwTPC3/Half" ||
	   // detname=="Target1" ||
	   // detname=="Target2" ||
	   // detname=="TPCUp1" ||
	   // detname=="TPCDown1" ||
	   // detname=="TPCUp2" ||
	   // detname=="TPCDown2"){
	  
	  sdedep += stepedep;
	  sdlength += steplength;
	  
	  // if(stepedep>0.00000001){	
	  //   cout << "ID: " << nd280UpTrackPoint->GetPointID() << ": " 
	  // 	 << detname
	  // 	 << " --> edep = " << nd280UpTrackPoint->GetEdep() << endl;
	  // }
	}
	
      } // end loop over points
      
      // if(sdedep!=nd280UpTrack->GetSDTotalEnergyDeposit()){
      //  	cout << endl;
      //  	//nd280UpTrack->PrintTrack();
      //  	cout << "nd280UpTrack->GetSDTotalEnergyDeposit() = " 
      // 	     << nd280UpTrack->GetSDTotalEnergyDeposit() << endl;
      // 	cout << "sdedep = " << sdedep << endl;
      //  	cout << endl;
      // }

      // if(sdlength!=nd280UpTrack->GetSDLength()){
      // 	cout << endl;
      // 	nd280UpTrack->PrintTrack();
      // 	cout << "nd280UpTrack->GetSDLength() = " << nd280UpTrack->GetSDLength() << endl;
      // 	cout << "sdlength = " << sdlength << endl;
      // 	cout << endl;
      // }
      
    } // end loop over tracks
  } // end loop over events
  

  // TCanvas *cVtx_X = new TCanvas("cVtx_X","cVtx_X");
  // hVtx_X->GetXaxis()->SetTitle("X (mm)");
  // hVtx_X->Draw();
  // TCanvas *cVtx_Y = new TCanvas("cVtx_Y","cVtx_Y");
  // hVtx_Y->GetXaxis()->SetTitle("Y (mm)");
  // hVtx_Y->Draw();
  // TCanvas *cVtx_Z = new TCanvas("cVtx_Z","cVtx_Z");
  // hVtx_Z->GetXaxis()->SetTitle("Z (mm)");
  // hVtx_Z->Draw();
  // TCanvas *cVtx_XY = new TCanvas("cVtx_XY","cVtx_XY");
  // hVtx_XY->GetXaxis()->SetTitle("X (mm)");
  // hVtx_XY->GetYaxis()->SetTitle("Y (mm)");
  // hVtx_XY->Draw("colz");
  
  TFile *out = new TFile("outfile.root","RECREATE");
  hVtx_X->Write();
  hVtx_Y->Write();
  hVtx_Z->Write();
  hVtx_XY->Write();
  
  

}
