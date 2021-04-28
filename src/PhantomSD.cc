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
	//All secondaries from this track     
	secondaries= step->GetfSecondary();
	//Secondaries in this step
	currentsecs= step->GetSecondaryInCurrentStep ();
        int currentsize=currentsecs->size();
	std::map<int,int> notused;
	TString mothername="";
        G4double mothermass=0;
        if (currentsize!=0 ){
		//Getting the photonevaporation manager
		photoev = (G4PhotonEvaporation*)(((G4ExcitationHandler*)(((G4VPreCompoundModel*)(G4HadronicInteractionRegistry::Instance()->FindModel("PRECO")))->GetExcitationHandler()))->GetPhotonEvaporation());
		//Getting the transition levels of this step (modified geant4 code)
		translevel= photoev->GetTransLevel();
		//Start parametes of this loop so that only the secondaries of the current step are analyzed
		for(size_t i=(secondaries->size()-currentsize);i< secondaries->size();i++){
			//Finding the heaviest secondary ( the nucleus)	
			if((*secondaries)[i]->GetDynamicParticle()->GetMass()>mothermass){
				mothermass=(*secondaries)[i]->GetDynamicParticle()->GetMass();
				mothername=(*secondaries)[i]->GetDynamicParticle()->GetDefinition()->GetParticleName();
			}
		}
		//Only for these Nucleii the prompt gamma need to be modified
		if(mothername.CompareTo("O16")==0 | mothername.CompareTo("C12")==0){
			for(size_t i=(secondaries->size()-currentsize);i< secondaries->size();i++){
				//Getting the secondaries that are actual gammas
				if((*secondaries)[i]->GetDynamicParticle()->GetDefinition()->GetParticleName()=="gamma"){
					std::pair<G4int,G4int> level=std::pair<G4int,G4int>(0,0);
					double smallestdiff=1e9;
					//Looping trhough all generated transitions ( there can be several)
					for(auto itmap : translevel){
						//Here a direct comparison of the energy from the seconady and the GetTransLevel is not possible since the lorentzboost is done after the generation of the translevels, Hence the gamma which has the smallest difference to the energy from the TransLevels is assigned to the corresponding translevels.
						if(TMath::Abs(itmap.first-(*secondaries)[i]->GetDynamicParticle()->GetKineticEnergy())<smallestdiff){
							level =itmap.second;
        	                                        smallestdiff=TMath::Abs(itmap.first-(*secondaries)[i]->GetDynamicParticle()->GetKineticEnergy());
                	                        }
					}
					//Moving the unphysical O16 Peak from 1 to 0 transition to the 6.13 peak
					if(mothername.CompareTo("O16")==0 && level.first == 1 && level.second == 0) (*secondaries)[i]->SetKineticEnergy((*secondaries)[i]->GetDynamicParticle()->GetKineticEnergy()+0.085);
					// removing the 3.15 peak completely
					else if(mothername.CompareTo("C12")==0 && level.first == 2 && level.second == 1){
						(*secondaries)[i]->SetTrackStatus(fStopAndKill);
						notused[i]=1;
					}
				}
			}
		}
		for(size_t i=(secondaries->size()-currentsize);i< secondaries->size();i++){
			if((*secondaries)[i]->GetDynamicParticle()->GetPDGcode()!=2212){
				if((*secondaries)[i]->GetDynamicParticle()->GetMass() < 4000){
					if(notused.find(i)==notused.end()){
						PhantomHit* hit = createHit((*secondaries)[i],step);
						PhantomCollection->insert(hit);
					}
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
      

