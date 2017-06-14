#include "IngridVetoHit.hh"

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4ThreeVector.hh"
#include <fstream>

#include "INGRID_Dimension.hh"

#define SideView 0  // Side View (Y-view) : 0
#define TopView 1  // Top View (X-view) : 1
#define PLNOFFSET 11
#define PLNOFFSETPM 18
// Veto Plane # : 11 ~ 14
// Veto Ch#/Plane : 0 ~ 21
// Veto Ch/Module : 88 ch

#define THRE 0.3 // MeV

// allocator
G4Allocator<IngridVetoHit> IngridVetoHitAllocator;

//IngridVetoHit::IngridVetoHit(G4int dID0, G4int P0, G4int trkid, G4double e, const G4ThreeVector& pos, G4double t) 
IngridVetoHit::IngridVetoHit(G4int dID0, G4int P0, G4int trkid, G4double e,G4double eq, G4ThreeVector pos, G4double t) 
{
    detID = dID0;
    trackID = trkid;
    edep = e;
    edep_q = eq;
    Particle = P0;
    //for(int i=0;i<3;i++) position[i] = pos[i];
    position = pos;
    time = t;

    //
    // detID of veto sinti = 88 * mod#(0~13) + 22*pln#(0~3) + ch(0~21)
    mod = detID/88;
    if(mod<16){
      pln = (detID-mod*88)/22 + PLNOFFSET;
      ch = detID-mod*88-(pln-PLNOFFSET)*22;
      
      if( pln==11 || pln==12 ) view = TopView;
      else if( pln==13 || pln==14 ) view = SideView;
    }
    
    else if(mod>=16){
      mod=16;
      pln = (detID-mod*88)/17 + PLNOFFSETPM;
      ch = detID-mod*88-(pln-PLNOFFSETPM)*17;
      
      if( pln==19 || pln==21 ) view = TopView;
      else if( pln==18 || pln==20 ) view = SideView;
    }
    
    //
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
    else if( mod==16 ) {
        posinmod[0] = pos[0];
        posinmod[1] = pos[1];
        posinmod[2] = pos[2] + 120*cm;
    }

}

IngridVetoHit::IngridVetoHit(G4int dID0, G4double e,G4int P0) 
{
  detID = dID0;
  edep = e;
  Particle = P0;

}

IngridVetoHit::IngridVetoHit(G4int dID0, G4double e) 
{
  detID = dID0;
  edep = e;

}

IngridVetoHit::~IngridVetoHit() {}

IngridVetoHit::IngridVetoHit(const IngridVetoHit &right)
  : G4VHit()
{
  detID = right.detID;
  edep       = right.edep;
  Particle = right.Particle;
  for(int i=0;i<3;i++) position[i] = right.position[i];  
  eventID = right.eventID;
}

const IngridVetoHit& IngridVetoHit::operator=(const IngridVetoHit &right)
{
  detID = right.detID;
  edep       = right.edep;
  Particle = right.Particle; 
  for(int i=0;i<3;i++) position[i] = right.position[i];  
  eventID = right.eventID;

 return *this;
}

G4int IngridVetoHit::operator==(const IngridVetoHit &right) const
{
  return (this==&right) ? 1 : 0;
}  

G4int IngridVetoHit::CompareID(const IngridVetoHit right) 
{
  return (detID==right.detID) ? 1 : 0;
}


G4int IngridVetoHit::CompareP(const IngridVetoHit right) 
{
  return (Particle==right.Particle) ? 1 : 0;
}

G4int IngridVetoHit::isFaster(const IngridVetoHit right) 
{
  return (time<right.time) ? 1 : 0;
}

void IngridVetoHit::Draw()
{
    double size = edep + 7.;
    if( edep>9 ) size = 16;

  #if 1
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
    {
      G4ThreeVector pos; 
      for(int i=0;i<3;i++) pos[i] = position[i]*mm;
      G4Circle circle(pos);
      circle.SetScreenSize(size); 
      circle.SetFillStyle(G4Circle::filled);
      G4Colour colour(1.,0.,0.); // red
      G4VisAttributes attribs(colour);
      circle.SetVisAttributes(attribs);
      pVVisManager->Draw(circle);
    }
  #endif
}

void IngridVetoHit::Print()
{
    G4cout.precision(4);

  G4cout << " Mod:" << mod
         << " View(0:Y,1:X):" << view  
         << " Pln:" << pln  
         << " Ch:" << ch  
         << " Time:" << time
         << " Edep:" << G4BestUnit(edep,"Energy")
         << " p.e.:" << pe
		 << G4endl;
  G4cout << " PID:" << Particle
         << " Trk:" << trackID
         << " pos:" << position[0]/cm << "  "
         << position[1]/cm << "  " << position[2]/cm
		<< G4endl;

}


void IngridVetoHit::Print_WeakEdep()
{
  std::ofstream ofs;
  ofs.open("veto_weakEnergy", std::ofstream::app);
  if (ofs.good()){
    ofs  << " veto-detID: " << detID  
	 << "  energy deposit: "<< G4BestUnit(edep,"Energy")
	 << "  particle: " << Particle
         << "  position (mm):{" << position[0] << ", "
         << position[1] << ", " << position[2]
         << G4endl;
  }
  
  ofs.close();
}

void IngridVetoHit::Print_Hit()
{

  std::ofstream vetoHit_ofs;
  vetoHit_ofs.open("vetoHit", std::ofstream::app);
  if (vetoHit_ofs.good()){
    vetoHit_ofs  << eventID << "   " << detID << "   " << Particle << "   " << edep << "   ";
  }

  vetoHit_ofs.close();
  
}

