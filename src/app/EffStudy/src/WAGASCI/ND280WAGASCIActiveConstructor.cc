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
#include "ExN02TrackerSD.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"
#include "ND280WAGASCIActiveConstructor.hh"

//#define DEBUG

ND280WAGASCIActiveConstructor::~ND280WAGASCIActiveConstructor() {;}

void ND280WAGASCIActiveConstructor::Init() {
  //G4cout <<"Hello, you are initializing the WAGASCI constructor. However, this Init() function needs to be defined later!" << G4endl;
}

G4LogicalVolume *ND280WAGASCIActiveConstructor::GetPiece(void) {
  // If not initialized it doesn't compile (abstract class)
} // D.S

void ND280WAGASCIActiveConstructor::Construct(G4LogicalVolume * TargetLV, G4ThreeVector ModulePosition, G4ThreeVector ModuleSize, const G4String RepositoryName){

  ND280RootPersistencyManager* persistencyManager
    = ND280RootPersistencyManager::GetInstance();
  fND280XMLInput = persistencyManager->GetXMLInput(); // S.D
  
  //0th, always create the world volume. In this simulation, it is already defined
  //0th, create an Air Volume in which the target will be inserted create a big air volume in which WAGASCI will be inserted.

  	
  //

  G4ThreeVector WAGASCIPosition    ( 0.0*mm, 0.0*mm, 0.0*mm );
  
  //const G4Rotation WAGASCIRotation    (0,0,0);
  G4RotationMatrix WAGASCIRotation (G4ThreeVector(0.,0.,0.),0.);

  //------------------ Target ---------------------

  // Target size
  double TargetWidth = 2300.0 * mm;
  double TargetHeight = 600.0 * mm;
  double TargetThickness = 1300.0 * mm;

  TargetWidth = ModuleSize[0];
  TargetHeight = ModuleSize[1];
  TargetThickness = ModuleSize[2];
  WAGASCIPosition = ModulePosition;
  
  
    
  //------------------ Scintillators ---------------------


  //------------------ Module = Assembly of layers of scintillators ---------------------
  // We will assume that the geometry is planH + grid + planV + grid
  // No submodule is assumed. The plans are created one after another until it is not possible anymore due to target space limitation.
  // As for within one layer, we have added a gap between the layers and also, an offset between the target and the first (and therefore, last) scintillator layer.
  //Offset of the first scintillator of the grid layer in one layer / in the target
  const double LayerOffset = 0.0 * mm; // Offset in Z direction

  //Spacing between a layer of plan and a layer of Grid scintillator.
  const double LayerGap = 1.0 * mm;
  //const double LayerGap = 10.0 * mm;


  //Scintillator's defintition: part I
  //Individual plan scintillator size. 
  double ScintiPlanWidth = 25.0 * mm;
  //double ScintiPlanWidth = 10.0 * mm;
  double ScintiPlanThickness = 3.0 * mm;

  //Individual grid scintillator size.
  double ScintiGridWidth = 25.0 * mm;
  //double ScintiGridWidth = 10.0 * mm;
  double ScintiGridThickness = 3.0 * mm;
  
  //------------------ Layers of scintillators ---------------------
  // We have added a possibility of offset of the first (and therefore, last) scintillator in the layer. Note that there is also Gap between the scintillators.
 
  //Offset of the first scintillator of the plan layer in one layer / in the target
  const double InitialLayerWidthPlanFirstScintiOffset = 0.0 * mm;

  //Offset of the first scintillator of the plan layer in one layer / in the target
  const double InitialLayerHeightPlanFirstScintiOffset = 0.0 * mm;

  //Offset of the first scintillator of the grid horizontal layer in one layer / in the target
  const double InitialLayerWidthGridFirstScintiOffset = 0.0 * mm;

  //Offset of the first scintillator of the grid vertical layer in one layer / in the target
  const double InitialLayerHeightGridFirstScintiOffset = 0.0 * mm;

  //Spacing between plan scintillators (consecutive scintillators in the same layer)
  const double ScintiPlanGap = 0.0 * mm;

  //Spacing between grid scintillators (consecutive scintillators in the same layer)
  const double CellSize = 25.0 * mm;
  //const double CellSize = 10.0 * mm;
  
  //Spacing between grid scintillators (consecutive scintillators in the same layer)
  const double ScintiGridGap = CellSize - ScintiGridThickness;



  //Scintillator's definition: part II
  //For the length
  //Separate for vertical and horizontal
  double ScintiVerticalPlanLength = TargetHeight-2*InitialLayerHeightPlanFirstScintiOffset;
  double ScintiHorizontalPlanLength = TargetWidth-2*InitialLayerWidthPlanFirstScintiOffset;
  double ScintiVerticalGridLength = TargetHeight-2*InitialLayerHeightGridFirstScintiOffset;
  double ScintiHorizontalGridLength = TargetWidth-2*InitialLayerWidthGridFirstScintiOffset;

  //
  const double LayerPlanThickness = ScintiPlanThickness;
  const double LayerGridThickness = ScintiGridWidth;
  const int NumberOfModules=1;//Number of modules/target (water, air etc...)

  //Number of scintillators in a layer of plan scintillators.Note: there is an error here, one scintiplangap is extra counted (edge problem). So I added the test below
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

  G4cout << "Vertical grid layer (=vertical scintillator):" << G4endl << "TargetHeight=" << TargetHeight << ", Layer Height = " << LayerGridHeight << ", Initial layer offset = "<< InitialLayerHeightGridFirstScintiOffset <<", final layer offset = " << LayerHeightGridFirstScintiOffset << G4endl; 
  G4cout << "Test of the grid vertical scintillator length = " << ScintiVerticalGridLength << " and if math are correct, should be equal to " << LayerGridHeight + 2*(LayerHeightGridFirstScintiOffset-InitialLayerHeightGridFirstScintiOffset) << G4endl;
  G4cout << "Horizontal grid layer (=horizontal scintillator):" << G4endl << "TargetWidth=" << TargetWidth << ", Layer Width = " << LayerGridWidth << ", Initial layer offset = "<< InitialLayerWidthGridFirstScintiOffset <<", final layer offset = " << LayerWidthGridFirstScintiOffset << G4endl; 
  G4cout << "Test of the grid horizontal scintillator length = " << ScintiHorizontalGridLength << " and if math are correct, should be equal to " << LayerGridWidth + 2*(LayerWidthGridFirstScintiOffset-InitialLayerWidthGridFirstScintiOffset) << G4endl;
#endif

  //This is absolutely not the number of plans. Just a maximal bound that is used to set the number ID of every scintillator without any overlap.
  const int MaxNumberOfLayerPlanVertical=500;
  const int MaxNumberOfLayerPlanHorizontal=500;
  const int MaxNumberOfLayerGrid=2*500;

  
 //Number of layers
  //Temporary variable, the thickness of the target that can be used to place layers (so, remove the offset of first and last layer)1.
  //double ModuleThickness = TargetThickness - 2*LayerOffset;
  //int ModuleNumberOfLayers;
  //To define
  int NumberOfLayers = 0.;

  //Grid Scintillator Slits
  //Safety margin (additional distance) for the slit size to avoid overlap. Can be used to mimic also the extrusion done, the real slit size!
  const double SlitSafetyMarginWidth = 0.1 * mm;
  const double SlitSafetyMarginHeight = 0.1 * mm;
  const double SlitSafetyMarginThickness = 0.1 * mm;

  double SlitWidth = ScintiGridThickness + SlitSafetyMarginWidth;
  double SlitHeight = ScintiGridWidth/2. + SlitSafetyMarginHeight;
  double SlitThickness = ScintiGridThickness + SlitSafetyMarginThickness;

  
  /*
  //DefineMaterial();
  //ND280ECalComponents components(GetName());
  if( ((LayerPlanNumberOfScintiHorizontal+1)*(ScintiPlanWidth + ScintiPlanGap)-LayerPlanHeight) <= ScintiPlanGap) LayerPlanNumberOfScintiHorizontal += 1;
  if((LayerPlanNumberOfScintiVertical+1)*(ScintiPlanWidth + ScintiPlanGap) -  LayerPlanWidth <= ScintiPlanGap) LayerPlanNumberOfScintiVertical += 1;
  if((LayerGridNumberOfScintiHorizontal+1)*(ScintiGridThickness + ScintiGridGap) -  LayerGridHeight <= ScintiGridGap) LayerGridNumberOfScintiHorizontal += 1;
  if((LayerGridNumberOfScintiVertical+1)*(ScintiGridThickness + ScintiGridGap) -  LayerGridWidth <= ScintiGridGap) LayerGridNumberOfScintiVertical += 1;
  
*/

#ifdef DEBUG
  G4cout << "WAGASCI: " << G4endl << "Number of modules =" << NumberOfModules << G4endl;
  G4cout << "Center position = (" << WAGASCIPosition[0] << ", "<< WAGASCIPosition[1] << ", "<<  WAGASCIPosition[2] << ")" << G4endl;
  G4cout << "Module size = (" << TargetWidth << ", "<< TargetHeight << ", "<<  TargetThickness << ")" << G4endl;
  
#endif

  // -- The scintillator layers. An assembly of scintillators
  //1. Layer plan horizontal
  //First, create the volume of a template scintillator:
  G4Box * ScintiHorizontalPlanBox = new G4Box("ScintiHorizontalPlanBox",ScintiHorizontalPlanLength/2.,ScintiPlanWidth/2.,ScintiPlanThickness/2.);
  //Second, create the LV of a template scintillator: 
  G4LogicalVolume * ScintiHorizontalPlanLV = new G4LogicalVolume(ScintiHorizontalPlanBox,FindMaterial("WAGASCIScintillator"),"ScintiHorizontalPlanLV"); // D.S

  

  //2. Layer plan vertical
  //First, create the volume of a template scintillator:
  G4Box * ScintiVerticalPlanBox = new G4Box("ScintiVerticalPlanBox",ScintiPlanWidth/2.,ScintiVerticalPlanLength/2.,ScintiPlanThickness/2.);
  //Second, create the LV of a template scintillator: 
  G4LogicalVolume * ScintiVerticalPlanLV = new G4LogicalVolume(ScintiVerticalPlanBox,FindMaterial("WAGASCIScintillator"),"ScintiVerticalPlanLV"); // D.S


  
  //Currently, the problem is that I do not have created a layer LV that allow me to place a layer anywhere I wish. So we will work with scintillator planes instead of layers. So we should create also the LV of other scintillator types.

  //Put a scintillator where there is a slit in fact!
  
  //1. Layer grid horizontal
  //First, create the volume of a template scintillator:
  G4Box * ScintiHorizontalGridExternalBox = new G4Box("ScintiHorizontalGridExternalBox",ScintiHorizontalGridLength/2.,ScintiGridThickness/2.,ScintiGridWidth/2.);
  G4VSolid * ScintiHorizontalGridBox = (G4VSolid*) ScintiHorizontalGridExternalBox ;
  G4VSolid* SlitHorizontalGrid = new G4Box("SlitHorizontalGrid",SlitWidth/2.,SlitThickness/2.,SlitHeight/2.);
  int NumberOfSlitsHorizontal = LayerGridNumberOfScintiVertical;

  //Iterate the slit removal in fact. Take the box, remove one slit, update the box with the box having one slit removed, remove another slit etc...
#ifdef DEBUG
  G4cout << "Horizontal GRID scintillator, dimensions = (" << ScintiHorizontalGridExternalBox->GetXHalfLength()*2 << ", " << ScintiHorizontalGridExternalBox->GetYHalfLength()*2 << ", " << ScintiHorizontalGridExternalBox->GetZHalfLength()*2 << ") " << G4endl;
  G4cout << "Number of slits = " << NumberOfSlitsHorizontal << ", Dimensions of the slits = ("<< SlitWidth << ", " << SlitThickness << ", " << SlitHeight << ") " << G4endl;
#endif
  double PositionAlongBar = LayerWidthGridFirstScintiOffset - InitialLayerWidthGridFirstScintiOffset + ScintiGridThickness/2 - ScintiHorizontalGridExternalBox->GetXHalfLength();
  
  for(int i=0;i<NumberOfSlitsHorizontal;i++){
    //Careful, the scintillator is already turned! It is not a plane scintillator but a grid one!
    G4ThreeVector SlitPosition (PositionAlongBar,0.,ScintiHorizontalGridExternalBox->GetZHalfLength()/2.);//x is iterated, y is always 50% of the scintillator thickness, z is always 75% of the scintillator width
    G4VSolid * temp =  new G4SubtractionSolid("",ScintiHorizontalGridBox,SlitHorizontalGrid,0,SlitPosition);
    ScintiHorizontalGridBox = temp;
#ifdef DEBUG
    G4cout << "Slit position = (" << SlitPosition[0] << ", " << SlitPosition[1] << ", " << SlitPosition[2] << ")" << G4endl;
#endif
    PositionAlongBar += CellSize;//Always relative to the bar.
  }

  //Second, create the LV of a template scintillator: 
  G4LogicalVolume * ScintiHorizontalGridLV = new G4LogicalVolume(ScintiHorizontalGridBox,FindMaterial("WAGASCIScintillator"),"ScintiHorizontalGridLV"); // D.S

  //2. Layer grid vertical
  //First, create the volume of a template scintillator:
  G4Box * ScintiVerticalGridExternalBox = new G4Box("ScintiVerticalGridExternalBox",ScintiGridThickness/2.,ScintiVerticalGridLength/2.,ScintiGridWidth/2.);
  G4VSolid * ScintiVerticalGridBox = (G4VSolid*) ScintiVerticalGridExternalBox;
  G4VSolid* SlitVerticalGrid = new G4Box("SlitVerticalGrid",SlitThickness/2.,SlitWidth/2.,SlitHeight/2.);
  int NumberOfSlitsVertical = LayerGridNumberOfScintiHorizontal;

  //Iterate the slit removal in fact. Take the box, remove one slit, update the box with the box having one slit removed, remove another slit etc...
#ifdef DEBUG
  G4cout << "Vertical GRID scintillator, dimensions = (" << ScintiVerticalGridExternalBox->GetXHalfLength()*2 << ", " << ScintiVerticalGridExternalBox->GetYHalfLength()*2 << ", " << ScintiVerticalGridExternalBox->GetZHalfLength()*2 << ") " << G4endl;
  G4cout << "Number of slits = " << NumberOfSlitsVertical << ", Dimensions of the slits = ("<< SlitThickness << ", " << SlitWidth << ", " << SlitHeight << ") " << G4endl;
#endif
  
  PositionAlongBar = LayerHeightGridFirstScintiOffset - InitialLayerHeightGridFirstScintiOffset + ScintiGridThickness/2 - ScintiVerticalGridExternalBox->GetYHalfLength();
  for(int i=0;i<NumberOfSlitsVertical;i++){
    //Careful, the scintillator is already turned! It is not a plane scintillator but a grid one!
    G4ThreeVector SlitPosition (0.,PositionAlongBar,-ScintiVerticalGridExternalBox->GetZHalfLength()/2.);//x is iterated, y is always 50% of the scintillator thickness, z is always 25% of the scintillator width
    //It should be 25% to be at the start of the scintillator, and therefore, overlaps with the 75% of horizontal scintillators
    G4VSolid * temp =  new G4SubtractionSolid("",ScintiVerticalGridBox,SlitVerticalGrid,0,SlitPosition);
    ScintiVerticalGridBox = temp;
#ifdef DEBUG
    G4cout << "Slit position = (" << SlitPosition[0] << ", " << SlitPosition[1] << ", " << SlitPosition[2] << ")" << G4endl;
#endif
    PositionAlongBar += CellSize;//Always relative to the bar.
  }

  G4LogicalVolume * ScintiVerticalGridLV = new G4LogicalVolume(ScintiVerticalGridBox,FindMaterial("WAGASCIScintillator"),"ScintiVerticalGridLV"); // D.S

   /*
   //First, create the volume of a template scintillator:
   G4VSolid * ScintiGridBox = new G4Box("ScintiGridBox",ScintiGridLength/2.,ScintiGridWidth/2.,ScintiGridThickness/2.);
   //Second, create the LV of a template scintillator: 
   G4LogicalVolume * ScintiGridLV = new G4LogicalVolume(ScintiGridBox,FindMaterial("WAGASCIScintillator"),"ScintiGridLV"); // D.S
  */
   
  //Third, place them <=> Create a physical volume => Create a layer
  double LayerCenter = 0.;
  G4ThreeVector FromLayertoTarget (0.0*mm, 0.0*mm, LayerCenter);
  G4ThreeVector FromTargettoGlobal (/*WAGASCIPosition[0]*/-TargetWidth/2.,/*WAGASCIPosition[1]*/-TargetHeight/2.,/*WAGASCIPosition[2]*/-TargetThickness/2.);
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
      int PlanType=NumberOfLayers%4;//0=plan vertical, 1=grid, 2=plan horizontal, 3=grid

      if(PlanType==3) PlanType=1;//Since for now, all grid planes are strictly equivalent (the "first" and "second" are the same)

      if(PlanType==0){//layer plan vertical
	//Second, create and place the scintillators
	LayerCenter = LayerUpstreamEdge + LayerPlanThickness/2;
	G4ThreeVector FromLayerToTarget (0.0*mm, 0.0*mm, LayerCenter);	
#ifdef DEBUG
	G4cout << "Layer #" << NumberOfLayers << " is of type 2D horizontal, number of scintillators = " << LayerPlanNumberOfScintiVertical << G4endl;
#endif
	G4ThreeVector ScintillatorPosition = (FromLayerToTarget + FromTargettoGlobal);
	ScintillatorPosition[0] = FromLayerToTarget[0] + FromTargettoGlobal[0] + LayerWidthPlanFirstScintiOffset + ScintiPlanWidth/2;
	ScintillatorPosition[1] = FromLayerToTarget[1] + FromTargettoGlobal[1] + TargetHeight/2.;
	ScintillatorPosition[2] = FromLayerToTarget[2] + FromTargettoGlobal[2];
	
	//Assuming the MPPC are placed on the top. This is in the local coordinate frame of each scintillator
	G4ThreeVector MPPCPosition;
	MPPCPosition[0]= 0;
	MPPCPosition[1]= ScintiVerticalPlanLength/2;
	MPPCPosition[2]= 0;
	
	if( (LayerCenter + LayerPlanThickness/2) > (TargetThickness - LayerOffset) ) break;
	   
	for(int i=0; i<LayerPlanNumberOfScintiVertical;i++){//Loop over scintillators
	  int id=m*NumberOfModules+NumberOfLayerPlanVertical*MaxNumberOfLayerPlanVertical+i;
	  new G4PVPlacement(0, ScintillatorPosition, ScintiVerticalPlanLV,RepositoryName+Form("LayerPlanVerticalScinti[%d][%d][%d]",m,NumberOfLayerPlanVertical,i),TargetLV,false,id,false);
	  persistencyManager->AddPhysicalVolumeInformation(id,ScintiVerticalPlanLV->GetName(),MPPCPosition);
	  
#ifdef DEBUG
	  G4cout << "Scintillator ID = " << id << ", position = (" << ScintillatorPosition[0] << ", " << ScintillatorPosition[1] << ", " << ScintillatorPosition[2] << ") " <<G4endl;
#endif
	  ScintillatorPosition[0] += (ScintiPlanWidth + ScintiPlanGap);
	}
	//
	NumberOfLayerPlanVertical++;
      }
      else if(PlanType==1){//layer grid
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
	//Assuming the MPPC are placed on the top.
	G4ThreeVector MPPCPosition;
	MPPCPosition[0]= 0;
	MPPCPosition[1]= ScintiVerticalGridLength/2;
	MPPCPosition[2]= 0;

	//vertical
	for(int i=0; i<LayerGridNumberOfScintiVertical;i++){//Loop over scintillators
	  int id=m*NumberOfModules+NumberOfLayerGrid*MaxNumberOfLayerGrid+i;
	  new G4PVPlacement(0, ScintillatorPosition, ScintiVerticalGridLV,RepositoryName+Form("LayerGridVerticalScinti[%d][%d][%d]",m,NumberOfLayerGrid,i),TargetLV,false,id,false);
	  persistencyManager->AddPhysicalVolumeInformation(id,ScintiVerticalGridLV->GetName(),MPPCPosition);

#ifdef DEBUG
	  G4cout << "Vertical grid, Scintillator ID = " << id << ", position = (" << ScintillatorPosition[0] << ", " << ScintillatorPosition[1] << ", " << ScintillatorPosition[2] << ") " <<G4endl;
#endif
	  ScintillatorPosition[0] += CellSize;
	}
	
	ScintillatorPosition[0] = (FromLayerToTarget[0] + FromTargettoGlobal[0]) + TargetWidth/2.;
	ScintillatorPosition[1] = (FromLayerToTarget[1] + FromTargettoGlobal[1]) + LayerHeightGridFirstScintiOffset + ScintiGridThickness/2;
	ScintillatorPosition[2] = (FromLayerToTarget[2] + FromTargettoGlobal[2]);
	//Assuming the MPPC are placed on the left when we are in the reference frame of the neutrino beam
	MPPCPosition[0]= ScintiHorizontalGridLength/2;
	MPPCPosition[1]= 0.;
	MPPCPosition[2]= 0.;

	//horizontal
	for(int i=0; i<LayerGridNumberOfScintiHorizontal;i++){//Loop over scintillators
	  int id=m*NumberOfModules+NumberOfLayerGrid*MaxNumberOfLayerGrid+i;
	  new G4PVPlacement(0, ScintillatorPosition, ScintiHorizontalGridLV,RepositoryName+Form("LayerGridHorizontalScinti[%d][%d][%d]",m,NumberOfLayerGrid,i),TargetLV,false,id,false);
	  persistencyManager->AddPhysicalVolumeInformation(id,ScintiHorizontalGridLV->GetName(),MPPCPosition);

#ifdef DEBUG
	  G4cout << "Horizontal grid, Scintillator ID = " << id << ", position = (" << ScintillatorPosition[0] << ", " << ScintillatorPosition[1] << ", " << ScintillatorPosition[2] << ") " <<G4endl;
#endif
	  ScintillatorPosition[1] += CellSize;
	}
	//
	NumberOfLayerGrid++;
      }
      else if(PlanType==2){
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

	//Assuming the MPPC are placed on the left when we are in the reference frame of the neutrino beam
	G4ThreeVector MPPCPosition;
	MPPCPosition[0]= ScintiHorizontalPlanLength/2;
	MPPCPosition[1]= 0;
	MPPCPosition[2]= 0;

	for(int i=0; i<LayerPlanNumberOfScintiHorizontal;i++){//Loop over scintillators
	  int id=m*NumberOfModules+NumberOfLayerPlanHorizontal*MaxNumberOfLayerPlanHorizontal+i;
	  new G4PVPlacement(0, ScintillatorPosition, ScintiHorizontalPlanLV,RepositoryName+Form("LayerPlanHorizontalScinti[%d][%d][%d]",m,NumberOfLayerPlanHorizontal,i),TargetLV,false,0,false);
	  persistencyManager->AddPhysicalVolumeInformation(id,ScintiHorizontalPlanLV->GetName(),MPPCPosition);

#ifdef DEBUG
	  G4cout << "Scintillator ID = " << id << ", position = (" << ScintillatorPosition[0] << ", " << ScintillatorPosition[1] << ", " << ScintillatorPosition[2] << ") " <<G4endl;
#endif
	  ScintillatorPosition[1] += (ScintiPlanWidth + ScintiPlanGap);
	}
	//
	NumberOfLayerPlanHorizontal++;
      }
      NumberOfLayers = NumberOfLayerPlanHorizontal + NumberOfLayerPlanVertical + NumberOfLayerGrid;
      //if(NumberOfLayers==2) break;
    }
    if( (NumberOfLayerPlanHorizontal>MaxNumberOfLayerPlanHorizontal) ||(NumberOfLayerPlanVertical>MaxNumberOfLayerPlanVertical) ||(NumberOfLayerGrid>MaxNumberOfLayerGrid)){ G4cout << "The number of maximal plans in WAGASCI code is too small. Change it manually please in the src/WAGASCI/ND280WAGASCIActiveConstructor.cc!" << endl;}

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
 ScintiVerticalGridLV->SetSensitiveDetector( aTrackerSD );
 //


 //
 // D.S 
 //
 // Take the visualization attributes from input XML file
 //

 G4VisAttributes* ScintVisAtt = new G4VisAttributes(G4Color(0.,0.7,0.));
 
  if( GetND280XML()->GetXMLInvisWAGASCI() ){
    ScintiHorizontalPlanLV->SetVisAttributes(G4VisAttributes::Invisible);
    ScintiVerticalPlanLV->SetVisAttributes(G4VisAttributes::Invisible);
    ScintiHorizontalGridLV->SetVisAttributes(G4VisAttributes::Invisible);
    ScintiVerticalGridLV->SetVisAttributes(G4VisAttributes::Invisible);
  }
  else{
    ScintiHorizontalPlanLV->SetVisAttributes( ScintVisAtt );
    ScintiVerticalPlanLV->SetVisAttributes( ScintVisAtt );
    ScintiHorizontalGridLV->SetVisAttributes( ScintVisAtt );
    ScintiVerticalGridLV->SetVisAttributes( ScintVisAtt );
  }
  /////////
  
}
