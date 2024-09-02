#include "DetectorConstruction.hh"
#include "OpticalPhotonSD.hh"
#include "G4SDManager.hh"

namespace G4_PCM
{
    DetectorConstruction::DetectorConstruction()
        : fTargetThickness(1 * mm), // Valor predeterminado
        fMessenger(new DetectorConstructionMessenger(this)) // Crear el mensajero
    {
        DefineMaterials();
        if (!target) G4cerr << "Error: Target material is not initialized." << G4endl;
        if (!E_PbWO4) G4cerr << "Error: E_PbWO4 material is not initialized." << G4endl;
    }

    DetectorConstruction::~DetectorConstruction()
    {
        delete fMessenger; // Eliminar el mensajero
    }

    G4Material* DefineVanadiumOxide() {
        G4NistManager* nist = G4NistManager::Instance();
        G4double density_V2O5 = 3.36 * g / cm3;
        G4Material* E_V2O5 = new G4Material("VanadiumOxide", density_V2O5, 2);
        E_V2O5->AddElement(nist->FindOrBuildElement("V"), 2);
        E_V2O5->AddElement(nist->FindOrBuildElement("O"), 5);
        return E_V2O5;
    }

    G4Material* DefineAmorphousGlass() {
        G4NistManager* nist = G4NistManager::Instance();
        G4double density_glass = 2.5 * g / cm3;
        G4Material* amorphousGlass = new G4Material("AmorphousGlass", density_glass, 2);
        amorphousGlass->AddElement(nist->FindOrBuildElement("Si"), 1);
        amorphousGlass->AddElement(nist->FindOrBuildElement("O"), 2);
        return amorphousGlass;
    }

    G4Material* DefineVanadiumGlassMix() {
        G4Material* vanadiumOxide = DefineVanadiumOxide();
        G4Material* amorphousGlass = DefineAmorphousGlass();

        G4double density_mix = 2.7 * g / cm3; // Ajusta la densidad según sea necesario
        G4Material* vanadiumGlassMix = new G4Material("VanadiumGlassMix", density_mix, 2);

        G4double fractionMass_VO2 = 0.5;  // Proporción de óxido de vanadio
        G4double fractionMass_SiO2 = 1.0 - fractionMass_VO2; // Proporción de vidrio

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

        G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
        const G4int nEntries = 2;
        G4double photonEnergy[nEntries] = { 2.0 * eV, 3.0 * eV };
        G4double refractiveIndex[nEntries] = { 1.5, 1.5 };
        G4double absorption[nEntries] = { 1 * m, 1 * m };

        mpt->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries);
        mpt->AddProperty("ABSLENGTH", photonEnergy, absorption, nEntries);

        E_PbWO4->SetMaterialPropertiesTable(mpt);

        if (!target) G4cerr << "Error: Target material is not initialized." << G4endl;
        if (!E_PbWO4) G4cerr << "Error: E_PbWO4 material is not initialized." << G4endl;
    }

    G4VPhysicalVolume* DetectorConstruction::Construct()
    {
        G4double worldSize = 1 * m;
        auto solidWorld = new G4Box("World", worldSize / 2, worldSize / 2, worldSize * 10);
        auto logicWorld = new G4LogicalVolume(solidWorld, vacuum, "World");
        auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0);

   /*     G4double innerTargetRadius = 0.0;
        G4double outerTargetRadius = 2.5 * cm;
        G4Tubs* solidTarget = new G4Tubs("Target", innerTargetRadius, outerTargetRadius, fTargetThickness / 2.0, 0.0, 360.0 * deg);
        G4LogicalVolume* logicTarget = new G4LogicalVolume(solidTarget, target, "Target");
        G4ThreeVector targetPos = G4ThreeVector();
        new G4PVPlacement(nullptr, targetPos, logicTarget, "Target", logicWorld, false, 0);*/

        G4double detectorSizeXY = 20 * cm;
        G4double detectorSizeZ = 5 * cm;
        G4Box* solidDetector = new G4Box("Detector", detectorSizeXY, detectorSizeXY, detectorSizeZ);
        G4LogicalVolume* logicDetector = new G4LogicalVolume(solidDetector, E_PbWO4, "Detector");
        G4ThreeVector detectorPos = G4ThreeVector(0, 0, 20 * cm);
        new G4PVPlacement(nullptr, detectorPos, logicDetector, "Detector", logicWorld, false, 0);

        fOpticalPhotonDetector = logicDetector;

        ConstructSDandField();

        return physWorld;
    }

    void DetectorConstruction::ConstructSDandField()
    {
        auto* opticalPhotonSD = new OpticalPhotonSD("OpticalPhotonSD");
        G4SDManager::GetSDMpointer()->AddNewDetector(opticalPhotonSD);
        if (fOpticalPhotonDetector) {
            fOpticalPhotonDetector->SetSensitiveDetector(opticalPhotonSD);
        }
        else {
            G4cerr << "Error: fOpticalPhotonDetector is not initialized." << G4endl;
        }
    }

    void DetectorConstruction::SetTargetThickness(G4double thickness)
    {
        G4cout << "Setting target thickness to: " << thickness << G4endl;
        if (thickness != fTargetThickness) {
            fTargetThickness = thickness;
            G4cout << "Target thickness changed to: " << fTargetThickness << G4endl;
            G4RunManager::GetRunManager()->ReinitializeGeometry();
        }
        else {
            G4cout << "Target thickness unchanged." << G4endl;
        }
    }
}
