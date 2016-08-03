
void TestND280UpEvent(){

  gROOT->ProcessLine(".L /afs/cern.ch/user/d/dsgalabe/T2K/work/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.cc+");
  gROOT->ProcessLine(".L /afs/cern.ch/user/d/dsgalabe/T2K/work/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.cc+");
  gROOT->ProcessLine(".L /afs/cern.ch/user/d/dsgalabe/T2K/work/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.cc+");
  gROOT->ProcessLine(".L /afs/cern.ch/user/d/dsgalabe/T2K/work/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.cc+");
  
  TFile *finput = new TFile("../bin/ND280upgrade.root","READ");
  TTree *tinput = (TTree*) finput->Get("ND280upEvents");
  
  TND280UpEvent *nd280UpEvent = new TND280UpEvent();
  
  tinput->SetBranchAddress("Event",&nd280UpEvent);

  int Nentries = tinput->GetEntries();
  cout << "# of events = " << Nentries << endl;
  
  for(int ievt=0;ievt<Nentries;ievt++){
    
    tinput->GetEntry(ievt);
    
    nd280UpEvent->PrintEvent();
    
    int NVertices = nd280UpEvent->GetNVertices();
    for(int ivtx=0;ivtx<NVertices;ivtx++){
      TND280UpVertex *nd280UpVertex = dynamic_cast<TND280UpVertex*>(nd280UpEvent->GetVertex(ivtx));
      nd280UpVertex->PrintVertex();
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

}
