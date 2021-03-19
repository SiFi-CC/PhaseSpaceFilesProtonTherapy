// Run with program with 
//
//		root DrawHistograms.cpp



#include "TFile.h"
#include "TH1F.h"
#include "TVector3.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"



void DrawHistoXS() {

        std::string myPath = "../Data/";

        std::string myFileName = "Out_run_0.root";

        TFile *myFile = TFile::Open(std::string(myPath+myFileName).c_str());

        TTreeReader myReader("Secondaries", myFile);

	TTreeReaderValue<vector<double>> vEnergies(myReader, "ParticleEnergy");

	TH1F* hEnergy = new TH1F("Energy Spectrum", ";E in MeV;#entries",700, 0, 10);
	
	

	while (myReader.Next()) {
		for (auto dEnergy : *vEnergies) {
			hEnergy->Fill(dEnergy);
		       
	       
		}

		
	}


	TCanvas* cEnergy = new TCanvas();

	cEnergy->cd();
	//hEnergy->GetYaxis()->SetRangeUser(0, 100000);
	//EnergySpectrum->SetTitle("Energy spectrum - Magnesium");
	hEnergy->Draw();
	//hEnergyerror->GetYaxis()->SetRangeUser(70, 100000);
	cEnergy->Update();
	hEnergy->SaveAs("../Data/EnergyPMMA.root");
	



}
