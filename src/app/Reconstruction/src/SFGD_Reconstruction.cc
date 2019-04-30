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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int SFGD_Reconstruction(int argc,char** argv) {
    if (argc!=14){   // batch mode
        cout << "You need to provide the following arguments:" << endl;
        cout << " 1) input ROOT file name (from GEANT4 simulation) " << endl;
        cout << " 2) first event number to run" << endl;
        cout << " 3) total number of events to run" << endl;
        cout << " 4) the tag for the output file name" << endl;
        cout << " 5) detector ID: 0-->SuperFGD, 1-->FGD-like, 2-->WAGASCI" << endl;
        cout << " 6) set debug option" << endl;
        cout << " 7) set debug plot option" << endl;
        cout << " 8) use view XY (0 or 1)" << endl;
        cout << " 9) use view XZ (0 or 1)" << endl;
        cout << " 10) use view YZ (0 or 1)" << endl;
        cout << " 11) Minimum track distance (if <=0 use default)" << endl;
        cout << " 12) Look only at tracks inside the Truth FV (use MC hits)" << endl;
        cout << " 13) Output file name. In iteractive run it can be first event, but for parallel need separate param" << endl;
        exit(1);
    }

    Double_t const xTalkProbX = 0.45;
    Double_t const xTalkProbY = 0.45;
    Double_t const xTalkProbXY = 0.2;
    Double_t const xTalkFact = 0.05;
    TRandom3* fRndm = new TRandom3(0);

    TString outfilename = "/nfs/neutrinos/cjesus/work/MC_output.root";
    TFile *fileout = new TFile(outfilename.Data(),"RECREATE");
    TTree *AllEvents = new TTree("AllEvents", "The ROOT tree of events");
    ND280SFGDEvent* event = new ND280SFGDEvent();

    AllEvents->Branch("Event", "Event", event);

    std::string prefix = "_pa";
    
    string rootfilename = argv[1];
    const int evtfirst = atoi(argv[2]);
    const int nevents = atoi(argv[3]);
    string tag = argv[4];
    tag += prefix;
    const int detectorID = atoi(argv[5]);
    const int UseViewXY = atoi(argv[8]);
    const int UseViewXZ = atoi(argv[9]);
    const int UseViewYZ = atoi(argv[10]);
    const int outname = atoi (argv[13]);

    int Nentries = -999;
    
    std::cout << "Run for prototype " << PROTO << std::endl;

    nd280upconv::TargetType_t DetType = nd280upconv::kUndefined;
    if     (detectorID == 0) DetType = nd280upconv::kSuperFGD;
    else   {std::cerr << "Detector must be SFGD!!" << std::endl; exit(1);}

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
    
    TTree *tinput = (TTree*) finput->Get("ND280upEvents");
    
    //
    // Take the MPPC 2D histogram
    //
    
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
    
    TStopwatch sw_event;
    
    vector<int> evt_vec;

    vector <ND280SFGDVoxel*> listOfVoxels;
    vector <ND280SFGDHit*> listOfHit;

    ///////////  Loop over events //////////////////////
    for(int ievt=evtfirst;ievt<=EntryLast;ievt++){
        tinput->GetEntry(ievt);
        evt_vec.push_back(ievt);
        
        int nhits_withsamepos=0;
        
        cout << "************************" << endl;
        cout << " starting event " << ievt << " and has #hits " << nd280UpEvent->GetNHits() << endl;
        
        if(!(ievt%500)){
            cout << "Event " << ievt;
            sw_event.Stop();
            double CPUtime = sw_event.CpuTime();
            sw_event.Reset();
            sw_event.Start(0);
            cout << " --> CPU time = " << CPUtime << endl;
        }
        
        listOfVoxels.reserve(200+nd280UpEvent->GetNHits()); // +200 to add some crosstalk latter...

        float inProtonMom=0.0;
        Int_t NTraj = nd280UpEvent->GetNTracks();

        cout << "# OF TRACKS: " << nd280UpEvent->GetNTracks() << endl;

        int NHits = nd280UpEvent->GetNHits();
        for(int ihit=0;ihit<NHits;ihit++){ // get last entry
            TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);

            //cout << " =======--------- starting hit loop ------=========" << endl;
            // I think we should consider only the hits from Proton by looking at the contributers
            
            double charge = 1.; // Apply alway Birks!
            double time = (nd280UpHit->GetStartT() + nd280UpHit->GetStopT())/2.; // middle step time
            double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
            double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2.; // middle step Y
            double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2.; // middle step Z
            TVector3 lightPos(posX,posY,posZ); // already in local position
            
            double edep = nd280UpHit->GetEnergyDeposit();
            double steplength = nd280UpHit->GetTrackLength(); // check how it's calculated in geant4
            string detname = nd280UpHit->GetDetName();
            
            ApplyResponse.SetTargetID(DetType);
            ApplyResponse.CalcResponse(lightPos,1,0,charge,time,steplength,edep,detname);
            
            double pex = ApplyResponse.GetHitPE().x();
            double pey = ApplyResponse.GetHitPE().y();
            double pez = ApplyResponse.GetHitPE().z();
            
            if(!UseViewXY) pez = 0;
            if(!UseViewYZ) pex = 0;
            if(!UseViewXZ) pey = 0;
            
            double timepex = ApplyResponse.GetHitTime().x();
            double timepey = ApplyResponse.GetHitTime().y();
            double timepez = ApplyResponse.GetHitTime().z();
            
            double poshitX = ApplyResponse.GetHitPos().x();
            double poshitY = ApplyResponse.GetHitPos().y();
            double poshitZ = ApplyResponse.GetHitPos().z();

            ND280SFGDVoxel* auxVoxel = new ND280SFGDVoxel(poshitX/10,poshitY/10,poshitZ/10);

            auxVoxel->SetPDG(nd280UpHit->GetPDG());
            auxVoxel->SetTrackID(nd280UpHit->GetTrackID());
            auxVoxel->SetParentID(nd280UpHit->GetParentID());
            auxVoxel->SetEdep(edep);

            listOfVoxels.push_back(auxVoxel);

            // cout << endl << "-------------------"        << endl;
            // cout << "X-cube:  " << auxVoxel->GetX()      << endl;
            // cout << "Y-cube:  " << auxVoxel->GetY()      << endl;
            // cout << "Z-cube:  " << auxVoxel->GetZ()      << endl;
            // cout << "PDG:     " << track->GetPDG()       << endl;
            // cout << "trackID: " << track->GetTrackID()   << endl;
            // cout << "parntID: " << track->GetParentID()  << endl;
            // cout << "-------------------"                << endl << endl;

            vector <ND280SFGDHit*> auxHitList;
            for(int j = 0; j <3; j++){
                ND280SFGDHit* hit = event->AddHit();
                hit->SetView(j);

                hit->SetX(poshitX/10);
                hit->SetY(poshitY/10);
                hit->SetZ(poshitZ/10);

                if(j == 0) hit->SetZ(-1);
                if(j == 1) hit->SetY(-1);
                if(j == 2) hit->SetX(-1);

                if(j == 0) hit->SetCharge(pez);
                if(j == 1) hit->SetCharge(pey);
                if(j == 2) hit->SetCharge(pex);

                hit->SetTrueXTalk(kFALSE);
                listOfHit.push_back(hit);
                auxHitList.push_back(hit);
            } 

            auxVoxel->SetHits(auxHitList);
            auxHitList.clear();

        } /// END LOOP OVER 3D HITS

        for(UInt_t vx=0; vx<listOfVoxels.size(); vx++){
            Double_t PROB = fRndm->Uniform();
            Int_t    NUM =  6*fRndm->Uniform();
            if(PROB<xTalkProbX){

            Double_t poshitX = 10*listOfVoxels[vx]->GetX();
            Double_t poshitY = 10*listOfVoxels[vx]->GetY();
            Double_t poshitZ = 10*listOfVoxels[vx]->GetZ();

            Double_t newXpos;
            Double_t newYpos;
            Double_t newZpos;

            cout << "NUM: " << NUM << endl;

            if(NUM >5 || NUM <0 ) continue;

            if(NUM == 0){
                newXpos = poshitX/10+1;
                newYpos = poshitY/10;
                newZpos = poshitZ/10;
            } 

            if(NUM == 1){
                newXpos = poshitX/10-1;
                newYpos = poshitY/10;
                newZpos = poshitZ/10;
            } 

            if(NUM == 2){
                newXpos = poshitX/10;
                newYpos = poshitY/10+1;
                newZpos = poshitZ/10;
            } 

            if(NUM == 3){
                newXpos = poshitX/10;
                newYpos = poshitY/10-1;
                newZpos = poshitZ/10;
            } 

            if(NUM == 4){
                newXpos = poshitX/10;
                newYpos = poshitY/10;
                newZpos = poshitZ/10+1;
            } 

            if(NUM == 5){
                newXpos = poshitX/10;
                newYpos = poshitY/10;
                newZpos = poshitZ/10-1;
            } 

            ND280SFGDVoxel* auxVoxel = new ND280SFGDVoxel(newXpos,newYpos,newZpos);

            auxVoxel->SetPDG(listOfVoxels[vx]->GetPDG());
            auxVoxel->SetTrackID(listOfVoxels[vx]->GetTrackID());
            auxVoxel->SetParentID(listOfVoxels[vx]->GetParentID());
            auxVoxel->SetEdep(0.05*fRndm->Gaus(listOfVoxels[vx]->GetEdep(),sqrt(listOfVoxels[vx]->GetEdep())));

            auxVoxel->SetTrueXTalk(kTRUE);

            vector <ND280SFGDHit*> auxHitList;
            for(int j = 0; j <3; j++){
                ND280SFGDHit* hit = event->AddHit();
                hit->SetView(j);

                hit->SetX(newXpos);
                hit->SetY(newYpos);
                hit->SetZ(newZpos);

                if(j == 0) hit->SetZ(-1);
                if(j == 1) hit->SetY(-1);
                if(j == 2) hit->SetX(-1);

                if(j == 0) hit->SetCharge(1);
                if(j == 1) hit->SetCharge(1);
                if(j == 2) hit->SetCharge(1);

                hit->SetTrueXTalk(kTRUE);
                listOfHit.push_back(hit);
                auxHitList.push_back(hit);
            }

            auxVoxel->SetHits(auxHitList);
            listOfVoxels.push_back(auxVoxel);
            auxHitList.clear();

            }
        }

        event->SetVoxels(listOfVoxels);
        event->SetHits(listOfHit);

        AllEvents->Fill();  
        cout << "voxels: " << event->GetVoxels().size() << endl;     
        for (UInt_t ite=0; ite<event->GetVoxels().size(); ite++){
            cout << endl << "-------------------"        << endl;
            cout << "X-cube:  "  << event->GetVoxel(ite)->GetX()      << endl;
            cout << "Y-cube:  "  << event->GetVoxel(ite)->GetY()      << endl;
            cout << "Z-cube:  "  << event->GetVoxel(ite)->GetZ()      << endl;
            cout << "PDG:     "  << event->GetVoxel(ite)->GetPDG()       << endl;
            cout << "trackID: "  << event->GetVoxel(ite)->GetTrackID()   << endl;
            cout << "parentID: " << event->GetVoxel(ite)->GetParentID()   << endl;
            cout << "Edep: "     << event->GetVoxel(ite)->GetEdep()   << endl;
            cout << "-------------------"                << endl << endl;

        }
        event->ResetEvent();
        listOfVoxels.clear();
        listOfHit.clear();
    } /// END LOOP OVER EVENTS LOOP

    fileout->cd();
    AllEvents->Write();
    fileout->Close();
    
    return 1;
}
