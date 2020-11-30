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
#include "G4PrimaryParticle.hh"



SPSEventAction::SPSEventAction(HistoManager* histo):dataManager(histo),
						    EventNo(0) 
{	
	Energy= new std::vector<double>;
	Time= new std::vector<double>;
	particleID=new std::vector<int>;
	Position = new std::vector<TVector3>;
	Momentum = new std::vector<TVector3>;
       

}

SPSEventAction::~SPSEventAction() {
}

void SPSEventAction::BeginOfEventAction(const G4Event* evt) {
	EventNo=GetEventno();
	//G4cout << "Event no: " << EventNo << G4endl;
	Energy->clear();
	Time->clear();
	particleID->clear();
	Position->clear();
	Momentum->clear();
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
				        particleID->push_back((*SHC)[i]->GetParticleID());
					Time->push_back((*SHC)[i]->GetTime());
					Position->push_back((*SHC)[i]->GetPosition());
					Momentum->push_back((*SHC)[i]->GetMomentum());
					Energy->push_back((*SHC)[i]->GetEkin());
				   
					}
			        
				
				if(phantom_hit!=0)dataManager->SaveSecondaries(particleID,Energy,Time,Position,Momentum);
		       
			}	
			
		}


	}
 }


//***********************Helper Functions ***************

G4int SPSEventAction::GetEventno() {
	G4int evno = fpEventManager->GetConstCurrentEvent()->GetEventID();
	return evno ;
}



