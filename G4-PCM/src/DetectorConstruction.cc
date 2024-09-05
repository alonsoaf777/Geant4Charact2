#include "DetectorConstruction.hh"
#include "MySensitiveDetector.hh"
#include "G4SDManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4RotationMatrix.hh"
#include "G4RunManager.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

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

    // Definir óxido de vanadio
    G4Material* DetectorConstruction::DefineVanadiumOxide() {
        G4NistManager* nist = G4NistManager::Instance();
        G4double density_V2O5 = 3.36 * g / cm3;
        G4Material* E_V2O5 = new G4Material("VanadiumOxide", density_V2O5, 2);
        E_V2O5->AddElement(nist->FindOrBuildElement("V"), 2);
        E_V2O5->AddElement(nist->FindOrBuildElement("O"), 5);
        return E_V2O5;
    }

    // Definir vidrio amorfo
    G4Material* DetectorConstruction::DefineAmorphousGlass() {
        G4NistManager* nist = G4NistManager::Instance();
        G4double density_glass = 2.5 * g / cm3;
        G4Material* amorphousGlass = new G4Material("AmorphousGlass", density_glass, 2);
        amorphousGlass->AddElement(nist->FindOrBuildElement("Si"), 1);
        amorphousGlass->AddElement(nist->FindOrBuildElement("O"), 2);
        return amorphousGlass;
    }

    // Definir mezcla de óxido de vanadio y vidrio
    G4Material* DetectorConstruction::DefineVanadiumGlassMix() {
        G4Material* vanadiumOxide = DefineVanadiumOxide();
        G4Material* amorphousGlass = DefineAmorphousGlass();

        G4double density_mix = 2.7 * g / cm3; // Ajuste de densidad
        G4Material* vanadiumGlassMix = new G4Material("VanadiumGlassMix", density_mix, 2);

        G4double fractionMass_VO2 = 0.50;
        G4double fractionMass_SiO2 = 1.0 - fractionMass_VO2;

        vanadiumGlassMix->AddMaterial(vanadiumOxide, fractionMass_VO2);
        vanadiumGlassMix->AddMaterial(amorphousGlass, fractionMass_SiO2);

        return vanadiumGlassMix;
    }

    void DetectorConstruction::DefineMaterials()
    {
        G4NistManager* nist = G4NistManager::Instance();

        target = DefineVanadiumGlassMix();
        vacuum = nist->FindOrBuildMaterial("G4_Galactic");

        E_PbWO4 = new G4Material("E_PbWO4", 8.28 * g / cm3, 3);
        E_PbWO4->AddElement(nist->FindOrBuildElement("Pb"), 1);
        E_PbWO4->AddElement(nist->FindOrBuildElement("W"), 1);
        E_PbWO4->AddElement(nist->FindOrBuildElement("O"), 4);

        // Definir las propiedades ópticas de PbWO4
        const G4int nEntries = 2;
        G4double photonEnergy[nEntries] = { 2.0 * eV, 3.0 * eV }; // Energías de los fotones (espectro visible)
        G4double refractiveIndexPbWO4[nEntries] = { 2.2, 2.2 }; // Índice de refracción

        G4MaterialPropertiesTable* opticalTable = new G4MaterialPropertiesTable();
        opticalTable->AddProperty("RINDEX", photonEnergy, refractiveIndexPbWO4, nEntries);

        E_PbWO4->SetMaterialPropertiesTable(opticalTable);
    }

    G4VPhysicalVolume* DetectorConstruction::Construct()
    {
        G4double worldSize = 1 * m;
        auto solidWorld = new G4Box("World", worldSize / 2, worldSize / 2, worldSize * 2);
        auto logicWorld = new G4LogicalVolume(solidWorld, vacuum, "World");
        auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0);

        // Crear el detector
        G4double detectorSizeXY = 20 * cm;
        G4double detectorSizeZ = 5 * cm;

        G4Box* solidDetector = new G4Box("Detector", detectorSizeXY, detectorSizeXY, detectorSizeZ);
        G4LogicalVolume* logicDetector = new G4LogicalVolume(solidDetector, E_PbWO4, "Detector");

        G4ThreeVector detectorPos = G4ThreeVector(0, 0, 20 * cm);
        G4RotationMatrix* detRotation = new G4RotationMatrix();

        auto physDetector = new G4PVPlacement(detRotation, detectorPos, logicDetector, "Detector", logicWorld, false, 0);

        fGammaDetector = logicDetector;

        // **Registro del Sensitive Detector**
        G4SDManager* sdManager = G4SDManager::GetSDMpointer();
        MySensitiveDetector* sensitiveDetector = new MySensitiveDetector("SensitiveDetector");
        logicDetector->SetSensitiveDetector(sensitiveDetector);
        sdManager->AddNewDetector(sensitiveDetector);

        // **Barreras ópticas**
        G4OpticalSurface* opticalSurface = new G4OpticalSurface("OpticalSurface");
        opticalSurface->SetType(dielectric_dielectric); // Entre dos medios dieléctricos
        opticalSurface->SetFinish(polished);
        opticalSurface->SetModel(glisur);

        // Propiedades de la superficie
        G4double photonEnergy=10 * eV; // Si es una variable global
        G4int nEntries = 10; // Si es una variable global
        G4double reflectivity[nEntries] = { 0.5, 0.5 }; // Reflectividad de la superficie
        G4MaterialPropertiesTable* surfaceProperties = new G4MaterialPropertiesTable();
        surfaceProperties->AddProperty("REFLECTIVITY", photonEnergy, reflectivity, nEntries);
        opticalSurface->SetMaterialPropertiesTable(surfaceProperties);

        // Aplicar la superficie óptica entre el detector y el mundo
        new G4LogicalBorderSurface("DetectorSurface", physDetector, physWorld, opticalSurface);

        return physWorld;
    }

    void DetectorConstruction::SetTargetThickness(G4double thickness)
    {
        G4cout << "Setting target thickness to: " << thickness << G4endl;
        if (thickness != fTargetThickness) {
            fTargetThickness = thickness;
            G4cout << "Target thickness changed to: " << fTargetThickness << G4endl;
            G4RunManager::GetRunManager()->GeometryHasBeenModified();
        }
        else {
            G4cout << "Target thickness unchanged." << G4endl;
        }
    }

    void DetectorConstruction::ConstructSDandField()
    {
        // Aquí es donde puedes crear campos electromagnéticos si lo necesitas.
    }
}
