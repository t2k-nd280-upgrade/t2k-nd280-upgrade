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


    Int_t maxEvents   = 1000;

    TString fileOut  = "/Users/cjesus/Desktop/Reconstruction_output.root";

    Int_t jobMode    = 0;
    TString prefix   = "/Users/cjesus/Desktop/";
    TString foutName = "Reconstruction_output.root";

    Int_t dataType   = 0; // 0 for MC, 1 for beam test Data. 

    TString fileIn;
    if(!dataType)  fileIn   = "/Users/cjesus/Desktop/MC_output.root";
    else           fileIn    = "/Users/cjesus/Documents/PhD/SFGD/25August_8/25August_8_MCR0_hadrons_0pt8Gev_0pt0T_Beam___NewStructure.root";

    float time = clock();

    /// ------------START--------------

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
        std::cout << "**** Evt " << iev << " ****" << std::endl;

        data->GetEntry(iev);

        std::cout << "original # of Voxels: " << inputEvent->GetVoxels().size() << std::endl;

        // TO WORK WITHOUT CROSSTALK:
        // vector <ND280SFGDHit*> auxHitList;
        // for(UInt_t i=0; i<inputEvent->GetHits().size(); i++){
        //     if(inputEvent->GetHits()[i]->GetTrueXTalk() == kFALSE) auxHitList.push_back(inputEvent->GetHits()[i]);
        // }
        // RecoEvent = Reconstruct(auxHitList);

        if(!dataType){
            recoEvent = Reconstruct(inputEvent->GetHits());
            rawEvent = inputEvent;
        }
        else{
            TClonesArray * unpackHits = unpackEvent->GetHits();
            vector <ND280SFGDHit*> auxHits;
            for(Int_t ihit=0; ihit<unpackEvent->GetNHits(); ihit++){
                Hit *hit = (Hit*) unpackHits->At(ihit);
                ND280SFGDHit* sfgdHit = new ND280SFGDHit();
                sfgdHit->SetX(hit->GetX());
                sfgdHit->SetY(hit->GetY());
                sfgdHit->SetZ(hit->GetZ());
                sfgdHit->SetCharge(hit->GetCharge());
                sfgdHit->SetView(hit->GetView());
                sfgdHit->SetMultiplicity(0);
                sfgdHit->SetTrueXTalk(kFALSE);
                sfgdHit->SetPDG(-999);
                sfgdHit->SetTrackID(-999);
                auxHits.push_back(sfgdHit);
            }
            rawEvent->SetHits(auxHits);
            recoEvent = Reconstruct(auxHits);
            auxHits.clear();
        }

        std::cout << "**********************" << std::endl << std::endl;

        dataOut->Fill();
    }

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
