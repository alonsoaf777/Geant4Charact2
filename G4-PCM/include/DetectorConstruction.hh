#ifndef G4_PCM_PRIMARY_GENERATOR_ACTION_H
#define G4_PCM_PRIMARY_GENERATOR_ACTION_H 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

namespace G4_PCM
{
// AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
	class PrimaryGeneratorMessenger;

	class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
	{
	public:
		PrimaryGeneratorAction();
		~PrimaryGeneratorAction();

		virtual void GeneratePrimaries(G4Event*);
		void SetGunZpos(G4double zpos);

		G4GeneralParticleSource* fParticleSource;

	private:
		G4double fPgun = -5. * cm;
		G4UIcmdWithADoubleAndUnit* fPgunCmd;
		PrimaryGeneratorMessenger* fMessenger;
	};
}

#endif
