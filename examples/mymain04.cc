// Created by Brad Bachu 05/31/18
// Generation of di-muon events at the LHC
// J/Psi production

// Stdlib header file for input and output.
#include <iostream>

// Header file to access Pythia 8 program elements.
#include "Pythia8/Pythia.h"

// ROOT, for histogramming.
#include "TH1.h"
#include "TLegend.h"
#include "TAxis.h"

// ROOT, for saving Pythia events as trees in a file.
#include "TTree.h"
#include "TFile.h"

// ROOT, for interactive graphics.
#include "TVirtualPad.h"
#include "TApplication.h"
#include "TCanvas.h"
#include <ROOT/TDataFrame.hxx>

#include "DarkPhotons/DiMuonMass.h" 
#include "DarkPhotons/MainChannel.h" 

using namespace Pythia8; // Let Pythia8:: be implicit.
using namespace DarkPhotons; // Let DarkPhotons be implicit.

int main(int argc, char* argv[]) 
{ 
	// Create the ROOT application environment. 
	TApplication theApp("hist", &argc, argv);

	// Create file on which histogram(s) can be saved.
	TFile *outFile = new TFile("jpsi.root", "RECREATE");


	string sJPsi[4] = {"443:oneChannel = 1 0.05971 0 13 -13 ",
					   "443:addChannel = 1 0.000000101 11 111 13 -13",
					   "443:addChannel = 1 0.00000300 11 221 13 -13",
					   "443:addChannel = 1 0.0000131 11 331 13 -13"};

	MainChannel jpsiChannel(sJPsi,4,1,"jpsi.cmnd");
	
	int nbins = 200;
	double xmin = 0;
	double xmax = 8;

	jpsiChannel.getAnalysis()->setHist("hPsi", "M_{#mu#mu}(J/#Psi#rightarrow#mu#mu)",nbins,xmin,xmax);
	int nEvents = 10000;
	jpsiChannel.generateChannel(nEvents);
	TH1D* hJPsi = jpsiChannel.getAnalysis()->getHist();
	hJPsi->SetFillColor(kCyan);
	hJPsi->Write();

	delete outFile;

	TCanvas* c2 = new TCanvas(); 
	gPad->WaitPrimitive();

  	return 0;
}
