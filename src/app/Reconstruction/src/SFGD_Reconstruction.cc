// 




















#include <fstream>


#include <TFile.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TH3F.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TTree.h>
#include <TVector3.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TMath.h>
#include <TStopwatch.h>
#include <TPolyMarker3D.h>
#include <TMarker.h>

#include <iostream>
#include <fstream>

#include <TRandom3.h>

#include "ND280UpConst.hh"
#include <ND280UpApplyResponse.hh>
#include "ND280UpRecoTrack.hh"
#include "ND280UpPID.hh"

#include "TND280UpEvent.hh"
#include "TND280UpVertex.hh"
#include "TND280UpRecoEvent.hh"
#include "TND280UpRecoTrack.hh"
#include "TND280UpHit.hh"
#include "ND280SFGDEvent.hh"
#include "ND280SFGDVoxel.hh"
#include "ND280SFGDVoxelSet.hh"
#include "ND280SFGDTrack.hh"
#include "ND280SFGDHit.hh"

#include "SFGD_Reconstruction.hh"

#include <map>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//#define CROSSTALK_OFF
//#define FV_CUT
//#define FORCE_NUMU

std::ofstream outCSVfile; 

int CC_NUMU = 0;
int NC_NUMU = 0;
int IN_SFGD = 0;
         
TCanvas* c_shift = new TCanvas("shift");
TH1F*    h_shift_X = new TH1F("h_shift_X","h_shift_X",500,90,100);
TH1F*    h_shift_Y = new TH1F("h_shift_Y","h_shift_Y",500,25,35);
TH1F*    h_shift_Z = new TH1F("h_shift_Z","h_shift_Z",500,85,95);

TH1F*    h_shift_int_X = new TH1F("h_shift_int_X","h_shift_int_X",21,90,100);
TH1F*    h_shift_int_Y = new TH1F("h_shift_int_Y","h_shift_int_Y",21,25,35);
TH1F*    h_shift_int_Z = new TH1F("h_shift_int_Z","h_shift_int_Z",21,85,95);

int FindParent(int inputID, std::vector<int> listOfParentID, TND280UpEvent* evt){
    if (inputID == 0) return 0;
    int outputID = -999;
    for (Int_t trjID = 0; trjID < evt->GetNTracks(); trjID++) {
        TND280UpTrack* track = evt->GetTrack(trjID);
        if (track->GetTrackID() == inputID){
            outputID = track->GetParentID();
            // cout << "parentID: " << track->GetParentID() << endl;
            // cout << "track ID: " << track->GetTrackID() << endl;
        }
    }
    //cout << endl;
    bool done = false;
    for (auto i : listOfParentID)
        if (outputID == i) done = true;

    if(outputID == -999) return -999;//return inputID;
    if(done) return outputID;
    else return FindParent(outputID,listOfParentID,evt);
 
    cout << "exit!!!! " << endl;
    exit(1);
    return -1;   
}

int FindPrimary(int inputID, TND280UpEvent* evt, int loops=0){
    int outputID;
    cout << endl;
    for (Int_t trjID = 0; trjID < evt->GetNTracks(); trjID++) {
        TND280UpTrack* track = evt->GetTrack(trjID);
        if (track->GetTrackID() == inputID){
            outputID = track->GetParentID();
            cout << "parentID: " << track->GetParentID() << endl;
            cout << "track ID: " << track->GetTrackID() << endl;
        }
    }

    if(outputID == 0 && loops == 0) return inputID; // primary
    else if(outputID == 0 && loops >= 1) return inputID; // secondary
    else{
        if(loops<1) return FindPrimary(outputID,evt,++loops);
        else{
            if (outputID == FindPrimary(outputID,evt,++loops)) return outputID;
            else return FindPrimary(outputID,evt,++loops);
        }
    }

    cout << "exit!!!! " << endl;
    exit(1);
    return -1;   
}

int BinomialCrossing(int pe){
    TRandom3 fRndm = TRandom3(0);
    int npe_passed = 0;
    double rndunif =0.;
    for (int i=0;i<pe;i++){
        rndunif = fRndm.Uniform();
        if (rndunif < 0.03) npe_passed++;
    }
    return npe_passed;
}

