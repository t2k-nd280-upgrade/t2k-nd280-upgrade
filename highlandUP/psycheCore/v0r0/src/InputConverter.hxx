#ifndef InputConverter_h
#define InputConverter_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "CoreDataClasses.hxx"
#include "Header.hxx"

/// This base class specifies the interface that all input converters should
/// implement.
///
/// An input converter takes an input file and converts the information into
/// the AnaSpillB format used in the framework. For example, the
/// oaAnalysisTreeConverter class converts an oaAnalysis file to the AnaSpillB
/// format.
///
/// The converters themselves are found in the highLevelAnalysisIO package.
class InputConverter {

  public:

    /// Constrctor provided a name
    InputConverter(const std::string& name);

    /// Everyone should have a destructor...
    virtual ~InputConverter() {};

    /// returns the name of the tree to convert
    const std::string& GetTreeName(){return _treeName;}

    /// Return the name of this converter.
    const std::string& Name() const{return _name;}

    /// Set the name of this converter, which is used internally by the InputManager
    void SetName(const std::string& name){_name=name;}

    /// Reset the trees in the TChain and the number of entries. Also the header (POT, etc)
    void Reset(){     
      if (fChain) fChain->Reset();
      _nentries=0;
      header().Reset();
    }
    //--------------------- MANDATORY METHODS --------------------------

    /// Any initialization that must be done. Called before the input file is
    /// read.
    virtual bool Initialize() = 0;

    /// Read the specified entry from the file, and fill the spill.
    /// The entry number must be incremented as part of this function, and the
    /// new entry number returned.
    /// 0 should be returned on error.
    virtual Int_t GetSpill(Long64_t& entry, AnaSpillC*& spill) = 0;

    /// Read the specified entry from the file, and fill the event.
    /// The entry number must be incremented inside this function
    /// 0 should be returned on error.
    virtual Int_t GetEvent(Long64_t& entry, AnaEventC*& event) = 0;

    /// Record the POT for the current spill, based on information in the AnaBeam
    /// member of the current AnaSpillB.
    virtual void IncrementPOTBySpill() = 0;

    /// Add the file specified to fChain, and any friend chains that were set up.
    virtual bool AddFileToTChain(const std::string& inputString) = 0;

    /// Handle loading the correct entry from fChain.
    virtual Long64_t LoadTree(Long64_t entry);

    /// Return the total number of entries in the chain.
    virtual Long64_t GetEntries();

    /// Return the total number of events for a given number of entries in the tree
    virtual Int_t GetNEvents(Int_t entries=-1);

    /// Whether the implementation can handle the specified file.
    virtual bool IsCorrectType(const std::string& inputString);

    /// Specify whether we're running in "cosmic" mode or not. Some converters
    /// don't care about this, but others do and can override this function.
    virtual void SetCosmicMode(const bool cosmic) {(void)cosmic;};

    /// Whether the converter has added a chain with the given name to its map
    /// of chains.
    bool HasChain(const std::string& name) {
      return (_chain_map.find(name) != _chain_map.end());
    }

    /// Add a chain with the given name to the map of chains the converter uses.
    /// When two names are given the first is the one we will use in the map, 
    /// while the second is the actual path in the root file. 
    /// If only one name is given, the path is used as name in the map
    void AddChain(const std::string& name, const std::string& name_path="") {
      if (!HasChain(name)) {
	if (name_path=="") 
	  _chain_map[name] = new TChain(name.c_str());
	else
	  _chain_map[name] = new TChain(name_path.c_str());
      } else {
        std::cout << "TChain " << name << " already exists. Not added !!!" << std::endl;
      }
    }

    /// Get the chain with the given name, if it has been added to the map of
    /// chains the converter uses. If no name is specified, the 'main' chain
    /// fChain is returned. If no chain with the given name is in the map of
    /// chains, then NULL is returned.
    TChain* GetChain(const std::string& name = "") {
      if (name == "") {
        return fChain;
      } else if (HasChain(name)) {
        std::cout<<"has name"<<std::endl;
        return _chain_map[name];
      } else {
        return NULL;
      }
    }

    /// Returns the Header manager
    Header& header(){return _header;}

  protected:

    /// The name of the converter
    std::string _name;

    /// The name of the tree to convert
    std::string _treeName;

    /// The number of entries in the chain. To be set by the implementation.
    int _nentries;

    /// The main TChain used to read events from the input file.
    TChain *fChain; //!

    /// current Tree number in a TChain
    Int_t fCurrent; //!

    /// Map of all the chains the converter uses. Access with AddChain(),
    /// HasChain() and GetChain() functions.
    std::map<std::string, TChain*> _chain_map;  

    ///  The header manager
    Header _header;
};

#endif

