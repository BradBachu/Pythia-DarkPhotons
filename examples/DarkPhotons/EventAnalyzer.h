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
	TH1D* _h = 0;

	// keep track if hist was set
	bool _kHistSet = 0;

public:

	//constructor
	EventAnalyzer() {};

	// execute the main analysis
	virtual const void analyze(Event& event) = 0;

	// reset the parameters
	virtual void initialize() = 0;

	// needed to set the histogram parameters
	void setHist(const char *name, const char *title, Int_t nbinsx, Double_t xlow, Double_t xup)
	{
		_h = (TH1D*) new TH1D(name, title, nbinsx, xlow, xup);
		_kHistSet = 1;
	}

	TH1D* getHist();

	void plotHist();
   
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
TH1D* EventAnalyzer::getHist()
{
	_h->SetStats(0);
	return _h;
}

//------------------------------------------------------------------------------
void EventAnalyzer::plotHist()
{
   _h->SetStats(0);
   std::cout << "plotting histogram"<< std::endl;
   TCanvas* c1 = new TCanvas();
   _h->Draw();
   gPad->WaitPrimitive();	
}

} // end namespace DarkPhotons 

} // end namespace Pythia8
#endif //DarkPhotons_eventAnalyzer_H
