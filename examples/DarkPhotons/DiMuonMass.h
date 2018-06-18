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

#ifndef DarkPhotons_dimuonmass_H
#define DarkPhotons_dimuonmass_H

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

class DiMuonMass : public EventAnalyzer
{
private:

   double _leadingMuPt = 0.0;
   
   double _leadingMuBarPt = 0.0;

   // Index of leading Mu
   int _leadingMuI = 0;
   
   // Index of leading MuBar
   int _leadingMuBarI = 0;

   int _leadingMuMother =0;

   int _leadingMuBarMother =0;

   double _dimumass = 0.0;

   double _etaCut = 1.5;

   double _pTCut =  3.0;

   double _deltaRCut = 1.4;

   bool _setEtaCut = false;

   const bool _muOutDet(Particle& particle);

   int _nMatch = 0;

   const bool _passedPtCut(Particle& particle);

   bool _twoMuons = false;

   const bool _passedDeltaRCut(Vec4 v1, Vec4 v2);


public:

   DiMuonMass() {} ;

   virtual ~DiMuonMass() {};

   virtual const void analyze(Event& event);

   virtual void initialize();

   void updateLeadingMu(Particle& particle, int index);

   const int getNMatch();

   void setEtaCut(double etaCut)
   {
      _etaCut = etaCut;
      _setEtaCut = true;
   }

   void setPtCut(double pTCut)
   {
      _pTCut = pTCut;
   }

   void setDeltaRCut(double deltaRCut)
   {
      _deltaRCut = deltaRCut;
   }

   int getLeadMuMother()
   {
      return _leadingMuMother;
   }

   int getLeadMuBarMother()
   {
      return _leadingMuBarMother;
   }

   int getLeadingMuI()
   {
      return _leadingMuI;
   }

   int getLeadingMuBarI()
   {
      return _leadingMuBarI;
   }

   const bool isDiMuEvent()
   {
      return _twoMuons;
   }

   const double getDiMuMass()
   {
      return _dimumass;
   }

};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void DiMuonMass::initialize()
{
   _leadingMuPt = 0.;
   _leadingMuBarPt = 0.;
   _leadingMuI = 0;
   _leadingMuBarI = 0;
   _dimumass = 0.;
   _twoMuons = false;
}

//------------------------------------------------------------------------------
void DiMuonMass::updateLeadingMu(Particle& particle, int index)
{
   if (particle.id() == 13) // if mu-
   {
      // std::cout << "mu- identified" << std::endl;
      // std::cout << "current leading mu- pT = "<< _leadingMuPt << std::endl;
      // std::cout << "mu- pT = " << particle.pT() << std::endl;
      if (particle.pT() > _leadingMuPt) // update the leading
      {
         // std::cout << "Update" << std::endl;
         _leadingMuPt = particle.pT();
         _leadingMuI = index;
      }
   }
   else if (particle.id() == -13)// mu+
   {
      // std::cout << "mu+ identified" << std::endl;
      // std::cout << "current leading mu+ pT = "<< _leadingMuBarPt << std::endl;
      // std::cout << "mu+ pT = " << particle.pT() << std::endl;
      if (particle.pT() > _leadingMuBarPt) // update the leading
      {
         // std::cout << "Update" << std::endl;
         _leadingMuBarPt = particle.pT();
         _leadingMuBarI = index;
      }         
   }   
   else 
   {
      std::cout <<"ERROR!"<< std::endl;
   }
}

//------------------------------------------------------------------------------
const void DiMuonMass::analyze(Event& event)
{
   if (_kHist1D == 0)
   {
      std::cout<<"ERROR! HISTOGRAM NOT SET"<< std::endl;
   }
   if (_setEtaCut = false)
   {
      std::cout <<"Warning: No Eta Cut Set" << std::endl;
   }
   this->initialize();

   // event loop
   for (int i = 0; i < event.size(); ++i)
   {
      if (abs(event[i].id())!=13) continue; // if particle is not mu skip
      if (_muOutDet(event[i]) == true) continue; // if mu out detector skip
      if (_passedPtCut(event[i]) == false) continue; // if mu.pT() <= pTCut skip
      // if passed eta and pT cut, update leading mu pT
      this->updateLeadingMu(event[i],i);
   }
   // if < 2 good opposite sign muons are found
   // fill with -1 (so that it does not affect distribution)
   if ((_leadingMuPt==0)||(_leadingMuBarPt ==0))
   {
      _dimumass = -1;
   }
   // else combine 4 vectors and get mass
   else
   {
      // then two good muons found
      Vec4 v1 = event[_leadingMuI].p();
      Vec4 v2 = event[_leadingMuBarI].p();
      // apply DeltaR cut
      if(_passedDeltaRCut(v1,v2) == false) 
      {
         _dimumass = -1;
      }
      else
      {
         _twoMuons = true;
         _dimumass = (v1 + v2).mCalc();
         _leadingMuMother = event[event[_leadingMuI].mother1()].id();
         _leadingMuBarMother = event[event[_leadingMuBarI].mother1()].id();
         // std::cout<< "Mu- mother index = "<< event[_leadingMuI].mother1() << std::endl;
         // std::cout<< "Mu+ mother index = "<< event[_leadingMuBarI].mother1() << std::endl;
         // if (event[_leadingMuI].mother1() != event[_leadingMuBarI].mother1()) {++_nMatch; _dimumass =  -1;}
      }
   }
   _h1D->Fill(_dimumass);
}

//------------------------------------------------------------------------------c
const bool DiMuonMass::_muOutDet(Particle& particle)
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

//------------------------------------------------------------------------------c
const bool DiMuonMass::_passedPtCut(Particle& particle)
{
   if (particle.pT() > _pTCut)
   {
      return true; 
   }
   else
   {
      return false;
   }
}

const bool DiMuonMass::_passedDeltaRCut(Vec4 v1, Vec4 v2)
{
   if ( abs(RRapPhi(v1,v2)) < _deltaRCut)
   {
      return true;
   }
   else
   {
      return false;
   }
}

//------------------------------------------------------------------------------c
const int DiMuonMass::getNMatch()
{
   return _nMatch;
}

} // end namespace DarkPhotons

} // end namespace Pythia8

#endif //DarkPhotons_dimuonmass_