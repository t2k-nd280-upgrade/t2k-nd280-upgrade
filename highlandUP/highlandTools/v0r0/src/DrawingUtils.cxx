#include "DrawingUtils.hxx"
#include <sstream>

std::map<std::string, std::string> _options;

double drawUtils::legendEntryHeight=0.05;

//*********************************************************
std::string drawUtils::ToUpper(const std::string& str) {
//*********************************************************
  TString tstr(str);
  tstr.ToUpper();
  return tstr.Data();
}

//*********************************************************
bool drawUtils::TreeHasVar(TTree* tree, const std::string& var){
//*********************************************************

  if (tree->FindLeaf(var.c_str()))
    return true;
  else
    return false;
}

//*********************************************************
int drawUtils::GetVarFromTree(TTree* tree, const std::string& var){
//*********************************************************

  if (!TreeHasVar(tree,var)) return 0;

  TH1F h0("v","v",1,0,1);
  tree->Project("v","0.5",("(1==1)*"+var).c_str(),"",1);
  return (int)(h0.GetBinContent(1));
}

//*********************************************************
int drawUtils::GetNWeights(TTree* tree){
//*********************************************************
  return GetVarFromTree(tree,"NWEIGHTS");
}

//*********************************************************
int drawUtils::GetNToys(TTree* tree){
//*********************************************************

  return drawUtils::GetVarFromTree(tree,"NTOYS");
}

//*********************************************************
int drawUtils::GetRefToy(TTree* tree){
//*********************************************************

  return drawUtils::GetVarFromTree(tree,"toy_ref");
}

//*********************************************************
std::string drawUtils::GetString(int code){
//*********************************************************

  std::stringstream scode;
  scode << code;

  return scode.str();
}

//*********************************************************
std::string drawUtils::GetString(double code){
//*********************************************************

  std::stringstream scode;
  scode << code;

  return scode.str();
}


//*********************************************************
void drawUtils::NormalizeVariableBinning(TH1* h, int mode, const std::string& uopt, Float_t& wmin){
//*********************************************************

  // mode = 0 --> do nothing
  // mode = 1 --> Normalize content
  // mode = 2 --> Normalize content and error

  //----- This is to properly normalized bins with different width -------


  // TODO. For some reason the bin error must be set, otherwise the histo appears empty when using NOVARBIN option
  if (uopt.find("NOVARBIN")!=std::string::npos || mode==0){
    for (int i=0;i<h->GetNbinsX();i++)
      h->SetBinError(i+1, h->GetBinError(i+1));
    return;
  }

  int entries = (int)h->GetEntries();

  // Get the bin with minimum width
  if (uopt.find("USEMINWIDTH")==std::string::npos){
    wmin=1e6;
    for (int i=0;i<h->GetNbinsX();i++){
      float w = h->GetBinWidth(i+1);
      if (w<wmin && h->GetBinContent(i+1)>0) wmin=w;
    }
  }

  float ymax=-1e16;
  
  // That bin will have normalization 1 and the others with width w normalization wmin/w
  for (int i=0;i<h->GetNbinsX();i++){
    float w = h->GetBinWidth(i+1);
    float y = h->GetBinContent(i+1);

    if (y==0) continue;
    float yp = y*(wmin/w);
    h->SetBinContent(i+1,yp);

    if (y>ymax) ymax=y;

    // normalize the bin error when requested
    if (mode==2){
      float e = h->GetBinError(i+1);
      float ep = e*(wmin/w);
      h->SetBinError(i+1,ep);

    }
  }

  // must set the original saved number of entries. Otherwise it will use the normalized one
  h->SetEntries(entries);
}

//*********************************************************
void drawUtils::NormalizeVariableBinning(TH1* h, int mode, const std::string& uopt){
//*********************************************************
  
  Float_t min_width;
  NormalizeVariableBinning(h,mode,uopt,min_width);
}

//*********************************************************
bool drawUtils::ContainValidOptions(const std::string& uopt){
//*********************************************************

  char c=' ';
  size_t pos = 0;
  int pos_ini = 0;
  bool valid=true;
  while(pos!=std::string::npos){
    pos = uopt.find(c,pos_ini);
    std::string single_opt = uopt.substr(pos_ini, pos-pos_ini);      
    if (single_opt!=""){
      if (!IsValidOption(single_opt)) valid=false;
    }
    pos_ini=pos+1;
  }

  return valid;
}

//*********************************************************
bool drawUtils::CheckOption(const std::string& uopt, const std::string& this_opt){
//*********************************************************

  if (!IsValidOption(this_opt)) return false;
  if (uopt.find(this_opt)!= std::string::npos) return true;
  return false;
}

