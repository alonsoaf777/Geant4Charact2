#include "DetectorConstruction.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"

namespace G4_PCM
{
    DetectorConstruction::DetectorConstruction()
        : fTargetThickness(1 * mm), // Valor predeterminado
        fMessenger(new DetectorConstructionMessenger(this)) // Crear el mensajero
    {
        DefineMaterials();
        // DefineOpticalProperties() se llamar� despu�s de la construcci�n de los vol�menes f�sicos
    }

    DetectorConstruction::~DetectorConstruction()
    {
        delete fMessenger; // Eliminar el mensajero
    }

    G4Material* DefineRealisticMaterial()
    {
        // Densidad en g/cm�
        G4double density = 1e-25 * g / cm3; // Ejemplo: vidrio de s�lice
        G4Material* realisticMaterial = new G4Material("RealisticMaterial", density, 1);

        // A�adir elementos (ejemplo para vidrio de s�lice)
        G4NistManager* nist = G4NistManager::Instance();
        realisticMaterial->AddElement(nist->FindOrBuildElement("O"), 2);

        return realisticMaterial;
    }

    void DetectorConstruction::DefineMaterials()
    {
        G4NistManager* nist = G4NistManager::Instance();
        // Reemplazamos el vac�o con un material m�s denso
        // detector = DefineRealisticMaterial();
        detector = nist->FindOrBuildMaterial("G4_Galactic");
        vacuum = nist->FindOrBuildMaterial("G4_Galactic"); // Para otros usos si es necesario
    }

    void DetectorConstruction::DefineOpticalProperties()
    {
        const G4int numEntries = 12;
        G4double photonEnergy[numEntries] = { 0.496 * eV, 0.7 * eV, 1.0 * eV, 1.55 * eV, 2.0 * eV, 3.0 * eV, 4.0 * eV, 5.0 * eV, 6.0 * eV, 8.0 * eV, 10.0 * eV, 12.4 * eV };

        // Valores de �ndice de refracci�n realistas para vidrio
        G4double refractiveIndex[numEntries] = { 1.48, 1.48, 1.48, 1.48, 1.48, 1.48, 1.48, 1.48, 1.48, 1.48, 1.48, 1.48 };

        // Longitud de absorci�n realista para vidrio (var�a dependiendo del material)
        G4double absorptionLength[numEntries] = { 0.1 * m, 0.1 * m, 0.1 * m, 0.1 * m, 0.1 * m, 0.1 * m, 0.1 * m, 0.1 * m, 0.1 * m, 0.1 * m, 0.1 * m, 0.1 * m };

        G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();
        MPT->AddProperty("RINDEX", photonEnergy, refractiveIndex, numEntries);
        MPT->AddProperty("ABSLENGTH", photonEnergy, absorptionLength, numEntries);

        // Asignar la tabla de propiedades al material
        detector->SetMaterialPropertiesTable(MPT);
    }

    G4VPhysicalVolume* DetectorConstruction::Construct()
    {
        // Definir el tama�o del mundo
        G4double worldSize = 1 * m;
        auto solidWorld = new G4Box("World",
            worldSize / 2,
            worldSize / 2,
            worldSize * 5);
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
        G4double detectorSizeZ = 1 * m;

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
        G4VPhysicalVolume* DetectorXD = new G4PVPlacement(detRotation,
            detectorPos,
            fDetectorLog,
            "Detector",
            fWorldLog,
            false,
            0);

        // Crear una superficie �ptica
        G4OpticalSurface* opticalSurface = new G4OpticalSurface("OpticalSurface");
        opticalSurface->SetType(dielectric_dielectric);
        opticalSurface->SetFinish(polished);
        opticalSurface->SetModel(unified);

        // Crear una superficie �ptica en el borde
        new G4LogicalBorderSurface("OpticalSurface", physWorld, DetectorXD, opticalSurface);

        // Crear una superficie �ptica en el material del detector
        G4LogicalSkinSurface* skinSurface = new G4LogicalSkinSurface("SkinSurface", fDetectorLog, opticalSurface);

        // Definir este detector como el detector gamma
        fGammaDetector = fDetectorLog;

        // Llamar a DefineOpticalProperties despu�s de que los vol�menes f�sicos se hayan creado
        DefineOpticalProperties();

        return physWorld;
    }

    void DetectorConstruction::SetTargetThickness(G4double thickness)
    {
        G4cout << "Setting target thickness to: " << thickness << G4endl;
        if (thickness != fTargetThickness) {
            fTargetThickness = thickness;
            G4cout << "Target thickness changed to: " << fTargetThickness << G4endl;

            // Forzar la actualizaci�n de la geometr�a
            G4RunManager::GetRunManager()->ReinitializeGeometry(); // Descomentado para actualizar la geometr�a
        }
        else {
            G4cout << "Target thickness unchanged." << G4endl;
        }
    }
}
