#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"

#include "construction.hh"
#include "generator.hh"
#include "run.hh"
#include "stepping.hh"
#include "event.hh"

class MyActionInitialization : public G4VUserActionInitialization
{
public: 
	MyActionInitialization(MyDetectorConstruction*); 
	~MyActionInitialization(); 
	
	virtual void Build() const; 
	virtual void BuildForMaster() const; 
	
	MyDetectorConstruction *fDetector = nullptr; 
};

#endif
