// Run with program with 
//
//		root DrawHistograms.cpp



#include "TFile.h"
#include "TH1F.h"
#include "TVector3.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"



void DrawSingleHistogram() {

 std::string myPath = "/net/scratch_g4rt1/beus/TracerSimulation/EnergySpectrum/Mg/Mg12mev/";


     // Set name of root files
     std::string myFileNames = "result*.root";

     // Create TChain with name of TTrees
     TChain *myChain = new TChain("Secondaries");

     // Add files with trees to chain
     myChain->Add(std::string(myPath+myFileNames).c_str());


   
     TTreeReader myReader(myChain);

	TTreeReaderValue<vector<double>> vEnergies(myReader, "ParticleEnergy");

	TH1F* hEnergy = new TH1F("hEnergy", ";E in MeV;#entries",500, 0, 10);
	
	

	while (myReader.Next()) {
		for (auto dEnergy : *vEnergies) {
			hEnergy->Fill(dEnergy);
		       
	       
		}

		
	}


	TCanvas* cEnergy = new TCanvas();

	cEnergy->cd();
	//hEnergy->GetYaxis()->SetRangeUser(0, 100000);
	hEnergy->Draw();
	//hEnergyerror->GetYaxis()->SetRangeUser(70, 100000);
	cEnergy->Update();
	cEnergy->SaveAs("../Data/histoEnergy12.root");
	



}
