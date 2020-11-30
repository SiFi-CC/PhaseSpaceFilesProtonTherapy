#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "SPSSetupConstruction.hh"
#include <vector>


#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "HistoManager.hh"
#include "G4UImessenger.hh"




extern std::ofstream output;

//char outfilename[100];


RunAction::RunAction(HistoManager* histo):histoManager(histo){}


RunAction::~RunAction() { }

void RunAction::BeginOfRunAction(const G4Run* aRun) {

  G4cout<<"*********************************         OOOOOOOOOOOOOOOOOOOooooooooooooooooooOOOOOOOOOOOOOOOOO   Start Run."<< aRun->GetRunID()<<G4endl;    
  histoManager->book(aRun->GetRunID());

 
       

}

void RunAction::EndOfRunAction(const G4Run* aRun) {

	G4cout<<"*********************************         OOOOOOOOOOOOOOOOOOOooooooooooooooooooOOOOOOOOOOOOOOOOO   End Run."<<G4endl; 
	// output<<"NumberOfEvents"<<" "<<aRun->GetNumberOfEventToBeProcessed()<<"\n";
	// output.close();

	
      histoManager->save();

	    
	
}


