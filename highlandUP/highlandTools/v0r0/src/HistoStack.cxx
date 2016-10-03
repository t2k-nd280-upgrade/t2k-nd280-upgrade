#include "HistoStack.hxx"
#include <cmath>
#include "TCanvas.h"

//********************************************************
HistoStack::HistoStack(const std::string& title, const std::string& titleX, const std::string& titleY) {
//*******************************************************

  _title = title;
  _titleX = titleX;
  _titleY = titleY;

  _hall1D = NULL;
  _hall2D = NULL;

  _hall1D_stat = NULL;
  _hall1D_syst = NULL;

  _is1D = false;
  _is2D = false;

  _stack = NULL;
  _hall1D_draw = NULL;

  //  _h1 = _h2 = _h1w = NULL;

  _systHistosGroups.clear();

  _currentSystGroup ="default";

}

//********************************************************
HistoStack::~HistoStack() {
//*******************************************************

  if (_hall1D) delete _hall1D;
  if (_hall2D) delete _hall2D;

  if (_hall1D_syst) delete _hall1D_syst;
  if (_hall1D_stat) delete _hall1D_stat;

  std::map<std::string, TH1F*>::iterator itm1;
  std::map<std::string, TH2F*>::iterator itm2;
  for (itm2 = _h1.begin(); itm2 != _h1.end(); itm2++)
    if (itm2->second) delete itm2->second;
  _h1.clear();

  for (itm2 = _h2.begin(); itm2 != _h2.end(); itm2++)
    if (itm2->second) delete itm2->second;
  _h2.clear();

  for (itm2 = _h1w.begin(); itm2 != _h1w.end(); itm2++)
    if (itm2->second) delete itm2->second;
  _h1w.clear();

  _systHistosGroups.clear();

  std::vector<TH1F*>::iterator it1;
  for (it1 = _histos1D.begin(); it1 != _histos1D.end(); it1++) delete *it1;
  _histos1D.clear();

  std::vector<TH2F*>::iterator it2;
  for (it2 = _histos2D.begin(); it2 != _histos2D.end(); it2++) delete *it2;
  _histos2D.clear();

  // delete also the histograms created for drawing
  ResetDrawHistos();
}


//********************************************************
void HistoStack::ResetDrawHistos() {
//*******************************************************

  if (_hall1D_draw) delete _hall1D_draw;

  if (_histos1D_draw.size()>0){
    for (std::vector<TH1F*>::iterator it1 = _histos1D_draw.begin(); it1 != _histos1D_draw.end(); it1++)
      delete *it1;
    _histos1D_draw.clear();
  }

}

//********************************************************
TH1F* HistoStack::GetHisto1D(const std::string& title) {
//*******************************************************

  if (_histos1D.size() > 0) {
    std::vector<TH1F*>::reverse_iterator rit1;
    for (rit1 = _histos1D.rbegin(); rit1 != _histos1D.rend(); rit1++) {      
      if (strcmp((*rit1)->GetTitle(), title.c_str())==0)
	return *rit1;
    }
  }

  return NULL;
}

//********************************************************
TH1F* HistoStack::GetHisto1D(Int_t index) {
//*******************************************************

  if (index >= 0 && (UInt_t)index < _histos1D.size()) {
	return _histos1D[index];
  }

  return NULL;
}


//********************************************************
TH2F* HistoStack::GetHisto2D(const std::string& title) {
//*******************************************************

  if (_histos2D.size() > 0) {
    std::vector<TH2F*>::reverse_iterator rit1;
    for (rit1 = _histos2D.rbegin(); rit1 != _histos2D.rend(); rit1++) {
      if (strcmp((*rit1)->GetTitle(), title.c_str())==0)
	return *rit1;
    }
  }
  return NULL;
}

//********************************************************
TH2F* HistoStack::GetHisto2D(Int_t index) {
//*******************************************************

  if (index >=0 && (UInt_t)index < _histos2D.size()) {
	return _histos2D[index];
  }

  return NULL;
}


//********************************************************
void HistoStack::Draw(const std::string& root_opt, const std::string& opt) {
//*******************************************************

  Draw(1,1,1,0,root_opt,opt,"",0);

}

