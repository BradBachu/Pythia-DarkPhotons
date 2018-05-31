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

// ROOT for stacking
#include "THStack.h"

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
	TFile *outFile = new TFile("DELETEME.root", "RECREATE");

	//Output file to store this info in 
	// ofstream muon_info; 
	// muon_info.open("muon_info.dat"); 

	bool hasPL = pythia.flag("PartonLevel:all");

	// List of particle decay channels
	string sRho[5] = {"113:oneChannel = 1 0.0000455 0 13 -13",
					  "113:addChannel = 1 0.00000046 11 111 13 -13",
					  "113:addChannel = 1 0.000000000070 11 221 13 -13",
					  "113:addChannel = 1 0.00000067 12 211 -211 13 -13",
					  "113:addChannel = 1 0.0000000024 12 111 111 13 -13"};

	string sRhoPlus[2] = {"213:oneChannel = 1 0.00000046 11 211 13 -13",
						  "213:addChannel = 1 0.00000018 12 211 111 13 -13"};

	string sOmega[5] = {"223:oneChannel = 1 0.000090 0 13 -13",
					    "223:addChannel = 1 0.00013 11 111 13 -13",
					    "223:addChannel = 1 0.0000000018 11 221 13 -13",
					    "223:addChannel = 1 0.000000029 12 211 -211 13 -13",
					    "223:addChannel = 1 0.0000000074 12 111 111 13 -13"};

	string sPhi[3] = {"333:oneChannel = 1 0.000287 0 13 -13",
					  "333:addChannel = 1 0.0000048 11 111 13 -13",
					  "333:addChannel = 1 0.0000068 11 221 13 -13"};

	string sEta[3] = {"221:oneChannel = 1 0.0000058 0 13 -13",
					  "221:addChannel = 1 0.00031 11 22 13 -13",
					  "221:addChannel = 1 0.000000012 12 211 -211 13 -13"};

	string sEtaPrime[2] = {"331:oneChannel = 1 0.000108 11 22 13 -13",
						   "331:addChannel = 1 0.000020 12 211 -211 13 -13"};

	string sRho1770[3] = {"113:oneChannel = 1 0.0000301671 0 13 -13",
						  "113:m0 = 1.72",
						  "113:mWidth = 0.250"};

	string sJPsi[4] = {"443:oneChannel = 1 0.05971 0 13 -13 ",
					   "443:addChannel = 1 0.000000101 11 111 13 -13",
					   "443:addChannel = 1 0.00000300 11 221 13 -13",
					   "443:addChannel = 1 0.0000131 11 331 13 -13"};

	string sPsi2S[1] = {"100443:oneChannel = 1 0.0079 0 13 -13"};

	string sPsi3770[1] = {"30443:oneChannel = 1 0.00000958 0 13 -13"};

	string sUpsilon2S[1] = {"100553:oneChannel = 1 0.0193 0 13 -13"};
	
	string sUpsilon3S[1] = {"200553:oneChannel = 1 0.0218 0 13 -13"};

	MainChannel rhoChannel(sRho,5,1);
	MainChannel rhoPlusChannel(sRhoPlus,2,1);
	// MainChannel omegaChannel(sOmega,5,1);
	// MainChannel omegaChannel;
	// omegaChannel.addChannels(sOmega,5,1);
	MainChannel phiChannel(sPhi,3,1);
	MainChannel etaChannel(sEta,3,1);
	MainChannel etaPrimeChannel(sEtaPrime,2,1);
	// MainChannel rho1770Channel(sRho1770,3,1);
	MainChannel jpsiChannel(sJPsi,4,1);
	MainChannel psi2SChannel(sPsi2S,1,1);
	MainChannel psi3770Channel(sPsi3770,1,1);
	MainChannel upsilon2SChannel(sUpsilon2S,1,1);
	MainChannel upsilon3SChannel(sUpsilon3S,1,1);

	int nbins = 200;
	double xmin = 0;
	double xmax = 8;

	rhoChannel.getAnalysis()->setHist("hRho", "M_{#mu#mu}(#rho#rightarrow#mu#mu)",nbins,xmin,xmax);
	rhoPlusChannel.getAnalysis()->setHist("hRhoPlus", "M_{#mu#mu}(#rho^{+}#rightarrow#mu#mu)",nbins,xmin,xmax);
	// omegaChannel.getAnalysis()->setHist("hOmega", "M_{#mu#mu}(#omega#rightarrow#mu#mu)",nbins,xmin,xmax);
	phiChannel.getAnalysis()->setHist("hPhi", "M_{#mu#mu}(#phi#rightarrow#mu#mu)",nbins,xmin,xmax);
	etaChannel.getAnalysis()->setHist("hEta", "M_{#mu#mu}(#eta#rightarrow#mu#mu)",nbins,xmin,xmax);
	etaPrimeChannel.getAnalysis()->setHist("hEtaPrime", "M_{#mu#mu}(#eta^{#prime}#rightarrow#mu#mu)",nbins,xmin,xmax);
	// rho1770Channel.getAnalysis()->setHist("hRho1770", "M_{#mu#mu}(#rho(1770)#rightarrow#mu#mu)",nbins,xmin,xmax);
	jpsiChannel.getAnalysis()->setHist("hPsi", "M_{#mu#mu}(J/#Psi#rightarrow#mu#mu)",nbins,xmin,xmax);
	psi2SChannel.getAnalysis()->setHist("hPsi2S", "M_{#mu#mu}(#Psi(2S)#rightarrow#mu#mu)",nbins,xmin,xmax);
	psi3770Channel.getAnalysis()->setHist("hPsi3770", "M_{#mu#mu}(#Psi(3770)#rightarrow#mu#mu)",nbins,xmin,xmax);
	upsilon2SChannel.getAnalysis()->setHist("hUpsilon2S", "M_{#mu#mu}(#Upsilon(2S)#rightarrow#mu#mu)",nbins,xmin,xmax);
	upsilon3SChannel.getAnalysis()->setHist("hUpsilon3S", "M_{#mu#mu}(#Upsilon(3S)#rightarrow#mu#mu)",nbins,xmin,xmax);

	int nEvents = 10000;
	rhoChannel.generateChannel(nEvents);
	rhoPlusChannel.generateChannel(nEvents);
	// omegaChannel.generateChannel(nEvents);
	phiChannel.generateChannel(nEvents);
	etaChannel.generateChannel(nEvents);
	etaPrimeChannel.generateChannel(nEvents);
	// rho1770Channel.generateChannel(nEvents);
	jpsiChannel.generateChannel(nEvents);
	psi2SChannel.generateChannel(nEvents);
	psi3770Channel.generateChannel(nEvents);
	upsilon2SChannel.generateChannel(nEvents);
	upsilon3SChannel.generateChannel(nEvents);

	TH1D* hRho = rhoChannel.getAnalysis()->getHist();
	hRho->SetFillColor(kRed);
	TH1D* hRhoPlus = rhoPlusChannel.getAnalysis()->getHist();
	hRhoPlus->SetFillColor(kRed+2);
	// TH1D* hOmega = omegaChannel.getAnalysis()->getHist();
	// hOmega->SetFillColor(kMagenta);
	TH1D* hPhi = phiChannel.getAnalysis()->getHist();
	hPhi->SetFillColor(kViolet);
	TH1D* hEta = etaChannel.getAnalysis()->getHist();
	hEta->SetFillColor(kBlue);
	TH1D* hEtaPrime = etaPrimeChannel.getAnalysis()->getHist();
	hEtaPrime->SetFillColor(kBlue+2);
	// TH1D* hRho1770 = rho1770Channel.getAnalysis()->getHist();
	// hRho1770->SetFillColor(kRed-3);
	TH1D* hJPsi = jpsiChannel.getAnalysis()->getHist();
	hJPsi->SetFillColor(kCyan);
	TH1D* hPsi2S = psi2SChannel.getAnalysis()->getHist();
	hPsi2S->SetFillColor(kTeal);
	TH1D* hPsi3770 = psi3770Channel.getAnalysis()->getHist();
	hPsi3770->SetFillColor(kTeal+3);
	TH1D* hUpsilon2S = upsilon2SChannel.getAnalysis()->getHist();
	hUpsilon2S->SetFillColor(kGreen);
	TH1D* hUpsilon3S = upsilon3SChannel.getAnalysis()->getHist();
	hUpsilon3S->SetFillColor(kGreen+3);
	
	hRho->Write();
	hRhoPlus->Write();
	// hOmega->Write();
	hPhi->Write();
	hEta->Write();
	hEtaPrime->Write();
	// hRho1770->Write();
	hJPsi->Write();
	hPsi2S->Write();
	hPsi3770->Write();
	hUpsilon2S->Write();
	hUpsilon3S->Write();

	THStack* hs = new THStack("hs", "M_{#mu#mu} All Channels");
	hs->Add(hRho);
	hs->Add(hRhoPlus);
	// hs->Add(hOmega);
	hs->Add(hPhi);
	hs->Add(hEta);
	hs->Add(hEtaPrime);
	// hs->Add(hRho1770);
	hs->Add(hJPsi);
	hs->Add(hPsi2S);
	hs->Add(hPsi3770);
	hs->Add(hUpsilon2S);
	hs->Add(hUpsilon3S);

	TLegend *legend = new TLegend(.60, .60, .8,.8);
	legend->AddEntry(hRho,"#rho","f");
	legend->AddEntry(hRhoPlus,"#rho^{+}","f");
	// legend->AddEntry(hOmega,"#Omega","f");
	legend->AddEntry(hPhi,"#Phi","f");
	legend->AddEntry(hEta,"#eta","f");
	legend->AddEntry(hEtaPrime,"#eta^{#prime}","f");
	// legend->AddEntry(hRho1770,"#hRho(1770)","f");
	legend->AddEntry(hJPsi,"J/#Psi","f");
	legend->AddEntry(hPsi2S,"#Psi(2S)","f");
	legend->AddEntry(hPsi3770,"#Psi(3370)","f");
	legend->AddEntry(hUpsilon2S,"#Upsilon(2S)","f");
	legend->AddEntry(hUpsilon3S,"#Upsilon(3S)","f");

	hs->Write("my_stack");
	legend->Write("my_leg");

	delete outFile;

	TCanvas* c2 = new TCanvas(); 
	gPad->WaitPrimitive();

  	return 0;
}
