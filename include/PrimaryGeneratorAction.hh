#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
class G4GeneralParticleSource;
class G4Event;

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
		const G4GeneralParticleSource* GetParticleGun() const { return particleGun; }


	private:

		G4GeneralParticleSource* particleGun;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


