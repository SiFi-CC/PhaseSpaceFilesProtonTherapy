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

PhantomHit* PhantomSD::createHit(G4Step* step) {
	const G4DynamicParticle* particle = step->GetTrack()->GetDynamicParticle();
	G4StepPoint* stepPoint = step->GetPreStepPoint();

	TVector3 temp=TVector3(0,0,0);
	// Create new hit.
	PhantomHit* hit = new PhantomHit();
	hit->SetEkin(step->GetTrack()->GetKineticEnergy());
	temp.SetXYZ(stepPoint->GetPosition().x(),stepPoint->GetPosition().y(),stepPoint->GetPosition().z());
	hit->SetPosition(temp);
	temp.SetXYZ(step->GetTrack()->GetMomentumDirection().x(),step->GetTrack()->GetMomentumDirection().y(),step->GetTrack()->GetMomentumDirection().z());
	hit->SetMomentum(temp);
	hit->SetParticleID(particle->GetPDGcode());

	return hit;
}


G4bool PhantomSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
        // Create hit.
        if(step->GetTrack()->GetTrackID()!=0){
		if(step->GetTrack()->GetDynamicParticle()->GetPDGcode() == 2112 | step->GetTrack()->GetDynamicParticle()->GetPDGcode() == 22){
			PhantomHit* hit = createHit(step);
        		PhantomCollection->insert(hit);
		}	
		step->GetTrack()->SetTrackStatus(fStopAndKill);
	}        
        return true;

}

void PhantomSD::EndOfEvent(G4HCofThisEvent* hCof) {
        int collectionId = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
        hCof->AddHitsCollection(collectionId, PhantomCollection);
        //        << "\" for sensitive detector \"" << SensitiveDetectorName << "\" with " << ScinCollection->GetSize() << std::endl;

}
      

