#include "DrawingTools.hxx"
#include "SystematicTools.hxx"
#include <iomanip>
#include <iostream>
#include <fstream>


//********************************************************************
DrawingTools::DrawingTools(const std::string& file, Int_t T2KstyleIndex):DrawingToolsBase(file,T2KstyleIndex){
//********************************************************************

  _treeForSystErrors = NULL;
}

//********************************************************************
DrawingTools::DrawingTools(Experiment& exp, Int_t T2KstyleIndex):DrawingToolsBase(exp.GetFilePath(),T2KstyleIndex){
//********************************************************************

  _treeForSystErrors = NULL;
}

//********************************************************************
//void DrawingTools::PrintSummary(){
//********************************************************************

/*

  std::cout << "   - Avg relative systematic error:  " << _avg_rel_syst_err << std::endl;
  std::cout << "   - Avg relative statistical error: " << _avg_rel_stat_err << std::endl;
  std::cout << "   - Avg relative total error:       " << _avg_rel_total_err << std::endl;
*/
//}



//**************************************************
//void DrawingTools::PrintHisto(const std::string& name, const std::string& categ){
//**************************************************

/*
  std::cout << "-------------------------------------------------------" << std::endl;
  std::cout << "Summary info for histo: " << name << std::endl;
  std::cout << "-------------------------------------------------------" << std::endl;

  if (opt!=0){
    std::map< std::string, std::map< std::string, bool > >::iterator it;
    for (it= GetTrees().begin();it!=GetTrees().end();it++){
      if (!TreeEnabled(it->first)) continue;
      std::cout << "- tree: " << it->first << std::endl;
      PrintSummary(it->first, name);
    }

  }
  else{
    std::cout << "avg systematic  error: " << std::endl; 
    std::map< std::string, std::map< std::string, bool > >::iterator it;
    for (it= GetTrees().begin();it!=GetTrees().end();it++){
      if (!TreeEnabled(it->first)) continue;
      GetCounter(it->first, name).ComputeAvgErrors();
      std::cout << " - " << it->first <<  "\t" <<  GetCounter(it->first, name).GetAvgRelSystError() << std::endl;
    }
    std::cout << "avg statistical error (default):  " << GetCounter("default", name).GetAvgRelStatError() << std::endl; 
    if (CheckHistoConf("all_syst", name,"all")){
      std::cout << "avg statistical error (all syst): " << GetCounter("all_syst", name).GetAvgRelStatError() << std::endl; 
      std::cout << "avg total  error:                 " << GetCounter("all_syst", name).GetAvgRelTotalError() << std::endl; 
    }
  }

  std::cout << "-------------------------------------------------------" << std::endl;
*/
//}




//}

//*********************************************************
std::string DrawingTools::GetCombinedCut(DataSample& sample, const std::string& cut){
//*********************************************************

  std::string cut1 = cut;  
  if (sample.GetCut() !="") cut1 = cut+" && ("+sample.GetCut()+")";
  return cut1;
}

//*********************************************************
void DrawingTools::Project(HistoStack* hs, const std::string& sample_name,DataSample& sample, const std::string& var, int nx, double* xbins, int ny, double* ybins, const std::string& categ,
			   const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm,  bool scale_errors){
//*********************************************************

  std::string opt2 = opt + " EXP";
  DrawingToolsBase::Project(hs, sample_name, sample.GetTree(),var,nx,xbins,ny,ybins,categ,GetCombinedCut(sample,cut),root_opt,opt2,norm,scale_errors);  
}

//*********************************************************
void DrawingTools::Project(HistoStack* hs1, HistoStack* hs2, DataSample* sample1, DataSample* sample2, 
			   const std::string& var, int nx, double* xbins, int ny, double* ybins, const std::string& categ,
			   const std::string& cut,  const std::string& root_opt, const std::string& opt, bool scale_errors, double norm, bool pot_norm){
//*********************************************************

  if (!sample1 || !sample2) return;

  // Get the normalisation factor between them
  norm = GetNormalisationFactor(sample1, sample2, norm, pot_norm, opt);

  // Project the first sample with normalization 1 (or with area normalization) and the second sample normalized to the first
  std::string opt2 = opt + " EXP";
  DrawingToolsBase::Project(hs1,hs2,sample1->GetTree(), sample2->GetTree(), var,nx,xbins,ny,ybins,categ,cut,root_opt,opt2,norm,scale_errors);
}

//*********************************************************
void DrawingTools::Project(HistoStack* hs1, HistoStack* hs2, SampleGroup& sampleGroup, const std::string& mcSampleName,  
                           const std::string& var, int nx, double* xbins, int ny, double* ybins, const std::string& categ,
                           const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm, bool scale_errors){
//*********************************************************

  if (mcSampleName!="all"){
    if (!sampleGroup.HasMCSample(mcSampleName)) return;
  }

  std::string uopt = drawUtils::ToUpper(opt);

  // Project the first sample with normalization 1
  DataSample* sample1 = NULL;
  if (hs1){
    sample1 = sampleGroup.GetDataSample();
    if (sample1)
      //      Project(hs1, "sample1", *sample1,var,nx,xbins,ny,ybins,"all",cut,root_opt,opt+" NOSYS",1   ,scale_errors);
      Project(hs1, "sample1", *sample1,var,nx,xbins,ny,ybins,"all",cut,root_opt,opt,1   ,scale_errors);
  }

  if (hs2){
    std::map< std::string, DataSample*>& mcSamples = sampleGroup.GetMCSamples();
    std::map< std::string, DataSample*>::iterator it;
    for (it = mcSamples.begin(); it != mcSamples.end(); it++) {
      const std::string&  mcSampleName2 = it->first;
      if (mcSampleName2 != mcSampleName && mcSampleName!="all") continue;
      DataSample* sample2 = it->second;
      if (sample2){
        // Get the normalisation factor between data and this MC sample
        // If the input normalization factor is >=0 (default is -1) that factor will be applied even when data is present
        double norm2 = GetNormalisationFactor(sample1, sample2, norm, true, opt);

        // Project the second sample normalized to the first one
        hs2->SetCurrentSystGroup(sample2->GetSystGroup());
        std::string syst_opt = sample2->GetSystOption();
        Project(hs2, "sample2_"+mcSampleName2, *sample2,var,nx,xbins,ny,ybins,categ,cut,root_opt,opt+" "+syst_opt,norm2,scale_errors);
      }
    }
  }
}

//*********************************************************
void DrawingTools::Project(HistoStack* hs1, HistoStack* hs2, Experiment& exp,  const std::string& groupName, const std::string& mcSampleName, 
                           const std::string& var, int nx, double* xbins, int ny, double* ybins, const std::string& categ,
                           const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm, bool scale_errors){
//*********************************************************

  std::string uopt = drawUtils::ToUpper(opt);

  // Loop over SampleGroup's in the experiment
  std::map< std::string, SampleGroup >::iterator it;
  for (it = exp.GetSampleGroups().begin(); it != exp.GetSampleGroups().end(); it++) {
    const std::string& groupName2 = it->first;
    if (groupName2 != groupName && groupName!="all") continue;
    SampleGroup& sampleGroup = it->second;
    // Project the first sample with normalization 1 and the MC samples normalized to the first sample
    Project(hs1, hs2, sampleGroup, mcSampleName, var,nx,xbins,ny,ybins,categ,cut,root_opt,uopt+" NOAREA",norm,scale_errors);
  }

  // normalize by area when requested
  if(drawUtils::CheckOption(uopt,"AREA")){
    if (hs1) hs1->NormalizeByArea(uopt);
    if (hs2){
      if (hs1)
        hs2->NormalizeByArea(uopt,hs1->GetTotal1D()->GetSumOfWeights());
      else
        hs2->NormalizeByArea(uopt);
    }
  }

}

//*********************************************************
void DrawingTools::PrintPurities(DataSample& sample, const std::string& categ,  const std::string& cut, double events_ratio){
//*********************************************************

  DrawingToolsBase::PrintPurities(sample.GetTree(), categ, GetCombinedCut(sample,cut), events_ratio);
}

//*********************************************************
void DrawingTools::PrintPurities(Experiment& exp, const std::string& categ,  const std::string& cut){
//*********************************************************

  // Read the categories from the config tree
  ReadCategories(_config_file);

  if (!cat().HasCategory(categ) || categ =="all" ) {
    std::cout << " ------------------------------------------ " << std::endl;
    std::cout << "  Invalid category " << categ << std::endl;
    std::cout << " ------------------------------------------ " << std::endl;
    return;
  }

  int i=0;

  std::vector<TrackTypeDefinition>::iterator it;

  std::cout << " --------------------------------------------------------" << std::endl;
  std::cout << "  Experiment Purities: " << std::endl;
  std::cout << "  Experiment Category: " << categ << std::endl;
  std::cout << "  Experiment Cut:      " << cut << std::endl;
  std::cout << " --------------------------------------------------------" << std::endl;

  // define stuff to print % on the legend
  std::ostringstream percstr, str_tmp;
  const int ntypes = (const int)cat().GetCategoryTypes(categ).size();
  std::string* cattypes = new std::string[ntypes]; // to initialize with const dimension
  int* catcodes = new int[ntypes]; // to initialize with const dimension
  int* catcolors = new int[ntypes]; // to initialize with const dimension
  int itype=0;

  for (it=cat().GetCategoryTypes(categ).begin();it!=cat().GetCategoryTypes(categ).end();it++, i++){
    std::string type = it->_name;
    std::string code = drawUtils::GetString(it->_code);

    std::string cut2 = categ+"=="+code;

    double dummy1, dummy2, nev1, nev2;
    double frac = GetEff(exp, false, dummy1, dummy2, nev1, nev2, cut2, cut,"","PUR");

    std::cout << std::setw(25) << type << std::setw(10) << frac*100. << " % (" << nev1 << " events)" << std::endl;

    // create categ_temp to print % on the legend
    percstr.str(std::string()); // to clear it
    percstr << std::setprecision(2) << std::fixed << frac*100.; // round to 2 decimal
    str_tmp.str(std::string()); // to clear it
    str_tmp << it->_name << std::setw(8) << percstr.str() << " %"; // tab
    cattypes[itype] = str_tmp.str();
    catcodes[itype] = it->_code;
    catcolors[itype] = it->_color;
    itype++;
  }

  // create categories with % in the name
  bool multi = cat().GetCategory(categ).IsMultiType();
  bool noWarning = true, addNOTRUTH = false, addSAND = false;
  cat().AddCategory(categ+"_withPurities",itype,cattypes,catcodes,catcolors,multi,noWarning,addNOTRUTH,addSAND);

  std::cout << " --------------------------------------------------------" << std::endl;
  std::cout << std::endl;

  return;
}

//*********************************************************
void DrawingTools::Draw(DataSample& sample, const std::string& var, int nx, double xmin, double xmax, 
			const std::string& categ, const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm, bool scale_errors){
//*********************************************************

  norm = GetNormalisationFactor(NULL, &sample, norm, true, opt);
  DrawingToolsBase::Draw(sample.GetTree(), var,nx,xmin,xmax,categ,GetCombinedCut(sample,cut),root_opt,opt,norm,scale_errors);
}

//*********************************************************
void DrawingTools::Draw(DataSample& sample, const std::string& var, int nbins, double* xbins, 
			const std::string& categ, const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm,  bool scale_errors){
//*********************************************************

  norm = GetNormalisationFactor(NULL, &sample, norm, true, opt);
  DrawingToolsBase::Draw(sample.GetTree(), var,nbins,xbins,categ,GetCombinedCut(sample,cut),root_opt,opt,norm,scale_errors);
}

//*********************************************************
void DrawingTools::Draw(DataSample& sample, const std::string& var, int nx, double xmin, double xmax, int ny, double ymin, double ymax, 
			const std::string& categ, const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm){
//*********************************************************

  norm = GetNormalisationFactor(NULL, &sample, norm, true, opt);
  DrawingToolsBase::Draw(sample.GetTree(), var,nx,xmin,xmax,ny,ymin,ymax,categ,GetCombinedCut(sample,cut),root_opt,opt,norm);
}

//*********************************************************
void DrawingTools::Draw(DataSample& sample, const std::string& var, int nx, double* xbins, int ny, double* ybins, 
			const std::string& categ, const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm){
//*********************************************************

  norm = GetNormalisationFactor(NULL, &sample, norm, true, opt);
  DrawingToolsBase::Draw(sample.GetTree(), var,nx,xbins,ny,ybins,categ,GetCombinedCut(sample,cut),root_opt,opt,norm);
}

//*********************************************************
void DrawingTools::DrawEff(DataSample& sample, const std::string& var, int nx, double xmin, double xmax, 
			   const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, const std::string& leg_name){
//*********************************************************

  DrawingToolsBase::DrawEff(sample.GetTree(), var,nx,xmin,xmax,cut1,cut2,root_opt,opt,leg_name);
}

//*********************************************************
void DrawingTools::DrawEff(DataSample& sample, const std::string& var, int nx, double* xbins, 
			   const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, const std::string& leg_name){
//*********************************************************

  DrawingToolsBase::DrawEff(sample.GetTree(), var,nx,xbins,cut1,cut2,root_opt,opt,leg_name);
}

//*********************************************************
void DrawingTools::DrawDoubleEff(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double xmin, double xmax, 
				 const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, const std::string& leg_name){
//*********************************************************

  DrawingToolsBase::DrawDoubleEff(sample1.GetTree(), sample2.GetTree(), var,nx,xmin,xmax,cut1,cut2,root_opt,opt,leg_name);
}

