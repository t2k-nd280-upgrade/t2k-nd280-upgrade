#include "IngridHLayerHit.hh"

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include <fstream>

#include "INGRID_Dimension.hh"

#define SideView 0 // resolution of vertical-direction

// allocator
G4Allocator<IngridHLayerHit> IngridHLayerHitAllocator;

//IngridHLayerHit::IngridHLayerHit(G4int dID0, G4int P0, G4int trkid, G4double e, G4double e_q, const G4ThreeVector& pos, G4double t) 
IngridHLayerHit::IngridHLayerHit(G4int dID0, G4int P0, G4int trkid, G4double e, G4double eq, G4ThreeVector pos, G4double t) 
{
  detID = dID0;
  trackID = trkid;
  edep = e;
  edep_q = eq;
  Particle = P0;
  //for(int i=0;i<3;i++) position[i] = pos[i];
  position = pos;
  time = t;

    mod = dID0/264;              // 264 scnti-bar / module
    pln = (dID0-mod*264)/24;     // 24 channel / plane
    ch = dID0-mod*264-pln*24;    //24 ch /plane  


    view = SideView;


    //Proton Module added by kikawa
    if(mod>=16){
      mod=16;
      pln = (dID0-mod*264+8)/32;


      if(pln==0){
	pln=0;
	ch=dID0-mod*264;
      }
      else ch = dID0-mod*264+8-pln*32;
    }
    
    if(ch>31||ch<0)cout<<"error ch"<<ch<<endl;
    if(pln>17||pln<0)cout<<"error pln"<<pln<<endl;




    if( mod<=6 ) {
        posinmod[0] = pos[0] + 150*cm*(3-mod);
        posinmod[1] = pos[1];
        posinmod[2] = pos[2];
    }

    else if( mod>=7 && mod<=14 ) {
        posinmod[0] = pos[0];
        posinmod[1] = pos[1] + 150*cm*(10-mod);
        posinmod[2] = pos[2] + 400*cm;
    }

    //Proton Module added by kikawa
    else if(mod==16){
      posinmod[0] = pos[0];
      posinmod[1] = pos[1];
      posinmod[2] = pos[2] + 120*cm;
    }
    

    //
	/*
    INGRID_Dimension *fdim = new INGRID_Dimension();
    double posxy, posz;
    fdim->get_posXY(mod,view,pln,ch,&posxy,&posz);

    //
    inghit.Clear("C");
    inghit.mod = mod;
    inghit.view = view;
    inghit.pln = pln;
    inghit.ch = ch;
    inghit.xy = posxy;
    inghit.z = posz;
    inghit.time = t;

    //
    ingsimhit.Clear("C");
    ingsimhit.edeposit = edep;
    ingsimhit.trackid = trkid;
    ingsimhit.pdg = P0;
	*/

    //inghit.AddIngridSimHit(&ingsimhit);

//	if(fdim) delete fdim;
}

IngridHLayerHit::IngridHLayerHit(G4int dID0, G4double e,G4int P0) 
{
  detID = dID0;
  edep = e;
  Particle = P0;

}

IngridHLayerHit::IngridHLayerHit(G4int dID0, G4double e) 
{
  detID = dID0;
  edep = e;

}


IngridHLayerHit::~IngridHLayerHit() 
{
}

IngridHLayerHit::IngridHLayerHit(const IngridHLayerHit &right)
  : G4VHit()
{
  detID = right.detID;
  edep       = right.edep;
  Particle = right.Particle;

  for(int i=0;i<3;i++) position[i] = right.position[i];
  eventID = right.eventID;

}

const IngridHLayerHit& IngridHLayerHit::operator=(const IngridHLayerHit &right)
{
  detID = right.detID;
  edep       = right.edep;
  Particle = right.Particle; 
  
  for(int i=0;i<3;i++) position[i] = right.position[i];
  eventID = right.eventID;


  return *this;
}

G4int IngridHLayerHit::operator==(const IngridHLayerHit &right) const
{
  return (this==&right) ? 1 : 0;
}  

G4int IngridHLayerHit::CompareID(const IngridHLayerHit right) 
{
  return (detID==right.detID) ? 1 : 0;
}


G4int IngridHLayerHit::CompareP(const IngridHLayerHit right) 
{
  return (Particle==right.Particle) ? 1 : 0;
}

G4int IngridHLayerHit::isFaster(const IngridHLayerHit right)
{
	return (time>right.time) ? 1 : 0;
}

void IngridHLayerHit::Draw()
{
    double size = edep + 7.;
    if( edep>9 ) size = 16.;
  #if 1
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
    {
      G4ThreeVector pos; 
      for(int i=0;i<3;i++) pos[i] = position[i]*mm;
      G4Circle circle(pos);
      circle.SetScreenSize(size); 
      circle.SetFillStyle(G4Circle::filled);
      G4Colour colour(0.,1.,1.3); //cyan
      G4VisAttributes attribs(colour);
      circle.SetVisAttributes(attribs);
      pVVisManager->Draw(circle);
    }
  #endif
}

void IngridHLayerHit::Print()
{
  G4cout.precision(4);
  
  G4cout << " Mod:" << mod 
		 //<< " detID:" << detID
         << " Pln:" << pln 
		 << " Ch:" << ch 
		 << " Time:" << time
	//<< "  Edep:" << G4BestUnit(edep,"Energy")
		 << " Edep:" << edep
		 << " p.e.:" << pe
         << " PID:" << Particle
//		 << G4endl;
		 << " Trk:" << trackID
         << " pos:{" << position[0]/cm << ", "
         << position[1]/cm << ", " << position[2]/cm
         << "}" << G4endl;

}


void IngridHLayerHit::Print_Hit()
{

  std::ofstream yokoHit_ofs;
  yokoHit_ofs.open("yokoHit", std::ofstream::app);
  if (yokoHit_ofs.good()){
    yokoHit_ofs  << eventID << "   " << detID << "   " << Particle << "   " << edep << "   ";
  }
  
  yokoHit_ofs.close();
  
}


