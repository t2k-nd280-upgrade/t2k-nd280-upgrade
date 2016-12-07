#ifndef ND280BrlECalTLBRConstructor_hh_seen
#define ND280BrlECalTLBRConstructor_hh_seen

// G4
#include <G4LogicalVolume.hh>

// nd280mc
#include <ND280Constructor.hh>
#include <ecal/ND280ECalModuleConstructor.hh>


class ND280BrlECalTLBRConstructor : public ND280ECalModuleConstructor
{
	public:
		
		ND280BrlECalTLBRConstructor(G4String name, ND280Constructor* parent)
			: ND280ECalModuleConstructor(name, parent)
			{	this->Init();	}
		
		virtual ~ND280BrlECalTLBRConstructor(){}
		
		G4LogicalVolume* GetPiece();
		
	private:
	
		void Init();
};

#endif