//*********************************************************
void DrawingTools::DrawDoubleEff(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double* xbins, 
				 const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, const std::string& leg_name){
//*********************************************************

  DrawingToolsBase::DrawDoubleEff(sample1.GetTree(), sample2.GetTree(), var,nx,xbins,cut1,cut2,root_opt,opt,leg_name);
}

//*********************************************************
void DrawingTools::DrawEff(Experiment& exp, bool usedata, const std::string& var, int nx, double xmin, double xmax,
                          const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, const std::string& leg_name) {
//*********************************************************
  double xbins[NMAXBINS];
  DrawEff(exp, usedata, var, nx, GetVariableBins(nx, xmin, xmax, xbins), cut1, cut2, root_opt, opt, leg_name);
}

//*********************************************************
void DrawingTools::DrawEff(Experiment& exp, bool usedata, const std::string& var, int nx, double* xbins,
                          const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, const std::string& leg) {
//*********************************************************
  double dummy1, dummy2;
  std::string uopt = drawUtils::ToUpper(opt);
  std::string uroot_opt = drawUtils::ToUpper(root_opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

  TGraphAsymmErrors* eff = GetEff(exp, usedata, var, nx, xbins, dummy1, dummy2, cut1, cut2, uroot_opt, uopt);

  // Draw the efficiency graph
  if (eff)
    DrawingToolsBase::DrawGraph(eff,nx,xbins,uroot_opt,uopt,leg);
}


//*********************************************************
void DrawingTools::DrawEffNew(Experiment& exp, bool usedata, const std::string& var, int nx, double xmin, double xmax,
			      const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt1, const std::string& opt2, 
			      const std::string& leg_name) {
//*********************************************************
  double xbins[NMAXBINS];
  DrawEffNew(exp, usedata, var, nx, GetVariableBins(nx, xmin, xmax, xbins), cut1, cut2, root_opt, opt1, opt2, leg_name);
}

//*********************************************************
void DrawingTools::DrawEffNew(Experiment& exp, bool usedata, const std::string& var, int nx, double* xbins,
			      const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt1, 
			      const std::string& opt2, const std::string& leg) {
//*********************************************************
  double dummy1, dummy2;
  std::string uopt1 = drawUtils::ToUpper(opt1);
  std::string uopt2 = drawUtils::ToUpper(opt2);
  std::string uroot_opt = drawUtils::ToUpper(root_opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt1)) return;

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt2)) return;

  TGraphAsymmErrors* eff = GetEffNew(exp, usedata, var, nx, xbins, dummy1, dummy2, cut1, cut2, uroot_opt, uopt1, uopt2);

  // Draw the efficiency graph
  if (eff)
    DrawingToolsBase::DrawGraph(eff,nx,xbins,uroot_opt,uopt1,leg);
}


//*********************************************************
void DrawingTools::DrawPur(Experiment& exp, const std::string& var, int nx, double xmin, double xmax,
                          const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, const std::string& leg_name) {
//*********************************************************
  double xbins[NMAXBINS];
  DrawPur(exp, var, nx, GetVariableBins(nx, xmin, xmax, xbins), cut1, cut2, root_opt, opt, leg_name);
}

