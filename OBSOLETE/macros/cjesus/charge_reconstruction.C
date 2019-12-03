
int VERBOSE(0);

typedef double dbl;

struct voxel
{
  int  X         = -999;
  int  Y         = -999;
  int  Z         = -999;
  int  ID        = - 1;           // unique ID for the voxel
  dbl  Q[3]      = {-1,-1,-1};    // estimators of the charge of the voxel.
  dbl  Qreco     = 0;          // reconstructed charge from the estimators;
  dbl  Qtrue     = -999;          // true charge from the MC;
  bool IsReco    = false;         // true if there is any Q != -1;
  bool IsDone    = false;         // true if Qreco is != -9999;
  int  fibers[3] = {-1,-1,-1};  // ID of XY,XZ and YZ fibers.
  int  type      = -1;

  double DistToMPPC(int x){
    double distance;
    if (x == 2)      distance =  X*10+1020;       // X is distance to plane YZ 
    else if (x == 1) distance =  Y*10+260;  // Y is distance to plane XZ
    else if (x == 0) distance =  Z*10+940; // Z is distance to plane 
    if (distance<0){cerr << "Distance can not be negative! case: " << x << "distance: " << distance << endl; exit(1);}
    return distance;
  }
  // methods:
  void initialize(int x, int y, int z) {X=x; Y=y; Z=z;}
};

struct fiber{
  int I  = -1;    // I coordinate in IJ plane
  int J  = -1;    // J coordiante in IJ plane
  int V  = -1;    // V plane: 0:XY, 1:XZ, 2:YZ
  int M  = -1;    // Multiplicity of the fiber
  dbl Qm = -1;   // Charge measured
  dbl Qg = -1;   // Charge given
  int m  = -1;    // Initial multiplicity in the fiber
  dbl ID = -1;    // ID of the fiber
  bool IsMerged = false;
  std::vector <int> voxels; // vector of ID of the interestected voxels; //this value shrinks when the voxel is reconstructed.

  // methods:
  void initialize(int i, int j, int v) {I=i; J=j; V=v;}
};

double att(double x){
  const double a             = 0.77;
  const double LongAtt       = 4634.;
  const double ShortAtt      = 332.;
  const double d             = 0;//41;
  double factor = a*exp((-x-d)/LongAtt) + (1-a)*exp((-x-d)/ShortAtt);
  return factor;
}

