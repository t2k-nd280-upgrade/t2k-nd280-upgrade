#define THIS_NAME prototypeDistributions
#define NOINTERACTIVE_OUTPUT
#define OVERRIDE_OPTIONS

#include "../utils/global_header.h"
#include "../utils/global_tools.C"
#include <time.h> 
#include <TMath.h>
#include <TF1.h>
#include <TGraphErrors.h>

void prototypeDistributions() {

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
    TString fileIn   = "/Users/cjesus/Documents/Data/SFGD_BTEST_2018/25August_11_12_0pt8GeV_0pt0T.root";
    TString fileOut  = "/Users/cjesus/Documents/Data/MCvsData/ana_output";
    TString fileMC = "/Users/cjesus/Documents/Data/MC_output.root";
    TString prefix = "/Users/cjesus/Desktop/";

    int maxEvents    = std::numeric_limits<int>::max();
    int maxSelEvents = std::numeric_limits<int>::max();
    int maxStore     = 100;
    int q_Cut        = 50;
    int selEvents    = 0;
    int jobMode      = 0;    // 1 closes program at termination.
    int dataType     = 1;    // 0 is MC.
    bool SAVE_EVENTS = false;
    bool SHOW_EVENTS = false;
    bool RM_CROSSTALK= false;
    float time       = clock();

    int ToT = 0;
    int HG  = 0;
    int LG  = 0;

    // Canvas for plots:

    TH1F* h_ChargeType[3];
    h_ChargeType [0] = new TH1F("h_PE_yz_HG","", 100,0,100);
    h_ChargeType [1] = new TH1F("h_PE_yz_LG","", 100,0,100);
    h_ChargeType [2] = new TH1F("h_PE_yz_ToT","",100,0,100);

    TH2F* h_heatMap_EnterPoint_L = new TH2F("XY_L","",24,0,24,8,0,8); // low dedz
    TH2F* h_heatMap_EnterPoint_H = new TH2F("XY_H","",24,0,24,8,0,8); // high dedz

    TH1F* h_Xdistribution;
    h_Xdistribution = new TH1F("h_Xdistribution","", 24,0,24);

    TH1F* h_Attenuation_Even[24];
    for(int hst=0; hst<24; hst++){
        TString hist_name  = "h_Att_Even";
        hist_name += hst;
        h_Attenuation_Even[hst] = new TH1F(hist_name.Data(),"", 100,0,300);  
    } 

    TH1F* h_Attenuation_Odd[24];
    for(int hst=0; hst<24; hst++){
        TString hist_name  = "h_Att_Odd";
        hist_name += hst;
        h_Attenuation_Odd[hst] = new TH1F(hist_name.Data(),"", 100,0,300);  
    } 

    TH1F* h_Attenuation_MC[24];
    for(int hst=0; hst<24; hst++){
        TString hist_name  = "h_Att_MC";
        hist_name += hst;
        h_Attenuation_MC[hst] = new TH1F(hist_name.Data(),"", 100,0,300);  
    } 

    // MC and data must be treated in different ways since both are not read in the same way. Data is read in different ends for odd or even X,Z positions.
    TGraphErrors* gr_Att_even = new TGraphErrors();
    TGraphErrors* gr_Att_odd = new TGraphErrors();
    TGraphErrors* gr_Att_data = new TGraphErrors();
    TGraphErrors* gr_Att_mc = new TGraphErrors();
    
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
            fileIn  = "/home/cjesus/Work/Data/SFGD_prototype/DATA/25August_ALL/25AugustAll.root";//fileIn  = "/home/cjesus/Work/Data/SFGD_prototype/DATA/25August_8_MCR0_hadrons_0pt8Gev_0pt0T_Beam___NewStructure.root";//30August_14_MCR0_hadrons_1pt0Gev_0pt2T_4trigg___NewStructure.root";//
            fileOut = "/home/cjesus/Work/Data/SFGD_prototype/ANALYSIS/ana_output";
            fileMC  = "/home/cjesus/Work/Data/SFGD_prototype/MC/RECO/MC_output.root";
            prefix  = "/home/cjesus/Work/Data/SFGD_prototype/ANALYSIS";
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
        if(selEvents > maxSelEvents) break;
        std::cout << "**** Evt " << iev << " ****" << std::endl;

        data->GetEntry(iev);
        inputEvent->MergeHits();

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
                sfgdHit->SetHG_pe(hit->GetHG_pe());
                sfgdHit->SetLG_pe(hit->GetLG_pe());
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

        if(cntXZ > 1 or cntYZ > 3) continue;
        if(maxLineZ_xz < 30 or maxLineZ_yz < 30) continue;

        // cout << endl;
        // cout << "length XZ: " << maxLineZ_xz << endl;
        // cout << "length YZ: " << maxLineZ_yz << endl;
        // cout << "width  XZ: " << cntXZ << endl;
        // cout << "width  YZ: " << cntYZ << endl;
        // cout << "dedz : "     << dedz << endl;
        // cout << endl;

        int X_pos = -10;
        for(auto s = 0; s<sizeOfX; s++) if(aboveCutXZ[s] == 2) X_pos = s;
        if(X_pos < 0 ) continue;

        if(dedz > 110) for(auto s = 0; s<sizeOfY; s++) if(aboveCutYZ[s] == 2) h_heatMap_EnterPoint_H->Fill(X_pos,s);
        if(dedz < 65) for(auto s = 0; s<sizeOfY; s++) if(aboveCutYZ[s] == 2) h_heatMap_EnterPoint_L->Fill(X_pos,s);

        if(dedz > 65) continue;
        h_Xdistribution->Fill(X_pos);

        for(auto sfgdhit:listOfHits){
            if(sfgdhit->GetView() != 2) continue;
            if(dataType){
                if((int(sfgdhit->GetZ())%2 == 0 && int(sfgdhit->GetY())%2 == 0 )) h_Attenuation_Even[X_pos]->Fill(sfgdhit->GetCharge());
                if((int(sfgdhit->GetZ())%2 == 1 && int(sfgdhit->GetY())%2 == 1 )) h_Attenuation_Odd[X_pos]->Fill(sfgdhit->GetCharge());
            }
            else h_Attenuation_MC[X_pos]->Fill(sfgdhit->GetCharge());
            if(sfgdhit->GetCharge() == sfgdhit->GetHG_pe()) {HG++; h_ChargeType[0]->Fill(sfgdhit->GetCharge());}
            else if(sfgdhit->GetCharge() == sfgdhit->GetLG_pe()) {LG++; h_ChargeType[1]->Fill(sfgdhit->GetCharge());}
            else {ToT++; h_ChargeType[2]->Fill(sfgdhit->GetCharge());}
            if(sfgdhit->GetView() == 1)cout << "X: " << sfgdhit->GetX() << ", Y: " << sfgdhit->GetY() << ",Z :" << sfgdhit->GetZ() << endl; 
        }

        std::cout << "**********************" << std::endl << std::endl;
        selEvents++;
    }

    cout << "Selected events:    " << selEvents << endl;

    TCanvas *c1 = new TCanvas("Q type");
    c1->cd();
    h_ChargeType[0]->SetLineColor(kBlack);
    h_ChargeType[1]->SetLineColor(kBlue);
    h_ChargeType[2]->SetLineColor(kRed);
    h_ChargeType[0]->Draw("HIST");
    h_ChargeType[1]->Draw("HIST same");
    h_ChargeType[2]->Draw("HIST same");
    c1->Update();

    TCanvas *c2 = new TCanvas("Y dist");
    c2->cd();
    h_Xdistribution->Draw("HIST");
    c2->Update();
    

    if(dataType){
        double att_even[24]={0};
        double att_odd[24] ={0};


        TCanvas *c3 = new TCanvas("Att even");
        c3->Divide(4,6);
        for(int i=1; i<25;i++){
            c3->cd(i);
            h_Attenuation_Even[i-1]->Fit("gaus");
            TF1 *gfit = h_Attenuation_Even[i-1]->GetFunction("gaus");
            if(!gfit) continue;
            double fmean = gfit->GetParameter(1);
            double fsigma = gfit->GetParameter(2);
            cout << gr_Att_even->GetN() << "," << i << "," << fmean << endl;
            if(gfit->GetParError(1) < 3 && fmean > 30){
                att_even[24-i+1] = fmean;
                gr_Att_even->SetPoint(gr_Att_even->GetN(),24-i+1,fmean);
                gr_Att_even->SetPointError(gr_Att_even->GetN()-1,0,gfit->GetParError(1));
            }
        }
        c3->Update();
        
        TCanvas *c4 = new TCanvas("Att odd");
        c4->Divide(4,6);
        for(int i=1; i<25;i++){
            c4->cd(i);
            h_Attenuation_Odd[i-1]->Fit("gaus");
            TF1 *gfit = h_Attenuation_Odd[i-1]->GetFunction("gaus");
            if(!gfit) continue;
            double fmean = gfit->GetParameter(1);
            double fsigma = gfit->GetParameter(2);
            cout << gr_Att_odd->GetN() << "," << i << "," << fmean << endl;
            if(gfit->GetParError(1) < 3 && fmean > 30){
                att_odd[i] = fmean;
                gr_Att_odd->SetPoint(gr_Att_odd->GetN(),i,fmean);
                gr_Att_odd->SetPointError(gr_Att_odd->GetN()-1,0,gfit->GetParError(1));
            }
        }
        c4->Update();

        TCanvas *c5 = new TCanvas("graph Att");
        c5->cd();
        gr_Att_even->SetMarkerStyle(22);
        gr_Att_even->SetMarkerSize(1);
        gr_Att_odd->SetMarkerStyle(22);
        gr_Att_odd->SetMarkerSize(1);
        gr_Att_even->SetMarkerColor(kRed);
        gr_Att_odd->SetMarkerColor(kBlue);
        if(gr_Att_even->GetN()){
            gr_Att_even->Draw("AP");
            gr_Att_even->GetXaxis()->SetTitle("Distance to MPPC [cm]");
        }
        if(gr_Att_even->GetN() && gr_Att_odd->GetN() ) gr_Att_odd->Draw("P same");
        auto legend = new TLegend(0.7,0.8,0.9,0.9);
        legend->AddEntry(gr_Att_even,"Even positions");
        legend->AddEntry(gr_Att_odd,"Odd positions");
        legend->Draw("same");
        c5->Update();

        for(int it=0; it<24; it++){
            if(att_even[it] && att_odd[it]){
                cout << "i: " << it << ", odd:" << att_odd[it] << ", even: " << att_even[it] << endl;
                double average = (att_even[it]+att_odd[it])/2; 
                cout << "average: " << average << endl << endl;
                gr_Att_data->SetPoint(gr_Att_data->GetN(),it+1,average);
            }
        }

        TCanvas* c_data = new TCanvas("graph att data");
        c_data->cd();
        // gr_Att_data->SetMarkerStyle(22);
        // gr_Att_data->SetMarkerSize(1);
        // gr_Att_data->SetMarkerColor(kGreen+1);
        // gr_Att_data->Draw("AP");
        TF1 *gfitatt = new TF1("gfitatt","[3]*([0]*exp ( (-10*x-20) /[1]) + (1-[0])*exp( (-10*x-20) /[2] )) ",0,25);
        gfitatt->SetParameters(0.77,4634,332,60);
/*        TF1 *gfitatt = new TF1("gfitatt","[0]*exp ( (-10*x-[1]) /[2])",2,15);
        gfitatt->SetParameters(60,20,800);*/
/*        TF1 *gfitatt = new TF1("gfitatt","[0]*exp ( (-10*x) /[1])",2,20);
        gfitatt->SetParameters(60,800);*/

        TF1 *gfitatt2 = new TF1("gfitatt","[4]*([0]*exp ( (-10*x-[1]) /[2]) + (1-[0])*exp( (-10*x-[1]) /[3] )) ",0,25);
        gfitatt2->SetLineColor(kBlue);
        gfitatt2->SetParameters(0.77,41,4634,332,60);
        gfitatt2->Draw("same");

        // gr_Att_data->Fit(gfitatt,"R");
        // c_data->Update();


/*        TCanvas* c_data = new TCanvas("graph att data");
        c_data->cd();
        TF1 *gfitatt = new TF1("gfitatt","[4]*([0]*exp ( (-10*x-[1]) /[2]) + (1-[0])*exp( (-10*x-[1]) /[3] )) ",5,15);
        gfitatt->SetParameters(0.77,41,4634,332,60);
        gfitatt->Draw();
        c_data->Update();*/
    }
    else{
        TCanvas *c4 = new TCanvas("Att MC");
        c4->Divide(4,6);
        for(int i=1; i<25;i++){
            c4->cd(i);
            h_Attenuation_MC[i-1]->Fit("gaus");
            TF1 *gfit = h_Attenuation_MC[i-1]->GetFunction("gaus");
            if(!gfit) continue;
            double fmean = gfit->GetParameter(1);
            double fsigma = gfit->GetParameter(2);
            cout << gr_Att_mc->GetN() << "," << i << "," << fmean << endl;
            if(gfit->GetParError(1) < 3 && fmean > 30){
                gr_Att_mc->SetPoint(gr_Att_mc->GetN(),i,fmean);
                gr_Att_mc->SetPointError(gr_Att_mc->GetN()-1,0,gfit->GetParError(1));
            }
        }
        c4->Update();

        TCanvas *c5 = new TCanvas("graph Att");
        c5->cd();
        gr_Att_mc->SetMarkerStyle(22);
        gr_Att_mc->SetMarkerSize(1);
        gr_Att_mc->SetMarkerColor(kGreen+1);
        if(gr_Att_mc->GetN()){
            gr_Att_mc->Draw("AP");
            gr_Att_mc->GetXaxis()->SetTitle("Distance to MPPC [cm]");
        }
        TF1 *gfitatt = new TF1("gfitatt","[0]*exp ( (-10*x) /[1])",2,20);
        gfitatt->SetParameters(60,800);
        gr_Att_data->Fit(gfitatt,"R");
        auto legend = new TLegend(0.7,0.85,0.9,0.9);
        legend->AddEntry(gr_Att_mc,"MC data");
        legend->Draw("same");

        TF1 *gfitatt3 = new TF1("gfitatt3","[3]*([0]*exp ( (-10*x-20) /[1]) + (1-[0])*exp( (-10*x-20) /[2] )) ",0,25);
        gfitatt3->SetParameters(0.77,4634,332,60);

        TF1 *gfitatt4 = new TF1("gfitatt","[3]*([0]*exp ( (-10*x-20) /[1]) + (1-[0])*exp( (-10*x-20) /[2] )) ",0,25);
        gfitatt4->SetLineColor(kBlue);
        gfitatt4->SetParameters(0.77,4634,332,61.5);
        gfitatt4->Draw("same");

        gr_Att_mc->Fit(gfitatt3,"R");

        c5->Update();
    }

    TCanvas *c6 = new TCanvas("Entrance Point");
    c6->Divide(3,1);
    c6->cd(1);
    h_heatMap_EnterPoint_L->Draw("COLZ");
    c6->cd(2);
    h_heatMap_EnterPoint_H->Draw("COLZ");
    c6->cd(3);
    h_heatMap_EnterPoint_L->Draw("COLZ");
    h_heatMap_EnterPoint_H->Draw("COLZ same");
    c6->Update();
    c6->WaitPrimitive();


    cout << "HG: " <<  1.*HG/(HG+LG+ToT) << endl;
    cout << "LG: " <<  1.*LG/(HG+LG+ToT) << endl;
    cout << "ToT: " << 1.*ToT/(HG+LG+ToT) << endl;

    /// ------------END--------------
    // FileInput->Close();
    // TFile *FileOutput  = new TFile(fileOut.Data(),"RECREATE");
    // FileOutput->cd();
    // FileOutput->Close();

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