//********************************************************
void HistoStack::Draw(int lc, int lw, int fc, int fs, const std::string& root_opt, const std::string& opt,const std::string& leg_opt,int mode) {
//*******************************************************

  (void)leg_opt;
  (void)mode;

  // Delete all histos used for drawing the stack, just in case this stack was drawn before 
  ResetDrawHistos();

  if (_is1D) {
    bool makeTotal = (!_hall1D);

    std::vector<TH1F*>::reverse_iterator rit1;
    int i = 0;
    for (rit1 = _histos1D.rbegin(); rit1 != _histos1D.rend(); rit1++, i++) {
      TH1F* ht = *rit1;

      if (makeTotal) {
        if (i == 0) {
          _hall1D = new TH1F(*ht);
          _hall1D->SetName((std::string(ht->GetName()) + "_cum").c_str());
          _hall1D->SetTitle(ht->GetTitle());
        } else {
          _hall1D->Add(_hall1D, ht);
        }
      }
      if (i == 0) {
        if (_stack) delete _stack;
        _stack = new THStack((std::string(ht->GetName()) + "_stack").c_str(), _title.c_str());
      }
    }

    // Copy the histogram before normalizing
    _hall1D_draw = new TH1F(*_hall1D);
    Float_t min_width;
    drawUtils::NormalizeVariableBinning(_hall1D_draw,2,opt, min_width);

    for (rit1 = _histos1D.rbegin(); rit1 != _histos1D.rend(); rit1++) {
      TH1F* ht = *rit1;
      // Clone the histograms before normalizing
      TH1F* htp = new TH1F(*ht);
      _histos1D_draw.push_back(htp);
      drawUtils::NormalizeVariableBinning(htp,2,opt+" USEMINWIDTH",min_width);
      // Add the normalized histo to the stack
      _stack->Add(htp);
    }


    // Set the maximum and minimum Y
    //    double maxy = _hall1D_draw->GetBinContent(_hall1D_draw->GetMaximumBin()) * 1.1;
    //    if (_stack && opt.find("ETOT")) {
    //      maxy += _hall1D_draw->GetBinError(_hall1D_draw->GetMaximumBin()) * 1.1;
    //    }    

    //    if (fabs(usermaxY) > 1e-6 && usermaxY > userminY) {
    //      maxy = usermaxY;
    //    }
    //    double miny = userminY;

    // If the stack is pressent (colore drawing) just draw it
    if (_stack){
      if (root_opt.find("same") == std::string::npos) {
        _stack->Draw(("HIST"+root_opt).c_str());
        _stack->GetXaxis()->SetTitle(_titleX.c_str());
        _stack->GetYaxis()->SetTitle(_titleY.c_str());
        _stack->Draw(("HIST"+root_opt).c_str());
      } else {
        _stack->Draw("HIST same");
      }
    }

    // This is just to draw the stats box before doing the variable bin normalization
    _hall1D->GetXaxis()->SetTitle(_titleX.c_str());
    _hall1D->GetYaxis()->SetTitle(_titleY.c_str());
    _hall1D->SetTitle(_title.c_str());


    if (_stack)
      _hall1D->Draw((root_opt+"sames AXIS").c_str());
    else
      _hall1D->Draw((root_opt+"AXIS").c_str());

    
    // Style for the total histo in the case it has to be drawn
    _hall1D_draw->SetLineColor(lc);
    _hall1D_draw->SetMarkerColor(lc);
    _hall1D_draw->SetLineWidth(lw);
    _hall1D_draw->SetFillColor(fc);
    _hall1D_draw->SetFillStyle(fs);

    // Draw error bars for the total stack when requested
    if (_stack && drawUtils::CheckOption(opt, "ETOT")) {
      _hall1D_draw->Draw("same e2");
    }
    // Draw the total when there is no stack
    if (!_stack){
      _hall1D_draw->SetMarkerStyle(20);
      _hall1D_draw->SetMarkerSize(1);
      _hall1D_draw->Draw((root_opt+"same").c_str());

      if (_hall1D_stat){
        _hall1D_stat->SetLineColor(4);
        _hall1D_stat->SetFillColor(4);
        _hall1D_stat->Draw((root_opt+"same").c_str());
      }
    }

  } else if (_is2D) {
    bool makeTotal = (!_hall2D);

    std::vector<TH2F*>::reverse_iterator rit1;
    int i = 0;
    for (rit1 = _histos2D.rbegin(); rit1 != _histos2D.rend(); rit1++, i++) {
      TH2F* ht = *rit1;

      if (makeTotal) {
        if (i == 0) {
          _hall2D = new TH2F(*ht);
          _hall2D->SetName((std::string(ht->GetName()) + "_cum").c_str());
          _hall2D->SetTitle(ht->GetTitle());
        } else {
          _hall2D->Add(_hall2D, ht);
        }
      }
      if (i == 0) {
        if (_stack) delete _stack;
        _stack = new THStack((std::string(ht->GetName()) + "_stack").c_str(), _title.c_str());
      }
      _stack->Add(ht);

    }


    // Style for the total histo in the case it has to be drawn
    _hall2D->SetLineColor(lc);
    _hall2D->SetMarkerColor(lc);
    _hall2D->SetLineWidth(lw);
    _hall2D->SetFillColor(fc);
    _hall2D->SetFillStyle(fs);

    if (_stack){
      if (root_opt.find("same") == std::string::npos) {
        _stack->Draw(("HIST "+root_opt).c_str());
        _stack->GetXaxis()->SetTitle(_titleX.c_str());
        _stack->GetYaxis()->SetTitle(_titleY.c_str());
        _stack->Draw(("HIST "+root_opt).c_str());
      } else {
        _stack->Draw("HIST same");
      }
    }
    else{
      _hall2D->Draw(root_opt.c_str());
    }
  }

}

