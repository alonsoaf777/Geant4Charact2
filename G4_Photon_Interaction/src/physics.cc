#include "physics.hh"

MyPhysicsList::MyPhysicsList()
{
	//Em physics 
	RegisterPhysics(new G4EmStandardPhysics());
	//RegisterPhysics (new G4EmStandardPhysics()); 
	new G4UnitDefinition( "mm2/g", "mm2/g","Surface/Mass", mm/g);
  	new G4UnitDefinition( "um2/mg", "um2/mg","Surface/Mass", um*um/mg);
  	
	SetVerboseLevel(1); 
	
}

MyPhysicsList::~MyPhysicsList()
{}