//*********************************************************
bool drawUtils::CheckInternalOption(const std::string& uopt, const std::string& this_opt){
//*********************************************************

  if (uopt.find(this_opt)!= std::string::npos) return true;
  return false;
}

//*********************************************************
bool drawUtils::CheckOption(const std::string& uopt, OptionEnum this_opt){
//*********************************************************
	
  if (uopt.find(ConvertOption(this_opt))!= std::string::npos) return true;
  return false;
}

//*********************************************************
std::string drawUtils::ConvertOption(OptionEnum opt){
//*********************************************************

  if      (opt==kNOERROR)  return "NOERROR";
  else if (opt==kAREA)     return "AREA";
  else return "";
}

//*********************************************************
void drawUtils::ExplainOption(OptionEnum opt){
//*********************************************************

  return ExplainOption(ConvertOption(opt));
}

//*********************************************************
void drawUtils::ExplainOption(const std::string& opt){
//*********************************************************
  
  if (!IsValidOption(opt)) return;

  char opt2[256];  
  sprintf(opt2,"%-20s", (opt+":").c_str());
  std::cout << opt2 << _options[opt] << std::endl;
}

//*********************************************************
void drawUtils::BuildOptions(){
//*********************************************************


  drawUtils::AddOption("SCALETODATA","Scale the MC covariance matrix to data POT when using Experiment");
  drawUtils::AddOption("NOCHOP",    "??");
  drawUtils::AddOption("E0",        "Error style option for second sample. Does the same as the root option e0");
  drawUtils::AddOption("E1",        "Error style option for second sample. Does the same as the root option e1");
  drawUtils::AddOption("E2",        "Error style option for second sample. Does the same as the root option e2");
  drawUtils::AddOption("E3",        "Error style option for second sample. Does the same as the root option e3");
  drawUtils::AddOption("E4",        "Error style option for second sample. Does the same as the root option e4");
  drawUtils::AddOption("E5",        "Error style option for second sample. Does the same as the root option e5");
  drawUtils::AddOption("E6",        "Error style option for second sample. Does the same as the root option e6");
  drawUtils::AddOption("ETOT",      "When drawing with color codes, draw also the total error for the stacked histogram");
  drawUtils::AddOption("NOTOTERROR","Don't draw the total error (stat+syst) but only the statistical or systematic error");
  drawUtils::AddOption("NOSTERROR", "Don't draw the statistical error but only the total error, and also the systematic error when SYSTERROR options is also used");
  drawUtils::AddOption("SYSTERROR", "Don't draw the statistical error but only the systematic error, together with the total");
  drawUtils::AddOption("DUMP",      "Print on the secreen the histo or graph contents");
  drawUtils::AddOption("NODRAW",    "Don't draw anything. You should use the option DUMP to see the results");
  drawUtils::AddOption("ST",        "When using DrawErrors or DrawRelativeErrors only statistical errors are plotted. In combination with SYS, SSYS or WSYS adds statisticak and systematic errors in quadrature");
  //  drawUtils::AddOption("DIS",       "error bars correspond to the dispersion of all toy experiments");
  drawUtils::AddOption("SYS",       "draw all systematics. Error bars correspond to the RMS of all toy experiments");
  drawUtils::AddOption("SSYS",      "Draw only standard systematics (reconstructed observable variations)");
  drawUtils::AddOption("WSYS",      "Draw only weight systematics");
  drawUtils::AddOption("WSi",       "Draw only the specified (ith) weight systematic(s) (\" WS0  WS3 ... \")");
  drawUtils::AddOption("NWSi",      "Exclude the specified (ith) weight systematic(s) (\"NWS0 NWS1 ... \")");
  drawUtils::AddOption("WCi",       "Draw only the specified (ith) weight correction(s) (\" WC0  WC3 ... \")");
  drawUtils::AddOption("NWCi",      "Exclude the specified (ith) weight correction(s) (\"NWC0 NWC1 ... \")");
  drawUtils::AddOption("WCORR",     "Apply Weight corrections only (not weight systematics)");
  //  drawUtils::AddOption("LOW",       "In combination with WSYS (and WS0, WS1, ..) plots only the low error"); 
  //  drawUtils::AddOption("HIGH",      "In combination with WSYS (and WS0, WS1, ..) plots only the high error"); 
  //  drawUtils::AddOption("REFTOY",    "For a microtree with multiple toys plot only the reference toy (only works for microtrees create with highlandTools above v0r29)");
  drawUtils::AddOption("NOLEG",     "No legend is drawn");
  drawUtils::AddOption("PUR",       "The purity is printed on the screen and on the legend");
  drawUtils::AddOption("EFF",       "The efficiency-like ratio plot (out of two histograms, pass and total) is drawn, internal option");
  drawUtils::AddOption("NOST2",     "The stat box for the second sample is not shown");
  drawUtils::AddOption("NOSTAT",    "No stat box is shown");
  drawUtils::AddOption("NOMIN",     "The minimum in Y is not used");
  drawUtils::AddOption("NOVARBIN",  "Plotting with variable binning, bin entries are normalized by bin width. This option disables this feature.");
  drawUtils::AddOption("NOTOYW",    "No toy weight is applied for toy experiments. All toys will have the same weight 1.");
  drawUtils::AddOption("NOW",       "None of the weights (flux and pileup) are applied to the event");
  drawUtils::AddOption("NOFLUXW",   "No flux weight is applied to the event");
  drawUtils::AddOption("NOPILEUPW", "No pile up weight is applied to the event");
  drawUtils::AddOption("NODATA",    "The data is not shown in the plot");
  drawUtils::AddOption("NOMC",      "The MC is not shown in the plot");
  drawUtils::AddOption("NODEFAULT", "By default the -999 default is plotted unless this option is used");
  drawUtils::AddOption("UNDER",     "Underflow entries are added to the first bin");
  drawUtils::AddOption("OVER",      "Overflow entries are added to the last bin");
  drawUtils::AddOption("PROFX",     "For 2D histos a Profile histogram in X is drawn");
  drawUtils::AddOption("PROFY",     "For 2D histos a Profile histogram in Y is drawn");
  drawUtils::AddOption("AREA",      "For 1D histos normalization to 1 (if there is only one sample) or to the area of the first sample. In this normalization procedure the whole range of the variable allowed by 'cut' parameter is taken into account");
  drawUtils::AddOption("AREA1",     "For 1D histos normalization to 1. In this normalization procedure the whole range of the variable allowed by 'cut' parameter is taken into account");
  drawUtils::AddOption("AREA100",   "For 1D histos normalization to 100. In this normalization procedure the whole range of the variable allowed by 'cut' parameter is taken into account");
  drawUtils::AddOption("POTNORM",   "Normalize the MC to the POT given as input normalization factor, regardless of the data POT (interprete valid norm factor as a POT value)");
  drawUtils::AddOption("IGNOREEMPTY", "Do not put categories without entries in the legend");
  drawUtils::AddOption("DRAWALLMC", "Draw with black contour a histogram for all MC entries (works only while plotting histogram stack)");
  drawUtils::AddOption("DRAWALLCUTS", "Draw all cuts even if they are below the MinAccumLevelToSave when doing plots VSCuts");
  drawUtils::AddOption("NOINFO",    "Dont dump on the screen histogram info when using the Draw methods");
  drawUtils::AddOption("SHOWSAND",    "Show the sand mu type even when it is empty");
  drawUtils::AddOption("SHOWNOTRUTH", "Show the NO TRUTH type even when it is empty");
}

