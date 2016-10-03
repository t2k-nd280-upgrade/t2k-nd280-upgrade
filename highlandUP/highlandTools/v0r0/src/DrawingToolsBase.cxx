#include "DrawingToolsBase.hxx"
#include "WeightTools.hxx"
#include "HEPConstants.hxx"
#include <TVirtualHistPainter.h>
#include <THistPainter.h>
#include "SetT2KStyle.H"
#include <TArrow.h>
#include <TColor.h>
#include <TLatex.h>
#include <TStyle.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cerrno>

int auto_colors[NAUTOCOLORS]  = { 1, 2 ,3 ,4 , 6 ,7, 8, 9, 31, 32};
int auto_markers[NAUTOCOLORS]  = { 20, 21, 22, 23, 33, 34, 29, 24, 25, 26};
int auto_styles[NAUTOCOLORS] = {3004, 3005, 3006, 3007, 3013, 3001, 3352, 3325, 3357, 3375};
const int NMAXTOYS=1000;

//********************************************************************
DrawingToolsBase::DrawingToolsBase(const std::string& file, Int_t T2KstyleIndex){
//********************************************************************

  _config_file ="";

  // Load the categories from the file, and any other information stored
  // in the "config" tree.
  if (file!=""){
    ReadConfig(file);
  }

  gStyle->SetStatFormat("6.6g"); // no scientifiic notation up to 10e6

  if (T2KstyleIndex==1) {
    // OLD T2K style

    // use plain black on white colors
    gStyle->SetFrameBorderMode(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetPadBorderMode(0);
    gStyle->SetPadColor(0);
    gStyle->SetCanvasColor(0);
    gStyle->SetStatColor(0);
    gStyle->SetLegendBorderSize(1);

    // set the paper & margin sizes
    gStyle->SetPaperSize(20,26);
    gStyle->SetPadTopMargin(0.05);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadBottomMargin(0.16);
    gStyle->SetPadLeftMargin(0.13);

    // use large Times-Roman fonts
    gStyle->SetTextFont(132);
    gStyle->SetTextSize(0.08);
    gStyle->SetLabelFont(132,"x");
    gStyle->SetLabelFont(132,"y");
    gStyle->SetLabelFont(132,"z");
    gStyle->SetLabelSize(0.05,"x");
    gStyle->SetTitleSize(0.06,"x");
    gStyle->SetLabelSize(0.05,"y");
    gStyle->SetTitleSize(0.06,"y");
    gStyle->SetLabelSize(0.05,"z");
    gStyle->SetTitleSize(0.06,"z");
    gStyle->SetLabelFont(132,"t");
    gStyle->SetTitleFont(132,"x");
    gStyle->SetTitleFont(132,"y");
    gStyle->SetTitleFont(132,"z");
    gStyle->SetTitleFont(132,"t");
    gStyle->SetStatFont(132);
    gStyle->SetTitleFillColor(0);
    gStyle->SetTitleX(0.25);
    gStyle->SetTitleFontSize(0.08);
    gStyle->SetTitleFont(132,"pad");

    // get rid of X error bars and y error bar caps
    //gStyle->SetErrorX(0.001);


    // put tick marks on top and RHS of plots
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);


    // Define a nicer color palette (red->blue)
    // Uncomment these lines for a color palette (default is B&W)
    gStyle->SetPalette(1,0);  // use the nice red->blue palette
    const Int_t NRGBs = 5;
    const Int_t NCont = 255;

    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
  }
  else if (T2KstyleIndex>1) {
    if (T2KstyleIndex>4){std::cout << "DrawingTools. Not existing style !!!!" << std::endl;}
    // Official T2K style as described in http://www.t2k.org/comm/pubboard/style/index_html
    TString localStyleName = "T2K";
    // -- WhichStyle --
    // 1 = presentation large fonts
    // 2 = presentation small fonts
    // 3 = publication/paper
    Int_t localWhichStyle = T2KstyleIndex-1;
    
    TStyle* t2kstyle = SetT2KStyle(localWhichStyle, localStyleName);
    gROOT->SetStyle(t2kstyle->GetName());
  }

  // Set the height reserved for each entry in the legend (in NDC coordinates)
  SetLegendEntryHeight(0.05);

  // The initial legend height should be 0, since it is increased when adding an entry
  SetLegendSize(0.3,0);

  // Fix legend size for the small legend
  SetSmallLegendSize(0.15,0.1);

  // By default legend on the top-right
  SetLegendPos("tr");

  // default position for Stats box
  _statPos[0] = 1 - gStyle->GetPadRightMargin();
  _statPos[1] = 1.04 - gStyle->GetPadTopMargin();
  SetStatPos(_statPos[0],_statPos[1]);

  // Only print integral by default
  _stat_option = 1000000;

  // Default stack fill
  _stack_fill_style = 3001;
  _different_fill_styles = false;
  _marker_style = 20;
  _marker_size = 1.;
  _cut_line_col = 2;
  _cut_line_width = 3;

  // No title
  _titleX="";
  _titleY="";
  _title="";

  _data_color = kBlack;
  _allmc_color = kBlack;
  _data_label = "Data";
  _allmc_label = "MC all";
  _allmcstat_label = "MC all (stat)";
  _allmcsyst_label = "MC all (syst)";

  // The default range is 0 -> auto.
  _minY = 0;
  _maxY = 0;

  // Default to non-log plots.
  _logY = false;
  _logZ = false;

  // The default fill style.
  _fill_style = 0;
  _line_width = 1;
  _line_color = 1;
  _fill_color = 2;
  _mcerror_color = 7;
  _mcstaterror_color = 6;

  // The default maximum in the vertical axes is 0 (it means no max value);
  _maxY = 0;

  // The default relative maximum in the vertical axes is 1.1 (10%)
  _relativeMaxY = 1.1;

  // No superimposition
  _same_level=0;
  _unique = 0;

  // no legend
  _drawleg=false;

  // Set the default method for computing efficiencies
  SetDefaultEffDivideParams();

  // switch on 2p2h type for reaction histos by default
  _draw_2p2h = true;

  // define the auto colors when superimposing histograms
  for (int i=0;i<NMAXAUTOCOLORS;i++) {
    if (i<NAUTOCOLORS){
      _auto_colors[i]  = auto_colors[i];
      _auto_markers[i] = auto_markers[i];
    }
    else{
      _auto_colors[i]  = _line_color;
      _auto_markers[i] = _marker_style;
    }
  }

  _pdfcanvas = NULL;
  _bodypad = NULL;
  _headpad = NULL;

  drawUtils::BuildOptions();
}

//********************************************************************
void DrawingToolsBase::Reset(){
//********************************************************************

  for (unsigned int i=0;i<_saved_histos.size();i++)
    delete _saved_histos[i];

  _saved_histos.clear();


  for (unsigned int i=0;i<_saved_histos2D.size();i++)
    delete _saved_histos2D[i];

  _saved_histos2D.clear();


  for (unsigned int i=0;i<_saved_graphs.size();i++)
    delete _saved_graphs[i];

  _saved_graphs.clear();


  for (unsigned int i=0;i<_saved_histoStacks.size();i++)
    delete _saved_histoStacks[i];

  _saved_histoStacks.clear();

  for (unsigned int i=0;i<_cut_lines.size();i++)
    delete _cut_lines[i];

  _cut_lines.clear();


  for (unsigned int i=0;i<_legends.size();i++)
    delete _legends[i];

  _legends.clear();

}

//********************************************************************
void DrawingToolsBase::ReadConfig(const std::string& file) {
//********************************************************************

  _config_file = file;

  ReadCategories(file);
  ReadSelections(file);
  ReadDocStrings(file);
  ReadCorrections(file);
  ReadSystematics(file);
  ReadConfigurations(file);
  ReadOther(file);
  ND::versioning().ReadVersions(file);

  _configTree_tools.Initialize(syst(),conf());

}

//*********************************************************
void DrawingToolsBase::SetLegendParam(double a, double b, double c, double d){
//*********************************************************

  // a = x min (in percentage, from 0 to 1)
  // b = y min
  // c = x max
  // d = y max

  _legendParam[0]=a;
  _legendParam[1]=b;
  _legendParam[2]=c;
  _legendParam[3]=d;
}

//*********************************************************
void DrawingToolsBase::SetLegendPos(double posX, double posY){
//*********************************************************

  _legendPosS = "";
  if (posX != -999) _legendPos[0] = posX;
  if (posY != -999) _legendPos[1] = posY;
  SetLegendParam(_legendPos[0],_legendPos[1],_legendPos[0]+_legendSize[0], _legendPos[1]+_legendSize[1]);
}

//*********************************************************
void DrawingToolsBase::SetLegendPos(std::string pos){
//*********************************************************

  if (pos.find("r") != std::string::npos)
    SetLegendPos(1-gStyle->GetPadRightMargin()-_legendSize[0],_legendPos[1]);
  else if (pos.find("l") != std::string::npos)
    SetLegendPos(gStyle->GetPadLeftMargin() + 0.02,_legendPos[1]);
  else if (pos.find("c") != std::string::npos)
    SetLegendPos((1-gStyle->GetPadRightMargin()+gStyle->GetPadLeftMargin()-_legendSize[0])/2.,_legendPos[1]);

  if (pos.find("t") != std::string::npos)
    SetLegendPos(_legendPos[0],(1-gStyle->GetPadTopMargin()-_legendSize[1]));
  else if (pos.find("b") != std::string::npos)
    SetLegendPos(_legendPos[0],gStyle->GetPadBottomMargin()+0.02);

  // a standard position has been set
  _legendPosS = pos;
}

//*********************************************************
void DrawingToolsBase::SetLegendSize(double w, double h){
//*********************************************************

  if (w != -999) _legendSize[0] = w;
  if (h != -999) _legendSize[1] = h;

  if (_legendPosS!="")
    SetLegendPos(_legendPosS);
  else
    SetLegendParam(_legendPos[0],_legendPos[1],_legendPos[0]+_legendSize[0], _legendPos[1]+_legendSize[1]);
}

//*********************************************************
void DrawingToolsBase::SetStatPos(double x, double y){
//*********************************************************
  if (x != -999) _statPos[0]=x;
  if (y != -999) _statPos[1]=y;
  gStyle->SetStatX(_statPos[0]);
  gStyle->SetStatY(_statPos[1]);
}

//*********************************************************
void DrawingToolsBase::SetEffDivideParams(const std::string& params) {
//*********************************************************
  if(params.find_first_not_of(" ") == std::string::npos){
    std::cout << " DrawingToolsBase::SetEffDivideParams() no options provided --> setting default " << std::endl;
    SetDefaultEffDivideParams();
    return;
  }
  _eff_params = params;
}

//*********************************************************
void DrawingToolsBase::SetDefaultEffDivideParams(){
//*********************************************************
  // default params for efficiency calculation (TGraphAsymmErrors::Divide)
  // we want the efficiency estimation to be by default equal passed/total and work for events/histograms with weights as well
  // so use bayesian intervals with beta params (prior): alpha = 1, beta = 1,  this gives a flat prior
  // confidence level is set to one sigma
  _eff_params = "cl=0.683 b(1,1) mode";
}

//*********************************************************
TH1* DrawingToolsBase::Draw(TTree* tree, const std::string& var, int nx, double xmin, double xmax, const std::string& categ,
                            const std::string& cut, const std::string& root_opt, const std::string& opt, double norm, bool scale_errors){
//*********************************************************

  // Draw 1D histogram for a given set of cuts and uniform binning
  double xbins[NMAXBINS];
  return Draw(tree, var, nx, GetVariableBins(nx,xmin,xmax,xbins), categ, cut, root_opt, opt, norm, scale_errors);
}

//*********************************************************
TH1* DrawingToolsBase::Draw(TTree* tree, const std::string& var, int nx, double* xbins, const std::string& categ,
                            const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm,  bool scale_errors){
//*********************************************************

  // Draw 1D histogram for a given set of cuts and variable binning
  int ny=0;
  double* ybins=NULL;
  return Draw(tree,var,nx,xbins,ny,ybins,categ,cut, root_opt, opt,norm,scale_errors);
}

//*********************************************************
TH1* DrawingToolsBase::Draw(TTree* tree, const std::string& var, int nx, double xmin, double xmax, int ny, double ymin, double ymax,
                            const std::string& categ, const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm,bool scale_errors){
//*********************************************************

  // Draw 2D histogram for a given set of cuts and uniform binning
  double xbins[NMAXBINS];
  double ybins[NMAXBINS];
  return Draw(tree, var, nx, GetVariableBins(nx,xmin,xmax,xbins), ny, GetVariableBins(ny,ymin,ymax,ybins), categ, cut, root_opt, opt, norm, scale_errors);
}

//*********************************************************
void DrawingToolsBase::Draw(TTree* tree, const std::string& var, int nx, double xmin, double xmax, int ny, double ymin, double ymax,
                            const std::string& idVar, std::vector<int> idSel, std::vector<std::string> idName, std::vector<int> idColor,
                            const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm,  bool scale_errors){
//*********************************************************
  double xbins[NMAXBINS];
  double ybins[NMAXBINS];
  Draw(tree, var, nx, GetVariableBins(nx,xmin,xmax,xbins), ny, GetVariableBins(ny,ymin,ymax,ybins), idVar,idSel,idName,idColor,cut,root_opt,opt,norm,scale_errors);
}

//*********************************************************
void DrawingToolsBase::Project(HistoStack* hs1, HistoStack* hs2, TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins, int ny, double* ybins, const std::string& categ,
                               const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm2,  bool scale_errors){
//*********************************************************

  ProjectNew(hs1,hs2,tree1,tree2,var,nx,xbins,ny,ybins,categ,cut,cut,root_opt,opt,norm2,scale_errors);
}

//*********************************************************
void DrawingToolsBase::ProjectNew(HistoStack* hs1, HistoStack* hs2, TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins, int ny, double* ybins, const std::string& categ,
                                  const std::string& cut1, const std::string& cut2,  const std::string& root_opt, const std::string& opt, double norm2,  bool scale_errors){
//*********************************************************

  double norm1=1;

  std::string uopt = drawUtils::ToUpper(opt);

  // Project both trees (first sample with no systematics)
  //  Project(hs1, "sample1", tree1,var,nx,xbins,ny,ybins,"all",cut1,root_opt,opt+" NOSYS NOAREA",norm1,scale_errors);
  Project(hs1, "sample1", tree1,var,nx,xbins,ny,ybins,"all",cut1,root_opt,opt+"       NOAREA",norm1,scale_errors);
  Project(hs2, "sample2", tree2,var,nx,xbins,ny,ybins,categ,cut2,root_opt,opt+"       NOAREA",norm2,scale_errors);

  if(drawUtils::CheckOption(uopt,"AREA")){
    hs1->NormalizeByArea(uopt);
    hs2->NormalizeByArea(uopt,hs1->GetTotal1D()->GetSumOfWeights());
  }
}

//*********************************************************
void DrawingToolsBase::Project(HistoStack* hs, const std::string& sample_name, TTree* tree, const std::string& var, int nx, double* xbins, int ny, double* ybins, const std::string& categ,
                               const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm,  bool scale_errors){
//*********************************************************

  // Project 1D (ny=0) or 2D histogram for a given set of cuts and variable binning

  // Read the categories from the config tree
  ReadCategories(_config_file);

  // Check that the category exists and dump category info
  // (don't dump for each Experiment class samples)
  if (! drawUtils::CheckInternalOption(opt,"EXP"))
      if ( ! HasCategory(categ)) return;

  std::string uopt = drawUtils::ToUpper(opt);
  std::string uroot_opt = drawUtils::ToUpper(root_opt);

  //-------- Project the total ------------------------------------------
  if (ny==0){
    TH1F* hsyst = NULL;
    TH1F* hall = GetHisto(hs, tree, GetUniqueName(sample_name+"_all"),var,nx,xbins,cut,root_opt,uopt,hsyst,norm,scale_errors);
    hs->AddTotal(hall,hsyst);
    _saved_histos.pop_back();
    delete hall;
  }
  else{
    TH2F* hall = GetHisto(tree, GetUniqueName(sample_name+"_all"),var,nx,xbins,ny,ybins,cut, root_opt, uopt,norm,scale_errors);
    hs->AddTotal(hall);
    _saved_histos.pop_back();
    delete hall;
  }

  //-------- Project the track categories -----------------------

  if (categ!="all"){

    std::string categ2 = categ;
    std::string categ_withoutPur = categ;

    // Print purities when requested, and create categories with % on the name
    if (drawUtils::CheckOption(uopt,"PUR")) {

      // if Experiment class, categories might already be "_withPurities"
      if (drawUtils::CheckInternalOption(uopt,"EXP")) {
        size_t pos = categ.find("_withPurities");
        if (pos != std::string::npos) categ_withoutPur = categ.substr(0,pos);
        else                          categ_withoutPur = categ;

      // otherwise create them
      } else {
        std::string cut_range = AddRangeCut(var,nx,xbins,ny,ybins,cut,uopt);
        PrintPurities(tree, categ, cut_range);
        categ2 += "_withPurities";
      }

    } // end if PUR

    std::vector<TrackTypeDefinition>::iterator it;
    int i=0;
    for (it=cat().GetCategoryTypes(categ2).begin();it!=cat().GetCategoryTypes(categ2).end();it++, i++){
      std::string type = it->_name;
      std::string code = drawUtils::GetString(it->_code);
      int color = it->_color;

      if(!_draw_2p2h && type=="2p2h") continue; // Ignore 2P2H when requested

      std::string cut2 = categ_withoutPur + "==" + code;

      if (cut!="") cut2 = cut + " && " + cut2;

      if (ny==0){
        TH1F* ht1 = GetHisto(hs, tree,GetUniqueName(sample_name+"_"+type),var,nx,xbins,cut2, root_opt,uopt+" CAT",norm,scale_errors);
        if( drawUtils::CheckOption(uopt,"IGNOREEMPTY") && ht1->Integral() == 0.)                                              continue; // Ignore empty categories in the legend
        if(!drawUtils::CheckOption(uopt,"SHOWSAND")    && ht1->Integral() == 0. && type.find(NAMESAND)   !=std::string::npos) continue; // Ignore sand mu when requested
        if(!drawUtils::CheckOption(uopt,"SHOWNOTRUTH") && ht1->Integral() == 0. && type.find(NAMENOTRUTH)!=std::string::npos) continue; // Ignore no truth when requested

        //drawUtils::NormalizeVariableBinning(ht1,2,uopt);
        if (hs->GetCurrentSystGroup()=="default")
            hs->Add(ht1,color,_line_width,color,GetFillStyle(i),type.c_str());
        _saved_histos.pop_back();
        delete ht1;
      }
      else{
        TH2F* ht2 = GetHisto(tree,GetUniqueName(sample_name+"_"+type),var,nx,xbins,ny,ybins,cut2,root_opt,uopt+" CAT",norm,scale_errors);
        drawUtils::NormalizeVariableBinning(ht2,2,uopt);
        if (!uroot_opt.find("BOX") == std::string::npos)
          hs->Add(ht2,0,color,type.c_str());
        else
          hs->Add(ht2,color,color,type.c_str());
        _saved_histos.pop_back();
        delete ht2;
      }
    }
  }

  if(drawUtils::CheckOption(uopt,"AREA"))
    hs->NormalizeByArea(uopt);
}

