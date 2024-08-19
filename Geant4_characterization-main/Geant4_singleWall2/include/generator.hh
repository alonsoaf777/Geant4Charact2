#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4RandomTools.hh"

#include "construction.hh"


class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public: 
	MyPrimaryGenerator(); 
	~MyPrimaryGenerator(); 
	
	virtual void GeneratePrimaries(G4Event*); 
	G4ParticleGun *GetParticleGun() const {return fParticleGun; }

private:
	G4ParticleGun *fParticleGun; 
};

#endif
