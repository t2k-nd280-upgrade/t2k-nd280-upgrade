#include "UnitsParser.hxx"
#include "Units.hxx"
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>

UnitsParser::UnitsParser() {

  // Define units.  Add to this list as needed.

  // Length 
  units["km"] = units::km;
  units["m"] = units::m;
  units["cm"] = units::cm;
  units["mm"] = units::mm;
  units["um"] = units::micrometer;
  units["nm"] = units::nanometer;
  units["Ang"] = units::angstrom;

  // Area
  units["km2"] = units::km2;
  units["m2"] = units::m2;
  units["cm2"] = units::cm2;
  units["mm2"] = units::mm2;
  units["barn"] = units::barn;
  units["mbarn"] = units::millibarn;
  units["mubarn"] = units::microbarn;
  units["nbarn"] = units::nanobarn;
  units["pbarn"] = units::picobarn;

  // Volume
  units["km3"] = units::km3;
  units["m3"] = units::m3;
  units["cm3"] = units::cm3;
  units["mm3"] = units::mm3;

  // Degree
  units["rad"] = units::radian;
  units["mrad"] = units::milliradian;
  units["sr"] = units::steradian;
  units["deg"] = units::degree;

  // Time
  units["s"] = units::second;
  units["ms"] = units::millisecond;
  units["mus"] = units::microsecond;
  units["ns"] = units::nanosecond;
  units["ps"] = units::picosecond;

  // Frequency
  units["Hz"] = units::hertz;
  units["kHz"] = units::kilohertz;
  units["MHz"] = units::megahertz;

  // Electric Charge
  units["e+"] = units::eplus;
  units["C"] = units::coulomb;

  // Energy
  units["eV"] = units::eV;
  units["keV"] = units::keV;
  units["MeV"] = units::MeV;
  units["GeV"] = units::GeV;
  units["TeV"] = units::TeV;
  units["PeV"] = units::PeV;
  units["J"] = units::joule;

  // Energy/Length
  units["GeV/cm"] = units::GeV / units::cm;
  units["MeV/cm"] = units::MeV / units::cm;
  units["keV/cm"] = units::keV / units::cm;
  units["eV/cm"] = units::eV / units::cm;

  // Inverse energy  
  units["1/eV"] = 1.0 / units["eV"];
  units["1/keV"] = 1.0 / units["keV"];
  units["1/MeV"] = 1.0 / units["MeV"];
  units["1/GeV"] = 1.0 / units["GeV"];

  // Mass
  units["mg"] = units::milligram;
  units["g"] = units::gram;
  units["kg"] = units::kilogram;

  // Volumic Mass
  units["g/cm3"] = units::gram / units::cm3;
  units["mg/cm3"] = units::milligram / units::cm3;
  units["kg/m3"] = units::kilogram / units::cm3;

  // Mass/Surface
  units["g/cm2"] = units::gram / units::cm2;
  units["mg/cm2"] = units::milligram / units::cm2;
  units["kg/cm2"] = units::kilogram / units::cm2;

  // Surface/Mass
  units["cm2/g"] = units::cm2 / units::gram;

  // Energy*Surface/Mass
  units["eV*cm2/g"] = units::eV * units::cm2 / units::gram;
  units["keV*cm2/g"] = units::keV * units::cm2 / units::gram;
  units["MeV*cm2/g"] = units::MeV * units::cm2 / units::gram;
  units["GeV*cm2/g"] = units::GeV * units::cm2 / units::gram;

  // Power
  units["W"] = units::watt;

  // Force
  units["N"] = units::newton;

  // Pressure
  units["Pa"] = units::pascal;
  units["bar"] = units::bar;
  units["atm"] = units::atmosphere;

  // Electric current
  units["A"] = units::ampere;
  units["mA"] = units::milliampere;
  units["muA"] = units::microampere;
  units["nA"] = units::nanoampere;

  // Electric potential
  units["V"] = units::volt;
  units["kV"] = units::kilovolt;
  units["MV"] = units::megavolt;

  // Magnetic flux
  units["Wb"] = units::weber;

  // Magnetic flux density
  units["T"] = units::tesla;
  units["kG"] = units::kilogauss;
  units["G"] = units::gauss;

  // Speed
  units["cm/us"] = units["cm"] / units["mus"];
  units["cm/ns"] = units["cm"] / units["ns"];
  units["mm/ns"] = units["mm"] / units["ns"];

  // Length/Energy
  units["mm/MeV"] = units["mm"] / units["MeV"];
  units["mm/keV"] = units["mm"] / units["keV"];
  units["cm/MeV"] = units["cm"] / units["MeV"];
  units["cm/keV"] = units["cm"] / units["keV"];

  // Dummy units for diffusion coefficient
  units["cm/sqrt(cm)"] = units["cm"] / sqrt(units["cm"]);
  units["mm/sqrt(cm)"] = units["mm"] / sqrt(units["cm"]);
  units["um/sqrt(cm)"] = units["um"] / sqrt(units["cm"]);

  // Dummy units for electron mobility
  units["cm2/(Vs)"] = units["cm2"] / (units["V"] * units["s"]);
}

UnitsParser::~UnitsParser() {

}

std::string UnitsParser::Convert2DoubleWithUnit(std::string input) {

  double value;
  std::string unit;

  std::istringstream line(input);
  if (!(line >> value >> unit)) {
    std::cerr << "ND::UnitsParser: badly formatted input string. Returning 0." << std::endl;
    return "0.0";
  }

  // Check if requested unit is in map.
  if (units.find(unit) == units.end()) {
    std::cerr << "ND::UnitsParser: requested unit '" << unit << "' not found. Returning 0." << std::endl;
    return "0.0";
  }

  value = value * units[unit];

  char s[256];
  sprintf(s, "%f", value);

  return s;

}

void UnitsParser::PrintListOfUnits() {

  std::cout << std::endl;
  std::cout << "***** List of available units *****" << std::endl << std::endl;
  for (std::map<std::string, double>::iterator unit = units.begin(); unit != units.end(); unit++)
    std::cout << (*unit).first << std::endl;
  std::cout << std::endl;

}
