#include "crossrun.hh"

G4double CrossSection = 0; 
G4double massicCS = 0; 

CrossRun::CrossRun()
{}

CrossRun::~CrossRun()
{}

void CrossRun::SetPrimary(G4ParticleDefinition *particle, G4double energy)
{
	fParticle = particle; 
	fEkin = energy; 
}

void CrossRun::CountProcesses(G4String procName)
{
	std::map<G4String, G4int>::iterator it = fProcCounter.find(procName); 
	if (it == fProcCounter.end())
	{
		fProcCounter[procName] = 1; 
	}
	else
	{
		fProcCounter[procName]++; 
	}
}

//void Merge

void CrossRun::EndOfRun()
{
	//Ni idea xd
	G4int prec = 5; 
	G4int dfprec = G4cout.precision(prec);
	
	//Obtain the material properties of the target and particle gun
	//Create an object to store the thickness
	const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction()); 
	G4Material *material = detectorConstruction->GetMaterial(); 
	G4double density = material->GetDensity(); 
	
	G4String partName = fParticle->GetParticleName(); 
	G4double thickness = detectorConstruction->GetThickness(); 
	
	G4cout << "\n ======================== run summary ======================\n";
 	G4cout << "\n The run is: " << numberOfEvent << " " << partName << " of "
        << G4BestUnit(fEkin,"Energy") << " through " 
        << G4BestUnit(thickness,"Length") << " of "
        << material->GetName() << " (density: " 
        << G4BestUnit(density,"Volumic Mass") << ")" << G4endl;
        
        //Frecuency
        //frequency of processes
	  G4int totalCount = 0;
	  G4int survive = 0;  
	  G4cout << "\n Process calls frequency --->";
	  std::map<G4String,G4int>::iterator it;  
	  for (it = fProcCounter.begin(); it != fProcCounter.end(); it++) {
	     G4String procName = it->first;
	     G4int    count    = it->second;
	     totalCount += count; 
	     G4cout << "\t" << procName << " = " << count;
	     if (procName == "Transportation") survive = count;
	  }
	  G4cout << G4endl;
	  
	  if (totalCount == 0) { G4cout.precision(dfprec);   return;};  
	  G4double ratio = double(survive)/totalCount;

	  G4cout << "\n Nb of incident particles unaltered after "
		 << G4BestUnit(thickness,"Length") << " of "
		 << material->GetName() << " : " << survive 
		 << " over " << totalCount << " incident particles."
		 << "  Ratio = " << 100*ratio << " %" << G4endl;
	  
	  if (ratio == 0.) return;
	  
	  //compute cross section and related quantities
	  //
	  G4double CrossSection = - std::log(ratio)/thickness;     
	  G4double massicCS  = CrossSection/density;
	   
	  G4cout << " ---> CrossSection per volume:\t" << CrossSection*cm << " cm^-1 "
		 << "\tCrossSection per mass: " << G4BestUnit(massicCS, "Surface/Mass")
		 << G4endl;
		 
          //Calculate cross section with calculator
	  G4cout << "\n Verification from G4EmCalculator: \n"; 
	  G4EmCalculator emCalculator;
	  G4double sumc = 0.0;  
	  for (it = fProcCounter.begin(); it != fProcCounter.end(); it++) {
	    G4String procName = it->first;  
	    G4double massSigma = 
	    emCalculator.GetCrossSectionPerVolume(fEkin,fParticle,
		                                      procName,material)/density;
	    if (fParticle == G4Gamma::Gamma())
	       massSigma = 
	       emCalculator.ComputeCrossSectionPerVolume(fEkin,fParticle,
		                                      procName,material)/density;
	    sumc += massSigma;
	    if (procName != "Transportation")
	      G4cout << "\t" << procName << "= " 
		     << G4BestUnit(massSigma, "Surface/Mass");
	  }         
	      
	  G4cout << "\ttotal= " 
		 << G4BestUnit(sumc, "Surface/Mass") << G4endl;
		 
	  //expected ratio of transmitted particles
	  G4double Ratio = std::exp(-sumc*density*thickness);
	  G4cout << "\tExpected ratio of transmitted particles= " 
		 << 100*Ratio << " %" << G4endl;         
		                
	  // remove all contents in fProcCounter 
	  fProcCounter.clear();
	  
	  //restore default format
	  G4cout.precision(dfprec); 
	 	
	  massicCS = massicCS * g / cm2; 
	  fEkin = fEkin / keV; 
	  
	  G4AnalysisManager *man = G4AnalysisManager::Instance(); 
	  
	  man->FillNtupleDColumn(2, 0, CrossSection); 
	  man->FillNtupleDColumn(2, 1, massicCS); 
	  man->FillNtupleDColumn(2, 2, fEkin); 
	  man->AddNtupleRow(2); 
	  
}
