#include <ecal/ND280ECalMessenger.hh>

#include <ND280Constructor.hh>

#include <G4UIcommand.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWith3VectorAndUnit.hh>

ND280ECalMessenger::ND280ECalMessenger(ND280ECalConstructor* c, const char* guide = NULL)
  : ND280ConstructorMessenger(c, guide),
    fConstructor(c)
{	
	fTranslationCMD = new G4UIcmdWith3VectorAndUnit(CommandName("setTranslation"), this);
	fTranslationCMD->SetParameterName("dx", "dy", "dz", false);
	fTranslationCMD->SetUnitCategory("Length");
	fTranslationCMD->SetGuidance("Set a translation to be applied to the ECal from it's design position.");
	fTranslationCMD->SetGuidance("Transformation is applied in the global co-ordinate system.");
	fTranslationCMD->SetGuidance("Usage: [dx] [dy] [dz] [unit]");
	
	fRotationAxisCMD = new G4UIcmdWith3VectorAndUnit(CommandName("setRotationAxis"),this);
	fRotationAxisCMD->SetParameterName("dx", "dy", "dz", false);
	fRotationAxisCMD->SetUnitCategory("Length");
	fRotationAxisCMD->SetGuidance("Set the axis about which a rotation of the module will be performed.");
	fRotationAxisCMD->SetGuidance("No rotation will take place unless you also set the angle.");
	fRotationAxisCMD->SetGuidance("Axis is defined in the global co-ordinate system.");
	fRotationAxisCMD->SetGuidance("Usage: [dx] [dy] [dz] [unit]");
	
	fRotationAngleCMD = new G4UIcmdWithADoubleAndUnit(CommandName("setRotationAngle"), this);
	fRotationAngleCMD->SetParameterName("angle", false);
	fRotationAngleCMD->SetUnitCategory("Angle");
	fRotationAngleCMD->SetGuidance("Set the angle through which the module will be rotated.");
	fRotationAngleCMD->SetGuidance("If you do not also set the axis, this will be performed about the vertical axis");
	fRotationAngleCMD->SetGuidance("Usage: [angle] [unit]");
}

void ND280ECalMessenger::SetNewValue(G4UIcommand *cmd, G4String val)
{
	if( cmd == fTranslationCMD )
	{
		G4ThreeVector translation = fTranslationCMD->GetNew3VectorValue(val);
		fConstructor->SetExtraOffset(translation);
	}
	else if( cmd == fRotationAxisCMD )
	{
		G4ThreeVector axis = fRotationAxisCMD->GetNew3VectorValue(val);
		fConstructor->SetRotationAxis(axis);
	}
	else if( cmd == fRotationAngleCMD )
	{
		double angle = fRotationAngleCMD->GetNewDoubleValue(val);
		fConstructor->SetRotationAngle(angle);
	}
	else
	{
		ND280ConstructorMessenger::SetNewValue(cmd, val);
	}
}
