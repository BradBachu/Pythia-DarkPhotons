//------------------------------------------------------------------------------
/// \file DiMuMultiplicity.h
//
// Author(s):
//    Brad Bachu
//
// Copyright:
//    Copyright (C) 2018  Brad Bachu
//
// Description:
//    Interface of class DiMuMultiplicity
//	  1) Loops through evenet
//	  2) Searches for a mu- in detector
//	  3) Finds a mu+ with the same mother
//	  4) Saves that mother
//------------------------------------------------------------------------------

#ifndef DarkPhotons_dimumultiplicity_H
#define DarkPhotons_dimumultiplicity_H

#include "Pythia8/Pythia.h"

// Friend class
#include "MuMultiplicity.h"

// ROOT histograming
#include "TH1.h"

// ROOT, for interactive graphics.
#include "TVirtualPad.h"
#include "TApplication.h"
#include "TCanvas.h"

#include "EventAnalyzer.h"

namespace Pythia8
{

namespace DarkPhotons
{

class DiMuMultiplicity : public MuMultiplicity
{
private:
	
	// int _etaCut = 0;
	
	// bool _muOutDet(Particle& particle); // muon is outside the

	// bool _muHasDaughter(Particle& particle); // muon has daughters
	
	int _nDiMuons = 0; // number of di muon events

	bool _isDimuon = false; // identified good di-muon event

	std::vector<int> _viMothers; // vector containing the mother indexes

public:

	DiMuMultiplicity() {};

	virtual ~DiMuMultiplicity() {};

	virtual const void analyze(Event& event);

	virtual void initialize();

	std::vector<int> getMothers();
	
	void setEtaCut(double etaCut)
	{
		_etaCut = etaCut;
	}
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void DiMuMultiplicity::initialize()
{
	_nDiMuons = 0;
	_viMothers.clear();
	_isDimuon = false;
}


//------------------------------------------------------------------------------
const void DiMuMultiplicity::analyze(Event& event)
{
	this->initialize();
	// particle loop
	for (int i = 0; i < event.size(); ++i)
	{
		// std::cout << "i = " << i <<std::endl;
		// look for a final state muon
		// TODO: also require pythia status?
		if (event[i].id() !=13) continue; // particle is not a mu-

		// std::cout <<"mu- index = " << i <<std::endl;
		// if ( event[i].isFinal() == false ) continue; // not a final state muon
		// if ( event[i].status() != -91 ) continue; // not a final state muon
		// std::cout <<"Event ID = " << event[i].id() <<std::endl;
		if (_muOutDet(event[i]) == true) continue; // check if in detector
		int iMother = event[i].mother1(); // mother index
		// int iMother2 = event[i].mother2#(); // mother index
		// std::cout<<"Mother1() = " << iMother <<std::endl;
		// std::cout<<"Mother2() = " << iMother2 <<std::endl;
		// make sure mother produces 2 muons
		// int iDaughter1 = event[iMother].daughter1(); //daughter1 index
		// int iDaughter2 = event[iMother].daughter2(); // daughter 2 index
		// std::cout <<"daughter 1 id = "  << event[iDaughter1].id() <<endl;
		// std::cout <<"daughter 2 id = "  << event[iDaughter2].id() <<endl;
		// make sure they are opposite sign (stupid way)
		// if ((event[iDaughter1].id()*event[iDaughter2].id())!= -169 ) continue;
		// if (abs(event[iDaughter1].id()) !=13 ) continue;
		// if (abs(event[iDaughter2].id()) !=13 ) continue;
		// std::cout <<"mother gives 2 muons = "  << iMother <<endl;
		// std::cout <<"mother index = "  << iMother <<endl;

		// if (event[iMother].id() == 113) continue;

		// since we do not keep track of daughters, check eta of daughters
		if (_muOutDet(event[iDaughter1])) continue;
		if (_muOutDet(event[iDaughter2])) continue;
		// we have a good di-muon event
		++_nDiMuons;
		// std::cout<<"Mother Id = " << event[iMother].id() << std::endl;
		_viMothers.push_back(iMother);
		_isDimuon = true;
	}
	_h->Fill(_nDiMuons);
}

//------------------------------------------------------------------------------
std::vector<int> DiMuMultiplicity::getMothers()
{
	return _viMothers;
}

// 


} // end namespace DarkPhotons

} // end namespace Pythia8

#endif //DarkPhotons_dimumultiplicity_H