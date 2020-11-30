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

  void SaveSecondaries(std::vector<G4int>* PDGcode, std::vector<G4double>* Ekin, std::vector<G4double>* Time, std::vector<TVector3>* pos, std::vector<TVector3>*  dir);
  void IncreasePrimaries(); 
  private:
 
    DetectorConstruction* construction;
  G4String fFileName;
 
    TFile*   rootFile;
    std::vector<int>* ParticlePDGcode;
    std::vector<G4double>* ParticleEnergy;
    std::vector<G4double>* ParticleTime;
    std::vector<TVector3>* ParticlePosition;
    std::vector<TVector3>* ParticleMomentum;
  
 
 
   
  
 
    
 
	
    int Primaries;
    int EventNo;
    TVector3* PPlace;
    TVector3* PDim;


    TTree* Info;


    TTree*   Secondaries;
   

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

