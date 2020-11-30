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
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh" 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:logicWorld(0),physicWorld(0),logicalPhantom(0),physicalPhantom(0)

{   //changable variables in SetUp
	PhantomThicknessX	= 6.*cm;	
	PhantomThicknessY	= 6.*cm;	
	PhantomThicknessZ	= 30.*cm;	

	PhantomPlaceX	= 0.*cm;	
	PhantomPlaceY	= 0.*cm;	
	PhantomPlaceZ	= 0.*cm;	

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
	G4double temperature, pressure, fractionmass;
	G4int natoms, ncomps;

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
	
	G4double denPMMA = 1.19*g/cm3;

	//Tracer Candidates

	//Magnesium
	G4Element* Mg = new G4Element("Element_Magnesium", "Mg",12, 24.305*g/mole);
	G4Material* matMg = new G4Material("Magnesium", 1.738*g/cm3, 1);
	matMg->AddElement(Mg, 1);


	//Iron
	G4Element* Fe = new G4Element("Element_Iron", "Fe", 26., 55.845*g/mole);
	G4Material* matFe = new G4Material("Iron", 7.87*g/cm3,1);
	matFe->AddElement(Fe, 1);

	//Chrome
	G4Element* Cr = new G4Element("Element_Chrome", "Cr", 24, 51.996*g/mole);
	G4Material* matCr = new G4Material("Chrome", 7.14*g/cm3, 1);
	matCr->AddElement(Cr, 1);

	//Zirconium
	G4Element* Zr = new G4Element("Element_Zirconium", "Zr", 40, 91.224*g/mole);
	G4Material* matZr = new G4Material("Zirconium", 6.50*g/cm3, 1);
	matZr->AddElement(Zr, 1);


	//Titan 
	G4Element* Ti = new G4Element("Element_Titan", "Ti", 22, 47.867*g/mole);
	G4Material* matTi = new G4Material("Titan", 4.5*g/cm3, 1);
	matTi->AddElement(Ti, 1);


	//Tracer materials

	//Mixture Magnesium & PMMA
	G4double denMg = 1.738*g/cm3;
	G4double fracMassMg = 0.01;
	G4double fracMassPMMA_Mg = 0.99;
	G4double densityTracerMg = (denMg*denPMMA)/(fracMassMg*denPMMA+fracMassPMMA_Mg*denMg);

	G4Material* Tracer_Mg = new G4Material("Tracer_Mg", densityTracerMg, ncomps = 2);
	Tracer_Mg->AddMaterial(mat_PMMA, fractionmass = fracMassPMMA_Mg);
	Tracer_Mg->AddMaterial(matMg, fractionmass = fracMassMg);


	//Mixture Iron & PMMA
	G4double denFe = 7.87*g/cm3;
	G4double fracMassFe = 0.025;
	G4double fracMassPMMA_Fe = 0.975;
	G4double densityTracerFe = (denFe*denPMMA)/(fracMassFe*denPMMA+fracMassPMMA_Fe*denFe);

	G4Material* Tracer_Fe = new G4Material("Tracer_Fe", densityTracerFe, ncomps = 2);
	Tracer_Fe->AddMaterial(mat_PMMA, fractionmass=fracMassPMMA_Fe);
	Tracer_Fe->AddMaterial(matFe, fractionmass= fracMassFe);

	//Mixture Cr  & PMMA
	G4double denCr = 7.14*g/cm3;
	G4double fracMassCr = 0.01;
	G4double fracMassPMMA_Cr = 0.99;
	G4double densityTracerCr = (denCr*denPMMA)/(fracMassCr*denPMMA+fracMassPMMA_Cr*denCr);

	G4Material* Tracer_Cr = new G4Material("Tracer_Cr", densityTracerCr, ncomps = 2);
	Tracer_Cr->AddMaterial(mat_PMMA, fractionmass= fracMassPMMA_Cr);
	Tracer_Cr->AddMaterial(matCr, fractionmass= fracMassCr);

	//Mixture Zr & PMMA
	G4double denZr = 6.5*g/cm3;
	G4double fracMassZr = 0.01;
	G4double fracMassPMMA_Zr = 0.99;
	G4double densityTracerZr = (denZr*denPMMA)/(fracMassZr*denPMMA+fracMassPMMA_Zr*denZr);

	G4Material* Tracer_Zr = new G4Material("Tracer_Zr", densityTracerZr, ncomps = 2);
	Tracer_Zr->AddMaterial(mat_PMMA, fractionmass = fracMassPMMA_Zr);
	Tracer_Zr->AddMaterial(matZr, fractionmass = fracMassZr);
       

	//Mixture Ti & PMMA
	G4double denTi = 4.5*g/cm3;
	G4double fracMassTi = 0.015;
	G4double fracMassPMMA_Ti = 0.985;
	G4double densityTracerTi = (denTi*denPMMA)/(fracMassTi*denPMMA+fracMassPMMA_Ti*denTi);

	G4Material* Tracer_Ti = new G4Material("Tracer_Ti", densityTracerTi, ncomps = 2);
	Tracer_Ti->AddMaterial(mat_PMMA, fractionmass=fracMassPMMA_Ti);
	Tracer_Ti->AddMaterial(matTi, fractionmass=fracMassTi);

	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
	
	//variable materials accessed by macro commands are set to default here
	MaterialPhantom= mat_PMMA;
	TracerMaterial_Mg = matMg;
	TracerMaterial_Fe = matFe;
	TracerMaterial_Cr = matCr;
	TracerMaterial_Zr = matZr;
	TracerMaterial_Ti = matTi;
     
	Tracer1 = Tracer_Mg;
	Tracer2 = Tracer_Fe;
	Tracer3 = Tracer_Cr;
	Tracer4 = Tracer_Zr;
       	Tracer5 = Tracer_Ti;

	
	


        
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

	//Phantom 

	G4Box* solidPhantom = new G4Box("solidPhantom",PhantomThicknessX/2,PhantomThicknessY/2,PhantomThicknessZ/2);

	//construct
	logicalPhantom = new G4LogicalVolume(solidPhantom,MaterialPhantom,"logicalPhantom");	//default Material is PMMA 


	physicalPhantom = new G4PVPlacement(0,G4ThreeVector(PhantomPlaceX,PhantomPlaceY,PhantomPlaceZ),"Phantom",logicalPhantom,physicWorld,false,0);
	
	G4double Rmin = 0.*mm;
	G4double Rmax = 1.*mm;
	G4double Lc = 3.*mm;
	G4double StartPhi = 0.*deg;
	G4double DeltaPhi = 360.*deg;
	
	G4Tubs* solidTracer = new G4Tubs("solidTracer",Rmin,Rmax,Lc/2, StartPhi, DeltaPhi);
	G4LogicalVolume* logicalTracer = new G4LogicalVolume(solidTracer, TracerMaterial_Fe, "logicalTracer");
	G4VPhysicalVolume* physicalTracer = new G4PVPlacement(0, G4ThreeVector(0,0,-7.5*cm), "Tracer", logicalTracer, physicalPhantom, false, 0);

	PhantomSD* phsd = new PhantomSD("PhantomSD","PhantomHits");
      
	G4SDManager::GetSDMpointer()->AddNewDetector(phsd);
	// Register Sensitive Detector to Geant4.
	logicalPhantom->SetSensitiveDetector(phsd);



	//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...
	/*Visualization
	 * 
	 */
	//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...
	// Visualization attributes
	G4VisAttributes * metal       = new G4VisAttributes( G4Colour(204/255. ,204/255. ,255/255.));
	G4VisAttributes * bleuCiel    = new G4VisAttributes( G4Colour(0/255. , 204/255. , 204/255.));
	G4VisAttributes * red         = new G4VisAttributes( G4Colour(255/255., 0/255., 0/255.));
	G4VisAttributes * roux        = new G4VisAttributes( G4Colour(204/255., 0/255., 51/255. ));
	G4VisAttributes * yellow      = new G4VisAttributes( G4Colour(255/255., 255/255., 0/255.));
	G4VisAttributes * green       = new G4VisAttributes( G4Colour(0/255., 255/255., 0/255.));
	G4VisAttributes * Lgreen      = new G4VisAttributes( G4Colour(153/255. ,255/255. ,153/255.));
	G4VisAttributes * Lbrown       = new G4VisAttributes (G4Colour(0.5,0.5,0.1));
	G4VisAttributes * brown       = new G4VisAttributes (G4Colour(0.7,0.4,0.1));
	G4VisAttributes * darkGrey    = new G4VisAttributes (G4Colour(0.6,0.6,0.6));
	G4VisAttributes * white       = new G4VisAttributes (G4Colour(1,1,1));
	G4VisAttributes * purple      = new G4VisAttributes (G4Colour(0.63, 0.13, 0.94));	
	G4VisAttributes * blue      = new G4VisAttributes (G4Colour(0, 0, 1));

	//World
	logicWorld->SetVisAttributes (G4VisAttributes::Invisible);
	//logicWorld->SetVisAttributes(bleuCiel);

	logicalPhantom ->SetVisAttributes(brown);
	logicalTracer->SetVisAttributes(blue);


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
	  MaterialPhantom= pttoMaterial;
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
