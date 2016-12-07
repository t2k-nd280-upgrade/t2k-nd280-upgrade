#ifndef ND280ECal_hh_seen
#define ND280ECal_hh_seen

#include <globals.hh>
#include <G4ThreeVector.hh>

#include "G4SystemOfUnits.hh"

/// Constants of the ECal construction.
/// All hard-coded values should be contained within this namespace and
/// not in any of the constructors.
namespace ND280ECal
{
	/// Detector identities
        enum Detector{kNoDetector, kDS, kP0D, kBrl};
        enum Module{kNoModule, kTop, kSide, kBottom, kDSmodule};
	
	/// Design offsets from the magnet of each module
	namespace Offsets
	{
		// These offsets place the ECal module in its design position
		
		// All of these values are dependent on the size of the volumes.
		// If the volume sizes change at any time, say to include extra
		// dead material, then these values need to change as well.
		
		//
		// = = Barrel and P0D ECals
		
		// - - X
		// For all modules, the x component is the offset of the volume's
		// outer face (away from the beam) with respect to the magnet wall
		
		// Note that for modules destined for the Right Clam, the x-coordinate
		// has to be negated. This is done by the top-level ECal constructor
		// when SetLeftClamshell() or SetRightClamshell() is called.
		
		// - - Y
		// For top and side modules, the y component is the offset of the
		// volume's upper face, from the upper arm of the magnet
		
		// For bottom modules, the y component is the offset of the volume's
		// bottom face from the bottom arm of the magnet
		
		// - - Z
		// For P0D ECal modules, the z component is the offset of the volume's
		// up-stream face from the up-stream end of the magnet
		
		// For Barrel ECal modules, the z component is the offset of the volume's
		// down-stream face from the downstream face of the magnet
		
		const G4ThreeVector P0DTop    ( -216.0*mm, -270.0*mm,  507.0*mm );
		const G4ThreeVector P0DSide   (  -12.0*mm, -570.0*mm,  507.0*mm );
		const G4ThreeVector P0DBottom ( -116.0*mm,  270.0*mm,  507.0*mm );
		const G4ThreeVector BrlTop    ( -124.0*mm, -277.0*mm, -468.0*mm );
		const G4ThreeVector BrlSide   (   -5.0*mm, -769.5*mm, -468.0*mm );
		const G4ThreeVector BrlBottom (  -24.0*mm,  277.0*mm, -468.0*mm );
		
		//
		// = = DS ECal
		
		// This is the translation of the DS ECal origin from the origin
		// (0,0,0) of the basket
		
		// - - X
		// Should be central, in design position.
		
		// - - Y
		// The design position of the DS ECal is 0.0mm. However, for the
		// basket survey technote, the default position of the DS ECal
		// was assumed to be +10.0mm. Since the alignment group works from
		// this document, the DS ECal will be positioned where the survey
		// expected it to be.
		
		// - - Z
		// This is the design value based on drawing 201-10000.
		// This assumes the magnet has a total length of 7628mm (which
		// it does as of this comment being written 14/02/2011).
		
		const G4ThreeVector Downstream(    0.0*mm,    10.0*mm, 3072.5*mm );
	}
	
	
	//------------------------------------------------------------------
	
	
	/// Barrel ECal specific values
	namespace Brl
	{
		//
		// Carbon Panel
		
		/// Width of the aluminium frame running along the +/- x edges
		/// of a Carbon Panel
		const double CarbonFrameX = 145*mm;
		
		/// Width of the aluminium frame running along the -y end of
		/// the Carbon Panel in a side module
		const double CarbonFrameYSide = 187*mm;
		
		/// Width of the aluminium frame running along the -y end of
		/// the Carbon Panel in a top/bottom module
		const double CarbonFrameYTB = 145*mm;
		
		
		//
		// Lids & Bases
		
		/// Thickness of the Carbon Panel lid on a side module
		const double LidThicknessSide = 50.0*mm;
		
		/// Thickness of the Aluminium lid on a top/bottom module
		const double LidThicknessTB = 30.0*mm;
		
		/// Thickness of the Carbon Panel base on a side module
		const double BaseThicknessSide = 50.0*mm;
		
		/// Thickness of the Carbon Panel base on a top/bottom module
		const double BaseThicknessTB = 40.0*mm;
		
		
		//
		// Bulkheads
		
		/// Inset from the edge of the module of the primary bulkhead which
		/// surrounds the active region of the detector.
		/// Inset is measured from the edge of the module base to the
		/// outer face of the bulkhead.
		const double BulkheadPrimaryInset = 93.0*mm;
		
		/// As for BulkheadPrimaryInset but applies only to the parallel
		/// bulkhead on a side module
		const double BulkheadPrimaryInsetSide = 160.0*mm;
		
		//
		// Layers
		
		/// Additional space in a layer
		const double LayerGapThickness = 0.55*mm;
		
		/// Thickness of lead absorber layers
		const double LeadThickness = 1.75*mm;
		
		/// Length of a longitudinal fibre
		const double ParaFibreLength = 3986.0*mm;
		
		/// Length of a perpendicular fibre in a side module
		const double PerpFibreLengthSide = 2343.0*mm;
		
		/// Length of a perpendicular fibre in a top/bottom module
		const double PerpFibreLengthTB = 1583.0*mm;
		
		/// Length of a longitudinal bar
		const double ParaBarLength = 3840.0*mm;
		
		/// Length of a perpendicular bar in a side module
		const double PerpBarLengthSide = 2280.0*mm;
		
		/// Length of a perpendicular bar in a top/bottom module
		const double PerpBarLengthTB = 1520.0*mm;
		
		/// Number of perpendicular bars in module
		const int NPerpBars = 96;
		
		/// Number of longitudinal bars in a Side module
		const int NParaBarsSide = 57;
		
