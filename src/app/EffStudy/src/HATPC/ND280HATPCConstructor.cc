#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4PVReplica.hh>
#include "ND280HATPCConstructor.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"

#include <MyND280BeamConstructor.hh>

//
// Cesar Jesus 19/3/18
//
// Constructor of the HATPC detectors
//

ND280HATPCConstructor::ND280HATPCConstructor(G4String n, ExN02DetectorConstruction *c)
    : ND280Constructor(n, c),
      fHATPCWidth(1 * CLHEP::cm),
      fHATPCHeight(1 * CLHEP::cm),
      fHATPCLength(1 * CLHEP::cm),
      fHATPCDriftWidth(1 * CLHEP::cm),
      fHATPCDriftHeight(1 * CLHEP::cm),
      fHATPCDriftLength(1 * CLHEP::cm),
      fHATPCCathodeThickness(1 * CLHEP::cm),
      fHATPCInnerBoxWall(1 * CLHEP::cm),
      fHATPCOuterBoxWall(1 * CLHEP::cm),
      fHATPCSteppingLimit(1 * CLHEP::cm),
      fActiveHATPCVerticalOffset(1 * CLHEP::cm),
      fHATPCFCName("")
{
}
ND280HATPCConstructor::ND280HATPCConstructor(G4String n, ND280Constructor *p)
    : ND280Constructor(n, p),
      fHATPCWidth(1 * CLHEP::cm),
      fHATPCHeight(1 * CLHEP::cm),
      fHATPCLength(1 * CLHEP::cm),
      fHATPCDriftWidth(1 * CLHEP::cm),
      fHATPCDriftHeight(1 * CLHEP::cm),
      fHATPCDriftLength(1 * CLHEP::cm),
      fHATPCCathodeThickness(1 * CLHEP::cm),
      fHATPCInnerBoxWall(1 * CLHEP::cm),
      fHATPCOuterBoxWall(1 * CLHEP::cm),
      fHATPCSteppingLimit(1 * CLHEP::cm),
      fActiveHATPCVerticalOffset(1 * CLHEP::cm),
      fHATPCFCName("")
{
}

ND280HATPCConstructor::~ND280HATPCConstructor() { ; }