//********************************************************
void HistoStack::FillLegend(TLegend* leg){
//*******************************************************

  // Fill the legend using the titles

  if (_histos1D.size() > 0) {
    std::vector<TH1F*>::iterator it1;
    for (it1 = _histos1D.begin(); it1 != _histos1D.end(); it1++) {
      TH1F* ht = *it1;
      drawUtils::AddLegendEntry( leg, ht, ht->GetTitle(),"f");
    }
  } else if (_histos2D.size() > 0) {
    std::vector<TH2F*>::iterator it1;
    for (it1 = _histos2D.begin(); it1 != _histos2D.end(); it1++) {
      TH2F* ht = *it1;
      drawUtils::AddLegendEntry( leg, ht, ht->GetTitle(),"f");
    }
  }
}


//********************************************************
void HistoStack::AddTotal(TH1F* h1, TH1F* hsyst) {
//*******************************************************

  _is1D = true;

  // The name of the total histogram is the title of the stack
  //  h1->SetName(_title.c_str());

  if (_currentSystGroup == "default"){ 
    if (_hall1D){
      _hall1D->Add(h1);
      if (hsyst){
        _hall1D_stat->Add(h1);
        _hall1D_syst->Add(h1);
      }
    }
    else{
      _hall1D = new TH1F(*h1);
      if (hsyst){
        _hall1D_syst = new TH1F(*h1);
        _hall1D_stat = new TH1F(*h1);
      }
    }
  }

  //When a realitive systematic histo is provided 
  if (hsyst){
    for (int i=0;i<hsyst->GetNbinsX();i++){
      // Compute the total systematic
      double err_syst = hsyst->GetBinError(i+1)*_hall1D_stat->GetBinContent(i+1);
      _hall1D_syst->SetBinError(i+1,err_syst);
      
      // Add stat and syst errors in quadrature
      double err_total = sqrt(pow(_hall1D_stat->GetBinError(i+1),2)+pow(_hall1D_syst->GetBinError(i+1),2));
      _hall1D->SetBinError(i+1,err_total);
    }
  }
  /*
  else{
    for (int i=0;i<_hall1D_syst->GetNbinsX();i++){
      _hall1D_syst->SetBinError(i+1,0);
    }
  }
  */
}

//********************************************************
void HistoStack::AddTotal(TH2F* h2) {
//*******************************************************

  _is2D = true;

  // The name of the total histogram is the title of the stack
  //  h2->SetName(_title.c_str());

  if (_hall2D)
    _hall2D->Add(h2);
  else
    _hall2D = new TH2F(*h2);
}


//********************************************************
void HistoStack::AddSystHistos(TH2F* h1, TH2F* h2, TH2F* h1w){
//*******************************************************

  std::string group =  _currentSystGroup;

  if (group==""){
    std::cout << "Error in HistoStack::AddSystHistos !!!  Current Systematics Group not specified !!!" << std::endl;
    return;
  }

  if (_h1.find(group)!=_h1.end()){
    _h1[group]->Add(h1);
    _h1w[group]->Add(h1w);
    _h2[group]->Add(h2);
     return;
  }

  AddSystHistosGroup(group);

  _h1[group] = new TH2F(*h1);
  _h1[group]->SetName("h1p");
  
  _h1w[group] = new TH2F(*h1w);
  _h1w[group]->SetName("h1wp");

  _h2[group] = new TH2F(*h2);
  _h2[group]->SetName("h2p");
}

//********************************************************
void HistoStack::GetSystHistos(const std::string& group, TH2F*& h1, TH2F*& h2,TH2F*& h1w){
//*******************************************************
  
  h1 = _h1[group];
  h1w = _h1w[group];
  h2 = _h2[group];
 
}

//********************************************************
void HistoStack::Add(TH1F* h1, int lc, int lw, int fc, int fs,const std::string& leg) {
//*******************************************************

  if (_is2D) return;
  _is1D = true;

  // Check whether a histo with the same title already exists
  TH1F* h1p = GetHisto1D(leg);
  if (h1p){
    // if it exists we need to add the new one to the existing one
    h1p->Add(h1);
    return;
  }

  //---- We get here when this title is new ------

  TH1F* h1pp = new TH1F(*h1);

  // set the atributes for this histo in the stacked plot
  h1pp->SetFillColor(fc);
  h1pp->SetLineColor(lc);
  h1pp->SetLineWidth(lw);
  h1pp->SetFillStyle(fs);

  if (fs == 0)
    h1pp->SetMarkerStyle(21);

  // Set the title
  if (leg!="")
    h1pp->SetTitle(leg.c_str());

  // add the histo to the vector
  _histos1D.push_back(h1pp);
}

