// Run with program with 
//
//		root DrawHistograms.cpp



#include "TFile.h"
#include "TH1F.h"
#include "TVector3.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TColor.h"



void DrawHistograms() {


// PRESETTINGS
     // Set path to root file
  std::string myPath1 = "/net/scratch_g4rt1/beus/TracerSimulation/EnergySpectrum/PMMA/PMMA_Version6_samePList/";
  std::string myPath2 = "/net/scratch_g4rt1/beus/TracerSimulation/EnergySpectrum/Mg/TracerMg3/";

     // Set name of root files
     std::string myFileNames = "result*.root";

     // Create TChain with name of TTrees
     TChain *myChain1 = new TChain("Secondaries");
     TChain *myChain2 = new TChain("Secondaries");

     // Add files with trees to chain
     myChain1->Add(std::string(myPath1+myFileNames).c_str());
     myChain2->Add(std::string(myPath2+myFileNames).c_str());

     // Create a TTreeReader for the TChain
     TTreeReader myReader1(myChain1);
     TTreeReader myReader2(myChain2);

	// Set variables to access the data in the tree
	// The branch "ParticleEnergy" contains a vector<double>; access them as vEnergies
	TTreeReaderValue<vector<double>> vEnergies1(myReader1, "ParticleEnergy");
	TTreeReaderValue<vector<double>> vEnergies2(myReader2, "ParticleEnergy");
	// The branch "ParticlePosition" contains a vector<TVector3>; access them as vPositions
	TTreeReaderValue<vector<TVector3>> vPositions(myReader1, "ParticlePosition");


	// Create histograms for the values we read

	// A histogram for the energies ranging from 0 MeV to 10 MeV with 600 bins
	TH1F* hEnergy1 = new TH1F("hEnergy1", ";E in MeV;#entries", 600, 0, 8);
	TH1F* hEnergy2 = new TH1F("hEnergy2", ";E in MeV;#entries", 600, 0, 8);

	// A 2D histogram to display the particle positions in the xy-plane
	// In both dimensions, the histogram ranges from -300 mm to 300 mm and has 600 bins
	TH2F* hPositionsXY = new TH2F("hPositionsXY", ";x-position in mm;y-position in mm", 600, -300, 300, 600, -300, 300);


// READ THE TREE

	// Loop over all entries of the TTree.
	while (myReader1.Next()) {
		// Access the data as if they were iterators (note the '*' in front of them):

		// Iterate over all entries in the energy vector (this means over all particles within this event)
		// and fill them into a histogram
		for (auto dEnergy1 : *vEnergies1) {
			hEnergy1->Fill(dEnergy1);
		}

		// Iterate over all entries in the positions vector
		// and fill the x- and y-component into a histogram
		for (auto dPosition : *vPositions) {
			hPositionsXY->Fill(dPosition.X(),dPosition.Y());
		}

	}

	while (myReader2.Next()) {
        
	for (auto dEnergy2 : *vEnergies2) {
	  hEnergy2->Fill(dEnergy2);
	 }   
	}


// DRAW THE HISTOGRAMS


	// Energy histogram

	// Create a canvas to draw into
	TCanvas* cEnergy = new TCanvas();
	//Set color
	TColor *blue = new TColor(9003,0,0,1);
	TColor *color = new TColor(9001,1,0.5,0);
	// Change into this canvas (now it is the active one and everything drawn will end up in this canvas)
	cEnergy->cd();
	// Draw the histogram
	hEnergy1->SetLineColor(9003);
	hEnergy2->SetLineColor(9001);
	hEnergy2->Draw("HISTO");
	hEnergy1->Draw("HISTSAME");
	
	TLegend* legend = new TLegend(0.7,0.7,0.9,0.9);
	legend->AddEntry(hEnergy1,"PMMA","l");
	legend->AddEntry(hEnergy2,"PMMA+2%Mg","l");

	legend->Draw("sames");

	// Force an update of the graphical view
	cEnergy->Update();
	// Save the image as pdf
	cEnergy->SaveAs("../images/EnergySpectrumMg3.pdf");


	// Position histogram

	//TCanvas* cPositionsXY = new TCanvas();
	//cPositionsXY->cd();
	//hPositionsXY->Draw();
	//cPositionsXY->Update();
	//cPositionsXY->SaveAs("../images/PositionsXY.pdf");

	//Plot concentration vs. DeltaN

	/*	Double_t I1 = hEnergy1->Integral(hEnergy1->FindBin(), hEnergy1->FindBin());
	Double_t I2 = hEnergy2->Integral(hEnergy2->FindBin(), hEnergy2->FindBin());

	Double_ deltaI = I2-I1;

	Double_t errorI1 = std::sqrt(I1);
	Double_t errorI2 = std::sqrt(I2);

	Double_t errorI1pow = std::pow(errorI1, 2.);
	Double_t errorI2pow = std::pow(errorI2, 2.);

	Double_t errorsum = errorI1pow + errorI2pow;
	
	Double_t errordelta = std::sqrt(errorsum);

	TH1F* h1 = new TH1F("h1", ";concentration in mol/l;deltaN", 100, 0, 5);
	h1->Fill(x1,y1);
	h1->Fill(x2,y2);
	h1->Fill(x3,y3);
	h1->Fill(x4,y4);
	h1->Draw();*/

	



}
