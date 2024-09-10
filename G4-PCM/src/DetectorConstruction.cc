#include "DetectorConstruction.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4RunManager.hh"

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

    G4Material* DetectorConstruction::DefineRealisticMaterial()
    {
        G4double density = 2.2 * g / cm3; // Densidad del vidrio de sílice
        G4Material* realisticMaterial = new G4Material("SilicaGlass", density, 2);

        G4NistManager* nist = G4NistManager::Instance();
        realisticMaterial->AddElement(nist->FindOrBuildElement("Si"), 1);
        realisticMaterial->AddElement(nist->FindOrBuildElement("O"), 2);

        return realisticMaterial;
    }

    void DetectorConstruction::DefineMaterials()
    {
        G4NistManager* nist = G4NistManager::Instance();

        // Definir vidrio de sílice (target)
        target = DefineRealisticMaterial();

        // Material para el detector
        E_PbWO4 = new G4Material("E_PbWO4", 8.28 * g / cm3, 3);
        E_PbWO4->AddElement(nist->FindOrBuildElement("Pb"), 1);
        E_PbWO4->AddElement(nist->FindOrBuildElement("W"), 1);
        E_PbWO4->AddElement(nist->FindOrBuildElement("O"), 4);

        // Material de vacío para el mundo
        vacuum = nist->FindOrBuildMaterial("G4_Galactic");

        // Asignar propiedades ópticas al vacío
        G4MaterialPropertiesTable* vacuumMPT = new G4MaterialPropertiesTable();
        G4double vacuumRindex[] = { 1.0, 1.0 }; // Índice de refracción para vacío
        G4double vacuumPhotonEnergy[] = { 0.1 * eV, 12.4 * eV };
        vacuumMPT->AddProperty("RINDEX", vacuumPhotonEnergy, vacuumRindex, 2);
        vacuum->SetMaterialPropertiesTable(vacuumMPT);
    }

    void DetectorConstruction::DefineOpticalProperties()
    {
        const G4int numEntries = 12;
        G4double photonEnergy[numEntries] = {
            0.496 * eV, 0.7 * eV, 1.0 * eV, 1.55 * eV,
            2.0 * eV, 3.0 * eV, 4.0 * eV, 5.0 * eV,
            6.0 * eV, 8.0 * eV, 10.0 * eV, 12.4 * eV
        };

        // Índice de refracción para el vidrio de sílice
        G4double refractiveIndex[numEntries] = {
        //    1.4, 1.45, 1.5, 1.55, 1.6, 1.65, 1.7, 1.75, 1.8, 1.85, 1.9, 1.95
            1.4, 1.45, 1.5, 1.55, 1.6, 1.65, 1.7, 1.75, 1.8, 1.85, 1.9, 1.95
        };

        // Longitudes de absorción ajustadas
        G4double absorptionLength[numEntries] = {
            10 * cm, 10 * cm, 10 * cm, 10 * cm, 10 * cm,
            10 * cm, 10 * cm, 10 * cm, 10 * cm, 10 * cm, 10 * cm, 10 * cm
        };

        // Reflexión especular y difusa (superficie no ideal)
        G4double reflectivity[numEntries] = {
            0.05, 0.05, 0.05, 0.05, 0.05,
            0.05, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05
        };

        // Crear tabla de propiedades del material para el vidrio
        G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();
        MPT->AddProperty("RINDEX", photonEnergy, refractiveIndex, numEntries);
        MPT->AddProperty("ABSLENGTH", photonEnergy, absorptionLength, numEntries);
        MPT->AddProperty("REFLECTIVITY", photonEnergy, reflectivity, numEntries);

        // Asignar la tabla de propiedades al vidrio
        target->SetMaterialPropertiesTable(MPT);
    }

    G4VPhysicalVolume* DetectorConstruction::Construct()
    {
        // Definir el tamaño del mundo
        G4double worldSize = 1 * m;
        auto solidWorld = new G4Box("World", worldSize / 2, worldSize / 2, worldSize * 5);
        fWorldLog = new G4LogicalVolume(solidWorld, vacuum, "World");
        auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), fWorldLog, "World", nullptr, false, 0);

        // Crear el target (vidrio de sílice)
        G4double innerTargetRadius = 0.0;
        G4double outerTargetRadius = 2.5 * cm;

        G4Tubs* solidTarget = new G4Tubs("Target", innerTargetRadius, outerTargetRadius, fTargetThickness / 2.0, 0.0, 360.0 * deg);
        G4LogicalVolume* logicTarget = new G4LogicalVolume(solidTarget, target, "Target");

        // Posicionar el target en el mundo
        G4ThreeVector targetPos = G4ThreeVector(); // Posición (0,0,0)
        G4VPhysicalVolume* fTargetLog = new G4PVPlacement(nullptr, targetPos, logicTarget, "Target", fWorldLog, false, 0);

        // Crear el detector (plomo tungsteno)
        G4double detectorSizeXY = 20 * cm;
        G4double detectorSizeZ = 10 * cm;
        auto solidDetector = new G4Box("Detector", detectorSizeXY, detectorSizeXY, detectorSizeZ);
        fDetectorLog = new G4LogicalVolume(solidDetector, E_PbWO4, "Detector");

        // Posicionar el detector en el mundo
        // G4ThreeVector detectorPos = G4ThreeVector(0, 0, fTargetThickness / 2 + detectorSizeZ / 2); // Ajuste Z
        G4ThreeVector detectorPos = G4ThreeVector(0, 0, 20 * cm);
        new G4PVPlacement(nullptr, detectorPos, fDetectorLog, "Detector", fWorldLog, false, 0);

        // Definir propiedades ópticas y asignarlas a la superficie
        DefineOpticalProperties();
        G4OpticalSurface* opticalSurface = new G4OpticalSurface("OpticalSurface");
        opticalSurface->SetType(dielectric_dielectric);
        opticalSurface->SetFinish(polished);
        opticalSurface->SetModel(unified);

        auto* surface = new G4LogicalBorderSurface("Surface", physWorld, fTargetLog, opticalSurface);

        // Definir este detector como el detector gamma
        fGammaDetector = fDetectorLog;

        return physWorld;
    }

    void DetectorConstruction::SetTargetThickness(G4double thickness)
    {
        if (thickness != fTargetThickness) {
            fTargetThickness = thickness;
            G4RunManager::GetRunManager()->ReinitializeGeometry();
        }
    }

    void DetectorConstruction::SetMaterial(G4Material* material)
    {
        target = material;
        G4RunManager::GetRunManager()->ReinitializeGeometry();
    }
}
