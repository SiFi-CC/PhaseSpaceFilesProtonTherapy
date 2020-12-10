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
   //The branch "ParticleID" contains a vector<int>; access them as vID
   TTreeReaderValue<vector<int>> vPDGcode (myReader, "ParticleID"); //New ADD

	// Create histograms for the values we read

	// A histogram for the energies ranging from 0 MeV to 20 MeV with 200 bins
	TH1F* hEnergyAll = new TH1F("hEnergyAll", ";E in MeV;#entries", 20, 0, 20);
	
	TH1F* hEnergyElectron = new TH1F("hEnergyElectron", ";E in MeV;#entries", 20, 0, 20);
	TH1F* hEnergyNeutron = new TH1F("hEnergyNeutron", ";E in MeV;#entries", 20, 0, 20);
	TH1F* hEnergyGamma = new TH1F("hEnergyGamma", ";E in MeV;#entries", 20, 0, 20);
				

	// A 2D histogram to display the particle positions in the xy-plane
	// In both dimensions, the histogram ranges from -300 mm to 300 mm and has 600 bins
	TH2F* hPositionsXY = new TH2F("hPositionsXY", ";x-position in mm;y-position in mm", 600, -300, 300, 600, -300, 300);
	
	// A histogram for Particle ID ranging from 1 to 100  with 200 bins
	TH1F* hPDGcode = new TH1F("hPDGcode", ";Number;#entries", 2500, 0, 2500); //New ADD
	
	
	


// READ THE TREE
  
	// Loop over all entries of the TTree.
	while (myReader.Next()) {
		int energySize = vEnergies->size();
		
		for(int i = 0; i<energySize; ++i){
			
			double myEnergy= vEnergies->at(i);
			int myPDG= vPDGcode->at(i);
			TVector3 myPos = vPositions->at(i);
			double myXPos = myPos.X();
			double myYPos = myPos.Y();
			
			hEnergyAll->Fill(myEnergy);
			if (myPDG==22){
				hEnergyGamma->Fill(myEnergy);
				hPositionsXY->Fill(myXPos,myYPos);
				hPDGcode->Fill(myPDG);
				}
			//if (myPDG==11) hEnergyElectron->Fill(myEnergy);
			//if (myPDG==2112) hEnergyNeutron->Fill(myEnergy);
			}
    } 

// DRAW THE HISTOGRAMS
	
	// Energy Histogram
	
	// Create a canvas to draw into
	TCanvas* cEnergy = new TCanvas();
	// Change into this canvas (now it is the active one and everything drawn will end up in this canvas)
	cEnergy->cd();
	// Draw the histogram
	hEnergyGamma->Draw();
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
	
	// PDGCode histogram  ///New Add
	
	TCanvas* cPDGcode = new TCanvas();
	cPDGcode->cd();
	hPDGcode->Draw();
	cPDGcode->Update();
	cPDGcode->SaveAs("../images/ParticleID.pdf");
	
}
