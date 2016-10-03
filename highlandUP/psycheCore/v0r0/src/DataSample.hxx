#ifndef DataSample_h
#define DataSample_h

#include <map>
#include <vector>
#include <stdexcept>

#include <TreeManager.hxx>
#include "Header.hxx"

/// This class handles a data sample with POT and spill info
/// A DataSample inherits from a Tree manager and therefore contains several trees
/// - The "header" tree with POT and spill info
/// - The "config" tree with info about the selection and the systematics
/// - The "default" analysis tree with the analysis variables and the accum_level
/// - Other analysis trees with systematics
/// It also inherits from Header, which holds POT info

class DataSample: public TreeManager, public Header{
 public :

  DataSample();
  DataSample(const std::string& file, double norm=0, const std::string& cut="");
  virtual ~DataSample(){}
  
  /// Set and returns the cut to be applied to this sample
  void SetCut(const std::string& cut){_cut = cut;}
  std::string GetCut() const{return _cut;}

  /// Set and Get the special normalisation to be used for this sample
  void SetNorm(Float_t norm){_norm = norm;}
  Float_t GetNorm() const{return _norm;}

  /// Systematic group for this sample
  void SetSystGroup(const std::string& systGroup){_systGroup = systGroup;}
  const std::string& GetSystGroup() const {return _systGroup;}

  /// Systematic option for this sample
  void SetSystOption(const std::string& systOption){_systOption = systOption;}
  const std::string& GetSystOption() const {return _systOption;}

  /// returns the filePath
  const std::string& GetFilePath(){return _filePath;}

  /// Set and Get the special normalisation to be used for this sample
  void SetNEntries(Int_t en){_nEntries = en;}
  Int_t GetNEntries() const{return _nEntries;}

 protected:

  /// The file path
  std::string _filePath;

  /// The number of entries in this DataSample
  Int_t _nEntries;

  /// An automatic cut to be applied to this sample (for example to select runs)
  std::string _cut;
  
  /// Special normalization for this sample (instead of POT in header)
  Float_t _norm;

  /// Systematic group for this sample
  std::string _systGroup;

  /// Systematic option for this sample
  std::string _systOption;
};

#endif


