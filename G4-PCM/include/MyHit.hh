#ifndef MY_HIT_HH
#define MY_HIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"

class MyHit : public G4VHit {
public:
    MyHit();
    virtual ~MyHit();

    // Métodos para almacenar y obtener información del hit
    void SetEnergy(G4double energy) { fEnergyDeposited = energy; }
    G4double GetEnergy() const { return fEnergyDeposited; }

    void SetPosition(G4ThreeVector pos) { fPosition = pos; }
    G4ThreeVector GetPosition() const { return fPosition; }

private:
    G4double fEnergyDeposited;      // Energía depositada
    G4ThreeVector fPosition;        // Posición del hit
};

// Definimos el tipo de colección de hits
typedef G4THitsCollection<MyHit> MyHitsCollection;

extern G4ThreadLocal G4Allocator<MyHit>* MyHitAllocator;

#endif // MY_HIT_HH
