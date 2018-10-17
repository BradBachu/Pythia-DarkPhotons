//------------------------------------------------------------------------------
// \file MuonPt.h
// \description: 
//------------------------------------------------------------------------------

#ifndef DarkPhotons_muonpt_H
#define DarkPhotons_muonpt_H

#include "Analysis.h"

namespace Pythia8
{

namespace DarkPhotons
{

class MuonPt : public Analysis
{

private:

   TH1D* _h = 0;

   virtual void initialize();

public:

   //constructor
   MuonPt(TH1D* h)
      :_h(h) {};

   virtual ~MuonPt() {};

   virtual void eventAnalysis(Event& event, int i);

};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void MuonPt::initialize()
{return;}


//------------------------------------------------------------------------------
void MuonPt::eventAnalysis(Event& event, int i)
{
   if (event[i].id()==13)
   {
      _h->Fill(event[i].pT());
   }
   else if (event[i].id()==-13)
   {
      _h->Fill(event[i].pT());
   }
}

} // end namespace DarkPhotons 

} // end namespace Pythia8


#endif //DarkPhotons_muonpt_H