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

PhantomHit* PhantomSD::createHit(G4Track* track,G4Step* step) {
	const G4DynamicParticle* particle = track->GetDynamicParticle();
//	std::cout<< "HIt of " << particle->GetParticleDefinition()->GetParticleName() << " Code  " <<particle->GetPDGcode() << " Mass" << particle->GetMass() << std::endl;
	TVector3 temp=TVector3(0,0,0);
	// Create new hit.
	PhantomHit* hit = new PhantomHit();
	hit->SetEkin(track->GetKineticEnergy());
	hit->SetTime(track->GetGlobalTime());
	temp.SetXYZ(track->GetPosition().x(),track->GetPosition().y(),track->GetPosition().z());
/*	if(std::abs(temp.x())>150. || std::abs(temp.x())>150. ){
		std::cout << "particle " << particle->GetPDGcode() << std::endl;
		std::cout << "ekin " << track->GetKineticEnergy() << std::endl;
		std::cout << "Process " << track->GetCreatorProcess()->GetProcessName() << std::endl;
		std::cout << "track pos " << track->GetPosition() << std::endl;
		std::cout << "track Volume " << track->GetVolume()->GetName() << std::endl;
		std::cout << "step post " << step->GetPostStepPoint()->GetPosition() << std::endl;
		std::cout << "step pre " << step->GetPreStepPoint()->GetPosition() << std::endl;
		std::cout << "Volume" << step->GetTrack()->GetVolume()->GetName() << std::endl;

	}
*/	hit->SetPosition(temp);
	temp.SetXYZ(track->GetMomentumDirection().x(),track->GetMomentumDirection().y(),track->GetMomentumDirection().z());
	hit->SetMomentum(temp);
	hit->SetParticleID(particle->GetPDGcode());

	return hit;
}


G4bool PhantomSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
     	secondaries= step->GetfSecondary();
	currentsecs= step->GetSecondaryInCurrentStep ();
        int currentsize=currentsecs->size();
	TString mothername="";
        G4double mothermass=0;
        if (currentsize!=0 ){
		photoev = (G4PhotonEvaporation*)(((G4ExcitationHandler*)(((G4VPreCompoundModel*)(G4HadronicInteractionRegistry::Instance()->FindModel("PRECO")))->GetExcitationHandler()))->GetPhotonEvaporation());
		translevel= photoev->GetTransLevel();
		for(size_t i=(secondaries->size()-currentsize);i< secondaries->size();i++){
			if((*secondaries)[i]->GetDynamicParticle()->GetMass()>mothermass){
				mothermass=(*secondaries)[i]->GetDynamicParticle()->GetMass();
				mothername=(*secondaries)[i]->GetDynamicParticle()->GetDefinition()->GetParticleName();
			}
		}
		if(mothername.CompareTo("O16")==0 | mothername.CompareTo("C12")==0){
			for(size_t i=(secondaries->size()-currentsize);i< secondaries->size();i++){
				if((*secondaries)[i]->GetDynamicParticle()->GetDefinition()->GetParticleName()=="gamma"){
//					std::cout << "gamma with " << (*secondaries)[i]->GetDynamicParticle()->GetKineticEnergy() << std::endl;
					std::pair<G4int,G4int> level=std::pair<G4int,G4int>(0,0);
					double smallestdiff=1e9;
					for(auto itmap : translevel){
						if(TMath::Abs(itmap.first-(*secondaries)[i]->GetDynamicParticle()->GetKineticEnergy())<smallestdiff){
							level =itmap.second;
        	                                        smallestdiff=TMath::Abs(itmap.first-(*secondaries)[i]->GetDynamicParticle()->GetKineticEnergy());
                	                        }
					}
//					if((*secondaries)[i]->GetDynamicParticle()->GetKineticEnergy() >3.15 && (*secondaries)[i]->GetDynamicParticle()->GetKineticEnergy() < 3.25)std::cout << mothername.Data() << "from " << level.first << " to " << level.second << std::endl;
					if(mothername.CompareTo("O16")==0 && level.first == 1 && level.second == 0) (*secondaries)[i]->SetKineticEnergy((*secondaries)[i]->GetDynamicParticle()->GetKineticEnergy()+0.085);
					else if(mothername.CompareTo("C12")==0 && level.first == 2 && level.second == 1) (*secondaries)[i]->SetTrackStatus(fStopAndKill);
				}
			}
		}
		for(size_t i=(secondaries->size()-currentsize);i< secondaries->size();i++){
			if((*secondaries)[i]->GetDynamicParticle()->GetPDGcode()!=2212){
				if((*secondaries)[i]->GetDynamicParticle()->GetMass() < 4000){
					PhantomHit* hit = createHit((*secondaries)[i],step);
					PhantomCollection->insert(hit);
				}
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
      