//*********************************************************
void DrawingTools::DrawPur(Experiment& exp, const std::string& var, int nx, double* xbins,
                          const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, const std::string& leg) {
//*********************************************************

  double dummy1, dummy2;
  std::string uopt = drawUtils::ToUpper(opt);
  std::string uroot_opt = drawUtils::ToUpper(root_opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

  TGraphAsymmErrors* eff = GetEff(exp, false, var, nx, xbins, dummy1, dummy2, cut1, cut2, uroot_opt, uopt+" PUR");

  // Draw the efficiency graph
  if (eff)
    DrawingToolsBase::DrawGraph(eff,nx,xbins,uroot_opt,uopt,leg);
}

//*********************************************************
void DrawingTools::DrawSignificance(DataSample& sample, const std::string& var, int nx, double xmin, double xmax, 
				    const std::string& cut1, const std::string& cut2, double norm, double rel_syst, const std::string& root_opt, const std::string& opt, const std::string& leg_name){
//*********************************************************

  DrawingToolsBase::DrawSignificance(sample.GetTree(), var,nx,xmin,xmax,cut1,cut2,norm,rel_syst,root_opt,opt,leg_name);
}

//*********************************************************
void DrawingTools::DrawSignificance(DataSample& sample, const std::string& var, int nx, double* xbins, const std::string& cut1, const std::string& cut2, 
				    double norm, double rel_syst, const std::string& root_opt, const std::string& opt, const std::string& leg_name){
//*********************************************************

  DrawingToolsBase::DrawSignificance(sample.GetTree(), var,nx,xbins,cut1,cut2,norm,rel_syst,root_opt,opt,leg_name);
}

//*********************************************************
void DrawingTools::DrawRatio(DataSample& sample, const std::string& var, int nx, double xmin, double xmax, 
			     const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, const std::string& leg_name){
//*********************************************************

  DrawingToolsBase::DrawRatio(sample.GetTree(), var,nx,xmin,xmax,cut1,cut2,root_opt,opt,leg_name);
}

//*********************************************************
void DrawingTools::PrintEventNumbers(DataSample& sample, const std::string& cut, const std::string& file, int toy_ref){
//*********************************************************

  DrawingToolsBase::PrintEventNumbers(sample.GetTree(), GetCombinedCut(sample,cut), file, toy_ref);
}

//*********************************************************
void DrawingTools::DrawRatio(DataSample& sample, const std::string& var, int nx, double* xbins, 
			     const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, const std::string& leg_name){
//*********************************************************

  DrawingToolsBase::DrawRatio(sample.GetTree(), var,nx,xbins,cut1,cut2,root_opt,opt,leg_name);
}

//**************************************************
void DrawingTools::Draw(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double xmin, double xmax, 
			const std::string& categ, const std::string& cut, const std::string& root_opt, const std::string& opt, double norm, bool scale_errors, bool pot_norm){
//**************************************************

  // 1D comparison between two data samples with different normalisation. Uniform binning
  double xbins[NMAXBINS];
  Draw(sample1,sample2,var,nx,GetVariableBins(nx,xmin,xmax,xbins),categ,cut,root_opt, opt,norm,scale_errors,pot_norm);
}

//**************************************************
void DrawingTools::Draw(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double* xbins, 
			const std::string& categ, const std::string& cut, const std::string& root_opt, const std::string& opt, double norm, bool scale_errors, bool pot_norm){
//**************************************************

  // 1D comparison between two data samples with different normalisation. Variable binning
  int ny=0;
  double *ybins=NULL;
  Draw(sample1,sample2,var,nx,xbins,ny,ybins,categ,cut,root_opt, opt,norm,scale_errors,pot_norm);
}

//**************************************************
void DrawingTools::Draw(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double xmin, double xmax, int ny, double ymin, double ymax, 
			const std::string& categ, const std::string& cut, const std::string& root_opt, const std::string& opt, double norm, bool scale_errors, bool pot_norm){
//**************************************************

  // 2D comparison between two data samples with different normalisation. Uniform binning
  double xbins[NMAXBINS];
  double ybins[NMAXBINS];
  Draw(sample1,sample2,var,nx,GetVariableBins(nx,xmin,xmax,xbins),ny,GetVariableBins(ny,ymin,ymax,ybins),categ,cut,root_opt, opt,norm,scale_errors,pot_norm);
}

//**************************************************
void DrawingTools::Draw(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double* xbins, int ny, double* ybins, 
			const std::string& categ, const std::string& cut, const std::string& root_opt, const std::string& opt, double norm, bool scale_errors, bool pot_norm){
//**************************************************

  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

  // Create empty Histo Stacks
  HistoStack* hs1 = new HistoStack(_title,_titleX,_titleY);
  HistoStack* hs2 = new HistoStack(_title,_titleX,_titleY);
  _saved_histoStacks.push_back(hs1);  
  _saved_histoStacks.push_back(hs2);  

  std::string slevel = GetSameLevel(root_opt);

  // Project the first sample with normalization 1 and the second sample normalized to the first one
  Project(hs1, hs2, &sample1, &sample2, var,nx,xbins,ny,ybins,categ,cut,root_opt,opt,scale_errors,norm,pot_norm);  

  // Draw the Total Stack after all samples have been projected
  DrawingToolsBase::DrawHistoStacks(hs1,hs2,categ,root_opt,opt,1);
}

//**************************************************
void DrawingTools::DrawRatio(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double xmin, double xmax, 
			     const std::string& cut1, const std::string& cut2, double norm, const std::string& root_opt, const std::string& opt, const std::string& leg_name, bool pot_norm){
//**************************************************

  norm = GetNormalisationFactor(&sample1, &sample2, norm, pot_norm, opt);
  DrawingToolsBase::DrawRatio(sample1.GetTree(),sample2.GetTree(),var,nx,xmin,xmax,cut1,cut2,norm,root_opt,opt,leg_name);
}

//**************************************************
void DrawingTools::DrawRatio(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double* xbins, 
			     const std::string& cut1, const std::string& cut2, double norm, const std::string& root_opt, const std::string& opt, const std::string& leg_name, bool pot_norm){
//**************************************************

  norm = GetNormalisationFactor(&sample1, &sample2, norm, pot_norm, opt);
  DrawingToolsBase::DrawRatio(sample1.GetTree(),sample2.GetTree(),var,nx,xbins,cut1,cut2,norm,root_opt,opt,leg_name);
}

//**************************************************
void DrawingTools::DrawRatio(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double xmin, double xmax, 
			     const std::string& cut, double norm, const std::string& root_opt, const std::string& opt, const std::string& leg_name, bool pot_norm){
//**************************************************

  norm = GetNormalisationFactor(&sample1, &sample2, norm, pot_norm, opt);
  DrawingToolsBase::DrawRatio(sample1.GetTree(),sample2.GetTree(),var,nx,xmin,xmax,cut,norm,root_opt,opt,leg_name);
}

//**************************************************
void DrawingTools::DrawRatio(DataSample& sample1, DataSample& sample2, const std::string& var, int nx, double* xbins, 
			     const std::string& cut, double norm, const std::string& root_opt, const std::string& opt, const std::string& leg_name, bool pot_norm){
//**************************************************

  norm = GetNormalisationFactor(&sample1, &sample2, norm, pot_norm, opt);
  DrawingToolsBase::DrawRatio(sample1.GetTree(),sample2.GetTree(),var,nx,xbins,cut,norm,root_opt,opt,leg_name);
}

//*********************************************************
void DrawingTools::DrawToys(DataSample& sample, const std::string& cut, const std::string& root_opt, const std::string& opt, const std::string& leg_name){
//*********************************************************

  DrawingToolsBase::DrawToys(sample.GetTree(), cut, root_opt, opt, leg_name);
}

//*********************************************************
void DrawingTools::DrawToysRatio(DataSample& sample1, DataSample& sample2, const std::string& cut, double norm, 
				    const std::string& root_opt, const std::string& opt, const std::string& leg_name, bool pot_norm){
//*********************************************************

  norm = GetNormalisationFactor(&sample1, &sample2, norm, pot_norm, opt);
  DrawingToolsBase::DrawToysRatio(sample1.GetTree(), sample2.GetTree(), cut, cut, norm, root_opt, opt,leg_name);
}

//*********************************************************
void DrawingTools::DrawToysRatio(DataSample& sample1, DataSample& sample2, const std::string& cut1, const std::string& cut2, double norm, 
				    const std::string& root_opt, const std::string& opt, const std::string& leg_name, bool pot_norm){
//*********************************************************

  norm = GetNormalisationFactor(&sample1, &sample2, norm, pot_norm);
  DrawingToolsBase::DrawToysRatio(sample1.GetTree(), sample2.GetTree(), cut1, cut2, norm, root_opt, opt, leg_name);
}


//**************************************************
void DrawingTools::DrawRatioVSCut(DataSample& sample1, DataSample& sample2, const std::string& precut, int first_cut, int last_cut,
                                  const std::string& root_opt, const std::string& opt, const std::string& leg, double norm, bool pot_norm){
//**************************************************

  DrawRatioVSCut(sample1,sample2,0,precut,first_cut,last_cut,root_opt,opt,leg,norm,pot_norm);
}

//**************************************************
void DrawingTools::DrawRatioVSCut(DataSample& sample1, DataSample& sample2, int ibranch, const std::string& precut, int first_cut, int last_cut,
                                  const std::string& root_opt, const std::string& opt, const std::string& leg, double norm, bool pot_norm){
//**************************************************

  // if selection exists
  if (sel().GetSelection(ibranch,true)) {
    int isel = ibranch;
    std::cout << "Drawing for selection " << isel << ", branch 0" << std::endl;
    return DrawRatioVSCut(sample1,sample2,isel,0,precut,first_cut,last_cut,root_opt,opt,leg,norm,pot_norm);
  }

  DrawRatioVSCut(sample1,sample2,0,ibranch,precut,first_cut,last_cut,root_opt,opt,leg,norm,pot_norm);
}

//**************************************************
void DrawingTools::DrawRatioVSCut(DataSample& sample1, DataSample& sample2, int isel, int ibranch, const std::string& precut, int first_cut, int last_cut,
                                  const std::string& root_opt, const std::string& opt, const std::string& leg, double norm, bool pot_norm){
//**************************************************

  ReadOther(sample1.GetTree("config"));
  norm = GetNormalisationFactor(&sample1, &sample2, norm, pot_norm, opt);
  DrawingToolsBase::DrawRatioVSCut(sample1.GetTree(),sample2.GetTree(),isel, ibranch,precut,first_cut,last_cut,root_opt,opt,leg,norm);
}

//*********************************************************
void DrawingTools::DrawEffPurVSCut(DataSample& sample, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
				   const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  DrawEffPurVSCut(sample,0,signal,precut,first_cut,last_cut, root_opt, opt, leg);
}

//*********************************************************
void DrawingTools::DrawEffPurVSCut(DataSample& sample, int branch, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
				   const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  DrawEffPurVSCut(sample,0,branch,signal,precut,first_cut,last_cut, root_opt, opt, leg);
}

//*********************************************************
void DrawingTools::DrawEffPurVSCut(DataSample& sample, int isel, int branch, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
				   const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  DrawEffPurVSCut(sample,isel,branch,signal,precut,first_cut,last_cut, first_cut,last_cut, root_opt, opt, leg);
}

//*********************************************************
void DrawingTools::DrawEffPurVSCut(DataSample& sample, int isel, int branch, const std::string& signal, const std::string& precut, 
				   int first_cut_pur, int last_cut_pur, int first_cut_eff, int last_cut_eff,
				   const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  // Check if selection exists
  if (!sel().GetSelection(isel,true)) return;

  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;


  if (!sample.GetTree("truth")){
    std::cout << "truth tree does not exists. Efficiency cannot be computed !!!" << std::endl;
    return;
  }

  // Read the min accum level to save from the config tree
  ReadOther(sample.GetTree("config"));

  // save the current legend size
  double sizex = _legendSize[0];
  double sizey = _legendSize[1];
  
  // small legend size
  SetLegendSize(0.1,0.1);

  // draw the efficiency
  DrawingToolsBase::DrawEffVSCut(sample.GetTree("truth"),isel,branch,signal,precut,first_cut_eff,last_cut_eff, root_opt,                  opt,"eff "+leg);

  // draw the purity
  DrawingToolsBase::DrawPurVSCut(sample.GetTree()       ,isel,branch,signal,precut,first_cut_pur,last_cut_pur,(root_opt+" same").c_str(),opt,"pur "+leg);

  // go back to previous size
  SetLegendSize(sizex,sizey);
}

//*********************************************************
TH1F* DrawingTools::GetHisto(DataSample& sample,const std::string& name, const std::string& var, int nx, double* xbins, 
			     const std::string& cut, const std::string& root_opt, const std::string& opt, double scale, bool scale_errors){
//*********************************************************

  return GetHisto(sample.GetTree(), name,var,nx,xbins,cut,root_opt,opt,scale,scale_errors);
}

//*********************************************************
TH1F* DrawingTools::GetHisto(HistoStack* hs, TTree* tree,const std::string& name, const std::string& var, int nx, double* xbins, 
			     const std::string& cut, const std::string& root_opt, const std::string& opt, double scale, bool scale_errors, int toy_ref){
//*********************************************************

  // This function overwrides the one of the base class. 
  // If an option (ST, SYS, DIS) is given only the reference toy experiment is plotted. 

  toy_ref=-1;
  //  if (opt.find("ST")!= std::string::npos || opt.find("SYS")!= std::string::npos || opt.find("DIS")!= std::string::npos)
  //    toy_ref = GetToy_Reflysis(tree);

  // Call the base class function (the only thing it changed is the cut)
  return DrawingToolsBase::GetHisto(hs, tree, name,var,nx,xbins,cut,root_opt,opt,scale,scale_errors,toy_ref);
}


//*********************************************************
TH1F* DrawingTools::GetRatioHisto(TTree* tree1,TTree* tree2,const std::string& name, const std::string& var, int nx, double* xbins, 
                                  const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, double norm, double scale, bool scale_errors, int toy_ref){
//*********************************************************

  // This function overwrides the one of the base class. 
  // If an option (ST, SYS, DIS) is given only the reference toy experiment is plotted. 

  toy_ref=-1;
  //  if (opt.find("ST")!= std::string::npos || opt.find("SYS")!= std::string::npos || opt.find("DIS")!= std::string::npos)
  //    toy_ref = GetToy_Reflysis(tree1);
    

  // Call the base class function (the only thing it changed is the cut)
  return DrawingToolsBase::GetRatioHisto(tree1, tree2, name,var,nx,xbins,cut1,cut2,root_opt,opt,norm,scale,scale_errors,toy_ref);
}

//**************************************************
void DrawingTools::FillGraphErrors(HistoStack* hs1, HistoStack* hs2, TGraphAsymmErrors* graph,  const std::string uopt){
//**************************************************

  if (drawUtils::CheckInternalOption(uopt,"NOSYS")) return;
  if (!drawUtils::CheckOption(uopt,"SYS")) return;

  Int_t nx = hs2->GetTotal1D()->GetNbinsX();

  if (hs1){
    const TMatrixD& cov = syst_tools().GetRatioSystematicCov(hs1,hs2, uopt+" RELATIVE");
    // Set the sqrt of the diagonal as error
    for (int i=0;i<nx;i++){
      double err_low  = cov(i,i);
      double err_high = err_low;
      if (!drawUtils::CheckOption(uopt,"NOSTERROR")) {	
	err_low += graph->GetErrorYlow(i)*graph->GetErrorYlow(i);
	err_high += graph->GetErrorYhigh(i)*graph->GetErrorYhigh(i);
      }
      graph->SetPointEYlow(i,sqrt(err_low));
      graph->SetPointEYhigh(i,sqrt(err_high));
    }
  }
  else{
    const TMatrixD& cov = syst_tools().GetSystematicCov(hs2, uopt);
    // Set the sqrt of the diagonal as error
    for (int i=0;i<nx;i++){
      Float_t err_low  = cov(i,i);
      Float_t err_high = err_low;
      err_low += graph->GetErrorXlow(i+1)*graph->GetErrorXlow(i+1);
      err_high += graph->GetErrorXhigh(i+1)*graph->GetErrorXhigh(i+1);
      graph->SetPointEXlow(i+1,sqrt(err_low));
      graph->SetPointEXhigh(i+1,sqrt(err_high));
    }
  }
}

//**************************************************
void DrawingTools::FillHistoErrors(HistoStack* hs1, HistoStack* hs2, TH1F* histo, const std::string uopt){
//**************************************************

  if (drawUtils::CheckInternalOption(uopt,"NOSYS")) return;
  if (!drawUtils::CheckOption(uopt,"SYS")) return;

  Int_t nx = hs2->GetTotal1D()->GetNbinsX();

  if (hs1){
    const TMatrixD& cov = syst_tools().GetRatioSystematicCov(hs1,hs2, uopt+" RELATIVE");
    // Set the sqrt of the diagonal as error
    for (int i=0;i<nx;i++){
      double err  = cov(i,i);
      if (!drawUtils::CheckOption(uopt,"NOSTERROR")) {	
        err+= histo->GetBinError(i+1)*histo->GetBinError(i+1);
      }
      histo->SetBinError(i+1,sqrt(err));
    }
  }
  else{
    const TMatrixD& cov = syst_tools().GetSystematicCov(hs2, uopt);
    // Set the sqrt of the diagonal as error
    for (int i=0;i<nx;i++)
      histo->SetBinError(i+1,sqrt(cov(i,i)));
  }

}

//*********************************************************
void DrawingTools::FillHistoErrors(HistoStack* hs1, HistoStack* hs2, TTree* tree1, TTree* tree2, const std::string& name, const std::string& var, int nx, double* xbins, 
				   const std::string& cut1, const std::string& cut2, const std::string& opt, double norm, TH1F* hstat, TH1F*& hsyst){
//*********************************************************

  (void)hstat;

  // This function overwrides the one of the base class
  // It puts the appropriate errors into histrogram depending on the plotting options (opt)
  // ST: plot only stat errors
  // ST SYS: plot stat and syst errors in quadrature
  // SYS: plot only systematic errors
  // DIS: the error bar is the dispersion 
  // ST DIS: plot stat error and dispersion in quadrature
  if(syst_tools().errdebug)  std::cout<<"FillHistoErrors \n=============== "<<std::endl;

  std::string uopt = drawUtils::ToUpper(opt);
  hsyst = NULL;
  if (!drawUtils::CheckInternalOption(uopt,"NOSYS") && drawUtils::CheckOption(uopt,"SYS")){
    // Compute the relative systematic errors
    if(syst_tools().errdebug)   std::cout<<" compute systematic errors cut1 "<<cut1<<" cut2 "<<cut2<<" name : "<<name<<std::endl;
    hsyst = GetHistoWithSystErrors(hs1, hs2, tree1, tree2, name, var, nx, xbins, cut1, cut2, uopt+ " RELATIVE", norm);
  }
}

//*********************************************************
TH1F* DrawingTools::GetHistoWithSystErrors(HistoStack* hs1, HistoStack* hs2, TTree* tree1, TTree* tree2, const std::string& name, const std::string& var, int nx, double* xbins, 
                                           const std::string& cut1, const std::string& cut2, const std::string& opt, double norm){
//*********************************************************
  if(syst_tools().errdebug)    std::cout<<"GetHistoWithSystErrors \n======================== "<<std::endl;

  (void)norm;

  // if specified (option "SYS") add systematics errors in quadrature
  // option ST  = statistics only
  // option SYS = systematics only
  // option SYS ST = systematics + statistical errors
  
  std::string uopt = drawUtils::ToUpper(opt);
  // No systematics in those cases
  //  if (drawUtils::GetNToys(tree1)==1 || (drawUtils::CheckOption(uopt,"SYS")  && drawUtils::CheckOption(uopt,"DIS"))){
  //  if (!drawUtils::CheckOption(uopt,"SYS")  && !drawUtils::CheckOption(uopt,"DIS")){
  if (!drawUtils::CheckOption(uopt,"SYS")){
    return NULL;
  }

  // Create histo to store systematics
  TH1F *hsyst = new TH1F(GetUniqueName("hsyst_"+name).c_str(),"hsyst",nx,xbins);
  _saved_histos.push_back(hsyst);

  TTree* tree_syst=NULL;
  if (_treeForSystErrors)
    tree_syst = _treeForSystErrors;
  else
    tree_syst = tree2;
    
  // compute the covariance matrix using the Systematics Tools singleton

  //    const TMatrixD& cov = syst_tools().GetSystematicCov(tree_syst, var, nx, xbins, cut1, drawUtils::GetNToys(tree_syst), uopt);
  syst_tools().UpdateSystematicCov(hs2, tree_syst, var, nx, xbins, cut2, drawUtils::GetNToys(tree_syst), uopt);
  if (tree1){
    syst_tools().UpdateSystematicCov(hs1, tree1, var, nx, xbins, cut1, drawUtils::GetNToys(tree1), uopt);
    const TMatrixD& cov = syst_tools().GetRatioSystematicCov(hs1,hs2, uopt);
    // Set the sqrt of the diagonal as error
    for (int i=0;i<nx;i++)
      hsyst->SetBinError(i+1,sqrt(cov(i,i)));
  }
  else{
    const TMatrixD& cov = syst_tools().GetSystematicCov(hs2, uopt);
    // Set the sqrt of the diagonal as error
    for (int i=0;i<nx;i++)
      hsyst->SetBinError(i+1,sqrt(cov(i,i)));
  }
  if(syst_tools().errdebug)    std::cout<<" =>set to hall, the systematic error bars "<<std::endl;
  return hsyst;    
}

//*********************************************************
void DrawingTools::UpdateSystInfo(HistoStack* hs1, HistoStack* hs2, TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins, 
                                  const std::string& cut1, const std::string& cut2, const std::string& opt, double norm){
//*********************************************************

  if(syst_tools().errdebug)    std::cout<<"UpdateSystInfo \n======================== "<<std::endl;

  (void)norm;

  // if specified (option "SYS") add systematics errors in quadrature
  // option ST  = statistics only
  // option SYS = systematics only
  // option SYS ST = systematics + statistical errors
  
  std::string uopt = drawUtils::ToUpper(opt);
  // No systematics in those cases
  //  if (drawUtils::GetNToys(tree1)==1 || (drawUtils::CheckOption(uopt,"SYS")  && drawUtils::CheckOption(uopt,"DIS"))){
  //  if ((!drawUtils::CheckOption(uopt,"SYS")  && !drawUtils::CheckOption(uopt,"DIS"))){
  if (!drawUtils::CheckOption(uopt,"SYS")){
    return;
  }

  TTree* tree_syst=NULL;
  if (_treeForSystErrors)
    tree_syst = _treeForSystErrors;
  else
    tree_syst = tree2;
    
  // update syst related histograms in the stacks using the Systematics Tools singleton

  //    const TMatrixD& cov = syst_tools().GetSystematicCov(tree_syst, var, nx, xbins, cut1, drawUtils::GetNToys(tree_syst), uopt);
  syst_tools().UpdateSystematicCov(hs2, tree_syst, var, nx, xbins, cut2, drawUtils::GetNToys(tree_syst), uopt);
  
  if(syst_tools().errdebug)    std::cout<<" updated information for stack 2 "<<std::endl;
  
  if (tree1){
    syst_tools().UpdateSystematicCov(hs1, tree1, var, nx, xbins, cut1, drawUtils::GetNToys(tree1), uopt);
    if(syst_tools().errdebug)    std::cout<<" updated information for stack 1 "<<std::endl;
  }
  
  return;    

  
}

//*********************************************************
void DrawingTools::DumpPOT(DataSample& sample) {
//*********************************************************

  sample.DumpPOT();
}

//*********************************************************
double DrawingTools::GetGoodPOT(DataSample& sample) {
//*********************************************************

  return sample.GetPOT();
}

//*********************************************************
int DrawingTools::GetGoodSpills(DataSample& sample) {
//*********************************************************

  return sample.GetNSpills();
}

//*********************************************************
void DrawingTools::DumpPOT(Experiment& exp, const std::string& samplegroup_name) {
//*********************************************************

  exp.DumpPOT(samplegroup_name);
}

//*********************************************************
double DrawingTools::GetPOTRatio(DataSample& sample1, DataSample& sample2, double POTsample1_byhand) {
//*********************************************************

  // so we must update it's values for the sample1 and sample2.


  // Three options
  // 1. sample1 and sample2 have standard POT values from the original files
  // 2. either sample1 or sample2 have POT set by hand when creating the samples
  // 3. there is no sample1 and sample2 is normalised to a POT value pass by argument to the drawing method (POTsample1p)

  double POTsample1 = POTsample1_byhand;
  if (POTsample1<0){
    POTsample1 = sample1.GetNorm();
    if (POTsample1==0){
      POTsample1 = sample1.GetPOT();      
    }
  }

  double POTsample2 = sample2.GetNorm();
  if (POTsample2==0 || POTsample2==1){

    // TODO: The sand muon factor. To be moved somewhere else
    double sandFactor = 1;
#if !VERSION_HAS_OFFICIAL_POT
    if (POTsample2==1)
      sandFactor = 2.5e17;
#endif

    POTsample2 = sandFactor*sample2.GetPOT();
  }

  double ratio;

  if (POTsample2 == 0) {
    std::cout << "Warning: denominator in POT ratio is zero! Setting ratio to 1." << std::endl;
    ratio = 1.;
  } else if (POTsample1 == 0) {
    std::cout << "Warning: numerator in POT ratio is zero! Setting ratio to 1." << std::endl;
    ratio = 1.;
  } else {
    ratio = POTsample1 / POTsample2;
  }

  return ratio;
}

//*********************************************************
double DrawingTools::GetPOTRatio(Experiment& exp) {
//*********************************************************
  return exp.GetOverallPOTRatio();
}

//*********************************************************
double DrawingTools::GetNormalisationFactor(DataSample* sample1,DataSample* sample2, double norm, bool pot_norm, const std::string& opt) {
//*********************************************************

  (void) pot_norm;

  std::string uopt = drawUtils::ToUpper(opt);

  // By default POT normalization is used unless a valid normalization factor is given  
  if (norm<=0 && sample1 && sample2){
    norm = GetPOTRatio(*sample1, *sample2);
  }
  // When there is no sample1, norm>0 and the POTNORM option is given, the second sample is normalized to the POT indicated by norm
  else if (norm>0 && sample2 && drawUtils::CheckOption(uopt,"POTNORM")){
    norm = GetPOTRatio(*sample2, *sample2, norm);  // sample1 may not exist, thus we give sample2 as dummy first argument
  }
  // otherwise, if the factor is still negative  don't normalize
  else if (norm<=0)
    norm=1;
  

  // Otherwise the normalization factor is used to directly scale sample2 regardless of its POT
  return norm;
}

//*********************************************************
void DrawingTools::Draw(Experiment& exp, const std::string& var, int nx, double xmin, double xmax, 
			const std::string& categ, const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm, bool scale_errors){
//*********************************************************

  double xbins[NMAXBINS];
  Draw(exp,var,nx,GetVariableBins(nx,xmin,xmax,xbins),categ,cut,root_opt,opt,norm,scale_errors);
}

//*********************************************************
void DrawingTools::Draw(Experiment& exp, const std::string& var, int nx, double xmin, double xmax, int ny, double ymin, double ymax, 
			const std::string& categ, const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm, bool scale_errors){
//*********************************************************

  double xbins[NMAXBINS];
  double ybins[NMAXBINS];
  Draw(exp,var,nx,GetVariableBins(nx,xmin,xmax,xbins),ny,GetVariableBins(ny,ymin,ymax,ybins),categ,cut,root_opt,opt,norm,scale_errors);
}

//*********************************************************
void DrawingTools::Draw(Experiment& exp, const std::string& var, int nx, double* xbins,
			const std::string& categ, const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm, bool scale_errors){
//*********************************************************

  double ybins[NMAXBINS];
  Draw(exp,var,nx,xbins,0,ybins,categ,cut,root_opt,opt,norm,scale_errors);
}

//*********************************************************
void DrawingTools::Draw(Experiment& exp, const std::string& var, int nx, double* xbins, int ny, double* ybins, 
			const std::string& categ, const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm, bool scale_errors){
//*********************************************************

  Draw(exp,"all","all",var,nx,xbins,ny,ybins,categ,cut,root_opt,opt,norm,scale_errors);
}

//*********************************************************
void DrawingTools::Draw(Experiment& exp, const std::string& groupName, const std::string& var, int nx, double xmin, double xmax, 
			const std::string& categ, const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm, bool scale_errors){
//*********************************************************

  double xbins[NMAXBINS];
  Draw(exp,groupName,var,nx,GetVariableBins(nx,xmin,xmax,xbins),categ,cut,root_opt,opt,norm,scale_errors);
}

//*********************************************************
void DrawingTools::Draw(Experiment& exp, const std::string& groupName, const std::string& var, int nx, double xmin, double xmax, int ny, double ymin, double ymax, 
			const std::string& categ, const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm, bool scale_errors){
//*********************************************************

  double xbins[NMAXBINS];
  double ybins[NMAXBINS];
  Draw(exp,groupName,var,nx,GetVariableBins(nx,xmin,xmax,xbins),ny,GetVariableBins(ny,ymin,ymax,ybins),categ,cut,root_opt,opt,norm,scale_errors);
}

//*********************************************************
void DrawingTools::Draw(Experiment& exp, const std::string& groupName, const std::string& var, int nx, double* xbins,
			const std::string& categ, const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm, bool scale_errors){
//*********************************************************

  double ybins[NMAXBINS];
  Draw(exp,groupName,var,nx,xbins,0,ybins,categ,cut,root_opt,opt,norm,scale_errors);
}

//*********************************************************
void DrawingTools::Draw(Experiment& exp, const std::string& groupName, const std::string& var, int nx, double* xbins, int ny, double* ybins, 
			const std::string& categ, const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm, bool scale_errors){
//*********************************************************

  Draw(exp,groupName,"all",var,nx,xbins,ny,ybins,categ,cut,root_opt,opt,norm,scale_errors);
}

//*********************************************************
void DrawingTools::Draw(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double xmin, double xmax, 
			const std::string& categ, const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm, bool scale_errors){
//*********************************************************

  double xbins[NMAXBINS];
  Draw(exp,groupName,mcSampleName,var,nx,GetVariableBins(nx,xmin,xmax,xbins),categ,cut,root_opt,opt,norm,scale_errors);
}

//*********************************************************
void DrawingTools::Draw(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double xmin, double xmax, int ny, double ymin, double ymax, 
			const std::string& categ, const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm, bool scale_errors){
//*********************************************************

  double xbins[NMAXBINS];
  double ybins[NMAXBINS];
  Draw(exp,groupName,mcSampleName,var,nx,GetVariableBins(nx,xmin,xmax,xbins),ny,GetVariableBins(ny,ymin,ymax,ybins),categ,cut,root_opt,opt,norm,scale_errors);
}

//*********************************************************
void DrawingTools::Draw(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double* xbins,
			const std::string& categ, const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm, bool scale_errors){
//*********************************************************

  double ybins[NMAXBINS];
  Draw(exp,groupName,mcSampleName,var,nx,xbins,0,ybins,categ,cut,root_opt,opt,norm,scale_errors);
}

//*********************************************************
void DrawingTools::Draw(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double* xbins, int ny, double* ybins, 
                        const std::string& categ, const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm, bool scale_errors){
//*********************************************************

  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;


  if (groupName!="all"){
    if (!exp.HasSampleGroup(groupName)) return;
  }

  // Print purities when requested, and % on the legend
  std::string categ2 = categ;
  if (drawUtils::CheckOption(uopt,"PUR") && categ!="all"){
    std::string cut_range = AddRangeCut(var,nx,xbins,ny,ybins,cut,uopt);
    // Print purities when requested, and create categories with % on the name
    PrintPurities(exp, categ, cut_range);
    categ2 += "_withPurities";
  }

  // Create empty Histo Stacks
  HistoStack* hs1 = NULL;
  HistoStack* hs2 = NULL;
  hs1 = new HistoStack(_title,_titleX,_titleY);
  hs2 = new HistoStack(_title,_titleX,_titleY);
  _saved_histoStacks.push_back(hs1);
  _saved_histoStacks.push_back(hs2);

  std::string slevel = GetSameLevel(root_opt);

  // Project the Experiment into the HistoStacks
  Project(hs1, hs2, exp, groupName, mcSampleName, var,nx,xbins,ny,ybins,categ2,cut,root_opt,uopt,norm,scale_errors);

  // Draw the Total Stack after all samples have been projected
  DrawingToolsBase::DrawHistoStacks(hs1,hs2,categ2,root_opt,opt,1);

}


//*********************************************************
void DrawingTools::DrawRatio(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double* xbins, 
			     const std::string& cut,  const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  double ybins[NMAXBINS];
  DrawRatio(exp,groupName,mcSampleName,var,nx,xbins,0,ybins,cut,root_opt,opt,leg);
}

//*********************************************************
void DrawingTools::DrawRatio(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double xmin, double xmax,  
			     const std::string& cut,  const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  double xbins[NMAXBINS];
  double ybins[NMAXBINS];
  DrawRatio(exp,groupName,mcSampleName,var,nx,GetVariableBins(nx,xmin,xmax,xbins),0,ybins,cut,root_opt,opt,leg);
}

//*********************************************************
void DrawingTools::DrawRatio(Experiment& exp, const std::string& var, int nx, double* xbins, 
			     const std::string& cut,  const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  DrawRatio(exp,"all","all",var,nx,xbins,cut,root_opt,opt,leg);
}

//*********************************************************
void DrawingTools::DrawRatio(Experiment& exp, const std::string& var, int nx, double xmin, double xmax, 
			     const std::string& cut,  const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  DrawRatio(exp,"all","all",var,nx,xmin,xmax,cut,root_opt,opt,leg);
}

//*********************************************************
void DrawingTools::DrawRatio(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double* xbins, int ny, double* ybins, 
			     const std::string& cut,  const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  if (groupName!="all"){
    if (!exp.HasSampleGroup(groupName)) return;
  }

  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

  // Create empty Histo Stacks
  HistoStack* hs1 = new HistoStack(_title,_titleX,_titleY);
  HistoStack* hs2 = new HistoStack(_title,_titleX,_titleY);
  _saved_histoStacks.push_back(hs1);  
  _saved_histoStacks.push_back(hs2);  

  // Project the Experiment into the HistoStacks
  Project(hs1, hs2, exp, groupName, mcSampleName, var,nx,xbins,ny,ybins,"all",cut,root_opt,uopt,1.,true);  

  // Draw the HistoStacks
  DrawRatioHistoStacks(hs1,hs2,root_opt,uopt,0.,leg);


  // get the ratio histogram with proper errors
  //  TH1F* ratio = GetRatioHisto(hs1,hs2, uopt);

  // Draw the ratio histogram
  //  DrawRatio(ratio,root_opt,uopt,leg);
}

//*********************************************************
void DrawingTools::AnalysisResults(Experiment& exp, const std::string& cut,  const std::string& opt, const std::string& categ){
//*********************************************************

  AnalysisResults(exp,"all","all",cut,opt,categ);
}

//*********************************************************
void DrawingTools::AnalysisResults(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& cut,  const std::string& opt, const std::string& categ){
//*********************************************************

  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;


  // Create empty Histo Stacks
  HistoStack* hs1 = NULL;
  HistoStack* hs2 = NULL;

  if (!drawUtils::CheckOption(uopt,"NODATA")){
    hs1 = new HistoStack(_title,_titleX,_titleY);
    _saved_histoStacks.push_back(hs1);  
  }
  if (!drawUtils::CheckOption(uopt,"NOMC")){
    hs2 = new HistoStack(_title,_titleX,_titleY);
    _saved_histoStacks.push_back(hs2);  
  }

  double xbins[2]={0.,1.};
  double *ybins = NULL;
  
  // Project the Experiment into the HistoStack
  Project(hs1,      hs2,      exp, groupName, mcSampleName, "0.5",1,xbins,0,ybins,"all",cut,"",opt,1.,true);  

  double N1 = hs1->GetTotal1D()->GetBinContent(1);
  double N2 = hs2->GetTotal1D()->GetBinContent(1);
  double est1 = hs1->GetTotal1D()->GetBinError(1);
  double est2 = hs2->GetTotal1D()->GetBinError(1);
  double esys2 = 0;
  if (hs2->GetTotalSyst1D()){
    est2  = hs2->GetTotalStat1D()->GetBinError(1);
    esys2 = hs2->GetTotalSyst1D()->GetBinError(1);
  }
    

  // compute data/MC ratio and its error
  float ratio = N1/N2;
  float ratio_stat = sqrt(pow(est1/N2,2)+ pow(N1/(N2*N2)*est2,2));
  float ratio_syst = N1/(N2*N2)*esys2;
			    
  std::cout << " --------------------------------------------------------" << std::endl;
  std::cout << " Analysis results for " << cut << std::endl;
  std::cout << " --------------------------------------------------------" << std::endl;
  char out1[256];  
  char out2[256];  
  char out3[256];  
  char out4[256];  
  sprintf(out1,"%-10s: %-12s %-12s %-12s", "sample","events", "stat error", "syst error");
  sprintf(out2,"%-10s: %-12.0f %-12.0f %-12.0f", "data", N1, est1, 0.);
  sprintf(out3,"%-10s: %-12.0f %-12.0f %-12.0f", "mc",   N2, est2, esys2);
  sprintf(out4,"%-10s: %-12.3f %-12.3f %-12.4f", "ratio",ratio, ratio_stat, ratio_syst);  
  std::cout << out1 << std::endl;
  std::cout << " --------------------------------------------------------" << std::endl;
  std::cout << out2 << std::endl;
  std::cout << out3 << std::endl;
  std::cout << out4 << std::endl;

  // Print purities when requested
  if (drawUtils::CheckOption(uopt,"PUR") && categ!="all")
    PrintPurities(exp,categ,cut);  
}

//**************************************************
void DrawingTools::DrawEventsVSCut(Experiment& exp, const std::string& cut_norm, int first_cut, int last_cut, 
				   const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  DrawEventsVSCut(exp,0,cut_norm,first_cut,last_cut,root_opt,opt,leg);
}

//**************************************************
void DrawingTools::DrawEventsVSCut(Experiment& exp, int branch, const std::string& cut_norm, int first_cut, int last_cut, 
				   const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  DrawEventsVSCut(exp,0,branch,cut_norm,first_cut,last_cut,root_opt,opt,leg);
}

//**************************************************
void DrawingTools::DrawEventsVSCut(Experiment& exp, int isel, int branch, const std::string& cut_norm, int first_cut, int last_cut, 
				   const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  // Read the steps from the config tree
  //  ReadSteps(_config_file);

  // Check if selection exists
  if (!sel().GetSelection(isel,true)) return;

  std::string slevel = GetSameLevel(root_opt);
  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;


  // Get the event vs cuts for both data and MC
  TH1F* hdata;
  TH1F* hmc ;
  GetEventsVSCut(exp,"events",cut_norm,isel,branch,first_cut, last_cut,root_opt,uopt,hdata,hmc);


  // Create the legend
  if (slevel=="0" && (leg!="" || (hdata && hmc))){
    // save the current legend size
    double sizex = _legendSize[0];
    double sizey = _legendSize[1];
    
    // small legend size
    SetLegendSize(0.2,0.2);

    CreateLegend();

    // go back to previous size
    SetLegendSize(sizex,sizey);
  }

  // Set cut names
  TH1F* hall;
  if (hdata) hall = hdata;
  else hall = hmc;

  Int_t cut_offset = 0;
  // First bin corresponds to no cut
  if (first_cut ==-1){ 
    hall->GetXaxis()->SetBinLabel(1, "NO CUT");
    cut_offset = 1;
  }

  std::vector<StepBase*> cuts = sel().GetSelection(isel)->GetCutsInBranch(branch);
  // Start from second bin since first corresponds to no cut
  for (int i=cut_offset;i<hall->GetNbinsX();i++ ){
    int icut = first_cut+i;    
    hall->GetXaxis()->SetBinLabel(i+1, cuts[icut]->Title().c_str());
  }

  // No error in X
  gStyle->SetErrorX(0.0001);

  hall->SetMarkerStyle(21);

  // Draw the data and MC histos
  if (hdata)
    DrawHisto(hdata, _auto_colors[_same_level], _line_width, _auto_colors[_same_level], _fill_style, "pl e"+root_opt, uopt+" NOSTAT NOLEG");
  
  if (hmc){
    if (hdata){
      slevel = GetSameLevel("same");
      DrawHisto(hmc, _auto_colors[_same_level], _line_width, _auto_colors[_same_level], _fill_style, "pl e same "+root_opt, uopt+" NOSTAT NOLEG");
    }
    else
      DrawHisto(hmc, _auto_colors[_same_level], _line_width, _auto_colors[_same_level], _fill_style, "pl e "+root_opt, uopt+" NOSTAT NOLEG");
  }

  hall->GetXaxis()->SetTitle("");
  hall->GetYaxis()->SetTitle("# events");

  // Add an entry to the legend if requested
  if (!drawUtils::CheckOption(uopt,"NOLEG")){
    if (leg!="" || (hdata && hmc)){
      if (hdata  && !hmc) _legends.back()->AddEntry( hdata, leg.c_str(),"LE1P");
      if (!hdata &&  hmc) _legends.back()->AddEntry( hmc,   leg.c_str(),"LE1P");
      if (hdata && hmc){
	_legends.back()->AddEntry( hdata, ("data "+leg).c_str(),"LE1P");
	_legends.back()->AddEntry( hmc,   ("MC   "+leg).c_str(),"LE1P");
      }
      _legends.back()->Draw();
    }
  }
  if (!drawUtils::CheckOption(uopt,"NODRAW"))
    gPad->Update();

  // reset error in X
  gStyle->SetErrorX();


  
  // Print numbers on the screen

  char out1[256];  
  char out2[256];  

  std::cout << " ------------- Events vs cut ---------------------------------- " << std::endl;
  for (int i=0;i<hall->GetNbinsX();i++ ){
    int icut = first_cut+i;    
    std::string cut_name="";
    if (icut==-1) 
      cut_name = "NO CUT";
    else
      cut_name = cuts[icut]->Title();      

    if (hdata && hmc){
      sprintf(out1,"%3s: %-25s %-10s %-10s", "#", "cut", "data", "MC");
      sprintf(out2,"%3d: %-25s %-10.2f %-10.2f", icut, cut_name.c_str(), hdata->GetBinContent(i+1), hmc->GetBinContent(i+1));
    }
    else if (hdata){
      sprintf(out1,"%3s: %-25s %-10s", "#", "cut", "data");
      sprintf(out2,"%3d: %-25s %-10.2f", icut, cut_name.c_str(), hdata->GetBinContent(i+1));      
    }
    else if (hmc){
      sprintf(out1,"%3s: %-25s %-10s", "#", "cut", "MC");
      sprintf(out2,"%3d: %-25s %-10.2f", icut, cut_name.c_str(), hmc->GetBinContent(i+1));      
    }
    
    if (i==0) std::cout << out1 << std::endl;
    std::cout << out2 << std::endl;
  }
}

//**************************************************
void DrawingTools::DrawRatioVSCut(Experiment& exp, const std::string& precut, int first_cut, int last_cut,
				  const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  DrawRatioVSCut(exp,0,precut,first_cut,last_cut,root_opt,opt,leg);
}

//**************************************************
void DrawingTools::DrawRatioVSCut(Experiment& exp, int branch, const std::string& precut, int first_cut, int last_cut,
				  const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  DrawRatioVSCut(exp,0,branch,precut,first_cut,last_cut,root_opt,opt,leg);
}

//**************************************************
void DrawingTools::DrawRatioVSCut(Experiment& exp, int isel, int branch, const std::string& precut, int first_cut, int last_cut,
				  const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  // Check if selection exists
  if (!sel().GetSelection(isel,true)) return;

  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

  std::string slevel = GetSameLevel(root_opt);
  if (slevel=="0" && leg!="")
    CreateLegend();

  TH1F* hdata;
  TH1F* hmc;
  Int_t first_cut2=-1;
  GetEventsVSCut(exp,"ratio",precut,isel,branch,first_cut2, last_cut,root_opt,uopt,hdata,hmc);

  if (!hdata || !hmc){
    std::cout << "Cannot compute ratio because either data or MC histos are empty" << std::endl;
    return;
  }

  std::cout << "----- Data/MC ratio values -----------" << std::endl;
  DrawingToolsBase::DrawRatioVSCut(hdata,hmc,isel,branch,first_cut,root_opt,uopt,leg);
}

//**************************************************
void DrawingTools::DrawPurVSCut(Experiment& exp, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
				const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  DrawPurVSCut(exp,0,signal,precut,first_cut,last_cut,root_opt, opt, leg);
}

//**************************************************
void DrawingTools::DrawPurVSCut(Experiment& exp, int branch, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
				const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  DrawPurVSCut(exp,0,branch,signal,precut,first_cut,last_cut,root_opt, opt, leg);
}

//**************************************************
void DrawingTools::DrawPurVSCut(Experiment& exp, int isel, int branch, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
				const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  // Check if selection exists
  if (!sel().GetSelection(isel,true)) return;

  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

  std::string slevel = GetSameLevel(root_opt);
  if (slevel=="0" && leg!="")
    CreateLegend();

  std::string numer = "";
  if (signal == "") {
    numer = precut;
  } else if (precut == "") {
    numer = signal;
  } else {
    numer = signal+"&&"+precut;
  }

  // Histogram for cut1+cut2 (numerator)
  TH1F* hdata;
  TH1F* hmc1, *hmc2 ;
  Int_t first_cut2=-1;
  GetEventsVSCut(exp,"pur1",numer,isel,branch,first_cut2, last_cut,root_opt,uopt,hdata,hmc1);

  // Histogram for cut2 (denominator)
  GetEventsVSCut(exp,"pur2",precut,isel,branch,first_cut2, last_cut,root_opt,uopt,hdata,hmc2);

  if (!hmc1 || !hmc2){
    std::cout << "Cannot compute purity because the experiment does not have any MC sample" << std::endl;
    return;
  }

  std::cout << "----- Purity values -----------" << std::endl;
  DrawingToolsBase::DrawRatioVSCut(hmc1,hmc2,isel,branch,first_cut,root_opt,uopt+ " EFF",leg);
}

//**************************************************
void DrawingTools::DrawEffVSCut(Experiment& exp, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
				const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  DrawEffVSCut(exp,0,signal,precut,first_cut,last_cut,root_opt, opt, leg);
}

//**************************************************
void DrawingTools::DrawEffVSCut(Experiment& exp, int branch, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
				const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  DrawEffVSCut(exp,0,branch,signal,precut,first_cut,last_cut,root_opt, opt, leg);
}

//**************************************************
void DrawingTools::DrawEffVSCut(Experiment& exp, int isel, int branch, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
				const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  // Check if selection exists
  if (!sel().GetSelection(isel,true)) return;

  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

  std::string slevel = GetSameLevel(root_opt);
  if (slevel=="0" && leg!="")
    CreateLegend();

  std::string numer = "";
  if (signal == "") {
    numer = precut;
  } else if (precut == "") {
    numer = signal;
  } else {
    numer = signal+"&&"+precut;
  }

  // Get the current tree name
  std::string tree_name = exp.GetCurrentTree();

  // use the "truth" tree for efficiencies
  exp.SetCurrentTree("truth");   

  // Histogram for cut1+cut2 (numerator)
  TH1F* hdata;
  TH1F* hmc1;
  Int_t first_cut2=-1;
  GetEventsVSCut(exp,"eff1",numer,isel,branch,first_cut2, last_cut,root_opt,uopt,hdata,hmc1);

  if (!hmc1){
    std::cout << "Cannot compute efficiency because the experiment does not have any MC sample" << std::endl;
    return;
  }

  // go back to the previous tree name
  exp.SetCurrentTree(tree_name);    

  // Histogram for cut2 (denominator)
  int nx = hmc1->GetNbinsX();
  double xmin = hmc1->GetXaxis()->GetXmin();
  double xmax = hmc1->GetXaxis()->GetXmax();
  double xbins[NMAXBINS];
  TH1F* hmc2 = new TH1F(GetUniqueName("eff2").c_str(),"eff2",nx,GetVariableBins(nx,xmin,xmax,xbins));
  _saved_histos.push_back(hmc2);

  for (int i=0; i<hmc2->GetNbinsX();i++){
    hmc2->SetBinContent(i+1,hmc1->GetBinContent(1));
    hmc2->SetBinError(  i+1,hmc1->GetBinError(1));
  }

  std::cout << "----- Efficiency values -----------" << std::endl;
  DrawingToolsBase::DrawRatioVSCut(hmc1,hmc2,isel,branch,first_cut,root_opt,uopt+" EFF",leg);
}

//*********************************************************
void DrawingTools::DrawEffPurVSCut(Experiment& exp, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
				   const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  DrawEffPurVSCut(exp,0,signal,precut,first_cut,last_cut, root_opt, opt, leg);
}


//*********************************************************
void DrawingTools::DrawEffPurVSCut(Experiment& exp, int branch, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
				   const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  DrawEffPurVSCut(exp,0,branch,signal,precut,first_cut,last_cut, root_opt, opt, leg);
}

//*********************************************************
void DrawingTools::DrawEffPurVSCut(Experiment& exp, int isel, int branch, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
				   const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  DrawEffPurVSCut(exp,isel,branch,signal,precut,first_cut,last_cut, first_cut,last_cut, root_opt, opt, leg);
}

//*********************************************************
void DrawingTools::DrawEffPurVSCut(Experiment& exp, int isel, int branch, const std::string& signal, const std::string& precut, 
				   int first_cut_pur, int last_cut_pur, int first_cut_eff, int last_cut_eff,
				   const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  // Check if selection exists
  if (!sel().GetSelection(isel,true)) return;

  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

  // save the current legend size
  double sizex = _legendSize[0];
  double sizey = _legendSize[1];
  
  // small legend size
  SetLegendSize(0.1,0.1);

  // draw the efficiency
  DrawEffVSCut(exp,isel,branch,signal,precut,first_cut_eff,last_cut_eff,root_opt,                  opt,"eff "+leg);

  // draw the purity
  DrawPurVSCut(exp,isel,branch,signal,precut,first_cut_pur,last_cut_pur,(root_opt+" same").c_str(),opt,"pur "+leg);

  // go back to previous size
  SetLegendSize(sizex,sizey);
}

//*********************************************************
double DrawingTools::GetEff(Experiment& exp, bool usedata,
             const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt) {
//*********************************************************
  double errhigh = 0.;
  double errlow = 0.;
  double nev1 = 0.;
  double nev2 = 0.;
  return GetEff(exp, usedata, errlow, errhigh, nev1, nev2, cut1, cut2, root_opt, opt);
}

//*********************************************************
double DrawingTools::GetEff(Experiment& exp, bool usedata, double& errlow, double& errhigh, double& nev1, double& nev2,
             const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt) {
//*********************************************************

  errlow = 0;
  errhigh = 0;
  nev1 = 0;
  nev2 = 0;
  std::string slevel = GetSameLevel(root_opt);

  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return 0;


  // Need to project against a variable. "NWEIGHTS" is stored in all trees.
  std::string var = "NWEIGHTS";
  int nx = 1;
  double xbins[2] = {-1e6, 1e6};

  bool scale_errors = true;

  TGraphAsymmErrors* eff = GetEff(exp, usedata, var, nx, xbins, nev1, nev2, cut1, cut2, root_opt, opt, scale_errors);
  if (!eff) return 0;

  if (eff->GetN() != 1) {
    std::cout << "Error computing efficiency" << std::endl;
    return 0;
  }

  errlow = eff->GetEYlow()[0];
  errhigh = eff->GetEYhigh()[0];

  return eff->GetY()[0];
}

//*********************************************************
TGraphAsymmErrors* DrawingTools::GetEff(Experiment& exp, bool usedata, const std::string& var, int nx, double* xbins, double& nev1, double& nev2,
                                        const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, bool scale_errors) {
//*********************************************************

  // Create empty Histo Stacks
  HistoStack* hs1d = NULL;
  HistoStack* hs2d = NULL;
  HistoStack* hs1m = NULL;
  HistoStack* hs2m = NULL;
  if (!drawUtils::CheckOption(opt,"NODATA")){
    hs1d = new HistoStack(_title,_titleX,_titleY);
    hs2d = new HistoStack(_title,_titleX,_titleY);
  }
  if (!drawUtils::CheckOption(opt,"NOMC")){
    hs1m = new HistoStack(_title,_titleX,_titleY);
    hs2m = new HistoStack(_title,_titleX,_titleY);
  }

  std::string groupName = "all";
  std::string mcSampleName = "all";
  std::string categ = "all";
  std::string newopt = opt + " NOVARBIN";

  // Get the current tree name
  std::string tree_name = exp.GetCurrentTree();

  // use the "truth" tree for efficiencies and "default" for purities
  if (newopt.find("PUR")==std::string::npos && newopt.find("NOTRUTH")==std::string::npos)
    exp.SetCurrentTree("truth");   

  int ny = 0;
  double ybins[1] = {0};

  // Project the Experiment into the HistoStacks
  Project(hs1d, hs1m, exp, groupName, mcSampleName, var,nx,xbins,ny,ybins,categ,(cut1+" && "+cut2),root_opt,newopt,1.,scale_errors);  
  Project(hs2d, hs2m, exp, groupName, mcSampleName, var,nx,xbins,ny,ybins,categ, cut2,             root_opt,newopt,1.,scale_errors);  

  // go back to the previous tree name
  if (newopt.find("PUR")==std::string::npos && newopt.find("NOTRUTH")==std::string::npos)
    exp.SetCurrentTree(tree_name);    

  // h1 = Histogram for cut1+cut2 (numerator)
  // h2 = Histogram for cut2 (denominator)
  TH1F* h1;
  TH1F* h2;
  if (usedata) {
    h1 = hs1d->GetTotal1D();
    h2 = hs2d->GetTotal1D();
  } else {
    h1 = hs1m->GetTotal1D();
    h2 = hs2m->GetTotal1D();
  }

  if (!h1){
    std::cout << "ERROR. numerator does not exist !!!" << std::endl;
    return NULL;
  }
  if (!h2){
    std::cout << "ERROR. denominator does not exist !!!" << std::endl;
    return NULL;
  }


  nev1 = h1->Integral(0, h1->GetNbinsX() + 1);
  nev2 = h2->Integral(0, h2->GetNbinsX() + 1);


  h1->Sumw2();
  h2->Sumw2();

  // compute the efficiency
  TGraphAsymmErrors* eff = new TGraphAsymmErrors(h1);
  _saved_graphs.push_back(eff);
  eff->Divide(h1, h2, _eff_params.c_str()); //the options are explicitely provided by SetEffDivideParams(const std::string&), root_opt not used to avoid possible confusions 
  
  // Put the errors into the ratio
  if (usedata)
    FillGraphErrors(hs1d, hs2d, eff, opt);
  else
    FillGraphErrors(hs1m, hs2m, eff, opt);


  if (hs1d) delete hs1d;
  if (hs2d) delete hs2d;
  if (hs1m) delete hs1m;
  if (hs2m) delete hs2m;


  return eff;
}

//*********************************************************
TGraphAsymmErrors* DrawingTools::GetEffNew(Experiment& exp, bool usedata, const std::string& var, int nx, double* xbins, double& nev1, double& nev2,
					   const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt1, 
					   const std::string& opt2, bool scale_errors) {
//*********************************************************

  // Create empty Histo Stacks
  HistoStack* hs1d = NULL;
  HistoStack* hs2d = NULL;
  HistoStack* hs1m = NULL;
  HistoStack* hs2m = NULL;

  // both opt1 and opt2 must contain the option "NODATA" or "NOMC"
  if (!drawUtils::CheckOption(opt1,"NODATA")){
    hs1d = new HistoStack(_title,_titleX,_titleY);
    hs2d = new HistoStack(_title,_titleX,_titleY);
  }
  if (!drawUtils::CheckOption(opt2,"NOMC")){
    hs1m = new HistoStack(_title,_titleX,_titleY);
    hs2m = new HistoStack(_title,_titleX,_titleY);
  }

  std::string groupName = "all";
  std::string mcSampleName = "all";
  std::string categ = "all";
  std::string newopt1 = opt1 + " NOVARBIN";
  std::string newopt2 = opt2 + " NOVARBIN";

  // Get the current tree name
  //  std::string tree_name = exp.GetCurrentTree();

  int ny = 0;
  double ybins[1] = {0};

  // Project the Experiment into the HistoStacks
  Project(hs1d, hs1m, exp, groupName, mcSampleName, var,nx,xbins,ny,ybins,categ,cut1,root_opt,newopt1,1.,scale_errors);  
  Project(hs2d, hs2m, exp, groupName, mcSampleName, var,nx,xbins,ny,ybins,categ,cut2,root_opt,newopt2,1.,scale_errors);  

  // go back to the previous tree name
  //  exp.SetCurrentTree(tree_name);    

  // h1 = Histogram for cut1+cut2 (numerator)
  // h2 = Histogram for cut2 (denominator)
  TH1F* h1;
  TH1F* h2;
  if (usedata) {
    h1 = hs1d->GetTotal1D();
    h2 = hs2d->GetTotal1D();
  } else {
    h1 = hs1m->GetTotal1D();
    h2 = hs2m->GetTotal1D();
  }

  nev1 = h1->Integral(0, h1->GetNbinsX() + 1);
  nev2 = h2->Integral(0, h2->GetNbinsX() + 1);

  // compute the efficiency
  TGraphAsymmErrors* eff = new TGraphAsymmErrors(h1);
  _saved_graphs.push_back(eff);
  eff->Divide(h1, h2, _eff_params.c_str()); //the options are explicitely provided by SetEffDivideParams(const std::string&), root_opt not used to avoid possible confusions 

  // Put the errors into the ratio
  if (usedata)
    FillGraphErrors(hs1d, hs2d, eff, opt1);
  else
    FillGraphErrors(hs1m, hs2m, eff, opt1);


  if (hs1d) delete hs1d;
  if (hs2d) delete hs2d;
  if (hs1m) delete hs1m;
  if (hs2m) delete hs2m;


  return eff;
}

//*********************************************************
void DrawingTools::DrawRatioNew(Experiment& exp, bool usedata, const std::string& var, int nx, double xmin, double xmax,
			      const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt1, const std::string& opt2) {
//*********************************************************
  double xbins[NMAXBINS];
  DrawRatioNew(exp, usedata, var, nx, GetVariableBins(nx, xmin, xmax, xbins), cut1, cut2, root_opt, opt1, opt2);
}

//*********************************************************
void DrawingTools::DrawRatioNew(Experiment& exp, bool usedata, const std::string& var, int nx, double* xbins,
			      const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt1, 
			      const std::string& opt2) {
//*********************************************************
  double dummy1, dummy2;
  std::string uopt1 = drawUtils::ToUpper(opt1);
  std::string uopt2 = drawUtils::ToUpper(opt2);
  std::string uroot_opt = drawUtils::ToUpper(root_opt);

  DrawRatioNew(exp, usedata, var, nx, xbins, dummy1, dummy2, cut1, cut2, uroot_opt, uopt1, uopt2);
}

//*********************************************************
void DrawingTools::DrawRatioNew(Experiment& exp, bool usedata, const std::string& var, int nx, double* xbins, double& nev1, double& nev2,
				const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt1, 
				const std::string& opt2, bool scale_errors) {
//*********************************************************


  std::string uopt1 = drawUtils::ToUpper(opt1);
  std::string uopt2 = drawUtils::ToUpper(opt2);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt1)) return;

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt2)) return;


  // Create empty Histo Stacks
  HistoStack* hs1d = NULL;
  HistoStack* hs2d = NULL;
  HistoStack* hs1m = NULL;
  HistoStack* hs2m = NULL;

  // both opt1 and opt2 must contain the option "NODATA" or "NOMC"
  if (!drawUtils::CheckOption(opt1,"NODATA")){
    hs1d = new HistoStack(_title,_titleX,_titleY);
    hs2d = new HistoStack(_title,_titleX,_titleY);
  }
  if (!drawUtils::CheckOption(opt2,"NOMC")){
    hs1m = new HistoStack(_title,_titleX,_titleY);
    hs2m = new HistoStack(_title,_titleX,_titleY);
  }

  std::string groupName = "all";
  std::string mcSampleName = "all";
  std::string categ = "all";
  std::string newopt1 = uopt1 + " NOVARBIN";
  std::string newopt2 = uopt2 + " NOVARBIN";

  // Get the current tree name
  //  std::string tree_name = exp.GetCurrentTree();

  int ny = 0;
  double ybins[1] = {0};

  // Project the Experiment into the HistoStacks
  Project(hs1d, hs1m, exp, groupName, mcSampleName, var,nx,xbins,ny,ybins,categ,cut1,root_opt,newopt1,1.,scale_errors);  
  Project(hs2d, hs2m, exp, groupName, mcSampleName, var,nx,xbins,ny,ybins,categ,cut2,root_opt,newopt2,1.,scale_errors);  

  // go back to the previous tree name
  //  exp.SetCurrentTree(tree_name);    

  // h1 = Histogram for cut1+cut2 (numerator)
  // h2 = Histogram for cut2 (denominator)
  TH1F* h1;
  TH1F* h2;
  if (usedata) {
    h1 = hs1d->GetTotal1D();
    h2 = hs2d->GetTotal1D();
  } else {
    h1 = hs1m->GetTotal1D();
    h2 = hs2m->GetTotal1D();
  }

  nev1 = h1->Integral(0, h1->GetNbinsX() + 1);
  nev2 = h2->Integral(0, h2->GetNbinsX() + 1);

  //Draw the HistoStacks
  if (usedata)
    DrawRatioHistoStacks(hs1d,hs2d,root_opt,newopt1,0.);
  else
    DrawRatioHistoStacks(hs1m,hs2m,root_opt,newopt1,0.);

  if (hs1d) delete hs1d;
  if (hs2d) delete hs2d;
  if (hs1m) delete hs1m;
  if (hs2m) delete hs2m;

}