G4LogicalVolume *ND280HATPCConstructor::GetPiece(void)
{

    // Specifications and definitions

    //G4bool DebugHATPCMass = true;
    // G4bool fVisible = true;

    // G4String parentname = GetHATPCParentName();
    // G4String name = GetHATPCName();

    G4LogicalVolume *logVolume    = NULL;

    // Volumes

    // Only logVolume for the HATPC.
    // No G4PVPlacement for logVolume in HATPCConstructor.
    // The HATPCs are placed in DetectorConstruction (Up and Down).

    logVolume = new G4LogicalVolume(new G4Box(GetHATPCParentName() + "/" + GetHATPCName(),
                                              fHATPCWidth / 2.,
                                              fHATPCHeight / 2.,
                                              fHATPCLength / 2.),
                                    FindMaterial("Galactic"),
                                    GetHATPCParentName() + "/" + GetHATPCName());
    logVolume->SetVisAttributes(G4Colour(1.0, 1.0, 0.0));
    if (fVisible != true)
        logVolume->SetVisAttributes(G4VisAttributes::Invisible);

    G4LogicalVolume *driftVolume  = NULL;

    fHATPCWidth = GetHATPCWidth();
    fHATPCHeight = GetHATPCHeight();
    fHATPCLength = GetHATPCLength();
    // double HATPCwidth = GetHATPCWidth();
    // double HATPCheight = GetHATPCHeight();
    // double HATPClength = GetHATPCLength();

    cout << "HATPCwidht is: " << fHATPCWidth << endl;
    cout << "HATPCheight is: " << fHATPCHeight << endl;
    cout << "HATPClength is: " << fHATPCLength << endl;

    double FCthickness;

    G4String FCName = GetHATPCFCName();

    ///////////////
    ///////////////
    ///////////////

    if (FCName == "FC")
    {
        BuildFCGeometry(logVolume, FCthickness);
    }
    else if (FCName == "Nexus")
    {
        BuildNexusGeometry(logVolume, FCthickness);
    }
    else if (FCName == "GFiber")
    {
        BuildGFiberGeometry(logVolume, FCthickness);
    }
    else if (FCName == "Solid")
    {
        BuildSolidGeometry(logVolume, FCthickness);
    }

//     ///////////////
//     ///////////////
//     ///////////////

    BuildDriftVolume(logVolume, driftVolume, FCthickness);
    BuildMicromegasVolume();

//     double layer1thickness = 0.05 * CLHEP::mm; //Katpon
//     double layer2thickness = 0.1 * CLHEP::mm;  // Copper
//     double layer3thickness = 2.0 * CLHEP::mm;  // Kevlar
//     double layer4thickness = 25.0 * CLHEP::mm; // Aramid HoneyComb
//     double layer5thickness = 2.0 * CLHEP::mm;  // Kevlar
//     double layer6thickness = 0.15 * CLHEP::mm; //Katpon
//     double layer7thickness = 0.1 * CLHEP::mm;  // Copper

//     std::setprecision(2);

//     G4cout << "\n------------------------------------------------------------"
//            << "\n\n The Field Cage Wall is made of: \n ";
//     G4cout << layer1thickness << "mm of " << FindMaterial("G4_KAPTON")->GetName() << " with a Radiation Length of " << (100 * layer1thickness) / FindMaterial("G4_KAPTON")->GetRadlen() << "%"
//                                                                                                                                                                                            "\n"
//            << " + ";
//     G4cout << layer2thickness << "mm of " << FindMaterial("Copper")->GetName() << " with a Radiation Length of " << (100 * layer2thickness) / FindMaterial("Copper")->GetRadlen() << "%"
//                                                                                                                                                                                      "\n"
//            << " + ";
//     G4cout << layer3thickness << "mm of " << FindMaterial("G4_KEVLAR")->GetName() << " with a Radiation Length of " << (100 * layer3thickness) / FindMaterial("G4_KEVLAR")->GetRadlen() << "%"
//                                                                                                                                                                                            "\n"
//            << " + ";
//     G4cout << layer4thickness << "mm of " << FindMaterial("NOMEX")->GetName() << " with a Radiation Length of " << (100 * layer4thickness) / FindMaterial("NOMEX")->GetRadlen() << "%"
//                                                                                                                                                                                    "\n"
//            << " + ";
//     G4cout << layer5thickness << "mm of " << FindMaterial("G4_KEVLAR")->GetName() << " with a Radiation Length of " << (100 * layer5thickness) / FindMaterial("G4_KEVLAR")->GetRadlen() << "%"
//                                                                                                                                                                                            "\n"
//            << " + ";
//     G4cout << layer6thickness << "mm of " << FindMaterial("G4_KAPTON")->GetName() << " with a Radiation Length of " << (100 * layer6thickness) / FindMaterial("G4_KAPTON")->GetRadlen() << "%"
//                                                                                                                                                                                            "\n"
//            << " + ";
//     G4cout << layer7thickness << "mm of " << FindMaterial("Copper")->GetName() << " with a Radiation Length of " << (100 * layer7thickness) / FindMaterial("Copper")->GetRadlen() << "%"
//                                                                                                                                                                                      "\n";

//     G4cout << "\n------------------------------------------------------------\n";

//     G4cout << " Kapton radiation Length of " << FindMaterial("G4_KAPTON")->GetRadlen() << " cm"
//                                                                                           "\n";
//     G4cout << " Copper radiation Length of " << FindMaterial("Copper")->GetRadlen() << " cm"
//                                                                                        "\n";
//     G4cout << " Kevlar radiation Length of " << FindMaterial("G4_KEVLAR")->GetRadlen() << " cm"
//                                                                                           "\n";
//     G4cout << " Nomex Honeycomb radiation Length of " << FindMaterial("NOMEX")->GetRadlen() << " cm"
//                                                                                                "\n";

//     // if(extragap3Material->GetName() != "Galactic"){
//     //   G4cout << extragap3Thickness/mm << "mm of " << extragap3Material->GetName() << " with a Radiation Length of " << (100*extragap3Thickness/mm)/extragap3Material->GetRadlen () << "%" "\n"
//     //          << " + "; }
//     // if(extragap4Material->GetName() != "Galactic"){
//     //   G4cout << extragap4Thickness/mm << "mm of " << extragap4Material->GetName() << " with a Radiation Length of " << (100*extragap4Thickness/mm)/extragap4Material->GetRadlen () << "%" "\n"
//     //          << " + "; }
//     // if(extragap5Material->GetName() != "Galactic"){
//     //   G4cout << extragap5Thickness/mm << "mm of " << extragap5Material->GetName() << " with a Radiation Length of " << (100*extragap5Thickness/mm)/extragap5Material->GetRadlen () << "%" "\n"
//     //          << " + "; }
//     // if(extragap6Material->GetName() != "Galactic"){
//     //   G4cout << extragap6Thickness/mm << "mm of " << extragap6Material->GetName() << " with a Radiation Length of " << (100*extragap6Thickness/mm)/extragap6Material->GetRadlen () << "%" "\n"
//     //          << " + "; }
//     // if(extragap7Material->GetName() != "Galactic"){
//     //   G4cout << extragap7Thickness/mm << "mm of " << extragap7Material->GetName() << " with a Radiation Length of " << (100*extragap7Thickness/mm)/extragap7Material->GetRadlen () << "%" "\n"
//     //          << " + "; }
//     // if(extragap8Material->GetName() != "Galactic"){
//     //   G4cout << extragap8Thickness/mm << "mm of " << extragap8Material->GetName() << " with a Radiation Length of " << (100*extragap8Thickness/mm)/extragap8Material->GetRadlen () << "%" << "\n"
//     //          << " + "; }
//     // if(extragap9Material->GetName() != "Galactic"){
//     //   G4cout << extragap9Thickness/mm << "mm of " << extragap9Material->GetName() << " with a Radiation Length of " << (100*extragap9Thickness/mm)/extragap9Material->GetRadlen () << "%"<< "\n"
//     //          << " + "; }
//     // if(extragap10Material->GetName() != "Galactic"){
//     //   G4cout << extragap10Thickness/mm << "mm of " << extragap10Material->GetName() << " with a Radiation Length of " << (100*extragap10Thickness/mm)/extragap10Material->GetRadlen () << "%" << "\n";}
//     G4cout << "\n------------------------------------------------------------\n";

    // BuildHATPCCentralCathode(driftVolume, FCthickness);
//     // BuildHATPCCages(logGasGap);

//     G4cout << "\n-----------------------------DONE-------------------------------\n";


    return logVolume;
}

