#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4AssemblyVolume.hh>
#include <G4RotationMatrix.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4UnitsTable.hh>

#include "ND280UA1FluxReturnConstructor.hh"
#include "ND280UA1MRDSideConstructor.hh"
#include "ND280UA1MRDArmConstructor.hh"

class ND280UA1FluxReturnMessenger: public ND280ConstructorMessenger {
private:
    ND280UA1FluxReturnConstructor* fConstructor;

public:
    ND280UA1FluxReturnMessenger(ND280UA1FluxReturnConstructor* c) 
        : ND280ConstructorMessenger(c,"Control the magnet geometry."),
          fConstructor(c) { };

    virtual ~ND280UA1FluxReturnMessenger() {  };

    void SetNewValue(G4UIcommand *cmd, G4String val) {
        ND280ConstructorMessenger::SetNewValue(cmd,val);
    };
};

ND280UA1FluxReturnConstructor::~ND280UA1FluxReturnConstructor() {;}

void ND280UA1FluxReturnConstructor::Init(void) {

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

  fPlateThickness = 48*CLHEP::mm;    // average iron plate thickness
    fPlateWidth = 876*CLHEP::mm;       // length of flux return along beam axis
    fGapThickness = 17*CLHEP::mm;      // average gap between iron plates
    fSideSpacerDiameter = 25*CLHEP::mm;
    fArmSpacerDiameter = 25*CLHEP::mm;
    fSideGapLength = 910*CLHEP::mm;
    fArmGapLength = 700*CLHEP::mm;
    fArmSpacerOffset = 300*CLHEP::mm; 

    fPlateSpacing = fPlateThickness + fGapThickness;
    fPlateCount = 16;

    // The correct, "fYokeIndex" specific, values will be set in the
    // GetAssembly method below. Do not allow more than 10 "installed"
    // layers, otherwise TGeomIdManager "Duplicate id" errors appear.
    for (unsigned int i = 0; i<fPlateCount; ++i)
      fScintInstalled.push_back(i < 6 ? -1 : 0);

    fInnerHeight = 4040*CLHEP::mm;
    fOuterArmLength = 2823*CLHEP::mm;
    fInnerArmLength = fOuterArmLength 
        - fPlateThickness*fPlateCount 
        - fGapThickness*(fPlateCount - 1);

    fIsLefthandReturn = true;

    AddConstructor(new ND280UA1MRDSideConstructor("MRDSide",this));
    AddConstructor(new ND280UA1MRDArmConstructor("MRDArm",this));

    SetMessenger(new ND280UA1FluxReturnMessenger(this));
}

G4LogicalVolume *ND280UA1FluxReturnConstructor::GetPiece(void) {return NULL;}

const G4VisAttributes* ND280UA1FluxReturnConstructor::GetVisual(void) const {
    if (GetVisible()) {
        G4VisAttributes* visual = new G4VisAttributes();
        if (fIsLefthandReturn) {
            visual->SetColor(0.6,0.1,0.1,1); // red
        }
        else {
            visual->SetColor(0.1,0.1,0.6,1); // blue
        }
        return visual;
    }
    else {
        return &G4VisAttributes::Invisible;
    }
}

G4LogicalVolume *ND280UA1FluxReturnConstructor::MakeIronPlate(double length) {
    G4LogicalVolume* logVolume =
        new G4LogicalVolume(new G4Box(GetName()+"Plate",
                                      length/2,
                                      fPlateThickness/2,
                                      fPlateWidth/2),
                            FindMaterial("Iron"),
                            GetName());
    return logVolume;
}

G4LogicalVolume *ND280UA1FluxReturnConstructor::MakeSpacer(double diameter) {
// The spacers don't really need to be simulated, but being able to add them
// to the geometry helps debug the magnet simulation.
#define CREATE_SPACERS
#ifdef CREATE_SPACERS
    G4LogicalVolume* logVolume =
        new G4LogicalVolume(new G4Box(GetName()+"Spacer",
                                      diameter/2,
                                      fGapThickness/2,
                                      fPlateWidth/2),
                            FindMaterial("Iron"),
                            GetName());
    return logVolume;
#else
    return NULL;
#endif
}

