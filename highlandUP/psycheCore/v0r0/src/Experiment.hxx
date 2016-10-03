#ifndef Experiment_h
#define Experiment_h

#include "DataSample.hxx"

/// Experiment class
/// This class handles multiple data samples in groups (SampleGroup). For example, one generally
/// wants to compare a sample of run 1 data to run 1 MC, and a sample of run 2
/// data to a sample of run 2 MC. This class allows you to add groups of data/MC
/// samples, then interact with them.


/// SampleGroup class
/// Contains a single data sample and a map of MC samples with their names as key. For example we can associate 
/// spill MC and sand muon MC to a given data run

class SampleGroup{
public:

  SampleGroup(){}
  SampleGroup(const std::string& name);
  SampleGroup(const std::string& name, DataSample* dataSample, std::map<std::string, DataSample*>& mcSamples);
  SampleGroup(const std::string& name, const std::string& dataFile, std::map<std::string, std::string>& mcFiles);
 
  /// Add a new Data Sample to a group (for the moment only one data sample can be added)
  void AddDataSample(const std::string& file);
  void AddDataSample(DataSample* sample);

  /// Add a new MC Sample to a group specifying its name (spill, sand, etc)
  void AddMCSample(const std::string& name, const std::string& file);
  void AddMCSample(const std::string& name,   DataSample* sample);

  /// Add a set of  MC Samples to a group specifying their names (spill, sand, etc)
  void AddMCSamples(std::map<std::string, std::string>& mcFiles);
  void AddMCSamples(std::map<std::string, DataSample*>& mcSamples);

  /// Get the data sample in a group
  DataSample* GetDataSample(){
    if (_dataSamples.size()>0) return _dataSamples[0];
    else return NULL;
  }

  /// Get a single MC sample in a group by name
  DataSample* GetMCSample(const std::string& name){return _mcSamples[name];}

  /// Get all MC samples in a group
  std::vector<DataSample*>& GetDataSamples(){return _dataSamples;}

  /// Get all MC samples in a group
  std::map<std::string, DataSample*>& GetMCSamples(){return _mcSamples;}

  /// Check whether the group has a given MC sample
  bool HasMCSample(const std::string& name);

  /// Get the good data and MC POT for this sample group.
  void GetPOT(Float_t& POTdata, Float_t& POTmc);

  /// Get the good data and MC POT for this sample group, including sand POT.
  void GetPOT(Float_t& POTdata, Float_t& POTmc, Float_t& POTsand);

  /// Set the current configuration to all samples in the SampleGroup
  void SetCurrentTree(const std::string& name);

protected:

  std::string _name;
  std::vector<DataSample*> _dataSamples;
  std::map<std::string, DataSample*> _mcSamples;
};



class Experiment{
 public :

  /// Default constructor - just set the name of the experiment. You can add sample
  /// groups using AddSampleGroup functions.
  Experiment(const std::string& name);

  /// Automatically add sampleGroups to the experiment, based on the contents of a
  /// configuration file.
  ///
  /// Each line in this file should relate to one SampleGroup. The first entry on each line is the group
  /// name.
  /// The samples are then specified in "sampleName:fileName" pairs. The first pair on each line
  /// corresponds to the data, the others to MC.
  ///
  /// NOTE THAT YOU CANNOT HAVE SPACES IN EITHER THE GROUP NAME OR SAMPLE NAMES.
  ///
  /// Example:
  /// Run1 Data:run1_data.root MC:run1_mc.root Sand:run1_sand.root
  /// Run2 Data:run2_data.root MC:run2_mc.root
  Experiment(const std::string& name, const std::string& configfile);

  /// Create an experiment provided a data and a MC files
  Experiment(const std::string& name, const std::string& file1, const std::string& file2);

  virtual ~Experiment(){}

  /// Checks if the Experiment has a SampleGroup with a given name 
  bool HasSampleGroup(const std::string& name);

  /// Add a group of samples (normally data and MC). A DataSample object 
  /// will be generated internally for each.
  void AddSampleGroup(const std::string& name, SampleGroup& sampleGroup);

  /// Add a group of samples (normally data and MC). A DataSample object 
  /// will be generated internally for each.
  void AddSampleGroup(const std::string& name, const std::string& dataFile, std::map<std::string, std::string>& mcFiles);

  /// Add group of samples (normally data and MC).
  void AddSampleGroup(const std::string& name, DataSample* dataFile, std::map<std::string, DataSample*>& mcSamples);

  /// Returns all sample groups
  std::map< std::string, SampleGroup>& GetSampleGroups(){return _sampleGroups;}

  /// Get a single sample group by name
  SampleGroup& GetSampleGroup(const std::string& name){HasSampleGroup(name); return _sampleGroups[name];}

  /// Returns the data sample in group
  DataSample* GetDataSample(const std::string& name){if (!HasSampleGroup(name)) return NULL; return GetSampleGroup(name).GetDataSample();}

  /// Returns the by name a MC sample in agroup
  DataSample* GetMCSample(const std::string& name, const std::string& name2){if (!HasSampleGroup(name)) return NULL; return GetSampleGroup(name).GetMCSample(name2);}

  /// Returns all MC samples in a group
  std::map<std::string, DataSample*>& GetMCSamples(const std::string& name){HasSampleGroup(name); return GetSampleGroup(name).GetMCSamples();}

  /// Dump detailed POT information for a given sample pair.
  void DumpPOT(const std::string& name);

  /// Dump total good POT for all sample pairs, and the POT ratio for each pair.
  void DumpPOTRatios();

  /// Get the overall POT ratio.
  Float_t GetOverallPOTRatio();

  /// If the Experiment was created by passing file paths in (rather than DataSample objects)
  /// then return the path to one of those files. This is mainly used in automated scripts where the
  /// Experiment(name, configfile) constructor was used and you later need an explicit file path to
  /// construct some other object.
  std::string GetFilePath() { return _filePath; }

  /// Set the current configuration to all samples in all SampleGroups
  void SetCurrentTree(const std::string& name);

  /// Get the current configuration
  const std::string& GetCurrentTree() const {return _tree;}

  /// Dump info about the experiment
  void Dump();

  /// Return pointer to vector of mc files
  std::vector<std::string> GetMCFileVector(){return _mcFiles;}

  /// Return pointer to vector of data files
  std::vector<std::string> GetDataFileVector(){return _dataFiles;}

  /// Method to add file paths to the MC and Data vectors that store them 
  void AddSampleFilesToVector(SampleGroup& sampleGroup);

 protected:

  /// What the user called this Experiment.
  std::string _name;

  /// Configurations that have been added. Structure is
  ///  name ==> [ sample1, sample2 ]
  std::map< std::string, SampleGroup > _sampleGroups;

  /// we keep a record of one of the input files the user specified, so that they
  /// can access it later (for example, if they want to instantiate a DrawingTools
  /// object using an Experiment rather than specifying a file path directly).
  std::string _filePath;

  /// The current tree from the tree manager
  std::string _tree;

  /// Vector holding all MC file paths associated with this experiment
  std::vector<std::string> _mcFiles;
  /// Vector holding all data file paths associated with this experiment
  std::vector<std::string> _dataFiles;



};

#endif


