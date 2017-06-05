#include <globals.hh>
#include <G4RotationMatrix.hh>
#include <G4VSolid.hh>
#include <G4ThreeVector.hh>
#include <G4Box.hh>
#include <G4PVPlacement.hh>

//#include <TND280Log.hxx>

#include "ecal/ND280ECal.hh"
#include "ecal/ND280ECalScintYConstructor.hh"

void ND280ECalScintYConstructor::Init() {
  //SetMaximumHitLength( ND280ECal::HitLength );
  //SetMaximumHitSagitta( ND280ECal::HitSagitta );
    SetBarBase( ND280ECal::BarWidth );
    SetBarHeight( ND280ECal::BarHeight );
    SetLength( ND280ECal::BarHeight );
    SetBarGap(0.0);
    SquareBar();
    MakeParallelBars();
    SetEdgeSpace(0.0);
}

void ND280ECalScintYConstructor::SetUp(int n_bars,
                           double bar_length,
                           ND280ECal::Detector detector) {
    SetBarNumber( n_bars );
    SetWidth( bar_length ); // length of an x-bar
    SetHeight(n_bars * ND280ECal::BarWidth + 0.001*mm);
    //switch (detector) {
    //case ND280ECal::kP0D:
    //SetSensitiveDetector("pec","segment");
    //break;
    //case ND280ECal::kBrl:
    //case ND280ECal::kDS:
    //SetSensitiveDetector("tec","segment");
    //break;
    //default:
    //G4Exception("Invalid detector for ECal scintillator bars");
    //}
}
