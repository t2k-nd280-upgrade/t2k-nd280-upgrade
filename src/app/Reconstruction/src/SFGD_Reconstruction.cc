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

double BirksSaturation(double edep, double steplength)
{
  const double CBIRKS = 0.0208 * CLHEP::cm/CLHEP::MeV; // used in SciBooNE MC
  double dedx = edep/steplength;
  return edep/(1. + CBIRKS*dedx);
}

int ComputeToT(double time, double q)
{
  double analogToT = q*0.43;
  int ToT = int((analogToT+1.25)/2.5);

  return ToT;
}


double FibAtt(double ell) {
    // const double LongCompFrac_FGD = 0.816;
    // const double LongAtt_FGD = 11926.; //*CLHEP::mm;
    // const double ShortAtt_FGD = 312.; //*CLHEP::mm;

    const double LongCompFrac_FGD = 0.77;
    const double LongAtt_FGD = 4634.*CLHEP::mm;
    const double ShortAtt_FGD = 332.*CLHEP::mm;

    return ( LongCompFrac_FGD*exp((-ell)/LongAtt_FGD) + (1-LongCompFrac_FGD)*exp((-ell)/ShortAtt_FGD) );
}

Double_t probCrosstalk(Double_t Q){
    return (1/(1+exp(-(Q-300)/100)))+0.1;
}

// New crosstalk hit probability:
Double_t AboveThreshold(Double_t Q){
    TRandom3 fRndm = TRandom3(0);
    Double_t CT_PE = fRndm.Poisson(Q); //crosstalk #p.e ... change gauss by poisson to account Lor low #p.e
    // if( CT_PE >= 1) return CT_PE;
    // return 0;
    return CT_PE;
}

Double_t CrosstalkPE(Double_t Q){
    TRandom3 fRndm = TRandom3(0);
    Double_t CT_PE = fRndm.Poisson(Q); //crosstalk #p.e ... change gauss by poisson to account Lor low #p.e
    if( CT_PE < 0) return 0;
    // return 0;
    return CT_PE;
}


// ND280SFGDHit* AddCrossTalkHit(Double_t posA, Double_t posB, Double_t Q, Int_t view){
//     TRandom3* fRndm = new TRandom3(0);
//     ND280SFGDHit* crosstalkHit = new ND280SFGDHit();
//     if(view == 0){
//         crosstalkHit->SetX(posA);
//         crosstalkHit->SetY(posB);
//         crosstalkHit->SetZ(-999);
//         crosstalkHit->SetView(0);
//         Double_t hitQ = 0.05*fRndm->Gaus(Q,sqrt(Q));
//         hitQ = hitQ > 0 ? hitQ : 0;
//         crosstalkHit->SetCharge(hitQ);
//         crosstalkHit->SetxTalkFlag(kTRUE);
//     }
//     if(view == 1){
//         crosstalkHit->SetX(posA);
//         crosstalkHit->SetY(-999);
//         crosstalkHit->SetZ(posB);
//         crosstalkHit->SetView(1);
//         Double_t hitQ = 0.05*fRndm->Gaus(Q,sqrt(Q));
//         hitQ = hitQ > 0 ? hitQ : 0;
//         crosstalkHit->SetCharge(hitQ);
//         crosstalkHit->SetxTalkFlag(kTRUE);
//     }
//     if(view == 2){
//         crosstalkHit->SetX(-999);
//         crosstalkHit->SetY(posA);
//         crosstalkHit->SetZ(posB);
//         crosstalkHit->SetView(2);
//         Double_t hitQ = 0.05*fRndm->Gaus(Q,sqrt(Q));
//         hitQ = hitQ > 0 ? hitQ : 0;
//         crosstalkHit->SetCharge(hitQ);
//         crosstalkHit->SetxTalkFlag(kTRUE);
//     }
//     return crosstalkHit;
// }