void ND280HATPCConstructor::BuildHATPCCentralCathode(G4LogicalVolume *driftVolume, double FCthickness)
{

    //G4bool DebugHATPCMass = true;
    //G4bool fVisible= false;

    //double y, z;

    // G4String parentname = driftVolume->GetName();

    MyND280BeamConstructor middleG10RoPlate(GetHATPCParentName() + "/CentralCathode");
    middleG10RoPlate.SetWidth(GetHATPCCathodeThickness());
    middleG10RoPlate.SetHeight(GetHATPCHeight() - 2. * FCthickness);
    middleG10RoPlate.SetLength(GetHATPCLength() - 2. * FCthickness);
    middleG10RoPlate.SetMaterial(FindMaterial("G10Roha"));

    //double width  = middleG10RoPlate.GetWidth();
    //double height = middleG10RoPlate.GetHeight();
    //double length = middleG10RoPlate.GetLength();

    G4LogicalVolume *log_middleG10RoPlate = middleG10RoPlate.GetPiece();

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      log_middleG10RoPlate,
                      middleG10RoPlate.GetName(),
                      driftVolume,
                      false,
                      0);

    log_middleG10RoPlate->SetVisAttributes(G4Colour(0.0, 1.0, 1.0));

    /*  
    // (green) solid G10 connector

    MyND280BeamConstructor frbaCon1(parentname+"/FrBaCon1");
    frbaCon1.SetWidth(GetHATPCCathodeThickness());
    frbaCon1.SetHeight(700.0*CLHEP::mm);
    frbaCon1.SetLength(386.0*CLHEP::mm - 373.0*CLHEP::mm);

    //frbaCon1.SetMaterialName("G10");
    frbaCon1.SetMaterial(FindMaterial("G10"));

    width  = frbaCon1.GetWidth();
    height = frbaCon1.GetHeight();
    length = frbaCon1.GetLength();


    G4LogicalVolume* log_frbaCon1 = frbaCon1.GetPiece();

    int k = 0;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        z = d * (373.0*CLHEP::mm + length/2.);
        new G4PVPlacement(0,
                          G4ThreeVector(0,0,z),
                          log_frbaCon1,
                          frbaCon1.GetName(),
                          logVolume,
                          false,
                          k);
         k++;

    }

    log_frbaCon1->SetVisAttributes(G4Colour(1.0,0.0,1.0));
    log_frbaCon1->SetVisAttributes(G4VisAttributes::Invisible);

    MyND280BeamConstructor toboCon1(parentname+"/ToBoCon1");

    toboCon1.SetWidth(GetHATPCCathodeThickness());
    toboCon1.SetHeight(386.0*CLHEP::mm - 373.0*CLHEP::mm);
    toboCon1.SetLength(700*CLHEP::mm);

    toboCon1.SetMaterial(FindMaterial("G10"));

    width  = toboCon1.GetWidth();
    height = toboCon1.GetHeight();
    length = toboCon1.GetLength();

    G4LogicalVolume* log_toboCon1 = toboCon1.GetPiece();

    k = 0;
    for (int i=0; i<2; i++) {
        double d = pow(-1,i);
        y = d * (350.0*CLHEP::mm - height/2.);
        new G4PVPlacement(0,
                          G4ThreeVector(0,y,0),
                          log_toboCon1,
                          toboCon1.GetName(),
                          logVolume,
                          false,
                          k);
	k++;
    }
    
    if (DebugHATPCMass){
      G4cout << "CentralCathode (1) mass="<<log_middleG10RoPlate->GetMass()/CLHEP::kg<<" kg" << G4endl;
      G4cout << "FrBaCon1 (2) mass="<<log_frbaCon1->GetMass()/CLHEP::kg<<" kg" << G4endl;
      G4cout << "ToBoCon1 (2) mass="<<log_toboCon1->GetMass()/CLHEP::kg<<" kg" << G4endl;
      G4cout << "Sum of central cathode parts: "<<(log_middleG10RoPlate->GetMass()+2*log_frbaCon1->GetMass()+2*log_toboCon1->GetMass())/CLHEP::kg<<" kg" << G4endl;
    }
*/
    return;
}

