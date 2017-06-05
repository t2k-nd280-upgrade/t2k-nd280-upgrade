#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RandGauss.h"
#include "Randomize.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "globals.hh"

#include "IngridPrimaryGeneratorAction.hh"
#include "IngridDetectorConstruction.hh"
#include "IngridRunAction.hh"

#define PROTON 2
#define HORIZONTAL 3
#define VERTICAL 4

#define MOD_CEN_GAP 150.	// the gap of each module center = 60(half of module)*2cm + 30(module gap)cm
#define OFFSET 400.		// Gap between horizotal modules and vertical modules (cm)
#define OFFSETPM 120. 	// Gap between horizotal modules and proton modules (cm)

// IRON 120x120x6.5 -------------------------

//
const double total_mass_fe = 99.54; //ton
const double total_mass_sci = 3.74; //ton
const double scinti_start = -54.5;  // the surface of the first scintillator by akira
const double iron_start = scinti_start + 2.0 + 1.1;  // the surface fo the first scinti + 2 scnti + air gap  by akira
const double width_fe = 6.5; // cm
const double width_sci = 2.0; // 1.0cm * 2
const double GAP = 10.7; // 6.5 + 2 + 2.2

const double HallX = -216.7; //cm 
const double HallZ = 170.;    //cm
//const double HallRadiusMax = 1320.;//cm
const double HallRadiusMax = 1150.;//cm
const double HallRadiusMin = 850.;//cm

//for Proton Module added by kikawa
const double total_mass_sci_pm = 0.556 ;//ton (total mass of ingrid type)
const double total_mass_front_pm = 0 ;//ton (total mass of ingrid type)
const double ingrid_width = 1 ;//(cm) (total mass of ingrid type)
const double scibar_width = 1.3; //(cm) (total mass of scibar type)
const double width_ingrid =1.0; //INGRID type 
const double width_scibar =1.3; //SciBar type 
const double scibar_region=44.2;//(cm)
const double sciing_region=39.1;//(cm)
const double ingrid_region=34;//(cm)
const double Pscinti_start=-40.95;//(cm)
const double distance_first=2.7;//(cm)
const double distance_pln=2.3;//(cm)
const double diff=-0.15;//(cm) difference of scibar and ingrid type start


//#define MOD3
//#define DEBUG 
//#define DEBUG2 

//
IngridPrimaryGeneratorAction::
IngridPrimaryGeneratorAction(Neut *neut0,IngridRunAction* rac, IngridEventAction* evt,int nd,int flavor0)
  :runaction(rac)
{
  eventaction = evt;
  neut_fe = neut0;
  module_mode = nd;
  neutrino_flavor = flavor0;
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);
  particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;

  runaction->NotEntry = 0;
}

//
IngridPrimaryGeneratorAction::~IngridPrimaryGeneratorAction()
{
  if( particleGun!=NULL ) { delete particleGun; particleGun=NULL; }
}

