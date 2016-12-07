#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4UnitsTable.hh>

//#include <TND280Log.hxx>

#include "ND280UA1MagnetConstructor.hh"
#include "ND280UA1ClamConstructor.hh"
#include "ND280BasketConstructor.hh"

class ND280UA1MagnetMessenger: public ND280ConstructorMessenger {
private:
    ND280UA1MagnetConstructor* fConstructor;
    G4UIcmdWithADoubleAndUnit* fLeftClamSplitCMD;
    G4UIcmdWithADoubleAndUnit* fRightClamSplitCMD;

public:
    ND280UA1MagnetMessenger(ND280UA1MagnetConstructor* c) 
        : ND280ConstructorMessenger(c,"Control the magnet geometry."),
          fConstructor(c) {
        fLeftClamSplitCMD
            = new G4UIcmdWithADoubleAndUnit(
                CommandName("leftSplit"),this);
        fLeftClamSplitCMD->SetGuidance(
            "Separation of the Left Clam from the center line.");
        fLeftClamSplitCMD->SetParameterName("LeftSplit",false);
        fLeftClamSplitCMD->SetUnitCategory("Length");

        fRightClamSplitCMD
            = new G4UIcmdWithADoubleAndUnit(
                CommandName("rightSplit"),this);
        fRightClamSplitCMD->SetGuidance(
            "Separation of the Right Clam from the center line.");
        fRightClamSplitCMD->SetParameterName("RightSplit",false);
        fRightClamSplitCMD->SetUnitCategory("Length");
    };

    virtual ~ND280UA1MagnetMessenger() {
        delete fLeftClamSplitCMD;
        delete fRightClamSplitCMD;
    };

    void SetNewValue(G4UIcommand *cmd, G4String val) {
        if (cmd==fLeftClamSplitCMD) {
            fConstructor->SetLeftClamSplit(
                fLeftClamSplitCMD->GetNewDoubleValue(val));
        }
        else if (cmd==fRightClamSplitCMD) {
            fConstructor->SetRightClamSplit(
                fRightClamSplitCMD->GetNewDoubleValue(val));
        }
        else {
            ND280ConstructorMessenger::SetNewValue(cmd,val);
        }
    };
};

ND280UA1MagnetConstructor::~ND280UA1MagnetConstructor() {;};

void ND280UA1MagnetConstructor::Init(void) {
  
  ////////
  //
  // NEW ND280UPGRADE
  //
  
  // Take XML inputs
  ND280RootPersistencyManager* InputPersistencyManager
    = ND280RootPersistencyManager::GetInstance();
  fND280XMLInput = InputPersistencyManager->GetXMLInput();  
  
  // Set visible / invisible Magnet
  if( GetND280XML()->GetXMLInvisMagnet() ) SetVisible(false);
  else SetVisible(true);
  
  ////////
 
  fMagnetWidth = 5.72*meter;
  fMagnetHeight = 6.2*meter;
  fMagnetLength = 8*meter;  // Magnet is actually about 7.7 m long
  
  fLeftClamSplit = 0*cm;
  fRightClamSplit = 0*cm;

  AddConstructor(new ND280UA1ClamConstructor("LeftClam",this));
  Get<ND280UA1ClamConstructor>("LeftClam").SetLeftClamshell();
  
  AddConstructor(new ND280UA1ClamConstructor("RightClam",this));
  Get<ND280UA1ClamConstructor>("RightClam").SetRightClamshell();

  AddConstructor(new ND280BasketConstructor("Basket",this));  
  
  SetMessenger(new ND280UA1MagnetMessenger(this));
}

G4LogicalVolume *ND280UA1MagnetConstructor::GetPiece(void) {
    G4LogicalVolume *logVolume
	= new G4LogicalVolume(new G4Box(GetName(),
                                        GetWidth()/2.0,
                                        GetHeight()/2.0,
                                        GetLength()/2.0),
                              FindMaterial("Air"),
                              GetName());
    
    //if (GetVisible()) {
    //G4VisAttributes *visual = new G4VisAttributes();
    //visual->SetColor(0.5,0.5,0.5,1); // white
    //visual->SetForceWireframe(true);
    //logVolume->SetVisAttributes(visual);
    //} 
    //else {
    //logVolume->SetVisAttributes(G4VisAttributes::Invisible);
    //}
    logVolume->SetVisAttributes(G4VisAttributes::Invisible);

    ND280BasketConstructor& basket
        = Get<ND280BasketConstructor>("Basket");

    ND280UA1ClamConstructor& leftClam = Get<ND280UA1ClamConstructor>("LeftClam");
    ND280UA1ClamConstructor& rightClam = Get<ND280UA1ClamConstructor>("RightClam");

  //------------------------------ 
  // CREATE INNER BASKET
  //------------------------------ 
  G4LogicalVolume *basketVolume = basket.GetPiece();

  // NEW ND280UPGRADE
  if( GetND280XML()->GetXMLInvisBasket() ||
      !GetND280XML()->GetXMLUseBasket() // It's just the volume with Air (everything inside is not drawn)
      ){
    basketVolume->SetVisAttributes(G4VisAttributes::Invisible); 
  }
    
  //

  new G4PVPlacement(0,                    // rotation
		    basket.GetDeflectedCenter(), // position
		    basketVolume,         // logical volume
		    basket.GetName(),     // name
		    logVolume,            // mother  volume
		    false,                // not MANY
		    0);                   // no copy number

  
  // The left and right clams are vertically offset from the center of the
  // unloaded basket.
  const double clamYOffset = -1*cm;

  
  //------------------------------ 
  // CREATE THE TWO HALVES OF THE UA1 MAGNET
  //------------------------------ 
      
  //if( GetND280XML()->GetXMLUseMagnet() ){    // comment otherwise it doesn't draw P0D- and Brl- ECals  
    
    double leftShift = leftClam.GetWidth()/2.0 + GetLeftClamSplit();
    new G4PVPlacement(0,                            // rotation
		      G4ThreeVector(leftShift,clamYOffset,0), // position
		      leftClam.GetPiece(),          // logical volume
		      leftClam.GetName(),           // name
		      logVolume,                    // mother  volume
		      false,                        // no boolean operations
		      0);                           // no specific field
    
    double rightShift = -rightClam.GetWidth()/2.0 - GetRightClamSplit();
    new G4PVPlacement(0,                             // rotation
		      G4ThreeVector(rightShift,clamYOffset,0), // position
		      rightClam.GetPiece(),          // logical volume
		      rightClam.GetName(),           // name
		      logVolume,                     // mother  volume
		      false,                         // no boolean operations
		      0);                            // no specific field

    //}
  
  if (leftClam.GetInnerLength() < basket.GetLength()
      || rightClam.GetInnerLength() < basket.GetLength()) {
    
    // ND280Error(basket.GetName());
    // ND280Error(" Is " << basket.GetLength()/cm << " cm long with "
    // 		 << leftClam.GetInnerLength()/cm << " cm"
    // 		 << " or " << rightClam.GetInnerLength()/cm
    // 		 << " cm available");
    // G4Exception("ND280UA1MagnetConstructor:: Basket is too long");
    
    G4cout << basket.GetName() << G4endl;
    G4cout << " Is " << basket.GetLength()/cm << " cm long with "
	   << leftClam.GetInnerLength()/cm << " cm"
	   << " or " << rightClam.GetInnerLength()/cm
	   << " cm available" << G4endl;
    G4Exception("ND280UA1MagnetConstructor:: Basket",
		"MyCode0002",FatalException,"Basket is too long");      
  }    
  
  return logVolume;
}