void ND280HATPCConstructor::BuildFCGeometry(G4LogicalVolume *logVolume, double &FCthickness)
{
    double thickness;
    // double fBoxXsize, fBoxYsize, fBoxZsize;

    double layer1thickness = 0.05 * CLHEP::mm; //Katpon
    double layer2thickness = 0.1 * CLHEP::mm;  // Copper
    double layer3thickness = 2.0 * CLHEP::mm;  // Kevlar
    double layer4thickness = 25.0 * CLHEP::mm; // Aramid HoneyComb
    double layer5thickness = 2.0 * CLHEP::mm;  // Kevlar
    double layer6thickness = 0.15 * CLHEP::mm; //Katpon
    double layer7thickness = 0.1 * CLHEP::mm;  // Copper

    // Field Cage, definition and placement layer by layer.
    // FC

    FCthickness = layer1thickness + layer2thickness + layer3thickness + layer4thickness + layer5thickness + layer6thickness + layer7thickness;

    fBoxXsize = fHATPCWidth;
    fBoxYsize = fHATPCHeight;
    fBoxZsize = fHATPCLength;

    G4Box *outerFCBox = new G4Box("Outer FC Box", fBoxXsize / 2., fBoxYsize / 2., fBoxZsize / 2.);
    G4Box *innerFCBox = new G4Box("Inner FC Box", (fBoxXsize - 2. * FCthickness) / 2., (fBoxYsize - 2. * FCthickness) / 2., (fBoxZsize - 2. * FCthickness) / 2.);
    G4SubtractionSolid *solidFC = new G4SubtractionSolid("Hollow FC Box", outerFCBox, innerFCBox);

    G4LogicalVolume *logFC = new G4LogicalVolume(solidFC,
                                                 FindMaterial("Galactic"),
                                                 GetHATPCParentName() + "/FC/" + GetHATPCName());

    logFC->SetVisAttributes(G4Colour(1.0, 1.0, 0.0));

    cout << "The FC total thickness is:  " << FCthickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      logFC,
                      GetHATPCParentName() + "/FC/" + GetHATPCName(),
                      logVolume,
                      false,
                      0);

    // FC layer1

    thickness = layer1thickness;

    G4Box *outerFClayer1Box = new G4Box("Outer FC layer1 Box", fBoxXsize / 2., fBoxYsize / 2., fBoxZsize / 2.);
    G4Box *innerFClayer1Box = new G4Box("Inner FC layer1 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer1 = new G4SubtractionSolid("Hollow FC layer1 Box", outerFClayer1Box, innerFClayer1Box);

    G4LogicalVolume *loglayer1 = new G4LogicalVolume(solidlayer1,
                                                     FindMaterial("G4_KAPTON"),
                                                     GetHATPCParentName() + "/FC/layer1");

    loglayer1->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer1->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer1 thickness is:  " << layer1thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer1,
                      GetHATPCParentName() + "/FC/layer1",
                      logFC,
                      false,
                      0);

    // FC layer2

    thickness += layer2thickness;

    G4Box *innerFClayer2Box = new G4Box("Inner FC layer2 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer2 = new G4SubtractionSolid("Hollow FC layer2 Box", innerFClayer1Box, innerFClayer2Box);

    G4LogicalVolume *loglayer2 = new G4LogicalVolume(solidlayer2,
                                                     FindMaterial("Copper"),
                                                     GetHATPCParentName() + "/FC/layer2");

    loglayer2->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer2->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer2 thickness is:  " << layer2thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer2,
                      GetHATPCParentName() + "/FC/layer2",
                      logFC,
                      false,
                      0);

    // FC layer3

    thickness += layer3thickness;

    G4Box *innerFClayer3Box = new G4Box("Inner FC layer3 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer3 = new G4SubtractionSolid("Hollow FC layer3 Box", innerFClayer2Box, innerFClayer3Box);

    G4LogicalVolume *loglayer3 = new G4LogicalVolume(solidlayer3,
                                                     FindMaterial("G4_KEVLAR"),
                                                     GetHATPCParentName() + "/FC/layer3");

    loglayer3->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer3->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer3 thickness is:  " << layer3thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer3,
                      GetHATPCParentName() + "/FC/layer3",
                      logFC,
                      false,
                      0);

    // FC layer4

    thickness += layer4thickness;

    G4Box *innerFClayer4Box = new G4Box("Inner FC layer4 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer4 = new G4SubtractionSolid("Hollow FC layer4 Box", innerFClayer3Box, innerFClayer4Box);

    G4LogicalVolume *loglayer4 = new G4LogicalVolume(solidlayer4,
                                                     FindMaterial("NOMEX"),
                                                     GetHATPCParentName() + "/FC/layer4");

    loglayer4->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer4->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer4 thickness is:  " << layer4thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer4,
                      GetHATPCParentName() + "/FC/layer4",
                      logFC,
                      false,
                      0);

    // FC layer5

    thickness += layer5thickness;

    G4Box *innerFClayer5Box = new G4Box("Inner FC layer5 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer5 = new G4SubtractionSolid("Hollow FC layer5 Box", innerFClayer4Box, innerFClayer5Box);

    G4LogicalVolume *loglayer5 = new G4LogicalVolume(solidlayer5,
                                                     FindMaterial("G4_KEVLAR"),
                                                     GetHATPCParentName() + "/FC/layer5");

    loglayer5->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer5->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer5 thickness is:  " << layer5thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer5,
                      GetHATPCParentName() + "/FC/layer5",
                      logFC,
                      false,
                      0);

    // FC layer6

    thickness += layer6thickness;

    G4Box *innerFClayer6Box = new G4Box("Inner FC layer6 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer6 = new G4SubtractionSolid("Hollow FC layer6 Box", innerFClayer5Box, innerFClayer6Box);

    G4LogicalVolume *loglayer6 = new G4LogicalVolume(solidlayer6,
                                                     FindMaterial("G4_KAPTON"),
                                                     GetHATPCParentName() + "/FC/layer6");

    loglayer6->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer6->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer6 thickness is:  " << layer6thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer6,
                      GetHATPCParentName() + "/FC/layer6",
                      logFC,
                      false,
                      0);

    // FC layer7

    thickness += layer7thickness;

    G4Box *innerFClayer7Box = new G4Box("Inner FC layer7 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer7 = new G4SubtractionSolid("Hollow FC layer7 Box", innerFClayer6Box, innerFClayer7Box);

    G4LogicalVolume *loglayer7 = new G4LogicalVolume(solidlayer7,
                                                     FindMaterial("Copper"),
                                                     GetHATPCParentName() + "/FC/layer7");

    loglayer7->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer7->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer7 thickness is:  " << layer7thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer7,
                      GetHATPCParentName() + "/FC/layer7",
                      logFC,
                      false,
                      0);
    return;
}

