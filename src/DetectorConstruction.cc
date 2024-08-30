#include "DetectorConstruction.hh"

namespace G4_PCM
{
    DetectorConstruction::DetectorConstruction()
        : fTargetThickness(50 * mm), // Valor predeterminado
        fMessenger(new DetectorConstructionMessenger(this)) // Crear el mensajero
    {
    	DefineMaterials();
    	
    	// Arm with Bone / OsBone 
    	// Single Bone / OsBone
    	isArm = false; 
    	isBone = true; 
    	isOsBone = false; 
    	outerBoneRadius = 1.5 * cm;
    }

    DetectorConstruction::~DetectorConstruction()
    {
        delete fMessenger; // Eliminar el mensajero
    }
    
    void DetectorConstruction::DefineMaterials()
    {
    	// Get nist material manager
        G4NistManager* nist = G4NistManager::Instance();
//G4_B-100_BONE
        // Define el material para el objetivo
        bone = nist->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
        muscle = nist->FindOrBuildMaterial("G4_MUSCLE_SKELETAL_ICRP");
	skin = nist->FindOrBuildMaterial("G4_SKIN_ICRP");
	grasa = nist->FindOrBuildMaterial("G4_ADIPOSE_TISSUE_ICRP");
        //target = nist->FindOrBuildMaterial("G4_W");
        vacuum = nist->FindOrBuildMaterial("G4_AIR");
        
        // Configure Lead Tungstate for crystals
        E_PbWO4 = new G4Material("E_PbWO4", 8.28 * g / cm3, 3);
        E_PbWO4->AddElement(nist->FindOrBuildElement("Pb"), 1);
        E_PbWO4->AddElement(nist->FindOrBuildElement("W"), 1);
        E_PbWO4->AddElement(nist->FindOrBuildElement("O"), 4);
        
        // Configure material for oesteoporotic bone
        H = nist->FindOrBuildMaterial("G4_H"); 
        C = nist->FindOrBuildMaterial("G4_C"); 
        N = nist->FindOrBuildMaterial("G4_N"); 
        O = nist->FindOrBuildMaterial("G4_O"); 
        Mg = nist->FindOrBuildMaterial("G4_Mg"); 
        P = nist->FindOrBuildMaterial("G4_P"); 
        S = nist->FindOrBuildMaterial("G4_S"); 
        Ca = nist->FindOrBuildMaterial("G4_Ca"); 
        OsBone = new G4Material("OsteoporoticBone", 1.3*g/cm3, 8); //Less d
        OsBone -> AddMaterial(H, 6.4*perCent); 
        OsBone -> AddMaterial(C, 27.8*perCent); 
        OsBone -> AddMaterial(N, 2.7*perCent); 
        OsBone -> AddMaterial(O, 41*perCent); 
        OsBone -> AddMaterial(Mg, 0.2*perCent); 
        OsBone -> AddMaterial(P, 7*perCent); 
        OsBone -> AddMaterial(S, 0.2*perCent); 
        OsBone -> AddMaterial(Ca, 14.7*perCent); 
        
    }
     // -----------------------------------------------------------------------
     // --------------------------- CONSTRUCT THE BONE WITH PORES
    void DetectorConstruction::ConstructOsBone()
    {
    	G4double startPhi = 0.*deg;
	G4double deltaPhi = 360.*deg;
	G4double startTheta = 0.*deg;
	G4double deltaTheta = 180.*deg;  // Esfera completa
		// Define los poros como esferas pequeñas
	G4double poreRadius = 100*um;
	pore = new G4Sphere("Pore", 0, poreRadius, startPhi, deltaPhi, startTheta, 	deltaTheta);
		// Número de poros que deseas simular
	int numPores = 300;
		//Solido
	porousBone = solidBone;

	for (int i = 1; i < numPores; i++) {
			// Generar coordenadas aleatorias dentro del cilindro
		G4double r = G4UniformRand() * outerBoneRadius; // Distancia radial aleatoria			
		G4double theta = G4UniformRand() * 360.0 * deg; // Ángulo aleatorio
		G4double z = G4UniformRand() * fTargetThickness - fTargetThickness/2; // Altura aleatoria en el cilindro
			// Convertir coordenadas cilíndricas a cartesianas
		G4double x = r * std::cos(theta);
		G4double y = r * std::sin(theta);

			// Definir la posición del poro
		G4ThreeVector porePosition = G4ThreeVector(x, y, z);

			// Crear y restar el poro del volumen del hueso
		porousBone = new G4SubtractionSolid("PorousBone", porousBone, pore, 0, porePosition);
		}
		//Logic
	logicBone = new G4LogicalVolume(porousBone,OsBone,"PorousBoneLogical");
		//Placement
	physBone = new G4PVPlacement(targetRotation, targetPos, logicBone, "physBone", logicWorld, false, 0);  	
	G4cout << " -----------------   Osteo generated ----------------" << G4endl;
    }
    
