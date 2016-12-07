#include <G4String.hh>
#include <G4ThreeVector.hh>

#include "ND280Constructor.hh"
#include "ecal/ND280ECalConstructor.hh"

ND280ECalConstructor::ND280ECalConstructor(G4String name,
                                           ND280Constructor* parent)
    : ND280Constructor(name, parent), fIsLeftClamshell(true),
      fDesignOffset(0.0), fExtraOffset(0.0),
      fRotationAxis(0.0, 1.0, 0.0), fRotationAngle(0.0) {}

void ND280ECalConstructor::SetExtraOffset(const G4ThreeVector offset) {
    fExtraOffset = offset;
}

void ND280ECalConstructor::ApplyExtraOffset(const G4ThreeVector offset) {
    fExtraOffset += offset;
}

void ND280ECalConstructor::SetRotationAxis(const G4ThreeVector axis) {
    fRotationAxis = axis;
}

void ND280ECalConstructor::SetRotationAngle(const double angle) {
    fRotationAngle = angle;
}

void ND280ECalConstructor::SetLeftClamshell() {
    fIsLeftClamshell = true;
}

void ND280ECalConstructor::SetRightClamshell() {
    fIsLeftClamshell = false;
}

G4ThreeVector ND280ECalConstructor::GetOffset() const {
    return (GetDesignOffset() + fExtraOffset);
}

G4ThreeVector ND280ECalConstructor::GetDesignOffset() const {
    G4ThreeVector doff(fDesignOffset);
    if (!fIsLeftClamshell) {
        doff.setX(-fDesignOffset.x());
    }
    return doff;
}
