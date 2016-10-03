#ifndef Header_hxx
#define Header_hxx

#include <CoreDataClasses.hxx>
#include <TChain.h> 

/// This class handles POT info, SoftwareVersion and IsMC

//-------------------------------------------------------------------
class Header{
public :


  /// constructor
  Header();

  virtual ~Header(){}

  /// Record the POT for the current spill, based on information in the AnaBeam
  /// member of the current AnaSpill.
  void IncrementPOTBySpill(const AnaSpillC& spill);

  /// Directly increment the good POT. Use this for MC files, which don't have
  /// beam information.
  void IncrementPOTByFile(Double_t pot, bool bySpillPOT=true);

  /// Read the "header" tree from the given file, but reset POT counting first
  void ReadHeader(const std::string& file);
  void ReadHeader(TTree* tree);

  /// Read the "header" tree from the given file and increment the existing POT counting
  bool AddHeader(const std::string& file, bool bySpillPOT=false);
  bool AddHeader(TTree* tree, bool bySpillPOT=false);

  /// This is the method used externaly. It corresponds to POT that passed beam and ND280 quality cuts.
  Double_t GetPOT() { return GetPOTGoodBeamGoodND280(); }

  /// This is the method used externaly. It corresponds to POT that passed beam and ND280 quality cuts.
  Int_t GetNSpills() { return GetSpillGoodBeamGoodND280(); }

  /// Get the total recorded POT, before any quality cuts.
  Double_t GetPOTNoCut() { return _POT_NoCut; }

  /// Get the total recorded POT counted per file
  Double_t GetPOTCountedPerFile() { return _POT_CountedPerFile; }

  /// Get the total POT that was removed due to bad beam quality.
  Double_t GetPOTBadBeam() { return _POT_BadBeam; }

  /// Get the total POT that was removed due to bad ND280 quality.
  Double_t GetPOTBadND280() { return _POT_BadND280; }

  /// Get the total POT that passed beam and ND280 quality cuts.
  Double_t GetPOTGoodBeamGoodND280() { return _POT_GoodBeamGoodND280; }

  /// Set the total POT that passed beam and ND280 quality cuts.
  void SetPOTGoodBeamGoodND280(Double_t good) {_POT_GoodBeamGoodND280 = good;}

  /// Get the total POT that passed beam and ND280 quality cuts, at 0KA horn current.
  Double_t GetPOT0KA() { return _POT_0KA; }

  /// Get the total POT that passed beam and ND280 quality cuts, at 200KA horn current.
  Double_t GetPOT200KA() { return _POT_200KA; }

  /// Get the total POT that passed beam and ND280 quality cuts, at 250KA horn current.
  Double_t GetPOT250KA() { return _POT_250KA; }

  /// Get the total POT that passed beam and ND280 quality cuts, at -250KA horn current.
  Double_t GetPOTMinus250KA() { return _POT_m250KA; }

  /// Get the total POT that passed beam and ND280 quality cuts, at a horn current
  /// that isn't 0KA, 200KA or 250KA.
  Double_t GetPOTOtherKA() { return _POT_OtherKA; }

  /// Get the total recorded spills, before any quality cuts.
  Int_t GetSpillNoCut() { return _Spill_NoCut; }

  /// Get the total spills that was removed due to bad beam quality.
  Int_t GetSpillBadBeam() { return _Spill_BadBeam; }

  /// Get the total spills that was removed due to bad ND280 quality.
  Int_t GetSpillBadND280() { return _Spill_BadND280; }

  /// Get the total spills that passed beam and ND280 quality cuts.
  Int_t GetSpillGoodBeamGoodND280() { return _Spill_GoodBeamGoodND280; }

  /// Print the POT information.
  void DumpPOT();

  /// Reset all the information in the header
  void Reset();

  /// Set the Data/MC mode
  bool SetIsMC(bool isMC);

  /// returns the Data/MC mode
  bool GetIsMC() const{return _IsMC;}

  /// Set the software version
  bool SetSoftwareVersion(const std::string& ver);
  
  /// Returns the software version
  const std::string& GetSoftwareVersion(){return _SoftwareVersion;}

 public:

  /// POT info
  Double_t _POT_CountedPerFile;
  Double_t _POT_NoCut;
  Double_t _POT_BadBeam;
  Double_t _POT_BadND280;
  Double_t _POT_GoodBeamGoodND280;
  Double_t _POT_0KA;
  Double_t _POT_200KA;
  Double_t _POT_250KA;
  Double_t _POT_m250KA;
  Double_t _POT_OtherKA;

  /// Spill info
  Int_t _Spill_NoCut;
  Int_t _Spill_BadBeam;
  Int_t _Spill_BadND280;
  Int_t _Spill_GoodBeamGoodND280;

  /// data or MC mode 
  bool _IsMC;

  /// The software version used to create the original oaAnalysis file of the last input file
  std::string _SoftwareVersion;

  /// iternal variable that tell us whether a file has been added previously
  bool _isFirstFile; //!

};

#endif


