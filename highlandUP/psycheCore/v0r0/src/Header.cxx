#include "Header.hxx"
#include "CoreUtils.hxx"
#include <stdio.h>
#include <iostream>

//********************************************************************
Header::Header(){
//********************************************************************
  Reset();
}

//********************************************************************
void Header::Reset() {
//********************************************************************

  _POT_CountedPerFile = 0;
  _POT_NoCut = 0;
  _POT_BadBeam = 0;
  _POT_BadND280 = 0;
  _POT_GoodBeamGoodND280 = 0;
  _POT_0KA = 0;
  _POT_200KA = 0;
  _POT_250KA = 0;
  _POT_m250KA = 0;
  _POT_OtherKA = 0;
  _Spill_NoCut = 0;
  _Spill_BadBeam = 0;
  _Spill_BadND280 = 0;
  _Spill_GoodBeamGoodND280 = 0;

  _SoftwareVersion="";
  _IsMC = false;
  _isFirstFile=true;
}

//********************************************************************
bool Header::AddHeader(TTree* tree, bool bySpillPOT){
//********************************************************************

  if (!tree) return false;

  double tmp_POT_NoCut, tmp_POT_BadBeam, tmp_POT_BadND280;
  int tmp_Spill_NoCut, tmp_Spill_BadBeam, tmp_Spill_BadND280;
  double tmp_POT_GoodBeamGoodND280;
  int tmp_Spill_GoodBeamGoodND280;
  double tmp_POT_0KA, tmp_POT_200KA, tmp_POT_250KA, tmp_POT_m250KA, tmp_POT_OtherKA;
  char tmp_SoftwareVersion[100];  


  if (tree->FindLeaf("POT_NoCut")){   
    anaUtils::ConfigureTreeBranch(tree, "SoftwareVersion",         tmp_SoftwareVersion);
    anaUtils::ConfigureTreeBranch(tree, "POT_NoCut",               &tmp_POT_NoCut);
    anaUtils::ConfigureTreeBranch(tree, "POT_BadBeam",             &tmp_POT_BadBeam);
    anaUtils::ConfigureTreeBranch(tree, "POT_BadND280",            &tmp_POT_BadND280);
    anaUtils::ConfigureTreeBranch(tree, "POT_GoodBeamGoodND280",   &tmp_POT_GoodBeamGoodND280);
    anaUtils::ConfigureTreeBranch(tree, "POT_0KA",                 &tmp_POT_0KA);
    anaUtils::ConfigureTreeBranch(tree, "POT_200KA",               &tmp_POT_200KA);
    anaUtils::ConfigureTreeBranch(tree, "POT_250KA",               &tmp_POT_250KA);
    anaUtils::ConfigureTreeBranch(tree, "POT_m250KA",              &tmp_POT_m250KA);
    anaUtils::ConfigureTreeBranch(tree, "POT_OtherKA",             &tmp_POT_OtherKA);
    anaUtils::ConfigureTreeBranch(tree, "Spill_NoCut",             &tmp_Spill_NoCut);
    anaUtils::ConfigureTreeBranch(tree, "Spill_BadBeam",           &tmp_Spill_BadBeam);
    anaUtils::ConfigureTreeBranch(tree, "Spill_BadND280",          &tmp_Spill_BadND280);
    anaUtils::ConfigureTreeBranch(tree, "Spill_GoodBeamGoodND280", &tmp_Spill_GoodBeamGoodND280);  
    
    // There can be multiple entries in the "header" tree if the analyser has
    // used hadd to merge multiple output trees. We sum the total POT in all
    // entries.
    for (int i = 0; i < tree->GetEntries(); i++) {
      tree->GetEntry(i);
      if (!SetSoftwareVersion(tmp_SoftwareVersion)) return false;
      _POT_CountedPerFile += tmp_POT_NoCut;
      _POT_NoCut += tmp_POT_NoCut;
      _POT_BadBeam += tmp_POT_BadBeam;
      _POT_BadND280 += tmp_POT_BadND280;
      _POT_GoodBeamGoodND280 += tmp_POT_GoodBeamGoodND280;
      _POT_0KA += tmp_POT_0KA;
      _POT_200KA += tmp_POT_200KA;
      _POT_250KA += tmp_POT_250KA;
      _POT_m250KA += tmp_POT_m250KA;
      _POT_OtherKA += tmp_POT_OtherKA;
      _Spill_NoCut += tmp_Spill_NoCut;
      _Spill_BadBeam += tmp_Spill_BadBeam;
      _Spill_BadND280 += tmp_Spill_BadND280;
      _Spill_GoodBeamGoodND280 += tmp_Spill_GoodBeamGoodND280;      
    }
  }
  else{
    Header *header=0;
    anaUtils::ConfigureTreeBranch(tree, "POTInfo",                 &header);
    
    // There can be multiple entries in the "header" tree if the analyser has
    // used hadd to merge multiple output trees. We sum the total POT in all
    // entries.
    for (int i = 0; i < tree->GetEntries(); i++) {
      tree->GetEntry(i);      
      if (!SetIsMC(header->GetIsMC())) return false;
      if (!SetSoftwareVersion(header->GetSoftwareVersion())) return false;

      _POT_CountedPerFile      += header->GetPOTCountedPerFile();
      if (!bySpillPOT){
        _POT_NoCut               += header->GetPOTNoCut();
        _POT_BadBeam             += header->GetPOTBadBeam();
        _POT_BadND280            += header->GetPOTBadND280();
        _POT_GoodBeamGoodND280   += header->GetPOTGoodBeamGoodND280();
        _POT_0KA                 += header->GetPOT0KA();
        _POT_200KA               += header->GetPOT200KA();
        _POT_250KA               += header->GetPOT250KA();
        _POT_m250KA              += header->GetPOTMinus250KA();
        _POT_OtherKA             += header->GetPOTOtherKA();
        _Spill_NoCut             += header->GetSpillNoCut();
        _Spill_BadBeam           += header->GetSpillBadBeam();
        _Spill_BadND280          += header->GetSpillBadND280();
        _Spill_GoodBeamGoodND280 += header->GetSpillGoodBeamGoodND280();
      }
    }   
  }

  return true;
}

