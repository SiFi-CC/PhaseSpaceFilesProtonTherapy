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

    void SaveSecondary(G4int PDGcode, G4double Ekin, TVector3*  pos, TVector3*  dir);
    void IncreasePrimaries(); 
  private:
 
    DetectorConstruction* construction;
 
    TFile*   rootFile;
    int ParticlePDGcode;
    G4double ParticleEnergie;
    TVector3* ParticlePosition;
    TVector3* ParticleMomentum;
	
    int Primaries;
    TVector3* PPlace;
    TVector3* PDim;

    TTree* Info;
	
    TTree*   Gamma;
    TTree*   Neutron;
    TTree*   Else;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

