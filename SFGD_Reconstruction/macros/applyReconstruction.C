#define THIS_NAME applyReconstruction
#define NOINTERACTIVE_OUTPUT
#define OVERRIDE_OPTIONS

#include "../utils/global_header.h"
#include <TError.h>
#include <time.h> 
#include <TMath.h>
#include "../src/Reconstruction.cc"
#include "../utils/global_variables.h"

void applyReconstruction() {

    cout << endl << "Staring the execution." << endl;
    // cout << "The goal of this macro is to apply the reconstruction algorithms (/SFGD_Reconstruction/src/) over the input events."       << endl 
    //      << "The reconstructed events are stored in the input file inside a new branch called *RecoEvents*. This reconstructed "        << endl 
    //      << "events can be used later to evaluate the perfomance of the reconstruction methods if the original input comes from MC,"   << endl 
    //      << "or to perform physics studies if the orignal input comes from beam test data."  << endl;

    Int_t maxEvents   = 10000;
    Int_t maxSelEvents= 1000;
    Int_t selEvents   = 0;



    TH1F* hitsQ = new TH1F("hQ","hQ",202,-1,200);
    TH1F* hitsRecoQ = new TH1F("hRQ","hRQ",202,-1,200);
    TH1F* QvsToTratio = new TH1F("hQ","hQ",100,0,1);

    TString fileOut  = "/Users/cjesus/Desktop/Reconstruction_output.root";

    Int_t jobMode    = 0;
    TString prefix   = "/Users/cjesus/Desktop/";
    TString foutName = "Reconstruction_output.root";

    Int_t dataType   = 0; // 0 for MC, 1 for beam test Data. 

    TString fileIn;
    if(!dataType)  fileIn   = "/Users/cjesus/Desktop/MC_output.root";
    //if(!dataType)  fileIn   = "/Users/cjesus/Desktop/newXtalk_MC_output.root";
    //if(!dataType)  fileIn   = "/Users/cjesus/Desktop/noCT_MC_output.root";
    //if(!dataType)  fileIn   = "/Users/cjesus/Desktop/smallSFGD_output.root";
    else           fileIn    = "/Users/cjesus/Documents/PhD/SFGD/25August_8/25August_8_MCR0_hadrons_0pt8Gev_0pt0T_Beam___NewStructure.root";
    //else           fileIn    = "/Users/cjesus/Documents/PhD/SFGD/1September_13_MCR0_hadrons_neg0pt8Gev_0pt2T_4trigg_0deg___NewStructure.root";

    float time = clock();

    vector <ND280SFGDHit*> listOfHitsCopy;

    /// ------------START--------------

    cout << endl;
    for (int iarg=0; iarg<gApplication->Argc(); iarg++){
        if (string( gApplication->Argv(iarg))=="-h" || string( gApplication->Argv(iarg))=="--help" ){
            cout << "**************************************" << endl;
            cout << "Macros run options:" << endl;
            cout << "   -h || --help      print help info." << endl;
            cout << "   -d || --data      Allows to read beamtest data inoput." << endl;
            cout << "   -i || --input     input file (*.root)." << endl;
            cout << "   -o || --output    output file, including the path." << endl;
            cout << "   -j || --job       Closes the macro at the end of the execution." << endl;
            cout << "   -c || --clean     remove crosstalk hits, only works for MC." << endl;
            cout << "   -f || --flag      Use crosstalk falg to reconstruct voxels." << endl;
            cout << "Display options: " << endl;
            cout << "   -sflag            Shows crosstalk flag algorithm events." << endl;
            cout << "   -strue            Shows true event displays." << endl;
            cout << "   -sreco            Shows reco event displays." << endl;
            cout << "**************************************" << endl;
        }
        else if (string( gApplication->Argv(iarg))=="-d" || string( gApplication->Argv(iarg))=="--data" ){
            dataType = 1;
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
        else if (string( gApplication->Argv(iarg))=="-o" || string( gApplication->Argv(iarg))=="--output" ){
            iarg++;
            prefix = gApplication->Argv(iarg);
        }
        else if (string( gApplication->Argv(iarg))=="-j" || string( gApplication->Argv(iarg))=="--job" ){
            jobMode = 1;
        }
        else if (string( gApplication->Argv(iarg))=="-c" || string( gApplication->Argv(iarg))=="--clean" ){
            cout << "Crosstalk removal is ON. This only does somethig on MC input. All hits that are true crosstalk will be erased." << endl;
            RM_CROSSTALK = true;
        }
        else if (string( gApplication->Argv(iarg))=="-f" || string( gApplication->Argv(iarg))=="--flag" ){
            USE_CROSSTALK_FLAG = true;
        }
        else if (string( gApplication->Argv(iarg))=="-sflag"){
            cout << "Activated crosstalk flag display option." << endl;
            SHOW_FLAG = true;
        }
        else if (string( gApplication->Argv(iarg))=="-strue"){
            SHOW_TRUE = true;
        }
        else if (string( gApplication->Argv(iarg))=="-sreco"){
            SHOW_RECO = true;
        }
    }

    TFile *FileOutput = new TFile(fileOut.Data(),"RECREATE");
    TTree* dataOut    = new TTree("Events","Events");

    ND280SFGDEvent* rawEvent = new ND280SFGDEvent();
    TBranch* rawBranch = dataOut->Branch("rawEvent", "rawEvent", rawEvent);
    rawBranch->Reset();
    rawBranch->SetAddress(&rawEvent);

    ND280SFGDEvent* recoEvent = new ND280SFGDEvent();
    TBranch* recoBranch = dataOut->Branch("recoEvent", "recoEvent", recoEvent);
    recoBranch->Reset();
    recoBranch->SetAddress(&recoEvent);

    TFile *FileInput  = new TFile(fileIn.Data(),"update");
    TTree* data = (TTree*) FileInput->Get("AllEvents");
    Int_t nEvents = data->GetEntries();

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

    for (int iev=0; iev<nEvents; iev++){
        if(iev == maxEvents) break;
        if(iev > maxSelEvents) break;
        std::cout << "**** Evt " << iev << " ****" << std::endl;

        data->GetEntry(iev);

        //if(iev < 10) continue;

        std::cout << "original # of Voxels: " << inputEvent->GetVoxels().size() << std::endl;

        if(!dataType){
            rawEvent = inputEvent;
            listOfHitsCopy.clear();


            vector <ND280SFGDHit*> inputHits = inputEvent->GetHits();

            if(RM_CROSSTALK){
                cout << "REMOVING CROSSTALK." << endl;
                cout << "Hits before removal: " << inputHits.size() << endl;
                inputHits.clear();
                for(size_t ihits=0; ihits<inputEvent->GetHits().size(); ihits++){
                    if(!inputEvent->GetHits()[ihits]->GetxTalkFlag()) inputHits.push_back(inputEvent->GetHits()[ihits]);
                }
                cout << "Hits after removal: " << inputHits.size() << endl;
            }

            recoEvent = Reconstruct(inputHits,listOfHitsCopy,inputEvent->GetVoxels());
        }
        else{
            TClonesArray * unpackHits = unpackEvent->GetHits();
            vector <ND280SFGDHit*> auxHits;
            for(Int_t ihit=0; ihit<unpackEvent->GetNHits(); ihit++){
                Hit *hit = (Hit*) unpackHits->At(ihit);
                if(hit->GetDt() < -100 || hit->GetDt() > -60) continue;
                //if(hit->GetCharge() < 0 ) {cout << "Q: " << hit->GetCharge() << endl; hit->SetCharge(100);}
                ND280SFGDHit* sfgdHit = new ND280SFGDHit();
                sfgdHit->SetX(hit->GetX()+0.5 - 12);
                sfgdHit->SetY(hit->GetY()+0.5 - 4);
                sfgdHit->SetZ(hit->GetZ()+0.5 -24);
                // cout << "fTfromSpill: " << hit->GetTfromSpill() << endl;
                // cout << "fGTrigTag: " << hit->GetGTrigTag()<< endl;
                // cout << "fGTrigTime: " << hit->GetGTrigTime() << endl;
                // cout << "fSpillTime: " << hit->GetSpillTime()<< endl;
                // cout << "fDt: " << hit->GetDt()<< endl;
                // cout << "fSpillTrailTime: " << hit->GetSpillTrailTime() << endl << endl;

                // cout << "Q" << hit->GetCharge() << endl;
                // cout << "ToT" << hit->GetToT() << endl;
                // cout << "ratio: " << hit->GetToT() / hit->GetCharge() << endl;
                QvsToTratio->Fill(hit->GetToT() / hit->GetCharge());

                sfgdHit->SetDt(hit->GetDt());
                sfgdHit->SetCharge(hit->GetCharge());
                sfgdHit->SetView(hit->GetView());
                sfgdHit->SetMultiplicity(0);
                sfgdHit->SetTrueXTalk(kFALSE);
                sfgdHit->SetPDG(-999);
                sfgdHit->SetTrackID(-999);
                auxHits.push_back(sfgdHit);
                hitsQ->Fill(hit->GetCharge());
            }
            if(auxHits.size() < 10) continue;
            rawEvent->SetHits(auxHits);
            
            listOfHitsCopy.clear();
            recoEvent = Reconstruct(auxHits,listOfHitsCopy,rawEvent->GetVoxels());

            //// ------ What to do with the reconstructed event -------////

            // if(recoEvent->GetTracks().size() == 1)
            //     if(recoEvent->GetTracks()[0]->GetVoxels().size() > 30 ){
            //         recoEvent->DrawHits(kFALSE,kFALSE,"cc1"); recoEvent->DrawTracks();
            //         ++selEvents;
            //     }                    

            //// ------------------------ end ----------------------------////

            auxHits.clear();
        }

        // for(size_t sh=0;sh<recoEvent->GetHits().size();sh++){
        //     cout << "Q: " << recoEvent->GetHits()[sh]->GetCharge() << endl;
        // }

        for(size_t sh=0;sh<recoEvent->GetHits().size();sh++){
            hitsRecoQ->Fill(recoEvent->GetHits()[sh]->GetCharge());
        }


        std::cout << "reconstructed # of Voxels: " << recoEvent->GetVoxels().size() << std::endl;
        std::cout << "reconstructed # of Hits: " <<   recoEvent->GetHits().size() << std::endl;

        //recoEvent->DrawHits(kTRUE,kTRUE,"cc1");

        std::cout << "**********************" << std::endl << std::endl;

        dataOut->Fill();
        // if(!dataType){
        //     for(size_t nhits=0; nhits<listOfHitsCopy.size(); nhits++) delete listOfHitsCopy[nhits];
        // }

        selEvents++;
    }

     cout << "final number of selected events: " << selEvents << endl;

     TCanvas* c = new TCanvas("C");
     c->cd();
     // hitsRecoQ->SetLineColor(kRed);
     // hitsRecoQ->Draw("HIST");
     // hitsQ->Draw("same");
     //QvsToTratio->Fit("landau","", "",0.3,1);
     //QvsToTratio->Draw("HIST");

     c->Update();

    FileOutput->cd();
    dataOut->Write("",TObject::kOverwrite);
    FileOutput->Close();

    delete FileInput;
    delete FileOutput;
    /// ------------END--------------

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
