#include <G4Event.hh>
#include <G4EventManager.hh>
#include <G4VVisManager.hh>
#include "G4SDManager.hh"
#include <G4UImanager.hh>
#include <G4ios.hh>
#include <vector>
#include <map>
#include <TVector3.h>
#include <TString.h>
#include <G4SystemOfUnits.hh>

#include "SPSEventAction.hh"
#include "PhantomHit.hh"
#include "HistoManager.hh"

SPSEventAction::SPSEventAction(HistoManager* histo):dataManager(histo),
EventNo(0)   
{	
	Energy= new std::vector<double>;
	Time= new std::vector<double>;
	particleID=new std::vector<int>;
	Position = new std::vector<TVector3>;
	Momentum = new std::vector<TVector3>;
	CreatorProcess= new std::vector<std::string>;
	MotherEnergy= new std::vector<double>;
	MotherparticleID=new std::vector<int>;
	MotherMomentum = new std::vector<TVector3>;
}

SPSEventAction::~SPSEventAction() {
}

void SPSEventAction::BeginOfEventAction(const G4Event* evt) {
	EventNo=GetEventno();
//	std::cout << "Event no " << EventNo << std::endl;
	Energy->clear();
	Time->clear();
	particleID->clear();
	Position->clear();
	Momentum->clear();
	CreatorProcess->clear();
	MotherEnergy->clear();
	MotherparticleID->clear();
	MotherMomentum->clear();
	dataManager->IncreasePrimaries();
}

void SPSEventAction::EndOfEventAction(const G4Event* evt) {


	G4SDManager * SDman = G4SDManager::GetSDMpointer();

	PhantomHitsCollection* SHC = 0;

	G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
	
	if (HCE){
	int nPhantom=HCE->GetNumberOfCollections();	
		//G4cout << "Number of Collections" << nScins << G4endl;
		for(int j=0;j<nPhantom;j++){

			//G4cout << "Current Collection ID" << ScinID << G4endl;
			SHC = (PhantomHitsCollection*)(HCE->GetHC(j));

			if (SHC){

				int phantom_hit = SHC->entries();
				for (G4int i=0;i<phantom_hit;i++){
					Energy->push_back((*SHC)[i]->GetEkin());
					Time->push_back((*SHC)[i]->GetTime());
					particleID->push_back((*SHC)[i]->GetParticleID());
					Position->push_back((*SHC)[i]->GetPosition());
					Momentum->push_back((*SHC)[i]->GetMomentum());
					CreatorProcess->push_back((*SHC)[i]->GetCreatorProcess());
					MotherEnergy->push_back((*SHC)[i]->GetMotherEkin());
					MotherparticleID->push_back((*SHC)[i]->GetMotherParticleID());
					MotherMomentum->push_back((*SHC)[i]->GetMotherMomentum());
				}
				if(phantom_hit!=0)dataManager->SaveSecondaries(particleID,Energy,Time,Position,Momentum,CreatorProcess,MotherparticleID,MotherEnergy,MotherMomentum);
			}


		}
	}
}

//***********************Helper Functions ***************

G4int SPSEventAction::GetEventno() {
	G4int evno = fpEventManager->GetConstCurrentEvent()->GetEventID();
	return evno ;
}