//
void IngridPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  SecondaryVector Secondary;
  Neut *neut = neut_fe;
  int fdid = 0;
  int mode = 0;
  float pos[3];
  int ID=-1;
  float direction[3];
  int vertex_flag=0;
  int flayer;
  double prob;//for Proton Module
  int scitype;//scintillator type

  // start loop of neut file
  while(1){

    mode = neut->NtupleReadEvent(Secondary,direction);
    if( mode==-1111 ){
      G4cout <<"Aboart Run (mode =" << mode << G4endl;
      G4RunManager* runManager = G4RunManager::GetRunManager();
      eventaction->SetWriteFlag(-1); 
      runManager->AbortRun(1);
      return;
    }

    // Check flavor ====================
    int  neutrino_flavor_tmp =  (int)(((neut->Vector).Neutrino.ProductionMode)/10);
    if( neutrino_flavor_tmp != neutrino_flavor ) {
#ifdef DEBUG
      G4cout << " === This neutrino id : " << neutrino_flavor_tmp
             << " ,not selected now === " << G4endl;
#endif
      continue;
    }

  // Define neutrino interaction vertex
    fdid = (neut->Vector).Neutrino.FDID;

    // X-Y vertex
    pos[0] = (neut->Vector).Neutrino.x;
    pos[1] = (neut->Vector).Neutrino.y;

    // Z-Vertex for INGRID Signal MC
    if( fdid==3 || fdid==4 ) {

      // select vertex in Fe or scinti ?
      double ratio = total_mass_sci / (total_mass_fe + total_mass_sci);
      if( ratio > G4UniformRand() ) vertex_flag = 1; // flag = 1 -> vertex in scinti
      else vertex_flag = 0;
      
      //the origin is the center of the horizontal modules.
      // vertcical moduleds center <---4m---> horizontal modules center 
      
      // iron#/module is 0 ~ 8, scinti#/module is 0~10
      // unit of pos is [cm];
      if( vertex_flag==0 ) {
        flayer = (int)(9*G4UniformRand());      // 0 < 9*rand < 9
        pos[2] = width_fe*G4UniformRand();
        pos[2] = pos[2] + iron_start + GAP*flayer;    
      }
      else if( vertex_flag==1 ) {
        flayer = (int)(11*G4UniformRand());     // 0 < 11*rand < 11
        pos[2] = width_sci*G4UniformRand();
        pos[2] = pos[2] + scinti_start + GAP*flayer;
      }
    }

    //Proton Module
    if( fdid==2 ) {

      double front =  total_mass_front_pm / (total_mass_front_pm + total_mass_sci_pm);
    
      if( front > (G4UniformRand()) ){
	vertex_flag=0;
	prob=sciing_region/scibar_region;
      }
      else if(fabs(pos[0])<=20&&fabs(pos[1])<=20){
	vertex_flag=1;
	prob=1;
      }
      else if(fabs(pos[0])<=20){
	vertex_flag=2;
	prob=sciing_region/scibar_region;
      }      
      else if(fabs(pos[1])<=20){
	vertex_flag=3;
	prob=sciing_region/scibar_region;
      }      
      else{
	vertex_flag=4;
	prob=ingrid_region/scibar_region;
      }


      if(prob<(G4UniformRand()))continue;

      
      // unit of pos is [cm];
      if( vertex_flag==0 ) {
        flayer = (int)(2*(G4UniformRand()));     // 0 < 2*rand < 2
	pos[2] = width_ingrid*(G4UniformRand());
        pos[2] = pos[2] + Pscinti_start + flayer*distance_pln;    
      }
      else if( vertex_flag==1 ) {
        flayer = (int)(34*(G4UniformRand()));     // 0 < 34*rand < 34
        pos[2] = width_scibar*(G4UniformRand());
        pos[2] = pos[2] + Pscinti_start + distance_pln + diff + distance_first + flayer*distance_pln;
      }
      else if( vertex_flag==2){
	scitype= (int)((G4UniformRand())/scibar_width*(ingrid_width+scibar_width));
	flayer = (int)(17*(G4UniformRand()));     // 0 < 17*rand < 17
	if(scitype==0){
	  pos[2] = width_scibar*(G4UniformRand());
	  pos[2] = pos[2] + Pscinti_start + 2*distance_pln + diff + distance_first + flayer*distance_pln*2;
	}
	else{
	  pos[2] = width_ingrid*(G4UniformRand());
	  pos[2] = pos[2] + Pscinti_start + distance_pln + distance_first + flayer*distance_pln*2;
	}
      }
      else if( vertex_flag==3){
	scitype= (int)((G4UniformRand())/scibar_width*(ingrid_width+scibar_width));
	flayer = (int)(17*(G4UniformRand()));     // 0 < 17*rand < 17
	if(scitype==0){
	  pos[2] = width_scibar*(G4UniformRand());
	  pos[2] = pos[2] + Pscinti_start + distance_pln + diff + distance_first + flayer*distance_pln*2;
	}
	else{
	  pos[2] = width_ingrid*(G4UniformRand());
	  pos[2] = pos[2] + Pscinti_start + 2*distance_pln + distance_first + flayer*distance_pln*2;
	}
      }
      else if( vertex_flag==4){
	flayer = (int)(34*(G4UniformRand()));     // 0 < 34*rand < 34
        pos[2] = width_ingrid*(G4UniformRand());
        pos[2] = pos[2] + Pscinti_start + distance_pln + distance_first + flayer*distance_pln;
      }


    }





    // jnubeam ndid = 7 (INGRID-upstream surface for BG study)
    else if(fdid==7){ // jnubeam ndid = 7(INGRID-upstream surface for BG study
      G4double lx = pos[0]- HallX;
      if( fabs(lx) < HallRadiusMin ){
	pos[2] = -1.0 * sqrt( HallRadiusMin * HallRadiusMin - lx * lx ) - G4UniformRand()*(HallRadiusMax-HallRadiusMin)+HallZ;
      }
      else{
	pos[2] = -1.0 *  G4UniformRand()*(HallRadiusMax-HallRadiusMin)+HallZ;
	//pos[2] = -1.*G4UniformRand()*(HallRadiusMax-HallRadiusMin)/370. * (1261.7-fabs(lx))+HallZ;
      }
    }






    // define vertex module
    ID = -1;
#ifdef MOD3 
     // for one module test
	if(fabs(pos[0])<60  &&  fabs(pos[1])<60){
		ID = 3;
		goto NEXTSTEP;
	}
#endif

#ifndef MOD3 
    if( fdid == 7 ) goto NEXTSTEP;

    //


    if( fdid==2 && module_mode == PROTON ) {
      for( int m=0;m<1;m++ ) {
        if( fabs(pos[0]) <= 60 &&
            fabs(pos[1]) <= 60 ) {
          ID = m+16;
          goto NEXTSTEP;
        }
      }
    }
    if( fdid==3 && module_mode == HORIZONTAL ) {
      for( int m=0;m<7;m++ ) {
        if( fabs(pos[0]+MOD_CEN_GAP*(3-m)) <= 60 &&
            fabs(pos[1]) <= 60 ) {
          ID = m;
          goto NEXTSTEP;
        }
      }
    }
    if( fdid==4 && module_mode == VERTICAL ) {
      for( int m=0;m<7;m++ ) {
        if( fabs(pos[0]) <= 60 &&
            fabs(pos[1]+MOD_CEN_GAP*(3-m)) <= 60 ) {
          ID = m+7;
          goto NEXTSTEP;
        }
      }
    }


#endif

    // count events which have vertex out of modules
#ifdef DEBUG
    G4cout << "##### Interaction vertex is out of modules #####" << G4endl;
    G4cout << "##### Skip this event                      #####" << G4endl;
#endif
    runaction->NotEntry++; 

  } // end while loop