//*********************************************************
TH1* DrawingToolsBase::Draw(TTree* tree, const std::string& var, int nx, double* xbins, int ny, double* ybins, const std::string& categ,
                            const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm2,  bool scale_errors){
//*********************************************************

  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return NULL;


  // Create the empty HistoStack
  HistoStack* hs = new HistoStack(_title,_titleX,_titleY);
  _saved_histoStacks.push_back(hs);

  //  std::string slevel = GetSameLevel(root_opt);

  // Project the trees
  Project(hs, "", tree,var,nx,xbins,ny,ybins,categ,cut,root_opt,opt,norm2,scale_errors);

  //Draw the HistoStack
  DrawHistoStack(hs,categ,root_opt, uopt);

  if (ny==0)
    return hs->GetTotal1D();
  else
    return hs->GetTotal2D();

}

//*********************************************************
void DrawingToolsBase::Project(HistoStack* hs, TTree* tree, const std::string& var, int nx, double* xbins, int ny, double* ybins,
                               const std::string& idVar, std::vector<int> idSel, std::vector<std::string> idName, std::vector<int> idColor,
                               const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm,  bool scale_errors) {
//*********************************************************
  //-------- Draw the total ------------------------------------------
  //  std::string slevel = GetSameLevel(root_opt);
  std::string uopt = drawUtils::ToUpper(opt);
  std::string uroot_opt = drawUtils::ToUpper(root_opt);

  if (ny == 0) {
    TH1F* hall = GetHisto(tree, "all", var, nx, xbins, cut, root_opt, uopt, norm, scale_errors);
    drawUtils::NormalizeVariableBinning(hall, 1, uopt);
    hs->SetTotal(hall);
  } else {
    TH2F* hall = GetHisto(tree, "all", var, nx, xbins, ny, ybins, cut, root_opt, uopt, norm, scale_errors);
    hs->SetTotal(hall);
  }

  //-------- Draw the track categories -----------------------
  for (unsigned int i = 0; i < idSel.size(); i++) {
    std::string type = idName[i];
    std::stringstream ss;
    ss << idSel[i];
    int color = idColor[i];

    if(!_draw_2p2h && type=="2p2h") continue; // Ignore 2P2H when requested

    std::string cut2 = idVar + "==" + ss.str();
    if (cut != "")
      cut2 = cut + " && " + cut2;

    if (ny == 0) {
      TH1F* ht1 = GetHisto(tree, type, var, nx, xbins, cut2, root_opt, uopt + " CAT", norm, scale_errors);
      if( drawUtils::CheckOption(uopt,"IGNOREEMPTY") && ht1->Integral() == 0.)                       continue; // Ignore empty categories in the legend
      if(!drawUtils::CheckOption(uopt,"SHOWSAND")    && ht1->Integral() == 0. && type==NAMESAND    ) continue; // Ignore sand mu when requested
      if(!drawUtils::CheckOption(uopt,"SHOWNOTRUTH") && ht1->Integral() == 0. && type==NAMENOTRUTH ) continue; // Ignore no truth when requested

      ht1->SetFillStyle(GetFillStyle(i));
      drawUtils::NormalizeVariableBinning(ht1, 1, uopt);
      hs->Add(ht1, color, _line_width, color, GetFillStyle(i), type);
      if (_drawleg && !drawUtils::CheckOption(uopt,"NOLEG"))
        drawUtils::AddLegendEntry(_legends.back(), ht1, type, "f");
    } else {
      TH2F* ht2 = GetHisto(tree, type, var, nx, xbins, ny, ybins, cut2, root_opt, uopt + " CAT", norm, scale_errors);
      drawUtils::NormalizeVariableBinning(ht2, 1, uopt);
      if (!uroot_opt.find("BOX") == std::string::npos)
        hs->Add(ht2, 0, color);
      else
        hs->Add(ht2, color, color);
      if (_drawleg && !drawUtils::CheckOption(uopt,"NOLEG"))
        drawUtils::AddLegendEntry(_legends.back(), ht2, type, "f");
    }
  }
}

//*********************************************************
void DrawingToolsBase::Draw(TTree* tree, const std::string& var, int nx, double* xbins, int ny, double* ybins,
                            const std::string& idVar, std::vector<int> idSel, std::vector<std::string> idName, std::vector<int> idColor,
                            const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm2,  bool scale_errors){
//*********************************************************

  // Draw 1D (ny=0) or 2D histogram for a given set of cuts and variable binning

  //  std::string slevel = GetSameLevel(root_opt);
  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;


  std::string uroot_opt = drawUtils::ToUpper(root_opt);

  // Create the legend
  if (!drawUtils::CheckOption(uopt,"NOLEG"))
    CreateLegend();

  HistoStack* hs = new HistoStack(_title,_titleX,_titleY);
  _saved_histoStacks.push_back(hs);

  Project(hs, tree, var, nx, xbins, ny, ybins, idVar, idSel, idName, idColor, cut, root_opt, opt, norm2, scale_errors);

  //Draw the HistoStack
  DrawHistoStack(hs,"",root_opt, uopt,"",2);

  // Draw the legend
  if (!drawUtils::CheckOption(uopt,"NOLEG"))
    _legends.back()->Draw();

  if (!drawUtils::CheckOption(uopt,"NODRAW"))
    gPad->Update();
}