//********************************************************************
void Header::ReadHeader(TTree* tree){
//********************************************************************
  
  if (!tree) return;
  Reset();
  AddHeader(tree);
}

//********************************************************************
bool Header::AddHeader(const std::string& file, bool bySpillPOT){
//********************************************************************

  TChain chain("header");
  chain.AddFile(file.c_str());
  return AddHeader(&chain, bySpillPOT);
}

//********************************************************************
void Header::ReadHeader(const std::string& file){
//********************************************************************

  TChain chain("header");
  chain.AddFile(file.c_str());
  ReadHeader(&chain);
}

//********************************************************************
void Header::IncrementPOTByFile(Double_t pot, bool bySpillPOT) {
//********************************************************************

  _POT_CountedPerFile += pot;

  // When no POT information by spill exists (prod5 MC) POT counting is done by file 
  if (!bySpillPOT){
    _POT_NoCut += pot;
    _POT_GoodBeamGoodND280 += pot;
  }
}

//********************************************************************
void Header::IncrementPOTBySpill(const AnaSpillC& spill) {
//********************************************************************

  (void)spill;
/*
  const AnaBeamB& beam = *spill.Beam;
  const AnaEventInfoB& info = *spill.EventInfo;

  if (beam.POTSincePreviousSavedSpill > 1e+16) {
    std::cout << "WARNING: POT is suspiciously large for run " << info.Run << ", subrun " << info.SubRun << ", event " << info.Event << ": " << beam.POTSincePreviousSavedSpill << ". POT will not be counted for this event." << std::endl;
    return;
  }
  
  if (beam.POTSincePreviousSavedSpill < 0) {
    std::cout << "WARNING: POT is negative for run " << info.Run << ", subrun " << info.SubRun << ", event " << info.Event << ": " << beam.POTSincePreviousSavedSpill << ". POT will not be counted for this event." << std::endl;
    return;
  }

  // For real data
  if (!spill.GetIsMC()) {

    _POT_NoCut   += beam.POTSincePreviousSavedSpill;
    _Spill_NoCut += beam.SpillsSincePreviousSavedSpill;

    if (beam.GoodSpill == 0) {
      _POT_BadBeam   += beam.POTSincePreviousSavedSpill;
      _Spill_BadBeam += beam.SpillsSincePreviousSavedSpill;
      return;
    }

    if (!spill.DataQuality->GoodDaq) {
      _POT_BadND280   += beam.POTSincePreviousSavedSpill;
      _Spill_BadND280 += beam.SpillsSincePreviousSavedSpill;
      return;
    }

    if (beam.GoodSpill == 100) {
      _POT_0KA += beam.POTSincePreviousSavedSpill;
    } else if (beam.GoodSpill == 1) {
      _POT_250KA += beam.POTSincePreviousSavedSpill;
    } else if (beam.GoodSpill == 2) {
      _POT_200KA += beam.POTSincePreviousSavedSpill;
    } else if (beam.GoodSpill == -1) {
      _POT_m250KA += beam.POTSincePreviousSavedSpill;
    } else {
      _POT_OtherKA += beam.POTSincePreviousSavedSpill;
    }

    _POT_GoodBeamGoodND280   += beam.POTSincePreviousSavedSpill;
    _Spill_GoodBeamGoodND280 += beam.SpillsSincePreviousSavedSpill;
  }
  else{
    // For MC there is no information about magnet current, Spill and DQ are always good
    _Spill_NoCut             += beam.SpillsSincePreviousSavedSpill;
    _Spill_GoodBeamGoodND280 += beam.SpillsSincePreviousSavedSpill;

    _POT_NoCut             += beam.POTSincePreviousSavedSpill;
    _POT_GoodBeamGoodND280 += beam.POTSincePreviousSavedSpill;
  }
*/
}

