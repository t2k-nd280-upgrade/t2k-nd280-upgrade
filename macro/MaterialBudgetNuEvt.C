  //Cesar Jesus-Valls. 6/7/2018:
  //Macro to study the Field Cage prototypes. It analyses the data of a beam of different particles (e,mu,pi,p) crossing a multilayer wall 
  //simulated with GEANT4 in the nd280-upgrade code. This macro is though to be run with a perl script inside nd280-upgrade-master/submit 
  //named 'submit_nd280upgrade_fcw' and can be run using: 
  // $T2KND280UP/submit/submit_nd280upgrade_fcw --softw-dir $T2KND280UP --job-dir $PWD --resource interactive --inputfile $T2KND280UP/macro/Cesar/FCStudies/Data/Solid500k.root --nexpts 500000 --nruns 1 
  //before running it, it is necessary to export the $T2KND280UP! If someone needs help, please contact to cjesus@ifae.es

  #include <TH1D.h>
  #include <TH2D.h>
  #include <TROOT.h>
  #include <TTree.h>
  #include <TFile.h>
  #include <TMath.h>
  #include <TStyle.h>
  #include <TLatex.h>
  #include <TCanvas.h>
  #include <TH3.h>
  #include <TPaveText.h>
  #include <TEfficiency.h>
  #include <TProfile2D.h>
  #include <TProfile.h>
  #include <TGraphAsymmErrors.h>

  //Include the special classes TrackPoint, Track, Vertx, and Event in which the output information is encoded.
  #include "/software/neutrinos/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.hh"
  #include "/software/neutrinos/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.hh"
  #include "/software/neutrinos/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.hh"
  #include "/software/neutrinos/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.hh"

  void MaterialBudgetNuEvt
  (
  //Initialize the variables fixed in the submit script
   const int evtfirst = 0,
   const int nevents = 1,
   string tag = "",
   string infilename = ""
  )
  {
  //Generate the Canvas and set the aesthetics
     TCanvas *c1 = new TCanvas("c1","c1",1200,900);
     TCanvas *c2 = new TCanvas("c2","c2",1200,900);
     TCanvas *c3 = new TCanvas("c3","c3",1200,900);
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
     c1->Divide(4,3); // Mom at 0,30,60 deg for e,mu,pi,pr
     c2->Divide(4,3); // Ang at 0,30,60 deg for e,mu,pi,pr
     c3->Divide(4,3); // Stop at 0,30,60 deg for e,mu,pi,pr

  // Basic Histograms to check the correct uniformity of the flat random pgun. Currently not used.
    TH1D *hAng = new TH1D("hAng","hAng",316,-3.15,3.15);

    TH1D *hdirX = new TH1D("dirx","dirx",100,-1,1);
    TH1D *hdirY = new TH1D("diry","diry",100,-1,1);
    TH1D *hdirZ = new TH1D("dirz","dirz",100,-1,1);  

    TH1D *hAngDevMu = new TH1D("hAngDevMu","hAngDevMu",200,-3,3);
    TH1D *hAngDevEl = new TH1D("hAngDevEl","hAngDevEl",200,-3,3);
    TH1D *hAngDevPi = new TH1D("hAngDevPi","hAngDevPi",200,-3,3);
    TH1D *hAngDevPr = new TH1D("hAngDevPr","hAngDevPr",200,-3,3);

  // Variables to specify histograms binning. It is built in such a way that 'bin surface' is the same for all histograms. Notice this implies different number of bins in each histogram.
    int ElIni = 0;
    int ElFin = 300;
    int ElY = 30;
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

  //Histograms definition. "Ang" are related with Angluar spreading. "Cnt" are counter for normalitzation. "Mom" are realted with Momentum.
    // TH3D *hMomEl3D = new TH3D("hMomEl3D","hMomEl3D",20,-75,75,ElY,ElIni,ElFin,50,0,100);
    // TH3D *hMomMu3D = new TH3D("hMomMu3D","hMomMu3D",20,-75,75,MuY,MuIni,MuFin,50,0,100);
    // TH3D *hMomPi3D = new TH3D("hMomPi3D","hMomPi3D",20,-75,75,PiY,PiIni,PiFin,50,0,100);
    // TH3D *hMomPr3D = new TH3D("hMomPr3D","hMomPr3D",20,-75,75,PrY,PrIni,PrFin,50,0,100);

    TH3D *hMomEl3D = new TH3D("hMomEl3D","hMomEl3D",20,-1,1,ElY,ElIni,ElFin,50,0,100);
    TH3D *hMomMu3D = new TH3D("hMomMu3D","hMomMu3D",20,-1,1,MuY,MuIni,MuFin,50,0,100);
    TH3D *hMomPi3D = new TH3D("hMomPi3D","hMomPi3D",20,-1,1,PiY,PiIni,PiFin,50,0,100);
    TH3D *hMomPr3D = new TH3D("hMomPr3D","hMomPr3D",20,-1,1,PrY,PrIni,PrFin,50,0,100);    

    TH3D *hAngEl3D = new TH3D("hAngEl3D","hAngEl3D",20,-1,1,ElY,ElIni,ElFin,50,0,100);
    TH3D *hAngMu3D = new TH3D("hAngMu3D","hAngMu3D",20,-1,1,MuY,MuIni,MuFin,50,0,100);
    TH3D *hAngPi3D = new TH3D("hAngPi3D","hAngPi3D",20,-1,1,PiY,PiIni,PiFin,50,0,100);
    TH3D *hAngPr3D = new TH3D("hAngPr3D","hAngPr3D",20,-1,1,PrY,PrIni,PrFin,50,0,100);

    TEfficiency* EffEl0 = new TEfficiency("EffEl0","EffEl0",ElY,ElIni,ElFin);
    TEfficiency* EffMu0 = new TEfficiency("EffMu0","EffMu0",MuY,MuIni,MuFin);
    TEfficiency* EffPi0 = new TEfficiency("EffPi0","EffPi0",PiY,PiIni,PiFin);
    TEfficiency* EffPr0 = new TEfficiency("EffPr0","EffPr0",PrY,PrIni,PrFin);

    TEfficiency* EffEl30 = new TEfficiency("EffEl30","EffEl30",ElY,ElIni,ElFin);
    TEfficiency* EffMu30 = new TEfficiency("EffMu30","EffMu30",MuY,MuIni,MuFin);
    TEfficiency* EffPi30 = new TEfficiency("EffPi30","EffPi30",PiY,PiIni,PiFin);
    TEfficiency* EffPr30 = new TEfficiency("EffPr30","EffPr30",PrY,PrIni,PrFin);

    TEfficiency* EffEl60 = new TEfficiency("EffEl60","EffEl60",ElY,ElIni,ElFin);
    TEfficiency* EffMu60 = new TEfficiency("EffMu60","EffMu60",MuY,MuIni,MuFin);
    TEfficiency* EffPi60 = new TEfficiency("EffPi60","EffPi60",PiY,PiIni,PiFin);
    TEfficiency* EffPr60 = new TEfficiency("EffPr60","EffPr60",PrY,PrIni,PrFin);

    TEfficiency* EffEl75 = new TEfficiency("EffEl75","EffEl75",ElY,ElIni,ElFin);
    TEfficiency* EffMu75 = new TEfficiency("EffMu75","EffMu75",MuY,MuIni,MuFin);
    TEfficiency* EffPi75 = new TEfficiency("EffPi75","EffPi75",PiY,PiIni,PiFin);
    TEfficiency* EffPr75 = new TEfficiency("EffPr75","EffPr75",PrY,PrIni,PrFin);

    TH1F *hSpeMu200MeV0deg = new TH1F("hSpeMu200MeV0deg","Similarity Study",100,0,10);

    bool bPassed;

    TFile *finput = new TFile(infilename.c_str(),"READ");
    TTree *tinput = (TTree*) finput->Get("ND280upEvents");
    
    TND280UpEvent *nd280UpEvent = new TND280UpEvent();  
    tinput->SetBranchAddress("Event",&nd280UpEvent);
    
    int NTreeEntries =  tinput->GetEntries();  
    int evtlasttree = NTreeEntries-1;
    int Nentries = -999;

    string FCname = "FC";

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

    int cntTotal=0;
    int cntE=0;
    int EntryLast = evtfirst+Nentries-1;
    
    cout << endl;
    cout << "First event = " << evtfirst << endl;
    cout << "Last event = " << EntryLast << endl;
    cout << "# of events = " << Nentries << endl;
    cout << endl;

  //Loop over events.
    for(int ievt=evtfirst;ievt<=EntryLast;ievt++)
  {
  cntTotal++;

  //if(ievt>10) linput->Reset();
  TTree *linput = (TTree*) finput->Get("ND280upEvents");

  linput->SetBranchAddress("Event",&nd280UpEvent);

  linput->GetEntry(ievt);

 



  if(!(ievt%10000)){
    cout << "Event " << ievt << endl; // Message for large files: Print the analysis status every 10000 analyzed events.    
 }

//cout << endl << endl << "NEW EVENT" << endl << endl;

  //Loop over Tracks (per each events).
  int NTracks = nd280UpEvent->GetNTracks();
  for(int itrk=0;itrk<NTracks;itrk++)
  {

// cout << endl << "NEW Track" << endl << endl;  


    TND280UpTrack *nd280UpTrack = nd280UpEvent->GetTrack(itrk);
    
    int trkID = nd280UpTrack->GetTrackID();
    int parentID = nd280UpTrack->GetParentID();
    int pdg = nd280UpTrack->GetPDG();
    double charge = nd280UpTrack->GetCharge();
    double ekin = nd280UpTrack->GetInitKinEnergy();
    double mom = nd280UpTrack->GetInitMom().Mag(); 

    double dirX = nd280UpTrack->GetInitMom().X() / mom;
    double dirY = nd280UpTrack->GetInitMom().Y() / mom;
    double dirZ = nd280UpTrack->GetInitMom().Z() / mom;
    double phi = atan2(dirY,dirZ);

    double cosphi = cos(phi);

    hAng->Fill(phi);
    hdirX->Fill(cosphi);
    hdirY->Fill(dirY);
    hdirZ->Fill(dirZ);

   // if(trkID != 2) break;    

 //  cout << "trkID is: " << trkID << endl;
 //  cout << "pdg is: " << pdg << endl; 

  //  phi = abs(phi);   

  // Note: (pdg) Electron (11) // Muon (13) // Pion- (-211) // proton (2212) 
  //Loop over points (per each track in each event).
    int NPoints = nd280UpTrack->GetNPoints();
    int cnt =0;
    int cnt1 =0;
    double MomIni = -1; //To ensure no double counting, or repetition errors
    double MomFin = -1; //To ensure no double counting, or repetition errors

  for(int ipt=0;ipt<NPoints;ipt++)
  {	

    TND280UpTrackPoint *nd280UpTrackPoint = nd280UpTrack->GetPoint(ipt);	
   	double MomX = nd280UpTrackPoint->GetMomentum().X();
   	double MomY = nd280UpTrackPoint->GetMomentum().Y();
   	double MomZ = nd280UpTrackPoint->GetMomentum().Z();     
    double MomMod = nd280UpTrackPoint->GetMomentum().Mag();  	
    double MomVec[] = {MomX/MomMod,MomY/MomMod,MomZ/MomMod};
    double phi2=atan2(MomVec[1],MomVec[2]);
    string volname = nd280UpTrackPoint->GetLogVolName();

   // phi2 = abs(phi2);

    //int b_cnt =0;

    if(ipt==0 && volname != "/t2k/OA/Magnet/Basket/target1/CFBox1/TargetUniform") break;
    if(ipt==1 && volname != "/t2k/OA/Magnet/Basket/target1/CFBox1/TargetUniform") break;
    if(ipt==2 && volname == "/t2k/OA/Magnet/Basket/target1/CFBox1/TargetUniform") break;
    if(volname.find("ToF") != std::string::npos) break;
    if(volname.find("LeftClam") != std::string::npos) break;
    if(volname.find("RightClam") != std::string::npos) break;
    if(volname.find("OutOfWorld") != std::string::npos) break;
    if(volname == "/t2k/OA/Magnet/Basket") break;

   if(ipt==2 && volname == "/t2k/OA/Magnet/Basket") break;
   if(ipt==2 && volname == "/t2k") break;
   if(ipt==2 && volname == "/t2k/OA") break;
   if(ipt==2 && volname == "/t2k/OA/Magnet") break;
   if(ipt==2 && volname == "OutOfWorld") break;
   if(ipt==2 && volname == "/t2k/OA/Magnet/LeftClam") break;
   if(ipt==2 && volname == "/t2k/OA/Magnet/RightClam") break;
    
    // if(ipt==2 && volname == "/t2k/OA/Magnet/Basket/FC/HATPCDown/Drift/Half0") b_cnt++;
    // if(ipt==2 && volname == "/t2k/OA/Magnet/Basket/FC/HATPCDown/Drift/Half1") b_cnt++;
    // if(ipt==2 && volname == "/t2k/OA/Magnet/Basket/FC/HATPCUp/Drift/Half0") b_cnt++;
    // if(ipt==2 && volname == "/t2k/OA/Magnet/Basket/FC/HATPCUp/Drift/Half1") b_cnt++;

    //  if(ipt ==2 && b_cnt==0) break;

//if(ipt >1 && volname.find("Drift") == std::string::npos) cout << "volname is: " << volname << "  |  ipt " << ipt <<endl;


 //cout << "volname is: " << volname << "  |  ipt " << ipt <<endl;
   
  // // Conditions to select the plot's .pdf file names.
  //    if(volname== "/t2k/OA/Magnet/Basket/HATPCUp/FC/layer8") {FCname = "old"; break;}
  //    if(volname== "/t2k/OA/Magnet/Basket/HATPCUp/FCNexus/layer8") FCname = "Nexus";
  //    if(volname== "/t2k/OA/Magnet/Basket/HATPCUp/FCGF/layer7") FCname = "GF";
  //    if(volname== "/t2k/OA/Magnet/Basket/HATPCUp/FCSolid/layer5") FCname = "Solid";

  // Condition to store information before entering in the FC
//     if(volname == "TargetUniform" && trkID ==1)
     if(volname == "/t2k/OA/Magnet/Basket/target1/CFBox1/TargetUniform" &&  ipt==1)
     {    
     if(cnt1 == 0) // Only enters the first time (per event) it is in "/t2k/OA/Magnet/Basket" to avoid double counting.
     {
     MomIni = MomMod;
     }
     cnt1++;
     }
     
     // cout << "cosphi: " << cosphi << endl;
     // cout << "phi: " << phi*360/(2*3.141592) << endl;
     // cout << "phi2: " << phi2*360/(2*3.141592) << endl;
     // cout << "abs(phi2-phi): " << abs(phi2-phi)*360/(2*3.141592) << endl;



// Condition to store information after entering in the FC
//     if(volname == "/t2k/OA/Magnet/Basket/HATPCUp/Drift/Half1" && cnt1 == 1 && trkID ==1) // (cnt1 == 1) is important. It ensures that any event is stored if we don't have both inital and final information.
     if(ipt==2 && volname != "/t2k/OA/Magnet/Basket/target1/CFBox1/TargetUniform" && cnt1 == 1) 
     {
        MomFin = MomMod;
        if(cnt == 0)
          {

// The idea is: EXPLANATION!!!!
          // if(pdg ==11)
          // {
          // hMomEl3D->Fill(phi*360/(2*3.141592),MomIni,100*MomFin/MomIni);
          // hAngEl3D->Fill(phi*360/(2*3.141592),MomIni,abs((phi2-phi)*360/(2*3.141592)));
          // }
          // if(pdg ==13)
          // {
          // hMomMu3D->Fill(phi*360/(2*3.141592),MomIni,100*MomFin/MomIni);
          // hAngMu3D->Fill(phi*360/(2*3.141592),MomIni,abs((phi2-phi)*360/(2*3.141592)));
          // hSpeMu200MeV0deg->Fill(MomIni-MomFin);
          // }
          // if(pdg == -211 || pdg == 211)
          // {
          // hMomPi3D->Fill(phi*360/(2*3.141592),MomIni,100*MomFin/MomIni);
          // hAngPi3D->Fill(phi*360/(2*3.141592),MomIni,abs((phi2-phi)*360/(2*3.141592)));
          // }
          // if(pdg ==2212)
          // {
          // hMomPr3D->Fill(phi*360/(2*3.141592),MomIni,100*MomFin/MomIni);
          // hAngPr3D->Fill(phi*360/(2*3.141592),MomIni,abs((phi2-phi)*360/(2*3.141592)));
          // }

          if(pdg ==11)
          {
          hMomEl3D->Fill(cosphi,MomIni,100*MomFin/MomIni);
          if(phi>0)hAngEl3D->Fill(cosphi,MomIni,abs((phi2-phi)*360/(2*3.141592)));
          //hAngEl3D->Fill(cosphi,MomIni,(abs(phi2)-abs(phi))*360/(2*3.141592));
          }
          if(pdg ==13)
          {
          hMomMu3D->Fill(cosphi,MomIni,100*MomFin/MomIni);
          if(phi>0)hAngMu3D->Fill(cosphi,MomIni,abs((phi2-phi)*360/(2*3.141592)));
          //if(phi>0)hAngMu3D->Fill(cosphi,MomIni,(abs(phi2)-abs(phi))*360/(2*3.141592));
          hSpeMu200MeV0deg->Fill(MomIni-MomFin);
          }
          if(pdg == -211)
          {
          hMomPi3D->Fill(cosphi,MomIni,100*MomFin/MomIni);
          if(phi>0)hAngPi3D->Fill(cosphi,MomIni,abs((phi2-phi)*360/(2*3.141592)));
          //if(phi>0)hAngPi3D->Fill(cosphi,MomIni,(abs(phi2)-abs(phi))*360/(2*3.141592));
          }
          if(pdg ==2212)
          {
          hMomPr3D->Fill(cosphi,MomIni,100*MomFin/MomIni);
          if(phi>0)hAngPr3D->Fill(cosphi,MomIni,abs((phi2-phi)*360/(2*3.141592)));
          //if(phi>0)hAngPr3D->Fill(cosphi,MomIni,(abs(phi2)-abs(phi))*360/(2*3.141592));
          }


          }      
     if(volname.find("Drift") != std::string::npos) cnt++;     //To avoid double counting, only 1 Fill in Half1 layer per event.
     //ipt = NPoints-1;
     }

     int AngWindow = 5; //Actually the real window is 2 times this

        if(ipt == 2 && cnt == 0){ 
          bPassed = false;
          if( abs(phi*360/(2*3.141592)) >= 90-AngWindow && abs(phi*360/(2*3.141592)) <= 90+AngWindow)
          {
          if(pdg ==11) EffEl0->Fill(bPassed,MomIni);
          if(pdg ==13) EffMu0->Fill(bPassed,MomIni);
          if(pdg ==-211)  EffPi0->Fill(bPassed,MomIni);
          if(pdg ==2212) EffPr0->Fill(bPassed,MomIni);
          }
          if(abs(phi*360/(2*3.141592)) >= (90-30-AngWindow) && abs(phi*360/(2*3.141592)) <= (90-30+AngWindow))
          {
          if(pdg ==11) EffEl30->Fill(bPassed,MomIni);
          if(pdg ==13) EffMu30->Fill(bPassed,MomIni);
          if(pdg ==-211)  EffPi30->Fill(bPassed,MomIni);
          if(pdg ==2212) EffPr30->Fill(bPassed,MomIni);
          }
          if(abs(phi*360/(2*3.141592)) >= (90-60-AngWindow) && abs(phi*360/(2*3.141592)) <= (90-60+AngWindow))
          {
          if(pdg ==11) EffEl60->Fill(bPassed,MomIni);
          if(pdg ==13) EffMu60->Fill(bPassed,MomIni);
          if(pdg ==-211)  EffPi60->Fill(bPassed,MomIni);
          if(pdg ==2212) EffPr60->Fill(bPassed,MomIni);
          }
          if(abs(phi*360/(2*3.141592)) >= (90-75-AngWindow) && abs(phi*360/(2*3.141592)) <= (90-75+AngWindow))
          {
          if(pdg ==11) EffEl60->Fill(bPassed,MomIni);
          if(pdg ==13) EffMu60->Fill(bPassed,MomIni);
          if(pdg ==-211)  EffPi60->Fill(bPassed,MomIni);
          if(pdg ==2212) EffPr60->Fill(bPassed,MomIni);
          }
        }

        if(ipt == 2 && cnt >= 1)
        { 
          bPassed = true;
          if( abs(phi*360/(2*3.141592)) >= 90-AngWindow && abs(phi*360/(2*3.141592)) <= 90+AngWindow)
          {
          if(pdg ==11) EffEl0->Fill(bPassed,MomIni);
          if(pdg ==13) EffMu0->Fill(bPassed,MomIni);
          if(pdg ==-211)  EffPi0->Fill(bPassed,MomIni);
          if(pdg ==2212) EffPr0->Fill(bPassed,MomIni);
          }
          if(abs(phi*360/(2*3.141592)) >= (90-30-AngWindow) && abs(phi*360/(2*3.141592)) <= (90-30+AngWindow))
          {
          if(pdg ==11) EffEl30->Fill(bPassed,MomIni);
          if(pdg ==13) EffMu30->Fill(bPassed,MomIni);
          if(pdg ==-211)  EffPi30->Fill(bPassed,MomIni);
          if(pdg ==2212) EffPr30->Fill(bPassed,MomIni);
          }
          if(abs(phi*360/(2*3.141592)) >= (90-60-AngWindow) && abs(phi*360/(2*3.141592)) <= (90-60+AngWindow))
          {
          if(pdg ==11) EffEl60->Fill(bPassed,MomIni);
          if(pdg ==13) EffMu60->Fill(bPassed,MomIni);
          if(pdg ==-211)  EffPi60->Fill(bPassed,MomIni);
          if(pdg ==2212) EffPr60->Fill(bPassed,MomIni);
          }
          if(abs(phi*360/(2*3.141592)) >= (90-75-AngWindow) && abs(phi*360/(2*3.141592)) <= (90-75+AngWindow))
          {
          if(pdg ==11) EffEl60->Fill(bPassed,MomIni);
          if(pdg ==13) EffMu60->Fill(bPassed,MomIni);
          if(pdg ==-211)  EffPi60->Fill(bPassed,MomIni);
          if(pdg ==2212) EffPr60->Fill(bPassed,MomIni);
          }
        }

        if(ipt == 2) ipt = NPoints-1;

      

      }
    }


  }

  //20 Ang bins form -75 to 75 deg: 150/20 each bin is 7.5 deg wide. 
  // 10 is 0 deg. 14 is 30 deg. 18 is 60 deg. 20 is 75 deg.

   //Momentum loss
   TProfile2D * hMomEl3DProj = hMomEl3D->Project3DProfile("xy");
   TProfile * hMomEl0Proj = hMomEl3DProj->ProfileX("hMomEl0Proj",10,10);
   TProfile * hMomEl30Proj = hMomEl3DProj->ProfileX("hMomEl30Proj",14,14);
   TProfile * hMomEl60Proj = hMomEl3DProj->ProfileX("hMomEl60Proj",18,18);
   TProfile * hMomEl75Proj = hMomEl3DProj->ProfileX("hMomEl75Proj",20,20);

   TProfile2D * hMomMu3DProj = hMomMu3D->Project3DProfile("xy");
   TProfile * hMomMu0Proj = hMomMu3DProj->ProfileX("hMomMu0Proj",10,10);
   TProfile * hMomMu30Proj = hMomMu3DProj->ProfileX("hMomMu30Proj",14,14);
   TProfile * hMomMu60Proj = hMomMu3DProj->ProfileX("hMomMu60Proj",18,18);
   TProfile * hMomMu75Proj = hMomMu3DProj->ProfileX("hMomMu75Proj",20,20);
   
   TProfile2D * hMomPi3DProj = hMomPi3D->Project3DProfile("xy");
   TProfile * hMomPi0Proj = hMomPi3DProj->ProfileX("hMomPi0Proj",10,10);
   TProfile * hMomPi30Proj = hMomPi3DProj->ProfileX("hMomPi30Proj",14,14);
   TProfile * hMomPi60Proj = hMomPi3DProj->ProfileX("hMomPi60Proj",18,18);
   TProfile * hMomPi75Proj = hMomPi3DProj->ProfileX("hMomPi75Proj",20,20);

   TProfile2D * hMomPr3DProj = hMomPr3D->Project3DProfile("xy");
   TProfile * hMomPr0Proj = hMomPr3DProj->ProfileX("hMomPr0Proj",10,10);
   TProfile * hMomPr30Proj = hMomPr3DProj->ProfileX("hMomPr30Proj",14,14);
   TProfile * hMomPr60Proj = hMomPr3DProj->ProfileX("hMomPr60Proj",18,18);
   TProfile * hMomPr75Proj = hMomPr3DProj->ProfileX("hMomPr75Proj",20,20);

  //Angular deviation
   TProfile2D * hAngEl3DProj = hAngEl3D->Project3DProfile("xy");
   TProfile * hAngEl0Proj = hAngEl3DProj->ProfileX("hAngEl0Proj",10,10);
   TProfile * hAngEl30Proj = hAngEl3DProj->ProfileX("hAngEl30Proj",14,14);
   TProfile * hAngEl60Proj = hAngEl3DProj->ProfileX("hAngEl60Proj",18,18);
   TProfile * hAngEl75Proj = hAngEl3DProj->ProfileX("hAngEl75Proj",20,20);

   TProfile2D * hAngMu3DProj = hAngMu3D->Project3DProfile("xy");
   TProfile * hAngMu0Proj = hAngMu3DProj->ProfileX("hAngMu0Proj",10,10);
   TProfile * hAngMu30Proj = hAngMu3DProj->ProfileX("hAngMu30Proj",14,14);
   TProfile * hAngMu60Proj = hAngMu3DProj->ProfileX("hAngMu60Proj",18,18);
   TProfile * hAngMu75Proj = hAngMu3DProj->ProfileX("hAngMu75Proj",20,20);
   
   TProfile2D * hAngPi3DProj = hAngPi3D->Project3DProfile("xy");
   TProfile * hAngPi0Proj = hAngPi3DProj->ProfileX("hAngPi0Proj",10,10);
   TProfile * hAngPi30Proj = hAngPi3DProj->ProfileX("hAngPi30Proj",14,14);
   TProfile * hAngPi60Proj = hAngPi3DProj->ProfileX("hAngPi60Proj",18,18);
   TProfile * hAngPi75Proj = hAngPi3DProj->ProfileX("hAngPi75Proj",20,20);

   TProfile2D * hAngPr3DProj = hAngPr3D->Project3DProfile("xy");
   TProfile * hAngPr0Proj = hAngPr3DProj->ProfileX("hAngPr0Proj",10,10);
   TProfile * hAngPr30Proj = hAngPr3DProj->ProfileX("hAngPr30Proj",14,14);
   TProfile * hAngPr60Proj = hAngPr3DProj->ProfileX("hAngPr60Proj",18,18);
   TProfile * hAngPr75Proj = hAngPr3DProj->ProfileX("hAngPr75Proj",20,20);

   c1->cd(1);
   hMomEl0Proj->GetYaxis()->SetTitle("Final True Momentum [%]");
   hMomEl0Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hMomEl0Proj->SetTitle("");
   hMomEl0Proj->GetXaxis()->CenterTitle(true);
   hMomEl0Proj->GetYaxis()->CenterTitle(true);
   hMomEl0Proj->GetXaxis()->SetTitleSize(0.050);
   hMomEl0Proj->GetYaxis()->SetTitleSize(0.050);
   hMomEl0Proj->GetXaxis()->SetLabelSize(0.050);
   hMomEl0Proj->GetYaxis()->SetLabelSize(0.050);
   hMomEl0Proj->GetXaxis()->SetTitleOffset(1.1);
   hMomEl0Proj->Draw("HIST E1");
   TPaveText *t1 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t1->AddText("Initial Electron at 0 deg");
   t1->SetTextFont(22);
   t1->SetTextSize(0.050);
   t1->SetFillColor(kWhite);
   t1->Draw();

   c1->cd(2);
   hMomMu0Proj->GetYaxis()->SetTitle("Final True Momentum [%]");
   hMomMu0Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hMomMu0Proj->SetTitle("");
   hMomMu0Proj->GetXaxis()->CenterTitle(true);
   hMomMu0Proj->GetYaxis()->CenterTitle(true);
   hMomMu0Proj->GetXaxis()->SetTitleSize(0.050);
   hMomMu0Proj->GetYaxis()->SetTitleSize(0.050);
   hMomMu0Proj->GetXaxis()->SetLabelSize(0.050);
   hMomMu0Proj->GetYaxis()->SetLabelSize(0.050);
   hMomMu0Proj->GetXaxis()->SetTitleOffset(1.1);
   hMomMu0Proj->Draw("HIST E");
   TPaveText *t2 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t2->AddText("Initial Muon at 0 deg");
   t2->SetTextFont(22);
   t2->SetTextSize(0.050);
   t2->SetFillColor(kWhite);
   t2->Draw();

   c1->cd(3);
   hMomPi0Proj->GetYaxis()->SetTitle("Final True Momentum [%]");
   hMomPi0Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hMomPi0Proj->SetTitle("");
   hMomPi0Proj->GetXaxis()->CenterTitle(true);
   hMomPi0Proj->GetYaxis()->CenterTitle(true);
   hMomPi0Proj->GetXaxis()->SetTitleSize(0.050);
   hMomPi0Proj->GetYaxis()->SetTitleSize(0.050);
   hMomPi0Proj->GetXaxis()->SetLabelSize(0.050);
   hMomPi0Proj->GetYaxis()->SetLabelSize(0.050);
   hMomPi0Proj->GetXaxis()->SetTitleOffset(1.1);
   hMomPi0Proj->Draw("HIST E");
   TPaveText *t3 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t3->AddText("Initial Pion at 0 deg");
   t3->SetTextFont(22);
   t3->SetTextSize(0.050);
   t3->SetFillColor(kWhite);
   t3->Draw();

   c1->cd(4);
   hMomPr0Proj->GetYaxis()->SetTitle("Final True Momentum [%]");
   hMomPr0Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hMomPr0Proj->SetTitle("");
   hMomPr0Proj->GetXaxis()->CenterTitle(true);
   hMomPr0Proj->GetYaxis()->CenterTitle(true);
   hMomPr0Proj->GetXaxis()->SetTitleSize(0.050);
   hMomPr0Proj->GetYaxis()->SetTitleSize(0.050);
   hMomPr0Proj->GetXaxis()->SetLabelSize(0.050);
   hMomPr0Proj->GetYaxis()->SetLabelSize(0.050);
   hMomPr0Proj->GetXaxis()->SetTitleOffset(1.1);
   hMomPr0Proj->Draw("HIST E");
   TPaveText *t4 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t4->AddText("Initial Proton at 0 deg");
   t4->SetTextFont(22);
   t4->SetTextSize(0.050);
   t4->SetFillColor(kWhite);
   t4->Draw();

   c1->cd(5);
   hMomEl30Proj->GetYaxis()->SetTitle("Final True Momentum [%]");
   hMomEl30Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hMomEl30Proj->SetTitle("");
   hMomEl30Proj->GetXaxis()->CenterTitle(true);
   hMomEl30Proj->GetYaxis()->CenterTitle(true);
   hMomEl30Proj->GetXaxis()->SetTitleSize(0.050);
   hMomEl30Proj->GetYaxis()->SetTitleSize(0.050);
   hMomEl30Proj->GetXaxis()->SetLabelSize(0.050);
   hMomEl30Proj->GetYaxis()->SetLabelSize(0.050);
   hMomEl30Proj->GetXaxis()->SetTitleOffset(1.1);
   hMomEl30Proj->Draw("HIST E");
   TPaveText *t5 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t5->AddText("Initial Electron at 30 deg");
   t5->SetTextFont(22);
   t5->SetTextSize(0.050);
   t5->SetFillColor(kWhite);
   t5->Draw();

   c1->cd(6);
   hMomMu30Proj->GetYaxis()->SetTitle("Final True Momentum [%]");
   hMomMu30Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hMomMu30Proj->SetTitle("");
   hMomMu30Proj->GetXaxis()->CenterTitle(true);
   hMomMu30Proj->GetYaxis()->CenterTitle(true);
   hMomMu30Proj->GetXaxis()->SetTitleSize(0.050);
   hMomMu30Proj->GetYaxis()->SetTitleSize(0.050);
   hMomMu30Proj->GetXaxis()->SetLabelSize(0.050);
   hMomMu30Proj->GetYaxis()->SetLabelSize(0.050);
   hMomMu30Proj->GetXaxis()->SetTitleOffset(1.1);
   hMomMu30Proj->Draw("HIST E");
   TPaveText *t6 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t6->AddText("Initial Muon at 30 deg");
   t6->SetTextFont(22);
   t6->SetTextSize(0.050);
   t6->SetFillColor(kWhite);
   t6->Draw();

   c1->cd(7);
   hMomPi30Proj->GetYaxis()->SetTitle("Final True Momentum [%]");
   hMomPi30Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hMomPi30Proj->SetTitle("");
   hMomPi30Proj->GetXaxis()->CenterTitle(true);
   hMomPi30Proj->GetYaxis()->CenterTitle(true);
   hMomPi30Proj->GetXaxis()->SetTitleSize(0.050);
   hMomPi30Proj->GetYaxis()->SetTitleSize(0.050);
   hMomPi30Proj->GetXaxis()->SetLabelSize(0.050);
   hMomPi30Proj->GetYaxis()->SetLabelSize(0.050);
   hMomPi30Proj->GetXaxis()->SetTitleOffset(1.1);
   hMomPi30Proj->Draw("HIST E");
   TPaveText *t7 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t7->AddText("Initial Pion at 30 deg");
   t7->SetTextFont(22);
   t7->SetTextSize(0.050);
   t7->SetFillColor(kWhite);
   t7->Draw();

   c1->cd(8);
   hMomPr30Proj->GetYaxis()->SetTitle("Final True Momentum [%]");
   hMomPr30Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hMomPr30Proj->SetTitle("");
   hMomPr30Proj->GetXaxis()->CenterTitle(true);
   hMomPr30Proj->GetYaxis()->CenterTitle(true);
   hMomPr30Proj->GetXaxis()->SetTitleSize(0.050);
   hMomPr30Proj->GetYaxis()->SetTitleSize(0.050);
   hMomPr30Proj->GetXaxis()->SetLabelSize(0.050);
   hMomPr30Proj->GetYaxis()->SetLabelSize(0.050);
   hMomPr30Proj->GetXaxis()->SetTitleOffset(1.1);
   hMomPr30Proj->Draw("HIST E");
   TPaveText *t8 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t8->AddText("Initial Proton at 30 deg");
   t8->SetTextFont(22);
   t8->SetTextSize(0.050);
   t8->SetFillColor(kWhite);
   t8->Draw();

   c1->cd(9);
   hMomEl60Proj->GetYaxis()->SetTitle("Final True Momentum [%]");
   hMomEl60Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hMomEl60Proj->SetTitle("");
   hMomEl60Proj->GetXaxis()->CenterTitle(true);
   hMomEl60Proj->GetYaxis()->CenterTitle(true);
   hMomEl60Proj->GetXaxis()->SetTitleSize(0.050);
   hMomEl60Proj->GetYaxis()->SetTitleSize(0.050);
   hMomEl60Proj->GetXaxis()->SetLabelSize(0.050);
   hMomEl60Proj->GetYaxis()->SetLabelSize(0.050);
   hMomEl60Proj->GetXaxis()->SetTitleOffset(1.1);
   hMomEl60Proj->Draw("HIST E");
   TPaveText *t9 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t9->AddText("Initial Electron at 60 deg");
   t9->SetTextFont(22);
   t9->SetTextSize(0.050);
   t9->SetFillColor(kWhite);
   t9->Draw();

   c1->cd(10);
   hMomMu60Proj->GetYaxis()->SetTitle("Final True Momentum [%]");
   hMomMu60Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hMomMu60Proj->SetTitle("");
   hMomMu60Proj->GetXaxis()->CenterTitle(true);
   hMomMu60Proj->GetYaxis()->CenterTitle(true);
   hMomMu60Proj->GetXaxis()->SetTitleSize(0.050);
   hMomMu60Proj->GetYaxis()->SetTitleSize(0.050);
   hMomMu60Proj->GetXaxis()->SetLabelSize(0.050);
   hMomMu60Proj->GetYaxis()->SetLabelSize(0.050);
   hMomMu60Proj->GetXaxis()->SetTitleOffset(1.1);
   hMomMu60Proj->Draw("HIST E");
   TPaveText *t10 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t10->AddText("Initial Muon at 60 deg");
   t10->SetTextFont(22);
   t10->SetTextSize(0.050);
   t10->SetFillColor(kWhite);
   t10->Draw();

   c1->cd(11);
   hMomPi60Proj->GetYaxis()->SetTitle("Final True Momentum [%]");
   hMomPi60Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hMomPi60Proj->SetTitle("");
   hMomPi60Proj->GetXaxis()->CenterTitle(true);
   hMomPi60Proj->GetYaxis()->CenterTitle(true);
   hMomPi60Proj->GetXaxis()->SetTitleSize(0.050);
   hMomPi60Proj->GetYaxis()->SetTitleSize(0.050);
   hMomPi60Proj->GetXaxis()->SetLabelSize(0.050);
   hMomPi60Proj->GetYaxis()->SetLabelSize(0.050);
   hMomPi60Proj->GetXaxis()->SetTitleOffset(1.1);
   hMomPi60Proj->Draw("HIST E");
   TPaveText *t11 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t11->AddText("Initial Pion at 60 deg");
   t11->SetTextFont(22);
   t11->SetTextSize(0.050);
   t11->SetFillColor(kWhite);
   t11->Draw();

   c1->cd(12);
   hMomPr60Proj->GetYaxis()->SetTitle("Final True Momentum [%]");
   hMomPr60Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hMomPr60Proj->SetTitle("");
   hMomPr60Proj->GetXaxis()->CenterTitle(true);
   hMomPr60Proj->GetYaxis()->CenterTitle(true);
   hMomPr60Proj->GetXaxis()->SetTitleSize(0.050);
   hMomPr60Proj->GetYaxis()->SetTitleSize(0.050);
   hMomPr60Proj->GetXaxis()->SetLabelSize(0.050);
   hMomPr60Proj->GetYaxis()->SetLabelSize(0.050);
   hMomPr60Proj->GetXaxis()->SetTitleOffset(1.1);
   hMomPr60Proj->Draw("HIST E");
   TPaveText *t12 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t12->AddText("Initial Proton at 60 deg");
   t12->SetTextFont(22);
   t12->SetTextSize(0.050);
   t12->SetFillColor(kWhite);
   t12->Draw();

//Ang

   c2->cd(1);
   hAngEl0Proj->GetYaxis()->SetTitle("Angular deviation [deg]");
   hAngEl0Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hAngEl0Proj->SetTitle("");
   hAngEl0Proj->GetXaxis()->CenterTitle(true);
   hAngEl0Proj->GetYaxis()->CenterTitle(true);
   hAngEl0Proj->GetXaxis()->SetTitleSize(0.050);
   hAngEl0Proj->GetYaxis()->SetTitleSize(0.050);
   hAngEl0Proj->GetXaxis()->SetLabelSize(0.050);
   hAngEl0Proj->GetYaxis()->SetLabelSize(0.050);
   hAngEl0Proj->GetXaxis()->SetTitleOffset(1.1);
   hAngEl0Proj->Draw("HIST E");
   TPaveText *t13 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t13->AddText("Initial Electron at 0 deg");
   t13->SetTextFont(22);
   t13->SetTextSize(0.050);
   t13->SetFillColor(kWhite);
   t13->Draw();

   c2->cd(2);
   hAngMu0Proj->GetYaxis()->SetTitle("Angular deviation [deg]");
   hAngMu0Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hAngMu0Proj->SetTitle("");
   hAngMu0Proj->GetXaxis()->CenterTitle(true);
   hAngMu0Proj->GetYaxis()->CenterTitle(true);
   hAngMu0Proj->GetXaxis()->SetTitleSize(0.050);
   hAngMu0Proj->GetYaxis()->SetTitleSize(0.050);
   hAngMu0Proj->GetXaxis()->SetLabelSize(0.050);
   hAngMu0Proj->GetYaxis()->SetLabelSize(0.050);
   hAngMu0Proj->GetXaxis()->SetTitleOffset(1.1);
   hAngMu0Proj->Draw("HIST E");
   TPaveText *t14 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t14->AddText("Initial Muon at 0 deg");
   t14->SetTextFont(22);
   t14->SetTextSize(0.050);
   t14->SetFillColor(kWhite);
   t14->Draw();

   c2->cd(3);
   hAngPi0Proj->GetYaxis()->SetTitle("Angular deviation [deg]");
   hAngPi0Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hAngPi0Proj->SetTitle("");
   hAngPi0Proj->GetXaxis()->CenterTitle(true);
   hAngPi0Proj->GetYaxis()->CenterTitle(true);
   hAngPi0Proj->GetXaxis()->SetTitleSize(0.050);
   hAngPi0Proj->GetYaxis()->SetTitleSize(0.050);
   hAngPi0Proj->GetXaxis()->SetLabelSize(0.050);
   hAngPi0Proj->GetYaxis()->SetLabelSize(0.050);
   hAngPi0Proj->GetXaxis()->SetTitleOffset(1.1);
   hAngPi0Proj->Draw("HIST E");
   TPaveText *t15 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t15->AddText("Initial Pion at 0 deg");
   t15->SetTextFont(22);
   t15->SetTextSize(0.050);
   t15->SetFillColor(kWhite);
   t15->Draw();

   c2->cd(4);
   hAngPr0Proj->GetYaxis()->SetTitle("Angular deviation [deg]");
   hAngPr0Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hAngPr0Proj->SetTitle("");
   hAngPr0Proj->GetXaxis()->CenterTitle(true);
   hAngPr0Proj->GetYaxis()->CenterTitle(true);
   hAngPr0Proj->GetXaxis()->SetTitleSize(0.050);
   hAngPr0Proj->GetYaxis()->SetTitleSize(0.050);
   hAngPr0Proj->GetXaxis()->SetLabelSize(0.050);
   hAngPr0Proj->GetYaxis()->SetLabelSize(0.050);
   hAngPr0Proj->GetXaxis()->SetTitleOffset(1.1);
   hAngPr0Proj->Draw("HIST E");
   TPaveText *t16 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t16->AddText("Initial Proton at 0 deg");
   t16->SetTextFont(22);
   t16->SetTextSize(0.050);
   t16->SetFillColor(kWhite);
   t16->Draw();

   c2->cd(5);
   hAngEl30Proj->GetYaxis()->SetTitle("Angular deviation [deg]");
   hAngEl30Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hAngEl30Proj->SetTitle("");
   hAngEl30Proj->GetXaxis()->CenterTitle(true);
   hAngEl30Proj->GetYaxis()->CenterTitle(true);
   hAngEl30Proj->GetXaxis()->SetTitleSize(0.050);
   hAngEl30Proj->GetYaxis()->SetTitleSize(0.050);
   hAngEl30Proj->GetXaxis()->SetLabelSize(0.050);
   hAngEl30Proj->GetYaxis()->SetLabelSize(0.050);
   hAngEl30Proj->GetXaxis()->SetTitleOffset(1.1);
   hAngEl30Proj->Draw("HIST E");
   TPaveText *t17 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t17->AddText("Initial Electron at 30 deg");
   t17->SetTextFont(22);
   t17->SetTextSize(0.050);
   t17->SetFillColor(kWhite);
   t17->Draw();

   c2->cd(6);
   hAngMu30Proj->GetYaxis()->SetTitle("Angular deviation [deg]");
   hAngMu30Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hAngMu30Proj->SetTitle("");
   hAngMu30Proj->GetXaxis()->CenterTitle(true);
   hAngMu30Proj->GetYaxis()->CenterTitle(true);
   hAngMu30Proj->GetXaxis()->SetTitleSize(0.050);
   hAngMu30Proj->GetYaxis()->SetTitleSize(0.050);
   hAngMu30Proj->GetXaxis()->SetLabelSize(0.050);
   hAngMu30Proj->GetYaxis()->SetLabelSize(0.050);
   hAngMu30Proj->GetXaxis()->SetTitleOffset(1.1);
   hAngMu30Proj->Draw("HIST E");
   TPaveText *t18 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t18->AddText("Initial Muon at 30 deg");
   t18->SetTextFont(22);
   t18->SetTextSize(0.050);
   t18->SetFillColor(kWhite);
   t18->Draw();

   c2->cd(7);
   hAngPi30Proj->GetYaxis()->SetTitle("Angular deviation [deg]");
   hAngPi30Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hAngPi30Proj->SetTitle("");
   hAngPi30Proj->GetXaxis()->CenterTitle(true);
   hAngPi30Proj->GetYaxis()->CenterTitle(true);
   hAngPi30Proj->GetXaxis()->SetTitleSize(0.050);
   hAngPi30Proj->GetYaxis()->SetTitleSize(0.050);
   hAngPi30Proj->GetXaxis()->SetLabelSize(0.050);
   hAngPi30Proj->GetYaxis()->SetLabelSize(0.050);
   hAngPi30Proj->GetXaxis()->SetTitleOffset(1.1);
   hAngPi30Proj->Draw("HIST E");
   TPaveText *t19 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t19->AddText("Initial Pion at 30 deg");
   t19->SetTextFont(22);
   t19->SetTextSize(0.050);
   t19->SetFillColor(kWhite);
   t19->Draw();

   c2->cd(8);
   hAngPr30Proj->GetYaxis()->SetTitle("Angular deviation [deg]");
   hAngPr30Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hAngPr30Proj->SetTitle("");
   hAngPr30Proj->GetXaxis()->CenterTitle(true);
   hAngPr30Proj->GetYaxis()->CenterTitle(true);
   hAngPr30Proj->GetXaxis()->SetTitleSize(0.050);
   hAngPr30Proj->GetYaxis()->SetTitleSize(0.050);
   hAngPr30Proj->GetXaxis()->SetLabelSize(0.050);
   hAngPr30Proj->GetYaxis()->SetLabelSize(0.050);
   hAngPr30Proj->GetXaxis()->SetTitleOffset(1.1);
   hAngPr30Proj->Draw("HIST E");
   TPaveText *t20 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t20->AddText("Initial Proton at 30 deg");
   t20->SetTextFont(22);
   t20->SetTextSize(0.050);
   t20->SetFillColor(kWhite);
   t20->Draw();

   c2->cd(9);
   hAngEl60Proj->GetYaxis()->SetTitle("Angular deviation [deg]");
   hAngEl60Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hAngEl60Proj->SetTitle("");
   hAngEl60Proj->GetXaxis()->CenterTitle(true);
   hAngEl60Proj->GetYaxis()->CenterTitle(true);
   hAngEl60Proj->GetXaxis()->SetTitleSize(0.050);
   hAngEl60Proj->GetYaxis()->SetTitleSize(0.050);
   hAngEl60Proj->GetXaxis()->SetLabelSize(0.050);
   hAngEl60Proj->GetYaxis()->SetLabelSize(0.050);
   hAngEl60Proj->GetXaxis()->SetTitleOffset(1.1);
   hAngEl60Proj->Draw("HIST E");
   TPaveText *t21 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t21->AddText("Initial Electron at 60 deg");
   t21->SetTextFont(22);
   t21->SetTextSize(0.050);
   t21->SetFillColor(kWhite);
   t21->Draw();

   c2->cd(10);
   hAngMu60Proj->GetYaxis()->SetTitle("Angular deviation [deg]");
   hAngMu60Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hAngMu60Proj->SetTitle("");
   hAngMu60Proj->GetXaxis()->CenterTitle(true);
   hAngMu60Proj->GetYaxis()->CenterTitle(true);
   hAngMu60Proj->GetXaxis()->SetTitleSize(0.050);
   hAngMu60Proj->GetYaxis()->SetTitleSize(0.050);
   hAngMu60Proj->GetXaxis()->SetLabelSize(0.050);
   hAngMu60Proj->GetYaxis()->SetLabelSize(0.050);
   hAngMu60Proj->GetXaxis()->SetTitleOffset(1.1);
   hAngMu60Proj->Draw("HIST E");
   TPaveText *t22 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t22->AddText("Initial Muon at 60 deg");
   t22->SetTextFont(22);
   t22->SetTextSize(0.050);
   t22->SetFillColor(kWhite);
   t22->Draw();

   c2->cd(11);
   hAngPi60Proj->GetYaxis()->SetTitle("Angular deviation [deg]");
   hAngPi60Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hAngPi60Proj->SetTitle("");
   hAngPi60Proj->GetXaxis()->CenterTitle(true);
   hAngPi60Proj->GetYaxis()->CenterTitle(true);
   hAngPi60Proj->GetXaxis()->SetTitleSize(0.050);
   hAngPi60Proj->GetYaxis()->SetTitleSize(0.050);
   hAngPi60Proj->GetXaxis()->SetLabelSize(0.050);
   hAngPi60Proj->GetYaxis()->SetLabelSize(0.050);
   hAngPi60Proj->GetXaxis()->SetTitleOffset(1.1);
   hAngPi60Proj->Draw("HIST E");
   TPaveText *t23 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t23->AddText("Initial Pion at 60 deg");
   t23->SetTextFont(22);
   t23->SetTextSize(0.050);
   t23->SetFillColor(kWhite);
   t23->Draw();

   c2->cd(12);
   hAngPr60Proj->GetYaxis()->SetTitle("Angular deviation [deg]");
   hAngPr60Proj->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
   hAngPr60Proj->SetTitle("");
   hAngPr60Proj->GetXaxis()->CenterTitle(true);
   hAngPr60Proj->GetYaxis()->CenterTitle(true);
   hAngPr60Proj->GetXaxis()->SetTitleSize(0.050);
   hAngPr60Proj->GetYaxis()->SetTitleSize(0.050);
   hAngPr60Proj->GetXaxis()->SetLabelSize(0.050);
   hAngPr60Proj->GetYaxis()->SetLabelSize(0.050);
   hAngPr60Proj->GetXaxis()->SetTitleOffset(1.1);
   hAngPr60Proj->Draw("HIST E");
   TPaveText *t24 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
   t24->AddText("Initial Proton at 60 deg");
   t24->SetTextFont(22);
   t24->SetTextSize(0.050);
   t24->SetFillColor(kWhite);
   t24->Draw();

   //Stopped

     // Crossing Efficiency
  double eylEl0[ElY], eyhEl0[ElY];
  double XbinEl0[ElY], YbinEl0[ElY];
  double eylMu0[MuY], eyhMu0[MuY];
  double XbinMu0[MuY], YbinMu0[MuY];
  double eylPi0[PiY], eyhPi0[PiY];
  double XbinPi0[PiY], YbinPi0[PiY];
  double eylPr0[PrY], eyhPr0[PrY];
  double XbinPr0[PrY], YbinPr0[PrY];
  double eylEl30[ElY], eyhEl30[ElY];
  double XbinEl30[ElY], YbinEl30[ElY];
  double eylMu30[MuY], eyhMu30[MuY];
  double XbinMu30[MuY], YbinMu30[MuY];
  double eylPi30[PiY], eyhPi30[PiY];
  double XbinPi30[PiY], YbinPi30[PiY];
  double eylPr30[PrY], eyhPr30[PrY];
  double XbinPr30[PrY], YbinPr30[PrY];
  double eylEl60[ElY], eyhEl60[ElY];
  double XbinEl60[ElY], YbinEl60[ElY];
  double eylMu60[MuY], eyhMu60[MuY];
  double XbinMu60[MuY], YbinMu60[MuY];
  double eylPi60[PiY], eyhPi60[PiY];
  double XbinPi60[PiY], YbinPi60[PiY];
  double eylPr60[PrY], eyhPr60[PrY];
  double XbinPr60[PrY], YbinPr60[PrY];
  
  c3->cd(1);
  TH1F *hStopEl0 = new TH1F("hStopEl0","hStopEl0 with AsymmErrors",ElY,ElIni,ElFin);
  for(int i=1; i<=ElY; i++)
  {
  hStopEl0->SetBinContent(i, 100*(EffEl0->GetEfficiency(i)));
  eylEl0[i-1] = 100*(EffEl0->GetEfficiencyErrorLow(i)); 
  eyhEl0[i-1] = 100*(EffEl0->GetEfficiencyErrorUp(i));
  if(eyhEl0[i-1] == 100) eyhEl0[i-1] = 0;
  XbinEl0[i-1] = ElIni+((ElFin-ElIni)/ElY)*(i-0.5);
  YbinEl0[i-1] = 100*(EffEl0->GetEfficiency(i));
  }
  hStopEl0->GetYaxis()->SetRangeUser(0,105);
  hStopEl0->GetYaxis()->SetTitle("Crossing Efficiency [%]");
  hStopEl0->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
  hStopEl0->SetTitle("");
  hStopEl0->GetXaxis()->CenterTitle(true);
  hStopEl0->GetYaxis()->CenterTitle(true);
  hStopEl0->GetXaxis()->SetTitleSize(0.050);
  hStopEl0->GetYaxis()->SetTitleSize(0.050);
  hStopEl0->GetXaxis()->SetLabelSize(0.050);
  hStopEl0->GetYaxis()->SetLabelSize(0.050);
  hStopEl0->GetXaxis()->SetTitleOffset(1.1);
  hStopEl0->Draw("HIST");    
  TPaveText *t25 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
  t25->AddText("Initial Electron at 0 deg");
  t25->SetTextFont(22);
  t25->SetTextSize(0.050);
  t25->SetFillColor(kWhite);
  t25->Draw();                                                                          
  TGraphAsymmErrors *  grStopEl0 = new TGraphAsymmErrors(ElY,XbinEl0,YbinEl0,0,0,eylEl0,eyhEl0);
  grStopEl0-> SetName("grStopEl0");
  grStopEl0->SetLineColor(kBlue);
  grStopEl0->Draw("P SAME");

  c3->cd(2);
  TH1F *hStopMu0 = new TH1F("hStopMu0","hStopMu0 with AsymmErrors",MuY,MuIni,MuFin);
  for(int i=1; i<=MuY; i++)
  {
  hStopMu0->SetBinContent(i, 100*(EffMu0->GetEfficiency(i)));
  eylMu0[i-1] = 100*(EffMu0->GetEfficiencyErrorLow(i)); 
  eyhMu0[i-1] = 100*(EffMu0->GetEfficiencyErrorUp(i));
  if(eyhMu0[i-1] == 100) eyhMu0[i-1] = 0;
  XbinMu0[i-1] = MuIni+((MuFin-MuIni)/MuY)*(i-0.5);
  YbinMu0[i-1] = 100*(EffMu0->GetEfficiency(i));

  }
  hStopMu0->GetYaxis()->SetRangeUser(0,105);
  hStopMu0->GetYaxis()->SetTitle("Crossing Efficiency [%]");
  hStopMu0->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
  hStopMu0->SetTitle("");
  hStopMu0->GetXaxis()->CenterTitle(true);
  hStopMu0->GetYaxis()->CenterTitle(true);
  hStopMu0->GetXaxis()->SetTitleSize(0.050);
  hStopMu0->GetYaxis()->SetTitleSize(0.050);
  hStopMu0->GetXaxis()->SetLabelSize(0.050);
  hStopMu0->GetYaxis()->SetLabelSize(0.050);
  hStopMu0->GetXaxis()->SetTitleOffset(1.1);
  hStopMu0->Draw("HIST");    
  TPaveText *t26 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
  t26->AddText("Initial Muon at 0 deg");
  t26->SetTextFont(22);
  t26->SetTextSize(0.050);
  t26->SetFillColor(kWhite);
  t26->Draw();                                                                              
  TGraphAsymmErrors *  grStopMu0 = new TGraphAsymmErrors(MuY,XbinMu0,YbinMu0,0,0,eylMu0,eyhMu0);
  grStopMu0-> SetName("grStopMu0");
  grStopMu0->SetLineColor(kBlue);
  grStopMu0->Draw("P SAME");

  c3->cd(3);
  TH1F *hStopPi0 = new TH1F("hStopPi0","hStopPi0 with AsymmErrors",PiY,PiIni,PiFin);
  for(int i=1; i<=PiY; i++)
  {
  hStopPi0->SetBinContent(i, 100*(EffPi0->GetEfficiency(i)));
  eylPi0[i-1] = 100*(EffPi0->GetEfficiencyErrorLow(i)); 
  eyhPi0[i-1] = 100*(EffPi0->GetEfficiencyErrorUp(i));
  if(eyhPi0[i-1] == 100) eyhPi0[i-1] = 0;
  XbinPi0[i-1] = PiIni+((PiFin-PiIni)/PiY)*(i-0.5);
  YbinPi0[i-1] = 100*(EffPi0->GetEfficiency(i));
  }
  hStopPi0->GetYaxis()->SetRangeUser(0,105);
  hStopPi0->GetYaxis()->SetTitle("Crossing Efficiency [%]");
  hStopPi0->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
  hStopPi0->SetTitle("");
  hStopPi0->GetXaxis()->CenterTitle(true);
  hStopPi0->GetYaxis()->CenterTitle(true);
  hStopPi0->GetXaxis()->SetTitleSize(0.050);
  hStopPi0->GetYaxis()->SetTitleSize(0.050);
  hStopPi0->GetXaxis()->SetLabelSize(0.050);
  hStopPi0->GetYaxis()->SetLabelSize(0.050);
  hStopPi0->GetXaxis()->SetTitleOffset(1.1);
  hStopPi0->Draw("HIST"); 
  TPaveText *t27 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
  t27->AddText("Initial Pion at 0 deg");
  t27->SetTextFont(22);
  t27->SetTextSize(0.050);
  t27->SetFillColor(kWhite);
  t27->Draw();                                                                              
  TGraphAsymmErrors *  grStopPi0 = new TGraphAsymmErrors(PiY,XbinPi0,YbinPi0,0,0,eylPi0,eyhPi0);
  grStopPi0-> SetName("grStopPi0");
  grStopPi0->SetLineColor(kBlue);
  grStopPi0->Draw("P SAME");

  c3->cd(4);
  TH1F *hStopPr0 = new TH1F("hStopPr0","hStopPr0 with AsymmErrors",PrY,PrIni,PrFin);
  for(int i=1; i<=PrY; i++)
  {
  hStopPr0->SetBinContent(i, 100*(EffPr0->GetEfficiency(i)));
  eylPr0[i-1] = 100*(EffPr0->GetEfficiencyErrorLow(i)); 
  eyhPr0[i-1] = 100*(EffPr0->GetEfficiencyErrorUp(i));
  if(eyhPr0[i-1] == 100) eyhPr0[i-1] = 0;
  XbinPr0[i-1] = PrIni+((PrFin-PrIni)/PrY)*(i-0.5);
  YbinPr0[i-1] = 100*(EffPr0->GetEfficiency(i));
  }
  hStopPr0->GetYaxis()->SetRangeUser(0,105);
  hStopPr0->GetYaxis()->SetTitle("Crossing Efficiency [%]");
  hStopPr0->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
  hStopPr0->SetTitle("");
  hStopPr0->GetXaxis()->CenterTitle(true);
  hStopPr0->GetYaxis()->CenterTitle(true);
  hStopPr0->GetXaxis()->SetTitleSize(0.050);
  hStopPr0->GetYaxis()->SetTitleSize(0.050);
  hStopPr0->GetXaxis()->SetLabelSize(0.050);
  hStopPr0->GetYaxis()->SetLabelSize(0.050);
  hStopPr0->GetXaxis()->SetTitleOffset(1.1);
  hStopPr0->Draw("HIST"); 
  TPaveText *t28 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
  t28->AddText("Initial Proton at 0 deg");
  t28->SetTextFont(22);
  t28->SetTextSize(0.050);
  t28->SetFillColor(kWhite);
  t28->Draw();                                                                             
  TGraphAsymmErrors *  grStopPr0 = new TGraphAsymmErrors(PrY,XbinPr0,YbinPr0,0,0,eylPr0,eyhPr0);
  grStopPr0-> SetName("grStopPr0");
  grStopPr0->SetLineColor(kBlue);
  grStopPr0->Draw("P SAME");

  c3->cd(5);
  TH1F *hStopEl30 = new TH1F("hStopEl30","hStopEl30 with AsymmErrors",ElY,ElIni,ElFin);
  for(int i=1; i<=ElY; i++)
  {
  hStopEl30->SetBinContent(i, 100*(EffEl30->GetEfficiency(i)));
  eylEl30[i-1] = 100*(EffEl30->GetEfficiencyErrorLow(i)); 
  eyhEl30[i-1] = 100*(EffEl30->GetEfficiencyErrorUp(i));
  if(eyhEl30[i-1] == 100) eyhEl30[i-1] = 0;
  XbinEl30[i-1] = ElIni+((ElFin-ElIni)/ElY)*(i-0.5);
  YbinEl30[i-1] = 100*(EffEl30->GetEfficiency(i));
  }
  hStopEl30->GetYaxis()->SetRangeUser(0,105);
  hStopEl30->GetYaxis()->SetTitle("Crossing Efficiency [%]");
  hStopEl30->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
  hStopEl30->SetTitle("");
  hStopEl30->GetXaxis()->CenterTitle(true);
  hStopEl30->GetYaxis()->CenterTitle(true);
  hStopEl30->GetXaxis()->SetTitleSize(0.050);
  hStopEl30->GetYaxis()->SetTitleSize(0.050);
  hStopEl30->GetXaxis()->SetLabelSize(0.050);
  hStopEl30->GetYaxis()->SetLabelSize(0.050);
  hStopEl30->GetXaxis()->SetTitleOffset(1.1);
  hStopEl30->Draw("HIST"); 
  TPaveText *t29 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
  t29->AddText("Initial Electron at 30 deg");
  t29->SetTextFont(22);
  t29->SetTextSize(0.050);
  t29->SetFillColor(kWhite);
  t29->Draw();                                                                              
  TGraphAsymmErrors *  grStopEl30 = new TGraphAsymmErrors(ElY,XbinEl30,YbinEl30,0,0,eylEl30,eyhEl30);
  grStopEl30-> SetName("grStopEl30");
  grStopEl30->SetLineColor(kBlue);
  grStopEl30->Draw("P SAME");

  c3->cd(6);
  TH1F *hStopMu30 = new TH1F("hStopMu30","hStopMu30 with AsymmErrors",MuY,MuIni,MuFin);
  for(int i=1; i<=MuY; i++)
  {
  hStopMu30->SetBinContent(i, 100*(EffMu30->GetEfficiency(i)));
  eylMu30[i-1] = 100*(EffMu30->GetEfficiencyErrorLow(i)); 
  eyhMu30[i-1] = 100*(EffMu30->GetEfficiencyErrorUp(i));
  if(eyhMu30[i-1] == 100) eyhMu30[i-1] = 0;
  XbinMu30[i-1] = MuIni+((MuFin-MuIni)/MuY)*(i-0.5);
  YbinMu30[i-1] = 100*(EffMu30->GetEfficiency(i));
  }
  hStopMu30->GetYaxis()->SetRangeUser(0,105);
  hStopMu30->GetYaxis()->SetTitle("Crossing Efficiency [%]");
  hStopMu30->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
  hStopMu30->SetTitle("");
  hStopMu30->GetXaxis()->CenterTitle(true);
  hStopMu30->GetYaxis()->CenterTitle(true);
  hStopMu30->GetXaxis()->SetTitleSize(0.050);
  hStopMu30->GetYaxis()->SetTitleSize(0.050);
  hStopMu30->GetXaxis()->SetLabelSize(0.050);
  hStopMu30->GetYaxis()->SetLabelSize(0.050);
  hStopMu30->GetXaxis()->SetTitleOffset(1.1);
  hStopMu30->Draw("HIST"); 
  TPaveText *t30 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
  t30->AddText("Initial Muon at 30 deg");
  t30->SetTextFont(22);
  t30->SetTextSize(0.050);
  t30->SetFillColor(kWhite);
  t30->Draw();                                                                           
  TGraphAsymmErrors *  grStopMu30 = new TGraphAsymmErrors(MuY,XbinMu30,YbinMu30,0,0,eylMu30,eyhMu30);
  grStopMu30-> SetName("grStopMu30");
  grStopMu30->SetLineColor(kBlue);
  grStopMu30->Draw("P SAME");

  c3->cd(7);
  TH1F *hStopPi30 = new TH1F("hStopPi30","hStopPi30 with AsymmErrors",PiY,PiIni,PiFin);
  for(int i=1; i<=PiY; i++)
  {
  hStopPi30->SetBinContent(i, 100*(EffPi30->GetEfficiency(i)));
  eylPi30[i-1] = 100*(EffPi30->GetEfficiencyErrorLow(i)); 
  eyhPi30[i-1] = 100*(EffPi30->GetEfficiencyErrorUp(i));
  if(eyhPi30[i-1] == 100) eyhPi30[i-1] = 0;
  XbinPi30[i-1] = PiIni+((PiFin-PiIni)/PiY)*(i-0.5);
  YbinPi30[i-1] = 100*(EffPi30->GetEfficiency(i));
  }
  hStopPi30->GetYaxis()->SetRangeUser(0,105);
  hStopPi30->GetYaxis()->SetTitle("Crossing Efficiency [%]");
  hStopPi30->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
  hStopPi30->SetTitle("");
  hStopPi30->GetXaxis()->CenterTitle(true);
  hStopPi30->GetYaxis()->CenterTitle(true);
  hStopPi30->GetXaxis()->SetTitleSize(0.050);
  hStopPi30->GetYaxis()->SetTitleSize(0.050);
  hStopPi30->GetXaxis()->SetLabelSize(0.050);
  hStopPi30->GetYaxis()->SetLabelSize(0.050);
  hStopPi30->GetXaxis()->SetTitleOffset(1.1);
  hStopPi30->Draw("HIST");   
  TPaveText *t31 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");
  t31->AddText("Initial Pion at 30 deg");
  t31->SetTextFont(22);
  t31->SetTextSize(0.050);
  t31->SetFillColor(kWhite);
  t31->Draw();                                                                         
  TGraphAsymmErrors *  grStopPi30 = new TGraphAsymmErrors(PiY,XbinPi30,YbinPi30,0,0,eylPi30,eyhPi30);
  grStopPi30-> SetName("grStopPi30");
  grStopPi30->SetLineColor(kBlue);
  grStopPi30->Draw("P SAME");

  c3->cd(8);
  TH1F *hStopPr30 = new TH1F("hStopPr30","hStopPr30 with AsymmErrors",PrY,PrIni,PrFin);
  for(int i=1; i<=PrY; i++)
  {
  hStopPr30->SetBinContent(i, 100*(EffPr30->GetEfficiency(i)));
  eylPr30[i-1] = 100*(EffPr30->GetEfficiencyErrorLow(i)); 
  eyhPr30[i-1] = 100*(EffPr30->GetEfficiencyErrorUp(i));
  if(eyhPr30[i-1] == 100) eyhPr30[i-1] = 0;
  XbinPr30[i-1] = PrIni+((PrFin-PrIni)/PrY)*(i-0.5);
  YbinPr30[i-1] = 100*(EffPr30->GetEfficiency(i));
  }
  hStopPr30->GetYaxis()->SetRangeUser(0,105);
  hStopPr30->GetYaxis()->SetTitle("Crossing Efficiency [%]");
  hStopPr30->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
  hStopPr30->SetTitle("");
  hStopPr30->GetXaxis()->CenterTitle(true);
  hStopPr30->GetYaxis()->CenterTitle(true);
  hStopPr30->GetXaxis()->SetTitleSize(0.050);
  hStopPr30->GetYaxis()->SetTitleSize(0.050);
  hStopPr30->GetXaxis()->SetLabelSize(0.050);
  hStopPr30->GetYaxis()->SetLabelSize(0.050);
  hStopPr30->GetXaxis()->SetTitleOffset(1.1);
  hStopPr30->Draw("HIST");  
  TPaveText *t32 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");                                                                             
  t32->AddText("Initial Proton at 30 deg");
  t32->SetTextFont(22);
  t32->SetTextSize(0.050);
  t32->SetFillColor(kWhite);
  t32->Draw();  
  TGraphAsymmErrors *  grStopPr30 = new TGraphAsymmErrors(PrY,XbinPr30,YbinPr30,0,0,eylPr30,eyhPr30);
  grStopPr30-> SetName("grStopPr30");
  grStopPr30->SetLineColor(kBlue);
  grStopPr30->Draw("P SAME");

  c3->cd(9);
  TH1F *hStopEl60 = new TH1F("hStopEl60","hStopEl60 with AsymmErrors",ElY,ElIni,ElFin);
  for(int i=1; i<=ElY; i++)
  {
  hStopEl60->SetBinContent(i, 100*(EffEl60->GetEfficiency(i)));
  eylEl60[i-1] = 100*(EffEl60->GetEfficiencyErrorLow(i)); 
  eyhEl60[i-1] = 100*(EffEl60->GetEfficiencyErrorUp(i));
  if(eyhEl60[i-1] == 100) eyhEl30[i-1] = 0;
  XbinEl60[i-1] = ElIni+((ElFin-ElIni)/ElY)*(i-0.5);
  YbinEl60[i-1] = 100*(EffEl60->GetEfficiency(i));
  } 
  hStopEl60->GetYaxis()->SetRangeUser(0,105);
  hStopEl60->GetYaxis()->SetTitle("Crossing Efficiency [%]");
  hStopEl60->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
  hStopEl60->SetTitle("");
  hStopEl60->GetXaxis()->CenterTitle(true);
  hStopEl60->GetYaxis()->CenterTitle(true);
  hStopEl60->GetXaxis()->SetTitleSize(0.050);
  hStopEl60->GetYaxis()->SetTitleSize(0.050);
  hStopEl60->GetXaxis()->SetLabelSize(0.050);
  hStopEl60->GetYaxis()->SetLabelSize(0.050);
  hStopEl60->GetXaxis()->SetTitleOffset(1.1);
  hStopEl60->Draw("HIST");  
  TPaveText *t33 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");                                                                               
  t33->AddText("Initial Electron at 60 deg");
  t33->SetTextFont(22);
  t33->SetTextSize(0.050);
  t33->SetFillColor(kWhite);
  t33->Draw();                                                                         
  TGraphAsymmErrors *  grStopEl60 = new TGraphAsymmErrors(ElY,XbinEl60,YbinEl60,0,0,eylEl60,eyhEl60);
  grStopEl60-> SetName("grStopEl60");
  grStopEl60->SetLineColor(kBlue);
  grStopEl60->Draw("P SAME");

  c3->cd(10);
  TH1F *hStopMu60 = new TH1F("hStopMu60","hStopMu60 with AsymmErrors",MuY,MuIni,MuFin);
  for(int i=1; i<=MuY; i++)
  {
  hStopMu60->SetBinContent(i, 100*(EffMu60->GetEfficiency(i)));
  eylMu60[i-1] = 100*(EffMu60->GetEfficiencyErrorLow(i)); 
  eyhMu60[i-1] = 100*(EffMu60->GetEfficiencyErrorUp(i));
  if(eyhMu60[i-1] == 100) eyhMu60[i-1] = 0;
  XbinMu60[i-1] = MuIni+((MuFin-MuIni)/MuY)*(i-0.5);
  YbinMu60[i-1] = 100*(EffMu60->GetEfficiency(i));
  }
  hStopMu60->GetYaxis()->SetRangeUser(0,105);
  hStopMu60->GetYaxis()->SetTitle("Crossing Efficiency [%]");
  hStopMu60->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
  hStopMu60->SetTitle("");
  hStopMu60->GetXaxis()->CenterTitle(true);
  hStopMu60->GetYaxis()->CenterTitle(true);
  hStopMu60->GetXaxis()->SetTitleSize(0.050);
  hStopMu60->GetYaxis()->SetTitleSize(0.050);
  hStopMu60->GetXaxis()->SetLabelSize(0.050);
  hStopMu60->GetYaxis()->SetLabelSize(0.050);
  hStopMu60->GetXaxis()->SetTitleOffset(1.1);
  hStopMu60->Draw("HIST");  
  TPaveText *t34 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");                                                                               
  t34->AddText("Initial Muon at 60 deg");
  t34->SetTextFont(22);
  t34->SetTextSize(0.050);
  t34->SetFillColor(kWhite);
  t34->Draw();                                                                              
  TGraphAsymmErrors *  grStopMu60 = new TGraphAsymmErrors(MuY,XbinMu60,YbinMu60,0,0,eylMu60,eyhMu60);
  grStopMu60-> SetName("grStopMu60");
  grStopMu60->SetLineColor(kBlue);
  grStopMu60->Draw("P SAME");

  c3->cd(11);
  TH1F *hStopPi60 = new TH1F("hStopPi60","hStopPi60 with AsymmErrors",PiY,PiIni,PiFin);
  for(int i=1; i<=PiY; i++)
  {
  hStopPi60->SetBinContent(i, 100*(EffPi60->GetEfficiency(i)));
  eylPi60[i-1] = 100*(EffPi60->GetEfficiencyErrorLow(i)); 
  eyhPi60[i-1] = 100*(EffPi60->GetEfficiencyErrorUp(i));
  if(eyhPi60[i-1] == 100) eyhPi60[i-1] = 0;
  XbinPi60[i-1] = PiIni+((PiFin-PiIni)/PiY)*(i-0.5);
  YbinPi60[i-1] = 100*(EffPi60->GetEfficiency(i));
  }
  hStopPi60->GetYaxis()->SetRangeUser(0,105);
  hStopPi60->GetYaxis()->SetTitle("Crossing Efficiency [%]");
  hStopPi60->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
  hStopPi60->SetTitle("");
  hStopPi60->GetXaxis()->CenterTitle(true);
  hStopPi60->GetYaxis()->CenterTitle(true);
  hStopPi60->GetXaxis()->SetTitleSize(0.050);
  hStopPi60->GetYaxis()->SetTitleSize(0.050);
  hStopPi60->GetXaxis()->SetLabelSize(0.050);
  hStopPi60->GetYaxis()->SetLabelSize(0.050);
  hStopPi60->GetXaxis()->SetTitleOffset(1.1);
  hStopPi60->Draw("HIST");  
  TPaveText *t35 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");                                                                               
  t35->AddText("Initial Pion at 60 deg");
  t35->SetTextFont(22);
  t35->SetTextSize(0.050);
  t35->SetFillColor(kWhite);
  t35->Draw();                                                                              
  TGraphAsymmErrors *  grStopPi60 = new TGraphAsymmErrors(PiY,XbinPi60,YbinPi60,0,0,eylPi60,eyhPi60);
  grStopPi60-> SetName("grStopPi60");
  grStopPi60->SetLineColor(kBlue);
  grStopPi60->Draw("P SAME");

  c3->cd(12);
  TH1F *hStopPr60 = new TH1F("hStopPr60","hStopPr60 with AsymmErrors",PrY,PrIni,PrFin);
  for(int i=1; i<=PrY; i++)
  {
  hStopPr60->SetBinContent(i, 100*(EffPr60->GetEfficiency(i)));
  eylPr60[i-1] = 100*(EffPr60->GetEfficiencyErrorLow(i)); 
  eyhPr60[i-1] = 100*(EffPr60->GetEfficiencyErrorUp(i));
  if(eyhPr60[i-1] == 100) eyhPr60[i-1] = 0;
  XbinPr60[i-1] = PrIni+((PrFin-PrIni)/PrY)*(i-0.5);
  YbinPr60[i-1] = 100*(EffPr60->GetEfficiency(i));
  }
  hStopPr60->GetYaxis()->SetRangeUser(0,105);
  hStopPr60->GetYaxis()->SetTitle("Crossing Efficiency [%]");
  hStopPr60->GetXaxis()->SetTitle("Initial True Momentum [MeV]");
  hStopPr60->SetTitle("");
  hStopPr60->GetXaxis()->CenterTitle(true);
  hStopPr60->GetYaxis()->CenterTitle(true);
  hStopPr60->GetXaxis()->SetTitleSize(0.050);
  hStopPr60->GetYaxis()->SetTitleSize(0.050);
  hStopPr60->GetXaxis()->SetLabelSize(0.050);
  hStopPr60->GetYaxis()->SetLabelSize(0.050);
  hStopPr60->GetXaxis()->SetTitleOffset(1.1);
  hStopPr60->Draw("HIST");  
  TPaveText *t36 = new TPaveText(0.26, 0.94, 0.76, 1.0, "bINDC");                                                                               
  t36->AddText("Initial Proton at 60 deg");
  t36->SetTextFont(22);
  t36->SetTextSize(0.050);
  t36->SetFillColor(kWhite);
  t36->Draw();                                                                                
  TGraphAsymmErrors *  grStopPr60 = new TGraphAsymmErrors(PrY,XbinPr60,YbinPr60,0,0,eylPr60,eyhPr60);
  grStopPr60-> SetName("grStopPr60");
  grStopPr60->SetLineColor(kBlue);
  grStopPr60->Draw("P SAME");

  TCanvas *c4 = new TCanvas("c4","c4",1200,900);

  hSpeMu200MeV0deg->Draw("HIST");

  cout << "MEAN IS: " << hSpeMu200MeV0deg->GetMean() << endl;

  // Save in an output file.


  TString outfilename = "";

  outfilename = TString::Format("%s_Evt%d_NEvt%d.root",tag.c_str(),evtfirst,nevents);

    TFile *out = new TFile(outfilename.Data(),"RECREATE");

//Mom

    hAng->Write();
    hdirX->Write();
    hdirY->Write();
    hdirZ->Write();

    hMomEl3DProj->Write();
    hMomEl0Proj->Write();
    hMomEl30Proj->Write();
    hMomEl60Proj->Write();
    hMomEl75Proj->Write();

    hMomMu3DProj->Write();
    hMomMu0Proj->Write();
    hMomMu30Proj->Write();
    hMomMu60Proj->Write();
    hMomMu75Proj->Write();

    hMomPi3DProj->Write();
    hMomPi0Proj->Write();
    hMomPi30Proj->Write();
    hMomPi60Proj->Write();
    hMomPi75Proj->Write();

    hMomPr3DProj->Write();
    hMomPr0Proj->Write();
    hMomPr30Proj->Write();
    hMomPr60Proj->Write();
    hMomPr75Proj->Write();

//Ang

    hAngEl3DProj->Write();
    hAngEl0Proj->Write();
    hAngEl30Proj->Write();
    hAngEl60Proj->Write();
    hAngEl75Proj->Write();

    hAngMu3DProj->Write();
    hAngMu0Proj->Write();
    hAngMu30Proj->Write();
    hAngMu60Proj->Write();
    hAngMu75Proj->Write();

    hAngPi3DProj->Write();
    hAngPi0Proj->Write();
    hAngPi30Proj->Write();
    hAngPi60Proj->Write();
    hAngPi75Proj->Write();

    hAngPr3DProj->Write();
    hAngPr0Proj->Write();
    hAngPr30Proj->Write();
    hAngPr60Proj->Write();
    hAngPr75Proj->Write();

//Stopped

    hStopEl0->Write();
    hStopEl30->Write();
    hStopEl60->Write();
    grStopEl0->Write();
    grStopEl30->Write();
    grStopEl60->Write();
    
    hStopMu0->Write();
    hStopMu30->Write();
    hStopMu60->Write();
    grStopMu0->Write();
    grStopMu30->Write();
    grStopMu60->Write();

    hStopPi0->Write();
    hStopPi30->Write();
    hStopPi60->Write();
    grStopPi0->Write();
    grStopPi30->Write();
    grStopPi60->Write();

    hStopPr0->Write();
    hStopPr30->Write();
    hStopPr60->Write();
    grStopPr0->Write();
    grStopPr30->Write();
    grStopPr60->Write();

    hSpeMu200MeV0deg->Write();

    out->Close();

  cout << "File created in: " << outfilename << endl;
  
  TString filename = "";

  filename = TString::Format("%s_Evt%d_NEvt%d.pdf",tag.c_str(),evtfirst,nevents);

  c1->Print(filename);



  }




