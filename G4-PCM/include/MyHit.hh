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

    // M�todos para almacenar y obtener informaci�n del hit
    void SetEnergy(G4double energy) { fEnergyDeposited = energy; }
    G4double GetEnergy() const { return fEnergyDeposited; }

    void SetPosition(G4ThreeVector pos) { fPosition = pos; }
    G4ThreeVector GetPosition() const { return fPosition; }

private:
    G4double fEnergyDeposited;      // Energ�a depositada
    G4ThreeVector fPosition;        // Posici�n del hit
};

// Definimos el tipo de colecci�n de hits
typedef G4THitsCollection<MyHit> MyHitsCollection;

extern G4ThreadLocal G4Allocator<MyHit>* MyHitAllocator;

#endif // MY_HIT_HH
