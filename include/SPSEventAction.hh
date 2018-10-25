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
		
    		G4int EventNo;    
    		G4double Energy;
    		TVector3* Position;
    		TVector3* Momentum;
		G4int particleID;
};

#endif


