#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "Randomize.hh"


namespace G4_PCM
{
	PrimaryGeneratorAction::PrimaryGeneratorAction() {
		// set up particle gun
		G4int nParticles = 1;
		fParticleGun = new G4ParticleGun(nParticles);

		// define particle properties
		const G4String& particleName = "gamma";

		G4ThreeVector momentumDirection = G4ThreeVector(0, 0, 1);

		// default particle kinematic
		G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
		G4ParticleDefinition* particle
			= particleTable->FindParticle(particleName);
		fParticleGun->SetParticleDefinition(particle);
		fParticleGun->SetParticleMomentumDirection(momentumDirection);
	}

	PrimaryGeneratorAction::~PrimaryGeneratorAction() {
		delete fParticleGun;
	}

	void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
	{
		// Randomize x and y starting point within a 1 mm diameter
		G4double radius = 10 * mm; // hay que cambiarlo .5*mm por defecto
		
		// generate random x and y positions within that radius
		double x, y;

		// to avoid using slow methods like sin and cos,
		// we generate random values in a cube and regect the ones
		// outside of a circle. This way 
		do {
			x = G4UniformRand() * (2.0 * radius) - radius;
			y = G4UniformRand() * (2.0 * radius) - radius;
		} while (x * x + y * y > radius * radius);

		G4ThreeVector position = G4ThreeVector(x, y, -5 * cm);
		fParticleGun->SetParticlePosition(position);

		// randomize energy with a .127 MeV std:dev gaussean distribution for an e-
		// Here, chance for a photon of 15 MeV with 4 stdDev 👍
		// G4double meanEnergy = 6. * MeV;
		// G4double stdDev = .127 * MeV;
		// G4double energy = G4RandGauss::shoot(meanEnergy, stdDev);
		G4double energy = 100 * keV;

		// fParticleGun->SetParticleEnergy(energy);


		// satisfy "generate primaries" here.
		fParticleGun->GeneratePrimaryVertex(event);
	}
}