// Run with program with 
//
//		root DrawHistograms.cpp



#include "TFile.h"
#include "TH1F.h"
#include "TVector3.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"



void DrawHistograms() {


// PRESETTINGS

	// Set path to root file
	std::string myPath = "../data/";

	// Set name of root file
	std::string myFileName = "Out_run_0.root";

	// Open the file containing the tree
	TFile *myFile = TFile::Open(std::string(myPath+myFileName).c_str());

	// Create a TTreeReader for the tree by passing the TTree's name and the TFile it is in
	TTreeReader myReader("Secondaries", myFile);

	// Set variables to access the data in the tree

	// The branch "ParticleEnergy" contains a vector<double>; access them as vEnergies
	TTreeReaderValue<vector<double>> vEnergies(myReader, "ParticleEnergy");
	// The branch "ParticlePosition" contains a vector<TVector3>; access them as vPositions
	TTreeReaderValue<vector<TVector3>> vPositions(myReader, "ParticlePosition");

	// Create histograms for the values we read

	// A histogram for the energies ranging from 0 MeV to 20 MeV with 200 bins
	TH1F* hEnergy = new TH1F("hEnergy", ";E in MeV;#entries", 20, 0, 20);

	// A 2D histogram to display the particle positions in the xy-plane
	// In both dimensions, the histogram ranges from -300 mm to 300 mm and has 600 bins
	TH2F* hPositionsXY = new TH2F("hPositionsXY", ";x-position in mm;y-position in mm", 600, -300, 300, 600, -300, 300);


// READ THE TREE

	// Loop over all entries of the TTree.
	while (myReader.Next()) {
		// Access the data as if they were iterators (note the '*' in front of them):

		// Iterate over all entries in the energy vector (this means over all particles within this event)
		// and fill them into a histogram
		for (auto dEnergy : *vEnergies) {
			hEnergy->Fill(dEnergy);
		}

		// Iterate over all entries in the positions vector
		// and fill the x- and y-component into a histogram
		for (auto dPosition : *vPositions) {
			hPositionsXY->Fill(dPosition.X(),dPosition.Y());
		}

	}


// DRAW THE HISTOGRAMS


	// Energy histogram

	// Create a canvas to draw into
	TCanvas* cEnergy = new TCanvas();
	// Change into this canvas (now it is the active one and everything drawn will end up in this canvas)
	cEnergy->cd();
	// Draw the histogram
	hEnergy->Draw();
	// Force an update of the graphical view
	cEnergy->Update();
	// Save the image as pdf
	cEnergy->SaveAs("../images/EnergySpectrum.pdf");


	// Position histogram

	TCanvas* cPositionsXY = new TCanvas();
	cPositionsXY->cd();
	hPositionsXY->Draw();
	cPositionsXY->Update();
	cPositionsXY->SaveAs("../images/PositionsXY.pdf");
}
