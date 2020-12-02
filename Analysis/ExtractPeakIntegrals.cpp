// Run with program with 
//
//		root ExtractPeakIntegrals.cpp



#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TFitResultPtr.h"
#include "TMatrixDSym.h"
#include "TGraph.h"


// OWN FUNCTIONS

// Define a parbolic function p0+p1*x+p2*x*x with a gap
// @param x		: function variable x as x[0]
// @param par	: list of function parameters, parameters 0, 1 and 2 are the normal polynominal coefficients, parameters 3 and 4 specify the gap
Double_t background(Double_t *x, Double_t *par){
	// Define gap with peak to skip
	if (x[0]>par[3] && x[0]<par[4]){
		TF1::RejectPoint();
		return 0;
	}
	// return a second order polynomial 
	return par[0] + par[1]*x[0] + par[2]*x[0]*x[0];
}


// Function to fit the background around a peak, and then calculate the peak integral without the background
// @param peakIntegral		: variable to hold the final result (passed by reference)
// @param e_peakIntegral	: variable to hold the error of the result (passed by reference)
// @param h					: histogram to fit to
// @param peakLeft			: left edge of peak
// @param peakRight			: right edge of peak
// @param backgoundLeft		: left point to start background fit
// @param backgoundRight	: right point to end background fit
void ComputePeakIntegral(double &peakIntegral, double &e_peakIntegral, TH1F* h, double peakLeft, double peakRight, double backgoundLeft, double backgoundRight){

       TCanvas* cPeak = new TCanvas("cPeak","cPeak");
       h->Draw();
       h->GetXaxis()->SetRangeUser(2*backgoundLeft-backgoundRight,2*backgoundRight-backgoundLeft);

	// Define a function to fit the backgound
	TF1* f = new TF1("f",background,backgoundLeft,backgoundRight,5); // xmin, xmax, number of parameters
	f->SetLineColor(kRed);
	// Give some starting parameters for the fit
	f->SetParameters(0,h->GetBinContent(peakLeft)-peakLeft*(h->GetBinContent(peakRight)-h->GetBinContent(peakLeft))/(peakRight-peakLeft));
	f->SetParameters(1,(h->GetBinContent(peakRight)-h->GetBinContent(peakLeft))/(peakRight-peakLeft));
	f->SetParameters(2,0);
	// Fix the position of the gap
	f->FixParameter(3,peakLeft);
	f->FixParameter(4,peakRight);
	// Fit the function in the given range (option R=apply range)
	TFitResultPtr fitResult = h->Fit("f","RS");
	TMatrixDSym covMatrix = fitResult->GetCovarianceMatrix();
	fitResult->Print("V");
	f->Draw("same");
	cPeak->Update();

	// Get the full peak integral
	double fullPeak = h->Integral(h->FindBin(peakLeft),h->FindBin(peakRight));
	cout<<"fullPeak = "<<fullPeak<<endl ;
	// Calculate the backgound integral from the fit and divide by binwidth w to get number of entries in this area
	// backgroundIntegral 	= int (p0  +  p1 * x  +  p2 * x^2) dx from b1 to b2 / w
	//						= (p0 * x  +  1/2 * p1 * x^2  +  1/3 * p2 * x^3) from b1 to b2 / w
	// 						= p0 * (b2-b1)/w  +  1/2 * p1 * (b2^2/b1^2)/w  +  1/3 * p2 * (b2^3/b1^3)/w
	//						= p0 * a  +  p1 * b  +  p2 * c
	double b1 = h->GetXaxis()->GetBinLowEdge(h->FindBin(peakLeft)); // left edge of first bin to have full bins and thus same range as integral over histogram
	double b2 = h->GetXaxis()->GetBinUpEdge(h->FindBin(peakRight)); // right edge of last bin
	double a = (b2-b1)/h->GetBinWidth(1);
	double b = 0.5*(b2*b2-b1*b1)/h->GetBinWidth(1);
	double c = 1./3.*(b2*b2*b2-b1*b1*b1)/h->GetBinWidth(1);
	double backgroundIntegral = f->GetParameter(0)*a+f->GetParameter(1)*b+f->GetParameter(2)*c;
	// Calculate squared error on background integral, as the parameters are correlated, we have to consider their covariance 
	double e2_backgroundIntegral = a*a*covMatrix(0,0) + b*b*covMatrix(1,1) + c*c*covMatrix(2,2) + 2.*a*b*covMatrix(0,1) + 2.*a*c*covMatrix(0,2) + 2.*b*c*covMatrix(1,2);
	cout<<"backgroundIntegral from "<<b1<<" to "<<b2<<" = "<<backgroundIntegral<<" +- "<<sqrt(e2_backgroundIntegral)<<endl;

	// Calculate the peak as difference from the two
	peakIntegral = fullPeak - backgroundIntegral;

	// Get the error through error propagation
	e_peakIntegral = sqrt(fullPeak+e2_backgroundIntegral);	

	// Keep the window open so that the user can check the fit until he/she presses a key
	std::cout<<"Insert any number and press enter to continue"<<std::endl;
	int dummy;
	std::cin >> dummy;

	// Delete canvas
	delete cPeak;

}





