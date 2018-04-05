//------------------------------------------------------------------------------
/// \file DiMuonMass.h
//
// Author(s):
//    Brad Bachu
//
// Copyright:
//    Copyright (C) 2018  Brad Bachu
//
// Description:
//    Interface of class DiMuonMass
//------------------------------------------------------------------------------

#ifndef DarkPhotons_simplemass_H
#define DarkPhotons_simplemass_H

#include "Pythia8/Pythia.h"

// ROOT histograming
#include "TH1.h"

// ROOT, for interactive graphics.
#include "TVirtualPad.h"
#include "TApplication.h"
#include "TCanvas.h"

namespace Pythia8
{

namespace DarkPhotons
{

class SimpleMass
{
private:

	int _iMu1 = 0;
	int _iMu2 = 0;
	TH1F* _hmass = new TH1F("_hmass","Simple Di Muon Mass", 30,0,1.5);

public:

	SimpleMass() {};

	void analyze(Event &event);

	void plotHist();
	
	TH1F* getHist();
	// get the histogram
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void SimpleMass::analyze(Event &event)
{
	_iMu1 = 0;
	_iMu2 = 0;	
	// particle loop
	for (int i = 0; i < event.size(); ++i)
	{
		//look for muon and anti-muons
		if (event[i].id() == 13)
		{
			_iMu1 = i;
		}
		if (event[i].id() == -13)
		{
			_iMu2 = i;
		}
	}

	_hmass->Fill((event[_iMu1].p() + event[_iMu2].p()).mCalc());

}

//------------------------------------------------------------------------------
TH1F* SimpleMass::getHist()
{
   return _hmass;
}

//------------------------------------------------------------------------------
void SimpleMass::plotHist()
{
   std::cout << "plotting histogram"<< std::endl;
   TCanvas* c1 = new TCanvas();
   this->getHist()->Draw();
   gPad->WaitPrimitive();
}



} // end namespace DarkPhotons

} // end namespace Pythia8

#endif // DarkPhotons_simplemass_H