#include "IngridVLayerHit.hh"

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include <fstream>

#include "INGRID_Dimension.hh"

#define TopView 1  /// x-view (resolution of x-direction)

// allocator
G4Allocator<IngridVLayerHit> IngridVLayerHitAllocator;

//IngridVLayerHit::IngridVLayerHit(G4int dID0, G4int P0, G4int trkID, G4double e, const G4ThreeVector& pos, G4double t) 
IngridVLayerHit::IngridVLayerHit(G4int dID0, G4int P0, G4int trkID, G4double e, G4double eq, G4ThreeVector pos, G4double t) 
{
  detID = dID0;
  trackID = trkID;
  edep = e;
  edep_q = eq;
  Particle = P0;
  //for(int i=0;i<3;i++) position[i]=pos[i];
  position = pos;
  time = t;

  mod = dID0/264;              // 264 scnti-bar / module
  pln = (dID0-mod*264)/24;     // 24 channel / channel
  ch = dID0-mod*264-pln*24;    //24 ch /plane  

  view = TopView;



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
    
}

IngridVLayerHit::IngridVLayerHit(G4int dID0, G4double e,G4int P0) 
{
  detID = dID0;
  edep = e;
  Particle = P0;

}

IngridVLayerHit::IngridVLayerHit(G4int dID0, G4double e) 
{
  detID = dID0;
  edep = e;
 

}



IngridVLayerHit::~IngridVLayerHit() {}

IngridVLayerHit::IngridVLayerHit(const IngridVLayerHit &right)
  : G4VHit()
{
  detID = right.detID;
  edep       = right.edep;
  Particle = right.Particle;

  for(int i=0;i<3;i++) position[i] = right.position[i];
  eventID = right.eventID;


}

const IngridVLayerHit& IngridVLayerHit::operator=(const IngridVLayerHit &right)
{
  detID = right.detID;
  edep       = right.edep;
  Particle = right.Particle; 
  
  for(int i=0;i<3;i++) position[i] = right.position[i];
  eventID = right.eventID;

  return *this;
}

G4int IngridVLayerHit::operator==(const IngridVLayerHit &right) const
{
  return (this==&right) ? 1 : 0;
}  

G4int IngridVLayerHit::CompareID(const IngridVLayerHit right) 
{
  return (detID==right.detID) ? 1 : 0;
}

G4int IngridVLayerHit::CompareP(const IngridVLayerHit right) 
{
  return (Particle==right.Particle) ? 1 : 0;
}

G4int IngridVLayerHit::isFaster(const IngridVLayerHit right) 
{
  return (time>right.time) ? 1 : 0;
}

void IngridVLayerHit::Draw() // seems that when # of hits in modules is larger than 0 (by akira)
{
    double size = edep + 7.;
    if( edep>9 ) size = 16;
    
  #if 1
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
    {
      G4ThreeVector pos; 
      for(int i=0;i<3;i++) pos[i] = position[i]*mm;
      //pos[0]=position[0]*mm; pos[1]=position[1]*mm; pos[2]=position[2]*mm;
      G4Circle circle(pos);
      //circle.SetScreenSize(8.);
      circle.SetScreenSize(size);
      circle.SetFillStyle(G4Circle::filled);
      //G4Colour colour(0.,0.,1.); //means Blue
      G4Colour colour(1.,1.,0.); //means yellow
      G4VisAttributes attribs(colour);
      circle.SetVisAttributes(attribs);
      pVVisManager->Draw(circle);

    }
  #endif
}


void IngridVLayerHit::Print()
{
    G4cout.precision(4);

  G4cout << " Mod:" << mod 
         << " Pln:" << pln
         << " Ch:" << ch
         << " Time:" << time
         << " Edep:"  << edep
         << " p.e.:"  << pe
         << " PID:" << Particle
		 << " Trk:" << trackID
         << " pos:{" << position[0]/cm << ", "
         << position[1]/cm << ", " << position[2]/cm << "} "
		 << G4endl;
}


void IngridVLayerHit::Print_Hit()
{

  std::ofstream tateHit_ofs;
  tateHit_ofs.open("tateHit", std::ofstream::app);
  if (tateHit_ofs.good()){
    tateHit_ofs << eventID << "   " << detID << "   " << Particle << "   " << edep << "   ";
  }
  
  tateHit_ofs.close();
  
}


