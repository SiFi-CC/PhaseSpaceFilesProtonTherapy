// Geant4 Training event "Colorimetry in HEP" - January 29 2014
// Geant4 project and exercise: Lead-Scintillator Sandwich calorimeter with interactive geometry and materials.

// K. Laihem
// GEANT4 tag $Name: geant4-09-06 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef SPSPhaseSpaceGPSMessenger_h
#define SPSPhaseSpaceGPSMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
//class G4UIcmdWithADoubleAndUnit;
//class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SPSPhaseSpaceMessenger: public G4UImessenger
{
	public:
		SPSPhaseSpaceMessenger(PrimaryGeneratorAction*);
		~SPSPhaseSpaceMessenger();

		void SetNewValue(G4UIcommand*, G4String);

	private:
		PrimaryGeneratorAction* myPrimaryGeneratorAction;
		G4UIdirectory*		PhaseSpaceDir;
		G4UIcmdWithAString*	setInputFileCmd;
		G4UIcmdWithAnInteger*	setInputStartNoCmd;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
