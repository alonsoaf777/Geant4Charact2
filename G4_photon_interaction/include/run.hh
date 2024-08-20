#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"
#include <iomanip>

#include "construction.hh"
#include "generator.hh"
#include "crossrun.hh"

class MyDetectorConstruction; 
class MyPrimaryGenerator; 
class CrossRun; 

class MyRunAction : public G4UserRunAction
{
public: 
	MyRunAction(); 
	~MyRunAction(); 
	
	G4Run* GenerateRun() override; 
	void BeginOfRunAction(const G4Run*); 
	void EndOfRunAction(const G4Run*); 
	
private: 
	//MyPrimaryGenerator *fParticle; 
	CrossRun *fRun = nullptr; 
};
#endif
