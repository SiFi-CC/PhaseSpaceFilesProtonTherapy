#include "PhantomSD.hh"
#include "PhantomHit.hh"

//G4 includes
#include <G4VProcess.hh>
#include <G4ParticleDefinition.hh>
#include <G4TouchableHandle.hh>
#include <G4VPhysicalVolume.hh>
#include <G4Step.hh>
#include <G4VTouchable.hh>
#include <G4TouchableHistory.hh>
#include <G4SDManager.hh>
#include <G4ios.hh>
#include "G4Proton.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhantomSD::PhantomSD(const G4String & name, const G4String & PhantomCollectionName):
                G4VSensitiveDetector(name), PhantomCollection(NULL){
        // Register name to the inherited collection.
        collectionName.push_back(PhantomCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhantomSD::~PhantomSD(){
}


void PhantomSD::Initialize(G4HCofThisEvent*) {
        // Initialize hits collection.
        PhantomCollection = new PhantomHitsCollection(SensitiveDetectorName, collectionName[0]);
}

PhantomHit* PhantomSD::createHit(G4Track* track) {
	const G4DynamicParticle* particle = track->GetDynamicParticle();
//	std::cout<< "HIt of " << particle->GetParticleDefinition()->GetParticleName() << " Code  " <<particle->GetPDGcode() << " Mass" << particle->GetMass() << std::endl;
	TVector3 temp=TVector3(0,0,0);
	// Create new hit.
	PhantomHit* hit = new PhantomHit();
	hit->SetEkin(track->GetKineticEnergy());
	hit->SetTime(track->GetGlobalTime());
	temp.SetXYZ(track->GetPosition().x(),track->GetPosition().y(),track->GetPosition().z());
	hit->SetPosition(temp);
	temp.SetXYZ(track->GetMomentumDirection().x(),track->GetMomentumDirection().y(),track->GetMomentumDirection().z());
	hit->SetMomentum(temp);
	hit->SetParticleID(particle->GetPDGcode());


	return hit;
}


G4bool PhantomSD::ProcessHits(G4Step* step, G4TouchableHistory*){
     	secondaries= step->GetfSecondary();
        TString mothername="";
        G4double mothermass=0;
        if (secondaries->size()!=0 ){
		photoev = (G4PhotonEvaporation*)(((G4ExcitationHandler*)(((G4VPreCompoundModel*)(G4HadronicInteractionRegistry::Instance()->FindModel("PRECO")))->GetExcitationHandler()))->GetPhotonEvaporation());
		translevel= photoev->GetTransLevel();
		for(size_t i=0;i< secondaries->size();i++){
			if((*secondaries)[i]->GetDynamicParticle()->GetMass()>mothermass){
				mothermass=(*secondaries)[i]->GetDynamicParticle()->GetMass();
				mothername=(*secondaries)[i]->GetDynamicParticle()->GetDefinition()->GetParticleName();
			}
		}
		if(mothername.CompareTo("O16")==0 | mothername.CompareTo("C12")==0){
			for(size_t i=0;i< secondaries->size();i++){
				if((*secondaries)[i]->GetDynamicParticle()->GetDefinition()->GetParticleName()=="gamma"){
					std::pair<G4int,G4int> level=std::pair<G4int,G4int>(0,0);
					for(auto itmap : translevel){
						if(itmap.first> ((*secondaries)[i]->GetDynamicParticle()->GetKineticEnergy() -0.005) && itmap.first< ((*secondaries)[i]->GetDynamicParticle()->GetKineticEnergy() +0.005)){
							if(mothername.CompareTo("O16")==0 && itmap.second.first == 1 && itmap.second.second == 0) (*secondaries)[i]->SetKineticEnergy((*secondaries)[i]->GetDynamicParticle()->GetKineticEnergy()+0.085);
							else if(mothername.CompareTo("C12")==0 && itmap.second.first == 2 && itmap.second.second == 1) (*secondaries)[i]->SetTrackStatus(fStopAndKill);
						}
					
					}
				}
			}
		}
		for(size_t i=0;i< secondaries->size();i++){
		  if((*secondaries)[i]->GetDynamicParticle()->GetPDGcode()==22){
				PhantomHit* hit = createHit((*secondaries)[i]);
				PhantomCollection->insert(hit);
				(*secondaries)[i]->SetTrackStatus(fStopAndKill);
		  }
		}
	}
	if(step->GetTrack()->GetDefinition()==G4Proton::Proton()){
	  G4StepPoint* preStepPoint = step->GetPreStepPoint();
	  const G4VProcess* currentProcess = preStepPoint->GetProcessDefinedStep();
	  if(currentProcess!=0){ 
	      G4String stepProcessName = currentProcess->GetProcessName();
	      for(size_t i =0;i<secondaries->size();i++){
		if((*secondaries)[i]->GetDynamicParticle()->GetPDGcode()==22){
		  G4cout<<"Process: "<<stepProcessName<< "     Energy: " <<(*secondaries)[i]->GetDynamicParticle()->GetKineticEnergy()<<G4endl;}}
	  }
	}
       
	
		//Create hit
		return true;
}

void PhantomSD::EndOfEvent(G4HCofThisEvent* hCof) {
        int collectionId = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
        hCof->AddHitsCollection(collectionId, PhantomCollection);
        //        << "\" for sensitive detector \"" << SensitiveDetectorName << "\" with " << ScinCollection->GetSize() << std::endl;

}
      

