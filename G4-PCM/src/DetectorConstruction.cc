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
    
    // Define el óxido de vanadio
    G4Material* DefineVanadiumOxide() {
        G4NistManager* nist = G4NistManager::Instance();
        G4double density_V2O5 = 3.36 * g / cm3;
        G4Material* E_V2O5 = new G4Material("VanadiumOxide", density_V2O5, 2);
        E_V2O5->AddElement(nist->FindOrBuildElement("V"), 2);
        E_V2O5->AddElement(nist->FindOrBuildElement("O"), 5);
        return E_V2O5;
    }

    // Define el vidrio amorfo
    G4Material* DefineAmorphousGlass() {
        G4NistManager* nist = G4NistManager::Instance();
        G4double density_glass = 2.5 * g / cm3;
        G4Material* amorphousGlass = new G4Material("AmorphousGlass", density_glass, 2);
        amorphousGlass->AddElement(nist->FindOrBuildElement("Si"), 1);
        amorphousGlass->AddElement(nist->FindOrBuildElement("O"), 2);
        return amorphousGlass;
    }

    // Define la mezcla de óxido de vanadio y vidrio con proporciones específicas
    G4Material* DefineVanadiumGlassMix() {
        G4Material* vanadiumOxide = DefineVanadiumOxide();
        G4Material* amorphousGlass = DefineAmorphousGlass();

        // Define la densidad de la mezcla (ajústala según sea necesario)
        G4double density_mix = 2.7 * g / cm3; // Ajusta la densidad según sea necesario
        G4Material* vanadiumGlassMix = new G4Material("VanadiumGlassMix", density_mix, 2);

        // Proporciones específicas
        G4double fractionMass_VO2 = 0.05;  // Proporción de óxido de vanadio
        G4double fractionMass_SiO2 = 1.0 - fractionMass_VO2; // Proporción de vidrio

        vanadiumGlassMix->AddMaterial(vanadiumOxide, fractionMass_VO2);
        vanadiumGlassMix->AddMaterial(amorphousGlass, fractionMass_SiO2);

        return vanadiumGlassMix;
    }

    void DetectorConstruction::DefineMaterials()
    {
    	// Get nist material manager
        G4NistManager* nist = G4NistManager::Instance();

        //// Configure Vanadium Oxide
        //E_V2O5 = new G4Material("E_V2O5", 3.36 * g / cm3, 2);
        //E_V2O5->AddElement(nist->FindOrBuildElement("V"), 2);
        //E_V2O5->AddElement(nist->FindOrBuildElement("O"), 5);

        // Define el material para el objetivo
        //target = nist->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
        target = DefineVanadiumGlassMix();
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
            worldSize * 10);
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
        G4double outerTargetRadius = 2.5 * cm;

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