//********************************************************
void HistoStack::Add(TH2F* h2, int fc, int lc, const std::string& leg) {
//*******************************************************

  if (_is1D) return;
  _is2D = true;

  // Check whether a histo with the same title already exists
  TH2F* h2p = GetHisto2D(leg);
  if (h2p){
    // if it exists we need to add the new one to the existing one
    h2p->Add(h2);
    return;
  }

  //---- We get here when this title is new ------

  TH2F* h2pp = new TH2F(*h2);

  // set the atributes for this histo in the stacked plot
  h2pp->SetFillColor(fc);
  h2pp->SetLineColor(lc);
  h2pp->SetMarkerColor(lc);

  // set the title
  if (leg!="")
    h2pp->SetTitle(leg.c_str());

  // add the histo to the vector
  _histos2D.push_back(h2pp);
}

//********************************************************
void HistoStack::NormalizeByArea(const std::string& uopt, double area){
//*******************************************************

  if (!drawUtils::CheckOption(uopt, "AREA") || drawUtils::CheckInternalOption(uopt, "NOAREA")) return;

  if (_hall1D){
    double integral = _hall1D->GetSumOfWeights();    
    if(integral > 0.){ 
      if      (drawUtils::CheckOption(uopt, "AREA100")) area = 100;
      else if (drawUtils::CheckOption(uopt, "AREA1")) area = 1.;
      else if (area==1) return;

      _hall1D->Sumw2();
      _hall1D->Scale(area/integral);      

      if (_hall1D_syst){
        _hall1D_syst->Sumw2();
        _hall1D_syst->Scale(area/integral);      
      }

      if (_hall1D_stat){
        _hall1D_stat->Sumw2();
        _hall1D_stat->Scale(area/integral);      
      }

      if (_histos1D.size() > 0) {
        std::vector<TH1F*>::iterator it1;
        for (it1 = _histos1D.begin(); it1 != _histos1D.end(); it1++) {
          TH1F* ht = *it1;
          ht->Scale(area/integral);      
        }
      }
    }
  }

}


//*********************************************************
double HistoStack::GetMaximum(const std::string& opt){
//*********************************************************

  // we must normalizeto the minimum width bin  before getting the maximum
  if (!_hall1D_draw){
    _hall1D_draw = new TH1F(*_hall1D);
    Float_t min_width;
    drawUtils::NormalizeVariableBinning(_hall1D_draw,2,opt, min_width);
  }

  return _hall1D_draw->GetBinContent(_hall1D_draw->GetMaximumBin());
}

//*********************************************************
double HistoStack::GetMaximumWithError(const std::string& opt){
//*********************************************************

  // Note: TH1F::GetBinErrorUp() was used in this function in stead of TH1F::GetBinError(). That
  // function is not available on ROOT 5.30, which is the default for production 5. Thus this
  // function cannot be used here, as we need production 5 compatibility!

  // we must normalizeto the minimum width bin  before getting the maximum
  if (!_hall1D_draw){
    _hall1D_draw = new TH1F(*_hall1D);
    Float_t min_width;
    drawUtils::NormalizeVariableBinning(_hall1D_draw,2,opt, min_width);
  }

  return _hall1D_draw->GetBinContent(_hall1D_draw->GetMaximumBin())+_hall1D_draw->GetBinError(_hall1D_draw->GetMaximumBin());
}


//*********************************************************
void HistoStack::SetMaximum(double max){
//*********************************************************

  _hall1D->SetMaximum(max);
  if (_hall1D_draw) _hall1D_draw->SetMaximum(max);
  if (_stack) _stack->SetMaximum(max);
}


//*********************************************************
void HistoStack::SetMinimum(double min){
//*********************************************************

  _hall1D->SetMinimum(min);
  if (_hall1D_draw) _hall1D_draw->SetMinimum(min);
  if (_stack) _stack->SetMinimum(min);
}

//*********************************************************
void HistoStack::Print() const{
//*********************************************************

  std::cout << "---------- Dumping HistoStack -----------" << std::endl;

  std::vector<TH1F*>::const_iterator rit1;
  for (rit1 = _histos1D.begin(); rit1 != _histos1D.end(); rit1++) {      
    std::cout << " - " << (*rit1)->GetTitle() << " " << (*rit1)->GetEntries() << std::endl;
  }
}
