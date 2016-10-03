////////////////////////////////////////////////////////////
// $Id: Units.hxx,v 1.1 2014/08/15 15:53:51 akaboth Exp $
///
/// A coherent system of units for HEP.
///
/// The basic units are :
///   - millimeter              (millimeter)
///   - nanosecond              (nanosecond)
///   - Mega electron Volt      (MeV)
///   - positron charge         (eplus)
///   - degree Kelvin           (kelvin)
///   - the amount of substance (mole)
///   - luminous intensity      (candela)
///   - radian                  (radian)
///   - steradian               (steradian)
///
/// Below is a non exhaustive list of derived and practical units (i.e. mostly
/// the SI units).  We can add our own units.  The SI numerical value of the
/// positron charge is defined here, as it is needed for conversion factor :
/// positron charge = e_SI (coulomb)
///
/// The unit definitions are defined in HEPUnits.hxx
///
/// The (non-unit) physical constants are defined in HEPConstants.hxx
///
/// This is taken from CLHEP and originated with GEANT4.  It is included here
/// so that we don't have to depend on CLHEP.  All of the units are in the
/// units:: namespace.
#ifndef UNITS_HXX
#define UNITS_HXX

namespace units {
    //
    // Factors
    //
    const double yotta  = 1.e+24;
    const double zetta  = 1.e+21;
    const double exa    = 1.e+18;
    const double peta   = 1.e+15;
    const double tera   = 1.e+12;
    const double giga   = 1.e+9;
    const double mega   = 1.e+6;
    const double kilo   = 1.e+3;
    const double hecto  = 100;
    const double deca   = 10;
    const double deci   = 0.1;
    const double centi  = 0.01;
    const double milli  = 1.e-3;
    const double micro  = 1.e-6;
    const double nano   = 1.e-9;
    const double pico   = 1.e-12;
    const double femto  = 1.e-15;
    const double atto   = 1.e-18;
    const double zepto  = 1.e-21;
    const double yocto  = 1.e-24;

    //
    // Length [L]
    //
    const double millimeter  = 1.;
    const double millimeter2 = millimeter*millimeter;
    const double millimeter3 = millimeter*millimeter*millimeter;

    const double centimeter  = 10.*millimeter;
    const double centimeter2 = centimeter*centimeter;
    const double centimeter3 = centimeter*centimeter*centimeter;

    const double meter  = 1000.*millimeter;
    const double meter2 = meter*meter;
    const double meter3 = meter*meter*meter;

    const double kilometer = kilo*meter;
    const double kilometer2 = kilometer*kilometer;
    const double kilometer3 = kilometer*kilometer*kilometer;

    const double micrometer = micro*meter;
    const double  nanometer = nano*meter;
    const double  angstrom  = 1.e-10*meter;
    const double  fermi     = femto*meter;

    const double      barn = 1.e-28*meter2;
    const double millibarn = milli*barn;
    const double microbarn = micro*barn;
    const double  nanobarn = nano*barn;
    const double  picobarn = pico*barn;

    // symbols
    const double nm  = nanometer;
    const double mm  = millimeter;
    const double mm2 = millimeter2;
    const double mm3 = millimeter3;

    const double cm  = centimeter;
    const double cm2 = centimeter2;
    const double cm3 = centimeter3;

    const double m  = meter;
    const double m2 = meter2;
    const double m3 = meter3;

    const double km  = kilometer;
    const double km2 = kilometer2;
    const double km3 = kilometer3;

    //
    // Angle
    //
    const double radian      = 1.;
    const double milliradian = milli*radian;
    const double degree = (3.14159265358979323846/180.0)*radian;

    const double   steradian = 1.;

    // symbols
    const double rad  = radian;
    const double mrad = milliradian;
    const double sr   = steradian;
    const double deg  = degree;

    //
    // Time [T]
    //
    const double nanosecond  = 1.;
    const double second      = 1.e+9 *nanosecond;
    const double millisecond = milli*second;
    const double microsecond = micro*second;
    const double  picosecond = pico*second;

    const double hertz = 1./second;
    const double kilohertz = kilo*hertz;
    const double megahertz = mega*hertz;

    // symbols
    const double ns = nanosecond;
    const double  s = second;
    const double ms = millisecond;

