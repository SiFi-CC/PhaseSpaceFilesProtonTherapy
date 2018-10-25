

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
class G4ParticleGun;
class G4GeneralParticleSource;
class G4Event;
class SPSPhaseSpaceMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
	public:
		PrimaryGeneratorAction();    
		~PrimaryGeneratorAction();

	public:
		//method from base class
		virtual void GeneratePrimaries(G4Event*);

		//method to access particle gun
		const G4ParticleGun* GetParticleGun() const { return particleGun; }
		//~ const G4GeneralParticleSource* GetParticleGun() const { return particleGun; }

		void SetInputFile(G4String PhaseSpaceFile);
		void SetInputStartNumber(G4int n_iteration);
		void ReadTTree(G4String fname);

	private:

		//~ G4GeneralParticleSource* particleGun;
		G4ParticleGun* particleGun; //pointer to G4Gun class


		SPSPhaseSpaceMessenger* gunMessenger;

		G4String InputPhaseSpaceFile;

		TFile* InputFile; TTree* InputTree;
		TBranch * InputEnergy;
		TBranch * InputXPos; TBranch * InputYPos;
		TBranch * InputXCos; TBranch * InputYCos;
		Long_t nb;

		G4int iStart;

		G4double zStart, xStart, yStart;
		G4double EStart;
		G4double xcosStart, ycosStart, zcosStart;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