ND280SFGDHit*AddCrossTalkHit(Double_t posA, Double_t posB, Double_t Q, Int_t view){
    TRandom3* fRndm = new TRandom3(0);
    ND280SFGDHit* crosstalkHit = new ND280SFGDHit();
    if(view == 0){
        crosstalkHit->SetX(posA);
        crosstalkHit->SetY(posB);
        crosstalkHit->SetZ(-999);
        crosstalkHit->SetView(0);
        crosstalkHit->SetCharge(Q);
        crosstalkHit->SetxTalkFlag(kTRUE);
    }
    if(view == 1){
        crosstalkHit->SetX(posA);
        crosstalkHit->SetY(-999);
        crosstalkHit->SetZ(posB);
        crosstalkHit->SetView(1);
        crosstalkHit->SetCharge(Q);
        crosstalkHit->SetxTalkFlag(kTRUE);
    }
    if(view == 2){
        crosstalkHit->SetX(-999);
        crosstalkHit->SetY(posA);
        crosstalkHit->SetZ(posB);
        crosstalkHit->SetView(2);
        crosstalkHit->SetCharge(Q);
        crosstalkHit->SetxTalkFlag(kTRUE);
    }
    return crosstalkHit;
}


// bool NoHitFoundInNewPos(Double_t posA, Double_t posB, Int_t view, vector <ND280SFGDVoxel*> listOfVoxels){

//         if(view == 0){        
//             for(UInt_t v=0; v<listOfVoxels.size(); v++){
//                 if (listOfVoxels[v]->GetHits()[0]->GetX() == posA && listOfVoxels[v]->GetHits()[0]->GetY() == posB)  return kFALSE;
//             }
//         }

//         if(view == 1){        
//             for(UInt_t v=0; v<listOfVoxels.size(); v++){
//                 if (listOfVoxels[v]->GetHits()[1]->GetX() == posA && listOfVoxels[v]->GetHits()[1]->GetZ() == posB)  return kFALSE;
//             }
//         }

//         if(view == 2){        
//             for(UInt_t v=0; v<listOfVoxels.size(); v++){
//                 if (listOfVoxels[v]->GetHits()[2]->GetY() == posA && listOfVoxels[v]->GetHits()[2]->GetZ() == posB)  return kFALSE;
//             }
//         }

//     return kTRUE;
// }

