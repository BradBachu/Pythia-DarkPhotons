//------------------------------------------------------------------------------
// \file MuonPtEta.h
// \description: 2D plot of muon pt and eta
//------------------------------------------------------------------------------

#ifndef DarkPhotons_muonpteta_H
#define DarkPhotons_muonpteta_H

#include "Analysis.h"
#include "TH2.h"

namespace Pythia8
{

namespace DarkPhotons
{

class MuonPtEta : public Analysis
{
      
private:

   TH2D* _h2_MuEta = 0;

   virtual void initialize();

public:

   //constructor
   MuonPtEta(TH2D* h2_MuEta)
     :_h2_MuEta(h2_MuEta) {};

   virtual ~MuonPtEta() {};

   virtual void eventAnalysis(Event& event, int iEvent);

   TH2D* getMuPtEta()
   {
      return _h2_MuEta;
   }

};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void MuonPtEta::initialize()
{
   // _h2_MuEta->Reset();
   return;
}

//------------------------------------------------------------------------------
void MuonPtEta::eventAnalysis(Event& event, int iEvent)
{
   if ( abs(event[iEvent].id())==13)
   {
      _h2_MuEta->Fill(event[iEvent].pT(),event[iEvent].eta());
   }
}

} // end namespace DarkPhotons 

} // end namespace Pythia8


#endif //DarkPhotons_muonpteta_H