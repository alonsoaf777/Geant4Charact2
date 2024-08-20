#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "Randomize.hh"


namespace G4_PCM
{

	PrimaryGeneratorAction::PrimaryGeneratorAction()
		: fPgun(-5 * cm), // Valor predeterminado
		fMessenger(new PrimaryGeneratorMessenger(this)) // Crear el mensajero
	{
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


	void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
	{
		// Randomize x and y starting point within a 1 mm diameter
		G4double radius = 10 * mm; // hay que cambiarlo .5*mm por defecto
		
		// generate random x and y positions within that radius
		double x, y, z = fPgun; // Aquí estableces z en -5 cm

		// to avoid using slow methods like sin and cos,
		// we generate random values in a cube and regect the ones
		// outside of a circle. This way 
		do {
			x = G4UniformRand() * (2.0 * radius) - radius;
			y = G4UniformRand() * (2.0 * radius) - radius;
		} while (x * x + y * y > radius * radius);

		//// Add conic behaviour
		//G4double theta, phi;
		//G4double angle = 0.0;
		//theta = angle * (G4UniformRand() - 0.5);
		//phi = angle * (G4UniformRand() - 0.5);
		//G4ThreeVector photonMomentum(theta, phi, 1.0);
		//fParticleGun->SetParticleMomentumDirection(photonMomentum);

		G4ThreeVector position = G4ThreeVector(x, y, z);
		// G4ThreeVector position = G4ThreeVector(x, y, z);
		fParticleGun->SetParticlePosition(position);

		// randomize energy with a .127 MeV std:dev gaussean distribution for an e-
		// Here, chance for a photon of 15 MeV with 4 stdDev 👍
		// G4double meanEnergy = 6. * MeV;
		// G4double stdDev = .127 * MeV;
		// G4double energy = G4RandGauss::shoot(meanEnergy, stdDev);
		G4double energy = 70 * keV;

		// fParticleGun->SetParticleEnergy(energy);


		// satisfy "generate primaries" here.
		fParticleGun->GeneratePrimaryVertex(event);
	}

	PrimaryGeneratorAction::~PrimaryGeneratorAction() {
		delete fParticleGun;
		delete fMessenger; // Eliminar el mensajero
	}

	void PrimaryGeneratorAction::SetGunZpos(G4double zpos)
	{
		G4cout << "Setting target thickness to: " << zpos << G4endl;
		if (zpos != fPgun) {
			fPgun = zpos;
			G4cout << "Target thickness changed to: " << fPgun << G4endl;

			// Forzar la actualización de la geometría
			//G4RunManager::GetRunManager()->ReinitializeGeometry();
		}
		else {
			G4cout << "Source Position Changed." << G4endl;
		}
	}
}