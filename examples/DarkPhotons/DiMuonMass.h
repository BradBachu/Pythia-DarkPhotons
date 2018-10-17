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

#include "Analysis.h"
#include "PtCut.h"
#include "EtaCut.h"

namespace Pythia8
{

namespace DarkPhotons
{

class DiMuonMass : public Analysis
{

private:

   double _leadingMuPt = 0.0; // leading mu- pt
    
   double _leadingMuBarPt = 0.0; // leading mu+ pt

   int _leadingMuI = 0; // leading mu- index
   
   int _leadingMuBarI = 0; // leading mu+ index

   int _leadingMuMotherI =0; // leading mu- mother index

   int _leadingMuBarMotherI =0; // leading mu+ mother index

   int _leadingMuMotherPDG = 0; // leading mu- mother PDG

   int _leadingMuBarMotherPDG = 0; // leading mu+ mother PDG

   double _dimumass = 0.0; // di-muon mass

   bool _twoMuons = false;

   bool _updated = false;

   TH1D* _h = 0;

public:

   DiMuonMass(TH1D* h)
      :_h(h) {} ;

   virtual ~DiMuonMass() {};

   virtual void initialize();

   virtual void eventAnalysis(Event& event, int index);

   void updateLeadingMu(Particle& particle, int index);

   int getLeadMuMotherI()
   {
      return _leadingMuMotherI;
   }

   int getLeadMuBarMotherI()
   {
      return _leadingMuBarMotherI;
   }

   int getLeadingMuMotherPDG()
   {
      return _leadingMuMotherPDG;
   }

   int getLeadingMuBarMotherPDG()
   {
      return _leadingMuBarMotherPDG;
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
   _dimumass = -1.;
   _twoMuons = false;
}

//------------------------------------------------------------------------------
void DiMuonMass::updateLeadingMu(Particle& particle, int index)
{  
   _updated = false;

   if (particle.id() == 13) // if mu-
   {
      // std::cout << "mu- identified" << std::endl;
      // std::cout << "current leading mu- pT = "<< _leadingMuPt << std::endl;
      // std::cout << "mu- pT = " << particle.pT() << std::endl;
      if (particle.pT() > _leadingMuPt) // update the leading
      {
         _updated = true;
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
         _updated = true;
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
void DiMuonMass::eventAnalysis(Event& event, int index)
{

   if ((abs(event[index].id()) != 13) ) return; // skip if not muon
   // if (event[index].status()<=0) return; // skip if negative status
   
   // Particle Cuts
   EtaCut etaCut(1.506);
   PtCut pTCut(3.);
   if (etaCut.failed(event[index])) return; // skip if failed eta cut
   if (pTCut.failed(event[index])) return; // skip if failed pt cut

   // if passed eta and pT cut, update leading mu pT   
   this->updateLeadingMu(event[index],index);
   
   if (_updated==false) return; // if no changes made
   if (_leadingMuPt == 0) return; // nothing to compute 
   if (_leadingMuBarPt ==0) return; // nothing to compute 

   _twoMuons = true; // now we have two OS good muons

   // compute the mass
   Vec4 v1 = event[_leadingMuI].p();
   Vec4 v2 = event[_leadingMuBarI].p();
   _dimumass = (v1 + v2).mCalc();
   std::cout<<"DiMuMass = " << _dimumass << std::endl;

   _leadingMuMotherI = event[_leadingMuI].mother1();
   _leadingMuBarMotherI = event[_leadingMuBarI].mother1();

   _leadingMuMotherPDG = event[_leadingMuMotherI].id();
   _leadingMuBarMotherPDG = event[_leadingMuBarMotherI].id();
}

} // end namespace DarkPhotons

} // end namespace Pythia8

#endif //DarkPhotons_dimuonmass_H