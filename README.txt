README for PhaseSpaceProtonTherpay 
by Jonas Kasper

Simulation to generate PhaseSpaceFiles from a proton beam used in proton therapy. Simulated are the interations of the proton beam in a target and all secondaries that are generated that are not protons are directly stopped (not further processes) and the information are saved to a file.

The master version only works for Geant4 10.4.2 with additional changes to the code, as explained in Geant4Changes.
The WithoutLineCorrection version does work for all Geant4 versions.

To see how to access the information from the GammaTransition have a look into:
	PhantomSD.hh/PhantomSD.cc
You can compare the difference between the master and the WithoutLineCorrection via
	git diff master WithoutLineCorrection -- pathtofiletobecompared
