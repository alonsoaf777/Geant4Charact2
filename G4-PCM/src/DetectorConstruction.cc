#include "DetectorConstruction.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

namespace G4_PCM
{
    DetectorConstruction::DetectorConstruction()
        : fTargetThickness(1 * mm), // Valor predeterminado
        fMessenger(new DetectorConstructionMessenger(this)) // Crear el mensajero
    {
        DefineMaterials();
        DefineOpticalProperties();
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

        G4double fractionMass_VO2 = 0.50;  // Proporción de óxido de vanadio
        G4double fractionMass_SiO2 = 1.0 - fractionMass_VO2; // Proporción de vidrio

        vanadiumGlassMix->AddMaterial(vanadiumOxide, fractionMass_VO2);
        vanadiumGlassMix->AddMaterial(amorphousGlass, fractionMass_SiO2);

        return vanadiumGlassMix;
    }

    void DetectorConstruction::DefineMaterials()
    {
        G4NistManager* nist = G4NistManager::Instance();
        E_PbWO4 = new G4Material("E_PbWO4", 8.28 * g / cm3, 3);
        E_PbWO4->AddElement(nist->FindOrBuildElement("Pb"), 1);
        E_PbWO4->AddElement(nist->FindOrBuildElement("W"), 1);
        E_PbWO4->AddElement(nist->FindOrBuildElement("O"), 4);

        target = DefineVanadiumGlassMix();
        vacuum = nist->FindOrBuildMaterial("G4_Galactic");
        detector = nist->FindOrBuildMaterial("G4_AIR");
    }

    void DetectorConstruction::DefineOpticalProperties()
    {
        // Define the optical properties of the detector material
        const G4int numEntries = 2;
        G4double photonEnergy[numEntries] = { 1.55 * eV, 7.76 * eV }; // Example range from ~800 nm to ~160 nm
        G4double refractiveIndex[numEntries] = { 1.5, 1.5 }; // Example constant index of refraction
        G4double absorptionLength[numEntries] = { 1.0 * m, 1.0 * m }; // Example constant absorption length

        // Create a material properties table and add properties
        G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();
        MPT->AddProperty("RINDEX", photonEnergy, refractiveIndex, numEntries);
        MPT->AddProperty("ABSLENGTH", photonEnergy, absorptionLength, numEntries);

        // Assign properties table to the material
        detector->SetMaterialPropertiesTable(MPT);

        // Crear una superficie óptica
        G4OpticalSurface* opticalSurface = new G4OpticalSurface("OpticalSurface");
        opticalSurface->SetType(dielectric_dielectric);
        opticalSurface->SetFinish(polished);
        opticalSurface->SetModel(unified);

        // Create a logical border surface (for demonstration)
        auto obsopticalSurface = new G4LogicalBorderSurface("OpticalSurface", fWorldLog, fDetectorLog, opticalSurface);
    }

    G4VPhysicalVolume* DetectorConstruction::Construct()
    {
        // Define el tamaño del mundo
        G4double worldSize = 1 * m;
        auto solidWorld = new G4Box("World",
            worldSize / 2,
            worldSize / 2,
            worldSize * 2);
        fWorldLog = new G4LogicalVolume(solidWorld,
            vacuum,
            "World");
        auto physWorld = new G4PVPlacement(nullptr,
            G4ThreeVector(),
            fWorldLog,
            "World",
            nullptr,
            false,
            0);

        // Crear el detector
        G4double detectorSizeXY = 20 * cm;
        G4double detectorSizeZ = 5 * cm;

        auto solidDetector = new G4Box(
            "Detector",
            detectorSizeXY,
            detectorSizeXY,
            detectorSizeZ);

        fDetectorLog = new G4LogicalVolume(
            solidDetector,
            detector,
            "Detector");

        G4ThreeVector detectorPos = G4ThreeVector(0, 0, 20 * cm);
        G4RotationMatrix* detRotation = new G4RotationMatrix();

        // Colocar el detector
        new G4PVPlacement(detRotation,
            detectorPos,
            fDetectorLog,
            "Detector",
            fWorldLog,
            false,
            0);

        // Definir este detector como el detector gamma
        fGammaDetector = fDetectorLog;

        return physWorld;
    }

    void DetectorConstruction::SetTargetThickness(G4double thickness)
    {
        G4cout << "Setting target thickness to: " << thickness << G4endl;
        if (thickness != fTargetThickness) {
            fTargetThickness = thickness;
            G4cout << "Target thickness changed to: " << fTargetThickness << G4endl;

            // Forzar la actualización de la geometría
            G4RunManager::GetRunManager()->ReinitializeGeometry(); // Descomentado para actualizar la geometría
        }
        else {
            G4cout << "Target thickness unchanged." << G4endl;
        }
    }
}
