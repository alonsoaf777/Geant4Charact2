#include "OPSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4ThreeVector.hh"
#include "OPHit.hh"

OPSensitiveDetector::OPSensitiveDetector(const G4String& name)
    : G4VSensitiveDetector(name), fHitsCollection(nullptr), fHCID(-1)
{
    collectionName.insert("OPHitCollection");
}

OPSensitiveDetector::~OPSensitiveDetector() = default;

void OPSensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
    // Crear una nueva colecci�n de hits
    fHitsCollection = new G4THitsCollection<OPHit>(SensitiveDetectorName, collectionName[0]);

    // Obtener el ID de la colecci�n de hits (si a�n no est� definido)
    if (fHCID < 0) {
        fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
    }

    // A�adir la colecci�n de hits al evento
    hce->AddHitsCollection(fHCID, fHitsCollection);
}

G4bool OPSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* /*history*/)
{
    // Obtener la energ�a depositada en el paso
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep == 0) return false; // Si no hay dep�sito de energ�a, no registrar hit

    // Crear un nuevo hit
    OPHit* newHit = new OPHit();

    // Obtener la posici�n del hit
    G4ThreeVector hitPos = step->GetPreStepPoint()->GetPosition();

    // Guardar la energ�a y la posici�n en el hit
    newHit->SetEdep(edep);
    newHit->SetPos(hitPos);

    // A�adir el hit a la colecci�n
    fHitsCollection->insert(newHit);

    return true;
}

void OPSensitiveDetector::EndOfEvent(G4HCofThisEvent* /*hce*/)
{
    // Aqu� podr�as procesar los hits si es necesario, por ejemplo, imprimirlos
    // O guardarlos en un archivo de salida si se desea
}
