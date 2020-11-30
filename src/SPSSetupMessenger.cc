#include "SPSSetupMessenger.hh"
#include "SPSSetupConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
//#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADouble.hh"

#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorMessenger::DetectorMessenger(DetectorConstruction* myDet)
:myDetector(myDet)
{ 


	HITSetUpDir = new G4UIdirectory("/SPSSetUp/");
	HITSetUpDir->SetGuidance("UI commands specific to  setup.");

	PhantomMaterialCmd = new G4UIcmdWithAString("/SPSSetUp/setPhantomMaterial",this);
	PhantomMaterialCmd->SetGuidance("Select Material of the Phantom Target (Default = Graphite).");
	PhantomMaterialCmd->SetParameterName("choice",false,true);
	PhantomMaterialCmd->SetDefaultValue("PMMA");
	PhantomMaterialCmd->SetCandidates("Graphite PMMA POM G4_AIR Vacuum");
	PhantomMaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	PhantomThicknessCmd = new G4UIcmdWithADoubleAndUnit("/SPSSetUp/setPhantomThickness",this);  
	PhantomThicknessCmd->SetGuidance("Set Thickness of 2 Phantoms combined at z-Axis");
	PhantomThicknessCmd->SetGuidance("double and unit must be a length in range (0.2,200)mm");
	PhantomThicknessCmd->SetParameterName("PhantomThick",false,false);
	PhantomThicknessCmd->SetUnitCategory("Length");
	PhantomThicknessCmd->SetRange("PhantomThick>0 && PhantomThick<799.");
	PhantomThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	//------------------------------    


	UpdateCmd = new G4UIcmdWithoutParameter("/SPSSetUp/UpdateGeometry",this);
	UpdateCmd->SetGuidance("Update HITSetUp geometry");
	UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
	UpdateCmd->SetGuidance("if you changed geometrical value(s).");
	UpdateCmd->AvailableForStates(G4State_PreInit,G4State_Idle);


	//------------------------------    

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
	delete PhantomMaterialCmd;
	delete PhantomThicknessCmd;
	delete HITSetUpDir;

	delete UpdateCmd;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 

	if( command == PhantomMaterialCmd) {
		myDetector->setPhantomMaterial(newValue);
		myDetector->UpdateGeometry();
	}

	if( command == PhantomThicknessCmd) {
		myDetector->setPhantomThickness(PhantomThicknessCmd->GetNewDoubleValue(newValue));
		myDetector->UpdateGeometry();
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
