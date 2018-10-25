#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
//class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADouble;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorMessenger: public G4UImessenger
{
	public:
		DetectorMessenger(DetectorConstruction*);
		~DetectorMessenger();

		void SetNewValue(G4UIcommand*, G4String);

	private:
		DetectorConstruction* myDetector;

		G4UIdirectory*             HITSetUpDir;

		G4UIcmdWithAString*        PhantomMaterialCmd;
		G4UIcmdWithADoubleAndUnit* PhantomThicknessCmd;


		G4UIcmdWithoutParameter*   UpdateCmd;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

