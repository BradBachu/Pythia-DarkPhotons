//------------------------------------------------------------------------------
// \file Trigger.h
//------------------------------------------------------------------------------

#ifndef DarkPhotons_trigger_H
#define DarkPhotons_trigger_H

namespace Pythia8
{

namespace DarkPhotons
{

class Trigger
{
private:
   
   // reset internal variables
   virtual void initialize() = 0;

protected:
   
   // number of events the trigger processed
   int nevents = 0;

   // number of times trigger fired
   int nfired = 0;

public:

   // return true if cut was passed
   virtual bool fired(Event& event) = 0;

   // return true if cut failed
   virtual bool failed(Event& event)
   {
      return(!(this->fired(event)));
   }

   // return trigger efficiency
   const double getEfficiency()
   {
      return double(nfired)/double(nevents);

   }

   const double getNFired()
   {
      return nfired;
   }

   const double getNEvents()
   {
      return nevents;
   }

};

} // namespace DarkPhotons

} // namespace Pythia8

#endif //DarkPhotons_trigger_H