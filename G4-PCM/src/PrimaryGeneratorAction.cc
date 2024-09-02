#include "PrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

namespace G4_PCM
{
    PrimaryGeneratorAction::PrimaryGeneratorAction()
    {
        // Crear una instancia de G4GeneralParticleSource
        fParticleSource = new G4GeneralParticleSource();

        // Configuración inicial de la fuente de partículas
        fParticleSource->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("opticalphoton"));
    }

    PrimaryGeneratorAction::~PrimaryGeneratorAction()
    {
        delete fParticleSource;
    }

    void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
    {
        // Genera el vértice primario
        fParticleSource->GeneratePrimaryVertex(event);
    }
}
