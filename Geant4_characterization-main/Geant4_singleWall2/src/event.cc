#include "event.hh"

extern G4double massicCS; 
extern G4double CrossSection; 

MyEventAction::MyEventAction(MyRunAction*)
{
	fEdep = 0.; 
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
	//At the begginig of an event, the energy resets
	fEdep = 0.; 
}

void MyEventAction::EndOfEventAction(const G4Event*)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance(); 
	
	man->FillNtupleDColumn(0, 0, fEdep); 
	man->AddNtupleRow(0); 
	
	const MyPrimaryGenerator *primaryGenerator = static_cast<const MyPrimaryGenerator*> (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction()); 
	
	G4ThreeVector pos(primaryGenerator->GetParticleGun()->GetParticlePosition()); 
	
	G4cout << "Energy Deposition: " << G4BestUnit(fEdep, "Energy") << G4endl;
	
	G4cout << "Particle position: " << pos << " " << G4endl; 
	
}
