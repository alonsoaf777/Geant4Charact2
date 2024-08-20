#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
	fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");
	fMessenger->DeclareProperty("nCols", nCols, "Number of columns");
	fMessenger->DeclareProperty("nRows", nRows, "Number of rows");
	fMessenger->DeclareProperty("targetWidth", singleWallZ, "Width of the target");
	fMessenger->DeclareProperty("targetPosition", targetPosition, "Distance of the target from the center");
	
	
	nCols = 30; 
	nRows = 30;
	singleWallZ =  1 * cm;
	//Position of the target to access it in the generator
	targetPosition = G4ThreeVector(0., 0., 5*cm);
	
	DefineMaterials(); 
}	

MyDetectorConstruction::~MyDetectorConstruction()
{}

// Define materials
void MyDetectorConstruction::DefineMaterials()
{
	// --------------------- Materials
	G4NistManager *nist = G4NistManager::Instance(); 
	// Element for the wall
	wallMat = nist->FindOrBuildMaterial("G4_B-100_BONE"); 
	//Material for the detector
	detectorMat = nist->FindOrBuildMaterial("G4_Si"); 
	//Material for world volume
	worldMat = nist->FindOrBuildMaterial("G4_AIR"); 
	
	//Access the material properties
	fMaterial = wallMat; 
	// ---------------------- Properties for the materials
}

//Construct the detector
G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
	// ---------------------- Geometry
	G4double worldSizeXYZ = 1*m / 2;
	G4double singleWallXY = 25*cm / 2; 
	G4double singleWallZ2 = singleWallZ / 2; 
	//singleWallZ =
	G4double detectorZ = 2*cm / 2; 
	// Construction
	//World
	solidWorld = new G4Box("SolidWorld", worldSizeXYZ, worldSizeXYZ, worldSizeXYZ); 
	logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld"); 
	physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true); 
	
	//Wall
	solidWall = new G4Box("SolidWall", singleWallXY, singleWallXY, singleWallZ2); 
	logicWall = new G4LogicalVolume(solidWall, wallMat, "logicWall"); 
	physWall = new G4PVPlacement(0, targetPosition, logicWall, "physWall", logicWorld, false, 0, true);
	//ScoringVol
	fScoringVolume = logicWall; 
		 
	//Detector
	solidDetector = new G4Box("SolidDetector", worldSizeXYZ / nRows, worldSizeXYZ / nCols, detectorZ); 
	logicDetector = new G4LogicalVolume(solidDetector, detectorMat, "logicDetector"); 
 
	for (G4int i = 0; i < nRows; i++)
	{
		for (G4int j = 0; j < nCols; j++)
		{
			physDetector = new G4PVPlacement(0, G4ThreeVector(-worldSizeXYZ + (i + 0.5)*m/nRows, -worldSizeXYZ + (j + 0.5)*m/nCols, worldSizeXYZ - detectorZ), logicDetector, "physDetector", logicWorld, false, i+j*nCols, false ); 
		}
	}
	
	
	return physWorld;
}

//Pass the sensitive detector 
void MyDetectorConstruction::ConstructSDandField()
{
	MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector"); 	
	
	logicDetector->SetSensitiveDetector(sensDet); 
}




