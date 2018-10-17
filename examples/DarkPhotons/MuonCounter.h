//------------------------------------------------------------------------------
// \file MuonCounter.h
// \description: count the number of muons, anti-muons, and total muons per event
//------------------------------------------------------------------------------

#ifndef DarkPhotons_muoncounter_H
#define DarkPhotons_muoncounter_H

#include "Analysis.h"

namespace Pythia8
{

namespace DarkPhotons
{

class MuonCounter : public Analysis
{
private:
   // no. muons
   int _nmu = 0;

   // no. anti-muons
   int _nmubar = 0;

   // total number of muons and anti-muons.
   int _nmutot = 0;

   virtual void initialize();
   
public:

   // constructor
   MuonCounter() {};

   virtual ~MuonCounter() {};

   virtual void eventAnalysis(Event& event, int iEvent);

   double const getNMu()
   {
      return _nmu;
   }

   double const getNMuBar()
   {
      return _nmubar;
   }

   double const getTotMuons()
   {
      return _nmutot;
   }

};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void MuonCounter::initialize()
{
   _nmu = 0;
   _nmubar = 0;
   _nmutot = 0;
}

//------------------------------------------------------------------------------
void MuonCounter::eventAnalysis(Event& event, int iEvent)
{
   if ( event[iEvent].id() == 13 )
   {
      _nmu = _nmu+1 ;
   }
   if ( event[iEvent].id() == -13)
   {
      _nmubar = _nmubar+1;
   }
   _nmutot = _nmu + _nmubar;
}

} // end namespace DarkPhotons 

} // end namespace Pythia8


#endif //DarkPhotons_muoncounter_H