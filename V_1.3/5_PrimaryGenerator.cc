#include "5_PrimaryGenerator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);

    G4ParticleTable * particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "gamma";
    G4ParticleDefinition * particle = particleTable -> FindParticle(particleName);

    fParticleGun -> SetParticleDefinition(particle);
    fParticleGun -> SetParticleEnergy(20 * keV);
}

MyPrimaryGenerator::~MyPrimaryGenerator() { delete fParticleGun; }

void MyPrimaryGenerator::GeneratePrimaries(G4Event * anEvent)
{ 
    G4double x0, y0, z0;
    G4double radius = 15.0;

    x0 = 2 * (G4UniformRand() - 0.5);
    y0 = 2 * (G4UniformRand() - 0.5);
    y0 = y0 * std::sqrt(1 - std::pow(x0, 2));

    x0 = x0 * radius * cm;
    y0 = y0 * radius * cm;
    z0 = - 20.0 * cm;

    G4ThreeVector photonPosition(x0, y0, z0);
    fParticleGun -> SetParticlePosition(photonPosition);

    G4double theta, phi;
    G4double angle = 0.0;
    theta = angle * (G4UniformRand() - 0.5);
    phi   = angle * (G4UniformRand() - 0.5);
    G4ThreeVector photonMomentum(theta, phi, 1.0);
    fParticleGun -> SetParticleMomentumDirection(photonMomentum);

    fParticleGun -> GeneratePrimaryVertex(anEvent); 
}
