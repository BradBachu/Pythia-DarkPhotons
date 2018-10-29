//------------------------------------------------------------------------------
/// \file LHCbDiMuMass.h
//
// Author(s):
//    Brad Bachu
//
// Copyright:
//    Copyright (C) 2018  Brad Bachu
//
// Description:
//    Computes the DiMuMass for the LHCb detector so that we can
//    compute cross-sections for a given epsilon.
//    Note: the selections hree for the mass are the same as that for
//    a trigger so we do not need a trigger here
//------------------------------------------------------------------------------

#ifndef DarkPhotons_lhcbdimumass_H
#define DarkPhotons_lhcbdimumass_H

#include "Pythia8/Pythia.h"


#include "Analysis.h"
#include "PtCut.h"
#include "EtaCut.h"
#include "PCut.h"
#include <cmath>

namespace Pythia8
{

namespace DarkPhotons
{

class LHCbDiMuMass : public Analysis 
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

   double _dimumassSS = -1.0; // di-muon mass SAME SIGN

   bool _twogoodOSMuons = false;
   bool _twogoodSSMuons = true;

   bool _updated = false;

   double _etaMin = 2.; // cut on all particles
   
   double _etaMax = 4.5 ;// cut on all particles
   
   double _pTMuMin = 0.5; // cut on all particles
   
   double _pMuMin = 1.;

   double _pTAMin = 1.; // cut on all particles

   double _pAMin = 0.; // cut on momentum

   int _nSignal = 0;

   int _ngoodmu = 0;

public:

   LHCbDiMuMass() {} ;
      
   LHCbDiMuMass( double etaMin, double etaMax, double pTMin, double pMin)
      :_etaMin(etaMin),_etaMax(etaMax),_pTMuMin(pTMin), _pMuMin(pMin){} ;

   virtual ~LHCbDiMuMass() {};

   virtual void initialize();

   virtual void eventAnalysis(Event& event, int index);

   // void setPtCuts(double pTMin, double pTMax)
   // {
   //    _pTMin = pTMin; _pTMax = pTMax;
   // }

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
      return _twogoodOSMuons;
   }

   const double getDiMuMass()
   {
      return _dimumass;
   }

   const int getNSignal()
   {
      return _nSignal;
   }

   const void fillMass(TH1D* h)
   {
      if ((_dimumass != -1)&&(_twogoodOSMuons))
      {
         h->Fill(_dimumass);
      }
   }

   const void fillBEMMass(TH1D* h)
   {
      if ((_dimumass != -1)&&(_twogoodOSMuons)&&(_leadingMotherPDG == _sleadingMotherPDG))
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

};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void LHCbDiMuMass::initialize()
{
   _leadingPt = 0.;
   _sleadingPt = 0.;
   _leadingI = 0;
   _sleadingI = 0;
   _leadingMotherPDG= 0;
   _sleadingMotherPDG = 0;
   _dimumass = -1.;
   _twogoodOSMuons = false;
   _ngoodmu = 0;
}


//------------------------------------------------------------------------------
void LHCbDiMuMass::eventAnalysis(Event& event, int index)
{

   if ((abs(event[index].id()) != 13) ) return; // skip if not muon
   if ((event[index].eta() < _etaMin)||(event[index].eta() > _etaMax)) return;
   if (event[index].pT() < _pTMuMin) return;

   // PCut pCut(_pMin);
   if (sqrt(pow(event[index].p().px(),2) + pow(event[index].p().py(),2) + pow(event[index].p().pz(),2)) < _pMuMin) return;

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
      _sleadingMotherPDG = event[_sleadingMotherI].id();
   }
   else
   {
      return;
   }

   _ngoodmu += 1;

   if (_ngoodmu <1) return;
   // now we have at least 2 muons to work with
   // if muons are SS 
   if (event[_leadingI].id() == event[_sleadingI].id())
   {
      _dimumass = -1.0;
      Vec4 v1 = event[_leadingI].p();
      Vec4 v2 = event[_sleadingI].p();
      if ((v1+v2).eta()<_etaMin || (v1+v2).eta()>_etaMax) return;
      if ((v1+v2).pT()<_pTAMin) return;
      _dimumassSS = (v1 + v2).mCalc();  
      _twogoodOSMuons = false;
      _twogoodSSMuons = true;
   }
   else 
   {
      _dimumass = -1.0;
      Vec4 v1 = event[_leadingI].p();
      Vec4 v2 = event[_sleadingI].p();
      if ((v1+v2).eta()<_etaMin || (v1+v2).eta()>_etaMax) return;
      if ((v1+v2).pT()<_pTAMin) return;
      _twogoodOSMuons = true;
      _twogoodSSMuons = false;
      _dimumass = (v1 + v2).mCalc();  
   }
}

} // namespace DarkPhotons

} // namespace Pythia8

#endif // DarkPhotons_lhcbdimumass_H