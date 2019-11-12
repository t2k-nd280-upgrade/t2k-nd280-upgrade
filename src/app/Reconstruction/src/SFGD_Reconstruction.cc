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

const int maxTracks = 10;
const int maxHits = 50000;
const int maxCont = 10;

const double fib_abs_const = 0.2;

Int_t numTracks;
Int_t all_numTracks; 
Int_t numHits;
bool crosstalk[maxHits];             // == CT, false != CT.
Float_t hitLocation[maxHits][3];     // position (X,Y,Z) of the GEANT4 hit.
Int_t hitPE[maxHits][3];             // charge of the GEANT4 edep, converted to PE.
Int_t threshold[maxHits][3];         // 0 below, 1 above threshold.
Int_t hitEdep[maxHits];              // auxiliar list to produce crosstalk. Contains saturated edep.
Int_t hitTime[maxHits][3];           // time of the hit, after travelling on the fiber.
Int_t hitTraj[maxHits];              // trackID of the hit.
Int_t primID[maxHits];              // trackID of the hit.
Int_t hitCont[maxHits][maxCont];     // trackID of contributors to the hit.

//from PrimID:
Int_t trajID[maxTracks];             // ID of the track ... this can not be stored, but is necessary to fill trajParent
Int_t trajPDG[maxTracks];            // PDG of the hit
Int_t trajStartHit[maxTracks];       // starting point of the trajectory.
Int_t trajEndHit[maxTracks];         // ending point of a trajectory.
Int_t trajEdep[maxTracks];           // total edep generated by the trajectory.
Int_t trajPrim[maxTracks];           // primary ID. 0 == primary. 
Int_t trajParent[maxTracks];         // ID of parent. -1 if primary.
Int_t trajHitsNum[maxTracks];        // num of hits in each track.
Int_t trajHits[maxTracks][maxHits];  // IDs of member hits in each track.

//from TND280UpTrack:
Int_t all_trajID[maxTracks];             
Int_t all_trajPDG[maxTracks];            
Int_t all_trajStartHit[maxTracks];       
Int_t all_trajEndHit[maxTracks];         
Int_t all_trajEdep[maxTracks];           
Int_t all_trajPrim[maxTracks];           
Int_t all_trajParent[maxTracks];             

bool aux_threshold(double x){
    TRandom3 fRndm = TRandom3(0);
    x = x/4;
    double prob = (exp(x)-exp(-x))/((exp(x)+exp(-x)));
    if(x*4==1) prob = 0.1;
    if(x*4==2) prob = 0.35;
    if(x*4==3) prob = 0.60;
    if(x>=1) return true;

    return fRndm.Uniform() < prob;
    // if(x>=1) return true;
    // return false;
}

void ResetVariables(){
    for (int i=0; i < maxHits; i++){
        crosstalk[i] = false;
        hitTraj[i] = -999;
        primID[i]  = -999;
        for (int j=0; j < 3; j++){            
            hitLocation[i][j] = -999;
            hitPE[i][j]       = -999;
            hitTime[i][j]     = -999;
            threshold[i][j]   = -999;
        }
    }

    for (int i=0; i < maxTracks; i++){    
        trajID[i] = -999;    
        trajPDG[i] = -999;
        trajStartHit[i] = -999;
        trajEndHit[i] = -999;
        trajEdep[i] = -999;
        trajPrim[i] = -999;
        trajParent[i] = -999;
        trajHitsNum[i] = 0;

        all_trajID[i] = -999;             
        all_trajPDG[i] = -999;            
        all_trajStartHit[i] = -999;       
        all_trajEndHit[i] = -999;         
        all_trajEdep[i] = -999;           
        all_trajPrim[i] = -999;           
        all_trajParent[i] = -999;         

        for (int j=0; j < maxHits; j++){
            trajHits[i][j] = -999;
        }   
    }
}


int FiberAbs(int numPE){  
    TRandom3 fRndm = TRandom3(0);  
    int numPE_fiber = 0;
    for (int i=0;i<numPE;i++){
        double rndunif = fRndm.Uniform();
        if (rndunif < fib_abs_const) numPE_fiber++;
    }
    return numPE_fiber;
}

