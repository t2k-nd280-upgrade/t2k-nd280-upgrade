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
#include "Hit.hh"
#include "Event.hh"

#include "PrototypeMC.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int PrototypeMC(int argc,char** argv) {
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
    Double_t const xTalkProbY = 0.04;
    Double_t const xTalkProbXY = 0.2;
    Double_t const xTalkFact = 0.05;
    TRandom3* fRndm = new TRandom3(0);

    // TF1* sigmoid = new TF1("func", "(TMath::Exp(x/[0]))/(1+ TMath::Exp(x/[0]))-0.5", 0, 500);
    // sigmoid->SetParameter(0,350);

    // Set outputs
    
    //TString outfilename = TString::Format("%s_Evt%d_NEvt%d.root",tag.c_str(),outname,nevents);
    TString outfilename = "/nfs/neutrinos/cjesus/work/MC_output.root";
    TFile *fileout = new TFile(outfilename.Data(),"RECREATE");

    TTree *AllEvents = new TTree("AllEvents", "The ROOT tree of events");
    Event* event = new Event();
    AllEvents->Branch("Event", "Event", event);

    std::cout << "Crosstalk algorithm is " << CROSSTALK << std::endl;
    
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
    // Set the inputs
    nd280upconv::TargetType_t DetType = nd280upconv::kUndefined;
    if     (detectorID == 0) DetType = nd280upconv::kSuperFGD;
    else   {std::cerr << "Detector must be SFGD!!" << std::endl; exit(1);}

    if(detectorID != 0) {cout << "It only works with SFGD input!" << endl; exit(0);}
    
    const int NEvtDisplTot = 100; //atoi(argv[5]); // max # of evt. displays
    
    
    /////////////////////////////
    //                         //
    // Apply detector response //
    //                         //
    /////////////////////////////
    
    // Take inputs
    TFile *finput = new TFile(rootfilename.c_str(),"READ");
    cout << "Reading the input ROOT file: " << rootfilename << endl;

    TH1F* hQxy = new TH1F("hQxy","hQxy",100,0,300);
    TH1F* hQxz = new TH1F("hQxz","hQxz",100,0,300);
    TH1F* hQyz = new TH1F("hQyz","hQyz",100,0,300);
    
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
    
    TH1F* hProtonTrueEMeV=new TH1F("ProtonTrueEMeV", "ProtonTrueEMeV", 500,0,2000);
    TH1F* hNhitsEvt=new TH1F("NHitsEvt", "NHitsEvt", 100,0,100);
    TH1F* hHitEnergyMeV=new TH1F("HitEnergyMeV", "HitsEnergyMeV", 50,0,20);
    TH1F* hHitEnergy_peX=new TH1F("HitEnergy_peX", "HitEnergy_peX", 1000,0,1000);
    TH1F* hHitEnergy_peY=new TH1F("HitEnergy_peY", "HitEnergy_peY", 1000,0,1000);
    TH1F* hHitEnergy_peZ=new TH1F("HitEnergy_peZ", "HitEnergy_peZ", 1000,0,1000);
    TH2F* hdEdZvsZ=new TH2F("dEdZvsZ","dEdZvsZ",48,-240,240,1000,0,1000);
    TH2F* hdEdZvsZ_X=new TH2F("dEdZvsZ_X","dEdZvsZ_X",48,-240,240,1000,0,1000);
    TH2F* hdEdZvsZ_Y=new TH2F("dEdZvsZ_Y","dEdZvsZ_Y",48,-240,240,1000,0,1000);
    
    TH2F* hEnergyvsZ_X=new TH2F("hEnergyvsZ_X","hEnergyvsZ_X",48,-240,240,500,0,500);
    TH2F* hEnergyvsZ_Y=new TH2F("hEnergyvsZ_Y","hEnergyvsZ_Y",48,-240,240,500,0,500);

    TH2F* hNpeVsTimeX=new TH2F("hNpeVsTimeX","hNpeVsTimeX",100,0,25,100,0,400);
    TH2F* hNpeVsTimeY=new TH2F("hNpeVsTimeY","hNpeVsTimeY",100,0,25,100,0,400);
    TH2F* hNpeVsTimeZ=new TH2F("hNpeVsTimeZ","hNpeVsTimeZ",100,0,25,100,0,400);
    
    TH2F* hMPPCHits_XY[1000];
    TH2F* hMPPCHits_XZ[1000];
    TH2F* hMPPCHits_YZ[1000];
    TH1F* hLY_SumXY[1000];
    
    TString name;
    for(int ievt=evtfirst;ievt<=EntryLast;ievt++){
        name = TString::Format("hMPPCHits_XY_%d",ievt);
        hMPPCHits_XY[ievt] = (TH2F*)h2d_xy->Clone(name);
        name = TString::Format("hMPPCHits_XZ_%d",ievt);
        hMPPCHits_XZ[ievt] = (TH2F*)h2d_xz->Clone(name);
        name = TString::Format("hMPPCHits_YZ_%d",ievt);
        hMPPCHits_YZ[ievt] = (TH2F*)h2d_yz->Clone(name);
        name = TString::Format("hLY_SumXY_%d",ievt);
        hLY_SumXY[ievt] = new TH1F(name,name,48,-240,240);
    }
    
    vector<int> evt_vec;
    
    ///////////  Loop over events //////////////////////
    for(int ievt=evtfirst;ievt<=EntryLast;ievt++){
        tinput->GetEntry(ievt);
        evt_vec.push_back(ievt);
        
        int nhits_withsamepos=0;
        
        cout << "************************" << endl;
        cout << " starting event " << ievt << " and has #hits " << nd280UpEvent->GetNHits() << endl;
        hNhitsEvt->Fill(nd280UpEvent->GetNHits());
        
        if(!(ievt%500)){
            cout << "Event " << ievt;
            sw_event.Stop();
            double CPUtime = sw_event.CpuTime();
            sw_event.Reset();
            sw_event.Start(0);
            cout << " --> CPU time = " << CPUtime << endl;
        }
        
        float inProtonMom=0.0;
        Int_t NTraj = nd280UpEvent->GetNTracks();

        cout << "# OF TRACKS: " << nd280UpEvent->GetNTracks() << endl;
        for (Int_t trajID = 0; trajID < NTraj; trajID++) {
            
            cout << " starting loop over tracks  " << trajID << endl;
            
            TND280UpTrack* track = nd280UpEvent->GetTrack(trajID);
            int Npoints=track->GetNPoints();
            inProtonMom=track->GetInitMom().Mag();
            cout << track->GetPDG() << "\t" << track->GetParentID() << "\t" << track->GetTrackID() << endl;
            //if (!(track->GetPDG() == 2212 && track->GetParentID() == 0 && track->GetTrackID() == 1)) continue;

            hProtonTrueEMeV->Fill(inProtonMom);

            /// LOOP OVER HITS
            TH2F* hits_map_XY = (TH2F*)h2d_xy->Clone("hits_map_XY");
            TH2F* hits_map_XZ = (TH2F*)h2d_xz->Clone("hits_map_XZ");
            TH2F* hits_map_YZ = (TH2F*)h2d_yz->Clone("hits_map_YZ");
            
            int tmp_binX=0.0;
            int tmp_binY=0.0;
            int tmp_binZ=0.0;
            
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
                hHitEnergyMeV->Fill(edep);
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

                hNpeVsTimeX->Fill(timepex, pex);
                hNpeVsTimeY->Fill(timepey, pey);
                hNpeVsTimeZ->Fill(timepez, pez);
                
                double poshitX = ApplyResponse.GetHitPos().x();
                double poshitY = ApplyResponse.GetHitPos().y();
                double poshitZ = ApplyResponse.GetHitPos().z();
                
                int binX = hits_map_XZ->GetXaxis()->FindBin(poshitX);
                int binY = hits_map_XY->GetYaxis()->FindBin(poshitY);
                int binZ = hits_map_XZ->GetYaxis()->FindBin(poshitZ);
                
                if(tmp_binX==binX && tmp_binY==binY && tmp_binZ==binZ) {
                    ++nhits_withsamepos;
                    //cout << " okay they matched " << tmp_binX << "\t" << binX << "\t" << tmp_binY << "\t" << binY << "\t" << tmp_binZ << "\t"  << binZ << endl;
                }
                tmp_binX=binX;
                tmp_binY=binY;
                tmp_binZ=binZ;
                
                //if(pex < 50 || pey < 50 || pez < 50) {
                    
                    hEnergyvsZ_X->Fill(poshitZ,pex);
                    hEnergyvsZ_Y->Fill(poshitZ,pey);
                    
                    // cout << "=======================" << "\n"
                    // << " event no. " << ievt << "\n"
                    // << " Proton initial momentum " << inProtonMom << "\n"
                    // << " LY in pex: pey : pez  " << pex << "  " << pey << "  " << pez << "\n"
                    // << " number of hits " << NHits << "\n"
                    // << " current hit number " << ihit << "\n"
                    // << " energy in the hit " << edep << "\n"
                    // << " gettrack length " << steplength << "\n"
                    // << " detname " << detname << "\n"
                    // << " posX : posY : posZ  " << posX << "  " << posY << "  " << posZ << "\n"
                    // << " hit contributor size " << nd280UpHit->fContributors.size() << "\n"
                    // << "======================="
                    // << endl;
                //}
                
                hMPPCHits_XY[ievt]->Fill(poshitX,poshitY,pez);
                hMPPCHits_XZ[ievt]->Fill(poshitX,poshitZ,pey);
                hMPPCHits_YZ[ievt]->Fill(poshitY,poshitZ,pex);

                int contCnt0_X = 0;
                int contCnt0_Y = 0;

                int contCnt1_X = 0;
                int contCnt1_Y = 0;

                //cout << " hit contributor size " << nd280UpHit->fContributors.size() << endl;
                for (UInt_t i = 0; i < nd280UpHit->fContributors.size(); ++i) {
                    if (hits_map_XZ->GetBinContent(binX, binZ) == 0){
                            contCnt1_X++;
                        }
                    if (hits_map_YZ->GetBinContent(binY, binZ) == 0) {
                            contCnt1_Y++;
                        } 
                    else{
                        if (hits_map_XZ->GetBinContent(binX, binZ) == 0) {
                            contCnt1_X++;
                        }
                        if (hits_map_YZ->GetBinContent(binY, binZ) == 0) {
                            contCnt1_Y++;
                        }
                    }
                }
                hHitEnergy_peX->Fill(pex);
                hHitEnergy_peY->Fill(pey);
                hHitEnergy_peZ->Fill(pez);

                hits_map_XY->Fill(poshitX,poshitY,pez);  // pe along Z
                hits_map_XZ->Fill(poshitX,poshitZ,pey); // pe along Y
                hits_map_YZ->Fill(poshitY,poshitZ,pex); // pe along X
            
                //hLY_SumXY[ievt]->Fill(poshitZ,(pex+pey));
                hdEdZvsZ->Fill(poshitZ,(pex+pey));
                hdEdZvsZ_X->Fill(poshitZ,pex);
                hdEdZvsZ_Y->Fill(poshitZ,pey);

                for(int j = 0; j <3; j++){
                    Hit* hit = event->AddHit();
                    hit->SetView(j);

                    //ALTERT:: HARD-CODED for PROTOTYPE!
                    hit->SetX(poshitX/10+12);
                    hit->SetY(poshitY/10+4);
                    hit->SetZ(poshitZ/10+24);

                    hit->SetTrueX(poshitX/10+12);
                    hit->SetTrueY(poshitY/10+4);
                    hit->SetTrueZ(poshitZ/10+24);

                    hit->SetTrueXPoint(posX/10+12);
                    hit->SetTrueYPoint(posY/10+4);
                    hit->SetTrueZPoint(posZ/10+24);
                    hit->SetPDG(track->GetPDG());
                    hit->SetTrackID(track->GetTrackID());
                    hit->SetParentID(track->GetParentID());
                    hit->SetEdep(edep);

                    // cout << endl << "-------------------"      << endl;
                    // cout << "X-cube:  " << poshitX/10+12       << endl;
                    // cout << "X-pos:   " << posX/10+12    << endl;
                    // cout << "Y-cube:  " << poshitY/10+4        << endl;
                    // cout << "Y-pos:   " << posY/10+4     << endl;
                    // cout << "Z-cube:  " << poshitZ/10+24       << endl;
                    // cout << "Z-pos:   " << posZ/10+24    << endl << endl;
                    // cout << "PDG:     " << track->GetPDG()     << endl;
                    // cout << "trackID: " << track->GetTrackID() << endl;
                    // cout << "parntID: " << track->GetParentID() << endl;
                    // cout << "-------------------"              << endl << endl;

                    if(j == 0) hit->SetZ(-1);
                    if(j == 1) hit->SetY(-1);
                    if(j == 2) hit->SetX(-1);

                    if(j == 0) hit->SetCharge(pez);
                    if(j == 1) hit->SetCharge(pey);
                    if(j == 2) hit->SetCharge(pex);

                    hit->SetTrueChargeXY(pez);
                    hit->SetTrueChargeXZ(pey);
                    hit->SetTrueChargeYZ(pex);

                    hQxy->Fill(pez);
                    hQxz->Fill(pey);
                    hQyz->Fill(pex);

                    hit->SetMC(kTRUE);
                    hit->SetTrueXTalk(kFALSE);
                }

//// ADDING CROSSTALK

            Double_t TotQ = 0;
            Double_t Qxy = 0;
            Double_t Qxz = 0;
            Double_t Qyz = 0;
            Double_t rndunif = 0;

            Double_t pX_p1 = fRndm->Uniform();
            Double_t pY_p1 = fRndm->Uniform();
            Double_t pX_m1 = fRndm->Uniform();
            Double_t pY_m1 = fRndm->Uniform();

            // X+1
            if((poshitX) >= X_min && (poshitX) <= X_max-10){
                if (pX_p1 < xTalkProbX/*+sigmoid->Eval(pex)*/) {
                    TotQ = (pex+pey+pez)/3;
                    Qxy = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    Qxz = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    Qyz = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    if(Qxy<0) Qxy = 0;
                    if(Qxy<0) Qxz = 0;
                    if(Qxy<0) Qyz = 0;
                    
                    for(int j = 0; j <3; j++){
                        Hit* hit = event->AddHit();
                        hit->SetView(j);
                        hit->SetX(poshitX/10+12+1);
                        hit->SetY(poshitY/10+4);
                        hit->SetZ(poshitZ/10+24);

                        hit->SetTrueX(poshitX/10+12+1);
                        hit->SetTrueY(poshitY/10+4);
                        hit->SetTrueZ(poshitZ/10+24);

                        hit->SetTrueXPoint(-999);
                        hit->SetTrueYPoint(-999);
                        hit->SetTrueZPoint(-999);
                        hit->SetPDG(track->GetPDG());
                        hit->SetTrackID(track->GetTrackID());
                        hit->SetParentID(track->GetParentID());
                        hit->SetEdep(-999);

                        if(j == 0) hit->SetZ(-1);
                        if(j == 1) hit->SetY(-1);
                        if(j == 2) hit->SetX(-1);

                        if(j == 0) hit->SetCharge(Qxy);
                        if(j == 1) hit->SetCharge(Qxz);
                        if(j == 2) hit->SetCharge(Qyz);

                        hit->SetTrueChargeXY(Qxy);
                        hit->SetTrueChargeXZ(Qxz);
                        hit->SetTrueChargeYZ(Qyz);

                        hit->SetMC(kTRUE);
                        hit->SetTrueXTalk(kTRUE);
                    }
                
                }
            }

            // X-1
            if((poshitX) >= X_min+10 && (poshitX) <= X_max){
                if (pX_m1 < xTalkProbX/*+sigmoid->Eval(pex)*/) {
                    TotQ = (pex+pey+pez)/3;
                    Qxy = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    Qxz = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    Qyz = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    if(Qxy<0) Qxy = 0;
                    if(Qxy<0) Qxz = 0;
                    if(Qxy<0) Qyz = 0;
                    
                    for(int j = 0; j <3; j++){
                        Hit* hit = event->AddHit();
                        hit->SetView(j);
                        hit->SetX(poshitX/10+12-1);
                        hit->SetY(poshitY/10+4);
                        hit->SetZ(poshitZ/10+24);

                        hit->SetTrueX(poshitX/10+12-1);
                        hit->SetTrueY(poshitY/10+4);
                        hit->SetTrueZ(poshitZ/10+24);

                        if(j == 0) hit->SetZ(-1);
                        if(j == 1) hit->SetY(-1);
                        if(j == 2) hit->SetX(-1);

                        if(j == 0) hit->SetCharge(Qxy);
                        if(j == 1) hit->SetCharge(Qxz);
                        if(j == 2) hit->SetCharge(Qyz);

                        hit->SetTrueChargeXY(Qxy);
                        hit->SetTrueChargeXZ(Qxz);
                        hit->SetTrueChargeYZ(Qyz);

                        hit->SetTrueXPoint(-999);
                        hit->SetTrueYPoint(-999);
                        hit->SetTrueZPoint(-999);
                        hit->SetPDG(track->GetPDG());
                        hit->SetTrackID(track->GetTrackID());
                        hit->SetParentID(track->GetParentID());
                        hit->SetEdep(-999);

                        hit->SetMC(kTRUE);
                        hit->SetTrueXTalk(kTRUE);
                    }
                
                }
            }

            // Y+1
            if((poshitY) >= Y_min && (poshitY) <= Y_max-10){
                if (pY_p1 < xTalkProbY) {
                    TotQ = (pex+pey+pez)/3;
                    Qxy = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    Qxz = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    Qyz = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    if(Qxy<0) Qxy = 0;
                    if(Qxy<0) Qxz = 0;
                    if(Qxy<0) Qyz = 0;
                    
                    for(int j = 0; j <3; j++){
                        Hit* hit = event->AddHit();
                        hit->SetView(j);
                        hit->SetX(poshitX/10+12);
                        hit->SetY(poshitY/10+4+1);
                        hit->SetZ(poshitZ/10+24);

                        hit->SetTrueX(poshitX/10+12);
                        hit->SetTrueY(poshitY/10+4+1);
                        hit->SetTrueZ(poshitZ/10+24);

                        hit->SetTrueXPoint(-999);
                        hit->SetTrueYPoint(-999);
                        hit->SetTrueZPoint(-999);
                        hit->SetPDG(track->GetPDG());
                        hit->SetTrackID(track->GetTrackID());
                        hit->SetParentID(track->GetParentID());
                        hit->SetEdep(-999);

                        if(j == 0) hit->SetZ(-1);
                        if(j == 1) hit->SetY(-1);
                        if(j == 2) hit->SetX(-1);

                        if(j == 0) hit->SetCharge(Qxy);
                        if(j == 1) hit->SetCharge(Qxz);
                        if(j == 2) hit->SetCharge(Qyz);

                        hit->SetTrueChargeXY(Qxy);
                        hit->SetTrueChargeXZ(Qxz);
                        hit->SetTrueChargeYZ(Qyz);

                        hit->SetMC(kTRUE);
                        hit->SetTrueXTalk(kTRUE);
                    }
                
                }
            }

            // Y-1
            if((poshitY) >= Y_min+10 && (poshitY) <= Y_max){
                if (pY_m1 < xTalkProbY) {
                    TotQ = (pex+pey+pez)/3;
                    Qxy = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    Qxz = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    Qyz = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    if(Qxy<0) Qxy = 0;
                    if(Qxy<0) Qxz = 0;
                    if(Qxy<0) Qyz = 0;
                    
                    for(int j = 0; j <3; j++){
                        Hit* hit = event->AddHit();
                        hit->SetView(j);
                        hit->SetX(poshitX/10+12);
                        hit->SetY(poshitY/10+4-1);
                        hit->SetZ(poshitZ/10+24);

                        hit->SetTrueX(poshitX/10+12);
                        hit->SetTrueY(poshitY/10+4-1);
                        hit->SetTrueZ(poshitZ/10+24);

                        hit->SetTrueXPoint(-999);
                        hit->SetTrueYPoint(-999);
                        hit->SetTrueZPoint(-999);
                        hit->SetPDG(track->GetPDG());
                        hit->SetTrackID(track->GetTrackID());
                        hit->SetParentID(track->GetParentID());
                        hit->SetEdep(-999);

                        if(j == 0) hit->SetZ(-1);
                        if(j == 1) hit->SetY(-1);
                        if(j == 2) hit->SetX(-1);

                        if(j == 0) hit->SetCharge(Qxy);
                        if(j == 1) hit->SetCharge(Qxz);
                        if(j == 2) hit->SetCharge(Qyz);

                        hit->SetTrueChargeXY(Qxy);
                        hit->SetTrueChargeXZ(Qxz);
                        hit->SetTrueChargeYZ(Qyz);


                        hit->SetMC(kTRUE);
                        hit->SetTrueXTalk(kTRUE);
                    }
                
                }
            }


            // X+1 to Y+1
            if((poshitX) >= X_min && (poshitX) <= X_max-10 && (poshitY) >= Y_min && (poshitY) <= Y_max-10){
                if ((pX_p1+pY_p1+fRndm->Uniform())/3 < xTalkProbXY) {
                    TotQ = (pex+pey+pez)/3;
                    Qxy = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    Qxz = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    Qyz = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    if(Qxy<0) Qxy = 0;
                    if(Qxy<0) Qxz = 0;
                    if(Qxy<0) Qyz = 0;
                    
                    for(int j = 0; j <3; j++){
                        Hit* hit = event->AddHit();
                        hit->SetView(j);
                        hit->SetX(poshitX/10+12+1);
                        hit->SetY(poshitY/10+4+1);
                        hit->SetZ(poshitZ/10+24);

                        hit->SetTrueX(poshitX/10+12+1);
                        hit->SetTrueY(poshitY/10+4+1);
                        hit->SetTrueZ(poshitZ/10+24);

                        hit->SetTrueXPoint(-999);
                        hit->SetTrueYPoint(-999);
                        hit->SetTrueZPoint(-999);
                        hit->SetPDG(track->GetPDG());
                        hit->SetTrackID(track->GetTrackID());
                        hit->SetParentID(track->GetParentID());
                        hit->SetEdep(-999);

                        if(j == 0) hit->SetZ(-1);
                        if(j == 1) hit->SetY(-1);
                        if(j == 2) hit->SetX(-1);

                        if(j == 0) hit->SetCharge(Qxy);
                        if(j == 1) hit->SetCharge(Qxz);
                        if(j == 2) hit->SetCharge(Qyz);

                        hit->SetTrueChargeXY(Qxy);
                        hit->SetTrueChargeXZ(Qxz);
                        hit->SetTrueChargeYZ(Qyz);

                        hit->SetMC(kTRUE);
                        hit->SetTrueXTalk(kTRUE);
                    }
                }
            }

            // X+1 to Y-1
            if((poshitX) >= X_min && (poshitX) <= X_max-10 && (poshitY) >= Y_min+10 && (poshitY) <= Y_max){
                if ((pX_p1+pY_m1+fRndm->Uniform())/3 < xTalkProbXY) {
                    TotQ = (pex+pey+pez)/3;
                    Qxy = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    Qxz = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    Qyz = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    if(Qxy<0) Qxy = 0;
                    if(Qxy<0) Qxz = 0;
                    if(Qxy<0) Qyz = 0;

                    for(int j = 0; j <3; j++){
                        Hit* hit = event->AddHit();
                        hit->SetView(j);
                        hit->SetX(poshitX/10+12+1);
                        hit->SetY(poshitY/10+4-1);
                        hit->SetZ(poshitZ/10+24);

                        hit->SetTrueX(poshitX/10+12+1);
                        hit->SetTrueY(poshitY/10+4-1);
                        hit->SetTrueZ(poshitZ/10+24);

                        hit->SetTrueXPoint(-999);
                        hit->SetTrueYPoint(-999);
                        hit->SetTrueZPoint(-999);
                        hit->SetPDG(track->GetPDG());
                        hit->SetTrackID(track->GetTrackID());
                        hit->SetParentID(track->GetParentID());
                        hit->SetEdep(-999);

                        if(j == 0) hit->SetZ(-1);
                        if(j == 1) hit->SetY(-1);
                        if(j == 2) hit->SetX(-1);

                        if(j == 0) hit->SetCharge(Qxy);
                        if(j == 1) hit->SetCharge(Qxz);
                        if(j == 2) hit->SetCharge(Qyz);

                        hit->SetTrueChargeXY(Qxy);
                        hit->SetTrueChargeXZ(Qxz);
                        hit->SetTrueChargeYZ(Qyz);

                        hit->SetMC(kTRUE);
                        hit->SetTrueXTalk(kTRUE);
                    }    
                }
            }

            // X-1 to Y+1
            if((poshitX) >= X_min+10 && (poshitX) <= X_max && (poshitY) >= Y_min && (poshitY) <= Y_max-10){
                if ((pX_m1+pY_p1+fRndm->Uniform())/3 < xTalkProbXY) {
                    TotQ = (pex+pey+pez)/3;
                    Qxy = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    Qxz = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    Qyz = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    if(Qxy<0) Qxy = 0;
                    if(Qxy<0) Qxz = 0;
                    if(Qxy<0) Qyz = 0;
                    
                    for(int j = 0; j <3; j++){
                        Hit* hit = event->AddHit();
                        hit->SetView(j);
                        hit->SetX(poshitX/10+12-1);
                        hit->SetY(poshitY/10+4+1);
                        hit->SetZ(poshitZ/10+24);

                        hit->SetTrueX(poshitX/10+12-1);
                        hit->SetTrueY(poshitY/10+4+1);
                        hit->SetTrueZ(poshitZ/10+24);

                        hit->SetTrueXPoint(-999);
                        hit->SetTrueYPoint(-999);
                        hit->SetTrueZPoint(-999);
                        hit->SetPDG(track->GetPDG());
                        hit->SetTrackID(track->GetTrackID());
                        hit->SetParentID(track->GetParentID());
                        hit->SetEdep(-999);

                        if(j == 0) hit->SetZ(-1);
                        if(j == 1) hit->SetY(-1);
                        if(j == 2) hit->SetX(-1);

                        if(j == 0) hit->SetCharge(Qxy);
                        if(j == 1) hit->SetCharge(Qxz);
                        if(j == 2) hit->SetCharge(Qyz);

                        hit->SetTrueChargeXY(Qxy);
                        hit->SetTrueChargeXZ(Qxz);
                        hit->SetTrueChargeYZ(Qyz);

                        hit->SetMC(kTRUE);
                        hit->SetTrueXTalk(kTRUE);
                    }             
                }
            }

            // X-1 to Y-1
            if((poshitX) >= X_min+10 && (poshitX) <= X_max && (poshitY) >= Y_min+10 && (poshitY) <= Y_max){
                if ((pX_m1+pY_m1+fRndm->Uniform())/3 < xTalkProbXY) {
                    TotQ = (pex+pey+pez)/3;
                    Qxy = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    Qxz = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    Qyz = xTalkFact*fRndm->Gaus(TotQ,sqrt(TotQ));
                    if(Qxy<0) Qxy = 0;
                    if(Qxy<0) Qxz = 0;
                    if(Qxy<0) Qyz = 0;

                    for(int j = 0; j <3; j++){
                        Hit* hit = event->AddHit();
                        hit->SetView(j);
                        hit->SetX(poshitX/10+12-1);
                        hit->SetY(poshitY/10+4-1);
                        hit->SetZ(poshitZ/10+24);

                        hit->SetTrueX(poshitX/10+12-1);
                        hit->SetTrueY(poshitY/10+4-1);
                        hit->SetTrueZ(poshitZ/10+24);

                        hit->SetTrueXPoint(-999);
                        hit->SetTrueYPoint(-999);
                        hit->SetTrueZPoint(-999);
                        hit->SetPDG(track->GetPDG());
                        hit->SetTrackID(track->GetTrackID());
                        hit->SetParentID(track->GetParentID());
                        hit->SetEdep(-999);
                        
                        if(j == 0) hit->SetZ(-1);
                        if(j == 1) hit->SetY(-1);
                        if(j == 2) hit->SetX(-1);

                        if(j == 0) hit->SetCharge(Qxy);
                        if(j == 1) hit->SetCharge(Qxz);
                        if(j == 2) hit->SetCharge(Qyz);

                        hit->SetTrueChargeXY(Qxy);
                        hit->SetTrueChargeXZ(Qxz);
                        hit->SetTrueChargeYZ(Qyz);

                        hit->SetMC(kTRUE);
                        hit->SetTrueXTalk(kTRUE);
                    }
                }
            }


            // cout << " size of primary  x  " << contCnt1_X << "  y  " << contCnt1_X << endl;
            // cout << " size of primary secondary x  " << contCnt0_X << "  y  " << contCnt0_X << endl;
            } /// END LOOP OVER HITS
            // cout << " hits having same pos in an event " << nhits_withsamepos <<  endl;
            // AllEvents->Fill();
            // event->Clear();
            //break; // only primary track is considered
        } /// END LOOP OVER TRACKS
        event->SetNumTrueTracks(nd280UpEvent->GetNTracks());
        AllEvents->Fill();
        event->Clear();
    } /// END LOOP OVER EVENTS LOOP


    TCanvas *cAux = new TCanvas("cAux");
    cAux->cd(1);
    hQxy->SetLineColor(kBlue);
    hQxy->SetLineWidth(2);
    hQxy->Draw();
    hQxz->SetLineColor(kGreen+1);
    hQxz->SetLineWidth(2);
    hQxz->Draw("same");
    hQyz->SetLineColor(kRed+1);
    hQyz->SetLineWidth(2);
    hQyz->Draw("same");
    cAux->Update();

    fileout->cd();

    AllEvents->Write();
    hNhitsEvt->Write();
    
    hHitEnergyMeV->Write();
    hHitEnergy_peX->Write();
    hHitEnergy_peY->Write();
    hHitEnergy_peZ->Write();
    hProtonTrueEMeV->Write();
    hdEdZvsZ->Write();
    hdEdZvsZ_X->Write();
    hdEdZvsZ_Y->Write();

    hEnergyvsZ_X->Write();
    hEnergyvsZ_Y->Write();

    hNpeVsTimeX->Write();
    hNpeVsTimeY->Write();
    hNpeVsTimeZ->Write();

    cAux->Write();
    
    // TCanvas* c[1000];
    // TH2F* hist=new TH2F("dEdZvsRR","dEdZvsRR",48,0,48,1000,0,1000);
            
    // for (int i= 0; i<Nentries; i++){
    //     c[i]=new TCanvas(TString::Format("can_evt%d",i),TString::Format("can_evt%d",i),1200,800);
    //     //if(hMPPCHits_XY[i]->GetEntries() > 5) {
    //         //          hMPPCHits_XY[ievt]->Write();
    //         //          hMPPCHits_XZ[ievt]->Write();
    //         //          hMPPCHits_YZ[ievt]->Write();
    //         //          hLY_SumXY[ievt]->Write();
    //         c[i]->Divide(2,2);
    //         c[i]->cd(1);
    //         hMPPCHits_XY[i]->Draw("colz");
            
    //         c[i]->cd(2);
    //         hMPPCHits_YZ[i]->Draw("colz");
            
    //         c[i]->cd(3);
    //         hMPPCHits_XZ[i]->Draw("colz");
            
    //         c[i]->cd(4);
    //         hLY_SumXY[i]->Draw("hist");
            
    //         c[i]->Write();
    //     //}
    // }
    //}
    
    //hist->Write();
    fileout->Close();
    
    return 1;
}
