#ifndef G4_PCM_OPTICAL_PHOTON_SD_H
#define G4_PCM_OPTICAL_PHOTON_SD_H 1

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"

class OpticalPhotonSD : public G4VSensitiveDetector
{
public:
    OpticalPhotonSD(const G4String& name);
    virtual ~OpticalPhotonSD() {}

    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*) override;
};

#endif
