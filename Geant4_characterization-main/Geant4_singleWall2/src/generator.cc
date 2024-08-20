#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{	
	//Direction and messenger
	const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction()); 
	
	zPos = -35*cm;
	  
	//Create the particle gun
	fParticleGun = new G4ParticleGun(1); 
	
	//Access the available particles
	G4ParticleTable *ParticleTable = G4ParticleTable::GetParticleTable(); 
	
	//Properties of the table
	G4ParticleDefinition *particle = ParticleTable->FindParticle("gamma"); 
	fParticleGun->SetParticleEnergy(300 * MeV); 
	//Position
	G4double minX = -50*cm, maxX = 50*cm; 
	G4double minY = -50*cm, maxY = 50*cm;
	
	// Generate random position within the world volume
   	G4double posX = G4UniformRand() * (maxX - minX) + minX;
    	G4double posY = G4UniformRand() * (maxY - minY) + minY;
	
	//Random
	//G4ThreeVector gunPosition(posX, posY, zPos); 
	
	//Normal
	G4ThreeVector gunPosition(0., 0., zPos);
	fParticleGun->SetParticlePosition(gunPosition);
	
	
	G4ThreeVector targetPosition = detectorConstruction->GetTargetPosition(); 
	
	G4ThreeVector direction = (targetPosition - gunPosition).unit(); 
	
	//Center of target
	//fParticleGun->SetParticleMomentumDirection(direction); 
	
	// 0 0 1
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1)); 
	fParticleGun->SetParticleDefinition(particle);  
	
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGun; 
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	//Generate the particle
	fParticleGun->GeneratePrimaryVertex(anEvent); 
}
