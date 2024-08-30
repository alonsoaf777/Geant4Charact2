#include "PrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

namespace G4_PCM
{

	PrimaryGeneratorAction::PrimaryGeneratorAction()
		: fPgun(-5 * cm),
		fMessenger(new PrimaryGeneratorMessenger(this))
	{
		// Initialize the particle source
		fParticleSource = new G4GeneralParticleSource();

		// Set default particle properties via the macro or command interface
		// For example, you can set particle type and energy distribution in the macro
	}

	void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
	{
		// The direction, energy, and other properties should be set via the macro commands.

		// Set the particle position
		G4ThreeVector position(0, 0, fPgun);
		fParticleSource->SetParticlePosition(position);

		// Generate the primary vertex
		fParticleSource->GeneratePrimaryVertex(event);
	}

	PrimaryGeneratorAction::~PrimaryGeneratorAction()
	{
		delete fParticleSource;
		delete fMessenger;
	}

	void PrimaryGeneratorAction::SetGunZpos(G4double zpos)
	{
		G4cout << "Setting source position to: " << zpos << G4endl;
		if (zpos != fPgun) {
			fPgun = zpos;
			G4cout << "Source Position changed to: " << fPgun << G4endl;
		}
		else {
			G4cout << "Source Position Changed." << G4endl;
		}
	}
}
