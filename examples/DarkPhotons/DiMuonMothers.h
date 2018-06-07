//------------------------------------------------------------------------------
/// \file DiMuonMothers.h
//
// Author(s):
//    Brad Bachu
//
// Copyright:
//    Copyright (C) 2018  Brad Bachu
//
// Description:
//    Interface of class DiMuonMothers
//------------------------------------------------------------------------------

#ifndef DarkPhotons_dimuonmothers_H
#define DarkPhotons_dimuonmothers_H

#include "Pythia8/Pythia.h"

// ROOT histograming
#include "TH1.h"
#include "TH2.h"
// ROOT, for interactive graphics.
#include "TVirtualPad.h"
#include "TApplication.h"
#include "TCanvas.h"

#include "EventAnalyzer.h"
#include "PdgNames.h"

namespace Pythia8
{

namespace DarkPhotons
{

class DiMuonMothers : public EventAnalyzer
{
private:

   // Index of leading Mu and MuBar
   int _leadingMuI = 0;

   int _leadingMuBarI = 0;

	// indexes of 
	int _leadingMuMotherI = 0;

   int _leadingMuBarMotherI = 0;

   int _leadingMuMother = 0; //PDG

   int _leadingMuBarMother = 0; //PDG

	// muons from same mother
	bool _ksamePDGMother = false;   

public:

	DiMuonMothers() {};

	virtual ~DiMuonMothers() {};

   virtual const void analyze(Event& event);
	
	virtual void initialize();

	// print the mother PDGs of the muons
	const void showMuMothers(Event& event);

	const void showMuBarMothers(Event& event);

	void setMuMuBar(int muI, int muBarI)
	{
		_leadingMuI = muI;
		_leadingMuBarI = muBarI;
	}

	const bool isSamePDGMother()
	{
		return _ksamePDGMother;
	}

	const int getMuMotherPDG()
	{
		return _leadingMuMother;
	}

	const int getMuBarMotherPDG()
	{
		return _leadingMuBarMother;
	}
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DiMuonMothers::initialize()
{
	_leadingMuI = 0;
	_leadingMuBarI = 0;
	_leadingMuMotherI =0;
	_leadingMuBarMotherI =0;
	_ksamePDGMother = false;
}

//------------------------------------------------------------------------------
const void DiMuonMothers::analyze(Event& event)
{
	// this->initialize();
	// fill histogram to see mothers
	// note use of abs since D mesons have anti-particles
	_leadingMuMother = event[event[_leadingMuI].mother1()].id() ;
	_leadingMuBarMother = event[event[_leadingMuBarI].mother1()].id() ;
	_h2I->Fill(abs(_leadingMuMother),abs(_leadingMuBarMother)) ;

	if (_leadingMuMother == _leadingMuBarMother)
	{
		_ksamePDGMother = true;
	}
}

//------------------------------------------------------------------------------
const void DiMuonMothers::showMuMothers(Event& event)
{
	std::cout<< "Mu Mothers : " << event[event[_leadingMuI].mother1()].id() << " - " << event[event[_leadingMuI].mother2()].id() << std::endl;
}

//------------------------------------------------------------------------------
const void DiMuonMothers::showMuBarMothers(Event& event)
{
	std::cout<< "MuBar Mothers : " << event[event[_leadingMuBarI].mother1()].id() << " - " << event[event[_leadingMuBarI].mother2()].id() << std::endl;
}

} // end namespace DarkPhotons

} // end namespace Pythia8

#endif //DarkPhotons_dimuonmothers_H