//
NEXTSTEP:

// Gap between horizotal modules and vertical modules (cm) 
    if(ID > 6 && ID < 14)  pos[2] = pos[2] - OFFSET;
// Gap between horizotal modules and proton modules (cm) 
    else if(ID == 16)  pos[2] = pos[2] - OFFSETPM;

    // Input Neut file info to output ROOT class
    neut->ID = ID;
    for(int i=0;i<3;i++) (runaction->vertex)[i] = pos[i];

    IngridSimVertexSummary* simvertex = new IngridSimVertexSummary();
    simvertex -> Clear();
    simvertex -> nutype   = neutrino_flavor;
    simvertex -> inttype  = (neut->Vector).Primary.Mode;
    simvertex -> nuE      = (neut->Vector).Neutrino.Energy;
    simvertex -> xnu      = pos[0];
    simvertex -> ynu      = pos[1];
    simvertex -> znu      = pos[2];
    simvertex -> mod      = ID;
		simvertex -> norm			= (neut->Vector).Neutrino.Norm;
		simvertex -> totcrsne	= (neut->Vector).neutcrs.Totcrsne;
    simvertex -> ng = (neut->Vector).Neutrino.ancestor.ng;
		for(int i=0;i<(neut->Vector).Neutrino.ancestor.ng;i++) {
      simvertex -> gpid.push_back   ( (neut->Vector).Neutrino.ancestor.gpid[i] );
      simvertex -> gmec.push_back   ( (neut->Vector).Neutrino.ancestor.gmec[i] );
      simvertex -> gcosbm.push_back ( (neut->Vector).Neutrino.ancestor.gcosbm[i] );
      simvertex -> gposx.push_back  ( (neut->Vector).Neutrino.ancestor.gvx[i] );
      simvertex -> gposy.push_back  ( (neut->Vector).Neutrino.ancestor.gvy[i] );
      simvertex -> gposz.push_back  ( (neut->Vector).Neutrino.ancestor.gvz[i] );
      simvertex -> gmomx.push_back  ( (neut->Vector).Neutrino.ancestor.gpx[i] );
      simvertex -> gmomy.push_back  ( (neut->Vector).Neutrino.ancestor.gpy[i] );
      simvertex -> gmomz.push_back  ( (neut->Vector).Neutrino.ancestor.gpz[i] );
    }

    runaction  -> GetEvtSum() -> AddSimVertex( simvertex );

    G4cout.precision( 3 );

