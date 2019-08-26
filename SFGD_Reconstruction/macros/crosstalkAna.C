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

    //gStyle->SetOptStat(0);
    TString fileIn = "/Users/cjesus/Documents/Data/SFGD_BTEST_2018/25August_11_12_0pt8GeV_0pt0T.root";
    TString fileOut  = "/Users/cjesus/Documents/Data/MCvsData/ana_output";
    TString fileMC [2];
    //fileMC[0]  = "/Users/cjesus/Documents/Data/SFGD_MC/MC_750_proton.root";
    //fileMC[0]  = "/Users/cjesus/Documents/Data/SFGD_MC/MC_750_piplus.root";
    //fileMC[0]  = "/Users/cjesus/Documents/Data/SFGD_MC/pr_v1.root";
    fileMC[0]  = "/Users/cjesus/Documents/Data/MC_output.root";
    //fileMC[1]  = "/Users/cjesus/Documents/Data/MC_pr.root";
    //fileMC[0]  = "/nfs/neutrinos/cjesus/work/MC_output.root";
    
    TString prefix = "/Users/cjesus/Desktop/";

    int maxEvents    = std::numeric_limits<int>::max();
    int maxSelEvents = std::numeric_limits<int>::max();
    int maxStore     = 100;
    int q_Cut        = 50;
    int selEvents    = 0;
    int selLI        = 0;
    int selHI        = 0;
    int selSP        = 0;    
    int jobMode      = 0;    // 1 closes program at termination.
    int dataType     = 1;    // 0 is MC.
    bool SAVE_EVENTS = false;
    bool SHOW_EVENTS = false;
    bool RM_CROSSTALK= false;
    float time       = clock();

    // Counter to stimate crosstalk amount
    int LI_HIT_CNT = 0;
    int LI_CT_CNT = 0;
    int LI_CT_CNT2 = 0;
    int HI_HIT_CNT = 0;
    int HI_CT_CNT = 0;
    int HI_CT_CNT2 = 0;
    int SP_F_HIT_CNT = 0;
    int SP_F_CT_CNT = 0;
    int SP_F_CT_CNT2 = 0;
    int SP_A_HIT_CNT = 0;
    int SP_A_CT_CNT = 0;
    int SP_A_CT_CNT2 = 0;
    int SP_S_HIT_CNT = 0;
    int SP_S_CT_CNT = 0;
    int SP_S_CT_CNT2 = 0;

    // Canvas for plots:

    // dEdZ:
    TH1F* h_dedz = new TH1F("dedz","dedz",100,0,250);

    // LI == Low Ionizing track
    // HI == High Ionizing track

    TH1F*  h_PE_ALL_0 = new TH1F("h_PE_ALL_xy_LI","",150,0,150);
    TH1F*  h_PE_ALL_1 = new TH1F("h_PE_ALL_xz_LI","",150,0,150);
    TH1F*  h_PE_ALL_2 = new TH1F("h_PE_ALL_yz_LI","",150,0,150);
    TH1F*  h_PE_ALL_3 = new TH1F("h_PE_ALL_xy_HI","",100,0,700);
    TH1F*  h_PE_ALL_4 = new TH1F("h_PE_ALL_xz_HI","",100,0,700);
    TH1F*  h_PE_ALL_5 = new TH1F("h_PE_ALL_yz_HI","",100,0,700);

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

    TH1F*  h_PE_ch[10];
    h_PE_ch[0] = new TH1F("h_PE_ch_0","",100,0,500);
    h_PE_ch[1] = new TH1F("h_PE_ch_1","",100,0,500);
    h_PE_ch[2] = new TH1F("h_PE_ch_2","",100,0,500);
    h_PE_ch[3] = new TH1F("h_PE_ch_3","",100,0,500);
    h_PE_ch[4] = new TH1F("h_PE_ch_4","",100,0,500);
    h_PE_ch[5] = new TH1F("h_PE_ch_5","",100,0,500);
    h_PE_ch[6] = new TH1F("h_PE_ch_6","",100,0,500);
    h_PE_ch[7] = new TH1F("h_PE_ch_7","",100,0,500);
    h_PE_ch[8] = new TH1F("h_PE_ch_8","",100,0,500);
    h_PE_ch[9] = new TH1F("h_PE_ch_9","",100,0,500);


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
            cout << "   -m || --MC        Allows to MC inoput." << endl;
            cout << "   -p || --ptype     Requires an extra argument. 1: proton, 2:pion, 3:electron, 4:muon." << endl;
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
        else if (string( gApplication->Argv(iarg))=="-m" || string( gApplication->Argv(iarg))=="--MC" ){
            dataType = 0;
            fileIn    = fileMC[0];
        }
        else if (string( gApplication->Argv(iarg))=="-p" || string( gApplication->Argv(iarg))=="--ptype" ){
            iarg++;
            cout << atoi(gApplication->Argv(iarg)) << endl;
            fileIn = fileMC[atoi(gApplication->Argv(iarg))];
        }
        else if (string( gApplication->Argv(iarg))=="-c" || string( gApplication->Argv(iarg))=="--clean" ){
            cout << "Crosstalk removal is ON. This only does somethig on MC input. All hits that are true crosstalk will be erased." << endl;
            RM_CROSSTALK = true;
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

    // The input file TTree of events is linked to 'inputBranch'.
    TFile *FileInput  = new TFile(fileIn.Data(),"update");
    TTree* data = (TTree*) FileInput->Get("AllEvents");
    Int_t nEvents = data->GetEntries();

    cout << "nEvents: " << nEvents << endl;

    if(!dataType) fileOut += "_MC.root";
    else fileOut += "_Data.root";

    TBranch *inputBranch;
    inputBranch = data->GetBranch("Event");
    ND280SFGDEvent* inputEvent = new ND280SFGDEvent();
    Event* unpackEvent = new Event();

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
        if(iev == maxEvents) break;
        if(iev > maxSelEvents) break;
        std::cout << "**** Evt " << iev << " ****" << std::endl;

        data->GetEntry(iev);
        inputEvent->MergeHits();
        cout << "NUMHITS:" << endl;
        cout << "numhits:" << inputEvent->GetHits().size() <<endl;

        vector <ND280SFGDHit*> listOfHits;

        if(!dataType) {
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
        else{
            if(iev < 20) continue;

            bool jumpNextEvent;
            int maxQYZ = 0;
            TClonesArray * unpackHits = unpackEvent->GetHits();
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
                listOfHits.push_back(sfgdHit);
            }
            if(jumpNextEvent) {listOfHits.clear();continue;} // cout << "FIFO problem, jumping to next event." << endl; 
        }
        cout << "size: " << listOfHits.size() << endl;
        if(listOfHits.size() < 60) {listOfHits.clear();continue;}  // not interested in small tracks, we want long ones.

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

        for(uint ihit=0; ihit<listOfHits.size(); ihit++){
            if(listOfHits[ihit]->GetView() == 1){
                lineZ_xz[int(listOfHits[ihit]->GetZ())] = 1;
                if(listOfHits[ihit]->GetCharge()>maxXZ) {
                    maxXZ = listOfHits[ihit]->GetCharge(); 
                    maxZ_xz = listOfHits[ihit]->GetZ();
                    hitmaxXZ = *listOfHits[ihit];
                }
                if(aboveCutXZ[int(listOfHits[ihit]->GetX())] == 2) continue;
                if(listOfHits[ihit]->GetCharge() > q_Cut) aboveCutXZ[int(listOfHits[ihit]->GetX())] = 2;
                else aboveCutXZ[int(listOfHits[ihit]->GetX())] = 1;
            }

            if(listOfHits[ihit]->GetView() == 2){
                lineZ_yz[int(listOfHits[ihit]->GetZ())] = 1;
                if(listOfHits[ihit]->GetCharge()>maxYZ) {
                    maxYZ = listOfHits[ihit]->GetCharge(); 
                    maxZ_yz = listOfHits[ihit]->GetZ();
                    hitmaxYZ = *listOfHits[ihit];
                }
                if(aboveCutYZ[int(listOfHits[ihit]->GetY())] == 2) continue;
                if(listOfHits[ihit]->GetCharge() > q_Cut) aboveCutYZ[int(listOfHits[ihit]->GetY())] = 2;
                else aboveCutYZ[int(listOfHits[ihit]->GetY())] = 1;
            }
        }

        // veto condition:
        //if(aboveCutXZ[0] == 2 or aboveCutXZ[sizeOfX-1] == 2) continue;

        int cntXZ = 0;
        int cntYZ = 0;
        for(auto s = 0; s<sizeOfX; s++) if(aboveCutXZ[s] == 2) cntXZ++;
        for(auto s = 0; s<sizeOfY; s++) if(aboveCutYZ[s] == 2) cntYZ++;

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
        for(uint ihit=0; ihit<listOfHits.size(); ihit++){
            if(listOfHits[ihit]->GetView() == 1){
                dedz += listOfHits[ihit]->GetCharge();
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


        // for(uint ihit = 0; ihit < listOfHits.size(); ihit++){
        //     ND280SFGDHit* hit = listOfHits[ihit];
        //     if(!(hit->GetView() == 1 && hit->GetX() == 12 && hit->GetZ() < 30  && hit->GetZ() >= 20) ) continue;
        //     int view = hit->GetView();
        //     int qhit = hit->GetCharge();
        //     h_PE_ch[(int) hit->GetZ()-20]->Fill(qhit);
        // }

        if(cntXZ > 1 or cntYZ > 1) continue;
        if(maxLineZ_xz < 30 or maxLineZ_yz < 30) continue;

        int xPos = -1;
        for(auto s = 0; s<sizeOfX; s++) if(aboveCutXZ[s] == 2)  xPos = int(s);
        if (xPos < 0) continue;

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
        else if(dedz > 80) {evt_tag = "HI"; selHI++; mode = 1;}
        else continue;
        if(mode <0) continue;

        bool stopping = false;
        if(lineZ_xz[sizeOfZ-1] == 0 && lineZ_xz[sizeOfZ] == 0) stopping = true;
        if( stopping && mode == 1) selSP++;

        // Stopping and HI sample, far from stopping point
        if(mode == 1 && cntXZ == 1 && cntYZ == 1 && stopping){
            for(uint ihit = 0; ihit < listOfHits.size(); ihit++){
                ND280SFGDHit* hit = listOfHits[ihit];
                if(hit->GetView() != 1) continue;
                if(hit->GetX() != xPos) continue;
                if(hit->GetCharge() < q_Cut) continue;
                if( abs(maxZ_xz - hit->GetZ()) <3) continue; 
                SP_F_HIT_CNT++;
                for(uint jhit = 0; jhit < listOfHits.size(); jhit++){
                    if(listOfHits[jhit]->GetView() != 1) continue;
                    if(hit->GetZ() == listOfHits[jhit]->GetZ() && abs(listOfHits[jhit]->GetX() -xPos) ==1) SP_F_CT_CNT++;
                    if(hit->GetZ() == listOfHits[jhit]->GetZ() && abs(listOfHits[jhit]->GetX() -xPos) ==2) SP_F_CT_CNT2++;
                }
            }
        }

        // Stopping and HI sample, 1 cube before stopping point
        if(mode == 1 && cntXZ == 1 && cntYZ == 1 && stopping){
            for(uint ihit = 0; ihit < listOfHits.size(); ihit++){
                ND280SFGDHit* hit = listOfHits[ihit];
                if(hit->GetView() != 1) continue;
                if(hit->GetX() != xPos) continue;
                if(hit->GetCharge() < q_Cut) continue;
                if( maxZ_xz - hit->GetZ() != -1) continue; 
                SP_A_HIT_CNT++;
                for(uint jhit = 0; jhit < listOfHits.size(); jhit++){
                    if(listOfHits[jhit]->GetView() != 1) continue;
                    if(hit->GetZ() == listOfHits[jhit]->GetZ() && abs(listOfHits[jhit]->GetX() -xPos) ==1) SP_A_CT_CNT++;
                    if(hit->GetZ() == listOfHits[jhit]->GetZ() && abs(listOfHits[jhit]->GetX() -xPos) ==2) SP_A_CT_CNT2++;
                }
            }
        }

        // Stopping and HI sample, at stopping point
        if(mode == 1 && cntXZ == 1 && cntYZ == 1 && stopping){
            for(uint ihit = 0; ihit < listOfHits.size(); ihit++){
                ND280SFGDHit* hit = listOfHits[ihit];
                if(hit->GetView() != 1) continue;
                if(hit->GetX() != xPos) continue;
                if(hit->GetCharge() < q_Cut) continue;
                if( maxZ_xz - hit->GetZ() != 0) continue; 
                SP_S_HIT_CNT++;
                // int view = hit->GetView();
                // int qhit = hit->GetCharge();
                // if(hit->GetView() == 0) h_PE_ALL_3->Fill(qhit);
                // if(hit->GetView() == 1) h_PE_ALL_4->Fill(qhit);
                // if(hit->GetView() == 2) h_PE_ALL_5->Fill(qhit);
                for(uint jhit = 0; jhit < listOfHits.size(); jhit++){
                    if(listOfHits[jhit]->GetView() != 1) continue;
                    if(hit->GetZ() == listOfHits[jhit]->GetZ() && abs(listOfHits[jhit]->GetX() -xPos) ==1) SP_S_CT_CNT++;
                    if(hit->GetZ() == listOfHits[jhit]->GetZ() && abs(listOfHits[jhit]->GetX() -xPos) ==2) SP_S_CT_CNT2++;
                }
            }
        }

         if(mode == 0 && cntXZ == 1 && cntYZ == 1){
            for(uint ihit = 0; ihit < listOfHits.size(); ihit++){
                ND280SFGDHit* hit = listOfHits[ihit];
                int view = hit->GetView();
                int qhit = hit->GetCharge();
                if(hit->GetView() == 0) h_PE_ALL_0->Fill(qhit);
                if(hit->GetView() == 1) h_PE_ALL_1->Fill(qhit);
                if(hit->GetView() == 2) h_PE_ALL_2->Fill(qhit);
            }
        }

         if(mode == 1 && cntXZ == 1 && cntYZ == 1){
            for(uint ihit = 0; ihit < listOfHits.size(); ihit++){
                ND280SFGDHit* hit = listOfHits[ihit];
                int view = hit->GetView();
                int qhit = hit->GetCharge();
                if(hit->GetView() == 0)  h_PE_ALL_3->Fill(qhit);
                if(hit->GetView() == 1)  h_PE_ALL_4->Fill(qhit);
                if(hit->GetView() == 2)  h_PE_ALL_5->Fill(qhit);
            }
        }

        // Not stopping and HI
        cout << "mode " << mode << ", cntXZ " << cntXZ << ", cntYZ " << cntYZ << ", stopping " << stopping << endl;
        if(mode == 1 && cntXZ == 1 && cntYZ == 1 && !stopping){
            for(uint ihit = 0; ihit < listOfHits.size(); ihit++){
                ND280SFGDHit* hit = listOfHits[ihit];
                if(hit->GetView() != 1) continue;
                if(hit->GetX() != xPos) continue;
                if(hit->GetCharge() < q_Cut) continue;
                if( abs(maxZ_xz - hit->GetZ()) <3) continue; 
                HI_HIT_CNT++;
                for(uint jhit = 0; jhit < listOfHits.size(); jhit++){
                    if(listOfHits[jhit]->GetView() != 1) continue;
                    if(hit->GetZ() == listOfHits[jhit]->GetZ() && abs(listOfHits[jhit]->GetX() -xPos) ==1) HI_CT_CNT++;
                    if(hit->GetZ() == listOfHits[jhit]->GetZ() && abs(listOfHits[jhit]->GetX() -xPos) ==2) HI_CT_CNT2++;
                }
            }
        }

        // Not stopping and LI
        if(mode == 0 && cntXZ == 1 && cntYZ == 1 /*&& !stopping*/){
            for(uint ihit = 0; ihit < listOfHits.size(); ihit++){
                ND280SFGDHit* hit = listOfHits[ihit];
                if(hit->GetView() != 1) continue;
                if(hit->GetX() != xPos) continue;
                if(hit->GetCharge() < q_Cut) continue;
                LI_HIT_CNT++;
                for(uint jhit = 0; jhit < listOfHits.size(); jhit++){
                    if(listOfHits[jhit]->GetView() != 1) continue;
                    if(hit->GetZ() == listOfHits[jhit]->GetZ() && abs(listOfHits[jhit]->GetX() -xPos) ==1) LI_CT_CNT++;
                    if(hit->GetZ() == listOfHits[jhit]->GetZ() && abs(listOfHits[jhit]->GetX() -xPos) ==2) LI_CT_CNT2++;
                }
            }
        }

        for(uint ihit = 0; ihit < listOfHits.size(); ihit++){
            if( listOfHits[ihit]->GetView() == 0 ) continue;
            else if( listOfHits[ihit]->GetView() == 1 ){

                // hit at maxZ:
                if(abs(maxZ_xz - listOfHits[ihit]->GetZ()) == 0){
                    // continue if it is crosstalk:
                    if(aboveCutXZ[int(listOfHits[ihit]->GetX())] == 2) continue; 
                    // fill crosstalk PE distribution for the XZ plane
                    if(mode == 1 && stopping) h_SP_PE_xz[2]->Fill(listOfHits[ihit]->GetCharge()); 
                    // search hits in X-1 and X+1:
                    for(uint jhit=0; jhit<listOfHits.size(); jhit++){
                        if (listOfHits[ihit]->GetView() != 1) continue;
                        if( abs(listOfHits[ihit]->GetX()-listOfHits[jhit]->GetX()) == 1 && abs(listOfHits[ihit]->GetZ()-listOfHits[jhit]->GetZ()) == 0 && listOfHits[jhit]->GetCharge() > q_Cut){
                            if(mode == 1 && stopping) h_SP_RQ_xz[2]->Fill( 100.*listOfHits[ihit]->GetCharge()/listOfHits[jhit]->GetCharge()); 
                        }
                    }
                }
                else if( maxZ_xz - listOfHits[ihit]->GetZ()  == 1){
                    // continue if it is crosstalk:
                    if(aboveCutXZ[int(listOfHits[ihit]->GetX())] == 2) continue; 
                    // fill crosstalk PE distribution for the XZ plane
                    if(mode == 1 && stopping) h_SP_PE_xz[1]->Fill(listOfHits[ihit]->GetCharge()); 
                    // search hits in X-1 and X+1:
                    for(uint jhit=0; jhit<listOfHits.size(); jhit++){
                        if (listOfHits[ihit]->GetView() != 1) continue;
                        if( abs(listOfHits[ihit]->GetX()-listOfHits[jhit]->GetX()) == 1 && abs(listOfHits[ihit]->GetZ()-listOfHits[jhit]->GetZ()) == 0 && listOfHits[jhit]->GetCharge() > q_Cut){
                            if(mode == 1 && stopping) h_SP_RQ_xz[1]->Fill( 100.*listOfHits[ihit]->GetCharge()/listOfHits[jhit]->GetCharge()); 
                        }
                    }
                }

                else if(abs(maxZ_xz - listOfHits[ihit]->GetZ()) > 2){
                    // continue if it is crosstalk:
                    if(aboveCutXZ[int(listOfHits[ihit]->GetX())] == 2) continue; 
                    // fill crosstalk PE distribution for the XZ plane
                    h_PE_xz[mode]->Fill(listOfHits[ihit]->GetCharge());
                    if(mode == 1 && stopping) h_SP_PE_xz [0]->Fill(listOfHits[ihit]->GetCharge()); 
                    // search hits in X-1 and X+1:
                    for(uint jhit=0; jhit<listOfHits.size(); jhit++){
                        if (listOfHits[ihit]->GetView() != 1) continue;
                        if( abs(listOfHits[ihit]->GetX()-listOfHits[jhit]->GetX()) == 1 && abs(listOfHits[ihit]->GetZ()-listOfHits[jhit]->GetZ()) == 0 && listOfHits[jhit]->GetCharge() > q_Cut){
                            h_RQ_xz[mode]->Fill( 100.*listOfHits[ihit]->GetCharge()/listOfHits[jhit]->GetCharge());
                            if(mode == 1 && stopping) h_SP_RQ_xz[0]->Fill( 100.*listOfHits[ihit]->GetCharge()/listOfHits[jhit]->GetCharge());
                        }
                    }
                }
            }
            else if( listOfHits[ihit]->GetView() == 2 ){
                // hit at maxZ:
                if(abs(maxZ_yz - listOfHits[ihit]->GetZ()) == 0){
                    // continue if it is crosstalk:
                    if(aboveCutYZ[int(listOfHits[ihit]->GetY())] == 2) continue; 
                    // fill crosstalk PE distribution for the YZ plane
                    if(mode == 1 && stopping) h_SP_PE_yz[2]->Fill(listOfHits[ihit]->GetCharge()); 
                    // search hits in Y-1 and Y+1:
                    for(uint jhit=0; jhit<listOfHits.size(); jhit++){
                        if (listOfHits[ihit]->GetView() != 2) continue;
                        if( abs(listOfHits[ihit]->GetY()-listOfHits[jhit]->GetY()) == 1 && abs(listOfHits[ihit]->GetZ()-listOfHits[jhit]->GetZ()) == 0 && listOfHits[jhit]->GetCharge() > q_Cut){
                            if(mode == 1 && stopping) h_SP_RQ_yz [2]->Fill( 100.*listOfHits[ihit]->GetCharge()/listOfHits[jhit]->GetCharge()); 
                        }
                    }
                }
                else if( maxZ_yz - listOfHits[ihit]->GetZ()  == 1){
                    // continue if it is crosstalk:
                    if(aboveCutYZ[int(listOfHits[ihit]->GetY())] == 2) continue; 
                    // fill crosstalk PE distribution for the YZ plane
                    if(mode == 1 && stopping) h_SP_PE_yz[1]->Fill(listOfHits[ihit]->GetCharge()); 
                    // search hits in Y-1 and Y+1:
                    for(uint jhit=0; jhit<listOfHits.size(); jhit++){
                        if (listOfHits[ihit]->GetView() != 2) continue;
                        if( abs(listOfHits[ihit]->GetY()-listOfHits[jhit]->GetY()) == 1 && abs(listOfHits[ihit]->GetZ()-listOfHits[jhit]->GetZ()) == 0 && listOfHits[jhit]->GetCharge() > q_Cut){
                            if(mode == 1 && stopping) h_SP_RQ_yz[1]->Fill( 100.*listOfHits[ihit]->GetCharge()/listOfHits[jhit]->GetCharge()); 
                        }
                    }
                }

                else if(abs(maxZ_yz - listOfHits[ihit]->GetZ()) > 2){
                    // continue if it is crosstalk:
                    if(aboveCutYZ[int(listOfHits[ihit]->GetY())] == 2) continue; 
                    // fill crosstalk PE distribution for the YZ plane
                    h_PE_yz[mode]->Fill(listOfHits[ihit]->GetCharge());
                    if(mode == 1 && stopping) h_SP_PE_yz [0]->Fill(listOfHits[ihit]->GetCharge()); 
                    // search hits in Y-1 and Y+1:
                    for(uint jhit=0; jhit<listOfHits.size(); jhit++){
                        if (listOfHits[ihit]->GetView() != 2) continue;
                        if( abs(listOfHits[ihit]->GetY()-listOfHits[jhit]->GetY()) == 1 && abs(listOfHits[ihit]->GetZ()-listOfHits[jhit]->GetZ()) == 0 && listOfHits[jhit]->GetCharge() > q_Cut){
                            h_RQ_yz[mode]->Fill( 100.*listOfHits[ihit]->GetCharge()/listOfHits[jhit]->GetCharge());
                            if(mode == 1 && stopping) h_SP_RQ_yz[0]->Fill( 100.*listOfHits[ihit]->GetCharge()/listOfHits[jhit]->GetCharge());
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

            for(size_t i=0; i<listOfHits.size(); i++){
                ND280SFGDHit* hit = listOfHits[i];

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

    cout << endl << "CROSSTALK SUMMARY: " << endl;
    cout << "SC: 'Stopping Cube' " << endl << endl;cout << "SC: 'Stopping Cube' " << endl;
    cout << "SP: 'Stopping proton track.' " << endl;
    cout << "LI: 'Low ionizing tracks.' " << endl;
    cout << "LI: 'High ionizing tracks.' " << endl << endl;

    if(SP_F_HIT_CNT > 0){
        cout << "SP TOT HITS far from SC:            " << SP_F_HIT_CNT << endl;
        cout << "SP HITS     far from SC  CT at 1cm: " << SP_F_CT_CNT  << ", " << 0.5*100.*SP_F_CT_CNT/SP_F_HIT_CNT << "%" << endl;
        cout << "SP HITS     far from SC  CT at 2cm: " << SP_F_CT_CNT2 << ", " << 0.5*100.*SP_F_CT_CNT2/SP_F_HIT_CNT << "%"<< endl << endl;
    }

    if(SP_A_HIT_CNT > 0){
        cout << "SP TOT HITS 1cube before SC:             " << SP_A_HIT_CNT << endl;
        cout << "SP HITS     1cube before SC  CT at 1cm: " << SP_A_CT_CNT  << ", " << 0.5*100.*SP_A_CT_CNT/SP_A_HIT_CNT << "%" << endl;
        cout << "SP HITS     1cube before SC  CT at 2cm: " << SP_A_CT_CNT2 << ", " << 0.5*100.*SP_A_CT_CNT2/SP_A_HIT_CNT << "%"<< endl << endl;
    }

    if(SP_S_HIT_CNT > 0){
        cout << "SP TOT HITS at SC:            " << SP_S_HIT_CNT << endl;
        cout << "SP HITS     at SC  CT at 1cm: " << SP_S_CT_CNT  << ", " << 0.5*100.*SP_S_CT_CNT/SP_S_HIT_CNT << "%" << endl;
        cout << "SP HITS     at SC  CT at 2cm: " << SP_S_CT_CNT2 << ", " << 0.5*100.*SP_S_CT_CNT2/SP_S_HIT_CNT << "%"<< endl << endl;
    }

    if(HI_HIT_CNT > 0){
        cout << "HI TOT HITS            " << HI_HIT_CNT << endl;
        cout << "HI HITS     CT at 1cm: " << HI_CT_CNT  << ", " << 0.5*100.*HI_CT_CNT/HI_HIT_CNT << "%" << endl;
        cout << "HI HITS     CT at 2cm: " << HI_CT_CNT2 << ", " << 0.5*100.*HI_CT_CNT2/HI_HIT_CNT << "%"<< endl << endl;
    }

    if(LI_HIT_CNT > 0){
        cout << "LI TOT HITS            " << LI_HIT_CNT << endl;
        cout << "LI HITS     CT at 1cm: " << LI_CT_CNT  << ", " << 0.5*100.*LI_CT_CNT/LI_HIT_CNT << "%" << endl;
        cout << "LI HITS     CT at 2cm: " << LI_CT_CNT2 << ", " << 0.5*100.*LI_CT_CNT2/LI_HIT_CNT << "%"<< endl << endl;
    }
    cout << endl; 

    TCanvas* c1 = new TCanvas("SP1");
    c1->Divide(3,2); 

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
    h_SP_PE_yz[0]->SetLineColor(kBlack);
    h_SP_PE_xz[0]->SetLineColor(kGreen+1);
    h_SP_PE_yz[1]->SetLineColor(kCyan);
    h_SP_PE_xz[1]->SetLineColor(kBlue);
    h_SP_PE_yz[2]->SetLineColor(kOrange);
    h_SP_PE_xz[2]->SetLineColor(kRed);

    c1->cd(1);
    h_SP_PE_yz[0]->Draw("HIST");

    c1->cd(2);
    h_SP_PE_yz[1]->Draw("HIST");

    c1->cd(3);
    h_SP_PE_yz[2]->Draw("HIST");

    c1->cd(4);
    h_SP_PE_xz[0]->Draw("HIST");

    c1->cd(5);
    h_SP_PE_xz[1]->Draw("HIST");

    c1->cd(6);
    h_SP_PE_xz[2]->Draw("HIST");

    c1->Update();
    TString c1_name = prefix + "SP1.pdf";
    c1->SaveAs(c1_name.Data());

    TCanvas* c2 = new TCanvas("SP2");
    c2->Divide(2,1); 

    c2->cd(1);
    h_SP_PE_yz[0]->Draw("HIST");
    h_SP_PE_yz[1]->Draw("HIST same");
    h_SP_PE_yz[2]->Draw("HIST same");

    c2->cd(2);
    gPad->SetTitle("");
    h_SP_PE_xz[0]->Draw("HIST");
    h_SP_PE_xz[1]->Draw("HIST same");
    h_SP_PE_xz[2]->Draw("HIST same");

    c2->Update();
    TString c2_name = prefix + "SP2.pdf";
    c2->SaveAs(c2_name.Data());

    TCanvas* c3 = new TCanvas("PE");
    c3->Divide(4,2); 

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
    h_PE_xz[0]->SetLineColor(kCyan);
    h_PE_yz[0]->SetLineColor(kRed);
    h_PE_xz[1]->SetLineColor(kGreen+1);
    h_PE_yz[1]->SetLineColor(kBlue);

    // TF1 *f1 = new TF1("f1","[0]*TMath::Power(([1]/[2]),(x/[2]))*(TMath::Exp(-([1]/[2])))/TMath::Gamma((x/[2])+1) + [3]*TMath::Power(([4]/[5]),(x/[5]))*(TMath::Exp(-([4]/[5])))/TMath::Gamma((x/[5])+1)",0,20);
    // f1->SetParameters(0.2, 3.3, 1.35, 0.1,5,2);
    
    // TF1 *f2 = new TF1("f2","[0]*TMath::Power(([1]/[2]),(x/[2]))*(TMath::Exp(-([1]/[2])))/TMath::Gamma((x/[2])+1)",0,20);
    // f2->SetParameters(0.2, 3.3, 1.35);

    c3->cd(1);
    h_PE_yz[0]->Draw("HIST");

    c3->cd(2);
    h_PE_yz[1]->Draw("HIST");
    //h_PE_yz[1]->Fit("f2","",0,8);
    //f2->DrawCopy("same");

    c3->cd(3);
    h_PE_xz[0]->Draw("HIST");
    //h_PE_xz[0]->Fit("f1","",0,6);
    //f1->DrawCopy("same");

    c3->cd(4);
    h_PE_xz[1]->Draw("HIST");
    //f1->SetParameters(0.13, 4.7,1.93,0.1,5,2);
    //h_PE_xz[1]->Fit("f1","",0,8);
    //f1->DrawCopy("same");

    c3->cd(5);
    h_PE_yz[1]->Draw("HIST");
    h_PE_yz[0]->Draw("HIST same");

    c3->cd(6);
    h_PE_xz[0]->Draw("HIST");
    h_PE_xz[1]->Draw("HIST same");

    c3->cd(7);
    h_PE_xz[0]->Draw("HIST");
    h_PE_yz[0]->Draw("HIST same");

    c3->cd(8);
    h_PE_yz[1]->Draw("HIST");
    h_PE_xz[1]->Draw("HIST same");

    c3->Update();
    TString c3_name = prefix + "PE.pdf";
    c3->SaveAs(c3_name.Data());

    TCanvas* c4 = new TCanvas("RQ");
    c4->Divide(4,2); 

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

    c4->cd(1);
    gPad->SetTitle("");
    h_RQ_yz[0]->Draw("HIST");

    c4->cd(2);
    h_RQ_yz[1]->Draw("HIST");

    c4->cd(3);
    h_RQ_xz[0]->Draw("HIST");

    c4->cd(4);
    h_RQ_xz[1]->Draw("HIST");

    c4->cd(5);
    h_RQ_yz[1]->Draw("HIST");
    h_RQ_yz[0]->Draw("HIST same");

    c4->cd(6);
    h_RQ_xz[1]->Draw("HIST");
    h_RQ_xz[0]->Draw("HIST same");

    c4->cd(7);
    h_RQ_xz[0]->Draw("HIST");
    h_RQ_yz[0]->Draw("HIST same");

    c4->cd(8);
    h_RQ_yz[1]->Draw("HIST");
    h_RQ_xz[1]->Draw("HIST same");

    c4->Update();
    TString c4_name = prefix + "RQ.pdf";
    c4->SaveAs(c4_name.Data());

    TCanvas* c5 = new TCanvas("dEdL");
    c5->cd(); 
    h_dedz->Draw("HIST");
    TString fname = prefix + "dEdL.pdf";
    c5->SaveAs(fname.Data());
    c5->Update();

    TCanvas* c6 = new TCanvas("RQ");
    c6->Divide(2,1);
    c6->cd(1);

    h_PE_ALL_0->SetLineColor(kBlack);
    h_PE_ALL_1->SetLineColor(kBlue);
    h_PE_ALL_2->SetLineColor(kRed);
    h_PE_ALL_3->SetLineColor(kBlack);
    h_PE_ALL_4->SetLineColor(kBlue);
    h_PE_ALL_5->SetLineColor(kRed);

    h_PE_ALL_2->Draw("HIST"); 
    h_PE_ALL_0->Draw("HIST same"); 
    h_PE_ALL_1->Draw("HIST same"); 

    c6->cd(2);
    h_PE_ALL_4->Draw("HIST"); 
    h_PE_ALL_3->Draw("HIST same"); 
    h_PE_ALL_5->Draw("HIST same"); 
    // h_PE_ch[0]->SetLineColor(kBlack);
    // h_PE_ch[1]->SetLineColor(kRed);
    // h_PE_ch[2]->SetLineColor(kBlue);
    // h_PE_ch[3]->SetLineColor(kGreen);

    // h_PE_ch[0]->Draw("HIST");
    // h_PE_ch[1]->Draw("HIST same");
    // h_PE_ch[2]->Draw("HIST same");
    // h_PE_ch[3]->Draw("HIST same");


    // for(auto x : h_PE_ch)
    //     x->Draw("HIST same");

    TString c6_name = prefix + "dEdL.pdf";
    c6->Update();
    c6->SaveAs(c6_name.Data());

    /// ------------END--------------
    FileInput->Close();
    TFile *FileOutput  = new TFile(fileOut.Data(),"RECREATE");
    FileOutput->cd();
    h_PE_ALL_0->Write();
    h_PE_ALL_1->Write();
    h_PE_ALL_2->Write();
    h_PE_ALL_3->Write();
    h_PE_ALL_4->Write();
    h_PE_ALL_5->Write();
    for(auto x:h_PE_ch) x->Write();
    FileOutput->Close();

    c6->WaitPrimitive();

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
