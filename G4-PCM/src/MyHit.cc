
#include "MyHit.hh"

G4ThreadLocal G4Allocator<MyHit>* MyHitAllocator = nullptr;

MyHit::MyHit() : fEnergyDeposited(0), fPosition(G4ThreeVector()) {}

MyHit::~MyHit() {}
