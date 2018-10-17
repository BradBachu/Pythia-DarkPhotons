//------------------------------------------------------------------------------
/// \file L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4.h
// Reproduce the triggering for scouting data
//------------------------------------------------------------------------------

#ifndef DarkPhotons_L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4_H
#define DarkPhotons_L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4_H

#include "Pythia8/Pythia.h"

// DarkPhoton
#include "PtCut.h"
#include "EtaCut.h"

namespace Pythia8
{

namespace DarkPhotons
{

class L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4: public Trigger
{

private:
   bool _twoOppSign = false; 

   bool _passed = false;

   double _deltaR = 1.4;

   virtual void initialize();

   void _sortMuons(Event& event);

   void _saveMuonIndex(Event& event, int i);

   bool _checkNGoodMuons();

   bool _checkDeltaRs(Event& event);

protected:

   // vector to store index of all mu+ and mu-
   std::vector<int> _v_mu;
   std::vector<int> _v_mubar;

public:

   // constructor
   L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4()
   {
      std::cout<<"Created L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4 trigger" << std::endl;
   }

   // return true if trigger fired
   virtual bool fired(Event& event)
   {
      nevents = nevents+1;
      this->initialize();
      // find mu+ and mu-
      this->_sortMuons(event);
      // if we dont have a good dimuon pair return false
      if (this->_checkNGoodMuons() == false) {return false;}
      // else
      // {
         // std::cout<<"Found at least 2 'good' muons" << std::endl;
         bool passedDeltaR = this->_checkDeltaRs(event);
         // return true;
      // }
      // if we have at least 1 pair, check for deltaR requirement
      if (passedDeltaR == true)
      {
         nfired = nfired+1;
         _passed = true;
         return true;
      }
      else {return false;}
   }

   // Return the vector of muons
   std::vector<int> getMuIndices()
   {
      return _v_mu;
   }
   std::vector<int> getMuBarIndices()
   {
      return _v_mubar;
   }

};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4::initialize()
{
   _passed = false;
   _v_mu.clear();
   _v_mubar.clear();
}

//------------------------------------------------------------------------------
void L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4::_sortMuons(Event& event)
{
   // EtaCut _etaCut(1.506);
   EtaCut _etaCut(2.4);
   PtCut _pTCut(3.);

   // loop and sort good muons into mu+ and mu-
   for (int i = 0; i < event.size(); ++i)
   {
      // if mu- or mu+
      if (( abs(event[i].id()) == 13) /*&& (event[i].status()>0)*/)
      {
         // apply basic cuts on muons
         // std::cout<<"Eta = " <<event[i].eta() << std::endl;
         // std::cout<<"Pt = " <<event[i].pT() << std::endl;
         // look at the range of mother indices for each muon
         // std::cout << "Mu index = " << i << "; Mother1 = " << event[i].mother1() << "; Mother2 = "<< event[i].mother2() << std::endl;
         if (_etaCut.failed(event[i])) continue;
         if (_pTCut.failed(event[i])) continue; 
         // std::cout<<"Good muon"<<std::endl;
         // save them in the right vector
         this->_saveMuonIndex(event,i);
      }
   }  
}

//------------------------------------------------------------------------------
void L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4::_saveMuonIndex(Event& event, int i)
{
   // determine if muon is mu-
   if (event[i].id() ==13)
   {
      _v_mu.push_back(i);
   }
   // else check if muon is mu+
   else if(event[i].id() ==-13)
   {
      _v_mubar.push_back(i);
   }
   else
   {
      std::cout << "Error, muon has not been identified"<< std::endl;
   }
}

//------------------------------------------------------------------------------
bool L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4::_checkDeltaRs(Event& event)
{
   // loop through deltaRs and ensure there is at least 1 canidate
   // with deltaR<1.4
   bool deltaRcut = false;
   // std::cout<< "# mu = " << _v_mu.size() << std::endl;
   // std::cout<< "# mubar = " << _v_mubar.size() << std::endl;
   for (int i = 0; i < _v_mu.size(); ++i)
   {
      for (int j = 0; j < _v_mubar.size(); ++j)
      {  
         // compute delta R for this pair
         double deltaR = RRapPhi(event[_v_mu[i]].p(),event[_v_mubar[j]].p());
         // std::cout << "mu- index = " << i << "; mu+ index = " << j << "; DeltaR = " << deltaR << std::endl;
         // std::cout << "Mu- : pT("<< i <<") = "<< event[_v_mu[i]].pT() << "; eta("<<i<<") = " <<  event[_v_mu[i]].eta() << "; phi = " << event[_v_mu[i]].phi() << std::endl;
         // std::cout << "Mu+ :pT("<< j <<") = "<< event[_v_mubar[j]].pT() << "; eta("<<j<<") = " <<  event[_v_mubar[j]].eta() << "; phi = " << event[_v_mubar[j]].phi() << std::endl;
         // std::cout<<"Mu- status = "<< event[_v_mu[i]].status() << "; Mu+ status = " << event[_v_mubar[j]].status() << std::endl;
         if ( deltaR < _deltaR )
         {
            deltaRcut = true;
            // std::cout<<"Passed Delta R cut" <<std::endl;
            return deltaRcut;
         }
         // if (deltaRcut == true) break;
      }
   }
   return deltaRcut;
}

//------------------------------------------------------------------------------
bool L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4::_checkNGoodMuons()
{
   if ((_v_mu.size() >= 1) && (_v_mubar.size() >=1))
   {
      return true;
   }
   else
   {
      return false;
   }
}

} // namespace DarkPhotons

} // namespace Pythia8

#endif // DarkPhotons_L1_DoubleMu0er1p5_SQ_OS_dR_Max1p4_H