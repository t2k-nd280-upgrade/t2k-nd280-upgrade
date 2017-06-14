#ifndef ND280BrlECalBLTRConstructor_hh_seen
#define ND280BrlECalBLTRConstructor_hh_seen

// G4
#include <G4LogicalVolume.hh>

// nd280mc
#include <ND280Constructor.hh>
#include <ecal/ND280ECalModuleConstructor.hh>


class ND280BrlECalBLTRConstructor : public ND280ECalModuleConstructor
{
	public:
		
		ND280BrlECalBLTRConstructor(G4String name, ND280Constructor* parent)
			: ND280ECalModuleConstructor(name, parent)
			{	this->Init();	}
		
		virtual ~ND280BrlECalBLTRConstructor(){}
		
		G4LogicalVolume* GetPiece();
		
	private:
	
		void Init();
};

#endif
