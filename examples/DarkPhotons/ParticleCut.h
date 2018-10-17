//------------------------------------------------------------------------------
// \file Cut.h
//------------------------------------------------------------------------------

#ifndef DarkPhotons_particlecut_H
#define DarkPhotons_particlecut_H

namespace Pythia8
{

namespace DarkPhotons
{

class ParticleCut 
{
   public:

      // return true if the particle passes the cuts
      virtual const bool passed(Particle& particle) = 0;

      // return false if the particle does not pass the cut
      virtual const bool failed(Particle& particle) = 0;
};

} // namespace DarkPhotons

} // namespace Pythia8

#endif // DarkPhotons_particlecut_H