//------------------------------------------------------------------------------
// \file EventAnalyzer.h
//
// Author(s):
//    Brad Bachu
//
// Copyright:
//    Copyright (C) 2018  Brad Bachu
//
// Description:
//    Interface of class EventAnalyzer
//    
//    This should serve as a interface to a system of cuts
//    to produce a single histogram for example
//
//------------------------------------------------------------------------------

#ifndef DarkPhotons_eventAnalyzer_H
#define DarkPhotons_eventAnalyzer_H

// ROOT, for histograming
#include "TH1.h"

// ROOT, for interactive graphics.
#include "TVirtualPad.h"
#include "TApplication.h"
#include "TCanvas.h"

namespace Pythia8
{

namespace DarkPhotons
{

class EventAnalyzer
{
protected:
    // null pointer
	TH1D* _h1D = 0;

	TH2I* _h2I = 0;

	// keep track if hist was set
	bool _kHist1D = 0;

	bool _kHist2I = 0;

public:

	//constructor
	EventAnalyzer() {};

	// execute the main analysis
	virtual const void analyze(Event& event) = 0;

	// reset the parameters
	virtual void initialize() = 0;

	// needed to set the histogram parameters
	void setHist1D(const char *name, const char *title, Int_t nbinsx, Double_t xlow, Double_t xup)
	{
		_h1D = (TH1D*) new TH1D(name, title, nbinsx, xlow, xup);
		_kHist1D = 1;
	}

	void setHist2I(const char *name, const char *title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup)
	{
		_h2I = (TH2I*) new TH2D(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup) ;
		_kHist2I = 1;
	}

	TH1D* getHist1D();

	TH2I* getHist2I();

	void plotHist();
   
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
TH1D* EventAnalyzer::getHist1D()
{
	_h1D->SetStats(0);
	return _h1D;
}

//------------------------------------------------------------------------------
TH2I* EventAnalyzer::getHist2I()
{
	_h2I->SetStats(0);
	return _h2I;
}

//------------------------------------------------------------------------------
void EventAnalyzer::plotHist()
{
   _h1D->SetStats(0);
   std::cout << "plotting histogram"<< std::endl;
   TCanvas* c1 = new TCanvas();
   _h1D->Draw();
   gPad->WaitPrimitive();	
}

} // end namespace DarkPhotons 

} // end namespace Pythia8
#endif //DarkPhotons_eventAnalyzer_H
