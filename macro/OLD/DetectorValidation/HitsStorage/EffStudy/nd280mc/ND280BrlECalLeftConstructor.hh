#ifndef ND280BrlECalLeftConstructor_hh_seen
#define ND280BrlECalLeftConstructor_hh_seen

// nd280mc
#include <ND280Constructor.hh>
#include <ecal/ND280ECalModuleConstructor.hh>

// G4
#include <G4LogicalVolume.hh>

class ND280BrlECalLeftConstructor : public ND280ECalModuleConstructor
{
	public:
		
		ND280BrlECalLeftConstructor(G4String name, ND280Constructor* parent)
			: ND280ECalModuleConstructor(name, parent)
			{	this->Init();	}
		
		virtual ~ND280BrlECalLeftConstructor(){}
		
		G4LogicalVolume* GetPiece();
		
	private:
	
		void Init();
};

#endif
