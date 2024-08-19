#include "DetectorConstructionMessenger.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"
#include "G4RunManager.hh"

namespace G4_PCM
{
    DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction* detector, PrimaryGeneratorAction* gun)
        : fDetector(detector), fGun(gun)
    {
        fTargetThicknessCmd = new G4UIcmdWithADoubleAndUnit("/det/t", this);
        fTargetThicknessCmd->SetGuidance("Set the target thickness.");
        fTargetThicknessCmd->SetParameterName("thickness", true);
        fTargetThicknessCmd->SetRange("thickness > 0.");
        fTargetThicknessCmd->SetDefaultUnit("mm");

        fPgunCmd = new G4UIcmdWithADoubleAndUnit("/Pgun/Z", this);
        fPgunCmd->SetGuidance("Set the source z position.");
        fPgunCmd->SetParameterName("zpos", true);
        fPgunCmd->SetDefaultUnit("cm");
    }

    DetectorConstructionMessenger::~DetectorConstructionMessenger()
    {
        delete fTargetThicknessCmd;
        delete fPgunCmd;
    }

    void DetectorConstructionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
    {
        if (command == fTargetThicknessCmd)
        {
            G4double thickness = fTargetThicknessCmd->GetNewDoubleValue(newValue);
            G4cout << "Command received: /det/targetThickness " << thickness << G4endl;
            fDetector->SetTargetThickness(thickness);
        }
        else if (command == fPgunCmd)
        {
            G4double zpos = fPgunCmd->GetNewDoubleValue(newValue);
            G4cout << "Command received: /Pgun/Z " << zpos << G4endl;
            fGun->SetGunZpos(zpos);
        }
    }
}
