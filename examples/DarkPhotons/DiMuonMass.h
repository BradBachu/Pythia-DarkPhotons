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

   double _leadingMuPt = 0;
   
   double _leadingMuBarPt = 0;

   int _leadingMuI = 0;
   
   int _leadingMuBarI = 0;

   int _leadingMuMother =0;

   int _leadingMuBarMother =0;

   double _dimumass = 0;

   double _etaCut = 0;

   bool _setEtaCut = false;

   const bool _muOutDet(Particle& particle);

   int _nMatch = 0;


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
   
   int getLeadMuMother()
   {
      return _leadingMuMother;
   }

   int getLeadMuBarMother()
   {
      return _leadingMuBarMother;
   }

};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void DiMuonMass::initialize()
{
   _leadingMuPt = 0;
   _leadingMuBarPt = 0;
   _leadingMuI = 0;
   _leadingMuBarI = 0;
   _dimumass = 0;
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
   if (_setEtaCut = false)
   {
      std::cout <<"Warning: No Eta Cut Set" << std::endl;
   }
   this->initialize();

   // event loop
   for (int i = 0; i < event.size(); ++i)
   {
      // std::cout << "i = " << i <<std::endl;
      if (abs(event[i].id())!=13) continue; // if particle is not mu skip
      if (_muOutDet(event[i]) == true) continue; // if mu out detector skip
      // std::cout << "Muon in detector" << std::endl;
      this->updateLeadingMu(event[i],i);
   }
   if ((_leadingMuPt==0)||(_leadingMuBarPt ==0))
   {
      _dimumass = -1;
   }
   else
   {
      Vec4 v1 = event[_leadingMuI].p();
      Vec4 v2 = event[_leadingMuBarI].p();
      _dimumass = (v1 + v2).mCalc();
      _leadingMuMother = event[event[_leadingMuI].mother1()].id();
      _leadingMuBarMother = event[event[_leadingMuBarI].mother1()].id();
      // std::cout<< "Mu- mother index = "<< event[_leadingMuI].mother1() << std::endl;
      // std::cout<< "Mu+ mother index = "<< event[_leadingMuBarI].mother1() << std::endl;
      // if (event[_leadingMuI].mother1() != event[_leadingMuBarI].mother1()) {++_nMatch; _dimumass =  -1;}
   }
   _h->Fill(_dimumass);

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
const int DiMuonMass::getNMatch()
{
   return _nMatch;
}



} // end namespace DarkPhotons

} // end namespace Pythia8


#endif //DarkPhotons_dimuonmass_H