//*********************************************************
void drawUtils::AddOption(const std::string& opt, const std::string& info){
//*********************************************************

  _options[opt]=info;
}

//*********************************************************
bool drawUtils::IsValidOption(const std::string& opt){
//*********************************************************

  if (_options.find(opt)==_options.end()){

    // Exclude from check WSi and NWSi options since only WS and NWS are declared as options
    if      (opt.find("WS")==0  && opt.find_first_not_of("0123456789",2)==std::string::npos)  return true;
    else if (opt.find("NWS")==0 && opt.find_first_not_of("0123456789",3)==std::string::npos)  return true;
    else if (opt.find("WC")==0  && opt.find_first_not_of("0123456789",2)==std::string::npos)  return true;
    else if (opt.find("NWC")==0 && opt.find_first_not_of("0123456789",3)==std::string::npos)  return true;

    std::cout << "Invalid option '" << opt << "' !!!!" << std::endl;
    return false;
  }
  return true;
}


//*********************************************************
void drawUtils::ListOptions(){
//*********************************************************

  for (std::map<std::string, std::string>::iterator it=_options.begin();it!=_options.end();it++){
    ExplainOption(it->first);    
  }
}

//*********************************************************
void drawUtils::AddLegendEntry(TLegend* leg, TObject* ht, const std::string&type, const std::string& opt) { 
//*********************************************************

  leg->AddEntry(ht, type.c_str(), opt.c_str());

  if (legendEntryHeight>0){
    if (leg->GetY1NDC()<0.001)
      leg->SetY1(leg->GetY1()-legendEntryHeight);
    else
      leg->SetY1NDC(leg->GetY1NDC()-legendEntryHeight);
  }
}

