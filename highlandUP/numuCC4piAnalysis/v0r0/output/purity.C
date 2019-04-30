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

TString INPDIR = "/nfs/neutrinos/cjesus/work/jobs/files/TDR_eff";
//TString INPDIR = "/nfs/neutrinos/cjesus/work/jobs/files/TDR_SFGD_Updated";
TString confName[3] = {"current", 
		       "old upgrade", 
		       "upgrade"};

TString topoName[4] = {"CC-0#pi", "CC-1#pi^{+}", "CC-other", "BKG"};

int NTargets[3] = {2, 2, 3};


Float_t computeEvents(TString suffix, int config, int target, int branch, int cut_level, int true_topo=-1, int useTargetOnly=0) {
  
int it;


  TFile f(TString::Format("%s/Target%i/config%i_Target%i%s.root",
        INPDIR.Data(),target, config, target, suffix.Data()));
  TTree* t = (TTree*)f.Get("default");
  
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

  // cout << "config: " << config << " target: " << target << endl;
  // cout << "true_topo: " << true_topo << endl;
  // cout << "selected events: " << selected_events << endl << endl;
  return selected_events;

}


Float_t computePurity(TString suffix, int config, int target, int branch, int topology, int cut_level, int useTargetOnly=0) {

  return computeEvents(suffix, config, target, branch, cut_level, topology, useTargetOnly)/
    computeEvents(suffix, config, target, branch, cut_level, -1, useTargetOnly);
  //return computeEvents(suffix, config, target, branch, cut_level, topology, useTargetOnly);
}


Float_t stoppingMuons(TString suffix, int config, int target, int useTargetOnly=0) {
  
int it;


  TFile f(TString::Format("%s/Target%i/config%i_Target%i%s.root",
        INPDIR.Data(),target, config, target, suffix.Data()));
  TTree* t = (TTree*)f.Get("default");
  
  TString condition="accum_level[][...]>4 && selmu_target_length > 200";

  Float_t selected_events = t->GetEntries(TString::Format("%s", condition.Data()));

  cout << "THE NUMBER OF SEL EVTS IS: " << selected_events << endl;

  return selected_events;

}


void printTable(TString suffix, int useTargetOnly=1) {

  int conf[2] = {0,2};

  cout << setprecision(3);  

  for (int c=0; c<2; c++) {
    cout << "\\multirow{" << NTargets[conf[c]] << "}{*}{" << confName[conf[c]] << "}";
    for (int t=1; t<=NTargets[conf[c]]; t++) {

      TString targetName="";
      if (conf[c]==0 || t<=2)
	targetName = TString::Format("FGD %i", t);
      else
	targetName = "SuperFGD";
      cout << " & " << targetName;

      cout << " & $" << (int)computeEvents(suffix, conf[c], t, -1, 4, -1, useTargetOnly) << "$";
      for (int topo=0; topo<=2; topo++)
	       cout << " & $" << 100*computePurity(suffix, conf[c], t, topo, topo, 4, useTargetOnly) << "\\%$";

  //cout << " & $" << (int)computePurity(suffix, conf[c], t, topo, topo, 4, useTargetOnly) << "$";
      cout << " \\\\" << endl;
    }
  }

}

void printTables() {

  TString suffix[3] = {"_FHC_numu", "_FHC_antinu", "_RHC_numu"};

  for (int s=0; s<1; s++) {
    cout << "\n\n" << suffix[s] << "\n\n";
    printTable(suffix[s]);
  }

}


void printSummaryTable(int useTargetOnly=0) {

  int conf[2] = {0,2};

  TString suffix[3] = {"_FHC_numu", "_FHC_antinu", "_RHC_numu"};
  TString suffix_name[3] = {"$\\nu_{\\mu}$ ($\\nu$ beam)", 
			    "$\\bar\\nu_{\\mu}$ ($\\bar\\nu$ beam)",
			    "$\\nu_{\\mu}$ ($\\bar\\nu$ beam)"};

  for (int s=0; s<1; s++) {
    cout << suffix_name[s];
    for (int c=1; c<2; c++) {
      Int_t ntot = 0;
      for (int t=1; t<=NTargets[conf[c]]; t++)
	ntot += (int)computeEvents(suffix[s], conf[c], t, -1, 4, -1, useTargetOnly);
      cout << " & $" << ntot << "$";
    }
    cout << " \\\\" << endl;
  }

}


void printPurityTarget() {

  TString suffix[3] = {"_FHC_numu", "_FHC_antinu", "_RHC_numu"};
  TString suffix_name[3] = {"$\\nu_{\\mu}$ (FHC)", 
			    "$\\bar\\nu_{\\mu}$ (RHC)",
			    "$\\nu_{\\mu}$ (RHC)"};

  cout << setprecision(1) << fixed; 

  for (int s=0; s<1; s++) {
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