void charge_reconstruction(){

  std::vector <voxel> vlist;
  std::vector <fiber> flist;
  int ID_v = -1;
  int ID_f = -1;

  TH1F* h_results = new TH1F("results","results",200,-500,500);
  TH1F* h_M1      = new TH1F("M1","M1",200,-500,500);
  TH1F* h_M1_plus = new TH1F("M1p","M1p",200,-500,500);
  TH2F* h_reco_2D = new TH2F("ratioVSq"," ",20,-100,200,20,0,200);
  TH2F* h_true_reco = new TH2F("recoVStrue"," ",100,0,200,100,0,200);

  int pre_evt_ID = 0;
  bool do_reconstruction = false;

  ifstream fin("/Users/cjesus/Documents/Data/SFGD_MC/InputNN_0_999.txt");
  if (!fin) { cout << "File not open\n"; exit(1); }
  string line;
  const char delim = ' ';
  while (getline(fin, line))
  {
    double evtID,N,x,y,z,q1,q2,q3,m1,m2,m3,trueQ,size,type;
    std::vector <double> pdgs;
    std::vector <double> trkIDs;
    std::vector <double> prtIDs;
    
    // evtID : number of event;
    // N ----------> 0: voxel info || 1: track info
    // x,y,z  -----> position
    // q1,q2,q3 ---> measurement of 3 fibers XY,XZ,YZ
    // m1,m2,m3 ---> multiplicity of fibers  XY,XZ,YZ
    // trueQ  -----> charge from MC used to produce the fiber hits. Is multiplied by 0.25 to account for MPPC inefficiency. 
    // size -------> number of contributing tracks to the voxel, namely the size == number different track crossing the voxel.
    // type -------> 0: track || 1: crosstalk || 2: ghost

    istringstream str(line);
    str >> evtID >> N;
    if(N) continue;
    str >> x >> y >> z >> q1 >> q2 >> q3 >> m1 >> m2 >> m3 >> trueQ >> size;
    for (int it=0; it<size; ++it){
      int pdg, trkID, prtID;
      str >> trkID >> prtID >> pdg;
      pdgs.push_back(pdg);
      trkIDs.push_back(trkID);
      prtIDs.push_back(prtID);
    }
    str >> type;

    if(evtID != pre_evt_ID) do_reconstruction = true;
    pre_evt_ID = evtID;

    if(VERBOSE>0){
      cout << "line: " << line << endl;
      cout << "Read: " << evtID << " " << N << " " << x << " " << y << " " << z << " " << q1 << " "
           << q2 << " " << q3 << " " << m1 << " " << m2 << " " << m3 << " " << trueQ << " "  << size;
      for (int it=0; it<size; ++it){
        cout << " " << trkIDs[it] << " " << prtIDs[it] << " " << pdgs[it]; 
      }
      cout << " " << type << endl;
    }

    if(do_reconstruction && flist.size()){

      cout << "EVENT: " << evtID << ", MPPC hits: " << flist.size() << endl;
      std::vector <fiber> new_flist;
      ID_f = -1;
      for (uint i=0; i<flist.size(); ++i){
        if (flist[i].IsMerged) continue;
        flist[i].IsMerged = true;
        fiber newf;
        newf.ID = ++ID_f;
        newf.I  = flist[i].I;
        newf.J  = flist[i].J;
        newf.V  = flist[i].V;
        newf.Qm = flist[i].Qm;
        newf.M  = 1;
        newf.voxels.push_back(flist[i].voxels[0]);
        vlist[flist[i].voxels[0]].fibers[flist[i].V] = newf.ID; 
        for (uint j=i+1; j<flist.size(); ++j){
          if (flist[j].IsMerged) continue;
          if (flist[i].I == flist[j].I && flist[i].J == flist[j].J && flist[i].V == flist[j].V){
            flist[j].IsMerged = true;
            newf.voxels.push_back(flist[j].voxels[0]);
            vlist[flist[j].voxels[0]].fibers[flist[j].V] = newf.ID; 
            newf.M++;
          }
        }
        newf.m = newf.M;
        newf.Qg = 0;
        new_flist.push_back(newf);
      }
      flist = new_flist;

      for(auto vxl:vlist){
        int estimators = 0;
        double sumQ = 0;
        for (int s=0; s<3; ++s) if (flist[vxl.fibers[s]].M == 1) {++estimators; sumQ+=flist[vxl.fibers[s]].Qm/att(vxl.DistToMPPC(s));}
        double Qr = sumQ/estimators;
        if(sumQ && estimators ==1 ) h_M1->Fill((Qr-vxl.Qtrue)*100/vxl.Qtrue);
        if(sumQ && estimators>1) h_M1_plus->Fill((Qr-vxl.Qtrue)*100/vxl.Qtrue);
      }

      if(VERBOSE){
        for (auto v:vlist){
          cout << "ID,x,y,z: " << v.ID << "," << v.X << "," << v.Y << "," << v.Z << endl;
          cout << "fID,V,I,J: " << flist[v.fibers[0]].ID << "," << flist[v.fibers[0]].I<< "," << flist[v.fibers[0]].J << "," << flist[v.fibers[0]].V << endl << endl;
        }
      }

      // ___IMPLEMENT_CHARGE_RECONSTRUCTION____
      int iterations = 0;
      double TotQg = 0;
      double TotQm = 0;
      double TotTrueQ = 0;
      double TotRecoQ = 0;
      for (auto f:flist) TotQm += f.Qm;
      for (auto v:vlist) TotTrueQ += v.Qtrue;

      double TotChange = 1E9;
      double ratio = 0;
      while(iterations < 10000 && abs(1-ratio)> 0.01 && ratio < 1){
        bool DEBUG = false;
        TotChange = 0;
        for (uint i=0; i<vlist.size(); ++i){
          cnt++;
          double dQ[3] = {0,0,0};
          double nSum[3] = {0,0,0};
          double own_sum = 0;
          double alpha = 1E-2;
          int estimators=0;
          double qFromM1=0;
          for (int x=0; x<3; ++x){
            int i1 = (x+1)%3;
            int i2 = (x+2)%3;
            int Nneighbors=0;
            for (std::vector<int>::iterator it = flist[vlist[i].fibers[x]].voxels.begin() ; it != flist[vlist[i].fibers[x]].voxels.end(); ++it){
              voxel neighbor = vlist[*it];
              ++Nneighbors;
              nSum[x] += (flist[neighbor.fibers[i1]].Qm/att(neighbor.DistToMPPC(i1))-flist[neighbor.fibers[i1]].Qg/att(neighbor.DistToMPPC(i1)))/flist[neighbor.fibers[i1]].M + (flist[neighbor.fibers[i2]].Qm/att(neighbor.DistToMPPC(i2))-flist[neighbor.fibers[i2]].Qg/att(neighbor.DistToMPPC(i2)))/flist[neighbor.fibers[i2]].M;
            }
            own_sum =   (flist[vlist[i].fibers[i1]].Qm/att(vlist[i].DistToMPPC(i1))-flist[vlist[i].fibers[i1]].Qg/att(vlist[i].DistToMPPC(i1)))/flist[vlist[i].fibers[i1]].M  + (flist[vlist[i].fibers[i2]].Qm/att(vlist[i].DistToMPPC(i2))-flist[vlist[i].fibers[i2]].Qg/att(vlist[i].DistToMPPC(i2)))/flist[vlist[i].fibers[i2]].M;
            if (nSum[x]) dQ[x] = alpha*(flist[vlist[i].fibers[x]].Qm/att(vlist[i].DistToMPPC(x)-flist[vlist[i].fibers[x]].Qg/att(vlist[i].DistToMPPC(x))))*own_sum/(nSum[x]);
            else dQ[x] = 0;
            if(DEBUG) cout << "dQ[" << x << "]: " << dQ[x] << endl;
          }
          double dQ_TOT = (dQ[0]+dQ[1]+dQ[2])/3;
          if((vlist[i].Qreco+2*dQ_TOT)>0 /*&& (flist[vlist[i].fibers[0]].Qg + dQ_TOT/att(vlist[i].DistToMPPC(0))) < flist[vlist[i].fibers[0]].Qm && (flist[vlist[i].fibers[1]].Qg + dQ_TOT/att(vlist[i].DistToMPPC(1))) < flist[vlist[i].fibers[1]].Qm && (flist[vlist[i].fibers[2]].Qg + dQ_TOT/att(vlist[i].DistToMPPC(2))) < flist[vlist[i].fibers[2]].Qm*/){
            vlist[i].Qreco += 2*dQ_TOT;
            TotChange += dQ_TOT;
            for(int x=0; x<3; ++x) flist[vlist[i].fibers[x]].Qg += dQ_TOT/att(vlist[i].DistToMPPC(x));
          }
            // for(int x=0; x<3; ++x){
            //   if(vlist[i].Qreco+2*dQ[x]/3>0 && (flist[vlist[i].fibers[x]].Qg+dQ[x]/att(vlist[i].DistToMPPC(x))) < flist[vlist[i].fibers[x]].Qm*1.001 ){
            //     vlist[i].Qreco += 2*dQ[x]/3;
            //     TotChange += dQ[x];
            //     flist[vlist[i].fibers[x]].Qg += dQ[x]/att(vlist[i].DistToMPPC(x));
            //   }
            // }
          if(DEBUG) cout << "v.Qreco: " << vlist[i].Qreco << endl;
        }
        TotQg = 0;
        for (auto f:flist) TotQg += f.Qg;
        ratio = TotQg/TotQm;
        if(iterations%1000 == 0){
          TotQg = 0;
          TotRecoQ = 0;
          for (auto f:flist) TotQg += f.Qg;
          for (auto v:vlist) TotRecoQ += v.Qreco;
          cout << "Charge measured: " << TotQm << " || Charge given: " << TotQg << " || ratio: " << TotQg/TotQm << endl;
        }
        iterations++;
      }

      cout << "SOLUTION: " << iterations << endl;
      for (uint i=0; i<vlist.size(); ++i) {cout << "v.Qreco: " << vlist[i].Qreco << "|| v.Qtrue: " << vlist[i].Qtrue << " || ratio: " << vlist[i].Qreco/ vlist[i].Qtrue << endl; h_results->Fill((vlist[i].Qreco-vlist[i].Qtrue)*100/vlist[i].Qtrue); h_reco_2D->Fill((vlist[i].Qreco-vlist[i].Qtrue)*100/vlist[i].Qtrue,vlist[i].Qtrue); h_true_reco->Fill(vlist[i].Qtrue,vlist[i].Qreco);}
      TotQg = 0;
      TotRecoQ = 0;
      for (auto f:flist) TotQg += f.Qg;
      for (auto v:vlist) TotRecoQ += v.Qreco;
      cout << "Charge measured: " << TotQm << " || Charge given: " << TotQg << " || ratio: " << TotQg/TotQm << endl;
      cout << "Charge TRUE: " << TotTrueQ << " || Charge RECO.: " << TotRecoQ << " || ratio: " << TotRecoQ/TotTrueQ << endl;
      cout << "Total Change: " << TotChange << endl;
      cout << endl << endl;

      vlist.clear();
      flist.clear();
      ID_v = -1;
      ID_f = -1;
      do_reconstruction = false;
    }

    if(evtID>100) break;

    bool fakeNNout = true;
    if( fakeNNout ){
      if (!trueQ) continue;
      TRandom3* rdnm = new TRandom3(0);
      if (type == 2 && rdnm->Uniform(1) > 0.01) continue;
      if (type == 1 && rdnm->Uniform(1) > 0.06) continue;
      if (type == 0 && rdnm->Uniform(1) > 0.95) continue;
    }

    voxel v;
    v.type = type;
    v.initialize(x,y,z);
    v.ID = ++ID_v;
    v.Qtrue = trueQ;
    fiber f1, f2, f3;
    f1.initialize(x,y,0);      f2.initialize(x,z,1);      f3.initialize(y,z,2);
    f1.Qm = q1;                f2.Qm = q2;                f3.Qm = q3;
    f1.ID = ++ID_f;            f2.ID = ++ID_f;            f3.ID = ++ID_f;
    f1.voxels.push_back(ID_v); f2.voxels.push_back(ID_v); f3.voxels.push_back(ID_v);
    v.fibers[0] = ID_f-2;      v.fibers[1] = ID_f-1;      v.fibers[2] = ID_f;
    flist.push_back(f1);       flist.push_back(f2);       flist.push_back(f3); 
    vlist.push_back(v);

  }

  cout << "ENTRIES: " << h_results->GetEntries() << "," <<  h_M1->GetEntries() << ","  << h_M1_plus->GetEntries() << "," << endl;

  h_results->Scale(1/h_results->Integral(), "width");
  h_M1->Scale(1/h_M1->Integral(), "width");
  h_M1_plus->Scale(1/h_M1_plus->Integral(), "width");


  TCanvas c;
  c.Divide(3,1);
  c.cd(1);
  h_M1_plus->SetLineColor(kGreen+1);
  h_M1_plus->Draw("HIST");
  h_M1->SetLineColor(kRed);
  h_M1->Draw("HIST same");
  h_results->Draw("HIST same");
  h_results->Fit("gaus");
  h_M1->Fit("gaus");
  h_M1_plus->Fit("gaus");
  c.cd(2);
  h_reco_2D->Draw("COLZ");
  c.cd(3);
  h_true_reco->Draw("COLZ");
  c.Update();
  c.WaitPrimitive();
}
