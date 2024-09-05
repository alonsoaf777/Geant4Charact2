#ifndef G4_PCM_DETECTOR_CONSTRUCTION_H
#define G4_PCM_DETECTOR_CONSTRUCTION_H 1

#include "G4NistManager.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "DetectorConstructionMessenger.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4OpticalSurface.hh"

// Forward declaration of the sensitive detector class
class MySensitiveDetector;

namespace G4_PCM
{
    class DetectorConstructionMessenger; // Forward declaration

    class DetectorConstruction : public G4VUserDetectorConstruction
    {
    public:
        DetectorConstruction();
        ~DetectorConstruction() override;

        G4VPhysicalVolume* Construct() override;

        void SetTargetThickness(G4double thickness);

        G4LogicalVolume* GetGammaDetector() const { return fGammaDetector; }

    private:
        G4LogicalVolume* fGammaDetector = nullptr;
        G4double fTargetThickness = 60 * mm; // Valor predeterminado

        G4UIcmdWithADoubleAndUnit* fTargetThicknessCmd;
        DetectorConstructionMessenger* fMessenger; // Puntero al mensajero

        G4Material* target;
        G4Material* vacuum;
        G4Material* E_PbWO4;
        G4Material* DefineVanadiumOxide();
        G4Material* DefineAmorphousGlass();
        G4Material* DefineVanadiumGlassMix();

        // Método para definir materiales
        void DefineMaterials();

        // Método para construir el detector sensible y definir barreras
        void ConstructSDandField();

        // Puntero a la clase del detector sensible
        MySensitiveDetector* fSensitiveDetector;
    };
}

#endif
