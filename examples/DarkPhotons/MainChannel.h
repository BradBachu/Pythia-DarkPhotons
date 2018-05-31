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

	// Weight of the channel
	// TODO: determine where this comes from
	// maybe pythia generates it or it comes from pdg
	double xsection;
	
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

	// add all decay channels to pythia generation
	void addChannels(string channels[], int size, double weight = 1);

	// run the loop over the channels 
	void generateChannel(int nEvents); //TODO give default value

	// return the Event analysis
	DiMuonMass* getAnalysis();
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
	} 
	// Statistics on event generation
	_pythia.stat(); 
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

} // namespace DarkPhotons

} // namespace Pythia8

#endif //DarkPhotons_mainChannel_H
