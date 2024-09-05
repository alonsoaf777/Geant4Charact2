#include "OPHit.hh"

G4Allocator<OPHit> OPHitAllocator;

OPHit::OPHit() : G4VHit(), fEdep(0.), fPos(G4ThreeVector()) {}
OPHit::~OPHit() = default;
