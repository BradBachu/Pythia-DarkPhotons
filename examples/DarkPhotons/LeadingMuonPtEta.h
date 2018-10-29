//------------------------------------------------------------------------------
// \file LeadingLeadingMuonPtEta.h
// \description: 2D plot of leading muon and antimuon pt and eta
//------------------------------------------------------------------------------

#ifndef DarkPhotons_leadingLeadingMuonPtEta_H
#define DarkPhotons_leadingLeadingMuonPtEta_H

#include "Analysis.h"

namespace Pythia8
{

namespace DarkPhotons
{

class LeadingMuonPtEta : public Analysis
{
      
private:

   virtual void initialize();

   double _leadingMuPt = -1.;
   double _leadingMuBarPt = -1.;

   double _leadingMuEta = -1.;
   double _leadingMuBarEta = -1.;


public:

   //constructor
   LeadingMuonPtEta(){};

   virtual ~LeadingMuonPtEta() {};

   virtual void eventAnalysis(Event& event, int iEvent);

   const double getLeadingMuPt()
   {
      return _leadingMuPt;
   }

   const double getLeadingMuBarPt()
   {
      return _leadingMuBarPt;
   }

   const double getLeadingMuEta()
   {
      return _leadingMuEta;
   }

   const double getLeadingMuBarEta()
   {
      return _leadingMuBarEta;
   }

   void Fill(TH2D* h2)
   {
      if (_leadingMuPt !=-1.)
      {
         h2->Fill(_leadingMuPt,_leadingMuEta);
      }
      if (_leadingMuBarPt != -1)
      {
         h2->Fill(_leadingMuBarPt,_leadingMuBarEta);
      }
   }

};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void LeadingMuonPtEta::initialize()
{
   _leadingMuPt = -1.;
   _leadingMuBarPt = -1.;
   _leadingMuEta = -1.;
   _leadingMuBarEta = -1.;
   return;
}

//------------------------------------------------------------------------------
void LeadingMuonPtEta::eventAnalysis(Event& event, int iEvent)
{
   if (event[iEvent].id()==13) // if mu-
   {
      // check if leading
      if (event[iEvent].pT() > _leadingMuPt)
      {
         _leadingMuPt = event[iEvent].pT();
         _leadingMuEta = event[iEvent].eta();
      }
   }
   else if (event[iEvent].id() == -13) // mu+
   {
      if (event[iEvent].pT() > _leadingMuBarPt)
      {
         _leadingMuBarPt = event[iEvent].pT();
         _leadingMuBarEta = event[iEvent].eta();
      }
   }
}

} // end namespace DarkPhotons 

} // end namespace Pythia8


#endif //DarkPhotons_leadingLeadingMuonPtEta_H