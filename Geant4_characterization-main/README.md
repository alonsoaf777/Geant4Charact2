# Geant4_characterization
## Geant4 material characterization

## Geant4_singleWall 
Llama al detector construction desde el sim.cc y lo jala a muchos scripts.hh como cross run, run, action, etc. para poder acceder a la geometría desde esos scripts.
## Geant4_singleWall2
Llama al detector usando la función de mustafa const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction()); y  const MyPrimaryGenerator *primaryGenerator = static_cast<const MyPrimaryGenerator*> (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction()); en el caso de run.cc 