//*********************************************************
void DrawingToolsBase::Draw(TTree* tree_mc, TTree* tree_data, const std::string& var, int nx, double* xbins, int ny, double* ybins,
                            const std::string& idVar, std::vector<int> idSel, std::vector<std::string> idName, std::vector<int> idColor,
                            const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm,  bool scale_errors){
//*********************************************************

  // Draw 1D (ny=0) or 2D histogram for a given set of cuts and variable binning

  //  std::string slevel = GetSameLevel(root_opt);
  std::string uopt = drawUtils::ToUpper(opt);
  std::string uroot_opt = drawUtils::ToUpper(root_opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;


  // Normalize to a given area
  double norm_data = 1.;
  if(drawUtils::CheckOption(uopt,"AREA")) {
    double entries_mc = GetEntries(tree_mc,cut,var,opt);
    double entries_data = GetEntries(tree_data,cut,var,opt);

    double area = entries_data;
    if     (drawUtils::CheckOption(uopt,"AREA100")) area = 100.;
    else if(drawUtils::CheckOption(uopt,"AREA1"))   area = 1.;

    if(entries_mc > 0.) norm = area/entries_mc; // norm for MC
    if(entries_data > 0.) norm_data = area/entries_data; // norm for data
    scale_errors = true;
  }

  // Create the legend
  if (!drawUtils::CheckOption(uopt,"NOLEG"))
    CreateLegend();

  //
  //data
  //

  TH1F* hdata = GetHisto(tree_data,"data",var,nx,xbins,cut, root_opt, uopt,norm_data,scale_errors);
  drawUtils::NormalizeVariableBinning(hdata,1,uopt);

  std::string leg = "data";

  if (_drawleg &&  drawUtils::CheckOption(uopt,"NOLEG"))
    drawUtils::AddLegendEntry(_legends.back(), hdata, leg,"LE1P");


  //
  // mc
  //

  HistoStack* hs = new HistoStack(_title,_titleX,_titleY);
  _saved_histoStacks.push_back(hs);



  //-------- Draw the total ------------------------------------------

  if (ny==0){

    TH1F* hall = GetHisto(tree_mc, "mcall",var,nx,xbins,cut, root_opt, uopt,norm,scale_errors);
    drawUtils::NormalizeVariableBinning(hall,1,uopt);

    hs->SetTotal(hall);
  }
  else{
    TH2F* hall = GetHisto(tree_mc,"mcall",var,nx,xbins,ny,ybins,cut, root_opt, uopt,norm,scale_errors);
    hs->SetTotal(hall);
  }

  //-------- Draw the track categories -----------------------
  for (unsigned int i = 0; i < idSel.size();i++ ) {
    std::string type = idName[i];
    std::stringstream ss;
    ss << idSel[i];
    int color = idColor[i];

    if(!_draw_2p2h && type=="2p2h") continue; // Ignore 2P2H when requested

    std::string cut2 = idVar+"=="+ss.str();
    if (cut!="") cut2 = cut+" && "+cut2;

    if (ny==0){

      TH1F* ht1 = GetHisto(tree_mc,type,var,nx,xbins,cut2, root_opt,uopt+" CAT",norm,scale_errors);
      if( drawUtils::CheckOption(uopt,"IGNOREEMPTY") && ht1->Integral() == 0.)                      continue; // Ignore empty categories in the legend
      if(!drawUtils::CheckOption(uopt,"SHOWSAND")    && ht1->Integral() == 0. && type==NAMESAND   ) continue; // Ignore sand mu when requested
      if(!drawUtils::CheckOption(uopt,"SHOWNOTRUTH") && ht1->Integral() == 0. && type==NAMENOTRUTH) continue; // Ignore no truth when requested

      ht1->SetFillStyle(GetFillStyle(i));
      drawUtils::NormalizeVariableBinning(ht1,1,uopt);

      hs->Add(ht1,color,1,color,GetFillStyle(i),type);

      if (_drawleg &&  !drawUtils::CheckOption(uopt,"NOLEG")) {
        drawUtils::AddLegendEntry(_legends.back(), ht1, type, "f");
      }

    }
    else{
      TH2F* ht2 = GetHisto(tree_mc,type,var,nx,xbins,ny,ybins,cut2,root_opt,uopt+" CAT",norm,scale_errors);
      drawUtils::NormalizeVariableBinning(ht2,1,uopt);

      if (!uroot_opt.find("BOX") == std::string::npos)
        hs->Add(ht2,0,color);
      else
        hs->Add(ht2,color,color);
      if (_drawleg &&  !drawUtils::CheckOption(uopt,"NOLEG"))
        drawUtils::AddLegendEntry(_legends.back(), ht2, type, "f");
    }
  }

  //Draw the HistoStack

  if(ny==0 && ( hdata->GetMaximum() + sqrt(hdata->GetBinError(hdata->GetMaximumBin()))) > hs->GetTotal1D()->GetMaximum()) { // draw "higher" histogram firstly
    DrawHisto(hdata, 1, 1, 0, 0, root_opt+"error", "ST NOSTAT ISDATA", "", 2);
    DrawHistoStack(hs,"",root_opt+" same", uopt,"",2);
    DrawHisto(hdata, 1, 1, 0, 0, root_opt+" same error", "ST NOSTAT ISDATA", "", 2);
  }
  else {
    DrawHistoStack(hs,"",root_opt, uopt,"",2);
    DrawHisto(hdata, 1, 1, 0, 0, root_opt+" same error", "ST NOSTAT ISDATA", "", 2);
  }

  // Draw the legend
  if (!drawUtils::CheckOption(uopt,"NOLEG"))
    _legends.back()->Draw();

  if (drawUtils::CheckOption(uopt,"NODRAW"))
    gPad->Update();
}


//*********************************************************
void DrawingToolsBase::Draw(TTree* tree_mc, TTree* tree_data, const std::string& var, int nx, double xmin, double xmax, int ny, double ymin, double ymax,
                            const std::string& idVar, std::vector<int> idSel, std::vector<std::string> idName, std::vector<int> idColor,
                            const std::string& cut,  const std::string& root_opt, const std::string& opt, double norm,  bool scale_errors){
//*********************************************************

  double xbins[NMAXBINS];
  double ybins[NMAXBINS];

  Draw(tree_mc, tree_data, var, nx, GetVariableBins(nx,xmin,xmax,xbins), ny, GetVariableBins(ny,ymin,ymax,ybins), idVar,idSel,idName,idColor,cut,root_opt,opt,norm,scale_errors);
}

//*********************************************************
double DrawingToolsBase::GetEff(TTree* tree, const std::string& var, double xmin, double xmax,
             const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt) {
//*********************************************************
  double errhigh = 0.;
  double errlow = 0.;
  return GetEff(tree, var, xmin, xmax, errlow, errhigh, cut1, cut2, root_opt, opt);
}

//*********************************************************
double DrawingToolsBase::GetEff(TTree* tree, const std::string& var, double xmin, double xmax, double& errlow, double& errhigh,
             const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt) {
//*********************************************************

  errlow = 0;
  errhigh = 0;
  //  std::string slevel = GetSameLevel(root_opt);
  int nbins = 1;
  double xbins[2] = {xmin, xmax};

  // Histogram for cut1+cut2 (numerator)
  TH1F* h1 = GetHisto(tree, "eff1",var,nbins,xbins,(cut1+" && "+cut2), root_opt, opt,1);
  h1->Sumw2();
  // Histogram for cut2 (denominator)
  TH1F* h2 = GetHisto(tree, "eff2",var,nbins,xbins,cut2, root_opt, opt,1);
  h2->Sumw2();

  // compute the efficiency
  TGraphAsymmErrors* eff = new TGraphAsymmErrors(h1);
  _saved_graphs.push_back(eff);
  eff->Divide(h1, h2, _eff_params.c_str()); //the options are explicitely provided by SetEffDivideParams(const std::string&), root_opt not used to avoid possible confusions 

  if (eff->GetN() != 1) {
    std::cout << "Error computing efficiency" << std::endl;
    return 0;
  }

  errlow = eff->GetEYlow()[0];
  errhigh = eff->GetEYhigh()[0];

  return eff->GetY()[0];
}


//*********************************************************
void DrawingToolsBase::DrawEff(TTree* tree, const std::string& var, int nx, double xmin, double xmax,
                               const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  // Draw 1D efficiency, meaning the ratio of two histograms in which the numerator (cut1+cut2) is a subset of the denominator (cut2).
  double xbins[NMAXBINS];
  DrawEff(tree, var, nx, GetVariableBins(nx,xmin,xmax,xbins), cut1, cut2, root_opt, opt, leg);
}

//*********************************************************
void DrawingToolsBase::DrawEff(TTree* tree, const std::string& var, int nx, double* xbins,
                               const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  // Draw 1D efficiency, meaning the Divide ratio of two histograms in which the numerator (cut1+cut2) is a subset of the denominator (cut2).
  std::string uopt = drawUtils::ToUpper(opt);
  std::string uroot_opt = drawUtils::ToUpper(root_opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

  // Create empty HistoStacks
  HistoStack* hs1 = new HistoStack(_title,_titleX,_titleY);
  HistoStack* hs2 = new HistoStack(_title,_titleX,_titleY);
  _saved_histoStacks.push_back(hs1);
  _saved_histoStacks.push_back(hs2);

  //  std::string slevel = GetSameLevel(root_opt);

  // Project both trees
  Int_t ny=0;
  double *ybins = NULL;
  ProjectNew(hs1, hs2, tree,tree,var,nx,xbins,ny,ybins,"all",(cut1+" && "+cut2),cut2,root_opt,opt,1.,true);

  // Histogram for numerator
  TH1F* h1 = hs1->GetTotalStat1D();
  TH1F* h2 = hs2->GetTotalStat1D();
  if (!h1) h1 = hs1->GetTotal1D();
  if (!h2) h2 = hs2->GetTotal1D();

  h1->Sumw2();
  h2->Sumw2();

  // compute the efficiency
  TGraphAsymmErrors* eff = new TGraphAsymmErrors(h1);
  _saved_graphs.push_back(eff);
  eff->Divide(h1, h2, _eff_params.c_str()); //the options are explicitely provided by SetEffDivideParams(const std::string&), root_opt not used to avoid possible confusions 

  // Put the errors into the ratio
  FillGraphErrors(hs1, hs2, eff, uopt);

  // Draw the efficiency graph
  DrawGraph(eff,nx,xbins,uroot_opt,uopt,leg);
}


//*********************************************************
void DrawingToolsBase::DrawDoubleEff(TTree* tree1, TTree* tree2, const std::string& var, int nx, double xmin, double xmax,
                                     const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  double xbins[NMAXBINS];
  DrawDoubleEff(tree1, tree2, var, nx, GetVariableBins(nx,xmin,xmax,xbins), cut1, cut2, root_opt, opt, leg);
}

//*********************************************************
void DrawingToolsBase::DrawDoubleEff(TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins,
                                     const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  // Draw 1D efficiency, meaning the Divide ratio of two histograms in which the numerator (cut1+cut2) is a subset of the denominator (cut2).
  std::string uopt = drawUtils::ToUpper(opt);
  std::string uroot_opt = drawUtils::ToUpper(root_opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

  // Create empty HistoStacks
  HistoStack* hs1_num = new HistoStack(_title,_titleX,_titleY);
  HistoStack* hs1_den = new HistoStack(_title,_titleX,_titleY);
  HistoStack* hs2_num = new HistoStack(_title,_titleX,_titleY);
  HistoStack* hs2_den = new HistoStack(_title,_titleX,_titleY);
  _saved_histoStacks.push_back(hs1_num);
  _saved_histoStacks.push_back(hs1_den);
  _saved_histoStacks.push_back(hs2_num);
  _saved_histoStacks.push_back(hs2_den);

  //  std::string slevel = GetSameLevel(root_opt);

  // Project both trees
  Int_t ny=0;
  double *ybins = NULL;
  ProjectNew(hs1_num, hs1_den, tree1,tree1,var,nx,xbins,ny,ybins,"all",(cut1+" && "+cut2),cut2,root_opt,opt,1.,true);
  ProjectNew(hs2_num, hs2_den, tree2,tree2,var,nx,xbins,ny,ybins,"all",(cut1+" && "+cut2),cut2,root_opt,opt,1.,true);

  // Histogram for numerator
  TH1F* h1n = hs1_num->GetTotalStat1D();
  TH1F* h1d = hs1_den->GetTotalStat1D();

  // Histogram for denominator
  TH1F* h2n = hs2_num->GetTotalStat1D();
  TH1F* h2d = hs2_den->GetTotalStat1D();

  if (!h1n) h1n = hs1_num->GetTotal1D();
  if (!h1d) h1d = hs1_den->GetTotal1D();
  if (!h2n) h2n = hs2_num->GetTotal1D();
  if (!h2d) h2d = hs2_den->GetTotal1D();


  h1d->Sumw2();
  h1n->Sumw2();
  h2d->Sumw2();
  h2n->Sumw2();

  // compute the efficiency for first sample
  TGraphAsymmErrors* eff1 = new TGraphAsymmErrors(h1d);
  _saved_graphs.push_back(eff1);
  eff1->Divide(h1n, h1d, _eff_params.c_str()); //the options are explicitely provided by SetEffDivideParams(const std::string&), root_opt not used to avoid possible confusions 

  // compute the efficiency for second sample
  TGraphAsymmErrors* eff2 = new TGraphAsymmErrors(h2d);
  _saved_graphs.push_back(eff2);
  eff2->Divide(h2n, h2d, _eff_params.c_str()); //the options are explicitely provided by SetEffDivideParams(const std::string&), root_opt not used to avoid possible confusions 

  // Put the errors into the efficiency of the first sample
  FillGraphErrors(hs1_num, hs1_den, eff1, uopt);

  // Put the errors into the efficiency of the second sample
  FillGraphErrors(hs2_num, hs2_den, eff2, uopt);

  // compute the efficiency for second sample
  TGraphAsymmErrors* ratio = new TGraphAsymmErrors(h2d);
  _saved_graphs.push_back(ratio);

  for (int i=0;i<nx;i++){
    double x1,y1,x2,y2;
    eff1->GetPoint(i,x1,y1);
    eff2->GetPoint(i,x2,y2);
    double e1 = eff1->GetErrorY(i);
    double e2l = eff2->GetErrorYlow(i);
    double e2h = eff2->GetErrorYhigh(i);
    double exl = eff2->GetErrorXlow(i);
    double exh = eff2->GetErrorXhigh(i);

    double r = y1/y2;
    double erl = sqrt(e1*e1/(y2*y2)+pow(y1/(y2*y2)*e2l,2));
    double erh = sqrt(e1*e1/(y2*y2)+pow(y1/(y2*y2)*e2h,2));

    ratio->SetPoint(i,x1,r);
    ratio->SetPointError(i,exl,exh,erl,erh);
  }

  // Draw the graph
  DrawGraph(ratio,nx,xbins,uroot_opt,uopt,leg,1.3);
}

//*********************************************************
void DrawingToolsBase::DrawGraph(TGraphAsymmErrors* graph, int nbins, double* xbins,
                                 const std::string& uroot_opt, const std::string& uopt, const std::string& leg, double ymax){
//*********************************************************

  std::string slevel = GetSameLevel(uroot_opt);
  if (slevel=="0" && leg!="")
    CreateLegend();

  // Dump te graph
  if (drawUtils::CheckOption(uopt,"DUMP")) 
    DumpGraph(graph,nbins,uopt);

  // Don't draw
  if (drawUtils::CheckOption(uopt,"NODRAW")) return;

  // Histogram attributes
  graph->GetXaxis()->SetRangeUser(xbins[0],xbins[nbins]);
  graph->GetYaxis()->SetRangeUser(0,ymax);
  graph->GetYaxis()->SetNdivisions(520);

  graph->SetTitle("");
  graph->SetLineColor(_auto_colors[_same_level]);
  graph->SetMarkerColor(_auto_colors[_same_level]);
  graph->SetMarkerStyle(_auto_markers[_same_level]);

  // No stats box
  gStyle->SetOptStat(0);

  // Draw the efficiency histogram
  if (uroot_opt.find("SAME")!=std::string::npos)
    graph->Draw(("P"+uroot_opt).c_str());
  else
    graph->Draw(("AP"+uroot_opt).c_str());

  gPad->Update();
  graph->GetXaxis()->SetTitle(_titleX.c_str());
  graph->GetYaxis()->SetTitle(_titleY.c_str());

  // Add an entry to the legend if requested
  if (!drawUtils::CheckOption(uopt,"NOLEG") && leg!=""){
    drawUtils::AddLegendEntry(_legends.back(), graph, leg ,"LE1P");
    _legends.back()->Draw();
  }

  gPad->Update();
}

//*********************************************************
void DrawingToolsBase::DumpGraph(TGraphAsymmErrors* graph, int nbins, const std::string& uopt){
//*********************************************************

  (void)uopt;

  std::cout << "---------- Dumping graph -----------" << std::endl;

  for (int i=0;i<nbins;i++){
    double x,y;
    graph->GetPoint(i,x,y);
    double el = graph->GetErrorYlow(i);
    double eh = graph->GetErrorYhigh(i);

    std::cout << i << ": x = " << x << ",   y = " << y << " +- " << eh << " " << el << std::endl;   
  }
}

//*********************************************************
void DrawingToolsBase::DumpHisto(TH1* histo, const std::string& uopt){
//*********************************************************

  (void)uopt;

  std::cout << "---------- Dumping histo -----------" << std::endl;

  for (int i=0;i<histo->GetNbinsX();i++){
    double x = histo->GetBinCenter(i+1);
    double y = histo->GetBinContent(i+1);
    double e = histo->GetBinError(i+1);

    std::cout << i << ": x = " << x << ",   y = " << y << " +- " << e << std::endl;   
  }
}


//*********************************************************
void DrawingToolsBase::DumpHistoInfo(TH1* histo, const std::string& uopt){
//*********************************************************

  (void)uopt;

  std::cout << " --------------------------------------------------------"        << std::endl;
  std::cout << " Statistics"                                                       << std::endl;
  std::cout << "  entries   = " << histo->GetEntries() << " (not weighted; under/overflow included) " << std::endl;
  std::cout << "  integral  = " << histo->Integral()                              << std::endl;
  std::cout << "  underflow = " << histo->GetBinContent(0)                        << std::endl;
  std::cout << "  overflow  = " << histo->GetBinContent(histo->GetNbinsX()+1)     << std::endl;
  std::cout << " --------------------------------------------------------"        << std::endl;
  std::cout << std::endl;
}

//*********************************************************
void DrawingToolsBase::DrawSignificance(TTree* tree, const std::string& var, int nx, double xmin, double xmax, const std::string& cut1, const std::string& cut2,
                                        double norm, double rel_syst, const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  // Draw 1D significance, defined as the number of signal events (bkg subtracted) divided by its error
  // In this case cut2 defines the selected events, while cut1+cut2 is a sunset (the true signal events)
  double xbins[NMAXBINS];
  DrawSignificance(tree,var, nx, GetVariableBins(nx,xmin,xmax,xbins), cut1, cut2, norm, rel_syst, root_opt, opt, leg);
}

//*********************************************************
void DrawingToolsBase::DrawSignificance(TTree* tree, const std::string& var, int nbins, double* xbins, const std::string& cut1, const std::string& cut2,
                                        double norm, double rel_syst, const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  // Draw 1D significance, defined as the number of signal events (bkg subtracted) divided by its error
  // In this case cut2 defines the selected events, while cut1+cut2 is a sunset (the true signal events)

  std::string slevel = GetSameLevel(root_opt);
  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

  if (slevel=="0" && leg!="")
    CreateLegend();

  // Histogram for cut1+cut2 (numerator)
  TH1F* h1 = GetHisto(tree, "sig1",var,nbins,xbins,(cut1+" && "+cut2), root_opt, uopt,norm);
  // Histogram for cut2 (denominator)
  TH1F* h2 = GetHisto(tree, "sig2",var,nbins,xbins,cut2, root_opt, uopt,norm);

  // create a histo to store the significance
  TH1F* sig = new TH1F(*h1);
  sig->SetName(GetUniqueName("sig").c_str());
  _saved_histos.push_back(sig);

  // compute the significance
  for (int ibin=0;ibin<sig->GetXaxis()->GetNbins();ibin++){
    double total = h2->GetBinContent(ibin+1);
    double s=0;
    if (total>0){
      double pur = h1->GetBinContent(ibin+1)/total;
      s = pur/(sqrt(1/(total*norm) + pow(rel_syst*(1-pur),2)));
    }
    sig->SetBinContent(ibin+1,s);
    sig->SetBinError(ibin+1,0);
  }

  // Histogram attributes
  sig->GetXaxis()->SetTitle(_titleX.c_str());
  sig->GetYaxis()->SetTitle("significance");
  sig->SetTitle(leg.c_str());

  // Draw the histo
  DrawHisto(sig, _auto_colors[_same_level], _line_width, 1, _fill_style, root_opt, uopt+ " NOSTAT","LE1P");

  // Draw the legend
  if (!drawUtils::CheckOption(uopt,"NOLEG")&& leg!="")
    _legends.back()->Draw();

  if (!drawUtils::CheckOption(uopt,"NODRAW"))
    gPad->Update();
}

//*********************************************************
void DrawingToolsBase::DrawRatio(TTree* tree, const std::string& var, int nx, double xmin, double xmax,
                                 const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  // Ratio between two histograms with independent cuts
  double xbins[NMAXBINS];
  DrawRatio(tree, var, nx, GetVariableBins(nx,xmin,xmax,xbins), cut1, cut2, root_opt, opt, leg);
}

//*********************************************************
void DrawingToolsBase::DrawRatio(TTree* tree, const std::string& var, int nx, double* xbins,
                                 const std::string& cut1, const std::string& cut2, const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  // Ratio between two histograms with independent cuts
  DrawRatio(tree, tree, var, nx, xbins, cut1, cut2, 1., root_opt, opt, leg);
}

//**************************************************
void DrawingToolsBase::Draw(TTree* tree1, TTree* tree2, const std::string& var, int nx, double xmin, double xmax,
                            const std::string& categ, const std::string& cut, const std::string& root_opt, const std::string& opt, double norm, bool scale_errors){
//**************************************************

  // 1D comparison between two data samples with different normalisation. Uniform binning
  double xbins[NMAXBINS];
  Draw(tree1,tree2,var,nx,GetVariableBins(nx,xmin,xmax,xbins),categ,cut,root_opt,opt,norm,scale_errors);
}

//**************************************************
void DrawingToolsBase::Draw(TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins,
                            const std::string& categ, const std::string& cut, const std::string& root_opt, const std::string& opt, double norm, bool scale_errors){
//**************************************************

  // 1D comparison between two data samples with different normalisation. Variable binning
  int ny=0;
  double *ybins=NULL;
  Draw(tree1,tree2,var,nx,xbins,ny,ybins,categ,cut,root_opt,opt,norm,scale_errors);
}

//**************************************************
void DrawingToolsBase::Draw(TTree* tree1, TTree* tree2, const std::string& var, int nx, double xmin, double xmax, int ny, double ymin, double ymax,
                        const std::string& categ, const std::string& cut, const std::string& root_opt, const std::string& opt, double norm, bool scale_errors){
//**************************************************

  // 2D comparison between two data samples with different normalisation. Uniform binning
  double xbins[NMAXBINS];
  double ybins[NMAXBINS];
  Draw(tree1,tree2,var,nx,GetVariableBins(nx,xmin,xmax,xbins),ny,GetVariableBins(ny,ymin,ymax,ybins),categ,cut,root_opt,opt,norm,scale_errors);
}

//**************************************************
void DrawingToolsBase::Draw(TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins, int ny, double* ybins,
                            const std::string& categ, const std::string& cut, const std::string& root_opt, const std::string& opt, double norm2, bool scale_errors){
//**************************************************

  // Draw 1D (ny=0) or 2D comparison between two data samples with different normalisation. Variable binning
  if (!HasCategory(categ)) return;

  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;


  // Create empty HistoStacks
  HistoStack* hs1 = new HistoStack(_title,_titleX,_titleY);
  HistoStack* hs2 = new HistoStack(_title,_titleX,_titleY);
  _saved_histoStacks.push_back(hs1);
  _saved_histoStacks.push_back(hs2);

  //  std::string slevel = GetSameLevel(root_opt);

  // Project both trees
  Project(hs1, hs2, tree1,tree2,var,nx,xbins,ny,ybins,categ,cut,root_opt,opt,norm2,scale_errors);

  // Draw the HistoStacks
  DrawHistoStacks(hs1,hs2,categ,root_opt,opt,norm2);
}

//**************************************************
void DrawingToolsBase::DrawRatio(TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins, int ny, double* ybins,
                                 const std::string& cut1, const std::string& cut2, double norm, const std::string& root_opt, const std::string& opt, const std::string& leg, bool scale_errors){
//**************************************************

  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;


  // Create empty HistoStacks
  HistoStack* hs1 = new HistoStack(_title,_titleX,_titleY);
  HistoStack* hs2 = new HistoStack(_title,_titleX,_titleY);
  _saved_histoStacks.push_back(hs1);
  _saved_histoStacks.push_back(hs2);

  // Project both trees
  ProjectNew(hs1, hs2, tree1,tree2,var,nx,xbins,ny,ybins,"all",cut1,cut2,root_opt,opt,norm,scale_errors);

  // Draw the HistoStacks
  DrawRatioHistoStacks(hs1,hs2,root_opt,opt,norm,leg);
}


//**************************************************
void DrawingToolsBase::DrawRatioHistoStacks(HistoStack* hs1, HistoStack* hs2,
                                            const std::string& root_opt, const std::string& opt, double norm, const std::string& leg){
//**************************************************

  (void)norm; // norm not currently used

  std::string uopt = drawUtils::ToUpper(opt);

  // Is a 2D histo ?
  bool is2D = false;
  if (hs1){
    if (hs1->Is2D()) is2D=true;
  }
  else if (hs2){
    if (hs2->Is2D()) is2D=true;
  }
  // options for 2D histos
  std::string box = "";
  std::string error = "e";
  if (is2D){
    box =" box";
    error = "";
  }
  //  std::string smallleg = " SMALLLEG";
  if (!hs2) uopt=uopt+" NOLEG";

  std::string slevel = GetSameLevel(root_opt);

  if (slevel=="0" && leg!="")
    CreateLegend(uopt);

  // Don't create the legend below
  uopt = uopt+ " NOCREATELEG";

  TH1F* ratio = GetRatioHisto(hs1,hs2,uopt);

  if (ratio)
    DrawRatio(ratio,root_opt+box+error,uopt,leg);
}


//**************************************************
void DrawingToolsBase::DrawRatio(TTree* tree1, TTree* tree2, const std::string& var, int nx, double xmin, double xmax,
                                 const std::string& cut, double norm, const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  // 1D ratio between two data samples with different normalisation and same cut. Uniform binning
  double xbins[NMAXBINS];
  DrawRatio(tree1,tree2,var,nx,GetVariableBins(nx,xmin,xmax,xbins),cut,cut,norm,root_opt,opt,leg);
}

//**************************************************
void DrawingToolsBase::DrawRatio(TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins,
                                 const std::string& cut, double norm, const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  // 1D ratio between two data samples with different normalisation and same cut. Variable binning
  DrawRatio(tree1,tree2,var,nx,xbins,cut,cut,norm,root_opt,opt,leg);
}

//**************************************************
void DrawingToolsBase::DrawRatio(TTree* tree1, TTree* tree2, const std::string& var, int nx, double xmin, double xmax,
                                 const std::string& cut1, const std::string& cut2, double norm, const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  // 1D ratio between two data samples with different normalisation. Uniform binning
  double xbins[NMAXBINS];
  DrawRatio(tree1,tree2,var,nx,GetVariableBins(nx,xmin,xmax,xbins),cut1,cut2,norm,root_opt,opt,leg);
}

//**************************************************
void DrawingToolsBase::DrawRatio(TTree* tree1, TTree* tree2, const std::string& var, int nx, double* xbins,
                                 const std::string& cut1, const std::string& cut2, double norm, const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  int ny=0;
  double* ybins=NULL;
  DrawRatio(tree1,tree2,var,nx,xbins,ny,ybins,cut1,cut2,norm,root_opt,opt,leg);
}

//**************************************************
void DrawingToolsBase::DrawRatio(TH1F* ratio, const std::string& root_opt, const std::string& uopt, const std::string& leg){
//**************************************************

  std::string slevel = GetSameLevel(root_opt);
  if (slevel=="0" && leg!="")
    CreateLegend();

  // Draw the histogram
  DrawHisto(ratio, _auto_colors[_same_level], _line_width, 1, _fill_style, root_opt, uopt+" NOSTAT NOVARBIN","LE1P",0);

  // Draw the legend
  if (_drawleg && !drawUtils::CheckOption(uopt,"NOLEG") && leg!="")
    _legends.back()->Draw();

  if (!drawUtils::CheckOption(uopt,"NODRAW"))
    gPad->Update();
}

//**************************************************
void DrawingToolsBase::DrawEventsVSCut(TTree* tree, const std::string& cut_norm, int first_cut, int last_cut,
                                       const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  DrawEventsVSCut(tree,0,cut_norm,first_cut,last_cut,root_opt,opt,leg);
}

//**************************************************
void DrawingToolsBase::DrawEventsVSCut(TTree* tree, int ibranch, const std::string& cut_norm, int first_cut, int last_cut,
                                       const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  // if selection exists
  if (sel().GetSelection(ibranch,true)) {
    int isel = ibranch;
    std::cout << "Drawing for selection " << isel << ", branch 0" << std::endl;
    return DrawEventsVSCut(tree,isel,0,cut_norm,first_cut,last_cut,root_opt,opt,leg);
  }

  DrawEventsVSCut(tree,0,ibranch,cut_norm,first_cut,last_cut,root_opt,opt,leg);
}


//**************************************************
void DrawingToolsBase::DrawEventsVSCut(TTree* tree, int isel, int ibranch, const std::string& cut_norm, int first_cut, int last_cut,
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

  if (slevel=="0" && leg!="")
    CreateLegend();

  if (strcmp(tree->GetName(), "truth") && !drawUtils::CheckOption(uopt,"DRAWALLCUTS")) first_cut = _minAccumLevelToSave-1;
  TH1F* hall = GetEventsVSCut(tree,"events",cut_norm, isel, ibranch,first_cut, last_cut,root_opt,uopt);

  Int_t cut_offset = 0;
  // First bin corresponds to no cut
  if (first_cut ==-1){
    hall->GetXaxis()->SetBinLabel(1, "NO CUT");
    cut_offset = 1;
  }

  std::vector<StepBase*> cuts = sel().GetSelection(isel)->GetCutsInBranch(ibranch);
  // Start from second bin since first corresponds to no cut
  for (int i=cut_offset;i<hall->GetNbinsX();i++ ){
    int icut = first_cut+i;
    hall->GetXaxis()->SetBinLabel(i+1, cuts[icut]->Title().c_str());
  }

  // No error in X
  gStyle->SetErrorX(0.0001);

  hall->SetTitle(_title.c_str());
  hall->SetMarkerStyle(21);

  DrawHisto(hall, _auto_colors[_same_level], _line_width, _auto_colors[_same_level], _fill_style, "pl e"+root_opt, uopt+" NOSTAT NOLEG");

  hall->GetXaxis()->SetTitle("");
  hall->GetYaxis()->SetTitle("# events");

  // Add an entry to the legend if requested
  if (!drawUtils::CheckOption(uopt,"NOLEG") && leg!=""){
    drawUtils::AddLegendEntry(_legends.back(), hall, leg, "LE1P");
    _legends.back()->Draw();
  }

  if (!drawUtils::CheckOption(uopt,"NODRAW"))
    gPad->Update();

  // reset error in X
  gStyle->SetErrorX();
}

//**************************************************
void DrawingToolsBase::DrawRatioVSCut(TTree* tree1, TTree* tree2, const std::string& precut, int first_cut, int last_cut,
                                      const std::string& root_opt, const std::string& opt, const std::string& leg, double norm){
//**************************************************

  DrawRatioVSCut(tree1,tree2,0, precut,first_cut,last_cut,root_opt,opt,leg,norm);
}

//**************************************************
void DrawingToolsBase::DrawRatioVSCut(TTree* tree1, TTree* tree2, int ibranch, const std::string& precut, int first_cut, int last_cut,
                                      const std::string& root_opt, const std::string& opt, const std::string& leg, double norm){
//**************************************************

  // if selection exists
  if (sel().GetSelection(ibranch,true)) {
    int isel = ibranch;
    std::cout << "Drawing for selection " << isel << ", branch 0" << std::endl;
    return DrawRatioVSCut(tree1,tree2,isel,0,precut,first_cut,last_cut,root_opt,opt,leg,norm);
  }

  DrawRatioVSCut(tree1,tree2,0,ibranch,precut,first_cut,last_cut,root_opt,opt,leg,norm);
}

//**************************************************
void DrawingToolsBase::DrawRatioVSCut(TTree* tree1, TTree* tree2, int isel, int branch, const std::string& precut, int first_cut, int last_cut,
                                      const std::string& root_opt, const std::string& opt, const std::string& leg, double norm){
//**************************************************

  // Check if selection exists
  if (!sel().GetSelection(isel,true)) return;

  std::string slevel = GetSameLevel(root_opt);
  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

  if (slevel=="0" && leg!="")
    CreateLegend();

  Int_t first_cut2=-1;
  // Histogram for cut1+cut2 (numerator)
  TH1F* h1 = GetEventsVSCut(tree1,"num",precut,isel,branch,first_cut2, last_cut,root_opt,uopt);

  first_cut2=-1;
  // Histogram for cut2 (denominator)
  TH1F* h2 = GetEventsVSCut(tree2,"den",precut,isel,branch,first_cut2, last_cut,root_opt,uopt);

  // scale the histo if requested
  ScaleHisto(h2,norm);

  DrawRatioVSCut(h1,h2,isel,branch,first_cut,root_opt,uopt,leg);
}


//**************************************************
void DrawingToolsBase::DrawPurVSCut(TTree* tree, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
                                    const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  DrawPurVSCut(tree,0,signal,precut,first_cut,last_cut,root_opt,opt,leg);
}

//**************************************************
void DrawingToolsBase::DrawPurVSCut(TTree* tree, int ibranch, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
                                    const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  // if selection exists
  if (sel().GetSelection(ibranch,true)) {
    int isel = ibranch;
    std::cout << "Drawing for selection " << isel << ", branch 0" << std::endl;
    return DrawPurVSCut(tree,isel,0,signal,precut,first_cut,last_cut,root_opt,opt,leg);
  }

  DrawPurVSCut(tree,0,ibranch,signal,precut,first_cut,last_cut,root_opt,opt,leg);
}


//**************************************************
void DrawingToolsBase::DrawPurVSCut(TTree* tree, int isel, int branch, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
                                    const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  // Check if selection exists
  if (!sel().GetSelection(isel,true)) return;

  std::string slevel = GetSameLevel(root_opt);
  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

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

  Int_t first_cut2 =-1;
  // Histogram for cut1+cut2 (numerator)
  TH1F* h1 = GetEventsVSCut(tree,"pur1",numer,isel,branch,first_cut2, last_cut,root_opt,uopt);

  first_cut2 =-1;
  // Histogram for cut2 (denominator)
  TH1F* h2 = GetEventsVSCut(tree,"pur2",precut,isel,branch,first_cut2, last_cut,root_opt,uopt);

  std::cout << "----- Purity values -----------" << std::endl;

  if (!drawUtils::CheckOption(uopt,"DRAWALLCUTS")) first_cut = _minAccumLevelToSave-1;
  DrawRatioVSCut(h1,h2,isel,branch,first_cut,root_opt,uopt+" EFF",leg);
}

//**************************************************
void DrawingToolsBase::DrawEffVSCut(TTree* tree, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
                                    const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  DrawEffVSCut(tree,0,signal,precut,first_cut,last_cut,root_opt,opt,leg);
}

//**************************************************
void DrawingToolsBase::DrawEffVSCut(TTree* tree, int ibranch, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
                                    const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  // if selection exists
  if (sel().GetSelection(ibranch,true)) {
    int isel = ibranch;
    std::cout << "Drawing for selection " << isel << ", branch 0" << std::endl;
    return DrawEffVSCut(tree,isel,0,signal,precut,first_cut,last_cut,root_opt,opt,leg);
  }

  DrawEffVSCut(tree,0,ibranch,signal,precut,first_cut,last_cut,root_opt,opt,leg);
}


//**************************************************
void DrawingToolsBase::DrawEffVSCut(TTree* tree, int isel, int branch, const std::string& signal, const std::string& precut, int first_cut, int last_cut,
                                    const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  // Check if selection exists
  if (!sel().GetSelection(isel,true)) return;

  std::string slevel = GetSameLevel(root_opt);
  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

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

  Int_t first_cut2 = -1;
  // Histogram for cut1+cut2 (numerator)
  TH1F* h1 = GetEventsVSCut(tree,"eff1",numer,isel,branch,first_cut2, last_cut,root_opt,uopt);

  // Histogram for cut2 (denominator)
  int nx = h1->GetNbinsX();
  double xmin = h1->GetXaxis()->GetXmin();
  double xmax = h1->GetXaxis()->GetXmax();
  double xbins[NMAXBINS];
  TH1F* h2 = new TH1F(GetUniqueName("eff2").c_str(),"eff2",nx,GetVariableBins(nx,xmin,xmax,xbins));
  _saved_histos.push_back(h2);

  for (int i=0; i<h2->GetNbinsX();i++){
    h2->SetBinContent(i+1,h1->GetBinContent(1));
    h2->SetBinError(  i+1,h1->GetBinError(1));
  }

  std::cout << "----- Efficiency values -----------" << std::endl;
  DrawRatioVSCut(h1,h2,isel,branch,first_cut,root_opt,uopt+" EFF",leg);
}

//**************************************************
void DrawingToolsBase::DrawRatioVSCut(TH1F* h1, TH1F* h2, int isel, int branch, int first_cut,
                                      const std::string& root_opt, const std::string& opt, const std::string& leg){
//**************************************************

  // TODO: The entire method must be simplified using DrawHisto and DrawGraph

  // Read the steps from the config tree
  //  ReadSteps(_config_file);

  // Check if selection exists
  if (!sel().GetSelection(isel,true)) return;

  std::string uopt = drawUtils::ToUpper(opt);
  std::string uroot_opt = drawUtils::ToUpper(root_opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

  TH1F* hratio = new TH1F(GetUniqueName("ratio").c_str(),"",h1->GetNbinsX(),h1->GetXaxis()->GetXmin(),h1->GetXaxis()->GetXmax());
  _saved_histos.push_back(hratio);

  hratio->Divide(h1,h2);

  Int_t cut_offset = 1;
  // First bin corresponds to no cut
  hratio->GetXaxis()->SetBinLabel(1, "NO CUT");

  std::vector<StepBase*> cuts = sel().GetSelection(isel)->GetCutsInBranch(branch);
  // Start from second bin since first corresponds to no cut
  for (int i=cut_offset;i<hratio->GetNbinsX();i++ ){
    int icut = i-cut_offset;
    hratio->GetXaxis()->SetBinLabel(i+1, cuts[icut]->Title().c_str());
  }

  // No stats box
  gStyle->SetOptStat(0);

  // Histogram attributes
  hratio->SetTitle(_title.c_str());
  hratio->GetXaxis()->SetTitle("");
  hratio->GetYaxis()->SetTitle(_titleY.c_str());
  hratio->GetYaxis()->SetNdivisions(520);

  // Efficiency ratio
  if (drawUtils::CheckInternalOption(uopt,"EFF")){

    hratio->GetYaxis()->SetRangeUser(0,1.1);

    // Set the proper range depending on the first_cut
    hratio->GetXaxis()->SetRange(first_cut+cut_offset+1,hratio->GetNbinsX());

    if (uroot_opt.find("SAME")!=std::string::npos)
      hratio->Draw("PHIST same");
    else
      hratio->Draw("PHIST");

    // compute the efficiency
    //    TGraphAsymmErrors* eff = new TGraphAsymmErrors(h1);
    TGraphAsymmErrors* eff = new TGraphAsymmErrors(h1->GetNbinsX());
    eff->SetName(GetUniqueName("eff").c_str());
    _saved_graphs.push_back(eff);
    eff->Divide(h1, h2, (_eff_params + " v").c_str()); //the options are explicitely provided by SetEffDivideParams(const std::string&), root_opt not used to avoid possible confusions 

    eff->SetLineColor(_auto_colors[_same_level]);
    eff->SetMarkerColor(_auto_colors[_same_level]);
    eff->SetMarkerStyle(21);

    //    eff->GetXaxis()->SetRange(first_cut+cut_offset+1,hratio->GetNbinsX());
    eff->GetXaxis()->SetLimits(first_cut+cut_offset+1,hratio->GetNbinsX());

    for (int i=0; i<h1->GetNbinsX();i++){
      eff->SetPointEXhigh(i,0);
      eff->SetPointEXlow(i,0);
    }

    // Draw the efficiency histogram  (TODO: use DrawGraph)
    eff->Draw(("PL "+root_opt).c_str());

    //    Add an entry to the legend if requested
    if (!drawUtils::CheckOption(uopt,"NOLEG") && leg!=""){
      drawUtils::AddLegendEntry(_legends.back(), eff, leg, "LE1P");
    }
  }
  // Normal ratio
  else{
    hratio->SetLineColor(_auto_colors[_same_level]);
    hratio->SetMarkerColor(_auto_colors[_same_level]);
    hratio->SetMarkerStyle(21);

    // Set the proper range depending on the first_cut
    hratio->GetXaxis()->SetRange(first_cut+cut_offset+1,hratio->GetNbinsX());

    // Draw the ratio histogram
    hratio->Draw(("PL "+uroot_opt).c_str());

    // Add an entry to the legend if requested
    if (!drawUtils::CheckOption(uopt,"NOLEG") && leg!=""){
      drawUtils::AddLegendEntry(_legends.back(), hratio, leg, "LE1P");
    }

    // Print numbers on the screen
    char out1[256];  
    char out2[256];  
    for (int i=0;i<hratio->GetNbinsX();i++ ){
      int icut = i-1;    
      std::string cut_name="";
      if (icut==-1) 
        cut_name = "NO CUT";
      else
        cut_name = cuts[icut]->Title();      
      sprintf(out1,"%3s: %-25s %-10s", "#", "cut", "ratio");
      sprintf(out2,"%3d: %-25s %-10.2f", icut, cut_name.c_str(), hratio->GetBinContent(i+1));      
      if (i==0) std::cout << out1 << std::endl;
      std::cout << out2 << std::endl;
    }
  }

  // Draw the legend if requestet
  if (!drawUtils::CheckOption(uopt,"NOLEG") && leg!="")
    _legends.back()->Draw();

  if (!drawUtils::CheckOption(uopt,"NODRAW"))
    gPad->Update();
}

//*********************************************************
void DrawingToolsBase::DrawToys(TTree* tree, const std::string& cut, const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  // Draw the distribution of entries for all toy experiments
  // The rms of this distribution is the systematic error

  std::string uopt = drawUtils::ToUpper(opt);


  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

  int ntoys = drawUtils::GetNToys(tree);

  TH1F h1("h1","h1",ntoys,0,ntoys);
  TH1F hw("hw","hw",ntoys,0,ntoys);

  // Apply event weights
  std::string cutp = weightTools::ApplyWeights(tree,FormatCut(cut),(uopt+" NOTOYW").c_str());

  // Project with no toy experiment weights
  tree->Project("h1","toy_index",cutp.c_str());

  std::string cut2=cut;
  if (cut2=="") cut2="1==1";

  // Project the toy experiment PDF weights
  if (drawUtils::TreeHasVar(tree,"toy_weight"))
    tree->Project("hw","toy_index",("("+cutp+")*toy_weight").c_str());
  else
    hw = h1;

  // compute the average toy exp weight
  TH1F hwa(hw);
  hwa.Divide(&hw,&h1);

  DrawToysBase(h1,hwa,root_opt,uopt,leg);
}


//*********************************************************
void DrawingToolsBase::DrawToysRatio(TTree* tree1, TTree* tree2, const std::string& cut, double norm, 
                                    const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  // Draw the distribution of ratios for all toy experiments
  // The rms of this distribution is the systematic error
  // Use same cut for both data samples

  DrawToysRatio(tree1,tree2,cut,cut,norm,root_opt,opt,leg);
}

//*********************************************************
void DrawingToolsBase::DrawToysRatio(TTree* tree1, TTree* tree2, const std::string& cut1, const std::string& cut2, double norm, 
                                    const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  // Draw the distribution of ratios for all toy experiments
  // The rms of this distribution is the systematic error
  // Use different cuts for both data samples

  int NTOYS1 = drawUtils::GetNToys(tree1);
  int NTOYS2 = drawUtils::GetNToys(tree2);

  // The number of toy experiments must be the same
  if (NTOYS1!=NTOYS2) return;

  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;


  // Apply event weights
  std::string cut1p = weightTools::ApplyWeights(tree1,FormatCut(cut1),(uopt+" NOTOYW").c_str());
  std::string cut2p = weightTools::ApplyWeights(tree2,FormatCut(cut2),(uopt+" NOTOYW").c_str());

  // Project the first sample
  TH1F h1("en1","en2",NTOYS1,0,NTOYS1);
  tree1->Project("en1","toy_index",cut1p.c_str());

  // Project the second sample
  TH1F h2("en2","en2",NTOYS1,0,NTOYS1);
  tree2->Project("en2","toy_index",cut2p.c_str());

  // compute the ratio
  TH1F ratio(h1);
  ratio.SetName("ratio");
  ScaleHisto(&h2,norm);
  ratio.Divide(&h1,&h2);

  // Project the weights for the first sample
  TH1F hw1("hw1","hw1",NTOYS1,0,NTOYS1);
  if (drawUtils::TreeHasVar(tree1,"toy_weight"))
    tree1->Project("hw1","toy_index",("("+cut1p+")*toy_weight").c_str());
  else
    hw1 = h1;

  // Project the weights for the first sample
  TH1F hw2("hw2","hw2",NTOYS1,0,NTOYS1);
  if (drawUtils::TreeHasVar(tree2,"toy_weight"))
    tree2->Project("hw2","toy_index",("("+cut2p+")*toy_weight").c_str());
  else
    hw2 = h2;


  // compute the average weights for sample 1
  TH1F hwa1(hw1);
  hwa1.Divide(&hw1,&h1);

  // compute the average weights for sample 2
  TH1F hwa2(hw2);
  hwa2.Divide(&hw2,&h2);


  DrawToysBase(ratio,hwa2,root_opt,uopt,leg);
}

//*********************************************************
void DrawingToolsBase::DrawToysBase(TH1F& result, TH1F& weights, const std::string& root_opt, const std::string& opt, const std::string& leg){
//*********************************************************

  // Draw the distribution of entries for all toy experiments

  std::string slevel = GetSameLevel(root_opt);
  std::string uopt = drawUtils::ToUpper(opt);
  if (slevel=="0" && leg!="")
    CreateLegend();

  // Stat option (entries, MEAN, RMS, and integral)
  int statTemp = _stat_option;
  SetOptStat(1001110);
  gStyle->SetOptStat(1001110);

  const int NTOYS = result.GetNbinsX();

  double nentries[NMAXTOYS];
  double avg_weight[NMAXTOYS];
  double sum_weights=0;
  double avg_nentries=0;
  double hmin = 1e10;
  double hmax = -1;

  // Get the entries
  for (int itoy=0;itoy<NTOYS;itoy++){
    nentries[itoy]=result.GetBinContent(itoy+1);
    // Get the average weight for each toy experiment
    if (nentries[itoy]>0)
      avg_weight[itoy] = weights.GetBinContent(itoy+1);
    else
      avg_weight[itoy] = 0;
    std::cout << "toy_index = "<< itoy << "\t --> #entries = " << nentries[itoy] << "\t w=" << avg_weight[itoy] << std::endl;

    if (nentries[itoy]>hmax) hmax = nentries[itoy];
    if (nentries[itoy]<hmin) hmin = nentries[itoy];

    // compute the total weight and average entries (justfor debugging)
    sum_weights += avg_weight[itoy];
    avg_nentries += nentries[itoy]*avg_weight[itoy];
  }

  std::cout << "avg[#entries] = sum[entries*avg_weight]/sum[avg_weight] = "<< avg_nentries/sum_weights
            << ", sum[avg_weights] = " << sum_weights
            << ", sum[entries*avg_weight] = " << avg_nentries << std::endl;

  //  Create an histogram and fill the distribution
  TH1F* hentries = new TH1F(GetUniqueName("hentries").c_str(),leg.c_str(),1000,hmin-(hmax-hmin)/10.,hmax+(hmax-hmin)/10.);
  _saved_histos.push_back(hentries);

  double rms=0;
  double rms2=0;
  for (int itoy=0;itoy<NTOYS;itoy++){
    //    hentries->Fill(nentries[itoy],avg_weight[itoy]);
    hentries->Fill(nentries[itoy],1./NTOYS);

    // this is just for debugging. rms should coincide with the rms given by root
    rms += pow(nentries[itoy]-avg_nentries/sum_weights,2)*avg_weight[itoy]/sum_weights;

    // this is the rms when the weights are not properly normilized to 1
    rms2 += pow(nentries[itoy]-avg_nentries,2)*avg_weight[itoy];
  }

  std::cout << "rms = "<< sqrt(rms) << ", rms' = " << sqrt(rms2) << std::endl;

  // Draw the entries histogram
  DrawHisto(hentries, _line_width, _line_color, _fill_style, root_opt, uopt, "LE1P");

  // Draw the legend
  if (!drawUtils::CheckOption(uopt,"NOLEG") && leg!="")
    _legends.back()->Draw();

  if (!drawUtils::CheckOption(uopt,"NODRAW"))
    gPad->Update();

  // go back to previous stat option
  SetOptStat(statTemp);
}

//**************************************************
void DrawingToolsBase::DrawHisto(TH1* h, int lw, int fc, int fs, const std::string& root_opt, const std::string& opt,const std::string& leg_opt,int mode){
//**************************************************

  std::string uopt = drawUtils::ToUpper(opt);

  // (_line_color != 1) allows SetLineColor with DrawRealtiveErrors
  if (drawUtils::CheckInternalOption(uopt,"NOAUTOCOLORS") || _line_color != 1){
    // Draw histogram without specific color
    DrawHisto(h,fc,lw,fc,fs, root_opt, opt,leg_opt,mode);
  }
  else{
    // Draw histogram without specific color. Use the auto colors
    DrawHisto(h,_auto_colors[_same_level],lw,fc,fs, root_opt, opt,leg_opt,mode);
  }
}

//**************************************************
void DrawingToolsBase::DrawHisto(TH1* h, int lc, int lw, int fc, int fs, const std::string& root_opt, const std::string& opt,const std::string& leg_opt,int mode){
//**************************************************

  (void)mode;


  // Basic function to draw 1D histo
  std::string uopt = drawUtils::ToUpper(opt);
  std::string uroot_opt = drawUtils::ToUpper(root_opt);

  // Dump the histogram
  if (drawUtils::CheckOption(uopt,"DUMP"))
    DumpHisto(h,uopt);

  // Don't draw
  if (drawUtils::CheckOption(uopt,"NODRAW")) return;

  if (drawUtils::CheckOption(uopt,"NOSTAT"))
    gStyle->SetOptStat(0);
  else
    gStyle->SetOptStat(_stat_option);

  h->SetLineColor(lc);
  h->SetMarkerColor(lc);

  // Use the marker in all cases 
  if (fs == 0)// && drawUtils::CheckInternalOption(uopt,"ISDATA"))   // check ISDATA in case of plotting mc,data with FillStyle 0
    h->SetMarkerStyle(_marker_style);

  h->SetMarkerSize(_marker_size);

  h->SetLineWidth(lw);
  h->SetFillColor(fc);
  h->SetFillStyle(fs);

  // get the title for the legend
  std::string title = h->GetTitle();
  std::string leg_opt_copy = leg_opt;
  if (_data_label != "" && drawUtils::CheckInternalOption(uopt,"ISDATA")) {
    title = _data_label;
    leg_opt_copy = "lpe";
  }
  else if (_allmcstat_label != "" && drawUtils::CheckInternalOption(uopt,"ALLMCSTATLEG")) {
    title = _allmcstat_label;
    leg_opt_copy = "f";
  }
  else if (_allmcsyst_label != "" && drawUtils::CheckInternalOption(uopt,"ALLMCSYSTLEG")) {
    title = _allmcsyst_label;
    leg_opt_copy = "f";
  }
  else if (_allmc_label != "" && drawUtils::CheckInternalOption(uopt,"ALLMCLEG")) {
    title = _allmc_label;
    leg_opt_copy = "f";
  }

  // Set axis labels and title
  h->GetXaxis()->SetTitle(_titleX.c_str());
  h->GetYaxis()->SetTitle(_titleY.c_str());
  h->SetTitle(_title.c_str());

  //---------- Put the right statistics box, with no variable binning normalization --------------
  TH1F* hp = new TH1F(*((TH1F*)h));
  std::string name = hp->GetName();
  hp->SetName((name+"_new").c_str());
  _saved_histos.push_back(hp);

  // TODO: for some reason putting an option that does not exist (i.e. Z) does the trick: plots the axis with stat and title. This solves bug 1043
  // If no one complains keep it like this
  std::string root_opt_novarbin = "Z";
  root_opt_novarbin = root_opt_novarbin + GetSameRootOption(uroot_opt);

  // I don't know why these lines were here. It causes problems when ploting two DataSample's with "all" category, since it does not plot the h histogram empty 
  //  if (uroot_opt.find("HIST")!=std::string::npos)
  //    root_opt_novarbin = root_opt_novarbin + " HIST";
  
  h->Draw(root_opt_novarbin.c_str());
  //----------------------------------------------------------------------------------------------

  drawUtils::NormalizeVariableBinning(hp,2,uopt);

  // Draw the final histogram with variable binning normalization but no statistics box
  // first remove SAME or SAMES from the root_opt, because if it appears twice we can have undesired effects
  // In addition we dont want the stat box again, so never use SAMES
  std::string uroot_opt2= GetNoSameRootOption(uroot_opt);

  hp->Draw((uroot_opt2+" same").c_str());


  // Note: TH1F::GetBinErrorUp() was used in this function in stead of TH1F::GetBinError(). That
  // function is not available on ROOT 5.30, which is the default for production 5. Thus this
  // function cannot be used here, as we need production 5 compatibility!

  double ymax = hp->GetBinContent(hp->GetMaximumBin())+hp->GetBinError(hp->GetMaximumBin());
  // Get the maximum y and set the y max of the histogram
  if (fabs(_maxY) > 1e-6 && _maxY > _minY)
    h->SetMaximum(_maxY);
  else
    h->SetMaximum(ymax*_relativeMaxY);


  // set the minimun Y
  if (!drawUtils::CheckOption(uopt,"NOMIN")|| _logY)
    h->SetMinimum(_minY);


  gPad->SetLogy(_logY);

  //Add entry to legend
  if (_drawleg && !drawUtils::CheckOption(uopt,"NOLEG") && !drawUtils::CheckInternalOption(uopt,"DONTADDLEGENTRY")  &&  title!="")
    drawUtils::AddLegendEntry(_legends.back(), h, title, leg_opt_copy);

}

//**************************************************
void DrawingToolsBase::DrawHistoStack(HistoStack* hs, const std::string& categ, const std::string& root_opt, const std::string& opt,const std::string& leg_opt,int mode){
//**************************************************

  DrawHistoStack(hs,categ,root_opt,opt,_line_color, _line_width, _fill_color, _fill_style, leg_opt, mode);
}

//**************************************************
void DrawingToolsBase::DrawHistoStack(HistoStack* hs, const std::string& categ, const std::string& root_opt, const std::string& opt, int lc, int lw, int fc, int fs, const std::string& leg_opt,int mode){
//**************************************************

  if (!hs->GetTotal1D() && !hs->GetTotal2D()) return;

  // mode not currently used. This is for variable bin normalization. This normalization is done when Projecting the histogram and not at drawing level
  // mainly to avoid normalizing several times when plotting the same histo twice
  (void)mode;
  (void)leg_opt;

  std::string uopt = drawUtils::ToUpper(opt);

  // Dump the histogram
  if (drawUtils::CheckOption(uopt,"DUMP") && hs->GetTotal1D())
    DumpHisto(hs->GetTotal1D(),uopt);

  if (!drawUtils::CheckOption(uopt,"NOINFO") && hs->GetTotal1D()){
    DumpHistoInfo(hs->GetTotal1D(),uopt);
  }


  // Don't draw
  if (drawUtils::CheckOption(uopt,"NODRAW")) return;

  if (drawUtils::CheckOption(uopt,"NOSTAT"))
    gStyle->SetOptStat(0);
  else
    gStyle->SetOptStat(_stat_option);

  // Create the legend
  CreateLegend(uopt);

  if (categ=="all") {
    // this if is needed to draw the legend for data when categ is all
    if (!drawUtils::CheckInternalOption(uopt,"ISDATA")  && !drawUtils::CheckInternalOption(uopt,"ALLMCLEG")) uopt=uopt+" NOLEG";

    if (hs->Is1D()){
      std::string same="";

      // Error bars for second sample (MC in general)
      std::string uopt2 = uopt;
      std::string error_opt = "";
      if (drawUtils::CheckInternalOption(uopt,"ISSECONDSAMPLE")){
        error_opt = GetErrorStyle(uopt);
        if (error_opt!=""){
          lc = fc = _mcerror_color;
          lw = _line_width;
          fs = 1001;
          uopt2 = uopt+" NOAUTOCOLORS";
          error_opt= " "+ error_opt;
        }
        else
          uopt2 = uopt+" NOERROR";
      }

      // Draw first the histo with all errors (stat+syst)
      if (!drawUtils::CheckOption(uopt2,"NOTOTERROR")  || drawUtils::CheckInternalOption(uopt2,"ISDATA")  || !hs->GetTotalSyst1D()){
        if (drawUtils::CheckInternalOption(uopt2,"NOERROR"))
          DrawHisto(hs->GetTotal1D(), lc, lw, fc, fs, root_opt+" HIST",    uopt2, "f");
        else
          DrawHisto(hs->GetTotal1D(), lc, lw, fc, fs, root_opt+error_opt,  uopt2, "f");
        same = " same";
      }

      // Draw superimposed the histo with only statistical or systematic errors
      if (!drawUtils::CheckInternalOption(uopt,"ISDATA")){
        if (hs->GetTotalStat1D() && !drawUtils::CheckOption(uopt,"NOSTERROR")  &&  !drawUtils::CheckOption(uopt,"SYSTERROR"))
          DrawHisto(hs->GetTotalStat1D(), _mcstaterror_color, lw, _mcstaterror_color, fs, root_opt+same+error_opt, uopt2+" ALLMCSTATLEG", "f");
        else if (hs->GetTotalSyst1D() && drawUtils::CheckOption(uopt,"SYSTERROR"))
          DrawHisto(hs->GetTotalSyst1D(), _mcstaterror_color, lw, _mcstaterror_color, fs, root_opt+same+error_opt, uopt2+" ALLMCSYSTLEG", "f");
      }
    }
    else if (hs->Is2D())
      DrawHisto(hs->GetTotal2D(), lc, lw, fc, fs, root_opt, uopt);

  }
  else {
  // if categ != all
    hs->Draw(root_opt+" HIST",uopt);
    if(hs->Is1D() && drawUtils::CheckOption(uopt,"DRAWALLMC"))
      DrawHisto(hs->GetTotal1D(), lc, lw, fc, fs, root_opt+" same", uopt,"f");
  }

  // Create the legend. After || is needed to draw the legend for data when categ is all
  //  if ((!drawUtils::CheckOption(uopt,"NOLEG") && categ!="all") || (drawUtils::CheckOption(uopt,"ISDATA")  && categ=="all")) {
  if ((!drawUtils::CheckOption(uopt,"NOLEG"))){
    hs->FillLegend(_legends.back());
    _legends.back()->Draw();
  }

  // If stat box has "name", set it manually (instead of having the hs name). scantaTM
  int optstat = gStyle->GetOptStat();
  if (optstat % 2 != 0) {
    gPad->Update(); // needed, PaveStats might not exist yet
    TPaveStats *pavestat = (TPaveStats*)gPad->GetPrimitive("stats");
    pavestat->SetName("mystats"); // needed, unknown reasons
    TList *list = pavestat->GetListOfLines();
    list->RemoveFirst(); // remove the current title
    // create new title
    TLatex *name = new TLatex(0,0,_allmc_label.c_str());
    if (drawUtils::CheckInternalOption(uopt,"ISDATA")) {
      name = new TLatex(0,0,_data_label.c_str());
      name->SetTextColor(_data_color);
    } else {
      name = new TLatex(0,0,_allmc_label.c_str());
      name->SetTextColor(_allmc_color);
    }
    list->AddFirst(name);
//    list->Print();
    if (hs->Is1D()) hs->GetTotal1D()->SetStats(0); // to avoid the automatic redraw of stats
    else            hs->GetTotal2D()->SetStats(0); // to avoid the automatic redraw of stats
    gPad->Draw(); // needed to update
    pavestat->Draw(); // needed when categ != all
  }

  // Set the appropriate maximum for 1D plots
  if (hs->Is1D()){
    double max = _maxY;
    double relmax=1.;
    if (fabs(_maxY) < 1e-6 || _maxY < _minY) {
      max = hs->GetMaximumWithError(opt);
      relmax=_relativeMaxY;
      if (categ!="all" && !drawUtils::CheckOption(uopt,"ETOT"))
        max = hs->GetMaximum(opt);
    }

    if (categ!="all") relmax/=1.05;

    hs->SetMaximum(max*relmax);

    // set the minimun Y
    if (!drawUtils::CheckOption(uopt,"NOMIN") || _logY)
      hs->SetMinimum(_minY);

    gPad->SetLogy(_logY);
    gPad->Update();
    gPad->Draw();
  }
}

//**************************************************
void DrawingToolsBase::DrawHistoStacks(HistoStack* hs1p, HistoStack* hs2p,
                                       const std::string& categ, const std::string& root_opt, const std::string& opt, double norm){
//**************************************************

  (void)norm; // norm not currently used

  // Draw 1D (ny=0) or 2D comparison between two data samples with different normalisation. Variable binning

  if (!HasCategory(categ)) return;
  std::string uopt = drawUtils::ToUpper(opt);
  std::string uroot_opt = drawUtils::ToUpper(root_opt);

  // Make sure the HistoStacks have been filled
  HistoStack* hs1 = NULL;
  HistoStack* hs2 = NULL;
  if (hs1p && (hs1p->GetTotal1D() || hs1p->GetTotal2D())) hs1=hs1p;
  if (hs2p && (hs2p->GetTotal1D() || hs2p->GetTotal2D())) hs2=hs2p;

  // Is a 2D histo ?
  bool is2D = false;
  if (hs1){
    if (hs1->Is2D()) is2D=true;
  }
  else if (hs2){
    if (hs2->Is2D()) is2D=true;
  }
  // options for 2D histos
  std::string box = "";
  std::string error1 = "";

  // Don't draw errors on first sample when option HIST is specified
  if (uroot_opt.find("HIST") == std::string::npos)
    error1=" e";
  else
    uopt= uopt+" NOERROR";

  if (is2D){
    box = " box";
    error1 = "";
  }

  std::string smallleg="";
  if (categ=="all") smallleg= " SMALLLEG";
  if (!hs2) uopt=uopt+" NOLEG";

  CreateLegend(uopt+smallleg);

  // Don't create the legend below
  uopt = uopt+ " NOCREATELEG";

  // 1. Add the data (first sample) entry to the legend, such that it goes first
  if (hs1 && hs2 && ! drawUtils::CheckOption(uopt,"NOLEG") && ! drawUtils::CheckOption(uopt,"NODATA"))
    drawUtils::AddLegendEntry(_legends.back(), hs1->GetTotal1D(), _data_label, "lpe");

  // 2. Draw the second sample (mc) with a specific category

  // must set the stat option before changing the legend position
  gStyle->SetOptStat(_stat_option);

  if (hs2 && ! drawUtils::CheckOption(uopt,"NOMC")) {
    if (hs1 && !is2D && ! drawUtils::CheckOption(uopt,"NODATA")){
      // Shift the position of the stat box to the left
      double w = gStyle->GetStatW();
      SetStatPos(_statPos[0]-w);
      // re-set the default stat position
      _statPos[0] = _statPos[0] + w;

      std::string uopt2 = uopt+" ALLMCLEG";
      // Don't show second sample stat when normalizing by area
      if (drawUtils::CheckOption(uopt,"AREA")) uopt2 = uopt2+" NOSTAT";

      // The root_opt is only for first sample
      DrawHistoStack(hs2,categ, GetSameRootOption(root_opt), uopt2+" ISSECONDSAMPLE");
    }
    else{
      // This is done for 2D histos and also when the NODATA option is specified. In this last case the second sample is treated similar to first
      SetStatPos(_statPos[0], _statPos[1]);
      DrawHistoStack(hs2,categ,root_opt+box,uopt+" ISSECONDSAMPLE");
    }
  }
  // 3. Draw first sample (data) on top and write the number of entries
  if (hs1 && ! drawUtils::CheckOption(uopt,"NODATA")) {
    SetStatPos(_statPos[0], _statPos[1]);
    std::string same="";
    if (hs2 && ! drawUtils::CheckOption(uopt,"NOMC")) same = " sames";
    DrawHistoStack(hs1, "all", root_opt+same+error1, uopt+" ISDATA DONTADDLEGENTRY",_line_color, _line_width, _fill_color, 0, "LE1P", 2);
  }


  // Don't draw
  if (drawUtils::CheckOption(uopt,"NODRAW")) return;

  // Set the appropriate maximum for 1D plots
  if (!is2D){
    bool draw1 = (hs1 && ! drawUtils::CheckOption(uopt,"NODATA"));
    bool draw2 = (hs2 && ! drawUtils::CheckOption(uopt,"NOMC"));

    double max = _maxY;
    double relmax=1.;
    if (fabs(_maxY) < 1e-6 || _maxY < _minY) {
      relmax=_relativeMaxY;
      double max2 = 0;
      if (draw2){
        if (drawUtils::CheckOption(uopt,"ETOT"))
          max2 = hs2->GetMaximumWithError(opt);
        else
          max2 = hs2->GetMaximum(opt);
      }
      if (draw1 && draw2) max = std::max(hs1->GetMaximumWithError(opt), max2);
      else if (draw1) max = hs1->GetMaximumWithError(opt);
      else if (draw2) max = max2;
    }
    if (categ!="all") relmax/=1.05;

    if (draw1) hs1->SetMaximum(max*relmax);
    if (draw2) hs2->SetMaximum(max*relmax);

    // set the minimun Y
    if (!drawUtils::CheckOption(uopt,"NOMIN") || _logY){
      if (draw1) hs1->SetMinimum(_minY);
      if (draw2) hs2->SetMinimum(_minY);
    }

    gPad->SetLogy(_logY);
    gPad->Update();
    gPad->Draw();
  }

}

//**************************************************
void DrawingToolsBase::DrawHisto(TH2* h, int lc, int lw, int fc, int fs, const std::string& root_opt, const std::string& opt,const std::string& leg_opt,int mode){
//**************************************************

  // Basic function to draw 2D histo

  (void)mode; // mode not currently used

  std::string uopt = drawUtils::ToUpper(opt);

  if (drawUtils::CheckOption(uopt,"NOSTAT"))
    gStyle->SetOptStat(0);
  else
    gStyle->SetOptStat(_stat_option);


  h->SetLineColor(lc);
  h->SetMarkerColor(lc);

  h->SetLineWidth(lw);
  h->SetFillColor(fc);
  h->SetFillStyle(fs);

  if (!drawUtils::CheckOption(uopt,"NOMIN"))
    h->SetMinimum(_minY);

  // Normalize the content to take into account variable binning
  //  drawUtils::NormalizeVariableBinning(h,mode,uopt);

  // get the title for the legend
  std::string title = h->GetTitle();

  h->GetXaxis()->SetTitle(_titleX.c_str());
  h->GetYaxis()->SetTitle(_titleY.c_str());

  h->SetTitle(_title.c_str());


  if (drawUtils::CheckInternalOption(uopt,"PROF")){
    TProfile* prof;
    if (drawUtils::CheckOption(uopt,"PROFX")){
      prof = h->ProfileX();
    }
    else if (drawUtils::CheckOption(uopt,"PROFY")){
      prof = h->ProfileY();
    }
    else{
      std::cout << "wrong profile type !!!!" << std::endl;
      return;
    }
    _saved_histos.push_back(prof);
    DrawHisto(prof,lc,lw,fc,fs, root_opt, uopt+" NOMIN",leg_opt);
  }
  else
    h->Draw(root_opt.c_str());

  gPad->SetLogz(_logZ);

  //Add entry to legend
  if (_drawleg &&  !drawUtils::CheckOption(uopt,"NOLEG") && title!="")
    drawUtils::AddLegendEntry(_legends.back(), h, title, leg_opt);

}

//**************************************************
void DrawingToolsBase::DrawHisto(TH3F* h, int lc, int lw, int fc, int fs, const std::string& root_opt){
//**************************************************

  // Basic function to draw 3D histo

  gStyle->SetOptStat(_stat_option);

  h->SetTitle("");

  h->SetLineColor(lc);
  h->SetLineWidth(lw);
  h->SetFillColor(fc);
  h->SetFillStyle(fs);

  h->SetMarkerColor(lc);

  h->Draw(root_opt.c_str());

}

//**************************************************
void DrawingToolsBase::ScaleHisto(TH1* h, double scale, bool scale_errors){
//**************************************************

  // Scale an histogram
  // if scale_errors=true, errors are also scaled
  // This is needed to draw comparisons or ratios between two data samples with different statistics

  if (scale==0 || scale ==1) return;

  if (scale_errors)
    h->Sumw2();
  h->Scale(scale);
}

//*********************************************************
double* DrawingToolsBase::GetVariableBins(int nx, double xmin, double xmax, double* xbins){
//*********************************************************

  for (int i=0;i<nx+1;i++)
    xbins[i]=  xmin+i*(xmax-xmin)/nx;

  return xbins;
}

//*********************************************************
std::string DrawingToolsBase::GetSameLevel(const std::string& root_opt ){
//*********************************************************

  std::string uroot_opt = drawUtils::ToUpper(root_opt);
  if (uroot_opt.find("SAME") == std::string::npos){
    _drawleg = false;
    _same_level=0;
  }
  else{
    _same_level++;
  }

  _same_level_tot++;

  std::stringstream slevel;
  slevel << _same_level;

  return slevel.str();
}

//*********************************************************
int DrawingToolsBase::GetFillStyle(int i) {
//*********************************************************

  if (_different_fill_styles && i < NAUTOCOLORS) {
    return auto_styles[i];
  } else {
    return _stack_fill_style;
  }
}

//*********************************************************
void DrawingToolsBase::CreateLegend(const std::string& uopt){
//*********************************************************


  if (drawUtils::CheckInternalOption(uopt,"NOCREATELEG") || drawUtils::CheckOption(uopt,"NOLEG")  ) return;

  // save the current legend size
  double sizex = _legendSize[0];
  double sizey = _legendSize[1];

  if (drawUtils::CheckInternalOption(uopt,"SMALLLEG")  ) {
    //      SetLegendSize(0.08,0.05);
    SetLegendSize(_legendSmallSize[0],_legendSmallSize[1]);
  }


  _legends.push_back(new TLegend(_legendParam[0],_legendParam[1],_legendParam[2],_legendParam[3]));
  _legends.back()->SetShadowColor(0);
  _legends.back()->SetFillColor(0);
  _drawleg = true;

  if (drawUtils::CheckOption(uopt,"PUR"))
    _legends.back()->SetTextAlign(32);
  else
    _legends.back()->SetTextAlign(12);

  SetLegendSize(sizex,sizey);
}

//*********************************************************
std::string DrawingToolsBase::GetUniqueName(const std::string& name) {
//*********************************************************
  std::stringstream unique;
  unique << _unique++;
  TString name2 = name;
  name2 = name2.ReplaceAll("(","_");
  name2 = name2.ReplaceAll(")","_");
  return std::string(name2.Data()) + "_" + unique.str();
}

//*********************************************************
TH1F* DrawingToolsBase::GetHisto(TTree* tree, const std::string& name, const std::string& var, int nx, double* xbins,
                                 const std::string& cut, const std::string& root_opt, const std::string& opt, double scale, bool scale_errors, int toy_ref){
//*********************************************************

  HistoStack* hs = NULL;
  return GetHisto(hs,tree,name,var,nx,xbins,cut,root_opt,opt,scale,scale_errors,toy_ref);
}

//*********************************************************
TH1F* DrawingToolsBase::GetHisto(HistoStack* hs, TTree* tree, const std::string& name, const std::string& var, int nx, double* xbins,
                                 const std::string& cut, const std::string& root_opt, const std::string& opt, double scale, bool scale_errors, int toy_ref){
//*********************************************************

  TH1F* hsyst = NULL;  
  return GetHisto(hs,tree,name,var,nx,xbins,cut,root_opt,opt,hsyst,scale,scale_errors,toy_ref);
}

//*********************************************************
TH1F* DrawingToolsBase::GetHisto(HistoStack* hs, TTree* tree, const std::string& name, const std::string& var, int nx, double* xbins,
                                 const std::string& cut, const std::string& root_opt, const std::string& opt, TH1F*& hsyst, double scale, bool scale_errors, int toy_ref){
//*********************************************************

  // tree:   the input tree
  // name:   name of the histogram
  // var:    variable to be plotted
  // nx:     number of bins
  // xbins:  vector of variable bins
  // cut:    cut to be applied when projecting the tree into the histo
  // opt:    plotting option (the ones of root + several specific to these tools)
  // scale:  scale factor for the histogram (useful when comparing different data samples)
  // scale_errors: specify whether errors have to be scaled as well
  // toy_ref: the reference toy index


  (void)root_opt; // root_opt not used here

  std::string uopt = drawUtils::ToUpper(opt);
  std::string name2 = GetUniqueName(name);

  std::string cut2=FormatCut(cut);

  // Don't show -999
  if (drawUtils::CheckOption(uopt,"NODEFAULT"))
    cut2 = cut2 + " && (" + var + "!=-999)";

  // Only plot the reference toy when it is indicated (toy_ref>-1)
  if (toy_ref>-1){
    std::stringstream stoy_ref;
    stoy_ref << toy_ref;
    cut2 = cut2 + " && toy_index=="+stoy_ref.str();
  }

  // Apply event weights
  std::string cutp  = weightTools::ApplyWeights(tree,cut,(uopt+" NOTOYW").c_str());
  std::string cut2p = weightTools::ApplyWeights(tree,cut2,uopt);

  // Create an histo and project the tree (only statistical errors)
  TH1F* hstat = new TH1F(name2.c_str(),"all",nx,xbins);
  _saved_histos.push_back(hstat);

  // Deal with overflow and underflow
  std::string newvar = var;
  if (drawUtils::CheckOption(uopt,"OVER")) {
    std::stringstream s;
    s << "((" << newvar << "<" << xbins[nx] << ")*(" << newvar << "))+((" << newvar << ">=" << xbins[nx] << ")*(" << (xbins[nx]+xbins[nx-1])/2. << "))";
    newvar = s.str();
  }
  if (drawUtils::CheckOption(uopt,"UNDER")) {
    std::stringstream s;
    s << "((" << newvar << ">" << xbins[0] << ")*(" << newvar << "))+((" << newvar << "<=" << xbins[0] << ")*(" << (xbins[0]+xbins[1])/2. << "))";
    newvar = s.str();
  }

  tree->Project(name2.c_str(),newvar.c_str(),cut2p.c_str());

  // Put the appropriate errors into the histogram
  if (!drawUtils::CheckInternalOption(uopt,"CAT"))
    FillHistoErrors(NULL, hs, NULL, tree,name2,var,nx,xbins,cutp,cutp,uopt,1,hstat,hsyst);

  else if(drawUtils::CheckInternalOption(uopt,"MATRIX"))
    UpdateSystInfo(NULL, hs, NULL, tree, var, nx, xbins, cutp, cutp, uopt,1);

  // scale the histo if requested
  ScaleHisto(hstat,scale,scale_errors);

  return hstat;
}

//*********************************************************
TH2F* DrawingToolsBase::GetHisto(TTree* tree, const std::string& name, const std::string& var, int nx, double* xbins, int ny, double* ybins,
                                 const std::string& cut, const std::string& root_opt, const std::string& opt, double scale, bool scale_errors, int toy_ref){
//*********************************************************

  // tree:   the input tree
  // name:   name of the histogram
  // var:    variable to be plotted
  // nx:     number of bins in x
  // xbins:  vector of variable bins in x
  // nx:     number of bins in y
  // xbins:  vector of variable bins in y
  // cut:    cut to be applied when projecting the tree into the histo
  // opt:    plotting option (the ones of root + several specific to these tools)
  // scale:  scale factor for the histogram (useful when comparing different data samples)
  // scale_errors: specify whether errors have to be scaled as well
  // toy_ref: the reference toy index

  (void)root_opt; // root_opt not used here

  std::string name2 = GetUniqueName(name);
  std::string uopt = drawUtils::ToUpper(opt);

  // Only plot the reference toy when it is indicated (toy_ref>-1)
  std::string cut2 = FormatCut(cut);
  if (toy_ref>-1){
    std::stringstream stoy_ref;
    stoy_ref << toy_ref;
    cut2 = (cut+" && toy_index=="+stoy_ref.str()).c_str();
  }

  // Apply event weights
  std::string cut2p  = weightTools::ApplyWeights(tree,cut2,uopt);

  // Create an histo and project the tree (only statistical errors)
  TH2F* hall = new TH2F(name2.c_str(),"all",nx,xbins,ny,ybins);
  _saved_histos2D.push_back(hall);
  tree->Project(name2.c_str(),var.c_str(),cut2p.c_str());

  // Put the appropriate errors into the histogram
  //  FillHistoErrors(tree,name2,var,nx,xbins,cut, opt,hall);

  // scale the histo if requested
  ScaleHisto(hall,scale,scale_errors);


  return hall;
}

//*********************************************************
TH1F* DrawingToolsBase::GetRatioHisto(HistoStack* hs1, HistoStack* hs2, const std::string& opt){
//*********************************************************

  std::string uopt = drawUtils::ToUpper(opt);

  if (!hs1->GetTotal1D()){
    std::cout << "ERROR. numerator does not exist !!!" << std::endl;
    return NULL;
  }

  if (!hs2->GetTotal1D()){
    std::cout << "ERROR. denominator does not exist !!!" << std::endl;
    return NULL;
  }


  // Histogram for numerator
  TH1F* h1 = hs1->GetTotal1D();
  TH1F* h2 = hs2->GetTotal1D();

  h1->Sumw2();
  h1->Sumw2();

  // compute the ratio
  TH1F* ratio = new TH1F(*h1);
  std::string name2 = GetUniqueName("ratio");
  ratio->SetName(name2.c_str());
  _saved_histos.push_back(ratio);
  ratio->Divide(h1,h2);

  // Put the errors into the ratio
  FillHistoErrors(hs1, hs2, ratio, uopt);

  return ratio;
}

//*********************************************************
TH1F* DrawingToolsBase::GetRatioHisto(TTree* tree1, TTree* tree2, const std::string& name, const std::string& var, int nx, double* xbins,
                                      const std::string& cut1, const std::string& cut2, 
                                      const std::string& root_opt, const std::string& opt, double norm, double scale, bool scale_errors, int toy_ref){
//*********************************************************

  // tree1:   the numerator input tree
  // tree2:   the denominator input tree
  // name:   name of the histogram
  // var:    variable to be plotted
  // nx:     number of bins
  // xbins:  vector of variable bins
  // cut1:    cut to be applied when projecting tree1 into the histo
  // cut2:    cut to be applied when projecting tree2 into the histo
  // opt:    plotting option (the ones of root + several specific to these tools)
  // norm:   normalization factor for second tree
  // scale:  scale factor for the histogram (useful when comparing different data samples)
  // scale_errors: specify whether errors have to be scaled as well
  // toy_ref: the reference toy index

  (void)root_opt; // root_opt not used here
  (void)scale; // Scaling not used
  (void)scale_errors; // Scaling not used

  std::string name2 = GetUniqueName(name);
  std::string uopt = drawUtils::ToUpper(opt);

  std::string cut1p = FormatCut(cut1);
  std::string cut2p = FormatCut(cut2);

  // Don't show -999
  if (drawUtils::CheckOption(uopt,"NODEFAULT")){
    cut1p = cut1p + " && (" + var + "!=-999)";
    cut2p = cut2p + " && (" + var + "!=-999)";
  }


  // Only plot the reference toy when it is indicated (toy_ref>-1)

  if (toy_ref>-1){
    std::stringstream stoy_ref;
    stoy_ref << toy_ref;
    cut1p = (cut1p+" && toy_index=="+stoy_ref.str()).c_str();
    cut2p = (cut2p+" && toy_index=="+stoy_ref.str()).c_str();
  }

  // Apply event weights
  std::string cut1pp = weightTools::ApplyWeights(tree1,cut1p,uopt);
  std::string cut2pp = weightTools::ApplyWeights(tree2,cut2p,uopt);

  // Deal with overflow and underflow
  std::string newvar = var;
  if (drawUtils::CheckOption(uopt,"OVER")) {
    std::stringstream s;
    s << "((" << newvar << "<" << xbins[nx] << ")*(" << newvar << "))+((" << newvar << ">=" << xbins[nx] << ")*(" << (xbins[nx]+xbins[nx-1])/2. << "))";
    newvar = s.str();
  }
  if (drawUtils::CheckOption(uopt,"UNDER")) {
    std::stringstream s;
    s << "((" << newvar << ">" << xbins[0] << ")*(" << newvar << "))+((" << newvar << "<=" << xbins[0] << ")*(" << (xbins[0]+xbins[1])/2. << "))";
    newvar = s.str();
  }

  // Create an histo and project the tree (only statistical errors)
  //  TH1F ratio1(GetUniqueName("ratio1").c_str(),"ratio1",nx,xbins);
  TH1F ratio1("ratio1","ratio1",nx,xbins);
  tree1->Project("ratio1",var.c_str(),cut1pp.c_str());

  //  TH1F ratio2(GetUniqueName("ratio2").c_str(),"ratio2",nx,xbins);
  TH1F ratio2("ratio2","ratio2",nx,xbins);
  tree2->Project("ratio2",var.c_str(),cut2pp.c_str());

  // scale the denominator
  ScaleHisto(&ratio2,norm);

  ratio1.Sumw2();
  ratio2.Sumw2();

  // compute the ratio
  TH1F* ratio = new TH1F(ratio1);
  ratio->SetName(name2.c_str());
  _saved_histos.push_back(ratio);

  ratio->Divide(&ratio1,&ratio2);

  // Put the appropriate errors into the histogram
  TH1F* hsyst;
  FillHistoErrors(NULL,NULL,tree1,tree2,name2,var,nx,xbins,cut1,cut2,uopt,norm,ratio,hsyst);

  return ratio;
}


//**************************************************
void DrawingToolsBase::FillHistoErrors(HistoStack* hs1, HistoStack* hs2, TH1F* histo, const std::string uopt){
//**************************************************

  (void)hs1;
  (void)hs2;
  (void)histo;
  (void)uopt;

}

//**************************************************
void DrawingToolsBase::FillGraphErrors(HistoStack* hs1, HistoStack* hs2, TGraphAsymmErrors* graph,  const std::string uopt){
//**************************************************

  (void)hs1;
  (void)hs2;
  (void)graph;
  (void)uopt;
}

//**************************************************
void DrawingToolsBase::FillHistoErrors(HistoStack* hs1, HistoStack* hs2, TTree* tree1,TTree* tree2, const std::string& name, const std::string& var, int nx, double* xbins,
                                       const std::string& cut1, const std::string& cut2, const std::string& opt, double norm, TH1F* hstat, TH1F*& hsyst) {
//**************************************************
  // TODO: This function isn't implemented. We just use all the variables to avoid warnings.
  (void)hs1;
  (void)hs2;
  (void)tree1;
  (void)tree2;
  (void)name;
  (void)var;
  (void)nx;
  (void)xbins;
  (void)cut1;
  (void)cut2;
  (void)norm;
  (void)opt;
  (void)hstat;
  (void)hsyst;
}

//**************************************************
void DrawingToolsBase::UpdateSystInfo(HistoStack* hs1, HistoStack* hs2, TTree* tree1,TTree* tree2, const std::string& var, int nx, double* xbins,
                                      const std::string& cut1, const std::string& cut2, const std::string& opt, double norm) {
//**************************************************
  // TODO: This function isn't implemented. We just use all the variables to avoid warnings.
  (void)hs1;
  (void)hs2;
  (void)tree1;
  (void)tree2;
  (void)var;
  (void)nx;
  (void)xbins;
  (void)cut1;
  (void)cut2;
  (void)norm;
  (void)opt;
}




//**************************************************
TH1F* DrawingToolsBase::GetEventsVSCut(TTree* tree, const std::string& name, const std::string& cut_norm, int isel, int ibranch, int& first_cut, int& last_cut,
                                       const std::string& root_opt, const std::string& opt){
//**************************************************

  // Check if selection exists
  if (!sel().GetSelection(isel,true)) return NULL;

  (void)root_opt; // root_opt not used here
  std::string uopt = drawUtils::ToUpper(opt);

  // Read the steps from the config tree
  //  ReadSteps(_config_file);

  if (last_cut>(int)(sel().GetSelection(isel)->GetNCuts(ibranch)-1) || last_cut == -1) last_cut=sel().GetSelection(isel)->GetNCuts(ibranch)-1; 
  if (first_cut<-1 || first_cut>last_cut ) first_cut=-1;

  double xmin=first_cut-0.5;
  double xmax=last_cut+0.5;
  int nx = (int)(xmax-xmin);
  double xbins[NMAXBINS];

  // Create an histo and project the tree (only statistical errors)
  TH1F* hall = new TH1F(GetUniqueName(name).c_str(),"events",nx,GetVariableBins(nx,xmin,xmax,xbins));
  _saved_histos.push_back(hall);

  std::string cut0 = FormatCut(cut_norm);

  std::vector<StepBase*> cuts = sel().GetSelection(isel)->GetCutsInBranch(ibranch);

  std::cout << std::endl <<"-------- List of selected cuts for branch " << ibranch;
  std::cout << ": #cuts = " << cuts.size() << "  -------" << std::endl;

  for (int i=0;i<nx;i++ ){
    int icut = first_cut+i;

    std::stringstream sbranch;
    sbranch << ibranch;
    std::stringstream ssel;
    ssel << isel;
    std::stringstream scut;
    scut << icut;

    Int_t accumLevelCut = std::max(icut,_minAccumLevelToSave-1);
    std::stringstream sAccumLevelCut;
    sAccumLevelCut << accumLevelCut;

    std::string cut;
    if (!strcmp(tree->GetName(), "truth")){
      if (sel().GetNEnabledSelections()>1)
        cut= cut0 + " && accum_level["+ssel.str()+"]["+sbranch.str()+"]>"+scut.str();
      else
        cut= cut0 + " && accum_level["+sbranch.str()+"]>"+scut.str();
    }
    else{
      if (sel().GetNEnabledSelections()>1)
        cut= cut0 + " && accum_level[]["+ssel.str()+"]["+sbranch.str()+"]>"+sAccumLevelCut.str();
      else
        cut= cut0 + " && accum_level[]["+sbranch.str()+"]>"+sAccumLevelCut.str();
    }

    // Apply event weights
    std::string cutp = weightTools::ApplyWeights(tree,cut,uopt);

    TH1F  htemp("temp","temp",1,0,1);
    tree->Project("temp","0.5",cutp.c_str());

    double ball = (double)htemp.Integral();
    hall->SetBinContent(i+1,ball);

    if (icut==-1)
      std::cout << ball << " events before any cut " << std::endl;
    else
      std::cout << ball << " events after cut " << icut << ":  " << cuts[icut]->Title() << std::endl;
  }

  std::cout << "---------------------------------------------------------------" << std::endl << std::endl;

  return hall;

}


//*********************************************************
TH1F* DrawingToolsBase::AddErrorsInQuadrature(TH1F* h1,TH1F* h2, const std::string& slevel){
//*********************************************************

  // Add the errors of histograms h1 and h2 in quadrature.
  // The contents are the ones of histogram h1

  TH1F *h3 = new TH1F(*h1);
  std::string name = "errors_quad_sum"+slevel;
  h3->SetName(name.c_str());
  _saved_histos.push_back(h3);

  // Set the sqrt of the diagonal as error
  for (int i=0;i<h1->GetNbinsX();i++){
    if (h1 && h2){
      // Require same number of bins
      if (h1->GetNbinsX() != h2->GetNbinsX()) return NULL;

      double err = sqrt(pow(h1->GetBinError(i+1),2)+pow(h2->GetBinError(i+1),2));
      h3->SetBinError(i+1,err);
      h3->SetBinContent(i+1,h1->GetBinContent(i+1));
    }
    else if (h1){
      h3->SetBinError(i+1,h1->GetBinError(i+1));
      h3->SetBinContent(i+1,h1->GetBinContent(i+1));
    }
    else if (h2){
      h3->SetBinError(i+1,h2->GetBinError(i+1));
      h3->SetBinContent(i+1,h2->GetBinContent(i+1));
    }
  }

  return h3;
}

//*********************************************************
std::string DrawingToolsBase::FormatCut(const std::string& cut) {
//*********************************************************

  // In the case the cut is empty
  std::string cut2=cut;
  if (cut2=="") cut2="1==1";

  // To avoid problems with || conditions
  cut2 ="("+cut2+")";
  return cut2;
}

//*********************************************************
double DrawingToolsBase::GetEntries(TTree* tree, const std::string& cut){
//*********************************************************
  TH1F *ht = new TH1F("temp"," ",10,0.,10.);

  std::string cut1 = FormatCut(cut);

  if (tree->FindLeaf("toy_ref"))
    cut1 = cut1 + "&& toy_index == toy_ref";

  // cut1 = "("+cut1+")"+w;


  cut1 = weightTools::ApplyWeights(tree,cut1,"");

  tree->Project("temp","1.",cut1.c_str());

  double TotalEntries = ht->GetSumOfWeights();
  delete ht;

  return TotalEntries;
}

//*********************************************************
double DrawingToolsBase::GetEntries(TTree* tree, const std::string& cut, const std::string& var, const std::string& opt, int toy_ref){
//*********************************************************
  // tree:   the input tree
  // var:    variable to be plotted
  // cut:    cut to be applied when projecting the tree into the histo
  // opt:    plotting option (the ones of root + several specific to these tools)
  // toy_ref: the reference toy index

  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return 0;

  // Only plot the reference toy when it is indicated (toy_ref>-1)
  std::string cut2 = FormatCut(cut);

  // Don't show -999
  if (drawUtils::CheckOption(uopt,"NODEFAULT")) {
    cut2 += "&&(" + var + "!=-999)";
  }

  if (toy_ref>-1){
    std::stringstream stoy_ref;
    stoy_ref << toy_ref;
    cut2 = (cut+" && toy_index=="+stoy_ref.str()).c_str();
  }

  // Apply event weights
  std::string cut2p = weightTools::ApplyWeights(tree,cut2,uopt);

  // Create an histo and project the tree (only statistical errors)
  double xbins[NMAXBINS];
  TH1F* hall = new TH1F("temp","all",10,GetVariableBins(10,-5.,5.,xbins));
  _saved_histos.push_back(hall);

  // Include all possible values of "var" ( newvar = var*var/(var*var+10), so it's always between 0 and 1 )
  std::string newvar = var;
  std::stringstream s;
  s << "((" << newvar << ")*(" << newvar << ")/((" << newvar << ")*(" << newvar << ")+10.))";
  newvar = s.str();

  tree->Project("temp",newvar.c_str(),cut2p.c_str());

  // Return sum of all weights
  double TotalEntries = hall->GetSumOfWeights();
  delete hall;

  return TotalEntries;
}

//*********************************************************
void DrawingToolsBase::PrintPurities(TTree* tree, const std::string& categ,  const std::string& cut, double events_ratio){
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

  TH1F *ht = new TH1F("temp"," ",10,0.,10.);

  std::string cut1 = FormatCut(cut);
  if (tree->FindLeaf("toy_ref"))
    cut1 = cut1 + " && toy_index == toy_ref";

  //  std::string cut1w = "("+cut1+")"+w;
  std::string cut1w = weightTools::ApplyWeights(tree,cut,"");

  tree->Project("temp","1.",cut1w.c_str());

  double TotalEntries = ht->GetSumOfWeights();
  delete ht;


  std::cout << " --------------------------------------------------------" << std::endl;
  std::cout << " Purities: " << std::endl;
  std::cout << "  Category: " << categ << std::endl;
  std::cout << "  Cut:      " << cut << std::endl;
  std::cout << "  Events:   " << TotalEntries << std::endl;
  std::cout << " --------------------------------------------------------" << std::endl;

  // define stuff to print % on the legend
  std::ostringstream percstr, str_tmp;
  const int ntypes = (const int)cat().GetCategoryTypes(categ).size();
  std::string* cattypes = new std::string[ntypes]; // to initialize with const dimension
  int* catcodes = new int[ntypes]; // to initialize with const dimension
  int* catcolors = new int[ntypes]; // to initialize with const dimension
  int itype=0;

  std::vector<TrackTypeDefinition>::iterator it;
  for (it=cat().GetCategoryTypes(categ).begin();it!=cat().GetCategoryTypes(categ).end();it++, i++){
    std::string type = it->_name;
    std::string code = drawUtils::GetString(it->_code);

    std::string cut2 = cut1+"&&"+categ+"=="+code;

    cut2 = weightTools::ApplyWeights(tree,cut2,"");
    //    cut2 = "("+cut2+")"+w;

    TH1F *ht1 = new TH1F("temp1"," ",10,0.,10.);

    tree->Project("temp1","1.",cut2.c_str());

    double caseEntries = ht1->GetSumOfWeights();
    double frac = caseEntries/TotalEntries;
    double nev1 = caseEntries * events_ratio;
    delete ht1;

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

//********************************************************************
void DrawingToolsBase::ChangeCategory(const std::string& categ_name, int ntypes, std::string* names, int* codes, int* colors){
//********************************************************************

  // Read the categories from the config tree
  ReadCategories(_config_file);

  bool multi = false;
  bool noWarning = false; 

  // don't add automatic categories 
  bool addNOTRUTH=false; 
  bool addSAND=false;


  cat().AddCategory(categ_name, ntypes, names, codes, colors, multi, noWarning, addNOTRUTH, addSAND);
}

//********************************************************************
bool DrawingToolsBase::HasCategory(const std::string& categ){
//********************************************************************

  // Read the categories from the config tree
  ReadCategories(_config_file);

  if (categ!="all"){
    if (!cat().HasCategory(categ)){
      std::cout << "Category '" << categ << "' does not exist" << std::endl;
      return false;
    }
    else DumpCategory(categ);
  }
  return true;
}


//*********************************************************
void DrawingToolsBase::PrintEventNumbers(TTree* tree, const std::string& cut, const std::string& file, int toy_ref){
//*********************************************************

  // tree:   the input tree
  // cut:    cut to be applied when projecting the tree into the histo
  // file:   the reference toy index
  // toy_ref: the reference toy index

  if ( !( drawUtils::TreeHasVar(tree, "evt") && drawUtils::TreeHasVar(tree, "run") && drawUtils::TreeHasVar(tree, "subrun"))){
    std::cout << "  Invalid tree. It is missing one or all of the following branches: evt, run, subrun " << std::endl;
    return;
  }

  // Only plot the reference toy when it is indicated (toy_ref>-1)
  std::string cut2 = FormatCut(cut);
  if (toy_ref>-1){
    std::stringstream stoy_ref;
    stoy_ref << toy_ref;
    cut2 = (cut+" && toy_index=="+stoy_ref.str()).c_str();
  }

  // Apply event weights
  std::string cutp = weightTools::ApplyWeights(tree,cut2,"");

  ofstream output;
  if (file!="")
    output.open (file.c_str());

  // Copy the entries we are interested into another tree
  char* tmpFilename= new char;
  strcpy(tmpFilename,"/tmp/XXXXXX");
  mktemp (tmpFilename);
  TFile *tmpFile = new TFile(tmpFilename, "RECREATE");
  TTree *tmpTree = tree->CopyTree(cutp.c_str());
  int run, subrun, evt;
  tmpTree->SetBranchAddress("run", &run);
  tmpTree->SetBranchAddress("subrun", &subrun);
  tmpTree->SetBranchAddress("evt", &evt);
  // Print out the event, run and subrun numbers
  Long64_t nentries = tmpTree->GetEntries();
  if (file=="")
    std::cout<<"# run,subrun,evt"<<std::endl;
  for (Long64_t i=0;i<nentries; i++) {
    tmpTree->GetEntry(i);
    if (file!="")
      output<<run<<","<<subrun<<","<<evt<<std::endl;
    else
      std::cout<<run<<","<<subrun<<","<<evt<<std::endl;
  }
  // Clean up
  output.close();
  tmpFile->Close();
  remove(tmpFilename);

}

//*********************************************************
void DrawingToolsBase::DrawCutLineVertical(double xval, bool addarrow, std::string arrowopt, double arrowpos) {
//*********************************************************

  double ymin = gPad->PadtoY(gPad->GetUymin());
  double ymax = gPad->PadtoY(gPad->GetUymax());
  DrawCutLine(xval, ymin, xval, ymax);

  if (addarrow) {
    std::string uarrowdir = drawUtils::ToUpper(arrowopt);
    double deltax = xval-gPad->PadtoX( gPad->XtoPad(xval)-(gPad->GetUxmax()-gPad->GetUxmin())/20.);

    if (uarrowdir.find("L") != std::string::npos) {
      deltax *= -1;
    }

    // It needs this to work in LogY scale
    double yarr = gPad->PadtoY((gPad->GetUymax()-gPad->GetUymin())*arrowpos+gPad->GetUymin());
    double xarrmax = xval + deltax;
    DrawCutArrow(xval, yarr, xarrmax, yarr);
  }
}

//*********************************************************
void DrawingToolsBase::DrawCutLineHorizontal(double yval, bool addarrow, std::string arrowdir, double arrowpos) {
//*********************************************************
  double xmin = gPad->PadtoX(gPad->GetUxmin());
  double xmax = gPad->PadtoX(gPad->GetUxmax());
  DrawCutLine(xmin, yval, xmax, yval);

  if (addarrow) {
    std::string uarrowdir = drawUtils::ToUpper(arrowdir);
    double deltay = yval-gPad->PadtoY( gPad->YtoPad(yval)-(gPad->GetUymax()-gPad->GetUymin())/20.);

    if (uarrowdir.find("D") != std::string::npos) {
      deltay *= -1;
    }

    // It needs this to work in LogX scale
    double xarr = gPad->PadtoX((gPad->GetUxmax()-gPad->GetUxmin())*arrowpos+gPad->GetUxmin());
    double yarrmax = yval + deltay;
    DrawCutArrow(xarr, yval, xarr, yarrmax);
  }
}

//*********************************************************
void DrawingToolsBase::DrawCutLine(double xmin, double ymin, double xmax, double ymax) {
//*********************************************************
  TLine* l = new TLine(xmin, ymin, xmax, ymax);

  l->SetLineColor(_cut_line_col);
  l->SetLineWidth(_cut_line_width);
  _cut_lines.push_back(l);
  l->Draw();

  if (_drawleg && _legends.size() > 0 && gPad->FindObject(_legends.back())) {
    _legends.back()->Draw();
  }
}

//*********************************************************
void DrawingToolsBase::DrawCutArrow(double xmin, double ymin, double xmax, double ymax) {
//*********************************************************
  TArrow* arr = new TArrow(xmin, ymin, xmax, ymax, 0.02, "|>");
  arr->SetLineColor(_cut_line_col);
  arr->SetFillColor(_cut_line_col);
  arr->SetLineWidth(_cut_line_width);
  _cut_lines.push_back(arr);
  arr->Draw();

  if (_drawleg && _legends.size() > 0 && gPad->FindObject(_legends.back())) {
    _legends.back()->Draw();
  }
}

//*********************************************************
void DrawingToolsBase::DrawCutRegion(double xmin, double ymin, double xmax, double ymax, std::string opt) {
//*********************************************************
  std::string uopt = drawUtils::ToUpper(opt);

  if (!drawUtils::CheckOption(uopt,"NOCHOP")) {
    xmin = std::max(xmin, gPad->PadtoX(gPad->GetUxmin()));
    xmax = std::min(xmax, gPad->PadtoY(gPad->GetUxmax()));
    ymin = std::max(ymin, gPad->PadtoX(gPad->GetUymin()));
    ymax = std::min(ymax, gPad->PadtoY(gPad->GetUymax()));
  }

  if (uopt.find("T")!=std::string::npos) {
    DrawCutLine(xmin, ymax, xmax, ymax);
  }
  if (uopt.find("B")!=std::string::npos) {
    DrawCutLine(xmin, ymin, xmax, ymin);
  }
  if (uopt.find("L")!=std::string::npos) {
    DrawCutLine(xmin, ymin, xmin, ymax);
  }
  if (uopt.find("R")!=std::string::npos) {
    DrawCutLine(xmax, ymin, xmax, ymax);
  }
}

//*********************************************************
void DrawingToolsBase::SetMinY(double minY) {
//*********************************************************

  if (minY<=0 && _logY) {
    std::cout << "Cannot set minimum <= 0 in logY scale!  Call SetLogY(false) before calling SetMinY()!" << std::endl;
    return;
  }
  _minY = minY;
}

//*********************************************************
void DrawingToolsBase::SetLogY(bool logY) {
//*********************************************************
  if (logY) {
    if (_minY > 0) {
      _logY = logY;
    } else {
      std::cout << "Cannot set a log-scale if minimum is <= 0! Call SetMinY() before calling SetLogY()!" << std::endl;
      _logY = false;
    }
  }  else {
    _logY = false;
  }
}

//*********************************************************
void DrawingToolsBase::SetLogZ(bool logZ) {
//*********************************************************
  _logZ = logZ;
}

//*********************************************************
void DrawingToolsBase::StartDocument(const std::string& title, bool pdf, bool web, const std::string& pdffile, const std::string& webpath) {
//*********************************************************
  if (_pdfcanvas) {
    delete _pdfcanvas;
  }
  _pdfpath = "";
  _webpath = "";
  _webheader = "";
  _webbody = "";

  if (web) {
    _webpath = webpath;
    MakeDirectory(_webpath, 0755);
    _webheader += "<html><head><title>" + title + "</title></head><body>\n";
    _webheader += "<h2>" + title + "</h2>\n<ol>\n";
    _webbody += "</ol>\n";
  }
  if (pdf) {
    _pdfpath = pdffile;

    if (!gPad) {
      // User hadn't set up a canvas - create one for them.
      // We split it into 2 sections, so we can save useful info
      // in the small top "header" bit.
      _pdfcanvas = new TCanvas("pdfcanvas", "pdfcanvas", 800, 630);
      _headpad = new TPad("header", "header", 0, 0.95, 1, 1, 0);
      _headpad->SetNumber(1);
      _headpad->Draw();
      _bodypad = new TPad("body", "body", 0, 0, 1, 0.95, 0);
      _bodypad->SetNumber(2);
      _bodypad->Draw();
      _pdfcanvas->Draw();
      _bodypad->cd();
    }

    std::cout << _pdfpath << std::endl;

    gPad->Print((_pdfpath+"[").c_str());
  }
}

//*********************************************************
void DrawingToolsBase::AddDocumentPage(const std::string& name) {
//*********************************************************
  if (_webpath != "") {
    TString safename = name;
    safename = safename.ReplaceAll(" ", "_");
    gPad->SaveAs((_webpath + "/" + safename.Data() + ".png").c_str());
    _webheader += std::string("<li><a href=\"#") + safename.Data() + "\">" + name + "</a></li>\n";
    _webbody += std::string("<p><a name=\"") + safename.Data() + "\"></a>" + name + "</p>\n";
    _webbody += std::string("<p><img src=\"") + safename.Data() + ".png\" alt=\"" + name + "\"/></p>\n";
  }

  if (_pdfpath != "") {
    TVirtualPad* currpad = gPad;

    if (currpad && currpad == _bodypad) {
      _headpad->Clear();
      _headpad->cd();
      TLatex l;
      l.SetTextSize(0.5);
      l.SetTextAlign(22);
      l.DrawLatex(0.5, 0.5, name.c_str());

      _pdfcanvas->cd();
      _pdfcanvas->Draw();
      _pdfcanvas->Print(_pdfpath.c_str());

      currpad->cd();
    } else {
      currpad->Print(_pdfpath.c_str());
    }
  }
}

//*********************************************************
void DrawingToolsBase::FinishDocument() {
//*********************************************************
  if (_webpath != "") {
    _webbody += "</body></html>\n";
    std::ofstream outfile;
    outfile.open((_webpath+"/index.html").c_str());
    outfile << _webheader << _webbody;
    outfile.close();
  }
  if (_pdfpath != "") {
    gPad->Print((_pdfpath+"]").c_str());
  }
}

//*********************************************************
int DrawingToolsBase::MakeDirectory(const std::string& path, mode_t mode) {
//*********************************************************
  size_t pre = 0, pos;
  std::string dir;
  std::string s = path;
  int mdret;

  if (s[s.size() - 1] != '/') {
    // force trailing / so we can handle everything in loop
    s += "/";
  }

  while ((pos = s.find_first_of("/", pre))) {
    dir = s.substr(0, pos++);
    pre = pos;
    if (dir.size() == 0)
      continue; // if leading / first time is 0 length
    if ((mdret = mkdir(dir.c_str(), mode)) && errno != EEXIST) {
      return mdret;
    }
  }

  return mdret;
}

//*********************************************************
void DrawingToolsBase::SetOptStat(Option_t *stat) {
//*********************************************************

  Int_t mode=0;
  TString opt = stat;

  if (opt.Contains("n")) mode+=1;
  if (opt.Contains("e")) mode+=10;
  if (opt.Contains("m")) mode+=100;
  if (opt.Contains("M")) mode+=200;
  if (opt.Contains("r")) mode+=1000;
  if (opt.Contains("R")) mode+=2000;
  if (opt.Contains("u")) mode+=10000;
  if (opt.Contains("o")) mode+=100000;
  if (opt.Contains("i")) mode+=1000000;
  if (opt.Contains("I")) mode+=2000000;
  if (opt.Contains("s")) mode+=10000000;
  if (opt.Contains("S")) mode+=20000000;
  if (opt.Contains("k")) mode+=100000000;
  if (opt.Contains("K")) mode+=200000000;
  if (mode == 1) mode = 1000000001;

  return SetOptStat(mode);
}


//*********************************************************
std::string DrawingToolsBase::GetErrorStyle(const std::string& uopt) {
//*********************************************************

  if      (drawUtils::CheckOption(uopt,"E0")) return "e0";
  else if (drawUtils::CheckOption(uopt,"E1")) return "e1";
  else if (drawUtils::CheckOption(uopt,"E2")) return "e2";
  else if (drawUtils::CheckOption(uopt,"E3")) return "e3";
  else if (drawUtils::CheckOption(uopt,"E4")) return "e4";
  else if (drawUtils::CheckOption(uopt,"E5")) return "e5";
  else if (drawUtils::CheckOption(uopt,"E6")) return "e6";
  else return "";
}

//********************************************************************
void DrawingToolsBase::DumpConfiguration(const std::string& name){
//********************************************************************

  ConfigurationBase* con = conf().GetConfiguration(name);
  if (!con) return;

  con->Dump(syst());
}

//********************************************************************
void DrawingToolsBase::DrawVariations(const std::string& name, Int_t ipar, int nx, double xmin, double xmax, const std::string& root_opt, const std::string& opt, const std::string& leg){
//********************************************************************

  ConfigurationBase* con = conf().GetConfiguration(name);
  if (!con) return;

  std::string uopt = drawUtils::ToUpper(opt);

  // Check that all user options are valid
  if (!drawUtils::ContainValidOptions(uopt)) return;

  std::string slevel = GetSameLevel(root_opt);
  if (slevel=="0" && leg!="")
    CreateLegend();

  // Stat option (entries, MEAN, RMS, and integral)
  int statTemp = _stat_option;
  SetOptStat(1110);
  gStyle->SetOptStat(1110);

  TH1F* hvar = new TH1F(GetUniqueName("hvar").c_str(),leg.c_str(),nx,xmin,xmax);
  _saved_histos.push_back(hvar);


  // Get the variations
  for (Int_t itoy=0;itoy<con->GetNToys();itoy++){
    if (ipar==-1){
      for (UInt_t ipar2=0;ipar2<con->_toys[itoy].npar;ipar2++)
        hvar->Fill(con->_toys[itoy].variations[ipar2]);
    }
    else
      hvar->Fill(con->_toys[itoy].variations[ipar]);
  }

  hvar->SetTitle(_title.c_str());

  // Draw the entries histogram
  //  DrawHisto(hvar, _line_width, _line_color, _fill_style, root_opt+" H", uopt+ "NOVARBIN", "LE1P");
  DrawHisto(hvar, _line_width, _line_color, _fill_style, root_opt, uopt, "LE1P");

  hvar->GetXaxis()->SetTitle("variation");
  hvar->GetYaxis()->SetTitle("# toys");

  // Draw the legend
  if (!drawUtils::CheckOption(uopt,"NOLEG") && leg!="")
    _legends.back()->Draw();

  if (!drawUtils::CheckOption(uopt,"NODRAW"))
    gPad->Update();

  // go back to previous stat option
  SetOptStat(statTemp);
}

//*********************************************************
void DrawingToolsBase::DumpOriginalSoftwareVersion(const std::string& file, bool all) {
//*********************************************************

  // Use the file used to create the DrawingTools if no file is specified
  std::string file2 = file;
  if (file=="") file2 = _config_file;

  // And read the new ones from the file
  TChain* chain = new TChain("header");
  chain->AddFile(file2.c_str());

  if (!drawUtils::TreeHasVar(chain,"SoftwareVersion")){
    delete chain;
    chain = new TChain("config");
    chain->AddFile(file2.c_str());
    if (!drawUtils::TreeHasVar(chain,"SoftwareVersion")) return;
  }

  char version[20];
  chain->SetBranchAddress("SoftwareVersion", version);

  Long64_t centry = chain->LoadTree(0);
  if (centry<0){
    std::cout << "failed in reading SoftwareVersion !!!" << std::endl;
    return;
  }
  Int_t nb = chain->GetEntry(0);
  if (nb==0){
    std::cout << "failed in reading SoftwareVersion !!!" << std::endl;
    return;
  }

  if (all){
    std::cout << "------ Software Version for original input files -------------------------------------" << std::endl;
    std::cout << "file:                " << file << std::endl;
    std::cout << "version:             " << version << std::endl;
  }
  else
    std::cout << "software version used to produce original input files:  " << version << std::endl;
}


//*********************************************************
void DrawingToolsBase::DumpSoftwareVersions(const std::string& file){
//*********************************************************

  // Use the file used to create the DrawingTools if no file is specified
  std::string file2 = file;
  if (file=="") file2 = _config_file;

  if (file2!="") ND::versioning().ReadVersions(file2);
  ND::versioning().DumpVersions();
  DumpOriginalSoftwareVersion(file2, false);
}

//*********************************************************
void DrawingToolsBase::DumpFileInfo(const std::string& file) {
//*********************************************************

  // And read the new ones from the file
  TChain* chain = new TChain("config");
  chain->AddFile(file.c_str());

  char cmtpath[200]="unknown";
  char hostname[50]="unknown";
  char inputFile[200]="unknown";
  char OriginalFile[200]="unknown";
  if (drawUtils::TreeHasVar(chain,"CMTPATH")){    
    chain->SetBranchAddress("CMTPATH", cmtpath);
    chain->SetBranchAddress("HOSTNAME", hostname);
    chain->SetBranchAddress("InputFile", inputFile);
    if (drawUtils::TreeHasVar(chain,"OriginalFile"))        
      chain->SetBranchAddress("OriginalFile", OriginalFile);


    Long64_t centry = chain->LoadTree(0);
    if (centry<0){
      std::cout << "failed in reading config tree !!!" << std::endl;
      return;
    }
    Int_t nb = chain->GetEntry(0);
    if (nb==0){
      std::cout << "failed in reading config tree !!!" << std::endl;
      return;
    }
  }

  std::cout << "====================== FILE INFORMATION ===================================" << std::endl;
  std::cout << std::endl;
  std::cout << "CMTPATH:                  " << cmtpath << std::endl;
  std::cout << "HOSTNAME:                 " << hostname << std::endl;
  std::cout << "Input file:               " << inputFile << std::endl;
  std::cout << "original file:            " << OriginalFile << std::endl;

  DumpSoftwareVersions(file);

  // Not available in Flat and Mini trees
  if (drawUtils::TreeHasVar(chain,"NSEL"))  DumpSelections(file);
  if (drawUtils::TreeHasVar(chain,"NCONF")) DumpConfigurations(file);

  DumpCorrections(file);

  std::cout << "===========================================================================" << std::endl;

}


//*********************************************************
void DrawingToolsBase::ReadOther(const std::string& file) {
//*********************************************************

  // And read the new ones from the file
  TChain chain("config");
  chain.AddFile(file.c_str());
  ReadOther(&chain);
}

//*********************************************************
void DrawingToolsBase::ReadOther(TTree* config) {
//*********************************************************

  _minAccumLevelToSave = 0;
  if (drawUtils::TreeHasVar(config,"MinAccumLevelToSave")){
    config->SetBranchAddress("MinAccumLevelToSave", &_minAccumLevelToSave);

    Long64_t centry = config->LoadTree(0);
    if (centry<0){
      std::cout << "failed in reading config tree !!!" << std::endl;
      return;
    }
    Int_t nb = config->GetEntry(0);
    if (nb==0){
      std::cout << "failed in reading config tree !!!" << std::endl;
      return;
    }
  }
}

//*********************************************************
std::string DrawingToolsBase::AddRangeCut(const std::string& var,  int nx, double* xbins, int ny, double* ybins, const std::string& cut, const std::string& uopt) {
//*********************************************************

  (void)ny;
  (void)ybins;

  std::string cut2 = FormatCut(cut);
  std::stringstream scut;

  if (!drawUtils::CheckOption(uopt,"UNDER") || !drawUtils::CheckOption(uopt,"OVER")){
    scut << "(" << cut2;

    if (!drawUtils::CheckOption(uopt,"UNDER")){
      scut << " && " << var << ">=" << xbins[0];
    }
    if (!drawUtils::CheckOption(uopt,"OVER")){
      scut << " && " << var << "<" << xbins[nx];
    }
    scut << ")";
  }
  else{
    scut << cut2;
  }

  return scut.str();
}

//*********************************************************
std::string DrawingToolsBase::GetSameRootOption(const std::string& root_opt) {
//*********************************************************

  std::string uroot_opt = drawUtils::ToUpper(root_opt);
  if (uroot_opt.find("SAMES")!=std::string::npos)
    return " sames";
  else if (uroot_opt.find("SAME")!=std::string::npos)
    return " same";
  else return "";
}

//*********************************************************
std::string DrawingToolsBase::GetNoSameRootOption(const std::string& root_opt) {
//*********************************************************

  std::string uroot_opt = drawUtils::ToUpper(root_opt);
  bool found=true;

  while(found){
    if (uroot_opt.find("SAMES")!=std::string::npos){
      uroot_opt = uroot_opt.replace(uroot_opt.find("SAMES"), 5, std::string(""));
      found=true;
    }
    else if (uroot_opt.find("SAME")!=std::string::npos){
      uroot_opt = uroot_opt.replace(uroot_opt.find("SAME"), 4, std::string(""));
      found=true;
    }
    else found=false;
  }

  return uroot_opt;
}

//*********************************************************
void DrawingToolsBase::ChangeLegendEntry(Int_t index, const std::string& new_label, const std::string& new_opt) { 
//*********************************************************

    TLegend* leg0 = GetLastLegend();

    std::vector<TLegendEntry*> entries;

    // Clear the output Legend
    while (leg0->GetNRows()>0){
      entries.push_back(new TLegendEntry(leg0->GetEntry()->GetObject(), leg0->GetEntry()->GetLabel(), leg0->GetEntry()->GetOption()));
      leg0->DeleteEntry();
    }

    // Put 0 height to the new legend
    if (drawUtils::legendEntryHeight>0)
      leg0->SetY1NDC(leg0->GetY2NDC());

    // Change the label/option of the entry with a given index 
    for (UInt_t i=0;i<entries.size();i++){
      const char* label = entries[i]->GetLabel();
      const char* opt   = entries[i]->GetOption();
      if ((Int_t)i==index && new_label!="") label = new_label.c_str();
      if ((Int_t)i==index && new_opt!=""  ) opt   = new_opt.c_str();
      drawUtils::AddLegendEntry(leg0, entries[i]->GetObject(),(std::string)label,(std::string)opt);
      delete entries[i];
    }

    entries.clear();
    leg0->Draw();
}

//*********************************************************
void DrawingToolsBase::DeleteLegendEntry(Int_t index) {
//*********************************************************

    TLegend* leg0 = GetLastLegend();

    std::vector<TLegendEntry*> entries;

    // Clear the output Legend
    while (leg0->GetNRows()>0){
      entries.push_back(new TLegendEntry(leg0->GetEntry()->GetObject(), leg0->GetEntry()->GetLabel(), leg0->GetEntry()->GetOption()));
      leg0->DeleteEntry();
    }

    // Put 0 height to the new legend
    if (drawUtils::legendEntryHeight>0)
      leg0->SetY1NDC(leg0->GetY2NDC());


    // Change the label/option of the entry with a given index 
    for (UInt_t i=0;i<entries.size();i++){
      if ((Int_t)i!=index)
        drawUtils::AddLegendEntry(leg0, entries[i]->GetObject(),entries[i]->GetLabel(),entries[i]->GetOption());
      delete entries[i];
    }

    entries.clear();
    leg0->Draw();
}


//*********************************************************
std::string DrawingToolsBase::ConvertCutForTruthTree(const std::string cut, bool replace_selmu) {
//*********************************************************

    std::string cut2 = cut;

    // replace accum_level[] with accum_level
    while (true) {
      size_t pos = cut2.find("accum_level[]");
      if (pos == std::string::npos) break;
      std::string pre = cut2.substr(0,pos);
      std::string post = cut2.substr(pos+strlen("accum_level[]"));
      cut2 = pre + "accum_level" + post;
    }

    if ( ! replace_selmu) return cut2;

    // replace selmu_ with truelepton_
    while (true) {
      size_t pos = cut2.find("selmu_");
      if (pos == std::string::npos) break;
      std::string pre = cut2.substr(0,pos);
      std::string post = cut2.substr(pos+strlen("selmu_"));
      cut2 = pre + "truelepton_" + post;
    }

    return cut2;
}

//********************************************************************
int DrawingToolsBase::GetNDOF(const TH1* h1, const TH1* h2) {
//********************************************************************
    int ndof=0;
    for (int i=1; i<=h1->GetXaxis()->GetNbins(); i++) {
      if (h1->GetBinContent(i)==0 && h2->GetBinContent(i)==0) continue;
      ndof++;
    }
    ndof--;
    return ndof;
}

//********************************************************************
 double DrawingToolsBase::GetChi2(const TH1* h1, const TH1* h2) {
 //********************************************************************

    if (h1->GetXaxis()->GetNbins() != h2->GetXaxis()->GetNbins())
      std::cout << "WARNING: the two histograms have different number of bins!" << std::endl;

    TH1* h1temp = (TH1*)h1->Clone();
    TH1* h2temp = (TH1*)h2->Clone();

    // Refill to avoid dealing with normalizing factors
    h1temp->Reset();
    h2temp->Reset();
    for (int i=1; i<=h1temp->GetXaxis()->GetNbins(); i++) {
      h1temp->SetBinContent(i,h1->GetBinContent(i));
      h2temp->SetBinContent(i,h2->GetBinContent(i));
      h1temp->SetBinError(i,h1->GetBinError(i));
      h2temp->SetBinError(i,h2->GetBinError(i));
    }

    double chi2=0;
    int ndof=0;
    for (int i=1; i<=h1temp->GetXaxis()->GetNbins(); i++) {
      if (h1temp->GetBinContent(i)==0 && h2temp->GetBinContent(i)==0) continue;
      ndof++;
      double diff = h1temp->GetBinContent(i) - h2temp->GetBinContent(i);
      double err1 = h1temp->GetBinError(i);
      double err2 = h2temp->GetBinError(i);
      double errsq = pow(err1,2) + pow(err2,2);
      if (errsq == 0 || errsq != errsq) {
        if (diff != 0)
          std::cout << "ERROR errsq = 0! \n";
        else {
          std::cout << "WARNING diff=0 and errsq= 0 (set to 1) \n";
          errsq=1;
          diff=1;
        }
      }
      chi2 += pow(diff,2) / errsq;
    }
    ndof--;
/*
    double chi2test;
    int ndoftest;
    int igood;
    double pvalue = h2temp->Chi2TestX(h1temp,chi2test,ndoftest,igood,"WW");

    cout << "my chi2/ndof (pvalue) = " << chi2 << " / " << ndof
         << " (" << TMath::Prob(chi2,ndof) << ")" << endl;
    cout << "my Chi2TestX (pvalue) = " << chi2test << " / " << ndoftest
         << " (" << TMath::Prob(chi2test,ndoftest) << ")" << endl;
*/
    delete h1temp;
    delete h2temp;

    return chi2;
}

