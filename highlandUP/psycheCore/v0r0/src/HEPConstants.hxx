////////////////////////////////////////////////////////////
// $Id: HEPConstants.hxx,v 1.4 2015/11/18 13:05:08 izmaylov Exp $
//
// This is taken from CLHEP and is included into here so that we don't 
// have to depend on CLHEP.  All of the constants are in the UNO:: namespace.
// ----------------------------------------------------------------------
// HEP coherent Physical Constants
//
// This file has been provided by Geant4 (simulation toolkit for HEP).
//
// The basic units are :
//  		millimeter  
// 		nanosecond  
// 		Mega electron Volt  
// 		positon charge 
// 		degree Kelvin
//              amount of substance (mole)
//              luminous intensity (candela)
// 		radian  
//              steradian 
//
// Below is a non exhaustive list of Physical CONSTANTS,
// computed in the Internal HEP System Of Units.
//
// Most of them are extracted from the Particle Data Book :
//        Phys. Rev. D  volume 50 3-1 (1994) page 1233
// 
//        ...with a meaningful (?) name ...
//
// You can add your own constants.
//
// Author: M.Maire
//
// History:
//
// 23.02.96 Created
// 26.03.96 Added constants for standard conditions of temperature
//          and pressure; also added Gas threshold.

#ifndef HEP_CONSTANTS_HXX
#define HEP_CONSTANTS_HXX

#include "Units.hxx"
#include "TDatabasePDG.h"

class TDatabasePDG; 

namespace units {

    const double     pi  = 3.14159265358979323846;
    const double  twopi  = 2*pi;
    const double halfpi  = pi/2;
    const double     pi2 = pi*pi;

    //
    // 
    //
    const double Avogadro = 6.0221367e+23/mole;

    //
    // c   = 299.792458 mm/ns
    // c^2 = 898.7404 (mm/ns)^2 
    //
    const double c_light   = 2.99792458e+8 * meter/second;
    const double c_squared = c_light * c_light;

    //
    // h     = 4.13566e-12 MeV*ns
    // hbar  = 6.58212e-13 MeV*ns
    // hbarc = 197.32705e-12 MeV*mm
    //
    const double h_Planck      = 6.6260755e-34 * joule*second;
    const double hbar_Planck   = h_Planck/twopi;
    const double hbarc         = hbar_Planck * c_light;
    const double hbarc_squared = hbarc * hbarc;

    //
    //
    //
    const double electron_charge = - eplus;  // see SystemOfUnits.h
    const double e_squared = eplus * eplus;

    //
    // amu_c2 - atomic equivalent mass unit
    // amu    - atomic mass unit
    //
    const double electron_mass_c2 = 0.51099906 * MeV;
    const double   proton_mass_c2 = 938.27231 * MeV;
    const double  neutron_mass_c2 = 939.56563 * MeV;
    const double           amu_c2 = 931.49432 * MeV;
    const double              amu = amu_c2/c_squared;

    //
    // permeability of free space mu0    = 2.01334e-16 Mev*(ns*eplus)^2/mm
    // permittivity of free space epsil0 = 5.52636e+10 eplus^2/(MeV*mm)
    //
    const double mu0      = 4*pi*1.e-7 * henry/meter;
    const double epsilon0 = 1./(c_squared*mu0);

    //
    // electromagnetic coupling = 1.43996e-12 MeV*mm/(eplus^2)
    //
    const double elm_coupling           = e_squared/(4*pi*epsilon0);
    const double fine_structure_const   = elm_coupling/hbarc;
    const double classic_electr_radius  = elm_coupling/electron_mass_c2;
    const double electron_Compton_length = hbarc/electron_mass_c2;
    const double Bohr_radius = electron_Compton_length/fine_structure_const;

    const double alpha_rcl2 = fine_structure_const
    *classic_electr_radius
    *classic_electr_radius;

    const double twopi_mc2_rcl2 = twopi*electron_mass_c2
    *classic_electr_radius
    *classic_electr_radius;
    //
    //
    //
    const double k_Boltzmann = 8.617385e-11 * MeV/kelvin;

    //
    //
    //
    const double STP_Temperature = 273.15*kelvin;
    const double STP_Pressure    = 1.*atmosphere;
    const double kGasThreshold   = 10.*milligram/centimeter3;

    //
    //
    //
    const double universe_mean_density = 1.e-25*gram/centimeter3;
    
    //-----
    //particle masses: keep them hard-coded for faster access (instead can use for example from TDatabasePDG)
    //-----
    extern const double mass_proton;   
    extern const double mass_neutron;   
   
    extern const double mass_electron; 
    extern const double mass_muon;
    
    extern const double mass_pion_zero;   
    extern const double mass_pion_charged;
    extern const double mass_kaon_zero;
    extern const double mass_kaon_charged;
  
    extern TDatabasePDG* pdgBase;

}
#endif 





