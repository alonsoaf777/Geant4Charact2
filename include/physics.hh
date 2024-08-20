#ifndef PHYSICS_HH
#define PHYSICS_HH	

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4VPhysicsConstructor.hh"


class MyPhysicsList : public G4VModularPhysicsList
{
public: 
	MyPhysicsList(); 
	~MyPhysicsList(); 
	
};
#endif
