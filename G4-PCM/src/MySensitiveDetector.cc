
#include "MySensitiveDetector.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"

MySensitiveDetector::MySensitiveDetector(const G4String& name)
    : G4VSensitiveDetector(name), fHitsCollection(nullptr) {
    collectionName.insert("MyHitsCollection");  // Nombre de la colecci�n de hits
}

void MySensitiveDetector::Initialize(G4HCofThisEvent* HCE) {
    // Crear la colecci�n de hits
    fHitsCollection = new MyHitsCollection(SensitiveDetectorName, collectionName[0]);
    G4int HCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
    HCE->AddHitsCollection(HCID, fHitsCollection);
}

G4bool MySensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* history) {
    G4double edep = step->GetTotalEnergyDeposit();

    if (edep == 0.0) return false;  // No hit si no hay energ�a depositada

    // Crear un nuevo hit
    MyHit* newHit = new MyHit();
    newHit->SetEnergy(edep);
    newHit->SetPosition(step->GetPreStepPoint()->GetPosition());

    // Agregar el hit a la colecci�n
    fHitsCollection->insert(newHit);

    return true;
}
