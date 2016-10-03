#include "baseAnalysis.hxx"
#include "AnaTreeConverterEvent.hxx"
#include "ConstituentsUtils.hxx"



//********************************************************************
baseAnalysis::baseAnalysis(AnalysisAlgorithm* ana): AnalysisAlgorithm(ana){
  //********************************************************************

}

//********************************************************************
void baseAnalysis::DefineProductions(){
//********************************************************************


}

//********************************************************************
void baseAnalysis::DefineInputConverters(){
//********************************************************************
 input().AddConverter("anatree",        new AnaTreeConverterEvent());


}

//********************************************************************
bool baseAnalysis::Initialize(){
  //********************************************************************

  // Initialize trees or not at the beginnng of each configuration
  SetInitializeTrees(ND::params().GetParameterI("baseAnalysis.InitializeTrees"));
  




  
  return true;
}


//********************************************************************
void baseAnalysis::DefineSystematics(){
//********************************************************************

}

//********************************************************************
void baseAnalysis::DefineCorrections(){
  //********************************************************************

}

//********************************************************************
void baseAnalysis::DefineConfigurations(){
  //********************************************************************

  //-------  Add and define individual configurations with one systematic only ------------------

}


//********************************************************************
void baseAnalysis::DefineMicroTrees(bool addBase){
  //********************************************************************

  (void)addBase;

  // -------- Add variables to the analysis tree ----------------------

  //--- event variables -------
  AddVarI(output(), run,    "run number");
  AddVarI(output(), evt,    "event number ");
  AddVarI(output(), bunch,  "bunch number ");

  //--- link with truth ----
  AddToyVarI(output(), TruthVertexID,"The unique ID of the true vertex associated to the selected event");
  // --- neutrino truth variables ----
  AddVarI(  output(), nu_pdg,       "neutrino PDG code");
  AddVarF(  output(), nu_trueE,     "true neutrino energy");
  AddVarI(  output(), nu_truereac,  "true netrino reaction code");
  AddVar3VF(output(), nu_truedir,   "true neutrino direction");

  // --- Vertex info
  AddVarI(  output(), selvtx_det,       "detector in which the reconstructed vertex is");
  AddVar4VF(output(), selvtx_pos,       "reconstructed vertex position");
  AddVar4VF(output(), selvtx_truepos,   "position of the true vertex associated to the reconstructed vertex");
}

//********************************************************************
void baseAnalysis::DefineTruthTree(){
  //********************************************************************

  //--- event variables -------
  AddVarI(output(),   evt,    "event number");
  AddVarI(output(),   run,    "run number");
  AddVarI(output(),   subrun, "subrun number");

  // ---- link with ...
  AddVarI(output(),   TruthVertexID, "The unique ID of the true vertex associated to the selected event");

  // ---- neutrino variables
  AddVarI(output(),   nu_pdg,            "neutrino pdg code");
  AddVarF(output(),   nu_trueE,          "true neutrino energy");  // true neutrino energy
  AddVarI(output(),   nu_truereac,       "true neutrino reaction type");  // true neutrino reaction code
  AddVar3VF(output(), nu_truedir,        "true neutrino direction");  // true neutrino direction
  AddVar4VF(output(), truevtx_pos,       "true neutrino interaction position");

}

