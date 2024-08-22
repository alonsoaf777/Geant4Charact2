#ifndef G4_PCM_PRIMARY_GENERATOR_ACTION_H
#define G4_PCM_PRIMARY_GENERATOR_ACTION_H 1


#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4RunManager.hh"



namespace G4_PCM
{
	class PrimaryGeneratorMessenger; // Forward declaration

	class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
	{
	public:
		PrimaryGeneratorAction();
		~PrimaryGeneratorAction();

		virtual void GeneratePrimaries(G4Event*);
		void SetGunZpos(G4double zpos);

		G4ParticleGun* fParticleGun;
	private:
		G4double fPgun = -5. * cm; // Valor predeterminado

		G4UIcmdWithADoubleAndUnit* fPgunCmd;
		PrimaryGeneratorMessenger* fMessenger; // Pointer to the messenger
	};
}

//Se bugeo el git xd
#endif 
