// K. Laihem
// GEANT4 tag $Name: geant4-09-06 $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SPSPhaseSpaceMessenger.hh"
#include "PrimaryGeneratorAction.hh"
#include "SPSSetupConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"

#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

	SPSPhaseSpaceMessenger::SPSPhaseSpaceMessenger(PrimaryGeneratorAction* myAction)
:myPrimaryGeneratorAction(myAction)
{

	G4UIdirectory* PhaseSpaceDir = new G4UIdirectory("/HITRun/PhaseSpace/");
	PhaseSpaceDir->SetGuidance("PhaseSpaceDir: directory controls Primary Generator Action.");


	setInputFileCmd = new G4UIcmdWithAString("/HITRun/PhaseSpace/SetPhaseSpaceFile",this);
	setInputFileCmd->SetGuidance("Set Input File: ROOT::TFile with E XPos YPos XCos YCos in TTrees");
	setInputFileCmd->SetParameterName("InputFile",false,false);
	setInputFileCmd->SetDefaultValue("21001_PHASEAP");
	setInputFileCmd->SetCandidates("103001_PHASESP 103001_PHASEAP 103RiFi001_PHASESP 103RiFi001_PHASEAP 21001_PHASESP 21001_PHASEAP");
	setInputFileCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	setInputStartNoCmd = new G4UIcmdWithAnInteger("/HITRun/PhaseSpace/SetInitialProton", this);
	setInputStartNoCmd->SetGuidance("Set Initial Proton: define line of PhaseSpaceFile to be read for first proton");
	setInputStartNoCmd->SetParameterName("InputInitNo",false,false);
	setInputStartNoCmd->SetDefaultValue(0);
	setInputStartNoCmd->SetRange("InputInitNo>=0 && InputInitNo <10000000");
	setInputStartNoCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SPSPhaseSpaceMessenger::~SPSPhaseSpaceMessenger()
{
	//~ delete setInputFileCmd;
	//~ delete setInputStartNoCmd;
	//~ delete PhaseSpaceDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SPSPhaseSpaceMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
	if( command == setInputFileCmd) 
	{ myPrimaryGeneratorAction->SetInputFile(newValue);}

	if (command == setInputStartNoCmd) 
	{ myPrimaryGeneratorAction->SetInputStartNumber(setInputStartNoCmd->GetNewIntValue(newValue));}
}