//**************************************************
void DrawingTools::GetEventsVSCut(Experiment& exp, const std::string& name, const std::string& cut_norm, int isel,int ibranch, int& first_cut, int& last_cut,
           const std::string& root_opt, const std::string& opt, TH1F*& data, TH1F*& mc){
//**************************************************
  
  // Check if selection exists
  if (!sel().GetSelection(isel,true)) return;

  (void)root_opt; // root_opt not used here
  std::string uopt = drawUtils::ToUpper(opt);

  // Read the steps from the config tree
  //  ReadSteps(_config_file);


  // Deal properly with first and last cuts
  if (last_cut>(int)(sel().GetSelection(isel)->GetNCuts(ibranch)-1) || last_cut == -1) last_cut=sel().GetSelection(isel)->GetNCuts(ibranch)-1; 
  if (first_cut<-1 || first_cut>last_cut ) first_cut=-1;

  // Output Histrogram attributes
  double xmin=first_cut-0.5;
  double xmax=last_cut+0.5;
  int nx = (int)(xmax-xmin);

  std::vector<double> balldata;
  balldata.resize(nx);
  std::vector<double> ballmc;
  ballmc.resize(nx);

  // initially no data and MC are found
  bool found_data=false;
  bool found_mc=false;
  data = NULL;
  mc = NULL;

  std::string cut0 = cut_norm;
  if (cut0 == "") cut0 = "1==1";

  for (int i=0;i<nx;i++ ){
    int icut = first_cut+i;
    
    // Define the cuts
    std::stringstream scut;
    scut << icut;
    std::stringstream sbranch;
    sbranch << ibranch;
    std::stringstream ssel;
    ssel << isel;

    std::string cut;
    if (exp.GetCurrentTree() == "truth"){
      if (sel().GetNEnabledSelections()>1)
        cut= cut0 + " && accum_level["+ssel.str()+"]["+sbranch.str()+"]>"+scut.str();
      else
        cut= cut0 + " && accum_level["+sbranch.str()+"]>"+scut.str();
    }
    else{
      if (sel().GetNEnabledSelections()>1)
        cut= cut0 + " && accum_level[]["+ssel.str()+"]["+sbranch.str()+"]>"+scut.str();
      else
        cut= cut0 + " && accum_level[]["+sbranch.str()+"]>"+scut.str();
    }

    // Create temporary empty Histo Stacks
    HistoStack* hs1 = new HistoStack((_title+"cut"+scut.str()).c_str(),_titleX,_titleY);
    HistoStack* hs2 = new HistoStack((_title+"cut"+scut.str()).c_str(),_titleX,_titleY);

    double xbins_dummy[2]={0,1};
    double ybins_dummy[1];

    // Project the Experiment into the HistoStacks
    Project(hs1, hs2, exp, "all", "all", "0.5",1,xbins_dummy,0,ybins_dummy,"all",cut,root_opt,opt,1.,false);  

    if (hs2->GetTotal1D()){
      ballmc[i] = (double)hs2->GetTotal1D()->Integral();
      found_mc = true;
    }

    if (hs1->GetTotal1D()){
      balldata[i] = (double)hs1->GetTotal1D()->Integral();
      found_data = true;
    }

    // delete the temporary HistoStacks
    delete hs1;
    delete hs2;
  }


  // Fill the output histograms
  double xbins[NMAXBINS];
  if (found_data){
    data = new TH1F(GetUniqueName(name+"_data").c_str(),"",nx,GetVariableBins(nx,xmin,xmax,xbins));
    _saved_histos.push_back(data);
    for (int i=0;i<nx;i++ )
      data->SetBinContent(i+1,balldata[i]);
  }
  if (found_mc){
    mc = new TH1F(GetUniqueName(name+"_mc").c_str(),"",nx,GetVariableBins(nx,xmin,xmax,xbins));
    _saved_histos.push_back(mc);
    for (int i=0;i<nx;i++ )
      mc->SetBinContent(i+1,ballmc[i]);
  }

}

