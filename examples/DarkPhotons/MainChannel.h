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
#include "DiMuonMothers.h"
#include "PdgNames.h"

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

	DiMuonMothers _dimumothers;

	// Estimated Cross Section
	double _xsection;

	// Error in cross section
	double _xsectionerr;

	std::vector<TH1D*> _v_subchannel_hist;

	std::vector<int> _v_subchannel_pdg;
	
public:

	// constructor
	MainChannel()
	{
		_pythia.readFile("/home/bbachu/Software/pythia8230/examples/mymain03.cmnd");
		_dimumass.setEtaCut(1.5);
	};

	// constructor
	MainChannel(int size, string channels[], TH1D* hists[], int pdg[], string inputfile)
	{
		_pythia.readString("Random:setSeed = on");
		_pythia.readFile(inputfile);
		_dimumass.setEtaCut(1.5);
		_dimumass.setPtCut(3.0);
		_dimumass.setDeltaRCut(1.4);
		this->addChannels(size, channels, hists, pdg);
	};

	// add all decay channels to pythia generation
	void addChannels(int size, string channels[], TH1D* hists[], int pdg[]);

	// run the loop over the channels 
	void generateChannel(int nEvents, int seed); //TODO give default 

	// return the mass analysis
	DiMuonMass* getMassAnalysis();
	
	// return the Mother analysis
	DiMuonMothers* getMotherAnalysis();

	// return estimated cross section
	double getXSection();

	// return cross section error
	double getXSectionErr();

	// fill the right histogram with the dimumass
	void FillChannel(int pdgMother, double mass);
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void MainChannel::generateChannel(int nEvents, int seed)
{
	// initialize pythia
	_pythia.readString("Random:seed = " + std::to_string(seed));
	_pythia.init();
	// Begin event loop. Generate event; skip if generation aborted.
  	for (int iEvent = 0; iEvent < nEvents; ++iEvent)
  	{
  		// Generate event. Skip if error
    	if (!_pythia.next()) {std::cout<<"Error in event generation" << std::endl; continue;} 
    	_dimumass.analyze(_pythia.event);
    	// Note: output indicates that all events are weighted with 1.00
    	if (_dimumass.isDiMuEvent())
    	{
    		// std::cout << "Mass = " << _dimumass.getDiMuMass() << std::endl;
    		_dimumothers.initialize();
	    	_dimumothers.setMuMuBar(_dimumass.getLeadingMuI(), _dimumass.getLeadingMuBarI());
	    	_dimumothers.showMuMothers(_pythia.event);
	    	_dimumothers.showMuBarMothers(_pythia.event);
	    	_dimumothers.analyze(_pythia.event);
	    	// check if they are the same PDG mother
	    	if (_dimumothers.isSamePDGMother())
	    	{
	    		// then fill the right histogram
	    		// std::cout<<"--------- Same Mother ---------" << std::endl;
	    		this->FillChannel(_dimumothers.getMuMotherPDG(),_dimumass.getDiMuMass());
	    	}
    	}
	} 
	// Statistics on event generation
	_pythia.stat();
	_xsection = _pythia.info.sigmaGen();
	_xsectionerr = _pythia.info.sigmaErr();
}

//------------------------------------------------------------------------------
void MainChannel::addChannels(int size, string channels[], TH1D* hists[], int pdg[])
{
	for (int i = 0; i < size; ++i)
	{
		_pythia.readString(channels[i]);
		_v_subchannel_hist.push_back(hists[i]);
		_v_subchannel_pdg.push_back(pdg[i]);
	}
}

//------------------------------------------------------------------------------
DiMuonMass* MainChannel::getMassAnalysis()
{
	return &_dimumass;
}

//------------------------------------------------------------------------------
DiMuonMothers* MainChannel::getMotherAnalysis()
{
	return &_dimumothers;
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

//------------------------------------------------------------------------------
void MainChannel::FillChannel(int pdgMother, double mass)
{
	// loop through the sub channel pdg's
	for (int i = 0; i <_v_subchannel_pdg.size(); ++i)
	{
		// std::cout <<"try" << _v_subchannel_pdg[i] << std::endl;
		if (pdgMother == _v_subchannel_pdg[i])
		{
			// std::cout << "PDG Mother =" << pdgMother << " matched to" << _v_subchannel_pdg[i] << std::endl;
			_v_subchannel_hist[i]->Fill(mass);	
			break;
		}
	}
}

} // namespace DarkPhotons

} // namespace Pythia8

#endif //DarkPhotons_mainChannel_H
