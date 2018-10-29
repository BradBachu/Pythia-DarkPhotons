//------------------------------------------------------------------------------
// \file Analysis.h
//------------------------------------------------------------------------------

#ifndef DarkPhotons_analysis_H
#define DarkPhotons_analysis_H

#include "Trigger.h"

namespace Pythia8
{

namespace DarkPhotons
{

class Analysis
{
private:
   // reset the parameters
   virtual void initialize() = 0;

public:

   virtual void eventAnalysis(Event& event, int iEvent) = 0;

   // execute the main analysis
   void analyze(Event& event, bool triggerFired)
   {
      this->initialize();
      // particle loop
      // if event fired trigger, then analyze event
      if (triggerFired)
      {
         // std::cout<<"trigger fired! Analyze ->" << std::endl; 
         for (int i = 0; i < event.size(); ++i)
         {
            this->eventAnalysis(event,i);
         }
      }
      else
      {
         // std::cout<<"Event did not fire trigger; Skip" << std::endl; 
         return;
      }
   }
   
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


} // end namespace DarkPhotons 

} // end namespace Pythia8

#endif //DarkPhotons_analysis_H
 