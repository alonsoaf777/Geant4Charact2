#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
	G4Track *track = aStep->GetTrack(); 
	
	//Only check when the particle collides with the external surface
	track->SetTrackStatus(fStopAndKill);
	//Obtain the position when hits the surface of the detector
	G4StepPoint *preStepPoint = aStep->GetPreStepPoint(); 
	G4StepPoint *postStepPoint = aStep->GetPostStepPoint(); 
	
	const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable(); 
	
	//Number of detector	
	G4int copyNo = touchable->GetCopyNumber(); 
	
	//Position of detector
	G4VPhysicalVolume *physVol = touchable->GetVolume(); 
	G4ThreeVector posDetector = physVol->GetTranslation();  
	
	G4cout << "Detector position" << posDetector << G4endl; 
	
	//Get number of event
	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID(); 
	
	G4AnalysisManager *man = G4AnalysisManager::Instance(); 
	
	man->FillNtupleIColumn(1, 0, evt); 
	man->FillNtupleDColumn(1, 1, posDetector[0]); 
	man->FillNtupleDColumn(1, 2, posDetector[1]); 
	man->FillNtupleDColumn(1, 3, posDetector[2]); 
	man->AddNtupleRow(1);
	
	
	return true; 
}


