#define THIS_NAME crosstalkAna
#define NOINTERACTIVE_OUTPUT
#define OVERRIDE_OPTIONS

#include "../utils/global_header.h"
#include "../utils/global_tools.C"
#include <time.h> 
#include <TMath.h>
#include <TF1.h>

void crosstalkAna() {

    cout << endl << "Staring the execution." << endl;

    // STRUCTURE of the crosstalkAna macro:

    // 1. some variables definitions.
    // 2. command line argument options parsing
    // 3. we open the input file an read each event
    // 4. for each event we apply some selection criteria
    // 5. we fill some histograms with the information for the selected events.
    // 6. we store plots

    // NOTE: In case of using option -s || --save it is necessary to update harcoded paths.

    gStyle->SetOptStat(0);
    //TString fileIn = "/Users/cjesus/Documents/PhD/SFGD/25August_8/25August_8_MCR0_hadrons_0pt8Gev_0pt0T_Beam___NewStructure.root";
    TString fileIn = "/Users/cjesus/Documents/Data/SFGD_BTEST_2018/25August_11_MCR0_hadrons_0pt8Gev_0pt0T_Beam___NewStructure.root";

    int maxEvents    = std::numeric_limits<int>::max();
    int maxSelEvents = std::numeric_limits<int>::max();
    int maxStore     = 100;
    int q_Cut        = 100;
    int selEvents    = 0;
    int selLI        = 0;
    int selHI        = 0;
    int selSP        = 0;
    int jobMode      = 0;
    bool SAVE_EVENTS = false;
    bool SHOW_EVENTS = false;
    float time       = clock();

    // Canvas for plots:

    TH1F* h_dedz = new TH1F("dedz","dedz",100,0,250);

    // LI == Low Ionizing track
    // HI == High Ionizing track

    // Bins of 1 PE

    TH1F* h_PE_xz [2];
    h_PE_xz [0] = new TH1F("h_PE_xz_LI","",q_Cut,0,q_Cut);
    h_PE_xz [1] = new TH1F("h_PE_xz_HI","",q_Cut,0,q_Cut);
    TH1F* h_PE_yz [2];
    h_PE_yz [0] = new TH1F("h_PE_yz_LI","",q_Cut,0,q_Cut);
    h_PE_yz [1] = new TH1F("h_PE_yz_HI","",q_Cut,0,q_Cut);

    // RQ: Relative Charge
    // Bins of 1% from 0 to 50%

    TH1F* h_RQ_xz [2]; 
    h_RQ_xz [0] = new TH1F("h_RQ_xz_LI","",50,0,50);
    h_RQ_xz [1] = new TH1F("h_RQ_xz_HI","",50,0,50);
    TH1F* h_RQ_yz [2]; 
    h_RQ_yz [0] = new TH1F("h_RQ_yz_LI","",50,0,50);
    h_RQ_yz [1] = new TH1F("h_RQ_yz_HI","",50,0,50);

    // SP: Stopping protons

    // F: Far from stopping
    // A: Almost stopping
    // S: Stopping

    TH1F* h_SP_PE_xz [3]; 
    h_SP_PE_xz [0] = new TH1F("h_SP_PE_xz_F","h_SP_PE_xz_F",100,0,100);
    h_SP_PE_xz [1] = new TH1F("h_SP_PE_xz_A","h_SP_PE_xz_A",100,0,100);
    h_SP_PE_xz [2] = new TH1F("h_SP_PE_xz_S","h_SP_PE_xz_S",100,0,100);

    TH1F* h_SP_PE_yz [3]; 
    h_SP_PE_yz [0] = new TH1F("h_SP_PE_yz_F","h_SP_PE_yz_F",100,0,100);
    h_SP_PE_yz [1] = new TH1F("h_SP_PE_yz_A","h_SP_PE_yz_A",100,0,100);
    h_SP_PE_yz [2] = new TH1F("h_SP_PE_yz_S","h_SP_PE_yz_S",100,0,100);

    TH1F* h_SP_RQ_xz [3]; 
    h_SP_RQ_xz [0] = new TH1F("h_SP_RQ_xz_F","h_SP_RQ_xz_F",50,0,50);
    h_SP_RQ_xz [1] = new TH1F("h_SP_RQ_xz_A","h_SP_RQ_xz_A",50,0,50);
    h_SP_RQ_xz [2] = new TH1F("h_SP_RQ_xz_S","h_SP_RQ_xz_S",50,0,50);

    TH1F* h_SP_RQ_yz [3]; 
    h_SP_RQ_yz [0] = new TH1F("h_SP_RQ_yz_F","h_SP_RQ_yz_F",50,0,50);
    h_SP_RQ_yz [1] = new TH1F("h_SP_RQ_yz_A","h_SP_RQ_yz_A",50,0,50);
    h_SP_RQ_yz [2] = new TH1F("h_SP_RQ_yz_S","h_SP_RQ_yz_S",50,0,50);

    /// ------------START--------------

    // The command line arguments are parsed
    for (int iarg=0; iarg<gApplication->Argc(); iarg++){
        if (string( gApplication->Argv(iarg))=="-h" || string( gApplication->Argv(iarg))=="--help" ){
            cout << "**************************************" << endl;
            cout << "Macros run options:" << endl;
            cout << "   -h || --help      print help info." << endl;
            cout << "   -i || --input     input file (*.root)." << endl;
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

    if(!SAVE_EVENTS and !SHOW_EVENTS){
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

    // The input file TTree of events is linked to 'unpackEvent' variable.
    TFile *FileInput  = new TFile(fileIn.Data(),"update");
    TTree* data = (TTree*) FileInput->Get("AllEvents");
    Int_t nEvents = data->GetEntries();
    TBranch *inputBranch;
    inputBranch = data->GetBranch("Event");
    Event* unpackEvent = new Event();
    inputBranch->SetAddress(&unpackEvent);

    cout << "Total # of Events: " << nEvents << endl << endl; 

    // Loop over events from the input file
    for (int iev=0; iev<nEvents; iev++){
        if(iev == maxEvents) break;
        if(iev > maxSelEvents) break;
        std::cout << "**** Evt " << iev << " ****" << std::endl;

        data->GetEntry(iev);

        if(iev < 20) continue;

        bool jumpNextEvent;
        int maxQYZ = 0;
        TClonesArray * unpackHits = unpackEvent->GetHits();
        vector <ND280SFGDHit*> auxHits;
        for(Int_t ihit=0; ihit<unpackEvent->GetNHits(); ihit++){
            jumpNextEvent = false;
            Hit *hit = (Hit*) unpackHits->At(ihit);
            if(hit->GetDt() < -100 || hit->GetDt() > -60) continue;
            if(hit->GetCharge() <= 0 ) {/*cout << "Q: " << hit->GetCharge() << endl; hit->SetCharge(10000);*/ jumpNextEvent = true; break;}

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
            auxHits.push_back(sfgdHit);
        }
        if(jumpNextEvent) {auxHits.clear();continue;} // cout << "FIFO problem, jumping to next event." << endl; 
        if(auxHits.size() < 60) {auxHits.clear();continue;}  // not interested in small tracks, we want long ones.
        
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

        for(uint ihit=0; ihit<auxHits.size(); ihit++){
            if(auxHits[ihit]->GetView() == 1){
                lineZ_xz[int(auxHits[ihit]->GetZ())] = 1;
                if(auxHits[ihit]->GetCharge()>maxXZ) {
                    maxXZ = auxHits[ihit]->GetCharge(); 
                    maxZ_xz = auxHits[ihit]->GetZ();
                    hitmaxXZ = *auxHits[ihit];
                }
                if(aboveCutXZ[int(auxHits[ihit]->GetX())] == 2) continue;
                if(auxHits[ihit]->GetCharge() > q_Cut) aboveCutXZ[int(auxHits[ihit]->GetX())] = 2;
                else aboveCutXZ[int(auxHits[ihit]->GetX())] = 1;
            }

            if(auxHits[ihit]->GetView() == 2){
                lineZ_yz[int(auxHits[ihit]->GetZ())] = 1;
                if(auxHits[ihit]->GetCharge()>maxYZ) {
                    maxYZ = auxHits[ihit]->GetCharge(); 
                    maxZ_yz = auxHits[ihit]->GetZ();
                    hitmaxYZ = *auxHits[ihit];
                }
                if(aboveCutYZ[int(auxHits[ihit]->GetY())] == 2) continue;
                if(auxHits[ihit]->GetCharge() > q_Cut) aboveCutYZ[int(auxHits[ihit]->GetY())] = 2;
                else aboveCutYZ[int(auxHits[ihit]->GetY())] = 1;
            }
        }

        int cntXZ = 0;
        int cntYZ = 0;
        for(auto s = 0; s<sizeOfX; s++) if(aboveCutXZ[s] == 2) cntXZ++;
        for(auto s = 0; s<sizeOfX; s++) if(aboveCutYZ[s] == 2) cntYZ++;

        int maxLineZ_xz = 0;
        int temp_maxLineZ_xz = 0;
        for(auto s = 1; s<sizeOfZ; s++){
            if(lineZ_xz[s] != 0) {
                temp_maxLineZ_xz++;
            }
            else temp_maxLineZ_xz = 0;
            if (temp_maxLineZ_xz > maxLineZ_xz){
                maxLineZ_xz = temp_maxLineZ_xz;
            }
        }
        double dedz = 0;
        for(uint ihit=0; ihit<auxHits.size(); ihit++){
            if(auxHits[ihit]->GetView() == 1){
                dedz += auxHits[ihit]->GetCharge();
            }
        }
        if (maxLineZ_xz) dedz /= maxLineZ_xz;

        int maxLineZ_yz = 0;
        int temp_maxLineZ_yz = 0;
        for(auto s = 1; s<sizeOfZ; s++){            
            if(lineZ_yz[s] != 0) temp_maxLineZ_yz++;
            else temp_maxLineZ_yz = 0;
            if (temp_maxLineZ_yz > maxLineZ_yz){
                maxLineZ_yz = temp_maxLineZ_yz;
            }
        }

        if(debug){
            cout << endl;
            cout << "length XZ: " << maxLineZ_xz << endl;
            cout << "length YZ: " << maxLineZ_yz << endl;
            cout << "width  XZ: " << cntXZ << endl;
            cout << "width  YZ: " << cntYZ << endl;
            cout << "dedz : "     << dedz << endl;
            cout << endl;
        }

        if(cntXZ > 2 or cntYZ > 3) continue;
        if(maxLineZ_xz < 30 or maxLineZ_yz < 30) continue;

        if(debug){        
            cout << endl << "XZ: " << endl;
            for(auto s = 0; s<sizeOfZ; s++)
                cout << aboveCutXZ[s] << endl;

            cout << "YZ: " << endl;
            for(auto s = 0; s<sizeOfZ; s++)
                cout << aboveCutYZ[s] << endl;
        }

        h_dedz->Fill(dedz);

        TString evt_tag = "";
        int mode = -1;        // 0 is LI, 1 is HI, 2 is SP
        if(dedz < 80) {evt_tag = "LI"; selLI++; mode = 0;}
        else if(dedz > 130) {evt_tag = "HI"; selHI++; mode = 1;}
        else continue;
        if(mode <0) continue;

        bool stopping = false;
        if(lineZ_xz[sizeOfZ-1] == 0 && lineZ_xz[sizeOfZ] == 0) stopping = true;
        if( stopping && mode == 1) selSP++;

        for(uint ihit = 0; ihit < auxHits.size(); ihit++){
            if( auxHits[ihit]->GetView() == 0 ) continue;
            else if( auxHits[ihit]->GetView() == 1 ){

                // hit at maxZ:
                if(abs(maxZ_xz - auxHits[ihit]->GetZ()) == 0){
                    // continue if it is crosstalk:
                    if(aboveCutXZ[int(auxHits[ihit]->GetX())] == 2) continue; 
                    // fill crosstalk PE distribution for the XZ plane
                    if(mode == 1 && stopping) h_SP_PE_xz[2]->Fill(auxHits[ihit]->GetCharge()); 
                    // search hits in X-1 and X+1:
                    for(uint jhit=0; jhit<auxHits.size(); jhit++){
                        if (auxHits[ihit]->GetView() != 1) continue;
                        if( abs(auxHits[ihit]->GetX()-auxHits[jhit]->GetX()) == 1 && abs(auxHits[ihit]->GetZ()-auxHits[jhit]->GetZ()) == 0 && auxHits[jhit]->GetCharge() > q_Cut){
                            if(mode == 1 && stopping) h_SP_RQ_xz[2]->Fill( 100.*auxHits[ihit]->GetCharge()/auxHits[jhit]->GetCharge()); 
                        }
                    }
                }
                else if( maxZ_xz - auxHits[ihit]->GetZ()  == 1){
                    // continue if it is crosstalk:
                    if(aboveCutXZ[int(auxHits[ihit]->GetX())] == 2) continue; 
                    // fill crosstalk PE distribution for the XZ plane
                    if(mode == 1 && stopping) h_SP_PE_xz[1]->Fill(auxHits[ihit]->GetCharge()); 
                    // search hits in X-1 and X+1:
                    for(uint jhit=0; jhit<auxHits.size(); jhit++){
                        if (auxHits[ihit]->GetView() != 1) continue;
                        if( abs(auxHits[ihit]->GetX()-auxHits[jhit]->GetX()) == 1 && abs(auxHits[ihit]->GetZ()-auxHits[jhit]->GetZ()) == 0 && auxHits[jhit]->GetCharge() > q_Cut){
                            if(mode == 1 && stopping) h_SP_RQ_xz[1]->Fill( 100.*auxHits[ihit]->GetCharge()/auxHits[jhit]->GetCharge()); 
                        }
                    }
                }

                else if(abs(maxZ_xz - auxHits[ihit]->GetZ()) > 2){
                    // continue if it is crosstalk:
                    if(aboveCutXZ[int(auxHits[ihit]->GetX())] == 2) continue; 
                    // fill crosstalk PE distribution for the XZ plane
                    h_PE_xz[mode]->Fill(auxHits[ihit]->GetCharge());
                    if(mode == 1 && stopping) h_SP_PE_xz [0]->Fill(auxHits[ihit]->GetCharge()); 
                    // search hits in X-1 and X+1:
                    for(uint jhit=0; jhit<auxHits.size(); jhit++){
                        if (auxHits[ihit]->GetView() != 1) continue;
                        if( abs(auxHits[ihit]->GetX()-auxHits[jhit]->GetX()) == 1 && abs(auxHits[ihit]->GetZ()-auxHits[jhit]->GetZ()) == 0 && auxHits[jhit]->GetCharge() > q_Cut){
                            h_RQ_xz[mode]->Fill( 100.*auxHits[ihit]->GetCharge()/auxHits[jhit]->GetCharge());
                            if(mode == 1 && stopping) h_SP_RQ_xz[0]->Fill( 100.*auxHits[ihit]->GetCharge()/auxHits[jhit]->GetCharge());
                        }
                    }
                }
            }
            else if( auxHits[ihit]->GetView() == 2 ){
                // hit at maxZ:
                if(abs(maxZ_yz - auxHits[ihit]->GetZ()) == 0){
                    // continue if it is crosstalk:
                    if(aboveCutYZ[int(auxHits[ihit]->GetY())] == 2) continue; 
                    // fill crosstalk PE distribution for the YZ plane
                    if(mode == 1 && stopping) h_SP_PE_yz[2]->Fill(auxHits[ihit]->GetCharge()); 
                    // search hits in Y-1 and Y+1:
                    for(uint jhit=0; jhit<auxHits.size(); jhit++){
                        if (auxHits[ihit]->GetView() != 2) continue;
                        if( abs(auxHits[ihit]->GetY()-auxHits[jhit]->GetY()) == 1 && abs(auxHits[ihit]->GetZ()-auxHits[jhit]->GetZ()) == 0 && auxHits[jhit]->GetCharge() > q_Cut){
                            if(mode == 1 && stopping) h_SP_RQ_yz [2]->Fill( 100.*auxHits[ihit]->GetCharge()/auxHits[jhit]->GetCharge()); 
                        }
                    }
                }
                else if( maxZ_yz - auxHits[ihit]->GetZ()  == 1){
                    // continue if it is crosstalk:
                    if(aboveCutYZ[int(auxHits[ihit]->GetY())] == 2) continue; 
                    // fill crosstalk PE distribution for the YZ plane
                    if(mode == 1 && stopping) h_SP_PE_yz[1]->Fill(auxHits[ihit]->GetCharge()); 
                    // search hits in Y-1 and Y+1:
                    for(uint jhit=0; jhit<auxHits.size(); jhit++){
                        if (auxHits[ihit]->GetView() != 2) continue;
                        if( abs(auxHits[ihit]->GetY()-auxHits[jhit]->GetY()) == 1 && abs(auxHits[ihit]->GetZ()-auxHits[jhit]->GetZ()) == 0 && auxHits[jhit]->GetCharge() > q_Cut){
                            if(mode == 1 && stopping) h_SP_RQ_yz[1]->Fill( 100.*auxHits[ihit]->GetCharge()/auxHits[jhit]->GetCharge()); 
                        }
                    }
                }

                else if(abs(maxZ_yz - auxHits[ihit]->GetZ()) > 2){
                    // continue if it is crosstalk:
                    if(aboveCutYZ[int(auxHits[ihit]->GetY())] == 2) continue; 
                    // fill crosstalk PE distribution for the YZ plane
                    h_PE_yz[mode]->Fill(auxHits[ihit]->GetCharge());
                    if(mode == 1 && stopping) h_SP_PE_yz [0]->Fill(auxHits[ihit]->GetCharge()); 
                    // search hits in Y-1 and Y+1:
                    for(uint jhit=0; jhit<auxHits.size(); jhit++){
                        if (auxHits[ihit]->GetView() != 2) continue;
                        if( abs(auxHits[ihit]->GetY()-auxHits[jhit]->GetY()) == 1 && abs(auxHits[ihit]->GetZ()-auxHits[jhit]->GetZ()) == 0 && auxHits[jhit]->GetCharge() > q_Cut){
                            h_RQ_yz[mode]->Fill( 100.*auxHits[ihit]->GetCharge()/auxHits[jhit]->GetCharge());
                            if(mode == 1 && stopping) h_SP_RQ_yz[0]->Fill( 100.*auxHits[ihit]->GetCharge()/auxHits[jhit]->GetCharge());
                        }
                    }
                }

            }
        }

        if( (SAVE_EVENTS and ( ( mode == 0 && (selLI-1 < maxStore)) or ( mode == 1 && (selHI-1 < maxStore)) )) or SHOW_EVENTS){
            TCanvas* ccc = new TCanvas("C"); 
            ccc->Divide(2,2);
            TString hXYname = "viewXY";
            TString hXZname = "viewXZ";
            TString hYZname = "viewYZ";

            TH2F *hXY = new TH2F(hXYname.Data(),hXYname.Data(), 220, -110., 110, 220, -110., 110);      
            TH2F *hXZ = new TH2F(hXZname.Data(),hXZname.Data(), 220, -110., 110, 220, -110., 110);
            TH2F *hYZ = new TH2F(hYZname.Data(),hYZname.Data(), 220, -110., 110, 220, -110., 110);

            Double_t Qmax1 = 0;
            Double_t Qmax2 = 0;
            Double_t Qmax3 = 0;

            Int_t maxX = -999;
            Int_t maxY = -999;
            Int_t maxZ = -999;
            Int_t minX = 999;
            Int_t minY = 999;
            Int_t minZ = 999;

            Int_t tolerance = 0;

            for(size_t i=0; i<auxHits.size(); i++){
                ND280SFGDHit* hit = auxHits[i];

                if(hit->GetView() == 0){
                    hXY->Fill(hit->GetX(),hit->GetY(),hit->GetCharge());
                    if(hit->GetCharge()>Qmax1) Qmax1 = hit->GetCharge();
                    if(hit->GetY()>maxY) maxY = hit->GetY();
                    if(hit->GetY()<minY) minY = hit->GetY();
                }
                if(hit->GetView() == 1){
                    hXZ->Fill(hit->GetX(),hit->GetZ(),hit->GetCharge());
                    if(hit->GetCharge()>Qmax2) Qmax2 = hit->GetCharge();
                    if(hit->GetX()>maxX) maxX = hit->GetX();
                    if(hit->GetX()<minX) minX = hit->GetX();
                }
                if(hit->GetView() == 2){
                    hYZ->Fill(hit->GetY(),hit->GetZ(),hit->GetCharge());
                    if(hit->GetCharge()>Qmax3) Qmax3 = hit->GetCharge();
                    if(hit->GetZ()>maxZ) maxZ = hit->GetZ();
                    if(hit->GetZ()<minZ) minZ = hit->GetZ();
                }
            }

            hXY->GetZaxis()->SetRangeUser(0.1,q_Cut);
            hXZ->GetZaxis()->SetRangeUser(0.1,q_Cut);
            hYZ->GetZaxis()->SetRangeUser(0.1,q_Cut);

            ccc->cd(1);
            hXY->Draw("COLZ");
            hXY->GetXaxis()->SetRangeUser(0,sizeOfX);
            hXY->GetYaxis()->SetRangeUser(0,sizeOfY);
            ccc->cd(2);
            hXZ->Draw("COLZ");
            hXZ->GetXaxis()->SetRangeUser(0,sizeOfX);
            hXZ->GetYaxis()->SetRangeUser(0,sizeOfZ);
            ccc->cd(3);
            hYZ->Draw("COLZ");
            hYZ->GetXaxis()->SetRangeUser(0,sizeOfY);
            hYZ->GetYaxis()->SetRangeUser(0,sizeOfZ);

            // Different way to plot tracks: centered and with 'homogeneous' spaces around.
            // 'homogenous': same distance from track limits to plot margins.
            if(false){
                ccc->cd(1);
                hXY->Draw("COLZ");
                hXY->GetXaxis()->SetRangeUser(minX-tolerance,maxX+tolerance);
                hXY->GetYaxis()->SetRangeUser(minY-tolerance,maxY+tolerance);
                ccc->cd(2);
                hXZ->Draw("COLZ");
                hXZ->GetXaxis()->SetRangeUser(minX-tolerance,maxX+tolerance);
                hXZ->GetYaxis()->SetRangeUser(minZ-tolerance,maxZ+tolerance);
                ccc->cd(3);
                hYZ->Draw("COLZ");
                hYZ->GetXaxis()->SetRangeUser(minY-tolerance,maxY+tolerance);
                hYZ->GetYaxis()->SetRangeUser(minZ-tolerance,maxZ+tolerance);
            }

            ccc->Update();
            
            std::string name = "/Users/cjesus/Desktop/SFGD_sel_events/";
            TString tname = "";
            tname += name.c_str();
            tname += evt_tag;
            tname += "_";
            if (dedz < 80) tname += std::to_string(selLI-1);
            else tname += std::to_string(selHI-1);
            tname += "_qCut";
            tname += ".pdf";
            if (dedz < 80 &&  selLI-1 < maxStore) ccc->SaveAs(tname.Data());
            if (dedz > 130 && selHI-1 < maxStore) ccc->SaveAs(tname.Data());

            hXY->GetZaxis()->SetRangeUser(0.1,1.05*Qmax1);
            hXZ->GetZaxis()->SetRangeUser(0.1,1.05*Qmax2);
            hYZ->GetZaxis()->SetRangeUser(0.1,1.05*Qmax3);

            ccc->cd(1);
            hXY->Draw("COLZ");
            hXY->GetXaxis()->SetRangeUser(0,sizeOfX);
            hXY->GetYaxis()->SetRangeUser(0,sizeOfY);
            ccc->cd(2);
            hXZ->Draw("COLZ");
            hXZ->GetXaxis()->SetRangeUser(0,sizeOfX);
            hXZ->GetYaxis()->SetRangeUser(0,sizeOfZ);
            ccc->cd(3);
            hYZ->Draw("COLZ");
            hYZ->GetXaxis()->SetRangeUser(0,sizeOfY);
            hYZ->GetYaxis()->SetRangeUser(0,sizeOfZ);

            // Different way to plot tracks: centered and with 'homogeneous' spaces around.
            // 'homogenous': same distance from track limits to plot margins.
            if(false){
                ccc->cd(1);
                hXY->Draw("COLZ");
                hXY->GetXaxis()->SetRangeUser(minX-tolerance,maxX+tolerance);
                hXY->GetYaxis()->SetRangeUser(minY-tolerance,maxY+tolerance);
                ccc->cd(2);
                hXZ->Draw("COLZ");
                hXZ->GetXaxis()->SetRangeUser(minX-tolerance,maxX+tolerance);
                hXZ->GetYaxis()->SetRangeUser(minZ-tolerance,maxZ+tolerance);
                ccc->cd(3);
                hYZ->Draw("COLZ");
                hYZ->GetXaxis()->SetRangeUser(minY-tolerance,maxY+tolerance);
                hYZ->GetYaxis()->SetRangeUser(minZ-tolerance,maxZ+tolerance);
            }

            name = "/Users/cjesus/Desktop/SFGD_sel_events/";
            tname = "";
            tname += name.c_str();
            tname += evt_tag;
            tname += "_";
            if (dedz < 80) tname += std::to_string(selLI-1);
            else tname += std::to_string(selHI-1);
            tname += "_original";
            tname += ".pdf";
            if (dedz < 80 &&  selLI-1 < maxStore) ccc->SaveAs(tname.Data());
            if (dedz > 130 && selHI-1 < maxStore) ccc->SaveAs(tname.Data());

            ccc->Update();
            if(SHOW_EVENTS) ccc->WaitPrimitive();
            ccc->Close();

            delete hXY;
            delete hXZ;
            delete hYZ;
            delete ccc;
        }

        std::cout << "**********************" << std::endl << std::endl;
        selEvents++;
    }

    cout << "Selected events:    " << selEvents << endl;
    cout << "Selected LI events: " << selLI << endl;
    cout << "Selected HI events: " << selHI << endl;
    cout << "Selected SP events: " << selSP << endl;

    TCanvas* b_1 = new TCanvas("SP_1");
    b_1->Divide(3,2); 

    if(h_SP_PE_xz[0]->GetEntries()) h_SP_PE_xz[0]->Scale(1/h_SP_PE_xz[0]->GetEntries(), "width");
    if(h_SP_PE_xz[1]->GetEntries()) h_SP_PE_xz[1]->Scale(1/h_SP_PE_xz[1]->GetEntries(), "width");
    if(h_SP_PE_xz[2]->GetEntries()) h_SP_PE_xz[2]->Scale(1/h_SP_PE_xz[2]->GetEntries(), "width");    
    if(h_SP_PE_yz[0]->GetEntries()) h_SP_PE_yz[0]->Scale(1/h_SP_PE_yz[0]->GetEntries(), "width");
    if(h_SP_PE_yz[1]->GetEntries()) h_SP_PE_yz[1]->Scale(1/h_SP_PE_yz[1]->GetEntries(), "width");
    if(h_SP_PE_yz[2]->GetEntries()) h_SP_PE_yz[2]->Scale(1/h_SP_PE_yz[2]->GetEntries(), "width");

    cout << "SP_PE_XZ_F entires: " << h_SP_PE_xz[0]->GetEntries() << endl;
    cout << "SP_PE_XZ_A entires: " << h_SP_PE_xz[1]->GetEntries() << endl;
    cout << "SP_PE_XZ_S entires: " << h_SP_PE_xz[2]->GetEntries() << endl;
    cout << "SP_PE_YZ_F entires: " << h_SP_PE_yz[0]->GetEntries() << endl;
    cout << "SP_PE_YZ_A entires: " << h_SP_PE_yz[1]->GetEntries() << endl;
    cout << "SP_PE_YZ_S entires: " << h_SP_PE_yz[2]->GetEntries() << endl;

    h_SP_PE_xz[0]->SetLineWidth(2);
    h_SP_PE_yz[0]->SetLineWidth(2);
    h_SP_PE_xz[1]->SetLineWidth(2);
    h_SP_PE_yz[1]->SetLineWidth(2);
    h_SP_PE_xz[2]->SetLineWidth(2);
    h_SP_PE_yz[2]->SetLineWidth(2);
    h_SP_PE_xz[0]->SetLineColor(kBlack);
    h_SP_PE_yz[0]->SetLineColor(kRed);
    h_SP_PE_xz[1]->SetLineColor(kBlue);
    h_SP_PE_yz[1]->SetLineColor(kGreen+1);
    h_SP_PE_xz[2]->SetLineColor(kCyan);
    h_SP_PE_yz[2]->SetLineColor(kOrange);

    b_1->cd(1);
    h_SP_PE_yz[0]->Draw("HIST");

    b_1->cd(2);
    h_SP_PE_yz[1]->Draw("HIST");

    b_1->cd(3);
    h_SP_PE_yz[2]->Draw("HIST");

    b_1->cd(4);
    h_SP_PE_xz[0]->Draw("HIST");

    b_1->cd(5);
    h_SP_PE_xz[1]->Draw("HIST");

    b_1->cd(6);
    h_SP_PE_xz[2]->Draw("HIST");

    b_1->Update();

    TCanvas* b_2 = new TCanvas("SP_2");
    b_2->Divide(2,1); 

    b_2->cd(1);
    gPad->SetTitle("");
    h_SP_PE_yz[0]->Draw("HIST");
    h_SP_PE_yz[1]->Draw("HIST same");
    h_SP_PE_yz[2]->Draw("HIST same");

    b_2->cd(2);
    gPad->SetTitle("");
    h_SP_PE_xz[0]->Draw("HIST");
    h_SP_PE_xz[1]->Draw("HIST same");
    h_SP_PE_xz[2]->Draw("HIST same");

    b_2->Update();

    TCanvas* c = new TCanvas("PE");
    c->Divide(4,2); 

    if(h_PE_xz[0]->GetEntries()) h_PE_xz[0]->Scale(1/h_PE_xz[0]->GetEntries(), "width");
    if(h_PE_xz[1]->GetEntries()) h_PE_xz[1]->Scale(1/h_PE_xz[1]->GetEntries(), "width");
    if(h_PE_yz[0]->GetEntries()) h_PE_yz[0]->Scale(1/h_PE_yz[0]->GetEntries(), "width");
    if(h_PE_yz[1]->GetEntries()) h_PE_yz[1]->Scale(1/h_PE_yz[1]->GetEntries(), "width");

    cout << "PE_XZ_LI entires: " << h_PE_xz[0]->GetEntries() << endl;
    cout << "PE_XZ_HI entires: " << h_PE_xz[1]->GetEntries() << endl;
    cout << "PE_YZ_LI entires: " << h_PE_yz[0]->GetEntries() << endl;
    cout << "PE_YZ_HI entires: " << h_PE_yz[1]->GetEntries() << endl;

    h_PE_xz[0]->SetLineWidth(2);
    h_PE_yz[0]->SetLineWidth(2);
    h_PE_xz[1]->SetLineWidth(2);
    h_PE_yz[1]->SetLineWidth(2);
    h_PE_xz[0]->SetLineColor(kBlack);
    h_PE_yz[0]->SetLineColor(kRed);
    h_PE_xz[1]->SetLineColor(kBlue);
    h_PE_yz[1]->SetLineColor(kGreen+1);

    TF1 *f1 = new TF1("f1","[0]*TMath::Power(([1]/[2]),(x/[2]))*(TMath::Exp(-([1]/[2])))/TMath::Gamma((x/[2])+1) + [3]*TMath::Power(([4]/[5]),(x/[5]))*(TMath::Exp(-([4]/[5])))/TMath::Gamma((x/[5])+1)",0,20);
    f1->SetParameters(0.2, 3.3, 1.35, 0.1,5,2);
    
    TF1 *f2 = new TF1("f2","[0]*TMath::Power(([1]/[2]),(x/[2]))*(TMath::Exp(-([1]/[2])))/TMath::Gamma((x/[2])+1)",0,20);
    f2->SetParameters(0.2, 3.3, 1.35);

    c->cd(1);
    h_PE_yz[0]->Draw("HIST");

    c->cd(2);
    h_PE_yz[1]->Draw("HIST");
    h_PE_yz[1]->Fit("f2","",0,8);
    f2->DrawCopy("same");

    c->cd(3);
    h_PE_xz[0]->Draw("HIST");
    h_PE_xz[0]->Fit("f1","",0,6);
    f1->DrawCopy("same");

    c->cd(4);
    h_PE_xz[1]->Draw("HIST");
    f1->SetParameters(0.13, 4.7,1.93,0.1,5,2);
    h_PE_xz[1]->Fit("f1","",0,8);
    f1->DrawCopy("same");

    c->cd(5);
    h_PE_yz[1]->Draw("HIST");
    h_PE_yz[0]->Draw("HIST same");

    c->cd(6);
    h_PE_xz[1]->Draw("HIST");
    h_PE_xz[0]->Draw("HIST same");

    c->cd(7);
    h_PE_xz[0]->Draw("HIST");
    h_PE_yz[0]->Draw("HIST same");

    c->cd(8);
    h_PE_yz[1]->Draw("HIST");
    h_PE_xz[1]->Draw("HIST same");

    c->Update();

    TCanvas* d = new TCanvas("RQ");
    d->Divide(4,2); 

    if(h_RQ_xz[0]->GetEntries()) h_RQ_xz[0]->Scale(1/h_RQ_xz[0]->GetEntries(), "width");
    if(h_RQ_xz[1]->GetEntries()) h_RQ_xz[1]->Scale(1/h_RQ_xz[1]->GetEntries(), "width");
    if(h_RQ_yz[0]->GetEntries()) h_RQ_yz[0]->Scale(1/h_RQ_yz[0]->GetEntries(), "width");
    if(h_RQ_yz[1]->GetEntries()) h_RQ_yz[1]->Scale(1/h_RQ_yz[1]->GetEntries(), "width");

    cout << "RQ_XZ_LI entires: " << h_RQ_xz[0]->GetEntries() << endl;
    cout << "RQ_XZ_HI entires: " << h_RQ_xz[1]->GetEntries() << endl;
    cout << "RQ_YZ_LI entires: " << h_RQ_yz[0]->GetEntries() << endl;
    cout << "RQ_YZ_HI entires: " << h_RQ_yz[1]->GetEntries() << endl;
   
    h_RQ_yz[0]->SetLineWidth(2);
    h_RQ_yz[0]->SetLineWidth(2);
    h_RQ_xz[1]->SetLineWidth(2);
    h_RQ_yz[1]->SetLineWidth(2);
    h_RQ_xz[0]->SetLineColor(kBlack);
    h_RQ_yz[0]->SetLineColor(kRed);
    h_RQ_xz[1]->SetLineColor(kBlue);
    h_RQ_yz[1]->SetLineColor(kGreen+1);

    d->cd(1);
    gPad->SetTitle("");
    h_RQ_yz[0]->Draw("HIST");

    d->cd(2);
    h_RQ_yz[1]->Draw("HIST");

    d->cd(3);
    h_RQ_xz[0]->Draw("HIST");

    d->cd(4);
    h_RQ_xz[1]->Draw("HIST");

    d->cd(5);
    h_RQ_yz[1]->Draw("HIST");
    h_RQ_yz[0]->Draw("HIST same");

    d->cd(6);
    h_RQ_xz[1]->Draw("HIST");
    h_RQ_xz[0]->Draw("HIST same");

    d->cd(7);
    h_RQ_xz[0]->Draw("HIST");
    h_RQ_yz[0]->Draw("HIST same");

    d->cd(8);
    h_RQ_yz[1]->Draw("HIST");
    h_RQ_xz[1]->Draw("HIST same");

    d->Update();
    d->WaitPrimitive();

    /// ------------END--------------
    FileInput->Close();

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
