#include <globals.hh>
#include <G4Box.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4PVReplica.hh>
#include "ND280HATPCConstructor.hh"
#include <MyND280BeamConstructor.hh>

//
// Cesar Jesus 19/3/18
//
// Constructor of the HATPC detectors
//

ND280HATPCConstructor::~ND280HATPCConstructor() {;}

void ND280HATPCConstructor::Init(void) {

  fHATPCWidth = 1*CLHEP::cm;
  fHATPCHeight = 1*CLHEP::cm;
  fHATPCLength = 1*CLHEP::cm; 

  fHATPCDriftWidth = 1*CLHEP::cm;
  fHATPCDriftHeight = 1*CLHEP::cm;
  fHATPCDriftLength = 1*CLHEP::cm;

  fHATPCCathodeThickness = 1*CLHEP::cm;
  fHATPCInnerBoxWall = 1*CLHEP::cm;
  fHATPCOuterBoxWall = 1*CLHEP::cm;

  fHATPCSteppingLimit = 1*CLHEP::cm;
  fActiveHATPCVerticalOffset = 1*CLHEP::cm;

}

G4LogicalVolume *ND280HATPCConstructor::GetPiece(void) {

  G4bool DebugHATPCMass = true;
  G4bool Vis = false;

// Specifications.

  G4String parentname = GetHATPCParentName();
  G4String name = GetHATPCName();

  double HATPCwidth  = GetHATPCWidth()/2.;
  double HATPCheight = GetHATPCHeight()/2.;
  double HATPClength = GetHATPCLength()/2.;

  cout << "HATPCwidht is: " << HATPCwidth << endl;
  cout << "HATPCheight is: " << HATPCwidth << endl;
  cout << "HATPClength is: " << HATPCwidth << endl;


  double layer1thickness = 10*CLHEP::mm;
  double layer2thickness = 10*CLHEP::mm;
  double layer3thickness = 10*CLHEP::mm;
  double layer4thickness = 10*CLHEP::mm;
  double layer5thickness = 10*CLHEP::mm;
  double layer6thickness = 10*CLHEP::mm;
  double layer7thickness = 10*CLHEP::mm;

  double thickness;

// Only logVolume for the HATPC.
// No G4PVPlacement for logVolume in HATPCConstructor. 
// The HATPCs are placed in DetectorConstruction (Up and Down).

  G4LogicalVolume* logVolume
    = new G4LogicalVolume(new G4Box(parentname+"/"+name,
				    HATPCwidth,
				    HATPCheight,
				    HATPClength),
			            FindMaterial("Air"),
			            parentname+"/"+name);
  
  logVolume->SetVisAttributes(G4Colour(1.0,1.0,0.0));


// Field Cage, definition and placement layer by layer.

// layer 1

  thickness = layer1thickness;

  G4VSolid* solidFClayer1 = new G4Box("FClayer1",HATPCwidth-thickness, HATPCheight-thickness, HATPClength-thickness);
  
  G4LogicalVolume* logFClayer1
    = new G4LogicalVolume(solidFClayer1,
			  FindMaterial("CO2"),
			  parentname+"/"+name+"/FClayer1");

  logFClayer1->SetVisAttributes(G4Colour(1.0,0.0,0.0));
  if(Vis == false) logFClayer1->SetVisAttributes(G4VisAttributes::Invisible);

  cout << "The FClayer1 thickness is:  " << layer1thickness << endl;

  new G4PVPlacement(0,
		    G4ThreeVector(0,0,0),
		    logFClayer1,
		    parentname+"/"+name+"/FClayer1",
		    logVolume,
		    false,
		    0);

// layer 2

  thickness += layer2thickness;

  G4VSolid* solidFClayer2 = new G4Box("FClayer2",HATPCwidth-thickness, HATPCheight-thickness, HATPClength-thickness);
  
  G4LogicalVolume* logFClayer2
    = new G4LogicalVolume(solidFClayer2,
			  FindMaterial("CO2"),
			  parentname+"/"+name+"/FClayer2");

  logFClayer2->SetVisAttributes(G4Colour(0.0,1.0,0.0));
  if(Vis == false) logFClayer2->SetVisAttributes(G4VisAttributes::Invisible);

  cout << "The FClayer2 thickness is:  " << layer2thickness << endl;

  new G4PVPlacement(0,
		    G4ThreeVector(0,0,0),
		    logFClayer2,
		    parentname+"/"+name+"/FClayer2",
		    logFClayer1,
		    false,
		    0);

// layer 3

  thickness += layer3thickness;

  G4VSolid* solidFClayer3 = new G4Box("FClayer3",HATPCwidth-thickness, HATPCheight-thickness, HATPClength-thickness);
  
  G4LogicalVolume* logFClayer3
    = new G4LogicalVolume(solidFClayer3,
			  FindMaterial("CO2"),
			  parentname+"/"+name+"/FClayer3");

  logFClayer3->SetVisAttributes(G4Colour(0.0,0.0,1.0));
  if(Vis == false) logFClayer3->SetVisAttributes(G4VisAttributes::Invisible);

  cout << "The FClayer3 thickness is:  " << layer3thickness << endl;

  new G4PVPlacement(0,
		    G4ThreeVector(0,0,0),
		    logFClayer3,
		    parentname+"/"+name+"/FClayer3",
		    logFClayer2,
		    false,
		    0);

// layer 4

  thickness += layer4thickness;

  G4VSolid* solidFClayer4 = new G4Box("FClayer4",HATPCwidth-thickness, HATPCheight-thickness, HATPClength-thickness);
  
  G4LogicalVolume* logFClayer4
    = new G4LogicalVolume(solidFClayer4,
			  FindMaterial("CO2"),
			  parentname+"/"+name+"/FClayer4");

  logFClayer4->SetVisAttributes(G4Colour(1.0,0.0,0.0));
  if(Vis == false) logFClayer4->SetVisAttributes(G4VisAttributes::Invisible);

  cout << "The FClayer4 thickness is:  " << layer4thickness << endl;

  new G4PVPlacement(0,
		    G4ThreeVector(0,0,0),
		    logFClayer4,
		    parentname+"/"+name+"/FClayer4",
		    logFClayer3,
		    false,
		    0);

// layer 5

  thickness += layer5thickness;

  G4VSolid* solidFClayer5 = new G4Box("FClayer5",HATPCwidth-thickness, HATPCheight-thickness, HATPClength-thickness);
  
  G4LogicalVolume* logFClayer5
    = new G4LogicalVolume(solidFClayer5,
			  FindMaterial("CO2"),
			  parentname+"/"+name+"/FClayer5");

  logFClayer5->SetVisAttributes(G4Colour(0.0,1.0,0.0));
  if(Vis == false) logFClayer5->SetVisAttributes(G4VisAttributes::Invisible);

  cout << "The FClayer5 thickness is:  " << layer5thickness << endl;

  new G4PVPlacement(0,
		    G4ThreeVector(0,0,0),
		    logFClayer5,
		    parentname+"/"+name+"/FClayer5",
		    logFClayer4,
		    false,
		    0);

// layer 6

  thickness += layer6thickness;

  G4VSolid* solidFClayer6 = new G4Box("FClayer6",HATPCwidth-thickness, HATPCheight-thickness, HATPClength-thickness);
  
  G4LogicalVolume* logFClayer6
    = new G4LogicalVolume(solidFClayer6,
			  FindMaterial("CO2"),
			  parentname+"/"+name+"/FClayer6");

  logFClayer6->SetVisAttributes(G4Colour(0.0,0.0,1.0));
  if(Vis == false) logFClayer6->SetVisAttributes(G4VisAttributes::Invisible);

  cout << "The FClayer6 thickness is:  " << layer6thickness << endl;

  new G4PVPlacement(0,
		    G4ThreeVector(0,0,0),
		    logFClayer6,
		    parentname+"/"+name+"/FClayer6",
		    logFClayer5,
		    false,
		    0);

  thickness += layer7thickness;

  G4VSolid* solidFClayer7 = new G4Box("FClayer7",HATPCwidth-thickness, HATPCheight-thickness, HATPClength-thickness);
  
  G4LogicalVolume* logFClayer7
    = new G4LogicalVolume(solidFClayer7,
			  FindMaterial("CO2"),
			  parentname+"/"+name+"/FClayer7");

  logFClayer7->SetVisAttributes(G4Colour(1.0,0.0,0.0));
  if(Vis == false) logFClayer7->SetVisAttributes(G4VisAttributes::Invisible);

  cout << "The FClayer7 thickness is:  " << layer7thickness << endl;

  new G4PVPlacement(0,
		    G4ThreeVector(0,0,0),
		    logFClayer7,
		    parentname+"/"+name+"/FClayer7",
		    logFClayer6,
		    false,
		    0);

// HATPC drift volume

  double width  = GetHATPCDriftWidth()/2.;
  double height = GetHATPCDriftHeight()/2.;
  double length = GetHATPCDriftLength()/2.;

  G4VSolid* solidDrift = new G4Box("InnerBox",width,height,length);
  
  G4LogicalVolume* logDrift
    = new G4LogicalVolume(solidDrift,
			  FindMaterial("GasMixtureTPC"),
			  parentname+"/"+name+"/Drift");
  
  logDrift->SetVisAttributes(G4Colour(0.0,0.0,1.0));

  fActiveHATPCVerticalOffset = 0;
 
  new G4PVPlacement(0,
  		    G4ThreeVector(0,fActiveHATPCVerticalOffset,0),
  		    logDrift,
  		    parentname+"/"+name+"/Drift",
  		    logFClayer7,
  		    false,
  		    0);
 
// Divide drift volume into two halves (readout planes)
// Make 2 separate halves (instead of 2 copies), so that each of 24 MM can be modified.

  width  = GetHATPCDriftWidth()/2.;
  height = GetHATPCDriftHeight()/2.;
  length = GetHATPCDriftLength()/2.;
  
  width -= GetHATPCCathodeThickness()/2.;
  width /= 2.;
  
  G4VSolid* innerHalf= new G4Box("InnerHalf",length,height,width);
    
  G4LogicalVolume* logHalf0
    = new G4LogicalVolume(innerHalf,
			  FindMaterial("GasMixtureTPC"),
			  parentname+"/"+name+"/Half");
  G4LogicalVolume* logHalf1
    = new G4LogicalVolume(innerHalf,
			  FindMaterial("GasMixtureTPC"),
			  parentname+"/"+name+"/Half");
  logHalf0->SetVisAttributes(G4Colour(1.0,0.0,0.0));
  logHalf1->SetVisAttributes(G4Colour(1.0,0.0,0.0));
  
  //logHalf0->SetUserLimits(new G4UserLimits(GetHATPCSteppingLimit()));
  //logHalf1->SetUserLimits(new G4UserLimits(GetHATPCSteppingLimit()));
    
  
  double delta = width + GetHATPCCathodeThickness()/2.;
  
  G4RotationMatrix rm;
  rm.rotateY(90.0*CLHEP::deg);
  
  new G4PVPlacement(G4Transform3D(rm,G4ThreeVector(-delta,0,0)),
		    logHalf0,
		    parentname+"/"+name+"/Half",
		    logDrift,
		    false,
		    0);
  
  rm.rotateY(180.0*CLHEP::deg);    
  
  new G4PVPlacement(G4Transform3D(rm,G4ThreeVector(delta,0,0)),
		    logHalf1,
		    parentname+"/"+name+"/Half",
		    logDrift,
		    false,
		    1);
  
  // add 12 Micromegas Modules to readout-plane

  
  double mmWidth  = 38*CLHEP::cm;
  double mmHeight = 33.61*CLHEP::cm;
  double mmLength = 2.*width;
  
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

  
  if (DebugHATPCMass){
    G4cout << "DriftGas (2) mass="<<logHalf0->GetMass()/CLHEP::kg<<" kg" << G4endl;
    G4cout << "FC mass="<<(logFClayer1->GetMass()+logFClayer2->GetMass()+logFClayer3->GetMass()+logFClayer4->GetMass()+logFClayer5->GetMass()+logFClayer6->GetMass()+logFClayer7->GetMass())/CLHEP::kg<<" kg" << G4endl;
    G4cout << "MM (8) mass="<<logMM->GetMass()/CLHEP::kg<<" kg" << G4endl;
    G4cout << "Sum of gases and MM :"<< (2*logHalf0->GetMass()+8*logMM->GetMass()+logFClayer1->GetMass()+logFClayer2->GetMass()+logFClayer3->GetMass()+logFClayer4->GetMass()+logFClayer5->GetMass()+logFClayer6->GetMass()+logFClayer7->GetMass())/CLHEP::kg << " kg" << G4endl;
  }
  
// BuildHATPCCentralCathode(logDrift);
// BuildHATPCCages(logGasGap);



return logVolume;  
}


void ND280HATPCConstructor::BuildHATPCCentralCathode(G4LogicalVolume* logVolume) {

    G4bool DebugHATPCMass = true;
    G4bool Vis = false;

    double y, z;

    G4String parentname = logVolume->GetName();

    MyND280BeamConstructor middleG10RoPlate(parentname+"/CentralCathode");
    middleG10RoPlate.SetWidth(GetHATPCCathodeThickness());
    middleG10RoPlate.SetHeight(700.0*CLHEP::mm);
    middleG10RoPlate.SetLength(1700.0*CLHEP::mm);
    middleG10RoPlate.SetMaterial(FindMaterial("G10Roha"));

    double width  = middleG10RoPlate.GetWidth();
    double height = middleG10RoPlate.GetHeight();
    double length = middleG10RoPlate.GetLength();

    G4LogicalVolume* log_middleG10RoPlate = middleG10RoPlate.GetPiece();
    
    new G4PVPlacement(0,
                      G4ThreeVector(0,0,0),
                      log_middleG10RoPlate,
                      middleG10RoPlate.GetName(),
                      logVolume,
                      false,
                      0);


    log_middleG10RoPlate->SetVisAttributes(G4Colour(0.0,1.0,1.0));
  
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

}

























