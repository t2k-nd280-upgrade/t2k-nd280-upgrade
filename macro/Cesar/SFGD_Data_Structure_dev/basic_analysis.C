  //Cesar Jesus-Valls. 6/7/2018:
  //Macro to study the Field Cage prototypes. It analyses the data of a beam of different particles (e,mu,pi,p) crossing a multilayer wall 
  //simulated with GEANT4 in the nd280-upgrade code. This macro is though to be run with a perl script inside nd280-upgrade-master/submit 
  //named 'submit_nd280upgrade_fcw' and can be run using: 
  // $T2KND280UP/submit/submit_nd280upgrade_fcw --softw-dir $T2KND280UP --job-dir $PWD --resource interactive --inputfile $T2KND280UP/macro/Cesar/FCStudies/Data/Solid500k.root --nexpts 500000 --nruns 1 
  //before running it, it is necessary to export the $T2KND280UP! If someone needs help, please contact to cjesus@ifae.es


  ///---------------  LIBRARIES ------------------------

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
  #include "/software/neutrinos/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpHit.hh"

  ///---------------  THE FUNCTION ------------------------

void basic_analysis
(
//Initialize the variables fixed in the submit script
 const int evtfirst = 0,
 const int nevents = 1,
 string tag = "",
 string infilename = ""
)
{

///---------------  GLOBAL DEFINITIONS AND MESSAGES------------------------
  
  TFile *finput = new TFile(infilename.c_str(),"READ");

  TH2F* h2D_XY = new TH2F("XY","XY", 24,0,24,8,0,8);
  TH2F* h2D_XZ = new TH2F("XZ","XZ", 24,0,24,48,0,48);
  TH2F* h2D_YZ = new TH2F("YZ","YZ", 8,0,8,48,0,48);
  
  // TH2F* h2d_xy; TH2F* h2d_xz; TH2F* h2d_yz;
  // h2d_xy = (TH2F*)finput->Get("OutMPPCProj2D_XY");
  // h2d_xz = (TH2F*)finput->Get("OutMPPCProj2D_XZ");
  // h2d_yz = (TH2F*)finput->Get("OutMPPCProj2D_YZ");

  // Int_t binX = h2d_xy->GetXaxis()->GetNbins();
  // Int_t binY = h2d_xy->GetYaxis()->GetNbins();
  // Int_t binZ = h2d_xz->GetYaxis()->GetNbins();
  
  // Float_t X_min = h2d_xy->GetXaxis()->GetBinLowEdge(0);
  // Float_t X_max = h2d_xy->GetXaxis()->GetBinUpEdge(binX);
  // Float_t Y_min = h2d_xy->GetYaxis()->GetBinLowEdge(0);
  // Float_t Y_max = h2d_xy->GetYaxis()->GetBinUpEdge(binY);
  // Float_t Z_min = h2d_xz->GetYaxis()->GetBinLowEdge(0);
  // Float_t Z_max = h2d_xz->GetYaxis()->GetBinUpEdge(binZ);
  
  // cout << " binsX " << binX << " X min " << X_min << " X max " << X_max << "\n"
  // << " binsY " << binY  << " Y min " << Y_min << " Y max " << Y_max << "\n"
  // << " binsZ " << binZ << " Z min " << Z_min << " Z max " << Z_max
  // << endl;
  
  // for(int i=1;i<h2d_xz->GetNbinsX()+1;++i) {
  //   for(int j=1;j<h2d_xz->GetNbinsY()+1;++j) {
  //     cout << " get bincont " << i << "  " << j << "  " << h2d_xy->GetBinContent(i,j) << endl;
  //   }
  // }

  //Generate the Canvas and set the aesthetics
   TCanvas *c1 = new TCanvas("c1","c1",1200,900);
   c1->Divide(2,2);

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

// Histograms definition
  TH1D *h1 = new TH1D("h1","h1",10,0,10);
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

 
  ///**********************************************************
  ///---------------  LOOP OVER EVENTs ------------------------
  ///**********************************************************


    for(int ievt=evtfirst;ievt<=nevents;ievt++)
    { 
      tinput->GetEntry(ievt); 
      if(!(ievt%10000)){
        cout << "Event " << ievt << endl;  
      }

      cout << endl << "NEW EVENT" << endl << endl;

      int nhits= nd280UpEvent->GetNHits();
      cout << " nhits " << nhits << endl;
      
      for(int ihit = 0; ihit < nd280UpEvent->GetNHits(); ++ihit) {
        TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);

        // double X = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/20.+12;
        // double Y = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/20.+4;
        // double Z = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/20.+24;
        // double E = nd280UpHit->GetEnergyDeposit();

        // h2D_XY->Fill(X, Y, E);
        // h2D_XZ->Fill(X, Z, E);
        // h2D_YZ->Fill(Y, Z, E);

        // cout << endl;
        // cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" << endl;
        // cout << endl;

        // cout << "X: " << X << endl;
        // cout << "Y: " << Y << endl;
        // cout << "Z: " << Z << endl;
        // cout << "Det: " << nd280UpHit->GetDetName() << endl;

        // cout << endl;
        // cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" << endl;
        // cout << endl;

        nd280UpHit->PrintHit();
      
        // cout << endl;
        // cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" << endl;
        // cout << endl;

        // cout << "Truth:" << endl;
        // cout << " - fPrimaryId: " << nd280UpHit->GetPrimaryId() << endl;
        // cout << " - startX: " << nd280UpHit->GetStartX() << endl;
        // cout << " - startY: " << nd280UpHit->GetStartY() << endl;
        // cout << " - startZ: " << nd280UpHit->GetStartZ() << endl;
        // cout << " - startT: " << nd280UpHit->GetStartT() << endl;
        // cout << " - finalX: " << nd280UpHit->GetStopX() << endl;
        // cout << " - finalY: " << nd280UpHit->GetStopY() << endl;
        // cout << " - finalZ: " << nd280UpHit->GetStopZ() << endl;
        // cout << " - finalT: " << nd280UpHit->GetStopT() << endl;
        // cout << " - energyDep: " << nd280UpHit->GetEnergyDeposit() << endl;
        // cout << " - trackLength: " << nd280UpHit->GetStopT() << endl;

        // cout << endl;
        // cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" << endl;
        // cout << endl;

      }
      // c1->cd(1);
      // h2D_XY->Draw("COLZ");
      // c1->cd(2);
      // h2D_XZ->Draw("COLZ");
      // c1->cd(3);

      // h2D_YZ->Draw("COLZ");
      // c1->SaveAs(TString::Format("/nfs/neutrinos/cjesus/work/MC_evts/blabla_%d.pdf", ievt));
      // h2D_XY->Reset();
      // h2D_XZ->Reset();
      // h2D_YZ->Reset();
    }

}




