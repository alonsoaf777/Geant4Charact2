#include "run.hh"


MyRunAction::MyRunAction()
{
	//Instantiate the analysis manager for outputs
	G4AnalysisManager *man = G4AnalysisManager::Instance(); 
	
	//Create the root data for energy deposition
	man->CreateNtuple("Scoring", "Scoring"); 
	man->CreateNtupleDColumn("fEdep"); 
	man->FinishNtuple(0);
	//For hits
	man->CreateNtuple("Hits", "Hits"); 
	man->CreateNtupleIColumn("fEvent"); 
	man->CreateNtupleDColumn("fX"); 
	man->CreateNtupleDColumn("fY"); 
	man->CreateNtupleDColumn("fZ");
	man->FinishNtuple(1); 
	
	//Create histogram for cross section
	man->CreateNtuple("CrossSection", "CrossSection");
	man->CreateNtupleDColumn("Volume");
	man->CreateNtupleDColumn("Mass");
	man->CreateNtupleDColumn("Energy");
	man->FinishNtuple(2); 
	 
}

MyRunAction::~MyRunAction()
{}

//Generate the run of cross section
G4Run *MyRunAction::GenerateRun()
{
	fRun = new CrossRun(); 
	return fRun;
}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
	//Create an object to store the thickness
	const MyPrimaryGenerator *primaryGenerator = static_cast<const MyPrimaryGenerator*> (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction()); 
	
	G4AnalysisManager *man = G4AnalysisManager::Instance(); 
	
	//Generate a root file with its individual run
	G4int runID = run->GetRunID(); 
	
	std::stringstream strRunID; 
	strRunID << runID; 
	
	man->OpenFile("output" + strRunID.str() + ".root"); 
	
	if (primaryGenerator)
	{
		G4ParticleDefinition *particle = primaryGenerator->GetParticleGun()->GetParticleDefinition(); 
		G4double energy = primaryGenerator->GetParticleGun()->GetParticleEnergy();
		// G4ThreeVector pos = primaryGenerator->GetParticleGun()->GetParticlePosition(); 
		fRun->SetPrimary(particle, energy); //pos);
 
	}
	
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
	fRun->EndOfRun();
	
	G4AnalysisManager *man = G4AnalysisManager::Instance(); 
	
	man->Write(); 
	man->CloseFile(); 
}
