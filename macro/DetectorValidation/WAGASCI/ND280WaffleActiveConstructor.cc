#include <globals.hh>
#include <G4RotationMatrix.hh>
#include <G4VSolid.hh>
#include <G4ThreeVector.hh>
#include <G4Box.hh>
#include <G4PVPlacement.hh>
#include <G4SubtractionSolid.hh>
//#include "WAGASCI/ND280WAGASCI.hh"
//#include "WAGASCI/ND280WAGASCIElements.hh"
//#include "ecal/ND280ECalComponents.hh"
//#include "ecal/ND280ECalScintXConstructor.hh"
//#include "ecal/ND280ECalScintYConstructor.hh"
#include "WAGASCI/ND280WaffleActiveConstructor.hh"
#define DEBUG

void ND280WaffleActiveConstructor::Init() {
  G4cout <<"Hello, you are initializing the Waffle constructor. However, this Init() function needs to be defined later!" << G4endl;
  //fLengthX = 0;
  //fWidthY = 0;
  //fHeightZ = 0;

    //AddConstructor(new ND280ECalScintYConstructor("ScintHoriz", this));
    //AddConstructor(new ND280ECalScintXConstructor("ScintVert", this));
}

void ND280WaffleActiveConstructor::Construct(G4LogicalVolume * TargetLV, G4ThreeVector ModulePosition, G4ThreeVector ModuleSize, const G4String RepositoryName){

  G4Material * Air;
  G4Material * Water;
  G4Material * Scintillator;

  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  G4String name, symbol;
  G4int nel;
  G4double iz;
  
  G4Element* elO;
  G4Element* elH;
  G4Element* elC;
  G4Element* elN;
  

  a = 16.00*g/mole;
  elO = new G4Element(name="Oxigen", symbol="O", iz=8., a);
  a = 1.01*g/mole;
  elH = new G4Element(name="Hydrogen", symbol="H", z=1., a);
  a = 12.01*g/mole;
  elC = new G4Element(name="Carbon", symbol="C", z=6., a);
  a = 14.01*g/mole;
  elN = new G4Element(name="Nitrogen", symbol="N", iz=7., a);
  
  //Air
  density = 1.29*mg/cm3;
  Air = new G4Material(name="Air", density, nel=2);
  Air->AddElement(elN, .7);
  Air->AddElement(elO, .3);

  //Water
  density = 1.00*g/cm3;
  Water = new G4Material(name="Water", density, nel=2);
  Water->AddElement(elH,2);
  Water->AddElement(elO,1);

  //Scintillator
  density = 1.032*g/cm3;
  Scintillator = new G4Material(name="Scintillator", density, nel=2);
  Scintillator->AddElement(elC, 8);
  Scintillator->AddElement(elH, 8);


  //0th, always create the world volume. In this simulation, it is already defined
  //0th, create an Air Volume in which the target will be inserted create a big air volume in which Waffle will be inserted.

  	
  //

  G4ThreeVector WafflePosition    ( 0.0*mm, 0.0*mm, 0.0*mm );
  
  //const G4Rotation WaffleRotation    (0,0,0);
  G4RotationMatrix WaffleRotation (G4ThreeVector(0.,0.,0.),0.);

  //------------------ Target ---------------------

  // Target size
  double TargetWidth = 2300.0 * mm;
  double TargetHeight = 600.0 * mm;
  double TargetThickness = 1300.0 * mm;

  TargetWidth = ModuleSize[0];
  TargetHeight = ModuleSize[1];
  TargetThickness = ModuleSize[2];
  WafflePosition = ModulePosition;
  
  
    
  //------------------ Scintillators ---------------------


  //------------------ Module = Assembly of layers of scintillators ---------------------
  // We will assume that the geometry is planH + grid + planV + grid
  // No submodule is assumed. The plans are created one after another until it is not possible anymore due to target space limitation.
  // As for within one layer, we have added a gap between the layers and also, an offset between the target and the first (and therefore, last) scintillator layer.
  //Offset of the first scintillator of the grid layer in one layer / in the target
  const double LayerOffset = 0.0 * mm; // Offset in Z direction

  //Spacing between a layer of plan and a layer of Grid scintillator.
  //const double LayerGap = 1.0 * mm;
  const double LayerGap = 500.0 * mm;


  //Scintillator's defintition: part I
  //Individual plan scintillator size. 
  double ScintiPlanWidth = 25.0 * mm;
  double ScintiPlanThickness = 3.0 * mm;

  //Individual grid scintillator size.
  double ScintiGridWidth = 25.0 * mm;
  double ScintiGridThickness = 3.0 * mm;
  
  //------------------ Layers of scintillators ---------------------
  // We have added a possibility of offset of the first (and therefore, last) scintillator in the layer. Note that there is also Gap between the scintillators.
 
  //Offset of the first scintillator of the plan layer in one layer / in the target
  const double InitialLayerWidthPlanFirstScintiOffset = 0.0 * mm;

  //Offset of the first scintillator of the plan layer in one layer / in the target
  const double InitialLayerHeightPlanFirstScintiOffset = 0.0 * mm;

  //Offset of the first transvserse scintillator of the longitudinal (along z direction) layer in one layer / in the target
  const double InitialLayerThicknessPlanFirstScintiOffset = 0.0 * mm;


  //Offset of the first scintillator of the grid horizontal layer in one layer / in the target
  const double InitialLayerWidthGridFirstScintiOffset = 0.0 * mm;

  //Offset of the first scintillator of the grid vertical layer in one layer / in the target
  const double InitialLayerHeightGridFirstScintiOffset = 0.0 * mm;

  
  //Spacing between grid scintillators (consecutive scintillators in the same layer)
  const double GapBtwGridPlan = 1*mm;

  //Spacing between grid scintillators (consecutive scintillators in the same layer)
  const double ScintiGridGap = ScintiPlanWidth + 2*GapBtwGridPlan;

  //Spacing between grid scintillators (consecutive scintillators in the same layer)
  const double CellSizeWidth = ScintiPlanWidth + ScintiGridThickness;

  //Spacing between grid scintillators (consecutive scintillators in the same layer)
  const double CellSizeHeight = ScintiPlanWidth + ScintiGridWidth;

  //Deduced from the gap between plan chosen
  //Spacing between plan scintillators (consecutive scintillators in the same layer)
  const double ScintiPlanGap = CellSizeWidth - ScintiPlanWidth;


  //const double CellSize = 10.0 * mm;
  
  //NumberOfLayerPlanHorizontal

  //Scintillator's definition: part II
  //For the length
  //Separate for vertical and horizontal
  double ScintiVerticalPlanLength = TargetHeight-2*InitialLayerHeightPlanFirstScintiOffset;
  double ScintiHorizontalPlanLength = TargetWidth-2*InitialLayerWidthPlanFirstScintiOffset;
  double ScintiHorizontalGridLength = TargetLength-2*LayerOffset;

  //
  const double LayerPlanThickness = ScintiPlanThickness;
  const double LayerGridThickness = ScintiGridWidth;
  const int NumberOfModules=1;//Number of modules/target (water, air etc...)

  //Number of scintillatorsNumberOfLayerPlanHorizontal in a layer of plan scintillators.Note: there is an error here, one scintiplangap is extra counted (edge problem). So I added the test below
  // Temporary variable, the height of the horizontal scintillator layer (remove the offset of first and last scintillator)
  double LayerPlanHeight = TargetHeight - InitialLayerHeightPlanFirstScintiOffset*2;
  //Number of horizontal plan scintillators in a layer of horizontal scintillators
  int LayerPlanNumberOfScintiHorizontal = floor( ((LayerPlanHeight - ScintiPlanWidth)/(ScintiPlanWidth + ScintiPlanGap)) )+1;
  // Temporary variable, the height of the vertical scintillator layer (remove the offset of first and last scintillator)
  double LayerPlanWidth = TargetWidth - InitialLayerWidthPlanFirstScintiOffset*2;
  //Number of vertical plan scintillators in a layer of vertical scintillators
  int LayerPlanNumberOfScintiVertical = floor( (LayerPlanWidth - ScintiPlanWidth)/(ScintiPlanWidth + ScintiPlanGap) )+1;
  //Number of scintillators in a layer of plan scintillators.
  //Note: there is an error here, one scintiplangap is extra counted (edge problem). So I added the test below
  // Temporary variable, the height of the scintillator layer (remove the offset of first and last scintillator)

  double LayerGridHeight = TargetHeight - InitialLayerHeightGridFirstScintiOffset*2;
  //Number of horizontal Grid scintillators in a Grid plane
  int LayerGridNumberOfScintiHorizontal = floor( (LayerGridHeight-ScintiGridThickness)/(ScintiGridThickness + ScintiGridGap) )+1;
  // Temporary variable, the width of the scintillator layer (remove the offset of first and last scintillator)
  double LayerGridWidth = TargetWidth - InitialLayerHeightGridFirstScintiOffset*2;
  //Number of vertical Grid scintillators in a Grid plane
  int LayerGridNumberOfScintiVertical = floor( (LayerGridWidth-ScintiGridThickness)/(ScintiGridThickness + ScintiGridGap) )+1;

  //
  LayerPlanHeight = ScintiPlanWidth + (LayerPlanNumberOfScintiHorizontal-1)*(ScintiPlanWidth + ScintiPlanGap);
  LayerPlanWidth = ScintiPlanWidth + (LayerPlanNumberOfScintiVertical-1)*(ScintiPlanWidth + ScintiPlanGap);
  double LayerWidthPlanFirstScintiOffset = (TargetWidth - LayerPlanWidth)/2.;
  double LayerHeightPlanFirstScintiOffset = (TargetHeight - LayerPlanHeight)/2.;
  
  //
  LayerGridHeight = ScintiGridThickness + (LayerGridNumberOfScintiHorizontal-1)*(ScintiGridThickness + ScintiGridGap);
  LayerGridWidth = ScintiGridThickness + (LayerGridNumberOfScintiVertical-1)*(ScintiGridThickness + ScintiGridGap);
  double LayerWidthGridFirstScintiOffset = (TargetWidth - LayerGridWidth)/2.;
  double LayerHeightGridFirstScintiOffset = (TargetHeight - LayerGridHeight)/2.;
  
#ifdef DEBUG
  
  G4cout << "Vertical plan layer (=vertical scintillator):" << G4endl << "TargetHeight=" << TargetHeight << ", Layer Height = " << LayerPlanHeight << ", Initial layer offset = "<< InitialLayerHeightPlanFirstScintiOffset <<", final layer offset = " << LayerHeightPlanFirstScintiOffset << G4endl; 
  G4cout << "Test of the plan vertical scintillator length = " << ScintiVerticalPlanLength << " and if math are correct, should be equal to " << LayerPlanHeight + 2*(LayerHeightPlanFirstScintiOffset-InitialLayerHeightPlanFirstScintiOffset) << G4endl;
  G4cout << "Horizontal plan layer (=horizontal scintillator):" << G4endl << "TargetWidth=" << TargetWidth << ", Layer Width = " << LayerPlanWidth << ", Initial layer offset = "<< InitialLayerWidthPlanFirstScintiOffset <<", final layer offset = " << LayerWidthPlanFirstScintiOffset << G4endl; 
  G4cout << "Test of the plan horizontal scintillator length = " << ScintiHorizontalPlanLength << " and if math are correct, should be equal to " << LayerPlanWidth + 2*(LayerWidthPlanFirstScintiOffset-InitialLayerWidthPlanFirstScintiOffset) << G4endl;

  G4cout << "Horizontal grid layer (=horizontal scintillator):" << G4endl << "TargetWidth=" << TargetWidth << ", Layer Width = " << LayerGridWidth << ", Initial layer offset = "<< InitialLayerWidthGridFirstScintiOffset <<", final layer offset = " << LayerWidthGridFirstScintiOffset << G4endl; 
  G4cout << "Test of the grid horizontal scintillator length = " << ScintiHorizontalGridLength << " and if math are correct, should be equal to " << LayerGridWidth + 2*(LayerWidthGridFirstScintiOffset-InitialLayerWidthGridFirstScintiOffset) << G4endl;
#endif
  
 //Number of layers
  //Temporary variable, the thickness of the target that can be used to place layers (so, remove the offset of first and last layer)1.
  //double ModuleThickness = TargetThickness - 2*LayerOffset;
  //int ModuleNumberOfLayers;
  //To define
  int NumberOfLayers = 0.;


#ifdef DEBUG
  G4cout << "Waffle: " << G4endl << "Number of modules =" << NumberOfModules << G4endl;
  G4cout << "Center position = (" << WafflePosition[0] << ", "<< WafflePosition[1] << ", "<<  WafflePosition[2] << ")" << G4endl;
  G4cout << "Module size = (" << TargetWidth << ", "<< TargetHeight << ", "<<  TargetThickness << ")" << G4endl;
  
#endif

  // -- The scintillator layers. An assembly of scintillators

  //1. Layer plan horizontal
  //First, create the volume of a template scintillator:
  G4Box * ScintiHorizontalPlanBox = new G4Box("ScintiHorizontalPlanBox",ScintiHorizontalPlanLength/2.,ScintiPlanWidth/2.,ScintiPlanThickness/2.);
  //Second, create the LV of a template scintillator: 
  G4LogicalVolume * ScintiHorizontalPlanLV = new G4LogicalVolume(ScintiHorizontalPlanBox,Scintillator,"ScintiPlanLV");

  //2. Layer plan vertical
  //First, create the volume of a template scintillator:
  G4Box * ScintiVerticalPlanBox = new G4Box("ScintiVerticalPlanBox",ScintiPlanWidth/2.,ScintiVerticalPlanLength/2.,ScintiPlanThickness/2.);
  //Second, create the LV of a template scintillator: 
  G4LogicalVolume * ScintiVerticalPlanLV = new G4LogicalVolume(ScintiVerticalPlanBox,Scintillator,"ScintiPlanLV");

  //Currently, the problem is that I do not have created a layer LV that allow me to place a layer anywhere I wish. So we will work with scintillator planes instead of layers. So we should create also the LV of other scintillator types.

  //Put a scintillator where there is a slit in fact!
  
  //1. Layer grid horizontal
  //First, create the volume of a template scintillator:
  G4Box * ScintiHorizontalGridExternalBox = new G4Box("ScintiHorizontalGridExternalBox",ScintiGridThickness/2.,ScintiGridWidth/2.,ScintiHorizontalGridLength/2.);
  G4VSolid * ScintiHorizontalGridBox = (G4VSolid*) ScintiHorizontalGridExternalBox ;
  //Second, create the LV of a template scintillator: 
  G4LogicalVolume * ScintiHorizontalGridLV = new G4LogicalVolume(ScintiHorizontalGridBox,Scintillator,"ScintiGridLV");

  //Iterate the slit removal in fact. Take the box, remove one slit, update the box with the box having one slit removed, remove another slit etc...
#ifdef DEBUG
  G4cout << "Horizontal GRID scintillator, dimensions = (" << ScintiHorizontalGridExternalBox->GetXHalfLength()*2 << ", " << ScintiHorizontalGridExternalBox->GetYHalfLength()*2 << ", " << ScintiHorizontalGridExternalBox->GetZHalfLength()*2 << ") " << G4endl;
#endif

  
   /*
  //First, create the volume of a template scintillator:
  G4VSolid * ScintiGridBox = new G4Box("ScintiGridBox",ScintiHorizontalGridLength/2.,ScintiGridWidth/2.,ScintiGridThickness/2.);
  //Second, create the LV of a template scintillator: 
  G4LogicalVolume * ScintiGridLV = new G4LogicalVolume(ScintiGridBox,Scintillator,"ScintiGridLV");
  */
  //Third, place them <=> Create a physical volume => Create a layer
  double LayerCenter;
  G4ThreeVector FromLayertoTarget (0.0*mm, 0.0*mm, LayerCenter);
  G4ThreeVector FromTargettoGlobal (/*WafflePosition[0]*/-TargetWidth/2.,/*WafflePosition[1]*/-TargetHeight/2.,/*WafflePosition[2]*/-TargetThickness/2.);
  //G4ThreeVector FromTargettoGlobal (0.,0.,0.);
  //G4ThreeVector FromTargettoGlobal (-TargetWidth/2.,-TargetHeight/2.,-TargetThickness/2.);

#ifdef DEBUG
  G4cout << "From target to global = (" << FromTargettoGlobal[0] << ", "<< FromTargettoGlobal[1] << ", "<<  FromTargettoGlobal[2] << ")" << G4endl;
  G4cout << "Number of modules =" << NumberOfModules << G4endl;
  G4cout << "Number of layers =" << NumberOfLayers << G4endl;
#endif

 for(int m=0;m<NumberOfModules;m++){//Loop over the number of modules (water/air etc...)
    int NumberOfLayerGrid=0;
    int NumberOfLayerPlanHorizontal=0;
    int NumberOfLayerPlanVertical=0;

    //for(int l=0;l<NumberOfLayers;l++){//Loop over the total number of layer. Note here that each grid, plan ... layer count individually as 1 layer.
    //2*LayerOffset + (NumberOfLayerPlan+1)*(LayerGap+LayerPlanThickness) + (NumberOfLayerGrid+1)*(LayerGap+LayerGridThickness)
     
    while(1){
      //First, define the position of the layer center "relative to the module". Assumed no offset in X and Y. The "module referential frame" is defined using the most upstream plane of the target (define Z=0). Z>0 corresponds to the downstream direction. X and Y directions are defined transverse to Z, respectively as the horizontal and vertical directions. X=0, Y=0 corresponds to the right bottom corner of the module when facing it from upstream (from Z<0). X>0 corresponds to the left in this view, and Y>0 corresponds to the up.   
      double LayerUpstreamEdge = LayerOffset+(NumberOfLayerGrid*(LayerGridThickness+LayerGap))+((NumberOfLayerPlanHorizontal+NumberOfLayerPlanVertical)*(LayerPlanThickness+LayerGap));

      //We assume the following structure: plan vertical, grid, plan horizontal, grid etc...
      int PlanType=NumberOfLayers%2;//0=plan vertical, 1=grid, 2=plan horizontal, 3=grid


      if(PlanType==0){//layer plan vertical
	//Second, create and place the scintillators
	LayerCenter = LayerUpstreamEdge + LayerPlanThickness/2;//Center of the layer in z position
	G4ThreeVector FromLayerToTarget (0.0*mm, 0.0*mm, LayerCenter);	
#ifdef DEBUG
	G4cout << "Layer #" << NumberOfLayers << " is of type 2D horizontal, number of scintillators = " << LayerPlanNumberOfScintiVertical << G4endl;
#endif
	G4ThreeVector ScintillatorPosition = (FromLayerToTarget + FromTargettoGlobal);
	ScintillatorPosition[0] = FromLayerToTarget[0] + FromTargettoGlobal[0] + LayerWidthPlanFirstScintiOffset + ScintiPlanWidth/2;
	ScintillatorPosition[1] = FromLayerToTarget[1] + FromTargettoGlobal[1] + TargetHeight/2.;
	ScintillatorPosition[2] = FromLayerToTarget[2] + FromTargettoGlobal[2];
	if( (LayerCenter + LayerPlanThickness/2) > (TargetThickness - LayerOffset) ) break;
	   
	for(int i=0; i<LayerPlanNumberOfScintiVertical;i++){//Loop over scintillators
	  new G4PVPlacement(0, ScintillatorPosition, ScintiVerticalPlanLV,RepositoryName+Form("LayerPlanVerticalScinti[%d][%d][%d]",m,NumberOfLayerPlanVertical,i),TargetLV,false,0,false);
#ifdef DEBUG
	  G4cout << "Scintillator position = (" << ScintillatorPosition[0] << ", " << ScintillatorPosition[1] << ", " << ScintillatorPosition[2] << ") " <<G4endl;
#endif
	  ScintillatorPosition[0] += (ScintiPlanWidth + ScintiPlanGap);
	}
	//
	NumberOfLayerPlanVertical++;
	
      }
      else if(PlanType==1){
	//Second, create and place the scintillators
	LayerCenter = LayerUpstreamEdge + LayerPlanThickness/2;
	G4ThreeVector FromLayerToTarget (0.0*mm, 0.0*mm, LayerCenter);
#ifdef DEBUG
	G4cout << "Layer #" << NumberOfLayers << " is of type grid, number of scintillators = " << LayerPlanNumberOfScintiHorizontal << G4endl;
#endif
	G4ThreeVector ScintillatorPosition = (FromLayerToTarget + FromTargettoGlobal);
	ScintillatorPosition[0] = FromLayerToTarget[0] + FromTargettoGlobal[0] + TargetWidth/2.;
	ScintillatorPosition[1] = FromLayerToTarget[1] + FromTargettoGlobal[1] + LayerHeightPlanFirstScintiOffset + ScintiPlanWidth/2;
	ScintillatorPosition[2] = FromLayerToTarget[2] + FromTargettoGlobal[2];
	if( (LayerCenter + LayerPlanThickness/2) > (TargetThickness - LayerOffset) ) break;
	
	for(int i=0; i<LayerPlanNumberOfScintiHorizontal;i++){//Loop over scintillators
	new G4PVPlacement(0, ScintillatorPosition, ScintiHorizontalPlanLV,RepositoryName+Form("LayerPlanHorizontalScinti[%d][%d][%d]",m,NumberOfLayerPlanHorizontal,i),TargetLV,false,0,false);       
#ifdef DEBUG
	  G4cout << "Scintillator position = (" << ScintillatorPosition[0] << ", " << ScintillatorPosition[1] << ", " << ScintillatorPosition[2] << ") " <<G4endl;
#endif
	  ScintillatorPosition[1] += (ScintiPlanWidth + ScintiPlanGap);
	}
	//
	NumberOfLayerPlanHorizontal++;
      }
      if(NumberOfLayerPlanHorizontal == 1 && NumberOfLayerPlanVertical == 1){
	//How to define a layer plan? There is a symmetry of the module here. But we will define one layer as plane along Z direction (scintillator) and Y. We start at X=0 -> X>0
	//First, where is placed the layer? It starts after the first Y scintillator, i.e at X=LayerWidthPlanFirstScintiOffset + ScintiPlanWidth + GapBtwGridPlan + ScintiGridThickness/2 <=> X=LayerWidthPlanFirstScintiOffset + ScintiGridGap - GapBtwGridPlan + ScintiGridThickness/2.
	//For the Y, it will start from after the first X scintillator : LayerHeightPlanFirstScintiOffset + ScintiPlanWidth + GapBtwGridPlan + ScintiGridWidth/2. The step will be then CellGrid (basically, ScintiGridWidth + ScintiPlanWidth + 2*GapBtwGridPlan) <=> starts at LayerHeightPlanFirstScintiOffset + ScintiGridGap - GapBtwGridPlan + ScintiGridWidth/2. and step = CellSizeHeight.
	//For the Z, it will be TargetLength/2.

	LayerHeightGridFirstScintiOffset = LayerHeightPlanFirstScintiOffset + ScintiGridGap - GapBtwGridPlan;//Offset of the first plan.
	double LayerOffsetGrid = LayerWidthPlanFirstScintiOffset + ScintiGridGap - GapBtwGridPlan;//Offset of the first plan.
	double LayerUpstreamEdge = LayerOffsetGrid + (NumberOfLayerGrid*CellSizeWidth);
	
	//Create the layers from (0,0,center) of the module.
	LayerCenter = LayerUpstreamEdge + LayerGridThickness/2;
	G4ThreeVector FromLayerToTarget (LayerCenter, 0.0*mm, 0.0*mm);

#ifdef DEBUG
	G4cout << "Layer #" << NumberOfLayers << " is of type grid, number of scintillators vertical grid= " << LayerGridNumberOfScintiVertical << ", horizontal grid= " << LayerGridNumberOfScintiHorizontal << ", Layer width = " << LayerGridWidth << ", Layer height = " << LayerGridHeight << G4endl;
#endif

	G4ThreeVector ScintillatorPosition = (FromLayerToTarget + FromTargettoGlobal);
	ScintillatorPosition[0] = (FromLayerToTarget[0] + FromTargettoGlobal[0]);
	ScintillatorPosition[1] = (FromLayerToTarget[1] + FromTargettoGlobal[1]) + LayerHeightGridFirstScintiOffset + ScintiPlanWidth/2;
	ScintillatorPosition[2] = (FromLayerToTarget[2] + FromTargettoGlobal[2]) + TargetThickness/2.;
	//if( (LayerCenter + LayerGridThickness/2) > (TargetThickness - LayerOffset) ) break;

	//horizontal
	for(int i=0; i<LayerGridNumberOfScintiHorizontal;i++){//Loop over scintillators
	  new G4PVPlacement(0, ScintillatorPosition, ScintiHorizontalGridLV,RepositoryName+Form("LayerGridHorizontalScinti[%d][%d][%d]",m,NumberOfLayerGrid,i),TargetLV,false,0,false);
#ifdef DEBUG
	  G4cout << "Horizontal grid, Scintillator position = (" << ScintillatorPosition[0] << ", " << ScintillatorPosition[1] << ", " << ScintillatorPosition[2] << ") " <<G4endl;
#endif
	  ScintillatorPosition[1] += CellSizeHeight;
	}
	//
	NumberOfLayerGrid++;
	//
	
      }
      /*
      else if(PlanType==2){//layer grid
		
	LayerCenter = LayerUpstreamEdge + LayerGridThickness/2;
	G4ThreeVector FromLayerToTarget (0.0*mm, 0.0*mm, LayerCenter);

#ifdef DEBUG
	G4cout << "Layer #" << NumberOfLayers << " is of type grid, number of scintillators vertical grid= " << LayerGridNumberOfScintiVertical << ", horizontal grid= " << LayerGridNumberOfScintiHorizontal << ", Layer width = " << LayerGridWidth << ", Layer height = " << LayerGridHeight << G4endl;
#endif

	G4ThreeVector ScintillatorPosition = (FromLayerToTarget + FromTargettoGlobal);
	ScintillatorPosition[0] = (FromLayerToTarget[0] + FromTargettoGlobal[0]) + LayerWidthGridFirstScintiOffset + ScintiGridThickness/2;
	ScintillatorPosition[1] = (FromLayerToTarget[1] + FromTargettoGlobal[1]) + TargetHeight/2.;
	ScintillatorPosition[2] = (FromLayerToTarget[2] + FromTargettoGlobal[2]);
	if( (LayerCenter + LayerGridThickness/2) > (TargetThickness - LayerOffset) ) break;
	
	//vertical
	for(int i=0; i<LayerGridNumberOfScintiVertical;i++){//Loop over scintillators
	  new G4PVPlacement(0, ScintillatorPosition, ScintiVerticalGridLV,RepositoryName+Form("LayerGridVerticalScinti[%d][%d][%d]",m,NumberOfLayerGrid,i),TargetLV,false,0,false);	 
#ifdef DEBUG
	  G4cout << "Vertical grid, Scintillator position = (" << ScintillatorPosition[0] << ", " << ScintillatorPosition[1] << ", " << ScintillatorPosition[2] << ") " <<G4endl;
#endif
	  ScintillatorPosition[0] += CellSize;
	}
	
	ScintillatorPosition[0] = (FromLayerToTarget[0] + FromTargettoGlobal[0]) + TargetWidth/2.;
	ScintillatorPosition[1] = (FromLayerToTarget[1] + FromTargettoGlobal[1]) + LayerHeightGridFirstScintiOffset + ScintiGridThickness/2;
	ScintillatorPosition[2] = (FromLayerToTarget[2] + FromTargettoGlobal[2]);

	//horizontal
	for(int i=0; i<LayerGridNumberOfScintiHorizontal;i++){//Loop over scintillators
	  new G4PVPlacement(0, ScintillatorPosition, ScintiHorizontalGridLV,RepositoryName+Form("LayerGridHorizontalScinti[%d][%d][%d]",m,NumberOfLayerGrid,i),TargetLV,false,0,false);
#ifdef DEBUG
	  G4cout << "Horizontal grid, Scintillator position = (" << ScintillatorPosition[0] << ", " << ScintillatorPosition[1] << ", " << ScintillatorPosition[2] << ") " <<G4endl;
#endif
	  ScintillatorPosition[1] += CellSize;
	}
	//
	NumberOfLayerGrid++;
      }*/
      NumberOfLayers = NumberOfLayerPlanHorizontal + NumberOfLayerPlanVertical + NumberOfLayerGrid;
      //if(NumberOfLayers==2) break;
    }
 }   

 //return TargetPV;
 //return vol_logical;
}
