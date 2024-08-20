#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
	fEventAction = eventAction; 
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
	//Search the volume that has a particle
	G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume(); 
	
	//Create an object to store the scoring volume
	const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction()); 
	
	//Tracking
	G4Track* track = step->GetTrack();
	// Check if this trackID has already interacted
    	G4int trackID = track->GetTrackID();
    	CrossRun *run = static_cast<CrossRun*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
    	
	//Scoring volume
	G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume(); 
	
	//We dont consider volumes other than the wall
	if(volume != fScoringVolume)
		return; 
	
	//Gets the total energy of all detector 
	G4double edep = step->GetTotalEnergyDeposit(); 
	fEventAction->AddEdep(edep); 
	
	//Process Counter
	G4StepPoint* endPoint = step->GetPostStepPoint(); 
	G4String procName = endPoint->GetProcessDefinedStep()->GetProcessName(); 
	
	run->CountProcesses(procName);
	

	//kill after interaction
	G4RunManager::GetRunManager()->AbortEvent(); 
}
