#include <algorithm>
#include <functional>

#include <G4UnitsTable.hh>

//#include "ND280Log.hh"

#include "ND280Constructor.hh"

//#include <TND280Log.hxx>

ND280Constructor::ND280Constructor(G4String n, 
                                   //ND280UserDetectorConstruction* c) 
                                   ExN02DetectorConstruction* c) 
    : fLocalName(n), fName(n), fConstruction(c), fParent(NULL),
      fMessenger(NULL), fVisible(false) {
    fMessenger = fConstruction->GetMessenger();
}

ND280Constructor::ND280Constructor(G4String n, ND280Constructor* p) 
    : fLocalName(n), fName(n), fConstruction(NULL), fParent(p), 
      fMessenger(NULL), fVisible(false) {
    fName = fParent->GetName() + "/" + fLocalName;
    fConstruction = fParent->GetConstruction();
    fMessenger = fParent->GetMessenger();
}

ND280Constructor::~ND280Constructor() {;};

G4LogicalVolume *ND280Constructor::GetPiece(void) {
  //G4Exception("ND280Constructor::GetPiece(): Not implemented");
  G4ExceptionDescription msg;
  msg << "ND280Constructor::GetPiece(): Not implemented" << G4endl;
  G4Exception("ND280Constructor::GetPiece",
	      "MyCode0002",FatalException, msg);
  return NULL;
}

G4Material* ND280Constructor::FindMaterial(G4String name) {
    G4Material* material = G4Material::GetMaterial(name,true);
    return material;
}

G4String ND280Constructor::GetName(void) {
    return fName;
}

/// Set the local name of the object to be built.  The local name of the
/// logical volume being set, and also sets the full name of the volume.
void ND280Constructor::SetLocalName(const G4String& name) {
    fLocalName = name;
    fName = name;
    if (fParent) fName = fParent->GetName() + "/" + fName;
}


/// Set the visibility of the constructed object.
void ND280Constructor::SetVisible(bool v) {
    if (fVisible != v) {
      //ND280Verbose("Set Visibility for " << GetName()
      //<< " from " << fVisible << " to " << v);   
      G4ExceptionDescription msg;
      msg << "Set Visibility for " << GetName() 
	  << " from " << fVisible << " to " << v
	  << G4endl;
      G4Exception("ND280Constructor::SetVisible",
		  "MyCode0002",JustWarning, msg);
    }
    fVisible = v;
}

/// Set the visibility of the constructed object.
void ND280Constructor::SetVisibleDaughters(bool v) {
  if (fVisible != v) {
    //ND280Verbose("Set daughter visibility for " << GetName()
    //<< " to " << v);
    G4ExceptionDescription msg;
    msg << "Set daughter visibility for " 
	<< GetName()
	<< " to " << v
	<< G4endl;
    G4Exception("ND280Constructor::SetVisibleDaughters",
		"MyCode0002",JustWarning, msg);
    
  }

    SetVisible(false);

    for (std::map<G4String,ND280Constructor*>::iterator p 
             = fSubConstructors.begin();
         p!=fSubConstructors.end();
         ++p) {
        (*p).second->SetVisible(v);
    }
}

G4RotationMatrix ND280Constructor::stringToRotationMatrix(G4String rotation)
{
  // We apply successive rotations OF THE OBJECT around the FIXED
  // axes of the parent's local coordinates; rotations are applied
  // left-to-right (rotation="r1,r2,r3" => r1 then r2 then r3).

  G4RotationMatrix rot;

  unsigned int place = 0;

  while (place < rotation.size()) {

        G4double angle;
        char* p(0);
        G4String current=rotation.substr(place+1);
        angle = strtod(current.c_str(),&p) * deg;

        if (!p || (*p != ',' && *p != '\0')) {
	  //ND280Error("Invalid rotation specification: "
	  //<< rotation.c_str());
	  G4ExceptionDescription msg;
	  msg << "Invalid rotation specification: "
	      << rotation.c_str()
	      << G4endl;
	  G4Exception("ND280Constructor::stringToRotationMatrix",
		    "MyCode0002",JustWarning, msg);
	
	   return rot;
        }
  
        G4RotationMatrix thisRotation;

        switch(rotation.substr(place,1).c_str()[0]) {
              case 'X': case 'x':
                thisRotation = G4RotationMatrix(CLHEP::HepRotationX(angle));
                break;
              case 'Y': case 'y':
                thisRotation = G4RotationMatrix(CLHEP::HepRotationY(angle));
                break;
              case 'Z': case 'z':
                thisRotation = G4RotationMatrix(CLHEP::HepRotationZ(angle));
                break;
              default:
                //ND280Error(" Invalid rotation specification: "
		//<< rotation);	  
		G4ExceptionDescription msg;
		msg << "Invalid rotation specification: "
		    << rotation.c_str()
		    << G4endl;
		G4Exception("ND280Constructor::stringToRotationMatrix",
		    "MyCode0002",JustWarning, msg);
		
                return rot;
        }

       rot = thisRotation * rot;
       place = rotation.find(',',place);
       if (place > rotation.size()) break;
       ++place;
  }

  return rot;

}

ND280ConstructorMessenger::ND280ConstructorMessenger(ND280Constructor* c,
                                                     const char* guide) {
    fConstructor = c;
    fDirName = c->GetName() + "/";
    fDirectory = new G4UIdirectory(fDirName);

    G4cout << G4endl;
    G4cout << "ND280ConstructorMessenger::ND280ConstructorMessenger" << G4endl;
    G4cout << fDirName << G4endl;
    G4cout << G4endl;

    if (guide) fDirectory->SetGuidance(guide);
    else fDirectory->SetGuidance("Commands for a T2K sub-detector.");
    
    fVisibleCMD = new G4UIcmdWithABool(CommandName("visible"),this);
    fVisibleCMD->SetGuidance("The object is drawn if this is true.");
    fVisibleCMD->SetParameterName("Visibility",false);
    
    fVisibleDaughtersCMD = new G4UIcmdWithABool(
        CommandName("visibleDaughters"),this);
    fVisibleDaughtersCMD->SetGuidance(
        "The daughters of this object are drawn if this is true.");
    fVisibleDaughtersCMD->SetParameterName("Visibility",false);
}

void ND280ConstructorMessenger::SetNewValue(G4UIcommand *cmd, G4String val) {
    if (cmd == fVisibleCMD) {
        fConstructor->SetVisible(fVisibleCMD->GetNewBoolValue(val));
    }
    else if (cmd == fVisibleDaughtersCMD) {
        fConstructor->SetVisibleDaughters(
            fVisibleDaughtersCMD->GetNewBoolValue(val));
    }
}

ND280ConstructorMessenger::~ND280ConstructorMessenger() {
    delete fDirectory;
    delete fVisibleCMD;
    delete fVisibleDaughtersCMD;
}


/////// NEW ND280UPGRADE ////////
