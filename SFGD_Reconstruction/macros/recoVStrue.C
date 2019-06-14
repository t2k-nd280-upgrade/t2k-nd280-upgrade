#define THIS_NAME recoVStrue
#define NOINTERACTIVE_OUTPUT
#define OVERRIDE_OPTIONS

#include "../utils/global_header.h"
#include <TError.h>
#include <time.h> 
#include <TLine.h>

// double FiberAttenuation(double x) {
//     // double LongCompFrac_FGD = 0.816;
//     // double LongAtt_FGD = 11926.; //*CLHEP::mm;
//     // double ShortAtt_FGD = 312.; //*CLHEP::mm;

//     double LongCompFrac_FGD = 0.77;
//     double LongAtt_FGD = 4634.;//*CLHEP::mm;
//     double ShortAtt_FGD = 332.;//*CLHEP::mm;

//     double a = LongCompFrac_FGD;
//     double LongAtt = LongAtt_FGD;
//     double ShortAtt = ShortAtt_FGD;  

//     return ( a*exp((-x)/LongAtt) + (1-a)*exp((-x)/ShortAtt) );
// }

double FiberAttenuation(double ell) {
    const double LongCompFrac_FGD = 0.77;
    const double LongAtt_FGD = 4634.;//*CLHEP::mm;
    const double ShortAtt_FGD = 332.;//*CLHEP::mm;

    return ( LongCompFrac_FGD*exp((-ell)/LongAtt_FGD) + (1-LongCompFrac_FGD)*exp((-ell)/ShortAtt_FGD) );
}

