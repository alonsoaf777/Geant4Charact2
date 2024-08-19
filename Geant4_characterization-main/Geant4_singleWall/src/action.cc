#include "action.hh"

MyActionInitialization::MyActionInitialization(MyDetectorConstruction *det) : fDetector(det)
{}

MyActionInitialization::~MyActionInitialization()
{}

void MyActionInitialization::BuildForMaster() const
{
	MyRunAction *runAction = new MyRunAction(fDetector, 0); 
	SetUserAction(runAction); 
}

void MyActionInitialization::Build() const
{
	MyPrimaryGenerator *generator = new MyPrimaryGenerator(); 
	SetUserAction(generator); 
	
	MyRunAction *runAction = new MyRunAction(fDetector, generator); 
	SetUserAction(runAction); 
	
	MyEventAction *eventAction = new MyEventAction(runAction); 
	SetUserAction(eventAction); 
	
	MySteppingAction *steppingAction = new MySteppingAction(eventAction); 
	SetUserAction(steppingAction); 
	
}
