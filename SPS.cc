#include "G4RunManager.hh"
#include "G4ScoringManager.hh" // scoring manager
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"





//mandatory classes
#include "SPSSetupConstruction.hh"
//#include "PhysicsList.hh"
//~ #include "QGSP_BIC_EMY.hh"
#include "G4PhysListFactory.hh"
#include "PrimaryGeneratorAction.hh"

//optional UserActionclasses
#include "HistoManager.hh"
#include "RunAction.hh"
#include "SPSEventAction.hh"

// STL / C++
#include <string>
#include <sstream>

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif


//std::ofstream output;

namespace CLHEP {}
using namespace CLHEP; 


template <class T>
bool from_string(T& t,const std::string& s,std::ios_base& (*f)(std::ios_base&))
{
	std::istringstream iss(s);
	return !(iss >> f >> t).fail();
}


int main(int argc,char** argv) {
	G4bool batch = false;
	unsigned long seed = 0;

	for(G4int i = 1; i < argc; i++ ) // parse first after "G4RTD" to last argument
	{
		if( std::string(argv[i]) == "-b" ) //! Batch mode
			batch = true;
		/*else if( std::string(argv[i]) == "-h" && i == 1 ) //! Need help!
		  {
		  printHelp();
		  exit(0);
		  }*/
		else if( std::string(argv[i]) == "-s" ) //! Seed was specified
		{
			G4cout<<"found seed "<< i << argv[i] << argv[i+1]<<" " << argc<<G4endl;
			if( i != (argc-1) ) // not the last argument

			{
				std::string seedstring(argv[i+1]);
				// Use only first 8 digits
				seedstring.erase(8);
				if(!from_string<unsigned long>(seed, seedstring, std::hex))
				{
					G4cerr << "Invalid random seed specified, exiting!" << G4endl;
					//printHelp();
					exit(0);
				}
				i++;
			}
			else // "-s" is last argument, so there is no following seed
			{
				G4cerr << "No random seed specified, exiting!" << G4endl;
				//printHelp();
				exit(0);
			}
		}
	}
	// Select random number engine
	if( seed != 0 )
	{
		// Initialise ranlux engine with calculated seed and luxury level 3 (of 4)
		G4cout << "Initialising ranlux engine with seed " << seed << G4endl;
		CLHEP::HepRandom::setTheEngine(new CLHEP::RanluxEngine( abs( long( seed ) ), 3));
	}
	else
	{
		G4cout << "Initialising DEFAULT ranlux engine (no seed specified)" << G4endl;
		CLHEP::HepRandom::setTheEngine(new CLHEP::RanluxEngine);
	}

	// choose the Random engine
	//HepRandom::setTheEngine(new RanecuEngine);

	// Construct the default run manager
	G4RunManager * runManager = new G4RunManager;

	// Activate command-based scorer
	G4ScoringManager::GetScoringManager();



	// set mandatory initialization classes
	DetectorConstruction* detector = new DetectorConstruction();  
	runManager->SetUserInitialization(detector);
	
	// set a histo manager
	HistoManager*  histo = new HistoManager(detector);

	//G4VUserPhysicsList* the_physics = new PhysicsList;
	//runManager->SetUserInitialization(the_physics);  
	G4PhysListFactory factory;
	G4VModularPhysicsList* physlist = factory.GetReferencePhysList("QGSP_BIC_HP_EMZ");
	runManager->SetUserInitialization(physlist);

	PrimaryGeneratorAction *primarygeneration = new PrimaryGeneratorAction();
	runManager->SetUserAction(primarygeneration);


	// set optional UserAction classes
	//RunAction* runaction = new RunAction(histo,jobnumber );
	RunAction* runaction = new RunAction(histo);
	runManager->SetUserAction(runaction);

	SPSEventAction* eventaction = new SPSEventAction(histo);
	runManager->SetUserAction(eventaction);

	//Initialize G4 kernel
	runManager->Initialize();

#ifdef G4VIS_USE
	// visualization manager
	G4VisManager* visManager = new G4VisExecutive;
	// G4VisManager* visManager = new DetectorPhysVisManager; //old
	visManager->Initialize();
#endif

	G4cout << "initialized visManager"<<G4endl;
	// get the pointer to the User Interface manager 
	G4UImanager* UI = G4UImanager::GetUIpointer();  //old


	if (!batch)   // Define UI session for interactive mode.
	{
		G4cout << "open interactive mode UI"<<G4endl;
		G4UIExecutive* UI2 = new G4UIExecutive(argc, argv);
		UI->ApplyCommand("/control/execute macros/vis_SPS.mac");    
		UI2->SessionStart();
		delete UI2;
	}
	else           // Batch mode
	{
		G4cout << "open batch mode UI"<<G4endl;
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UI->ApplyCommand(command+fileName);
	}

#ifdef G4VIS_USE
	delete visManager;
#endif
	delete histo;
	delete runManager;

	return 0;
}

