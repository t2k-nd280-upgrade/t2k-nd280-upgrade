//##################################################################################
//##################################################################################
//################### Created by B. Quilain on 2017/05/17 ##########################
//################### Last modification by B. Quilain on 2017/05/17 ################
//##################################################################################
//##################################################################################
#include <globals.hh>
#include <G4RotationMatrix.hh>
#include <G4VSolid.hh>
#include <G4ThreeVector.hh>
#include <G4Box.hh>
#include <G4PVPlacement.hh>
#include <G4SubtractionSolid.hh>
#include "ND280WaffleActiveConstructor.hh"
//#define DEBUG

ND280WaffleActiveConstructor::~ND280WaffleActiveConstructor() {;}

void ND280WaffleActiveConstructor::Init() {
  //G4cout <<"Hello, you are initializing the Waffle constructor. However, this Init() function needs to be defined later!" << G4endl;
}

G4LogicalVolume *ND280WaffleActiveConstructor::GetPiece(void) {
  // If not initialized it doesn't compile (abstract class)
  return NULL;
} // D.S

void ND280WaffleActiveConstructor::Construct(G4LogicalVolume * TargetLV, G4ThreeVector ModulePosition, G4ThreeVector ModuleSize, const G4String RepositoryName){

  ND280RootPersistencyManager* persistencyManager
    = ND280RootPersistencyManager::GetInstance();
  fND280XMLInput = persistencyManager->GetXMLInput(); // S.D

  //0th, always create the world volume. In this simulation, it is already defined
  //0th, create an Air Volume in which the target will be inserted create a big air volume in which Waffle will be inserted.

  	
  //

  G4ThreeVector WafflePosition    ( 0.0*CLHEP::mm, 0.0*CLHEP::mm, 0.0*CLHEP::mm );
  
  //const G4Rotation WaffleRotation    (0,0,0);
  G4RotationMatrix WaffleRotation (G4ThreeVector(0.,0.,0.),0.);

  //------------------ Target ---------------------

  // Target size
  double TargetWidth = 2300.0 * CLHEP::mm;
  double TargetHeight = 600.0 * CLHEP::mm;
  double TargetThickness = 1300.0 * CLHEP::mm;

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
  const double LayerOffset = 0.0 * CLHEP::mm; // Offset in Z direction

  //Spacing between a layer of plan and a layer of Grid scintillator.
  const double LayerGap = 1.0 * CLHEP::mm;
  //const double LayerGap = 500.0 * mm;


  //Scintillator's defintition: part I
  //Individual plan scintillator size. 
  double ScintiPlanWidth = 25.0 * CLHEP::mm;
  double ScintiPlanThickness = 25.0 * CLHEP::mm;

  //Individual grid scintillator size.
  double ScintiGridWidth = 25.0 * CLHEP::mm;
  double ScintiGridThickness = 25.0 * CLHEP::mm;
  
  //------------------ Layers of scintillators ---------------------
  // We have added a possibility of offset of the first (and therefore, last) scintillator in the layer. Note that there is also Gap between the scintillators.
 
  //Offset of the first scintillator of the plan layer in one layer / in the target
  const double InitialLayerWidthPlanFirstScintiOffset = 0.0 * CLHEP::mm;

  //Offset of the first scintillator of the plan layer in one layer / in the target
  const double InitialLayerHeightPlanFirstScintiOffset = 0.0 * CLHEP::mm;

  //Offset of the first transvserse scintillator of the longitudinal (along z direction) layer in one layer / in the target
  // const double InitialLayerThicknessPlanFirstScintiOffset = 0.0 * CLHEP::mm;


  //Offset of the first scintillator of the grid horizontal layer in one layer / in the target
  // const double InitialLayerWidthGridFirstScintiOffset = 0.0 * CLHEP::mm;

  //Offset of the first scintillator of the grid vertical layer in one layer / in the target
  //const double InitialLayerHeightGridFirstScintiOffset = 0.0 * CLHEP::mm;

  
  //Spacing between grid scintillators (consecutive scintillators in the same layer)
  const double GapBtwGridPlan = 1*CLHEP::mm;//We assume same spacing between GRID and Y scintillators and X scintillators.

  //Spacing between grid scintillators (consecutive scintillators in the same layer)
  const double ScintiGridGap = ScintiPlanWidth + 2*GapBtwGridPlan;

  //Spacing between grid scintillators (consecutive scintillators in the same layer)
  const double CellSizeWidth =  ScintiGridGap + ScintiGridThickness;

  //Spacing between grid scintillators (consecutive scintillators in the same layer)
  const double CellSizeHeight = ScintiGridGap + ScintiGridWidth;

  //Deduced from the gap between plan chosen
  //Spacing between plan scintillators (consecutive scintillators in the same layer)
  //const double ScintiPlanGap = CellSizeWidth - ScintiPlanWidth;


  //const double CellSize = 10.0 * mm;
  
  //NumberOfLayerPlanHorizontal

  //Scintillator's definition: part II
  //For the length
  //Separate for vertical and horizontal
  double ScintiVerticalPlanLength = TargetHeight-2*InitialLayerHeightPlanFirstScintiOffset;
  double ScintiHorizontalPlanLength = TargetWidth-2*InitialLayerWidthPlanFirstScintiOffset;

  //
  const int NumberOfModules=1;//Number of modules/target (water, air etc...)

  //Number of scintillatorsNumberOfLayerPlanHorizontal in a layer of plan scintillators.Note: there is an error here, one scintiplangap is extra counted (edge problem). So I added the test below
  // Temporary variable, the height of the horizontal scintillator layer (remove the offset of first and last scintillator)
  double LayerPlanHeight = TargetHeight - InitialLayerHeightPlanFirstScintiOffset*2;
  //Number of horizontal plan scintillators in a layer of horizontal scintillators
  int LayerPlanNumberOfScintiHorizontal = floor( ((LayerPlanHeight - ScintiPlanWidth)/(CellSizeHeight)) )+1;
  // Temporary variable, the height of the vertical scintillator layer (remove the offset of first and last scintillator)
  double LayerPlanWidth = TargetWidth - InitialLayerWidthPlanFirstScintiOffset*2;
  //Number of vertical plan scintillators in a layer of vertical scintillators
  int LayerPlanNumberOfScintiVertical = floor( (LayerPlanWidth - ScintiPlanWidth)/(CellSizeWidth) )+1;
  //Number of scintillators in a layer of plan scintillators.
  //Note: there is an error here, one scintiplangap is extra counted (edge problem). So I added the test below
  // Temporary variable, the height of the scintillator layer (remove the offset of first and last scintillator)

  //
  LayerPlanHeight = ScintiPlanWidth + (LayerPlanNumberOfScintiHorizontal-1)*(CellSizeHeight);
  LayerPlanWidth = ScintiPlanWidth + (LayerPlanNumberOfScintiVertical-1)*(CellSizeWidth);
  const double LayerPlanThickness = ScintiPlanThickness;
   double LayerWidthPlanFirstScintiOffset = (TargetWidth - LayerPlanWidth)/2.;
  double LayerHeightPlanFirstScintiOffset = (TargetHeight - LayerPlanHeight)/2.;

  double LayerHeightGridFirstScintiOffset = LayerHeightPlanFirstScintiOffset + ScintiPlanWidth + GapBtwGridPlan;//Offset of the first plan.
  double LayerGridOffset = LayerWidthPlanFirstScintiOffset + ScintiPlanWidth + GapBtwGridPlan;//Offset of the first plan along x axis.
  double LayerGridHeight = TargetHeight - LayerHeightGridFirstScintiOffset*2;
  double LayerGridWidth = ScintiGridThickness;//TargetWidth - LayerGridOffset*2;
  double ScintiHorizontalGridLength = TargetThickness-2*LayerOffset;
  const double LayerGridThickness = ScintiHorizontalGridLength;
  int LayerGridNumberOfScintiHorizontal = floor( (LayerGridHeight-ScintiGridWidth)/CellSizeHeight )+1;
  //Number of vertical Grid scintillators in a Grid plane


  //
  //LayerGridHeight = ScintiGridThickness + (LayerGridNumberOfScintiHorizontal-1)*(ScintiGridThickness + ScintiGridGap);
  //LayerGridWidth = ScintiGridThickness + (LayerGridNumberOfScintiVertical-1)*(ScintiGridThickness + ScintiGridGap);
  //double LayerWidthGridFirstScintiOffset = LayerGridOffset;//(TargetWidth - LayerGridWidth)/2.;
  //double LayerHeightGridFirstScintiOffset = (TargetHeight - LayerGridHeight)/2.;
  
#ifdef DEBUG
  
  G4cout << "Vertical plan layer (=vertical scintillator):" << G4endl << "TargetHeight=" << TargetHeight << ", Layer Height = " << LayerPlanHeight << ", Initial layer offset = "<< InitialLayerHeightPlanFirstScintiOffset <<", final layer offset = " << LayerHeightPlanFirstScintiOffset << G4endl; 
  G4cout << "Test of the plan vertical scintillator length = " << ScintiVerticalPlanLength << " and if math are correct, should be equal to " << LayerPlanHeight + 2*(LayerHeightPlanFirstScintiOffset-InitialLayerHeightPlanFirstScintiOffset) << G4endl;
  G4cout << "Horizontal plan layer (=horizontal scintillator):" << G4endl << "TargetWidth=" << TargetWidth << ", Layer Width = " << LayerPlanWidth << ", Initial layer offset = "<< InitialLayerWidthPlanFirstScintiOffset <<", final layer offset = " << LayerWidthPlanFirstScintiOffset << G4endl; 
  G4cout << "Test of the plan horizontal scintillator length = " << ScintiHorizontalPlanLength << " and if math are correct, should be equal to " << LayerPlanWidth + 2*(LayerWidthPlanFirstScintiOffset-InitialLayerWidthPlanFirstScintiOffset) << G4endl;

  G4cout << "Horizontal grid layer (=horizontal scintillator):" << G4endl << "TargetHeight=" << TargetHeight << ", Layer height = " << LayerGridHeight << ", Initial layer offset = "<< InitialLayerHeightGridFirstScintiOffset <<", final layer offset = " << LayerHeightGridFirstScintiOffset << ", offset in the X direction (so, of the first grid scinti) = "<< LayerGridOffset << G4endl; 
  G4cout << "Test of the grid horizontal scintillator length = " << ScintiHorizontalGridLength << " and if math are correct, should be equal to " << LayerGridHeight + 2*(LayerHeightGridFirstScintiOffset-InitialLayerHeightGridFirstScintiOffset) << G4endl;
#endif
  
 //Number of layers
  //Temporary variable, the thickness of the target that can be used to place layers (so, remove the offset of first and last layer)1.
  //double ModuleThickness = TargetThickness - 2*LayerOffset;
  //int ModuleNumberOfLayers;
  //To define
  //int NumberOfLayers = 0.;

  //This is absolutely not the number of plans. Just a maximal bound that is used to set the number ID of every scintillator without any overlap.
  const int MaxNumberOfLayerPlanVertical=500;
  const int MaxNumberOfLayerPlanHorizontal=500;
  const int MaxNumberOfLayerGrid=500;
  
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
  G4LogicalVolume * ScintiHorizontalPlanLV = new G4LogicalVolume(ScintiHorizontalPlanBox,FindMaterial("FGDScintillator"),"ScintiPlanLV"); // D.S

  //2. Layer plan vertical
  //First, create the volume of a template scintillator:
  G4Box * ScintiVerticalPlanBox = new G4Box("ScintiVerticalPlanBox",ScintiPlanWidth/2.,ScintiVerticalPlanLength/2.,ScintiPlanThickness/2.);
  //Second, create the LV of a template scintillator: 
  G4LogicalVolume * ScintiVerticalPlanLV = new G4LogicalVolume(ScintiVerticalPlanBox,FindMaterial("FGDScintillator"),"ScintiPlanLV"); // D.S

  //Currently, the problem is that I do not have created a layer LV that allow me to place a layer anywhere I wish. So we will work with scintillator planes instead of layers. So we should create also the LV of other scintillator types.

  //Put a scintillator where there is a slit in fact!
  
  //1. Layer grid horizontal
  //First, create the volume of a template scintillator:
  G4Box * ScintiHorizontalGridExternalBox = new G4Box("ScintiHorizontalGridExternalBox",ScintiGridThickness/2.,ScintiGridWidth/2.,ScintiHorizontalGridLength/2.);
  G4VSolid * ScintiHorizontalGridBox = (G4VSolid*) ScintiHorizontalGridExternalBox ;
  //Second, create the LV of a template scintillator: 
  G4LogicalVolume * ScintiHorizontalGridLV = new G4LogicalVolume(ScintiHorizontalGridBox,FindMaterial("FGDScintillator"),"ScintiGridLV"); // D.S

  //Iterate the slit removal in fact. Take the box, remove one slit, update the box with the box having one slit removed, remove another slit etc...
#ifdef DEBUG
  G4cout << "Horizontal GRID scintillator, dimensions = (" << ScintiHorizontalGridExternalBox->GetXHalfLength()*2 << ", " << ScintiHorizontalGridExternalBox->GetYHalfLength()*2 << ", " << ScintiHorizontalGridExternalBox->GetZHalfLength()*2 << ") " << G4endl;
#endif

  
   /*
  //First, create the volume of a template scintillator:
  G4VSolid * ScintiGridBox = new G4Box("ScintiGridBox",ScintiHorizontalGridLength/2.,ScintiGridWidth/2.,ScintiGridThickness/2.);
  //Second, create the LV of a template scintillator: 
  G4LogicalVolume * ScintiGridLV = new G4LogicalVolume(ScintiGridBox,FindMaterial("FGDScintillator"),"ScintiGridLV"); // D.S
  */
  //Third, place them <=> Create a physical volume => Create a layer
  double LayerCenter = 0.; // D.S
  G4ThreeVector FromLayertoTarget (0.0*CLHEP::mm, 0.0*CLHEP::mm, LayerCenter);
  G4ThreeVector FromTargettoGlobal (/*WafflePosition[0]*/-TargetWidth/2.,/*WafflePosition[1]*/-TargetHeight/2.,/*WafflePosition[2]*/-TargetThickness/2.);
  //G4ThreeVector FromTargettoGlobal (0.,0.,0.);
  //G4ThreeVector FromTargettoGlobal (-TargetWidth/2.,-TargetHeight/2.,-TargetThickness/2.);

#ifdef DEBUG
  G4cout << "From target to global = (" << FromTargettoGlobal[0] << ", "<< FromTargettoGlobal[1] << ", "<<  FromTargettoGlobal[2] << ")" << G4endl;
  G4cout << "Number of modules =" << NumberOfModules << G4endl;
  //G4cout << "Number of layers =" << NumberOfLayers << G4endl;
#endif

 for(int m=0;m<NumberOfModules;m++){//Loop over the number of modules (water/air etc...)
    int NumberOfLayerGrid=0;
    int NumberOfLayerPlanHorizontal=0;
    int NumberOfLayerPlanVertical=0;
    int NumberOfLayerPlan=0;

    //for(int l=0;l<NumberOfLayers;l++){//Loop over the total number of layer. Note here that each grid, plan ... layer count individually as 1 layer.
    //2*LayerOffset + (NumberOfLayerPlan+1)*(LayerGap+LayerPlanThickness) + (NumberOfLayerGrid+1)*(LayerGap+LayerGridThickness)
     
    while(1){
      //First, define the position of the layer center "relative to the module". Assumed no offset in X and Y. The "module referential frame" is defined using the most upstream plane of the target (define Z=0). Z>0 corresponds to the downstream direction. X and Y directions are defined transverse to Z, respectively as the horizontal and vertical directions. X=0, Y=0 corresponds to the right bottom corner of the module when facing it from upstream (from Z<0). X>0 corresponds to the left in this view, and Y>0 corresponds to the up.   
      double LayerUpstreamEdge = LayerOffset+(NumberOfLayerPlan*LayerPlanThickness+LayerGap);

      //We assume the following structure: plan vertical, grid, plan horizontal, grid etc...
      int PlanType=NumberOfLayerPlan%2;//0=plan vertical, 1=grid, 2=plan horizontal, 3=grid


      if(PlanType==0){//layer plan vertical
	//Second, create and place the scintillators
	LayerCenter = LayerUpstreamEdge + LayerPlanThickness/2;//Center of the layer in z position
	G4ThreeVector FromLayerToTarget (0.0*CLHEP::mm, 0.0*CLHEP::mm, LayerCenter);	
#ifdef DEBUG
	G4cout << "Layer #" << NumberOfLayerPlan << " is of type 2D horizontal, number of scintillators = " << LayerPlanNumberOfScintiVertical << G4endl;
#endif
	G4ThreeVector ScintillatorPosition = (FromLayerToTarget + FromTargettoGlobal);
	ScintillatorPosition[0] = FromLayerToTarget[0] + FromTargettoGlobal[0] + LayerWidthPlanFirstScintiOffset + ScintiPlanWidth/2;
	ScintillatorPosition[1] = FromLayerToTarget[1] + FromTargettoGlobal[1] + TargetHeight/2.;
	ScintillatorPosition[2] = FromLayerToTarget[2] + FromTargettoGlobal[2];
	if( (LayerCenter + LayerPlanThickness/2) > (TargetThickness - LayerOffset) ) break;
	   
	for(int i=0; i<LayerPlanNumberOfScintiVertical;i++){//Loop over scintillators
	  int id=m*NumberOfModules+NumberOfLayerPlanVertical*MaxNumberOfLayerPlanVertical+i;
	  new G4PVPlacement(0, ScintillatorPosition, ScintiVerticalPlanLV,RepositoryName+Form("LayerPlanVerticalScinti[%d][%d][%d]",m,NumberOfLayerPlanVertical,i),TargetLV,false,id,false);
	  
#ifdef DEBUG
	  G4cout << "Scintillator position = (" << ScintillatorPosition[0] << ", " << ScintillatorPosition[1] << ", " << ScintillatorPosition[2] << ") " <<G4endl;
#endif
	  ScintillatorPosition[0] += (CellSizeWidth);
	}
	//
	NumberOfLayerPlanVertical++;
	
      }
      else if(PlanType==1){
	//Second, create and place the scintillators
	LayerCenter = LayerUpstreamEdge + LayerPlanThickness/2;
	G4ThreeVector FromLayerToTarget (0.0*CLHEP::mm, 0.0*CLHEP::mm, LayerCenter);
#ifdef DEBUG
	G4cout << "Layer #" << NumberOfLayerPlan << " is of type grid, number of scintillators = " << LayerPlanNumberOfScintiHorizontal << G4endl;
#endif
	G4ThreeVector ScintillatorPosition = (FromLayerToTarget + FromTargettoGlobal);
	ScintillatorPosition[0] = FromLayerToTarget[0] + FromTargettoGlobal[0] + TargetWidth/2.;
	ScintillatorPosition[1] = FromLayerToTarget[1] + FromTargettoGlobal[1] + LayerHeightPlanFirstScintiOffset + ScintiPlanWidth/2;
	ScintillatorPosition[2] = FromLayerToTarget[2] + FromTargettoGlobal[2];
	if( (LayerCenter + LayerPlanThickness/2) > (TargetThickness - LayerOffset) ) break;
	
	for(int i=0; i<LayerPlanNumberOfScintiHorizontal;i++){//Loop over scintillators
	  int id=m*NumberOfModules+NumberOfLayerPlanHorizontal*MaxNumberOfLayerPlanHorizontal+i;
	  new G4PVPlacement(0, ScintillatorPosition, ScintiHorizontalPlanLV,RepositoryName+Form("LayerPlanHorizontalScinti[%d][%d][%d]",m,NumberOfLayerPlanHorizontal,i),TargetLV,false,id,false);       
#ifdef DEBUG
	  G4cout << "Scintillator position = (" << ScintillatorPosition[0] << ", " << ScintillatorPosition[1] << ", " << ScintillatorPosition[2] << ") " <<G4endl;
#endif
	  ScintillatorPosition[1] += (CellSizeHeight);
	}
	//
	NumberOfLayerPlanHorizontal++;
      }
      NumberOfLayerPlan = NumberOfLayerPlanHorizontal + NumberOfLayerPlanVertical;

    }


    while(1){
	//How to define a layer plan? There is a symmetry of the module here. But we will define one layer as plane along Z direction (scintillator) and Y. We start at X=0 -> X>0
	//First, where is placed the layer? It starts after the first Y scintillator, i.e at X=LayerWidthPlanFirstScintiOffset + ScintiPlanWidth + GapBtwGridPlan + ScintiGridThickness/2 <=> X=LayerWidthPlanFirstScintiOffset + ScintiGridGap - GapBtwGridPlan + ScintiGridThickness/2.
	//For the Y, it will start from after the first X scintillator : LayerHeightPlanFirstScintiOffset + ScintiPlanWidth + GapBtwGridPlan + ScintiGridWidth/2. The step will be then CellGrid (basically, ScintiGridWidth + ScintiPlanWidth + 2*GapBtwGridPlan) <=> starts at LayerHeightPlanFirstScintiOffset + ScintiGridGap - GapBtwGridPlan + ScintiGridWidth/2. and step = CellSizeHeight.
	//For the Z, it will be TargetLength/2.

	double LayerUpstreamEdge = LayerGridOffset + (NumberOfLayerGrid*CellSizeWidth);
	
	//Create the layers from (0,0,center) of the module.
	LayerCenter = LayerUpstreamEdge + LayerGridWidth/2;
	G4ThreeVector FromLayerToTarget (LayerCenter, 0.0*CLHEP::mm, 0.0*CLHEP::mm);

#ifdef DEBUG
	G4cout << "Layer #" << NumberOfLayerGrid << " is of type grid, number of scintillators horizontal grid= " << LayerGridNumberOfScintiHorizontal << ", Position = " << LayerCenter << ", Layer width = " << LayerGridWidth << ", Layer height = " << LayerGridHeight << G4endl;
#endif

	if( (LayerCenter + LayerGridThickness/2) > (TargetWidth - LayerGridOffset) ) break;

	G4ThreeVector ScintillatorPosition = (FromLayerToTarget + FromTargettoGlobal);
	ScintillatorPosition[0] = (FromLayerToTarget[0] + FromTargettoGlobal[0]);
	ScintillatorPosition[1] = (FromLayerToTarget[1] + FromTargettoGlobal[1]) + LayerHeightGridFirstScintiOffset + ScintiPlanWidth/2;
	ScintillatorPosition[2] = (FromLayerToTarget[2] + FromTargettoGlobal[2]) + TargetThickness/2.;
	//if( (LayerCenter + LayerGridThickness/2) > (TargetThickness - LayerOffset) ) break;

	//horizontal
	for(int i=0; i<LayerGridNumberOfScintiHorizontal;i++){//Loop over scintillators
	  int id=m*NumberOfModules+NumberOfLayerGrid*MaxNumberOfLayerGrid+i;
	  new G4PVPlacement(0, ScintillatorPosition, ScintiHorizontalGridLV,RepositoryName+Form("LayerGridHorizontalScinti[%d][%d][%d]",m,NumberOfLayerGrid,i),TargetLV,false,id,false);
#ifdef DEBUG
	  G4cout << "Horizontal grid, Scintillator position = (" << ScintillatorPosition[0] << ", " << ScintillatorPosition[1] << ", " << ScintillatorPosition[2] << ") " <<G4endl;
#endif
	  ScintillatorPosition[1] += CellSizeHeight;
	}
	//
	NumberOfLayerGrid++;
	//

	if( (NumberOfLayerPlanHorizontal>MaxNumberOfLayerPlanHorizontal) ||(NumberOfLayerPlanVertical>MaxNumberOfLayerPlanVertical) ||(NumberOfLayerGrid>MaxNumberOfLayerGrid)){ G4cout << "The number of maximal plans in Waffle code is too small. Change it manually please in the src/FGD3D/ND280WaffleActiveConstructor.cc!" << endl;}
	
    }
 }
 
 //
 // D.S
 //
 // Get the SD defined in ExN02DetectorConstrucion  
 G4SDManager* SDman = G4SDManager::GetSDMpointer();
 ExN02TrackerSD *aTrackerSD = (ExN02TrackerSD*)SDman->FindSensitiveDetector("ExN02/Tracker"); 
 ScintiHorizontalPlanLV->SetSensitiveDetector( aTrackerSD );
 ScintiVerticalPlanLV->SetSensitiveDetector( aTrackerSD );
 ScintiHorizontalGridLV->SetSensitiveDetector( aTrackerSD );
 //
 
 //
 // D.S 
 //
 // Take the visualization attributes from input XML file
 //
 G4VisAttributes* ScintVisAtt = new G4VisAttributes(G4Color(0.,0.7,0.));
 if( GetND280XML()->GetXMLInvisFGD3D() ){
    ScintiHorizontalPlanLV->SetVisAttributes(G4VisAttributes::Invisible);
    ScintiVerticalPlanLV->SetVisAttributes(G4VisAttributes::Invisible);
    ScintiHorizontalGridLV->SetVisAttributes(G4VisAttributes::Invisible);
  }
  else{
    ScintiHorizontalPlanLV->SetVisAttributes( ScintVisAtt );
    ScintiVerticalPlanLV->SetVisAttributes( ScintVisAtt );
    ScintiHorizontalGridLV->SetVisAttributes( ScintVisAtt );
  }
  /////////

}
