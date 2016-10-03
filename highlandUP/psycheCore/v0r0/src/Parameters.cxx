#include "Parameters.hxx"
#include <cstdlib>

using namespace std;

Parameters* Parameters::fParameters = NULL;

Parameters& ND::params() {
  return Parameters::Get();
}

Parameters::Parameters() {
  // Build the set of units.
  fUnitsParser = new UnitsParser();

  // We do not open any parameter files by default in the constructor.  
  // Instead, parameter files are open automatically when an entry
  // in the file is requested by the user.

  // The point in which the overwride parameters file is read is not yet passed
  fReadParamOverrideFilePointPassed=false;
}

Parameters::~Parameters() {
}

/// Read in each parameter file.
void Parameters::ReadInputFile(TString fileName, TString dirName, UInt_t tryFileMode, bool fixParameters) {
  TString fullFileName;

  // Make full file name.
  if (!dirName.CompareTo(""))
    fullFileName = fileName.Data();
  else
    fullFileName = dirName.Data() + TString("/") + fileName.Data();

  ifstream inputFile(fullFileName.Data(), ios::in);

  // If input file doesn't exist.
  if (!inputFile) {
    // Just return if 'tryFile' is specified.
    if (tryFileMode==0) {
      std::cout << "\n***** Parameters::ReadInputFile *****\n" << "Cannot open input file '" << fullFileName.Data() << "'." << std::endl;
      return;
    }
    if (tryFileMode==1) {
      std::cout << " --> " << "This package has no parameters file !!!" << std::endl;
    } else { // Otherwise, throw exception.
      std::cerr << "\n***** Parameters::ReadInputFile *****\n" << "Cannot open input file '" << fullFileName.Data() << "'." << std::endl;
      throw BadParameterFileException();
    }
  }
  else{
    if (tryFileMode==1) {
      std::cout << std::endl;
    }
  }

  int inputState = 0;
  string inputString;
  string parameterName;
  string parameterValueUnit;
  string parameterValue;

  while (inputFile >> inputString) {
    if (inputState == 0) {
      if (inputString == "<")
        inputState = 1;
    } else if (inputState == 1) {
      parameterName = inputString;
      inputState = 2;
    } else if (inputState == 2) {
      if (inputString == "=")
        inputState = 3;
      else {
        std::cerr << "\n***** Parameters::ReadInputFile *****\n"
          "Input file '" << fileName << "'. Last parameter '" << parameterName << "'.\n" << "Cannot find symbol '='.\n"
          << "Badly formatted parameters file." << std::endl;
        throw BadParameterFileException();
      }
    } else if (inputState == 3) {
      //        parameterValue = atof(inputString.c_str());
      parameterValue = inputString.c_str();
      parameterValueUnit = inputString;
      inputState = 4;
    } else if (inputState == 4) {
      if (inputString == ">") {
        // Finished reading. Save parameter; but only if the parameter
        // isn't already 'fixed'
        if (fixedParameters.find(parameterName) == fixedParameters.end()) {
          mapOfParameters[parameterName] = parameterValue;
          // If fixParameters bool is set, fix this parameter now.
          if (fixParameters)
            fixedParameters.insert(parameterName);
        }
        inputState = 0;
      } else if (inputString == "<") {
        std::cerr << "\n***** Parameters::ReadInputFile *****\n" << "Input file '" << fileName << "'. Last parameter '" << parameterName
          << "'.\n" << "Unexpected symbol '<'.\n" << "Badly formatted parameters file." << std::endl;
        throw BadParameterFileException();
      } else {
        // The parameter must have a unit.  Resave the value with the correct unit.
        parameterValueUnit.append(" ");
        parameterValueUnit.append(inputString);

        // Use ND::TUnitsTableParser class to convert string of value+unit to
        // a double with value+unit.
        parameterValue = fUnitsParser->Convert2DoubleWithUnit(parameterValueUnit);
      }
    }
  }

  if (inputState != 0) {
    std::cerr << "\n***** Parameters::ReadInputFile *****\n" << "Input file '" << fileName << "'. Last parameter '" << parameterName
      << "'.\n" << "Cannot find symbol '>' at the end of file.\n" << "Badly formatted parameters file." << std::endl;
    throw BadParameterFileException();
  }

  inputFile.close();
}

void Parameters::PrintListOfParameters() {

  std::cout << "***** Parameters::PrintListOfParameters *****" << std::endl;
  std::cout << "List of parameters:" << std::endl;

  for (mapIterator i = mapOfParameters.begin(); i != mapOfParameters.end(); i++)
    std::cout << "  " << (*i).first << " = " << (*i).second << std::endl;

  std::cout << std::endl;
}

