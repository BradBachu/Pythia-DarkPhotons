//------------------------------------------------------------------------------
/// \file MuMultiplicity.h
//
// Author(s):
//    Brad Bachu
//
// Copyright:
//    Copyright (C) 2018  Brad Bachu
//
// Description:
//    Interface of class MuMultiplicity
//------------------------------------------------------------------------------

#ifndef DarkPhotons_mumultiplicity_H
#define DarkPhotons_mumultiplicity_H

#include "Pythia8/Pythia.h"

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

class MuMultiplicity : public EventAnalyzer
{

private:

	int _nMuons = 0;

	
protected:

	int _etaCut = 0;

	// muon is outside the
	const bool _muOutDet(Particle& particle);

	// muon has no daughters
	const bool _muHasDaughter(Particle& particle);

public:

	MuMultiplicity() {};

	virtual ~MuMultiplicity() {};

	virtual const void analyze(Event& event);

	virtual void initialize();

	void setEtaCut(double etaCut)
	{
		_etaCut = etaCut;
	}

};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void MuMultiplicity::initialize()
{
	_nMuons = 0;
}

//------------------------------------------------------------------------------c
const bool MuMultiplicity::_muOutDet(Particle& particle)
{
	if (abs(particle.eta()) >= _etaCut)
	{
		// std::cout<<"outside detector"<<std::endl;
		return true; // mu outside detector
	}
	else
	{
		// std::cout<<"in detector"<<std::endl;
		return false; // mu inside detector
	}
}

//------------------------------------------------------------------------------
const bool MuMultiplicity::_muHasDaughter(Particle& particle)
{
	// std::cout << "Particle.daughter1() = " <<particle.daughter1() << std::endl;
	// std::cout << "Particle.daughter2() = " <<particle.daughter2() << std::endl;
	if ((particle.daughter1()!=0)||(particle.daughter2()!=0))
	{
		// std::cout << "has daughters"<< std::endl;
		return true;
	}
	else
	{
		// std::cout << "no daughters"<< std::endl;
		return false;
	}
}

//------------------------------------------------------------------------------
const void MuMultiplicity::analyze(Event& event)
{
	this->initialize();
	// particle loop
	for (int i = 0; i < event.size(); ++i)
	{
		// look for a final state muon
		// TODO: also require pythia status
		if (event[i].id() !=13) continue; // particle is not a muon
		if (_muOutDet(event[i])) continue; // check if in detector
		if (_muHasDaughter(event[i])) continue; // particle has no daughters
		++_nMuons;	
	}
	_h->Fill(_nMuons);
}

} // end namespace DarkPhotons

} // end namespace Pythia8

#endif //DarkPhotons_mumultiplicity_H