G4AssemblyVolume *ND280UA1FluxReturnConstructor::GetAssembly(void) {
    G4AssemblyVolume* assembly = new G4AssemblyVolume();
    ND280UA1MRDSideConstructor& mrdSide 
        = Get<ND280UA1MRDSideConstructor>("MRDSide");
    mrdSide.SetYokeIndex(fYokeIndex);
    ND280UA1MRDArmConstructor& mrdArm
        = Get<ND280UA1MRDArmConstructor>("MRDArm");
    mrdArm.SetYokeIndex(fYokeIndex);

#if 1
    // MRDArm (i.e. horizontal, top/bottom) - yokes 0 - 7 : 3 inner layers
    // MRDSide (i.e. vertical, left/right)  - yokes 0 - 4 : 3 inner layers
    //                                        yoke  5 : 4 inner layers
    //                                        yokes 6 - 7 : 6 inner layers
    // This layout is the same for both, the left and the right, flux returns.
    for (unsigned int i = 0; i<fPlateCount; ++i) {
      if (i >= fScintInstalled.size()) break; // just a precaution
      if (fIsLefthandReturn) {
	if ((fYokeIndex >= 0) && (fYokeIndex <= 4))
	  fScintInstalled[i] = (i<3 ? (1+2+4) : 0);
	else if (fYokeIndex == 5)
	  fScintInstalled[i] = (i<3 ? (1+2+4) : (i==3 ? (0+2+0) : 0));
	else if ((fYokeIndex >= 6) && (fYokeIndex <= 7))
	  fScintInstalled[i] = (i<3 ? (1+2+4) : (i<6 ? (0+2+0) : 0));
	else
	  fScintInstalled[i] = (0);
      } else {
	if ((fYokeIndex >= 0) && (fYokeIndex <= 4))
	  fScintInstalled[i] = (i<3 ? (1+2+4) : 0);
	else if (fYokeIndex == 5)
	  fScintInstalled[i] = (i<3 ? (1+2+4) : (i==3 ? (0+2+0) : 0));
	else if ((fYokeIndex >= 6) && (fYokeIndex <= 7))
	  fScintInstalled[i] = (i<3 ? (1+2+4) : (i<6 ? (0+2+0) : 0));
	else
	  fScintInstalled[i] = (0);
      }
    }
#endif /* 0 or 1 */

    // Construct the other arm of the magnet.
    double armLength = fInnerArmLength + fPlateSpacing;
    double armHeight = - (fInnerHeight/2.0 + fPlateThickness/2.0);
    double armPos = (fInnerArmLength - fPlateSpacing)/2.0;
    for (unsigned int layer = 0; layer<fPlateCount; ++layer) {
        if (layer == fPlateCount-1) {
            // The outer most plate doesn't have the extra space for the gap.
            armLength -= fGapThickness;
            armPos += fGapThickness/2;
        }
        G4ThreeVector volCenter(armHeight,armPos,0);
        G4LogicalVolume *logVolume = MakeIronPlate(armLength);
        G4RotationMatrix *rotation = new G4RotationMatrix();
        rotation->rotateZ(-90*CLHEP::degree);
        assembly->AddPlacedVolume(logVolume,volCenter,rotation);
        logVolume->SetVisAttributes(GetVisual());

        // Stop after placing the outer plate.
        if (layer==fPlateCount-1) continue;

        // Add the spacers. 
        G4LogicalVolume* spacerVolume = MakeSpacer(fArmSpacerDiameter);
        spacerVolume->SetVisAttributes(GetVisual());
        if (spacerVolume) {
            for (double offset=fArmSpacerOffset; 
                 offset < fInnerArmLength;
                 offset += fArmGapLength + fArmSpacerDiameter) {
                double correctedOffset = armLength/2 - offset;
                G4ThreeVector spacerOffset(-fPlateSpacing/2,
                                           correctedOffset,
                                           0);
                G4ThreeVector spacerCenter = spacerOffset + volCenter;
                assembly->AddPlacedVolume(spacerVolume,spacerCenter,rotation);
            }
        }

        // Add the scintillating plates.
        if (layer<fScintInstalled.size() && (fScintInstalled[layer] & 1)) {
            int armSlotIndex = 0;
            for (int  slot = 0; slot < 2; ++slot) {
                mrdArm.SetSlotLayer(layer);
                mrdArm.SetSlotIndex(armSlotIndex++);
                G4LogicalVolume* scintVolume = mrdArm.GetPiece();
                double correctedSpacing = - fPlateSpacing / 2.0;
                double correctedOffset =
                    armLength / 2.0
                    - fArmSpacerOffset
                    - slot * (fArmGapLength + fArmSpacerDiameter)
                    - fArmGapLength / 2.0
                    - fArmSpacerDiameter / 2.0;
#if 1
                // Max. allowed = +- (fGapThickness - mrdArm.GetHeight()) / 2.0
                // ... move completely downwards (+2.5mm -> module's frame) ...
                correctedSpacing += (fIsLefthandReturn ? 1.0 : -1.0) *
                    (-((fGapThickness - mrdArm.GetHeight()) / 2.0) + 2.5*CLHEP::mm);
                // Max. allowed = +- (fArmGapLength - mrdArm.GetWidth()) / 2.0
                correctedOffset += (-1.0) *
                    (0.0*CLHEP::mm);
#endif /* 0 or 1 */
                G4ThreeVector scintOffset(correctedSpacing,
                                          correctedOffset,
                                          0);
                G4ThreeVector scintCenter = scintOffset + volCenter;
                G4RotationMatrix *mrdRotation = new G4RotationMatrix();
                mrdRotation->rotateZ(-90*CLHEP::degree);
                assembly->AddPlacedVolume(scintVolume,scintCenter,mrdRotation);
            }
        }

        armLength += fPlateSpacing;
        armPos -= fPlateSpacing/2;
        armHeight -= fPlateSpacing;
    }

    // Construct the back side of the flux return.
    double sideLength = fInnerHeight;
    double sidePos = -fPlateThickness/2.0;
    for (unsigned int layer = 0; layer<fPlateCount; ++layer) {
        G4ThreeVector volCenter(0,sidePos,0);
        G4LogicalVolume *logVolume = MakeIronPlate(sideLength);
        assembly->AddPlacedVolume(logVolume,volCenter,NULL);
        logVolume->SetVisAttributes(GetVisual());

        // Stop after placing the outer plate.
        if (layer==fPlateCount-1) continue;

        // Add the spacers.  They are symmetric on the sides.
        for (double offset=0.0; 
             offset<fInnerHeight/2;
             offset += fSideGapLength + fSideSpacerDiameter) {
            G4LogicalVolume* spacerVolume = MakeSpacer(fSideSpacerDiameter);
            spacerVolume->SetVisAttributes(GetVisual());
            if (!spacerVolume) continue;
            G4ThreeVector spacerOffset(offset,
                                       - fPlateSpacing/2,
                                       0);
            G4ThreeVector spacerCenter = spacerOffset + volCenter;
            assembly->AddPlacedVolume(spacerVolume,spacerCenter,NULL);
            if (offset>1*CLHEP::cm) {
                spacerOffset.setX(-offset);
                spacerCenter = spacerOffset + volCenter;
                assembly->AddPlacedVolume(spacerVolume,spacerCenter,NULL);
            }
        }

        // Add the scintillating plates.
        if (layer<fScintInstalled.size() && (fScintInstalled[layer] & 2)) {
            int sideSlotIndex = 2;
            for (int slot = -2; slot < 2; ++slot) {
                double correctedSpacing = - fPlateSpacing / 2.0;
                double correctedOffset =
                    slot * (fSideGapLength + fSideSpacerDiameter)
                    + fSideGapLength / 2.0
                    + fSideSpacerDiameter / 2.0;
#if 1
                // Max. allowed = +- (fGapThickness - mrdSide.GetHeight()) / 2.0
                correctedSpacing += (-1.0) *
                    (0.0*CLHEP::mm);
                // Max. allowed = +- (fSideGapLength - mrdSide.GetWidth()) / 2.0
                // ... move completely downwards (+1mm -> module's frame) ...
                correctedOffset += (fIsLefthandReturn ? 1.0 : -1.0) *
                    (-((fSideGapLength - mrdSide.GetWidth()) / 2.0) + 1.0*CLHEP::mm);
#endif /* 0 or 1 */
                // Place one SMRD scintillator in the magnet.
                G4ThreeVector scintOffset(correctedOffset,
                                          correctedSpacing,
                                          0);
                G4ThreeVector scintCenter = scintOffset + volCenter;
                mrdSide.SetSlotLayer(layer);
                mrdSide.SetSlotIndex(sideSlotIndex++);
                G4LogicalVolume* scintVolume = mrdSide.GetPiece();
                assembly->AddPlacedVolume(scintVolume,scintCenter,NULL);
            }
        }

        sideLength += 2*fPlateSpacing;
        sidePos -= fPlateSpacing;
    }

    // Construct an arm of the magnet.
    armLength = fInnerArmLength + fPlateSpacing;
    armHeight = fInnerHeight/2.0 + fPlateThickness/2.0;
    armPos = (fInnerArmLength - fPlateSpacing)/2.0;
    for (unsigned int layer = 0; layer<fPlateCount; ++layer) {
        if (layer == fPlateCount-1) {
            // The outer most plate doesn't have the extra space for the gap.
            armLength -= fGapThickness;
            armPos += fGapThickness/2;
        }
        G4ThreeVector volCenter(armHeight,armPos,0);
        G4LogicalVolume *logVolume = MakeIronPlate(armLength);
        G4RotationMatrix *rotation = new G4RotationMatrix();
        rotation->rotateZ(90*CLHEP::degree);
        assembly->AddPlacedVolume(logVolume,volCenter,rotation);
        logVolume->SetVisAttributes(GetVisual());

        // Stop after placing the outer plate.
        if (layer==fPlateCount-1) continue;

        // Add the spacers. 
        G4LogicalVolume* spacerVolume = MakeSpacer(fArmSpacerDiameter);
        spacerVolume->SetVisAttributes(GetVisual());
        if (spacerVolume) {
            for (double offset=fArmSpacerOffset; 
                 offset < fInnerArmLength;
                 offset += fArmGapLength + fArmSpacerDiameter) {
                double correctedOffset = armLength/2 - offset;
                G4ThreeVector spacerOffset(fPlateSpacing/2,
                                           correctedOffset,
                                           0.0);
                G4ThreeVector spacerCenter = spacerOffset + volCenter;
                assembly->AddPlacedVolume(spacerVolume,
                                          spacerCenter,
                                          rotation);
            }
        }

        // Add the scintillating plates.
        if (layer<fScintInstalled.size() && (fScintInstalled[layer] & 4)) {
            int armSlotIndex = 6;
            for (int  slot = 1; slot >=0; --slot) {
                mrdArm.SetSlotLayer(layer);
                mrdArm.SetSlotIndex(armSlotIndex++);
                G4LogicalVolume* scintVolume = mrdArm.GetPiece();
                double correctedSpacing = fPlateSpacing / 2.0;
                double correctedOffset =
                    armLength / 2.0
                    - fArmSpacerOffset
                    - slot * (fArmGapLength + fArmSpacerDiameter)
                    - fArmGapLength / 2.0 - fArmSpacerDiameter / 2.0;
#if 1
                // Max. allowed = +- (fGapThickness - mrdArm.GetHeight()) / 2.0
                // ... move completely downwards (+2.5mm -> module's frame) ...
                correctedSpacing += (fIsLefthandReturn ? 1.0 : -1.0) *
                    (-((fGapThickness - mrdArm.GetHeight()) / 2.0) + 2.5*CLHEP::mm);
                // Max. allowed = +- (fArmGapLength - mrdArm.GetWidth()) / 2.0
                correctedOffset += (-1.0) *
                    (0.0*CLHEP::mm);
#endif /* 0 or 1 */
                G4ThreeVector scintOffset(correctedSpacing,
                                          correctedOffset,
                                          0.0);
                G4ThreeVector scintCenter = scintOffset + volCenter;
                G4RotationMatrix *mrdRotation = new G4RotationMatrix();
                mrdRotation->rotateZ(90*CLHEP::degree);
                assembly->AddPlacedVolume(scintVolume,scintCenter,mrdRotation);
            }
        }

        armLength += fPlateSpacing;
        armPos -= fPlateSpacing/2;
        armHeight += fPlateSpacing;
    }


    return assembly;
}
