//------------------------------------------------------------------------------
/// \file EtaCut.h
//------------------------------------------------------------------------------

#ifndef DarkPhotons_etacut_H
#define DarkPhotons_etacut_H

#include "ParticleCut.h"
#include "Pythia8/Pythia.h"

namespace Pythia8
{

namespace DarkPhotons
{

class EtaCut : public ParticleCut
{

private:
   double _etaMax = 0.; 
   double _etaMin = 0.;

public:

   // constructor
   EtaCut(double etaMax)
      :_etaMax(etaMax) {};

   EtaCut(double etaMin, double etaMax)
      :_etaMax(etaMax), _etaMin(etaMin) {};

   // return true if passed eta requirements
   const bool passed(Particle& particle)
   {
      if ((abs(particle.eta()) <= _etaMax) && abs(particle.eta())  >= _etaMin)
      {
         // std::cout<<"Passed Eta Cut"<< std::endl;
         return true; 
      }
      else
      {
         return false;
      }
   }

   // return false if failed eta requirements
   const bool failed(Particle& particle)
   {
      return !(this->passed(particle));
   }

};

} // namespace DarkPhotons

} // namespace Pythia8

#endif // DarkPhotons_etacut_H
