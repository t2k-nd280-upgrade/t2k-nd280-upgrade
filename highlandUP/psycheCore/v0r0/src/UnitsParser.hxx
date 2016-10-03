#ifndef UnitsParser_h
#define UnitsParser_h
#include "TString.h"
#include <iostream>
#include <string>
#include <fstream>
#include <map>

/// This class provides a method for converting a 
/// string like "1.5 cm" into a double with the 
/// appropriate unit.  To do so it defines a set 
/// of units, using the same base units as in 
/// oaEvent/src/HEPUnits.hxx: ie mm, ns, MeV...
/// Only a fairly limited set of units is defined.
class UnitsParser {

  public:

    /// Constructor that creates the list of units.
    /// Try not to call this too often.
    UnitsParser();
    ~UnitsParser();

    /// Converts a string like "1.5 cm" into a double
    /// with the appropriate units.
    std::string Convert2DoubleWithUnit(std::string line);

    /// Prints all the defined units.
    void PrintListOfUnits();

  private:

    /// Internal map of unit ==> conversion factor.
    std::map<std::string, double> units;
};

#endif
