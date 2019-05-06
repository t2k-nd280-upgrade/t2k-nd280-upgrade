#define THIS_NAME recoVStrue
#define NOINTERACTIVE_OUTPUT
#define OVERRIDE_OPTIONS

#include "../utils/global_header.h"
#include <TError.h>
#include <time.h> 

void recoVStrue() {

    cout << endl << "Staring the execution." << endl;
    cout << "The goal of this macro is to visualize selected events." << endl;

    TString file      = "/Users/cjesus/Desktop/MC_output.root";

    Int_t jobMode   = 0;
    TString prefix  = "/Users/cjesus/Desktop/";
    TString foutName = "Reconstruction_performance.root";

    float time = clock();

    /// ------------START--------------

    TFile *FileInput=new TFile(file.Data(),"read");
    TTree* data = (TTree*) FileInput->Get("AllEvents");

    cout << endl;
    for (int iarg=0; iarg<gApplication->Argc(); iarg++){
        if (string( gApplication->Argv(iarg))=="-f" || string( gApplication->Argv(iarg))=="--file" ){
            iarg++;
            file = gApplication->Argv(iarg);
            if (file.Contains(".root")) {
                cout << "Analyzing file: " << file << endl;
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
            iarg++;
            jobMode = 1;
        }
        else if (string( gApplication->Argv(iarg))=="-h" || string( gApplication->Argv(iarg))=="--help" ){
            cout << "**************************************" << endl;
            cout << "Macros run options:" << endl;
            cout << "   -f || --file      input file (*.root)." << endl;
            cout << "   -o || --output    output folder for plots and out file." << endl;
            cout << "   -h || --help      print help info." << endl;
            cout << "   -j || --job       closes the macro at the end of the execution." << endl;
            cout << "**************************************" << endl;
        }
    }

    ND280SFGDEvent* TrueEvent = new ND280SFGDEvent();
    TBranch *TrueBranch = data->GetBranch("Event");
    TrueBranch->SetAddress(&TrueEvent);
    cout << "ENTRIES true: " << TrueBranch->GetEntries() << endl;

    ND280SFGDEvent* RecoEvent = new ND280SFGDEvent();
    TBranch* RecoBranch = data->GetBranch("Reconstructed");
    RecoBranch->SetAddress(&RecoEvent);
    cout << "ENTRIES reco: " << RecoBranch->GetEntries() << endl;

    Int_t nEvents = data->GetEntries();

    cout << "Total # of Events: " << nEvents << endl; 

    for (int iev=0; iev<nEvents; iev++){

        data->GetEntry(iev);

        cout << "*****   Evt " << iev <<  "   *****" << endl;   
        cout << "Number of hits in the event: "   << TrueEvent->GetHits().size() << endl;
        cout << "Number of voxels in the event: " << TrueEvent->GetVoxels().size() << endl;
        cout << "Number of tracks in the event: " << TrueEvent->GetTracks().size() << endl;
        cout << "----------------------" << endl << endl;

        TrueEvent->DrawHits(kTRUE,kTRUE,"cc1");

        cout << "***** RECO Evt " << iev <<  "   *****" << endl;   
        cout << "Number of hits in the event: "   << RecoEvent->GetHits().size() << endl;
        cout << "Number of voxels in the event: " << RecoEvent->GetVoxels().size() << endl;
        cout << "Number of tracks in the event: " << RecoEvent->GetTracks().size() << endl;
        cout << "----------------------" << endl << endl;

        RecoEvent->DrawHits(kTRUE,kTRUE,"cc2");

        if(iev == 3) break; 
    }

    /// ------------END--------------

    time = ( clock() - time ) / CLOCKS_PER_SEC;
    cout << std::setprecision(8) << "Elapsed time: " << time << endl;

    if(!jobMode){
        cout << "This macro remains open until it is manually closed with Ctrl+C\nallowing to watch the output on the output Canvas.\nTo avoid this, use option '-j'." << endl;
        return;
    }
    else{
        cout << "Execution completed successfully." << endl << endl;
        exit(0);
    }
}
