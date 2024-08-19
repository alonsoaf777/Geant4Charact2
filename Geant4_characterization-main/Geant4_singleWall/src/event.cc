#include "event.hh"

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
	//G4cout << "Energy Deposition" << fEdep << G4endl; 
	
	G4AnalysisManager *man = G4AnalysisManager::Instance(); 
	
	man->FillNtupleDColumn(0, 0, fEdep); 
	man->AddNtupleRow(0); 
}
