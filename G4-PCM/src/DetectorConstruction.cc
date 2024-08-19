#include "DetectorConstruction.hh"

namespace G4_PCM
{
    DetectorConstruction::DetectorConstruction()
        : fTargetThickness(1 * mm), // Valor predeterminado
        fMessenger(new DetectorConstructionMessenger(this)) // Crear el mensajero
    {
    	DefineMaterials();
    }

    DetectorConstruction::~DetectorConstruction()
    {
        delete fMessenger; // Eliminar el mensajero
    }
    
    void DetectorConstruction::DefineMaterials()
    {
    	// Get nist material manager
        G4NistManager* nist = G4NistManager::Instance();

        // Define el material para el objetivo
        target = nist->FindOrBuildMaterial("G4_Al");
        vacuum = nist->FindOrBuildMaterial("G4_Galactic");
        
        // Configure Lead Tungstate for crystals
        E_PbWO4 = new G4Material("E_PbWO4", 8.28 * g / cm3, 3);
        E_PbWO4->AddElement(nist->FindOrBuildElement("Pb"), 1);
        E_PbWO4->AddElement(nist->FindOrBuildElement("W"), 1);
        E_PbWO4->AddElement(nist->FindOrBuildElement("O"), 4);

    }
    
    G4VPhysicalVolume* DetectorConstruction::Construct()
    {
 
        // Define el tamaño del mundo
        G4double worldSize = 1 * m;
        auto solidWorld = new G4Box("World",
            worldSize / 2,
            worldSize / 2,
            worldSize);
        auto logicWorld = new G4LogicalVolume(solidWorld,
            vacuum,
            "World");
        auto physWorld = new G4PVPlacement(nullptr,
            G4ThreeVector(),
            logicWorld,
            "World",
            nullptr,
            false,
            0);

        // Crear el objetivo con el grosor especificado
        G4double innerTargetRadius = 0.0;
        G4double outerTargetRadius = 1.5 * cm;

        G4Tubs* solidTarget = new G4Tubs("Target",
            innerTargetRadius,
            outerTargetRadius,
            fTargetThickness / 2.0,
            0.0,
            360.0 * deg);

        G4LogicalVolume* logicTarget = new G4LogicalVolume(solidTarget,
            target,
            "Target");

        // Posicionar el objetivo en el mundo
        G4ThreeVector targetPos = G4ThreeVector(); // 0,0,0
        G4RotationMatrix* targetRotation = new G4RotationMatrix();
        new G4PVPlacement(targetRotation,
            targetPos,
            logicTarget,
            "Target",
            logicWorld,
            false,
            0);

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