//********************************************************************
bool baseAnalysis::FinalizeConfiguration(){
  //********************************************************************

  // This is called before FillMicroTrees()

  // Save the accum level for the true vertex associated to the recon one such that efficiencies can be computed from the truth tree
  if (conf().GetCurrentConfigurationIndex() != ConfigurationManager::default_conf)
    return true;


  std::vector<AnaTrueVertex*> trueVertices;
  trueVertices.reserve(NMAXTRUEVERTICES);
  std::vector<AnaTrueVertex*>::const_iterator iter;


  AnaTrueVertex* trueVertex = NULL;
  if (GetVertex()) trueVertex = dynamic_cast<AnaTrueVertex*>(GetVertex()->TrueVertex);
  else             trueVertex = dynamic_cast<AnaTrueVertex*>(GetTrueVertex());

  if (trueVertex) trueVertices.push_back(trueVertex);


  // If true vertex does not exist (e.g. can happen that reco vertex is not yet available at this step of the selection) then 
  // store the accum_level to all true vertices of the bunch -> i.e. this basically corresponds to the fact that event as a whole 
  // passed some cuts
  if (trueVertices.size() == 0){
    // Loop over all true vertices in the event and found those that belong to the bunch being processed
    std::vector<AnaTrueVertexB*> vertices = GetSpill().TrueVertices;
    for (std::vector<AnaTrueVertexB*>::iterator it = vertices.begin(); it!=vertices.end(); it++) {
      if (!(*it)) continue;
      AnaTrueVertex* vtx = dynamic_cast<AnaTrueVertex*>(*it);
      // Check the bunch
      if (GetBunch().Bunch != vtx->Bunch) continue;

      trueVertices.push_back(vtx); 
    }
  }

  // Loop over all true vertices of interest
  for (iter = trueVertices.begin(); iter != trueVertices.end(); iter++){ 
    AnaTrueVertex* vtx = *iter;
    if (!vtx) continue;

    // When the AccumLevel has not been already saved for this vertex 
    if (vtx->AccumLevel.size() == 0)
      vtx->AccumLevel.resize(sel().GetNEnabledSelections());
    //else{
    // Sometimes the same true vertex is asigned to the candidate in another bunch, most likely because of a delayed track. In this case 
    // save the higher accum level
    // std::cout << "baseAnalysis::FinalizeConfiguration(). This true vertex was used in another bunch (likely a delayed track). Save higher accum_level" << std::endl;
    //}
    for (std::vector<SelectionBase*>::iterator it = sel().GetSelections().begin(); it != sel().GetSelections().end(); it++){

      if (!(*it)->IsEnabled()) continue;

      Int_t isel = (*it)->GetEnabledIndex();

      if (vtx->AccumLevel[isel].size() == 0)
        vtx->AccumLevel[isel].resize((*it)->GetNBranches());

      for (UInt_t ibranch=0;ibranch<(*it)->GetNBranches();ibranch++)
        vtx->AccumLevel[isel][ibranch]=(*it)->GetAccumCutLevel(ibranch);
    }
  }
  return true;
}

//********************************************************************
void baseAnalysis::FillMicroTreesBase(bool addBase){
  //********************************************************************
  //std::cout<<"-------- baseAnalysis::FillMicroTreesBase ----------------------"<<std::endl;

  (void)addBase;

  // This is called after FinalizeConfiguration

  // Fill all tree variables that are common to all toys in a given configuration

  // Event variables
  output().FillVar(run,    GetSpill().EventInfo->Run);
  output().FillVar(evt,    GetSpill().EventInfo->Event);
  output().FillVar(bunch,  GetBunch().Bunch);

  // default flux_weight. TO BE REMOVED
  //  Float_t flux_weight =1;

  // Vertex info
  if (GetVertex()){
   // output().FillVar(selvtx_det,anaUtils::GetDetector(GetVertex()->Position));
    output().FillVectorVarFromArray(selvtx_pos,GetVertex()->Position, 4);
    if (GetVertex()->TrueVertex)
      output().FillVectorVarFromArray(selvtx_truepos, GetVertex()->TrueVertex->Position, 4);
  }

  // Neutrino truth variables
  if (GetVertex()){
    if (GetVertex()->TrueVertex){
      AnaTrueVertex* trueVertex = dynamic_cast<AnaTrueVertex*>(GetVertex()->TrueVertex);
      output().FillVar(nu_pdg,        trueVertex->NuPDG);
      output().FillVar(nu_trueE,      trueVertex->NuEnergy);
      output().FillVar(nu_truereac,   trueVertex->ReacCode);

//      output().FillVectorVarFromArray(nu_truedir,        trueVertex->NuDir,    3);

      // Get the beam weight. By construction this is 1 for the data (no need to disable it). TO BE REMOVED
      //      if (_flux && !GetSpill().GetIsSandMC()){
      //        flux_weight = _flux->GetWeight(trueVertex,anaUtils::GetRunPeriod(GetSpill().EventInfo->Run));
      //      }    
    }
  }

  // Fill the flux weight.  TO BE REMOVED
  //  if (_flux){
  //    output().FillVectorVar(weight, flux_weight);
  //    output().IncrementCounter(NWEIGHTS);
  //  }

  // The pile up correction weight.  TO BE REMOVED
  //  output().FillVectorVar(weight, GetBunch().Weight);
  //  output().IncrementCounter(NWEIGHTS);*/
}

//********************************************************************
void baseAnalysis::FillToyVarsInMicroTreesBase(bool addBase){
  //********************************************************************

  (void)addBase;

  if (!GetVertex()) return;
  if (!GetVertex()->TrueVertex) return;

  AnaTrueVertex* trueVertex = dynamic_cast<AnaTrueVertex*>(GetVertex()->TrueVertex);

  output().FillToyVar(TruthVertexID, trueVertex->ID);  

  // Check whether to save the RooTracker information. If we're not saving
  // it, we set the "RooVtxEntry" (which is the link to the right entry in
  // the RooTrackerVtx tree) to -999.
}