//*********************************************************
void DrawingTools::CompareSampleGroups(Experiment& exp, const std::string& mcSampleName, const std::string& var, int nx, double xmin, double xmax,
      const std::string& cut,  const std::string& root_opt, const std::string& opt, bool scale_errors, const std::string& normtype){
//*********************************************************

  double xbins[NMAXBINS];
  CompareSampleGroups(exp,mcSampleName,var,nx,GetVariableBins(nx,xmin,xmax,xbins),cut,root_opt,opt,scale_errors,normtype);
}

//*********************************************************
void DrawingTools::CompareSampleGroups(Experiment& exp, const std::string& mcSampleName, const std::string& var, int nx, double* xbins,
      const std::string& cut,  const std::string& root_opt, const std::string& opt, bool scale_errors, const std::string& normtype){
//*********************************************************

  std::vector<HistoStack*> hs;
  int ny = 0;
  double ybins[NMAXBINS];
  bool first = true;
  double norm;
  int lc = 1;
  DataSample* firstSample=NULL;
  std::string unormtype = drawUtils::ToUpper(normtype);
  std::string uroot_opt = drawUtils::ToUpper(root_opt);
  std::string uopt = drawUtils::ToUpper(opt);
  int count = 0;

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

  // Create the legend
  if (!drawUtils::CheckInternalOption(uopt,"NOCREATELEG")) {
    CreateLegend();
  }

  // Loop over SampleGroup's in the experiment
  std::map< std::string, SampleGroup >::iterator it;
  for (it = exp.GetSampleGroups().begin(); it != exp.GetSampleGroups().end(); it++) {
    SampleGroup& sampleGroup = it->second;

    DataSample* data = sampleGroup.GetDataSample();
    std::map< std::string, DataSample*>& mcSamples = sampleGroup.GetMCSamples();

    if (data) {
      bool thisfirst = false;
      HistoStack* hs = new HistoStack(_title,_titleX,_titleY);
      _saved_histoStacks.push_back(hs);

      if (unormtype == "POT") {
        if (first) {
          firstSample = data;
          norm = 1;
        } else {
          norm = GetNormalisationFactor(firstSample, data, 1.0, true, opt);
        }
      } else {
        norm = 1;
      }

      if (first) {
        first = false;
        thisfirst = true;
      }

      // Project the first sample
      Project(hs, "sample1", *data, var, nx, xbins, ny, ybins, "all", cut, uroot_opt, uopt, norm, scale_errors);

      if (unormtype == "ONE") {
        double integral = hs->GetTotal1D()->Integral();
        hs->GetTotal1D()->Sumw2();
        hs->GetTotal1D()->Scale(1./integral);
      }

      std::string leg = it->first + " Data";
      hs->GetTotal1D()->SetTitle(leg.c_str());
      lc = _auto_colors[count];
      if (!drawUtils::CheckInternalOption(uopt,"NOCREATELEG")) {
        _drawleg = true;
      }

      SetStatPos((1-0.4*count-gStyle->GetPadRightMargin()),(1.04-gStyle->GetPadTopMargin()));
      DrawHisto(hs->GetTotal1D(),lc,1,lc,0, uroot_opt + " E", uopt, "lpe");
      if (!drawUtils::CheckOption(uopt,"NODRAW"))
	gPad->Update();

      if (thisfirst) {
        uroot_opt += " SAMES";
      }
    }

    if (mcSamples.size() > 0) {
      bool thisfirst = false;
      HistoStack* hs = new HistoStack(_title,_titleX,_titleY);
      _saved_histoStacks.push_back(hs);

      std::map<std::string, DataSample*>::iterator it2;
      for (it2 = mcSamples.begin(); it2 != mcSamples.end(); it2++) {
        const std::string& mcSampleName2 = it2->first;
        if (mcSampleName2 != mcSampleName && mcSampleName != "all") continue;

        DataSample* sample2 = it2->second;
        if (sample2) {
          if (unormtype == "POT") {
            if (first) {
              firstSample = sample2;
              norm = 1;
            } else {
              norm = GetNormalisationFactor(firstSample, sample2, 1.0, true, opt);
            }
          } else {
            norm = 1;
          }

          if (first) {
            first = false;
            thisfirst = true;
          }

          // Project the second sample normalized to the first one
          Project(hs, "sample2", *sample2, var, nx, xbins, ny, ybins, "all", cut, uroot_opt, uopt, norm, scale_errors);
        }
      }

      if (unormtype == "ONE") {
        double integral = hs->GetTotal1D()->Integral();
        hs->GetTotal1D()->Sumw2();
        hs->GetTotal1D()->Scale(1./integral);
      }

      std::string leg = it->first;
      if (mcSampleName == "all") {
        leg += " MC";
      } else {
        leg += " " + mcSampleName;
      }

      lc = _auto_colors[count];
      hs->GetTotal1D()->SetTitle(leg.c_str());
      if (!drawUtils::CheckInternalOption(uopt,"NOCREATELEG")) {
        _drawleg = true;
      }
      
      SetStatPos((0.8-0.4*count-gStyle->GetPadRightMargin()),(1.04-gStyle->GetPadTopMargin()));
      DrawHisto(hs->GetTotal1D(),lc,2,lc,3004+count, uroot_opt + " HIST", uopt, "f");
      if (!drawUtils::CheckOption(uopt,"NODRAW"))
	gPad->Update();

      if (thisfirst) {
        uroot_opt += " SAMES";
      }
    }

    count++;
  }

  // Create the legend
  if (!drawUtils::CheckOption(uopt,"NOLEG")){
    _legends.back()->Draw();
  }

}


