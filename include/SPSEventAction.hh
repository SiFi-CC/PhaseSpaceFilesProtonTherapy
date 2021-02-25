#ifndef SPSEventAction_h
#define SPSEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <TVector3.h>
#include <iostream>

#include "HistoManager.hh"

class SPSEventAction : public G4UserEventAction{
	public:
		SPSEventAction(HistoManager* histo );
		virtual ~SPSEventAction();

	public:
		virtual void   BeginOfEventAction(const G4Event*);
		virtual void   EndOfEventAction(const G4Event*);
		
		G4int GetEventno();
		
	private:
		HistoManager* dataManager;
		
    		int EventNo;    
    		std::vector<double>* Energy;
    		std::vector<double>* Time;
    		std::vector<TVector3>* Position;
    		std::vector<TVector3>* Momentum;
    		std::vector<std::string>* CreatorProcess;
		std::vector<int>* particleID;
    		std::vector<double>* MotherEnergy;
    		std::vector<TVector3>* MotherMomentum;
		std::vector<int>* MotherparticleID;
};

#endif