		/// Number of longitudinal bars in a Top/Bottom module
		const int NParaBarsTB = 38;
		
		/// Number of layers
		const int NLayers = 31;
	}
	
	
	//------------------------------------------------------------------
	
	
	/// P0D ECal specific values
	namespace P0D
	{
		//
		// Carbon Panel
		
		/// Width of the aluminium frame running along the +/- x edges
		/// of a Carbon Panel
		const double CarbonFrameX = 45.0*mm;
		
		/// Width of the aluminium frame running along the -y end of
		/// the Carbon Panel in a side module
		const double CarbonFrameYSide = 115.0*mm;
		
		/// Width of the aluminium frame running along the -y end of
		/// the Carbon Panel in a top/bottom module
		const double CarbonFrameYTB = 45.0*mm;
		
		
		//
		// Lids and Bases
		
		/// Thickness of an Aluminium lid
		const double LidThickness = 25.0*mm;
		
		/// Thickness of a Carbon Panel base
		const double BaseThickness = 30.0*mm;
		
		
		//
		// Bulkheads
		
		/// Inset from the egde of the module, of the parallel bulkhead
		/// on a side module
		const double BulkheadPrimaryInsetSide = 70.0*mm;
		
		/// Full height of the bulkhead at the readout end of the module
		const double BulkheadReadoutHeight = 250.0*mm;
		
		
		//
		// Layers
		
		/// Additional space in a layer
		const double LayerGapThickness = 0.5*mm;
		
		/// Thickness of lead absorber layers
		const double LeadThickness = 4.0*mm;
		
		/// Fibre length
		const double FibreLength = 2410.0*mm;
		
		/// Scintillator bar length
		const double BarLength = 2340.0*mm;
		
		/// Number of bars in side module layer
		const int NBarsSide = 69;
		
		/// Number of bars in top/bottom module layer
		const int NBarsTB = 38;
		
		/// Number of layers
		const int NLayers = 6;
	}
	
	
	
	//------------------------------------------------------------------
	
	
	
	/// Down-Stream ECal specific values
	namespace DS
	{
		//
		// Carbon Panel
		
		/// Width of the aluminium frame which surrounds the Carbon Panel
		const double CarbonFrameWidth = 142*mm;
		
		
		//
		// Lids and Bases
		
		/// Thickness of the Carbon Panel lid
		const double LidThickness = 25.0*mm;
		
		/// Thickness of the Carbon Panel base
		const double BaseThickness = 25.0*mm;
		
		
		//
		// Bulkheads
		
		/// Inset from the edge of the module of the primary bulkhead which
		/// surrounds the active region of the detector.
		/// Inset is measured from the edge of the module base to the
		/// outer face of the bulkhead.
		const double BulkheadPrimaryInset = 93.0*mm;
		
		
		//
		// Layers
		
		/// Additional space in a layer
		const double LayerGapThickness = 0.55*mm;
		
		/// Thickness of lead absorber layers
		const double LeadThickness = 1.75*mm;
		
		/// Fibre length
		const double FibreLength = 2144.0*mm;
		
		/// Scintillator bar length
		const double BarLength = 2000.0*mm;
		
		/// Number of bars in a layer
		const int NBars = 50;
		
		/// Number of layers
		const int NLayers = 34;
	}
	
	
	//------------------------------------------------------------------
	
	
	//
	// Carbon Panel
	
	/// Thickness of a sheet of carbon fibre in a Carbon Panel.
	const double CarbonSheetThickness = 2.0*mm;
	
	/// Width of the aluminium bar which separates the carbon-fibre sheets
	/// at the -y edge of a P0D or Brl ECal.
	const double CarbonSpacerWidth = 18*mm;
	
	
	//
	// Bulkheads
	
	/// Thickness of the thin aluminium plate which covers the inner
	/// face of a top/bottom module, or the top bulkhead of a side module.
	/// Applies to Barrel and P0D modules.
	const double BulkheadPlateThickness = 4.0*mm;	// TO CHECK - this was from memory
	
	/// Thickness of the primary bulkheads which surround the active region
	/// of every module
	const double BulkheadPrimaryThickness = 25.0*mm;
	
	
	//
	// Cooling Plates (Barrel & DS)
	
	/// Vertical thickness of the horizontal "feet" which attach to
	/// the module lid and base
	const double CoolingPlateBaseThickness = 25.0*mm;
	
	/// Horizontal width of the the horizontal "feet" which attached to
	/// the module lid and base
	const double CoolingPlateBaseWidth = 50.0*mm;
	
	/// Horizontal thickness of the vertical face onto which everything
	/// was mounted.
	const double CoolingPlateThickness = 8.0*mm;
	
	/// Distance from module's edge of the parallel colling plate in
	/// a Barrel ECal side module (measured to the start of the cooling
	/// plate feet, not the cooling plate's origin)
	const double CoolingPlateBrlSideInset = 67.0*mm;
	// This could have gone in the Brl namespace, but since there's only 
	// one module-specific valiable for the cooling plates I've left it
	// here with all the other cooling plate values.
	
	
	//
	// Layers
	
	/// Width of the aluminium frame surrounding the layers
	const double LayerFrameWidth = 20.0*mm;
	
	/// Lead overhang beyond end of bars
	const double LeadOverhang = 8.0*mm;
	
	/// Width of a scintillator bar (dimension parallel to layer)
	const double BarWidth = 40*mm;
	
	/// Height of a scintillator bar (dimension perpendicular to layer)
	const double BarHeight = 10*mm;
	
	
	//
	// Simulation control
	
	/// G4 Hit Length
	const double HitLength = 50.0*mm;
	
	/// G4 Hit Sagitta
	const double HitSagitta = 10.0*mm;
}

#endif
