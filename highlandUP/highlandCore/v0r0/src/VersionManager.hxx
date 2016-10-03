#ifndef VERSIONMANAGER_HXX_SEEN
#define VERSIONMANAGER_HXX_SEEN
//#include "Versioning.hxx"
#include "HLClonesArray.hxx"
#include "BasicTypes.hxx"

//-------------------------------------------------------------------

/// Maximum number of packages that can be added
const UInt_t NMAXPACKAGES=20;

typedef Int_t ProdId_h;

const ProdId_h UNKNOWN_PRODUCTION = -1;

class PackageVersion: public TObject{
public:

  PackageVersion();

  // constructor provided name and version
  PackageVersion(const std::string& name, const std::string& version);
  
  /// Everyone should have a destructor.
  virtual ~PackageVersion() {}
  
  /// Return the name of this package. This overrides the TObject::GetName() interface.
  virtual const char* GetName() const {return _name.c_str();}

  /// Return the name of this package.
  const std::string& Name() const {return _name;}
  
  /// Set the name of this package.
  void SetName(const std::string& name) {_name = name;}
  
  /// Return the version of this package.
  const std::string& Version() const {return _version;}
  
  /// Set the version of this package.
  void SetVersion(const std::string& version) {_version = version;}
  
  ClassDef(PackageVersion, 1);
  
protected:
  
  /// The name of this package.
  std::string _name;

  /// The name of this package.
  std::string _version;
};


class Versioning: public HLClonesArray{

public:

  virtual ~Versioning(){}

  /// return the singleton
  static Versioning& Get(void);

  /// Check compatibility between input file software version and file used to compile nd280AnalysisTools
  bool CheckVersionCompatibility(const std::string& versionInput,ProdId_h prodND280);

  /// Add a production
  void AddProduction(ProdId_h prodId, const std::string& prodName, const std::string& prodLowVersion, const std::string& prodHighVersion);

  /// Get the enum of the production corresponding to a given sofware version
  ProdId_h GetProduction(const std::string softVersion);

  /// Get the name of the production corresponding to a given sofware version
  std::string GetProductionName(const std::string softVersion);

  /// Add a package
  void AddPackage(const std::string& name, const std::string& version);

  /// Read package names and versions use to produce this file
  void ReadVersions(const std::string& file);

  /// dump package names and versions
  void DumpVersions();

 private:
  
  /// The singleton instance.
  static Versioning* _versioning;

  /// Private constructor as this is a singleton.
  Versioning();

  /// Unimplemented copy constructor as this is a singleton.
  Versioning(Versioning const&);

  /// Unimplemented assignment operator as this is a singleton.
  void operator=(Versioning const&);

  /// Get the software version range for a given production name
  bool GetSoftwareVersionRangeForProduction(ProdId_h prodId, std::string& lowVersion, std::string& highVersion);

  /// Convert the production enum into an string
  std::string ConvertProduction(ProdId_h prod);

  /// Check wether a software version is prior to another
  bool CheckBeforeVersion(const std::string& version, const std::string& version0);

  /// Parse the version, revision and patch numbers
  void ParseSoftwareVersion(const std::string& version, int& v, int& r, int& p);

  /// vector of sofware version ranges corresponding to each production
  std::vector<std::string> _prodLowVersion;
  std::vector<std::string> _prodHighVersion;

  /// vector of production enums
  std::vector<ProdId_h> _prodId;

  /// vector of production names
  std::vector<std::string> _prodName;

  /// vector of versions of different packages
  std::vector<PackageVersion*> _packages;
};


namespace ND{
  Versioning& versioning();
}

#endif
