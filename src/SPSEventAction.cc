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
Energy(0),
EventNo(0),   
particleID(-1)
{	
	Position = new TVector3(0,0,0);
	Momentum= new TVector3(0,0,0);
}

SPSEventAction::~SPSEventAction() {
}

void SPSEventAction::BeginOfEventAction(const G4Event* evt) {
	EventNo=GetEventno();
	Energy=0;
	particleID=-1;
	Position = new TVector3(0,0,0);
	Momentum = new TVector3(0,0,0);
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
				for (G4int i=0;i<phantom_hit;i++)
				{
					Energy = (*SHC)[i]->GetEkin();
					particleID = (*SHC)[i]->GetParticleID();
					Position = (*SHC)[i]->GetPosition();
					Momentum = (*SHC)[i]->GetMomentum();
					
					dataManager->SaveSecondary(particleID,Energy,Position,Momentum);
				}
			}


		}
	}
}

//***********************Helper Functions ***************

G4int SPSEventAction::GetEventno() {
	G4int evno = fpEventManager->GetConstCurrentEvent()->GetEventID();
	return evno ;
}