int SFGD_Reconstruction(int argc,char** argv) {
    
    string rootfilename = argv[1];
    TString outfilename = argv[2];;
    const int evtfirst  = atoi(argv[3]);
    const int nevents   = atoi(argv[4]);

    cout << "reading file: " << rootfilename.c_str() << endl;

    cout << "evtfirst: " << evtfirst << ",nevents: " << nevents << endl;
    cout << "outfilename: " <<outfilename.Data() << endl;

    outCSVfile.open (outfilename.Data(),std::ofstream::out);

    // TFile *fileout = new TFile(outfilename.Data(),"RECREATE");
    TTree *AllEvents = new TTree("AllEvents", "The ROOT tree of events");
    ND280SFGDEvent* event = new ND280SFGDEvent();

    int Nentries = -999;

    nd280upconv::TargetType_t DetType = nd280upconv::kSuperFGD;

    /////////////////////////////
    //                         //
    // Apply detector response //
    //                         //
    /////////////////////////////
    
    // Take inputs
    TFile *finput = new TFile(rootfilename.c_str(),"READ");
    cout << "Reading the input ROOT file: " << rootfilename << endl;

    TH2F* h2d_xy; TH2F* h2d_xz; TH2F* h2d_yz;
    h2d_xy = (TH2F*)finput->Get("OutMPPCProj2D_XY");
    h2d_xz = (TH2F*)finput->Get("OutMPPCProj2D_XZ");
    h2d_yz = (TH2F*)finput->Get("OutMPPCProj2D_YZ");

    Int_t binX = h2d_xy->GetXaxis()->GetNbins();
    Int_t binY = h2d_xy->GetYaxis()->GetNbins();
    Int_t binZ = h2d_xz->GetYaxis()->GetNbins();
    
    Float_t X_min = h2d_xy->GetXaxis()->GetBinLowEdge(0);
    Float_t X_max = h2d_xy->GetXaxis()->GetBinUpEdge(binX);
    Float_t Y_min = h2d_xy->GetYaxis()->GetBinLowEdge(0);
    Float_t Y_max = h2d_xy->GetYaxis()->GetBinUpEdge(binY);
    Float_t Z_min = h2d_xz->GetYaxis()->GetBinLowEdge(0);
    Float_t Z_max = h2d_xz->GetYaxis()->GetBinUpEdge(binZ);
    
    cout << " x min " << X_min << " x max " << X_max << "  nbins " << binX << endl;
    cout << " y min " << Y_min << " y max " << Y_max << "  nbins " << binY << endl;
    cout << " z min " << Z_min << " z max " << Z_max << "  nbins " << binZ << endl;

    AllEvents->Branch("Event", "Event", event);    
    TTree *tinput = (TTree*) finput->Get("ND280upEvents");    
    ND280UpApplyResponse ApplyResponse;
    ApplyResponse.SetMPPCProj2D_XY(h2d_xy);
    ApplyResponse.SetMPPCProj2D_XZ(h2d_xz);
    ApplyResponse.SetMPPCProj2D_YZ(h2d_yz);
    
    // Declare the input true event object
    TND280UpEvent *nd280UpEvent = new TND280UpEvent();
    tinput->SetBranchAddress("Event",&nd280UpEvent);
    
    int NTreeEntries = tinput->GetEntries();
    int evtlasttree = NTreeEntries-1;
    
    cout << endl;
    cout << "# of entries in the Tree: " << NTreeEntries << endl;
    cout << "last tree event ID: " << evtlasttree << endl;
    cout << "first event ID: " << evtfirst << endl;
    cout << endl;
    
    if(evtfirst > evtlasttree){
        cout << endl;
        cout << "first selection evt ID > first evt ID of tree:" << endl;
        cout << " - # of tree events = " << NTreeEntries << endl;
        cout << " - last tree event = " << evtlasttree << endl;
        cout << " - first event = " << evtfirst << endl;
        cout << "Exit!!!" << endl;
        cout << endl;
        exit(1);
    }
    else if(NTreeEntries < (evtfirst+nevents)){
        
        Nentries = NTreeEntries - evtfirst;
        
        cout << endl;
        cout << "WARNING:" << endl;
        cout << "first evt ID + # of events > tot # of tree events:" << endl;
        cout << " - # of tree events = " << NTreeEntries << endl;
        cout << " - # of events to run = " << nevents << endl;
        cout << " - first event = " << evtfirst << endl;
        cout << "Set # of events to run to " << Nentries;
        cout << endl;
    }
    else{
        Nentries = nevents;
    }
    
    int EntryLast = evtfirst+Nentries-1;
    
    cout << endl;
    cout << "First event = " << evtfirst << endl;
    cout << "Last event =  " << EntryLast << endl;
    cout << "# of events = " << Nentries << endl;
    cout << endl;


    ///////////  Loop over events //////////////////////


    for(int ievt=evtfirst;ievt<=EntryLast;ievt++){
        tinput->GetEntry(ievt);
        std::map<int,int> trackToParentID;
        std::map<int,int> trackToPDG;

        cout << CLHEP::cm/CLHEP::MeV << endl;

        vector <ND280SFGDHit*>   listOfHits;
        vector <ND280SFGDVoxel*> listOfVoxels;
        vector <ND280SFGDTrack*> listOfTracks;
        std::vector<int> listOfParentID;
        std::vector <int> listOfTrueTrackID;

        int vtxX = -999;
        int vtxY = -999;
        int vtxZ = -999;


        bool store = true;
        cout << "************************" << endl;
        cout << " starting event " << ievt << " and has #hits " << nd280UpEvent->GetNHits() << endl;

        int NHits = nd280UpEvent->GetNHits();

        bool mu_found = false;
        int  nPrimTrk = 0;
        bool IsNu = true;
        double iniMom = 0;

        TND280UpVertex* vrtx = nd280UpEvent->GetVertex(0);
        cout << "ReacMode: " << vrtx->GetReacModeString() << endl;
        TString ReacInfo = vrtx->GetReacModeString();
        if (!ReacInfo.Length()){
            IsNu = false;
        }

        for(int itrk=0;itrk<vrtx->GetNInTracks();itrk++){
            if(vrtx->GetInTrack(itrk)->GetPDG() == 14){
                event->SetNuMom(vrtx->GetInTrack(itrk)->GetInitMom().Mag());
                iniMom = vrtx->GetInTrack(itrk)->GetInitMom().Mag();
                cout << "neutrino momentum: " << vrtx->GetInTrack(itrk)->GetInitMom().Mag() << endl;
                vtxX = (int) binX/2+vrtx->GetPosition().X()/10;
                vtxY = (int) binY/2+vrtx->GetPosition().Y()/10+1.6;
                vtxZ = (int) binZ/2+vrtx->GetPosition().Z()/10+170.7;
                cout << vrtx->GetPosition().X() << "," << vrtx->GetPosition().Y() << "," << vrtx->GetPosition().Z() << endl;
                cout << vtxX << "," << vtxY << "," << vtxZ << endl;
            }
        }
        cout << " --- true track in the event --- " << endl;
        for (Int_t trjID = 0; trjID < nd280UpEvent->GetNTracks(); trjID++) {
            TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);
            listOfTrueTrackID.push_back(track->GetTrackID());
            listOfParentID.push_back(track->GetTrackID());
            trackToPDG[track->GetTrackID()] = track->GetPDG();
            cout << "PDG: " <<  track->GetPDG()
                     << ", ID: " <<    track->GetTrackID() 
                     << ", prntID: " << track->GetParentID() << endl;
            if(!track->GetParentID()) nPrimTrk++;
            if(!IsNu && !track->GetParentID()){
                ReacInfo += track->GetPDG();
                iniMom = track->GetInitMom().Mag();
            }
            if(track->GetPDG() == 13 && !track->GetParentID()){
                mu_found = true;
                if(IsNu){
                    ReacInfo += "_";
                    ReacInfo += track->GetInitMom().Mag();
                }
            }
        }
        cout << "-------------------------------" << endl; 

        cout << "Initial Momentum: " << iniMom << endl;
        cout << "# primary tracks: " << nPrimTrk << endl;
        #ifdef FORCE_NUMU
            if(!mu_found) store = false;
        #endif

        for(int ihit=0;ihit<NHits;ihit++){ // get last entry
            TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);

            int trkID = -999;
            double time = (nd280UpHit->GetStartT() + nd280UpHit->GetStopT())/2.; // middle step time
            double posX = binX/2+(nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/20.; // middle step X
            double posY = binY/2+(nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/20.; // middle step Y
            double posZ = binZ/2+(nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/20.; // middle step Z

            if(!ihit and ReacInfo=="13"){
                cout << posX << "," << posY << "," << posZ << endl;
                h_shift_X->Fill(posX);
                h_shift_Y->Fill(posY);
                h_shift_Z->Fill(posZ);
                h_shift_int_X->Fill((int)posX);
                h_shift_int_Y->Fill((int)posY);
                h_shift_int_Z->Fill((int)posZ);
            }

            double edep = nd280UpHit->GetEnergyDeposit();
            double steplength = nd280UpHit->GetTrackLength();
            // bool done = false;
            // std::vector<int>::reverse_iterator it = nd280UpHit->fContributors.rbegin();
            // for (; it!= nd280UpHit->fContributors.rend(); ++it){
            //     for (auto trueTrackID:listOfTrueTrackID){
            //         if ((*it) == trueTrackID) {trkID =(*it); done = true; break;}                 
            //     }
            //     if(done) break;
            // }
            // if (trkID < 0) { trkID = nd280UpHit->GetPrimaryId();}

            //________
            bool done = false;
            std::vector<int>::iterator it = nd280UpHit->fContributors.begin();
            for (; it!= nd280UpHit->fContributors.end(); ++it){
                for (auto trueTrackID:listOfTrueTrackID){
                    if ((*it) == trueTrackID) {trkID =(*it); done = true; break;}                 
                }
                if(done) break;
            }
            //if (trkID < 0) { trkID = nd280UpHit->GetPrimaryId();}
             if (trkID < 0) { trkID = 999;}
            //________

            if (trkID < 0) { cout << "NOT STORING THE EVENT! negative TRK ID!: " << trkID << endl; store = false; exit(1);}

            ND280SFGDVoxel* Voxel = new ND280SFGDVoxel(posX,posY,posZ);
            Voxel->SetTrueTime(time);
            Voxel->SetTrueEdep(edep);
            Voxel->SetTruePE(steplength); // STORING HERE steplength to avoid having an extra method...
            Voxel->AddTrueTrackID(trkID);
            Voxel->SetTrueType(0);
            listOfVoxels.push_back(Voxel);
                
        } /// END LOOP OVER 3D HITS

        cout << "# stored hits: " << listOfVoxels.size()*3 << endl;
        std::vector <int> listOfTrackID;


        for(auto v:listOfVoxels) listOfTrackID.push_back(v->GetTrueTrackIDs()[0]);

        //keep only unique track IDs:
        if(listOfTrackID.size()){
            std::sort   (listOfTrackID.begin(), listOfTrackID.end());
            listOfTrackID.erase(std::unique (listOfTrackID.begin(), listOfTrackID.end()), listOfTrackID.end()); 
        }

        int numTracks = (int) listOfTrackID.size();

        cout << "# of trkID: " << numTracks << endl;

        for (auto t_id:listOfTrackID) cout << "track_id: " << t_id  << ", pdg: " << trackToPDG[t_id] << endl;

        // // fill all trajectories information:
        // all_numTracks = nd280UpEvent->GetNTracks();
        // for (Int_t trjID = 0; trjID < nd280UpEvent->GetNTracks(); trjID++) {
        //     TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);

        //     cout << "trk is: " << track->GetTrackID() << " || GetInitMom " << track->GetInitMom().Mag()
        //          << " || trueCosTheta: " << track->GetInitCosTheta()
        //          << " || Range: " << track->GetRange() 
        //          << endl;

        //     bool found = false;
        //     for(auto trk_ID:listOfTrackID) if (trk_ID == track->GetTrackID()) found = true;
        //     if(!found) continue;

        //     ND280SFGDTrack* sfgdtrack = new ND280SFGDTrack();
        //     sfgdtrack->SetPDG(track->GetPDG());
        //     sfgdtrack->SetTrackID(track->GetTrackID());
        //     sfgdtrack->SetParentID(track->GetParentID());
        //     sfgdtrack->SetCosTheta(track->GetInitCosTheta());
        //     sfgdtrack->SetRange(track->GetRange());
        //     sfgdtrack->SetMomentum(track->GetInitMom().Mag());
        //     sfgdtrack->fMomVec = track->GetInitMom();

        //     //if (sfgdtrack->GetParentID() == -1) cout << "TrackInfo: [Id,ParentId,PDG,Mom,Range,CosTheta] " << sfgdtrack->GetTrackID() << "," <<  sfgdtrack->GetParentID() << "," <<  sfgdtrack->GetPDG() << "," <<  sfgdtrack->GetMomentum() << "," <<  sfgdtrack->GetRange() << "," <<  sfgdtrack->GetCosTheta() << endl;

        //     all_trajPDG[trjID]    = track->GetPDG();
        //     all_trajID[trjID]     = track->GetTrackID();
        //     all_trajParent[trjID] = track->GetParentID();
        //     listOfTracks.push_back(sfgdtrack);
        // }


        // for(UInt_t i=0; i<listOfTrackID.size(); i++){
        //     //if(trajID[i] == -999) cout << "999-listOfTrackID[i]: " << listOfTrackID[i] << endl;
        //     if( trajPrim[i] == 0 ) trajParent[i] = -1;
        //     else{
        //         trajParent[i] = FindParent(trajID[i],listOfParentID,nd280UpEvent);
        //         trackToParentID[trajID[i]] = trajParent[i]; 
        //         trajPrim[i] = 1; 
        //     }
        //     if (trajParent[i] == -999) trajParent[i] = primID[i];
        //     if (trajParent[i] == -999) {cout << "\n\n\n ERRRROR in parent ID!!! \n\n\n";store = false;}    // deactivate to store all...
        //     //cout << "ID: " << trajID[i] << ", parent: " << trajParent[i] << endl;
        //     trackToParentID[trajID[i]] = trajParent[i]; 
        // }
        // for(uint vxl=0; vxl<listOfVoxels.size(); ++vxl){
        //     listOfVoxels[vxl]->AddTrueParentID(trackToParentID.find(listOfVoxels[vxl]->GetTrueTrackIDs()[0])->second);
        //     listOfVoxels[vxl]->AddTruePDG(trackToPDG.find(listOfVoxels[vxl]->GetTrueTrackIDs()[0])->second);
        //     //if(listOfVoxels[vxl]->GetTrueParentIDs()[0] <-1 ) { if(WriteText) cout << "ERROR IN PARENT ID! [ParentID, Id, PDG] (pdg 22 is gamma): " << listOfVoxels[vxl]->GetTrueParentIDs()[0] << "," << listOfVoxels[vxl]->GetTrueTrackIDs()[0] << "," << listOfVoxels[vxl]->GetTruePDGs()[0] << endl; store=false;}
        //     if(WriteText) cout << "voxel: "<< vxl << "\t, Type: " << listOfVoxels[vxl]->GetTrueType() << "\tXYZ: " << listOfVoxels[vxl]->GetX() << ","  << listOfVoxels[vxl]->GetY() << "," << listOfVoxels[vxl]->GetZ() << ",\ttrueDeposits -[Edep,FiberPE,xy,xz,yz]: " <<  listOfVoxels[vxl]->GetTrueEdep()*0.25 << ",\t" << listOfVoxels[vxl]->GetTruePE()*0.25 <<  ",\t" << listOfVoxels[vxl]->GetHits()[0]->GetPE() << ",\t" << listOfVoxels[vxl]->GetHits()[1]->GetPE() << ",\t" << listOfVoxels[vxl]->GetHits()[2]->GetPE()
        //          << ",\tTrackInfo [ID,prntID,PDG]: " <<  listOfVoxels[vxl]->GetTrueTrackIDs()[0] << "," << listOfVoxels[vxl]->GetTrueParentIDs()[0] << "," << listOfVoxels[vxl]->GetTruePDGs()[0] << endl;
        // }
        // cout << "Voxels voxels w/o clear track ID: " << edep_without_clear_contributor << endl;



        // trajParent[i] = 


        if(TRUE){
            //cout << "X, Y, Z, Edep, TrkID, ParentID, PDG, nuMom" << endl;
            // for (auto vxl:listOfVoxels){
            //     cout    << vxl->GetX() << "," 
            //             << vxl->GetY() << ","
            //             << vxl->GetZ() << ","
            //             << vxl->GetTrueTrackIDs()[0] << ","
            //             << FindParent(vxl->GetTrueTrackIDs()[0],listOfParentID,nd280UpEvent) << ","
            //             << trackToPDG[vxl->GetTrueTrackIDs()[0]] << ","
            //             << nuMom
            //             // << vxl->GetY() << ","
            //             // << vxl->GetY() << ","
            //             // << vxl->GetY() << ","
            //             << endl;
            // }

        //outCSVfile << "evtID,X, Y, Z, Edep, G4Step, Time, TrkID, ParentID, PDG, nuMom" << endl;
            for (auto vxl:listOfVoxels){
                outCSVfile    
                        << ievt        << ","
                        << vxl->GetX() << "," 
                        << vxl->GetY() << ","
                        << vxl->GetZ() << ","
                        << vxl->GetTrueTime() << ","
                        << vxl->GetTrueEdep() << ","
                        << vxl->GetTruePE() << ","    // WARNING!!! THIS IN REALITY CONTAINS G4Step
                        << vxl->GetTrueTrackIDs()[0] << ","
                        << FindParent(vxl->GetTrueTrackIDs()[0],listOfParentID,nd280UpEvent) << ","
                        << trackToPDG[vxl->GetTrueTrackIDs()[0]] << ","
                        << iniMom << ","
                        <<  << "," 
                        << vtxY << "," 
                        << vtxZ << "," 
                        << ReacInfo
                        

                        // << vxl->GetY() << ","
                        // << vxl->GetY() << ","
                        // << vxl->GetY() << ","
                        << endl;
            }
        }

        // int sumEdepPE = 0;
        // int hitIndx = 0;
        // for(uint i=0; i<listOfTrackID.size(); i++){
        //     hitIndx = 0;
        //     sumEdepPE = 0;
        //     for (Int_t trjID = 0; trjID < nd280UpEvent->GetNTracks(); trjID++) {
        //         TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);
        //         if (track->GetTrackID() != listOfTrackID[i]) continue;
        //         trajPDG[i] = track->GetPDG();
        //         trajID[i] = track->GetTrackID();
        //         trajPrim[i] = track->GetParentID();
        //         trackToPDG[track->GetTrackID()] = track->GetPDG();
        //         cout << "STORED || PDG: " <<  track->GetPDG()
        //              << ", ID: " <<    track->GetTrackID() 
        //              << ", prntID: " << track->GetParentID() << endl;
        //     }        
        //     for(int j = 0; j < maxHits; j++){
        //         if(listOfTrackID[i] == hitTraj[j]){
        //             trajHitsNum[i]++;
        //             trajHits[i][hitIndx] = j;
        //             hitIndx++;
        //             sumEdepPE += hitPE[j][0];
        //             sumEdepPE += hitPE[j][1];
        //             sumEdepPE += hitPE[j][2];
        //         }
        //     }
        //     trajEdep[i] = sumEdepPE;
        // }


        // // fill all trajectories information:
        // all_numTracks = nd280UpEvent->GetNTracks();
        // for (Int_t trjID = 0; trjID < nd280UpEvent->GetNTracks(); trjID++) {
        //     TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);
        //     if (trjID >= maxTracks){
        //         store = false;
        //         break;
        //     }
        //     // cout << "trk is: " << track->GetTrackID() << " || GetInitMom " << track->GetInitMom().Mag()
        //     //      << " || trueCosTheta: " << track->GetInitCosTheta()
        //     //      << " || Range: " << track->GetRange() 
        //     //      << endl;

        //     bool found = false;
        //     for(auto trk_ID:listOfTrackID) if (trk_ID == track->GetTrackID()) found = true;
        //     if(!found) continue;

        //     ND280SFGDTrack* sfgdtrack = new ND280SFGDTrack();
        //     sfgdtrack->SetPDG(track->GetPDG());
        //     sfgdtrack->SetTrackID(track->GetTrackID());
        //     sfgdtrack->SetParentID(track->GetParentID());
        //     sfgdtrack->SetCosTheta(track->GetInitCosTheta());
        //     sfgdtrack->SetRange(track->GetRange());
        //     sfgdtrack->SetMomentum(track->GetInitMom().Mag());
        //     sfgdtrack->fMomVec = track->GetInitMom();

        //     //if (sfgdtrack->GetParentID() == -1) cout << "TrackInfo: [Id,ParentId,PDG,Mom,Range,CosTheta] " << sfgdtrack->GetTrackID() << "," <<  sfgdtrack->GetParentID() << "," <<  sfgdtrack->GetPDG() << "," <<  sfgdtrack->GetMomentum() << "," <<  sfgdtrack->GetRange() << "," <<  sfgdtrack->GetCosTheta() << endl;

        //     all_trajPDG[trjID]    = track->GetPDG();
        //     all_trajID[trjID]     = track->GetTrackID();
        //     all_trajParent[trjID] = track->GetParentID();
        //     listOfTracks.push_back(sfgdtrack);
        // }

        // if (WriteText) for (auto lprntID : listOfParentID) cout << "parentID: " << lprntID << endl;

        // for(UInt_t i=0; i<listOfTrackID.size(); i++){
        //     //if(trajID[i] == -999) cout << "999-listOfTrackID[i]: " << listOfTrackID[i] << endl;
        //     if( trajPrim[i] == 0 ) trajParent[i] = -1;
        //     else{
        //         trajParent[i] = FindParent(trajID[i],listOfParentID,nd280UpEvent);
        //         trackToParentID[trajID[i]] = trajParent[i]; 
        //         trajPrim[i] = 1; 
        //     }
        //     if (trajParent[i] == -999) trajParent[i] = primID[i];
        //     if (trajParent[i] == -999) {cout << "\n\n\n ERRRROR in parent ID!!! \n\n\n";store = false;}    // deactivate to store all...
        //     //cout << "ID: " << trajID[i] << ", parent: " << trajParent[i] << endl;
        //     trackToParentID[trajID[i]] = trajParent[i]; 
        // }
        // for(uint vxl=0; vxl<listOfVoxels.size(); ++vxl){
        //     listOfVoxels[vxl]->AddTrueParentID(trackToParentID.find(listOfVoxels[vxl]->GetTrueTrackIDs()[0])->second);
        //     listOfVoxels[vxl]->AddTruePDG(trackToPDG.find(listOfVoxels[vxl]->GetTrueTrackIDs()[0])->second);
        //     //if(listOfVoxels[vxl]->GetTrueParentIDs()[0] <-1 ) { if(WriteText) cout << "ERROR IN PARENT ID! [ParentID, Id, PDG] (pdg 22 is gamma): " << listOfVoxels[vxl]->GetTrueParentIDs()[0] << "," << listOfVoxels[vxl]->GetTrueTrackIDs()[0] << "," << listOfVoxels[vxl]->GetTruePDGs()[0] << endl; store=false;}
        //     if(WriteText) cout << "voxel: "<< vxl << "\t, Type: " << listOfVoxels[vxl]->GetTrueType() << "\tXYZ: " << listOfVoxels[vxl]->GetX() << ","  << listOfVoxels[vxl]->GetY() << "," << listOfVoxels[vxl]->GetZ() << ",\ttrueDeposits -[Edep,FiberPE,xy,xz,yz]: " <<  listOfVoxels[vxl]->GetTrueEdep()*0.25 << ",\t" << listOfVoxels[vxl]->GetTruePE()*0.25 <<  ",\t" << listOfVoxels[vxl]->GetHits()[0]->GetPE() << ",\t" << listOfVoxels[vxl]->GetHits()[1]->GetPE() << ",\t" << listOfVoxels[vxl]->GetHits()[2]->GetPE()
        //          << ",\tTrackInfo [ID,prntID,PDG]: " <<  listOfVoxels[vxl]->GetTrueTrackIDs()[0] << "," << listOfVoxels[vxl]->GetTrueParentIDs()[0] << "," << listOfVoxels[vxl]->GetTruePDGs()[0] << endl;
        // }
        // cout << "Voxels voxels w/o clear track ID: " << edep_without_clear_contributor << endl;

        // numHits = index;

        // if(NHits){ // TODO: store this information in output && comment couts; + avoid entering into ApplyResponse if outside SFGD!
        //     int nVertices = nd280UpEvent->GetNVertices();
        //     cout << " *** Vertices: " << nVertices << endl;
        //     TND280UpVertex* vrtx = nd280UpEvent->GetVertex(0);
        //     // vrtx->PrintVertex();
        //     TND280UpHit *hitSFGD = nd280UpEvent->GetHit(0);
        //     TVector3 vPos(vrtx->GetPosition().x(),vrtx->GetPosition().y()/*+16.2*/,vrtx->GetPosition().z()+1707 );
        //     cout << "ReacMode: " << vrtx->GetReacModeString() << endl;
        //     for(int itrk=0;itrk<vrtx->GetNInTracks();itrk++){
        //         if(vrtx->GetInTrack(itrk)->GetPDG() == 14){
        //             event->SetNuMom(vrtx->GetInTrack(itrk)->GetInitMom().Mag());
        //             cout << "neutrino momentum: " << vrtx->GetInTrack(itrk)->GetInitMom().Mag() << endl;
        //         }
        //     }
        //     //cout << "Original Vtx Position: " << vrtx->GetPosition().x() << "," << vrtx->GetPosition().y() << "," << vrtx->GetPosition().z()<< endl;
        //     cout << "Vertex Position: " << vPos.x() << "," << vPos.y() << "," << vPos.z() << endl;
        //     double minDist = 1000;
        //     double minX    = 1000;
        //     double minY    = 1000;
        //     double minZ    = 1000;
        //     double minXY    = 1000;
        //     double minXZ    = 1000;
        //     double minYZ    = 1000;
        //     double mx = -1;
        //     double my = -1;
        //     double mz = -1;
        //     double XXX=-999;
        //     double YYY=-999;
        //     double ZZZ=-999;
        //     for(int ihit=0;ihit<NHits;ihit++){ // get last entry
        //         TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);
        //         double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
        //         double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2.; // middle step Y
        //         double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2.; // middle step Z
        //         double dist = sqrt(pow(vPos.x()-posX,2)+pow(vPos.y()-posY,2)+pow(vPos.z()-posZ,2));
        //         double distX = abs(vPos.x()-posX);
        //         double distY = abs(vPos.y()-posY);
        //         double distZ = abs(vPos.z()-posZ);
        //         double distXY = sqrt(pow(vPos.x()-posX,2)+pow(vPos.y()-posY,2));
        //         double distXZ = sqrt(pow(vPos.x()-posX,2)+pow(vPos.z()-posZ,2));
        //         double distYZ = sqrt(pow(vPos.y()-posY,2)+pow(vPos.z()-posZ,2));
        //         if(dist<minDist) {minDist = dist; mx = posX; my = posY; mz=posZ;}
        //         if(distX<minX) minX = distX;
        //         if(distY<minY) minY = distY;
        //         if(distZ<minZ) minZ = distZ;
        //         if(distXY<minXY) {minXY = distXY; XXX=posX;}
        //         if(distYZ<minXZ) {minXZ = distXZ; YYY=posY;}
        //         if(distYZ<minYZ) {minYZ = distYZ; ZZZ=posZ;}
        //     }
        //     cout <<"minDist: " << minDist << endl;
        //     cout <<"min [XYZ]       : "<< mx<<","<<my<<","<<mz<<endl;
        //     cout <<"min [XY-XZ-YZ]  : "<< minXY<<","<<minXZ<<","<<minYZ<<endl;
        //     cout <<"XYZ-shift       : "<< XXX-vPos.x() << ","<< YYY-vPos.y() << ","<< ZZZ-vPos.z() << endl;
        //     cout <<"mindif [XYZ]    : " <<minX<<","<<minY<<","<<minZ << endl;
        //     h_shift->Fill(YYY-vPos.y());
            
        //     double tol = 10;
        //     bool outFV = true;
        //     if(vPos.z() > ApplyResponse.GetMPPCPosZ()+tol && vPos.z() < -ApplyResponse.GetMPPCPosZ()-tol) if(vPos.y() > ApplyResponse.GetMPPCPosY()+tol && vPos.y() < -ApplyResponse.GetMPPCPosY()-tol) if(vPos.x() > ApplyResponse.GetMPPCPosX()+tol && vPos.x() < -ApplyResponse.GetMPPCPosX()-tol) outFV = false;
        //     //if(vPos.z() > ApplyResponse.GetMPPCPosZ() && vPos.z() < -ApplyResponse.GetMPPCPosZ()) if(vPos.y() > ApplyResponse.GetMPPCPosY()+tol && vPos.y() < -ApplyResponse.GetMPPCPosY()-tol) if(vPos.x() > ApplyResponse.GetMPPCPosX()+tol && vPos.x() < -ApplyResponse.GetMPPCPosX()-tol) outFV = false;
        //     if(!outFV) cout << "\n\n inside FV!!!" << endl;
        //     else {cout << "\n\n out FV!!!!" << endl;}
        //     cout << "MPPC limits Position: " << ApplyResponse.GetMPPCPosX()/10 << "," << ApplyResponse.GetMPPCPosY()/10 << "," << ApplyResponse.GetMPPCPosZ()/10<< endl;
        //     if(!outFV){
        //         ApplyResponse.CalcResponse(vPos,1,0,1,0,0,0,hitSFGD->GetDetName());
        //         cout << "Vertex Cube Position: " << ApplyResponse.GetHitPos().x()/10-ApplyResponse.GetMPPCPosX()/10 << "," << ApplyResponse.GetHitPos().y()/10-ApplyResponse.GetMPPCPosY()/10 << "," << ApplyResponse.GetHitPos().z()/10-ApplyResponse.GetMPPCPosZ()/10 << endl;
        //         ND280SFGDVoxel* vertexVoxel = new ND280SFGDVoxel(ApplyResponse.GetHitPos().x()/10,ApplyResponse.GetHitPos().y()/10,ApplyResponse.GetHitPos().z()/10);
        //         event->SetTrueVertex(vertexVoxel);
        //     }
        //     if(outFV){
        //         ND280SFGDVoxel* vertexVoxel = new ND280SFGDVoxel(-999,-999,-999);
        //         event->SetTrueVertex(vertexVoxel);
        //     }
        //     #ifdef FV_CUT
        //         store = false;
        //     #endif
        //     if (!outFV){
        //         IN_SFGD++;
        //         if(mu_found) CC_NUMU++;
        //         else NC_NUMU++;
        //     }
        // }

        // if(false){
        //  for (std::map<int,int>::iterator it=trackToParentID.begin(); it!=trackToParentID.end(); ++it)
        //     std::cout << "trackToParentID: " <<it->first << " => " << it->second << '\n';

        //  for (std::map<int,int>::iterator it=trackToPDG.begin(); it!=trackToPDG.end(); ++it)
        //     std::cout << "trackPDG: " <<it->first << " => " << it->second << '\n';
        // }

        // if(store && index){
        //         cout << " --- Event summary --- " << endl;
        //         cout << "The event has: " << index << "hits." << endl;
        //         cout << "The event has: " << numTracks << "tracks." << endl << endl;
        //         cout << " --- summary of tracks --- " << endl;
        //         int locSum = 0;
        //         for (int trks=0; trks<numTracks; trks++){
        //             locSum += trajHitsNum[trks];
        //             cout << "Track " << trks << ", id: " << trajID[trks] << ", #hits: " << trajHitsNum[trks] << ", totPE: " << trajEdep[trks] << ", Prim: " << trajPrim[trks] << ", Parent: " << trajParent[trks] << ", PDG: " << trajPDG[trks] << endl;
        //         }
        //         cout << "nuIniMom: " << event->GetNuMom() << endl;
        //         cout << "vertex   position: " << event->GetTrueVertex()->GetX() << "," << event->GetTrueVertex()->GetY() << "," << event->GetTrueVertex()->GetZ() << endl;
        //     if(WriteText){
        //         cout << "Hits assocaited to tracks: " << locSum << endl;
        //         cout << "Hits without associated track: " << numHits-locSum << endl;
        //         cout << " --- summary of hits --- " << endl;
        //         for (int hts=0; hts<numHits; hts++){
        //             cout << "Hit " << hts << "crosstalk: " << crosstalk[hts] << ", trajID: " << hitTraj[hts] << ", XYZ: " << hitLocation[hts][0] << "," << hitLocation[hts][1] <<"," << hitLocation[hts][2] << ", PE: " << hitPE[hts][0] << "," << hitPE[hts][1] <<"," << hitPE[hts][2] << ", time: " << hitTime[hts][0] << "," << hitTime[hts][1] <<"," << hitTime[hts][2] << endl;
        //         }
        //     }
        // }

        // event->SetVoxels(listOfVoxels);
        // event->SetHits(listOfHits);
        // event->SetTrueTracks(listOfTracks);
        // cout << "event->GetHits().size(): " << event->GetHits().size() << endl;
        // if(!index) store = false;
        // if(store) {cout << "\n\n ----------------> EVENT IS STORED AS OUTPUT\n\n"; AllEvents->Fill(); }
        // else{ cout << "\n\nEVENT IS NOT STORED\n\n";}
        // event->ResetEvent();
        // listOfHits.clear();
        // listOfVoxels.clear();
        // listOfTracks.clear();
    } /// END LOOP OVER EVENTS LOOP

    // cout << endl << endl;
    // cout << "Total Events: " << Nentries << endl;
    // cout << "In SFGD FV:   " << IN_SFGD << endl;
    // cout << "CC-NC:        " << CC_NUMU << "," << NC_NUMU << endl;

    c_shift->Divide(3,2);
    c_shift->cd(1);
    h_shift_X->Draw("HIST");
    c_shift->cd(2);
    h_shift_Y->Draw("HIST");
    c_shift->cd(3);
    h_shift_Z->Draw("HIST");
    c_shift->cd(4);
    h_shift_int_X->Draw("HIST");
    c_shift->cd(5);
    h_shift_int_Y->Draw("HIST");
    c_shift->cd(6);
    h_shift_int_Z->Draw("HIST");

    c_shift->Update();
    c_shift->SaveAs("~/Desktop/shift.pdf");

    // cout << "Writing events. " << endl;
    // fileout->cd();
    // AllEvents->Write();
    // h2d_xy->Write();
    // h2d_xz->Write();
    // h2d_yz->Write();
    // fileout->Close();
    // cout << "End of program. " << endl;

    outCSVfile.close();
    return 1;
}
