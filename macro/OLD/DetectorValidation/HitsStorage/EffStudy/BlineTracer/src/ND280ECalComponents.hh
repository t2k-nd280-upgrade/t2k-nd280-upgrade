#ifndef ND280ECalComponents_hh_seen
#define ND280ECalComponents_hh_seen
#include <string>

#include <G4LogicalVolume.hh>
#include <ND280Constructor.hh>

// // NEW ND280UPGRADE
// #include "ND280RootPersistencyManager.hh" // XML input file
// #include "ExN02ND280XML.hh" // XML input file 
// //

/// Contains static methods which return G4LogicalVolumes containing 
/// dead-material components which are common to all ECals
class ND280ECalComponents {
public:
    
    ND280ECalComponents(const std::string& prefix)
        : fPrefix(prefix) {}
    ~ND280ECalComponents(){}
    
    /// Solid box of lead
    G4LogicalVolume* Lead(const double x, const double y, const double z) const;
    
    /// Solid box of aluminium
    G4LogicalVolume* Bulkhead(const double x, const double y, const double z) const;
    
    /// A gap (currently air) to act as a spacer between layers
    G4LogicalVolume* LayerGap(const double x, const double y, const double z) const;
    
    /// Solid box of carbon fibre
    G4LogicalVolume* CarbonFibre(const double x, const double y, const double z) const;
    
    /// Carbon pannel for P0D or Brl ECals.
    /// Contains:
    /// - Two sheets of carbon fibre (+z and -z)
    /// - Framed on three sides by alumimium
    /// - - A single piece on the +y edge
    /// - - Pieces on both x edges
    /// - Separated by an alumimum bar on -y edge
    /// - Gap between filled with air (in reality it is filled with Nomex)
    G4LogicalVolume* CarbonPanel(
        const double x,
        const double y,
        const double z,
        const double al_thickness_x,
        const double al_thickness_y ) const;
    
    /// Aluminium cooling plate for Barrel and DS ECals
    G4LogicalVolume* CoolingPlate(const double height, const double length) const;

  // // NEW ND280UPGRADE
  // ExN02ND280XML * GetND280XML() const {return fND280XMLInput;};
  
private:
    /// The  prefix for all of the volume names.
    std::string fPrefix;

  //ExN02ND280XML *fND280XMLInput;
};

#endif

