#ifndef DetectorConstruction_hh
#define DetectorConstruction_hh

#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4GenericMessenger.hh"

#include "G4PhysicalVolumeStore.hh"

#include "3.1_DetectorAction.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        MyDetectorConstruction();
        ~MyDetectorConstruction();

        G4LogicalVolume * GetScoringVolume() const {return fScoringVolume;}
        virtual G4VPhysicalVolume * Construct();

        G4Material * GetMaterial() const {return materialTarget;}
	    G4double GetThickness() const {return thicknessTarget;}
    
    private:
        G4Box * solidWorld, * solidDetector, * solidRadiator;
        G4LogicalVolume * logicWorld, * logicRadiator, * logicDetector, * fScoringVolume;
        G4VPhysicalVolume * physicalWorld, * physicalRadiator, * physicalDetector;

        virtual void ConstructSDandField();
        
        void DefineMaterials();

        G4Material * SiO2, * H2O, * Aerogel, * worldMaterial, * Aluminum, * Air, 
                   * Silicon, * materialTarget, * Wolframium, * V2O5, * Bone, * CadTel;
        G4Element * C, * Al, * N, * O, * V, * Cd, * Te;

        G4int nColumns, nRows; 
        G4GenericMessenger * fDetectorMessenger;  

        G4double thicknessTarget;
};

#endif 