    	// -----------------------------------------------------------------------
	// ------------- CREATE ARM --------------------------------
    void DetectorConstruction::ConstructArm()
    {
    	// Crear el antebrazo hueso -> musculo -> grasa -> piel
		// HUESO
	G4double innerBoneRadius = 0.0;
	//outerBoneRadius = 1.5 * cm;
		
		// MUSCULO
	G4double innerMuscleRadius = outerBoneRadius;
	G4double outerMuscleRadius =  innerMuscleRadius + 2.5 * cm;
		// GRASA
	G4double innerGrasaRadius = outerMuscleRadius;
	G4double outerGrasaRadius =  innerGrasaRadius + 0.5 * cm;
		//PIEL
	G4double innerSkinRadius = outerGrasaRadius;
	G4double outerSkinRadius =  innerSkinRadius + 0.15 * cm;
		
		// Tubs
	solidBone = new G4Tubs("Bone", innerBoneRadius, outerBoneRadius, fTargetThickness / 2.0, 0.0, 360.0 * deg); 
	solidMuscle = new G4Tubs("Muscle", innerMuscleRadius, outerMuscleRadius, fTargetThickness / 2.0, 0.0, 360.0 * deg);
	solidGrasa = new G4Tubs("Grasa", innerGrasaRadius, outerGrasaRadius, fTargetThickness / 2.0, 0.0, 360.0 * deg);  
	solidSkin = new G4Tubs("Skin", innerSkinRadius, outerSkinRadius, fTargetThickness / 2.0, 0.0, 360.0 * deg);
	
	// ------------------ OSBONE --------------------------
	if(isOsBone) //Bone with osteoporos
	{
		ConstructOsBone(); 
	}
	else //Normal bone
	{
		logicBone = new G4LogicalVolume(solidBone,bone,"LogicBone");
		physBone = new G4PVPlacement(targetRotation, targetPos, logicBone, "physBone", logicWorld, false, 0, true); 
	}
	// ----------------------------------------------------
	     	//Logical
	logicMuscle = new G4LogicalVolume(solidMuscle,muscle,"LogicMuscle");
	logicGrasa = new G4LogicalVolume(solidGrasa,grasa,"LogicGrasa");
	logicSkin = new G4LogicalVolume(solidSkin,skin,"LogicSkin");
	     	//PphysVOlume
	     	
	physMuscle = new G4PVPlacement(targetRotation, targetPos, logicMuscle, "physMuscle", logicWorld, false, 0, true); 
	physGrasa = new G4PVPlacement(targetRotation, targetPos, logicGrasa, "physGrasa", logicWorld, false, 0, true); 
	physSkin = new G4PVPlacement(targetRotation, targetPos, logicGrasa, "physSkin", logicWorld, false, 0, true); 
	
    }
    
	// -----------------------------------------------------------------------
	// ------------------------------ CREATE SINGLE BONE -------------------------- 
    void DetectorConstruction::ConstructBone()
    {
    	// HUESO
	G4double innerBoneRadius = 0.0;
	// G4double outerBoneRadius = 1.5 * cm;
	
	// Tubs
	solidBone = new G4Tubs("Bone", innerBoneRadius, outerBoneRadius, fTargetThickness / 2.0, 0.0, 360.0 * deg); 
	// ------------------ OSBONE --------------------------
	if(isOsBone) //Bone with osteoporos
	{
		ConstructOsBone(); 
	}
	else //Normal bone
	{
		logicBone = new G4LogicalVolume(solidBone,bone,"LogicBone");
		physBone = new G4PVPlacement(targetRotation, targetPos, logicBone, "physBone", logicWorld, false, 0, true); 
	
	}
    }
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
// ------------------ CONSTRUCT GEOM ---------------------------- 
    G4VPhysicalVolume* DetectorConstruction::Construct()
    {
 
        // Define el tamaño del mundo
        G4double worldSize = 1 * m;
        solidWorld = new G4Box("World",
            worldSize / 2,
            worldSize / 2,
            worldSize * 10);
        logicWorld = new G4LogicalVolume(solidWorld,
            vacuum,
            "World");
        physWorld = new G4PVPlacement(nullptr,
            G4ThreeVector(),
            logicWorld,
            "World",
            nullptr,
            false,
            0);
            
        //Define the position and rotation of the tubs no matter isArm or bone
        targetPos = G4ThreeVector(); // 0,0,0
        targetRotation = new G4RotationMatrix(0, 0, 0);
	
	// type selection
	if(isArm)
	{
		ConstructArm(); 
     	}
     	
     	if(isBone)
     	{
     		ConstructBone(); 
     	}
        

        // Crear el detector
        G4double detectorSizeXY = 20 * cm;
        G4double detectorSizeZ = 5 * cm;

     
        G4Box* solidDetector = new G4Box(
            "Detector",
            detectorSizeXY,
            detectorSizeXY,
            detectorSizeZ);

        G4LogicalVolume* logicDetector = new G4LogicalVolume(
            solidDetector,
            E_PbWO4,
            "Detector");

        G4ThreeVector detectorPos = G4ThreeVector(0, 0, 20 * cm);
        G4RotationMatrix* detRotation = new G4RotationMatrix();

        // Colocar el detector
        new G4PVPlacement(detRotation,
            detectorPos,
            logicDetector,
            "Detector",
            logicWorld,
            false,
            0);

        // Definir este detector como el detector gamma
        fGammaDetector = logicDetector;

        return physWorld;
    }

    void DetectorConstruction::SetTargetThickness(G4double thickness)
    {
        G4cout << "Setting target thickness to: " << thickness << G4endl;
        if (thickness != fTargetThickness) {
            fTargetThickness = thickness;
            G4cout << "Target thickness changed to: " << fTargetThickness << G4endl;

            // Forzar la actualización de la geometría
            //G4RunManager::GetRunManager()->ReinitializeGeometry();
        }
        else {
            G4cout << "Target thickness unchanged." << G4endl;
        }
    }
}
