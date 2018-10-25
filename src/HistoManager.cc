#include "G4SystemOfUnits.hh"
#include "HistoManager.hh"
#include "G4UnitsTable.hh"



#ifdef G4ANALYSIS_USE
#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#endif


extern G4String TrackInfo;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager(DetectorConstruction* dec)
	:construction(dec),
	rootFile(0),
	ParticlePDGcode(0),
	ParticleEnergie(0),
	Primaries(0)  
{   // histograms
	ParticlePosition = new TVector3(0,0,0);
	ParticleMomentum = new TVector3(0,0,0);
	PPlace = new TVector3(0,0,0);
	PDim = new TVector3(0,0,0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
#ifdef G4ANALYSIS_USE 
	if ( rootFile ) delete rootFile;
#endif    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::book(G4int runNb)
{ 
#ifdef G4ANALYSIS_USE
	std::stringstream fileName;

	fileName << "Out_run_" << runNb << ".root";
	cout<<" The ROOT file is : "<< fileName.str() <<endl;

	//G4String dataDir = "~/Masterthesis/";
	G4String dataDir = "./";
	TString dataPath=dataDir+fileName.str();

	rootFile = new TFile(dataPath,"RECREATE");

	if(!rootFile) {
		cout << " HistoManager::book :" 
			<< " problem creating the ROOT TFile "
			<< endl;
		return;
	}

	// Histogram 1D energy resolution of 2 keV for 5500 bins
	rootFile->cd();
	Info = new TTree("Info","Info about the runs");
	Info->Branch("Primaries", &Primaries,"Primaries/I");
	Info->Branch("PhantomPlace"     , &PPlace ); 
	Info->Branch("PhantomDimensions"     , &PDim ); 

	Gamma = new TTree("Gammas", "Gammas in the Phantom");
	Gamma->Branch("ParticleEnergie"     , &ParticleEnergie  , "ParticleEnergie/D");
	Gamma->Branch("ParticlePosition"     , &ParticlePosition  ); 
	Gamma->Branch("ParticleMomentum"     , &ParticleMomentum ); 
	
	Neutron = new TTree("Neutron", "Neutron in the Phantom");
	Neutron->Branch("ParticleEnergie"     , &ParticleEnergie  , "ParticleEnergie/D");
	Neutron->Branch("ParticlePosition"     , &ParticlePosition  ); 
	Neutron->Branch("ParticleMomentum"     , &ParticleMomentum ); 
	
	Else = new TTree("ElseParticle", "All other ParticlAll other Particlee Phantom");
	Else->Branch("ParticlePDGcode"   , &ParticlePDGcode   , "ParticlePDGcode/I");
	Else->Branch("ParticleEnergie"     , &ParticleEnergie  , "ParticleEnergie/D");
	Else->Branch("ParticlePosition"     , &ParticlePosition  ); 
	Else->Branch("ParticleMomentum"     , &ParticleMomentum ); 
#endif
	PPlace->SetXYZ(construction->getPhantomPlace()->x(),construction->getPhantomPlace()->y(),construction->getPhantomPlace()->z());
	PDim= construction->getPhantomDimensions();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::save()
{ 
#ifdef G4ANALYSIS_USE
	if (rootFile) {
		Info->Fill();

		rootFile->Write();       // Writing the histograms to the file

		rootFile->Close();        // and closing the tree (and the file)

		G4cout << "\n---->  Rootfile is saved \n" << G4endl;
	}
#endif
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void HistoManager::SaveSecondary(G4int PDGcode, G4double Ekin, TVector3* pos, TVector3*  dir){
#ifdef G4ANALYSIS_USE
	ParticlePDGcode = PDGcode;
	ParticleEnergie = Ekin/MeV;
	ParticlePosition= pos;
    	ParticleMomentum= dir;
	if (ParticlePDGcode == 22) Gamma->Fill();
	else if (ParticlePDGcode == 2112) Neutron->Fill();
	//else Else->Fill();
    
#endif 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::IncreasePrimaries(){
    Primaries++;
}

