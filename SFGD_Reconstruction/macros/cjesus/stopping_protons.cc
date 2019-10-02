#define THIS_NAME stopping_protons
#define NOINTERACTIVE_OUTPUT
#define OVERRIDE_OPTIONS

#include "../../utils/global_header.h"
#include "../../utils/global_tools.C"
#include "../../utils/global_variables.h"
#include <time.h>
#include <TMath.h>
#include <TF1.h>
#include <TGraphErrors.h>






// Setting The Event Selection Criteria_____________________________________________________________
bool Selection (Double_t Emax, Int_t ZatMax, TH2F* event_XY, array<Double_t, 24> dEdxXZ, array<Double_t, 8> dEdyZY)
{
  Double_t stdx = event_XY->GetStdDev(1);
  Double_t stdy = event_XY->GetStdDev(2);

  if (Emax>125 && ZatMax>20 && stdx<1 && stdy<1 && dEdxXZ[0]==0 & dEdxXZ[23]==0 && dEdyZY[0]<20 && dEdyZY[7]<20) return 0;
  return 1;
}



void stopping_protons() {
    cout << endl << "Staring the execution." << endl;

    // STRUCTURE of the crosstalkAna macro:

    // 1. some variables definitions.
    // 2. command line argument options parsing
    // 3. we open the input file an read each event
    // 4. for each event we apply some selection criteria
    // 5. we fill some histograms with the information for the selected events.
    // 6. we store plots

    // NOTE: In case of using option -s || --save it is necessary to update harcoded paths.

    //gStyle->SetOptStat(0);
    //TString fileIn   = "/Users/dana/SuperFGD/NewUnpacking/27August_3/27August_3_MCR0_hadrons_0pt8Gev_0pt0T_Beam___NewStructure.root";
    //TString fileOut  = "/Users/dana/SuperFGD/NewUnpacking/ana_output2";
    //TString fileMC = "/Users/dana/SuperFGD/NewUnpacking/MCoutput.root";
    //TString prefix = "/Users/dana/SuperFGD/NewUnpacking/";

    TString fileIn  = "/home/cjesus/Work/Data/SFGD_prototype/DATA/25August_ALL/25AugustAll.root";
    TString fileOut = "/home/cjesus/Work/Data/SFGD_prototype/ANALYSIS/ana_stopping_protons";
    TString fileMC  = "/home/cjesus/Work/Data/SFGD_prototype/MC/RECO/MC_output.root";
    TString prefix  = "/home/cjesus/Work/Data/SFGD_prototype/ANALYSIS";

    int maxEvents    = std::numeric_limits<int>::max();
    int maxSelEvents = 100;//std::numeric_limits<int>::max();
    int maxStore     = 100;
    int q_Cut        = 50;
    int selEvents    = 0;
    int jobMode      = 0;    // 1 closes program at termination.
    int dataType     = 1;    // 0 is MC.
    bool SAVE_EVENTS = false;
    bool SHOW_EVENTS = false;
    bool RM_CROSSTALK= false;
    float time       = clock();


    //=====================================================================================
    //=====================================================================================



    // Canvas for plots:
    TH2F *event_XY = new TH2F("","", 24,0,24, 8,0,8);
    event_XY->SetTitle("XY View;X axis [cm];Y axis[cm];Energy [p.e.]");

    TH2F *event_ZY = new TH2F("","", 48,0,48, 8,0,8);
    event_ZY->SetTitle("ZY View;Z axis [cm];Y axis [cm];Energy [p.e.]");

    TH2F *event_XZ = new TH2F("","", 24,0,24, 48,0,48);
    event_XZ->SetTitle("XZ View;X axis [cm];Z axis [cm];Energy [p.e.]");

    TH1D *dEdzXZ = new TH1D("","",48,0,48);
    dEdzXZ->SetTitle("dE/dz in XZ;Z axis [cm];dE/dz [p.e.]");

    TH1D *dEdzZY = new TH1D("","",48,0,48);
    dEdzZY->SetTitle("dE/dz in ZY;Z axis [cm];dE/dz [p.e.]");

    TH1D *dEdzSUM = new TH1D("","",48,0,48);
    dEdzSUM->SetTitle("dE/dz in XZ and ZY;Z axis [cm];dE/dz [p.e.]");

    TH1D *E_at_ZatMax = new TH1D("E_at_ZatMax","",200,0,2000);
    E_at_ZatMax->GetXaxis()->SetTitle("Maximum energy deposited in each event [p.e.]");
    E_at_ZatMax->GetYaxis()->SetTitle("Number of events");

    TH1D *E_at_ZatMaxPlusOne = new TH1D("E_at_ZatMax+1","dEdz at ZatMax+1",200,0,2000);

    TH1D *All_Events_ZY = new TH1D("All_Events_ZY", "", 100, 0 ,100);
    All_Events_ZY->GetXaxis()->SetTitle("Z position relative to peak [cm]");
    All_Events_ZY->GetYaxis()->SetTitle("Mean deposited energy [p.e.]");

    TH1D *All_Events_norm_ZY = new TH1D("All_Events_norm_ZY", "All_Events_norm_ZY", 100, 0 ,100);

    TH1D *Range = new TH1D("Range","Range",50,0,50);
    Range->SetTitle("Range;Z axis [cm];");


    array<Double_t, 24> dEdxXZ;
    array<Double_t, 8> dEdyZY;



    //=====================================================================================
    //=====================================================================================



    /// ------------START--------------


    // The command line arguments are parsed
    for (int iarg=0; iarg<gApplication->Argc(); iarg++){
        if (string( gApplication->Argv(iarg))=="-h" || string( gApplication->Argv(iarg))=="--help" ){
            cout << "**************************************" << endl;
            cout << "Macros run options:" << endl;
            cout << "   -h || --help      print help info." << endl;
            cout << "   -i || --input     input file (*.root)." << endl;
            cout << "   -m || --MC        Allows to MC inoput." << endl;
            cout << "   -l || --local     Changes paths to local." << endl;
            cout << "   -j || --job       Closes the macro at the end of the execution." << endl;
            cout << "   -s || --save      Save hits of selected events." << endl;
            cout << "   -d || --display   Interactive display of selected events." << endl;
            cout << "**************************************" << endl;
        }
        else if (string( gApplication->Argv(iarg))=="-i" || string( gApplication->Argv(iarg))=="--input" ){
            iarg++;
            fileIn = gApplication->Argv(iarg);
            if (fileIn.Contains(".root")) {
                cout << "Input file: " << fileIn << endl;
            }
            else {
                cerr << "input file must be a .root file!" << endl;
                exit(1);
            }
        }
        else if (string( gApplication->Argv(iarg))=="-l" || string( gApplication->Argv(iarg))=="--local" ){
            cout << "Using local paths." << endl;
            fileIn   = "/Users/dana/SuperFGD/NewUnpacking/27August_3/27August_3_MCR0_hadrons_0pt8Gev_0pt0T_Beam___NewStructure.root";
            fileOut  = "/Users/dana/SuperFGD/NewUnpacking/ana_output2";
            fileMC = "/Users/dana/SuperFGD/NewUnpacking/MCoutput.root";
            prefix = "/Users/dana/SuperFGD/NewUnpacking/";
        }
        else if (string( gApplication->Argv(iarg))=="-m" || string( gApplication->Argv(iarg))=="--MC" ){
            dataType = 0;
            cout << "Using MC data." << endl;
            fileIn   = fileMC;
        }
        else if (string( gApplication->Argv(iarg))=="-j" || string( gApplication->Argv(iarg))=="--job" ){
            jobMode = 1;
        }
        else if (string( gApplication->Argv(iarg))=="-s" || string( gApplication->Argv(iarg))=="--save" ){
            cout << "Saving selected events." << endl;
            SAVE_EVENTS = true;
            // rm displays before storing new ones:
            system("rm /Users/cjesus/Desktop/SFGD_sel_events/charge_cut/*");
            system("rm /Users/cjesus/Desktop/SFGD_sel_events/original/*");
        }
        else if (string( gApplication->Argv(iarg))=="-d" || string( gApplication->Argv(iarg))=="--display" ){
            cout << "Showing selected events." << endl;
            SHOW_EVENTS = true;
        }
    }


    //=====================================================================================
    //=====================================================================================




  /*

    // I disabled this for the moment, I don't like COLZ option with this Style...
    // TODO: Change default colormap the make own style.
    if(!SAVE_EVENTS and !SHOW_EVENTS and false){
        Int_t T2KstyleIndex = 1;
        // Official T2K style as described in http://www.t2k.org/comm/pubboard/style/index_html
        TString localStyleName = "T2K";
        // -- WhichStyle --
        // 1 = presentation large fonts
        // 2 = presentation small fonts
        // 3 = publication/paper
        Int_t localWhichStyle = T2KstyleIndex;

        TStyle* t2kstyle = SetT2KStyle(localWhichStyle, localStyleName);
        gROOT->SetStyle(t2kstyle->GetName());
        gROOT->ForceStyle(t2kstyle);
    }
*/
    //=====================================================================================
    //=====================================================================================




    // The input file TTree of events is linked to 'inputBranch'.
    //TFile *FileInput  = new TFile(fileIn.Data(),"update");
    TFile *FileInput  = new TFile(fileIn.Data(),"open");
    TTree* data = (TTree*) FileInput->Get("AllEvents");
    Int_t nEvents = data->GetEntries();

    cout << "nEvents: " << nEvents << endl;

    if(!dataType) fileOut += "_MC.root";
    else fileOut += "_Data.root";

    TBranch *inputBranch;
    inputBranch = data->GetBranch("Event");
    ND280SFGDEvent* inputEvent = new ND280SFGDEvent();  //MC event == inputEvent
    Event* unpackEvent = new Event();                   //Data event == unpackEvent

    //MC
    if(!dataType){
        cout << "MC FILE" << endl;
        inputBranch->SetAddress(&inputEvent);
    }
    //Data
    else{
        cout << "DATA FILE" << endl;
        inputBranch->SetAddress(&unpackEvent);
    }

    cout << "Total # of Events: " << nEvents << endl << endl;

    // Loop over events from the input file
    for (int iev=0; iev<nEvents; iev++){
      dEdxXZ.fill(0);
      dEdyZY.fill(0);
        if(iev == maxEvents) break;
        if(selEvents > maxSelEvents) break;
        std::cout << "**** Evt " << iev << " ****" << std::endl;

        cout<<"CHECK 1"<<endl;
        data->GetEntry(iev); //ERROR for MC!

        cout<<"CHECK 2"<<endl;
        inputEvent->MergeHits();
        cout << "NUMHITS:" << endl;
        cout << "numhits:" << inputEvent->GetHits().size() <<endl;
        
        vector <ND280SFGDHit*> listOfHits;
        cout<<"CHECK 3"<<endl;
        if(!dataType) {  //for the MC
            listOfHits = inputEvent->GetHits();
            for (auto hit : listOfHits){
                if(hit->GetView() == 0){
                    hit->SetX(hit->GetX()-0.5+12);
                    hit->SetY(hit->GetY()-0.5+4);
                }
                if(hit->GetView() == 1){
                    hit->SetX(hit->GetX()-0.5+12);
                    hit->SetZ(hit->GetZ()-0.5+24);
                }
                if(hit->GetView() == 2){
                    hit->SetY(hit->GetY()-0.5+4);
                    hit->SetZ(hit->GetZ()-0.5+24);
                }
            }

            if(RM_CROSSTALK){
                vector <ND280SFGDHit*> inputHits = inputEvent->GetHits();
                cout << "REMOVING CROSSTALK." << endl;
                cout << "Hits before removal: " << inputHits.size() << endl;
                inputHits.clear();
                for(size_t ihits=0; ihits<inputEvent->GetHits().size(); ihits++){
                    if(!inputEvent->GetHits()[ihits]->GetxTalkFlag()) inputHits.push_back(inputEvent->GetHits()[ihits]);
                }
                cout << "Hits after removal: " << inputHits.size() << endl;
                listOfHits = inputHits;
            }
        }
        else{   //for the Data
            //if(iev < 20) continue;

            bool jumpNextEvent;
            int maxQYZ = 0;
            TClonesArray * unpackHits = unpackEvent->GetHits();
            for(Int_t ihit=0; ihit<unpackEvent->GetNHits(); ihit++){
                jumpNextEvent = false;
                Hit *hit = (Hit*) unpackHits->At(ihit);
                if(hit->GetDt() < -80 || hit->GetDt() > -60) continue;  //cut on time window
                //if(hit->GetCharge() <= 0 ) { jumpNextEvent = true; break;}

                if(hit->GetView() == 2 && maxQYZ < hit->GetCharge() ) maxQYZ = hit->GetCharge();

                ND280SFGDHit* sfgdHit = new ND280SFGDHit();
                sfgdHit->SetX(hit->GetX());
                sfgdHit->SetY(hit->GetY());
                sfgdHit->SetZ(hit->GetZ());
                sfgdHit->SetDt(hit->GetDt());
                sfgdHit->SetCharge(hit->GetCharge());
                sfgdHit->SetView(hit->GetView());
                sfgdHit->SetMultiplicity(0);
                sfgdHit->SetTrueXTalk(kFALSE);
                sfgdHit->SetPDG(-999);
                sfgdHit->SetTrackID(-999);
                sfgdHit->SetHG_pe(hit->GetHG_pe());
                sfgdHit->SetLG_pe(hit->GetLG_pe());
                listOfHits.push_back(sfgdHit);
            }
            if(jumpNextEvent) {listOfHits.clear();continue;} // cout << "FIFO problem, jumping to next event." << endl;
        }




        //cout << "size: " << listOfHits.size() << endl;
        //if(listOfHits.size() < 60) {listOfHits.clear();continue;}  // not interested in small tracks, we want long ones.
/*
        int sizeOfX = 24;
        int sizeOfY = 8;
        int sizeOfZ = 48;

        int aboveCutXZ [sizeOfX];
        int aboveCutYZ [sizeOfY];
        int lineZ_xz   [sizeOfZ];
        int lineZ_yz   [sizeOfZ];

        for (auto i = 0; i<sizeOfX; i++) aboveCutXZ[i] = 0;
        for (auto i = 0; i<sizeOfY; i++) aboveCutYZ[i] = 0;
        for (auto i = 0; i<sizeOfZ; i++) lineZ_xz[i] = 0;
        for (auto i = 0; i<sizeOfZ; i++) lineZ_yz[i] = 0;

        bool debug = false;

        double maxXZ = 0;
        double maxYZ = 0;
        double maxZ_xz  = 0;
        double maxZ_yz  = 0;

        ND280SFGDHit hitmaxXZ;
        ND280SFGDHit hitmaxYZ;
*/
        //loop over hits
        for(uint ihit=0; ihit<listOfHits.size(); ihit++){
          if(listOfHits[ihit]->GetView() == 0){
            event_XY->Fill(listOfHits[ihit]->GetX(),listOfHits[ihit]->GetY(),listOfHits[ihit]->GetCharge());
          }

          if(listOfHits[ihit]->GetView() == 1){
            event_XZ->Fill(listOfHits[ihit]->GetX(),listOfHits[ihit]->GetZ(),listOfHits[ihit]->GetCharge());
            dEdzXZ->Fill(listOfHits[ihit]->GetZ(),listOfHits[ihit]->GetCharge());
            dEdzSUM->Fill(listOfHits[ihit]->GetZ(),listOfHits[ihit]->GetCharge());
            dEdxXZ[listOfHits[ihit]->GetX()]+=listOfHits[ihit]->GetCharge();
          }

          if(listOfHits[ihit]->GetView() == 2){
            event_ZY->Fill(listOfHits[ihit]->GetZ(),listOfHits[ihit]->GetY(),listOfHits[ihit]->GetCharge());
            dEdzZY->Fill(listOfHits[ihit]->GetZ(),listOfHits[ihit]->GetCharge());
            dEdzSUM->Fill(listOfHits[ihit]->GetZ(),listOfHits[ihit]->GetCharge());
            dEdyZY[listOfHits[ihit]->GetY()]+=listOfHits[ihit]->GetCharge();
          }
        }

        Int_t ZatMax = dEdzZY->GetMaximumBin();
        if (dEdzZY->GetBinContent(ZatMax+1)>0.5*dEdzZY->GetBinContent(ZatMax)) ZatMax++;


        int empty=0;
        for (int Z = 0; Z < ZatMax; Z++ ){
          if (dEdzZY->GetBinContent(Z)==0) empty++;
        }

        Double_t Emax = dEdzZY->GetBinContent(ZatMax);
        if (event_XZ->GetEntries()>10 && empty<3 && dEdzSUM->GetBinContent(47)==0 && Selection(Emax, ZatMax, event_XY, dEdxXZ, dEdyZY)==0){
          selEvents++;
          E_at_ZatMax->Fill(dEdzZY->GetBinContent(ZatMax));
          E_at_ZatMaxPlusOne->Fill(dEdzZY->GetBinContent(ZatMax+1));
          Range->Fill(ZatMax,1);

          for (int i = 0; i<ZatMax; i++){
            //Double_t x = dEdzSUM->GetBinContent(ZatMax-i);
            //Double_t xz = dEdzXZ->GetBinContent(ZatMax-i);
            Double_t zy = dEdzZY->GetBinContent(ZatMax-i);
            //All_Events->Fill(50-i,x);
            //All_Events_XZ->Fill(50-i,xz);
            All_Events_ZY->Fill(50-i,zy);
            //ZYrms->Fill(50-i,zy*zy);
            //if (x>0) {All_Events_norm->Fill(50-i,1);}
            //if (xz>0) {All_Events_norm_XZ->Fill(50-i,1);}
            if (zy>0) {All_Events_norm_ZY->Fill(50-i,1);}
          }

          for (int i = 1; i<=47-ZatMax; i++){
            //Double_t x = dEdzSUM->GetBinContent(ZatMax+i);
            //Double_t xz = dEdzXZ->GetBinContent(ZatMax+i);
            Double_t zy = dEdzZY->GetBinContent(ZatMax+i);
            //All_Events->Fill(50+i,x);
            //All_Events_XZ->Fill(50+i,xz);
            All_Events_ZY->Fill(50+i,zy);
            //ZYrms->Fill(50+i,zy*zy);
            //if (x>0) {All_Events_norm->Fill(50+i,1);}
            //if (xz>0) {All_Events_norm_XZ->Fill(50+i,1);}
            if (zy>0) {All_Events_norm_ZY->Fill(50+i,1);}
          }

        }

        event_XY->Reset();
        event_XZ->Reset();
        event_ZY->Reset();
        dEdzXZ->Reset();
        dEdzZY->Reset();
        dEdzSUM->Reset();








        // cout << endl;
        // cout << "length XZ: " << maxLineZ_xz << endl;
        // cout << "length YZ: " << maxLineZ_yz << endl;
        // cout << "width  XZ: " << cntXZ << endl;
        // cout << "width  YZ: " << cntYZ << endl;
        // cout << "dedz : "     << dedz << endl;
        // cout << endl;



        //std::cout << "**********************" << std::endl << std::endl;

    }

    cout << "Selected events:    " << selEvents << endl;

    /// ------------END--------------
     FileInput->Close();
     TFile *FileOutput  = new TFile(fileOut.Data(),"RECREATE");
     FileOutput->cd();
     Range->Write();
     E_at_ZatMax->Write();
     E_at_ZatMaxPlusOne->Write();





     TH1D *All_Events_normalized_ZY = (TH1D*)All_Events_ZY->Clone("All_Events_normalized_ZY");
     All_Events_normalized_ZY->Divide(All_Events_norm_ZY);
     All_Events_normalized_ZY->SetLineColor(46);

     All_Events_ZY->GetXaxis()->SetLimits(-50,50);
     All_Events_normalized_ZY->GetXaxis()->SetLimits(-50,50);

     All_Events_normalized_ZY->GetXaxis()->SetTitle("Z position relative to peak [cm]");
     All_Events_normalized_ZY->GetYaxis()->SetTitle("Mean deposited energy [p.e.]");
     All_Events_normalized_ZY->SetLineColor(46);
     All_Events_normalized_ZY->Write();

     FileOutput->Close();

    time = ( clock() - time ) / CLOCKS_PER_SEC;
    cout << std::setprecision(8) << "Elapsed time: " << time << endl;

    if(!jobMode){
        cout << "This macro remains open until it is manually closed with Ctrl+C\nallowing to watch the output on the output Canvas.\nTo avoid this, use option '-j'." << endl;
        return;
    }
    else{
        cout << "Execution completed successfully." << endl << endl;
        exit(1);
    }
}