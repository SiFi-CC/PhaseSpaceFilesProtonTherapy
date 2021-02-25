#ifndef HistoManager_h
#define HistoManager_h 1

#include <iostream>
#include <string>
using namespace std;


#include "globals.hh"
#include "G4ThreeVector.hh"
#include "TVector3.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#define G4ANALYSIS_USE

#include "SPSSetupConstruction.hh"
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistoManager
{
  public:
  
    HistoManager(DetectorConstruction* dec);
   ~HistoManager();
  
    void book(G4int runNb);
    void save();

    void SaveSecondaries(std::vector<G4int>* PDGcode, std::vector<G4double>* Ekin, std::vector<G4double>* Time, std::vector<TVector3>* pos, std::vector<TVector3>*  dir,std::vector<std::string>* process,std::vector<G4int>* motherPDGcode, std::vector<G4double>* motherEkin, std::vector<TVector3>*  motherdir);
    void IncreasePrimaries(); 
  private:
 
    DetectorConstruction* construction;
 
    TFile*   rootFile;
    std::vector<int>* ParticlePDGcode;
    std::vector<G4double>* ParticleEnergy;
    std::vector<G4double>* ParticleTime;
    std::vector<std::string>* ParticleCreatorProcess;
    std::vector<TVector3>* ParticlePosition;
    std::vector<TVector3>* ParticleMomentum;
    std::vector<int>* MotherParticlePDGcode;
    std::vector<G4double>* MotherParticleEnergy;
    std::vector<TVector3>* MotherParticleMomentum;
	
    int Primaries;
    TVector3* PPlace;
    TVector3* PDim;

    TTree* Info;
	
    TTree*   Secondaries;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

