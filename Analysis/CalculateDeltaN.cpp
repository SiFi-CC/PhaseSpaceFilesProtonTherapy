#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TFitResultPtr.h"
#include "TMatrixDSym.h"
#include "TGraph.h"


// OWN FUNCTIONS


void ComputePeakIntegral(double &peakIntegral, double &e_peakIntegral, TH1F* h, double peakLeft, double peakRight){

       TCanvas* cPeak = new TCanvas("cPeak","cPeak");
       h->Draw();

	// Get the full peak integral
	double fullPeak = h->Integral(h->FindBin(peakLeft),h->FindBin(peakRight));
	cout<<"fullPeak = "<<fullPeak<<endl ;

	// Calculate the peak as difference from the two
	peakIntegral = fullPeak;

	// Get the error through error propagation
	e_peakIntegral = sqrt(fullPeak);	

	// Keep the window open so that the user can check the fit until he/she presses a key
	std::cout<<"Insert any number and press enter to continue"<<std::endl;
	int dummy;
	std::cin >> dummy;

	// Delete canvas
	delete cPeak;

}

// MAIN FUNCTION

void CalculateDeltaN() {



// PRESETTINGS

	// Set path to root file
	std::string myPath = "../Data/";

	// Set name of root files with energy spectra
	std::vector<std::string> myFileNames = {"EnergyPMMA.root","histoEnergy05.root","histoEnergy1.root", "histoEnergy2.root","histoEnergy3.root", "histoEnergy4.root", "histoEnergy5.root"};
	std::vector<std::string>myFileName = {"EnergyPMMA.root","EnergyPMMA.root","EnergyPMMA.root", "EnergyPMMA.root","EnergyPMMA.root", "EnergyPMMA.root", "EnergyPMMA.root"};
	// Proton energies used to produce the spectra, there should be one number for each file
	std::vector<double> concentrations = {0.,1.,2.,3.,4.,5.};

	double peakLeft = 1.36;
	double peakRight = 1.3699;

	
	// Create TGraph to hold result
	TGraphErrors* gConcentrations = new TGraphErrors();
	// Set nice drawing attributes and axis titles
	gConcentrations->SetTitle(";%; #Delta N");
	gConcentrations->SetMarkerStyle(20);
	gConcentrations->SetMarkerColor(kBlack);
	gConcentrations->SetLineColor(kBlack);




// CALCULATE Delta N 

	// Iterate over all files
	for(int point = 0; point<myFileNames.size(); point++){
		// Open the file containing the histogram
		TFile *myFile = TFile::Open(std::string(myPath+myFileNames[point]).c_str());
		TFile* myFile1 = TFile::Open(std::string(myPath+myFileName[point]).c_str());
		// Get the histogram from the file by its name
		TH1F* hEnergySpectrum =(TH1F*)myFile->Get("hEnergy");
		TH1F* hEnergySpectrum1 =(TH1F*)myFile1->Get("hEnergy");
		// Define varibales to hold peak integral and its error
		double peakIntegral = 0;
		double e_peakIntegral= 0;

		double peakIntegral1 = 0.;
		double e_peakIntegral1 = 0.;
		// Call own function to calculate peak integral, the results are passed by reference
		ComputePeakIntegral(peakIntegral, e_peakIntegral, hEnergySpectrum, peakLeft, peakRight);
		ComputePeakIntegral(peakIntegral1, e_peakIntegral1, hEnergySpectrum1, peakLeft, peakRight);
		
		double deltaN = peakIntegral-peakIntegral1;
		double e_deltaN = sqrt(pow(e_peakIntegral,2.)+pow(e_peakIntegral1,2.));
		// Insert the point in the graph
		gConcentrations->SetPoint(point, concentrations[point], deltaN);
		gConcentrations->SetPointError(point, 0, e_deltaN);

		TF1 *line = new TF1("line","[0]*x",0,0.06);
		gConcentrations->Fit("line");
		  
	}


// DRAW THE GRAPH

	// Create a canvas to draw into
	TCanvas* canv = new TCanvas();
	// Change into this canvas (now it is the active one and everything drawn will end up in this canvas)
	canv->cd();
	// Draw the graph (option A=draw axis (in contrast to histograms this has to be stated explicitly for graphs), option P=draw points, option L=draw line through points)
	gConcentrations->SetMarkerStyle(20);
	gConcentrations->Draw("AP");
	// Force an update of the graphical view
	canv->Update();

	// Save the image as pdf
	canv->SaveAs("../images/Concentrations.pdf");

}