//*********************************************************
void DrawingTools::DrawErrors(TTree* tree, const std::string& var, int nx, double xmin, double xmax,
			      const std::string& cut, const std::string& root_opt, const std::string& opt, const std::string& leg, double norm, bool scale_errors){
//*********************************************************

  // Draw 1D histogram errors for a given set of cuts and uniform binning
  double xbins[NMAXBINS];
  DrawErrors(tree, var, nx, GetVariableBins(nx,xmin,xmax,xbins), cut, root_opt, opt, leg, norm, scale_errors);
}

//*********************************************************
void DrawingTools::DrawRelativeErrors(TTree* tree, const std::string& var, int nx, double xmin, double xmax,
				      const std::string& cut, const std::string& root_opt, const std::string& opt, const std::string& leg, double norm, bool scale_errors){
//*********************************************************

  // Draw 1D histogram relative errors for a given set of cuts and uniform binning
  double xbins[NMAXBINS];
  DrawRelativeErrors(tree, var, nx, GetVariableBins(nx,xmin,xmax,xbins), cut, root_opt, opt, leg, norm, scale_errors);
}

//*********************************************************
void DrawingTools::DrawErrors(TTree* tree, const std::string& var, int nx, double* xbins, 
			      const std::string& cut, const std::string& root_opt, const std::string& opt, const std::string& leg, double norm, bool scale_errors){
//*********************************************************

  // Draw 1D histogram errors for a given set of cuts and variable binning
  bool relative=false;
  DrawErrorsBase(tree, var, nx, xbins, cut, relative, root_opt, opt, leg, norm, scale_errors);
}

