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
#include "THStack.h"
#include "TChain.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TString.h"
#include "TLatex.h"

TString confName[6] = {"current", 
		       "ref.", 
		       "alt. Target(60cm)$\\to$TPC",
		       "alt. Target(40cm)$\\to$TPC",
		       "alt. TPC$\\to$Target(60cm)",
		       "alt. TPC$\\to$Target(40cm)"};

TString modeName[4] = {"FwdTPC", "BwdTPC", "HATPC", "ECal"};
TString topoName[4] = {"CC-0#pi", "CC-1#pi^{+}", "CC-other", "BKG"};

int NTargets[6] = {2, 2, 3, 3, 3, 3};

void computePurTopo(int target, int config, int mode, int cut_level, int topoToSel, float *fractions) {
   

  TFile* f = new TFile(TString::Format("jobs/files/config%i_Target%i_%i.root",
				       config, target, mode));
  TTree* t = (TTree*)f->Get("default");
  
  int numSel[4], tot=0;
  TString topoSelection;
  
  if (topoToSel==0)
    topoSelection = "sel_nPosPions==0 && sel_nOtherPions==0";
  else if (topoToSel==1)
    topoSelection = "sel_nPosPions==1 && sel_nOtherPions==0";
  else if (topoToSel==2)
    topoSelection = "sel_nPosPions>1 || sel_nOtherPions>0";

  tot = t->GetEntries(TString::Format("accum_level[][0]>%i && %s", 
				      cut_level, topoSelection.Data()));
  for (int topo=0; topo<3; topo++)
    numSel[topo] = t->GetEntries(TString::Format("accum_level[][0]>%i && %s && topology==%i", 
						 cut_level, topoSelection.Data(), topo));
  numSel[3] = t->GetEntries(TString::Format("accum_level[][0]>%i && %s && (topology<0 || topology>2)", 
					    cut_level, topoSelection.Data()));

  for (int i=0; i<4; i++) {
    if (tot==0) fractions[i] = -1;
    else        fractions[i] = (float)numSel[i]/(float)tot;
  }


}

//computePurTopo(int target, int config, int mode, int cut_level, int topoToSel, float* fractions)
void printPurTopo(int topoSel) {

  cout << setprecision(3);
  for (int ic=0; ic<6; ic++) {
    cout << "\\hline \\multirow{" << NTargets[ic] << "}{*}{" << confName[ic] << "} & ";
      for (int it=1; it<=NTargets[ic]; it++) {
	if (it!=1) cout << " & ";
	cout << TString::Format("%s %i", (ic!=1 && it<=2) ? "FGD":"Tgt", it<=2 ? it:1);
	for (int im=0; im<2; im++) {
	  float fractions[4];
	  computePurTopo(it, ic, im, 4, topoSel, fractions);
	  for (int topo=0; topo<4; topo++) {
	    if (fractions[topo]<0) cout << " & -";
	    else                   cout << " & " << 100*fractions[topo];
	  }
	}
	cout << " \\\\" << endl;
      }
  }

  cout << endl << endl;

  for (int ic=0; ic<6; ic++) {
    cout << "\\hline \\multirow{" << NTargets[ic] << "}{*}{" << confName[ic] << "} & ";
    for (int it=1; it<=NTargets[ic]; it++) {
      if (it!=1) cout << " & ";
      cout << TString::Format("%s %i", (ic!=1 && it<=2) ? "FGD":"Tgt", it<=2 ? it:1);
      for (int im=2; im<4; im++) {
	float fractions[4];
	computePurTopo(it, ic, im, 4, topoSel, fractions);
	for (int topo=0; topo<4; topo++) {
	  if (fractions[topo]<0) cout << " & -";
	  else                   cout << " & " << 100*fractions[topo];
	}
      }
      cout << " \\\\" << endl;
    }
  }
}
