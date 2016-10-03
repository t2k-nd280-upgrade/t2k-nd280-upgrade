#ifndef Parameters_hxx_seen
#define Parameters_hxx_seen

#include "TString.h"
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <set>
#include <stdexcept>
#include "UnitsParser.hxx"

/// This exception class indicates an error when reading a parameters file.
class BadParameterFileException: public std::runtime_error {
  public:
    BadParameterFileException() :
      std::runtime_error("Error reading parameters") {
    }
};

/// This exception class indicates that the user asked for a parameter that we
/// know nothing about.
class NoSuchParameterException: public std::runtime_error {
  public:
    NoSuchParameterException() :
      std::runtime_error("No such parameter") {
    }
};

/// This class provides access to a set of parameters that are defined in a
/// set of text files. The parameters can be used at runtime, by calling
/// ND::params().GetParameterI("tutAnalsyis.param.name") or similar.
///
/// Parameters files should be placed in the "parameters" directory in an
/// analysis package, and named as tutAnalysis.parameters.dat, where tutAnalysis
/// is the name of the analysis package. This is where the code will look for a parameter, unless you
/// have already specified the parameter in an "override" file (see below).
///
/// The format for parameters files is:
/// < tutAnalysis.param.name = 12345 >
/// Note that you should have spaces between the <>= symbols and your parameter names / values.
/// The first bit of your parameter name MUST be the same as your package name. After that you can
/// have whatever you like. It is useful to use the "dots" to logically group parameters that are related.
/// For example, you could have \c tutAnalysis.Cuts.PullMu.Max and \c tutAnalysis.Cuts.PullMu.Min.
///
/// Parameters can be read as integers, doubles or strings, using the ND::params().GetParameterI(),
/// ND::params().GetParameterD() and ND::params().GetParameterS() functions, respectively.
///
/// You don't need to do anything special to use the ND::params() singleton - the first time you call
/// one of the ND::params().GetParameter functions it will parse the relevant parameters file.
///
/// You can use a parameters "override" file to override the values set in the normal parameters file.
/// This override file can contain a subset of the parameters (even just 1 or 2), and can be placed
/// anywhere on your file system. Specify the path to this override file using the \c -p command-line
/// option. This is very useful if you're using the FLuxWeighting, for example, so that you can run
/// multiple jobs in parallel, and can specify to apply the run 1 flux to run 1 MC files, run 2 flux to
/// run 2 MC files etc.
///
class Parameters {

  public:

    ~Parameters();

    ///  Get a reference to the singleton instance of dummy
    ///  database information.  If this is first attempt at reference
    ///  then singleton is instantiated and parameters are
    ///  read from text files.
    static Parameters& Get(void) {
      if (!fParameters)
        fParameters = new Parameters();
      return *fParameters;
    }

    /// Check if Parameter is stored in database
    bool HasParameter(std::string);

    /// Get parameter.  Value is returned as integer.
    int GetParameterI(std::string);

    /// Get parameter.  Value is returned as double.
    double GetParameterD(std::string);

    /// Get parameter.  Value is returned as string.
    std::string GetParameterS(std::string);

    /// Clear all the parameters that have been read.
    void ClearMapOfParameters();

    /// Return the class which parses any units specified in the parameters
    /// file, and converts them to a consistent set of HEP units.
    UnitsParser* GetUnitsTableParser() {
      return fUnitsParser;
    }

    /// This command allows the user to set parameters from the
    /// command line; the command is different from the standard
    /// file reading, in that the parameters that are set are 'fixed'.
    /// Ie, they are immutable and cannot be changed, even if they
    /// exist in some other parameters file that is read in later.
    void ReadParamOverrideFile(TString filename);

    /// Load the parameters for a given package - needed for multi-threading
    int LoadParametersFile(TString packageName, UInt_t tryFileMode=0, bool fixParameters = false);
  
    /// Load the parameters for a list of packages
    void LoadParametersFiles(const std::vector<std::string>& packageNames, bool fixed=false);

    /// Set whether the point in which the overwride parameters file is read is passed or not
    void SetReadParamOverrideFilePointPassed(){fReadParamOverrideFilePointPassed=true;}

    /// Checks whether the point in which the overwride parameters file is read is passed or not.
    /// To be used in GetParameters methods. It stops and prints an error message if the 
    /// ReadParamOverrideFile point is not yet passed
    void CheckReadParamOverrideFilePointPassed(std::string parameterName);

  private:

    /// Private constructor as this is a singleton. Use ND::params() to access.
    Parameters();

    /// Unimplemented copy constructor as this is a singleton.
    Parameters(Parameters const&);

    /// Unimplemented assignment operator as this is a singleton.
    void operator=(Parameters const&);

    /// Reads parameters from input files.
    /// Function can be used to read in extra parameter files.
    void ReadInputFile(TString filename, TString dirName = "", UInt_t tryFileMode = 0, bool fixParameters = false);

    /// Prints list of saved parameters
    void PrintListOfParameters();

    /// Helper method to attempt to read parameters file based on parameters name.
    /// The return value indicates whether we can now find this parameter after
    /// trying to open this file (return 1 means parameter found).
    int TryLoadingParametersFile(std::string parameterName);

    /// Map containing list of parameters and their values.
    std::map<std::string, std::string, std::less<std::string> > mapOfParameters;

    /// Define an iterator for mapOfParameters, to make the code more readable.
    typedef std::map<std::string, std::string, std::less<std::string> >::iterator mapIterator;

    /// A set of all the 'fixed'.  'Fixed' parameters cannot be changed, even if they reappear
    /// in a different parameters file.  This is used for the parameter override files.
    std::set<std::string> fixedParameters;

    /// Units can be specified in the parameters files. The UnitsParser will
    /// convert them to a consistent set of HEP units.
    UnitsParser *fUnitsParser;

    /// Indicates whether the point in which the overwride parameters file is read is passed or not
    bool fReadParamOverrideFilePointPassed;

    /// The static pointer to the singleton instance.
    static Parameters* fParameters;
};

namespace ND {
  /// Access to parameters. See Parameters class documentation for more.
  Parameters& params();
}

#endif
