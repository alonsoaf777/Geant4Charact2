#include "RunAction.hh"
#include "G4ThreeVector.hh"
#include "G4UnitsTable.hh"
#include "G4Run.hh"
#include "EventAction.hh" // Include EventAction for access to the static method
#include "G4UImanager.hh" // For mac commands

namespace G4_PCM {
    RunAction::RunAction() {
        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->SetDefaultFileType("root");
        analysisManager->SetNtupleMerging(true);
        analysisManager->SetVerboseLevel(4);
        analysisManager->CreateNtuple("G4_PCM", "Hits");
        analysisManager->CreateNtupleDColumn("Energy");
        analysisManager->FinishNtuple();
    }

    RunAction::~RunAction() {}

    void RunAction::BeginOfRunAction(const G4Run* aRun) {
        fTimer.Start();
        auto* analysisManager = G4AnalysisManager::Instance();
        G4int runNumber = aRun->GetRunID();
        G4String fileName = "NTuples_Run" + std::to_string(runNumber);
        analysisManager->SetFileName(fileName);
        analysisManager->OpenFile();

        // Reset the ntuple registration count at the beginning of each run
        EventAction::ResetNtupleRegistrationCount();
    }

    void RunAction::EndOfRunAction(const G4Run* aRun) {
        auto* analysisManager = G4AnalysisManager::Instance();
        G4UImanager* uiManager = G4UImanager::GetUIpointer();
        analysisManager->Write();
        analysisManager->CloseFile();
        fTimer.Stop();
        PrintTime();


        G4cout
            << "Total number of hits registrations: "
            << EventAction::GetNtupleRegistrationCount()
            << G4endl;

        for (int i = 0; i < 10; ++i) {
            uiManager->ApplyCommand("/gun/energy 1000 keV");
            uiManager->ApplyCommand("/run/beamOn 1000000");
        }
        // Print the total number of ntuple registrations

        //// Ejecuta la función de bisección
        //double bestThickness = PerformBisection(uiManager);
        //G4cout << "Bisección finalizada. Mejor valor de t encontrado: " << bestThickness << " nm" << G4endl;
    }

    double RunAction::PerformBisection(G4UImanager* uiManager) {
        // Parámetros iniciales para la bisección
        double t_min = 20.0; // nm
        double t_max = 20000000.0; // nm
        double tolerance = 1.0; // nm, tolerancia para la convergencia
        int maxIterations = 100; // Máximo número de iteraciones

        for (int i = 0; i < maxIterations; ++i) {
            // Calcula el valor medio
            double t_mid = (t_min + t_max) / 2.0;

            // Aplica el comando con el nuevo valor de t
            G4String command = "/det/t " + std::to_string(t_mid) + " nm";
            G4int status = uiManager->ApplyCommand(command);
            if (status != 0) {
                G4cerr << "Error al ejecutar " << command << G4endl;
                break;
            }

            // Reconfigura la geometría
            status = uiManager->ApplyCommand("/run/reinitializeGeometry");
            if (status != 0) {
                G4cerr << "Error al ejecutar /run/reinitializeGeometry" << G4endl;
                break;
            }

            // Ejecuta una corrida con 10000 eventos
            uiManager->ApplyCommand("/run/beamOn 10000");

            // Lee el número de hits
            G4int hitCount = EventAction::GetNtupleRegistrationCount();

            G4cout << "Iteración " << i + 1 << ": t = " << t_mid << " nm, hits = " << hitCount << G4endl;

            // Verifica la condición de parada
            if (hitCount >= 3000 && hitCount <= 4000) {
                G4cout << "Condición de parada alcanzada. t = " << t_mid << " nm, hits = " << hitCount << G4endl;
                return t_mid;
            }
            else if (hitCount < 3000) {
                // Si el número de hits es menor, disminuye t_mid
                t_max = t_mid;
            }
            else {
                // Si el número de hits es mayor, aumenta t_mid
                t_min = t_mid;
            }

            // Verifica si la tolerancia es suficiente para detenerse
            if ((t_max - t_min) / 2.0 < tolerance) {
                G4cout << "Tolerancia alcanzada. t = " << t_mid << " nm, hits = " << hitCount << G4endl;
                return t_mid;
            }
        }

        // Retorna el mejor valor de t encontrado después de todas las iteraciones
        return (t_min + t_max) / 2.0;
    }

    void RunAction::PrintTime() {
        auto time = fTimer.GetRealElapsed();
        G4cout << "Elapsed time: " << time << " Seconds." << G4endl;
    }
}
