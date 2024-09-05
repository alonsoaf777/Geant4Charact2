#ifndef G4_PCM_OP_SENSITIVE_DETECTOR_H
#define G4_PCM_OP_SENSITIVE_DETECTOR_H 1

#include "G4VSensitiveDetector.hh"
#include "G4THitsCollection.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4ThreeVector.hh"

// Forward declaration for the hit class
class OPHit;

class OPSensitiveDetector : public G4VSensitiveDetector
{
public:
    OPSensitiveDetector(const G4String& name);
    ~OPSensitiveDetector() override;

    // Initialize hit collection
    void Initialize(G4HCofThisEvent* hce) override;

    // Process hits during each step
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;

    // End of event
    void EndOfEvent(G4HCofThisEvent* hce) override;

private:
    G4THitsCollection<OPHit>* fHitsCollection;
    G4int fHCID; // ID for hits collection
};

#endif
