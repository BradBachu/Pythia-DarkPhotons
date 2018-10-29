//------------------------------------------------------------------------------
/// \file PCut.h
// Description: Places a cut on the 4 momentum P (used in LHCb analysis)
//------------------------------------------------------------------------------

#ifdef DarkPhotons_pcut_H
#define DarkPhotons_pcut_H

#include "ParticleCut.h"
#include "Pythia8/Pythia.h"

namespace Pythia8
{

namespace DarkPhotons
{

class PCut : public ParticleCut
{

private:
   double _pmin = 0;

public:

   // constructor
   PCut(double pmin)
      :_pmin(pmin)
   {};

   // return true if passed LHCb requirements
   const bool passed(Vec4 vec)
   {
      if (vec.p() < _pmin) return false;
      return true;
   }

   const bool failed(Vec4 vec) 
   {
      return !(this->passed());
   }

};

}

}

#endif //DarkPhotons_pcut_H