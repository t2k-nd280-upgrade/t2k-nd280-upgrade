#define THIS_NAME ana_ReadFile
#define NOINTERACTIVE_OUTPUT
#define OVERRIDE_OPTIONS

#include "../utils/global_header.h"
#include <TError.h>
#include <time.h> 

void ana_ReadFile() {

    cout << endl << "Staring the execution." << endl;
    cout << "The goal of this macro is to visualize selected events." << endl;

    TString file      = "/Users/cjesus/Desktop/MC_output.root";

    Int_t jobMode   = 0;
    TString prefix  = "";
    vector<TString> listOfFiles;

    float time = clock();

    /// ------------START--------------

    TChain* data = new TChain("AllEvents");

    cout << endl;
    for (int iarg=0; iarg<gApplication->Argc(); iarg++){
        if (string( gApplication->Argv(iarg))=="-f" || string( gApplication->Argv(iarg))=="--file" ){
            iarg++;
            file = gApplication->Argv(iarg);
            if (file.Contains(".root")) {
                cout << "Adding filename" <<" " << file << endl;
                listOfFiles.push_back(file);
            }
            else {
                fstream fList(file);
                if (fList.good()) {
                    while (fList.good()) {
                        string filename;
                        getline(fList, filename);
                        if (fList.eof()) break;
                        cout << "adding filename" <<" " << filename << endl;
                        listOfFiles.push_back(filename);
                    }
                }
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
            cout << "   -f || --file      input file (*.root) or filelist." << endl;
            cout << "   -o || --output    output folder for plots and out file." << endl;
            cout << "   -h || --help      print help info." << endl;
            cout << "   -j || --job       closes the macro at the end of the execution." << endl;
            cout << "**************************************" << endl;
        }
    }

    if(listOfFiles.size() == 0) {listOfFiles.push_back(file); cout << "Adding default file" <<" " << file << endl;}
    cout << endl;

    //Use TChain to merge all files in a single file.
    for (UInt_t ifile=0; ifile <listOfFiles.size(); ifile++){
        file = listOfFiles[ifile];
        data->AddFile(file);
    }

    ND280SFGDEvent* exampleEvent = new ND280SFGDEvent();

    TBranch *EvBranch  = data->GetBranch("Event");
    EvBranch->SetAddress(&exampleEvent);

    Int_t nEvents = data->GetEntries();

    cout << "Total # of Events: " << nEvents << endl; 

    for (int iev=0; iev<nEvents; iev++){

        data->GetEntry(iev);

        if(!exampleEvent->GetHits().size()) continue;

            // for (UInt_t ite=0; ite<exampleEvent->GetVoxels().size(); ite++){
            //     cout << endl << "-------------------"        << endl;
            //     cout << "X-cube:  " << exampleEvent->GetVoxel(ite)->GetX()      << endl;
            //     cout << "Y-cube:  " << exampleEvent->GetVoxel(ite)->GetY()      << endl;
            //     cout << "Z-cube:  " << exampleEvent->GetVoxel(ite)->GetZ()      << endl;
            //     cout << "PDG:     " << exampleEvent->GetVoxel(ite)->GetPDG()       << endl;
            //     cout << "trackID: " << exampleEvent->GetVoxel(ite)->GetTrackID()   << endl;
            //     cout << "-------------------"                << endl << endl;
            // }

        cout << "Number of hits in the event: "   << exampleEvent->GetHits().size() << endl;
        cout << "Number of voxels in the event: " << exampleEvent->GetVoxels().size() << endl;
        cout << "Number of tracks in the event: " << exampleEvent->GetTracks().size() << endl;
        exampleEvent->DrawHitsAndVoxels();
        //exampleEvent->GetData()->Reset();

        // if(exampleEvent->GetTracks().size() > 0){
        //     ND280SFGDTrack * auxTrack = exampleEvent->GetTracks()[0];
        //     auxTrack->DrawHitsAndVoxels();
        // }

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
