#ifndef IngridDetectorConstruction_H
#define IngridDetectorConstruction_H 1

#include "IngridHLayerSD.hh"
#include "IngridVetoSD.hh"
#include "IngridVLayerSD.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

#include "G4VUserDetectorConstruction.hh"

#define HALL_Z 1.0
#define HALL_XY 1.5
#define nSciVertex 8

class IngridDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  void DefineMaterial();
  void DefineSpace();
  void DefineStructures();

  G4Material *Air, *Fe, *Scinti, *Water, *Concrete, *Alu;//B2 add Alu
  G4double WorldX, WorldY, WorldZ;
  G4double HallDirtRadiusMin, HallDirtRadiusMax, HallDirtHeight,
           HallDirtSPhi, HallDirtDPhi; //## Dirt Dimension
  G4double HallDirtX, HallDirtY, HallDirtZ; //## Dirt position
  G4double HallX, HallZ; //## center position of Hall
  G4double Floorup; 
  G4double PillarX,PillarY,PillarZ; 
  G4double PillarposX,PillarposY,PillarposZ,PillarposX_rev; 

  G4double HorizonX,HorizonY, HorizonZ, PHorizonZ;
  G4double ModuleX, ModuleY, ModuleZ;
  G4double ModuleSpace, ModuleStart;
  G4double iron_z,iron_xy, Gap, iron_start;
  G4double scibar_x, scibar_y,scibar_z, scibar_start, scibar_xy_start,Pscibar_start;
  G4double Lveto_x, Lveto_y, Lveto_z, veto_start, Pveto_start; 
  G4double Sveto_x, Sveto_y, Sveto_z;
  G4double Niron_start, Niron_z, Nscibar_start, NGap, NModuleSpace;
  G4double SciVertex_x[nSciVertex], SciVertex_y[nSciVertex];
  G4double SciVertex2_x[nSciVertex], SciVertex2_y[nSciVertex];
  G4double SciVertex3_x[nSciVertex], SciVertex3_y[nSciVertex];



  //for Proton Module added by kikawa
  G4double PModuleZ;
  G4double scibar2_y,scibar2_z;//scibar_start, scibar_xy_start;
  G4double PLveto_x, PLveto_y, PLveto_z;
  G4double PSveto_x, PSveto_y, PSveto_z;
  G4double dist_first,dist;

  //wagaschi
  G4double WagaX,WagaY,WagaZ;
  G4double WagaposX,WagaposY,WagaposZ;

  G4double WagaboxX,WagaboxY,WagaboxZ;
  G4double WagaboxforX,WagaboxforY,WagaboxforZ;
  G4double WagaboxsideX,WagaboxsideY,WagaboxsideZ;
  G4double WagaboxbackX,WagaboxbackY,WagaboxbackZ;
  G4double WagaboxbottomX,WagaboxbottomY,WagaboxbottomZ;
  G4double WagaboxposX,WagaboxposY,WagaboxposZ;
  G4double WagaboxforposX,WagaboxforposY,WagaboxforposZ;
  G4double WagaboxsideposX,WagaboxsideposY,WagaboxsideposZ;
  G4double WagaboxbackposX,WagaboxbackposY,WagaboxbackposZ;
  G4double WagaboxbottomposX,WagaboxbottomposY,WagaboxbottomposZ;

  G4double Iron2X,Iron2Y,Iron2Z;
  G4double Iron3X,Iron3Y,Iron3Z;
  G4double Iron4X,Iron4Y,Iron4Z;

  G4double Hlayer2X,Hlayer2Y,Hlayer2Z;
  G4double Hlayer3X,Hlayer3Y,Hlayer3Z;
  G4double Hlayer4X,Hlayer4Y,Hlayer4Z;
  G4double Hlayer5X,Hlayer5Y,Hlayer5Z;
  G4double Hlayer6X,Hlayer6Y,Hlayer6Z;
  G4double Hlayer7X,Hlayer7Y,Hlayer7Z;
  G4double Hlayer8X,Hlayer8Y,Hlayer8Z;
  G4double Hlayer9X,Hlayer9Y,Hlayer9Z;

  G4double Vlayer2X,Vlayer2Y,Vlayer2Z;
  G4double Vlayer3X,Vlayer3Y,Vlayer3Z;
  G4double Vlayer4X,Vlayer4Y,Vlayer4Z;
  G4double Vlayer5X,Vlayer5Y,Vlayer5Z;
  G4double Vlayer6X,Vlayer6Y,Vlayer6Z;
  G4double Vlayer7X,Vlayer7Y,Vlayer7Z;
  G4double Vlayer8X,Vlayer8Y,Vlayer8Z;
  G4double Vlayer9X,Vlayer9Y,Vlayer9Z;

  G4double VetoGap,MRDGap,UpstGap;

  G4double StartVetoupst_z,StartVetoupst_v,StartVetoupst_h;
  G4double StartVetodown_z,StartVetodown_v,StartVetodown_h;
  G4double StartVetoright_z,StartVetoright_v,StartVetoright_h;
  G4double StartVetoleft_z,StartVetoleft_v,StartVetoleft_h;
  G4double StartVetobottom_z,StartVetobottom_v,StartVetobottom_h;
  G4double StartVetoup_z,StartVetoup_v,StartVetoup_h;

  G4double Startmod1_iron_z;
  G4double Startmod1_sci_z;
  G4double Startmod1_sci_v;
  G4double Startmod1_sci_h;

  G4double Startmod2_iron_z;
  G4double Startmod2_sci_z;
  G4double Startmod2_sci_v;
  G4double Startmod2_sci_h;

  G4double Startmod3_sci_z;
  G4double Startmod3_sci_v;
  G4double Startmod3_sci_h;

  G4double Startmod9_sci_z;
  G4double Startmod9_sci_v;
  G4double Startmod9_sci_h;

  G4double offset_mod5_v;
  G4double offset_mod5_h;
  G4double offset_mod5_vbar;
  G4double offset_mod5_hbar;


  IngridVetoSD* avetoSD;
  IngridHLayerSD* ahlayerSD;  
  IngridVLayerSD* avlayerSD; 
 
  int flag;
  int mode;

  IngridDetectorConstruction(int);
  //IngridDetectorConstruction();
  ~IngridDetectorConstruction();
  
  
  G4VPhysicalVolume* Construct();
 
};

#endif