void Parameters::LoadParametersFiles(const std::vector<std::string>& packageNames, bool fixed) {

  // The one given as argument reffers to the message printed when the parameters file is not found

  std::cout << "Load parameters file for a list of packages package: fixed = " << fixed << std::endl;
  for (std::vector<std::string>::const_iterator it=packageNames.begin();it!=packageNames.end();it++){
    LoadParametersFile(*it, 1, fixed);
  }
}

int Parameters::LoadParametersFile(TString packageName, UInt_t tryFileMode, bool fixed) {

  std::cout << "Trying to load parameters file for package: " << packageName;
  if (tryFileMode!=1){
    std::cout << std::endl;
  }

  // and the file name for parameters file
  TString fileName = packageName + ".parameters.dat";
  // and the directory of parameters file.
  packageName.ToUpper();

  TString packageROOT = packageName + "ROOT";
  TString dirName = getenv(packageROOT.Data()) + TString("/parameters");
  
  // Now try reading in this file.  Last input variable is set to true,
  // indicating that we don't want to throw exception if a file is not found.
  ReadInputFile(fileName, dirName, tryFileMode, fixed);
  return 1;
}

int Parameters::TryLoadingParametersFile(std::string parameterName) {

  std::cout << "Trying to load parameters file for parameter: " << parameterName << std::endl;

  // Figure out the name of the package
  int pos = parameterName.find(".");
  TString packageName(parameterName.c_str(), pos);

  // and the file name for parameters file
  TString fileName = packageName + ".parameters.dat";

  // and the directory of parameters file.
  packageName.ToUpper();
  TString packageROOT = packageName + "ROOT";
  TString dirName = getenv(packageROOT.Data()) + TString("/parameters/");
  // Now try reading in this file.  Last input variable is set to 0,
  // indicating that we don't want to throw exception if a file is not found.
  ReadInputFile(fileName, dirName, 0);

  // Now try to find this parameter again
  mapIterator i = mapOfParameters.find(parameterName);
  if (i != mapOfParameters.end())
    return 1;
  else {
    return 0;
  }
}

void Parameters::ReadParamOverrideFile(TString filename) {

  std::cout << "Using Parameters override file = " << filename << std::endl;

  // Setting final input variable to true forces the parameters
  // that are loaded to be 'fixed'; ie immutable.
  // The 2 means that an exception is thrown if the file is not found
  ReadInputFile(filename, "", 2, true);
}

bool Parameters::HasParameter(string parameterName) {

  mapIterator i = mapOfParameters.find(parameterName);

  if (i != mapOfParameters.end())
    return 1;
  else {
    // Okay, we didn't find this parameter on the first try.  
    // Let's see if we can try loading the parameters file.
    // The function will return whether this parameter 
    // was found afterwards.
    return TryLoadingParametersFile(parameterName);
  }
}

int Parameters::GetParameterI(string parameterName) {
  if (HasParameter(parameterName)) {
    CheckReadParamOverrideFilePointPassed(parameterName);
    return atoi(mapOfParameters[parameterName].c_str());
  } else {
    std::cerr << "\n***** Parameters::GetParameterAsInteger *****\n" << "Cannot find parameter '" << parameterName << "'." << std::endl;
    throw NoSuchParameterException();
  }

  return -1;
}

double Parameters::GetParameterD(string parameterName) {
  if (HasParameter(parameterName)) {
    CheckReadParamOverrideFilePointPassed(parameterName);
    return atof(mapOfParameters[parameterName].c_str());
  } else {
    std::cerr << "\n***** Parameters::GetParameterAsDouble *****\n" << "Cannot find parameter '" << parameterName << "'." << std::endl;
    throw NoSuchParameterException();
  }

  return -1;

}

std::string Parameters::GetParameterS(string parameterName) {
  if (HasParameter(parameterName)) {
    CheckReadParamOverrideFilePointPassed(parameterName);
    return mapOfParameters[parameterName];
  } else {
    std::cerr << "\n***** Parameters::GetParameterAsString *****\n" << "Cannot find parameter '" << parameterName << "'." << std::endl;
    throw NoSuchParameterException();
  }

  return std::string();
}

void Parameters::ClearMapOfParameters() {
  mapOfParameters.clear();
}


void  Parameters::CheckReadParamOverrideFilePointPassed(string parameterName){

  if (!fReadParamOverrideFilePointPassed){
    std::cerr << "ERROR: Parameter '" << parameterName << "'" << std::endl
              << "should no be accessed in analysis algorithm constructors or globally" << std::endl
              << "since it will not be overwritten when an override parameter file is used (-p option)" << std::endl
              << "Please move the GetParameter calls to the Initialize method of the analysis algorithm." << std::endl;
    exit(1);
  }

}
