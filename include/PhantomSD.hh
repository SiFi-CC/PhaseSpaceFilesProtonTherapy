//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef PhantomSD_h
#define PhantomSD_h 1

#include <G4VSensitiveDetector.hh>
#include <globals.hh>

class EPConstruction;
class G4HCofThisEvent;
class G4Step;
#include "PhantomHit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class PhantomSD : public G4VSensitiveDetector {
public:


        PhantomSD(const G4String& name, const G4String& hitsCollectionName);
        ~PhantomSD();

    	EPConstruction* Detector;
        
	virtual void Initialize(G4HCofThisEvent* hCof);
        virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* roHist);
        virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);
        PhantomHit* createHit(G4Track* track);

        private:
    	PhantomHitsCollection*  PhantomCollection;
	
	std::vector<G4Track*>* secondaries;

};

#endif


