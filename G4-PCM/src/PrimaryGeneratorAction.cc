#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include "cmath"


namespace G4_PCM
{

	PrimaryGeneratorAction::PrimaryGeneratorAction()
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


	// Distribución de Planck para la radiación solar
	G4double PlanckDistribution(G4double T) {
		// Constantes
		const G4double h = 4.135667696e-15 * eV * s; // Constante de Planck en eV·s
		const G4double c = 299792458 * m / s; // Velocidad de la luz en m/s
		const G4double k_B = 8.617333262e-5 * eV / kelvin; // Constante de Boltzmann en eV/K

		// Generar una longitud de onda aleatoria en el rango deseado (300 nm a 2500 nm)
		G4double lambda_min = 300 * nm;
		G4double lambda_max = 2500 * nm;
		G4double lambda = lambda_min + ((lambda_max - lambda_min) * G4UniformRand());

		// Calcular la energía correspondiente
		G4double energy = h * c / lambda;

		// Calcular la probabilidad según la distribución de Planck
		G4double prob = (2 * h * c * c) / (std::pow(lambda, 5) * (std::exp((h * c) / (lambda * k_B * T)) - 1));

		// Generar un número aleatorio para decidir si aceptar o rechazar la energía generada
		G4double maxProb = (2 * h * c * c) / (std::pow(lambda_min, 5) * (std::exp((h * c) / (lambda_min * k_B * T)) - 1));
		G4double randomProb = maxProb * G4UniformRand();
		if (randomProb < prob) {
			return energy;
		}
		else {
			return PlanckDistribution(T); // Reintentar si no se acepta
		}
	}



	void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
	{
		// Randomize x and y starting point within a 1 mm diameter
		G4double radius = 2 * cm; // hay que cambiarlo .5*mm por defecto
		
		// generate random x and y positions within that radius
		double x, y, z = -5 * cm; // Aquí estableces z en -5 cm

		// to avoid using slow methods like sin and cos,
		// we generate random values in a cube and regect the ones
		// outside of a circle. This way 
		do {
			x = G4UniformRand() * (2.0 * radius) - radius;
			y = G4UniformRand() * (2.0 * radius) - radius;
		} while (x * x + y * y > radius * radius);

		// Define el valor de π
		const G4double pi = 3.14159265358979323846;

		// Define el ángulo en grados
		G4double angleInDegrees = 0.0;  // 45 grados

		// Convierte el ángulo a radianes
		G4double angleInRadians = angleInDegrees * (pi / 180.0);

		// Add conic behaviour
		G4double theta, phi;
		theta = angleInRadians * (G4UniformRand() - 0.5);
		phi = angleInRadians * (G4UniformRand() - 0.5);
		G4ThreeVector photonMomentum(theta, phi, 1.0);
		fParticleGun->SetParticleMomentumDirection(photonMomentum);

		G4ThreeVector position = G4ThreeVector(x, y, z);
		// G4ThreeVector position = G4ThreeVector(x, y, z);
		fParticleGun->SetParticlePosition(position);

		// randomize energy with a .127 MeV std:dev gaussean distribution for an e-
		// Here, chance for a photon of 15 MeV with 4 stdDev 👍
		// G4double meanEnergy = 6. * MeV;
		// G4double stdDev = .127 * MeV;
		// G4double energy = G4RandGauss::shoot(meanEnergy, stdDev);
		//G4double energy = 0.01 * keV;
		// fParticleGun->SetParticleEnergy(energy);


		// Temperatura efectiva del Sol (aproximadamente 5778 K)
		G4double temperature = 5778 * kelvin;

		// Genera la energía utilizando la distribución de Planck
		G4double energy = PlanckDistribution(temperature);

		// Establece la energía del fotón generado
		fParticleGun->SetParticleEnergy(energy);


		// satisfy "generate primaries" here.
		fParticleGun->GeneratePrimaryVertex(event);
	}

	PrimaryGeneratorAction::~PrimaryGeneratorAction() {
		delete fParticleGun;
	}
}