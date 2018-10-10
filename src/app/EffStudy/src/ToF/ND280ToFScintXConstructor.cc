#include <globals.hh>
#include <G4RotationMatrix.hh>
#include <G4VSolid.hh>
#include <G4ThreeVector.hh>
#include <G4Box.hh>
#include <G4PVPlacement.hh>

//#include <TND280Log.hxx>

//#include "ecal/ND280ECal.hh"
#include "ND280ToFScintXConstructor.hh"

void ND280ToFScintXConstructor::Init() {
  //SetMaximumHitLength( ND280ECal::HitLength );
  //SetMaximumHitSagitta( ND280ECal::HitSagitta );

  SetBarBase( 1.*CLHEP::cm );
  //SetBarBase( ND280ECal::BarWidth );
  SetBarHeight( 1.*CLHEP::cm );
  SetLength( 1.*CLHEP::cm );
  SetBarGap(0.0);
  SquareBar();
  MakeParallelBars();
  SetEdgeSpace(0.0);
}

void ND280ToFScintXConstructor::SetUp(int n_bars,
				      double bar_length,
				      double bar_base){
  // NEW ND280 UPGRADE
  SetBarBase( bar_base*CLHEP::mm );
  SetBarHeight( bar_base*CLHEP::mm );
  //

  SetBarNumber( n_bars );
  SetHeight( bar_length ); // length of an x-bar
  //SetHeight(n_bars * ND280ECal::BarWidth + 0.001*mm);
  SetWidth(bar_width); // + 0.001*mm);
}
