#ifndef ND280P0DECalLeftConstructor_hh_seen
#define ND280P0DECalLeftConstructor_hh_seen

// nd280mc
#include <ND280Constructor.hh>
#include <ecal/ND280ECalModuleConstructor.hh>

// G4
#include <G4LogicalVolume.hh>

class ND280P0DECalLeftConstructor : public ND280ECalModuleConstructor
{
	public:
		
		ND280P0DECalLeftConstructor(G4String name, ND280Constructor* parent)
			: ND280ECalModuleConstructor(name, parent)
			{	this->Init();	}
		
		virtual ~ND280P0DECalLeftConstructor(){}
		
		G4LogicalVolume* GetPiece();
		
	private:
		
		void Init();
		
		/// This is the height from the bottom of the base to the top of
		/// the lid. This is necessary since the upstream bulkhead of the
		/// P0D ECals extends beyond the lid. SO the volume fHeightZ must
		/// encompass it.
		double fModuleHeightZ;
};

#endif
