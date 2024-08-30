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

<<<<<<< HEAD
=======
	// Plack distribution for sunlight
	G4double PlanckDistribution(G4double T) {
		// Constantes
		const G4double h = 4.135667696e-15 * eV * s; // Constante de Planck en eV·s
		const G4double c = 299792458 * m / s; // Velocidad de la luz en m/s
		const G4double k_B = 8.617333262e-5 * eV / kelvin; // Constante de Boltzmann en eV/K

		// Generar una longitud de onda aleatoria en el rango deseado
		G4double lambda_min = 400 * nm; // Mínimo en nm (violeta)
		G4double lambda_max = 700 * nm; // Máximo en nm (rojo)
		G4double lambda = lambda_min + ((lambda_max - lambda_min) * G4UniformRand());

		// Calcular la energía correspondiente
		G4double energy = h * c / lambda;

		// Calcular la probabilidad según la distribución de Planck
		G4double prob = (2 * h * c * c) / (std::pow(lambda, 5) * (std::exp((h * c) / (lambda * k_B * T)) - 1));

		// Generar un número aleatorio para decidir si aceptar o rechazar la energía generada
		G4double randomProb = G4UniformRand();
		if (randomProb < prob) {
			return energy/eV;
		}
		else {
			return PlanckDistribution(T); // Reintentar si no se acepta
		}
	}



>>>>>>> parent of 384b1d1 (Plankiana de energias, pero no es correcto.)
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
