//------------------------------------------------------------------------------
/// \file DiMu2LpTOSMass.h
//
// Author(s):
//    Brad Bachu
//
// Copyright:
//    Copyright (C) 2018  Brad Bachu
//
// Description:
//    Interface of class DiMu2LpTOSMass
//------------------------------------------------------------------------------

#ifndef DarkPhotons_DiMu2LpTOSMass_H
#define DarkPhotons_DiMu2LpTOSMass_H

#include "Pythia8/Pythia.h"

#include "Analysis.h"
#include "PtCut.h"
#include "EtaCut.h"

namespace Pythia8
{

namespace DarkPhotons
{

class DiMu2LpTOSMass : public Analysis
{
   // friend class LHCbDiMuMass;

private:

   double _leadingPt = 0.0; // leading mu pt
    
   double _sleadingPt = 0.0; // second leading mu pt

   int _leadingI = 0; // leading mu index
   
   int _sleadingI = 0; // second leading mu index

   int _leadingMotherI =0; // leading mu mother index

   int _sleadingMotherI =0; // second leading mu mother index

   int _leadingMotherPDG = 0; // leading mu mother PDG

   int _sleadingMotherPDG = 0; // second leading mu mother PDG

   double _dimumass = -1.0; // di-muon mass

   bool _twogoodOSMuons = false;
   bool _twogoodSSMuons = false;

   bool _updated = false;

   double _etaMin = 0.; // cut on all particles
   
   double _etaMax = 2.4; // cut on all particles
   
   double _pTMin = 3.; // cut on all particles
   
   double _pTMax = 0.; // cut on all particles


   int _nSignal = 0;

   int _ngoodmu = 0;

public:

   DiMu2LpTOSMass(){} ;
      
   DiMu2LpTOSMass(double etaMin, double etaMax, double pTMin, double pTMax)
      :_etaMin(etaMin),_etaMax(etaMax),_pTMin(pTMin),_pTMax(pTMax) {} ;

   virtual ~DiMu2LpTOSMass() {};

   virtual void initialize();

   virtual void eventAnalysis(Event& event, int index);

   void setPtCuts(double pTMin, double pTMax)
   {
      _pTMin = pTMin; _pTMax = pTMax;
   }

   void setEtaCuts(double etaMin, double etaMax)
   {
      _etaMin = etaMin; _etaMax = etaMax;
   }

   int getLeadMotherI()
   {
      return _leadingMotherI;
   }

   int getSLeadMotherI()
   {
      return _sleadingMotherI;
   }

   int getLeadingMotherPDG()
   {
      return _leadingMotherPDG;
   }

   int getSLeadingMotherPDG()
   {
      return _sleadingMotherPDG;
   }

   int getLeadingI()
   {
      return _leadingI;
   }

   int getSLeadingMuBarI()
   {
      return _sleadingI;
   }

   const bool isDiMuEvent()
   {
      return _twoMuons;
   }

   const double getDiMuMass()
   {
      return _dimumass;
   }

   const void fillMass(TH1D* h)
   {
      if ((_dimumass != -1)&&(_twogoodOSMuons))
      {
         h->Fill(_dimumass);
      }
   }

   const void fillMassSS(TH1D* h)
   {
      if ((_dimumassSS != -1)&&(_twogoodSSMuons))
      {
         h->Fill(_dimumassSS);
      }
   }


   const bool twoSSMuons()
   {
      return _twogoodSSMuons;
   }

   const bool twoOSMuons()
   {
      return _twogoodOSMuons;
   }

};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void DiMu2LpTOSMass::initialize()
{
   _leadingPt = 0.;
   _sleadingPt = 0.;
   _leadingI = 0;
   _sleadingI = 0;
   _leadingMotherPDG= 0;
   _sleadingMotherPDG = 0;
   _dimumass = -1.;
   _twogoodOSMuons = false;
   _twogoodSSMuons = false;
}


//------------------------------------------------------------------------------
void DiMu2LpTOSMass::eventAnalysis(Event& event, int index)
{

   if ((abs(event[index].id()) != 13) ) return; // skip if not muon

   EtaCut etaCut(_etaMin,_etaMax);
   PtCut pTCut(_pTMin,_pTMax);

   if (etaCut.failed(event[index])) return; // skip if failed eta cut
   if (pTCut.failed(event[index])) return; // skip if failed pt cut

   // mu passed eta and pT cut
   if (event[index].pT() > _leadingPt) // if it has more pT than leading mu
   {
      // demote old leading pt to second leading pt
      _sleadingPt = _leadingPt;
      _sleadingI = _leadingI;
      _sleadingMotherI = _leadingMotherI;
      _sleadingMotherPDG = _leadingMotherPDG;
      // new muon takes the leading pt position
      _leadingPt = event[index].pT();
      _leadingI = index;
      _leadingMotherI = event[index].mother1();
      _leadingMotherPDG = event[_leadingMotherI].id();
   }
   else if (event[index].pT() > _sleadingPt) // if it can replace the second leading pT
   {
      _sleadingPt = event[index].pT();
      _sleadingI = index;
      _sleadingMotherI = event[index].mother1();
      _sleadingMotherPDG = event[_sleadingMotherI]();
   }
   else
   {
      return; // nothing more interesting in this event
   }   

   _ngoodmu += 1;

   if (_ngoodmu < 1) return; // not enough muons to do anything
   // now we have at least 2 muons to work with
   // if muons are SS 
   if (event[_leadingI].id() == event[_sleadingI].id())
   {
      _twogoodOSMuons = false;
      _twogoodSSMuons = true;
      Vec4 v1 = event[_leadingI].p();
      Vec4 v2 = event[_sleadingI].p();
      _dimumassSS = (v1 + v2).mCalc();  
   }
   else 
   {
      _twogoodSSMuons = false;
      _twogoodOSMuons = true;
      Vec4 v1 = event[_leadingI].p();
      Vec4 v2 = event[_sleadingI].p();
      _dimumass = (v1 + v2).mCalc();
      
   }
}

} // end namespace DarkPhotons

} // end namespace Pythia8

#endif //DarkPhotons_DiMu2LpTOSMass_H