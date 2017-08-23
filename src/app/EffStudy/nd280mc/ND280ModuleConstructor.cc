#include <cmath>

#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>

//#include <TND280Log.hxx>

#include "ND280ModuleConstructor.hh"

ND280ModuleConstructor::~ND280ModuleConstructor() {
    if (fPartsList != 0) {
        fPartsList->clear();
        delete fPartsList;
    }
    if (fTransList != 0) {
        fTransList->clear();
        delete fTransList;
    }
}

void ND280ModuleConstructor::Init() {
  fWidth = 250*CLHEP::cm;
    fHeight = 250*CLHEP::cm;
    fLength = 0*CLHEP::cm;
    fTargetLength = 0*CLHEP::cm;
    fFixLength = false;
    xPosition = 0*CLHEP::cm;
    yPosition = 0*CLHEP::cm;

    // Set default translation parameters as 0.
    fPair.first = 0.0*CLHEP::mm;
    fPair.second = 0.0*CLHEP::mm;

    SetMessenger(new ND280ModuleConstructorMessenger(this));

    fPartsList = new PartsList();
    fTransList = new TransList();

    xmax = xmin = ymax = ymin = 0.0*CLHEP::mm;
}

G4LogicalVolume *ND280ModuleConstructor::GetPiece(void) {

    // Build the logical volumes.

    for (PartsList::iterator e = fPartsList->begin();
                             e != fPartsList->end(); ++e) {
        (*e)->SetWidth(GetWidth());
        (*e)->SetHeight(GetHeight());
    }

    // Calculate the length
    fLength = 0.0;
    for (PartsList::iterator e = fPartsList->begin();
                             e != fPartsList->end();++e) {
        double componentLength = (*e)->GetLength();
        if (componentLength < 0.0) {
	  //ND280Warn("%%%%%%% Negative Component Length ");
	  //ND280Warn("%% " << (*e)->GetName());

	  G4String strwarning = "%%%%%%% Negative Component Length %%"+(*e)->GetName();
	  G4Exception("ND280ModuleConstructor::GetPiece()",
		      "MyCode0002",JustWarning,
		      strwarning);

	  continue;
        }
        fLength += componentLength;
    }

    if (fLength<0.10*CLHEP::mm) return NULL;

    if (GetLength()>GetTargetLength()) {
      //ND280Error(GetName());
      //ND280Error(" Is " << GetLength()/cm << " cm long with "
      //<< GetTargetLength()/cm << " cm available");
      G4cerr << GetName() << G4endl;
      G4cerr << " Is " << GetLength()/CLHEP::cm << " cm long with "
	     << GetTargetLength()/CLHEP::cm << " cm available" << G4endl;
      
      //G4Exception("ND280ModuleConstructor::GetPiece(): Volume too long");
      G4Exception("ND280ModuleConstructor::GetPiece()",
		  "MyCode0002",FatalException,
		  " Volume too long");
    }

    /// Position all of the sub volumes.
    double zPosition = -fLength/2.;

    if (fFixLength) fLength = GetTargetLength();

    // Construct the envelope.
    G4VSolid *solid = new G4Box(GetName(),
                                GetWidth()/2,
                                GetHeight()/2,
                                GetLength()/2);

    /// Create the volume for the envelope.
    G4LogicalVolume *logVolume
	= new G4LogicalVolume(solid,
                              FindMaterial("Air"),
                              GetName());

    int i = 0;
    for (PartsList::iterator e = fPartsList->begin();
                             e != fPartsList->end(); ++e) {
        zPosition += (*e)->GetLength()/2;

	// Add translation parameters from fTransList in the placement, called
	// by the index i.
        
        G4double xpos = fTransList->at(i).first;
        G4double ypos = fTransList->at(i).second;

        if(xpos>xmax)xmax=xpos;
        if(xpos<xmin)xmin=xpos;
        if(ypos>ymax)ymax=ypos;
        if(ypos<ymin)ymin=ypos;

        new G4PVPlacement(0,
                          G4ThreeVector(xPosition+xpos,
                                        yPosition+ypos,
                                        zPosition),
                          (*e)->GetPiece(),
                          (*e)->GetName(),
                          logVolume,
                          false,
                          i);
        zPosition += (*e)->GetLength()/2;
        i++;
    }

    return logVolume;
}

void ND280ModuleConstructor::ClearComponentList(void) {
    fPartsList->clear();
}

void ND280ModuleConstructor::AddComponent(G4String name) {
      ND280ComponentConstructor* fConstructor
                   = &Get<ND280ComponentConstructor>(name);
      fPartsList->push_back(fConstructor);

      // Run parallel with fPartsList.
      fTransList->push_back(fPair);
}

void ND280ModuleConstructor::SetRepetitions(int r, int c) {
    if (r<1) {
      //G4Exception("*** Repetition count must be greater than "
      //"or equal to one.");
      G4Exception("ND280ModuleConstructor::SetRepetitions",
		  "MyCode0002",FatalException,
		  "*** Repetition count must be greater than or equal to one."); 
   }
    
    if (c<1) {
      //G4Exception("*** Component count must be greater than "
      //"or equal to one.");
      G4Exception("ND280ModuleConstructor::SetRepetitions",
		  "MyCode0002",FatalException,
		  "*** Component count must be greater than or equal to one."); 
    }
    
    if (fPartsList->size()<(unsigned) c) {
      //ND280Error("Not enough parts to build module");
      //ND280Error("    Parts supplied: " << fPartsList->size() 
      //<< "  Parts required: " << c);
      G4cerr << "Not enough parts to build module" << G4endl;
      G4cerr <<"    Parts supplied: " << fPartsList->size() 
	     << "  Parts required: " << c << G4endl;
	
      //G4Exception("Module construction failed.");
      G4Exception("ND280ModuleConstructor::SetRepetitions",
		  "MyCode0002",FatalException,
		  "Module construction failed."); 
      return;
    }

    PartsList additions(fPartsList->end() - c, fPartsList->end());

    // Start the count from 1 since there is already one component on the list.
    for (int i=1; i<r; ++i) {
        PartsList::iterator e  = fPartsList->end();
        fPartsList->insert(e,additions.begin(),additions.end());
    }
    for (int i = 0; i < r*c; i++)
      {
	fTransList->push_back(fPair);
      }
}

// Method to translate each component in a module, so that misalignment is
// simulated.  m is the module number, cPerM is the component interval (number
// of components in a module).  c is the number of components to translate,
// starting from the top of the list.  This was design to simulate FGD
// scintillator module misalignments.  There is no translation parameter in z.
void ND280ModuleConstructor::SetModuleCompTrans(int m, int cPerM, int c,
                                                double transX, double transY)
{
  for (int i = 0; i < c; i++)
    {
      fTransList->at(m*cPerM+i).first = transX;
      fTransList->at(m*cPerM+i).second = transY;
    }
}
