#ifndef CROSSRUN_HH
#define CROSSRUN_HH

#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4EmCalculator.hh"
#include "G4Gamma.hh"
#include "globals.hh"
#include <map>
#include <iomanip>

#include "construction.hh"
#include "generator.hh"

//Specific used classes
class MyDetectorConstruction; 
class G4ParticleDefinition; 

class CrossRun : public G4Run
{
public: 
	//I must access to materials, etc
	CrossRun(MyDetectorConstruction*); 
	~CrossRun();
	 
	//Functions
	//Get the characteristics of the incident particle to calculate cross
	void SetPrimary(G4ParticleDefinition* particle, G4double energy); 
	//Get the dictionary of processes
	void CountProcesses(G4String procName); 
	//Counter for multithread
	//void Merge(const G4Run* run); 
	//End run
	void EndOfRun(); 
	
private: 
	MyDetectorConstruction *fDetector = nullptr; 
	G4ParticleDefinition *fParticle = nullptr; 
	G4double fEkin = 0; 
	
	//Counter
	std::map<G4String, G4int> fProcCounter; 

};

#endif