void ND280HATPCConstructor::BuildNexusGeometry(G4LogicalVolume *logVolume, double &FCthickness)
{
    double thickness;
    // double FCthickness;
    // double fBoxXsize, fBoxYsize, fBoxZsize;

    double layer1thickness = 0.7 * CLHEP::mm;  // Glue
    double layer2thickness = 0.25 * CLHEP::mm; // Kapton
    double layer3thickness = 10.0 * CLHEP::mm; // Aramid HoneyComb
    double layer4thickness = 1.2 * CLHEP::mm;  // CarbonFiber
    double layer5thickness = 16.5 * CLHEP::mm; // Aramid HoneyComb
    double layer6thickness = 1.2 * CLHEP::mm;  // CarbonFiber
    double layer7thickness = 0.05 * CLHEP::mm; //Katpon
    double layer8thickness = 0.01 * CLHEP::mm; // Copper

    // Field Cage, definition and placement layer by layer.
    // FC

    FCthickness = layer1thickness + layer2thickness + layer3thickness + layer4thickness + layer5thickness + layer6thickness + layer7thickness + layer8thickness;

    fBoxXsize = fHATPCWidth;
    fBoxYsize = fHATPCHeight;
    fBoxZsize = fHATPCLength;

    G4Box *outerFCBox = new G4Box("Outer FC Box", fBoxXsize / 2., fBoxYsize / 2., fBoxZsize / 2.);
    G4Box *innerFCBox = new G4Box("Inner FC Box", (fBoxXsize - 2. * FCthickness) / 2., (fBoxYsize - 2. * FCthickness) / 2., (fBoxZsize - 2. * FCthickness) / 2.);
    G4SubtractionSolid *solidFC = new G4SubtractionSolid("Hollow FC Box", outerFCBox, innerFCBox);

    G4LogicalVolume *logFC = new G4LogicalVolume(solidFC,
                                                 FindMaterial("Galactic"),
                                                 GetHATPCParentName() + "/" + GetHATPCName() + "/FC");

    logFC->SetVisAttributes(G4Colour(1.0, 1.0, 0.0));

    cout << "The FC total thickness is:  " << FCthickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      logFC,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC",
                      logVolume,
                      false,
                      0);

    // Nexus layer1

    thickness = layer1thickness;

    G4Box *outerFClayer1Box = new G4Box("Outer FC layer1 Box", fBoxXsize / 2., fBoxYsize / 2., fBoxZsize / 2.);
    G4Box *innerFClayer1Box = new G4Box("Inner FC layer1 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer1 = new G4SubtractionSolid("Hollow FC layer1 Box", outerFClayer1Box, innerFClayer1Box);

    G4LogicalVolume *loglayer1 = new G4LogicalVolume(solidlayer1,
                                                     FindMaterial("G4_POLYVINYL_ACETATE"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "Nexus/layer1");

    loglayer1->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer1->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer1 thickness is:  " << layer1thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer1,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "Nexus/layer1",
                      logFC,
                      false,
                      0);

    // Nexus layer2

    thickness += layer2thickness;

    G4Box *innerFClayer2Box = new G4Box("Inner FC layer2 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer2 = new G4SubtractionSolid("Hollow FC layer2 Box", innerFClayer1Box, innerFClayer2Box);

    G4LogicalVolume *loglayer2 = new G4LogicalVolume(solidlayer2,
                                                     FindMaterial("G4_KAPTON"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "Nexus/layer2");

    loglayer2->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer2->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer2 thickness is:  " << layer2thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer2,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "Nexus/layer2",
                      logFC,
                      false,
                      0);

    // Nexus layer3

    thickness += layer3thickness;

    G4Box *innerFClayer3Box = new G4Box("Inner FC layer3 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer3 = new G4SubtractionSolid("Hollow FC layer3 Box", innerFClayer2Box, innerFClayer3Box);

    G4LogicalVolume *loglayer3 = new G4LogicalVolume(solidlayer3,
                                                     FindMaterial("AramidHoneycomb"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "Nexus/layer3");

    loglayer3->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer3->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer3 thickness is:  " << layer3thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer3,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "Nexus/layer3",
                      logFC,
                      false,
                      0);

    // Nexus layer4

    thickness += layer4thickness;

    G4Box *innerFClayer4Box = new G4Box("Inner FC layer4 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer4 = new G4SubtractionSolid("Hollow FC layer4 Box", innerFClayer3Box, innerFClayer4Box);

    G4LogicalVolume *loglayer4 = new G4LogicalVolume(solidlayer4,
                                                     FindMaterial("CarbonFiber"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "Nexus/layer4");

    loglayer4->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer4->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer4 thickness is:  " << layer4thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer4,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "Nexus/layer4",
                      logFC,
                      false,
                      0);

    // Nexus layer5

    thickness += layer5thickness;

    G4Box *innerFClayer5Box = new G4Box("Inner FC layer5 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer5 = new G4SubtractionSolid("Hollow FC layer5 Box", innerFClayer4Box, innerFClayer5Box);

    G4LogicalVolume *loglayer5 = new G4LogicalVolume(solidlayer5,
                                                     FindMaterial("AramidHoneycomb"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "Nexus/layer5");

    loglayer5->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer5->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer5 thickness is:  " << layer5thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer5,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "Nexus/layer5",
                      logFC,
                      false,
                      0);

    // Nexus layer6

    thickness += layer6thickness;

    G4Box *innerFClayer6Box = new G4Box("Inner FC layer6 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer6 = new G4SubtractionSolid("Hollow FC layer6 Box", innerFClayer5Box, innerFClayer6Box);

    G4LogicalVolume *loglayer6 = new G4LogicalVolume(solidlayer6,
                                                     FindMaterial("CarbonFiber"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "Nexus/layer6");

    loglayer6->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer6->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer6 thickness is:  " << layer6thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer6,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "Nexus/layer6",
                      logFC,
                      false,
                      0);

    // Nexus layer7

    thickness += layer7thickness;

    G4Box *innerFClayer7Box = new G4Box("Inner FC layer7 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer7 = new G4SubtractionSolid("Hollow FC layer7 Box", innerFClayer6Box, innerFClayer7Box);

    G4LogicalVolume *loglayer7 = new G4LogicalVolume(solidlayer7,
                                                     FindMaterial("G4_KAPTON"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "Nexus/layer7");

    loglayer7->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer7->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer7 thickness is:  " << layer7thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer7,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "Nexus/layer7",
                      logFC,
                      false,
                      0);

    // Nexus layer8

    thickness += layer8thickness;

    G4Box *innerFClayer8Box = new G4Box("Inner FC layer8 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer8 = new G4SubtractionSolid("Hollow FC layer8 Box", innerFClayer7Box, innerFClayer8Box);

    G4LogicalVolume *loglayer8 = new G4LogicalVolume(solidlayer8,
                                                     FindMaterial("Copper"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "Nexus/layer8");

    loglayer8->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer8->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer8 thickness is:  " << layer8thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer8,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "Nexus/layer8",
                      logFC,
                      false,
                      0);
    return;
}

void ND280HATPCConstructor::BuildSolidGeometry(G4LogicalVolume *logVolume, double &FCthickness)
{
    double thickness;
    // double FCthickness;
    // double fBoxXsize, fBoxYsize, fBoxZsize;

    double layer1thickness = 0.7 * CLHEP::mm;  // Glue
    double layer2thickness = 8 * CLHEP::mm;    // Aramid (No Honeycomb)
    double layer3thickness = 15.0 * CLHEP::mm; // Aramid HoneyComb
    double layer4thickness = 0.25 * CLHEP::mm; // Kapton
    double layer5thickness = 0.01 * CLHEP::mm; // Copper

    // Field Cage, definition and placement layer by layer.
    // FC

    FCthickness = layer1thickness + layer2thickness + layer3thickness + layer4thickness + layer5thickness;

    fBoxXsize = fHATPCWidth;
    fBoxYsize = fHATPCHeight;
    fBoxZsize = fHATPCLength;

    G4Box *outerFCBox = new G4Box("Outer FC Box", fBoxXsize / 2., fBoxYsize / 2., fBoxZsize / 2.);
    G4Box *innerFCBox = new G4Box("Inner FC Box", (fBoxXsize - 2. * FCthickness) / 2., (fBoxYsize - 2. * FCthickness) / 2., (fBoxZsize - 2. * FCthickness) / 2.);
    G4SubtractionSolid *solidFC = new G4SubtractionSolid("Hollow FC Box", outerFCBox, innerFCBox);

    G4LogicalVolume *logFC = new G4LogicalVolume(solidFC,
                                                 FindMaterial("Galactic"),
                                                 GetHATPCParentName() + "/" + GetHATPCName() + "/FC");

    logFC->SetVisAttributes(G4Colour(1.0, 1.0, 0.0));

    cout << "The FC total thickness is:  " << FCthickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      logFC,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC",
                      logVolume,
                      false,
                      0);

    // Solid layer1

    thickness = layer1thickness;

    G4Box *outerFClayer1Box = new G4Box("Outer FC layer1 Box", fBoxXsize / 2., fBoxYsize / 2., fBoxZsize / 2.);
    G4Box *innerFClayer1Box = new G4Box("Inner FC layer1 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer1 = new G4SubtractionSolid("Hollow FC layer1 Box", outerFClayer1Box, innerFClayer1Box);

    G4LogicalVolume *loglayer1 = new G4LogicalVolume(solidlayer1,
                                                     FindMaterial("G4_POLYVINYL_ACETATE"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "Solid/layer1");

    loglayer1->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer1->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer1 thickness is:  " << layer1thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer1,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "Solid/layer1",
                      logFC,
                      false,
                      0);

    // Solid layer2

    thickness += layer2thickness;

    G4Box *innerFClayer2Box = new G4Box("Inner FC layer2 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer2 = new G4SubtractionSolid("Hollow FC layer2 Box", innerFClayer1Box, innerFClayer2Box);

    G4LogicalVolume *loglayer2 = new G4LogicalVolume(solidlayer2,
                                                     FindMaterial("G4_KEVLAR"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "Solid/layer2");

    loglayer2->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer2->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer2 thickness is:  " << layer2thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer2,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "Solid/layer2",
                      logFC,
                      false,
                      0);

    // Solid layer3

    thickness += layer3thickness;

    G4Box *innerFClayer3Box = new G4Box("Inner FC layer3 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer3 = new G4SubtractionSolid("Hollow FC layer3 Box", innerFClayer2Box, innerFClayer3Box);

    G4LogicalVolume *loglayer3 = new G4LogicalVolume(solidlayer3,
                                                     FindMaterial("AramidHoneycomb"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "Solid/layer3");

    loglayer3->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer3->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer3 thickness is:  " << layer3thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer3,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "Solid/layer3",
                      logFC,
                      false,
                      0);

    // Solid layer4

    thickness += layer4thickness;

    G4Box *innerFClayer4Box = new G4Box("Inner FC layer4 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer4 = new G4SubtractionSolid("Hollow FC layer4 Box", innerFClayer3Box, innerFClayer4Box);

    G4LogicalVolume *loglayer4 = new G4LogicalVolume(solidlayer4,
                                                     FindMaterial("G4_KAPTON"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "Solid/layer4");

    loglayer4->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer4->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer4 thickness is:  " << layer4thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer4,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "Solid/layer4",
                      logFC,
                      false,
                      0);

    // Solid layer5

    thickness += layer5thickness;

    G4Box *innerFClayer5Box = new G4Box("Inner FC layer5 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer5 = new G4SubtractionSolid("Hollow FC layer5 Box", innerFClayer4Box, innerFClayer5Box);

    G4LogicalVolume *loglayer5 = new G4LogicalVolume(solidlayer5,
                                                     FindMaterial("Copper"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "Solid/layer5");

    loglayer5->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer5->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer5 thickness is:  " << layer5thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer5,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "Solid/layer5",
                      logFC,
                      false,
                      0);
    return;
}

void ND280HATPCConstructor::BuildGFiberGeometry(G4LogicalVolume *logVolume, double &FCthickness)
{
    double thickness;
    // double FCthickness;
    // double fBoxXsize, fBoxYsize, fBoxZsize;

    double layer1thickness = 0.7 * CLHEP::mm;  // Glue
    double layer2thickness = 0.25 * CLHEP::mm; // Kapton
    double layer3thickness = 1.2 * CLHEP::mm;  // GlassFiber
    double layer4thickness = 20.0 * CLHEP::mm; // Aramid HoneyComb
    double layer5thickness = 1.2 * CLHEP::mm;  // GlassFiber
    double layer6thickness = 0.05 * CLHEP::mm; // Kapton
    double layer7thickness = 0.01 * CLHEP::mm; //Copper

    // Field Cage, definition and placement layer by layer.
    // FC

    FCthickness = layer1thickness + layer2thickness + layer3thickness + layer4thickness + layer5thickness + layer6thickness + layer7thickness;

    fBoxXsize = fHATPCWidth;
    fBoxYsize = fHATPCHeight;
    fBoxZsize = fHATPCLength;

    G4Box *outerFCBox = new G4Box("Outer FC Box", fBoxXsize / 2., fBoxYsize / 2., fBoxZsize / 2.);
    G4Box *innerFCBox = new G4Box("Inner FC Box", (fBoxXsize - 2. * FCthickness) / 2., (fBoxYsize - 2. * FCthickness) / 2., (fBoxZsize - 2. * FCthickness) / 2.);
    G4SubtractionSolid *solidFC = new G4SubtractionSolid("Hollow FC Box", outerFCBox, innerFCBox);

    G4LogicalVolume *logFC = new G4LogicalVolume(solidFC,
                                                 FindMaterial("Galactic"),
                                                 GetHATPCParentName() + "/" + GetHATPCName() + "/FC");

    logFC->SetVisAttributes(G4Colour(1.0, 1.0, 0.0));

    cout << "The FC total thickness is:  " << FCthickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      logFC,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC",
                      logVolume,
                      false,
                      0);

    // GF layer1

    thickness = layer1thickness;

    G4Box *outerFClayer1Box = new G4Box("Outer FC layer1 Box", fBoxXsize / 2., fBoxYsize / 2., fBoxZsize / 2.);
    G4Box *innerFClayer1Box = new G4Box("Inner FC layer1 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer1 = new G4SubtractionSolid("Hollow FC layer1 Box", outerFClayer1Box, innerFClayer1Box);

    G4LogicalVolume *loglayer1 = new G4LogicalVolume(solidlayer1,
                                                     FindMaterial("G4_POLYVINYL_ACETATE"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "GF/layer1");

    loglayer1->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer1->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer1 thickness is:  " << layer1thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer1,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "GF/layer1",
                      logFC,
                      false,
                      0);

    // GF layer2

    thickness += layer2thickness;

    G4Box *innerFClayer2Box = new G4Box("Inner FC layer2 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer2 = new G4SubtractionSolid("Hollow FC layer2 Box", innerFClayer1Box, innerFClayer2Box);

    G4LogicalVolume *loglayer2 = new G4LogicalVolume(solidlayer2,
                                                     FindMaterial("G4_KAPTON"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "GF/layer2");

    loglayer2->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer2->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer2 thickness is:  " << layer2thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer2,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "GF/layer2",
                      logFC,
                      false,
                      0);

    // GF layer3

    thickness += layer3thickness;

    G4Box *innerFClayer3Box = new G4Box("Inner FC layer3 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer3 = new G4SubtractionSolid("Hollow FC layer3 Box", innerFClayer2Box, innerFClayer3Box);

    G4LogicalVolume *loglayer3 = new G4LogicalVolume(solidlayer3,
                                                     FindMaterial("GlassFiber"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "GF/layer3");

    loglayer3->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer3->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer3 thickness is:  " << layer3thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer3,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "GF/layer3",
                      logFC,
                      false,
                      0);

    // GF layer4

    thickness += layer4thickness;

    G4Box *innerFClayer4Box = new G4Box("Inner FC layer4 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer4 = new G4SubtractionSolid("Hollow FC layer4 Box", innerFClayer3Box, innerFClayer4Box);

    G4LogicalVolume *loglayer4 = new G4LogicalVolume(solidlayer4,
                                                     FindMaterial("AramidHoneycomb"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "GF/layer4");

    loglayer4->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer4->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer4 thickness is:  " << layer4thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer4,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "GF/layer4",
                      logFC,
                      false,
                      0);

    // GF layer5

    thickness += layer5thickness;

    G4Box *innerFClayer5Box = new G4Box("Inner FC layer5 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer5 = new G4SubtractionSolid("Hollow FC layer5 Box", innerFClayer4Box, innerFClayer5Box);

    G4LogicalVolume *loglayer5 = new G4LogicalVolume(solidlayer5,
                                                     FindMaterial("GlassFiber"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "GF/layer5");

    loglayer5->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer5->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer5 thickness is:  " << layer5thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer5,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "GF/layer5",
                      logFC,
                      false,
                      0);

    // GF layer6

    thickness += layer6thickness;

    G4Box *innerFClayer6Box = new G4Box("Inner FC layer6 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer6 = new G4SubtractionSolid("Hollow FC layer6 Box", innerFClayer5Box, innerFClayer6Box);

    G4LogicalVolume *loglayer6 = new G4LogicalVolume(solidlayer6,
                                                     FindMaterial("G4_KAPTON"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "GF/layer6");

    loglayer6->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer6->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer6 thickness is:  " << layer6thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer6,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "GF/layer6",
                      logFC,
                      false,
                      0);

    // GF layer7

    thickness += layer7thickness;

    G4Box *innerFClayer7Box = new G4Box("Inner FC layer7 Box", (fBoxXsize - 2. * thickness) / 2., (fBoxYsize - 2. * thickness) / 2., (fBoxZsize - 2. * thickness) / 2.);
    G4SubtractionSolid *solidlayer7 = new G4SubtractionSolid("Hollow FC layer7 Box", innerFClayer6Box, innerFClayer7Box);

    G4LogicalVolume *loglayer7 = new G4LogicalVolume(solidlayer7,
                                                     FindMaterial("Copper"),
                                                     GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                                                   "GF/layer7");

    loglayer7->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    if (fVisible != true)
        loglayer7->SetVisAttributes(G4VisAttributes::Invisible);

    cout << "The FC layer7 thickness is:  " << layer7thickness << endl;

    new G4PVPlacement(0,
                      G4ThreeVector(0, 0, 0),
                      loglayer7,
                      GetHATPCParentName() + "/" + GetHATPCName() + "/FC"
                                                                    "GF/layer7",
                      logFC,
                      false,
                      0);
    return;
}

void ND280HATPCConstructor::BuildDriftVolume(G4LogicalVolume *logVolume, G4LogicalVolume *driftVolume, double &FCthickness)
{

    //Drift Volume

    G4VSolid *solidDrift = new G4Box("InnerBox", (fBoxXsize - 2 * FCthickness) / 2., (fBoxYsize - 2 * FCthickness) / 2., (fBoxZsize - 2 * FCthickness) / 2.);

    driftVolume = new G4LogicalVolume(solidDrift,
                                      FindMaterial("GasMixtureTPC"),
                                      GetHATPCParentName() + "/FC/" + GetHATPCName() + "/Drift");

    driftVolume->SetVisAttributes(G4Colour(0.0, 0.0, 1.0));

    fActiveHATPCVerticalOffset = 0;

    new G4PVPlacement(0,
                      G4ThreeVector(0, fActiveHATPCVerticalOffset, 0),
                      driftVolume,
                      GetHATPCParentName() + "/FC/" + GetHATPCName() + "/Drift",
                      logVolume,
                      false,
                      0);

    // Divide drift volume into two halves (readout planes)
    // Make 2 separate halves (instead of 2 copies), so that each of 24 MM can be modified.

    double width = (fBoxXsize - 2 * FCthickness) / 2.;

    width -= GetHATPCCathodeThickness() / 2.;

    G4VSolid *innerHalf = new G4Box("InnerHalf", (fBoxZsize - 2 * FCthickness) / 2., (fBoxYsize - 2 * FCthickness) / 2, width / 2.);

    G4LogicalVolume *logHalf0 = new G4LogicalVolume(innerHalf,
                                                    FindMaterial("GasMixtureTPC"),
                                                    GetHATPCParentName() + "/FC/" + GetHATPCName() + "/Drift" + "/Half0");
    G4LogicalVolume *logHalf1 = new G4LogicalVolume(innerHalf,
                                                    FindMaterial("GasMixtureTPC"),
                                                    GetHATPCParentName() + "/FC/" + GetHATPCName() + "/Drift" + "/Half1");
    logHalf0->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));
    logHalf1->SetVisAttributes(G4Colour(1.0, 0.0, 0.0));

    //logHalf0->SetUserLimits(new G4UserLimits(GetHATPCSteppingLimit()));
    //logHalf1->SetUserLimits(new G4UserLimits(GetHATPCSteppingLimit()));

    double delta = width / 2. + GetHATPCCathodeThickness() / 2.;

    G4RotationMatrix rm;
    rm.rotateY(90.0 * CLHEP::deg);

    new G4PVPlacement(G4Transform3D(rm, G4ThreeVector(-delta, 0, 0)),
                      logHalf0,
                      GetHATPCParentName() + "/FC/" + GetHATPCName() + "/Drift" + "/Half0",
                      driftVolume,
                      false,
                      0);

    rm.rotateY(180.0 * CLHEP::deg);

    new G4PVPlacement(G4Transform3D(rm, G4ThreeVector(delta, 0, 0)),
                      logHalf1,
                      GetHATPCParentName() + "/FC/" + GetHATPCName() + "/Drift" + "/Half1",
                      driftVolume,
                      false,
                      1);

    return;
}

void ND280HATPCConstructor::BuildMicromegasVolume()
{

    // add 12 Micromegas Modules to readout-plane

    /*  
  double mmWidth  = 38*CLHEP::cm;
  double mmHeight = 33.61*CLHEP::cm;
  double mmLength = width;
  
  G4LogicalVolume* logMM
    = new G4LogicalVolume(new G4Box("MicromegasModule",
				    mmWidth/2.,
				    mmHeight/2.,
				    mmLength/2.),
			  FindMaterial("GasMixtureTPC"),
			  parentname+"/"+name+"/MM");
  

  G4VisAttributes* visual = new G4VisAttributes();
  visual->SetColor(0.1,0.6,0.1,1);
  logMM->SetVisAttributes(visual);

  // Initialize all translations and rotations to 0.
  for (int rp = 0; rp < 2; rp++)
    {
      for (int imm = 0; imm < 12; imm++)
	{
	  tpcMMTrans[rp][imm] = G4ThreeVector(0., 0., 0.);
	  tpcMMRot[rp][imm] = new G4RotationMatrix (G4ThreeVector(0, 0, 1), 0);
	}
    }
  // Define the rotation axis to +x in TPC coordinate system (+z in the interface).
  tpcMMRotAxis = G4ThreeVector(0, 0, 1);

  int nmod = 8;
  double xMM = 208*CLHEP::mm;
  double yMM = 173*CLHEP::mm;  
  double xmod[8] = {  3*xMM ,  3*xMM ,  xMM , xMM ,
		       -xMM ,   -xMM ,  -3*xMM , -3*xMM };
  
  double ymod[8] = {  yMM ,  -yMM ,  yMM ,  -yMM ,
		      yMM ,  -yMM ,  yMM ,  -yMM  };
  
  // Translations and rotations get applied in this version
  // Placement in first readout plane
  for (int imod=0; imod<nmod; imod++ ) {
    
    if ( imod < 4 ) {
      new G4PVPlacement(tpcMMRot[0][imod],
			G4ThreeVector(xmod[imod]-tpcMMTrans[0][imod].z(),ymod[imod]+tpcMMTrans[0][imod].y(), 0.),
			logMM,
			//GetName()+"/MM",
			//"TPC1/MM",
			parentname+"/"+name+"/MM",
			logHalf0,
			false,
			imod);
    }
    else {
      tpcMMRot[0][imod]->rotateZ(180.0*CLHEP::deg);
      new G4PVPlacement(tpcMMRot[0][imod],
			G4ThreeVector(xmod[imod]-tpcMMTrans[0][imod].z(),ymod[imod]+tpcMMTrans[0][imod].y(),0.),
			logMM,
			//GetName()+"/MM",
			//"TPC1/MM",
			parentname+"/"+name+"/MM",
			logHalf0,
			false,
			imod);
    }
  }
  // Placement in second readout plane
  for (int imod=0; imod<nmod; imod++ ) {
    
    if ( imod < 4 ) {
      new G4PVPlacement(tpcMMRot[1][imod],
			G4ThreeVector(xmod[imod]+tpcMMTrans[1][imod].z(),ymod[imod]+tpcMMTrans[1][imod].y(),0.),
			logMM,
			//GetName()+"/MM",
			//"TPC1/MM",
			parentname+"/"+name+"/MM",
			logHalf1,
			false,
			imod);
    }
    else {
      tpcMMRot[1][imod]->rotateZ(180.0*CLHEP::deg);
      new G4PVPlacement(tpcMMRot[1][imod],
			G4ThreeVector(xmod[imod]+tpcMMTrans[1][imod].z(),ymod[imod]+tpcMMTrans[1][imod].y(),0.),
			logMM,
			//GetName()+"/MM",
			//"TPC1/MM",
			parentname+"/"+name+"/MM",
			logHalf1,
			false,
			imod);
    }
  }
*/
    return;
}