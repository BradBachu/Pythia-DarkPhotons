// Created by Brad Bachu 05/31/18
// Generation of di-muon events at the LHC
// J/Psi production

// Stdlib header file for input and output.
#include <iostream>

// Header file to access Pythia 8 program elements.
#include "Pythia8/Pythia.h"

// ROOT, for histogramming.
#include "TH1.h"
#include "TH2.h"
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

// ROOT Colors
#include "TAttFill.h"
#include "TColor.h"

// #include "DarkPhotons/DiMuonMass.h" 
// #include "DarkPhotons/DiMuonMothers.h" 
#include "DarkPhotons/MainChannel.h" 

using namespace Pythia8; // Let Pythia8:: be implicit.
using namespace DarkPhotons; // Let DarkPhotons be implicit.

int main(int argc, char* argv[]) 
{ 

std::map<int,std::string> pdgParticle;
pdgParticle[443] = "J/#Psi";
pdgParticle[100443] = "#Psi(2S)";
pdgParticle[30443] = "#Psi(3770)";

pdgParticle[553] = "#Upsilon";
pdgParticle[100553] = "#Upsilon(2S)";
pdgParticle[200553] = "#Upsilon(3S)";

pdgParticle[111] = "#pi^{0}";
pdgParticle[211] = "#pi^{+}";

pdgParticle[113] = "#rho(770)^{0}";
pdgParticle[213] = "#rho(770)^{+}";

pdgParticle[221] = "#eta";
pdgParticle[331] = "#eta'";

pdgParticle[223] = "#omega(782)";
pdgParticle[333] = "#phi(1020)";

	// Create the ROOT application environment. 
	TApplication theApp("hist", &argc, argv);

	int seed = std::stoi(argv[1],nullptr,10);

	// Create file on which histogram(s) can be saved.
	TString outname = "out" + TString(argv[1]) + ".root";
	TFile *outFile = new TFile(outname, "RECREATE");


	string sJPsi[4] = {"443:oneChannel = 1 0.05971 0 13 -13 ",
					   "443:addChannel = 1 0.000000101 11 111 13 -13",
					   "443:addChannel = 1 0.00000300 11 221 13 -13",
					   "443:addChannel = 1 0.0000131 11 331 13 -13"};

	string sAll[13] = {"443: mayDecay = on", // j/psi
					  "100443: mayDecay = on", // psi(2S)
					  "30443: mayDecay = on", // psi(3770)
					  "553: mayDecay = on", // upsilon
					  "100553: mayDecay = on", // upsilon(2S)
					  "200553: mayDecay = on", // upsilon(3S)
					  "111: mayDecay = on", // pi^0
					  // "211: mayDecay = on", // pi^+
					  "113: mayDecay = on", // rho(770)^0
					  "213: mayDecay = on", // rho(770)^+
					  "221: mayDecay = on", // eta
					  "331: mayDecay = on", // eta^\prime
					  "223: mayDecay = on", // omega(782)
					  "333: mayDecay = on"}; //phi(1020)
	
	int nbins = 200;
	double xmin = 0.;
	double xmax = 8.;


	TH1D* h443    = new TH1D("h443",    "M_{#mu#mu}("+TString(pdgParticle[443])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
	TH1D* h100443 = new TH1D("h100443", "M_{#mu#mu}("+TString(pdgParticle[100443])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
	TH1D* h30443  = new TH1D("h30443",  "M_{#mu#mu}("+TString(pdgParticle[30443])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
	TH1D* h553    = new TH1D("h553",    "M_{#mu#mu}("+TString(pdgParticle[553])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
	TH1D* h100553 = new TH1D("h100553", "M_{#mu#mu}("+TString(pdgParticle[100553])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
	TH1D* h200553 = new TH1D("h200553", "M_{#mu#mu}("+TString(pdgParticle[200553])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
	TH1D* h111    = new TH1D("h111",    "M_{#mu#mu}("+TString(pdgParticle[111])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
	// TH1D* h211    = new TH1D("h211",    "M_{#mu#mu}("+TString(pdgParticle[211])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
	TH1D* h113    = new TH1D("h113",    "M_{#mu#mu}("+TString(pdgParticle[113])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
	TH1D* h213    = new TH1D("h213",    "M_{#mu#mu}("+TString(pdgParticle[213])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
	TH1D* h221    = new TH1D("h221",    "M_{#mu#mu}("+TString(pdgParticle[221])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
	TH1D* h331    = new TH1D("h331",    "M_{#mu#mu}("+TString(pdgParticle[331])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
	TH1D* h223    = new TH1D("h223",    "M_{#mu#mu}("+TString(pdgParticle[223])+"#rightarrow#mu#mu)", nbins,xmin,xmax);
	TH1D* h333    = new TH1D("h333",    "M_{#mu#mu}("+TString(pdgParticle[333])+"#rightarrow#mu#mu)", nbins,xmin,xmax);


	TH1D* hAll[13] = {  h443,
							h100443,
							h30443,
							h553,
							h100553,
							h200553,
							h111,
							// h211,
							h113,
							h213,
							h221,
							h331,
							h223,
							h333
						};

	int pdgIDs[13] ={443,
						100443,
						30443,
						553,
						100553,
						200553,
						111,
						113,
						213,
						221,
						331,
						223,
						333};



	// MainChannel jpsiChannel(sJPsi,4,1,"jpsi.cmnd");
	MainChannel allChannel(13, sAll, hAll, pdgIDs, "jpsi.cmnd"); 

	// jpsiChannel.getAnalysis()->setHist("hPsi", "M_{#mu#mu}(J/#Psi#rightarrow#mu#mu)",nbins,xmin,xmax);
	allChannel.getMassAnalysis()->setHist1D("hAllmass", "M_{#mu#mu}(All#rightarrow#mu#mu)",nbins,xmin,xmax);
	allChannel.getMotherAnalysis()->setHist2I("hAllmothers", "PDG Mothers of #mu^{+} and #mu^{-}",40,200,600,40,200,600);
	int nEvents = 20000;
	// jpsiChannel.generateChannel(nEvents);
	allChannel.generateChannel(nEvents, seed);

	h443->SetFillColor(kRed);				h443->Write();
	h100443->SetFillColor(kRed+1);		h100443->Write();
	h30443->SetFillColor(kRed+2);			h30443->Write();
	h553->SetFillColor(kBlue);				h553->Write();
	h100553->SetFillColor(kBlue+1);		h100553->Write();
	h200553->SetFillColor(kBlue+2);		h200553->Write();
	h111->SetFillColor(kGreen);			h111->Write();
	// h211->SetFillColor(kGreen+2);			h211->Write();
	h113->SetFillColor(kOrange);			h113->Write();
	h213->SetFillColor(kOrange+8);		h213->Write();
	h221->SetFillColor(kViolet);			h221->Write();
	h331->SetFillColor(kViolet+1);		h331->Write();
	h223->SetFillColor(kTeal);				h223->Write();
	h333->SetFillColor(kTeal+2);			h333->Write();

	// TH1D* hJPsi = jpsiChannel.getAnalysis()->getHist();
	TH1D* hAllmass = allChannel.getMassAnalysis()->getHist1D();
	TH2I* hAllmothers = allChannel.getMotherAnalysis()->getHist2I();

	THStack* hs = new THStack("hSamePdgMother", "M_{#mu#mu} All Channels");
	TLegend *legend = new TLegend(.60, .60, .8,.8);
	for (int i = 0; i <13; ++i)
	{
		hs->Add(hAll[i]);
		legend->AddEntry(hAll[i],TString(pdgParticle[pdgIDs[i]]),"f");
	}


	// hJPsi->SetFillColor(kCyan);
	// hJPsi->Write();
	hAllmass->Write();
	hAllmothers->Write("h2AllPdgMother");
	legend->Write("leg");
	hs->Write();
	delete outFile;

	TCanvas* c2 = new TCanvas();
	gPad->WaitPrimitive();

  	return 0;
}