//*********************************************************
void DrawingTools::DrawRelativeErrors(TTree* tree, const std::string& var, int nx, double* xbins, 
				      const std::string& cut, const std::string& root_opt, const std::string& opt, const std::string& leg, double norm, bool scale_errors){
//*********************************************************

  // Draw 1D histogram relative errors for a given set of cuts and variable binning
  bool relative=true;
  DrawErrorsBase(tree, var, nx, xbins, cut, relative, root_opt, opt, leg, norm, scale_errors);
}

//*********************************************************
void DrawingTools::DrawRelativeErrors(TTree* tree1, TTree* tree2, const std::string& var, int nx, double xmin, double xmax,
				      const std::string& cut, const std::string& root_opt, const std::string& opt, const std::string& leg, double norm, bool scale_errors){
//*********************************************************

  // Draw 1D histogram relative errors for a given set of cuts and uniform binning
  double xbins[NMAXBINS];
  DrawRelativeErrors(tree1, tree2, var, nx, GetVariableBins(nx,xmin,xmax,xbins), cut, root_opt, opt, leg, norm, scale_errors);
}

//*********************************************************
void DrawingTools::DrawRelativeErrors(TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins, 
				      const std::string& cut, const std::string& root_opt, const std::string& opt, const std::string& leg, double norm, bool scale_errors){
//*********************************************************

  // Draw 1D histogram relative errors for a given set of cuts and variable binning
  bool relative=true;
  DrawErrorsBase(tree1, var, nx, xbins, cut, relative, root_opt,         opt, leg, norm, scale_errors);
  DrawErrorsBase(tree2, var, nx, xbins, cut, relative, root_opt+" same", opt, leg, norm, scale_errors);
}

//*********************************************************
void DrawingTools::DrawErrorsBase(TTree* tree, const std::string& var, int nx, double* xbins, 
				  const std::string& cut, bool relative,  const std::string& root_opt, const std::string& opt, const std::string& leg, double norm,  bool scale_errors){
//*********************************************************
  if(syst_tools().errdebug)    std::cout<<"DrawErrorBase \n=============== "<<std::endl;
  (void)norm;
  (void)scale_errors;

  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

  // Create the empty HistoStack
  HistoStack* hs = new HistoStack(_title,_titleX,_titleY);
  _saved_histoStacks.push_back(hs);

  // Project the tree
  double* ybins=NULL;
  uopt += " EXP";
  DrawingToolsBase::Project(hs, "", tree,var,nx,xbins,0,ybins,"all",cut,root_opt,uopt,norm,scale_errors);

  DrawErrorsBase(hs,relative,root_opt,opt,leg);
}

//*********************************************************
void DrawingTools::DrawErrors(Experiment& exp, const std::string& var, int nx, double xmin, double xmax,
			      const std::string& cut, const std::string& root_opt, const std::string& opt, const std::string& leg, bool scale_errors){
//*********************************************************

  // Draw 1D histogram errors for a given set of cuts and uniform binning
  double xbins[NMAXBINS];
  DrawErrors(exp, var, nx, GetVariableBins(nx,xmin,xmax,xbins), cut, root_opt, opt, leg, scale_errors);
}

//*********************************************************
void DrawingTools::DrawRelativeErrors(Experiment& exp, const std::string& var, int nx, double xmin, double xmax,
				      const std::string& cut, const std::string& root_opt, const std::string& opt, const std::string& leg, bool scale_errors){
//*********************************************************

  // Draw 1D histogram relative errors for a given set of cuts and uniform binning
  double xbins[NMAXBINS];
  DrawRelativeErrors(exp, var, nx, GetVariableBins(nx,xmin,xmax,xbins), cut, root_opt, opt, leg, scale_errors);
}

//*********************************************************
void DrawingTools::DrawErrors(Experiment& exp, const std::string& var, int nx, double* xbins, 
			      const std::string& cut, const std::string& root_opt, const std::string& opt, const std::string& leg, bool scale_errors){
//*********************************************************

  // Draw 1D histogram errors for a given set of cuts and variable binning
  bool relative=false;
  DrawErrorsBase(exp, "all", "all", var, nx, xbins, cut, relative, root_opt, opt, leg, scale_errors);
}

