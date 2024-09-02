#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"

namespace G4_PCM {
	SteppingAction::SteppingAction(EventAction* eventAction) : G4UserSteppingAction(), feventAction(eventAction) {
		// El constructor inicializa el puntero de EventAction
	}

	SteppingAction::~SteppingAction() {
		// Destructor
	}

	void SteppingAction::UserSteppingAction(const G4Step* step) {
		// Obtener la construcci�n del detector
		const auto* detConstruction = static_cast<const DetectorConstruction*>(
			G4RunManager::GetRunManager()->GetUserDetectorConstruction());

		// Obtener el volumen del detector de fotones �pticos
		fOpticalPhotonDetector = detConstruction->GetOpticalPhotonDetector();

		// Obtener el volumen l�gico del paso actual
		G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

		// Verificar si el volumen del paso actual es el detector deseado
		if (volume != fOpticalPhotonDetector) {
			return;  // Salir si no es el detector correcto
		}

		// Si es el primer paso en el volumen, guardar la posici�n
		if (step->IsFirstStepInVolume()) {
			feventAction->SetPosition(step->GetPreStepPoint()->GetPosition());
		}

		// Agregar la energ�a depositada al EventAction
		feventAction->AddEnergy(step->GetTotalEnergyDeposit());
	}
}
