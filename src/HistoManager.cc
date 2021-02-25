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
	Primaries(0)  
{   
	ParticlePosition = new std::vector<TVector3>;
	ParticleMomentum =  new std::vector<TVector3>;
	ParticlePDGcode=  new std::vector<int>;
	ParticleCreatorProcess=  new std::vector<std::string>;
	ParticleEnergy =  new std::vector<double>;
	ParticleTime =  new std::vector<double>;
	MotherParticleMomentum =  new std::vector<TVector3>;
	MotherParticlePDGcode=  new std::vector<int>;
	MotherParticleEnergy =  new std::vector<double>;
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

	Secondaries = new TTree("Secondaries", "Particle in the Phantom");
	Secondaries->Branch("ParticleID"     , &ParticlePDGcode  );
	Secondaries->Branch("ParticleTime"     , &ParticleTime);
	Secondaries->Branch("ParticleEnergy"     , &ParticleEnergy  );
	Secondaries->Branch("ParticleCreatorProcess"     , &ParticleCreatorProcess);
	Secondaries->Branch("ParticlePosition"     , &ParticlePosition  ); 
	Secondaries->Branch("ParticleMomentum"     , &ParticleMomentum ); 
	Secondaries->Branch("MotherParticleID"     , &MotherParticlePDGcode  );
	Secondaries->Branch("MotherParticleEnergy"     , &MotherParticleEnergy  );
	Secondaries->Branch("MotherParticleMomentum"     , &MotherParticleMomentum ); 
	
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

		rootFile->Write(0,TObject::kOverwrite);       // Writing the histograms to the file

		rootFile->Close();        // and closing the tree (and the file)

		G4cout << "\n---->  Rootfile is saved \n" << G4endl;
	}
#endif
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void HistoManager::SaveSecondaries(std::vector<G4int>* PDGcode, std::vector<G4double>* Ekin, std::vector<G4double>* Time, std::vector<TVector3>* pos, std::vector<TVector3>*  dir,std::vector<std::string>* process,std::vector<G4int>* motherPDGcode, std::vector<G4double>* motherEkin, std::vector<TVector3>*  motherdir){
#ifdef G4ANALYSIS_USE
	ParticlePDGcode = PDGcode;
	ParticleEnergy = Ekin;
	ParticleTime= Time;
	ParticlePosition= pos;
    	ParticleMomentum= dir;
    	ParticleCreatorProcess= process;
	MotherParticlePDGcode = motherPDGcode;
	MotherParticleEnergy = motherEkin;
    	MotherParticleMomentum= motherdir;
	Secondaries->Fill();
#endif 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::IncreasePrimaries(){
    Primaries++;
}