//*********************************************************
void DrawingTools::DrawRelativeErrors(Experiment& exp, const std::string& var, int nx, double* xbins, 
				      const std::string& cut, const std::string& root_opt, const std::string& opt, const std::string& leg, bool scale_errors){
//*********************************************************

  // Draw 1D histogram relative errors for a given set of cuts and variable binning
  bool relative=true;
  DrawErrorsBase(exp, "all", "all", var, nx, xbins, cut, relative, root_opt, opt, leg, scale_errors);
}

//*********************************************************
void DrawingTools::DrawErrors(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double xmin, double xmax,
			      const std::string& cut, const std::string& root_opt, const std::string& opt, const std::string& leg, bool scale_errors){
//*********************************************************

  // Draw 1D histogram errors for a given set of cuts and uniform binning
  double xbins[NMAXBINS];
  DrawErrors(exp, groupName, mcSampleName, var, nx, GetVariableBins(nx,xmin,xmax,xbins), cut, root_opt, opt, leg, scale_errors);
}

//*********************************************************
void DrawingTools::DrawRelativeErrors(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double xmin, double xmax,
				      const std::string& cut, const std::string& root_opt, const std::string& opt, const std::string& leg, bool scale_errors){
//*********************************************************

  // Draw 1D histogram relative errors for a given set of cuts and uniform binning
  double xbins[NMAXBINS];
  DrawRelativeErrors(exp, groupName, mcSampleName, var, nx, GetVariableBins(nx,xmin,xmax,xbins), cut, root_opt, opt, leg, scale_errors);
}

//*********************************************************
void DrawingTools::DrawErrors(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double* xbins, 
			      const std::string& cut, const std::string& root_opt, const std::string& opt, const std::string& leg, bool scale_errors){
//*********************************************************

  // Draw 1D histogram errors for a given set of cuts and variable binning
  bool relative=false;
  DrawErrorsBase(exp, groupName, mcSampleName, var, nx, xbins, cut, relative, root_opt, opt, leg, scale_errors);
}

//*********************************************************
void DrawingTools::DrawRelativeErrors(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double* xbins, 
				      const std::string& cut, const std::string& root_opt, const std::string& opt, const std::string& leg, bool scale_errors){
//*********************************************************

  // Draw 1D histogram relative errors for a given set of cuts and variable binning
  bool relative=true;
  DrawErrorsBase(exp, groupName, mcSampleName, var, nx, xbins, cut, relative, root_opt, opt, leg, scale_errors);
}

//*********************************************************
void DrawingTools::DrawErrorsBase(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double* xbins,
				  const std::string& cut, bool relative, const std::string& root_opt, const std::string& opt, const std::string& leg, bool scale_errors){
//*********************************************************

  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;


  if (groupName!="all"){
    if (!exp.HasSampleGroup(groupName)) return;
  }

  // Create empty Histo Stacks
  HistoStack* hs1 = NULL;
  HistoStack* hs2 = NULL;
  if (!drawUtils::CheckOption(uopt,"NODATA")){
    hs1 = new HistoStack(_title,_titleX,_titleY);
    _saved_histoStacks.push_back(hs1);
  }
  if (!drawUtils::CheckOption(uopt,"NOMC")){
    hs2 = new HistoStack(_title,_titleX,_titleY);
    _saved_histoStacks.push_back(hs2);
  }

  // Project the Experiment into the HistoStacks
  double* ybins=NULL;
  Project(hs1, hs2, exp, groupName, mcSampleName, var,nx,xbins,0,ybins,"all",cut,root_opt,opt,1.,scale_errors);

  if (hs2)
    DrawErrorsBase(hs2,relative,root_opt,opt,leg);
  else if (hs1)
    DrawErrorsBase(hs1,relative,root_opt,opt,leg);
}

//*********************************************************
void DrawingTools::DrawErrorsBase(HistoStack* hs, bool relative, const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

  std::string slevel = GetSameLevel(root_opt);
  std::string name3 = GetUniqueName("alle");

  if (slevel=="0" && leg!="")
    CreateLegend();

  TH1F* hall = hs->GetTotal1D();

  if (opt.find("ST")==std::string::npos && (opt.find("SYS")!=std::string::npos || opt.find("DIS")!=std::string::npos))
    hall = hs->GetTotalSyst1D();
  else if (opt.find("ST")!=std::string::npos && (opt.find("SYS")==std::string::npos && opt.find("DIS")==std::string::npos))
    hall = hs->GetTotalStat1D();

  TH1F* herrors = new TH1F(*hall);
  herrors->SetName(name3.c_str());
  _saved_histos.push_back(herrors);  

  Float_t ntotal =0;
  Float_t avg_error =0;
  for (int i=0;i<hall->GetNbinsX();i++){
    if (relative)
      if (hall->GetBinContent(i+1)!=0){
	herrors->SetBinContent(i+1, hall->GetBinError(i+1)/hall->GetBinContent(i+1));
	avg_error += hall->GetBinError(i+1);
      }
      else
	herrors->SetBinContent(i+1, 0.);
    else{
      avg_error += hall->GetBinError(i+1)*hall->GetBinContent(i+1);
      herrors->SetBinContent(i+1, hall->GetBinError(i+1));
    }

    ntotal += hall->GetBinContent(i+1);
    herrors->SetBinError(i+1, 0);
  }     

  avg_error/=ntotal;
  std::cout << "average differencial error = " << avg_error << std::endl;

  std::string title_temp = _title;

  // Histogram attributes
  if (relative)
    SetTitleY("relative error");
  else
    SetTitleY("absolute error");

  herrors->SetTitle(leg.c_str());

  std::string varbin = "";
  if (relative) varbin= " NOVARBIN";

  // Draw the histo
  DrawHisto(herrors, _line_width, _line_color, _fill_style, root_opt, uopt+ " NOSTAT"+varbin,"LE1P");

  // reset the Y title
  SetTitleY(title_temp);

  // Draw the legend
  if (!drawUtils::CheckOption(uopt,"NOLEG")  && leg!="")
    _legends.back()->Draw();

  if (!drawUtils::CheckOption(uopt,"NODRAW"))
    gPad->Update();

}

//*********************************************************
void DrawingTools::DrawCovMatrix(TTree* tree, const std::string& var, int nx, double* xbins,
				 const std::string& cut, const std::string& root_opt, const std::string& uopt){
//*********************************************************

  const TMatrixD& mcov = GetCovMatrix(tree, var, nx, xbins, cut, uopt);

  // Draw the histo
  DrawMatrix(mcov, _auto_colors[_same_level], _line_width, _line_color, _fill_style, root_opt, uopt);
    
}

//*********************************************************
void DrawingTools::DrawCovMatrix(TTree* tree, const std::string& var, int nx, double xmin, double xmax,
				 const std::string& cut, const std::string& root_opt, const std::string& uopt){
//*********************************************************
  double xbins[NMAXBINS];
  
  DrawCovMatrix(tree, var, nx, GetVariableBins(nx, xmin, xmax, xbins), cut, root_opt, uopt); 
    
}

//*********************************************************
void DrawingTools::DrawCovMatrix(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double* xbins, 
				 const std::string& cut, const std::string& root_opt, const std::string& uopt){
//*********************************************************
  const TMatrixD& mcov = GetCovMatrix(exp, groupName, mcSampleName, var, nx, xbins, cut, root_opt, uopt);
  
  // Draw the histo
  DrawMatrix(mcov, _auto_colors[_same_level], _line_width, _line_color, _fill_style, root_opt, uopt); 
}

//*********************************************************
void DrawingTools::DrawCovMatrix(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double xmin, double xmax,  
				 const std::string& cut, const std::string& root_opt, const std::string& uopt){
//*********************************************************
  const TMatrixD& mcov = GetCovMatrix(exp, groupName, mcSampleName, var, nx, xmin, xmax, cut, root_opt, uopt);
  
  // Draw the histo
  DrawMatrix(mcov, _auto_colors[_same_level], _line_width, _line_color, _fill_style, root_opt, uopt); 
}
 
//*********************************************************
void DrawingTools::DrawCovMatrix(Experiment& exp, const std::string& var, int nx, double* xbins,  
				 const std::string& cut, const std::string& root_opt, const std::string& uopt){
//*********************************************************
  const TMatrixD& mcov = GetCovMatrix(exp, var, nx, xbins, cut, root_opt, uopt);
  
  // Draw the histo
  DrawMatrix(mcov, _auto_colors[_same_level], _line_width, _line_color, _fill_style, root_opt, uopt); 
}


//*********************************************************
void DrawingTools::DrawCovMatrix(Experiment& exp, const std::string& var, int nx, double xmin, double xmax,  
				 const std::string& cut, const std::string& root_opt, const std::string& uopt){
//*********************************************************
  const TMatrixD& mcov = GetCovMatrix(exp, var, nx, xmin, xmax, cut, root_opt, uopt);
  
  // Draw the histo
  DrawMatrix(mcov, _auto_colors[_same_level], _line_width, _line_color, _fill_style, root_opt, uopt); 
}

//*********************************************************
const TMatrixD& DrawingTools::GetCovMatrix(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double* xbins, 
					   const std::string& cut, const std::string& root_opt, const std::string& opt){
//*********************************************************
  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return syst_tools().GetSystematicCov(NULL, "DUMMY");


  if (groupName!="all"){
    if (!exp.HasSampleGroup(groupName)) return syst_tools().GetSystematicCov(NULL, "DUMMY");
  }

  // Create empty Histo Stacks
  HistoStack* hs1 = NULL; 
  if (drawUtils::CheckOption(uopt,"SCALETODATA")){
    hs1 = new HistoStack(_title,_titleX,_titleY);
    _saved_histoStacks.push_back(hs1);  
  }
 
  // For MC information
  HistoStack* hs2 = new HistoStack(_title,_titleX,_titleY);
  _saved_histoStacks.push_back(hs2);  
  
  
  std::string opt_tmp = uopt;
  
  opt_tmp += " CAT"; //not to fill histos with errors
  
  opt_tmp += "MATRIX"; //matrix mode, still update the histo strack histograms related to systematics

  // Project the Experiment into the HistoStacks
  double* ybins=NULL;
  Project(hs1, hs2, exp, groupName, mcSampleName, var,nx,xbins,0,ybins,"all",cut,root_opt,opt_tmp, 1., false); // false --> do not care about stat errors

  // Experiment was projected so can calculate the covariance matrix
  // Get the covariance matrix using systematic tools
  const TMatrixD& mcov = syst_tools().GetSystematicCov(hs2, uopt);

  return mcov;
}

//*********************************************************
const TMatrixD& DrawingTools::GetCovMatrix(Experiment& exp, const std::string& groupName, const std::string& mcSampleName, const std::string& var, int nx, double xmin, double xmax,  
					   const std::string& cut, const std::string& root_opt, const std::string& uopt){
//*********************************************************
  double xbins[NMAXBINS];

  return  GetCovMatrix(exp, groupName, mcSampleName, var, nx, GetVariableBins(nx, xmin, xmax, xbins), cut, root_opt, uopt);
} 

//*********************************************************
const TMatrixD& DrawingTools::GetCovMatrix(Experiment& exp, const std::string& var, int nx, double* xbins,  
					   const std::string& cut, const std::string& root_opt, const std::string& uopt){
//*********************************************************

  return  GetCovMatrix(exp, "all", "all", var, nx, xbins, cut, root_opt, uopt);
} 

//*********************************************************
const TMatrixD& DrawingTools::GetCovMatrix(Experiment& exp, const std::string& var, int nx, double xmin, double xmax,  
					   const std::string& cut, const std::string& root_opt, const std::string& uopt){
//*********************************************************
  double xbins[NMAXBINS];

  return  GetCovMatrix(exp, "all", "all", var, nx, GetVariableBins(nx, xmin, xmax, xbins), cut, root_opt, uopt);
} 

//*********************************************************
const TMatrixD& DrawingTools::GetCovMatrix(TTree* tree, const std::string& var, int nx, double xmin, double xmax,
					   const std::string& cut, const std::string& uopt){
//*********************************************************
  double xbins[NMAXBINS];
  
  //get the covariance matrix using systematic tools
  return GetCovMatrix(tree, var, nx, GetVariableBins(nx, xmin, xmax, xbins), cut, uopt);
}

//*********************************************************
const TMatrixD& DrawingTools::GetCovMatrix(TTree* tree, const std::string& var, int nx, double* xbins,
					   const std::string& cut, const std::string& uopt){
//*********************************************************
  int NTOYS = drawUtils::GetNToys(tree);
  
  //get the covariance matrix using systematic tools
  const TMatrixD& mcov = syst_tools().GetSystematicCov(tree, var, nx, xbins, cut, NTOYS, uopt);
   
  return mcov; 
}

//*********************************************************
void  DrawingTools::DrawMatrix(const TMatrixD& m, int lc, int lw, int fc, int fs,
			       const std::string& root_opt, const std::string& uopt){
//*********************************************************
 
  // Histogram corresponding to the matrix
  TH2D* h = new TH2D(m);
  
  //set unique name
  std::string name = h->GetName();
  h->SetName(GetUniqueName(name).c_str());
 
  _saved_histos.push_back(h);  
 
  // Draw the histo
  DrawHisto(h, lc, lw, fc, fs, root_opt, uopt+ " NOSTAT NOMIN","");
  
  std::string ropt = drawUtils::ToUpper(root_opt);
  if (ropt.find("COLZ"))
    gPad->SetRightMargin(0.15);  //to be able to see the palette labels
 
  h->GetXaxis()->SetTitle("Bins");
  h->GetYaxis()->SetTitle("Bins");
  gPad->Modified();
  gPad->Update();
 
}

