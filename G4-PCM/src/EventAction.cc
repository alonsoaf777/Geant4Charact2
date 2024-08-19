#include "EventAction.hh"
#include "G4AnalysisManager.hh"

namespace G4_PCM {
    // Initialize static member
    int EventAction::fNtupleRegistrationCount = 0;

    EventAction::EventAction(RunAction*) {
        fEnergy = 0. / keV;
    }

    void EventAction::BeginOfEventAction(const G4Event* anEvent) {
        fEnergy = 0. / keV;
    }

    void EventAction::AddEnergy(G4double e) {
        fEnergy += e;
    }

    void EventAction::SetPosition(G4ThreeVector p) {
        fPosition = p;
    }

    void EventAction::EndOfEventAction(const G4Event* anEvent) {
        if (fEnergy > 0.0) {
            G4AnalysisManager* man = G4AnalysisManager::Instance();
            man->FillNtupleDColumn(0, 0, fEnergy / keV);
            man->AddNtupleRow(0);

            // Increment the registration count
            ++fNtupleRegistrationCount;
        }
    }

    void EventAction::Print() {
        G4cout
            << "Energy: "
            << G4BestUnit(fEnergy, "Energy")
            << G4endl;
    }

    int EventAction::GetNtupleRegistrationCount() {
        return fNtupleRegistrationCount;
    }

    void EventAction::ResetNtupleRegistrationCount() {
        fNtupleRegistrationCount = 0;
    }
}
