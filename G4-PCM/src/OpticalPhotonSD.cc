#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "OpticalPhotonSD.hh"
#include "G4OpticalPhoton.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"

OpticalPhotonSD::OpticalPhotonSD(const G4String& name)
    : G4VSensitiveDetector(name) {}

G4bool OpticalPhotonSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
    // Obtener el track del paso
    G4Track* track = aStep->GetTrack();

    // Verificar si el track es un fot�n �ptico
    if (track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
        G4cout << "Optical photon detected!" << G4endl;
        // Aqu� puedes procesar la detecci�n, como almacenar datos, contar fotones, etc.
    }

    return true;
}
