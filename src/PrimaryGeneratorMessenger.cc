#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"
#include "G4RunManager.hh"

namespace G4_PCM
{
    PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* gun)
        : fGun(gun)
    {
        fPgunCmd = new G4UIcmdWithADoubleAndUnit("/Pgun/Z", this);
        fPgunCmd->SetGuidance("Set the source z position.");
        fPgunCmd->SetParameterName("zpos", true);
        fPgunCmd->SetDefaultUnit("cm");
    }

    PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
    {
        delete fPgunCmd;
    }

    void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
    {
        if (command == fPgunCmd)
        {
            G4double zpos = fPgunCmd->GetNewDoubleValue(newValue);
            G4cout << "Command received: /Pgun/Z " << zpos << G4endl;
            fGun->SetGunZpos(zpos);
        }
    }
}