#ifdef DEBUG
    G4cout << "\n=== Neutrino Information from Jnubeam ===" << G4endl;
		G4cout << "Norm: " <<  (neut->Vector).Neutrino.Norm << G4endl;
		G4cout << "Totcrsne: " <<  (neut->Vector).neutcrs.Totcrsne << G4endl;
    G4cout << "ParentID: " << (neut->Vector).Neutrino.ParentID;
    G4cout << "  Neut Production Mode: " << (neut->Vector).Neutrino.ProductionMode;
    G4cout << "  Neutrino.FDID: " << (neut->Vector).Neutrino.FDID << G4endl;
    G4cout << "Neut interaction Mode: " << (neut->Vector).Primary.Mode << G4endl;
    G4cout << "Energy[GeV]: " << (neut->Vector).Neutrino.Energy;
    G4cout << "  Direction: {" << direction[0] << "," << direction[1] << "," << direction[2] << "}" << G4endl;
    G4cout << "Vertex(cm): {" << pos[0] << ", "<< pos[1] << ", "<< pos[2] << "}";
    G4cout << "  Module: " << ID << "\n\n";
#endif

	particleGun->SetParticlePosition(G4ThreeVector(pos[0]*cm,pos[1]*cm,pos[2]*cm));

  // #############################################################################
  // ### Fill primary state info of partcle generated at neutrino interaction
  // #############################################################################
  NeutInfoSummary* neutinfo = new NeutInfoSummary();
  neutinfo -> Clear();
  neutinfo -> Mode = (neut->Vector).Primary.Mode;
  neutinfo -> Numnu = (neut->Vector).Primary.NumParticle;
  for(int i=0;i<(neutinfo->Numnu);i++) {
    neutinfo -> Ipnu[i] = (neut->Vector).Primary.ParticleID[i];
    neutinfo -> Abspnu[i] = (neut->Vector).Primary.AbsMomentum[i];
    for(int j=0;j<3;j++) neutinfo -> Pnu[i][j] = (neut->Vector).Primary.Momentum[i][j];
  }
  runaction -> GetEvtSum() -> AddNeut( neutinfo );

	for(int ipart=0; ipart<Secondary.NumParticle; ipart++) {
  // #############################################################################
	// ### consider only TrackingFlag for use non interacted particle in neucleus ###
  // #############################################################################
		if( Secondary.TrackingFlag[ipart]==1 ) {

#ifdef DEBUG2
	    G4cout << "Particle:" << (neut->Vector).Secondary.ParticleID[ipart];
	    G4cout << "  Index: " << ipart;
	    G4cout << "  Parent Index: " << (neut->Vector).Secondary.ParentID[ipart] -1 << "\n";
	    G4cout << "Tracking Flag: " << (neut->Vector).Secondary.TrackingFlag[ipart];
	    G4cout << "  Interaction code: " << (neut->Vector).Secondary.InteractionCode[ipart] << "\n";
	    G4cout << " Momentum[MeV/c]:";
	    for (int k=0;k<3;k++)   G4cout << (neut->Vector).Secondary.Momentum[ipart][k]*MeV << " ";
	    G4cout << "\n";
#endif

			G4ParticleDefinition* particle;
			particle = particleTable->FindParticle(Secondary.ParticleID[ipart]);
			particleGun->SetParticleDefinition(particle);

			double nvec[3];
			for(int ixyz=0; ixyz<3; ixyz++)
				nvec[ixyz] = Secondary.Momentum[ipart][ixyz]/ Secondary.AbsMomentum[ipart];
			G4ThreeVector dir(nvec[0], nvec[1], nvec[2]);
			particleGun->SetParticleMomentumDirection(dir);

			G4double mass = particle->GetPDGMass();
			G4double mom = Secondary.AbsMomentum[ipart]*MeV;
			G4double energy = sqrt(mass*mass+mom*mom) - mass;
			particleGun->SetParticleEnergy(energy);
		
			particleGun->SetParticleTime(0.0*ns);

			//
			particleGun->GeneratePrimaryVertex(anEvent);

#ifdef DEBUG2
	    G4cout << "ipart: " << ipart << "\n";
	    G4cout << "PID:" << (neut->Vector).Secondary.ParticleID[ipart] << "\n";
	    G4cout << "Tracking Flag: " << (neut->Vector).Secondary.TrackingFlag[ipart] << "\n";
	    G4cout << "  Kinetic Energy[MeV]: " << energy << G4endl;;
	    G4cout << "  Momentum:";
	    for (int k=0;k<3;k++)   G4cout << (neut->Vector).Secondary.Momentum[ipart][k]*MeV << " ";
	    G4cout << " [MeV]\n";
#endif

		} // end of if
	} // end of for loop
}
