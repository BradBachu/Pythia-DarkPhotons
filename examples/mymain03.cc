// Created by Brad Bachu 02/20/18
// Generation of di-muon events at the LHC

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
#include "DarkPhotons/SimpleMass.h"

using namespace Pythia8; // Let Pythia8:: be implicit.
using namespace DarkPhotons; // Let DarkPhotons be implicit.

int main(int argc, char* argv[]) 
{ 

	// Create the ROOT application environment. 
	TApplication theApp("hist", &argc, argv);
	
	// Set up generation
	Pythia pythia;
	pythia.readFile("/home/bbachu/Software/pythia8230/examples/mymain03.cmnd");

	pythia.init(); // Initialize; incoming pp beams is default.

	// Set up the ROOT TFile and TTree.
 	// TFile *file = TFile::Open("dimuons.root","recreate");
	Event *event = &pythia.event;  
	TTree *T = new TTree("T","ev1 Tree");
	T->Branch("event",&event); 
  
	// Create file on which histogram(s) can be saved.
	// TFile *outFile = new TFile("dimuonMass.root", "RECREATE");

	//Output file to store this info in 
	// ofstream muon_info; 
	// muon_info.open("muon_info.dat"); 
 
	// Dimuon Mass analysis
	DiMuonMass mass1;
	DiMuonMass mass2;
	DiMuonMass mass3;
	mass1.setHist("hmass1","M_{#mu#mu} #eta < #infty",100,0,5);
	mass1.setEtaCut(100000000);
	mass2.setHist("hmass2","M_{#mu#mu} #eta < 2.4",100,0,5);
	mass2.setEtaCut(2.4);
	mass3.setHist("hmass3","M_{#mu#mu} #eta < 1.5",100,0,5);
	mass3.setEtaCut(1.5);

	TH1D* hLeadingMuMo = new TH1D("hLeadMuMo", "Leading Mu Mother", 1000,0,1000);
	TH1D* hLeadingMuBMo = new TH1D("hLeadMuBMo", "Leading MuB Mother", 1000,0,1000);

	bool hasPL = pythia.flag("PartonLevel:all");

	// List of particle decay channels
	// string rho0Channels[5] = {"113:oneChannel = 1 0.0000455 0 13 -13",
	// 						 "113:addChannel = 1 0.00000046 11 111 13 -13",
	// 						 "113:addChannel = 1 0.000000000070 11 221 13 -13",
	// 						 "113:addChannel = 1 0.00000067 12 211 -211 13 -13",
	// 						 "113:addChannel = 1 0.0000000024 12 111 111 13 -13"};
	// string omegaChannels[5] = {"223:oneChannel = 1 0.000090 0 13 -13",
	// 						  "223:addChannel = 1 0.00013 11 111 13 -13",
	// 						  "223:addChannel = 1 0.0000000018 11 221 13 -13",
	// 						  "223:addChannel = 1 0.000000029 12 211 -211 13 -13",
	// 						  "223:addChannel = 1 0.0000000074 12 111 111 13 -13"};

	
	// Begin event loop. Generate event; skip if generation aborted.
  	for (int iEvent = 0; iEvent < 10000; ++iEvent)
  	{
  		// std::cout<<"Event no. "<< iEvent <<std::endl;
    	if (!pythia.next()) continue; 
	    // Fill the pythia event into the TTree.
	    // Warning: the files will rapidly become large if all events
	    // are saved. In some cases it may be convenient to do some
	    // processing of events and only save those that appear
	    // interesting for future analyses.
	    // For now dont bother to fill events until i understand what is happening
	    // T->Fill(); 
        mass1.analyze(pythia.event); 
        mass2.analyze(pythia.event); 
        mass3.analyze(pythia.event); 

        hLeadingMuMo->Fill(mass3.getLeadMuMother());
        hLeadingMuBMo->Fill(mass3.getLeadMuBarMother());
	} 

	TCanvas* c8 = new TCanvas();
	mass1.getHist()->SetLineColor(kBlack);
	mass2.getHist()->SetLineColor(kBlue);
	mass3.getHist()->SetLineColor(kRed);
	mass1.getHist()->SetTitle("M_{#mu#mu}");
	mass1.getHist()->Draw();
	mass3.getHist()->Draw("same");
	mass2.getHist()->Draw("same"); 
 	TLegend *legend3 = new TLegend(.60, .60, .8,.8);
	legend3->AddEntry(mass1.getHist(), "|#eta| < #infty" ,"l" );
	legend3->AddEntry(mass2.getHist(), "|#eta| < 2.4" , "l");
	legend3->AddEntry(mass3.getHist(), "|#eta| < 1.5" , "l"); 
	legend3->Draw("same");	
 
	// Statistics on event generation
	pythia.stat(); 
 
	//  Write tree.
  	// T->Print();
	// T->Write(); 
	// delete file; 
  
	// Show histogram. Possibility to close it.
	// std::cout << "\nDouble click on the histogram window to quit.\n";
	// gPad->WaitPrimitive();

	// Save histogram on file and close file.
	// h_dimuon->Write();
	// delete outFile;
// 
	std::cout<<"N Match = " << mass3.getNMatch() << std::endl;
	TCanvas* c1 = new TCanvas();
	hLeadingMuMo->Draw();

	TCanvas* c2 = new TCanvas(); 
	gPad->WaitPrimitive();

  	return 0;
}
