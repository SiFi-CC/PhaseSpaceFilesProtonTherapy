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

PhantomHit* PhantomSD::createHit(G4Track* track, G4Step* mother) {
	const G4DynamicParticle* particle = track->GetDynamicParticle();
	const G4DynamicParticle* motherparticle = mother->GetTrack()->GetDynamicParticle();
//	std::cout<< "HIt of " << particle->GetParticleDefinition()->GetParticleName() << " Code  " <<particle->GetPDGcode() << " Mass" << particle->GetMass() << std::endl;
	TVector3 temp=TVector3(0,0,0);
	// Create new hit.
	PhantomHit* hit = new PhantomHit();
	hit->SetEkin(track->GetKineticEnergy());
	hit->SetCreatorProcess(track->GetCreatorProcess()->GetProcessName());
	hit->SetTime(track->GetGlobalTime());
	temp.SetXYZ(mother->GetPostStepPoint()->GetPosition().x(),mother->GetPostStepPoint()->GetPosition().y(),mother->GetPostStepPoint()->GetPosition().z());
	hit->SetPosition(temp);
	temp.SetXYZ(track->GetMomentumDirection().x(),track->GetMomentumDirection().y(),track->GetMomentumDirection().z());
	hit->SetMomentum(temp);
	hit->SetParticleID(particle->GetPDGcode());
	hit->SetMotherParticleID(motherparticle->GetPDGcode());
	hit->SetMotherEkin(mother->GetTrack()->GetKineticEnergy());
	temp.SetXYZ(mother->GetPostStepPoint()->GetMomentumDirection().x(),mother->GetPostStepPoint()->GetMomentumDirection().y(),mother->GetPostStepPoint()->GetMomentumDirection().z());
	hit->SetMotherMomentum(temp);

//	hit->Print();
	return hit;
}


G4bool PhantomSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
     	secondaries= step->GetfSecondary();
        TString mothername="";
        G4double mothermass=0;
        if (secondaries->size()!=0 ){
		for(size_t i=0;i< secondaries->size();i++){
			if((*secondaries)[i]->GetDynamicParticle()->GetPDGcode()!=2212 & (*secondaries)[i]->GetDynamicParticle()->GetMass() < 4000){
				PhantomHit* hit = createHit((*secondaries)[i],step);
				PhantomCollection->insert(hit);
				(*secondaries)[i]->SetTrackStatus(fStopAndKill);
			}
		}        
	}
	    // Create hit.
        return true;

}

void PhantomSD::EndOfEvent(G4HCofThisEvent* hCof) {
        int collectionId = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
        hCof->AddHitsCollection(collectionId, PhantomCollection);
        //        << "\" for sensitive detector \"" << SensitiveDetectorName << "\" with " << ScinCollection->GetSize() << std::endl;

}
      

