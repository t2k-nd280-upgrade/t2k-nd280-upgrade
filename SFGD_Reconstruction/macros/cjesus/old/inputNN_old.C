#define THIS_NAME inputNN
#define NOINTERACTIVE_OUTPUT
#define OVERRIDE_OPTIONS

#include "../../utils/global_header.h"
#include "../../utils/global_tools.C"
#include <TError.h>
#include <time.h> 
#include <TMath.h>
#include "../../utils/global_variables.h"

#include "../src/HitsToVoxels.cc"
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
    double x_asim     = -999;
    double y_asim     = -999;
    double z_asim     = -999;
    double centroid   = -999;
    double qave       = -999;
    double qavemult   = -999;
    double r1         = -999;
    double r2         = -999;
    double r3         = -999;
    double qr1        = -999;
    double qr2        = -999;
    double qr3        = -999;
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
    // cout << "The goal of this macro is to apply the reconstruction algorithms (/SFGD_Reconstruction/src/) over the input events."       << endl 
    //      << "The reconstructed events are stored in the input file inside a new branch called *RecoEvents*. This reconstructed "        << endl 
    //      << "events can be used later to evaluate the perfomance of the reconstruction methods if the original input comes from MC,"   << endl 
    //      << "or to perform physics studies if the orignal input comes from beam test data."  << endl;

    Int_t maxEvents   = 100000;
    Int_t maxSelEvents= 100000;
    Int_t selEvents   = 0;

    int asim_nbin = 5;
    int asim_min = -2;
    int asim_max = 2;

    TH1F* centroid_dist[3];
    centroid_dist[0] = new TH1F("cent_dist0","",100,0,5);
    centroid_dist[1] = new TH1F("cent_dist1","",100,0,5);
    centroid_dist[2] = new TH1F("cent_dist2","",100,0,5);

    TH2F* XY_asim[3];
    XY_asim[0] = new TH2F("XY_asim0","",asim_nbin,asim_min,asim_max,asim_nbin,asim_min,asim_max);
    XY_asim[1] = new TH2F("XY_asim1","",asim_nbin,asim_min,asim_max,asim_nbin,asim_min,asim_max);
    XY_asim[2] = new TH2F("XY_asim2","",asim_nbin,asim_min,asim_max,asim_nbin,asim_min,asim_max);

    TH2F* M_corr[3];
    M_corr[0] = new TH2F("M_corr0","",10,0,10,10,0,10);
    M_corr[1] = new TH2F("M_corr1","",10,0,10,10,0,10);
    M_corr[2] = new TH2F("M_corr2","",10,0,10,10,0,10);

    TH1F* X_asim[3];
    Add1DHist(X_asim,"X_asim",5,-2,2);
    TH1F* Y_asim[3];
    Add1DHist(Y_asim,"Y_asim",5,-2,2);
    TH1F* Z_asim[3];
    Add1DHist(Z_asim,"Z_asim",5,-2,2);
    TH1F* MultXY[3];
    Add1DHist(MultXY,"MultXY",100,0,100);
    TH1F* MultXZ[3];
    Add1DHist(MultXZ,"MultXZ",100,0,100);
    TH1F* MultYZ[3];
    Add1DHist(MultYZ,"MultYZ",100,0,100);
    TH1F* AveQ[3];
    Add1DHist(AveQ,"AveQ",1000,0,3000);
    TH1F* AveM[3];
    Add1DHist(AveM,"AveM",25,0,25);
    TH1F* AveQw[3];
    Add1DHist(AveQw,"AveM",100,0,1000);
    TH1F* R1[3];
    Add1DHist(R1,"R1",10,0,10);
    TH1F* R2[3];
    Add1DHist(R2,"R2",50,0,50);
    TH1F* R3[3];
    Add1DHist(R3,"R3",150,0,150);


    bool batch = false;

    TString fileIn   = "/Users/cjesus/Documents/Data/MC_output.root";
    //TString fileOut  = "/Users/cjesus/Desktop/inputNN.root";



    Int_t jobMode    = 0;
    TString prefix   = "/Users/cjesus/Desktop/";
    TString foutName = "example.txt";

    Int_t dataType   = 0; // 0 for MC, 1 for beam test Data. 

    
    float time = clock();

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
            cout << "   -b || --batch     Runs in batch mode." << endl;
            cout << "Display options: " << endl;
            cout << "   -sflag            Shows crosstalk flag algorithm events." << endl;
            cout << "   -strue            Shows true event displays." << endl;
            cout << "   -sreco            Shows reco event displays." << endl;
            cout << "**************************************" << endl;
        }
        else if (string( gApplication->Argv(iarg))=="-d" || string( gApplication->Argv(iarg))=="--data" ){
            dataType = 1;
            fileIn    = "/Users/cjesus/Documents/PhD/SFGD/25August_8/25August_8_MCR0_hadrons_0pt8Gev_0pt0T_Beam___NewStructure.root";
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
            foutName = gApplication->Argv(iarg);
        }
        else if (string( gApplication->Argv(iarg))=="-j" || string( gApplication->Argv(iarg))=="--job" ){
            jobMode = 1;
        }
        else if (string( gApplication->Argv(iarg))=="-b" || string( gApplication->Argv(iarg))=="--batch" ){
            batch = true;
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

    //TFile *FileOutput = new TFile(fileOut.Data(),"RECREATE");
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

    ofstream myfile; 
    myfile.open (foutName.Data());
    for (int iev=0; iev<nEvents; iev++){
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
                // if(i->GetTrueXTalk()) cout << "xtalkflag: " << 1 << endl;
                // else cout << "xtalkflag: " << 0 << endl;
                // if(i->GetTrueXTalk() == true) continue;
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

            int cnt = 0;
            for(auto i:iNodes){
                int numVoxels = 0;
                double totDist = 0;
                for(auto j:iNodes){
                    double dist = pow(pow(i.x - j.x,2) + pow(i.y - j.y,2) + pow(i.z - j.z,2),0.5);
                    if(dist <= 2.5){
                        totDist += dist;
                        numVoxels++;
                    }
                }
                iNodes[cnt].centroid = totDist/numVoxels;
                cnt++;
            }
            
            cnt = 0;
            for(auto i:iNodes){
                double xAsim = 0;
                double yAsim = 0;
                double zAsim = 0;
                for(auto j:iNodes){
                    double dist = pow(pow(i.x - j.x,2) + pow(i.y - j.y,2) + pow(i.z - j.z,2),0.5);
                    if(dist <= 2){
                        xAsim += i.x - j.x;
                        yAsim += i.y - j.z;
                        zAsim += i.z - j.z;
                    }
                }
                iNodes[cnt].x_asim = xAsim;
                iNodes[cnt].y_asim = yAsim;
                iNodes[cnt].z_asim = zAsim;
                cnt++;
            }

            cnt = 0;
            for(uint it=0; it<iNodes.size();it++){
                inputNode i = iNodes[it];
                double n_r1 = 0;
                double n_r2 = 0;
                double n_r3 = 0;
                double q_r1 = 0;
                double q_r2 = 0;
                double q_r3 = 0;
                for(uint jt=it+1; jt<iNodes.size();jt++){
                    inputNode j = iNodes[jt];
                    double dist = pow(pow(i.x - j.x,2) + pow(i.y - j.y,2) + pow(i.z - j.z,2),0.5);
                    if(dist <= 1) {n_r1++; q_r1 += (j.qxy/j.mxy + j.qxz/j.mxz + j.qyz/j.myz)/3;}
                    if(dist <= 2) {n_r2++; q_r2 += (j.qxy/j.mxy + j.qxz/j.mxz + j.qyz/j.myz)/3;}  
                    if(dist <= 5) {n_r3++; q_r3 += (j.qxy/j.mxy + j.qxz/j.mxz + j.qyz/j.myz)/3;}  
                }
                iNodes[cnt].r1  = n_r1;
                iNodes[cnt].r2  = n_r2;
                iNodes[cnt].r3  = n_r3;
                n_r1 > 0 ? iNodes[cnt].qr1 = q_r1/n_r1 : iNodes[cnt].qr1 = 0;
                n_r2 > 0 ? iNodes[cnt].qr2 = q_r2/n_r2 : iNodes[cnt].qr2 = 0;
                n_r3 > 0 ? iNodes[cnt].qr3 = q_r3/n_r3 : iNodes[cnt].qr3 = 0;
                cnt++;
            }

            cnt = 0;
            for(auto i:iNodes){

                // cout << iev                                                 // event ID to group hits in same event on the txt.
                // << " " << iNodes.size()                                     // #voxels
                // << " " << i.x << " " << i.y << " " << i.z                   // position to draw events.
                // << " " << i.qxy << " " << i.qxz << " " << i.qyz             // #pe on the 3 2D views.         
                // << " " << i.mxy << " " << i.mxz << " " << i.myz             // hits multiplicity in each fiber.
                // << " " << (i.qxy + i.qxz + i.qyz)/3                         // average #pe.
                // << " " << (i.qxy/i.mxy + i.qxz/i.mxz + i.qyz/i.myz)/3       // average #pe weighted by hits multiplicity.
                // << " " << (i.mxy + i.mxz + i.myz)/3                         // average multiplicity.
                // << " " << i.r1 << " " << i.r2 << " " << i.r3                // #voxels at different radius distances.
                // << " " << i.x_asim << " " << i.y_asim << " " << i.z_asim    // asimetry for x,y,z.
                // << " " << i.qr1 << " " << i.qr1 << " " << i.qr1             // qdenisty in radius r1, r2, r3
                // << " " << i.centroid   
                // << " " << i.trueID
                // << " " << "\n";


                myfile << iev                                               // event ID to group hits in same event on the txt.
                << " " << iNodes.size()                                     // #voxels
                << " " << i.x << " " << i.y << " " << i.z                   // position to draw events.
                << " " << i.qxy << " " << i.qxz << " " << i.qyz             // #pe on the 3 2D views.         
                << " " << i.mxy << " " << i.mxz << " " << i.myz             // hits multiplicity in each fiber.
                << " " << (i.qxy + i.qxz + i.qyz)/3                         // average #pe.
                << " " << (i.qxy/i.mxy + i.qxz/i.mxz + i.qyz/i.myz)/3       // average #pe weighted by hits multiplicity.
                << " " << (i.mxy + i.mxz + i.myz)/3                         // average multiplicity.
                << " " << i.r1 << " " << i.r2 << " " << i.r3                // #voxels at different radius distances.
                << " " << i.x_asim << " " << i.y_asim << " " << i.z_asim    // asimetry for x,y,z.
                << " " << i.qr1 << " " << i.qr2 << " " << i.qr3             // qdenisty in radius r1, r2, r3
                << " " << i.centroid   
                << " " << i.trueID
                << " " << "\n";

                if(i.trueID>=0 && i.trueID<=2){
                    centroid_dist[i.trueID]->Fill(i.centroid);
                    X_asim [i.trueID]->Fill(i.x_asim);
                    Y_asim [i.trueID]->Fill(i.y_asim);
                    Z_asim [i.trueID]->Fill(i.z_asim); 
                    MultXY [i.trueID]->Fill(i.mxy);
                    MultXZ [i.trueID]->Fill(i.mxz);
                    MultYZ [i.trueID]->Fill(i.myz);
                    AveM   [i.trueID]->Fill((i.mxy + i.mxz + i.myz)/3);
                    AveQ   [i.trueID]->Fill((i.qxy + i.qxz + i.qyz)/3);
                    AveQw  [i.trueID]->Fill((i.qxy/i.mxy + i.qxz/i.mxz + i.qyz/i.myz)/3);
                    XY_asim[i.trueID]->Fill(i.x_asim,i.y_asim);  // to show correlation!
                    R1[i.trueID]->Fill(i.r1);
                    R2[i.trueID]->Fill(i.r2);
                    R3[i.trueID]->Fill(i.r3);
                    M_corr[i.trueID]->Fill(i.mxy,i.mxz);
                }
                cnt++;
            }
    }
  
    myfile.close();

    delete FileInput;
    //delete FileOutput;
    /// ------------END--------------

    time = ( clock() - time ) / CLOCKS_PER_SEC;
    cout << std::setprecision(8) << "Elapsed time: " << time << endl;

    if(!batch){

        Norm1DHist(X_asim);
        Norm1DHist(Y_asim);
        Norm1DHist(Z_asim);
        Norm1DHist(MultXY);
        Norm1DHist(MultXZ);
        Norm1DHist(MultYZ);
        Norm1DHist(AveQ);
        Norm1DHist(AveM);
        Norm1DHist(AveQw);
        Norm1DHist(R1);
        Norm1DHist(R2);
        Norm1DHist(R3);

        TCanvas canv2D;
        canv2D.Divide(3,1);
        canv2D.cd(1);
        XY_asim[0]->Draw("COLZ");
        canv2D.cd(2);
        XY_asim[1]->Draw("COLZ");
        canv2D.cd(3);
        XY_asim[2]->Draw("COLZ");
        canv2D.Update();

        TCanvas canvMcorr;
        canvMcorr.Divide(3,1);
        canvMcorr.cd(1);
        M_corr[0]->Draw("COLZ");
        canvMcorr.cd(2);
        M_corr[1]->Draw("COLZ");
        canvMcorr.cd(3);
        M_corr[2]->Draw("COLZ");
        canvMcorr.Update();

        TCanvas canvAve;
        canvAve.Divide(3,1);
        canvAve.cd(1);
        AveM[1]->Draw("HIST");
        AveM[0]->Draw("HIST same");
        AveM[2]->Draw("HIST same");
        canvAve.cd(2);
        AveQ[1]->Draw("HIST");
        AveQ[0]->Draw("HIST same");
        AveQ[2]->Draw("HIST same");
        canvAve.cd(3);
        AveQw[1]->Draw("HIST");
        AveQw[0]->Draw("HIST same");
        AveQw[2]->Draw("HIST same");
        canvAve.Update();

        TCanvas canvMult;
        canvMult.Divide(3,1);
        canvMult.cd(1);
        MultXY[1]->Draw("HIST");
        MultXY[0]->Draw("HIST same");
        MultXY[2]->Draw("HIST same");
        canvMult.cd(2);
        MultXZ[1]->Draw("HIST");
        MultXZ[0]->Draw("HIST same");
        MultXZ[2]->Draw("HIST same");
        canvMult.cd(3);
        MultYZ[1]->Draw("HIST");
        MultYZ[0]->Draw("HIST same");
        MultYZ[2]->Draw("HIST same");
        canvMult.Update();

        TCanvas canvRadius;
        canvRadius.Divide(3,1);
        canvRadius.cd(1);
        R1[1]->Draw("HIST");
        R1[0]->Draw("HIST same");
        R1[2]->Draw("HIST same");
        canvRadius.cd(2);
        R2[1]->Draw("HIST");
        R2[0]->Draw("HIST same");
        R2[2]->Draw("HIST same");
        canvRadius.cd(3);
        R3[1]->Draw("HIST");
        R3[0]->Draw("HIST same");
        R3[2]->Draw("HIST same");
        canvRadius.Update();

        TCanvas canv;
        canv.Divide(2,2);
        canv.cd(1);
        centroid_dist[1]->Draw("HIST");
        centroid_dist[0]->Draw("HIST same");
        centroid_dist[2]->Draw("HIST same");
        canv.cd(2);
        X_asim[1]->Draw("HIST");
        X_asim[0]->Draw("HIST same");
        X_asim[2]->Draw("HIST same");
        canv.cd(3);
        Y_asim[1]->Draw("HIST");
        Y_asim[0]->Draw("HIST same");
        Y_asim[2]->Draw("HIST same");
        canv.cd(4);
        Z_asim[1]->Draw("HIST");
        Z_asim[0]->Draw("HIST same");
        Z_asim[2]->Draw("HIST same");
        canv.Update();
        canv.WaitPrimitive();

    }

    if(!jobMode){
        cout << "This macro remains open until it is manually closed with Ctrl+C\nallowing to watch the output on the output Canvas.\nTo avoid this, use option '-j'." << endl;
        return;
    }
    else{
        cout << "Execution completed successfully." << endl << endl;
        exit(1);
    }
}
