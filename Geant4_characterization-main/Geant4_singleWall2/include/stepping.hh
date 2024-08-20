#ifndef STEPPING_HH
#define STEPPING_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"

#include "construction.hh"
#include "event.hh"
#include "crossrun.hh"

class MySteppingAction : public G4UserSteppingAction
{
public: 
	MySteppingAction(MyEventAction* eventAction); 
	~MySteppingAction(); 
	
	virtual void UserSteppingAction(const G4Step*); 

private:
	MyEventAction *fEventAction; 
	//bool fInteractionCounted; 
	//G4int i; 

};

#endif
