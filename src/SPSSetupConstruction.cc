#include "SPSSetupConstruction.hh"
#include "SPSSetupMessenger.hh"


#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"


#include "G4GeometryManager.hh"
#include "G4RunManager.hh"
#include "G4SolidStore.hh"
#include "G4RegionStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"


#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "TVector3.h"

#include "PhantomSD.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:logicWorld(0),physicWorld(0),logicalPhantom(0),physicalPhantom(0),logicalCylinder(0),physicalCylinder(0)

{   //changable variables in SetUp
	PhantomThicknessX	= 6.*cm;	
	PhantomThicknessY	= 6.*cm;	
	PhantomThicknessZ	= 30.*cm;	

	PhantomPlaceX	= 0.*cm;	
	PhantomPlaceY	= 0.*cm;	
	PhantomPlaceZ	= 0.*cm;
	
   CylinderRMin =   25.*cm;
   CylinderRMax =  25.01*cm;
   CylinderLz   =  100.*cm;
   CylinderPhiS =   0.*deg;
   CylinderPhiE = 360.*deg;	
   


	detectorMessenger = new DetectorMessenger(this);
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
	delete detectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	DefineMaterials();
	return ConstructSetUp();

}

void DetectorConstruction::DefineMaterials()
{
	G4NistManager* nman= G4NistManager::Instance();


	G4String name, symbol;        
	G4double A, Z, density;           
	G4double temperature, pressure;
	G4int natoms;

	// Vaccum material
	name        = "Vacuum";
	density     = universe_mean_density;
	pressure    = 3.e-18*pascal;
	temperature = 2.73*kelvin;
	Z=1.;
	A=1.01*g/mole;
	Vacuum = new G4Material(name, Z, A , density, kStateGas,temperature,pressure);

	mat_Air	= nman->FindOrBuildMaterial("G4_AIR");			//World

	// define Elements, copied from Sabine
	G4Element* N	= new G4Element(name="Nitrogen","N" , 7., 14.01*g/mole);
	G4Element* O	= new G4Element(name="Oxygen"  ,"O" , 8., 16.00*g/mole);
	G4Element* C	= new G4Element("Carbon","C",6.,12.*g/mole);
	G4Element* H	= new G4Element("Hydrogen","H",1.,1.01*g/mole);

	mat_Graphite = nman->BuildMaterialWithNewDensity("Graphite","G4_C",1.74*g/cm3);
	mat_PMMA = nman->BuildMaterialWithNewDensity("PMMA","G4_PLEXIGLASS",1.19*g/cm3);
	mat_POM = nman->BuildMaterialWithNewDensity("POM","G4_POLYOXYMETHYLENE",1.43 *g/cm3);

	G4cout << *(G4Material::GetMaterialTable()) << G4endl;

	//variable materials accessed by macro commands are set to default here
	MaterialPhantom= mat_PMMA;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructSetUp()
{

	//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
	//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
	//     
	// World
	//

	G4double           WorldSizeX = 1 *m;
	G4double           WorldSizeY = 1 *m;
	G4double           WorldSizeZ = 1 *m;

	G4Box* solidWorld = new G4Box("solidsWorld",				//its name
			WorldSizeX/2,WorldSizeY/2,WorldSizeZ/2);//its size

	logicWorld	= new G4LogicalVolume(solidWorld,mat_Air,"logicWorld");		    //its solid,defaultMaterial,name

	physicWorld	= new G4PVPlacement(0,G4ThreeVector(),	//no rotation,at (0,0,0)
			"physicWorld",	//its name
			logicWorld,	//its logical volume
			0,		//its mother  volume
			false,		//no boolean operation
			0);		//copy number
	//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...


	//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...

	
	//Cylinder
	

  G4Tubs* solidCylinder = new G4Tubs("solidCylinder", CylinderRMin, CylinderRMax, CylinderLz/2, CylinderPhiS, CylinderPhiE);
  
  logicalCylinder = new G4LogicalVolume(solidCylinder, mat_Air, "logicalCylinder");   // its solid, defaultMaterial, name

  physicalCylinder =
                                 new G4PVPlacement(0,			               // no rotation
  							     G4ThreeVector(0,0,0.*cm),                    // at (0,0,0)
							     "physicalCylinder",                         // its name
							     logicalCylinder,                           // its logical volume
							     physicWorld,	                           // its mother  volume
							     false,		                              // not used
							     0);		                             // copy number 
	
	//Phantom 

	G4Box* solidPhantom = new G4Box("solidPhantom",PhantomThicknessX/2,PhantomThicknessY/2,PhantomThicknessZ/2);

	//construct
	logicalPhantom = new G4LogicalVolume(solidPhantom,MaterialPhantom,"logicalPhantom");	//default Material is PMMA 


	physicalPhantom = new G4PVPlacement(0,G4ThreeVector(PhantomPlaceX,PhantomPlaceY,PhantomPlaceZ),"Phantom", logicalPhantom, physicWorld,false,0);


	PhantomSD* phsd = new PhantomSD("PhantomSD","Phantom2Hits"); 


	// Register Sensitive Detector to Geant4.
	G4SDManager::GetSDMpointer()->AddNewDetector(phsd);
	logicalCylinder->SetSensitiveDetector(phsd);

	//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...
	/*Visualization
	 * 
	 */
	//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...
	// Visualization attributes
	//G4VisAttributes * metal       = new G4VisAttributes( G4Colour(204/255. ,204/255. ,255/255.));
	//G4VisAttributes * bleuCiel    = new G4VisAttributes( G4Colour(0/255. , 204/255. , 204/255.));
	//G4VisAttributes * red         = new G4VisAttributes( G4Colour(255/255., 0/255., 0/255.));
	//G4VisAttributes * roux        = new G4VisAttributes( G4Colour(204/255., 0/255., 51/255. ));
	G4VisAttributes * yellow      = new G4VisAttributes( G4Colour(255/255., 255/255., 0/255.));
	//G4VisAttributes * green       = new G4VisAttributes( G4Colour(0/255., 255/255., 0/255.));
	//G4VisAttributes * Lgreen      = new G4VisAttributes( G4Colour(153/255. ,255/255. ,153/255.));
	//G4VisAttributes * Lbrown       = new G4VisAttributes (G4Colour(0.5,0.5,0.1));
	//G4VisAttributes * brown       = new G4VisAttributes (G4Colour(0.7,0.4,0.1));
	G4VisAttributes * darkGrey    = new G4VisAttributes (G4Colour(0.6,0.6,0.6));
	//G4VisAttributes * white       = new G4VisAttributes (G4Colour(1,1,1));
	//G4VisAttributes * purple      = new G4VisAttributes (G4Colour(0.63, 0.13, 0.94));	
	//G4VisAttributes * blue      = new G4VisAttributes (G4Colour(0, 0, 1));

	//World
	logicWorld->SetVisAttributes (G4VisAttributes::Invisible);
	//logicWorld->SetVisAttributes(bleuCiel);

	logicalPhantom ->SetVisAttributes (yellow);
	logicalCylinder->SetVisAttributes (darkGrey);



	return physicWorld;
}


void DetectorConstruction::setPhantomThickness(G4double ZPhantom)
{
	if (ZPhantom>0.)
	{
		PhantomThicknessZ = ZPhantom;       
		G4cout << "\n----> The new  Phantom thick is " << ZPhantom << " mm."<< G4endl;
	} else {
		G4cout << "Proposed Thickness is out of range (should be >0)." <<G4endl;
	}
}

void DetectorConstruction::setPhantomMaterial(G4String materialName)		//This function changes the Wedgematerial
{
	// search the material by its name 
	G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
	if (pttoMaterial)
	{
		MaterialPhantom = pttoMaterial;
		logicalPhantom->SetMaterial(pttoMaterial); 
	}             
}

void DetectorConstruction::UpdateGeometry() 					//This function updates the geometry, needs to be called after each change during execution
{
	if (!physicWorld) return;

	// clean-up old geometry
	G4GeometryManager::GetInstance()->OpenGeometry();

	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();


	//define new geometry
	G4RunManager::GetRunManager()->DefineWorldVolume(ConstructSetUp());

	G4RunManager::GetRunManager()->GeometryHasBeenModified();
	G4RunManager::GetRunManager()->PhysicsHasBeenModified();

	G4RegionStore::GetInstance()->UpdateMaterialList(physicWorld);
}

TVector3* DetectorConstruction::getPhantomDimensions()
{
	TVector3* dim = new TVector3(PhantomThicknessX,PhantomThicknessY,PhantomThicknessZ);
	return dim;
}

TVector3* DetectorConstruction::getPhantomPlace()
{
	TVector3* place = new TVector3(PhantomPlaceX,PhantomPlaceY,PhantomPlaceZ);
	return place;
}

