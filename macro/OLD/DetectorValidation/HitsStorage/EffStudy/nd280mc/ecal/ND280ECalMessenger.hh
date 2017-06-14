#ifndef ND280ECalMessenger_hh_seen
#define ND280ECalMessenger_hh_seen

// nd280mc
#include <ecal/ND280ECalConstructor.hh>

// G4
#include <G4UIcommand.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWith3VectorAndUnit.hh>

/// G4 Messenger class for the ECalConstructors
class ND280ECalMessenger : public ND280ConstructorMessenger
{
	public:
		
		ND280ECalMessenger(ND280ECalConstructor* c, const char* guide);
		
		~ND280ECalMessenger()
		{
			delete fBuildCMD;		
			delete fTranslationCMD;
			delete fRotationAxisCMD;
			delete fRotationAngleCMD;
		}
		
	private:
		
		void SetNewValue(G4UIcommand* cmd, G4String val);
		
		ND280ECalConstructor* fConstructor;
		
		G4UIcmdWithABool* fBuildCMD;
		
		G4UIcmdWith3VectorAndUnit* fTranslationCMD;
		G4UIcmdWith3VectorAndUnit* fRotationAxisCMD;
		G4UIcmdWithADoubleAndUnit* fRotationAngleCMD;
};

#endif
