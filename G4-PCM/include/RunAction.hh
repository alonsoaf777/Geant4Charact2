#ifndef G4_PCM_RUN_ACTION_HH
#define G4_PCM_RUN_ACTION_HH

#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Timer.hh"
#include "EventAction.hh" // Include EventAction to reset the ntuple registration count
#include "G4UImanager.hh" // Include for G4UImanager

namespace G4_PCM {

    class HitsCollection;

    class RunAction : public G4UserRunAction {
    public:
        RunAction();
        ~RunAction();

        void BeginOfRunAction(const G4Run* aRun) override;
        void EndOfRunAction(const G4Run* aRun) override;

    private:
        G4Timer fTimer;

        void PrintTime();

        // Declare the bisection function
        double PerformBisection(G4UImanager* uiManager);
    };

}

#endif // !G4_PCM_RUN_ACTION_HH