// MAIN FUNCTION

void ExtractPeakIntegrals() {



// PRESETTINGS

	// Set path to root file
	std::string myPath = "../Data/";

	// Set name of root files with energy spectra
	std::vector<std::string> myFileNames = {"histoEnergy8.root","histoEnergy10.root","histoEnergy12.root","histoEnergy15.root","histoEnergy18.root","histoEnergy20.root","histoEnergy22.root","histoEnergy25.root","histoEnergy27.root","histoEnergy30.root"};
	// Proton energies used to produce the spectra, there should be one number for each file
	std::vector<double> protonEnergies = {8.,10.,12.,15.,18.,20.,22.,25.,27.,30.};

	// Set range of peak
	// Attention: // if you specifiy a lower edge of a bin for teh variable peakRight, the full bin will be included, if you want to exclude this bin, pick a value slightly below the edge
	double peakLeft = 1.3601;
	double peakRight = 1.3799;
	// Set range for background fit
	double backgoundLeft = 1.1;
	double backgoundRight = 1.8;


	// Create TGraph to hold result
	TGraphErrors* gCrossSection = new TGraphErrors();
	// Set nice drawing attributes and axis titles
	gCrossSection->SetTitle(";Proton Energy in MeV;Cross Section in mb");
	gCrossSection->SetMarkerStyle(20);
	gCrossSection->SetMarkerColor(kBlack);
	gCrossSection->SetLineColor(kBlack);



// CALCULATE CROSS SECTIONS 

	// Iterate over all files
	for(int point = 0; point<myFileNames.size(); point++){
		// Open the file containing the histogram
		TFile *myFile = TFile::Open(std::string(myPath+myFileNames[point]).c_str());
		// Get the histogram from the file by its name
		TH1F* hEnergySpectrum =(TH1F*)myFile->Get("hEnergy");
		// Define varibales to hold peak integral and its error
		double peakIntegral = 0.;
		double e_peakIntegral= 0.;
		// Call own function to calculate peak integral, the results are passed by reference
		ComputePeakIntegral(peakIntegral, e_peakIntegral, hEnergySpectrum, peakLeft, peakRight, backgoundLeft, backgoundRight);
		// !!!! ************* Here you will need to calculate the cross section from the peak integral by something like cs = N / L **************
       
		double density = 1.738;
		double molarMass = 24.305;
		double value = pow(10.,23.);
		double barn = pow(10.,-27.);
		double avogadro = 6.02214086*value;
		double n = (density*avogadro)/molarMass;
		double Luminosität = 5000000*n*0.001;
		double crossSection = peakIntegral/(Luminosität*barn);
		// The error needs to be transformed accordingly
		double e_crossSection = e_peakIntegral/(Luminosität*barn);
		// Insert the point in the graph
		gCrossSection->SetPoint(point, protonEnergies[point], crossSection);
		gCrossSection->SetPointError(point, 0, e_crossSection);
	}

//Talys Data

	TGraph* graph1 = new TGraph("Talys_Mg.txt");
	TGraph* graph2 = new TGraph("Tendl_Mg.txt");
	
	TMultiGraph* mg = new TMultiGraph();
	mg->Add(graph1, "L");
	mg->Add(graph2, "L");
	mg->Add(gCrossSection, "AP");

// DRAW THE GRAPH

	// Create a canvas to draw into
	TCanvas* canv = new TCanvas();
	// Change into this canvas (now it is the active one and everything drawn will end up in this canvas)
	canv->cd();
	// Draw the graph (option A=draw axis (in contrast to histograms this has to be stated explicitly for graphs), option P=draw points, option L=draw line through points)
	graph1->SetLineColor(2);
	graph2->SetLineColor(3);
	mg->Draw("APL");
	// Force an update of the graphical view
	canv->Update();

	TLegend* legend = new TLegend(0.7,0.7,0.9,0.9);
	legend->AddEntry(graph1, "TALYS 1.95", "l");
	legend->AddEntry(graph2, "TENDL-2019", "l");
	legend->AddEntry(gCrossSection, "this work", "p");

	legend->Draw("same");

	// Save the image as pdf
	canv->SaveAs("../images/CrossSectionMg.pdf");

}