    //
    // Electric charge [Q]
    //
    const double eplus = 1. ;       // positron charge
    const double e_SI  = 1.60217733e-19;    // positron charge in coulomb
    const double coulomb = eplus/e_SI;      // coulomb = 6.24150 e+18 * eplus
    const double microcoulomb = micro*coulomb;
    const double femtocoulomb = femto*coulomb;

    // symbols
    const double fC = femtocoulomb;

    //
    // Energy [E]
    //
    const double megaelectronvolt = 1. ;
    const double     electronvolt = 1.e-6*megaelectronvolt;
    const double kiloelectronvolt = kilo*electronvolt;
    const double gigaelectronvolt = giga*electronvolt;
    const double teraelectronvolt = tera*electronvolt;
    const double petaelectronvolt = peta*electronvolt;

    const double joule = electronvolt/e_SI; // joule = 6.24150 e+12 * MeV

    // symbols
    const double MeV = megaelectronvolt;
    const double  eV = electronvolt;
    const double keV = kiloelectronvolt;
    const double GeV = gigaelectronvolt;
    const double TeV = teraelectronvolt;
    const double PeV = petaelectronvolt;

    //
    // Mass [E][T^2][L^-2]
    //
    const double  kilogram = joule*second*second/(meter*meter);
    const double      gram = 1.e-3*kilogram;
    const double milligram = milli*gram;

    // symbols
    const double  kg = kilogram;
    const double   g = gram;
    const double  mg = milligram;

    //
    // Power [E][T^-1]
    //
    const double watt = joule/second; // watt = 6.24150 e+3 * MeV/ns

    //
    // Force [E][L^-1]
    //
    const double newton = joule/meter;  // newton = 6.24150 e+9 * MeV/mm

    //
    // Pressure [E][L^-3]
    //
    const double pascal = newton/m2;       // pascal = 6.24150 e+3 * MeV/mm3
    const double bar        = 100000*pascal; // bar    = 6.24150 e+8 * MeV/mm3
    const double atmosphere = 101325*pascal; // atm    = 6.32420 e+8 * MeV/mm3

    //
    // Electric current [Q][T^-1]
    //
    const double    ampere = coulomb/second; // ampere = 6.24150 e+9 * eplus/ns
    const double milliampere = milli*ampere;
    const double microampere = micro*ampere;
    const double  nanoampere = nano*ampere;

    //
    // Electric potential [E][Q^-1]
    //
    const double megavolt = megaelectronvolt/eplus;
    const double kilovolt = 1.e-3*megavolt;
    const double     volt = 1.e-6*megavolt;

    //
    // Electric resistance [E][T][Q^-2]
    //
    const double ohm = volt/ampere; // ohm = 1.60217e-16*(MeV/eplus)/(eplus/ns)

    //
    // Electric capacitance [Q^2][E^-1]
    //
    const double farad = coulomb/volt;  // farad = 6.24150e+24 * eplus/Megavolt
    const double millifarad = milli*farad;
    const double microfarad = micro*farad;
    const double  nanofarad = nano*farad;
    const double  picofarad = pico*farad;

    //
    // Magnetic Flux [T][E][Q^-1]
    //
    const double weber = volt*second; // weber = 1000*megavolt*ns

    //
    // Magnetic Field [T][E][Q^-1][L^-2]
    //
    const double tesla    = volt*second/meter2;// tesla =0.001*megavolt*ns/mm2

    const double gauss    = 1.e-4*tesla;
    const double kilogauss = 1.e-1*tesla;

    //
    // Inductance [T^2][E][Q^-2]
    //
    const double henry = weber/ampere;  // henry = 1.60217e-7*MeV*(ns/eplus)**2

    //
    // Temperature
    //
    const double kelvin = 1.;

    //
    // Amount of substance
    //
    const double mole = 1.;

    //
    // Activity [T^-1]
    //
    const double becquerel = 1./second ;
    const double curie = 3.7e+10 * becquerel;

    //
    // Absorbed dose [L^2][T^-2]
    //
    const double gray = joule/kilogram ;

    //
    // Luminous intensity [I]
    //
    const double candela = 1.;

    //
    // Luminous flux [I]
    //
    const double lumen = candela*steradian;

    //
    // Illuminance [I][L^-2]
    //
    const double lux = lumen/meter2;

    //
    // Miscellaneous
    //
    const double perCent     = 0.01 ;
    const double perThousand = 0.001;
    const double perMillion  = 0.000001;
}
#endif
