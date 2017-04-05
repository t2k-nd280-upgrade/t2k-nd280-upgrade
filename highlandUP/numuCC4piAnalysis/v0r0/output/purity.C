#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <list>
#include <vector>
#include <string>
#include <cstdlib>

#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "THStack.h"
#include "TChain.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TString.h"
#include "TLatex.h"

TString confName[6] = {"current", 
		       "ref.", 
		       "alt. Target$\\to$TPC",
		       "alt. TPC$\\to$Target",	       
		       "alt. Target(40cm)$\\to$TPC",
		       "alt. TPC$\\to$Target(40cm)"};
TString confName_short[6] = {"curr", 
			     "ref.", 
			     "alt.1 (60cm)",
			     "alt.2 (60cm)",
			     "alt.1 (40cm)",
			     "alt.2 (40cm)"};

TString modeName[4] = {"FwdTPC", "BwdTPC", "HATPC", "ECal"};
TString topoName[4] = {"CC-0#pi", "CC-1#pi^{+}", "CC-other", "BKG"};

int NTargets[6] = {2, 2, 3, 3, 3, 3};

void computePurTopo(int target, int config, int topology, int cut_level, float *fractions, TString suffix="") {
   

  TFile* f = new TFile(TString::Format("jobs/files/config%i_Target%i%s.root",
				       config, target, suffix.Data()));
  TTree* t = (TTree*)f->Get("default");
  
  int numSel[4], tot=0;
  for (int topo=0; topo<4; topo++)
    numSel[topo] = 0;

  for (int i=0; i<4; i++)
    tot += t->GetEntries(TString::Format("accum_level[][%i]>%i", 4*topology+i, cut_level));
  for (int topo=0; topo<3; topo++)
    for (int i=0; i<4; i++)
      numSel[topo] += t->GetEntries(TString::Format("accum_level[][%i]>%i && topology==%i", 4*topology+i, cut_level, topo));
  for (int i=0; i<4; i++)
    numSel[3] += t->GetEntries(TString::Format("accum_level[][%i]>%i && (topology<0 || topology>2)", 4*topology+i, cut_level));

  for (int i=0; i<4; i++) {
    if (tot<10) fractions[i] = -1;
    else        fractions[i] = (float)numSel[i]/(float)tot;
  }


}

float computeEffTopo(int target, int config, int topology, int cut_level, TString suffix="") {
   

  TFile* f = new TFile(TString::Format("jobs/files/config%i_Target%i%s.root",
				       config, target, suffix.Data()));
  TTree* t = (TTree*)f->Get("truth");
  
  int sel=0, tot=0;

  for (int i=0; i<4; i++)
    sel += t->GetEntries(TString::Format("accum_level[%i]>%i && topology==%i", 4*topology+i, cut_level, topology));
  tot = t->GetEntries(TString::Format("topology==%i", topology));
 
  return (float)sel/(float)tot;


}

void printPurTopo(int topoSel, TString suffix="") {

  cout << setprecision(3);
  for (int ic=0; ic<4; ic++) {
    cout << "\\hline \\multirow{" << NTargets[ic] << "}{*}{" << confName[ic] << "} & ";
    for (int it=1; it<=NTargets[ic]; it++) {
      if (it!=1) cout << " & ";
      cout << TString::Format("%s %i", (ic!=1 && it<=2) ? "FGD":"Tgt", it<=2 ? it:1);
      float fractions[4];
      computePurTopo(it, ic, topoSel, 5, fractions, suffix);
      for (int topo=0; topo<4; topo++) {
	if (fractions[topo]<0) cout << " & -";
	else                   cout << " & " << 100*fractions[topo];
      }
      cout << " \\\\" << endl;
    }
  }
  
}

void printPurTopoInfluence(int topoSel) {

  TString suffix1[3] = {"_20mm_0ME", "_70mm_0ME", "_infmm_0ME"};
  TString suffix2[4] = {"_infmm_0ME", "_infmm_30ME", "_infmm_50ME", "_infmm_70ME"};

  cout << setprecision(3);
  for (int ic=0; ic<4; ic++) {
    cout << "\\hline \\multirow{" << NTargets[ic] << "}{*}{" << confName[ic] << "} & ";
    for (int it=1; it<=NTargets[ic]; it++) {
      if (it!=1) cout << " & ";
      cout << TString::Format("%s %i", (ic!=1 && it<=2) ? "FGD":"Tgt", it<=2 ? it:1);
      float fractions[4];
      for (int is=0; is<3; is++) {
	computePurTopo(it, ic, topoSel, 5, fractions, suffix1[is]);
	if (fractions[topoSel]<0) cout << " & -";
	else {                 
	  cout << " & ";
	  //cout << 100*computeEffTopo(it, ic, topoSel, 5, suffix1[is]) << "//"; 
	  cout << 100*fractions[topoSel];
	}
      }
      cout << " \\\\" << endl;
    }
  }

  cout << endl << endl;

  for (int ic=0; ic<4; ic++) {
    cout << "\\hline \\multirow{" << NTargets[ic] << "}{*}{" << confName[ic] << "} & ";
    for (int it=1; it<=NTargets[ic]; it++) {
      if (it!=1) cout << " & ";
      cout << TString::Format("%s %i", (ic!=1 && it<=2) ? "FGD":"Tgt", it<=2 ? it:1);
      float fractions[4];
      for (int is=0; is<4; is++) {
	computePurTopo(it, ic, topoSel, 5, fractions, suffix2[is]);
	if (fractions[topoSel]<0) cout << " & -";
	else {            
	  cout << " & ";
	  //cout << 100*computeEffTopo(it, ic, topoSel, 5, suffix2[is]) << "//";
	  cout << 100*fractions[topoSel];
	}
      }
      cout << " \\\\" << endl;
    }
  }
  
}

