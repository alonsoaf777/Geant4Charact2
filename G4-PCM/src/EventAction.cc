#include "EventAction.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4AnalysisManager.hh"

EventAction::EventAction() {}

void EventAction::EndOfEventAction(const G4Event* event) {
    // Obtener la colección de hits
    G4HCofThisEvent* HCE = event->GetHCofThisEvent();
    if (!HCE) return;

    G4int HCID = GetHitsCollectionID("MyHitsCollection");
    auto hitsCollection = static_cast<MyHitsCollection*>(HCE->GetHC(HCID));

    // Si no hay hits, no procesamos
    if (!hitsCollection) return;

    G4double totalEnergy = 0.0;
    for (size_t i = 0; i < hitsCollection->entries(); ++i) {
        MyHit* hit = (*hitsCollection)[i];
        totalEnergy += hit->GetEnergy();

        // Aquí puedes procesar los hits individualmente o guardar sus posiciones
    }

    // Guardar la energía total en el ntuple
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0, totalEnergy * 1e6);  // Guardar en microjoules
    analysisManager->AddNtupleRow();
}

G4int EventAction::GetHitsCollectionID(const G4String& colName) const {
    return G4SDManager::GetSDMpointer()->GetCollectionID(colName);
}
