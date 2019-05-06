#define THIS_NAME applyReconstruction
#define NOINTERACTIVE_OUTPUT
#define OVERRIDE_OPTIONS

#include "../utils/global_header.h"
#include <TError.h>
#include <time.h> 

#include "../src/Reconstruction.cc"

void applyReconstruction() {

    cout << endl << "Staring the execution." << endl;
    // cout << "The goal of this macro is to apply the reconstruction algorithms (/SFGD_Reconstruction/src/) over the input events."       << endl 
    //      << "The reconstructed events are stored in the input file inside a new branch called *RecoEvents*. This reconstructed "        << endl 
    //      << "events can be used later to evaluate the perfomance of the reconstruction methods if the original input comes from MC,"   << endl 
    //      << "or to perform physics studies if the orignal input comes from beam test data."  << endl;

    TString fileIn      = "/Users/cjesus/Desktop/MC_output.root";
    //TString fileIn   = "/Users/cjesus/Documents/PhD/SFGD/25August_8/25August_8_MCR0_hadrons_0pt8Gev_0pt0T_Beam___NewStructure.root";
    TString fileOut  = "/Users/cjesus/Desktop/Reconstruction_output.root";

    Int_t jobMode    = 0;
    TString prefix   = "/Users/cjesus/Desktop/";
    TString foutName = "Reconstruction_output.root";

    Int_t dataType   = 0; // 0 for MC, 1 for beam test Data. 

    float time = clock();

    /// ------------START--------------

    TFile *FileOutput = new TFile(fileOut.Data(),"RECREATE");
    TFile *FileInput=new TFile(fileIn.Data(),"update");
    TTree* data = (TTree*) FileInput->Get("AllEvents");

    cout << endl;
    for (int iarg=0; iarg<gApplication->Argc(); iarg++){
        if (string( gApplication->Argv(iarg))=="-f" || string( gApplication->Argv(iarg))=="--file" ){
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
            iarg++;
            jobMode = 1;
        }
        else if (string( gApplication->Argv(iarg))=="-h" || string( gApplication->Argv(iarg))=="--help" ){
            cout << "**************************************" << endl;
            cout << "Macros run options:" << endl;
            cout << "   -f || --file      input file (*.root)." << endl;
            cout << "   -o || --output    output file, including the path." << endl;
            cout << "   -h || --help      print help info." << endl;
            cout << "   -j || --job       closes the macro at the end of the execution." << endl;
            cout << "   -m || --mode      0 for MC, 1 for beam test data.  " << endl;
            cout << "**************************************" << endl;
        }
    }

    ND280SFGDEvent* TrueEvent = new ND280SFGDEvent();
    TBranch *TrueBranch;
    if(!dataType) = data->GetBranch("Event");
    else          = data->GetBranch("Event");
    TrueBranch->SetAddress(&TrueEvent);

    ND280SFGDEvent* RawEvent = new ND280SFGDEvent();
    TBranch* RawBranch = data->Branch("RawEvent", "RawEvent", RawEvent);
    RawBranch->Reset();
    RawBranch->SetAddress(&RawEvent);

    ND280SFGDEvent* RecoEvent = new ND280SFGDEvent();
    TBranch* RecoBranch = data->Branch("RecoEvent", "RecoEvent", RecoEvent);
    RecoBranch->Reset();
    RecoBranch->SetAddress(&RecoEvent);

    Int_t nEvents = data->GetEntries();

    cout << "Total # of Events: " << nEvents << endl << endl; 

    for (int iev=0; iev<nEvents; iev++){

        data->GetEntry(iev);

        std::cout << "**** Evt " << iev << " ****" << std::endl;
        std::cout << "original # of Voxels: " << TrueEvent->GetVoxels().size() << std::endl;

        // TO WORK WITHOUT CROSSTALK:
        // vector <ND280SFGDHit*> auxHitList;
        // for(UInt_t i=0; i<TrueEvent->GetHits().size(); i++){
        //     if(TrueEvent->GetHits()[i]->GetTrueXTalk() == kFALSE) auxHitList.push_back(TrueEvent->GetHits()[i]);
        // }
        // RecoEvent = Reconstruct(auxHitList);

        if (! dataType) RawEvent  = TrueEvent;
        else RawEvent = TrueEvent;
        RecoEvent = Reconstruct(TrueEvent->GetHits());

        std::cout << "**********************" << std::endl << std::endl;

        RawBranch->Fill();
        RecoBranch->Fill();
    }

    FileInput->cd();
    data->Write("",TObject::kOverwrite);
    FileInput->Close();

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