int FindParent(int inputID, std::vector<int> listOfParentID, TND280UpEvent* evt){
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

double BirksSaturation(double edep, double steplength)
{
  //const double CBIRKS = 0.08 * CLHEP::cm/CLHEP::MeV; 
  const double CBIRKS = 0.0208 * CLHEP::cm/CLHEP::MeV; 
  double dedx = edep/steplength;
  return edep/(1. + CBIRKS*dedx);
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

    TString outfilename = "/nfs/neutrinos/cjesus/work/MC_output.root";
    string rootfilename = argv[1];
    const int evtfirst = atoi(argv[2]);
    const int nevents = atoi(argv[3]);
    const int detectorID = atoi(argv[5]);
    outfilename = argv[13];

    cout << "evtfirst: " << evtfirst << ",nevents: " << nevents << endl;

    TFile *fileout = new TFile(outfilename.Data(),"RECREATE");
    TTree *AllEvents = new TTree("AllEvents", "The ROOT tree of events");
    ND280SFGDEvent* event = new ND280SFGDEvent();

    int Nentries = -999;

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

    AllEvents->Branch("Event", "Event", event);

    bool fillAll = false;
    if(fillAll){
        AllEvents->Branch( "numTracks",      &numTracks,     "numTracks/I"                     );
        AllEvents->Branch( "numHits",        &numHits,       "numHits/I"                       );
        AllEvents->Branch( "all_numTracks",  &all_numTracks, "all_numTracks/I"                 );
        AllEvents->Branch( "crosstalk",      crosstalk,      "crosstalk[numHits]/B"            );
        AllEvents->Branch( "hitLocation",    hitLocation,    "hitLocation[numHits][3]/F"       );
        AllEvents->Branch( "hitPE",          hitPE,          "hitPE[numHits][3]/I"             );
        AllEvents->Branch( "threshold",      threshold,      "threshold[numHits][3]/I"         );
        AllEvents->Branch( "hitTime",        hitTime,        "hitTime[numHits][3]/I"           );
        AllEvents->Branch( "hitTraj",        hitTraj,        "hitTraj[numHits]/I"              );
        AllEvents->Branch( "primID",         primID,         "primID[numHits]/I"              );
        AllEvents->Branch( "hitCont",        hitCont,        "hitCont[numHits][1000]/I"     );
        AllEvents->Branch( "trajID",         trajID,         "trajID[numTracks]/I"             );
        AllEvents->Branch( "trajPDG",        trajPDG,        "trajPDG[numTracks]/I"            );
        // AllEvents->Branch( "trajStartHit",   trajStartHit,   "trajStartHit[numTracks]/I"       );
        // AllEvents->Branch( "trajEndHit",     trajEndHit,     "trajEndHit[numTracks]/I"         );
        AllEvents->Branch( "trajEdep",       trajEdep,       "trajEdep[numTracks]/I"           );
        AllEvents->Branch( "trajParent",     trajParent,     "trajParent[numTracks]/I"         );
        AllEvents->Branch( "trajHitsNum",    trajHitsNum,    "trajHitsNum[numTracks]/I"        );
        AllEvents->Branch( "trajHits",       trajHits,       "trajHits[numTracks][5000]/I"  );
        AllEvents->Branch( "all_trajID",         all_trajID,         "all_trajID[1000]/I"             );
        AllEvents->Branch( "all_trajPDG",        all_trajPDG,        "all_trajPDG[1000]/I"            );
        // AllEvents->Branch( "all_trajStartHit",   all_trajStartHit,   "all_trajStartHit[numTracks]/I"       );
        // AllEvents->Branch( "all_trajEndHit",     all_trajEndHit,     "all_trajEndHit[numTracks]/I"         );
        AllEvents->Branch( "all_trajParent",     all_trajParent,     "all_trajParent[1000]/I"         );       
    }

    
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

    vector <ND280SFGDHit*> listOfHits;
    vector <ND280SFGDVoxel*> listOfVoxels;
    vector <ND280SFGDTrack*> listOfTracks;
    ///////////  Loop over events //////////////////////
    for(int ievt=evtfirst;ievt<=EntryLast;ievt++){
        tinput->GetEntry(ievt);
        ResetVariables();
        int index = 0;
        bool store = true;
        bool WriteText = false;
        int PE_expect = 0;
        int PE_found = 0;

        std::map<int,int> trackToParentID;
        std::map<int,int> trackToPDG;

        cout << "************************" << endl;
        cout << " starting event " << ievt << " and has #hits " << nd280UpEvent->GetNHits() << endl;

        if(nd280UpEvent->GetNHits()*7+1 > maxHits){
            cout << "ERROR: the maxHits value must be enlarged to at least: " << nd280UpEvent->GetNHits()*7+2 << endl;
            exit(0);
        }

        int NHits = nd280UpEvent->GetNHits();
        numHits = NHits;

        int edep_without_clear_contributor = 0;


        std::vector<int> listOfParentID;
        std::vector <int> listOfTrueTrackID;
        for (Int_t trjID = 0; trjID < nd280UpEvent->GetNTracks(); trjID++) {
            TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);
            listOfTrueTrackID.push_back(track->GetTrackID());
            listOfParentID.push_back(track->GetTrackID());
            cout << "ALL || PDG: " <<  track->GetPDG()
                     << ", ID: " <<    track->GetTrackID() 
                     << ", prntID: " << track->GetParentID() << endl;
        }

        for(int ihit=0;ihit<NHits;ihit++){ // get last entry
            TND280UpHit *nd280UpHit = nd280UpEvent->GetHit(ihit);

            TRandom3 fRndm = TRandom3(0);
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

            ApplyResponse.CalcResponse(lightPos,1,0,charge,time,steplength,edep,detname);//ApplyResponse.CalcResponse(lightPos,1,0,1 ,0 /*time*/,-1 /*steplength*/,sat_edep*156.42,"kSuperFGD");
            PE_expect += (ApplyResponse.GetHitPE().x() +  ApplyResponse.GetHitPE().y() +  ApplyResponse.GetHitPE().z());

#ifdef CROSSTALK_OFF
            for(int view=0; view<3; view++){                
                ND280SFGDHit* hit = event->AddHit();
                hit->SetView(view);

                hit->SetX(ApplyResponse.GetHitPos().x()/10);
                hit->SetY(ApplyResponse.GetHitPos().y()/10);
                hit->SetZ(ApplyResponse.GetHitPos().z()/10);

                if(view == 0) hit->SetZ(-1);
                if(view == 1) hit->SetY(-1);
                if(view == 2) hit->SetX(-1);
                if(view == 0) hit->SetPE(ApplyResponse.GetHitPE().z());
                if(view == 1) hit->SetPE(ApplyResponse.GetHitPE().y());
                if(view == 2) hit->SetPE(ApplyResponse.GetHitPE().x());

                listOfHits.push_back(hit);
            }
            index++;
#else
            double edeposit = nd280UpHit->GetEnergyDeposit();
            double trueTime = (nd280UpHit->GetStartT() + nd280UpHit->GetStopT())/2.;
            ND280UpTargReadOut TargetReadOut;
            TargetReadOut.SetTargType(DetType);
            int nphot = TargetReadOut.ApplyScintiResponse(edeposit,steplength,1);
            double probLatCT = 0.01*fRndm.Poisson(2.8)*6;//0.025*6;//0.037*6;
            if(WriteText) cout << "nphot= " << nphot << endl;
            int numPE = nphot*(1./fib_abs_const);
            if(numPE < 0) numPE = 0;
            int numPE_CT[6] = {0};           // photons flowing to sourrounding elements
            for (int i=0;i<numPE;i++){
                double rndunif = fRndm.Uniform();
                if (rndunif < probLatCT) numPE_CT[(int) (fRndm.Uniform()*6)]++;
            }
            int totCT = 0;
            for (auto p : numPE_CT) totCT += p;
            if(WriteText) cout << "Flowed PE: " << totCT << endl;
            if(WriteText) cout << "Number of remaining PE: " << numPE-totCT << endl;

            int trkID = - 999;
            for (int m=0; m<7;m++){                
                int numPE_fiber = 0;
                if(m==0) numPE_fiber = FiberAbs(numPE-totCT);
                else numPE_fiber = FiberAbs(numPE_CT[m-1]);

                if(m==0){
                    bool done = false;
                    std::vector<int>::reverse_iterator it = nd280UpHit->fContributors.rbegin();
                    for (; it!= nd280UpHit->fContributors.rend(); ++it){
                        for (auto trueTrackID:listOfTrueTrackID){
                            if ((*it) == trueTrackID) {trkID =(*it); done = true; break;}                 
                        }
                    if(done) break;
                    }
                }

                if(m==0){
                    if (trkID < 0) {trkID = nd280UpHit->GetPrimaryId(); edep_without_clear_contributor++;}
                    // cout << "nd280UpHit->GetPrimaryId() " << nd280UpHit->GetPrimaryId() << endl;
                    // cout << "trkID: " << trkID << endl;
                    // cout << "fContributors: " << nd280UpHit->fContributors.size() << endl;
                    //for (auto c:nd280UpHit->fContributors) cout << c << ",";
                    //cout << endl << endl;
                }

                 if(trkID <0) { cout << "NOT STORING THE EVENT! negative TRK ID!: " << trkID <<","<< m << endl; store = false;}

                //cout << m <<", New # of Abs PE: " << numPE_fiber << endl;

                ApplyResponse.CalcResponse(lightPos,1,0,1 ,0 /*time*/,-1 /*steplength*/,numPE_fiber,detname);
                
                double pex = ApplyResponse.GetHitPE().x();
                double pey = ApplyResponse.GetHitPE().y();
                double pez = ApplyResponse.GetHitPE().z();
 
                PE_found += (pex+pey+pez);

                double timepex = ApplyResponse.GetHitTime().x();
                double timepey = ApplyResponse.GetHitTime().y();
                double timepez = ApplyResponse.GetHitTime().z();
                
                double poshitX = ApplyResponse.GetHitPos().x();
                double poshitY = ApplyResponse.GetHitPos().y();
                double poshitZ = ApplyResponse.GetHitPos().z();

                if(m==1) poshitX += 10;
                if(m==2) poshitX -= 10;
                if(m==3) poshitY += 10;
                if(m==4) poshitY -= 10;
                if(m==5) poshitZ += 10;
                if(m==6) poshitZ -= 10;
                
                if (m==0) crosstalk[index] = false;
                else      crosstalk[index] = true;
                hitLocation[index][0] = poshitX/10;
                hitLocation[index][1] = poshitY/10;
                hitLocation[index][2] = poshitZ/10;
                hitTime[index][0]     = timepez;
                hitTime[index][1]     = timepey;
                hitTime[index][2]     = timepex;
                hitPE[index][0]       = pez;
                hitPE[index][1]       = pey;
                hitPE[index][2]       = pex;
                hitEdep[index]        = edeposit;
                hitTraj[index]        = trkID;
                primID[index]         = nd280UpHit->GetPrimaryId();

                threshold[index][0] = aux_threshold(hitPE[index][0]);
                threshold[index][1] = aux_threshold(hitPE[index][1]);
                threshold[index][2] = aux_threshold(hitPE[index][2]);

                if(!threshold[index][0]) hitPE[index][0] = 0;
                if(!threshold[index][1]) hitPE[index][1] = 0;
                if(!threshold[index][2]) hitPE[index][2] = 0;

                ND280SFGDVoxel* Voxel = new ND280SFGDVoxel(hitLocation[index][0],hitLocation[index][1],hitLocation[index][2]);
                Voxel->SetTrueTime(trueTime);
                Voxel->SetTrueEdep(edeposit);
                Voxel->SetTruePE(numPE_fiber);
                Voxel->AddTrueTrackID(trkID);
                if (crosstalk[index]) Voxel->SetTrueType(1);
                else  Voxel->SetTrueType(0);
                if(Voxel->GetTruePE()>0) listOfVoxels.push_back(Voxel);

                if(m!=0) Voxel->SetTrueEdep(0);

                std::vector<ND280SFGDHit*> voxHits;        
                for(int view=0; view<3; view++){        
                    ND280SFGDHit* hit = event->AddHit();
                    hit->SetView(view);

                    hit->SetX(hitLocation[index][0]);
                    hit->SetY(hitLocation[index][1]);
                    hit->SetZ(hitLocation[index][2]);

                    if(view == 0) hit->SetZ(-1);
                    if(view == 1) hit->SetY(-1);
                    if(view == 2) hit->SetX(-1);
                    if(view == 0) hit->SetPE(hitPE[index][0]);
                    if(view == 1) hit->SetPE(hitPE[index][1]);
                    if(view == 2) hit->SetPE(hitPE[index][2]);

                    listOfHits.push_back(hit);
                    voxHits.push_back(hit);
                }
                Voxel->SetHits(voxHits);
                voxHits.clear();

                // int counter = 0;
                // for (auto contrib : nd280UpHit->fContributors){
                //     if(counter >= maxCont){
                //         store = false;
                //         break;
                //     }
                //     hitCont[index][counter] = contrib;
                //     counter++;
                // }

                if(hitLocation[index][0] > X_min and hitLocation[index][0] < X_max
                   and hitLocation[index][1] > Y_min and hitLocation[index][1] < Y_max
                   and hitLocation[index][2] > Z_min and hitLocation[index][2] < Z_max){
                    if(index+1 != maxHits) index++;
                    if(index+1 == maxHits) store = false;
                }
                if(WriteText) cout << "hit at: " << hitLocation[index][0] << ", " << hitLocation[index][1] << ", " << hitLocation[index][2] << endl;
                if(WriteText) cout << "PE (YZ,XZ,XY): " << pex << ", " << pey << ", " << pez << endl;
            }
            if(WriteText) cout << "PE_expect: " << PE_expect << "PE_found: " << PE_found << endl;
            if(WriteText) cout << endl;
#endif
        } /// END LOOP OVER 3D HITS

        if(WriteText) cout << "total g4hits:   " << NHits << endl;
        if(WriteText) cout << "total g4hits*7: " << NHits*7 << endl;
        if(WriteText) cout << "index:          " << index << endl;

        cout << "# stored hits: " << index << endl;


        std::vector <int> listOfTrackID;

        for(UInt_t i=0; i<maxHits; i++){
            if(hitTraj[i] == -999) break;
            listOfTrackID.push_back(hitTraj[i]);
        }

        //keep only unique track IDs:
        if(listOfTrackID.size()){
            std::sort   (listOfTrackID.begin(), listOfTrackID.end());
            for(size_t itrkID = 0; itrkID < listOfTrackID.size(); itrkID++){
            }
            listOfTrackID.erase(std::unique (listOfTrackID.begin(), listOfTrackID.end()), listOfTrackID.end()); 
        }

        numTracks = listOfTrackID.size();

        int sumEdepPE = 0;
        int hitIndx = 0;
        for(uint i=0; i<listOfTrackID.size(); i++){
            hitIndx = 0;
            sumEdepPE = 0;
            for (Int_t trjID = 0; trjID < nd280UpEvent->GetNTracks(); trjID++) {
                TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);
                if (track->GetTrackID() != listOfTrackID[i]) continue;
                trajPDG[i] = track->GetPDG();
                trajID[i] = track->GetTrackID();
                trajPrim[i] = track->GetParentID();
                trackToPDG[track->GetTrackID()] = track->GetPDG();
                cout << "STORED || PDG: " <<  track->GetPDG()
                     << ", ID: " <<    track->GetTrackID() 
                     << ", prntID: " << track->GetParentID() << endl;
            }            
            for(int j = 0; j < maxHits; j++){
                if(listOfTrackID[i] == hitTraj[j]){
                    trajHitsNum[i]++;
                    trajHits[i][hitIndx] = j;
                    hitIndx++;
                    sumEdepPE += hitPE[j][0];
                    sumEdepPE += hitPE[j][1];
                    sumEdepPE += hitPE[j][2];
                }
            }
            trajEdep[i] = sumEdepPE;
        }

        // fill all trajectories information:
        all_numTracks = nd280UpEvent->GetNTracks();
        for (Int_t trjID = 0; trjID < nd280UpEvent->GetNTracks(); trjID++) {
            TND280UpTrack* track = nd280UpEvent->GetTrack(trjID);
            if (trjID >= maxTracks){
                store = false;
                break;
            }
            // cout << "trk is: " << track->GetTrackID() << " || GetInitMom " << track->GetInitMom().Mag()
            //      << " || trueCosTheta: " << track->GetInitCosTheta()
            //      << " || Range: " << track->GetRange() 
            //      << endl;

            bool found = false;
            for(auto trk_ID:listOfTrackID) if (trk_ID == track->GetTrackID()) found = true;
            if(!found) continue;

            ND280SFGDTrack* sfgdtrack = new ND280SFGDTrack();
            sfgdtrack->SetPDG(track->GetPDG());
            sfgdtrack->SetTrackID(track->GetTrackID());
            sfgdtrack->SetParentID(track->GetParentID());
            sfgdtrack->SetCosTheta(track->GetInitCosTheta());
            sfgdtrack->SetRange(track->GetRange());
            sfgdtrack->SetMomentum(track->GetInitMom().Mag());
            sfgdtrack->fMomVec = track->GetInitMom();

            //if (sfgdtrack->GetParentID() == -1) cout << "TrackInfo: [Id,ParentId,PDG,Mom,Range,CosTheta] " << sfgdtrack->GetTrackID() << "," <<  sfgdtrack->GetParentID() << "," <<  sfgdtrack->GetPDG() << "," <<  sfgdtrack->GetMomentum() << "," <<  sfgdtrack->GetRange() << "," <<  sfgdtrack->GetCosTheta() << endl;

            all_trajPDG[trjID]    = track->GetPDG();
            all_trajID[trjID]     = track->GetTrackID();
            all_trajParent[trjID] = track->GetParentID();
            listOfTracks.push_back(sfgdtrack);
        }

        if (WriteText) for (auto lprntID : listOfParentID) cout << "parentID: " << lprntID << endl;

        for(UInt_t i=0; i<listOfTrackID.size(); i++){
            //if(trajID[i] == -999) cout << "999-listOfTrackID[i]: " << listOfTrackID[i] << endl;
            if( trajPrim[i] == 0 ) trajParent[i] = -1;
            else{
                trajParent[i] = FindParent(trajID[i],listOfParentID,nd280UpEvent);
                trackToParentID[trajID[i]] = trajParent[i]; 
                trajPrim[i] = 1; 
            }
            if (trajParent[i] == -999) trajParent[i] = primID[i];
            if (trajParent[i] == -999) {cout << "\n\n\n ERRRROR in parent ID!!! \n\n\n";store = false;}    // deactivate to store all...
            //cout << "ID: " << trajID[i] << ", parent: " << trajParent[i] << endl;
            trackToParentID[trajID[i]] = trajParent[i]; 
        }

        for(uint vxl=0; vxl<listOfVoxels.size(); ++vxl){
            listOfVoxels[vxl]->AddTrueParentID(trackToParentID.find(listOfVoxels[vxl]->GetTrueTrackIDs()[0])->second);
            listOfVoxels[vxl]->AddTruePDG(trackToPDG.find(listOfVoxels[vxl]->GetTrueTrackIDs()[0])->second);
            //if(listOfVoxels[vxl]->GetTrueParentIDs()[0] <-1 ) { if(WriteText) cout << "ERROR IN PARENT ID! [ParentID, Id, PDG] (pdg 22 is gamma): " << listOfVoxels[vxl]->GetTrueParentIDs()[0] << "," << listOfVoxels[vxl]->GetTrueTrackIDs()[0] << "," << listOfVoxels[vxl]->GetTruePDGs()[0] << endl; store=false;}
            if(WriteText) cout << "voxel: "<< vxl << "\t, Type: " << listOfVoxels[vxl]->GetTrueType() << "\tXYZ: " << listOfVoxels[vxl]->GetX() << ","  << listOfVoxels[vxl]->GetY() << "," << listOfVoxels[vxl]->GetZ() << ",\ttrueDeposits -[Edep,FiberPE,xy,xz,yz]: " <<  listOfVoxels[vxl]->GetTrueEdep()*0.25 << ",\t" << listOfVoxels[vxl]->GetTruePE()*0.25 <<  ",\t" << listOfVoxels[vxl]->GetHits()[0]->GetPE() << ",\t" << listOfVoxels[vxl]->GetHits()[1]->GetPE() << ",\t" << listOfVoxels[vxl]->GetHits()[2]->GetPE()
                 << ",\tTrackInfo [ID,prntID,PDG]: " <<  listOfVoxels[vxl]->GetTrueTrackIDs()[0] << "," << listOfVoxels[vxl]->GetTrueParentIDs()[0] << "," << listOfVoxels[vxl]->GetTruePDGs()[0] << endl;
        }
        cout << "Voxels voxels w/o clear track ID: " << edep_without_clear_contributor << endl;

        numHits = index;

        // if(NHits){ // TODO: store this information in output && comment couts; + avoid entering into ApplyResponse if outside SFGD!
        //     int nVertices = nd280UpEvent->GetNVertices();
        //     cout << " *** Vertices: " << nVertices << endl;
        //     TND280UpVertex* vrtx = nd280UpEvent->GetVertex(0);
        //     TND280UpHit *hitSFGD = nd280UpEvent->GetHit(0);
        //     TVector3 vPos(vrtx->GetPosition().x(),vrtx->GetPosition().y(),vrtx->GetPosition().z()+1707 );
        //     cout << "Original Vtx Position: " << vrtx->GetPosition().x() << "," << vrtx->GetPosition().y() << "," << vrtx->GetPosition().z()<< endl;
        //     cout << "Vertex Position: " << vPos.x() << "," << vPos.y() << "," << vPos.z() << endl;
        //     cout << "MPPC limits Position: " << ApplyResponse.GetMPPCPosX() << "," << ApplyResponse.GetMPPCPosY() << "," << ApplyResponse.GetMPPCPosZ()<< endl;
        //     ApplyResponse.CalcResponse(vPos,1,0,1,0,0,0,hitSFGD->GetDetName());
        //     cout << "Vertex Cube Position: " << ApplyResponse.GetHitPos().x()/10 << "," << ApplyResponse.GetHitPos().y()/10 << "," << ApplyResponse.GetHitPos().z()/10 << endl;
        // }

        if(true){
         for (std::map<int,int>::iterator it=trackToParentID.begin(); it!=trackToParentID.end(); ++it)
            std::cout << "trackToParentID: " <<it->first << " => " << it->second << '\n';

         for (std::map<int,int>::iterator it=trackToPDG.begin(); it!=trackToPDG.end(); ++it)
            std::cout << "trackPDG: " <<it->first << " => " << it->second << '\n';
        }

        if(store){
                cout << " --- Event summary --- " << endl;
                cout << "The event has: " << index << "hits." << endl;
                cout << "The event has: " << numTracks << "tracks." << endl << endl;
                cout << " --- summary of tracks --- " << endl;
                int locSum = 0;
                for (int trks=0; trks<numTracks; trks++){
                    locSum += trajHitsNum[trks];
                    cout << "Track " << trks << ", id: " << trajID[trks] << ", #hits: " << trajHitsNum[trks] << ", totPE: " << trajEdep[trks] << ", Prim: " << trajPrim[trks] << ", Parent: " << trajParent[trks] << ", PDG: " << trajPDG[trks] << endl;
                }
            if(WriteText){
                cout << "Hits assocaited to tracks: " << locSum << endl;
                cout << "Hits without associated track: " << numHits-locSum << endl;
                cout << " --- summary of hits --- " << endl;
                for (int hts=0; hts<numHits; hts++){
                    cout << "Hit " << hts << "crosstalk: " << crosstalk[hts] << ", trajID: " << hitTraj[hts] << ", XYZ: " << hitLocation[hts][0] << "," << hitLocation[hts][1] <<"," << hitLocation[hts][2] << ", PE: " << hitPE[hts][0] << "," << hitPE[hts][1] <<"," << hitPE[hts][2] << ", time: " << hitTime[hts][0] << "," << hitTime[hts][1] <<"," << hitTime[hts][2] << endl;
                }
            }
        }

        event->SetVoxels(listOfVoxels);
        event->SetHits(listOfHits);
        event->SetTrueTracks(listOfTracks);
        cout << "event->GetHits().size(): " << event->GetHits().size() << endl;
        if(!index) store = false;
        if(store) {cout << "Event stored as input. " << endl; AllEvents->Fill(); }
        else{ cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nEVENT IS NOT STORED\n\n";}
        event->ResetEvent();
        listOfHits.clear();
        listOfVoxels.clear();
        listOfTracks.clear();
    } /// END LOOP OVER EVENTS LOOP

    cout << "Writing events. " << endl;
    fileout->cd();
    AllEvents->Write();
    h2d_xy->Write();
    h2d_xz->Write();
    h2d_yz->Write();
    fileout->Close();
    cout << "End of program. " << endl;

    return 1;
}
