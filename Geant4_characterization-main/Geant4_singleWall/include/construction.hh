#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4GenericMessenger.hh"

#include "detector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public: 
	MyDetectorConstruction(); 
	~MyDetectorConstruction(); 
	
	//Func to access the scoring volume
	G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }
	//Func to access the target position
	G4ThreeVector GetTargetPosition() const { return targetPosition; }
	//Func to obtain the material properties of the target
	G4Material *GetMaterial() const {return fMaterial; }
	G4double GetThickness() const {return targetThickness; }
	
	virtual G4VPhysicalVolume *Construct(); 

private:
	virtual void ConstructSDandField(); 
	
	G4Box *solidWorld, *solidWall, *solidDetector; 
	G4LogicalVolume *logicWorld, *logicWall, *logicDetector, *fScoringVolume; 
	G4VPhysicalVolume *physWorld, *physWall, *physDetector; 
	
	G4int nCols, nRows; 
	G4double singleWallZ, targetThickness; //Depth of the target 
	G4ThreeVector targetPosition; //Position of the target we will access it in the generator
	
	G4GenericMessenger *fMessenger; 
	
	G4Material *wallMat, *detectorMat, *worldMat, *fMaterial; 
	
	void DefineMaterials();

};

#endif

