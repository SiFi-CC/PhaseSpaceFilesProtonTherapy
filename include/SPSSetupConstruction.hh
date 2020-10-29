#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "TVector3.h"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
	public:

		DetectorConstruction();
		~DetectorConstruction();

		G4VPhysicalVolume* Construct();

		void setPhantomThickness(G4double);
		TVector3* getPhantomDimensions();
		TVector3* getPhantomPlace();
		void setPhantomMaterial(G4String);
		void UpdateGeometry();
                void SetPDGcode(G4int);
  
        public:
           
           G4Material* GetMaterial(){return material;};
          
	private:

		//***********  World

		G4LogicalVolume*   logicWorld;    
		G4VPhysicalVolume* physicWorld; 


		//*********** Phantom 
		G4LogicalVolume* logicalPhantom;
		G4VPhysicalVolume* physicalPhantom;


		//***********  Material
		G4Material* Vacuum;
		G4Material* mat_Air;
                G4Material* material;


		G4Material* mat_Graphite;
		G4Material* mat_PMMA;
		G4Material* mat_POM;
                
		
		G4Material* MaterialPhantom; //pointer to Phantom Material (default mat_PMMA)
                G4Material* TracerMaterial_Ca;
                G4Material* TracerMaterial_Mg;
                G4Material* TracerMaterial_Fe;
                G4Material* Tracer1;
                G4Material* Tracer2;
                G4Material* Tracer3;
  

		DetectorMessenger* detectorMessenger;

		//changeable distances and angles
		G4double dist_NozzletoWedge;

		G4double PhantomThicknessX;
		G4double PhantomThicknessY;
		G4double PhantomThicknessZ;
		
		G4double PhantomPlaceX;
		G4double PhantomPlaceY;
		G4double PhantomPlaceZ;

                G4int pdgCode;

	private:

		void DefineMaterials();
		G4VPhysicalVolume* ConstructSetUp();   
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
