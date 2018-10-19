//------------------------------------------------------------------------------
/// \file PtCut.h
//------------------------------------------------------------------------------

#ifndef DarkPhotons_pTcut_H
#define DarkPhotons_pTcut_H

#include "ParticleCut.h"
#include "Pythia8/Pythia.h"

namespace Pythia8
{

namespace DarkPhotons
{

class PtCut : public ParticleCut
{

private:
   double _pTMin = 0.;
   double _pTMax = 0.;

public:

   // constructor
   PtCut(double pTMin)
      :_pTMin(pTMin) {};

   PtCut(double pTMin, double pTMax)
      :_pTMin(pTMin), _pTMax(pTMax) {};

   // return true if passed pT requirements
   const bool passed(Particle& particle)
   {
      if ((particle.pT() > _pTMin)&&(_pTMax == 0.))
      {
         // std::cout<<"Passed pT cut" << std::endl;
         return true; 
      }
      else if ((particle.pT() > _pTMin) && (particle.pT() < _pTMax) && (_pTMax != 0.))
      {
         return true; 
      }
      else
      {
         return false;
      }
   }

   // return false if failed pT requirements
   const bool failed(Particle& particle)
   {
      return !(this->passed(particle));
   }

};

} // namespace DarkPhotons

} // namespace Pythia8

#endif // DarkPhotons_pTcut_H
