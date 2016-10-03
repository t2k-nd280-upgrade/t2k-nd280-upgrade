#ifndef ConfigurationBase_h
#define ConfigurationBase_h

#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>

#include <TObject.h>
#include <BasicTypes.hxx>
#include "SystematicManager.hxx"
#include "ToyMaker.hxx"

/// Maximum number of systematic parameters to be saved in the config tree
const UInt_t NMAXPARAMETERS = 500;

/// A Class that allows saving the variations in the config tree
class ToyVariationWrite{
public:
  ToyVariationWrite(){npar=0;}
  virtual ~ToyVariationWrite(){}

public:
  Float_t  variations[NMAXPARAMETERS];
  Float_t  weights[NMAXPARAMETERS];
  Int_t    par_index[NMAXPARAMETERS];
  UInt_t   npar;

  ClassDef(ToyVariationWrite, 1);
};

class ConfigurationBase: public TObject{
public :

  ConfigurationBase();  
  ConfigurationBase(Int_t index, const std::string& conf_name, UInt_t ntoys=1, Int_t randomSeed=-1, ToyMaker* toyMaker=NULL);
  virtual ~ConfigurationBase(){}

  /// Enable the systematic registered with the given name
  void EnableSystematic(Int_t index){_systematicsEnabled.push_back(index);}

  /// Enable the systematic registered with the given name
  void EnableSystematics(const std::vector<Int_t>& indices);

  /// Disable the systematic registered with the given name
  void DisableSystematic(Int_t index){(void)index;}

  /// Disable the systematic registered with the given name
  void DisableAllSystematics(){_systematicsEnabled.clear();}

  /// Get the systematics enabled for this configuration
  const std::vector<Int_t>& GetEnabledSystematics(){return _systematicsEnabled;}


  /// Enable the systematic registered with the given name
  void EnableEventWeight(Int_t index){_weightsEnabled.push_back(index);}

  /// Enable the systematic registered with the given name
  void EnableEventWeights(const std::vector<Int_t>& indices){_weightsEnabled = indices;}

  /// Disable the systematic registered with the given name
  void DisableEventWeight(Int_t index){(void)index;}

  /// Disable the systematic registered with the given name
  void DisableAllEventWeights(){_weightsEnabled.clear();}

  /// Get the weights enabled for this configuration
  const std::vector<Int_t>& GetEnabledEventWeights(){return _weightsEnabled;}


  /// Enable the systematic registered with the given name
  void EnableEventVariation(Int_t index){_variationsEnabled.push_back(index);}

  /// Enable the systematic registered with the given name
  void EnableEventVariations(const std::vector<Int_t>& indices){_variationsEnabled = indices;}

  /// Disable the systematic registered with the given name
  void DisableEventVariation(Int_t index){(void)index;}

  /// Disable the systematic registered with the given name
  void DisableAllEventVariations(){_variationsEnabled.clear();}

  /// Get the variations enabled for this configuration
  const std::vector<Int_t>& GetEnabledEventVariations(){return _variationsEnabled;}


  /// Is this configuration enabled ?
  void SetEnabled(bool ena){_enabled=ena;}
  bool IsEnabled() const {return _enabled;}
  
  /// Set and gets enable flags for a given toy
  void SetToyEnabled(Int_t n, bool ena){_toyEnabled[n]=ena;}
  bool GetToyEnabled(Int_t n) const {return _toyEnabled[n];}

  /// Get and sets the number of toys
  Int_t GetNToys() const {return _ntoys;}
  void SetNToys(Int_t ntoys){_ntoys= ntoys;}

  /// Returns the refernce toy experiment index
  Int_t GetRefToyIndex() const {return _toy_ref;}

  /// Get and sets the random seed used to generate the toys
  Int_t GetToyRandomSeed() const {return _toy_randomSeed;}
  void SetToyRandomSeed(UInt_t seed){_toy_randomSeed = (Int_t)seed;}

  /// Sets the ToyMaker
  void SetToyMaker(ToyMaker* toyMaker){_toyMaker = toyMaker;}

  /// Returns the ToyMaker
  ToyMaker& GetToyMaker(){return *_toyMaker;}

  /// Returns the configuration index (should match the one in the Configuration Manager)
  Int_t GetIndex() const {return _conf_index;}

  /// Sets the configuration index
  void SetIndex(Int_t index){_conf_index= index;}

  /// Returns the index of the tree associated to this configuration in the TreeManager
  Int_t GetTreeIndex() const {return _tree_index;}

  /// Sets the tree index
  void SetTreeIndex(Int_t index){_tree_index= index;}

  /// Returns the name of this configuration
  const std::string& Name() const {return _name;}

  /// Dump on the screen info about this configuration
  using TObject::Dump;
  void Dump(SystematicManager& syst);

  /// Create the ToyExperiments using the ToyMaker and the SystematicManager
  void CreateToyExperiments(const SystematicManager& syst);

  ClassDef(ConfigurationBase, 1);

protected:

  /// Name of the configuration
  std::string _name;

  /// Configuration index
  Int_t _conf_index;

  /// the index of the tree associated to this configuration in the TreeManager
  Int_t _tree_index; 
  
  /// Number of Toy experiments or virtual analyses
  Int_t _ntoys;

  /// The reference toy with the nominal selection
  Int_t _toy_ref;

  /// The random seed used to generate the toys
  Int_t _toy_randomSeed;

  /// Is this configuration enabled ?
  bool _enabled;

  /// Tell wether a toy is enabled or not
  std::vector<bool> _toyEnabled; //!

  /// Vector of indices of the systematics enabled in this configuration
  std::vector<Int_t> _systematicsEnabled; 

  /// Vector of indices of the weights enabled in this configuration
  std::vector<Int_t> _weightsEnabled; //!

  /// Vector of indices of the variations enabled in this configuration
  std::vector<Int_t> _variationsEnabled; //!


  /// The ToyMaker, which fills the toy
  ToyMaker* _toyMaker; //!

public:

  /// The variations for each of the toys
  std::vector< ToyVariationWrite > _toys;

};

#endif


