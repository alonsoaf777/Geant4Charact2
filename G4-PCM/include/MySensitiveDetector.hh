#ifndef MY_SENSITIVE_DETECTOR_HH
#define MY_SENSITIVE_DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "MyHit.hh"

class MySensitiveDetector : public G4VSensitiveDetector {
public:
    MySensitiveDetector(const G4String& name);
    virtual ~MySensitiveDetector() = default;

    virtual void Initialize(G4HCofThisEvent* HCE) override;
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;

private:
    MyHitsCollection* fHitsCollection;
};

#endif // MY_SENSITIVE_DETECTOR_HH
