#include "WeightTools.hxx"
#include "DrawingUtils.hxx"


//*********************************************************
std::string weightTools::ApplyWeightsOld(TTree* tree, const std::string& uopt){
//*********************************************************

  /*
    This method applies deals with flux and pileup weights for old micro-trees
   */ 


  std::string w="";

  if (drawUtils::CheckOption(uopt,"NOW")) return w;
  if (!drawUtils::TreeHasVar(tree,"weight") || drawUtils::TreeHasVar(tree,"weight_corr")) return w;

  int NWEIGHTS = drawUtils::GetNWeights(tree);
  for (int i=0;i<NWEIGHTS;i++){
    if (drawUtils::CheckOption(uopt,"NOFLUXW")  && i==0) continue;
    if (drawUtils::CheckOption(uopt,"NOPILEUPW")  && i==1) continue;
    w = w + "*weight["+drawUtils::GetString(i)+"]";
  }

  // Apply Weight systematics (effective correction when the mean of the systematic source is not 0)
  //  if (drawUtils::CheckOption(uopt,"WSCORR"))
  //    cutp = ApplyWeightSyst(tree,cutp,uopt);

  //  return ApplyWeightsNew(tree,cutp,uopt);

  return w;
}

//*********************************************************
std::string weightTools::ApplyWeights(TTree* tree, const std::string& cut,const std::string& opt){
//*********************************************************

  /* 
     This method computes the total weight. This is done modifiying the user cut in the following way:

     cut_w = (cut)*weight_syst[][0]*weight_syst[][1]*....*weight_syst[][10]*...  

     The user can select the weights to be applied via the user option uopt. For example if uopt contains "WS2 WS4" the output cut would be 
     of the form

     cut_w = (cut)*weight_syst[][2]*weight_syst[][4]

     The user can also combine weight systematics and corrections. For example if uopt contains "WS2 WC4" the output cut would be 
     of the form

     cut_w = (cut)*weight_syst[][2]*weight_corr[][4]

*/

  std::string uopt = drawUtils::ToUpper(opt);

  // Nothing to do when NOW (noweights) option is present  
  if (drawUtils::CheckOption(uopt,"NOW")) return cut;

  std::string w0="";

  if (!drawUtils::CheckOption(uopt,"NOTOYW")  && drawUtils::TreeHasVar(tree,"toy_weight"))
    w0 = "*toy_weight";  // The toy experiment weight

  // For backwards compatibility
  w0 = w0 + ApplyWeightsOld(tree,uopt);

  // Build the cut
  std::string cutp;
  if (cut !="")
    cutp = "("+cut+")"+w0;
  else
    cutp = "(1==1)"+w0;

  // The tree should contain weight_syst (not present in old default trees)
  if (!drawUtils::TreeHasVar(tree,"weight_syst")) return cutp;

  // Check if it has weight_corr
  bool has_corr = drawUtils::TreeHasVar(tree,"weight_corr");

  std::string w="";
  int NWEIGHTSYST = drawUtils::GetVarFromTree(tree,"NWEIGHTSYST");
  std::string weight_syst_name="weight_syst";
  std::string weight_corr_name="weight_corr";

  //  int redo = GetVarFromTree(tree,"redo");  //TODO

  // A second index for the "weight_syst" variables has been added for highlandTools >= v0r27. The SoftwareVersion variable has been added into the config file in v0r27
  // so it is enough to check the existence of any sofware version.
  //  if (_softwareVersion!="")
  if (drawUtils::GetNToys(tree)>1){
    weight_syst_name = weight_syst_name + "[]";
    weight_corr_name = weight_corr_name + "[]";
  }

  std::string uopt2 = uopt+" ";

  // If none of the weights is excluded look for weights explicitely included
  if (uopt2.find("NWS")==std::string::npos && uopt2.find("NWC")==std::string::npos){
    for (int iw=NWEIGHTSYST-1;iw>=0;iw--){
      std::stringstream siw;
      siw << iw;
      std::string ssiw = siw.str(); 
      bool found=false;
      // WS options are ignored for WCORR option
      if (uopt2.find("WCORR")==std::string::npos && uopt2.find("WS"+ssiw+" ")!=std::string::npos){
        w = w+"*"+weight_syst_name+"["+ssiw+"]";
        found=true;
      }
      if (has_corr && uopt2.find("WC"+ssiw+" ")!=std::string::npos){
        if (found) std::cout << "Option WS and WC cannot be specified for the same systematic !!!! WS taken !!!" << std::endl;
        else
          w = w+"*"+weight_corr_name+"["+ssiw+"]";
      }
    }
    if (w==""){
      if (uopt2.find("WCORR")==std::string::npos) 
        w = w+"*weight_syst_total";
      else if (has_corr)
        w = w+"*weight_corr_total";
    }    
  }
  else{
    for (int iw=0;iw<NWEIGHTSYST;iw++){
      std::stringstream siw;
      siw << iw;
      std::string ssiw = siw.str(); 
      // If NWSi is not found weight_syst[][i] is added
      // NWS options are ignored for WCORR option
      if (uopt2.find("WCORR")==std::string::npos && uopt2.find("NWS"+ssiw+" ")==std::string::npos){
        w = w+"*"+weight_syst_name+"["+ssiw+"]";
      }
      // If NWSi is found and WCi is found weight_corr[][i] is added
      else if (has_corr && uopt2.find("WC"+ssiw+" ")!=std::string::npos){
        w = w+"*"+weight_corr_name+"["+ssiw+"]";
      }
      else if (uopt2.find("WCORR")==std::string::npos && uopt2.find("WS"+ssiw+" ")!=std::string::npos){
        std::cout << "Option NWSi and WSi cannot be combined. WSi will have no effect" << std::endl;
      }
    }
  }

  // update the cut
  cutp = cutp + w;

  return cutp;
}