bool NoHitFoundInNewPos(Double_t posA, Double_t posB, Int_t view, vector <ND280SFGDHit*> listOfHit){

        if(view == 0){        
            for(UInt_t i=0; i<listOfHit.size(); i++){
                if (listOfHit[i]->GetX() == posA && listOfHit[i]->GetY() == posB)  return kFALSE;
            }
        }

        if(view == 1){        
            for(UInt_t i=0; i<listOfHit.size(); i++){
                if (listOfHit[i]->GetX() == posA && listOfHit[i]->GetZ() == posB)  return kFALSE;
            }
        }

        if(view == 2){        
            for(UInt_t i=0; i<listOfHit.size(); i++){
                if (listOfHit[i]->GetY() == posA && listOfHit[i]->GetZ() == posB)  return kFALSE;
            }
        }

    return kTRUE;
}


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

    Double_t const f_lateral_CT  = 0.01;  // attenuation of charge in the lateral fiber 1u dist
    Double_t const f_lateral2_CT = f_lateral_CT*f_lateral_CT;  // attenuation of charge in the lateral fiber 1u dist
    Double_t const f_diagonal_CT = 0.45; // attenuation of charge in the diagonal fiber 1.5u dist

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


    TH1D* h_chargeTrue        = new TH1D("h_chargeTrue","h_chargeTrue",100,0,300);
    TH1D* h_chargeReco        = new TH1D("h_chargeReco","h_chargeReco",100,0,300);

    TH1D* h_chargeRes       = new TH1D("h_chargeRes","h_chargeRes",300,-30,30);

    h_chargeTrue->SetLineColor(kRed);

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

    Double_t C_true_Tot = 0;
    Double_t C_reco_Tot = 0;

    ///////////  Loop over events //////////////////////
    for(int ievt=evtfirst;ievt<=EntryLast;ievt++){
        tinput->GetEntry(ievt);
        evt_vec.push_back(ievt);

        C_true_Tot = 0;
        C_reco_Tot = 0;
        
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
        
        vector <ND280SFGDTrack*> listOfTracks;

        float inProtonMom=0.0;
        Int_t NTraj = nd280UpEvent->GetNTracks();

        int NHits = nd280UpEvent->GetNHits();
        for(int ihit=0;ihit<NHits;ihit++){ // get last entry
            TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);
            
            double charge = 1.;
            double time = (nd280UpHit->GetStartT() + nd280UpHit->GetStopT())/2.; // middle step time
            double posX = (nd280UpHit->GetStartX() + nd280UpHit->GetStopX())/2.; // middle step X
            double posY = (nd280UpHit->GetStartY() + nd280UpHit->GetStopY())/2.; // middle step Y
            double posZ = (nd280UpHit->GetStartZ() + nd280UpHit->GetStopZ())/2.; // middle step Z
            TVector3 lightPos(posX,posY,posZ);
            
            double edep = nd280UpHit->GetEnergyDeposit();
            double steplength = nd280UpHit->GetTrackLength();
            string detname = nd280UpHit->GetDetName();
            
            ApplyResponse.SetTargetID(DetType);
            ApplyResponse.CalcResponse(lightPos,1,0,charge,time,steplength,edep,detname);

            
            double pex = ApplyResponse.GetHitPE().x();
            double pey = ApplyResponse.GetHitPE().y();
            double pez = ApplyResponse.GetHitPE().z();

            double sat_edep = BirksSaturation(nd280UpHit->GetEnergyDeposit(),steplength);

            // cout << "True saturated Edep in #p.e: " << sat_edep << endl;
            // cout << " ... raw ... " << endl;
            // cout << "XY -- #p.e: " << pez << endl;
            // cout << "XZ -- #p.e: " << pey << endl;
            // cout << "YZ -- #p.e: " << pex  << endl;
            // cout << " ... corrected ... " << endl;
            // cout << "POS: " << posX << "," << posY << "," << posZ << endl;
            // cout << "fact_reco: " << FibAtt(posX+10*102) << endl;
            // cout << "fact_reco: " << FibAtt(posY+10*26) << endl;
            // cout << "fact_reco: " << FibAtt(posZ+10*94) << endl;
            double c_PE_xy = (1/0.35)*pez / FibAtt(posZ+10*94);
            double c_PE_xz = (1/0.35)*pey / FibAtt(posY+10*26);
            double c_PE_yz = (1/0.35)*pex / FibAtt(posX+10*102);
            // cout << "XY -- #p.e: " << c_PE_xy << endl;
            // cout << "XZ -- #p.e: " << c_PE_xz<< endl;
            // cout << "YZ -- #p.e: " << c_PE_yz << endl;
            // cout << "Expected Reco Edep in #p.e: " << (c_PE_xy+c_PE_xz+c_PE_yz)/3 << endl;

            // h_chargeRes->Fill( (sat_edep*156.42 - (c_PE_xy+c_PE_xz+c_PE_yz)/3 ) / ( sat_edep*156.42 ) );

            // h_chargeTrue->Fill(sat_edep*156.42);
            // h_chargeReco->Fill((c_PE_xy+c_PE_xz+c_PE_yz)/3 );

            C_true_Tot += sat_edep*156.42;
            C_reco_Tot += (c_PE_xy+c_PE_xz+c_PE_yz)/3 ;


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
            auxVoxel->SetTrackID(nd280UpHit->GetPrimaryId());
            auxVoxel->SetParentID(0);
            auxVoxel->SetEdep((c_PE_xy+c_PE_xz+c_PE_yz)/3);  /// FROM THE TIME BEING USS SATURATED VALUE AS TRUE!

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

                hit->SetTrackID(auxVoxel->GetTrackID());
                hit->SetxTalkFlag(kFALSE);
                auxHitList.push_back(hit);
            } 

            auxVoxel->SetHits(auxHitList);
            auxHitList.clear();

        } /// END LOOP OVER 3D HITS

        // ADD ALL DEPOSITS IN THE SAME XYZ
        vector <ND280SFGDVoxel*> newVoxelList;   
        vector <Int_t> analyzedVoxels(listOfVoxels.size(),0);
        for(UInt_t ivox=0; ivox<listOfVoxels.size(); ivox++){
            if(analyzedVoxels[ivox]) continue;
            analyzedVoxels[ivox] = 1;
            for(UInt_t jvox=0; jvox<listOfVoxels.size(); jvox++){
                if(analyzedVoxels[jvox]) continue;
                else if(listOfVoxels[ivox]->GetX() == listOfVoxels[jvox]->GetX() && listOfVoxels[ivox]->GetY() == listOfVoxels[jvox]->GetY() && listOfVoxels[ivox]->GetZ() == listOfVoxels[jvox]->GetZ()){
                    if(listOfVoxels[ivox]->GetTrackID() == listOfVoxels[jvox]->GetTrackID()){                    
                        analyzedVoxels[jvox] = 1;
                        listOfVoxels[ivox]->SetEdep( listOfVoxels[ivox]->GetEdep() + listOfVoxels[jvox]->GetEdep() );
                        listOfVoxels[ivox]->GetHits()[0]->SetCharge( listOfVoxels[ivox]->GetHits()[0]->GetCharge() + listOfVoxels[jvox]->GetHits()[0]->GetCharge() );
                        listOfVoxels[ivox]->GetHits()[1]->SetCharge( listOfVoxels[ivox]->GetHits()[1]->GetCharge() + listOfVoxels[jvox]->GetHits()[1]->GetCharge() );
                        listOfVoxels[ivox]->GetHits()[2]->SetCharge( listOfVoxels[ivox]->GetHits()[2]->GetCharge() + listOfVoxels[jvox]->GetHits()[2]->GetCharge() );
                    }
                    else if(listOfVoxels[ivox]->GetTrackID() != listOfVoxels[jvox]->GetTrackID()){
                        analyzedVoxels[jvox] = 1;
                        listOfVoxels[ivox]->SetEdep( listOfVoxels[ivox]->GetEdep() + listOfVoxels[jvox]->GetEdep() );
                        listOfVoxels[ivox]->GetHits()[0]->SetCharge( listOfVoxels[ivox]->GetHits()[0]->GetCharge() + listOfVoxels[jvox]->GetHits()[0]->GetCharge() );
                        listOfVoxels[ivox]->GetHits()[1]->SetCharge( listOfVoxels[ivox]->GetHits()[1]->GetCharge() + listOfVoxels[jvox]->GetHits()[1]->GetCharge() );
                        listOfVoxels[ivox]->GetHits()[2]->SetCharge( listOfVoxels[ivox]->GetHits()[2]->GetCharge() + listOfVoxels[jvox]->GetHits()[2]->GetCharge() );
                        if(!listOfVoxels[ivox]->GetContributors().size()) listOfVoxels[ivox]->AddContributor(listOfVoxels[ivox]->GetTrackID());    // only if it is the first time, add also the ivox contributor, this avoids ivox double counting.
                        listOfVoxels[ivox]->AddContributor(listOfVoxels[jvox]->GetTrackID());
                    }
                }
                if(!listOfVoxels[ivox]->GetContributors().size()) listOfVoxels[ivox]->AddContributor(listOfVoxels[ivox]->GetTrackID());
            }
            newVoxelList.push_back(listOfVoxels[ivox]);
        }

        // UPDATE VOXEL LIST
        listOfVoxels.clear();
        listOfVoxels = newVoxelList;
        newVoxelList.clear();

        for(UInt_t ivox=0; ivox<listOfVoxels.size(); ivox++){
            // cout << "True Edep in #p.e: " << listOfVoxels[ivox]->GetEdep()*156.42 << endl;
            // cout << " ... raw ... " << endl;
            // cout << "XY -- #p.e: " << listOfVoxels[ivox]->GetHits()[0]->GetCharge() << endl;
            // cout << "XZ -- #p.e: " << listOfVoxels[ivox]->GetHits()[1]->GetCharge()  << endl;
            // cout << "YZ -- #p.e: " << listOfVoxels[ivox]->GetHits()[2]->GetCharge()  << endl;
            // cout << " ... corrected ... " << endl;
            double c_PE_xy = (1/0.35)*listOfVoxels[ivox]->GetHits()[0]->GetCharge() / FibAtt(10*(listOfVoxels[ivox]->GetZ()+94));
            double c_PE_xz = (1/0.35)*listOfVoxels[ivox]->GetHits()[1]->GetCharge() / FibAtt(10*(listOfVoxels[ivox]->GetY()+26));
            double c_PE_yz = (1/0.35)*listOfVoxels[ivox]->GetHits()[2]->GetCharge() / FibAtt(10*(listOfVoxels[ivox]->GetX()+102));

            vector <Double_t> auxLocalHits;

            auxLocalHits.push_back(c_PE_xy);
            auxLocalHits.push_back(c_PE_xz);
            auxLocalHits.push_back(c_PE_yz);

            listOfVoxels[ivox]->SetLocalHitsQ(auxLocalHits);

            // cout << "XY -- #p.e: " << c_PE_xy << endl;
            // cout << "XZ -- #p.e: " << c_PE_xz<< endl;
            // cout << "YZ -- #p.e: " << c_PE_yz << endl;
            // cout << "Expected Reco Edep in #p.e: " << (c_PE_xy+c_PE_xz+c_PE_yz)/3 << endl;

            h_chargeRes->Fill( 100*(listOfVoxels[ivox]->GetEdep() - (c_PE_xy+c_PE_xz+c_PE_yz)/3 ) / ( listOfVoxels[ivox]->GetEdep()*156.42 ) );

            h_chargeTrue->Fill(listOfVoxels[ivox]->GetEdep());
            h_chargeReco->Fill((c_PE_xy+c_PE_xz+c_PE_yz)/3 );

            C_true_Tot += listOfVoxels[ivox]->GetEdep();
            C_reco_Tot += (c_PE_xy+c_PE_xz+c_PE_yz)/3 ;
        }

        cout << "# of core-voxles: " << listOfVoxels.size() << endl;

        // MAKE A HIT LIST
        for(UInt_t vx=0; vx<listOfVoxels.size(); vx++){
            listOfHit.push_back(listOfVoxels[vx]->GetHits()[0]);
            listOfHit.push_back(listOfVoxels[vx]->GetHits()[1]);
            listOfHit.push_back(listOfVoxels[vx]->GetHits()[2]);
        }

        // MERGE HITS --- Latter on add time conditions ... !!!
        cout << "# of hits before merging: " << listOfHit.size() << endl;
        vector <ND280SFGDHit*> newListOfHits;
        vector <Int_t> analyzedHits(listOfHit.size(),0);
        for(UInt_t ahit=0; ahit<listOfHit.size(); ahit++){
            listOfHit[ahit]->SetxTalkFlag(0);
            if(analyzedHits[ahit]) continue;
            newListOfHits.push_back(listOfHit[ahit]);
            analyzedHits[ahit]=1;
            for(UInt_t bhit=0; bhit<listOfHit.size(); bhit++){
                if(analyzedHits[bhit]) continue;
                if(listOfHit[ahit]->GetView() == listOfHit[bhit]->GetView() && listOfHit[ahit]->GetX() == listOfHit[bhit]->GetX() && listOfHit[ahit]->GetY() == listOfHit[bhit]->GetY() && listOfHit[ahit]->GetZ() == listOfHit[bhit]->GetZ() ){
                    analyzedHits[bhit] = 1;
                    listOfHit[ahit]->SetCharge( listOfHit[ahit]->GetCharge() + listOfHit[bhit]->GetCharge() );
                }
            }
        }
        listOfHit = newListOfHits;
        cout << "hits after merging: " << listOfHit.size() << endl;
        newListOfHits.clear();

        // ASSIGN MERGED HITS TO VOXELS
        // ONLY KEEP VOXELS WITH HITS WITH Q > 0.
        for(UInt_t ivox=0; ivox<listOfVoxels.size(); ivox++){
            vector <ND280SFGDHit*> auxHit;
            auxHit.resize(3);
            for(UInt_t ahit=0; ahit<listOfHit.size(); ahit++){
                if(listOfHit[ahit]->GetView() == 0 && listOfVoxels[ivox]->GetX() == listOfHit[ahit]->GetX() && listOfVoxels[ivox]->GetY() == listOfHit[ahit]->GetY()){
                    auxHit[0] = listOfHit[ahit];
                } 
                if(listOfHit[ahit]->GetView() == 1 && listOfVoxels[ivox]->GetX() == listOfHit[ahit]->GetX() && listOfVoxels[ivox]->GetZ() == listOfHit[ahit]->GetZ()){
                    auxHit[1] = listOfHit[ahit];
                } 
                if(listOfHit[ahit]->GetView() == 2 && listOfVoxels[ivox]->GetY() == listOfHit[ahit]->GetY() && listOfVoxels[ivox]->GetZ() == listOfHit[ahit]->GetZ()){
                    auxHit[2] = listOfHit[ahit];
                }
            }
            if(auxHit[0] == nullptr || auxHit[1] == nullptr || auxHit[2] == nullptr ){cerr << "check hit filling associated to voxels!" << endl; exit(1);}
            listOfVoxels[ivox]->SetHits(auxHit);
            if(auxHit[0]->GetCharge() > 0 && auxHit[1]->GetCharge() > 0  && auxHit[2]->GetCharge() > 0 ){
                 newVoxelList.push_back(listOfVoxels[ivox]);
                 newListOfHits.push_back(auxHit[0]);
                 newListOfHits.push_back(auxHit[1]);
                 newListOfHits.push_back(auxHit[2]);
            }
        }

        if(newListOfHits.size()){
            std::sort   (newListOfHits.begin(), newListOfHits.end());
            newListOfHits.erase(std::unique (newListOfHits.begin(), newListOfHits.end()), newListOfHits.end()); 
        }

        cout << "# of unique non-zero hits: " << newListOfHits.size() << endl;

        //ONLY KEEP VOXELS AND HITS THAT ARE DIFFERENT FROM 0
        // for(UInt_t vx=0; vx<listOfVoxels.size(); vx++){
        //     if(listOfVoxels[vx]->GetHits()[0]->GetCharge() == 0 || listOfVoxels[vx]->GetHits()[1]->GetCharge() == 0 || listOfVoxels[vx]->GetHits()[2]->GetCharge() == 0) {cout << "skipping voxel" << endl; continue;}  // for the time being, store only voxels with 3 hits with #p.e > 0
        //     newVoxelList.push_back(listOfVoxels[vx]);
        // }
        listOfVoxels.clear();
        listOfVoxels = newVoxelList;
        newVoxelList.clear();

        listOfHit.clear();
        listOfHit = newListOfHits;
        newListOfHits.clear();

        // ADD CROSSTALK HITS:
        for(UInt_t hidx=0; hidx<listOfHit.size(); hidx++){

            Double_t posX = listOfHit[hidx]->GetX();
            Double_t posY = listOfHit[hidx]->GetY();
            Double_t posZ = listOfHit[hidx]->GetZ();

            Double_t pex = f_lateral_CT*listOfHit[hidx]->GetCharge();
            Double_t pey = f_lateral_CT*listOfHit[hidx]->GetCharge();
            Double_t pez = f_lateral_CT*listOfHit[hidx]->GetCharge();

            Double_t CT_Q = 0;

            // add conditions to avoid hits out of plane!

            if(listOfHit[hidx]->GetView() == 0){
                CT_Q = CrosstalkPE(pez);
                    if ( NoHitFoundInNewPos(posX+1,posY,0,listOfHit) )
                        newListOfHits.push_back(AddCrossTalkHit(posX+1,posY,CT_Q,0));

                CT_Q = CrosstalkPE(pez);
                    if ( NoHitFoundInNewPos(posX-1,posY,0,listOfHit) )
                        newListOfHits.push_back(AddCrossTalkHit(posX-1,posY,CT_Q,0));

                CT_Q = CrosstalkPE(pez);
                    if ( NoHitFoundInNewPos(posX,posY+1,0,listOfHit) )
                        newListOfHits.push_back(AddCrossTalkHit(posX,posY+1,CT_Q,0));

                CT_Q = CrosstalkPE(pez);
                    if ( NoHitFoundInNewPos(posX,posY-1,0,listOfHit) )
                        newListOfHits.push_back(AddCrossTalkHit(posX,posY-1,CT_Q,0));
            }
        
            if(listOfHit[hidx]->GetView() == 1){
                CT_Q = CrosstalkPE(pey);
                    if ( NoHitFoundInNewPos(posX+1,posZ,0,listOfHit) )
                        newListOfHits.push_back(AddCrossTalkHit(posX+1,posZ,CT_Q,1));

                CT_Q = CrosstalkPE(pey);
                    if ( NoHitFoundInNewPos(posX-1,posZ,0,listOfHit) )
                        newListOfHits.push_back(AddCrossTalkHit(posX-1,posZ,CT_Q,1));

                CT_Q = CrosstalkPE(pey);
                    if ( NoHitFoundInNewPos(posX,posZ+1,0,listOfHit) )
                        newListOfHits.push_back(AddCrossTalkHit(posX,posZ+1,CT_Q,1));

                CT_Q = CrosstalkPE(pey);
                    if ( NoHitFoundInNewPos(posX,posZ-1,0,listOfHit) )
                        newListOfHits.push_back(AddCrossTalkHit(posX,posZ-1,CT_Q,1));
            }

            if(listOfHit[hidx]->GetView() == 2){
                CT_Q = CrosstalkPE(pex);
                    if ( NoHitFoundInNewPos(posY+1,posZ,0,listOfHit) )
                        newListOfHits.push_back(AddCrossTalkHit(posY+1,posZ,CT_Q,2));

                CT_Q = CrosstalkPE(pex);
                    if ( NoHitFoundInNewPos(posY-1,posZ,0,listOfHit) )
                        newListOfHits.push_back(AddCrossTalkHit(posY-1,posZ,CT_Q,2));

                CT_Q = CrosstalkPE(pex);
                    if ( NoHitFoundInNewPos(posY,posZ+1,0,listOfHit) )
                        newListOfHits.push_back(AddCrossTalkHit(posY,posZ+1,CT_Q,2));

                CT_Q = CrosstalkPE(pex);
                    if ( NoHitFoundInNewPos(posY,posZ-1,0,listOfHit) )
                        newListOfHits.push_back(AddCrossTalkHit(posY,posZ-1,CT_Q,2));
            }

        }

        cout << "# of unmerged crosstalk hits: " << newListOfHits.size() << endl;

        vector <ND280SFGDHit*> crosstalkHits;
        analyzedHits.clear();
        analyzedHits.resize(newListOfHits.size(),0);
        for(UInt_t ahit=0; ahit<newListOfHits.size(); ahit++){
            if(analyzedHits[ahit]) continue;
            crosstalkHits.push_back(newListOfHits[ahit]);
            analyzedHits[ahit]=1;
            for(UInt_t bhit=0; bhit<newListOfHits.size(); bhit++){
                if(analyzedHits[bhit]) continue;
                if(newListOfHits[ahit]->GetView() == newListOfHits[bhit]->GetView() && newListOfHits[ahit]->GetX() == newListOfHits[bhit]->GetX() && newListOfHits[ahit]->GetY() == newListOfHits[bhit]->GetY() && newListOfHits[ahit]->GetZ() == newListOfHits[bhit]->GetZ() ){
                    analyzedHits[bhit] = 1;
                    newListOfHits[ahit]->SetCharge( newListOfHits[ahit]->GetCharge() + newListOfHits[bhit]->GetCharge() );
                }
            }
        }

        cout << "# of merged crosstalk hits: " << crosstalkHits.size() << endl;

        for(UInt_t ahit=0; ahit<crosstalkHits.size(); ahit++){
            if(crosstalkHits[ahit]->GetCharge() >= 1) listOfHit.push_back(crosstalkHits[ahit]);
        }
        
        cout << "# of hits including crosstalk: " << listOfHit.size() << endl;

        cout << "total # of voxels: " << listOfVoxels.size() << endl;   
        if(!listOfVoxels.size()) continue;     

        event->SetVoxels(listOfVoxels);
        event->SetHits(listOfHit);

        std::vector <ND280SFGDHit*> totalListOfHits;
        std::vector <Int_t> listOfTrackID;

        listOfTrackID.reserve(listOfVoxels.size());

        for(UInt_t i=0; i<listOfVoxels.size(); i++){
            for(size_t icont = 0; icont < listOfVoxels[i]->GetContributors().size(); icont++){
                listOfTrackID.push_back(listOfVoxels[i]->GetContributors()[icont]);
            }
        }

        //keep only unique track IDs:
        if(listOfTrackID.size()){
            std::sort   (listOfTrackID.begin(), listOfTrackID.end());
            for(size_t itrkID = 0; itrkID < listOfTrackID.size(); itrkID++){
            }
            listOfTrackID.erase(std::unique (listOfTrackID.begin(), listOfTrackID.end()), listOfTrackID.end()); 
            //group hits and voxels with the same track ID:
            event->GetTracks().reserve(listOfTrackID.size());
        }

        ////// ADD TRACK INFO
        std::vector <ND280SFGDVoxel*> listOfTrackVoxels;
        std::vector <ND280SFGDHit*> listOfTrackHits;
        Int_t trackPDG = -999;
        for(UInt_t i=0; i<listOfTrackID.size(); i++){
            trackPDG = -999;
            for(UInt_t j=0; j<listOfVoxels.size(); j++){
                for(size_t icont = 0; icont < listOfVoxels[j]->GetContributors().size(); icont++){
                    if(listOfVoxels[j]->GetContributors().size() == 1) trackPDG = listOfVoxels[j]->GetPDG();
                    if(listOfVoxels[j]->GetContributors()[icont] == listOfTrackID[i]){
                        trackPDG = listOfVoxels[j]->GetPDG();
                        listOfTrackVoxels.push_back(listOfVoxels[j]);
                        listOfTrackHits.push_back(listOfVoxels[j]->GetHits()[0]);
                        listOfTrackHits.push_back(listOfVoxels[j]->GetHits()[1]);
                        listOfTrackHits.push_back(listOfVoxels[j]->GetHits()[2]);
                        totalListOfHits.push_back(listOfVoxels[j]->GetHits()[0]);
                        totalListOfHits.push_back(listOfVoxels[j]->GetHits()[1]);
                        totalListOfHits.push_back(listOfVoxels[j]->GetHits()[2]);
                    }
                }
            }
            ND280SFGDTrack* auxTrack = new ND280SFGDTrack(listOfTrackVoxels);
            auxTrack->SetHits(listOfTrackHits);
            auxTrack->SetPDG(trackPDG);
            auxTrack->SetTrackID(listOfTrackID[i]);
            listOfTracks.push_back(auxTrack);
            listOfTrackHits.clear();
            listOfTrackVoxels.clear();
        }
        ////////

        event->SetTracks(listOfTracks);

        cout << "Real # Tracks in the event:    " << nd280UpEvent->GetNTracks() << endl;
        cout << "Stroed # Tracks in the event:  " << event->GetTracks().size() << endl;
        cout << "Stroed # Tracks in the vector: " << listOfTracks.size() << endl;
        cout << "Total True Q: " << C_true_Tot << endl;
        cout << "Total Reco Q: " << C_reco_Tot << endl;

        cout << endl << endl << endl << endl;

        if(listOfTracks.size()) AllEvents->Fill();  
        // cout << "voxels: " << event->GetVoxels().size() << endl;     
        // for (UInt_t ite=0; ite<event->GetVoxels().size(); ite++){
        //     cout << endl << "-------------------"        << endl;
        //     cout << "X-cube:  "  << event->GetVoxel(ite)->GetX()      << endl;
        //     cout << "Y-cube:  "  << event->GetVoxel(ite)->GetY()      << endl;
        //     cout << "Z-cube:  "  << event->GetVoxel(ite)->GetZ()      << endl;
        //     cout << "PDG:     "  << event->GetVoxel(ite)->GetPDG()       << endl;
        //     cout << "trackID: "  << event->GetVoxel(ite)->GetTrackID()   << endl;
        //     cout << "parentID: " << event->GetVoxel(ite)->GetParentID()   << endl;
        //     cout << "Edep: "     << event->GetVoxel(ite)->GetEdep()   << endl;
        //     cout << "-------------------"                << endl << endl;
        // }
        event->ResetEvent();
        listOfTracks.clear();
        listOfVoxels.clear();
        listOfHit.clear();
    } /// END LOOP OVER EVENTS LOOP

    fileout->cd();
    AllEvents->Write();
    h_chargeTrue->Write();
    h_chargeReco->Write();
    h_chargeRes->Write();
    fileout->Close();

    return 1;
}
