#ifndef G4_PCM_OP_HIT_H
#define G4_PCM_OP_HIT_H 1

#include "G4VHit.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"

class OPHit : public G4VHit
{
public:
    OPHit();
    ~OPHit() override;

    inline void* operator new(size_t);
    inline void operator delete(void*);

    // Setters
    void SetEdep(G4double de) { fEdep = de; }
    void SetPos(G4ThreeVector pos) { fPos = pos; }

    // Getters
    G4double GetEdep() const { return fEdep; }
    G4ThreeVector GetPos() const { return fPos; }

private:
    G4double fEdep;         // Energía depositada
    G4ThreeVector fPos;     // Posición del hit
};

// Necesario para la asignación de memoria eficiente
extern G4Allocator<OPHit> OPHitAllocator;

inline void* OPHit::operator new(size_t)
{
    return (void*)OPHitAllocator.MallocSingle();
}

inline void OPHit::operator delete(void* hit)
{
    OPHitAllocator.FreeSingle((OPHit*)hit);
}

#endif
