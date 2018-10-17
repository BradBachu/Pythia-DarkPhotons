//------------------------------------------------------------------------------
/// \file L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4.h
// Trigger to compare with scouting trigger
// True when leading mu+ and mu- are 'good'
// This was my previous analysis
//------------------------------------------------------------------------------

#ifndef DarkPhotons_LeadingMu_OS_Good_H
#define DarkPhotons_LeadingMu_OS_Good_H

#include "Pythia8/Pythia.h"

// DarkPhoton
#include "PtCut.h"
#include "EtaCut.h"

namespace Pythia8
{

namespace DarkPhotons
{

class LeadingMu_OS_Good: public Trigger
{

private:

   virtual void initialize();

   bool _passed = false;

   int _leadingMuPt = 0;

   int _leadingMuBarPt = 0;

   int _leadingMubarIndex = 0;

   int _leadingMuIndex = 0;

   void _findLeadingMuons(Event& event);

   void _updateLeadingMu(Particle& particle, int index);

protected:
   // store the index of the leading mu+ and mu- here to avoid looking for it again

public:

   // constructor
   LeadingMu_OS_Good()
   {
      std::cout<<"Created LeadingMu_OS_Good trigger" << std::endl;
   }

   // return true if trigger fired
   virtual bool fired(Event &event)
   {
      nevents = nevents+1;
      this->initialize(); 
      // find leading mu- and mu+ // and update leading if found
      this->_findLeadingMuons(event);
      // if more than 2 'good' muons found fire
      if ((_leadingMuIndex!=0)&&(_leadingMubarIndex!=0))
      {
         _passed = true;
         nfired = nfired+1;
         return true;
      }
      else
      {
         return false;
      }
   }  

   const int getLeadingMuIndex();

   const int getLeadingMuBarIndex();

   // const double getDiMuMass();

};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void LeadingMu_OS_Good::initialize()
{
   _passed = false;
   _leadingMuPt = 0;
   _leadingMuBarPt = 0;
   _leadingMuIndex = 0;
   _leadingMubarIndex = 0;
}

//------------------------------------------------------------------------------
void LeadingMu_OS_Good::_findLeadingMuons(Event& event)
{
   
   // EtaCut _etaCut(1.506);
   EtaCut _etaCut(2.4);
   PtCut _pTCut(3.);   

   // loop through and select leading mu+ and mu-
   for (int i = 0; i < event.size(); ++i)
   {
      // if mu- or mu+
      if (( abs(event[i].id()) == 13) /* && (event[i].status()>0)*/)
      {
         // apply basic cuts on muons
         if (_etaCut.failed(event[i])) continue;
         if (_pTCut.failed(event[i])) continue; 
         // if muon passes basic cuts, update accordingly
         this->_updateLeadingMu(event[i],i);
      }
   }  
}

//------------------------------------------------------------------------------
void LeadingMu_OS_Good::_updateLeadingMu(Particle& particle, int index)
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
         _leadingMuIndex = index;
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
         _leadingMubarIndex = index;
      }         
   }
}

//------------------------------------------------------------------------------
const int LeadingMu_OS_Good::getLeadingMuIndex()
{
   return _leadingMuIndex;
}

//------------------------------------------------------------------------------
const int LeadingMu_OS_Good::getLeadingMuBarIndex()
{
   return _leadingMubarIndex;
}

// //------------------------------------------------------------------------------
// const double LeadingMu_OS_Good::getDiMuMass()
// {
//    Vec4 v1 = event[_leadingMuIndex].p();
//    Vec4 v2 = event[_leadingMubarIndex].p();
//    return (v1 + v2).mCalc();
// }

} // namespace DarkPhotons

} // namespace Pythia8

#endif // DarkPhotons_LeadingMu_OS_Good_H