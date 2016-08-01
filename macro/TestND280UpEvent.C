
void TestND280UpEvent(){

  gROOT->ProcessLine(".L /afs/cern.ch/user/d/dsgalabe/T2K/work/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.cc+");
  gROOT->ProcessLine(".L /afs/cern.ch/user/d/dsgalabe/T2K/work/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.cc+");
  gROOT->ProcessLine(".L /afs/cern.ch/user/d/dsgalabe/T2K/work/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.cc+");
  
  TFile *finput = new TFile("../bin/ND280upgrade.root","READ");
  TTree *tinput = (TTree*) finput->Get("ND280upEvents");
  
  TND280UpEvent *nd280UpEvent = new TND280UpEvent();
  
  tinput->SetBranchAddress("Event",&nd280UpEvent);

  int Nentries = tinput->GetEntries();
  cout << "Nentries = " << Nentries << endl;
  
  for(int ievt=0;ievt<Nentries;ievt++){
    
    tinput->GetEntry(ievt);
    
    cout << "Evt ID: " << nd280UpEvent->GetEventID() << endl;
    cout << "# of tracks: " << nd280UpEvent->GetNTracks() << endl;
    
    nd280UpEvent->PrintEvent();
    
    int NTracks = nd280UpEvent->GetNTracks();
    for(int itrk=0;itrk<NTracks;itrk++){
	TND280UpTrack *nd280UpTrack = dynamic_cast<TND280UpTrack*>(nd280UpEvent->GetTrack(itrk));
	
	nd280UpTrack->PrintTrack();
	
	int NPoints = nd280UpTrack->GetNPoints();
	for(int ipt=0;ipt<NPoints;ipt++){
	  
	  TND280UpTrackPoint *nd280UpTrackPoint = 
	    dynamic_cast<TND280UpTrackPoint*>(nd280UpTrack->GetPoint(ipt));	  
	nd280UpTrackPoint->PrintTrackPoint();
	
	} // end loop over points    
    } // end loop over tracks
  } // end loop over events

}