void recoVStrue() {

    cout << endl << "Staring the execution." << endl;
    cout << "The goal of this macro is to visualize selected events." << endl;

    TString file      = "/Users/cjesus/Desktop/Reconstruction_output.root";

    Int_t jobMode   = 0;
    TString prefix  = "/Users/cjesus/Desktop/";
    TString foutName = "Reconstruction_performance.root";

    TH1D* h_ghosts        = new TH1D("h_ghosts","h_ghosts",200,0,600);
    TH2D* h_tracks        = new TH2D("h_tracks","h_tracks",8,0,8,8,0,8);
    TH2D* h_charge        = new TH2D("h_charge","h_charge",20,0,300,20,0,300);
    TH1D* h_chargeTrue    = new TH1D("h_chargeTrue","h_chargeTrue",100,0,1600);
    TH1D* h_chargeReco    = new TH1D("h_chargeReco","h_chargeReco",100,0,1600);

    TH1D* h_chargeTrue_m1  = new TH1D("h_chargeTrue_m1","h_chargeTrue_m1",100,0,1600);
    TH1D* h_chargeTrue_m2  = new TH1D("h_chargeTrue_m2","h_chargeTrue_m2",100,0,1600);

    TH1D* h_chargeReco_m1  = new TH1D("h_chargeReco_m1","h_chargeReco_m1",100,0,1600);
    TH1D* h_chargeReco_m2  = new TH1D("h_chargeReco_m2","h_chargeReco_m2",100,0,1600);

    TH2D* h_crosstalkFlag = new TH2D("h_xtalkFlag","h_xtalkFlag",20,0,500,20,0,500);

    TH1D* h_trueCrosstalkQ = new TH1D("h1","h1",100,0,300);
    TH1D* h_trueCoreQ = new TH1D("h2","h2",100,0,300);

    TH1D* h_Qresolution = new TH1D("h_Qresolution","h_Qresolution",200,-100,100);

    TH1D* h_Qresolution_m1 = new TH1D("h_Qresolution_m1","h_Qresolution_m1",200,-100,100);
    TH1D* h_Qresolution_m2 = new TH1D("h_Qresolution_m2","h_Qresolution_m2",200,-100,100);

    TH1D* Qxy = new TH1D("Qxy","Qxy",100,0,300);
    TH1D* Qxz = new TH1D("Qxz","Qxz",100,0,300);
    TH1D* Qyz = new TH1D("Qyz","Qyz",100,0,300);

    TH1D* Qxy_c = new TH1D("Qxy_c","Qxy_c",100,0,300);
    TH1D* Qxz_c = new TH1D("Qxz_c","Qxz_c",100,0,300);
    TH1D* Qyz_c = new TH1D("Qyz_c","Qyz_c",100,0,300);

    float time = clock();

    Int_t NtrueVox = 0;
    Int_t NrecoVox = 0;
    // counters for efficiencies:

    Int_t total_hits                  = 0;
    Int_t good_flag                   = 0;
    Int_t bad_flag_on_true_core_hit   = 0;
    Int_t bad_flag_on_true_crosstalk  = 0;

    /// ------------START--------------

    TFile *FileInput=new TFile(file.Data(),"read");
    TTree* data = (TTree*) FileInput->Get("Events");

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
    TBranch *TrueBranch = data->GetBranch("rawEvent");
    TrueBranch->SetAddress(&TrueEvent);
    cout << "ENTRIES true: " << TrueBranch->GetEntries() << endl;

    ND280SFGDEvent* RecoEvent = new ND280SFGDEvent();
    TBranch* RecoBranch = data->GetBranch("recoEvent");
    RecoBranch->SetAddress(&RecoEvent);
    cout << "ENTRIES reco: " << RecoBranch->GetEntries() << endl;

    Int_t nEvents = data->GetEntries();

    cout << "Total # of Events: " << nEvents << endl; 

    for (int iev=0; iev<nEvents; iev++){

        data->GetEntry(iev);
        if(!TrueEvent || !RecoEvent){cerr << "No event in the branch!" << endl; continue;}

        cout << "*****   Evt " << iev <<  "   *****" << endl;   

    vector <ND280SFGDVoxel*> listOfVoxels = RecoEvent->GetVoxels();

        //TrueEvent->DrawHits(kTRUE,kTRUE,"cc1");
        //RecoEvent->DrawHits(kTRUE,kTRUE,"cc2");
 
        vector <ND280SFGDHit*> listOfTrueHit = TrueEvent->GetHits();
        vector <ND280SFGDHit*> listOfRecoHit = RecoEvent->GetHits();

        cout << "# of true hits: " << listOfTrueHit.size() << endl;
        cout << "# of reco hits: " << listOfRecoHit.size() << endl;

        for (size_t ihit=0; ihit<listOfTrueHit.size(); ihit++){
            total_hits++;
            if(listOfTrueHit[ihit]->GetxTalkFlag() == kTRUE) h_trueCrosstalkQ->Fill(listOfTrueHit[ihit]->GetCharge());
            if(listOfTrueHit[ihit]->GetxTalkFlag() == kFALSE) h_trueCoreQ->Fill(listOfTrueHit[ihit]->GetCharge());
            for (size_t jhit=0; jhit<listOfRecoHit.size(); jhit++){
                if(listOfTrueHit[ihit]->GetX()== listOfRecoHit[jhit]->GetX() && listOfTrueHit[ihit]->GetY()== listOfRecoHit[jhit]->GetY() && listOfTrueHit[ihit]->GetZ()== listOfRecoHit[jhit]->GetZ()){
                    if(listOfTrueHit[ihit]->GetCharge() != listOfRecoHit[jhit]->GetCharge()){                    
                    }
                    if (listOfTrueHit[ihit]->GetxTalkFlag() == listOfRecoHit[jhit]->GetxTalkFlag()) good_flag++;
                    else{
                        if(listOfTrueHit[ihit]->GetxTalkFlag() == kTRUE) bad_flag_on_true_crosstalk++;
                        else bad_flag_on_true_core_hit++;
                    }
                }
            }    
        }

        vector <ND280SFGDVoxel*> listOfTrueVox = TrueEvent->GetVoxels();
        vector <ND280SFGDVoxel*> listOfRecoVox = RecoEvent->GetVoxels();

        cout << "# of true voxels: " <<  listOfTrueVox.size() << endl; 
        cout << "# of reco voxels: " <<  listOfRecoVox.size() << endl;

        NtrueVox += listOfTrueVox.size();
        NrecoVox += listOfRecoVox.size();

        for(size_t ivox=0; ivox< listOfTrueVox.size(); ivox++){

            double distX = 10*(listOfTrueVox[ivox]->GetX() + 102);
            double distY = 10*(listOfTrueVox[ivox]->GetY() + 26);
            double distZ = 10*(listOfTrueVox[ivox]->GetZ() + 94);

            double reco_pex = listOfTrueVox[ivox]->GetHits()[2]->GetCharge() / (FiberAttenuation(distX));
            double reco_pey = listOfTrueVox[ivox]->GetHits()[1]->GetCharge() / (FiberAttenuation(distY));
            double reco_pez = listOfTrueVox[ivox]->GetHits()[0]->GetCharge() / (FiberAttenuation(distZ));

            Qyz_c->Fill(reco_pex);
            Qxz_c->Fill(reco_pey);
            Qxy_c->Fill(reco_pez);

            Qyz->Fill(listOfTrueVox[ivox]->GetHits()[2]->GetCharge());
            Qxz->Fill(listOfTrueVox[ivox]->GetHits()[1]->GetCharge());
            Qxy->Fill(listOfTrueVox[ivox]->GetHits()[0]->GetCharge());


            for(size_t jvox=0; jvox< listOfRecoVox.size(); jvox++){
                if(listOfTrueVox[ivox]->GetX() == listOfRecoVox[jvox]->GetX() && listOfTrueVox[ivox]->GetY() == listOfRecoVox[jvox]->GetY() && listOfTrueVox[ivox]->GetZ() == listOfRecoVox[jvox]->GetZ()){  
                    cout << endl << "expected: " << listOfTrueVox[ivox]->GetEdep() << endl;
                    cout << "reconstructed: " << listOfRecoVox[jvox]->GetEdep() << endl;
                    cout << listOfRecoVox[jvox]->GetHits()[0]->GetCharge() << ", " << listOfRecoVox[jvox]->GetHits()[1]->GetCharge() << ", " << listOfRecoVox[jvox]->GetHits()[2]->GetCharge() << endl;
                    h_charge->Fill(listOfTrueVox[ivox]->GetEdep(), listOfRecoVox[jvox]->GetEdep());
                    h_Qresolution->Fill( 100*(listOfTrueVox[ivox]->GetEdep()-listOfRecoVox[jvox]->GetEdep())/listOfRecoVox[jvox]->GetEdep());
                    if(listOfRecoVox[jvox]->GetHits()[0]->GetMultiplicity() == 1 || listOfRecoVox[jvox]->GetHits()[1]->GetMultiplicity() == 1 || listOfRecoVox[jvox]->GetHits()[2]->GetMultiplicity() == 1) {
                        h_Qresolution_m1->Fill( 100*(listOfTrueVox[ivox]->GetEdep()-listOfRecoVox[jvox]->GetEdep())/(listOfTrueVox[ivox]->GetEdep()));
                        h_chargeTrue->Fill(listOfTrueVox[ivox]->GetEdep());
                        h_chargeReco->Fill(listOfRecoVox[jvox]->GetEdep());
                        h_chargeTrue_m1->Fill(listOfTrueVox[ivox]->GetEdep());
                        h_chargeReco_m1->Fill(listOfRecoVox[jvox]->GetEdep());
                    }
                    else {
                        h_chargeTrue->Fill(listOfTrueVox[ivox]->GetEdep());
                        h_chargeReco->Fill(listOfRecoVox[jvox]->GetEdep());
                        h_chargeTrue_m2->Fill(listOfTrueVox[ivox]->GetEdep());
                        h_chargeReco_m2->Fill(listOfRecoVox[jvox]->GetEdep());
                        h_Qresolution_m2->Fill( 100*(listOfTrueVox[ivox]->GetEdep()-listOfRecoVox[jvox]->GetEdep())/(listOfTrueVox[ivox]->GetEdep()));
                    }
                }
            }
        }

        cout << "#TRUE: " << listOfTrueVox.size() <<endl;
        cout << "#RECO: " << listOfRecoVox.size() <<endl;
        // if(  listOfRecoVox.size()*1./listOfTrueVox.size() -1 > 0.4){
        //   //TrueEvent->DrawHits(kTRUE,kTRUE,"cc3");
        //   RecoEvent->DrawHits(kTRUE,kTRUE,"cc4");  
        // } 
        Int_t CNT_seen = 0;
        
        for(size_t jvox=0; jvox< listOfRecoVox.size(); jvox++){
            bool seen = false;
            for(size_t ivox=0; ivox< listOfTrueVox.size(); ivox++){
                if(listOfTrueVox[ivox]->GetX() == listOfRecoVox[jvox]->GetX() && listOfTrueVox[ivox]->GetY() == listOfRecoVox[jvox]->GetY() && listOfTrueVox[ivox]->GetZ() == listOfRecoVox[jvox]->GetZ()){
                    seen = true;
                    CNT_seen++;
                    break;
                }
            }
            if(!seen){
                h_ghosts->Fill(listOfRecoVox[jvox]->GetEdep());
                h_chargeReco->Fill(listOfRecoVox[jvox]->GetEdep());
                if(listOfRecoVox[jvox]->GetHits()[0]->GetMultiplicity() == 1 || listOfRecoVox[jvox]->GetHits()[1]->GetMultiplicity() == 1 || listOfRecoVox[jvox]->GetHits()[2]->GetMultiplicity() == 1) {
                        h_chargeReco_m1->Fill(listOfRecoVox[jvox]->GetEdep());
                }
                else    h_chargeReco_m2->Fill(listOfRecoVox[jvox]->GetEdep());
            }
        }
        cout << "#GHOSTS: " << listOfRecoVox.size() - CNT_seen << endl;


        // for(size_t ivox=0; ivox< listOfTrueVox.size(); ivox++){
        //     bool seen = false;
        //     for(size_t jvox=0; jvox< listOfRecoVox.size(); jvox++){
        //         if(listOfTrueVox[ivox]->GetX() == listOfRecoVox[jvox]->GetX() && listOfTrueVox[ivox]->GetY() == listOfRecoVox[jvox]->GetY() && listOfTrueVox[ivox]->GetZ() == listOfRecoVox[jvox]->GetZ()){
        //             seen = true;
        //             break;
        //         }
        //     }
        //     if(!seen) listOfTrueVox[ivox]->SetEdep(100000);
        //     //if(!seen) h_ghosts->Fill(listOfTrueVox[ivox]->GetEdep());
        // }

        //TrueEvent->DrawHits(kTRUE,kTRUE,"cc1");
        //RecoEvent->DrawHits(kTRUE,kTRUE,"cc2");

        // cout << "alive" << endl;   

        // h_tracks->Fill(TrueEvent->GetTracks().size(), RecoEvent->GetTracks().size() );

        // cout << "Number of hits in the event: "   << TrueEvent->GetHits().size() << endl;
        // cout << "Number of voxels in the event: " << TrueEvent->GetVoxels().size() << endl;
        // cout << "Number of tracks in the event: " << TrueEvent->GetTracks().size() << endl;
        // cout << "----------------------" << endl << endl;
        // cout << "***** RECO Evt " << iev <<  "   *****" << endl;   
        // cout << "Number of hits in the event: "   << RecoEvent->GetHits().size() << endl;
        // cout << "Number of voxels in the event: " << RecoEvent->GetVoxels().size() << endl;
        // cout << "Number of tracks in the event: " << RecoEvent->GetTracks().size() << endl;
        // cout << "----------------------" << endl << endl;
    }

    TCanvas* results = new TCanvas("results");
    results->Divide(3,2);

    results->cd(1);
    //h_Qresolution->Draw("HIST");
    h_Qresolution_m1->SetLineColor(kRed);
    h_Qresolution_m1->Draw("HIST");
    h_Qresolution_m2->Draw("HIST same");
    // h_trueCrosstalkQ->SetLineColor(kRed);
    // h_trueCrosstalkQ->Draw("HIST");
    // h_trueCoreQ->Draw("HIST same");//h_ghosts->Draw("COLZ");
    results->cd(2);
    h_chargeTrue_m1->SetLineColor(kRed);
    h_chargeReco_m1->Draw("HIST");
    h_chargeTrue_m1->Draw("HIST same");
    results->cd(3);
    h_chargeTrue_m2->SetLineColor(kRed);
    h_chargeReco_m2->Draw("HIST");
    h_chargeTrue_m2->Draw("HIST same");

    //h_crosstalkFlag->Draw("COLZ");
    results->cd(4);
    h_charge->Draw("COLZ");
    TLine *line = new TLine(0,0,300,300);
    line->SetLineColor(kRed);
    line->Draw();
    results->cd(5);
    h_chargeTrue->SetLineColor(kRed);
    cout << h_chargeTrue->GetEntries() << endl;
    cout << h_chargeReco->GetEntries() << endl;
    h_chargeReco->Draw("HIST");
    h_chargeTrue->Draw("HIST same");
    results->Update();
    results->cd(6);
    h_ghosts->Draw("HIST");
    //results->WaitPrimitive();

    cout << endl << endl << "-- Ghosts --" << endl;
    cout << "Total True Vox: " << NtrueVox << endl; 
    cout << "Total Reco Vox: " << NrecoVox << endl;
    cout << "Ghosts: " << h_ghosts->GetEntries() << endl;
    cout << "Missing true voxels: " << NtrueVox-(NrecoVox-h_ghosts->GetEntries()) << endl << endl;

    cout << "-- Flag efficiency --" << endl;
    cout << "-- Total hits:       " << total_hits << endl;
    cout << "-- Total flagged:    " << good_flag + bad_flag_on_true_crosstalk + bad_flag_on_true_core_hit << endl;
    cout << "-- Good:             " << good_flag << endl;
    cout << "-- Bad on core:      " << bad_flag_on_true_core_hit << endl;
    cout << "-- Bad on crosstalk: " << bad_flag_on_true_crosstalk << endl;

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
