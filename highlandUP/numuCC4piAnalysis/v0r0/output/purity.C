#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <list>
#include <vector>
#include <string>
#include <cstdlib>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"

using namespace std;

TString confName[3] = {"current", 
		       "old upgrade", 
		       "upgrade"};

TString topoName[4] = {"CC-0#pi", "CC-1#pi^{+}", "CC-other", "BKG"};

int NTargets[3] = {2, 2, 3};


Float_t computeEvents(TString suffix, int config, int target, int branch, int cut_level, int true_topo=-1, int useTargetOnly=0) {
   
  TFile* f = new TFile(TString::Format("jobs/files/config%i_Target%i%s.root",
				       config, target, suffix.Data()));
  TTree* t = (TTree*)f->Get("default");
  
  TString cutTargetOnly="";
  if (useTargetOnly==0)
    cutTargetOnly = " && accum_level[][6]<=4";
  if (useTargetOnly==2)
    cutTargetOnly = " && accum_level[][6]>4";
  TString cutTopo="";
  if (true_topo>=0)
    cutTopo = TString::Format(" && topology==%i", true_topo);

  TString cutBranch = TString::Format("accum_level[][%i]>%i", branch, cut_level);
  if (branch==-1)
    cutBranch = TString::Format("(accum_level[][3]>%i || accum_level[][4]>%i || accum_level[][5]>%i || accum_level[][6]>%i || accum_level[][7]>%i)", 
				cut_level, cut_level, cut_level, cut_level, cut_level);

  Float_t selected_events = t->GetEntries(TString::Format("%s%s%s", cutBranch.Data(), cutTopo.Data(), cutTargetOnly.Data()));

  return selected_events;

}


Float_t computePurity(TString suffix, int config, int target, int branch, int topology, int cut_level, int useTargetOnly=0) {

  return computeEvents(suffix, config, target, branch, cut_level, topology, useTargetOnly)/
    computeEvents(suffix, config, target, branch, cut_level, -1, useTargetOnly);

}


void printTable(TString suffix, int useTargetOnly=0) {

  int conf[2] = {0,2};

  cout << setprecision(3);  

  for (int c=0; c<2; c++) {
    cout << "\\multirow{" << NTargets[conf[c]] << "}{*}{" << confName[conf[c]] << "}";
    for (int t=1; t<=NTargets[conf[c]]; t++) {

      TString targetName="";
      if (conf[c]==0 || t<=2)
	targetName = TString::Format("FGD %i", t);
      else
	targetName = "Horiz.Target";
      cout << " & " << targetName;

      cout << " & $" << (int)computeEvents(suffix, conf[c], t, -1, 4, -1, useTargetOnly) << "$";
      for (int topo=0; topo<=2; topo++)
	cout << " & $" << 100*computePurity(suffix, conf[c], t, topo, topo, 4, useTargetOnly) << "\\%$";
      cout << " \\\\" << endl;
    }
  }

}

void printTables() {

  TString suffix[3] = {"", "_antinu", "_nubkg_antinu"};

  for (int s=0; s<3; s++) {
    cout << "\n\n" << suffix[s] << "\n\n";
    printTable(suffix[s]);
  }

}


void printSummaryTable(int useTargetOnly=0) {

  int conf[2] = {0,2};

  TString suffix[3] = {"", "_antinu", "_nubkg_antinu"};
  TString suffix_name[3] = {"$\\nu_{\\mu}$ ($\\nu$ beam)", 
			    "$\\bar\\nu_{\\mu}$ ($\\bar\\nu$ beam)",
			    "$\\nu_{\\mu}$ ($\\bar\\nu$ beam)"};

  for (int s=0; s<3; s++) {
    cout << suffix_name[s];
    for (int c=0; c<2; c++) {
      Int_t ntot = 0;
      for (int t=1; t<=NTargets[conf[c]]; t++)
	ntot += (int)computeEvents(suffix[s], conf[c], t, -1, 4, -1, useTargetOnly);
      cout << " & $" << ntot << "$";
    }
    cout << " \\\\" << endl;
  }

}


void printPurityTarget() {

  TString suffix[3] = {"", "_antinu", "_nubkg_antinu"};
  TString suffix_name[3] = {"$\\nu_{\\mu}$ (FHC)", 
			    "$\\bar\\nu_{\\mu}$ (RHC)",
			    "$\\nu_{\\mu}$ (RHC)"};

  cout << setprecision(1) << fixed; 

  for (int s=0; s<3; s++) {
    cout << "\\multirow{2}{*}{" << suffix_name[s] << "}";

      for (int target=0; target<=2; target+=2) {
	if (target==0) cout << " & TPC+ECal";
	if (target==2) cout << " & Target";

	for (int topo=-1; topo<3; topo++) {
	  float pur = 0;
	  if (topo==-1) {
	    for (int t=0; t<3; t++)
	      pur += computePurity(suffix[s], 2, 3, -1, t, 4, target);
	  }
	  else
	    pur = computePurity(suffix[s], 2, 3, topo, topo, 4, target);

	  cout << " & $" << 100*pur << "\\%$";
	}

	cout << " \\\\ " << endl;
      }
  }

}