//********************************************************************
void Header::DumpPOT() {
//********************************************************************
  std::cout << "Initial POT (per File).. " << _POT_CountedPerFile << std::endl;
  std::cout << "Initial POT............. " << _POT_NoCut << " Spills " << _Spill_NoCut  << std::endl;
  std::cout << "|-- Bad Beam............ " << _POT_BadBeam << " Spills " << _Spill_BadBeam  << std::endl;
  std::cout << "|-- Bad ND280........... " << _POT_BadND280 << " Spills " << _Spill_BadND280  << std::endl;
  std::cout << "|-- Total Good POT...... " << _POT_GoodBeamGoodND280 << " Spills " << _Spill_GoodBeamGoodND280  << std::endl;
  std::cout << "  |-- @  0KA............ " << _POT_0KA << std::endl;
  std::cout << "  |-- @  200KA.......... " << _POT_200KA << std::endl;
  std::cout << "  |-- @  250KA.......... " << _POT_250KA << std::endl;
  std::cout << "  |-- @ -250KA.......... " << _POT_m250KA << std::endl;
  std::cout << "  |-- @  Other power.... " << _POT_OtherKA << std::endl;
}


//********************************************************************
bool Header::SetIsMC(bool IsMC){
//********************************************************************

  if (!_isFirstFile && _IsMC != IsMC){
    std::cout << "Header::SetIsMC(). New file with IsMC=" << IsMC << " differs from previous with IsMC=" << _IsMC << ". You cannot mix DATA and MC files !!!" << std::endl;
    return false;
  }

  _isFirstFile = false;
  _IsMC = IsMC;

  return true;
}

//********************************************************************
bool Header::SetSoftwareVersion(const std::string& ver){
//********************************************************************

  if (_SoftwareVersion!="" && ver != _SoftwareVersion){
    std::cout << "Header::SetSoftwareVersion(). New version " << ver << " differs from previous " << _SoftwareVersion << std::endl;
    return false;
  }
  _SoftwareVersion = ver;
  return true;
}
