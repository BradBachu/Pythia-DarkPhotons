//------------------------------------------------------------------------------
// \file MainChannel.h
//
// Author(s):
//    Brad Bachu
//
// Copyright:
//    Copyright (C) 2018  Brad Bachu
//
// Description:
//    Interface of class MainChannel
//    
//    TODO: Description 
//	The Main channel should be a sum of all the decays of
// 	a particular particle to mu+ mu-
//
//------------------------------------------------------------------------------

#ifndef DarkPhotons_mainChannel_H
#define DarkPhotons_mainChannel_H

// ROOT, for histograming
#include "TH1.h"

// DarkPhotons
#include "DiMuonMass.h"

namespace Pythia8
{

namespace DarkPhotons
{

class MainChannel
{

protected:

private:

	// Pythia class
	Pythia _pythia;

	// vector of subchannels to loop over
	std::vector<char> _v_subchannels;

	// Analysis
	DiMuonMass _dimumass;

	// Estimated Cross Section
	double _xsection;

	// Error in cross section
	double _xsectionerr;
	
public:

	// constructor
	MainChannel()
	{
		_pythia.readFile("/home/bbachu/Software/pythia8230/examples/mymain03.cmnd");
		_dimumass.setEtaCut(1.5);
	};

	// constructor
	MainChannel(string channels[], int size, double weight)
	{
		_pythia.readFile("/home/bbachu/Software/pythia8230/examples/mymain03.cmnd");
		_dimumass.setEtaCut(1.5);
		this->addChannels(channels,size,weight);
	};

	// constructor
	MainChannel(string channels[], int size, double weight, string inputfile)
	{
		_pythia.readFile(inputfile);
		_dimumass.setEtaCut(1.5);
		this->addChannels(channels,size,weight);
	};

	// add all decay channels to pythia generation
	void addChannels(string channels[], int size, double weight = 1);

	// run the loop over the channels 
	void generateChannel(int nEvents); //TODO give default value

	// return the Event analysis
	DiMuonMass* getAnalysis();

	// return estimated cross section
	double getXSection();

	// return cross section error
	double getXSectionErr();
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void MainChannel::generateChannel(int nEvents)
{
	// initialize pythia
	_pythia.init();
	// Begin event loop. Generate event; skip if generation aborted.
  	for (int iEvent = 0; iEvent < nEvents; ++iEvent)
  	{
  		// Generate event. Skip if error
    	if (!_pythia.next()) continue; 
    	_dimumass.analyze(_pythia.event);
    	// std::cout<<"Event weight = " << _pythia.info.weight() << std::endl;
    	// Note: output indicates that all events are weighted with 1.00
	} 
	// Statistics on event generation
	_pythia.stat(); 
	_xsection = _pythia.info.sigmaGen();
	_xsectionerr = _pythia.info.sigmaErr();
}

//------------------------------------------------------------------------------
void MainChannel::addChannels(string channels[], int size, double weight)
{
	for (int i = 0; i < size; ++i)
	{
		_pythia.readString(channels[i]);
	}
}

//------------------------------------------------------------------------------
DiMuonMass* MainChannel::getAnalysis()
{
	return &_dimumass;
}

//------------------------------------------------------------------------------
double MainChannel::getXSection()
{
	if (_xsection == 0)
	{
		std::cout<<"ERROR: Event not generated" << std::endl;
	}
	return _xsection;
}

//------------------------------------------------------------------------------
double MainChannel::getXSectionErr()
{
	if (_xsectionerr == 0)
	{
		std::cout<<"ERROR: Event not generated" << std::endl;
	}
	return _xsectionerr;
}

} // namespace DarkPhotons

} // namespace Pythia8

#endif //DarkPhotons_mainChannel_H
