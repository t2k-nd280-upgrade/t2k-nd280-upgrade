#ifndef BeamBunching_h
#define BeamBunching_h

#include "BaseDataClasses.hxx"

class BeamBunching {

public:
  
  BeamBunching(){_nbunches=0;}
  BeamBunching(const std::string& filename){ReadBeamBunchStructure(filename);}
  
  virtual ~BeamBunching(){}

  /// Read the bunch structure for analysis usage. 
  void ReadBeamBunchStructure(const std::string& filename);

  /// Define the Bunch Period from the run number. 
  Int_t GetBunchPeriod(const AnaEventB& event); 

  /// Get the central time for bunch ibunch
  Float_t GetBunchCentralTime(const AnaEventB& event, Int_t ibunch); 

  /// Number of bunches in the run period for the current run
  UInt_t GetNBunches(const AnaEventB& event); 

  /// The run number for a given time
  Int_t GetBunch(Float_t tTrack, Int_t run, bool isMC, bool cosmic_mode, Float_t sigma = 15., Float_t NSigmas = 4.0 ) const;

public:

  UInt_t _nbunches;

  std::vector<Float_t>               bunch_time_mc;    // Bunch times in MC 
  std::vector<std::vector<Float_t> > bunch_time_data;  // Bunch times in Data
  std::vector<std::pair<int,int> >   bunchrunperiod;   // Last  run in  Data period.  

};

#endif