//********************************************************************
void baseAnalysis::FillTruthTreeBase(const AnaTrueVertex& vtx, const SubDetId::SubDetEnum det, bool IsAntinu){
  //********************************************************************

  // Fill track categories for color drawing
  anaUtils::FillCategories(&vtx, det, IsAntinu, GetSpill().GetIsSandMC());

  // Event variables
  output().FillVar(run,    GetSpill().EventInfo->Run);
  output().FillVar(subrun, GetSpill().EventInfo->SubRun);
  output().FillVar(evt,    GetSpill().EventInfo->Event);

  // true variables
  output().FillVar(nu_pdg,        vtx.NuPDG);
  output().FillVar(nu_trueE,      vtx.NuEnergy);
  output().FillVar(nu_truereac,   vtx.ReacCode);
  output().FillVectorVarFromArray(truevtx_pos,    vtx.Position, 4);

  // Fill event weights. Currently, only the weight from flux tuning is filled here.
  //  if (_flux && !GetSpill().GetIsSandMC()) {
  //    output().FillVectorVar(weight, _flux->GetWeight(vtx,anaUtils::GetRunPeriod(GetSpill().EventInfo->Run)));
  //    output().IncrementCounterForVar(weight);
  //  }
  
}

//********************************************************************
void baseAnalysis::FillTruthTree(){
//********************************************************************

  // Fill the "truth" tree

  if (!output().HasTree(OutputManager::truth)) return;

  output().SetCurrentTree(OutputManager::truth);

  // Loop over all true vertices
  std::vector<AnaTrueVertexB*> vertices = GetSpill().TrueVertices;
  for (std::vector<AnaTrueVertexB*>::iterator it = vertices.begin(); it!=vertices.end(); it++) {
    AnaTrueVertex& vtx = *dynamic_cast<AnaTrueVertex*>(*it);
    // Check if this vertex needs to be saved in the truth tree
    if (!CheckFillTruthTree(vtx)) continue;
    // Initialize the truth tree. We must do that for each saved vertex since several entries may correspond to the save spill
    output().InitializeTree(OutputManager::truth);

    // Associate the vertex with the current RooTrackerVtx entry
    // We should do this here (even when the current RooTrackerVtx entry has been already saved).
    // if the RooVtxEntry has been already save in  baseAnalysis::FillMicroTreesBase, but there is more than one vertex to be saved for
    // this entry the link has not been yet established
 
    // accumulated cut levels to compute efficiencies. This is taken directly from the AnaTrueVertex
    Int_t accumLevel=1;  // intialize to 1 because EventQuality is passed by definition in MC (TODO: what about analysis in which the first cut is not EventQuality)
    for (std::vector<SelectionBase*>::iterator itf=sel().GetSelections().begin();itf!=sel().GetSelections().end();itf++){
      if (!(*itf)->IsEnabled()) continue;

      Int_t isel = (*itf)->GetEnabledIndex();

      for (UInt_t ibranch=0;ibranch<(*itf)->GetNBranches();ibranch++){
        if (vtx.AccumLevel.size()>0) accumLevel = vtx.AccumLevel[isel][ibranch];
        if (sel().GetNEnabledSelections()>1){
          if (sel().GetNMaxBranches()>1)
            output().FillMatrixVar(accum_level, accumLevel, isel, ibranch);
          else
            output().FillVectorVar(accum_level, accumLevel, isel);
        }
        else{
          if (sel().GetNMaxBranches()>1)
            output().FillVectorVar(accum_level, accumLevel, ibranch);
          else
            output().FillVar(accum_level, accumLevel);
        }
      }
    }

    // Reset the categories for the current track
    cat().ResetCurrentCategories();

    // Call the derive classes functions, that also fills the categories
    FillTruthTree(vtx);

    // Fill the truth tree provided the true codes for color drawing
    std::map< std::string, TrackCategoryDefinition* >::iterator its;
    Int_t categ_index = AnalysisAlgorithm::firstCategory;
    for (its=cat().GetCategories().begin();its!=cat().GetCategories().end();its++, categ_index++ ){
      std::string categ_name = its->first;
      TrackCategoryDefinition& categ = *(its->second);
      if (categ.IsMultiType()){
        for (unsigned int i=0;i<categ.GetNTypes();i++)
           output().FillVectorVar(categ_index, (int)cat().CheckCategoryType(categ_name,i),i);
      }
      else output().FillVar(categ_index, cat().GetCode(categ_name));
    }

    // Fill the tree
    output().GetTree(OutputManager::truth)->Fill();
  }


}
