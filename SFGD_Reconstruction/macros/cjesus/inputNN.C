#define THIS_NAME inputNN
#define NOINTERACTIVE_OUTPUT
#define OVERRIDE_OPTIONS

#include "../../utils/global_header.h"
#include "../../utils/global_tools.C"
#include "../../src/HitsToVoxels.cc"
#include <TError.h>
#include <time.h> 
#include <TMath.h>

#include <iostream>
#include <fstream>


struct inputNode{
    double x          = -999;
    double y          = -999;
    double z          = -999;
    double qxy        = -999;
    double qxz        = -999;
    double qyz        = -999;
    double mxy        = -999;
    double mxz        = -999;
    double myz        = -999;
    int trueID        = -999;
};

void Add1DHist(TH1F* h [3], TString name, int nbin, int min, int max){
    h[0] = new TH1F((name+"_track").Data(),"",nbin,min,max);
    h[1] = new TH1F((name+"_cross").Data(),"",nbin,min,max);
    h[2] = new TH1F((name+"_ghost").Data(),"",nbin,min,max);   
    h[0]->SetLineColor(kRed);
    h[1]->SetLineColor(kBlue);
    h[2]->SetLineColor(kGreen+1);
    h[0]->SetLineWidth(2);
    h[1]->SetLineWidth(2);
    h[2]->SetLineWidth(2);
}

void Norm1DHist(TH1F* h [3]){
    h[0]->Scale(1/h[0]->Integral(), "width");
    h[1]->Scale(1/h[1]->Integral(), "width");
    h[2]->Scale(1/h[2]->Integral(), "width");
}

void inputNN() {

    cout << endl << "Staring the execution." << endl;

    Int_t maxEvents   = 10;
    Int_t maxSelEvents= 100000;
    Int_t selEvents   = 0;

    TString fileIn   = "/home/cjesus/Work/Data/SFGD_final/MC/RECO/PION_RECON/Reconstructed_SFGD_MC.root";

    Int_t jobMode    = 0;
    TString prefix   = "/home/cjesus/Work/Data/SFGD_final/NN/inputNN";
    TString foutName = prefix;

    float time = clock();

    Int_t evtFirst = 0;
    Int_t evtLast  = 0; 

    cout << endl;
    for (int iarg=0; iarg<gApplication->Argc(); iarg++){
        if (string( gApplication->Argv(iarg))=="-i" || string( gApplication->Argv(iarg))=="--input" ){
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
            foutName = gApplication->Argv(iarg);
        }
        else if (string( gApplication->Argv(iarg))=="-j" || string( gApplication->Argv(iarg))=="--job" ){
            jobMode = 1;
        }
        else if (string( gApplication->Argv(iarg))=="-a"){
            iarg++;
            evtFirst = atoi(gApplication->Argv(iarg));
            foutName += "_";
            foutName += evtFirst;
        }
        else if (string( gApplication->Argv(iarg))=="-b"){
            iarg++;
            evtLast = atoi(gApplication->Argv(iarg));
            foutName += "_";
            foutName += evtLast;
        }
    }

    foutName += ".txt";

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

    if(evtFirst < 0 or evtFirst > nEvents) {cout << "wrong evtFirst!" << endl; exit(0);}
    if(evtLast < evtFirst) {cout << "wrong evtLast!" << endl; exit(0);}
    if(!evtLast or evtLast > nEvents) evtLast = nEvents; 

    TBranch *inputBranch;
    inputBranch = data->GetBranch("Event");
    ND280SFGDEvent* inputEvent = new ND280SFGDEvent();

    inputBranch->SetAddress(&inputEvent);

    cout << "Total # of Events: " << nEvents << endl << endl; 

    ofstream myfile; 
    myfile.open (foutName.Data());
    for (int iev=evtFirst; iev<evtLast; iev++){
        if(iev == maxEvents) break;
        if(iev > maxSelEvents) break;
        std::cout << "**** Evt " << iev << " ****" << std::endl;
        vector <inputNode> iNodes;
        data->GetEntry(iev);
        inputEvent->MergeHits();
        std::cout << "original # of Voxels: " << inputEvent->GetVoxels().size() << std::endl;
        
        rawEvent = inputEvent;
        vector <ND280SFGDHit*> inputHits = inputEvent->GetHits();
        vector <ND280SFGDVoxel*> trueVoxels = inputEvent->GetVoxels();
        vector <ND280SFGDVoxel*> recoVoxels = HitsToVoxels(inputHits,0);
        
        for(auto i:recoVoxels){
            i->SetPDG(2);
            for(auto j:trueVoxels){
                if (i->GetX() == j->GetX() && i->GetY() == j->GetY() && i->GetZ() == j->GetZ()) i->SetPDG(1);
            }
        }
        
        for(auto i:recoVoxels){
            if(i->GetPDG() == 2 or i->GetPDG() == 0) continue;
            for(auto j:trueVoxels){
                if( j->GetTrueXTalk()) continue;
                if (i->GetX() == j->GetX() && i->GetY() == j->GetY() && i->GetZ() == j->GetZ()) i->SetPDG(0);
            }
        }
        
        for(auto i:recoVoxels){
            i->GetHits()[0]->SetMultiplicity(i->GetHits()[0]->GetMultiplicity()+1);
            i->GetHits()[1]->SetMultiplicity(i->GetHits()[1]->GetMultiplicity()+1);
            i->GetHits()[2]->SetMultiplicity(i->GetHits()[2]->GetMultiplicity()+1);
        }
        
        for(auto i:recoVoxels){
            inputNode newNode;
            newNode.x      =  i->GetX();
            newNode.y      =  i->GetY();
            newNode.z      =  i->GetZ();
            newNode.qxy    =  i->GetHits()[0]->GetCharge();
            newNode.qxz    =  i->GetHits()[1]->GetCharge();
            newNode.qyz    =  i->GetHits()[2]->GetCharge();
            newNode.mxy    =  i->GetHits()[0]->GetMultiplicity();
            newNode.mxz    =  i->GetHits()[1]->GetMultiplicity();
            newNode.myz    =  i->GetHits()[2]->GetMultiplicity();
            newNode.myz    =  i->GetHits()[2]->GetMultiplicity();
            newNode.trueID =  i->GetPDG();  // true id stored in pdg...!
            iNodes.push_back(newNode);
        }
        
        for(auto i:iNodes){
            bool Write = false;
            if(Write){
                cout << iev                                                 // event ID to group hits in same event on the txt.
                << " " << i.x << " " << i.y << " " << i.z                   // position to draw events.
                << " " << i.qxy << " " << i.qxz << " " << i.qyz             // #pe on the 3 2D views.         
                << " " << i.mxy << " " << i.mxz << " " << i.myz             // hits multiplicity in each fiber.
                << " " << i.trueID
                << "\n";                    
            }
            myfile << iev                                                         
            << " " << i.x << " " << i.y << " " << i.z      
            << " " << i.qxy << " " << i.qxz << " " << i.qyz
            << " " << i.mxy << " " << i.mxz << " " << i.myz
            << " " << i.trueID
            << "\n";
        }
        selEvents++;
        if(selEvents%100==0) cout << "Stored events: " << selEvents << endl;
    }
  
    myfile.close();

    delete FileInput;

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
